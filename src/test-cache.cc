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

#અન્તર્ભાવ "hb-cache.hh"

template <unsigned ચાવી_bits, unsigned કિંમત_bits, unsigned cache_bits, તાર્કિક thread_safe>
સ્થિર શૂન્ય
test_cache ()
{
  hb_cache_t<ચાવી_bits, કિંમત_bits, cache_bits, thread_safe> cache;
  unsigned ચાવી_max = (1u << ચાવી_bits) - 1;
  unsigned value = 42;
  unsigned ચાવીઓ[] = {0, 1, 255, 256, ચાવી_max};
  unsigned overflow_ચાવીઓ[] = {ચાવી_max + 1, 32512, 32767, 65535, UINT_MAX};

  માટે (unsigned key : ચાવીઓ)
    hb_always_assert (!cache.get (key, &value));

  માટે (unsigned key : overflow_ચાવીઓ)
    જો (key > ચાવી_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  માટે (unsigned key : ચાવીઓ)
  {
    cache.set (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.set (42, cache.MAX_કિંમત);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_કિંમત);

  માટે (unsigned key : overflow_ચાવીઓ)
    જો (key > ચાવી_max)
    {
      cache.set (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.set (42, cache.MAX_કિંમત + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_કિંમત);

  cache.clear ();
  માટે (unsigned key : ચાવીઓ)
    hb_always_assert (!cache.get (key, &value));
}

template <તાર્કિક thread_safe>
સ્થિર શૂન્ય
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  test_caches<સાચું> ();
  test_caches<ખોટું> ();
}
