/*
 * TestCut.cc
 *
 *  Created on: Jan 6, 2024
 *      Author: btfoust
 */

#include "CutStatus.hh"

#include "catch.hpp"

TEST_CASE( "cut_present", "[cut]" )
{
    using namespace Nymph;
    //using namespace NymphTesting;

    CutStatus testCutStatus;
    testCutStatus.AssignCutResult("fTestCut", 1);

    // check fTestCut applied
    REQUIRE ( testCutStatus.size() == 1 );
    //REQUIRE( testCutStatus.FindCutResult("fTestCut") == 0 ); // This returns position of cut in map, not state
    // check fTestCut state
    REQUIRE( testCutStatus.GetCutState("fTestCut") == 1 );

    // check default is to assign cut status FALSE
    testCutStatus.AssignCutResult("fNotCut");
    REQUIRE ( testCutStatus.size() == 2 );
    REQUIRE ( testCutStatus.GetCutState("fNotCut") == 0 );

    // Test Readout
    UNSCOPED_INFO ( testCutStatus );
    //CHECK ( false ); // Force readout even if no fails

    // Check IsCut()
    REQUIRE ( testCutStatus.IsCut() == true );

    // Check Num Cuts (size but only for cuts that are true)
    REQUIRE ( testCutStatus.NumCuts() == 1 );

    // Change Cut State
    testCutStatus.SetCutState("fTestCut",0);
    REQUIRE ( testCutStatus.GetCutState("fTestCut") == 0);

    // Check throw if set/check state for non-existant cut
    REQUIRE_THROWS ( testCutStatus.SetCutState("fMissingCut",1) );
    REQUIRE_THROWS ( testCutStatus.GetCutState("fMissingCut") );

    // Check initialize from other cut status
    CutStatus testCutStatus2(testCutStatus);
    REQUIRE ( testCutStatus2.size() == 2 );
    REQUIRE ( testCutStatus2.GetCutState("fTestCut") == 0);

    // Check assignment operator
    testCutStatus2.SetCutState("fNotCut",1);
    testCutStatus2 = testCutStatus;
    REQUIRE ( testCutStatus2.GetCutState("fNotCut") == 0);

    // Remove cuts
    testCutStatus.RemoveCutResult("fTestCut");
    testCutStatus.RemoveCutResult("fNotCut");
    REQUIRE ( testCutStatus.size() == 0 );
    testCutStatus2.RemoveCutResult("fTestCut");
    testCutStatus2.RemoveCutResult("fNotCut");
    REQUIRE ( testCutStatus2.size() == 0 );
    
} 
