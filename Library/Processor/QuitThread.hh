/*
 * QuitThread.hh
 *
 *  Created on: Oct 22, 2019
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_QUITTHREAD_HH_
#define NYMPH_QUITTHREAD_HH_

#include <string>

namespace Nymph
{
    class QuitThread : public scarab::typed_exception< QuitThread >
    {
        public:
            using scarab::typed_exception< QuitThread >::typed_exception;
            ~QuitThread() = default;
    };

    #define QUIT_THREAD  THROW_EXCEPT_HERE( ::Nymph::QuitThread() )

} /* namespace Nymph */

#endif /* NYMPH_QUITTHREAD_HH_ */
