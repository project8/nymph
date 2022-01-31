

#include <exception>
#include <iostream>
#include <vector>


struct Typeless
{
    template< typename XGet >
    XGet& Get()
    {
        HiddenType< XGet >* thisCast = dynamic_cast< XGet* >( this );
        if( ! thisCast )
        {
            throw std::runtime_error( "Invalid cast type" );
        }
        return thisCast->fObj;
    }
};

template< typename XHidden >
struct HiddenType : Typeless
{
    XHidden fObj;
};

template< typename... XTypes >
void ProcessVector( const std::vector< Typeless* >& captured )
{
    try
    {
        unsigned index = 0;
        for( auto iter = captured.begin(); iter != captured.end(); ++iter )
        {
            std::cout << index << ": " << captured->Get< XType >() << std::endl;
            ++index;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return;    
}

template< typename... XArgs >
std::vector< Typeless* > ProcessArgs( XArgs... args )
{
    std::tuple< XArgs... > argsTuple{ args };

    std::vector< Typeless* > captured;
    const unsigned nArgs = sizeof...(args);
    for( unsigned i = 0; i < nArgs; ++i)
    {
        captured[i] = new HiddenType< std::tuple_element<i, args...> >{std::get<i>(argsTuple)};
    }

}




int main()
{

    auto vec = ProcessArgs( 1, 2.5, "hello" );

    ProcessVector< int, double, std::string >( *vec );

    return 0;
}