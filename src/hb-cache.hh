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

#ಅನ್ತರ್ಭಾವ "hb.hh"


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

template <unsigned ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ_bits=16,
	 unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_bits=8 + 32 - ಕೀಲಿ_bits,
	 unsigned ಪೂರ್ಣಾಙ್ಕ cache_bits=8,
	 ತಾರ್ಕಿಕ thread_safe=ಸತ್ಯ>
ಸಂರಚನಾ hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<ಕೀಲಿ_bits + ಮೌಲ್ಯ_bits - cache_bits <= 8,
								     hb_atomic_t<unsigned ವರ್ಣ>,
								     typename std::conditional<ಕೀಲಿ_bits + ಮೌಲ್ಯ_bits - cache_bits <= 16,
											       hb_atomic_t<unsigned short>,
											       hb_atomic_t<unsigned ಪೂರ್ಣಾಙ್ಕ>>::type>::type,
					   typename std::conditional<ಕೀಲಿ_bits + ಮೌಲ್ಯ_bits - cache_bits <= 8,
								     unsigned ವರ್ಣ,
								     typename std::conditional<ಕೀಲಿ_bits + ಮೌಲ್ಯ_bits - cache_bits <= 16,
											       unsigned short,
											       unsigned ಪೂರ್ಣಾಙ್ಕ>::type>::type
					  >::type;

  static_assert ((ಕೀಲಿ_bits >= cache_bits), "");
  static_assert ((ಕೀಲಿ_bits + ಮೌಲ್ಯ_bits <= cache_bits + 8 * ಪರಿಮಾಣ (item_t)), "");

  ಸ್ಥಿರ constexpr unsigned MAX_ಮೌಲ್ಯ = (1u << ಮೌಲ್ಯ_bits) - 1;

  hb_cache_t () { clear (); }

  ಶೂನ್ಯ clear ()
  {
    ಪ್ರತಿ (auto &v : values)
      v = -1;
  }

  HB_HOT
  ತಾರ್ಕಿಕ get (unsigned ಪೂರ್ಣಾಙ್ಕ key, unsigned ಪೂರ್ಣಾಙ್ಕ *value) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ k = key & ((1u<<cache_bits)-1);
    unsigned ಪೂರ್ಣಾಙ್ಕ v = values[k];
    constexpr unsigned ಪೂರ್ಣಾಙ್ಕ empty = (1ULL << (8 * ಪರಿಮಾಣ (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    ಯದಿ ((key >> ಕೀಲಿ_bits) |
	((v >> ಮೌಲ್ಯ_bits) ^ (key >> cache_bits)) |
	((ಕೀಲಿ_bits + ಮೌಲ್ಯ_bits - cache_bits == 8 * ಪರಿಮಾಣ (item_t)) && (v == empty)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    *value = v & ((1u<<ಮೌಲ್ಯ_bits)-1);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  HB_HOT
  ಶೂನ್ಯ set (unsigned ಪೂರ್ಣಾಙ್ಕ key, unsigned ಪೂರ್ಣಾಙ್ಕ value)
  {
    ಯದಿ (unlikely ((key >> ಕೀಲಿ_bits) || (value >> ಮೌಲ್ಯ_bits)))
      ಪ್ರತ್ಯಾಗಮ; /* Overflows */
    unsigned ಪೂರ್ಣಾಙ್ಕ k = key & ((1u<<cache_bits)-1);
    unsigned ಪೂರ್ಣಾಙ್ಕ v = ((key>>cache_bits)<<ಮೌಲ್ಯ_bits) | value;
    values[k] = v;
  }

  ಗುಪ್ತ:
  item_t values[1u<<cache_bits];
};


#endif /* HB_CACHE_HH */
