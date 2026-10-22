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

#నిర్వచితం_కాకపోతే HB_CACHE_HH
#నిర్వచించు HB_CACHE_HH

#అన్తర్భావ "hb.hh"


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

మూస <గుర్తులేని పూర్ణాంకం కీలకం_బిట్లు=16,
	 గుర్తులేని పూర్ణాంకం విలువ_బిట్లు=8 + 32 - కీలకం_బిట్లు,
	 గుర్తులేని పూర్ణాంకం cache_బిట్లు=8,
	 తార్కిక thread_సురక్షిత=నిజం>
నిర్మాణం hb_cache_t
{
  ఉపయోగించి అంశం_t = రకం_పేరు std::conditional<thread_సురక్షిత,
					   రకం_పేరు std::conditional<కీలకం_బిట్లు + విలువ_బిట్లు - cache_బిట్లు <= 8,
								     hb_atomic_t<గుర్తులేని అక్షరం>,
								     రకం_పేరు std::conditional<కీలకం_బిట్లు + విలువ_బిట్లు - cache_బిట్లు <= 16,
											       hb_atomic_t<గుర్తులేని పొట్టి>,
											       hb_atomic_t<గుర్తులేని పూర్ణాంకం>>::type>::type,
					   రకం_పేరు std::conditional<కీలకం_బిట్లు + విలువ_బిట్లు - cache_బిట్లు <= 8,
								     గుర్తులేని అక్షరం,
								     రకం_పేరు std::conditional<కీలకం_బిట్లు + విలువ_బిట్లు - cache_బిట్లు <= 16,
											       గుర్తులేని పొట్టి,
											       గుర్తులేని పూర్ణాంకం>::type>::type
					  >::type;

  static_assert ((కీలకం_బిట్లు >= cache_బిట్లు), "");
  static_assert ((కీలకం_బిట్లు + విలువ_బిట్లు <= cache_బిట్లు + 8 * పరిమాణం (అంశం_t)), "");

  స్థిర constexpr గుర్తులేని MAX_విలువ = (1u << విలువ_బిట్లు) - 1;

  hb_cache_t () { clear (); }

  శూన్య clear ()
  {
    కొరకు (స్వయంచాలక &v : values)
      v = -1;
  }

  HB_HOT
  తార్కిక get (గుర్తులేని పూర్ణాంకం key, గుర్తులేని పూర్ణాంకం *value) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం k = key & ((1u<<cache_బిట్లు)-1);
    గుర్తులేని పూర్ణాంకం v = values[k];
    constexpr గుర్తులేని పూర్ణాంకం ఖాళీలు_2 = (1ULL << (8 * పరిమాణం (అంశం_t))) - 1;
    /* Combine overflow, tag mismatch, and empty checks into one branch. */
    ఒకవేళ ((key >> కీలకం_బిట్లు) |
	((v >> విలువ_బిట్లు) ^ (key >> cache_బిట్లు)) |
	((కీలకం_బిట్లు + విలువ_బిట్లు - cache_బిట్లు == 8 * పరిమాణం (అంశం_t)) && (v == ఖాళీలు_2)))
      తిరిగి అబద్ధం;
    *value = v & ((1u<<విలువ_బిట్లు)-1);
    తిరిగి నిజం;
  }

  HB_HOT
  శూన్య సమితి (గుర్తులేని పూర్ణాంకం key, గుర్తులేని పూర్ణాంకం value)
  {
    ఒకవేళ (unlikely ((key >> కీలకం_బిట్లు) || (value >> విలువ_బిట్లు)))
      తిరిగి; /* Overflows */
    గుర్తులేని పూర్ణాంకం k = key & ((1u<<cache_బిట్లు)-1);
    గుర్తులేని పూర్ణాంకం v = ((key>>cache_బిట్లు)<<విలువ_బిట్లు) | value;
    values[k] = v;
  }

  అంతరంగ:
  అంశం_t values[1u<<cache_బిట్లు];
};


#అయితే_ముగింపు /* HB_CACHE_HH */
