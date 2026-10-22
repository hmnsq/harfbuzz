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

#അന്തര്ഭാവ "hb.hh"


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

template <unsigned പൂർണ്ണസംഖ്യ താക്കോൽ_bits=16,
	 unsigned പൂർണ്ണസംഖ്യ മൂല്യം_bits=8 + 32 - താക്കോൽ_bits,
	 unsigned പൂർണ്ണസംഖ്യ cache_bits=8,
	 തർക്കം thread_safe=സത്യം>
ഘടന hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<താക്കോൽ_bits + മൂല്യം_bits - cache_bits <= 8,
								     hb_atomic_t<unsigned അക്ഷരം>,
								     typename std::conditional<താക്കോൽ_bits + മൂല്യം_bits - cache_bits <= 16,
											       hb_atomic_t<unsigned short>,
											       hb_atomic_t<unsigned പൂർണ്ണസംഖ്യ>>::type>::type,
					   typename std::conditional<താക്കോൽ_bits + മൂല്യം_bits - cache_bits <= 8,
								     unsigned അക്ഷരം,
								     typename std::conditional<താക്കോൽ_bits + മൂല്യം_bits - cache_bits <= 16,
											       unsigned short,
											       unsigned പൂർണ്ണസംഖ്യ>::type>::type
					  >::type;

  static_assert ((താക്കോൽ_bits >= cache_bits), "");
  static_assert ((താക്കോൽ_bits + മൂല്യം_bits <= cache_bits + 8 * വലുപ്പം (item_t)), "");

  സ്ഥിതം constexpr unsigned MAX_മൂല്യം = (1u << മൂല്യം_bits) - 1;

  hb_cache_t () { clear (); }

  ശൂന്യം clear ()
  {
    വേണ്ടി (auto &v : values)
      v = -1;
  }

  HB_HOT
  തർക്കം get (unsigned പൂർണ്ണസംഖ്യ key, unsigned പൂർണ്ണസംഖ്യ *value) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ k = key & ((1u<<cache_bits)-1);
    unsigned പൂർണ്ണസംഖ്യ v = values[k];
    constexpr unsigned പൂർണ്ണസംഖ്യ empty = (1ULL << (8 * വലുപ്പം (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    എങ്കിൽ ((key >> താക്കോൽ_bits) |
	((v >> മൂല്യം_bits) ^ (key >> cache_bits)) |
	((താക്കോൽ_bits + മൂല്യം_bits - cache_bits == 8 * വലുപ്പം (item_t)) && (v == empty)))
      തിരികെ അസത്യം;
    *value = v & ((1u<<മൂല്യം_bits)-1);
    തിരികെ സത്യം;
  }

  HB_HOT
  ശൂന്യം set (unsigned പൂർണ്ണസംഖ്യ key, unsigned പൂർണ്ണസംഖ്യ value)
  {
    എങ്കിൽ (unlikely ((key >> താക്കോൽ_bits) || (value >> മൂല്യം_bits)))
      തിരികെ; /* Overflows */
    unsigned പൂർണ്ണസംഖ്യ k = key & ((1u<<cache_bits)-1);
    unsigned പൂർണ്ണസംഖ്യ v = ((key>>cache_bits)<<മൂല്യം_bits) | value;
    values[k] = v;
  }

  സ്വകാര്യ:
  item_t values[1u<<cache_bits];
};


#endif /* HB_CACHE_HH */
