/*
 * KTDataPy.hh
 *
 * Created on: April 10, 2017
 *     Author: laroque
 */

 #ifndef KTDATAPY_HH_
 #define KTDATAPY_HH_

#include "KTCoreData.hh"

void export_DataPy()
{
    using namespace Nymph;
    using namespace boost::python;
    class_<KTCoreData, KTDataHandle, boost::noncopyable>("KTCoreData", init<>())
        .add_property("Counter", &KTCoreData::GetCounter, &KTCoreData::SetCounter)
        .add_property("LastData", &KTCoreData::GetLastData, &KTCoreData::SetLastData)
        ;
}

#endif /* KTDATAPY_HH_ */
