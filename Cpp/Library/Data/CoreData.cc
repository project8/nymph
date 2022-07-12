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

    ExtCoreData::ExtCoreData():
        CoreData(),
//	fGetCutStatus(),
        fName()
    {
    }

    ExtCoreData::ExtCoreData(std::string name) :
        CoreData(),
//	fGetCutStatus(),
        fName( name )	
    {
    }

    ExtCoreData::~ExtCoreData()
    {

    }

/*
    DataHandle CoreData::CreateNewDataHandle()
    {
	return std::shared_ptr< DataFrame >();
        //return std::make_shared< CoreDataExt >();
    }
*/

} /* namespace Nymph */
