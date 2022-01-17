/*
 * KTException.hh
 *
 *  Created on: Feb 25, 2014
 *      Author: nsoblath
 */


#ifndef KTEXCEPTION_HH_
#define KTEXCEPTION_HH_

#include <exception>
#include <sstream>
#include <string>

namespace Nymph
{

    class KTException :
        public std::exception
    {
        public:
            KTException();
            KTException( const KTException& );
            ~KTException() throw ();

            template< class XStreamable >
            KTException& operator<<( XStreamable a_fragment );
            KTException& operator<<( const std::string& a_fragment );
            KTException& operator<<( const char* a_fragment );

            virtual const char* what() const throw();

        private:
            std::string fException;
    };

    template< class XStreamable >
    KTException& KTException::operator<<( XStreamable a_fragment )
    {
        std::stringstream stream;
        stream << a_fragment;
        stream >> fException;
        return *this;
    }

    inline KTException& KTException::operator<<( const std::string& a_fragment )
    {
        fException += a_fragment;
        return *this;
    }

    inline KTException& KTException::operator<<( const char* a_fragment )
    {
        fException += std::string( a_fragment );
        return *this;
    }

}

#endif /* KTEXCEPTION_HH_ */
