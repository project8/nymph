/*
 * KTTestPrimaryProcessor.hh
 *
 *  Created on: May 4, 2017
 *      Author: N.S. Oblath
 */

#ifndef KTTESTPRIMARYPROCESSOR_HH_
#define KTTESTPRIMARYPROCESSOR_HH_

#include "KTPrimaryProcessor.hh"

#include "KTMemberVariable.hh"
#include "KTSignal.hh"

namespace Nymph
{

   class KTTestPrimaryProcessor : public KTPrimaryProcessor
    {
        public:
            KTTestPrimaryProcessor( const std::string& name = "test-p-proc" );
            virtual ~KTTestPrimaryProcessor();

        public:
            bool Configure( const scarab::param_node* node );

            MEMBERVARIABLE( unsigned, Iterations );

        public:
            virtual bool Run();

        private:
            KTSignal< int > fTheSignal;

    };

} /* namespace Nymph */

#endif /* KTTESTPRIMARYPROCESSOR_HH_ */
