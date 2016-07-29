/*
 * KTParamInputJSON.hh
 *
 *  CreAted on: Jan 14, 2014
 *      Author: nsoblAth
 */

#ifndef KTPARAMINPUTJSON_HH_
#define KTPARAMINPUTJSON_HH_

#include "document.h"

#include "param.hh"

#include <string>

namespace Nymph
{

    class KTParamInputJSON
    {
        public:
            KTParamInputJSON();
            virtual ~KTParamInputJSON();

            static scarab::param_node* ReadFile( const std::string& aFilename );
            static scarab::param_node* ReadString( const std::string& aJSONStr );
            static scarab::param_node* ReadDocument( const rapidjson::Document& aDocument );
            static scarab::param* ReadValue( const rapidjson::Value& aValue );

            static bool IsNameComment( const std::string& name );
    };

} /* namespace Nymph */

#endif /* KTPARAMINPUTJSON_HH_ */
