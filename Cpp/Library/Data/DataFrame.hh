/*
 *  DataFrame.hh
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */


#ifndef NYMPH_DATAFRAME_HH_
#define NYMPH_DATAFRAME_HH_

#include "Data.hh"
#include "Exception.hh"

#include "MemberVariable.hh"

#include "typename.hh"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/key.hpp>

#include <memory>
#include <typeindex>

namespace bmi = ::boost::multi_index;

namespace Nymph
{
    class DataFrameException : public scarab::typed_exception< DataFrameException >
    {
        public:
            using scarab::typed_exception< DataFrameException >::typed_exception;
            ~DataFrameException() = default;
    };

    // map< std::string, std::type_index > may give us what we need to go from string to class
    //  * this would enable DataFrame::Set( std::string, Data* ) and DataFrame::Set( std::string, std::unique_ptr<Data> )
    //  * would not enable DataFrame::Get(); need to be able to create an instance of the type

    /// Struct to hold individual data pointers and indexing information for the DataFrame
    struct IndexableData
    {
        std::unique_ptr< Data > fDataPtr;
        std::type_index fTypeIndex;
        std::string fTypeName;

        IndexableData( std::unique_ptr< Data >&& dataPtr, std::type_index&& typeIndex, std::string&& typeName );

        // Use factory functions so that we can template it with the derived data type 
        // (wouldn't be able to have the empty constructor beacuse there's nothing to determine the template type by, 
        //  and you can't have a template argument for a constructor)

        /// Create an IndexableData with an empty object of type XData
        template< typename XData >
        static IndexableData Create();
        /// Create an IndexableData with data (claims ownership of that object)
        template< typename XData >
        static IndexableData Create( XData* data );
        /// Create an IndexableData with the object pointed to by dataPtr (claims ownership of that object)
        template< typename XData >
        static IndexableData Create( std::unique_ptr< XData >&& dataPtr );
        /// Create an IndexableData with a copy of the provided object
        template< typename XData >
        static IndexableData Create( const XData& data );
    };

    // forward declare so we can define DataHandle here
    class DataFrame;

    /// Pointer object to use for passing between Processors
    typedef std::shared_ptr< DataFrame > DataHandle;

    /*!
     @class DataFrame

     @author N. S. Oblath

     @brief Container for Data objects used during data processing

     @details
     Individual Data objects are held in an unordered map, indexed by type.

    */
    class DataFrame
    {
        public:
            DataFrame();
            DataFrame( const DataFrame& ) = delete;
            virtual ~DataFrame();

            DataFrame& operator=( const DataFrame& ) = delete;

            /// Returns true if the frame has no data objects
            bool Empty() const;

            //**********************
            // Type-based interface
            //**********************

            /// Returns true if object of type(s) XData exist in the frame; returns false otherwise; Has<>() returns true
            template< typename... XData >
            bool Has() const;

            /// Returns a reference to the object of type XData if it exists in the frame.
            /// Creates and returns a reference to an object of type XData if it does not exist.
            template< typename XData >
            XData& Get();

            /// Returns a reference to the object of type XData if it exists in the frame.
            /// Throws DataFrameException if it does not exist.
            template< typename XData >
            const XData& Get() const;

            /// Inserts the provided object of type XData into the frame, taking ownership of the object.
            /// If an object of this type exists, it is replaced.
            template< typename XData >
            void Set( XData* ptr );

            /// Inserts the provided object of type XData into the frame, taking ownership of the object.
            /// If an object of this type exists, it is replaced.
            template< typename XData >
            void Set( std::unique_ptr<XData>&& ptr );

            /// Inserts a copy of the provided object of type XData into the frame.
            /// If an object of this type exists, it is replaced.
            template< typename XData >
            void Set( const XData& obj );

            /// Removes and destroys the object of type XData if it exists in the frame.
            /// Does nothing if the object does not exist.
            template< typename XData >
            void Remove();


            //************************
            // String-based interface
            //************************

            bool Has( const std::string& typeName ) const;

            Data& Get( const std::string& typeName );

            const Data& Get( const std::string& typeName ) const;

            void Set( const std::string& typeName, Data* ptr );

            void Set( const std::string& typeName, std::unique_ptr<Data>&& ptr );

            //void Set( const std::string& typeName, const Data& obj );  <-- this probably requires having Clone() functions setup in Data

            void Remove( const std::string& typeName );


            //*********
            // Storage
            //*********

