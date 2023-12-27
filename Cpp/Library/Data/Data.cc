/*
 *  Data.cc
 *
 *  Created on: Nov 5, 2019
 *      Author: N.S. Oblath
 */

#include "Data.hh"

#include "typename.hh"

namespace Nymph
{
    Data::Data()
    {}

    Data::~Data()
    {}

    std::string Data::GetName() const
    {
        return scarab::type(*this);
    }

} /* namespace Nymph */
