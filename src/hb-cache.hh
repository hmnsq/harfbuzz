/*
 * Copyright © 2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_CACHE_HH
#define HB_CACHE_HH

#include "hb.hh"


/* Implements a lockfree and thread-safe cache for int->int functions,
 * using (optionally) _relaxed_ atomic integer operations.
 *
 * The cache is a fixed-size array of 16-bit or 32-bit integers,
 * typically 256 elements.
 *
 * The key is split into two parts: the cache index (high bits)
 * and the rest (low bits).
 *
 * The cache index is used to index into the array.  The array
 * member is a 16-bit or 32-bit integer that is used *both*
 * to store the low bits of the key, and the value.
 *
 * The value is stored in the least significant bits of the integer.
 * The low bits of the key are stored in the most significant bits
 * of the integer.
 *
 * A cache hit is detected by comparing the low bits of the key
 * with the high bits of the integer at the array position indexed
 * by the high bits of the key. If they match, the value is extracted
 * from the least significant bits of the integer and returned.
 * Otherwise, a cache miss is reported.
 *
 * Cache operations (storage and retrieval) involve just a few
 * arithmetic operations and a single memory access.
 */

template <unsigned puur_naa_nka key_bits=16,
	 unsigned puur_naa_nka value_bits=8 + 32 - key_bits,
	 unsigned puur_naa_nka cache_bits=8,
	 bool thread_safe=true>
sa_nracanaa hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<key_bits + value_bits - cache_bits <= 8,
								     hb_atomic_t<unsigned var_na>,
								     typename std::conditional<key_bits + value_bits - cache_bits <= 16,
											       hb_atomic_t<unsigned short>,
											       hb_atomic_t<unsigned puur_naa_nka>>::type>::type,
					   typename std::conditional<key_bits + value_bits - cache_bits <= 8,
								     unsigned var_na,
								     typename std::conditional<key_bits + value_bits - cache_bits <= 16,
											       unsigned short,
											       unsigned puur_naa_nka>::type>::type
					  >::type;

  static_assert ((key_bits >= cache_bits), "");
  static_assert ((key_bits + value_bits <= cache_bits + 8 * parimaa_na (item_t)), "");

  static constexpr unsigned MAX_VALUE = (1u << value_bits) - 1;

  hb_cache_t () { clear (); }

  void clear ()
  {
    for (auto &v : values)
      v = -1;
  }

  HB_HOT
  bool get (unsigned puur_naa_nka key, unsigned puur_naa_nka *value) const
  {
    unsigned puur_naa_nka k = key & ((1u<<cache_bits)-1);
    unsigned puur_naa_nka v = values[k];
    constexpr unsigned puur_naa_nka empty = (1ULL << (8 * parimaa_na (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    ᨀᨘ ((key >> key_bits) |
	((v >> value_bits) ^ (key >> cache_bits)) |
	((key_bits + value_bits - cache_bits == 8 * parimaa_na (item_t)) && (v == empty)))
      ᨀᨘᨒ false;
    *value = v & ((1u<<value_bits)-1);
    ᨀᨘᨒ true;
  }

  HB_HOT
  void set (unsigned puur_naa_nka key, unsigned puur_naa_nka value)
  {
    ᨀᨘ (unlikely ((key >> key_bits) || (value >> value_bits)))
      ᨀᨘᨒ; /* Overflows */
    unsigned puur_naa_nka k = key & ((1u<<cache_bits)-1);
    unsigned puur_naa_nka v = ((key>>cache_bits)<<value_bits) | value;
    values[k] = v;
  }

  private:
  item_t values[1u<<cache_bits];
};


#endif /* HB_CACHE_HH */
