/*
 *  DataFrame.hh
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */


#ifndef NYMPH_DATAFRAME_HH_
#define NYMPH_DATAFRAME_HH_

#include "Data.hh"
#include "CutStatus.hh"
#include "Exception.hh"

#include "MemberVariable.hh"

#include "typename.hh"

#include <unordered_map>
#include <memory>
#include <typeindex>

namespace Nymph
{
    class DataFrameException : public scarab::typed_exception< DataFrameException >
    {
        public:
            using scarab::typed_exception< DataFrameException >::typed_exception;
            ~DataFrameException() = default;
    };

    // forward declare so we can define DataHandle here
    class DataFrame;

    /// Pointer object to use for passing between Processors
    typedef std::shared_ptr< DataFrame > DataHandle;

    /*!
     @class DataFrame

     @author N. S. Oblath

     @brief Container for Data objects and their cut information used during data processing

     @details
     Individual Data objects are held in an unordered map, indexed by type.
     A CutStatus object contains cut information in an ordered map, with keys of cut name.
     The CutStatus describes the latest/most processed data object in the dataframe, and is updated as new data objects are added with more cuts.

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

            // CutStatus object for storing cut information
            CutStatus fCuts; 
    };


    inline bool DataFrame::Empty() const
    {
        return fDataObjects.empty();
    }

    template< typename XData >
    bool DataFrame::Has() const
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        return fDataObjects.count( typeid(XDataNoConst) ) != 0;
    }

    template< typename XData >
    XData& DataFrame::Get()
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        if( ! Has< XDataNoConst >() )
        {
            fDataObjects[ typeid(XDataNoConst) ].reset( new XDataNoConst() );
        }
        return static_cast< XDataNoConst& >( *fDataObjects[typeid(XDataNoConst)] );
    }

    template< typename XData >
    const XData& DataFrame::Get() const
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        if( Has< XDataNoConst >() )
        {
            return static_cast< const XDataNoConst& >( *fDataObjects.at(typeid(XDataNoConst)) );
        }
        THROW_EXCEPT_HERE( DataFrameException() << "Data type <" << scarab::type(XDataNoConst()) << "> is not present when const Get() was called" );
    }

    template< typename XData >
    void DataFrame::Set( XData* ptr )
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        fDataObjects[ typeid(XDataNoConst) ].reset( ptr );  // take ownership of ptr
        return;
    }

    template< typename XData >
    void DataFrame::Set( std::unique_ptr< XData >&& ptr )
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        fDataObjects[ typeid(XDataNoConst) ] = std::move(ptr);  // take ownership of ptr
        return;
    }

    template< typename XData >
    void DataFrame::Set( const XData& obj )
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        fDataObjects[ typeid(XDataNoConst) ].reset( new XDataNoConst(obj) );  // make a copy of obj
        return;
    }

    template< typename XData >
    void DataFrame::Remove()
    {
        typedef std::remove_const_t< XData > XDataNoConst;
        fDataObjects.erase( typeid(XDataNoConst) );
        return;
    }

} /* namespace Nymph */

#endif /* NYMPH_DATAFRAME_HH_ */
