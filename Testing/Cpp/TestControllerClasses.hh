/*
 * TestControllerClasses.hh
 *
 *  Created on: Jan 24, 2022
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_TESTING_TESTCONTROLLERCLASSES
#define NYMPH_TESTING_TESTCONTROLLERCLASSES

#include "Controller.hh"

namespace Nymph
{

    class CIQThrowController : public Controller
    {

        public:
            using Controller::Controller;
            virtual ~CIQThrowController() {}

            virtual void ChainIsQuitting( const std::string& name, std::exception_ptr ePtr = std::exception_ptr() )
            {
                if( ePtr )
                {
                    std::rethrow_exception( ePtr );
                }
                return;
            }

    };

} /* namespace Nymph */

#endif /* NYMPH_TESTING_TESTCONTROLLERCLASSES */
