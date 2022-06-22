/*
 * Cut.hh
 *
 *  Created on: Sept 19, 2014
 *      Author: nsoblath
 */

#ifndef CUT_HH_
#define CUT_HH_

///#include "KTConfigurable.hh"
#include "CutResult.hh"
//#include "KTCoreData.hh"
#include "DataFrame.hh"
#include "Exception.hh"
///#include "KTExtensibleStructFactory.hh"
#include "Logger.hh"
#include "MemberVariable.hh"
#include "DataPresent.hh"

#include <boost/exception/all.hpp>
#include "factory.hh"
#include "typename.hh"

#include <functional>

namespace Nymph
{
    LOGGER(cutlog_h, "Cut.h");

    /*!
     @class Cut
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

     REGISTER_CUT(KTAwesomenessCut);

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
    // Cut -- base class for all cuts
    //************************************

    class Cut
    {
        public:
            Cut(const std::string& name = "default-cut-name");
            virtual ~Cut();

            virtual bool Apply(DataHandle) = 0;

        public:
            const std::string& GetCutName() const;
            void SetCutName(const std::string& name);

        protected:
            std::string fCutName;
    };



    inline const std::string& Cut::GetCutName() const
    {
        return fCutName;
    }

    inline void Cut::SetCutName(const std::string& name)
    {
        fCutName = name;
    }

    //*******************************************************
    // CutOnData -- cut applied to one or more data objects
    //*******************************************************

    template< class ... XData >
    class CutOnData : public Cut
    {
        public:
            CutOnData( const std::string& name = "default-cut-name" );
            virtual ~CutOnData();

            virtual bool Apply( DataHandle dataHandle );

        protected:
            template< class... SomeExtDataTypes >
            bool DataPresent( DataHandle data );

            template< class XFuncOwnerType, class... XFuncDataTypes >
            void SetApplyFunc( XFuncOwnerType* owner, bool (XFuncOwnerType::*func)( DataFrame&, const XFuncDataTypes&... ) );

            std::function< bool ( const XData&... dataType ) > fFunc;
//            std::function< bool ( DataFrame& data, const XData&... dataType ) > fFunc;

    };


    //*******************
    // Implementations
    //*******************

    template< class ... XData >
    CutOnData< XData... >::CutOnData( const std::string& name ) :
            Cut(name)
    {
    }

    template< class ... XData >
    CutOnData< XData... >::~CutOnData()
    {}

    template< class ... XData >
    bool CutOnData< XData... >::Apply( DataHandle dataHandle )
    {
        // Check to ensure that the required data type is present
        if( ! DataPresent< XData... >( dataHandle ) )
        {
            LERROR( cutlog_h, "Failed to find all of the necessary data types in slot <" << fCutName << ">. Aborting." );
//            BOOST_THROW_EXCEPTION( Exception() << "Failed to find all of the necessary data types in slot <" << fCutName << ">. Aborting." << eom );
        }

        try
        {
            return fFunc( dataHandle->Get< XData >()... );
        }
        catch( boost::exception& e )
        {
//            e << KTErrorMsgInfo< struct slotData_RunFunc >( "Something went wrong in slot <" + fCutName + ">. Aborting." );
            throw;
        }
    }

//----------RemoveExtensibleDataFeatures-------------------

    template< class... XData >
    template< typename... SomeExtDataTypes >
    bool CutOnData< XData... >::DataPresent( DataHandle data )
    {
        return DataPresentHelper< SomeExtDataTypes... >::DataPresent( data );
    }
/*
    template< class... XData >
    template< class XFuncOwnerType, class... XFuncDataTypes >
    void CutOnData< XData... >::SetApplyFunc( XFuncOwnerType* owner, bool (XFuncOwnerType::*func)( const XFuncDataTypes&... ) )
    {
        fFunc = [func, owner]( const XData&... testData )->bool {return (owner->*func)(data, testData...);};
    }
*/

    // this macro enforces the existence of cut_class::Result and cut_class::Result::sName at compile time
#define REGISTER_CUT(cut_class, cut_name) \
        static ::scarab::registrar< ::Nymph::Cut, cut_class, const std::string& > sCut##cut_class##Registrar( cut_name );

} /* namespace Nymph */

#endif /* CUT_HH_ */
