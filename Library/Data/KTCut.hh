/*
 * KTCut.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef KTCUT_HH_
#define KTCUT_HH_

#include "KTConfigurable.hh"
#include "KTCutResult.hh"
#include "KTCoreData.hh"
#include "KTException.hh"
#include "KTExtensibleStructFactory.hh"
#include "KTLogger.hh"
#include "KTMemberVariable.hh"

#include "factory.hh"
#include "typename.hh"

#include <functional>

namespace Nymph
{
    KTLOGGER(cutlog_h, "KTCut.h");

    /*!
     @class KTCut
     @author N. S. Oblath

     @brief Base class for a cut that gets applied to data objects.

     @details
     A fully implemented cut MUST have the following:
     - Public nested class called Result, inheriting from KTExtensibleCutResult< Result >, and containing a public static std::string name sName.
     - Cut registration using the macro KT_REGISTER_CUT([class name])
     - Implementation of bool Configure(const scarab::param_node*)
     - Implementation of bool Apply(KTCoreData&, <DataType(s)>)

     Your cut class should inherit from KTCutOneArg or KTCutTwoArgs, depending on the number of data types involved in your cut.

     The existence of [class name]::Result and [class name]::Result::sName are enforces at compile time by the KT_REGISTER_CUT macro.

     The functions bool Configure(const scarab::param_node*) and void Apply(KTCoreData&, <DataType(s)>) are abstract in the base classes, and therefore must be implemented.

     Boolean return value interpretation:
     - TRUE means the cut was failed
     - FALSE means the cut was passed

     --------------------------------------
     ------- Example Cut Definition -------
     --------------------------------------

     class KTSomeData;
     class KTSomeDataExt;

     // Data must be at least as awesome as fAwesomenessThreshold to pass this cut
     class KTAwesomenessCut : public KTCutOnData< KTSomeDataExt >
     {
         public:
             struct Result : KTExtensibleCutResult< Result >
             {
                 static const std::string sName;
             };

         public:
             KTAwesomenessCut(const std::string& name = "default-example-cut");
             virtual ~KTAwesomenessCut();

             bool Configure(const scarab::param_node* node);

             MEMBERVARIABLE(double, AwesomenessThreshold);

         public:
             bool Apply(KTCoreData& data, KTSomeData& data);

     };

     --------------------------------------
     ------- Example Implementation -------
     --------------------------------------

     const std::string KTAwesomenessCut::Result::sName = "awesomeness-cut";

     KT_REGISTER_CUT(KTAwesomenessCut);

     KTAwesomenessCut::KTAwesomenessCut(const std::string& name) :
             KTCutOnData(name),
             fAwesomenessThreshold(1000000.)
     {
         SetApplyFunc( this, &KTAwesomeCut::Apply );
     }

     KTAwesomenessCut::~KTExampleCut()
     {}

     bool KTAwesomenessCut::Configure(const scarab::param_node* node)
     {
         if (node == NULL) return true;
         SetAwesomenessThreshold(node->get_value("awesomeness", GetAwesomenessThreshold()));
         return true;
     }

     bool KTAwesomenessCut::Apply(KTCoreData& data, KTSomeData& someData)
     {
         bool isCut = someData.Awesomeness() < fAwesomenessThreshold;
         data.GetCutStatus().AddCutResult< KTAwesomenessCut::Result >(isCut);
         return isCut;
     }

    */

    //************************************
    // KTCut -- base class for all cuts
    //************************************

    class KTCut : public KTConfigurable
    {
        public:
            KTCut(const std::string& name = "default-cut-name");
            virtual ~KTCut();

            virtual bool Apply(KTDataHandle) = 0;

    };

    //*******************************************************
    // KTCutOnData -- cut applied to one or more data objects
    //*******************************************************

    template< class ... XExtDataTypes >
    class KTCutOnData : public KTCut
    {
        public:
            KTCutOnData( const std::string& name = "default-cut-name" );
            virtual ~KTCutOnData();

            virtual bool Apply( KTDataHandle dataHandle );

        protected:
            template< class... SomeExtDataTypes >
            bool DataPresent( KTDataHandle data );

            template< class XFuncOwnerType, class... XFuncDataTypes >
            void SetApplyFunc( XFuncOwnerType* owner, bool (XFuncOwnerType::*func)( KTCoreData&, const XFuncDataTypes&... ) );

            std::function< bool ( KTCoreDataExt& data, const XExtDataTypes&... dataType ) > fFunc;

    };


    //*******************
    // Implementations
    //*******************

    template< class ... XExtDataTypes >
    KTCutOnData< XExtDataTypes... >::KTCutOnData( const std::string& name ) :
            KTCut(name)
    {
    }

    template< class ... XExtDataTypes >
    KTCutOnData< XExtDataTypes... >::~KTCutOnData()
    {}

    template< class ... XExtDataTypes >
    bool KTCutOnData< XExtDataTypes... >::Apply( KTDataHandle dataHandle )
    {
        // Check to ensure that the required data type is present
        if( ! DataPresent< XExtDataTypes... >( dataHandle ) )
        {
            KTERROR( cutlog_h, "Failed to find all of the necessary data types in slot <" << fConfigName << ">. Aborting." );
            BOOST_THROW_EXCEPTION( KTException() << "Failed to find all of the necessary data types in slot <" << fConfigName << ">. Aborting." << eom );
        }

        try
        {
            return fFunc( dataHandle->Of< KTCoreDataExt >() , dataHandle->Of< XExtDataTypes >()... );
        }
        catch( boost::exception& e )
        {
            e << KTErrorMsgInfo< struct slotData_RunFunc >( "Something went wrong in slot <" + fConfigName + ">. Aborting." );
            throw;
        }
    }

    template< class... XExtDataTypes >
    template< typename... SomeExtDataTypes >
    bool KTCutOnData< XExtDataTypes... >::DataPresent( KTDataHandle data )
    {
        return DataPresentHelper< SomeExtDataTypes... >::DataPresent( data );
    }

    template< class... XExtDataTypes >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    void KTCutOnData< XExtDataTypes... >::SetApplyFunc( XFuncOwnerType* owner, bool (XFuncOwnerType::*func)( KTCoreData&, const XFuncDataTypes&... ) )
    {
        fFunc = [func, owner]( KTCoreDataExt& data, const XExtDataTypes&... testData )->bool {return (owner->*func)(data, testData...);};
    }


    // this macro enforces the existence of cut_class::Result and cut_class::Result::sName at compile time
#define KT_REGISTER_CUT(cut_class) \
        static ::scarab::registrar< ::Nymph::KTCut, cut_class, const std::string& > sCut##cut_class##Registrar( cut_class::Result::sName ); \
        static ::Nymph::KTExtensibleStructRegistrar< ::Nymph::KTCutResultCore, cut_class::Result > sCut##cut_class##ResultRegistrar( cut_class::Result::sName );

} /* namespace Nymph */

#endif /* KTCUT_HH_ */
