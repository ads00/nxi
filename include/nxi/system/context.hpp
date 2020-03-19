#ifndef INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI
#define INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI

#include <nxi/context.hpp>

#include <vector>

#include <QObject>

namespace nxi
{
    class core;

    class context_system : public QObject
    {
        Q_OBJECT
    public:
		context_system(nxi::core& );
		context_system(const context_system&) = delete;
        void operator=(const context_system&) = delete;

        void load();

        // enter
        // leave

        template<class Context, class... Args>
        void add(Args&&... args)
        {
            contexts_.emplace_back( std::make_unique<nxi::context>( Context{ std::forward<Args>(args)... } ) );
            std::sort(contexts_.begin(), contexts_.end(), [](const auto& c1, const auto& c2){ return c1->priority() > c2->priority(); });
            emit event_context_update(*contexts_.front());
        }

        template<class Context>
        void del()
        {
            contexts_.erase(std::find_if(contexts_.begin(), contexts_.end(), [](const auto& c) { return nxi::context::is<Context>(*c); }));
            std::sort(contexts_.begin(), contexts_.end(), [](const auto& c1, const auto& c2){ return c1->priority() > c2->priority(); });
            emit event_context_update(*contexts_.front());
        }


        template<class... Fs>
        void apply_on_active(Fs&&... fs)
        {
            int active_priority = contexts_.front()->priority();
            for (const auto& context : contexts_)
            {
                if (context->priority() == active_priority) context->apply( std::forward<Fs>(fs)... );
                else break;
            }
        }

        template<class Context>
        bool is_active() const
        {
            int active_priority = contexts_.front()->priority();
            auto f = [active_priority](const std::unique_ptr<nxi::context>& context)
            {
                return context->apply([active_priority](const auto& ctx){ return std::is_same_v<std::decay_t<decltype(ctx)>, Context> && ctx.priority() == active_priority; });
            };
            if (std::find_if(contexts_.begin(), contexts_.end(), f) != contexts_.end()) return true;

            return false;
        }

/*
        void add(const nxi::context&);
        void add(nxi::context, int priority);
        void del(const nxi::context&);
        void focus(const nxi::context&);
        bool is_active(const nxi::context&) const;*/

    signals:
        void event_context_update(const nxi::context&) const;

    private:
		nxi::core& nxi_core_;

		std::vector<std::unique_ptr<nxi::context>> contexts_;
    };
} // nxi

#endif // INCLUDE_NXI_SYSTEM_CONTEXT_HPP_NXI
