/*
 * KTTestDataPrimaryProcessor.cc
 *
 *  Created on: Feb 17, 2018
 *      Author: N.S. Oblath
 */

#include "KTTestDataPrimaryProcessor.hh"

#include "KTTestData.hh"

namespace Nymph
{

    KT_REGISTER_PROCESSOR(KTTestDataPrimaryProcessor, "test-data-p-proc");

    KTTestDataPrimaryProcessor::KTTestDataPrimaryProcessor( const std::string& name ) :
            KTPrimaryProcessor( {"test-derived-1"}, name ),
            fIterations(10),
            fTheSignal("test-derived-1", this)
    {
    }

    KTTestDataPrimaryProcessor::~KTTestDataPrimaryProcessor()
    {
    }

    bool KTTestDataPrimaryProcessor::Configure( const scarab::param_node& node )
    {
        SetIterations( node.get_value( "iterations", GetIterations() ) );

        return true;
    }

    bool KTTestDataPrimaryProcessor::Run()
    {
        for( unsigned iIteration = 0; iIteration < fIterations; ++iIteration )
        {
            // e.g. for a real processor, do some work here instead of sleeping
            boost::this_thread::sleep_for( boost::chrono::milliseconds(1) );

            if( fThreadRef && fThreadRef->GetCanceled() ) break;

            KTDataHandle handle = CreateNewDataHandle();
            KTTestDerived1DataExt& testData = handle->Of< KTTestDerived1DataExt >();
            testData.SetFunniness( 867 );

            fTheSignal( handle );
        }
        return true;
    }

} /* namespace Nymph */
