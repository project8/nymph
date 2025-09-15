
#include "Adder.hh"

#include "IntData.hh"


REGISTER_PROCESSOR( tettigoniidae, Adder, "adder" )

namespace tettigoniidae
{
    Adder::Adder( const std::string& name ) :
            Processor( name ),
            fIntValue( 0 ),
            fIntSignal( "int", this ),
            fAddIntSlot( "add-int", this, &Adder::Add, &fIntSignal )
    {}

    Adder::~Adder()
    {}

    void Adder::Configure( const scarab::param_node& node )
    {
        fIntValue = node.get_value( "int-value", fIntValue );

        return;
    }

    void Adder::Add( IntData& data )
    {
        data.SetIValue1( data.GetIValue1() + fIntValue );
        data.SetIValue2( data.GetIValue2() + fIntValue );
        return;
    }

} /* namespace tettigoniidae */
