/*
 * RunNymph.hh
 *
 *  Created on: Sep 13, 2016
 *      Author: obla999
 */

#ifndef NYMPH_RUNNYMPH_HH_
#define NYMPH_RUNNYMPH_HH_

#include <memory>

namespace Nymph
{
    class KTApplication;

    int RunNymph( std::shared_ptr< KTApplication > aApp);
}


#endif /* NYMPH_RUNNYMPH_HH_ */
