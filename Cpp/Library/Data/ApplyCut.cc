/*
 * ApplyCut.cc
 *
 *  Created on: Oct 06, 2014
 *      Author: nsoblath
 */

#include "ApplyCut.hh"

#include "Cut.hh"

using std::string;


namespace Nymph
{
    LOGGER(cutlog, "ApplyCut");

    REGISTER_PROCESSOR(ApplyCut, "apply-cut");

    ApplyCut::ApplyCut(const std::string& name) :
            Processor(name),
            fCut(NULL),
            fAfterCutSignal("all", this),
            fAfterCutPassSignal("pass", this),
            fAfterCutFailSignal("fail", this)
    {
	    Slot CutSlot("apply", this, &ApplyCut::ApplyTheCut );
            RegisterSlot("apply", &CutSlot  );
    }

    ApplyCut::~ApplyCut()
    {
        delete fCut;
    }

    void ApplyCut::Configure(const scarab::param_node& node)
    {
        // Config-file settings

        for (scarab::param_node::const_iterator nodeIt = node.begin(); nodeIt != node.end(); ++nodeIt)
        {
            // first do configuration values we know about
            // as it happens, there aren't any

            // any remaining should be cut names
            // ignore any that don't work
            if (SelectCut(nodeIt.name()))
            {
                if (nodeIt->is_node())
                {
                    //fCut->Configure(nodeIt->as_node());
                }
                continue;
            }
        }

        if (fCut == NULL)
        {
            LERROR(cutlog, "No cut was selected");
        }
    }

    void ApplyCut::SetCut(Cut* cut)
    {
        delete fCut;
        fCut = cut;
        return;
    }

    bool ApplyCut::SelectCut(const string& cutName)
    {
        Cut* tempCut = scarab::factory< Cut, const std::string& >::get_instance()->create(cutName, cutName);
        if (tempCut == NULL)
        {
            LERROR(cutlog, "Invalid cut name given: <" << cutName << ">.");
            return false;
        }
        SetCut(tempCut);
        return true;
    }

    void ApplyCut::ApplyTheCut(DataHandle dataHandle)
    {
	LINFO(cutlog, "Entered ApplyTheCut");
        if (fCut == NULL)
        {
            THROW_EXCEPT_HERE(Exception() << "No cut was specified" );
            return;
        }

        LINFO(cutlog, "About to apply cut on dataHandle");
        bool cutFailed = fCut->Apply(dataHandle);
	LINFO(cutlog, "bool from Apply: " << cutFailed);
        if (cutFailed)
        {
            fAfterCutFailSignal(dataHandle);
        }
        else
        {
            fAfterCutPassSignal(dataHandle);
        }
        fAfterCutSignal(dataHandle);
        return;
    }



} /* namespace Nymph */
