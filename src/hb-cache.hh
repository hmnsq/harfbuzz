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

ढाँचा <अचिन्हित पूर्णाङ्क कुंजी_bits=16,
	 अचिन्हित पूर्णाङ्क मूल्य_bits=8 + 32 - कुंजी_bits,
	 अचिन्हित पूर्णाङ्क cache_bits=8,
	 तार्किक thread_safe=सत्य>
संरचना hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<कुंजी_bits + मूल्य_bits - cache_bits <= 8,
								     hb_atomic_t<अचिन्हित वर्ण>,
								     typename std::conditional<कुंजी_bits + मूल्य_bits - cache_bits <= 16,
											       hb_atomic_t<अचिन्हित लघु>,
											       hb_atomic_t<अचिन्हित पूर्णाङ्क>>::type>::type,
					   typename std::conditional<कुंजी_bits + मूल्य_bits - cache_bits <= 8,
								     अचिन्हित वर्ण,
								     typename std::conditional<कुंजी_bits + मूल्य_bits - cache_bits <= 16,
											       अचिन्हित लघु,
											       अचिन्हित पूर्णाङ्क>::type>::type
					  >::type;

  static_assert ((कुंजी_bits >= cache_bits), "");
  static_assert ((कुंजी_bits + मूल्य_bits <= cache_bits + 8 * परिमाण (item_t)), "");

  स्थिर constexpr अचिन्हित MAX_मूल्य = (1u << मूल्य_bits) - 1;

  hb_cache_t () { clear (); }

  शून्य clear ()
  {
    प्रति (स्वतः &v : values)
      v = -1;
  }

  HB_HOT
  तार्किक get (अचिन्हित पूर्णाङ्क key, अचिन्हित पूर्णाङ्क *value) ध्रुव
  {
    अचिन्हित पूर्णाङ्क k = key & ((1u<<cache_bits)-1);
    अचिन्हित पूर्णाङ्क v = values[k];
    constexpr अचिन्हित पूर्णाङ्क empty = (1ULL << (8 * परिमाण (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    यदि ((key >> कुंजी_bits) |
	((v >> मूल्य_bits) ^ (key >> cache_bits)) |
	((कुंजी_bits + मूल्य_bits - cache_bits == 8 * परिमाण (item_t)) && (v == empty)))
      प्रत्यागम असत्य;
    *value = v & ((1u<<मूल्य_bits)-1);
    प्रत्यागम सत्य;
  }

  HB_HOT
  शून्य set (अचिन्हित पूर्णाङ्क key, अचिन्हित पूर्णाङ्क value)
  {
    यदि (unlikely ((key >> कुंजी_bits) || (value >> मूल्य_bits)))
      प्रत्यागम; /* Overflows */
    अचिन्हित पूर्णाङ्क k = key & ((1u<<cache_bits)-1);
    अचिन्हित पूर्णाङ्क v = ((key>>cache_bits)<<मूल्य_bits) | value;
    values[k] = v;
  }

  गुप्त:
  item_t values[1u<<cache_bits];
};


#endif /* HB_CACHE_HH */
