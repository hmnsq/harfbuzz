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
درخت_recurse_دودویی (بیعلامت value,
		     بیعلامت max_مقدار,
		     hb_decycler_t &decycler)
{
  إذا (value >= max_مقدار)
		أرجع;

  hb_decycler_گره_t node (decycler);

  منطقي ret = node.بازدید (value);
  hb_always_assert (ret);

  درخت_recurse_دودویی (value * 2 + 1, max_مقدار, decycler);
  درخت_recurse_دودویی (value * 2 + 2, max_مقدار, decycler);
}

ساكن فراغ
درخت_recurse_tertiary (بیعلامت value,
		       بیعلامت max_مقدار,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  إذا (value >= max_مقدار)
    أرجع;

  hb_decycler_گره_t node (decycler);

  value *= 3;

  لكل (بیعلامت i = 1; i <= 3; i++)
  {
    منطقي ret = node.بازدید (value + i);
    hb_always_assert (ret);

    درخت_recurse_tertiary (value + i, max_مقدار, decycler);
  }
}

ساكن فراغ
test_درخت ()
{
  hb_decycler_t decycler;
  درخت_recurse_دودویی (0, 64, decycler);
  درخت_recurse_tertiary (0, 1000, decycler);
}

ساكن فراغ
چرخه_recurse (علامتدار value,
	       علامتدار چرخه_درازی,
	       hb_decycler_t &decycler)
{
  hb_always_assert (چرخه_درازی > 0);

  hb_decycler_گره_t node (decycler);

  إذا (!node.بازدید (value))
    أرجع;

  إذا (value >= چرخه_درازی)
    value = value % چرخه_درازی;

  چرخه_recurse (value + 1, چرخه_درازی, decycler);
}

ساكن فراغ
test_چرخه ()
{
  hb_decycler_t decycler;
  چرخه_recurse (0, 1, decycler);
  چرخه_recurse (0, 2, decycler);
  چرخه_recurse (2, 3, decycler);
  چرخه_recurse (-20, 8, decycler);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  test_درخت ();
  test_چرخه ();

  أرجع 0;
}

