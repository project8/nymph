/*
 * RunNymph.hh
 *
 *  Created on: Sep 13, 2016
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_RUNNYMPH_HH_
#define NYMPH_RUNNYMPH_HH_

namespace scarab
{
    class main_app;
    class param_node;
}

namespace Nymph
{
    int RunNymph( scarab::param_node& config );

    void AddRunNymphOptions( scarab::main_app& an_app );
}


#endif /* NYMPH_RUNNYMPH_HH_ */
