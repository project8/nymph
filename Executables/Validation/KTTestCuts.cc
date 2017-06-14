/*
 * KTTestCuts.cc
 *
 *  Created on: Nov 2, 2016
 *      Author: obla999
 */

#include "KTTestCuts.hh"

#include "KTLogger.hh"
#include "KTTestData.hh"

BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTAwesomeCut::Result );
BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTNotAwesomeCut::Result );

namespace Nymph
{
    KTLOGGER(testlog, "KTTestCuts");

    const std::string KTAwesomeCut::Result::sName = "awesome-cut";
    const std::string KTNotAwesomeCut::Result::sName = "not-awesome-cut";

    KT_REGISTER_CUT(KTAwesomeCut);
    KT_REGISTER_CUT(KTNotAwesomeCut);


    KTAwesomeCut::KTAwesomeCut(const std::string& name) :
            KTCutOnData(name)
    {
        SetApplyFunc( this, &KTAwesomeCut::Apply );
    }

    KTAwesomeCut::~KTAwesomeCut()
    {}

    bool KTAwesomeCut::Configure(const scarab::param_node* node)
    {
        return true;
    }

    bool KTAwesomeCut::Apply(KTCoreData& data, const KTTestData& testData)
    {
        bool isCut = ! testData.GetIsAwesome();
        KTDEBUG(testlog, "Is data awesome? " << testData.GetIsAwesome());
        KTDEBUG(testlog, "Is data cut? " << isCut);
        data.CutStatus().AddCutResult< KTAwesomeCut::Result >(isCut);
        return isCut;
    }


    KTNotAwesomeCut::KTNotAwesomeCut(const std::string& name) :
            KTCutOnData(name)
    {
        SetApplyFunc( this, &KTNotAwesomeCut::Apply );
    }

    KTNotAwesomeCut::~KTNotAwesomeCut()
    {}

    bool KTNotAwesomeCut::Configure(const scarab::param_node* node)
    {
        return true;
    }

    bool KTNotAwesomeCut::Apply(KTCoreData& data, const KTTestData& testData)
    {
        bool isCut = testData.GetIsAwesome();
        KTDEBUG(testlog, "Is data awesome? " << testData.GetIsAwesome());
        KTDEBUG(testlog, "Is data cut? " << isCut);
        // use the name-based AddCutResult
        data.CutStatus().AddCutResult("not-awesome-cut", isCut);
        return isCut;
    }

}

