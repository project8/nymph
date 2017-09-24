/*
 * KTTestCuts.hh
 *
 *  Created on: Nov 2, 2016
 *      Author: obla999
 */

#ifndef NYMPH_KTTESTCUTS_HH_
#define NYMPH_KTTESTCUTS_HH_

#include "KTCut.hh"
#include "KTMemberVariable.hh"
#include "KTCoreData.hh"


namespace Nymph
{
    class KTTestData;
    class KTTestDataExt;

    // Cuts data that is NOT awesome
    class KTAwesomeCut : public KTCutOnData< KTTestDataExt >
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

            bool Apply(KTCoreData& data, const KTTestData& testData);
    };

    // Cuts data that is IS awesome
    class KTNotAwesomeCut : public KTCutOnData< KTTestDataExt >
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

            bool Apply(KTCoreData& data, const KTTestData& testData);
    };

}


#endif /* NYMPH_KTTESTCUTS_HH_ */
