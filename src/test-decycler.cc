/*
 * Copyright © 2025 Behdad Esfahbod
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
 * Author(s): Behdad Esfahbod
 */

#ضمن "hb.hh"
#ضمن "hb-decycler.hh"

ساكن فراغ
وڻ_recurse_ٻٽو (بنانشان value,
		     بنانشان max_قدر,
		     hb_decycler_t &decycler)
{
  إذا (value >= max_قدر)
		أرجع;

  hb_decycler_ڳنڍ_t node (decycler);

  منطقي ret = node.گهمو (value);
  hb_always_assert (ret);

  وڻ_recurse_ٻٽو (value * 2 + 1, max_قدر, decycler);
  وڻ_recurse_ٻٽو (value * 2 + 2, max_قدر, decycler);
}

ساكن فراغ
وڻ_recurse_tertiary (بنانشان value,
		       بنانشان max_قدر,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  إذا (value >= max_قدر)
    أرجع;

  hb_decycler_ڳنڍ_t node (decycler);

  value *= 3;

  لكل (بنانشان i = 1; i <= 3; i++)
  {
    منطقي ret = node.گهمو (value + i);
    hb_always_assert (ret);

    وڻ_recurse_tertiary (value + i, max_قدر, decycler);
  }
}

ساكن فراغ
test_وڻ ()
{
  hb_decycler_t decycler;
  وڻ_recurse_ٻٽو (0, 64, decycler);
  وڻ_recurse_tertiary (0, 1000, decycler);
}

ساكن فراغ
چڪر_recurse (نشانوارو value,
	       نشانوارو چڪر_ڊيگهائي,
	       hb_decycler_t &decycler)
{
  hb_always_assert (چڪر_ڊيگهائي > 0);

  hb_decycler_ڳنڍ_t node (decycler);

  إذا (!node.گهمو (value))
    أرجع;

  إذا (value >= چڪر_ڊيگهائي)
    value = value % چڪر_ڊيگهائي;

  چڪر_recurse (value + 1, چڪر_ڊيگهائي, decycler);
}

ساكن فراغ
test_چڪر ()
{
  hb_decycler_t decycler;
  چڪر_recurse (0, 1, decycler);
  چڪر_recurse (0, 2, decycler);
  چڪر_recurse (2, 3, decycler);
  چڪر_recurse (-20, 8, decycler);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  test_وڻ ();
  test_چڪر ();

  أرجع 0;
}

