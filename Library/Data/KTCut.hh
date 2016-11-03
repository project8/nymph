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
#include "KTData.hh"
#include "KTExtensibleStructFactory.hh"
#include "KTLogger.hh"
#include "KTMemberVariable.hh"

#include "factory.hh"
#include "typename.hh"

namespace Nymph
{
    KTLOGGER(cutlog_h, "KTCut.h");

    /*!
     @class KTCut
     @author N. S. Oblath

     @brief Base class for a cut that gets applied to data objects.

     @details
     A fully implemented cut MUST have the following:
     - Public nested class called Result, inheriting from KTCutResult, and containing a public static std::string name sName.
     - Cut registration using the macro KT_REGISTER_CUT([class name])
     - Implementation of bool Configure(const scarab::param_node*)
     - Implementation of bool Apply(KTData&, <DataType(s)>)

     Your cut class should inherit from KTCutOneArg or KTCutTwoArgs, depending on the number of data types involved in your cut.

     The existence of [class name]::Result and [class name]::Result::sName are enforces at compile time by the KT_REGISTER_CUT macro.

     The functions bool Configure(const scarab::param_node*) and void Apply(KTData&, <DataType(s)>) are abstract in the base classes, and therefore must be implemented.

     Boolean return value interpretation:
     - TRUE means the cut was failed
     - FALSE means the cut was passed

     --------------------------------------
     ------- Example Cut Definition -------
     --------------------------------------

     class KTSomeData;

     // Data must be at least as awesome as fAwesomenessThreshold to pass this cut
     class KTAwesomenessCut : public KTCutOneArg< KTSomeData >
     {
         public:
             struct Result : KTCutResult
             {
                 static const std::string sName;
             };

         public:
             KTAwesomenessCut(const std::string& name = "default-example-cut");
             ~KTAwesomenessCut();

             bool Configure(const scarab::param_node* node);

             MEMBERVARIABLE(double, AwesomenessThreshold);

         public:
             bool Apply(KTData& data, KTSomeData& data);

     };

     --------------------------------------
     ------- Example Implementation -------
     --------------------------------------

     const std::string KTExampleCut::Result::sName = "awesomeness-cut";

     KT_REGISTER_CUT(KTExampleCut, KTExampleCut::Result::sName);

     KTAwesomenessCut::KTAwesomenessCut(const std::string& name) :
             KTCutOneArg(name),
             fAwesomenessThreshold(1000000.)
     {}

     KTAwesomenessCut::~KTExampleCut()
     {}

     bool KTAwesomenessCut::Configure(const scarab::param_node* node)
     {
         if (node == NULL) return true;
         SetAwesomenessThreshold(node->GetValue("awesomeness", GetAwesomenessThreshold()));
         return true;
     }

     bool KTAwesomenessCut::Apply(KTData& data, KTSomeData& someData)
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

            virtual bool Apply(KTDataPtr) = 0;
    };


    //*****************************************************************
    // KTCutOneArg -- base class for cuts operating on one data type
    //*****************************************************************

    template< class XDataType >
    class KTCutOneArg : public KTCut
    {
        public:
            KTCutOneArg(const std::string& name = "default-cut-name");
            virtual ~KTCutOneArg();

            virtual bool Apply(KTData& data, XDataType& dataType) = 0;

            virtual bool Apply(KTDataPtr dataPtr);
    };


    //*******************************************************************
    // KTCutTwoArgs -- base class for cuts operating on two data types
    //*******************************************************************

    template< class XDataType1, class XDataType2 >
    class KTCutTwoArgs : public KTCut
    {
        public:
            KTCutTwoArgs(const std::string& name = "default-cut-name");
            virtual ~KTCutTwoArgs();

            virtual bool Apply(KTData& data, XDataType1& dataType1, XDataType2& dataType2) = 0;

            virtual bool Apply(KTDataPtr dataPtr);
    };


    //*******************
    // Implementations
    //*******************

    template< class XDataType >
    KTCutOneArg< XDataType >::KTCutOneArg(const std::string& name) :
            KTCut(name)
    {
    }

    template< class XDataType >
    KTCutOneArg< XDataType >::~KTCutOneArg()
    {}

    template< class XDataType >
    bool KTCutOneArg< XDataType >::Apply(KTDataPtr dataPtr)
    {
        if (! dataPtr->Has< XDataType >())
        {
            KTERROR(cutlog_h, "Data type <" << scarab::type(XDataType()) << "> was not present");
            return false;
        }
        return Apply(dataPtr->Of< KTData >(), dataPtr->Of< XDataType >());
    }


    template< class XDataType1, class XDataType2 >
    KTCutTwoArgs< XDataType1, XDataType2 >::KTCutTwoArgs(const std::string& name) :
            KTCut(name)
    {
    }

    template< class XDataType1, class XDataType2 >
    KTCutTwoArgs< XDataType1, XDataType2 >::~KTCutTwoArgs()
    {}

    template< class XDataType1, class XDataType2 >
    bool KTCutTwoArgs< XDataType1, XDataType2 >::Apply(KTDataPtr dataPtr)
    {
        if (! dataPtr->Has< XDataType1 >())
        {
            KTERROR(cutlog_h, "Data type <" << scarab::type(XDataType1()) << "> was not present");
            return false;
        }
        if (! dataPtr->Has< XDataType2 >())
        {
            KTERROR(cutlog_h, "Data type <" << scarab::type(XDataType2()) << "> was not present");
            return false;
        }
        return Apply(dataPtr->Of< KTData >(), dataPtr->Of< XDataType1 >(), dataPtr->Of< XDataType2 >());
    }

/* Playing around: wouldn't it be cool if this could be done with variadic tmeplates?
 * Unfortunately we'll need to be able to iterate over the types in the template pack in the Apply(KTDataPtr) function.
 *
    template< class ... DataTypes >
    class KTCutOnData : public KTCut
    {
        public:
            KTCutOnData(const std::string& name = "default-cut-name");
            virtual ~KTCutOnData();

            virtual bool Apply(DataTypes ...) = 0;

            virtual bool Apply(KTDataPtr dataPtr);
    };

    template< class ... DataTypes >
    KTCutOnData< DataTypes... >::KTCutOnData(const std::string& name) :
            KTCut(name)
    {
    }

    template< class ... DataTypes >
    KTCutOnData< DataTypes... >::~KTCutOnData()
    {}

    template< class ... DataTypes >
    bool KTCutOnData< DataTypes... >::Apply(KTDataPtr dataPtr)
    {

    }
*/

    // this macro enforces the existence of cut_class::Result and cut_class::Result::sName at compile time
#define KT_REGISTER_CUT(cut_class) \
        static ::scarab::registrar< ::Nymph::KTCut, cut_class, const std::string& > sCut##cut_class##Registrar( cut_class::Result::sName ); \
        static ::Nymph::KTExtensibleStructRegistrar< ::Nymph::KTCutResultCore, cut_class::Result > sCut##cut_class##ResultRegistrar( cut_class::Result::sName );

} /* namespace Nymph */

#endif /* KTCUT_HH_ */
