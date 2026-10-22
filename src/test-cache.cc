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

کينډۍ <بینښه کلی_بټونه, بینښه ارزښت_بټونه, بینښه cache_بټونه, منطقي thread_خوندي>
ساكن فراغ
test_cache ()
{
  hb_cache_t<کلی_بټونه, ارزښت_بټونه, cache_بټونه, thread_خوندي> cache;
  بینښه کلی_max = (1u << کلی_بټونه) - 1;
  بینښه value = 42;
  بینښه کلیګانې[] = {0, 1, 255, 256, کلی_max};
  بینښه ډکېدنه_کلیګانې[] = {کلی_max + 1, 32512, 32767, 65535, UINT_MAX};

  لكل (بینښه key : کلیګانې)
    hb_always_assert (!cache.get (key, &value));

  لكل (بینښه key : ډکېدنه_کلیګانې)
    إذا (key > کلی_max)
    {
      hb_always_assert (!cache.get (key, &value));
      hb_always_assert (value == 42);
    }

  لكل (بینښه key : کلیګانې)
  {
    cache.سېټ (key, 0);
    hb_always_assert (cache.get (key, &value) && value == 0);
  }

  cache.سېټ (42, cache.MAX_ارزښت);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_ارزښت);

  لكل (بینښه key : ډکېدنه_کلیګانې)
    إذا (key > کلی_max)
    {
      cache.سېټ (key, 0);
      hb_always_assert (!cache.get (key, &value));
    }

  cache.سېټ (42, cache.MAX_ارزښت + 1);
  hb_always_assert (cache.get (42, &value) && value == cache.MAX_ارزښت);

  cache.clear ();
  لكل (بینښه key : کلیګانې)
    hb_always_assert (!cache.get (key, &value));
}

کينډۍ <منطقي thread_خوندي>
ساكن فراغ
test_caches ()
{
  /* Entries with spare bits, and entries that fill the storage type. */
  test_cache<14, 1, 8, thread_خوندي> ();
  test_cache<14, 1, 7, thread_خوندي> ();
  test_cache<14, 8, 8, thread_خوندي> ();
  test_cache<16, 8, 8, thread_خوندي> ();
  test_cache<24, 8, 8, thread_خوندي> ();
  test_cache<24, 16, 8, thread_خوندي> ();
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  test_caches<ریښتیا> ();
  test_caches<خطأ> ();
}
