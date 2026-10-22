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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_CACHE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_CACHE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"


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

ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕੁੰਜੀ_ਬਿੱਟ=16,
	 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ_ਬਿੱਟ=8 + 32 - ਕੁੰਜੀ_ਬਿੱਟ,
	 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cache_ਬਿੱਟ=8,
	 ਤਾਰਕਿਕ thread_ਸੁਰੱਖਿਅਤ=ਸੱਚ>
ਬਣਤਰ hb_cache_t
{
  ਵਰਤੋਂ ਵਸਤੂ_t = ਕਿਸਮ_ਨਾਂ std::conditional<thread_ਸੁਰੱਖਿਅਤ,
					   ਕਿਸਮ_ਨਾਂ std::conditional<ਕੁੰਜੀ_ਬਿੱਟ + ਮੁੱਲ_ਬਿੱਟ - cache_ਬਿੱਟ <= 8,
								     hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>,
								     ਕਿਸਮ_ਨਾਂ std::conditional<ਕੁੰਜੀ_ਬਿੱਟ + ਮੁੱਲ_ਬਿੱਟ - cache_ਬਿੱਟ <= 16,
											       hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ>,
											       hb_atomic_t<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>>::type>::type,
					   ਕਿਸਮ_ਨਾਂ std::conditional<ਕੁੰਜੀ_ਬਿੱਟ + ਮੁੱਲ_ਬਿੱਟ - cache_ਬਿੱਟ <= 8,
								     ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ,
								     ਕਿਸਮ_ਨਾਂ std::conditional<ਕੁੰਜੀ_ਬਿੱਟ + ਮੁੱਲ_ਬਿੱਟ - cache_ਬਿੱਟ <= 16,
											       ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ,
											       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>::type>::type
					  >::type;

  static_assert ((ਕੁੰਜੀ_ਬਿੱਟ >= cache_ਬਿੱਟ), "");
  static_assert ((ਕੁੰਜੀ_ਬਿੱਟ + ਮੁੱਲ_ਬਿੱਟ <= cache_ਬਿੱਟ + 8 * ਆਕਾਰ (ਵਸਤੂ_t)), "");

  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ MAX_ਮੁੱਲ = (1u << ਮੁੱਲ_ਬਿੱਟ) - 1;

  hb_cache_t () { clear (); }

  ਖਾਲੀ clear ()
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ &v : values)
      v = -1;
  }

  HB_HOT
  ਤਾਰਕਿਕ get (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ key, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *value) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = key & ((1u<<cache_ਬਿੱਟ)-1);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v = values[k];
    constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖਾਲੀ_ਥਾਵਾਂ_2 = (1ULL << (8 * ਆਕਾਰ (ਵਸਤੂ_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    ਜੇ ((key >> ਕੁੰਜੀ_ਬਿੱਟ) |
	((v >> ਮੁੱਲ_ਬਿੱਟ) ^ (key >> cache_ਬਿੱਟ)) |
	((ਕੁੰਜੀ_ਬਿੱਟ + ਮੁੱਲ_ਬਿੱਟ - cache_ਬਿੱਟ == 8 * ਆਕਾਰ (ਵਸਤੂ_t)) && (v == ਖਾਲੀ_ਥਾਵਾਂ_2)))
      ਵਾਪਸ ਝੂਠ;
    *value = v & ((1u<<ਮੁੱਲ_ਬਿੱਟ)-1);
    ਵਾਪਸ ਸੱਚ;
  }

  HB_HOT
  ਖਾਲੀ ਸੈੱਟ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ key, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ value)
  {
    ਜੇ (unlikely ((key >> ਕੁੰਜੀ_ਬਿੱਟ) || (value >> ਮੁੱਲ_ਬਿੱਟ)))
      ਵਾਪਸ; /* Overflows */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = key & ((1u<<cache_ਬਿੱਟ)-1);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ v = ((key>>cache_ਬਿੱਟ)<<ਮੁੱਲ_ਬਿੱਟ) | value;
    values[k] = v;
  }

  ਨਿਜੀ:
  ਵਸਤੂ_t values[1u<<cache_ਬਿੱਟ];
};


#ਜੇ_ਅੰਤ /* HB_CACHE_HH */
