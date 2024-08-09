/*
 * TestContext.cc
 *
 *  Created on: Aug 9, 2024
 *      Author: N.S. Oblath
 *
 *  Usage: > ./TestContext
 *
 *  Purpose: Test access to context from multiple processors
 *
 */

#include "KTTestProcessor.hh"

#include "KTContext.hh"
#include "KTLogger.hh"
#include "KTProcessorToolbox.hh"

using namespace Nymph;

KTLOGGER(testlog, "TestContext");

int main()
{
    KTTestProcessorA procA;
    KTTestProcessorB procB;

    KTINFO(testlog, "Contexts belonging to procA and procB should be independent\n&procA.Context() == &procB.Context(): " << (&procA.Context() == &procB.Context()));

    KTProcessorToolbox toolbox;    

    if (! toolbox.AddProcessor("test-proc-a", "tpa"))
    {
        KTERROR(testlog, "Unable to create test proc A with the Processor Toolbox");
        return 1;
    }
    if (! toolbox.AddProcessor("test-proc-b", "tpb"))
    {
        KTERROR(testlog, "Unable to create test proc B with the Processor Toolbox");
        return 1;
    }

    KTProcessor& procAtb = *toolbox.GetProcessor("tpa");
    KTProcessor& procBtb = *toolbox.GetProcessor("tpb");

    KTINFO(testlog, "Contexts belonging to procAtb and procBtb should be identical\n&procAtb.Context() == &procBtb.Context(): " << (&procAtb.Context() == &procBtb.Context()));

    toolbox.Context().Add("test", scarab::param_value(5));
    KTINFO(testlog, "toolbox has context \"test\"? (expect \"1\") " << toolbox.Context().Has("test"));
    KTINFO(testlog, "procAtb has context \"test\"? (expect \"1\") " << procAtb.Context().Has("test"));
    KTINFO(testlog, "procBtb has context \"test\"? (expect \"1\") " << procBtb.Context().Has("test"));
    KTINFO(testlog, "Values are all equal? (expect \"1 1\") " << (toolbox.Context().Get("test")().as_int() == procAtb.Context().Get("test")().as_int()) << " " << (toolbox.Context().Get("test")().as_int() == procBtb.Context().Get("test")().as_int()));

    return 0;
}
