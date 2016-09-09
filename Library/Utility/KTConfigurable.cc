/*
 * KTConfigurable.cc
 *
 *  Created on: Aug 21, 2012
 *      Author: nsoblath
 */

#include "KTConfigurable.hh"

#include "KTCommandLineHandler.hh"
#include "KTLogger.hh"
#include "KTConfigurator.hh"
#include "param.hh"

using std::string;

namespace Nymph
{

    KTLOGGER(conflog, "KTConfigurable");

    //******************
    // KTConfigurable
    //******************

    KTConfigurable::KTConfigurable(const string& name) :
            fCLHandler(KTCommandLineHandler::get_instance()),
            fConfigName(name)
    {
    }

    KTConfigurable::~KTConfigurable()
    {
    }



    //**********************
    // KTSelfConfigurable
    //**********************

    KTSelfConfigurable::KTSelfConfigurable(const string& name) :
            KTConfigurable(name),
            fIsConfigured(false)
    {
    }

    KTSelfConfigurable::~KTSelfConfigurable()
    {
    }

    bool KTSelfConfigurable::Configure()
    {
        if (fIsConfigured) return true;

        scarab::param_node* node = KTConfigurator::get_instance()->Config();
        if (! this->Configure(node))
        {
            KTERROR(conflog, "An error occurred while configuring <" << fConfigName << ">");
            return false;
        }
        fIsConfigured = IsReady();
        return fIsConfigured;
    }

} /* namespace Nymph */
