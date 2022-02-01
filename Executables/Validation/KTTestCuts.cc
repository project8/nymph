/*
 * KTTestCuts.cc
 *
 *  Created on: Nov 2, 2016
 *      Author: obla999
 */

#include "KTTestCuts.hh"

#include "KTLogger.hh"
#include "KTTestData.hh"

namespace Nymph
{
    KTLOGGER(testlog, "KTTestCuts");

    KT_REGISTER_CUT(KTAwesomeCut, "awesome-cut");
    KT_REGISTER_CUT(KTNotAwesomeCut, "not-awesome-cut"  );


    KTAwesomeCut::KTAwesomeCut(const std::string& name) :
            KTCutOnData(name)
    {
        SetApplyFunc( this, &KTAwesomeCut::Apply );
    }

    KTAwesomeCut::~KTAwesomeCut()
    {}

    bool KTAwesomeCut::Configure(const scarab::param_node&)
    {
        return true;
    }

    bool KTAwesomeCut::Apply(KTCoreData& data, const KTTestData& testData)
    {
        bool isCut = ! testData.GetIsAwesome();
        KTDEBUG(testlog, "Is data awesome? " << testData.GetIsAwesome());
        KTDEBUG(testlog, "Is data cut? " << isCut);
        data.CutStatus().SetCutState(fConfigName, isCut);
        return isCut;
    }


    KTNotAwesomeCut::KTNotAwesomeCut(const std::string& name) :
            KTCutOnData(name)
    {
        SetApplyFunc( this, &KTNotAwesomeCut::Apply );
    }

    KTNotAwesomeCut::~KTNotAwesomeCut()
    {}

    bool KTNotAwesomeCut::Configure(const scarab::param_node&)
    {
        return true;
    }

    bool KTNotAwesomeCut::Apply(KTCoreData& data, const KTTestData& testData)
    {
        bool isCut = testData.GetIsAwesome();
        KTDEBUG(testlog, "Is data awesome? " << testData.GetIsAwesome());
        KTDEBUG(testlog, "Is data cut? " << isCut);
        data.CutStatus().SetCutState(fConfigName, isCut);
        return isCut;
    }

}

