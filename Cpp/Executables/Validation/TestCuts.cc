/*
 * TestCuts.cc
 *
 *  Created on: Nov 2, 2016
 *      Author: obla999
 */

#include "TestCuts.hh"

#include "Logger.hh"
#include "TestData.hh"

namespace Nymph
{
    LOGGER(testlog, "TestCuts");

    REGISTER_CUT(AwesomeCut, "awesome-cut");
    REGISTER_CUT(NotAwesomeCut, "not-awesome-cut"  );


    AwesomeCut::AwesomeCut(const std::string& name) :
            CutOnData(name)
    {
//        SetApplyFunc( this, &AwesomeCut::Apply );
    }

    AwesomeCut::~AwesomeCut()
    {}

    void AwesomeCut::Configure(const scarab::param_node&)
    {
//        return true;
    }

    bool AwesomeCut::Apply(Data& data, const TestData& testData)
    {
        bool isCut = ! testData.GetIsAwesome();
        LDEBUG(testlog, "Is data awesome? " << testData.GetIsAwesome());
        LDEBUG(testlog, "Is data cut? " << isCut);
//        data.GetCutStatus().SetCutState(fConfigName, isCut);
        return isCut;
    }


    NotAwesomeCut::NotAwesomeCut(const std::string& name) :
            CutOnData(name)
    {
//        SetApplyFunc( this, &NotAwesomeCut::Apply );
    }

    NotAwesomeCut::~NotAwesomeCut()
    {}

    void NotAwesomeCut::Configure(const scarab::param_node&)
    {
//        return true;
    }

    bool NotAwesomeCut::Apply(Data& data, const TestData& testData)
    {
        bool isCut = testData.GetIsAwesome();
        LDEBUG(testlog, "Is data awesome? " << testData.GetIsAwesome());
        LDEBUG(testlog, "Is data cut? " << isCut);
//        data.GetCutStatus().SetCutState(fConfigName, isCut);
        return isCut;
    }

}

