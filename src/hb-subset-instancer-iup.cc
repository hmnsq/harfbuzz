/*
 * Copyright © 2024  Google, Inc.
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
 */

#ضمن "hb-subset-instancer-iup.hh"

#ضمن "hb-bit-page.hh"

کارول hb_iup_سېټ_t = hb_بټ_پاڼه_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ساكن بینښه MAX_LOOKBACK = 8;

ساكن فراغ iup_contour_bound_forced_سېټ (ثابت hb_array_t<ثابت contour_ټکی_t> contour_points,
                                           ثابت hb_array_t<ثابت صحيح> x_deltas,
                                           ثابت hb_array_t<ثابت صحيح> y_deltas,
                                           hb_iup_سېټ_t& forced_سېټ, /*بهر*/
                                           مزدوج tolerance = 0.0)
{
  بینښه len = contour_points.اوږدتیا;
  بینښه بل_i = 0;
  لكل (صحيح i = len - 1; i >= 0; i--)
  {
    بینښه وروستی_i = (len + i - 1) % len;
    لكل (بینښه j = 0; j < 2; j++)
    {
      مزدوج cj, lcj, ncj;
      مزدوج dj, ldj, ndj;
      إذا (j == 0)
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[وروستی_i].x);
        ldj = x_deltas.arrayZ[وروستی_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[بل_i].x);
        ndj = x_deltas.arrayZ[بل_i];
      }
      وإلا
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[وروستی_i].y);
        ldj = y_deltas.arrayZ[وروستی_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[بل_i].y);
        ndj = y_deltas.arrayZ[بل_i];
      }

      مزدوج c1, c2;
      مزدوج d1, d2;
      إذا (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      وإلا
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      منطقي force = خطأ;
      إذا (c1 == c2)
      {
        إذا (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = ریښتیا;
      }
      وإلا إذا (c1 <= cj && cj <= c2)
      {
        إذا (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = ریښتیا;
      }
      وإلا
      {
        إذا (d1 != d2)
        {
          إذا (cj < c1)
          {
            إذا (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = ریښتیا;
          }
          وإلا
          {
            إذا (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = ریښتیا;
          }
        }
      }

      إذا (force)
      {
        forced_سېټ.جمع_کړه (i);
        اكسر;
      }
    }
    بل_i = i;
  }
}

کينډۍ <ډول_نوم T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ساكن منطقي rotate_array (ثابت hb_array_t<ثابت T>& org_array,
                          صحيح k,
                          hb_ویکټور_t<T>& بهر)
{
  بینښه n = org_array.اوږدتیا;
  إذا (!n) أرجع ریښتیا;
  إذا (unlikely (!بهر.resize_dirty  (n)))
    أرجع خطأ;

  بینښه توکی_اندازه = hb_static_اندازه (T);
  إذا (k < 0)
    k = n - (-k) % n;
  وإلا
    k %= n;

  hb_memcpy ((فراغ *) بهر.arrayZ, (ثابت فراغ *) (org_array.arrayZ + n - k), k * توکی_اندازه);
  hb_memcpy ((فراغ *) (بهر.arrayZ + k), (ثابت فراغ *) org_array.arrayZ, (n - k) * توکی_اندازه);
  أرجع ریښتیا;
}

ساكن منطقي rotate_سېټ (ثابت hb_iup_سېټ_t& org_سېټ,
                        صحيح k,
                        بینښه n,
                        hb_iup_سېټ_t& بهر)
{
  إذا (!n) أرجع خطأ;
  k %= n;
  إذا (k < 0)
    k = n + k;

  إذا (k == 0)
  {
    بهر = org_سېټ;
  }
  وإلا
  {
    لكل (بینښه v : org_سېټ)
      بهر.جمع_کړه ((v + k) % n);
  }
  أرجع ریښتیا;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ساكن منطقي _iup_segment (ثابت hb_array_t<ثابت contour_ټکی_t> contour_points,
                          ثابت hb_array_t<ثابت صحيح> x_deltas,
                          ثابت hb_array_t<ثابت صحيح> y_deltas,
                          ثابت contour_ټکی_t& p1, ثابت contour_ټکی_t& p2,
                          صحيح p1_dx, صحيح p2_dx,
                          صحيح p1_dy, صحيح p2_dy,
			  مزدوج tolerance_sq,
                          hb_ویکټور_t<مزدوج>& interp_x_deltas, /*بهر*/
                          hb_ویکټور_t<مزدوج>& interp_y_deltas /*بهر*/)
{
  بینښه n = contour_points.اوږدتیا;
  إذا (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    أرجع خطأ;

  لكل (بینښه j = 0; j < 2; j++)
  {
    عائم contour_ټکی_t::* xp;
    مزدوج x1, x2, d1, d2;
    ثابت صحيح *دننه;
    مزدوج *بهر;
    إذا (j == 0)
    {
      xp = &contour_ټکی_t::x;
      x1 = static_cast<مزدوج> (p1.x);
      x2 = static_cast<مزدوج> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      دننه = x_deltas.arrayZ;
      بهر = interp_x_deltas.arrayZ;
    }
    وإلا
    {
      xp = &contour_ټکی_t::y;
      x1 = static_cast<مزدوج> (p1.y);
      x2 = static_cast<مزدوج> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      دننه = y_deltas.arrayZ;
      بهر = interp_y_deltas.arrayZ;
    }

    إذا (x1 == x2)
    {
      إذا (d1 == d2)
      {
        لكل (بینښه i = 0; i < n; i++)
          بهر[i] = d1;
      }
      وإلا
      {
        لكل (بینښه i = 0; i < n; i++)
          بهر[i] = 0.0;
      }
      استمر;
    }

    إذا (x1 > x2)
    {
      hb_بدلون (x1, x2);
      hb_بدلون (d1, d2);
    }

    مزدوج scale = (d2 - d1) / (x2 - x1);
    لكل (بینښه i = 0; i < n; i++)
    {
      مزدوج x = (مزدوج) (contour_points.arrayZ[i].*xp);
      مزدوج d;
      إذا (x <= x1)
        d = d1;
      وإلا إذا (x >= x2)
        d = d2;
      وإلا
        d = d1 + (x - x1) * scale;

      بهر[i] = d;
      مزدوج err = d - دننه[i];
      إذا (err * err > tolerance_sq)
	أرجع خطأ;
    }
  }
  أرجع ریښتیا;
}

ساكن منطقي can_iup_دننه_between (ثابت hb_array_t<ثابت contour_ټکی_t> contour_points,
                                 ثابت hb_array_t<ثابت صحيح> x_deltas,
                                 ثابت hb_array_t<ثابت صحيح> y_deltas,
                                 ثابت contour_ټکی_t& p1, ثابت contour_ټکی_t& p2,
                                 صحيح p1_dx, صحيح p2_dx,
                                 صحيح p1_dy, صحيح p2_dy,
                                 مزدوج tolerance_sq,
				 hb_ویکټور_t<مزدوج> &interp_x_deltas, /* scratch */
				 hb_ویکټور_t<مزدوج> &interp_y_deltas /* scratch */)
{
  إذا (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    أرجع خطأ;

  بینښه عدد = contour_points.اوږدتیا;

  لكل (بینښه i = 0; i < عدد; i++)
  {
    مزدوج dx = static_cast<مزدوج> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    مزدوج dy = static_cast<مزدوج> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    إذا (dx * dx + dy * dy > tolerance_sq)
      أرجع خطأ;
  }
  أرجع ریښتیا;
}

ساكن منطقي _iup_contour_optimize_dp (ثابت contour_ټکی_ویکټور_t& contour_points,
                                      ثابت hb_ویکټور_t<صحيح>& x_deltas,
                                      ثابت hb_ویکټور_t<صحيح>& y_deltas,
                                      ثابت hb_iup_سېټ_t& forced_سېټ,
                                      مزدوج tolerance_sq,
                                      بینښه lookback,
                                      hb_ویکټور_t<بینښه>& costs, /*بهر*/
                                      hb_ویکټور_t<صحيح>& chain, /*بهر*/
				      hb_ویکټور_t<مزدوج> &interp_x_deltas_scratch,
				      hb_ویکټور_t<مزدوج> &interp_y_deltas_scratch)
{
  بینښه n = contour_points.اوږدتیا;
  إذا (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    أرجع خطأ;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  لكل (بینښه i = 0; i < n; i++)
  {
    بینښه غوره_لګښت = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = غوره_لګښت;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    إذا (i > 0 && forced_سېټ.has (i - 1))
      استمر;

    صحيح lookback_شاخص = hb_max ((صحيح) i - (صحيح) lookback + 1, -1);
    لكل (صحيح j = i - 2; j >= lookback_شاخص; j--)
    {
      بینښه لګښت = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      بینښه عدد_points = i - j - 1;
      بینښه p1 = (j == -1 ? n - 1 : j);
      إذا (لګښت < غوره_لګښت &&
          can_iup_دننه_between (contour_points.as_array ().sub_array (j + 1, عدد_points),
                               x_deltas.as_array ().sub_array (j + 1, عدد_points),
                               y_deltas.as_array ().sub_array (j + 1, عدد_points),
                               contour_points.arrayZ[p1], contour_points.arrayZ[i],
                               x_deltas.arrayZ[p1], x_deltas.arrayZ[i],
                               y_deltas.arrayZ[p1], y_deltas.arrayZ[i],
                               tolerance_sq,
			       interp_x_deltas_scratch, interp_y_deltas_scratch))
      {
        غوره_لګښت = لګښت;
        costs.arrayZ[i] = غوره_لګښت;
        chain.arrayZ[i] = j;
      }

      إذا (j > 0 && forced_سېټ.has (j))
        اكسر;
    }
  }
  أرجع ریښتیا;
}

ساكن منطقي _iup_contour_optimize (ثابت hb_array_t<ثابت contour_ټکی_t> contour_points,
                                   ثابت hb_array_t<ثابت صحيح> x_deltas,
                                   ثابت hb_array_t<ثابت صحيح> y_deltas,
                                   hb_array_t<منطقي> opt_indices, /*بهر*/
                                   مزدوج tolerance,
				   iup_scratch_t &scratch)
{
  بینښه n = contour_points.اوږدتیا;
  إذا (opt_indices.اوږدتیا != n ||
      x_deltas.اوږدتیا != n ||
      y_deltas.اوږدتیا != n)
    أرجع خطأ;

  إذا (unlikely (n > hb_iup_سېټ_t::پاڼه_بټونه))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    لكل (بینښه i = 0; i < n; i++)
      opt_indices.arrayZ[i] = ریښتیا;
    أرجع ریښتیا;
  }

  منطقي all_within_tolerance = ریښتیا;
  مزدوج tolerance_sq = tolerance * tolerance;
  لكل (بینښه i = 0; i < n; i++)
  {
    صحيح dx = x_deltas.arrayZ[i];
    صحيح dy = y_deltas.arrayZ[i];
    إذا ((مزدوج) dx * dx + (مزدوج) dy * dy > tolerance_sq)
    {
      all_within_tolerance = خطأ;
      اكسر;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  إذا (all_within_tolerance)
    أرجع ریښتیا;

  /* If there's exactly one point, return it */
  إذا (n == 1)
  {
    opt_indices.arrayZ[0] = ریښتیا;
    أرجع ریښتیا;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  منطقي all_deltas_are_equal = ریښتیا;
  لكل (بینښه i = 1; i < n; i++)
    إذا (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = خطأ;
      اكسر;
    }

  إذا (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = ریښتیا;
    أرجع ریښتیا;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_سېټ_t forced_سېټ;
  iup_contour_bound_forced_سېټ (contour_points, x_deltas, y_deltas, forced_سېټ, tolerance);

  hb_ویکټور_t<بینښه> &costs = scratch.costs.reset ();
  hb_ویکټور_t<صحيح> &chain = scratch.chain.reset ();

  إذا (!forced_سېټ.is_empty ())
  {
    صحيح k = n - 1 - forced_سېټ.get_max ();
    إذا (k < 0)
      أرجع خطأ;

    hb_ویکټور_t<صحيح> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_ویکټور_t<صحيح> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_ټکی_ویکټور_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_سېټ_t rot_forced_سېټ;
    إذا (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_سېټ (forced_سېټ, k, n, rot_forced_سېټ))
      أرجع خطأ;

    إذا (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_سېټ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      أرجع خطأ;

    hb_iup_سېټ_t solution;
    صحيح شاخص_2 = n - 1;
    طالما (شاخص_2 != -1)
    {
      solution.جمع_کړه (شاخص_2);
      شاخص_2 = chain.arrayZ[شاخص_2];
    }

    إذا (solution.is_empty () ||
        forced_سېټ.get_population () > solution.get_population ())
      أرجع خطأ;

    لكل (بینښه i : solution)
      opt_indices.arrayZ[i] = ریښتیا;

    hb_ویکټور_t<منطقي> &rot_indices = scratch.rot_indices.reset ();
    ثابت hb_array_t<ثابت منطقي> opt_indices_array (opt_indices.arrayZ, opt_indices.اوږدتیا);
    إذا (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      أرجع خطأ;

    لكل (بینښه i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  وإلا
  {
    hb_ویکټور_t<صحيح> repeat_x_deltas, repeat_y_deltas;
    contour_ټکی_ویکټور_t repeat_points;

    إذا (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      أرجع خطأ;

    بینښه contour_ټکی_اندازه = hb_static_اندازه (contour_ټکی_t);
    لكل (بینښه i = 0; i < n; i++)
    {
      hb_memcpy ((فراغ *) repeat_x_deltas.arrayZ, (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_x_deltas.arrayZ + n), (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_y_deltas.arrayZ, (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_y_deltas.arrayZ + n), (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_points.arrayZ, (ثابت فراغ *) contour_points.arrayZ, n * contour_ټکی_اندازه);
      hb_memcpy ((فراغ *) (repeat_points.arrayZ + n), (ثابت فراغ *) contour_points.arrayZ, n * contour_ټکی_اندازه);
    }

    إذا (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_سېټ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      أرجع خطأ;

    بینښه غوره_لګښت = n + 1;
    صحيح len = costs.اوږدتیا;
    hb_iup_سېټ_t غوره_sol;
    لكل (صحيح پیل = n - 1; پیل < len; پیل++)
    {
      hb_iup_سېټ_t solution;
      صحيح i = پیل;
      صحيح lookback = پیل - (صحيح) n;
      طالما (i > lookback)
      {
        solution.جمع_کړه (i % n);
        i = chain.arrayZ[i];
      }
      إذا (i == lookback)
      {
        بینښه لګښت_i = i < 0 ? 0 : costs.arrayZ[i];
        بینښه لګښت = costs.arrayZ[پیل] - لګښت_i;
        إذا (لګښت <= غوره_لګښت)
        {
          غوره_sol = solution;
          غوره_لګښت = لګښت;
        }
      }
    }

    لكل (بینښه i = 0; i < n; i++)
      إذا (غوره_sol.has (i))
        opt_indices.arrayZ[i] = ریښتیا;
  }
  أرجع ریښتیا;
}

منطقي iup_delta_optimize (ثابت contour_ټکی_ویکټور_t& contour_points,
                         ثابت hb_ویکټور_t<صحيح>& x_deltas,
                         ثابت hb_ویکټور_t<صحيح>& y_deltas,
                         hb_ویکټور_t<منطقي>& opt_indices, /*بهر*/
			 iup_scratch_t &scratch,
                         مزدوج tolerance)
{
  إذا (!opt_indices.resize (contour_points.اوږدتیا))
      أرجع خطأ;

  hb_ویکټور_t<بینښه> &پای_ټکی_points = scratch.پای_ټکی_points.reset ();

  بینښه شمېر = contour_points.اوږدتیا;
  إذا (unlikely (!پای_ټکی_points.alloc (شمېر)))
    أرجع خطأ;

  إذا (شمېر < 4)
  {
    لكل (بینښه i = 0; i < شمېر; i++)
      پای_ټکی_points.push (i);
  }
  وإلا
  {
    لكل (بینښه i = 0; i < شمېر - 4; i++)
      إذا (contour_points.arrayZ[i].is_پای_ټکی_ټکی)
        پای_ټکی_points.push (i);

    /* phantom points */
    لكل (بینښه i = شمېر - 4; i < شمېر; i++)
      پای_ټکی_points.push (i);
  }

  إذا (پای_ټکی_points.دننه_error ()) أرجع خطأ;

  بینښه پیل = 0;
  لكل (بینښه پای_ټکی : پای_ټکی_points)
  {
    بینښه len = پای_ټکی - پیل + 1;
    إذا (!_iup_contour_optimize (contour_points.as_array ().sub_array (پیل, len),
                                x_deltas.as_array ().sub_array (پیل, len),
                                y_deltas.as_array ().sub_array (پیل, len),
                                opt_indices.as_array ().sub_array (پیل, len),
                                tolerance,
				scratch))
      أرجع خطأ;
    پیل = پای_ټکی + 1;
  }
  أرجع ریښتیا;
}
