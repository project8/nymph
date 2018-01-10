/*
 * KTCacheDirectory.cc
 *
 *  Created on: Dec 3, 2012
 *      Author: nsoblath
 */

#include "KTCacheDirectory.hh"

#include "KTLogger.hh"
#include "param.hh"

using std::string;

namespace Nymph
{
    KTLOGGER(dirlog, "KTCacheDirectory");

    KTCacheDirectory::KTCacheDirectory(const std::string& name) :
            KTDirectory(),
            KTSelfConfigurable(name)
    {
    }

    KTCacheDirectory::~KTCacheDirectory()
    {
    }

    bool KTCacheDirectory::Configure(const scarab::param_node& node)
    {
        return SetPath(node.get_value("path", fPath.string()));
    }

} /* namespace Nymph */
