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
            fMasterConfig(),
            fStringBuffer()
    {
    }

    KTConfigurator::~KTConfigurator()
    {
<<<<<<< HEAD:Library_v1/Application/KTConfigurator.cc
        delete fMasterConfig;
=======
>>>>>>> develop:Library/Application/KTConfigurator.cc
    }

} /* namespace Nymph */
