#pragma once

#include <source_location>
#include <fstream>
#include <chrono>
#include <format>
#include <iostream>

#include <nlohmann/json.hpp>

#include <babrat/wal/default/wal_default.hpp>
#include <babrat/wal/common/boundary_index_exception.hpp>
#include <babrat/storage/wal_mask.hpp>
#include <babrat/storage/db_options.hpp>
#include <babrat/common/string_empty_error.hpp>
#include <babrat/storage/interfaces/persist_storage.hpp>
#include <babrat/storage/common/convert_timestamp.hpp>
#include <babrat/storage/common/wal_encoder.hpp>
#include <babrat/common/not_implemented_error.hpp>
#include <babrat/storage/in_memory/in_memory_storage.hpp>

namespace fs = std::filesystem;

namespace babrat {

     namespace storage {

         template<typename Key, typename Value, typename Compare = std::less<Key>>
         class DatabaseStorage final : public IPersistStorage<Key, Value>
         {

         private:

             std::uint64_t m_indexWal;
             std::unique_ptr<wal::WAL> m_wal;

         private:

             DatabaseOptions m_options;

         private:

             std::unique_ptr<IStorage<Key, Value>> m_storageContainer;

         private:

             template<typename... Args>
             std::string buildPath(Args &&... args) const {

                 std::ostringstream path;

                 path << m_options.archivePath;

                 const char separator = std::filesystem::path::preferred_separator;

                 (void) std::initializer_list<int>{
                         ((path << separator << std::forward<Args>(args)), 0)...
                 };

                 return path.str();
             }

         public:

             bool saveTo(const std::string &dbFile) override {

                 if (dbFile.empty()) {
                     throw StringEmptyError("dbFile");
                 }

                 std::string full_path = buildPath(dbFile);
                 fs::path dir_path = fs::path(full_path).parent_path();

                 if (!dir_path.empty() && !fs::exists(dir_path)) {
                     fs::create_directories(dir_path);
                 }

                 std::ofstream out(full_path, std::ios::binary);

                 if (!out.is_open()) return false;

                 serialize(out);

                 if (!out.good()) return false;

                 out.close();

                 return true;

             }

             bool loadFrom(const std::string &dbFile) override {

                 if (dbFile.empty()) {
                     throw StringEmptyError("dbFile");
                 }

                 std::ifstream in(buildPath(dbFile), std::ios::binary);

                 if (!in.is_open()) return false;

                 deserialize(in);

                 if (!in.good()) return false;

                 in.close();
                 return true;

             }

         public:

             bool persist(std::uint64_t targetTime) override {
                 return
                    saveTo(std::format("{}{}", targetTime, m_options.formatFile));
             }

             bool restore(std::uint64_t targetTime, bool withWal) override {

                 if (!withWal){
                     return restoreFileForTimestamp(targetTime);
                 }

                 return restoreFileForTimestamp(targetTime)
                    && loadWalForTimestamp(targetTime);

             }

         private:

             bool restoreFileForTimestamp(std::uint64_t targetTime) {

                 std::vector<std::filesystem::path> db_files;

                 for (const auto &entry:
                    std::filesystem::directory_iterator(m_options.archivePath)) {
                     if (entry.path().extension() == m_options.formatFile) {
                         db_files.push_back(entry.path());
                     }
                 }

                 auto min_diff = UINT64_MAX;
                 auto closest_file = db_files.end();

                 for (auto it = db_files.begin(); it != db_files.end(); ++it) {

                     std::string filename = it->stem().string();

                     if (filename.empty() ||
                         !std::all_of(filename.begin(), filename.end(), ::isdigit)) {
                         continue;
                     }

                     unsigned long long diff = targetTime - std::stoull(filename);

                     if (diff < min_diff) {
                         min_diff = diff;
                         closest_file = it;
                     }

                 }

                 if (closest_file == db_files.end()) {
                     return false;
                 }

                 return loadFrom(closest_file->filename());
             }

             bool loadWalForTimestamp(std::uint64_t targetTime) {

                 auto first = m_wal->firstIndex();
                 auto last = m_wal->lastIndex();

                 for(auto i = first; i <= last; i++) {

                     auto m = m_wal->read(i);

                     WALMask mask;
                     mask.decode(m);

                     if(mask.getTimestamp() >= targetTime) {
                         std::cout << "Index " << i << ": " << mask.getTimestamp() << std::endl;
                     }
                 }

                 return true;
             }

         public:

             size_t serialize(std::ostream &stream) const override {


                 if (!stream.good()) {
                     throw std::runtime_error("Stream is not in good state");
                 }

                 size_t total_bytes = 0;

                 total_bytes += babrat::write(stream, m_options.archivePath);

                 total_bytes += babrat::write(stream, *m_storageContainer);

                 m_wal->truncateBack(0);

                 return total_bytes;
             }


             size_t deserialize(std::istream &stream) override {

                 if (!stream.good()) {
                     throw std::runtime_error("Stream is not in good state");
                 }

                 size_t total_bytes = 0;

                 total_bytes += babrat::read(stream, m_indexWal);

                 total_bytes += babrat::read(stream, m_options.archivePath);

                 total_bytes += babrat::read(stream, *m_storageContainer);

                 return total_bytes;
             }


             [[nodiscard]] size_t serializedSize() const noexcept override {
                 size_t total_size = sizeof(size_t) + m_options.archivePath.size();

                 total_size += m_storageContainer->serializedSize();

                 return total_size;
             }

         public:

             DatabaseStorage(
                     DatabaseStorage const &) = delete;

             DatabaseStorage(
                     DatabaseStorage &&) = delete;

         public:

             ~DatabaseStorage() noexcept override = default;

         public:

