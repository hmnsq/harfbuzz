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

#அந்தர்பாவ "hb.hh"


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

template <unsigned முழு சாவி_bits=16,
	 unsigned முழு மதிப்பு_bits=8 + 32 - சாவி_bits,
	 unsigned முழு cache_bits=8,
	 தருக்க thread_safe=உண்மை>
கட்டமைப்பு hb_cache_t
{
  using item_t = typename std::conditional<thread_safe,
					   typename std::conditional<சாவி_bits + மதிப்பு_bits - cache_bits <= 8,
								     hb_atomic_t<unsigned எழுத்து>,
								     typename std::conditional<சாவி_bits + மதிப்பு_bits - cache_bits <= 16,
											       hb_atomic_t<unsigned short>,
											       hb_atomic_t<unsigned முழு>>::type>::type,
					   typename std::conditional<சாவி_bits + மதிப்பு_bits - cache_bits <= 8,
								     unsigned எழுத்து,
								     typename std::conditional<சாவி_bits + மதிப்பு_bits - cache_bits <= 16,
											       unsigned short,
											       unsigned முழு>::type>::type
					  >::type;

  static_assert ((சாவி_bits >= cache_bits), "");
  static_assert ((சாவி_bits + மதிப்பு_bits <= cache_bits + 8 * அளவு (item_t)), "");

  நிலையான constexpr unsigned MAX_மதிப்பு = (1u << மதிப்பு_bits) - 1;

  hb_cache_t () { clear (); }

  வெறுமை clear ()
  {
    க்கு (auto &v : values)
      v = -1;
  }

  HB_HOT
  தருக்க get (unsigned முழு key, unsigned முழு *value) மாறிலி
  {
    unsigned முழு k = key & ((1u<<cache_bits)-1);
    unsigned முழு v = values[k];
    constexpr unsigned முழு empty = (1ULL << (8 * அளவு (item_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    எனில் ((key >> சாவி_bits) |
	((v >> மதிப்பு_bits) ^ (key >> cache_bits)) |
	((சாவி_bits + மதிப்பு_bits - cache_bits == 8 * அளவு (item_t)) && (v == empty)))
      திரும்பு பொய்;
    *value = v & ((1u<<மதிப்பு_bits)-1);
    திரும்பு உண்மை;
  }

  HB_HOT
  வெறுமை set (unsigned முழு key, unsigned முழு value)
  {
    எனில் (unlikely ((key >> சாவி_bits) || (value >> மதிப்பு_bits)))
      திரும்பு; /* Overflows */
    unsigned முழு k = key & ((1u<<cache_bits)-1);
    unsigned முழு v = ((key>>cache_bits)<<மதிப்பு_bits) | value;
    values[k] = v;
  }

  தனிப்பட்ட:
  item_t values[1u<<cache_bits];
};


#endif /* HB_CACHE_HH */
