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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-decycler.hh"

நிலையான வெறுமை
மரம்_recurse_இருமம் (unsigned value,
		     unsigned max_மதிப்பு,
		     hb_decycler_t &decycler)
{
  எனில் (value >= max_மதிப்பு)
		திரும்பு;

  hb_decycler_முனை_t node (decycler);

  தருக்க ret = node.visit (value);
  hb_always_assert (ret);

  மரம்_recurse_இருமம் (value * 2 + 1, max_மதிப்பு, decycler);
  மரம்_recurse_இருமம் (value * 2 + 2, max_மதிப்பு, decycler);
}

நிலையான வெறுமை
மரம்_recurse_tertiary (unsigned value,
		       unsigned max_மதிப்பு,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  எனில் (value >= max_மதிப்பு)
    திரும்பு;

  hb_decycler_முனை_t node (decycler);

  value *= 3;

  க்கு (unsigned i = 1; i <= 3; i++)
  {
    தருக்க ret = node.visit (value + i);
    hb_always_assert (ret);

    மரம்_recurse_tertiary (value + i, max_மதிப்பு, decycler);
  }
}

நிலையான வெறுமை
test_மரம் ()
{
  hb_decycler_t decycler;
  மரம்_recurse_இருமம் (0, 64, decycler);
  மரம்_recurse_tertiary (0, 1000, decycler);
}

நிலையான வெறுமை
cycle_recurse (signed value,
	       signed cycle_length,
	       hb_decycler_t &decycler)
{
  hb_always_assert (cycle_length > 0);

  hb_decycler_முனை_t node (decycler);

  எனில் (!node.visit (value))
    திரும்பு;

  எனில் (value >= cycle_length)
    value = value % cycle_length;

  cycle_recurse (value + 1, cycle_length, decycler);
}

நிலையான வெறுமை
test_cycle ()
{
  hb_decycler_t decycler;
  cycle_recurse (0, 1, decycler);
  cycle_recurse (0, 2, decycler);
  cycle_recurse (2, 3, decycler);
  cycle_recurse (-20, 8, decycler);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  test_மரம் ();
  test_cycle ();

  திரும்பு 0;
}

