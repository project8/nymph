/*
 * Nymph.cc
 *
 *  Created on: Sep 28, 2012
 *      Author: nsoblath
 *
 *  This program will run any processor-based code in packages built with Nymph.
 *  All of the action is setup with a config file.
 *  See KTProcessorToolbox for details on the configuration option.
 */

#include "RunNymph.hh"
#include "logger.hh"
#include "param_codec.hh"

LOGGER( nymphlog, "Nymph" );


int main( int argc, char** argv )
{
    LPROG( nymphlog, "Welcome to Nymph!" );
    LDEBUG( nymphlog,
            "\n" <<
            "                                           Z                                 \n" <<
            "                                            =M                               \n" <<
            "                                             M                               \n" <<
            "                                             M                               \n" <<
            "                                            =M                               \n" <<
            "                                            N                                \n" <<
            "                                            M             NINMMMMMZ...M      \n" <<
            "                                            M            $$                  \n" <<
            "                        M:                  N.           IM                  \n" <<
            "                          ZD.                 M?          M                  \n" <<
            "                             $                M ?M       MM           .$     \n" <<
            "                               DM               M  O      8       .+MZ       \n" <<
            "                                 ~M                M~ 8  M~  MM:~M           \n" <<
            "                                   O:MMIZ+,?DNMD$.... M,MM MZ ? D$=  .8MOM+  \n" <<
            "                                      ,= MN ZZ D:  M:MM~MM==     MMZM        \n" <<
            "                                           ...MM MMM8,M+ I:ND $MMM           \n" <<
            "                                         MMMMO   ~M, ,MM7MOD$                \n" <<
            "                                    MMMMMM ~~8M? ,O~= MDNM M ,               \n" <<
            "                                MMMM:M   Z8 MMM :MNM = ..,  MZ               \n" <<
            "                            MNMMNM MM IM NM,M    N M   MM    +M         .NM M\n" <<
            "                        ~MMD8 :  D  MM=MM,     O MO    8=    + $     .IM     \n" <<
            "               ONMMMMMM..?  8 :MMMMM          = M      N      M    :M        \n" <<
            "   ..+M8=...:=MMMMMMMMMN OMM:M,             M  ~       7       MMM8          \n" <<
            "ZM8         ~M+:MZMMM~ M                    OM         M                     \n" <<
            "      .:M..$D..$D ? M,                    MM           MN                    \n" <<
            "   $M7      M   M+                       M              NMMMM:..             \n" <<
            "         M..M                          :N                       7M           \n" <<
            "      MM,                             M                           D          \n" <<
            "                                     M                                       \n" <<
            "                                   M=                                        \n" <<
            "                                   M                                         \n" <<
            "                                  =                                          \n" <<
            "                                  M                                          \n");


        std::string config_str(
            "processors:\n"
            "- type: test-primary\n"
            "  name: pp\n"
            "connections:\n"
            "- signal: \"pp:value\"\n"
            "  slot: \"pp:value\"\n"
            "run-queue:\n"
            "- pp\n"
        );

        scarab::param_translator translator;
        auto config = translator.read_string( config_str, "yaml" );

    try
    {
        //return Nymph::RunNymph( scarab::param_node& config );
	return Nymph::RunNymph( config->as_node() );
    }
    catch( std::exception& e )
    {
        LERROR( nymphlog, "Exception caught:\n" << e.what() );
        return -1;
    }

}
