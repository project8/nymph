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

    struct QuitThread
    {
        std::string fFile;
        std::string fLine;
        QuitThread( const std::string& file, const std::string& line ) :
                fFile( file ),
                fLine( line )
        {}
    };

    #define QUIT_THREAD \
        throw ::Nymph::QuitThread( TOSTRING(__FILE__), TOSTRING(__LINE__) );

} /* namespace Nymph */

#endif /* NYMPH_QUITTHREAD_HH_ */
