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

    KTApplication::KTApplication(bool makeTApp) :
            KTConfigurable("app"),
            fCLHandler(KTCommandLineHandler::GetInstance()),
            fConfigurator( KTConfigurator::GetInstance() ),
            fConfigFilename()
    {

#ifdef ROOT_FOUND
        fTApp = NULL;
        if (makeTApp)
        {
            fTApp = new TApplication("", 0, 0);
        }
#else
        if (makeTApp)
        {
            KTWARN(applog, "TApplication requested, but Nymph has been built without ROOT dependence.");
        }
#endif
    }

    KTApplication::KTApplication(int argC, char** argV, bool makeTApp, bool requireArgs, scarab::param_node* defaultConfig) :
            KTConfigurable("app"),
            fCLHandler(KTCommandLineHandler::GetInstance()),
            fConfigurator( KTConfigurator::GetInstance() ),
            fConfigFilename()
    {
#ifdef ROOT_FOUND
        fTApp = NULL;
        if (makeTApp)
        {
            StartTApplication();
        }
#else
        if (makeTApp)
        {
            KTWARN(applog, "TApplication requested, but Nymph has been built without ROOT dependence.");
        }
#endif

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
        if (defaultConfig != NULL)
        {
            fConfigurator->Merge(*defaultConfig);
        }

        // JSON file configuration
        if (! fConfigFilename.empty())
        {
            scarab::path configFilePath = scarab::expand_path( fConfigFilename );
            scarab::param_translator translator;
            scarab::param* configFromFile = translator.read_file( configFilePath.native() );
            if( configFromFile == NULL )
            {
                throw KTException() << "[KTApplication] error parsing config file";
            }
            if( ! configFromFile->is_node() )
            {
                throw KTException() << "[KTApplication] configuration file must consist of an object/node";
            }
            fConfigurator->Merge( configFromFile->as_node() );
            delete configFromFile;
        }

        // Command-line JSON configuration
        if (! clJSON.empty())
        {
            scarab::param_input_json inputJSON;
            scarab::param* configFromJSON = inputJSON.read_string( clJSON );
            if( ! configFromJSON->is_node() )
            {
                throw KTException() << "[KTApplication] command line json must be an object";
            }
            fConfigurator->Merge( configFromJSON->as_node() );
            delete configFromJSON;
        }

        // Command-line overrides
        if (clConfigOverride != NULL)
        {
            fConfigurator->Merge( *clConfigOverride );
        }

        KTINFO( applog, "Final configuration:\n" << *(fConfigurator->Config()) );

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
#ifdef ROOT_FOUND
        delete fTApp;
#endif
    }

    void KTApplication::AddConfigOptionsToCLHandler(const scarab::param* param, const std::string& rootName)
    {
    	if (param->is_null())
    	{
    		fCLHandler->AddOption("Config File Options", "Configuration flag: " + rootName, rootName, false);
    	}
    	else if (param->is_value())
    	{
    		fCLHandler->AddOption< string >("Config File Options", "Configuration value: " + rootName, rootName, false);
    	}
    	else if (param->is_array())
    	{
        	string nextRootName(rootName);
        	if (! rootName.empty() && rootName.back() != '.') nextRootName += ".";

        	const scarab::param_array* paramArray = &param->as_array();
    		unsigned arraySize = paramArray->size();
    		for (unsigned iParam = 0; iParam < arraySize; ++iParam)
    		{
    			AddConfigOptionsToCLHandler(paramArray->at(iParam), nextRootName + std::to_string(iParam));
    		}
    	}
    	else if (param->is_node())
    	{
        	string nextRootName(rootName);
        	if (! rootName.empty()) nextRootName += ".";

        	const scarab::param_node* paramNode = &param->as_node();
    		for (scarab::param_node::const_iterator nodeIt = paramNode->begin(); nodeIt != paramNode->end(); ++nodeIt)
    		{
    			AddConfigOptionsToCLHandler(nodeIt->second, nextRootName + nodeIt->first);
    		}
    	}

    	return;
    }

    bool KTApplication::Configure(const scarab::param_node* node)
    {
        if (node == NULL) return true;

        if (node->get_value("root-app", false))
        {
#ifdef ROOT_FOUND
            StartTApplication();
#else
            KTWARN(applog, "TApplication requested, but Nymph has been built without ROOT dependence.");
#endif
        }
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

#ifdef ROOT_FOUND
    bool KTApplication::StartTApplication()
    {
        if (fTApp != NULL)
            return true;

        fTApp = new TApplication("", 0, 0);

        if (fTApp != NULL)
        {
            return true;
        }
        return false;
    }
#endif

} /* namespace Nymph */
