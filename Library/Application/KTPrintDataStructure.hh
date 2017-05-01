/**
 @file KTPrintDataStructure.hh
 @brief Contains KTPrintDataStructure
 @details Prints the structure of data objects
 @author: N.S. Oblath
 @date: Sept 12, 2014
 */

#ifndef KTPRINTDATASTRUCTURE_HH_
#define KTPRINTDATASTRUCTURE_HH_

#include "KTProcessor.hh"

#include "KTSlot.hh"


namespace Nymph
{

    /*!
     @class KTPrintDataStructure
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
     - "print-data": void (KTDataPtr) -- Prints the structure of the data object; Does not modify the data or cuts; Emits signal "data"
     - "print-cuts": void (KTDataPtr) -- Prints the structure of the data's cuts; Does not modify the data or cuts; Emits signal "data"
     - "print-data-and-cuts": void (KTDataPtr) -- Prints the structure of the data object and its cuts; Does not modify the data or cuts; Emits signal "data"

     Signals:
     - "data": void (KTDataPtr) -- Emitted after structure information is printed
    */

    class KTPrintDataStructure : public KTProcessor
    {
        public:
            KTPrintDataStructure(const std::string& name = "print-data-structure");
            virtual ~KTPrintDataStructure();

            bool Configure(const scarab::param_node* node);

        public:
            void PrintDataStructure(KTDataPtr dataPtr, KTDataPtrReturn& ret);
            void PrintCutStructure(KTDataPtr dataPtr, KTDataPtrReturn& ret);
            void PrintDataAndCutStructure(KTDataPtr dataPtr, KTDataPtrReturn& ret);

        private:
            void DoPrintDataStructure(KTDataPtr dataPtr, KTDataPtrReturn& ret);
            void DoPrintCutStructure(KTDataPtr dataPtr, KTDataPtrReturn& ret);

            //***************
            // Signals
            //***************

        private:
            KTSignalData fDataSignal;

            //***************
            // Slots
            //***************

        private:
            KTSlot< void, KTDataPtr, KTDataPtrReturn& > fDataStructSlot;
            KTSlot< void, KTDataPtr, KTDataPtrReturn& > fCutStructSlot;
            KTSlot< void, KTDataPtr, KTDataPtrReturn& > fDataAndCutStructSlot;

    };
}
 /* namespace Nymph */
#endif /* KTPRINTDATASTRUCTURE_HH_ */
