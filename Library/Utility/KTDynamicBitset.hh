/*
 * KTDynamicBitset.hh
 *
 *  Created on: Jun 9, 2017
 *      Author: obla999
 */

#ifndef KTDYNAMICBITSET_HH_
#define KTDYNAMICBITSET_HH_

#include <boost/dynamic_bitset.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

// serialization of dynamic bitsets taken from https://stackoverflow.com/a/31014564

namespace boost
{
    namespace serialization
    {

        template <typename Ar, typename Block, typename Alloc>
        void save(Ar& ar, dynamic_bitset<Block, Alloc> const& bs, unsigned)
        {
            size_t num_bits = bs.size();
            std::vector<Block> blocks(bs.num_blocks());
            to_block_range(bs, blocks.begin());

            ar & num_bits & blocks;
        }

        template <typename Ar, typename Block, typename Alloc>
        void load(Ar& ar, dynamic_bitset<Block, Alloc>& bs, unsigned)
        {
            size_t num_bits;
            std::vector<Block> blocks;
            ar & num_bits & blocks;

            bs.resize(num_bits);
            from_block_range(blocks.begin(), blocks.end(), bs);
            bs.resize(num_bits);
        }

        template <typename Ar, typename Block, typename Alloc>
        void serialize(Ar& ar, dynamic_bitset<Block, Alloc>& bs, unsigned version)
        {
            split_free(ar, bs, version);
        }

    }
}


#endif /* KTDYNAMICBITSET_HH_ */
