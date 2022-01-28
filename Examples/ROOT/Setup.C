/*
 * Setup.C
 *
 *  Created on: Jan 27, 2022
 *      Author: N.S. Oblath
 * 
 *  Add include paths for Nymph and Scarab, and load the Nymph library
 */

#include "TROOT.h"

#include <string>

void Setup( const std::string& buildPath )
{
    std::string includePath(
        " -I" + buildPath + "/include/Nymph" + 
        " -I" + buildPath + "/include/Nymph/Scarab"
    );

    gSystem->AddIncludePath(includePath.c_str());


}
