/*
 * KTApplication.cc
 *
 *  Created on: Aug 5, 2012
 *      Author: nsoblath
 */

#include "KTApplication.hh"

#include "KTEventLoop.hh"
#include "KTLogger.hh"

#include "param_codec.hh"
#include "param_json.hh"

using std::set;
using std::string;

#include <iostream>

namespace Nymph
{
    KTLOGGER(applog, "KTApplication");

    KTApplication::KTApplication() :
            KTConfigurable("app"),
            fCLHandler(KTCommandLineHandler::get_instance()),
            fConfigurator( KTConfigurator::get_instance() ),
            fConfigFilename()
    {
    }

    KTApplication::KTApplication(int argC, char** argV, bool requireArgs, const scarab::param_node& defaultConfig) :
            KTConfigurable("app"),
            fCLHandler(KTCommandLineHandler::get_instance()),
            fConfigurator( KTConfigurator::get_instance() ),
            fConfigFilename()
    {
        // process command-line arguments
        fCLHandler->ProcessCommandLine(argC, argV);

        // if requested, print help or version messages, and exit
        if (fCLHandler->GetPrintHelpMessageFlag() || (requireArgs && fCLHandler->GetNArgs() == 1))
        {
            fCLHandler->PrintHelpMessage();
            exit(0);
        }
        if (fCLHandler->GetPrintVersionMessage())
        {
            fCLHandler->PrintVersionMessage();
            exit(0);
        }

        // get configuration information from the CLHandler
        fConfigFilename = fCLHandler->GetConfigFilename();
        string clJSON = fCLHandler->GetCommandLineJSON();
        const scarab::param_node* clConfigOverride = fCLHandler->GetConfigOverride();

        // Default configuration
        fConfigurator->Merge(defaultConfig);

        // JSON file configuration
        if (! fConfigFilename.empty())
        {
            scarab::path configFilePath = scarab::expand_path( fConfigFilename );
            scarab::param_translator translator;
            scarab::param_ptr_t configFromFile = translator.read_file( configFilePath.native() );
            if( configFromFile == NULL )
            {
                throw KTException() << "[KTApplication] error parsing config file";
            }
            if( ! configFromFile->is_node() )
            {
                throw KTException() << "[KTApplication] configuration file must consist of an object/node";
            }
            fConfigurator->Merge( configFromFile->as_node() );
        }

        // Command-line JSON configuration
        if (! clJSON.empty())
        {
            scarab::param_input_json inputJSON;
            scarab::param_ptr_t configFromJSON = inputJSON.read_string( clJSON );
            if( ! configFromJSON->is_node() )
            {
                throw KTException() << "[KTApplication] command line json must be an object";
            }
            fConfigurator->Merge( configFromJSON->as_node() );
        }

        // Command-line overrides
        if (clConfigOverride != NULL)
        {
            fConfigurator->Merge( *clConfigOverride );
        }

        KTINFO( applog, "Final configuration:\n" << fConfigurator->Config() );

        AddConfigOptionsToCLHandler(fConfigurator->Config(), "");
        fCLHandler->FinalizeNewOptionGroups();

        if (fCLHandler->GetPrintHelpMessageAfterConfigFlag())
        {
        	fCLHandler->PrintHelpMessage();
        	exit(0);
        }

        fCLHandler->DelayedCommandLineProcessing();
    }

    KTApplication::~KTApplication()
    {
        for (set< KTEventLoop* >::iterator loopIt = fEventLoops.begin(); loopIt != fEventLoops.end(); ++loopIt)
        {
            // does NOT delete event loops
            (*loopIt)->Stop();
        }
    }

    void KTApplication::AddConfigOptionsToCLHandler(const scarab::param& param, const std::string& rootName)
    {
    	if (param.is_null())
    	{
    		fCLHandler->AddOption("Config File Options", "Configuration flag: " + rootName, rootName, false);
    	}
    	else if (param.is_value())
    	{
    		fCLHandler->AddOption< string >("Config File Options", "Configuration value: " + rootName, rootName, false);
    	}
    	else if (param.is_array())
    	{
        	string nextRootName(rootName);
        	if (! rootName.empty() && rootName.back() != '.') nextRootName += ".";

        	const scarab::param_array& paramArray = param.as_array();
    		unsigned arraySize = paramArray.size();
    		for (unsigned iParam = 0; iParam < arraySize; ++iParam)
    		{
    			AddConfigOptionsToCLHandler(paramArray[iParam], nextRootName + std::to_string(iParam));
    		}
    	}
    	else if (param.is_node())
    	{
        	string nextRootName(rootName);
        	if (! rootName.empty()) nextRootName += ".";

        	const scarab::param_node& paramNode = param.as_node();
    		for (scarab::param_node::const_iterator nodeIt = paramNode.begin(); nodeIt != paramNode.end(); ++nodeIt)
    		{
    			AddConfigOptionsToCLHandler(*nodeIt, nextRootName + nodeIt.name());
    		}
    	}

    	return;
    }

    bool KTApplication::Configure(const scarab::param_node&)
    {
        return true;
    }

    void KTApplication::AddEventLoop(KTEventLoop* loop)
    {
        fEventLoops.insert(loop);
        return;
    }

    void KTApplication::RemoveEventLoop(KTEventLoop* loop)
    {
        fEventLoops.erase(loop);
        return;
    }

} /* namespace Nymph */
