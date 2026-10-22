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

#अन्तर्भाव "hb.hh"


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

template <unsigned पूर्णाङ्क कुञ्चिका_bits=16,
	 unsigned पूर्णाङ्क मानम्_bits=8 + 32 - कुञ्चिका_bits,
	 unsigned पूर्णाङ्क cache_bits=8,
	 तार्किक thread_safe=सच्च>
संरचना hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<कुञ्चिका_bits + मानम्_bits - cache_bits <= 8,
								     hb_atomic_t<unsigned वर्ण>,
								     typename std::conditional<कुञ्चिका_bits + मानम्_bits - cache_bits <= 16,
											       hb_atomic_t<unsigned short>,
											       hb_atomic_t<unsigned पूर्णाङ्क>>::type>::type,
					   typename std::conditional<कुञ्चिका_bits + मानम्_bits - cache_bits <= 8,
								     unsigned वर्ण,
								     typename std::conditional<कुञ्चिका_bits + मानम्_bits - cache_bits <= 16,
											       unsigned short,
											       unsigned पूर्णाङ्क>::type>::type
					  >::type;

  static_assert ((कुञ्चिका_bits >= cache_bits), "");
  static_assert ((कुञ्चिका_bits + मानम्_bits <= cache_bits + 8 * परिमाण (item_t)), "");

  स्थिर constexpr unsigned MAX_मानम् = (1u << मानम्_bits) - 1;

  hb_cache_t () { clear (); }

  शून्य clear ()
  {
    अत्थाय (auto &v : values)
      v = -1;
  }

  HB_HOT
  तार्किक get (unsigned पूर्णाङ्क key, unsigned पूर्णाङ्क *value) ध्रुव
  {
    unsigned पूर्णाङ्क k = key & ((1u<<cache_bits)-1);
    unsigned पूर्णाङ्क v = values[k];
    constexpr unsigned पूर्णाङ्क empty = (1ULL << (8 * परिमाण (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    सचे ((key >> कुञ्चिका_bits) |
	((v >> मानम्_bits) ^ (key >> cache_bits)) |
	((कुञ्चिका_bits + मानम्_bits - cache_bits == 8 * परिमाण (item_t)) && (v == empty)))
      पटिनिवत्त मिच्छा;
    *value = v & ((1u<<मानम्_bits)-1);
    पटिनिवत्त सच्च;
  }

  HB_HOT
  शून्य set (unsigned पूर्णाङ्क key, unsigned पूर्णाङ्क value)
  {
    सचे (unlikely ((key >> कुञ्चिका_bits) || (value >> मानम्_bits)))
      पटिनिवत्त; /* Overflows */
    unsigned पूर्णाङ्क k = key & ((1u<<cache_bits)-1);
    unsigned पूर्णाङ्क v = ((key>>cache_bits)<<मानम्_bits) | value;
    values[k] = v;
  }

  गुप्त:
  item_t values[1u<<cache_bits];
};


#endif /* HB_CACHE_HH */
