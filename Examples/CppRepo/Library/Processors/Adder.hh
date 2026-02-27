/*!
 * @file Adder.hh
 *
 *  Created on: Nov 25, 2021
 *      Author: N.S. Oblath
 */

#ifndef TTGD_PROCESSORS_ADDER
#define TTGD_PROCESSORS_ADDER

#include "Processor.hh"
#include "SignalData.hh"
#include "SlotData.hh"

namespace tettigoniidae
{
    class IntData;

    /*!
     @class Adder
     @author N. S. Oblath

     @brief Adds a value to data

     @procdetails
     Does addition in place

     @proctype adder

     @config
     @configparam{int-value,int} Value to add to IntData

     @slots
     @slot{add-int,void (DataHandle)} Adds a value to IntData; Requires IntData; Adds no data; Emits signal "int"

     @signals
     @signal{int,void (DataHandle)} Emitted after adding to IntData
    */
    class Adder : public Nymph::Processor
    {
        public:
            Adder( const std::string& name = "adder" );

            virtual ~Adder();

            /// Processor configuration
            void Configure( const scarab::param_node& node );

            MEMVAR( int, IntValue );

        public:
            /// Add an integer to integer data
            void Add( IntData& data );

        public:
            Nymph::SignalData fIntSignal;

            Nymph::SlotData< Nymph::In<IntData>, Nymph::Out<> > fAddIntSlot;

    };

} /* namespace tettigoniidae */

#endif /* TTGD_PROCESSORS_ADDER */
