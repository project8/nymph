/*
 * KTConfigurator.cc
 *
 *  Created on: Nov 5, 2013
 *      Author: nsoblath
 */

#include "KTConfigurator.hh"

#include "KTLogger.hh"

using std::string;

namespace Nymph
{
    KTLOGGER( conflog, "KTConfigurator" );

    KTConfigurator::KTConfigurator() :
            fMasterConfig( new scarab::param_node() ),
            fParamBuffer( NULL ),
            fStringBuffer()
    {
    }

    KTConfigurator::~KTConfigurator()
    {
        delete fMasterConfig;
    }

} /* namespace Nymph */
