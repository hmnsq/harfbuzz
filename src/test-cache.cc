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

#अन्तर्भाव "hb-cache.hh"

ढाँचा <अचिन्हित किल्ली_bits, अचिन्हित मूल्य_bits, अचिन्हित cache_bits, तार्किक thread_safe>
स्थिर शून्य
test_cache ()
{
  hb_cache_t<किल्ली_bits, मूल्य_bits, cache_bits, thread_safe> cache;
  अचिन्हित किल्ली_max = (1u << किल्ली_bits) - 1;
  अचिन्हित value = 42;
  अचिन्हित किल्ल्या[] = {0, 1, 255, 256, किल्ली_max};
  अचिन्हित overflow_किल्ल्या[] = {किल्ली_max + 1, 32512, 32767, 65535, अच_पूर्णांक_उच्च};

  साठी (अचिन्हित key : किल्ल्या)
    hb_always_assert (!cache.get (key, &value));

  साठी (अचिन्हित key : overflow_किल्ल्या)
    जर (key > किल्ली_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  साठी (अचिन्हित key : किल्ल्या)
  {
    cache.set (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.set (42, cache.MAX_मूल्य);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_मूल्य);

  साठी (अचिन्हित key : overflow_किल्ल्या)
    जर (key > किल्ली_max)
    {
      cache.set (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.set (42, cache.MAX_मूल्य + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_मूल्य);

  cache.clear ();
  साठी (अचिन्हित key : किल्ल्या)
    hb_always_assert (!cache.get (key, &value));
}

ढाँचा <तार्किक thread_safe>
स्थिर शून्य
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

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  test_caches<सत्य> ();
  test_caches<असत्य> ();
}
