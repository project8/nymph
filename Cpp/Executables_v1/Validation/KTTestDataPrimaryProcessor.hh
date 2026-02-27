/*
 * KTTestPrimaryProcessor.hh
 *
 *  Created on: Feb 17, 2018
 *      Author: N.S. Oblath
 */

#ifndef KTTESTDATAPRIMARYPROCESSOR_HH_
#define KTTESTDATAPRIMARYPROCESSOR_HH_

#include "KTPrimaryProcessor.hh"

#include "KTMemberVariable.hh"
#include "KTSignal.hh"

namespace Nymph
{

   class KTTestDataPrimaryProcessor : public KTPrimaryProcessor
    {
        public:
            KTTestDataPrimaryProcessor( const std::string& name = "test-data-p-proc" );
            virtual ~KTTestDataPrimaryProcessor();

        public:
            bool Configure( const scarab::param_node& node );

            MEMBERVARIABLE( unsigned, Iterations );

        public:
            virtual bool Run();

        private:
            KTSignalData fTheSignal;

    };

} /* namespace Nymph */

#endif /* KTTESTDATAPRIMARYPROCESSOR_HH_ */
