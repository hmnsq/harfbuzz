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

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-decycler.hh"

స్థిర శూన్య
వృక్షం_recurse_ద్విమాన (గుర్తులేని value,
		     గుర్తులేని max_విలువ,
		     hb_decycler_t &decycler)
{
  ఒకవేళ (value >= max_విలువ)
		తిరిగి;

  hb_decycler_కణుపు_t node (decycler);

  తార్కిక ret = node.సందర్శించు (value);
  hb_always_assert (ret);

  వృక్షం_recurse_ద్విమాన (value * 2 + 1, max_విలువ, decycler);
  వృక్షం_recurse_ద్విమాన (value * 2 + 2, max_విలువ, decycler);
}

స్థిర శూన్య
వృక్షం_recurse_tertiary (గుర్తులేని value,
		       గుర్తులేని max_విలువ,
		       hb_decycler_t &decycler)
{
  /* This function implements an alternative way to use the
   * decycler. It checks for each node before visiting it.
   * It demonstrates reusing a node for multiple visits. */

  ఒకవేళ (value >= max_విలువ)
    తిరిగి;

  hb_decycler_కణుపు_t node (decycler);

  value *= 3;

  కొరకు (గుర్తులేని i = 1; i <= 3; i++)
  {
    తార్కిక ret = node.సందర్శించు (value + i);
    hb_always_assert (ret);

    వృక్షం_recurse_tertiary (value + i, max_విలువ, decycler);
  }
}

స్థిర శూన్య
test_వృక్షం ()
{
  hb_decycler_t decycler;
  వృక్షం_recurse_ద్విమాన (0, 64, decycler);
  వృక్షం_recurse_tertiary (0, 1000, decycler);
}

స్థిర శూన్య
చక్రం_recurse (గుర్తుగల value,
	       గుర్తుగల చక్రం_నిడివి,
	       hb_decycler_t &decycler)
{
  hb_always_assert (చక్రం_నిడివి > 0);

  hb_decycler_కణుపు_t node (decycler);

  ఒకవేళ (!node.సందర్శించు (value))
    తిరిగి;

  ఒకవేళ (value >= చక్రం_నిడివి)
    value = value % చక్రం_నిడివి;

  చక్రం_recurse (value + 1, చక్రం_నిడివి, decycler);
}

స్థిర శూన్య
test_చక్రం ()
{
  hb_decycler_t decycler;
  చక్రం_recurse (0, 1, decycler);
  చక్రం_recurse (0, 2, decycler);
  చక్రం_recurse (2, 3, decycler);
  చక్రం_recurse (-20, 8, decycler);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_వృక్షం ();
  test_చక్రం ();

  తిరిగి 0;
}

