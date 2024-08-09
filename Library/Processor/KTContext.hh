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

} /* namespace Nymph */

#endif /* KTCONTEXT_HH_ */
