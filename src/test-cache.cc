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

#അന്തര്ഭാവ "hb-cache.hh"

template <unsigned താക്കോൽ_bits, unsigned മൂല്യം_bits, unsigned cache_bits, തർക്കം thread_safe>
സ്ഥിതം ശൂന്യം
test_cache ()
{
  hb_cache_t<താക്കോൽ_bits, മൂല്യം_bits, cache_bits, thread_safe> cache;
  unsigned താക്കോൽ_max = (1u << താക്കോൽ_bits) - 1;
  unsigned value = 42;
  unsigned താക്കോലുകൾ[] = {0, 1, 255, 256, താക്കോൽ_max};
  unsigned overflow_താക്കോലുകൾ[] = {താക്കോൽ_max + 1, 32512, 32767, 65535, UINT_MAX};

  വേണ്ടി (unsigned key : താക്കോലുകൾ)
    hb_always_assert (!cache.get (key, &value));

  വേണ്ടി (unsigned key : overflow_താക്കോലുകൾ)
    എങ്കിൽ (key > താക്കോൽ_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  വേണ്ടി (unsigned key : താക്കോലുകൾ)
  {
    cache.set (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.set (42, cache.MAX_മൂല്യം);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_മൂല്യം);

  വേണ്ടി (unsigned key : overflow_താക്കോലുകൾ)
    എങ്കിൽ (key > താക്കോൽ_max)
    {
      cache.set (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.set (42, cache.MAX_മൂല്യം + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_മൂല്യം);

  cache.clear ();
  വേണ്ടി (unsigned key : താക്കോലുകൾ)
    hb_always_assert (!cache.get (key, &value));
}

template <തർക്കം thread_safe>
സ്ഥിതം ശൂന്യം
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  test_caches<സത്യം> ();
  test_caches<അസത്യം> ();
}
