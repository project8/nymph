/*
 *  KTJSONWriter.cc
 *
 *  Created on: Feb 1, 2018
 *      Author: N.S. Oblath
 *
*/

#include "KTJSONWriter.hh"

#include "KTLogger.hh"


namespace Nymph
{
    KT_REGISTER_PROCESSOR( KTJSONWriter, "json-writer" );

    KTJSONWriter::KTJSONWriter( const std::string& name ) :
        KTWriterWithTypists< KTJSONWriter, KTJSONTypeWriter >( name ),
        fFilename( "json_writer_default_file.json" ),
        fStreamOutPtr( nullptr ),
        fArchiveOutPtr( nullptr ),
        fExtDataSlot( "ext-data", this, &KTJSONWriter::WriteExtData ),
        fCoreDataSlot( "core", this, &KTJSONWriter::WriteData< KTCoreDataExt > )
        {
        }

    KTJSONWriter::~KTJSONWriter()
    {
        // archive must be delete before the stream!
        delete fArchiveOutPtr;
        delete fStreamOutPtr;
    }

    bool KTJSONWriter::Configure( const scarab::param_node& node )
    {
        SetFilename( node.get_value( "filename", fFilename ) );

        return true;
    }

    void KTJSONWriter::WriteExtData( KTDataHandle handle )
    {
        if( fStreamOutPtr == nullptr )
        {
            fStreamOutPtr = new std::ofstream( fFilename );
            fArchiveOutPtr = new cereal::JSONOutputArchive( *fStreamOutPtr );
        }

        // Write to JSON archive
        KTDEBUG( avlog_hh, "Writing extensible data to JSON archive" );
        (*fArchiveOutPtr)( handle );

        return;
    }

}
