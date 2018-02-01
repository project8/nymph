/*
 *  KTJSONWriter.cc
 *
 *  Created on: Feb 1, 2018
 *      Author: E Zayas
 *
*/

#include "KTJSONWriter.hh"
#include "KTLogger.hh"


namespace Nymph
{
    KTJSONWriter::KTJSONWriter( const std::string& name ) :
        KTProcessor( name ),
        fFilename( "json_writer_default_file.json" ),
        fStreamOutPtr( nullptr ),
        fArchiveOutPtr( nullptr )
        {
            // Register slots here
            RegisterSlot( "test-data", this, &KTJSONWriter::WriteData< KTTestData > );
        }

    KTJSONWriter::~KTJSONWriter()
    {
        delete fStreamOutPtr;
        delete fArchiveOutPtr;
    }

    bool KTJSONWriter::Configure( const scarab::param_node& node )
    {
        SetFilename( node.get_value( "file-name", fFilename ) );

        return true;
    }

}
