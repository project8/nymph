/**
 @file ApplyCut.hh
 @brief Contains ApplyCut
 @details Applies a cut to data
 @author: N. S. Oblath
 @date: Oct 6, 2014
 */

#ifndef APPLYCUT_HH_
#define APPLYCUT_HH_

#include "Signal.hh"
#include "Slot.hh"
#include "SignalData.hh"
#include "Processor.hh"
#include "DataFrame.hh"

#include "Logger.hh"
#include "MemberVariable.hh"
#include "Slot.hh"

#include <string>

namespace Nymph
{
    class Cut;

    /*!
     @class ApplyCut
     @author N. S. Oblath

     @brief Applies a cut to data.

     @details
     ApplyCut applies a cut to data -- the type of cut and its parameters are specified at runtime
     from the set of cuts registered.

     Only one cut can be applied by a given instance of ApplyCut.  To apply more than one cut, add another instance.

     Interpretation of boolean returned by Cut::Apply
     - TRUE means the cut was failed
     - FALSE means the cut was passed

     Configuration name: "apply-cut"

     Available configuration values:
     - "[cut name]": subtree -- specifies the cut to be used; parent node for the cut configuration

     Slots:
     - "apply": void (DataHandle) -- Applies the cut to the received data; Requirements are set by the cut; No data is added.

     Signals:
     - "all": void (DataHandle) -- Emitted upon application of the cut regardless of cut result.
     - "pass": void (DataHandle) -- Emitted upon application of the cut if the cut passed.
     - "fail": void (DataHandle) -- Emitted upon application of the cut if the cut failed.
    */

//    template< typename... XArgs >
    class ApplyCut : public Processor
    {
        public:
            ApplyCut(const std::string& name = "apply-cut");
            virtual ~ApplyCut();

            void Configure(const scarab::param_node& node);

            Cut* GetCut() const;
            void SetCut(Cut* cut);
            bool SelectCut(const std::string& cutName);

        private:
            Cut* fCut;

        public:
            void ApplyTheCut(DataHandle dataHandle);

            //***************
            // Signals
            //***************

        private:
            SignalData fAfterCutSignal;
            SignalData fAfterCutPassSignal;
            SignalData fAfterCutFailSignal;

            //***************
            // Slots
            //***************

        private:

    };


    inline Cut* ApplyCut::GetCut() const
    {
        return fCut;
    }

} /* namespace Nymph */
#endif /* APPLYCUT_HH_ */
