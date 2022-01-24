/*
 * TestProcessorClasses.hh
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#include "PrimaryProcessor.hh"
#include "Processor.hh"
#include "QuitChain.hh"
#include "Signal.hh"
#include "Slot.hh"

#include "logger.hh"

#include <chrono>
#include <thread>

LOGGER( testprocclasses_h_log, "TestProcessorClasses" );

namespace Nymph
{
    // concrete processor class that we can test
    // implements Configure() and has its own signal and slot
    class TestProc : public Processor
    {
        public:
            TestProc( const std::string& name = "test" ) :
                    Processor( name ),
                    fValue( 0 ),
                    fSecondValue( 0 ),
                    fValueSig( "value", this ),
                    fValueSlot( "value", this, &TestProc::SetValue ),
                    fSecondValueSig( "second-value", this ),
                    fSecondValueSlot( "second-value", this, &TestProc::SecondValueSlotFunction, {"second-value"} ),
                    fSecondValueSlot2( "second-value-2", this, &TestProc::SetSecondValue )
            {}

            virtual ~TestProc()
            {}

            void Configure( const scarab::param_node& node )
            {
                fValue = node.get_value( "value", fValue );

                // we'll test configuration error throwing by having a string with restricted values
                if( node.has("string") )
                {
                    if( node["string"]().as_string() == "ok value 1" ) fStringValue = "ok 1";
                    else if( node["string"]().as_string() == "ok value 2" ) fStringValue = "ok 2";
                    else THROW_EXCEPT_HERE( ConfigException( node ) << "Invalid choice for \"string\"" );
                }

                return;
            }

            MEMVAR( int, Value );
            MEMVAR( int, SecondValue );
            MEMVAR_REF( std::string, StringValue );

            void SecondValueSlotFunction( int newValue )
            {
                fSecondValueSig( newValue );
            }

            MEMVAR_REF( Signal< int >, ValueSig );
            MEMVAR_REF( Slot< int >, ValueSlot );

            MEMVAR_REF( Signal< int >, SecondValueSig );
            MEMVAR_REF( Slot< int >, SecondValueSlot );
            MEMVAR_REF( Slot< int >, SecondValueSlot2 );

    };

    // external slot function owner
    struct TestSlotOwner
    {
        int fValue = 0;
        void TestSlotFunc( int input )
        {
            fValue = input;
            return;
        }
    };


    class TestPPException : public scarab::typed_exception< TestPPException >
    {
        public:
            using scarab::typed_exception< TestPPException >::typed_exception;
            ~TestPPException() = default;
    };


    // concrete primary processor class that we can test
    // implements Configure(), Run(), and has its own signal and slot
    class TestPrimaryProc : public PrimaryProcessor
    {
        public:
            enum class TestType
            {
                SignalNewValue,
                WaitTwoSec,
                ThrowExcept,
                QuitChain
            };

            TestPrimaryProc( const std::string& name = "test" ) :
                    PrimaryProcessor( name ),
                    fTestSelection( TestType::ThrowExcept ),
                    fNewValue( 10 ),
                    fValue( 0 ),
                    fValueSig( "value", this ),
                    fValueSlot( "value", this, &TestPrimaryProc::SetValue )
            {}

            virtual ~TestPrimaryProc()
            {}

            void Configure( const scarab::param_node& node )
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
                        THROW_EXCEPT_HERE( ConfigException() << "Invalid test selection: " << testSelectionStr );
                    }
                }
                return;
            }

            void Run()
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
                        THROW_EXCEPT_HERE( Exception() << "Invalid test choice" );
                }
                return;
            }

            void TestSignalNewValue()
            {
                fValueSig( fNewValue );
                return;
            }

            void TestWaitTwoSec()
            {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                return;
            }

            void TestThrowExcept()
            {
                THROW_EXCEPT_HERE( TestPPException() << "PrimaryProcessor test function: throw Exception" );
                return;
            }

            void TestQuitChain()
            {
                QUIT_CHAIN;
            }

            MEMVAR( TestType, TestSelection );

            MEMVAR( int, NewValue );
            MEMVAR( int, Value );

            MEMVAR_REF( Signal< int >, ValueSig );
            MEMVAR_REF( Slot< int >, ValueSlot );

    };

}
