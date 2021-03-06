/*
 * TestApplication.cc
 *
 *  Created on: Aug 1, 2012
 *      Author: nsoblath
 *
 *  To test the command-line interface:
 *    > TestApplication -c /path/to/TestConfigurator.json
 *    > TestApplication -c /path/to/TestConfigurator.json --test-app-opt=blah
 *    > TestApplication -c /path/to/TestConfigurator.json --int-data 20
 */


#include "KTTestConfigurable.hh"
#include "KTApplication.hh"
#include "KTCommandLineOption.hh"

#include "param.hh"

using namespace Nymph;
using namespace std;

KTLOGGER(testapplog, "TestApplication");

// Add an application-specific command-line option
static KTCommandLineOption< string > sTestAppOption("TestApplication", "Application-specific command-line option", "test-app-opt", 'a');


int main(int argc, char** argv)
{
    //****************************
    // Create the app; this performs the initial command-line processing
    //****************************

    KTApplication* app = NULL;
    try
    {
        app = new KTApplication(argc, argv);
    }
    catch (std::exception& e)
    {
        KTERROR(testapplog, "Exception caught from KTApplication constructor:\n"
                << '\t' << e.what());
        return 0;
    }


    //****************************
    // Check basic command-line reading
    //****************************

    KTINFO(testapplog, "Application: " << app->GetCommandLineHandler()->GetExecutableName());
    KTINFO(testapplog, "Config filename: " << app->GetCommandLineHandler()->GetConfigFilename());


    //****************************
    // Check reading of command-line options
    //****************************

    if (app->GetCommandLineHandler()->IsCommandLineOptSet("test-app-opt"))
    {
        KTINFO(testapplog, "Test application option is set to value <" << app->GetCommandLineHandler()->GetCommandLineValue< string >("test-app-opt") << ">");
    }
    else
    {
        KTINFO(testapplog, "Test application option was not set");
    }

    if (app->GetCommandLineHandler()->IsCommandLineOptSet("int-data"))
    {
        KTINFO(testapplog, "Test option <int-data> is set to value <" << app->GetCommandLineHandler()->GetCommandLineValue< int >("int-data") << ">");
    }
    else
    {
        KTINFO(testapplog, "Test option <int-data> was not set");
    }


    //****************************
    // Check reading of the config file
    //****************************

    KTTestConfigurable* testObj = new KTTestConfigurable();

    const scarab::param_node& topNode = app->GetConfigurator()->Config();

    if (testObj->Configure(topNode[testObj->GetConfigName()].as_node()))
    {
        KTINFO(testapplog, "Configuration complete:\n"
                << "\tInt data: " << testObj->GetIntData() << '\n'
                << "\tDouble data: " << testObj->GetDoubleData() << '\n'
                << "\tString data: " << testObj->GetStringData());
    }
    else
    {
        KTERROR(testapplog, "Something went wrong during the configuration");
    }

    delete testObj;

    return 0;
}
