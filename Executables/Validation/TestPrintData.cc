/*
 * TestPrintData.cc
 *
 *  Created on: Oct 07, 2014
 *      Author: nsoblath
 */

#include "KTCut.hh"
#include "KTLogger.hh"
#include "KTPrintDataStructure.hh"
#include "KTTestData.hh"

#include "param.hh"

namespace Nymph
{
    KTLOGGER(testlog, "TestPrintData");

    // Cuts data that is NOT awesome
    class KTAwesomeCut : public KTCut
    {
        public:
            struct Result : KTExtensibleCutResult< Result >
            {
                static const std::string sName;
            };

        public:
            KTAwesomeCut(const std::string& name = "default-awesome-cut") :
                KTCut(name)
            {}
            ~KTAwesomeCut() {}

            bool Configure(const scarab::param_node* node)
            {return true;}

            bool Apply(KTCoreData& data, KTTestData& testData)
            {
                bool isCut = ! testData.GetIsAwesome();
                data.CutStatus().AddCutResult< KTAwesomeCut::Result >(isCut);
                return isCut;
            }

            bool Apply(KTDataHandle dataHandle)
            {
                if (! dataHandle->Has< KTTestDataExt >())
                {
                    KTERROR(testlog, "Data type <KTTestData> was not present");
                    return false;
                }
                return Apply(dataHandle->Of< KTCoreData >(), dataHandle->Of< KTTestDataExt >());
            }
    };

    // Cuts data that is IS awesome
    class KTNotAwesomeCut : public KTCut
    {
        public:
            struct Result : KTExtensibleCutResult< Result >
            {
                static const std::string sName;
            };

        public:
            KTNotAwesomeCut(const std::string& name = "default-not-awesome-cut") :
                KTCut(name)
            {}
            ~KTNotAwesomeCut() {}

            bool Configure(const scarab::param_node* node)
            {return true;}

            bool Apply(KTCoreData& data, KTTestData& testData)
            {
                bool isCut = testData.GetIsAwesome();
                // use the name-based AddCutResult
                data.CutStatus().AddCutResult("not-awesome-cut", isCut);
                return isCut;
            }

            bool Apply(KTDataHandle dataHandle)
            {
                if (! dataHandle->Has< KTTestData >())
                {
                    KTERROR(testlog, "Data type <KTTestData> was not present");
                    return false;
                }
                return Apply(dataHandle->Of< KTCoreData >(), dataHandle->Of< KTTestData >());
            }
    };


    const std::string KTAwesomeCut::Result::sName = "awesome-cut";
    const std::string KTNotAwesomeCut::Result::sName = "not-awesome-cut";

    KT_REGISTER_CUT(KTAwesomeCut);
    KT_REGISTER_CUT(KTNotAwesomeCut);
}


using namespace Nymph;
using namespace std;

int main()
{
    KTDataHandle dataHandle(new KTCoreData());
    KTCoreDataExt& data = dataHandle->Of< KTCoreDataExt >();
    KTTestDataExt& testData = dataHandle->Of< KTTestDataExt >();

    KTINFO(testlog, "Applying awesome cut");
    KTAwesomeCut cut;
    cut.Apply(data, testData);

    KTINFO(testlog, "Applying not-awesome cut");
    KTNotAwesomeCut naCut;
    naCut.Apply(data, testData);

    KTPrintDataStructure printer;

    KTINFO(testlog, "Printing data structure");
    printer.PrintDataStructure(dataHandle);

    KTINFO(testlog, "Printing cut structure");
    printer.PrintCutStructure(dataHandle);

    return 0;
}
