/*
 * QuitChain.hh
 *
 *  Created on: Oct 22, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_QUITCHAIN_HH_
#define NYMPH_QUITCHAIN_HH_

#include "base_exception.hh"

namespace Nymph
{
    /*!
     @class QuitChain
     @author N. S. Oblath

     @brief Thrown from a Processor chain to communicate within the chain that the chain 
     should stop running from the top (i.e. from the Primary Processor)

    */
    class QuitChain : public scarab::typed_exception< QuitChain >
    {
        public:
            using scarab::typed_exception< QuitChain >::typed_exception;
            ~QuitChain() = default;
    };

    #define QUIT_CHAIN  throw ::Nymph::QuitChain()(__FILE__, __LINE__)
    //THROW_EXCEPT_HERE( ::Nymph::QuitChain() )

} /* namespace Nymph */

#endif /* NYMPH_QUITCHAIN_HH_ */
