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

#ଅନ୍ତର୍ଭାଵ "hb.hh"


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

template <unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି_bits=16,
	 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_bits=8 + 32 - ଚାବି_bits,
	 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cache_bits=8,
	 ତାର୍କିକ thread_safe=ସତ୍ୟ>
ସଂରଚନା hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<ଚାବି_bits + ମୂଲ୍ୟ_bits - cache_bits <= 8,
								     hb_atomic_t<unsigned ଅକ୍ଷର>,
								     typename std::conditional<ଚାବି_bits + ମୂଲ୍ୟ_bits - cache_bits <= 16,
											       hb_atomic_t<unsigned short>,
											       hb_atomic_t<unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::type>::type,
					   typename std::conditional<ଚାବି_bits + ମୂଲ୍ୟ_bits - cache_bits <= 8,
								     unsigned ଅକ୍ଷର,
								     typename std::conditional<ଚାବି_bits + ମୂଲ୍ୟ_bits - cache_bits <= 16,
											       unsigned short,
											       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::type>::type
					  >::type;

  static_assert ((ଚାବି_bits >= cache_bits), "");
  static_assert ((ଚାବି_bits + ମୂଲ୍ୟ_bits <= cache_bits + 8 * ଆକାର (item_t)), "");

  ସ୍ଥିର constexpr unsigned MAX_ମୂଲ୍ୟ = (1u << ମୂଲ୍ୟ_bits) - 1;

  hb_cache_t () { clear (); }

  ଶୂନ୍ୟ clear ()
  {
    ପାଇଁ (auto &v : values)
      v = -1;
  }

  HB_HOT
  ତାର୍କିକ get (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା key, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *value) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = key & ((1u<<cache_bits)-1);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = values[k];
    constexpr unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା empty = (1ULL << (8 * ଆକାର (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    ଯଦି ((key >> ଚାବି_bits) |
	((v >> ମୂଲ୍ୟ_bits) ^ (key >> cache_bits)) |
	((ଚାବି_bits + ମୂଲ୍ୟ_bits - cache_bits == 8 * ଆକାର (item_t)) && (v == empty)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    *value = v & ((1u<<ମୂଲ୍ୟ_bits)-1);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  HB_HOT
  ଶୂନ୍ୟ set (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା key, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
  {
    ଯଦି (unlikely ((key >> ଚାବି_bits) || (value >> ମୂଲ୍ୟ_bits)))
      ଫେରନ୍ତୁ; /* Overflows */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = key & ((1u<<cache_bits)-1);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = ((key>>cache_bits)<<ମୂଲ୍ୟ_bits) | value;
    values[k] = v;
  }

  ବ୍ୟକ୍ତିଗତ:
  item_t values[1u<<cache_bits];
};


#endif /* HB_CACHE_HH */
