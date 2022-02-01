/*
 * KTTIFactory.hh
 *
 *  Created on: Jan 2, 2013
 *      Author: nsoblath
 *
 *      Type-Indexed Factory and Registrars
 */

#ifndef KTTIFACTORY_HH_
#define KTTIFACTORY_HH_

#include "KTLogger.hh"

#include "singleton.hh"

#include <map>
#include <typeinfo>

namespace Nymph
{
    KTLOGGER(utillog_ti_factory, "KTTIFactory");

    //********************
    // Class definitions
    //********************

    template< class XBaseType, typename ... XArgs >
    class KTTIFactory;

    template< class XBaseType, typename ... XArgs >
    class KTTIRegistrarBase
    {
        public:
            KTTIRegistrarBase() {}
            virtual ~KTTIRegistrarBase() {}

        public:
            friend class KTTIFactory< XBaseType, XArgs... >;

        protected:
            virtual XBaseType* Create(XArgs ... args) const = 0;

    };

    template< class XBaseType, class XDerivedType, typename ... XArgs >
    class KTTIRegistrar : public KTTIRegistrarBase< XBaseType, XArgs... >
    {
        public:
            KTTIRegistrar();
            virtual ~KTTIRegistrar();

        protected:
            void Register() const;

            XBaseType* Create(XArgs ... args) const;

    };


    template< class XBaseType, typename ... XArgs >
    class KTTIFactory : public scarab::singleton< KTTIFactory< XBaseType, XArgs... > >
    {
        public:
            struct CompareTypeInfo
            {
                bool operator() (const std::type_info* lhs, const std::type_info* rhs)
                {
                    return lhs->before(*rhs);
                }
            };

        public:
            typedef std::map< const std::type_info*, const KTTIRegistrarBase< XBaseType, XArgs... >* > FactoryMap;
            typedef typename FactoryMap::value_type FactoryEntry;
            typedef typename FactoryMap::iterator FactoryIt;
            typedef typename FactoryMap::const_iterator FactoryCIt;

        public:
            template< class XDerivedType >
            XBaseType* Create(XArgs ... args);

            XBaseType* Create(const FactoryCIt& iter, XArgs ... args);

            template< class XDerivedType >
            void Register(const KTTIRegistrarBase< XBaseType, XArgs... >* registrar);

            FactoryCIt GetFactoryMapBegin() const;
            FactoryCIt GetFactoryMapEnd() const;

        protected:
            FactoryMap* fMap;


        protected:
            friend class scarab::singleton< KTTIFactory >;
            friend class scarab::destroyer< KTTIFactory >;
            KTTIFactory();
            ~KTTIFactory();
    };

    //*****************************************
    // Partial specialization for empty XArgs
    //*****************************************

    template< class XBaseType >
    class KTTIRegistrarBase< XBaseType, void >
    {
        public:
            KTTIRegistrarBase() {}
            virtual ~KTTIRegistrarBase() {}

        public:
            friend class KTTIFactory< XBaseType >;

        protected:
            virtual XBaseType* Create() const = 0;

    };

    template< class XBaseType, class XDerivedType >
    class KTTIRegistrar< XBaseType, XDerivedType, void > : public KTTIRegistrarBase< XBaseType, void >
    {
        public:
            KTTIRegistrar();
            virtual ~KTTIRegistrar();

        protected:
            void Register() const;

            XBaseType* Create() const;

    };


    template< class XBaseType >
    class KTTIFactory< XBaseType, void > : public scarab::singleton< KTTIFactory< XBaseType, void > >
    {
        public:
            struct CompareTypeInfo
            {
                bool operator() (const std::type_info* lhs, const std::type_info* rhs)
                {
                    return lhs->before(*rhs);
                }
            };

        public:
            typedef std::map< const std::type_info*, const KTTIRegistrarBase< XBaseType, void >* > FactoryMap;
            typedef typename FactoryMap::value_type FactoryEntry;
            typedef typename FactoryMap::iterator FactoryIt;
            typedef typename FactoryMap::const_iterator FactoryCIt;

        public:
            template< class XDerivedType >
            XBaseType* Create();

            XBaseType* Create(const FactoryCIt& iter);

            template< class XDerivedType >
            void Register(const KTTIRegistrarBase< XBaseType, void >* registrar);

            FactoryCIt GetFactoryMapBegin() const;
            FactoryCIt GetFactoryMapEnd() const;

        protected:
            FactoryMap* fMap;


        protected:
            friend class scarab::singleton< KTTIFactory >;
            friend class scarab::destroyer< KTTIFactory >;
            KTTIFactory();
            ~KTTIFactory();
    };


    //******************
    // Implementations
    //******************

    // Factory

    template< class XBaseType, typename ... XArgs >
    template< class XDerivedType >
    XBaseType* KTTIFactory< XBaseType, XArgs... >::Create(XArgs ... args)
    {
        FactoryCIt it = fMap->find(&typeid(XDerivedType));
        if (it == fMap->end())
        {
            KTERROR(utillog_ti_factory, "Did not find factory with type <" << typeid(XDerivedType).name() << ">.");
            return NULL;
        }

        return it->second->Create(args...);
    }

    template< class XBaseType, typename ... XArgs >
    XBaseType* KTTIFactory< XBaseType, XArgs... >::Create(const FactoryCIt& iter, XArgs ... args)
    {
        return iter->second->Create(args...);
    }

