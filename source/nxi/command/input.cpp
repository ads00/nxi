#include <nxi/command/input.hpp>

#include <nxi/command.hpp>
#include <nxi/log.hpp>
#include <nxi/system/command.hpp>

#include <QKeyEvent>

namespace nxi
{

    command_input::command_input(nxi::command_system& command_system)
        : command_system_{ command_system }
        , shortcut_input_{ *this }
        , state_{ states::command }
        , command_{ nullptr }
        , selected_suggestion_index_{ -1 }
    {
        connect(&command_system_, &nxi::command_system::event_param_required, [this](stz::observer_ptr<nxi::command> command)
        {
            reset();
            command_ = command;
            set_state(states::command_param);
        });
    }

    void command_input::add_param(const QString& param)
    {

    }

    void command_input::update(const QString& input, QKeyEvent* event)
    {
        input_ = input;
        auto key = static_cast<Qt::Key>(event->key());

        // ignore autorepeat in shortcut mode
        if (state_ == states::shortcut && event->isAutoRepeat()) return;

        if (event->type() == QEvent::KeyPress)
        {
            if (shortcut_input_.is_trigger_key(static_cast<Qt::Key>(event->key())))
            {
                set_state(states::shortcut);
            }

            if (input_.isEmpty() && state_ == states::command)
            {
                reset();
                return;
            }

            switch(state_)
            {
                case states::command:
                    suggestions_ = command_system_.search(input_);
                    // suggestions_ = history_system_.search(input_);
                    break;

                case states::command_param:
                    //command_->suggest_param(param_index_);
                    param_suggestions_.clear();
                    command_->add_suggestion(param_suggestions_);
                    break;

                case states::shortcut:

                    suggestions_ = shortcut_input_.update(event);
                    emit event_shortcut_input_update(shortcut_input_.to_string());
                    break;
            }

            if (suggestion_count() > 0) select_suggestion(0);
            emit event_suggestion_update(stz::make_observer<nxi::commands_view>(&suggestions_));
        }

        // shortcut mode need release event
        if (event->type() == QEvent::KeyRelease && state_ == states::shortcut)
        {
            suggestions_ = shortcut_input_.update(event);
            emit event_shortcut_input_update(shortcut_input_.to_string());
        }
    }

    void command_input::exec()
    {
        nxi_trace(" exec {} - {}", input_, state_text());

        if (state_ == states::command)
        {
            qDebug() << "exec action" << suggestion(selected_suggestion_index_)->name();

            command_ = suggestion(selected_suggestion_index_);
            // required parameters
            qDebug() << "params" << command_->params().size();
            if (command_->params().size() > 0)
            {
                reset();
                set_state(states::command_param);
            }
            else
            {
                command_system().exec(command_);
                reset();
            }
        }
        else if (state_ == states::command_param)
        {
            params_.add(input_);
            param_index_++;

            // no more parameters
            if (command_ && param_index_ >= command_->params().size())
            {
                // exec command
                if (command_) command_->exec(params_);

                qDebug() << "final exec " << command_->name() << "  " << input_;
                reset();
            }
        }
    }

    command_input::states command_input::state() const { return state_; }

    QString command_input::state_text() const
    {
        if (state_ == states::command) return "command";
        if (state_ == states::shortcut) return "shortcut";
        if (state_ == states::command_param) return "command_param";
        //if (state_ == states::command_param && param_index_ < command_->params().size()) return command_->params()[param_index_];

        return "no command";
    }

    void command_input::set_state(states state)
    {
        state_ = state;
        if (state == states::command_param)
        {
            auto p = command_->params()[param_index_];
            emit event_command_param_required(p);
        }
    }

    nxi::command_system::commands_view command_input::suggestions() const
    {
        return suggestions_;
    }
    const std::vector<QString>& command_input::param_suggestions() const
    {
        return param_suggestions_;
    }

    const QString& command_input::text() const { return input_; }
    size_t command_input::suggestion_count() const { return suggestions_.size(); }
    bool command_input::is_empty() const { return input_.isEmpty(); }

    bool command_input::is_valid() const
    {
        return true;
    }

    const nxi::command_params& command_input::params()
    {
        return params_;
    }

    void command_input::select_suggestion(int index)
    {
        selected_suggestion_index_ = index;
        emit event_selection_update(index);
    }

    void command_input::select_previous_suggestion()
    {
        if (selected_suggestion_index_ > 0) select_suggestion(selected_suggestion_index_ - 1);
    }

    void command_input::select_next_suggestion()
    {
        if (selected_suggestion_index_ + 1 < suggestions_.size()) select_suggestion(selected_suggestion_index_ + 1);
    }

    stz::observer_ptr<nxi::command> command_input::suggestion(int index)
    {
        nxi_assert(index >= 0 && index < suggestions_.size());
        return suggestions_[index];
    }

    stz::observer_ptr<nxi::command> command_input::selected_suggestion()
    {
        return suggestion(selected_suggestion_index_);
    }

    void command_input::suggest_command()
    {
        suggestions_ = command_system_.root_list();
        emit event_suggestion_update(stz::make_observer(&suggestions_));
    }

    void command_input::reset()
    {
        param_index_ = 0;
        selected_suggestion_index_ = -1;
        set_state(states::command);
        params_.clear();
        suggestions_.clear();
        emit event_reset();
    }

    int command_input::selected_suggestion_index()
    {
        return selected_suggestion_index_;
    }

    bool command_input::has_selected_suggestion()
    {
        return selected_suggestion_index_ > -1;
    }

    void command_input::clear()
    {
        input_ = "";
        reset();
    }

    nxi::command_system& command_input::command_system()
    {
        return command_system_;
    }

    nxi::shortcut_input& command_input::shortcut_input()
    {
        return shortcut_input_;
    }
} // nxi