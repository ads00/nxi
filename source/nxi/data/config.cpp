#include <nxi/data/config.hpp>

#include <nxi/core.hpp>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


namespace nxi::data::config::internal
{
    void make(nxi::database& db)
    {
        db.query(internal::str_table.data());
    }

    void prepare(nxi::database& db)
    {
        db.prepare(prepared_query::get_config_value, "SELECT * FROM config WHERE key = ?");
        db.prepare(prepared_query::set_config_value, "INSERT OR REPLACE INTO config(key, value) VALUES(?, ?)");
    }
} // nxi::data::config::internal