/**
 @file ProcessorRegistrar.hh
 @brief Contains ProcessorRegistrar
 @details Customized indexed_factory registrar for Processors.
 @author: N. S. Oblath
 @date: Oct 3, 2025
 */

#ifndef NYMPH_PROCESSORREGISTRAR_HH_
#define NYMPH_PROCESSORREGISTRAR_HH_

//#include "Processor.hh"
#include "ProcessorToolbox.hh"

#include "indexed_factory.hh"


namespace Nymph
{
    class Processor;

    // registrar

    template< class XDerivedType, typename ... XArgs >
    class ProcessorRegistrar : public scarab::base_registrar< Processor, XArgs... >
    {
        public:
            ProcessorRegistrar( const std::string& a_index );
            virtual ~ProcessorRegistrar();

            void register_class() const;

            Processor* create( XArgs ... args ) const;

        protected:
            std::string f_index;
    };


    template< class XDerivedType, typename ... XArgs >
    ProcessorRegistrar< XDerivedType, XArgs... >::ProcessorRegistrar( const std::string& a_index ) :
            scarab::base_registrar< Processor, XArgs... >(),
            f_index( a_index )
    {
        register_class();
    }

    template< class XDerivedType, typename ... XArgs >
    ProcessorRegistrar< XDerivedType, XArgs... >::~ProcessorRegistrar()
    {
        ProcessorToolbox::GetProcFactory()->remove_class( f_index );
    }

    template< class XDerivedType, typename ... XArgs >
    void ProcessorRegistrar< XDerivedType, XArgs... >::register_class() const
    {
        ProcessorToolbox::GetProcFactory()->register_class( f_index, this );
        return;
    }

    template< class XDerivedType, typename ... XArgs >
    Processor* ProcessorRegistrar< XDerivedType, XArgs... >::create( XArgs... args ) const
    {
        return dynamic_cast< Processor* >( new XDerivedType( args... ) );
    }

}

#endif /* NYMPH_PROCESSORREGISTRAR_HH_ */
