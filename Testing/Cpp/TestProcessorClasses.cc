/*
 * TestProcessorClasses.cc
 *
 *  Created on: Jun 14, 2022
 *      Author: N.S. Oblath
 */

#include "TestProcessorClasses.hh"

REGISTER_PROCESSOR( NymphTesting, TestProc, "test-proc" );
REGISTER_PROCESSOR( NymphTesting, TestPrimaryProc, "test-primary" );

namespace NymphTesting
{
    TestProc::TestProc( const std::string& name ) :
            Nymph::Processor( name ),
            fValue( 0 ),
            fSecondValue( 0 ),
            fValueSig( "value", this ),
            fValueSlot( "value", this, &TestProc::SetValue ),
            fSecondValueSig( "second-value", this ),
            fSecondValueSlot( "second-value", this, &TestProc::SecondValueSlotFunction ),
            fSecondValueSlot2( "second-value-2", this, &TestProc::SetSecondValue )
    {}

    TestProc::~TestProc()
    {}

    void TestProc::Configure( const scarab::param_node& node )
    {
        fValue = node.get_value( "value", fValue );

        // we'll test configuration error throwing by having a string with restricted values
        if( node.has("string") )
        {
            if( node["string"]().as_string() == "ok value 1" ) fStringValue = "ok 1";
            else if( node["string"]().as_string() == "ok value 2" ) fStringValue = "ok 2";
            else THROW_EXCEPT_HERE( Nymph::ConfigException( node ) << "Invalid choice for \"string\"" );
        }

        return;
    }

    void TestProc::SecondValueSlotFunction( int newValue )
    {
        fSecondValueSig( newValue );
    }


    TestPrimaryProc::TestPrimaryProc( const std::string& name ) :
            PrimaryProcessor( name ),
            fTestSelection( TestType::ThrowExcept ),
            fNewValue( 10 ),
            fValue( 0 ),
            fValueSig( "value", this ),
            fValueSlot( "value", this, &TestPrimaryProc::SetValue )
    {}

    TestPrimaryProc::~TestPrimaryProc()
    {}

    void TestPrimaryProc::Configure( const scarab::param_node& node )
    {
        if( node.has("test") )
        {
            std::string testSelectionStr( node["test"]().as_string() );
            if( testSelectionStr == "signal-new-value" ) fTestSelection = TestType::SignalNewValue;
            else if( testSelectionStr == "wait-two-sec" ) fTestSelection = TestType::WaitTwoSec;
            else if( testSelectionStr == "throw-except" ) fTestSelection = TestType::ThrowExcept;
            else
            {
                LERROR( testprocclasses_h_log, "Invalid test selection: " << testSelectionStr );
                THROW_EXCEPT_HERE( Nymph::ConfigException() << "Invalid test selection: " << testSelectionStr );
            }
        }
        return;
    }

    void TestPrimaryProc::Run()
    {
        switch( fTestSelection )
        {
            case TestType::SignalNewValue:
                TestSignalNewValue();
                return;
            case TestType::WaitTwoSec:
                TestWaitTwoSec();
                return;
            case TestType::ThrowExcept:
                TestThrowExcept();
                return;
            case TestType::QuitChain:
                TestQuitChain();
                return;
            default:
                LERROR( testprocclasses_h_log, "Invalid test choice" );
                THROW_EXCEPT_HERE( Nymph::Exception() << "Invalid test choice" );
        }
        return;
    }

    void TestPrimaryProc::TestSignalNewValue()
    {
        fValueSig( fNewValue );
        return;
    }

    void TestPrimaryProc::TestWaitTwoSec()
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }

    void TestPrimaryProc::TestThrowExcept()
    {
        THROW_EXCEPT_HERE( TestPPException() << "PrimaryProcessor test function: throw Exception" );
        return;
    }

    void TestPrimaryProc::TestQuitChain()
    {
        QUIT_CHAIN;
    }

} /* namespace NymphTesting */
