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
        fStreamOutPtr(nullptr),
        fArchiveOutPtr(nullptr)
        {
            // Register slots here
        }

    KTSerialWriter::~KTSerialWriter()
    {
        //delete fStreamOutPtr;
        //delete fArchiveOutPtr;
    }

    bool KTSerialWriter::Configure( const scarab::param_node* node )
    {
        if (node == NULL) return false;

        SetFileName( node->get_value< std::string >( "file-name", fFileName ) );

        *fStreamOutPtr = std::ofstream( fFileName );
        cereal::JSONOutputArchive fStupid( *fStreamOutPtr );
        fArchiveOutPtr = &fStupid;

        return true;
    }

    template< class XDataType >
    void KTSerialWriter::SlotFunction( const XDataType& data )
    {
        // Write to JSON archive
        KTINFO( avlog_hh, "Writing data to JSON acrhive" );
        *fArchiveOutPtr( data );

        KTINFO( avlog_hh, "Successfully wrote data to archive" );
        return;
    }
}