             explicit DatabaseStorage(DatabaseOptions options)
                     : m_options(std::move(options)) {

                 if (m_options.enableWal) {
                     if (m_options.walPath.empty()) {
                         throw std::invalid_argument("WAL path cannot be empty when WAL is enabled");
                     }

                     m_wal = std::make_unique<wal::WALManager>(
                             m_options.walPath, m_options.walOptions);

                     m_indexWal = m_wal->lastIndex();

                 }

                 if (m_options.mode == DatabaseMode::in_memory){
                    m_storageContainer = std::make_unique<
                            InMemoryStorage<Key, Value, Compare>>(m_options.variant);
                 }else{
                     throw NotImplementedError("InMemoryStorage<Key, Value, Compare>>");
                 }


             }

         public:

             void clearPool(
                     const std::string &pool_name) override {

                 m_storageContainer->clearPool(
                         pool_name);
             }

             void clearSchema(
                     const std::string &pool_name,
                     const std::string &schema_name) override {

                 m_storageContainer->clearSchema(
                         pool_name,
                         schema_name);
             }

             void clearCollection(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name) override {

                 m_storageContainer->clearCollection(
                         pool_name,
                         schema_name,
                         collection_name);

             }

         public:

             void addPool(
                     const std::string &pool_name) override {
                 m_storageContainer->addPool(
                         pool_name);
             }

             void addSchema(
                     const std::string &pool_name,
                     const std::string &schema_name) override {

                 m_storageContainer->addSchema(
                         pool_name,
                         schema_name);

             }

             void addCollection(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name) override {

                 m_storageContainer->addCollection(
                         pool_name,
                         schema_name,
                         collection_name);

             }

         public:

             void disposePool(
                     const std::string &pool_name) override {

                 m_storageContainer->disposePool(
                         pool_name);
             }

             void disposeSchema(
                     const std::string &pool_name,
                     const std::string &schema_name) override {
                 m_storageContainer->disposeSchema(
                         pool_name,
                         schema_name);
             }

             void disposeCollection(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name) override {

                 m_storageContainer->disposeCollection(
                         pool_name,
                         schema_name,
                         collection_name);

             }

         public:

             void reset() override {
                 m_storageContainer->reset();
             }

         public:

             void add(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key,
                     Value const &value) override {

                 writeIfWalMode<Operation::INSERT>(
                         key,
                         value);

                 m_storageContainer->add(
                         pool_name,
                         schema_name,
                         collection_name,
                         key,
                         value);

             }

             void add(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key,
                     Value &&value) override {

                 writeIfWalMode<Operation::INSERT>(
                         key,
                         value);

                 m_storageContainer->add(
                         pool_name,
                         schema_name,
                         collection_name,
                         key,
                         std::move(value));


             }

             void update(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key,
                     Value const &value) override {

                 writeIfWalMode<Operation::UPDATE>(
                         key,
                         value);

                 m_storageContainer->update(
                         pool_name,
                         schema_name,
                         collection_name,
                         key,
                         value);

             }

             void update(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key,
                     Value &&value) override {

                 writeIfWalMode<Operation::UPDATE>(
                         key,
                         value);

                 m_storageContainer->add(
                         pool_name,
                         schema_name,
                         collection_name,
                         key,
                         std::move(value));

             }

             Value &search(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key) override {

                 return m_storageContainer->search(
                         pool_name,
                         schema_name,
                         collection_name,
                         key);

             }

             std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair> searchBetween(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &lower_bound,
                     Key const &upper_bound,
                     bool lower_bound_inclusive,
                     bool upper_bound_inclusive) override {


                 return m_storageContainer->searchBetween(
                         pool_name,
                         schema_name,
                         collection_name,
                         lower_bound,
                         upper_bound,
                         lower_bound_inclusive,
                         upper_bound_inclusive);

             }

             Value dispose(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key) override {

                 writeIfWalMode<Operation::DELETE>(key);

                 return m_storageContainer->dispose(
                         pool_name,
                         schema_name,
                         collection_name,
                         key);

             }

         public:

             void addBatch(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     const std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair> &items) override {

                 for (auto it: items) {
                     writeIfWalMode<Operation::INSERT>(items);
                 }

                 m_storageContainer->addBatch(
                         pool_name,
                         schema_name,
                         collection_name,
                         items);

             }

             std::vector<Value> disposeBatch(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     const std::vector<Key> &items) override {

                 for (auto it: items) {
                     writeIfWalMode<Operation::DELETE>(items);
                 }

                 return m_storageContainer->disposeBatch(
                         pool_name,
                         schema_name,
                         collection_name,
                         items);

             }

         public:

             template<typename Func>
             void range(const std::string &pool_name,
                        const std::string &schema_name,
                        const std::string &collection_name,
                        Func &&processor) {

                 auto &coll = search(pool_name)
                         .search(schema_name)
                         .search(collection_name);

                 for (auto it = coll.begin(); it != coll.end(); ++it) {
                     processor(*it);
                 }

             }


             template<Operation Op, typename ...Args>
             void writeIfWalMode(Args &&... args) {
                 if (m_options.enableWal) {
                     try{

                         m_wal->write(++m_indexWal,
                                      WALMask{
                                              convert::timestamp_to_uint64(
                                                      std::chrono::system_clock::now()),
                                              Op,
                                              WalEncoder::encode(std::forward<Args>(args)...)}
                                              .encode());
                     } catch([[maybe_unused]] const wal::BoundaryIndexWalException& ex){}

                 }

             }

         };

     } // namespace storage

} // namespace babrat

/*using DatabaseStorage = babrat::storage::DatabaseStorage<
           TypeColumn,
           std::vector<TypeColumn>>;*/
