/*
 * TestCut.cc
 *
 *  Created on: Sep 29, 2014
 *      Author: nsoblath
 */

#include "TestCuts.hh"

#include "Logger.hh"
#include "TestData.hh"

LOGGER(testlog, "TestCut");

using namespace Nymph;
using namespace std;

int main()
{
    try
    {
        ExtCoreData data;
        TestDataExt testData("test");

        CutStatus& cutStatus = data.GetCutStatus();

        LINFO(testlog, "Assigning awesome-cut");
        //unsigned awesomeCutPos = 0;
        string awesomeCutName("awesome-cut");
        //cutStatus.AssignCutResult(awesomeCutPos, awesomeCutName);
        cutStatus.AssignCutResult(awesomeCutName,1,1);

        LINFO(testlog, "Initial cut state: " << cutStatus.IsCut());

        LINFO(testlog, "Applying awesome cut");
        AwesomeCut cut(awesomeCutName);
        cut.Apply(data, testData);

        LINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
        LINFO(testlog, "Has cut result <" << awesomeCutName << ">? " << cutStatus.HasCutResult(awesomeCutName));
        //LINFO(testlog, "Has cut result at " << awesomeCutPos << "? " << cutStatus.HasCutResult(awesomeCutPos));
        LINFO(testlog, "Cut state of <" << awesomeCutName << ">is: " << cutStatus.GetCutState(awesomeCutName));
        //LINFO(testlog, "Cut state at " << awesomeCutPos << " is: " << cutStatus.GetCutState(awesomeCutPos));
        LINFO(testlog, "Is cut (all results)? " << cutStatus.IsCut());
        LINFO(testlog, "Current cut status: " << cutStatus);
        //LINFO(testlog, "Is cut (with mask \"0\")? " << cutStatus.IsCut("0"));

        LINFO(testlog, "Assigning not-awesome-cut");
        //unsigned notAwesomeCutPos = 1;
        string notAwesomeCutName("not-awesome-cut");
        //cutStatus.AssignCutResult(notAwesomeCutPos, notAwesomeCutName);
        cutStatus.AssignCutResult(notAwesomeCutName,1,1);

        LINFO(testlog, "Applying not-awesome cut");
        NotAwesomeCut naCut(notAwesomeCutName);
        naCut.Apply(data, testData);

        LINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
        LINFO(testlog, "Has cut result <" << awesomeCutName << ">? " << cutStatus.HasCutResult(awesomeCutName));
        //LINFO(testlog, "Has cut result at " << awesomeCutPos << "? " << cutStatus.HasCutResult(awesomeCutPos));
        LINFO(testlog, "Cut state of <" << awesomeCutName << "> is: " << cutStatus.GetCutState(awesomeCutName));
        //LINFO(testlog, "Cut state at " << awesomeCutPos << " is: " << cutStatus.GetCutState(awesomeCutPos));
        LINFO(testlog, "Has cut result <" << notAwesomeCutName << ">? " << cutStatus.HasCutResult(notAwesomeCutName));
        //LINFO(testlog, "Has cut result at " << notAwesomeCutPos << "? " << cutStatus.HasCutResult(notAwesomeCutPos));
        LINFO(testlog, "Cut state of <" << notAwesomeCutName << "> is: " << cutStatus.GetCutState(notAwesomeCutName));
        LINFO(testlog, "Cut state at " << notAwesomeCutPos << " is: " << cutStatus.GetCutState(notAwesomeCutPos));
        LINFO(testlog, "Is cut (all results)? " << cutStatus.IsCut());
        //LINFO(testlog, "Is cut with mask \"00\"? " << cutStatus.IsCut("00"));
        //LINFO(testlog, "Is cut with mask \"01\"? " << cutStatus.IsCut("01"));
        //LINFO(testlog, "Is cut with mask \"10\"? " << cutStatus.IsCut("10"));
        //LINFO(testlog, "Is cut with mask \"11\"? " << cutStatus.IsCut("11"));
        //LINFO(testlog, "Is cut with mask 0? " << cutStatus.IsCut(0));
        //LINFO(testlog, "Is cut with mask 1? " << cutStatus.IsCut(1));
        //LINFO(testlog, "Is cut with mask 2? " << cutStatus.IsCut(2));
        //LINFO(testlog, "Is cut with mask 3? " << cutStatus.IsCut(3));
    }
    catch(std::exception& e)
    {
        LERROR(testlog, "Exception caught: " << e.what());
    }

    return 0;
}
