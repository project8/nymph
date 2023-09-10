/*
 *  DataFrame.cc
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */

#include "DataFrame.hh"

namespace Nymph
{
    IndexableData::IndexableData( std::unique_ptr< Data >&& dataPtr, std::type_index&& typeIndex, std::string&& typeName ) :
            fDataPtr( std::move(dataPtr) ),
            fTypeIndex( std::move(typeIndex) ),
            fTypeName( std::move(typeName) )
    {}


    DataFrame::DataFrame() :
        fDataObjects(),
        fDataMapByType( fDataObjects.get<type_t>() ),
        fDataMapByTypeName( fDataObjects.get<tname_t>() )
    {}

    DataFrame::~DataFrame()
    {}

    bool DataFrame::Has( const std::string& typeName ) const
    {
        return fDataMapByTypeName.count( typeName ) != 0;
    }

    Data& DataFrame::Get( const std::string& typeName )
    {
        auto iter = fDataMapByTypeName.find( typeName );
        if( iter == fDataMapByTypeName.end() )
        {
            THROW_EXCEPT_HERE( DataFrameException() << "Adding event type by Get(string) is not yet implemented" );
/*            // TODO: this doesn't work; needs mechanism to go from string to type
            auto result = fDataMapByTypeName.insert( IndexableData::Create<XDataNoConst>() );
            if( result.second )
            {
                return static_cast< XDataNoConst& >( *result.first->fDataPtr );
            }
            else
            {
                THROW_EXCEPT_HERE( DataFrameException() << "Data type called <" << typeName << "> could not be added to the data frame" );
            }
*/        }
        return *iter->fDataPtr;
    }

    const Data& DataFrame::Get( const std::string& typeName ) const
    {
        auto iter = fDataMapByTypeName.find( typeName );
        if( iter == fDataMapByTypeName.end() )
        {
            THROW_EXCEPT_HERE( DataFrameException() << "Data type called <" << typeName << "> is not present when const Get() was called" );
        }
        return *iter->fDataPtr;
    }

    void DataFrame::Set( const std::string& typeName, Data* ptr )
    {
        // Note: takes ownership of ptr
        DoSet( IndexableData( std::unique_ptr<Data>(ptr), typeid(*ptr), scarab::type(*ptr) ) );
        return;
    }

    void DataFrame::Set( const std::string& typeName, std::unique_ptr<Data>&& ptr )
    {
        // Note: takes ownership of object pointed to by ptr
        auto& ref = *ptr.get();
        DoSet( IndexableData( std::move(ptr), typeid(ref), scarab::type(*ptr) ) );
        return;
    }

    void DataFrame::DoSet( IndexableData&& indData )
    {
        auto iter = fDataMapByType.find( indData.fTypeIndex );
        if( iter == fDataMapByType.end() )
        {
            auto result = fDataMapByType.insert( std::move(indData) );
            if( ! result.second )
            {
                THROW_EXCEPT_HERE( DataFrameException() << "Attempt to insert new data object failed for data type named <" << indData.fTypeName << ">" );
            }
            return;
        }
        if( ! fDataMapByType.replace( iter, std::move(indData) ) )
        {
            THROW_EXCEPT_HERE( DataFrameException() << "Attempt to replace data object failed for data type <" << indData.fTypeName << ">" );
        }
        return;
    }

    void DataFrame::Remove( const std::string& typeName )
    {
        fDataMapByTypeName.erase( typeName );
        return;
    }

} /* namespace Nymph */
