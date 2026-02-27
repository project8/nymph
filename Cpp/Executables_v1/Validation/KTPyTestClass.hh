/*
 * KTPyTestClass.hh
 *
 *  Created on: Jan 24, 2018
 *      Author: obla999
 */

#ifndef NYMPH_KTPYTESTCLASS_HH_
#define NYMPH_KTPYTESTCLASS_HH_

#include <iostream>

namespace Nymph
{

    class KTPyTestClass
    {
        public:
            KTPyTestClass();
            ~KTPyTestClass();

            void SayHello()
            {
                std::cout << "Hello" << std::endl;
            }

            int GetValue()
            {
                return fValue;
            }

            void SetValue( int value )
            {
                fValue = value;
                return;
            }

        private:
            int fValue;
    };

} /* namespace Nymph */

#endif /* NYMPH_KTPYTESTCLASS_HH_ */
