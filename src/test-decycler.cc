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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-decycler.hh"

ಸ್ಥಿರ ಶೂನ್ಯ
ವೃಕ್ಷ_recurse_ದ್ವಿಮಾನ (unsigned value,
		     unsigned max_ಮೌಲ್ಯ,
		     hb_decycler_t &decycler)
{
  ಯದಿ (value >= max_ಮೌಲ್ಯ)
		ಪ್ರತ್ಯಾಗಮ;

  hb_decycler_ಗಂಟು_t node (decycler);

  ತಾರ್ಕಿಕ ret = node.visit (value);
  hb_always_assert (ret);

  ವೃಕ್ಷ_recurse_ದ್ವಿಮಾನ (value * 2 + 1, max_ಮೌಲ್ಯ, decycler);
  ವೃಕ್ಷ_recurse_ದ್ವಿಮಾನ (value * 2 + 2, max_ಮೌಲ್ಯ, decycler);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ವೃಕ್ಷ_recurse_tertiary (unsigned value,
		       unsigned max_ಮೌಲ್ಯ,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  ಯದಿ (value >= max_ಮೌಲ್ಯ)
    ಪ್ರತ್ಯಾಗಮ;

  hb_decycler_ಗಂಟು_t node (decycler);

  value *= 3;

  ಪ್ರತಿ (unsigned i = 1; i <= 3; i++)
  {
    ತಾರ್ಕಿಕ ret = node.visit (value + i);
    hb_always_assert (ret);

    ವೃಕ್ಷ_recurse_tertiary (value + i, max_ಮೌಲ್ಯ, decycler);
  }
}

ಸ್ಥಿರ ಶೂನ್ಯ
test_ವೃಕ್ಷ ()
{
  hb_decycler_t decycler;
  ವೃಕ್ಷ_recurse_ದ್ವಿಮಾನ (0, 64, decycler);
  ವೃಕ್ಷ_recurse_tertiary (0, 1000, decycler);
}

ಸ್ಥಿರ ಶೂನ್ಯ
cycle_recurse (signed value,
	       signed cycle_length,
	       hb_decycler_t &decycler)
{
  hb_always_assert (cycle_length > 0);

  hb_decycler_ಗಂಟು_t node (decycler);

  ಯದಿ (!node.visit (value))
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (value >= cycle_length)
    value = value % cycle_length;

  cycle_recurse (value + 1, cycle_length, decycler);
}

ಸ್ಥಿರ ಶೂನ್ಯ
test_cycle ()
{
  hb_decycler_t decycler;
  cycle_recurse (0, 1, decycler);
  cycle_recurse (0, 2, decycler);
  cycle_recurse (2, 3, decycler);
  cycle_recurse (-20, 8, decycler);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  test_ವೃಕ್ಷ ();
  test_cycle ();

  ಪ್ರತ್ಯಾಗಮ 0;
}

