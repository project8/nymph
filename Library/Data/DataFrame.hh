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

#include <unordered_map>
#include <memory>
#include <typeindex>

namespace Nymph
{
    class DataFrameException : public BaseException< DataFrameException >
    {
        public:
            using BaseException< DataFrameException >::BaseException;
            ~DataFrameException() = default;
    };

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
            virtual ~DataFrame();

            /// Returns true if the frame has no data objects
            bool Empty() const;

            /// Returns true if object of type XData exists in the frame; returns false otherwise
            template< typename XData >
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

            // typedef used to avoid problems with the comma in the MEMVAR macro
            typedef std::unordered_map< std::type_index, std::unique_ptr<Data> > DataMap;
            MEMVAR_REF( DataMap, DataObjects );
    };


    inline bool DataFrame::Empty() const
    {
        return fDataObjects.empty();
    }

    template< typename XData >
    bool DataFrame::Has() const
    {
        return fDataObjects.count( typeid(XData) ) != 0;
    }

    template< typename XData >
    XData& DataFrame::Get()
    {
        if( Has< XData >() )
        {
            return static_cast< XData& >( *fDataObjects[typeid(XData)] );
        }

        fDataObjects[ typeid(XData) ].reset( new XData() );
        return static_cast< XData& >( *fDataObjects[typeid(XData)] );
    }

    template< typename XData >
    const XData& DataFrame::Get() const
    {
        if( Has< XData >() )
        {
            return static_cast< const XData& >( *fDataObjects.at(typeid(XData) ));
        }
        THROW_EXCEPT_HERE( DataFrameException() << "Data type <" << scarab::type(XData()) << "> is not present when const Get() was called" );
    }

    template< typename XData >
    void DataFrame::Set( XData* ptr )
    {
        fDataObjects[ typeid(XData) ].reset( ptr );  // take ownership of ptr
    }

    template< typename XData >
    void DataFrame::Set( std::unique_ptr< XData >&& ptr )
    {
        fDataObjects[ typeid(XData) ] = std::move(ptr);  // take ownership of ptr
        return;
    }

    template< typename XData >
    void DataFrame::Set( const XData& obj )
    {
        fDataObjects[ typeid(XData) ].reset( new XData(obj) );  // make a copy of obj
    }

    template< typename XData >
    void DataFrame::Remove()
    {
        fDataObjects.erase( typeid(XData) );
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_DATAFRAME_HH_ */
