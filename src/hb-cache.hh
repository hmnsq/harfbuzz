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

#اگر_تعریف_نشده HB_CACHE_HH
#تعریف_کن HB_CACHE_HH

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

قالب <بیعلامت صحيح کلید_بیتها=16,
	 بیعلامت صحيح مقدار_بیتها=8 + 32 - کلید_بیتها,
	 بیعلامت صحيح cache_بیتها=8,
	 منطقي thread_امن=درست>
بنية hb_cache_t
{
  استفاده قلم_t = نام_نوع std::conditional<thread_امن,
					   نام_نوع std::conditional<کلید_بیتها + مقدار_بیتها - cache_بیتها <= 8,
								     hb_atomic_t<بیعلامت حرف>,
								     نام_نوع std::conditional<کلید_بیتها + مقدار_بیتها - cache_بیتها <= 16,
											       hb_atomic_t<بیعلامت کوتاه>,
											       hb_atomic_t<بیعلامت صحيح>>::type>::type,
					   نام_نوع std::conditional<کلید_بیتها + مقدار_بیتها - cache_بیتها <= 8,
								     بیعلامت حرف,
								     نام_نوع std::conditional<کلید_بیتها + مقدار_بیتها - cache_بیتها <= 16,
											       بیعلامت کوتاه,
											       بیعلامت صحيح>::type>::type
					  >::type;

  static_assert ((کلید_بیتها >= cache_بیتها), "");
  static_assert ((کلید_بیتها + مقدار_بیتها <= cache_بیتها + 8 * حجم (قلم_t)), "");

  ساكن constexpr بیعلامت MAX_مقدار = (1u << مقدار_بیتها) - 1;

  hb_cache_t () { clear (); }

  فراغ clear ()
  {
    لكل (خودکار &v : values)
      v = -1;
  }

  HB_HOT
  منطقي get (بیعلامت صحيح key, بیعلامت صحيح *value) ثابت
  {
    بیعلامت صحيح k = key & ((1u<<cache_بیتها)-1);
    بیعلامت صحيح v = values[k];
    constexpr بیعلامت صحيح خالی_2 = (1ULL << (8 * حجم (قلم_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    إذا ((key >> کلید_بیتها) |
	((v >> مقدار_بیتها) ^ (key >> cache_بیتها)) |
	((کلید_بیتها + مقدار_بیتها - cache_بیتها == 8 * حجم (قلم_t)) && (v == خالی_2)))
      أرجع خطأ;
    *value = v & ((1u<<مقدار_بیتها)-1);
    أرجع درست;
  }

  HB_HOT
  فراغ مجموعه (بیعلامت صحيح key, بیعلامت صحيح value)
  {
    إذا (unlikely ((key >> کلید_بیتها) || (value >> مقدار_بیتها)))
      أرجع; /* Overflows */
    بیعلامت صحيح k = key & ((1u<<cache_بیتها)-1);
    بیعلامت صحيح v = ((key>>cache_بیتها)<<مقدار_بیتها) | value;
    values[k] = v;
  }

  خاص:
  قلم_t values[1u<<cache_بیتها];
};


#پایان_اگر /* HB_CACHE_HH */
