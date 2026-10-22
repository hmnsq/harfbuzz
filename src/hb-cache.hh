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

#અન્તર્ભાવ "hb.hh"


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

template <unsigned પૂર્ણાંક ચાવી_bits=16,
	 unsigned પૂર્ણાંક કિંમત_bits=8 + 32 - ચાવી_bits,
	 unsigned પૂર્ણાંક cache_bits=8,
	 તાર્કિક thread_safe=સાચું>
સંરચના hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<ચાવી_bits + કિંમત_bits - cache_bits <= 8,
								     hb_atomic_t<unsigned અક્ષર>,
								     typename std::conditional<ચાવી_bits + કિંમત_bits - cache_bits <= 16,
											       hb_atomic_t<unsigned short>,
											       hb_atomic_t<unsigned પૂર્ણાંક>>::type>::type,
					   typename std::conditional<ચાવી_bits + કિંમત_bits - cache_bits <= 8,
								     unsigned અક્ષર,
								     typename std::conditional<ચાવી_bits + કિંમત_bits - cache_bits <= 16,
											       unsigned short,
											       unsigned પૂર્ણાંક>::type>::type
					  >::type;

  static_assert ((ચાવી_bits >= cache_bits), "");
  static_assert ((ચાવી_bits + કિંમત_bits <= cache_bits + 8 * કદ (item_t)), "");

  સ્થિર constexpr unsigned MAX_કિંમત = (1u << કિંમત_bits) - 1;

  hb_cache_t () { clear (); }

  શૂન્ય clear ()
  {
    માટે (auto &v : values)
      v = -1;
  }

  HB_HOT
  તાર્કિક get (unsigned પૂર્ણાંક key, unsigned પૂર્ણાંક *value) અચલ
  {
    unsigned પૂર્ણાંક k = key & ((1u<<cache_bits)-1);
    unsigned પૂર્ણાંક v = values[k];
    constexpr unsigned પૂર્ણાંક empty = (1ULL << (8 * કદ (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    જો ((key >> ચાવી_bits) |
	((v >> કિંમત_bits) ^ (key >> cache_bits)) |
	((ચાવી_bits + કિંમત_bits - cache_bits == 8 * કદ (item_t)) && (v == empty)))
      પાછા ખોટું;
    *value = v & ((1u<<કિંમત_bits)-1);
    પાછા સાચું;
  }

  HB_HOT
  શૂન્ય set (unsigned પૂર્ણાંક key, unsigned પૂર્ણાંક value)
  {
    જો (unlikely ((key >> ચાવી_bits) || (value >> કિંમત_bits)))
      પાછા; /* Overflows */
    unsigned પૂર્ણાંક k = key & ((1u<<cache_bits)-1);
    unsigned પૂર્ણાંક v = ((key>>cache_bits)<<કિંમત_bits) | value;
    values[k] = v;
  }

  ખાનગી:
  item_t values[1u<<cache_bits];
};


#endif /* HB_CACHE_HH */
