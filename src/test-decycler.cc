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

#شامل "hb.hh"
#شامل "hb-decycler.hh"

ساکن خالی
درخت_recurse_ثنائی (غیرعلامتی value,
		     غیرعلامتی max_قدر,
		     hb_decycler_t &decycler)
{
  اگر (value >= max_قدر)
		واپس;

  hb_decycler_گرہ_t node (decycler);

  منطقی ret = node.دورہ (value);
  hb_always_assert (ret);

  درخت_recurse_ثنائی (value * 2 + 1, max_قدر, decycler);
  درخت_recurse_ثنائی (value * 2 + 2, max_قدر, decycler);
}

ساکن خالی
درخت_recurse_tertiary (غیرعلامتی value,
		       غیرعلامتی max_قدر,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  اگر (value >= max_قدر)
    واپس;

  hb_decycler_گرہ_t node (decycler);

  value *= 3;

  برائے (غیرعلامتی i = 1; i <= 3; i++)
  {
    منطقی ret = node.دورہ (value + i);
    hb_always_assert (ret);

    درخت_recurse_tertiary (value + i, max_قدر, decycler);
  }
}

ساکن خالی
test_درخت ()
{
  hb_decycler_t decycler;
  درخت_recurse_ثنائی (0, 64, decycler);
  درخت_recurse_tertiary (0, 1000, decycler);
}

ساکن خالی
چکر_recurse (علامتی value,
	       علامتی چکر_لمبائی,
	       hb_decycler_t &decycler)
{
  hb_always_assert (چکر_لمبائی > 0);

  hb_decycler_گرہ_t node (decycler);

  اگر (!node.دورہ (value))
    واپس;

  اگر (value >= چکر_لمبائی)
    value = value % چکر_لمبائی;

  چکر_recurse (value + 1, چکر_لمبائی, decycler);
}

ساکن خالی
test_چکر ()
{
  hb_decycler_t decycler;
  چکر_recurse (0, 1, decycler);
  چکر_recurse (0, 2, decycler);
  چکر_recurse (2, 3, decycler);
  چکر_recurse (-20, 8, decycler);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  test_درخت ();
  test_چکر ();

  واپس 0;
}

