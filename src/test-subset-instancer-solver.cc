/*
 * Copyright © 2023  Google, Inc.
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
 * Google Author(s): Qunxin Liu
 */

#అన్తర్భావ <math.h>
#అన్తర్భావ "hb-subset-instancer-solver.hh"

స్థిర అంతఃవరుస తార్కిక approx (Triple a, Triple b)
{
  తిరిగి నిరపేక్ష (a.minimum - b.minimum) < 0.000001f &&
         నిరపేక్ష (a.మధ్య - b.మధ్య) < 0.000001f &&
         నిరపేక్ష (a.గరిష్ఠ - b.గరిష్ఠ) < 0.000001f;
}

స్థిర అంతఃవరుస తార్కిక approx (దశాంశం a, దశాంశం b)
{ తిరిగి నిరపేక్ష (a - b) < 0.000001f; }

/* tests ported from
 * https://github.com/fonttools/fonttools/blob/main/Tests/varLib/instancer/solver_test.py */
పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  TripleDistances default_axis_distances{1.0f, 1.0f};
  /* Case 1 */
  {
    /* pin axis*/
    Triple tent (0.0f, 1.0f, 1.0f);
    Triple axis_range (0.0f, 0.0f, 0.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 0);
  }

  {
    /* pin axis*/
    Triple tent (0.0f, 1.0f, 1.0f);
    Triple axis_range (0.5f, 0.5f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
  }

  {
    /* tent falls outside the new axis range */
    Triple tent (0.3f, 0.5f, 0.8f);
    Triple axis_range (0.1f, 0.2f, 0.3f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 0);
  }

  {
    /* malformed axis range should be rejected, not abort */
    Triple tent (0.3f, 0.5f, 0.8f);
    Triple axis_range (0.4f, 0.2f, 0.3f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 0);
  }

  {
    /* malformed triple should not abort in renormalization */
    Triple malformed_range (0.4f, 0.2f, 0.3f);
    దశాంశం normalized = renormalize_విలువ (0.25f, malformed_range, default_axis_distances, నిజం);
    hb_always_assert (approx (normalized, 0.25f));
  }

  /* Case 2 */
  {
    Triple tent (0.0f, 1.0f, 1.0f);
    Triple axis_range (-1.0f, 0.0f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[0].second == Triple (0.0f, 1.0f, 1.0f));
  }

  /* Case 2 */
  {
    Triple tent (0.0f, 1.0f, 1.0f);
    Triple axis_range (-1.0f, 0.0f, 0.75f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 0.75f);
    hb_always_assert (బహిర్గతం[0].second == Triple (0.0f, 1.0f, 1.0f));
  }

  /* Without gain: */
  /* Case 3 */
  {
    Triple tent (0.0f, 0.2f, 1.0f);
    Triple axis_range (-1.0f, 0.0f, 0.8f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 2);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (0.0f, 0.25f, 1.0f));
    hb_always_assert (approx (బహిర్గతం[1].మొదటి, 0.250f));
    hb_always_assert (బహిర్గతం[1].second == Triple (0.25f, 1.0f, 1.0f));
  }

  /* Case 3 boundary */
  {
    Triple tent (0.0f, 0.4f, 1.0f);
    Triple axis_range (-1.0f, 0.0f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 2);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (0.0f, 0.8f, 1.0f));
    hb_always_assert (approx (బహిర్గతం[1].మొదటి, 2.5f/3));
    hb_always_assert (బహిర్గతం[1].second == Triple (0.8f, 1.0f, 1.0f));
  }

  /* Case 4 */
  {
    Triple tent (0.0f, 0.25f, 1.0f);
    Triple axis_range (-1.0f, 0.0f, 0.4f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 2);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (0.0f, 0.625f, 1.0f));
    hb_always_assert (approx (బహిర్గతం[1].మొదటి, 0.80f));
    hb_always_assert (బహిర్గతం[1].second == Triple (0.625f, 1.0f, 1.0f));
  }

  /* Case 4 */
  {
    Triple tent (0.25f, 0.3f, 1.05f);
    Triple axis_range (0.0f, 0.2f, 0.4f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 2);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (approx (బహిర్గతం[0].second, Triple (0.25f, 0.5f, 1.0f)));
    hb_always_assert (approx (బహిర్గతం[1].మొదటి, 2.6f / 3));
    hb_always_assert (approx (బహిర్గతం[1].second, Triple (0.5f, 1.0f, 1.0f)));
  }

  /* Case 4 boundary */
  {
    Triple tent (0.25f, 0.5f, 1.0f);
    Triple axis_range (0.0f, 0.25f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (0.0f, 1.0f, 1.0f));
  }

  /* With gain */
  /* Case 3a/1neg */
  {
    Triple tent (0.0f, 0.5f, 1.0f);
    Triple axis_range (0.0f, 0.5f, 1.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 3);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f, 1.0f));
    hb_always_assert (బహిర్గతం[2].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[2].second == Triple (-1.0f, -1.0f, 0.0f));
  }

  {
    Triple tent (0.0f, 0.5f, 1.0f);
    Triple axis_range (0.0f, 0.5f, 0.75f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 3);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == -0.5f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f, 1.0f));
    hb_always_assert (బహిర్గతం[2].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[2].second == Triple (-1.0f, -1.0f, 0.0f));
  }

  {
    Triple tent (0.0f, 0.50f, 1.0f);
    Triple axis_range (0.0f, 0.25f, 0.8f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 4);
    hb_always_assert (బహిర్గతం[0].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == 0.5f);
    hb_always_assert (approx (బహిర్గతం[1].second, Triple (0.0f, 0.454545f, 0.909091f)));
    hb_always_assert (approx (బహిర్గతం[2].మొదటి, -0.1f));
    hb_always_assert (approx (బహిర్గతం[2].second, Triple (0.909091f, 1.0f, 1.0f)));
    hb_always_assert (బహిర్గతం[3].మొదటి == -0.5f);
    hb_always_assert (బహిర్గతం[3].second == Triple (-1.0f, -1.0f, 0.0f));
  }

  /* Case 3a/1neg */
  {
    Triple tent (0.0f, 0.5f, 2.0f);
    Triple axis_range (0.2f, 0.5f, 0.8f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 3);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (approx (బహిర్గతం[1].మొదటి, -0.2f));
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f, 1.0f));
    hb_always_assert (approx (బహిర్గతం[2].మొదటి, -0.6f));
    hb_always_assert (approx (బహిర్గతం[2].second, Triple (-1.0f, -1.0f, 0.0f)));
  }

  /* Case 3a/1neg */
  {
    Triple tent (0.0f, 0.5f, 2.0f);
    Triple axis_range (0.2f, 0.5f, 1.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 3);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (approx (బహిర్గతం[1].మొదటి, -1.0f/3));
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f, 1.0f));
    hb_always_assert (approx (బహిర్గతం[2].మొదటి, -0.6f));
    hb_always_assert (approx (బహిర్గతం[2].second, Triple (-1.0f, -1.0f, 0.0f)));
  }

  /* Case 3 */
  {
    Triple tent (0.0f, 0.5f, 1.0f);
    Triple axis_range (0.25f, 0.25f, 0.75f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 2);
    hb_always_assert (బహిర్గతం[0].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 0.5f, 1.0f));
  }

  /* Case 1neg */
  {
    Triple tent (0.0f, 0.5f, 1.0f);
    Triple axis_range (0.0f, 0.25f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 3);
    hb_always_assert (బహిర్గతం[0].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f, 1.0f));
    hb_always_assert (బహిర్గతం[2].మొదటి == -0.5f);
    hb_always_assert (బహిర్గతం[2].second == Triple (-1.0f, -1.0f, 0.0f));
  }

  /* Case 2neg */
  {
    Triple tent (0.05f, 0.55f, 1.0f);
    Triple axis_range (0.0f, 0.25f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 4);
    hb_always_assert (approx (బహిర్గతం[0].మొదటి, 0.4f));
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (approx (బహిర్గతం[1].మొదటి, 0.5f));
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f, 1.0f));
    hb_always_assert (approx (బహిర్గతం[2].మొదటి, -0.4f));
    hb_always_assert (బహిర్గతం[2].second == Triple (-1.0f, -0.8f, 0.0f));
    hb_always_assert (approx (బహిర్గతం[3].మొదటి, -0.4f));
    hb_always_assert (బహిర్గతం[3].second == Triple (-1.0f, -1.0f, -0.8f));
  }

  /* Case 2neg, other side */
  {
    Triple tent (-1.0f, -0.55f, -0.05f);
    Triple axis_range (-0.5f, -0.25f, 0.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 4);
    hb_always_assert (approx (బహిర్గతం[0].మొదటి, 0.4f));
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (approx (బహిర్గతం[1].మొదటి, 0.5f));
    hb_always_assert (బహిర్గతం[1].second == Triple (-1.0f, -1.0f, 0.0f));
    hb_always_assert (approx (బహిర్గతం[2].మొదటి, -0.4f));
    hb_always_assert (బహిర్గతం[2].second == Triple (0.0f, 0.8f, 1.0f));
    hb_always_assert (approx (బహిర్గతం[3].మొదటి, -0.4f));
    hb_always_assert (బహిర్గతం[3].second == Triple (0.8f, 1.0f, 1.0f));
  }

  /* Misc corner cases */
  {
    Triple tent (0.5f, 0.5f, 0.5f);
    Triple axis_range (0.5f, 0.5f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
  }

  {
    Triple tent (0.3f, 0.5f, 0.7f);
    Triple axis_range (0.1f, 0.5f, 0.9f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 5);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == -1.0f);
    hb_always_assert (approx(బహిర్గతం[1].second, Triple (0.0f, 0.5f, 1.0f)));
    hb_always_assert (బహిర్గతం[2].మొదటి == -1.0f);
    hb_always_assert (approx(బహిర్గతం[2].second, Triple (0.5f, 1.0f, 1.0f)));
    hb_always_assert (బహిర్గతం[3].మొదటి == -1.0f);
    hb_always_assert (approx (బహిర్గతం[3].second, Triple (-1.0f, -0.5f, 0.0f)));
    hb_always_assert (బహిర్గతం[4].మొదటి == -1.0f);
    hb_always_assert (approx (బహిర్గతం[4].second, Triple (-1.0f, -1.0f, -0.5f)));
  }

  {
    Triple tent (0.5f, 0.5f, 0.5f);
    Triple axis_range (0.25f, 0.25f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (1.0f, 1.0f, 1.0f));
  }

  {
    Triple tent (0.5f, 0.5f, 0.5f);
    Triple axis_range (0.25f, 0.35f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (1.0f, 1.0f, 1.0f));
  }

  {
    Triple tent (0.5f, 0.5f, 0.55f);
    Triple axis_range (0.25f, 0.35f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (1.0f, 1.0f, 1.0f));
  }

  {
    Triple tent (0.5f, 0.5f, 1.0f);
    Triple axis_range (0.5f, 0.5f, 1.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 2);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f, 1.0f));
  }

  {
    Triple tent (0.25f, 0.5f, 1.0f);
    Triple axis_range (0.5f, 0.5f, 1.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 2);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f, 1.0f));
  }

  {
    Triple tent (0.0f, 0.2f, 1.0f);
    Triple axis_range (0.0f, 0.0f, 0.5f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 2);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (0.0f, 0.4f, 1.0f));
    hb_always_assert (బహిర్గతం[1].మొదటి == 0.625f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.4f, 1.0f, 1.0f));
  }


  {
    Triple tent (0.0f, 0.5f, 1.0f);
    Triple axis_range (-1.0f, 0.25f, 1.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 5);
    hb_always_assert (బహిర్గతం[0].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == 0.5f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 1.0f/3, 2.0f/3));
    hb_always_assert (బహిర్గతం[2].మొదటి == -0.5f);
    hb_always_assert (బహిర్గతం[2].second == Triple (2.0f/3, 1.0f, 1.0f));
    hb_always_assert (బహిర్గతం[3].మొదటి == -0.5f);
    hb_always_assert (బహిర్గతం[3].second == Triple (-1.0f, -0.2f, 0.0f));
    hb_always_assert (బహిర్గతం[4].మొదటి == -0.5f);
    hb_always_assert (బహిర్గతం[4].second == Triple (-1.0f, -1.0f, -0.2f));
  }

  {
    Triple tent (0.5f, 0.5f, 0.5f);
    Triple axis_range (0.0f, 0.5f, 1.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 5);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple ());
    hb_always_assert (బహిర్గతం[1].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[1].second == Triple (0.0f, 2/(ద్విగుణ) (1 << 14), 1.0f));
    hb_always_assert (బహిర్గతం[2].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[2].second == Triple (2/(ద్విగుణ) (1 << 14), 1.0f, 1.0f));
    hb_always_assert (బహిర్గతం[3].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[3].second == Triple (-1.0f, -2/(ద్విగుణ) (1 << 14), 0.0f));
    hb_always_assert (బహిర్గతం[4].మొదటి == -1.0f);
    hb_always_assert (బహిర్గతం[4].second == Triple (-1.0f, -1.0f, -2/(ద్విగుణ) (1 << 14)));
  }

  {
    Triple tent (0.0f, 1.0f, 1.0f);
    Triple axis_range (-1.0f, -0.5f, 1.0f);
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, default_axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (1.0f/3, 1.0f, 1.0f));
  }

  {
    Triple tent (0.0f, 1.0f, 1.0f);
    Triple axis_range (-1.0f, -0.5f, 1.0f);
    TripleDistances axis_distances{2.0f, 1.0f};
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (బహిర్గతం[0].second == Triple (0.5f, 1.0f, 1.0f));
  }

  {
    Triple tent (0.6f, 0.7f, 0.8f);
    Triple axis_range (-1.0f, 0.2f, 1.0f);
    TripleDistances axis_distances{1.0f, 1.0f};
    rebase_tent_ఫలితం_t బహిర్గతం, scratch;
    rebase_tent (tent, axis_range, axis_distances, బహిర్గతం, scratch);
    hb_always_assert (బహిర్గతం.నిడివి == 1);
    hb_always_assert (బహిర్గతం[0].మొదటి == 1.0f);
    hb_always_assert (approx (బహిర్గతం[0].second, Triple (0.5f, 0.625f, 0.75f)));
  }
}
