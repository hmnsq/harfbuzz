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

#که_نه_تعریف_شوی HB_CACHE_HH
#تعریف_کړه HB_CACHE_HH

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

کينډۍ <بینښه صحيح کلی_بټونه=16,
	 بینښه صحيح ارزښت_بټونه=8 + 32 - کلی_بټونه,
	 بینښه صحيح cache_بټونه=8,
	 منطقي thread_خوندي=ریښتیا>
بنية hb_cache_t
{
  کارول توکی_t = ډول_نوم std::conditional<thread_خوندي,
					   ډول_نوم std::conditional<کلی_بټونه + ارزښت_بټونه - cache_بټونه <= 8,
								     hb_atomic_t<بینښه حرف>,
								     ډول_نوم std::conditional<کلی_بټونه + ارزښت_بټونه - cache_بټونه <= 16,
											       hb_atomic_t<بینښه لنډ>,
											       hb_atomic_t<بینښه صحيح>>::type>::type,
					   ډول_نوم std::conditional<کلی_بټونه + ارزښت_بټونه - cache_بټونه <= 8,
								     بینښه حرف,
								     ډول_نوم std::conditional<کلی_بټونه + ارزښت_بټونه - cache_بټونه <= 16,
											       بینښه لنډ,
											       بینښه صحيح>::type>::type
					  >::type;

  static_assert ((کلی_بټونه >= cache_بټونه), "");
  static_assert ((کلی_بټونه + ارزښت_بټونه <= cache_بټونه + 8 * حجم (توکی_t)), "");

  ساكن constexpr بینښه MAX_ارزښت = (1u << ارزښت_بټونه) - 1;

  hb_cache_t () { clear (); }

  فراغ clear ()
  {
    لكل (خپلکاری &v : values)
      v = -1;
  }

  HB_HOT
  منطقي get (بینښه صحيح key, بینښه صحيح *value) ثابت
  {
    بینښه صحيح k = key & ((1u<<cache_بټونه)-1);
    بینښه صحيح v = values[k];
    constexpr بینښه صحيح تشه_2 = (1ULL << (8 * حجم (توکی_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    إذا ((key >> کلی_بټونه) |
	((v >> ارزښت_بټونه) ^ (key >> cache_بټونه)) |
	((کلی_بټونه + ارزښت_بټونه - cache_بټونه == 8 * حجم (توکی_t)) && (v == تشه_2)))
      أرجع خطأ;
    *value = v & ((1u<<ارزښت_بټونه)-1);
    أرجع ریښتیا;
  }

  HB_HOT
  فراغ سېټ (بینښه صحيح key, بینښه صحيح value)
  {
    إذا (unlikely ((key >> کلی_بټونه) || (value >> ارزښت_بټونه)))
      أرجع; /* Overflows */
    بینښه صحيح k = key & ((1u<<cache_بټونه)-1);
    بینښه صحيح v = ((key>>cache_بټونه)<<ارزښت_بټونه) | value;
    values[k] = v;
  }

  خاص:
  توکی_t values[1u<<cache_بټونه];
};


#د_که_پای /* HB_CACHE_HH */
