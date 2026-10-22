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

#ಅನ್ತರ್ಭಾವ "hb-cache.hh"

template <unsigned ಕೀಲಿ_bits, unsigned ಮೌಲ್ಯ_bits, unsigned cache_bits, ತಾರ್ಕಿಕ thread_safe>
ಸ್ಥಿರ ಶೂನ್ಯ
test_cache ()
{
  hb_cache_t<ಕೀಲಿ_bits, ಮೌಲ್ಯ_bits, cache_bits, thread_safe> cache;
  unsigned ಕೀಲಿ_max = (1u << ಕೀಲಿ_bits) - 1;
  unsigned value = 42;
  unsigned ಕೀಲಿಗಳು[] = {0, 1, 255, 256, ಕೀಲಿ_max};
  unsigned overflow_ಕೀಲಿಗಳು[] = {ಕೀಲಿ_max + 1, 32512, 32767, 65535, UINT_MAX};

  ಪ್ರತಿ (unsigned key : ಕೀಲಿಗಳು)
    hb_always_assert (!cache.get (key, &value));

  ಪ್ರತಿ (unsigned key : overflow_ಕೀಲಿಗಳು)
    ಯದಿ (key > ಕೀಲಿ_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  ಪ್ರತಿ (unsigned key : ಕೀಲಿಗಳು)
  {
    cache.set (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.set (42, cache.MAX_ಮೌಲ್ಯ);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_ಮೌಲ್ಯ);

  ಪ್ರತಿ (unsigned key : overflow_ಕೀಲಿಗಳು)
    ಯದಿ (key > ಕೀಲಿ_max)
    {
      cache.set (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.set (42, cache.MAX_ಮೌಲ್ಯ + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_ಮೌಲ್ಯ);

  cache.clear ();
  ಪ್ರತಿ (unsigned key : ಕೀಲಿಗಳು)
    hb_always_assert (!cache.get (key, &value));
}

template <ತಾರ್ಕಿಕ thread_safe>
ಸ್ಥಿರ ಶೂನ್ಯ
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

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  test_caches<ಸತ್ಯ> ();
  test_caches<ಅಸತ್ಯ> ();
}
