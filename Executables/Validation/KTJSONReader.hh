/*
 * KTJSONReader.hh
 *
 *  Created on: Feb 1, 2018
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_KTJSONREADER_HH_
#define NYMPH_KTJSONREADER_HH_

#include "KTProcessor.hh"

#include "KTLogger.hh"

#include "cereal/archives/json.hpp"


KTLOGGER( jsrlog_hh, "KTJSONReader" )


namespace Nymph
{

    class KTJSONReader : public KTProcessor
    {
        public:
            KTJSONReader();
            virtual ~KTJSONReader();

            bool Configure( const scarab::param_node& node );

            MEMBERVARIABLE( std::string, Filename );

        private:
            std::ifstream* fStreamInPtr;
            cereal::JSONInputArchive* fArchiveInPtr;

            template< class XDataType >
            void ReadData( XDataType& data );
    };

    template< class XDataType >
    void KTJSONReader::ReadData( XDataType& data )
    {
        if( fStreamInPtr == nullptr )
        {
            KTDEBUG( jsrlog_hh, "Creating stream and archive" );
            fStreamInPtr = new std::ifstream( fFilename );
            fArchiveInPtr = new cereal::JSONInputArchive( *fStreamInPtr );
        }

        *fArchiveInPtr( data );

        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_KTJSONREADER_HH_ */
