#ifndef NXI_PAGE_WEB_H_NXI
#define NXI_PAGE_WEB_H_NXI

#include <nxi/page.hpp>

namespace nxi
{
    class page_system;

    class web_page : public nxi::page
    {
        Q_OBJECT
    public:
        web_page(nxi::page_system& ps, nxi::page_id id);
        web_page(nxi::page_system& ps, nxi::page_id id, const QString& name) : nxi::page(ps, id, name) {}

        void focus();
        void load();

        const QString& url() const { return url_; }
        void url_update(const QString& url) { url_ = url; }

        static nxi::page_type type() { return page_type::web; }

    private:
        QString url_;

    signals:
        void event_update_icon(const QIcon&);
        //void event_load_begin();
        //void event_load_end();
    };
} // nxi

#endif // NXI_PAGE_WEB_H_NXI
