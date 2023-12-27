/*
 *  Data.hh
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */


#ifndef NYMPH_DATA_HH_
#define NYMPH_DATA_HH_

#include <string>

namespace Nymph
{

    class Data
    {
        public:
            Data();
            virtual ~Data();

            std::string GetName() const;
    };

} /* namespace Nymph */

#endif /* NYMPH_DATA_HH_ */
