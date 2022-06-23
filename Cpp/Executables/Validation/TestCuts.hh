/*
 * TestCuts.hh
 *
 *  Created on: Nov 2, 2016
 *      Author: obla999
 */

#ifndef NYMPH_TESTCUTS_HH_
#define NYMPH_TESTCUTS_HH_

#include "Cut.hh"
#include "MemberVariable.hh"
#include "CoreData.hh"
#include "param.hh"

namespace Nymph
{
    class TestData;
//    class TestDataExt;

    // Cuts data that is NOT awesome
//    class AwesomeCut : public CutOnData< TestDataExt >
     class AwesomeCut : public CutOnData< TestData >
    {
        public:
            AwesomeCut(const std::string& name = "default-awesome-cut");
            virtual ~AwesomeCut();

            void Configure(const scarab::param_node& node);

            bool Apply(ExtCoreData& data, const TestData& testData);
    };

    // Cuts data that is IS awesome
//    class NotAwesomeCut : public CutOnData< TestDataExt >
    class NotAwesomeCut : public CutOnData< TestData >
    {
        public:
            NotAwesomeCut(const std::string& name = "default-not-awesome-cut");
            virtual ~NotAwesomeCut();

            void Configure(const scarab::param_node& node);

            bool Apply(ExtCoreData& data, const TestData& testData);
    };

}


#endif /* NYMPH_KTTESTCUTS_HH_ */
