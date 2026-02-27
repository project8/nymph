/*
 * Exception.hh
 *
 *  Created on: Feb 25, 2014
 *      Author: nsoblath
 */


#ifndef NYMPH_EXCEPTION_HH_
#define NYMPH_EXCEPTION_HH_

#include "base_exception.hh"


namespace Nymph
{
    /*!
     @class Exception
     @author N. S. Oblath

     @brief Basic exception class for use in Nymph and derived packages

    */
    class Exception : public scarab::typed_exception< Exception >
    {
        public:
            using scarab::typed_exception< Exception >::typed_exception;
            virtual ~Exception() noexcept = default;
    };

#define CREATE_EXCEPT_HERE( anException )  anException( __FILE__, __LINE__ )
#define EXCEPT_HERE( anException )  (anException)( __FILE__, __LINE__ )

#define THROW_EXCEPT_HERE( anException ) throw EXCEPT_HERE( anException )
#define THROW_NESTED_EXCEPT_HERE( anException ) std::throw_with_nested( EXCEPT_HERE( anException ) )

    /// Recursive print function for potentially-nested exceptions (for scarab::base_exception)
    void PrintException( const scarab::base_exception& e, unsigned count = 0 );

    /// Recursive print function for potentially-nested exceptions (for std::exception)
    void PrintException( const std::exception& e, unsigned count = 0 );

}

#endif /* NYMPH_EXCEPTION_HH_ */
