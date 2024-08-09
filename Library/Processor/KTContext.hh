/**
 @file KTContext.hh
 @brief Contains KTContext
 @details KTContext contains the run context that's available to all processors
 @author: N. S. Oblath
 @date: Aug 7, 2024
 */

#ifndef KTCONTEXT_HH_
#define KTCONTEXT_HH_

#include "param.hh"

namespace Nymph
{

    class KTContext
    {
        public:
            KTContext();
            virtual ~KTContext();

            bool Has(const std::string& name) const;

            scarab::param& Get(const std::string& name);
            const scarab::param& Get(const std::string& name) const;

            void Add(const std::string& name, const scarab::param& data);

        protected:
            scarab::param_node fData;
    };

    class KTHasContext
    {
        public:
            KTHasContext();
            virtual ~KTHasContext();

            KTContext& Context();
            const KTContext& Context() const;

            void SetContext(const std::shared_ptr< KTContext > context);

        protected:
            std::shared_ptr< KTContext > fContext;

    };

    inline KTContext& KTHasContext::Context()
    {
        return *fContext;
    }

    inline const KTContext& KTHasContext::Context() const
    {
        return *fContext;
    }

    inline void KTHasContext::SetContext(std::shared_ptr< KTContext > context)
    {
        fContext = context;
        return;
    }

} /* namespace Nymph */

#endif /* KTCONTEXT_HH_ */
