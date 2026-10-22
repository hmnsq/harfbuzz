/*
 * Copyright © 2026  Google, Inc.
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
 */

#అన్తర్భావ "hb-cache.hh"

మూస <గుర్తులేని కీలకం_బిట్లు, గుర్తులేని విలువ_బిట్లు, గుర్తులేని cache_బిట్లు, తార్కిక thread_సురక్షిత>
స్థిర శూన్య
test_cache ()
{
  hb_cache_t<కీలకం_బిట్లు, విలువ_బిట్లు, cache_బిట్లు, thread_సురక్షిత> cache;
  గుర్తులేని కీలకం_max = (1u << కీలకం_బిట్లు) - 1;
  గుర్తులేని value = 42;
  గుర్తులేని కీలకాలు[] = {0, 1, 255, 256, కీలకం_max};
  గుర్తులేని అధికప్రవాహం_కీలకాలు[] = {కీలకం_max + 1, 32512, 32767, 65535, UINT_MAX};

  కొరకు (గుర్తులేని key : కీలకాలు)
    hb_always_assert (!cache.get (key, &value));

  కొరకు (గుర్తులేని key : అధికప్రవాహం_కీలకాలు)
    ఒకవేళ (key > కీలకం_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  కొరకు (గుర్తులేని key : కీలకాలు)
  {
    cache.సమితి (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.సమితి (42, cache.MAX_విలువ);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_విలువ);

  కొరకు (గుర్తులేని key : అధికప్రవాహం_కీలకాలు)
    ఒకవేళ (key > కీలకం_max)
    {
      cache.సమితి (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.సమితి (42, cache.MAX_విలువ + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_విలువ);

  cache.clear ();
  కొరకు (గుర్తులేని key : కీలకాలు)
    hb_always_assert (!cache.get (key, &value));
}

మూస <తార్కిక thread_సురక్షిత>
స్థిర శూన్య
test_caches ()
{
  /* Entries with spare bits, and entries that fill the storage type. */
  test_cache<14, 1, 8, thread_సురక్షిత> ();
  test_cache<14, 1, 7, thread_సురక్షిత> ();
  test_cache<14, 8, 8, thread_సురక్షిత> ();
  test_cache<16, 8, 8, thread_సురక్షిత> ();
  test_cache<24, 8, 8, thread_సురక్షిత> ();
  test_cache<24, 16, 8, thread_సురక్షిత> ();
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_caches<నిజం> ();
  test_caches<అబద్ధం> ();
}
