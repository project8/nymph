/*
 * HelloWorldProc.hh
 *
 *  Created on: Sept 20, 2025
 *      Author: N.S. Oblath
 */

#ifndef NYMPH_HELLOWORLDPROC_HH_
#define NYMPH_HELLOWORLDPROC_HH_

#include "Processor.hh"
#include "Signal.hh"
#include "Slot.hh"

namespace Nymph
{

    class HelloWorldCpp : public Nymph::Processor
    {
        public:
            HelloWorldCpp( const std::string& name = "hello-world-cpp" );

            virtual ~HelloWorldCpp();

            void Configure( const scarab::param_node& node ) {};

            void SayHello();

            MEMVAR_REF( Nymph::Signal<>, HelloSig );
            MEMVAR_REF( Nymph::Slot<>, HelloSlot );
    };

} /* namespace Nymph */

#endif /* NYMPH_HELLOWORLDPROC_HH_ */
