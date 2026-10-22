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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cache.hh"

ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੰਜੀ_ਬਿੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ_ਬਿੱਟ, ਬਿਨਾਨਿਸ਼ਾਨੀ cache_ਬਿੱਟ, ਤਾਰਕਿਕ thread_ਸੁਰੱਖਿਅਤ>
ਸਥਿਰ ਖਾਲੀ
test_cache ()
{
  hb_cache_t<ਕੁੰਜੀ_ਬਿੱਟ, ਮੁੱਲ_ਬਿੱਟ, cache_ਬਿੱਟ, thread_ਸੁਰੱਖਿਅਤ> cache;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੰਜੀ_max = (1u << ਕੁੰਜੀ_ਬਿੱਟ) - 1;
  ਬਿਨਾਨਿਸ਼ਾਨੀ value = 42;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੰਜੀਆਂ[] = {0, 1, 255, 256, ਕੁੰਜੀ_max};
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਛਾਲ_ਕੁੰਜੀਆਂ[] = {ਕੁੰਜੀ_max + 1, 32512, 32767, 65535, UINT_MAX};

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ key : ਕੁੰਜੀਆਂ)
    hb_always_assert (!cache.get (key, &value));

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ key : ਉਛਾਲ_ਕੁੰਜੀਆਂ)
    ਜੇ (key > ਕੁੰਜੀ_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ key : ਕੁੰਜੀਆਂ)
  {
    cache.ਸੈੱਟ (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.ਸੈੱਟ (42, cache.MAX_ਮੁੱਲ);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_ਮੁੱਲ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ key : ਉਛਾਲ_ਕੁੰਜੀਆਂ)
    ਜੇ (key > ਕੁੰਜੀ_max)
    {
      cache.ਸੈੱਟ (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.ਸੈੱਟ (42, cache.MAX_ਮੁੱਲ + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_ਮੁੱਲ);

  cache.clear ();
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ key : ਕੁੰਜੀਆਂ)
    hb_always_assert (!cache.get (key, &value));
}

ਸਾਂਚਾ <ਤਾਰਕਿਕ thread_ਸੁਰੱਖਿਅਤ>
ਸਥਿਰ ਖਾਲੀ
test_caches ()
{
  /* Entries with spare bits, and entries that fill the storage type. */
  test_cache<14, 1, 8, thread_ਸੁਰੱਖਿਅਤ> ();
  test_cache<14, 1, 7, thread_ਸੁਰੱਖਿਅਤ> ();
  test_cache<14, 8, 8, thread_ਸੁਰੱਖਿਅਤ> ();
  test_cache<16, 8, 8, thread_ਸੁਰੱਖਿਅਤ> ();
  test_cache<24, 8, 8, thread_ਸੁਰੱਖਿਅਤ> ();
  test_cache<24, 16, 8, thread_ਸੁਰੱਖਿਅਤ> ();
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  test_caches<ਸੱਚ> ();
  test_caches<ਝੂਠ> ();
}