    template< class XBaseType, typename ... XArgs >
    template< class XDerivedType >
    void KTTIFactory< XBaseType, XArgs... >::Register(const KTTIRegistrarBase< XBaseType, XArgs... >* registrar)
    {
        // A local (static) logger is created inside this function to avoid static initialization order problems
        KTLOGGER(utillog_ti_factory_reg, "KTTIFactory-Register");

        FactoryCIt it = fMap->find(&typeid(XDerivedType));
        if (it != fMap->end())
        {
            KTERROR(utillog_ti_factory_reg, "Already have factory registered for type <" << typeid(XDerivedType).name() << ">.");
            return;
        }
        fMap->insert(std::pair< const std::type_info*, const KTTIRegistrarBase< XBaseType, XArgs... >* >(&typeid(XDerivedType), registrar));
        KTDEBUG(utillog_ti_factory_reg, "Registered a factory for class type " << typeid(XDerivedType).name() << ", factory #" << fMap->size()-1);
    }

    template< class XBaseType, typename ... XArgs >
    KTTIFactory< XBaseType, XArgs... >::KTTIFactory() :
        fMap(new FactoryMap())
    {}

    template< class XBaseType, typename ... XArgs >
    KTTIFactory< XBaseType, XArgs... >::~KTTIFactory()
    {
        delete fMap;
    }

    template< class XBaseType, typename ... XArgs >
    typename KTTIFactory< XBaseType, XArgs... >::FactoryCIt KTTIFactory< XBaseType, XArgs... >::GetFactoryMapBegin() const
    {
        return fMap->begin();
    }

    template< class XBaseType, typename ... XArgs >
    typename KTTIFactory< XBaseType, XArgs... >::FactoryCIt KTTIFactory< XBaseType, XArgs... >::GetFactoryMapEnd() const
    {
        return fMap->end();
    }

    // Registrar

    template< class XBaseType, class XDerivedType, typename ... XArgs >
    KTTIRegistrar< XBaseType, XDerivedType, XArgs... >::KTTIRegistrar() :
            KTTIRegistrarBase< XBaseType, XArgs... >()
    {
        Register();
    }

    template< class XBaseType, class XDerivedType, typename ... XArgs >
    KTTIRegistrar< XBaseType, XDerivedType, XArgs... >::~KTTIRegistrar()
    {}

    template< class XBaseType, class XDerivedType, typename ... XArgs >
    void KTTIRegistrar< XBaseType, XDerivedType, XArgs... >::Register() const
    {
        KTTIFactory< XBaseType, XArgs... >::get_instance()->template Register<XDerivedType>(this);
        return;
    }

    template< class XBaseType, class XDerivedType, typename ... XArgs >
    XBaseType* KTTIRegistrar< XBaseType, XDerivedType, XArgs... >::Create(XArgs ... args) const
    {
        return dynamic_cast< XBaseType* >(new XDerivedType(args...));
    }

    //*******************************************
    // Implementations (partial specialization)
    //*******************************************

    // Factory

    template< class XBaseType >
    template< class XDerivedType >
    XBaseType* KTTIFactory< XBaseType, void >::Create()
    {
        FactoryCIt it = fMap->find(&typeid(XDerivedType));
        if (it == fMap->end())
        {
            KTERROR(utillog_ti_factory, "Did not find factory with type <" << typeid(XDerivedType).name() << ">.");
            return NULL;
        }

        return it->second->Create();
    }

    template< class XBaseType >
    XBaseType* KTTIFactory< XBaseType, void >::Create(const FactoryCIt& iter)
    {
        return iter->second->Create();
    }

    template< class XBaseType >
    template< class XDerivedType >
    void KTTIFactory< XBaseType, void >::Register(const KTTIRegistrarBase< XBaseType, void >* registrar)
    {
        // A local (static) logger is created inside this function to avoid static initialization order problems
        KTLOGGER(utillog_ti_factory_reg, "KTTIFactory-Register");

        FactoryCIt it = fMap->find(&typeid(XDerivedType));
        if (it != fMap->end())
        {
            KTERROR(utillog_ti_factory_reg, "Already have factory registered for type <" << typeid(XDerivedType).name() << ">.");
            return;
        }
        fMap->insert(std::pair< const std::type_info*, const KTTIRegistrarBase< XBaseType, void >* >(&typeid(XDerivedType), registrar));
        KTDEBUG(utillog_ti_factory_reg, "Registered a factory for class type " << typeid(XDerivedType).name() << ", factory #" << fMap->size()-1);
    }

    template< class XBaseType >
    KTTIFactory< XBaseType, void >::KTTIFactory() :
        fMap(new FactoryMap())
    {}

    template< class XBaseType >
    KTTIFactory< XBaseType, void >::~KTTIFactory()
    {
        delete fMap;
    }

    template< class XBaseType >
    typename KTTIFactory< XBaseType, void >::FactoryCIt KTTIFactory< XBaseType, void >::GetFactoryMapBegin() const
    {
        return fMap->begin();
    }

    template< class XBaseType >
    typename KTTIFactory< XBaseType, void >::FactoryCIt KTTIFactory< XBaseType, void >::GetFactoryMapEnd() const
    {
        return fMap->end();
    }

    // Registrar

    template< class XBaseType, class XDerivedType >
    KTTIRegistrar< XBaseType, XDerivedType, void >::KTTIRegistrar() :
            KTTIRegistrarBase< XBaseType, void >()
    {
        Register();
    }

    template< class XBaseType, class XDerivedType >
    KTTIRegistrar< XBaseType, XDerivedType, void >::~KTTIRegistrar()
    {}

    template< class XBaseType, class XDerivedType >
    void KTTIRegistrar< XBaseType, XDerivedType, void >::Register() const
    {
        KTTIFactory< XBaseType, void >::get_instance()->template Register<XDerivedType>(this);
        return;
    }

    template< class XBaseType, class XDerivedType >
    XBaseType* KTTIRegistrar< XBaseType, XDerivedType, void >::Create() const
    {
        return dynamic_cast< XBaseType* >(new XDerivedType());
    }


} /* namespace Nymph */
#endif /* KTTIFACTORY_HH_ */
