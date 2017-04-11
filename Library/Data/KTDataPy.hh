/*
 * KTDataPy.hh
 *
 * Created on: April 10, 2017
 *     Author: laroque
 */

 #ifndef KTDATAPY_HH_
 #define KTDATAPY_HH_

#include "KTData.hh"

void export_KTDataPy()
{
    using namespace Nymph;
    using namespace boost::python;
    class_<KTData, boost::noncopyable>("KTData", init<>())
        .add_property("Counter", &KTData::GetCounter, &KTData::SetCounter)
        .add_property("LastData", &KTData::GetLastData, &KTData::SetLastData)
        ;
}

#endif /* KTDATAPY_HH_ */
