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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-decycler.hh"

ਸਥਿਰ ਖਾਲੀ
ਰੁੱਖ_recurse_ਦੁਆਧਾਰੀ (ਬਿਨਾਨਿਸ਼ਾਨੀ value,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਮੁੱਲ,
		     hb_decycler_t &decycler)
{
  ਜੇ (value >= max_ਮੁੱਲ)
		ਵਾਪਸ;

  hb_decycler_ਗੰਢ_t node (decycler);

  ਤਾਰਕਿਕ ret = node.ਦੇਖੋ (value);
  hb_always_assert (ret);

  ਰੁੱਖ_recurse_ਦੁਆਧਾਰੀ (value * 2 + 1, max_ਮੁੱਲ, decycler);
  ਰੁੱਖ_recurse_ਦੁਆਧਾਰੀ (value * 2 + 2, max_ਮੁੱਲ, decycler);
}

ਸਥਿਰ ਖਾਲੀ
ਰੁੱਖ_recurse_tertiary (ਬਿਨਾਨਿਸ਼ਾਨੀ value,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਮੁੱਲ,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  ਜੇ (value >= max_ਮੁੱਲ)
    ਵਾਪਸ;

  hb_decycler_ਗੰਢ_t node (decycler);

  value *= 3;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i <= 3; i++)
  {
    ਤਾਰਕਿਕ ret = node.ਦੇਖੋ (value + i);
    hb_always_assert (ret);

    ਰੁੱਖ_recurse_tertiary (value + i, max_ਮੁੱਲ, decycler);
  }
}

ਸਥਿਰ ਖਾਲੀ
test_ਰੁੱਖ ()
{
  hb_decycler_t decycler;
  ਰੁੱਖ_recurse_ਦੁਆਧਾਰੀ (0, 64, decycler);
  ਰੁੱਖ_recurse_tertiary (0, 1000, decycler);
}

ਸਥਿਰ ਖਾਲੀ
ਚੱਕਰ_recurse (ਨਿਸ਼ਾਨੀਵਾਲਾ value,
	       ਨਿਸ਼ਾਨੀਵਾਲਾ ਚੱਕਰ_ਲੰਮਾਈ,
	       hb_decycler_t &decycler)
{
  hb_always_assert (ਚੱਕਰ_ਲੰਮਾਈ > 0);

  hb_decycler_ਗੰਢ_t node (decycler);

  ਜੇ (!node.ਦੇਖੋ (value))
    ਵਾਪਸ;

  ਜੇ (value >= ਚੱਕਰ_ਲੰਮਾਈ)
    value = value % ਚੱਕਰ_ਲੰਮਾਈ;

  ਚੱਕਰ_recurse (value + 1, ਚੱਕਰ_ਲੰਮਾਈ, decycler);
}

ਸਥਿਰ ਖਾਲੀ
test_ਚੱਕਰ ()
{
  hb_decycler_t decycler;
  ਚੱਕਰ_recurse (0, 1, decycler);
  ਚੱਕਰ_recurse (0, 2, decycler);
  ਚੱਕਰ_recurse (2, 3, decycler);
  ਚੱਕਰ_recurse (-20, 8, decycler);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  test_ਰੁੱਖ ();
  test_ਚੱਕਰ ();

  ਵਾਪਸ 0;
}

