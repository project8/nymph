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

    /*!
     @class KTJSONWriter
     @author N. S. Oblath

     @brief Writes data to a JSON file

     @details
     Can write single data objects or full extensible data objects.

     The Cereal serialization library is used for this writer.
     Data objects must have a serial or save function to be successfully serialized.

     The slots for this writer can be extended in other compilation units using the type-writer system.
     As an example, see Executables/Validation/KTJSONTypeWriterValidation and KTRegisterNymphValidationExtData.

     Data objects must be registered after the Cereal JSON archive is included, and before any serialization takes place.
     As an example when extending the writer, see KTRegisterNymphValidationExtData.

     Configuration name: "json-writer"

     Available configuration options:
     - "filename": string -- JSON filename to write to

     Signals: N/A

     Slots:
     - "ext-data": void (KTDataHandle) -- write a full extensible data object
     - "core" void (KTDataHandle) -- write a KTCoreData object; requires KTCoreData

   */
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

            void WriteExtData( KTDataHandle handle );

        private:
            std::ofstream* fStreamOutPtr;
            cereal::JSONOutputArchive* fArchiveOutPtr;

        private:
            KTSlot< KTDataHandle > fExtDataSlot;

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
        KTDEBUG( avlog_hh, "Writing data to JSON archive" );
        (*fArchiveOutPtr)( data );

        return;
    }

} // namespace Nymph

#endif // NYMPH_KTJSONWRITER_HH_
