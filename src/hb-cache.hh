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

#جيڪڏهن_وصف_نه_ٿيل HB_CACHE_HH
#وصف_ڪريو HB_CACHE_HH

#ضمن "hb.hh"


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

سانچو <بنانشان صحيح ڪنجي_بٽ=16,
	 بنانشان صحيح قدر_بٽ=8 + 32 - ڪنجي_بٽ,
	 بنانشان صحيح cache_بٽ=8,
	 منطقي thread_محفوظ=سچ>
بنية hb_cache_t
{
  استعمال شي_t = قسم_نالو std::conditional<thread_محفوظ,
					   قسم_نالو std::conditional<ڪنجي_بٽ + قدر_بٽ - cache_بٽ <= 8,
								     hb_atomic_t<بنانشان حرف>,
								     قسم_نالو std::conditional<ڪنجي_بٽ + قدر_بٽ - cache_بٽ <= 16,
											       hb_atomic_t<بنانشان ننڍو>,
											       hb_atomic_t<بنانشان صحيح>>::type>::type,
					   قسم_نالو std::conditional<ڪنجي_بٽ + قدر_بٽ - cache_بٽ <= 8,
								     بنانشان حرف,
								     قسم_نالو std::conditional<ڪنجي_بٽ + قدر_بٽ - cache_بٽ <= 16,
											       بنانشان ننڍو,
											       بنانشان صحيح>::type>::type
					  >::type;

  static_assert ((ڪنجي_بٽ >= cache_بٽ), "");
  static_assert ((ڪنجي_بٽ + قدر_بٽ <= cache_بٽ + 8 * حجم (شي_t)), "");

  ساكن constexpr بنانشان MAX_قدر = (1u << قدر_بٽ) - 1;

  hb_cache_t () { clear (); }

  فراغ clear ()
  {
    لكل (خودڪار &v : values)
      v = -1;
  }

  HB_HOT
  منطقي get (بنانشان صحيح key, بنانشان صحيح *value) ثابت
  {
    بنانشان صحيح k = key & ((1u<<cache_بٽ)-1);
    بنانشان صحيح v = values[k];
    constexpr بنانشان صحيح خالي_جايون_2 = (1ULL << (8 * حجم (شي_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    إذا ((key >> ڪنجي_بٽ) |
	((v >> قدر_بٽ) ^ (key >> cache_بٽ)) |
	((ڪنجي_بٽ + قدر_بٽ - cache_بٽ == 8 * حجم (شي_t)) && (v == خالي_جايون_2)))
      أرجع خطأ;
    *value = v & ((1u<<قدر_بٽ)-1);
    أرجع سچ;
  }

  HB_HOT
  فراغ سيٽ (بنانشان صحيح key, بنانشان صحيح value)
  {
    إذا (unlikely ((key >> ڪنجي_بٽ) || (value >> قدر_بٽ)))
      أرجع; /* Overflows */
    بنانشان صحيح k = key & ((1u<<cache_بٽ)-1);
    بنانشان صحيح v = ((key>>cache_بٽ)<<قدر_بٽ) | value;
    values[k] = v;
  }

  خاص:
  شي_t values[1u<<cache_بٽ];
};


#جيڪڏهن_پورو /* HB_CACHE_HH */
