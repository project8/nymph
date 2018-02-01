/*
 * KTJSONReader.cc
 *
 *  Created on: Feb 1, 2018
 *      Author: N.S. Oblath
 */

// Data classes need to be included before the archive header
#include "KTTestData.hh"

#include "KTJSONReader.hh"


namespace Nymph
{

    KTJSONReader::KTJSONReader( const std::string& name) :
            KTProcessor( name ),
            fFilename( "json_reader_default_file.json" ),
            fStreamInPtr( nullptr ),
            fArchiveInPtr( nullptr )
    {
    }

    KTJSONReader::~KTJSONReader()
    {
        delete fArchiveInPtr;
        delete fStreamInPtr;
    }

    bool KTJSONReader::Configure( const scarab::param_node& node )
    {
        SetFilename( node.get_value( "filename" , fFilename ));

        return true;
    }

} /* namespace Nymph */
