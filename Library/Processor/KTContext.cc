/*
 * KTContext.cc
 *
 *  Created on: Aug 7, 2024
 *      Author: N.S. Oblath
 */

#include "KTContext.hh"

#include "KTLogger.hh"


namespace Nymph
{

    KTLOGGER(proclog, "KTContext.hh");

    KTContext::KTContext() :
            fData()
    {}

    KTContext::~KTContext()
    {}

    scarab::param& KTContext::Get(const std::string& name)
    {
        return fData[name];
    }

    const scarab::param& KTContext::Get(const std::string& name) const
    {
        return fData[name];
    }

    void KTContext::Add(const std::string& name, const scarab::param& data)
    {
        fData.add(name, data);
        return;
    }


} /* namespace Nymph */
