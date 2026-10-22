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

#अगर_अघोषित HB_CACHE_HH
#घोषणा HB_CACHE_HH

#समावेश "hb.hh"


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

ढाँचा <अचिन्हित पूर्णांक कुंजी_bits=16,
	 अचिन्हित पूर्णांक मूल्य_bits=8 + 32 - कुंजी_bits,
	 अचिन्हित पूर्णांक cache_bits=8,
	 bool thread_safe=true>
काष्ठा hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<कुंजी_bits + मूल्य_bits - cache_bits <= 8,
								     hb_atomic_t<अचिन्हित अक्षर>,
								     typename std::conditional<कुंजी_bits + मूल्य_bits - cache_bits <= 16,
											       hb_atomic_t<अचिन्हित लघु>,
											       hb_atomic_t<अचिन्हित पूर्णांक>>::type>::type,
					   typename std::conditional<कुंजी_bits + मूल्य_bits - cache_bits <= 8,
								     अचिन्हित अक्षर,
								     typename std::conditional<कुंजी_bits + मूल्य_bits - cache_bits <= 16,
											       अचिन्हित लघु,
											       अचिन्हित पूर्णांक>::type>::type
					  >::type;

  static_assert ((कुंजी_bits >= cache_bits), "");
  static_assert ((कुंजी_bits + मूल्य_bits <= cache_bits + 8 * माप (item_t)), "");

  अटल constexpr अचिन्हित MAX_मूल्य = (1u << मूल्य_bits) - 1;

  hb_cache_t () { सफा (); }

  व्योम सफा ()
  {
    क्रम (स्वतः &v : values)
      v = -1;
  }

  HB_HOT
  bool लो (अचिन्हित पूर्णांक key, अचिन्हित पूर्णांक *value) स्थिर
  {
    अचिन्हित पूर्णांक k = key & ((1u<<cache_bits)-1);
    अचिन्हित पूर्णांक v = values[k];
    constexpr अचिन्हित पूर्णांक empty = (1ULL << (8 * माप (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    अगर ((key >> कुंजी_bits) |
	((v >> मूल्य_bits) ^ (key >> cache_bits)) |
	((कुंजी_bits + मूल्य_bits - cache_bits == 8 * माप (item_t)) && (v == empty)))
      वापस false;
    *value = v & ((1u<<मूल्य_bits)-1);
    वापस true;
  }

  HB_HOT
  व्योम set (अचिन्हित पूर्णांक key, अचिन्हित पूर्णांक value)
  {
    अगर (unlikely ((key >> कुंजी_bits) || (value >> मूल्य_bits)))
      वापस; /* Overflows */
    अचिन्हित पूर्णांक k = key & ((1u<<cache_bits)-1);
    अचिन्हित पूर्णांक v = ((key>>cache_bits)<<मूल्य_bits) | value;
    values[k] = v;
  }

  निजी:
  item_t values[1u<<cache_bits];
};


#पूर्ण_अगर /* HB_CACHE_HH */
