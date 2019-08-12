/*
 * KTTestCuts.hh
 *
 *  Created on: Nov 2, 2016
 *      Author: obla999
 */

#ifndef NYMPH_KTTESTCUTS_HH_
#define NYMPH_KTTESTCUTS_HH_

#include "KTCut.hh"
#include "KTData.hh"
#include "KTMemberVariable.hh"


namespace Nymph
{

    class KTTestData : public KTExtensibleData< KTTestData >
    {
        public:
            KTTestData();
            virtual ~KTTestData();

            MEMBERVARIABLE(bool, IsAwesome);

        public:
            static const std::string sName;

    };

    // Cuts data that is NOT awesome
    class KTAwesomeCut : public KTCutOneArg< KTTestData >
    {
        public:
            struct Result : KTExtensibleCutResult< Result >
            {
                static const std::string sName;
            };

        public:
            KTAwesomeCut(const std::string& name = "default-awesome-cut");
            virtual ~KTAwesomeCut();

            bool Configure(const scarab::param_node& node);

            bool Apply(KTData& data, KTTestData& testData);
    };

    // Cuts data that is IS awesome
    class KTNotAwesomeCut : public KTCutOneArg< KTTestData >
    {
        public:
            struct Result : KTExtensibleCutResult< Result >
            {
                static const std::string sName;
            };

        public:
            KTNotAwesomeCut(const std::string& name = "default-not-awesome-cut");
            virtual ~KTNotAwesomeCut();

            bool Configure(const scarab::param_node& node);

            bool Apply(KTData& data, KTTestData& testData);
    };

}


#endif /* NYMPH_KTTESTCUTS_HH_ */
