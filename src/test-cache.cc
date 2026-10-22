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

#அந்தர்பாவ "hb-cache.hh"

template <unsigned சாவி_bits, unsigned மதிப்பு_bits, unsigned cache_bits, தருக்க thread_safe>
நிலையான வெறுமை
test_cache ()
{
  hb_cache_t<சாவி_bits, மதிப்பு_bits, cache_bits, thread_safe> cache;
  unsigned சாவி_max = (1u << சாவி_bits) - 1;
  unsigned value = 42;
  unsigned சாவிகள்[] = {0, 1, 255, 256, சாவி_max};
  unsigned overflow_சாவிகள்[] = {சாவி_max + 1, 32512, 32767, 65535, UINT_MAX};

  க்கு (unsigned key : சாவிகள்)
    hb_always_assert (!cache.get (key, &value));

  க்கு (unsigned key : overflow_சாவிகள்)
    எனில் (key > சாவி_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  க்கு (unsigned key : சாவிகள்)
  {
    cache.set (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.set (42, cache.MAX_மதிப்பு);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_மதிப்பு);

  க்கு (unsigned key : overflow_சாவிகள்)
    எனில் (key > சாவி_max)
    {
      cache.set (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.set (42, cache.MAX_மதிப்பு + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_மதிப்பு);

  cache.clear ();
  க்கு (unsigned key : சாவிகள்)
    hb_always_assert (!cache.get (key, &value));
}

template <தருக்க thread_safe>
நிலையான வெறுமை
test_caches ()
{
  /* Entries with spare bits, and entries that fill the storage type. */
  test_cache<14, 1, 8, thread_safe> ();
  test_cache<14, 1, 7, thread_safe> ();
  test_cache<14, 8, 8, thread_safe> ();
  test_cache<16, 8, 8, thread_safe> ();
  test_cache<24, 8, 8, thread_safe> ();
  test_cache<24, 16, 8, thread_safe> ();
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  test_caches<உண்மை> ();
  test_caches<பொய்> ();
}
