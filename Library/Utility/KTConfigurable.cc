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

#include "param_codec.hh"

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

    bool KTConfigurable::Configure(const std::string& config)
    {
        scarab::param_translator translator;
        scarab::param_node optNode;
        optNode.add( "encoding", "json" );
        return Configure( translator.read_string( config, optNode )->as_node() );;
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

        if (! this->Configure(KTConfigurator::get_instance()->Config()))
        {
            KTERROR(conflog, "An error occurred while configuring <" << fConfigName << ">");
            return false;
        }
        fIsConfigured = IsReady();
        return fIsConfigured;
    }

} /* namespace Nymph */
