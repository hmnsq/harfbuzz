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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-decycler.hh"

ସ୍ଥିର ଶୂନ୍ୟ
ବୃକ୍ଷ_recurse_ଦ୍ୱିମିକ (unsigned value,
		     unsigned max_ମୂଲ୍ୟ,
		     hb_decycler_t &decycler)
{
  ଯଦି (value >= max_ମୂଲ୍ୟ)
		ଫେରନ୍ତୁ;

  hb_decycler_ଗଣ୍ଠି_t node (decycler);

  ତାର୍କିକ ret = node.visit (value);
  hb_always_assert (ret);

  ବୃକ୍ଷ_recurse_ଦ୍ୱିମିକ (value * 2 + 1, max_ମୂଲ୍ୟ, decycler);
  ବୃକ୍ଷ_recurse_ଦ୍ୱିମିକ (value * 2 + 2, max_ମୂଲ୍ୟ, decycler);
}

ସ୍ଥିର ଶୂନ୍ୟ
ବୃକ୍ଷ_recurse_tertiary (unsigned value,
		       unsigned max_ମୂଲ୍ୟ,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  ଯଦି (value >= max_ମୂଲ୍ୟ)
    ଫେରନ୍ତୁ;

  hb_decycler_ଗଣ୍ଠି_t node (decycler);

  value *= 3;

  ପାଇଁ (unsigned i = 1; i <= 3; i++)
  {
    ତାର୍କିକ ret = node.visit (value + i);
    hb_always_assert (ret);

    ବୃକ୍ଷ_recurse_tertiary (value + i, max_ମୂଲ୍ୟ, decycler);
  }
}

ସ୍ଥିର ଶୂନ୍ୟ
test_ବୃକ୍ଷ ()
{
  hb_decycler_t decycler;
  ବୃକ୍ଷ_recurse_ଦ୍ୱିମିକ (0, 64, decycler);
  ବୃକ୍ଷ_recurse_tertiary (0, 1000, decycler);
}

ସ୍ଥିର ଶୂନ୍ୟ
cycle_recurse (signed value,
	       signed cycle_length,
	       hb_decycler_t &decycler)
{
  hb_always_assert (cycle_length > 0);

  hb_decycler_ଗଣ୍ଠି_t node (decycler);

  ଯଦି (!node.visit (value))
    ଫେରନ୍ତୁ;

  ଯଦି (value >= cycle_length)
    value = value % cycle_length;

  cycle_recurse (value + 1, cycle_length, decycler);
}

ସ୍ଥିର ଶୂନ୍ୟ
test_cycle ()
{
  hb_decycler_t decycler;
  cycle_recurse (0, 1, decycler);
  cycle_recurse (0, 2, decycler);
  cycle_recurse (2, 3, decycler);
  cycle_recurse (-20, 8, decycler);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  test_ବୃକ୍ଷ ();
  test_cycle ();

  ଫେରନ୍ତୁ 0;
}

