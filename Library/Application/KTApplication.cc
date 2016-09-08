/*
 * KTApplication.cc
 *
 *  Created on: Aug 5, 2012
 *      Author: nsoblath
 */

#include "KTApplication.hh"

#include "../Utility/KTEventLoop.hh"
#include "KTLogger.hh"
#include "KTParamInputJSON.hh"

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

    KTApplication::KTApplication(int argC, char** argV, bool makeTApp, bool requireArgs, KTParamNode* defaultConfig) :
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
        fCLHandler->TakeArguments(argC, argV);

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
        const KTParamNode* clConfigOverride = fCLHandler->GetConfigOverride();

        // Default configuration
        if (defaultConfig != NULL)
        {
            fConfigurator->Merge(*defaultConfig);
        }

        // JSON file configuration
        if (! fConfigFilename.empty())
        {
            KTParamNode* t_config_from_file = KTParamInputJSON::ReadFile( fConfigFilename );
            if( t_config_from_file == NULL )
            {
                throw KTException() << "error parsing config file <" << fConfigFilename << ">";
            }
            fConfigurator->Merge( *t_config_from_file );
            delete t_config_from_file;
        }

        // Command-line JSON configuration
        if (! clJSON.empty())
        {
            KTParamNode* t_config_from_json = KTParamInputJSON::ReadString( clJSON );
            if( t_config_from_json == NULL )
            {
                throw KTException() << "error parsing CL JSON";
            }
            fConfigurator->Merge( *t_config_from_json );
            delete t_config_from_json;
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

    void KTApplication::AddConfigOptionsToCLHandler(const KTParam* param, const std::string& rootName)
    {
    	if (param->IsNull())
    	{
    		fCLHandler->AddOption("Config File Options", "Configuration flag: " + rootName, rootName, false);
    	}
    	else if (param->IsValue())
    	{
    		fCLHandler->AddOption< string >("Config File Options", "Configuration value: " + rootName, rootName, false);
    	}
    	else if (param->IsArray())
    	{
        	string nextRootName(rootName);
        	if (! rootName.empty() && rootName.back() != '.') nextRootName += ".";

        	const KTParamArray* paramArray = &param->AsArray();
    		unsigned arraySize = paramArray->Size();
    		for (unsigned iParam = 0; iParam < arraySize; ++iParam)
    		{
    			AddConfigOptionsToCLHandler(paramArray->At(iParam), nextRootName + std::to_string(iParam));
    		}
    	}
    	else if (param->IsNode())
    	{
        	string nextRootName(rootName);
        	if (! rootName.empty()) nextRootName += ".";

        	const KTParamNode* paramNode = &param->AsNode();
    		for (KTParamNode::const_iterator nodeIt = paramNode->Begin(); nodeIt != paramNode->End(); ++nodeIt)
    		{
    			AddConfigOptionsToCLHandler(nodeIt->second, nextRootName + nodeIt->first);
    		}
    	}

    	return;
    }

    bool KTApplication::Configure(const KTParamNode* node)
    {
        if (node == NULL) return true;

        if (node->GetValue("root-app", false))
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
