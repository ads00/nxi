#include <nxi/command/initializer.hpp>

#include <nxi/core.hpp>
#include <nxi/command.hpp>
#include <nxi/suggestion/text.hpp>

namespace nxi
{
    command_initializer::command_initializer(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
        , command_{ nullptr }
        , node_{ nullptr }
    {}

    nds::node_ptr<nxi::command> command_initializer::add(nxi::command_data data)
    {
        command_ = nxi_core_.command_system().add(nxi::command(std::move(data)), node_);
        return command_;
    }

    nds::node_ptr<nxi::command> command_initializer::add(const QString& action, command_system::function_type fn, const QString& icon)
    {
        command_ = nxi_core_.command_system().add(nxi::command("nxi", action, std::move(fn), icon), node_);
        return command_;
    }

    nds::node_ptr<nxi::command> command_initializer::add_node(const QString& command_node)
    {
        nds::node_ptr<nxi::command> tmp_node;
        auto fn = [this, &tmp_node](const nxi::command_params&)
        {
            //nxi_core_.command_system().set_root(node);
        };
        node_ = nxi_core_.command_system().add(nxi::command("nxi", command_node, fn, ":/icon/node"), node_);
        tmp_node.reset(node_.get());
        return node_;
    }

    void command_initializer::set_root(nds::node_ptr<nxi::command> node)
    {
        node_ = std::move(node);
        nxi_core_.command_system().set_root(node_);
    }

    void command_initializer::load()
    {
        init_main();
        init_page();

        add_node("interface");

        nxi::command_data load_style;
        load_style.action = "load_style";
        load_style.description = "Load style...";
        load_style.preview = true;
        load_style.function = [this](const nxi::command_params& params)
        {
            auto name = params.get(0);
            nxi_core_.interface_system().load_style(name);
        };
        load_style.parameters = {
        { "name", [](nxi::suggestion_vector& suggestion)
            {
                suggestion.add(nxi::text_suggestion{ "nxi", "", "default style" });
                suggestion.add(nxi::text_suggestion{ "ffx", "", "test" });
                suggestion.add(nxi::text_suggestion{ "nebula_space" });
            }
        }};


        add(std::move(load_style));


        add_node("settings");

        nxi::command_data config;
        config.action = "config";
        config.shortcut = {{ Qt::Key_Control, Qt::Key_Alt }, { Qt::Key_S }};
        config.function = [this](const nxi::command_params&){ nxi_core_.page_system().open_static("nxi/config", nxi::renderer_type::widget); };
        add(std::move(config));
    }
} // nxi