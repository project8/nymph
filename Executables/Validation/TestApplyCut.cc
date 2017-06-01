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
    KTDataPtr dataPtr(new KTData());
    KTTestData& testData = dataPtr->Of< KTTestData >();

    KTCutStatus& cutStatus = dataPtr->CutStatus();
    KTINFO(testlog, "Initial cut state: " << cutStatus.IsCut());

    KTApplyCut applyCut;

    KTINFO(testlog, "Applying awesome cut");
    applyCut.SetCut(new KTAwesomeCut());
    applyCut.ApplyCut(dataPtr);

    KTINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
    KTINFO(testlog, "Has cut result \"awesome-cut\"? " << cutStatus.HasCutResult("awesome-cut"));
    KTINFO(testlog, "Has cut result <KTAwesomeCut::Result>? " << cutStatus.HasCutResult< KTAwesomeCut::Result >());
    KTINFO(testlog, "Cut state of \"awesome-cut\" is: " << cutStatus.GetCutState("awesome-cut"));
    KTINFO(testlog, "Cut state of <KTAwesomeCut::Result> is: " << cutStatus.GetCutState< KTAwesomeCut::Result >());
    KTINFO(testlog, "Is cut (all results)? " << cutStatus.IsCut());
    KTINFO(testlog, "Is cut (with mask \"0\")? " << cutStatus.IsCut("0"));

    KTINFO(testlog, "Applying not-awesome cut");
    applyCut.SelectCut("not-awesome-cut");
    applyCut.ApplyCut(dataPtr);

    KTINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
    KTINFO(testlog, "Has cut result \"awesome-cut\"? " << cutStatus.HasCutResult("awesome-cut"));
    KTINFO(testlog, "Has cut result <KTAwesomeCut::Result>? " << cutStatus.HasCutResult< KTAwesomeCut::Result >());
    KTINFO(testlog, "Cut state of \"awesome-cut\" is: " << cutStatus.GetCutState("awesome-cut"));
    KTINFO(testlog, "Cut state of <KTAwesomeCut::Result> is: " << cutStatus.GetCutState< KTAwesomeCut::Result >());
    KTINFO(testlog, "Has cut result \"not-awesome-cut\"? " << cutStatus.HasCutResult("not-awesome-cut"));
    KTINFO(testlog, "Has cut result <KTNotAwesomeCut::Result>? " << cutStatus.HasCutResult< KTNotAwesomeCut::Result >());
    KTINFO(testlog, "Cut state of \"not-awesome-cut\" is: " << cutStatus.GetCutState("not-awesome-cut"));
    KTINFO(testlog, "Cut state of <KTNotAwesomeCut::Result> is: " << cutStatus.GetCutState< KTNotAwesomeCut::Result >());
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
