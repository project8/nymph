/*
 *  KTSerialWriter.cc
 *
 *  Created on: Feb 1, 2018
 *      Author: E Zayas
 *
*/

#include "KTSerialWriter.hh"

#include "KTLogger.hh"

LOGGER( avlog_hh, "KTSerialWriter" );

namespace Nymph
{
    KTSerialWriter::KTSerialWriter( const std::string& name ) :
        KTProcessor( name ),
        fFileName( "placeholder.json" ),
        fStreamOutPtr( nullptr ),
        fArchiveOutPtr( nullptr )
        {
            // Register slots here
            RegisterSlot( "test-data", this, &KTSerialWriter::SlotFunction< KTTestData > );
        }

    KTSerialWriter::~KTSerialWriter()
    {
        //delete fStreamOutPtr;
        //delete fArchiveOutPtr;
    }

    bool KTSerialWriter::Configure( const scarab::param_node& node )
    {
        SetFileName( node.get_value< std::string >( "file-name", fFileName ) );
        Initialize();

        return true;
    }

    void KTSerialWriter::Initialize()
    {
        *fStreamOutPtr = std::ofstream( fFileName );
        cereal::JSONOutputArchive fStupid( *fStreamOutPtr );
        fArchiveOutPtr = &fStupid;

        return;
    }

    template< class XDataType >
    void KTSerialWriter::SlotFunction( XDataType& data )
    {
        // Write to JSON archive
        KTINFO( avlog_hh, "Writing data to JSON acrhive" );
        (*fArchiveOutPtr)( data );

        KTINFO( avlog_hh, "Successfully wrote data to archive" );
        return;
    }
}
