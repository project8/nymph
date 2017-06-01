/*
 * TestCutFilter.cc
 *
 *  Created on: Oct 07, 2014
 *      Author: nsoblath
 */

#include "KTTestCuts.hh"

#include "KTApplyCut.hh"
#include "KTCutFilter.hh"
#include "KTLogger.hh"
#include "KTTestData.hh"

KTLOGGER(testlog, "TestCutFilter");

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

    KTCutFilter cutFilter;
    KTData& data = dataPtr->Of< KTData >();

    KTINFO(testlog, "Filtering with all cuts");
    cutFilter.SetCutMaskAll();
    KTINFO(testlog, "Is cut (all results)? " << cutFilter.Filter(data));

    KTINFO(testlog, "Testing filter set with a string");
    cutFilter.SetCutMask("00");
    KTINFO(testlog, "Is cut with mask \"00\"? " << cutFilter.Filter(data));
    cutFilter.SetCutMask("01");
    KTINFO(testlog, "Is cut with mask \"01\"? " << cutFilter.Filter(data));
    cutFilter.SetCutMask("10");
    KTINFO(testlog, "Is cut with mask \"10\"? " << cutFilter.Filter(data));
    cutFilter.SetCutMask("11");
    KTINFO(testlog, "Is cut with mask \"11\"? " << cutFilter.Filter(data));

    KTINFO(testlog, "Testing filter set with an integer");
    cutFilter.SetCutMask(0);
    KTINFO(testlog, "Is cut with mask 0? " << cutFilter.Filter(data));
    cutFilter.SetCutMask(1);
    KTINFO(testlog, "Is cut with mask 1? " << cutFilter.Filter(data));
    cutFilter.SetCutMask(2);
    KTINFO(testlog, "Is cut with mask 2? " << cutFilter.Filter(data));
    cutFilter.SetCutMask(3);
    KTINFO(testlog, "Is cut with mask 3? " << cutFilter.Filter(data));

    return 0;
}
