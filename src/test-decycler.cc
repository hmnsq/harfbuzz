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

#समावेश "hb.hh"
#समावेश "hb-decycler.hh"

अटल व्योम
वृक्ष_recurse_द्विआधारी (अचिन्हित value,
		     अचिन्हित max_मूल्य,
		     hb_decycler_t &decycler)
{
  अगर (value >= max_मूल्य)
		वापस;

  hb_decycler_गाँठ_t node (decycler);

  bool ret = node.visit (value);
  hb_always_assert (ret);

  वृक्ष_recurse_द्विआधारी (value * 2 + 1, max_मूल्य, decycler);
  वृक्ष_recurse_द्विआधारी (value * 2 + 2, max_मूल्य, decycler);
}

अटल व्योम
वृक्ष_recurse_tertiary (अचिन्हित value,
		       अचिन्हित max_मूल्य,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  अगर (value >= max_मूल्य)
    वापस;

  hb_decycler_गाँठ_t node (decycler);

  value *= 3;

  क्रम (अचिन्हित i = 1; i <= 3; i++)
  {
    bool ret = node.visit (value + i);
    hb_always_assert (ret);

    वृक्ष_recurse_tertiary (value + i, max_मूल्य, decycler);
  }
}

अटल व्योम
test_वृक्ष ()
{
  hb_decycler_t decycler;
  वृक्ष_recurse_द्विआधारी (0, 64, decycler);
  वृक्ष_recurse_tertiary (0, 1000, decycler);
}

अटल व्योम
cycle_recurse (चिन्हित value,
	       चिन्हित cycle_लंबाई,
	       hb_decycler_t &decycler)
{
  hb_always_assert (cycle_लंबाई > 0);

  hb_decycler_गाँठ_t node (decycler);

  अगर (!node.visit (value))
    वापस;

  अगर (value >= cycle_लंबाई)
    value = value % cycle_लंबाई;

  cycle_recurse (value + 1, cycle_लंबाई, decycler);
}

अटल व्योम
test_cycle ()
{
  hb_decycler_t decycler;
  cycle_recurse (0, 1, decycler);
  cycle_recurse (0, 2, decycler);
  cycle_recurse (2, 3, decycler);
  cycle_recurse (-20, 8, decycler);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  test_वृक्ष ();
  test_cycle ();

  वापस 0;
}

