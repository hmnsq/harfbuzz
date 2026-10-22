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

#جے_بنا_تعریف HB_CACHE_HH
#تعریف_کرو HB_CACHE_HH

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

سانچا <بنانشانی صحيح کلید_بٹ=16,
	 بنانشانی صحيح قدر_بٹ=8 + 32 - کلید_بٹ,
	 بنانشانی صحيح cache_بٹ=8,
	 منطقي thread_محفوظ=سچ>
بنية hb_cache_t
{
  ورتوں شے_t = قسم_ناں std::conditional<thread_محفوظ,
					   قسم_ناں std::conditional<کلید_بٹ + قدر_بٹ - cache_بٹ <= 8,
								     hb_atomic_t<بنانشانی حرف>,
								     قسم_ناں std::conditional<کلید_بٹ + قدر_بٹ - cache_بٹ <= 16,
											       hb_atomic_t<بنانشانی چھوٹا>,
											       hb_atomic_t<بنانشانی صحيح>>::type>::type,
					   قسم_ناں std::conditional<کلید_بٹ + قدر_بٹ - cache_بٹ <= 8,
								     بنانشانی حرف,
								     قسم_ناں std::conditional<کلید_بٹ + قدر_بٹ - cache_بٹ <= 16,
											       بنانشانی چھوٹا,
											       بنانشانی صحيح>::type>::type
					  >::type;

  static_assert ((کلید_بٹ >= cache_بٹ), "");
  static_assert ((کلید_بٹ + قدر_بٹ <= cache_بٹ + 8 * حجم (شے_t)), "");

  ساكن constexpr بنانشانی MAX_قدر = (1u << قدر_بٹ) - 1;

  hb_cache_t () { clear (); }

  فراغ clear ()
  {
    لكل (خودکار &v : values)
      v = -1;
  }

  HB_HOT
  منطقي get (بنانشانی صحيح key, بنانشانی صحيح *value) ثابت
  {
    بنانشانی صحيح k = key & ((1u<<cache_بٹ)-1);
    بنانشانی صحيح v = values[k];
    constexpr بنانشانی صحيح خالی_تھاواں_2 = (1ULL << (8 * حجم (شے_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    إذا ((key >> کلید_بٹ) |
	((v >> قدر_بٹ) ^ (key >> cache_بٹ)) |
	((کلید_بٹ + قدر_بٹ - cache_بٹ == 8 * حجم (شے_t)) && (v == خالی_تھاواں_2)))
      أرجع خطأ;
    *value = v & ((1u<<قدر_بٹ)-1);
    أرجع سچ;
  }

  HB_HOT
  فراغ سیٹ (بنانشانی صحيح key, بنانشانی صحيح value)
  {
    إذا (unlikely ((key >> کلید_بٹ) || (value >> قدر_بٹ)))
      أرجع; /* Overflows */
    بنانشانی صحيح k = key & ((1u<<cache_بٹ)-1);
    بنانشانی صحيح v = ((key>>cache_بٹ)<<قدر_بٹ) | value;
    values[k] = v;
  }

  خاص:
  شے_t values[1u<<cache_بٹ];
};


#جے_انت /* HB_CACHE_HH */
