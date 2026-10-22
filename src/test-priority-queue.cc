/*
 * Copyright © 2020  Google, Inc.
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
 *
 * Google Author(s): Garret Rieger
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-priority-queue.hh"

ਸਥਿਰ ਖਾਲੀ
test_ਪਾਓ ()
{
  hb_ਤਰਜੀਹ_ਕਤਾਰ_t<int64_t> ਕਤਾਰ;
  hb_always_assert (ਕਤਾਰ.is_empty ());

  ਕਤਾਰ.insert (10, 0);
  hb_always_assert (!ਕਤਾਰ.is_empty ());
  hb_always_assert (ਕਤਾਰ.minimum () == hb_pair (10, 0));

  ਕਤਾਰ.insert (20, 1);
  hb_always_assert (ਕਤਾਰ.minimum () == hb_pair (10, 0));

  ਕਤਾਰ.insert (5, 2);
  hb_always_assert (ਕਤਾਰ.minimum () == hb_pair (5, 2));

  ਕਤਾਰ.insert (15, 3);
  hb_always_assert (ਕਤਾਰ.minimum () == hb_pair (5, 2));

  ਕਤਾਰ.insert (1, 4);
  hb_always_assert (ਕਤਾਰ.minimum () == hb_pair (1, 4));
}

ਸਥਿਰ ਖਾਲੀ
test_ਕੱਢੋ ()
{
  hb_ਤਰਜੀਹ_ਕਤਾਰ_t<int32_t> ਕਤਾਰ;
  ਕਤਾਰ.insert (0, 0);
  ਕਤਾਰ.insert (60, 6);
  ਕਤਾਰ.insert (30, 3);
  ਕਤਾਰ.insert (40 ,4);
  ਕਤਾਰ.insert (20, 2);
  ਕਤਾਰ.insert (50, 5);
  ਕਤਾਰ.insert (70, 7);
  ਕਤਾਰ.insert (10, 1);

  ਲਈ (ਪੂਰਨ i = 0; i < 8; i++)
  {
    hb_always_assert (!ਕਤਾਰ.is_empty ());
    hb_always_assert (ਕਤਾਰ.minimum () == hb_pair (i * 10, i));
    hb_always_assert (ਕਤਾਰ.ਕੱਢੋ_minimum () == hb_pair (i * 10, i));
  }

  hb_always_assert (ਕਤਾਰ.is_empty ());
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  test_ਪਾਓ ();
  test_ਕੱਢੋ ();
}
