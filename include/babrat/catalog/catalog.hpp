#pragma once

namespace babrat {

    namespace catalog {

        class ICatalog  {

        public:

            virtual ~ICatalog() noexcept = default;

            /*
            public:

                virtual void init() = 0;

            public:

                virtual Oid getOidByColumn(Column* column,
                                           const std::string& catalog_name) = 0;
                                           */

            /*  virtual const SystemTable& findTableByOid(Oid tableOid) const = 0;

               virtual const SystemTable& findTableByColumnAndOid(const Column& searchColumn, Oid tableOid) const = 0;

               virtual std::vector<std::unique_ptr<Column>> findAllColumnsByOid(Oid oid) const = 0;*/

            /*   virtual const void createSchema(CreateSchema& schema) = 0;*/

            /*  virtual const void createTable(CreateTable& table) = 0;*/

        };

    } // namespace catalog

} // namespace babrat

