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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-decycler.hh"

સ્થિર શૂન્ય
વૃક્ષ_recurse_દ્વિઅંકી (unsigned value,
		     unsigned max_કિંમત,
		     hb_decycler_t &decycler)
{
  જો (value >= max_કિંમત)
		પાછા;

  hb_decycler_ગાંઠ_t node (decycler);

  તાર્કિક ret = node.visit (value);
  hb_always_assert (ret);

  વૃક્ષ_recurse_દ્વિઅંકી (value * 2 + 1, max_કિંમત, decycler);
  વૃક્ષ_recurse_દ્વિઅંકી (value * 2 + 2, max_કિંમત, decycler);
}

સ્થિર શૂન્ય
વૃક્ષ_recurse_tertiary (unsigned value,
		       unsigned max_કિંમત,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  જો (value >= max_કિંમત)
    પાછા;

  hb_decycler_ગાંઠ_t node (decycler);

  value *= 3;

  માટે (unsigned i = 1; i <= 3; i++)
  {
    તાર્કિક ret = node.visit (value + i);
    hb_always_assert (ret);

    વૃક્ષ_recurse_tertiary (value + i, max_કિંમત, decycler);
  }
}

સ્થિર શૂન્ય
test_વૃક્ષ ()
{
  hb_decycler_t decycler;
  વૃક્ષ_recurse_દ્વિઅંકી (0, 64, decycler);
  વૃક્ષ_recurse_tertiary (0, 1000, decycler);
}

સ્થિર શૂન્ય
cycle_recurse (signed value,
	       signed cycle_length,
	       hb_decycler_t &decycler)
{
  hb_always_assert (cycle_length > 0);

  hb_decycler_ગાંઠ_t node (decycler);

  જો (!node.visit (value))
    પાછા;

  જો (value >= cycle_length)
    value = value % cycle_length;

  cycle_recurse (value + 1, cycle_length, decycler);
}

સ્થિર શૂન્ય
test_cycle ()
{
  hb_decycler_t decycler;
  cycle_recurse (0, 1, decycler);
  cycle_recurse (0, 2, decycler);
  cycle_recurse (2, 3, decycler);
  cycle_recurse (-20, 8, decycler);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  test_વૃક્ષ ();
  test_cycle ();

  પાછા 0;
}

