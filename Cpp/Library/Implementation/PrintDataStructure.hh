/**
 @file PrintDataStructure.hh
 @brief Contains PrintDataStructure
 @details Prints the structure of data objects
 @author: N.S. Oblath
 @date: Sept 12, 2014
 */

#ifndef PRINTDATASTRUCTURE_HH_
#define PRINTDATASTRUCTURE_HH_

#include "Processor.hh"

#include "Slot.hh"


namespace Nymph
{

    /*!
     @class PrintDataStructure
     @author N.S. Oblath

     @brief Prints the structure of data objects

     @details

     Configuration name: "print-data-structure"

     Example usage:
       "egg-processor:ts" --> "simple-fft:ts"
       "simple-fft:fft-forward" --> "print-data-structure:print-data"
     will show that the data consists of data, slice-header, raw-time-series, time-series, and frequency-spectrum-polar

     Available configuration values: none

     Slots:
     - "print-data": void (DataHandle) -- Prints the structure of the data object; Does not modify the data or cuts; Emits signal "data"
     - "print-cuts": void (DataHandle) -- Prints the structure of the data's cuts; Does not modify the data or cuts; Emits signal "data"
     - "print-data-and-cuts": void (DataHandle) -- Prints the structure of the data object and its cuts; Does not modify the data or cuts; Emits signal "data"

     Signals:
     - "data": void (DataHandle) -- Emitted after structure information is printed
    */

    class PrintDataStructure : public Processor
    {
        public:
            PrintDataStructure(const std::string& name = "print-data-structure");
            virtual ~PrintDataStructure();

            void Configure(const scarab::param_node& node);

        public:
            void PrintDataStructure(DataHandle dataHandle);
            void PrintCutStructure(DataHandle dataHandle);
            void PrintDataAndCutStructure(DataHandle dataHandle);

        private:
            void DoPrintDataStructure(DataHandle dataHandle);
            void DoPrintCutStructure(DataHandle dataHandle);

            //***************
            // Signals
            //***************

        private:
            SignalData fDataSignal;

            //***************
            // Slots
            //***************

        private:
            Slot< DataHandle > fDataStructSlot;
            Slot< DataHandle > fCutStructSlot;
            Slot< DataHandle > fDataAndCutStructSlot;

    };
}
 /* namespace Nymph */
#endif /* PRINTDATASTRUCTURE_HH_ */
