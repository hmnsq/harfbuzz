/*
 * Copyright © 2020  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-array.hh"

स्थिर शून्य
test_reverse ()
{
  पूर्णाङ्क values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  hb_array_t<पूर्णाङ्क> a (values, 9);
  a.reverse();

  पूर्णाङ्क expected_मानानि[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  hb_array_t<पूर्णाङ्क> expected (expected_मानानि, 9);
  hb_always_assert (a == expected);
}

स्थिर शून्य
test_reverse_range ()
{
  पूर्णाङ्क values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  hb_array_t<पूर्णाङ्क> a (values, 9);
  a.reverse(2, 6);

  पूर्णाङ्क expected_मानानि[] = {1, 2, 6, 5, 4, 3, 7, 8, 9};
  hb_array_t<पूर्णाङ्क> expected (expected_मानानि, 9);
  hb_always_assert (a == expected);
}

स्थिर शून्य
test_reverse_invalid ()
{
  पूर्णाङ्क values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  hb_array_t<पूर्णाङ्क> a (values, 9);

  a.reverse(4, 3);
  a.reverse(2, 3);
  a.reverse(5, 5);
  a.reverse(12, 15);

  पूर्णाङ्क expected_मानानि[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  hb_array_t<पूर्णाङ्क> expected (expected_मानानि, 9);
  hb_always_assert (a == expected);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  /* The following fails on MSVC. */
  // hb_always_assert (sizeof (hb_array_t<int>) == sizeof (hb_sorted_array_t<int>));

  test_reverse ();
  test_reverse_range ();
  test_reverse_invalid ();
}
