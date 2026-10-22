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

#ضمن "hb-cache.hh"

سانچو <بنانشان ڪنجي_بٽ, بنانشان قدر_بٽ, بنانشان cache_بٽ, منطقي thread_محفوظ>
ساكن فراغ
test_cache ()
{
  hb_cache_t<ڪنجي_بٽ, قدر_بٽ, cache_بٽ, thread_محفوظ> cache;
  بنانشان ڪنجي_max = (1u << ڪنجي_بٽ) - 1;
  بنانشان value = 42;
  بنانشان ڪنجيون[] = {0, 1, 255, 256, ڪنجي_max};
  بنانشان حد_کان_مٿي_ڪنجيون[] = {ڪنجي_max + 1, 32512, 32767, 65535, UINT_MAX};

  لكل (بنانشان key : ڪنجيون)
    hb_always_assert (!cache.get (key, &value));

  لكل (بنانشان key : حد_کان_مٿي_ڪنجيون)
    إذا (key > ڪنجي_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  لكل (بنانشان key : ڪنجيون)
  {
    cache.سيٽ (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.سيٽ (42, cache.MAX_قدر);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_قدر);

  لكل (بنانشان key : حد_کان_مٿي_ڪنجيون)
    إذا (key > ڪنجي_max)
    {
      cache.سيٽ (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.سيٽ (42, cache.MAX_قدر + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_قدر);

  cache.clear ();
  لكل (بنانشان key : ڪنجيون)
    hb_always_assert (!cache.get (key, &value));
}

سانچو <منطقي thread_محفوظ>
ساكن فراغ
test_caches ()
{
  /* Entries with spare bits, and entries that fill the storage type. */
  test_cache<14, 1, 8, thread_محفوظ> ();
  test_cache<14, 1, 7, thread_محفوظ> ();
  test_cache<14, 8, 8, thread_محفوظ> ();
  test_cache<16, 8, 8, thread_محفوظ> ();
  test_cache<24, 8, 8, thread_محفوظ> ();
  test_cache<24, 16, 8, thread_محفوظ> ();
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  test_caches<سچ> ();
  test_caches<خطأ> ();
}
