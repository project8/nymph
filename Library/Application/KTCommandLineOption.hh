/*
 * KTCommandLineOption.hh
 *
 *  Created on: Aug 1, 2012
 *      Author: nsoblath
 */

#ifndef KTCOMMANDLINEOPTION_H_
#define KTCOMMANDLINEOPTION_H_

#include "KTCommandLineHandler.hh"

#include <string>

namespace Nymph
{
    //class KTCommandLineHandler;

    template< typename XInputType >
    class KTCommandLineOption
    {
        public:
            KTCommandLineOption();
            KTCommandLineOption(const std::string& group, const std::string& helpMsg, const std::string& longOpt, char shortOpt='#');
            virtual ~KTCommandLineOption();

        protected:
            KTCommandLineHandler* fCLHandler;

    };

    template< typename XInputType >
    KTCommandLineOption< XInputType >::KTCommandLineOption() :
            fCLHandler(KTCommandLineHandler::get_instance())
    {
    }

    template< typename XInputType >
    KTCommandLineOption< XInputType >::KTCommandLineOption(const std::string& group, const std::string& helpMsg, const std::string& longOpt, char shortOpt) :
            fCLHandler(KTCommandLineHandler::get_instance())
    {
        if (shortOpt == '#')
        {
            fCLHandler->AddOption< XInputType >(group, helpMsg, longOpt);
        }
        else
        {
            fCLHandler->AddOption< XInputType >(group, helpMsg, longOpt, shortOpt);
        }
    }

    template< typename XInputType >
    KTCommandLineOption< XInputType >::~KTCommandLineOption()
    {
    }


    template< >
    class KTCommandLineOption< bool >
    {
        public:
            KTCommandLineOption() :
                fCLHandler(KTCommandLineHandler::get_instance())
            {
            }
            KTCommandLineOption(const std::string& group, const std::string& helpMsg, const std::string& longOpt, char shortOpt='#') :
                fCLHandler(KTCommandLineHandler::get_instance())
            {
                if (shortOpt == '#')
                {
                    fCLHandler->AddOption(group, helpMsg, longOpt);
                }
                else
                {
                    fCLHandler->AddOption(group, helpMsg, longOpt, shortOpt);
                }
            }
            virtual ~KTCommandLineOption()
            {
            }

        protected:
            KTCommandLineHandler* fCLHandler;

    };

} /* namespace Nymph */

#endif /* KTCOMMANDLINEOPTION_H_ */
