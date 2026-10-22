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

#شامل "hb.hh"
#شامل "hb-priority-queue.hh"

ساکن خالی
test_داخل_کرو ()
{
  hb_ترجیح_قطار_t<int64_t> قطار;
  hb_always_assert (قطار.is_empty ());

  قطار.insert (10, 0);
  hb_always_assert (!قطار.is_empty ());
  hb_always_assert (قطار.minimum () == hb_pair (10, 0));

  قطار.insert (20, 1);
  hb_always_assert (قطار.minimum () == hb_pair (10, 0));

  قطار.insert (5, 2);
  hb_always_assert (قطار.minimum () == hb_pair (5, 2));

  قطار.insert (15, 3);
  hb_always_assert (قطار.minimum () == hb_pair (5, 2));

  قطار.insert (1, 4);
  hb_always_assert (قطار.minimum () == hb_pair (1, 4));
}

ساکن خالی
test_نکالو ()
{
  hb_ترجیح_قطار_t<int32_t> قطار;
  قطار.insert (0, 0);
  قطار.insert (60, 6);
  قطار.insert (30, 3);
  قطار.insert (40 ,4);
  قطار.insert (20, 2);
  قطار.insert (50, 5);
  قطار.insert (70, 7);
  قطار.insert (10, 1);

  برائے (صحیح i = 0; i < 8; i++)
  {
    hb_always_assert (!قطار.is_empty ());
    hb_always_assert (قطار.minimum () == hb_pair (i * 10, i));
    hb_always_assert (قطار.نکالو_minimum () == hb_pair (i * 10, i));
  }

  hb_always_assert (قطار.is_empty ());
}

صحیح
مین (صحیح argc, حرف **argv)
{
  test_داخل_کرو ();
  test_نکالو ();
}
