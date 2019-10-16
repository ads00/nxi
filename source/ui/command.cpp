#include <ui/command.hpp>

#include <nxi/core.hpp>

#include <ui/core.hpp>
#include <ui/menu.hpp>
#include <ui/command/menu.hpp>

#include <QKeyEvent>
#include <QPaintEvent>
#include <QLabel>
#include <nxi/system/interface.hpp>
#include <nxi/style.hpp>

namespace ui
{
    command::command(ui::core& ui_core)
        : ui_core_{ ui_core }
    {
        setPlaceholderText("Enter command");

        info_ = new QLabel(this);
        header_ = new QLabel(this);

        connect(this, &QLineEdit::returnPressed, [this]()
        {
            user_input().exec();
        });


        connect(this, &QLineEdit::editingFinished, [this]()
        {
            //menu_->hide();
        });

        // text changed by the user
        connect(this, &QLineEdit::textEdited, [this](const QString& text)
        {
            if (!hasFocus()) return;

            user_input().update(text);


            /*
            menu_->clear();

            header_->setText(input_.text());
            info_->setText("results : " + QString::number(input_.suggestion_count()));
            menu_->exec();

            if (input_.is_empty())
            {
                auto focused_page = ui_core_.nxi_core().page_system().focus();
                blockSignals(true);
                if (focused_page.has_value()) setText(focused_page.value()->command());
                selectAll();
                blockSignals(false);
            }*/
        });

        connect(&ui_core_.nxi_core().page_system(), qOverload<nxi::page&>(&nxi::page_system::event_focus), this, [this](nxi::page& page)
        {
            setText(page.name());
        });
    }

    void command::resizeEvent(QResizeEvent* event)
    {
        QLineEdit::resizeEvent(event);
        //menu_->setFixedWidth(width());
    }

    void command::keyPressEvent(QKeyEvent* event)
    {
        QLineEdit::keyPressEvent(event);
        switch (event->key())
        {
            case Qt::Key_Up:
                user_input().select_previous_suggestion();
                break;
            case Qt::Key_Down:
                user_input().select_next_suggestion();
                break;
        }
    }

    void command::focusOutEvent(QFocusEvent* event)
    {
        QLineEdit::focusOutEvent(event);
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page.has_value()) setText(focused_page.value()->name());
    }

    void command::enterEvent(QEvent* event)
    {
        if (hasFocus()) return;
        //if (!input_.is_empty()) return;
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page.has_value()) setText(focused_page.value()->command());
    }

    void command::leaveEvent(QEvent* event)
    {
        if (hasFocus()) return;
        //if (!input_.is_empty()) return;
        auto focused_page = ui_core_.nxi_core().page_system().focus();
        if (focused_page.has_value()) setText(focused_page.value()->name());
    }

    void command::paintEvent(QPaintEvent* event)
    {


        QLineEdit::paintEvent(event);
    }

    nxi::command_input& command::user_input()
    {
        return ui_core_.nxi_core().command_system().user_input();
    }
} // ui