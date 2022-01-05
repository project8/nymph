/*
 *  DataFrame.hh
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */


#ifndef NYMPH_DATAFRAME_HH_
#define NYMPH_DATAFRAME_HH_

#include "Data.hh"

#include "MemberVariable.hh"

#include <unordered_map>
#include <memory>
#include <typeindex>

namespace Nymph
{

    class DataFrame
    {
        public:
            DataFrame();
            virtual ~DataFrame();

            // typedef used to avoid problems with the comma in the MEMVAR macro
            typedef std::unordered_map< std::type_index, std::unique_ptr<Data> > DataMap;
            MEMVAR_REF( DataMap, DataObjects );
    };

} /* namespace Nymph */

#endif /* NYMPH_DATAFRAME_HH_ */
