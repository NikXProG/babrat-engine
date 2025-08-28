#pragma once

#include <fstream>
#include <chrono>
#include <format>
#include <iostream>

#include <nlohmann/json.hpp>


using json = nlohmann::ordered_json;


#include <babrat/wal/default/wal_default.hpp>
#include <babrat/wal/common/boundary_index_exception.hpp>
#include <babrat/storage/db_wal_serializer.hpp>
#include <babrat/storage/db_options.hpp>
#include <babrat/common/error/string_empty_error.hpp>
#include <babrat/storage/interfaces/persist_storage.hpp>
#include <babrat/storage/common/convert_timestamp.hpp>
#include <babrat/storage/common/wal_encoder.hpp>
#include <babrat/common/error/not_implemented_error.hpp>
#include <babrat/storage/in_memory/in_memory_storage.hpp>

namespace fs = std::filesystem;

namespace babrat {

     namespace storage {

         template<typename Key, typename Value, typename Compare = std::less<Key>>
         class DatabaseStorage final : public IPersistStorage<Key, Value>
         {

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

             /*BABRAT_API bool saveTo(const std::string &dbFile) override {

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

             BABRAT_API bool loadFrom(const std::string &dbFile) override {

                 if (dbFile.empty()) {
                     throw StringEmptyError("dbFile");
                 }

                 std::ifstream in(buildPath(dbFile), std::ios::binary);

                 if (!in.is_open()) return false;

                 deserialize(in);

                 if (!in.good()) return false;

                 in.close();
                 return true;

             }*/

         public:

            /*BABRAT_API bool persist(std::uint64_t targetTime) override {
                 return
                    saveTo(std::format("{}{}", targetTime, m_options.formatFile));
            }*/

             /*BABRAT_API bool restore(std::uint64_t targetTime, bool withWal) override {

                 if (!m_options.enableWal || !withWal){
                     return restoreFileForTimestamp(targetTime);
                 }

                 return restoreFileForTimestamp(targetTime)
                    && loadWalForTimestamp(targetTime);

            }*/

         private:

             /*
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
             */

             /*
             bool loadWalForTimestamp(std::uint64_t targetTime) {

                 auto first = m_wal->firstIndex();
                 auto last = m_wal->lastIndex();

                 for(auto i = first; i <= last; i++) {

                     auto m = m_wal->read(i);

                     m_maskReader->decode(m);

                     if(m_maskReader->getTimestamp() >= targetTime) {
                         std::cout << "Index " << i << ": " << m_maskReader->getTimestamp() << std::endl;
                     }

                 }

                 return true;
             }
             */

         public:

             BABRAT_API size_t serialize(std::ostream &stream) const override {


                 if (!stream.good()) {
                     throw std::runtime_error("Stream is not in good state");
                 }

                 size_t total_bytes = 0;

                 total_bytes += babrat::write(stream, m_options.archivePath);

                 total_bytes += babrat::write(stream, *m_storageContainer);

                 /*m_wal->truncateBack(0);*/

                 return total_bytes;
             }

             BABRAT_API size_t deserialize(std::istream &stream) override {

                 if (!stream.good()) {
                     throw std::runtime_error("Stream is not in good state");
                 }

                 size_t total_bytes = 0;

                 /*total_bytes += babrat::read(stream, m_indexWal);*/

                 total_bytes += babrat::read(stream, m_options.archivePath);

                 total_bytes += babrat::read(stream, *m_storageContainer);

                 return total_bytes;
             }

