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
         SetApplyFunc( this, &AwesomeCut::Apply );
    }

    AwesomeCut::~AwesomeCut()
    {}

    void AwesomeCut::Configure(const scarab::param_node&)
    {
//        return true;
    }

    bool AwesomeCut::Apply(ExtCoreData& data, const TestDataExt& testData)
    {
        bool isCut = ! testData.GetIsAwesome();
        LDEBUG(testlog, "Is data awesome? " << testData.GetIsAwesome());
        LDEBUG(testlog, "Is data cut? " << isCut);
//        data.GetCutStatus().SetCutState(fConfigName, isCut);
	std::cout << fCutName << " " << isCut << std::endl;
	std::cout << data.GetCutStatus().size() << " is the size of the cut" << std::endl;
	std::cout << data.GetCutStatus().HasCutResult(fCutName) << "does the result exist?" << std::endl;
        data.GetCutStatus().SetCutState(fCutName, isCut);
	LDEBUG(testlog, "Leaving AwesomeCut::Apply");
        return isCut;
    }


    NotAwesomeCut::NotAwesomeCut(const std::string& name) :
            CutOnData(name)
    {
        SetApplyFunc( this, &NotAwesomeCut::Apply );
    }

    NotAwesomeCut::~NotAwesomeCut()
    {}

    void NotAwesomeCut::Configure(const scarab::param_node&)
    {
//        return true;
    }

    bool NotAwesomeCut::Apply(ExtCoreData& data, const TestDataExt& testData)
    {
        bool isCut = testData.GetIsAwesome();
        LDEBUG(testlog, "Is data awesome? " << testData.GetIsAwesome());
        LDEBUG(testlog, "Is data cut? " << isCut);
//         data.GetCutStatus().SetCutState(fConfigName, isCut);
        data.GetCutStatus().SetCutState(fCutName, isCut);
	LDEBUG(testlog, "Leaving NotAwesomeCut::Apply");
        return isCut;
    }

}

