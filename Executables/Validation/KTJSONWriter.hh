/*
 *  KTJSONWriter.hh
 *
 *  Created on: Feb 1, 2018
 *      Author: E Zayas
 *
*/

#ifndef KTJSONWRITER_HH_
#define KTJSONWRITER_HH_

#include "KTData.hh"
#include "KTProcessor.hh"
#include "KTSlot.hh"

#include "KTTestData.hh"

#include "KTLogger.hh"

#include "cereal/archives/json.hpp"

#include <fstream>

KTLOGGER( avlog_hh, "KTJSONWriter" );

namespace Nymph
{
    class KTJSONWriter : public KTProcessor
    {
        public:
            KTJSONWriter( const std::string& name = "serial-writer" );
            virtual ~KTJSONWriter();

            bool Configure( const scarab::param_node& node );

            MEMBERVARIABLE( std::string, Filename );

        public:
            template< class XDataType >
            void WriteData( XDataType& data );

        private:
            std::ofstream* fStreamOutPtr;
            cereal::JSONOutputArchive* fArchiveOutPtr;

    };

    template< class XDataType >
    void KTJSONWriter::WriteData( XDataType& data )
    {
        if( fStreamOutPtr == nullptr )
        {
            fStreamOutPtr = new std::ofstream( fFilename );
            fArchiveOutPtr = new cereal::JSONOutputArchive( *fStreamOutPtr );
        }

        // Write to JSON archive
        KTINFO( avlog_hh, "Writing data to JSON acrhive" );
        (*fArchiveOutPtr)( data );

        KTINFO( avlog_hh, "Successfully wrote data to archive" );
        return;
    }

} // namespace Nymph

#endif // KTJSONWRITER_HH_
