/*
 * TestCutFilter.cc
 *
 *  Created on: Oct 07, 2014
 *      Author: nsoblath
 */

#include "TestCuts.hh"

#include "ApplyCut.hh"
#include "CutFilter.hh"
#include "Logger.hh"
#include "DataFrame.hh"
#include "TestData.hh"
#include "CoreData.hh"

LOGGER(testlog, "TestCutFilter");

using namespace Nymph;
using namespace std;

int main()
{
    DataHandle dataHandle;
    ExtCoreData data("data");
    TestDataExt testData("test");

    CutStatus& cutStatus = data.GetCutStatus();
    unsigned awesomeCutPos = 0;
    string awesomeCutName("awesome-cut");
    unsigned notAwesomeCutPos = 1;
    string notAwesomeCutName("not-awesome-cut");
    cutStatus.AssignCutResult(awesomeCutPos, awesomeCutName);
    cutStatus.AssignCutResult(notAwesomeCutPos, notAwesomeCutName);
    LINFO(testlog, "Initial cut state: " << cutStatus.IsCut());

    ApplyCut applyCut;

    LINFO(testlog, "Applying awesome cut");
    applyCut.SetCut(new AwesomeCut());
    applyCut.ApplyTheCut(dataHandle);

    LINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
    LINFO(testlog, "Has cut result <" << awesomeCutName << ">? " << cutStatus.HasCutResult(awesomeCutName));
    LINFO(testlog, "Has cut result at " << awesomeCutPos << "? " << cutStatus.HasCutResult(awesomeCutPos));
    LINFO(testlog, "Cut state of <" << awesomeCutName << ">is: " << cutStatus.GetCutState(awesomeCutName));
    LINFO(testlog, "Cut state at " << awesomeCutPos << " is: " << cutStatus.GetCutState(awesomeCutPos));
    LINFO(testlog, "Is cut (all results)? " << cutStatus.IsCut());
    LINFO(testlog, "Is cut (with mask \"0\")? " << cutStatus.IsCut("0"));

    LINFO(testlog, "Applying not-awesome cut");
    applyCut.SelectCut("not-awesome-cut");
    applyCut.ApplyTheCut(dataHandle);

    LINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
    LINFO(testlog, "Has cut result <" << awesomeCutName << ">? " << cutStatus.HasCutResult(awesomeCutName));
    LINFO(testlog, "Has cut result at " << awesomeCutPos << "? " << cutStatus.HasCutResult(awesomeCutPos));
    LINFO(testlog, "Cut state of <" << awesomeCutName << "> is: " << cutStatus.GetCutState(awesomeCutName));
    LINFO(testlog, "Cut state at " << awesomeCutPos << " is: " << cutStatus.GetCutState(awesomeCutPos));
    LINFO(testlog, "Has cut result <" << notAwesomeCutName << ">? " << cutStatus.HasCutResult(notAwesomeCutName));
    LINFO(testlog, "Has cut result at " << notAwesomeCutPos << "? " << cutStatus.HasCutResult(notAwesomeCutPos));
    LINFO(testlog, "Cut state of <" << notAwesomeCutName << "> is: " << cutStatus.GetCutState(notAwesomeCutName));
    LINFO(testlog, "Cut state at " << notAwesomeCutPos << " is: " << cutStatus.GetCutState(notAwesomeCutPos));

    CutFilter cutFilter;
    ExtCoreData dataExt;

    LINFO(testlog, "Filtering with all cuts");
    cutFilter.SetCutMaskAll();
    LINFO(testlog, "Is cut (all results)? " << cutFilter.Filter(dataExt));

    LINFO(testlog, "Testing filter set with a string");
    cutFilter.SetCutMask("00");
    LINFO(testlog, "Is cut with mask \"00\"? " << cutFilter.Filter(dataExt));
    cutFilter.SetCutMask("01");
    LINFO(testlog, "Is cut with mask \"01\"? " << cutFilter.Filter(dataExt));
    cutFilter.SetCutMask("10");
    LINFO(testlog, "Is cut with mask \"10\"? " << cutFilter.Filter(dataExt));
    cutFilter.SetCutMask("11");
    LINFO(testlog, "Is cut with mask \"11\"? " << cutFilter.Filter(dataExt));

    LINFO(testlog, "Testing filter set with an integer");
    cutFilter.SetCutMask(0);
    LINFO(testlog, "Is cut with mask 0? " << cutFilter.Filter(dataExt));
    cutFilter.SetCutMask(1);
    LINFO(testlog, "Is cut with mask 1? " << cutFilter.Filter(dataExt));
    cutFilter.SetCutMask(2);
    LINFO(testlog, "Is cut with mask 2? " << cutFilter.Filter(dataExt));
    cutFilter.SetCutMask(3);
    LINFO(testlog, "Is cut with mask 3? " << cutFilter.Filter(dataExt));

    return 0;
}
