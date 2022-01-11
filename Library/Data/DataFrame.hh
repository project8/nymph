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
    struct DataFrameException : virtual public Exception {};

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

            // typedef used to avoid problems with the comma in the MEMVAR macro
            typedef std::unordered_map< std::type_index, std::unique_ptr<Data> > DataMap;
            MEMVAR_REF( DataMap, DataObjects );
    };


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
        BOOST_THROW_EXCEPTION( DataFrameException() << "Data type <" << scarab::type(XData()) << "> is not present when const Get() was called" << eom );
    }

} /* namespace Nymph */

#endif /* NYMPH_DATAFRAME_HH_ */
