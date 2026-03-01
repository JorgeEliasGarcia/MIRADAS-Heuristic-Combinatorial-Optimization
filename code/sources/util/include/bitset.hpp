#pragma once

#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>

using namespace std;

#define N_BITS_WORD (8 * sizeof(long))

namespace GOMA
{

	class bitset
	{
	public:
		long *block_;
		size_t sz_;
		size_t max_inx_;

	public:
		bitset(size_t max_inx) : block_(NULL),
					   sz_(ceil((double)(max_inx + 1) / (double)N_BITS_WORD)),
					   max_inx_(max_inx)
		{

			block_ = new long [sz_];
			clear();
		};

		bitset(void) : block_(NULL),
					   sz_(ceil((double)(32 + 1) / (double)N_BITS_WORD)),
					   max_inx_(32)
		{

			block_ = new long [sz_];
			clear();
		};

		bitset(const bitset &bs) : block_(NULL),
								   sz_(bs.sz_),
								   max_inx_(bs.max_inx_)
		{
			block_ = new long[sz_];

			for (size_t i{0}; i < sz_; i++)
				block_[i] = bs.block_[i];
		}

		~bitset(void)
		{
			if (block_)
				delete[] block_;
		}

		void set_size(size_t max_inx)
		{
			if (block_)
				delete[] block_;

			max_inx_ = max_inx;
			sz_ = ceil((double)(max_inx + 1) / (double)N_BITS_WORD);

			block_ = new long[sz_];
			clear();
		}

		const bitset &operator=(const bitset &bs)
		{
			for (size_t i{0}; i < sz_; i++)
				block_[i] = bs.block_[i];

			return bs;
		}

		void clear(void)
		{

			for (size_t i{0}; i < sz_; i++)
				block_[i] = 0;
		}

		void reset(void)
		{
			clear();
		}

		bool empty(void) const
		{
			bool empty_set = true;

			size_t i{0};

			while ((i < sz_) && (empty_set))
			{
				empty_set = empty_set && (block_[i] == 0);
				i++;
			}

			return empty_set;
		}

		void insert(unsigned int i)
		{

			assert(i <= max_inx_);
			assert(i >= 1);

			i--;

			const size_t pos = i / N_BITS_WORD;
			const size_t offset = i % N_BITS_WORD;

			block_[pos] |= (long(0x1) << offset);
		}

		void set(unsigned int i)
		{
			insert(i);
		}

		void remove(unsigned int i)
		{

			assert(i <= max_inx_);
			assert(i >= 1);

			i--;

			const size_t pos = i / N_BITS_WORD;
			const size_t offset = i % N_BITS_WORD;

			block_[pos] &= ~(long(0x1) << offset);
		}

		bool contains(unsigned int i) const
		{

			assert(i <= max_inx_);
			assert(i >= 1);

			i--;

			const size_t pos = i / N_BITS_WORD;
			const size_t offset = i % N_BITS_WORD;

			return (block_[pos] & (long(0x1) << offset)) != long(0x0);
		}

		bool test(unsigned int i) const
		{
			return contains(i);
		}

		bool find(unsigned int i) const
		{
			return contains(i);
		}

		void insert(const bitset &bs)
		{
			for (size_t i{0}; i < sz_; i++)
				block_[i] |= bs.block_[i];
		}

		void remove(const bitset &bs)
		{
			for (size_t i{0}; i < sz_; i++)
				block_[i] &= ~bs.block_[i];
		}

		bool contains(const bitset &bs) const
		{
			bool contains_set = true;

			size_t i{0};

			while ((i < sz_) && (contains_set))
			{
				contains_set = contains_set && ((block_[i] & bs.block_[i]) == bs.block_[i]);
				i++;
			}

			return contains_set;
		}

		bool disjoint(const bitset &bs) const
		{
			bool disjoint_set = true;

			size_t i{0};

			while ((i < sz_) && (disjoint_set))
			{
				disjoint_set = disjoint_set && ((block_[i] & bs.block_[i]) == long(0x0));
				i++;
			}

			return disjoint_set;
		}

		int first_item(void) const
		{
			int sm = -1;

			for (size_t i{0}; (i < sz_) && (sm == -1); i++)
			{
				if (block_[i] != 0)
					sm = first_item(block_[i]) + N_BITS_WORD * i;
			}

			return sm;
		}

		int last_item(void) const
		{
			int bg = -1;

			for (int i = sz_ - 1; (i >= 0) && (bg == -1); i--)
			{
				if (block_[i] != 0)
					bg = last_item(block_[i]) + N_BITS_WORD * i;
			}

			return bg;
		}

		int cardinality(void) const
		{
			int card = 0;

			for (size_t i{0}; i < sz_; i++)
				card += cardinality(block_[i]);

			return card;
		}

		ostream &write(ostream &os) const
		{

			string s;
			to_string(s);
			os << s;

			return os;
		}

		void union_set(const bitset &B, bitset &C) const
		{
			for (size_t j{0}; j < sz_; j++)
				C.block_[j] = block_[j] | B.block_[j];
		}

		void intersec_set(const bitset &B, bitset &C) const
		{
			for (size_t j{0}; j < sz_; j++)
				C.block_[j] = block_[j] & B.block_[j];
		}

		void diff_set(const bitset &B, bitset &C) const
		{
			for (size_t j{0}; j < sz_; j++)
				C.block_[j] = block_[j] & ~B.block_[j];
		}

	private:
		int first_item(long block) const
		{
			return __builtin_ffsl(block);
		}

		int last_item(long block) const
		{
			return N_BITS_WORD - __builtin_clzl(block);
		}

		int cardinality(long block) const
		{
			return __builtin_popcount(block);
		}

		void to_string(string &s) const
		{
			for (size_t j{0}; j < sz_; j++)
			{

				const size_t sz = min(N_BITS_WORD,max_inx_ - j * N_BITS_WORD);

				long block = block_[j];

				for (size_t i{0}; i < sz; i++)
				{
					const char c = '0' + (block & long(0x1));
					s.insert(s.begin(), c);
					block >>= 1;
				}
			}
		}
	};

}