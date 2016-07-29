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

    void KTConfigurator::Merge(const scarab::param_node& aNode)
    {
        fMasterConfig->Merge(aNode);
        return;
    }

    scarab::param_node* KTConfigurator::Config()
    {
        return fMasterConfig;
    }

    const scarab::param_node* KTConfigurator::Config() const
    {
        return fMasterConfig;
    }

} /* namespace Nymph */
