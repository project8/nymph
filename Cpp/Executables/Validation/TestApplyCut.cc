/*
 * TestApplyCut.cc
 *
 *  Created on: Oct 07, 2014
 *      Author: nsoblath
 */

#include "KTTestCuts.hh"

#include "KTApplyCut.hh"
#include "KTLogger.hh"
#include "KTTestData.hh"

KTLOGGER(testlog, "TestApplyCut");

using namespace Nymph;
using namespace std;

int main()
{
    KTDataHandle dataHandle = CreateNewDataHandle();
    KTTestData& testData = dataHandle->Of< KTTestDataExt >();

    KTCutStatus& cutStatus = dataHandle->CutStatus();
    unsigned awesomeCutPos = 0;
    string awesomeCutName("awesome-cut");
    unsigned notAwesomeCutPos = 1;
    string notAwesomeCutName("not-awesome-cut");
    cutStatus.AssignCutResult(awesomeCutPos, awesomeCutName);
    cutStatus.AssignCutResult(notAwesomeCutPos, notAwesomeCutName);
    KTINFO(testlog, "Initial cut state: " << cutStatus.IsCut());

    KTApplyCut applyCut;

    KTINFO(testlog, "Applying awesome cut");
    applyCut.SetCut(new KTAwesomeCut());
    applyCut.ApplyCut(dataHandle);

    KTINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
    KTINFO(testlog, "Has cut result <" << awesomeCutName << ">? " << cutStatus.HasCutResult(awesomeCutName));
    KTINFO(testlog, "Has cut result at " << awesomeCutPos << "? " << cutStatus.HasCutResult(awesomeCutPos));
    KTINFO(testlog, "Cut state of <" << awesomeCutName << ">is: " << cutStatus.GetCutState(awesomeCutName));
    KTINFO(testlog, "Cut state at " << awesomeCutPos << " is: " << cutStatus.GetCutState(awesomeCutPos));
    KTINFO(testlog, "Is cut (all results)? " << cutStatus.IsCut());
    KTINFO(testlog, "Is cut (with mask \"0\")? " << cutStatus.IsCut("0"));

    KTINFO(testlog, "Applying not-awesome cut");
    applyCut.SelectCut("not-awesome-cut");
    applyCut.ApplyCut(dataHandle);

    KTINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
    KTINFO(testlog, "Has cut result <" << awesomeCutName << ">? " << cutStatus.HasCutResult(awesomeCutName));
    KTINFO(testlog, "Has cut result at " << awesomeCutPos << "? " << cutStatus.HasCutResult(awesomeCutPos));
    KTINFO(testlog, "Cut state of <" << awesomeCutName << "> is: " << cutStatus.GetCutState(awesomeCutName));
    KTINFO(testlog, "Cut state at " << awesomeCutPos << " is: " << cutStatus.GetCutState(awesomeCutPos));
    KTINFO(testlog, "Has cut result <" << notAwesomeCutName << ">? " << cutStatus.HasCutResult(notAwesomeCutName));
    KTINFO(testlog, "Has cut result at " << notAwesomeCutPos << "? " << cutStatus.HasCutResult(notAwesomeCutPos));
    KTINFO(testlog, "Cut state of <" << notAwesomeCutName << "> is: " << cutStatus.GetCutState(notAwesomeCutName));
    KTINFO(testlog, "Cut state at " << notAwesomeCutPos << " is: " << cutStatus.GetCutState(notAwesomeCutPos));
    KTINFO(testlog, "Is cut (all results)? " << cutStatus.IsCut());
    KTINFO(testlog, "Is cut with mask \"00\"? " << cutStatus.IsCut("00"));
    KTINFO(testlog, "Is cut with mask \"01\"? " << cutStatus.IsCut("01"));
    KTINFO(testlog, "Is cut with mask \"10\"? " << cutStatus.IsCut("10"));
    KTINFO(testlog, "Is cut with mask \"11\"? " << cutStatus.IsCut("11"));
    KTINFO(testlog, "Is cut with mask 0? " << cutStatus.IsCut(0));
    KTINFO(testlog, "Is cut with mask 1? " << cutStatus.IsCut(1));
    KTINFO(testlog, "Is cut with mask 2? " << cutStatus.IsCut(2));
    KTINFO(testlog, "Is cut with mask 3? " << cutStatus.IsCut(3));

    return 0;
}
