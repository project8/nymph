/*
 * TestProcessorClasses.hh
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_TESTING_PROCESSORCLASSES_HH_
#define NYMPH_TESTING_PROCESSORCLASSES_HH_

#include "PrimaryProcessor.hh"
#include "Processor.hh"
#include "QuitChain.hh"
#include "Signal.hh"
#include "Slot.hh"

#include "logger.hh"

#include <chrono>
#include <thread>

LOGGER( testprocclasses_h_log, "TestProcessorClasses" );

namespace NymphTesting
{
    // concrete processor class that we can test
    // implements Configure() and has its own signal and slot
    class TestProc : public Nymph::Processor
    {
        public:
            TestProc( const std::string& name = "test" );

            virtual ~TestProc();

            void Configure( const scarab::param_node& node );

            MEMVAR( int, Value );
            MEMVAR( int, SecondValue );
            MEMVAR_REF( std::string, StringValue );

            void SecondValueSlotFunction( int newValue );

            MEMVAR_REF( Nymph::Signal< int >, ValueSig );
            MEMVAR_REF( Nymph::Slot< int >, ValueSlot );

            MEMVAR_REF( Nymph::Signal< int >, SecondValueSig );
            MEMVAR_REF( Nymph::Slot< int >, SecondValueSlot );
            MEMVAR_REF( Nymph::Slot< int >, SecondValueSlot2 );

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
    class TestPrimaryProc : public Nymph::PrimaryProcessor
    {
        public:
            enum class TestType
            {
                SignalNewValue,
                WaitTwoSec,
                ThrowExcept,
                QuitChain
            };

            TestPrimaryProc( const std::string& name = "test" );

            virtual ~TestPrimaryProc();

            void Configure( const scarab::param_node& node );

            void Run();

            void TestSignalNewValue();

            void TestWaitTwoSec();

            void TestThrowExcept();

            void TestQuitChain();

            MEMVAR( TestType, TestSelection );

            MEMVAR( int, NewValue );
            MEMVAR( int, Value );

            MEMVAR_REF( Nymph::Signal< int >, ValueSig );
            MEMVAR_REF( Nymph::Slot< int >, ValueSlot );

    };

} /* namespace NymphTesting */

#endif
