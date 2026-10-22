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

#include "hb-cache.hh"

template <unsigned सुसि_bits, unsigned हालागा_bits, unsigned cache_bits, bool thread_safe>
static void
test_cache ()
{
  hb_cache_t<सुसि_bits, हालागा_bits, cache_bits, thread_safe> cache;
  unsigned सुसि_max = (1u << सुसि_bits) - 1;
  unsigned value = 42;
  unsigned मगा_सुसि[] = {0, 1, 255, 256, सुसि_max};
  unsigned overflow_मगा_सुसि[] = {सुसि_max + 1, 32512, 32767, 65535, UINT_MAX};

  for (unsigned key : मगा_सुसि)
    hb_always_assert (!cache.get (key, &value));

  for (unsigned key : overflow_मगा_सुसि)
    if (key > सुसि_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  for (unsigned key : मगा_सुसि)
  {
    cache.set (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.set (42, cache.MAX_हालागा);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_हालागा);

  for (unsigned key : overflow_मगा_सुसि)
    if (key > सुसि_max)
    {
      cache.set (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.set (42, cache.MAX_हालागा + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_हालागा);

  cache.clear ();
  for (unsigned key : मगा_सुसि)
    hb_always_assert (!cache.get (key, &value));
}

template <bool thread_safe>
static void
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

int
main (int argc, char **argv)
{
  test_caches<true> ();
  test_caches<false> ();
}
