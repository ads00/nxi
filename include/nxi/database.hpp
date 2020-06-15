#ifndef DATABASE_H_NXI
#define DATABASE_H_NXI

#include <ndb/engine/sqlite/sqlite.hpp>

#include <ndb/preprocessor.hpp>

#include <ndb/expression.hpp>
#include <ndb/function.hpp>
#include <ndb/query.hpp>

// include type_mapped types
#include <QString>
#include <nxi/type.hpp>

// ndb type_maps
namespace ndb
{
    ndb_bijective_type_map(string_, QString, scope::global);

    ndb_type_map(nxi::page_id, int64_, scope::global);
    ndb_type_map(nxi::page_type, int64_, scope::global);
    ndb_type_map(nxi::renderer_type, int64_, scope::global);
    ndb_type_map(nxi::module_type, int64_, scope::global);
}

#include <nxi/database/command.hpp>
#include <nxi/database/context.hpp>
#include <nxi/database/module.hpp>
#include <nxi/database/page.hpp>
#include <nxi/database/window.hpp>

ndb_model(
    nxi_model
    , context
    , context_available
    , module
    , page
    , page_connection
    , page_system
    , window
)

ndb_project(nxi_project,
            ndb_database(db_core, nxi_model, ndb::sqlite)
)

namespace dbs
{
    using core = ndb::databases::nxi_project::db_core_;
} // nxi

using ndb::models::nxi_model;

#endif // DATABASE_H_NXI