            // typedef used to avoid problems with the comma in the MEMVAR macro
            //typedef std::unordered_map< std::type_index, std::unique_ptr<Data> > DataMap;
            // tags
            struct type_t{};
            struct tname_t{};
            typedef bmi::multi_index_container< 
                IndexableData,
                bmi::indexed_by<
                    bmi::hashed_unique< bmi::tag<type_t>, bmi::key< &IndexableData::fTypeIndex > >,
                    bmi::hashed_unique< bmi::tag<tname_t>, bmi::key< &IndexableData::fTypeName > >
                >
            > DataMap;
            typedef DataMap::index<type_t>::type DataMapByType;
            typedef DataMap::index<tname_t>::type DataMapByTypeName;
            MEMVAR_REF( DataMap, DataObjects );

        protected:
            template< typename XData >
            bool HasOneType() const;

            void DoSet( IndexableData&& indData );

            DataMapByType& fDataMapByType;
            DataMapByTypeName& fDataMapByTypeName;
    };


    //*************************
    // IndexableData functions
    //*************************

    template< typename XData >
    IndexableData IndexableData::Create()
    {
        return IndexableData{ std::make_unique<XData>(), typeid(XData), scarab::type<XData>() };
    }

    template< typename XData >
    IndexableData IndexableData::Create( XData* data )
    {
        return IndexableData{ std::unique_ptr<XData>(data), typeid(XData), scarab::type<XData>() };
    }

    template< typename XData >
    IndexableData IndexableData::Create( std::unique_ptr< XData >&& dataPtr )
    {
        return IndexableData{ std::move(dataPtr), typeid(XData), scarab::type<XData>() };
    }

    template< typename XData >
    IndexableData IndexableData::Create( const XData& data )
    {
        return IndexableData{ std::make_unique<XData>(data), typeid(XData), scarab::type<XData>() };
    }


    //*********************
    // DataFrame functions
    //*********************

    inline bool DataFrame::Empty() const
    {
        return fDataObjects.empty();
    }

    template< typename... XData >
    bool DataFrame::Has() const
    {
        return ( HasOneType<XData>() && ... );
    }

    template< typename XData >
    bool DataFrame::HasOneType() const
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        return fDataMapByType.count( typeid(XDataNoConst) ) != 0;
    }

    template< typename XData >
    XData& DataFrame::Get()
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        auto iter = fDataMapByType.find( typeid(XDataNoConst) );
        if( iter == fDataMapByType.end() )
        {
            auto result = fDataMapByType.insert( IndexableData::Create<XDataNoConst>() );
            if( result.second )
            {
                return static_cast< XDataNoConst& >( *result.first->fDataPtr );
            }
            else
            {
                THROW_EXCEPT_HERE( DataFrameException() << "Data type <" << scarab::type<XDataNoConst>() << "> could not be added to the data frame" );
            }
        }
        return static_cast< XDataNoConst& >( *iter->fDataPtr );
    }

    template< typename XData >
    const XData& DataFrame::Get() const
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        auto iter = fDataMapByType.find( typeid(XDataNoConst) );
        if( iter == fDataMapByType.end() )
        {
            THROW_EXCEPT_HERE( DataFrameException() << "Data type <" << scarab::type<XDataNoConst>() << "> is not present when const Get() was called" );
        }
        return static_cast< const XDataNoConst& >( *iter->fDataPtr );
    }

    template< typename XData >
    void DataFrame::Set( XData* ptr )
    {
        // Note: takes ownership of ptr
        typedef std::remove_const_t< XData > XDataNoConst;
        DoSet( IndexableData::Create<XDataNoConst>( ptr ) );
        return;
    }

    template< typename XData >
    void DataFrame::Set( std::unique_ptr< XData >&& ptr )
    {
        // Note: takes ownership of object pointed to by ptr
        typedef std::remove_const_t< XData > XDataNoConst;
        DoSet( IndexableData::Create<XDataNoConst>( std::move(ptr) ) );
        return;
    }

    template< typename XData >
    void DataFrame::Set( const XData& obj )
    {
        // Note: makes a copy of obj and takes ownership of the copy
        typedef std::remove_const_t< XData > XDataNoConst;
        DoSet( IndexableData::Create<XDataNoConst>( obj ) );
        return;
    }

    template< typename XData >
    void DataFrame::Remove()
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        fDataMapByType.erase( typeid(XDataNoConst) );
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_DATAFRAME_HH_ */
