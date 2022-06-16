/*
 * CoreData.cc
 *
 *  Created on: Aug 24, 2012
 *      Author: nsoblath
 */

#include "CoreData.hh"

namespace Nymph
{
    CoreData::CoreData() :
            Data(),
            fCounter( 0 ),
            fLastData( false ),
            fGetCutStatus()
    {
        std::cout << "### CoreData constructor" << std::endl;
    }

    CoreData::~CoreData()
    {
        std::cout << "### CoreData destrutor" << std::endl;
    }

/*
    DataHandle CreateNewDataHandle()
    {
        return std::make_shared< CoreDataExt >();
    }
*/

} /* namespace Nymph */
