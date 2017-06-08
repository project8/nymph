/*
 * KTApplyCut.cc
 *
 *  Created on: Oct 06, 2014
 *      Author: nsoblath
 */

#include "KTApplyCut.hh"

#include "KTCut.hh"

using std::string;


namespace Nymph
{
    KTLOGGER(cutlog, "KTApplyCut");

    KT_REGISTER_PROCESSOR(KTApplyCut, "apply-cut");

    KTApplyCut::KTApplyCut(const std::string& name) :
            KTProcessor(name),
            fCut(NULL),
            fAfterCutSignal("all", this),
            fAfterCutPassSignal("pass", this),
            fAfterCutFailSignal("fail", this)
    {
        fApplyCutSW = RegisterSlot("apply", this, &KTApplyCut::ApplyCut, {"all", "pass", "fail"});
    }

    KTApplyCut::~KTApplyCut()
    {
        delete fCut;
    }

    bool KTApplyCut::Configure(const scarab::param_node* node)
    {
        // Config-file settings
        if (node == NULL) return false;

        for (scarab::param_node::const_iterator nodeIt = node->begin(); nodeIt != node->end(); ++nodeIt)
        {
            // first do configuration values we know about
            // as it happens, there aren't any

            // any remaining should be cut names
            // ignore any that don't work
            if (SelectCut(nodeIt.name()))
            {
                if (nodeIt->is_node())
                {
                    fCut->Configure(&nodeIt->as_node());
                }
                continue;
            }
        }

        if (fCut == NULL)
        {
            KTERROR(cutlog, "No cut was selected");
            return false;
        }
        return true;
    }

    void KTApplyCut::SetCut(KTCut* cut)
    {
        delete fCut;
        fCut = cut;
        return;
    }

    bool KTApplyCut::SelectCut(const string& cutName)
    {
        KTCut* tempCut = scarab::factory< KTCut, const std::string& >::get_instance()->create(cutName, cutName);
        if (tempCut == NULL)
        {
            KTERROR(cutlog, "Invalid cut name given: <" << cutName << ">.");
            return false;
        }
        SetCut(tempCut);
        return true;
    }


    void KTApplyCut::ApplyCut(KTDataHandle dataHandle)
    {
        std::shared_ptr< KTThreadReference > ref = fApplyCutSW->GetThreadRef();

        if (fCut == NULL)
        {
            THROW_THREADREF_EXCEPTION( ref, KTException() << "No cut was specified" );
            return;
        }

        bool cutFailed = fCut->Apply(dataHandle);

        ref->Break( dataHandle, fApplyCutSW->GetDoBreakpoint() );

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
