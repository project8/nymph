/*
 * Exception.hh
 *
 *  Created on: Feb 25, 2014
 *      Author: nsoblath
 */


#ifndef NYMPH_EXCEPTION_HH_
#define NYMPH_EXCEPTION_HH_

#include "base_exception.hh"

//#include <boost/exception/all.hpp>

//#include <exception>
//#include <sstream>
//#include <string>

namespace Nymph
{
    /*
    class BaseException : virtual public std::exception
    {
        public:
            BaseException();
            ~BaseException() noexcept;

            MEMVAR_REF( std::string, AtFilename );
            MEMVAR( int, AtLineNumber );

            virtual const char* where() const noexcept;

        private:
            mutable std::string fBuffer;
    };

    template< typename XDerived >
    class TypedException : virtual public scarab::base_exception< XDerived >, public BaseException
    {
        public:
            TypedException();
            TypedException( const std::string& filename, int lineNum );
            ~TypedException() noexcept;

            XDerived& operator()( const std::string& filename, int lineNum );

    };
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
/*
    template< typename XDerived >
    TypedException< XDerived >::TypedException() :
            scarab::base_exception< XDerived >(),
            BaseException()
    {}

    template< typename XDerived >
    TypedException< XDerived >::TypedException( const std::string& filename, int lineNumber ) :
            scarab::base_exception< XDerived >(),
            BaseException()
    {
        fAtFilename = filename;
        fAtLineNumber = lineNumber;
    }

    template< typename XDerived >
    TypedException< XDerived >::~TypedException() noexcept
    {}

    template< typename XDerived >
    XDerived& TypedException< XDerived >::operator()( const std::string& filename, int lineNumber )
    {
        fAtFilename = filename;
        fAtLineNumber = lineNumber;
        return *static_cast< XDerived* >(this);
    }
*/
/* removed for trying home-grown exceptions, 1/10/22
    struct MessageEnd {};
    static const MessageEnd eom = MessageEnd();

    //typedef boost::error_info<struct tag_errmsg, std::string> ErrorMsgInfo;

    template< class XLabel >
    using ErrorMsgInfo = boost::error_info< XLabel, std::string>;
*/
    /* // removed for trying home-grown exceptions, 1/10/22
     @class Exception
     @author N.S. Oblath

     @brief Exception base class for Nymph and Nymph-based projects

     @details

     How to throw:

     BOOST_THROW_EXCEPTION( Exception() << "[error message here]" << eom );

     Why do it like this?
      - BOOST_THROW_EXCEPTION will record the location that the exception was thrown.  This is incredibly useful in diagnosing problems.
      - eom ensure that your error message is handled properly; until that's processed the message is stored in a buffer, and the eom takes that buffer and adds it to the boost::exception base class's storage.
      - Exception can be replaced with whatever derived exception class you're using.

      How to catch, add information, and re-throw:

      try
      {
          // code that throws a Exception
      }
      catch( boost::exception& e )
      {
          e << ErrorMsgInfo< struct UniqueLocationTag >( "Here's some information about the context in which the exception was thrown" );
          throw;
      }


      How to do the final catch

      try
      {
          // code that throws a Exception
      }
      catch( boost::exception& e )
      {
          ERROR( my_log, "An exception was caught: " << diagnostic_information( e ) ):
      }
    */
/* removed for trying home-grown exceptions, 1/10/22
    class Exception : virtual public boost::exception, virtual public std::exception
    {
        public:
            Exception();
            Exception( const Exception& );
            ~Exception() throw ();

            // adds to the message buffer
            template< class XStreamable >
            Exception& operator<<( XStreamable a_fragment );
            // adds to the message buffer
            Exception& operator<<( const std::string& a_fragment );
            // adds to the message buffer
            Exception& operator<<( const char* a_fragment );

            // adds the current contents of the message buffer as a ErrorMsgInfo
            Exception& operator<<( const MessageEnd& eom );

        private:
            mutable std::string fMsgBuffer;
    };

    template< class XStreamable >
    Exception& Exception::operator<<( XStreamable a_fragment )
    {
        std::stringstream stream;
        stream << a_fragment;
        stream >> fMsgBuffer;
        return *this;
    }

    inline Exception& Exception::operator<<( const std::string& a_fragment )
    {
        fMsgBuffer += a_fragment;
        return *this;
    }

    inline Exception& Exception::operator<<( const char* a_fragment )
    {
        fMsgBuffer += std::string( a_fragment );
        return *this;
    }

    inline Exception& Exception::operator<<( const MessageEnd& eom )
    {
        if( ! fMsgBuffer.empty() )
        {
            boost::operator<<( *this, ErrorMsgInfo< struct ExcpMsg >{ fMsgBuffer } );
            fMsgBuffer.clear();
        }
        return *this;
    }
*/
}

#endif /* NYMPH_EXCEPTION_HH_ */