             BABRAT_API size_t serializedSize() const noexcept override {
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

             BABRAT_API explicit DatabaseStorage(DatabaseOptions options)
                     : m_options(std::move(options)) {

                 /*
                 if (m_options.enableWal) {

                     if (m_options.walPath.empty()) {
                         throw std::invalid_argument("WAL path cannot be empty when WAL is enabled");
                     }

                     m_wal = std::make_unique<wal::WALManager>(
                             m_options.walPath, m_options.walOptions);

                     m_indexWal = m_wal->lastIndex();

                     m_maskReader = std::make_unique<WALMask>();

                 }
                 */

                 if (m_options.mode == DatabaseMode::IN_MEMORY){
                    m_storageContainer = std::make_unique<
                            InMemoryStorage<Key, Value, Compare>>(m_options.variant);
                 }else{
                     throw NotImplementedError("InMemoryStorage<Key, Value, Compare>>");
                 }

             }

         public:

             BABRAT_API void clearPool(
                     const std::string &pool_name) override {

                 m_storageContainer->clearPool(
                         pool_name);
             }

             BABRAT_API void clearSchema(
                     const std::string &pool_name,
                     const std::string &schema_name) override {

                 m_storageContainer->clearSchema(
                         pool_name,
                         schema_name);
             }

             BABRAT_API void clearCollection(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name) override {

                 m_storageContainer->clearCollection(
                         pool_name,
                         schema_name,
                         collection_name);

             }

         public:

             BABRAT_API void addPool(
                     const std::string &pool_name) override {
                 m_storageContainer->addPool(
                         pool_name);
             }

             BABRAT_API void addSchema(
                     const std::string &pool_name,
                     const std::string &schema_name) override {

                 m_storageContainer->addSchema(
                         pool_name,
                         schema_name);

             }

             BABRAT_API void addCollection(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name) override {

                 m_storageContainer->addCollection(
                         pool_name,
                         schema_name,
                         collection_name);

             }

         public:

             BABRAT_API void disposePool(
                     const std::string &pool_name) override {

                 m_storageContainer->disposePool(
                         pool_name);
             }

             BABRAT_API void disposeSchema(
                     const std::string &pool_name,
                     const std::string &schema_name) override {
                 m_storageContainer->disposeSchema(
                         pool_name,
                         schema_name);
             }

             BABRAT_API void disposeCollection(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name) override {

                 m_storageContainer->disposeCollection(
                         pool_name,
                         schema_name,
                         collection_name);

             }

         public:

             BABRAT_API void reset() override {
                 m_storageContainer->reset();
             }

         public:

             BABRAT_API void add(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key,
                     Value const &value) override {

                 m_storageContainer->add(
                         pool_name,
                         schema_name,
                         collection_name,
                         key,
                         value);

             }

             BABRAT_API void add(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key,
                     Value &&value) override {

                 m_storageContainer->add(
                         pool_name,
                         schema_name,
                         collection_name,
                         key,
                         std::move(value));


             }

             BABRAT_API void update(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key,
                     Value const &value) override {

                 m_storageContainer->update(
                         pool_name,
                         schema_name,
                         collection_name,
                         key,
                         value);

             }

             BABRAT_API void update(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key,
                     Value &&value) override {

                 m_storageContainer->add(
                         pool_name,
                         schema_name,
                         collection_name,
                         key,
                         std::move(value));

             }

             BABRAT_API Value &search(
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

             BABRAT_API std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair> searchBetween(
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

             BABRAT_API Value dispose(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     Key const &key) override {

                 return m_storageContainer->dispose(
                         pool_name,
                         schema_name,
                         collection_name,
                         key);

             }

         public:

             BABRAT_API void addBatch(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     const std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair> &items) override {

                 m_storageContainer->addBatch(
                         pool_name,
                         schema_name,
                         collection_name,
                         items);

             }

             BABRAT_API std::vector<Value> disposeBatch(
                     const std::string &pool_name,
                     const std::string &schema_name,
                     const std::string &collection_name,
                     const std::vector<Key> &items) override {

                 return m_storageContainer->disposeBatch(
                         pool_name,
                         schema_name,
                         collection_name,
                         items);

             }

         public:

             /*
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

             }*/

         private:

             /*
             template<typename ...Args>
             void writeLog(
                 WalLevelType level,
                 WalOperationType operation,
                 Args &&... args [[maybe_unused]]) {

                 if (!m_options.enableWal) {
                     return;
                 }

                 try {

                     // serializing data
                     std::ostringstream dataStream;
                     (babrat::write(dataStream, std::forward<Args>(args)), ...);

                     // encoding data

                     // formatting wal data
                     JsonWalFormatter formatter;

                     std::vector<uint8_t> walData;

                     formatter.encode(
                         {
                             convert::timestamp_now(), // current_timestamp
                             level, // current wal level ( POOL, SCHEMA, COLLECTION, OBJECT... )
                             operation  // type operation wal ( INSERT, UPDATE, DELETE )
                         },
                         dataStream.str(), // serialize data
                         walData // wal out buffer
                    );

                     m_wal->write(++m_indexWal, walData);


                 } catch(const wal::BoundaryIndexWalException&) {
                     // default pass
                 } catch(const WalException&) {
                     //TODO: executing exception throw
                 } catch(const std::exception&) {
                     //TODO: executing other exception throw
                 }

             }
             */

         };

     } // namespace storage

} // namespace babrat

/*using DatabaseStorage = babrat::storage::DatabaseStorage<
           TypeColumn,
           std::vector<TypeColumn>>;*/
