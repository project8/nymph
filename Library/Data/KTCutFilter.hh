/**
 @file KTApplyCut.hh
 @brief Contains KTApplyCut
 @details Applies a cut to data
 @author: N. S. Oblath
 @date: Oct 6, 2014
 */

#ifndef KTCUTFILTER_HH_
#define KTCUTFILTER_HH_

#include "KTProcessor.hh"

#include "KTCutStatus.hh"

#include "KTLogger.hh"
#include "KTMemberVariable.hh"
#include "KTSlot.hh"

#include <string>

namespace Nymph
{
    class KTCut;

    /*!
     @class KTCutFilter
     @author N. S. Oblath

     @brief Filters data based on cuts already applied and a filter mask.

     @details
     KTCutFilter checks the status of cuts that have already been applied to a data.  If the bitwise AND of the cut status with
     the configurable cut mask is non-zero, than the data fails the filter.

     Interpretation of the boolean returned by Filter(KTData&):
     - TRUE means the data failed the cut filter.
     - FALSE means the data passed the cut filter.

     Configuration name: "cut-filter"

     Available configuration values:
     - "cut-mask-all": bool -- If true, filter will use all cuts. If this is present and true, it overrules other mask configurations.
     - "cut-mask": string -- Set the cut mask with a string of 1's and 0's. The first character is the highest significant bit, and the last character is the least significant bit.
                             If present, it overrules cut-mask-int.
     - "cut-mask-int": unsigned int -- Set the cut mask with an unsigned integer's bit values.

     Slots:
     - "filter": void (KTDataPtr) -- Checks the cut status of the received data ANDed with the cut mask; No data is added.

     Signals:
     - "all": void (KTDataPtr) -- Emitted after cut status is checked for all data.
     - "pass": void (KTDataPtr) -- Emitted after cut status is checked if the cut filter passed.
     - "fail": void (KTDataPtr) -- Emitted after cut status is checked if the cut filter failed.
    */

    class KTCutFilter : public KTProcessor
    {
        public:
            KTCutFilter(const std::string& name = "cut-filter");
            virtual ~KTCutFilter();

            bool Configure(const scarab::param_node* node);

            void SetCutMask(KTCutStatus::bitset_type mask);
            void SetCutMask(unsigned long long mask);
            /// Set the mask with a string; String must consist of all 0's and 1's.
            void SetCutMask(const std::string& mask);

            /// Set the cut mask to use all cuts
            void SetCutMaskAll();

        private:
            KTCutStatus::bitset_type fCutMask;
            MEMBERVARIABLE_NOSET(unsigned long long, CutMaskInt);

            bool fConvertToBitset;
            bool fAllBits;

        public:
            bool Filter(KTData& data);

            void FilterData(KTDataPtr);


            //***************
            // Signals
            //***************

        private:
            KTSignalData fAfterCutSignal;
            KTSignalData fAfterCutPassSignal;
            KTSignalData fAfterCutFailSignal;

            //***************
            // Slots
            //***************

        private:

    };


    inline void KTCutFilter::SetCutMask(KTCutStatus::bitset_type mask)
    {
        fCutMask = mask;
        fConvertToBitset = false;
        fAllBits = false;
        return;
    }

    inline void KTCutFilter::SetCutMask(unsigned long long mask)
    {
        fCutMaskInt = mask;
        fConvertToBitset = true;
        fAllBits = false;
        return;
    }

    inline void KTCutFilter::SetCutMask(const std::string& mask)
    {
        fCutMask = KTCutStatus::bitset_type(mask);
        fConvertToBitset = false;
        fAllBits = false;
        return;
    }

    inline void KTCutFilter::SetCutMaskAll()
    {
        fCutMask.clear();
        fConvertToBitset = false;
        fAllBits = true;
        return;
    }

} /* namespace Nymph */
#endif /* KTCUTFILTER_HH_ */
