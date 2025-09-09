/*
 * IntData.hh
 *
 *  Created on: Jan 6, 2022
 *      Author: N.S. Oblath
 */

#ifndef TTGD_DATA_INTDATA
#define TTGD_DATA_INTDATA

#include "Data.hh"
#include "MemberVariable.hh"

namespace tettigoniidae
{

    /*!
     @class IntData
     @author N. S. Oblath
     @brief Two integer values
    */
    class IntData : public Nymph::Data
    {
        public:
            IntData();

            virtual ~IntData();

            MEMVAR( int, IValue1 );
            MEMVAR( int, IValue2 );
    };

} /* namespace tettigoniidae */

#endif /* TTGD_DATA_INTDATA */
