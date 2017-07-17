/*
 * TestSerialization.cc
 *
 *  Created on: Jun 12, 2017
 *      Author: obla999
 */

#include "KTLogger.hh"
#include "KTPrintDataStructure.hh"

#include "KTTestData.hh"

#include <boost/archive/polymorphic_text_oarchive.hpp>
#include <boost/archive/polymorphic_text_iarchive.hpp>

#include <fstream>

KTLOGGER(testlog, "TestSerialization");


using namespace Nymph;
using namespace std;

// both of these (BOOST_CLASS_EXPORT AND BOOST_CLASS_EXPORT_GUID) work without anything in the data class files
// these simply put in BOOST_CLASS_EXPORT_KEY and BOOST_CLASS_EXPORT_IMPLEMENT right here
// as noted here (http://www.boost.org/doc/libs/1_64_0/libs/serialization/doc/special.html#export) this export line has to be after the inclusion of the archive headers
//BOOST_CLASS_EXPORT_GUID( KTCoreDataExt, "core-data-ext")
//BOOST_CLASS_EXPORT_GUID( KTTestDataExt, "test-data-ext")
BOOST_CLASS_EXPORT( KTCoreDataExt)
BOOST_CLASS_EXPORT( KTTestDataExt)

// this works when BOOST_CLASS_EXPORT_KEY is used in the respective data class headers
// does not work if i put these macros into the data class source files
//BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTCoreDataExt );
//BOOST_CLASS_EXPORT_IMPLEMENT( Nymph::KTTestDataExt );

// so what's the benefit of using the latter?  it seems that i can't help but to have something in this file about the specific data type
// projecting forward to having a writer, i will still need to refer to each allowed data type in the writer

int main()
{
    KTDataHandle dataHandle = CreateNewDataHandle();
    KTCoreDataExt& coreDataExt = dataHandle->Of< KTCoreDataExt >();
    KTTestDataExt& testDataExt = dataHandle->Of< KTTestDataExt >();

    //KTCoreData* coreData = (KTCoreData*)&coreDataExt;
    //KTTestData* testData = (KTTestData*)&testDataExt;

    // change data values from their defaults
    coreDataExt.SetCounter( 5 );
    testDataExt.SetIsAwesome( true );

    KTPrintDataStructure printer;

    KTINFO(testlog, "Printing data structure -- before saving");
    printer.PrintDataStructure(dataHandle);

    KTINFO(testlog, "KTCoreData counter: " << coreDataExt.GetCounter());
    KTINFO(testlog, "KTTestData is-awesome: " << testDataExt.GetIsAwesome());

    // save data to archive
    {
        // create and open a character archive for output
        std::ofstream fileOut("test_serialization_ext_output.txt");

        boost::archive::polymorphic_text_oarchive oArchive(fileOut);
        boost::archive::polymorphic_oarchive & oa_interface = oArchive;
        // write data objects to archive
        KTDEBUG(testlog, "Archiving extensible data object");
        oa_interface << dataHandle;
        //oArchive << dataHandle;
        // archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its original state
    KTDataHandle newDataHandle;
    //KTCoreDataExt& newData = newDataHandle->Of< KTCoreDataExt >();
    //KTTestDataExt& newTestData = newDataHandle->Of< KTTestDataExt >();
    {
        // create and open an archive for input
        std::ifstream fileIn("test_serialization_ext_output.txt");

        boost::archive::polymorphic_text_iarchive iArchive(fileIn);
        // read class state from archive
        iArchive >> newDataHandle;
        // archive and stream closed when destructors are called
    }

    KTINFO(testlog, "Printing data structure -- after loading");
    printer.PrintDataStructure(newDataHandle);

    KTCoreDataExt& newData = newDataHandle->Of< KTCoreDataExt >();
    KTTestDataExt& newTestData = newDataHandle->Of< KTTestDataExt >();

    KTINFO(testlog, "KTCoreData counter: " << newData.GetCounter());
    KTINFO(testlog, "KTTestData is-awesome: " << newTestData.GetIsAwesome());

    return 0;
}



