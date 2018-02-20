/*
 *  KTJSONWriter.hh
 *
 *  Created on: Feb 1, 2018
 *      Author: N.S. Oblath
 *
*/

#ifndef NYMPH_KTJSONWRITER_HH_
#define NYMPH_KTJSONWRITER_HH_

// included first so that the archive is registered before the data classes
#include "cereal/archives/json.hpp"
// then register the nymph extensible data classes
#include "KTRegisterNymphExtData.hh"


#include "KTWriter.hh"
#include "KTSlot.hh"

#include "KTLogger.hh"

#include <fstream>

KTLOGGER( avlog_hh, "KTJSONWriter" );

namespace Nymph
{
    class KTJSONWriter;

    typedef KTDerivedTypeWriter< KTJSONWriter > KTJSONTypeWriter;

    class KTJSONWriter : public KTWriterWithTypists< KTJSONWriter, KTJSONTypeWriter >
    {
        public:
            KTJSONWriter( const std::string& name = "json-writer" );
            virtual ~KTJSONWriter();

            bool Configure( const scarab::param_node& node );

            MEMBERVARIABLE( std::string, Filename );

        public:
            template< class XDataType >
            void WriteData( const XDataType& data );

        private:
            std::ofstream* fStreamOutPtr;
            cereal::JSONOutputArchive* fArchiveOutPtr;

        private:
            KTSlotData< void, KTCoreDataExt > fCoreDataSlot;
    };

    template< class XDataType >
    void KTJSONWriter::WriteData( const XDataType& data )
    {
        if( fStreamOutPtr == nullptr )
        {
            fStreamOutPtr = new std::ofstream( fFilename );
            fArchiveOutPtr = new cereal::JSONOutputArchive( *fStreamOutPtr );
        }

        // Write to JSON archive
        KTINFO( avlog_hh, "Writing data to JSON archive" );
        (*fArchiveOutPtr)( data );

        KTINFO( avlog_hh, "Successfully wrote data to archive" );
        return;
    }

} // namespace Nymph

#endif // NYMPH_KTJSONWRITER_HH_
