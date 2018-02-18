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
    KTJSONWriter::KTJSONWriter( const std::string& name ) :
        KTWriterWithTypists< KTJSONWriter, KTJSONWriterTypist >( name ),
        fFilename( "json_writer_default_file.json" ),
        fStreamOutPtr( nullptr ),
        fArchiveOutPtr( nullptr ),
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
        SetFilename( node.get_value( "file-name", fFilename ) );

        return true;
    }

}
