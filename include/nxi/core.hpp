#ifndef CORE_H_NXI
#define CORE_H_NXI

#include <nxi/module/api/core.hpp>
#include <nxi/system/command.hpp>
#include <nxi/system/context.hpp>
#include <nxi/system/page.hpp>
#include <nxi/system/module.hpp>
#include <nxi/system/interface.hpp>
#include <nxi/system/session.hpp>
#include <nxi/system/window.hpp>

#include <nxi/config.hpp>

namespace nxi
{
    class core : public QObject
    {
        Q_OBJECT
    public:
        core();
        ~core();
        core(const core&) = delete;
        void operator=(const core&) = delete;

        void load();
        void quit() const;

        void error(const QString& message) const;

        nxi::api::core& api();
        nxi::config& config();
        nxi::database& database();
        nxi::database& global_database();

        nxi::command_system& command_system();
        nxi::context_system& context_system();
        nxi::interface_system& interface_system();
        nxi::module_system& module_system();
        nxi::page_system& page_system();
        nxi::session_system& session_system();
        nxi::window_system& window_system();

        static QString module_path();
        static QString module_path(const QString& name, nxi::module_type);
        static QString page_path(const QString&);

    signals:
        void event_quit() const;
        void event_error(const QString&) const;

    private:
        nxi::global_database global_database_;
        nxi::api::core api_;
        nxi::config config_;

        nxi::module_system module_system_;
        nxi::page_system page_system_;
        nxi::session_system session_system_;
    };
} // nxi

#endif // CORE_H_NXI