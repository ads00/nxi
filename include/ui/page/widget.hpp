#ifndef UI_PAGE_WIDGET_H_NXI
#define UI_PAGE_WIDGET_H_NXI

#include <ui/page.hpp>

namespace nxi { class custom_page; }

namespace ui
{
    class core;
    class renderer;

    class widget_page : public ui::page
    {
        Q_OBJECT
    public:
        widget_page(ui::core& ui_core, nxi::custom_page& page);

        void display(ui::renderer* renderer) override;
        renderer* make_renderer() const override;

        QWidget* widget();

    private:
        ui::core& ui_core_;
        nxi::custom_page& page_;
        QWidget* widget_;
    };
} // ui

#endif // UI_PAGE_WIDGET_H_NXI
