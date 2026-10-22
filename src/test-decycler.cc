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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-decycler.hh"

স্থির শূন্য
বৃক্ষ_recurse_দ্বিমিক (unsigned value,
		     unsigned max_মান,
		     hb_decycler_t &decycler)
{
  যদি (value >= max_মান)
		প্রত্যাগম;

  hb_decycler_গ্রন্থি_t node (decycler);

  তার্কিক ret = node.visit (value);
  hb_always_assert (ret);

  বৃক্ষ_recurse_দ্বিমিক (value * 2 + 1, max_মান, decycler);
  বৃক্ষ_recurse_দ্বিমিক (value * 2 + 2, max_মান, decycler);
}

স্থির শূন্য
বৃক্ষ_recurse_tertiary (unsigned value,
		       unsigned max_মান,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  যদি (value >= max_মান)
    প্রত্যাগম;

  hb_decycler_গ্রন্থি_t node (decycler);

  value *= 3;

  প্রতি (unsigned i = 1; i <= 3; i++)
  {
    তার্কিক ret = node.visit (value + i);
    hb_always_assert (ret);

    বৃক্ষ_recurse_tertiary (value + i, max_মান, decycler);
  }
}

স্থির শূন্য
test_বৃক্ষ ()
{
  hb_decycler_t decycler;
  বৃক্ষ_recurse_দ্বিমিক (0, 64, decycler);
  বৃক্ষ_recurse_tertiary (0, 1000, decycler);
}

স্থির শূন্য
cycle_recurse (signed value,
	       signed cycle_length,
	       hb_decycler_t &decycler)
{
  hb_always_assert (cycle_length > 0);

  hb_decycler_গ্রন্থি_t node (decycler);

  যদি (!node.visit (value))
    প্রত্যাগম;

  যদি (value >= cycle_length)
    value = value % cycle_length;

  cycle_recurse (value + 1, cycle_length, decycler);
}

স্থির শূন্য
test_cycle ()
{
  hb_decycler_t decycler;
  cycle_recurse (0, 1, decycler);
  cycle_recurse (0, 2, decycler);
  cycle_recurse (2, 3, decycler);
  cycle_recurse (-20, 8, decycler);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  test_বৃক্ষ ();
  test_cycle ();

  প্রত্যাগম 0;
}

