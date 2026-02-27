/*
 *  DataManager.hh
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */


#ifndef NYMPH_DATAMANAGER_HH_
#define NYMPH_DATAMANAGER_HH_

#include "MemberVariable.hh"

#include <typeindex>
#include <unordered_map>

namespace Nymph
{
    struct Holder
    {}

    template< typename XType >
    struct TypedHolder
    {
        
    }

    class DataManager
    {
        public:
            DataManager();
            virtual ~DataManager();

            MEMVAR_REF( std::map< std::type_index, std::list< Holder > >, DataStore );
    };

} /* namespace Nymph */

#endif /* NYMPH_DATAMANAGER_HH_ */
