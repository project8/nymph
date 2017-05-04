/*
 * KTTestPrimaryProcessor.cc
 *
 *  Created on: May 4, 2017
 *      Author: N.S. Oblath
 */

#include "KTTestPrimaryProcessor.hh"

namespace Nymph
{

    KTTestPrimaryProcessor::KTTestPrimaryProcessor() :
            KTPrimaryProcessor( {"the-signal"} ),
            fIterations(10),
            fTheSignal("the-signal", this)
    {
    }

    KTTestPrimaryProcessor::~KTTestPrimaryProcessor()
    {
    }

    bool KTTestPrimaryProcessor::Configure( const scarab::param_node* node )
    {
        if( node == nullptr ) return true;

        SetIterations( node->get_value( "iterations", GetIterations() ) );

        return true;
    }

    bool KTTestPrimaryProcessor::Run()
    {
        for( unsigned iIteration = 0; iIteration < fIterations; ++iIteration )
        {
            // e.g. for a real processor, do some work here instead of sleeping
            std::this_thread::sleep_for( std::chrono::milliseconds(1) );

            if( fThreadRef.fThreadIndicator->fCanceled ) break;

            fTheSignal( iIteration );
        }
        return true;
    }

} /* namespace Nymph */
