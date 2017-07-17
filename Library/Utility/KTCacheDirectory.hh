/*
 * KTCacheDirectory.hh
 *
 *  Created on: Dec 3, 2012
 *      Author: nsoblath
 */

#ifndef KTCACHEDIRECTORY_HH_
#define KTCACHEDIRECTORY_HH_

#include "KTConfigurable.hh"
#include "KTDirectory.hh"

#include "singleton.hh"

namespace Nymph
{

    class KTCacheDirectory : public KTDirectory, public scarab::singleton< KTCacheDirectory >, public KTSelfConfigurable
    {
        protected:
            friend class scarab::singleton< KTCacheDirectory >;
            friend class scarab::destroyer< KTCacheDirectory >;
            KTCacheDirectory(const std::string& name = "cache-directory");
            virtual ~KTCacheDirectory();

        public:
            using KTSelfConfigurable::Configure;

            bool Configure(const scarab::param_node& node);

            bool IsReady() const;

    };

    inline bool KTCacheDirectory::IsReady() const
    {
        return fPathExists && fAccess == eReadWrite;
    }

} /* namespace Nymph */
#endif /* KTCACHEDIRECTORY_HH_ */
