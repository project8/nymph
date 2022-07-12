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
/*
//    DataHandle dataHandle = CreateNewDataHandle();
//    DataHandle dataHandle = std::shared_ptr< DataFrame >();
//    DataHandle dataHandle;
    DataFrame dataFrame;
    DataHandle dataHandle = std::shared_ptr< DataFrame >() ;
    ExtCoreData data("data");
    TestDataExt& testData = dataFrame.Get< TestDataExt >();	
//     TestDataExt& testData = dataHandle->Get< TestDataExt >();
//    TestDataExt testData("test");

    CutStatus& cutStatus = dataFrame.GetCutStatus();
//    CutStatus& cutStatus = dataHandle.fGetCutStatus;
//     CutStatus& cutStatus = data.GetCutStatus();
    dataHandle.reset(&dataFrame);
    unsigned awesomeCutPos = 0;
    string awesomeCutName("awesome-cut");
    unsigned notAwesomeCutPos = 1;
    string notAwesomeCutName("not-awesome-cut");
    cutStatus.AssignCutResult(awesomeCutPos, awesomeCutName);
    cutStatus.AssignCutResult(notAwesomeCutPos, notAwesomeCutName);
    LINFO(testlog, "Initial cut state: " << cutStatus.IsCut());

    ApplyCut applyCut;

    LINFO(testlog, "Applying awesome cut");
//    applyCut.SetCut(new AwesomeCut());
    applyCut.SetCut(new AwesomeCut(awesomeCutName));
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
*/

        ExtCoreData data;
        TestDataExt testData("test");

        CutStatus& cutStatus = data.GetCutStatus();

        LINFO(testlog, "Assigning awesome-cut");
        unsigned awesomeCutPos = 0;
        string awesomeCutName("awesome-cut");
        cutStatus.AssignCutResult(awesomeCutPos, awesomeCutName);

        LINFO(testlog, "Initial cut state: " << cutStatus.IsCut());

        LINFO(testlog, "Applying awesome cut");
        AwesomeCut cut(awesomeCutName);
        cut.Apply(data, testData);

        LINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
        LINFO(testlog, "Has cut result <" << awesomeCutName << ">? " << cutStatus.HasCutResult(awesomeCutName));
        LINFO(testlog, "Has cut result at " << awesomeCutPos << "? " << cutStatus.HasCutResult(awesomeCutPos));
        LINFO(testlog, "Cut state of <" << awesomeCutName << ">is: " << cutStatus.GetCutState(awesomeCutName));
        LINFO(testlog, "Cut state at " << awesomeCutPos << " is: " << cutStatus.GetCutState(awesomeCutPos));
        LINFO(testlog, "Is cut (all results)? " << cutStatus.IsCut());
        LINFO(testlog, "Current cut status: " << cutStatus);
        LINFO(testlog, "Is cut (with mask \"0\")? " << cutStatus.IsCut("0"));

        LINFO(testlog, "Assigning not-awesome-cut");
        unsigned notAwesomeCutPos = 1;
        string notAwesomeCutName("not-awesome-cut");
        cutStatus.AssignCutResult(notAwesomeCutPos, notAwesomeCutName);

        LINFO(testlog, "Applying not-awesome cut");
        NotAwesomeCut naCut(notAwesomeCutName);
        naCut.Apply(data, testData);

        LINFO(testlog, "Cuts present: " << cutStatus.CutResultsPresent())
        LINFO(testlog, "Has cut result <" << awesomeCutName << ">? " << cutStatus.HasCutResult(awesomeCutName));
        LINFO(testlog, "Has cut result at " << awesomeCutPos << "? " << cutStatus.HasCutResult(awesomeCutPos));
        LINFO(testlog, "Cut state of <" << awesomeCutName << "> is: " << cutStatus.GetCutState(awesomeCutName));
        LINFO(testlog, "Cut state at " << awesomeCutPos << " is: " << cutStatus.GetCutState(awesomeCutPos));
        LINFO(testlog, "Has cut result <" << notAwesomeCutName << ">? " << cutStatus.HasCutResult(notAwesomeCutName));
        LINFO(testlog, "Has cut result at " << notAwesomeCutPos << "? " << cutStatus.HasCutResult(notAwesomeCutPos));
        LINFO(testlog, "Cut state of <" << notAwesomeCutName << "> is: " << cutStatus.GetCutState(notAwesomeCutName));
        LINFO(testlog, "Is cut (all results)? " << cutStatus.IsCut());
        LINFO(testlog, "Is cut with mask \"00\"? " << cutStatus.IsCut("00"));
        LINFO(testlog, "Is cut with mask \"01\"? " << cutStatus.IsCut("01"));
        LINFO(testlog, "Is cut with mask \"10\"? " << cutStatus.IsCut("10"));
        LINFO(testlog, "Is cut with mask \"11\"? " << cutStatus.IsCut("11"));
        LINFO(testlog, "Is cut with mask 0? " << cutStatus.IsCut(0));
        LINFO(testlog, "Is cut with mask 1? " << cutStatus.IsCut(1));
        LINFO(testlog, "Is cut with mask 2? " << cutStatus.IsCut(2));
        LINFO(testlog, "Is cut with mask 3? " << cutStatus.IsCut(3));


    CutFilter cutFilter;

    LINFO(testlog, "Filtering with all cuts");
    cutFilter.SetCutMaskAll();
    LINFO(testlog, "Is cut (all results)? " << cutFilter.Filter(data));

    LINFO(testlog, "Testing filter set with a string");
    cutFilter.SetCutMask("00");
    LINFO(testlog, "Is cut with mask \"00\"? " << cutFilter.Filter(data));
    cutFilter.SetCutMask("01");
    LINFO(testlog, "Is cut with mask \"01\"? " << cutFilter.Filter(data));
    cutFilter.SetCutMask("10");
    LINFO(testlog, "Is cut with mask \"10\"? " << cutFilter.Filter(data));
    cutFilter.SetCutMask("11");
    LINFO(testlog, "Is cut with mask \"11\"? " << cutFilter.Filter(data));

    LINFO(testlog, "Testing filter set with an integer");
    cutFilter.SetCutMask(0);
    LINFO(testlog, "Is cut with mask 0? " << cutFilter.Filter(data));
    cutFilter.SetCutMask(1);
    LINFO(testlog, "Is cut with mask 1? " << cutFilter.Filter(data));
    cutFilter.SetCutMask(2);
    LINFO(testlog, "Is cut with mask 2? " << cutFilter.Filter(data));
    cutFilter.SetCutMask(3);
    LINFO(testlog, "Is cut with mask 3? " << cutFilter.Filter(data));

    return 0;
}
