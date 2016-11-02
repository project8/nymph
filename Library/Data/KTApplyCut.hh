/**
 @file KTApplyCut.hh
 @brief Contains KTApplyCut
 @details Applies a cut to data
 @author: N. S. Oblath
 @date: Oct 6, 2014
 */

#ifndef KTAPPLYCUT_HH_
#define KTAPPLYCUT_HH_

#include "KTProcessor.hh"

#include "KTLogger.hh"
#include "KTMemberVariable.hh"
#include "KTSlot.hh"

#include <string>

namespace Nymph
{
    class KTCut;

    /*!
     @class KTApplyCut
     @author N. S. Oblath

     @brief Applies a cut to data.

     @details
     KTApplyCut applies a cut to data -- the type of cut and its parameters are specified at runtime
     from the set of cuts registered.

     Only one cut can be applied by a given instance of KTApplyCut.  To apply more than one cut, add another instance.

     Configuration name: "apply-cut"

     Available configuration values:
     - "[cut name]": subtree -- specifies the cut to be used; parent node for the cut configuration

     Slots:
     - "apply-cut": void (KTDataPtr) -- Applies the cut to the received data; Requirements are set by the cut; No data is added.

     Signals:
     - "after-cut": void (KTDataPtr) -- Emitted upon application of the cut regardless of cut result.
     - "after-cut-pass": void (KTDataPtr) -- Emitted upon application of the cut if the cut passed.
     - "after-cut-fail": void (KTDataPtr) -- Emitted upon application of the cut if the cut failed.
    */

    class KTApplyCut : public KTProcessor
    {
        public:
            KTApplyCut(const std::string& name = "apply-cut");
            virtual ~KTApplyCut();

            bool Configure(const scarab::param_node* node);

            KTCut* GetCut() const;
            void SetCut(KTCut* cut);
            bool SelectCut(const std::string& cutName);

        private:
            KTCut* fCut;

        public:
            void ApplyCut(KTDataPtr);


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


    inline KTCut* KTApplyCut::GetCut() const
    {
        return fCut;
    }

} /* namespace Nymph */
#endif /* KTAPPLYCUT_HH_ */
