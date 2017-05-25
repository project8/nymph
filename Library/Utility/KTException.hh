/*
 * KTException.hh
 *
 *  Created on: Feb 25, 2014
 *      Author: nsoblath
 */


#ifndef KTEXCEPTION_HH_
#define KTEXCEPTION_HH_

#include <boost/exception/all.hpp>

#include <exception>
#include <sstream>
#include <string>

namespace Nymph
{
    struct KTMessageEnd {};
    static const KTMessageEnd eom = KTMessageEnd();

    typedef boost::error_info<struct tag_errmsg, std::string> KTErrorMsgInfo;

    /*!
     @class KTException
     @author N.S. Oblath

     @brief Exception base class for Nymph and Nymph-based projects

     @details

     How to throw:

     BOOST_THROW_EXCEPTION( KTException() << "[error message here]" << eom );

     Why do it like this?
      - BOOST_THROW_EXCEPTION will record the location that the exception was thrown.  This is incredibly useful in diagnosing problems.
      - eom ensure that your error message is handled properly; until that's processed the message is stored in a buffer, and the eom takes that buffer and adds it to the boost::exception base class's storage.
      - KTException can be replaced with whatever derived exception class you're using.

      How to catch, add information, and re-throw:

      try
      {
          // code that throws a KTException
      }
      catch( boost::exception& e )
      {
          e << KTErrorMsgInfo( "Here's some information about the context in which the exception was thrown" );
          throw;
      }


      How to do the final catch

      try
      {
          // code that throws a KTException
      }
      catch( boost::exception& e )
      {
          KTERROR( my_log, "An exception was caught: " << diagnostic_information( e ) ):
      }
    */

    class KTException : virtual public boost::exception, virtual public std::exception
    {
        public:
            KTException();
            KTException( const KTException& );
            ~KTException() throw ();

            // adds to the message buffer
            template< class XStreamable >
            KTException& operator<<( XStreamable a_fragment );
            // adds to the message buffer
            KTException& operator<<( const std::string& a_fragment );
            // adds to the message buffer
            KTException& operator<<( const char* a_fragment );

            // adds the current contents of the message buffer as a KTErrorMsgInfo
            KTException& operator<<( const KTMessageEnd& eom );

            // overload for KTErrorMsgInfo to make sure error messages are streamed correctly, and not via the template function
            const KTException& operator<<( KTErrorMsgInfo& emi );

            virtual const char* what() const throw();

        private:
            mutable std::string fMsgBuffer;
    };

    template< class XStreamable >
    KTException& KTException::operator<<( XStreamable a_fragment )
    {
        std::stringstream stream;
        stream << a_fragment;
        stream >> fMsgBuffer;
        return *this;
    }

    inline KTException& KTException::operator<<( const std::string& a_fragment )
    {
        fMsgBuffer += a_fragment;
        return *this;
    }

    inline KTException& KTException::operator<<( const char* a_fragment )
    {
        fMsgBuffer += std::string( a_fragment );
        return *this;
    }

    inline KTException& KTException::operator<<( const KTMessageEnd& eom )
    {
        if( ! fMsgBuffer.empty() )
        {
            boost::operator<<( *this, KTErrorMsgInfo{ fMsgBuffer } );
            fMsgBuffer.clear();
        }
        return *this;
    }

    inline const KTException& KTException::operator<<( KTErrorMsgInfo& emi )
    {
        return boost::operator<<( *this, emi );
    }

}

#endif /* KTEXCEPTION_HH_ */
