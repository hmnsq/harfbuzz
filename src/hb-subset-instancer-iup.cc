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

استعمال hb_iup_سيٽ_t = hb_ٻٽو_انگ_صفحو_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ساكن بنانشان MAX_LOOKBACK = 8;

ساكن فراغ iup_contour_bound_forced_سيٽ (ثابت hb_array_t<ثابت contour_نقطو_t> contour_points,
                                           ثابت hb_array_t<ثابت صحيح> x_deltas,
                                           ثابت hb_array_t<ثابت صحيح> y_deltas,
                                           hb_iup_سيٽ_t& forced_سيٽ, /*ٻاهريون*/
                                           مزدوج tolerance = 0.0)
{
  بنانشان len = contour_points.ڊيگهائي;
  بنانشان ايندڙ_i = 0;
  لكل (صحيح i = len - 1; i >= 0; i--)
  {
    بنانشان آخري_i = (len + i - 1) % len;
    لكل (بنانشان j = 0; j < 2; j++)
    {
      مزدوج cj, lcj, ncj;
      مزدوج dj, ldj, ndj;
      إذا (j == 0)
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[آخري_i].x);
        ldj = x_deltas.arrayZ[آخري_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[ايندڙ_i].x);
        ndj = x_deltas.arrayZ[ايندڙ_i];
      }
      وإلا
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[آخري_i].y);
        ldj = y_deltas.arrayZ[آخري_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[ايندڙ_i].y);
        ndj = y_deltas.arrayZ[ايندڙ_i];
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
          force = سچ;
      }
      وإلا إذا (c1 <= cj && cj <= c2)
      {
        إذا (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = سچ;
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
                force = سچ;
          }
          وإلا
          {
            إذا (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = سچ;
          }
        }
      }

      إذا (force)
      {
        forced_سيٽ.جوڙيو (i);
        اكسر;
      }
    }
    ايندڙ_i = i;
  }
}

سانچو <قسم_نالو T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ساكن منطقي rotate_array (ثابت hb_array_t<ثابت T>& org_array,
                          صحيح k,
                          hb_ويڪٽر_t<T>& ٻاهريون)
{
  بنانشان n = org_array.ڊيگهائي;
  إذا (!n) أرجع سچ;
  إذا (unlikely (!ٻاهريون.resize_dirty  (n)))
    أرجع خطأ;

  بنانشان شي_سائيز = hb_static_سائيز (T);
  إذا (k < 0)
    k = n - (-k) % n;
  وإلا
    k %= n;

  hb_memcpy ((فراغ *) ٻاهريون.arrayZ, (ثابت فراغ *) (org_array.arrayZ + n - k), k * شي_سائيز);
  hb_memcpy ((فراغ *) (ٻاهريون.arrayZ + k), (ثابت فراغ *) org_array.arrayZ, (n - k) * شي_سائيز);
  أرجع سچ;
}

ساكن منطقي rotate_سيٽ (ثابت hb_iup_سيٽ_t& org_سيٽ,
                        صحيح k,
                        بنانشان n,
                        hb_iup_سيٽ_t& ٻاهريون)
{
  إذا (!n) أرجع خطأ;
  k %= n;
  إذا (k < 0)
    k = n + k;

  إذا (k == 0)
  {
    ٻاهريون = org_سيٽ;
  }
  وإلا
  {
    لكل (بنانشان v : org_سيٽ)
      ٻاهريون.جوڙيو ((v + k) % n);
  }
  أرجع سچ;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ساكن منطقي _iup_segment (ثابت hb_array_t<ثابت contour_نقطو_t> contour_points,
                          ثابت hb_array_t<ثابت صحيح> x_deltas,
                          ثابت hb_array_t<ثابت صحيح> y_deltas,
                          ثابت contour_نقطو_t& p1, ثابت contour_نقطو_t& p2,
                          صحيح p1_dx, صحيح p2_dx,
                          صحيح p1_dy, صحيح p2_dy,
			  مزدوج tolerance_sq,
                          hb_ويڪٽر_t<مزدوج>& interp_x_deltas, /*ٻاهريون*/
                          hb_ويڪٽر_t<مزدوج>& interp_y_deltas /*ٻاهريون*/)
{
  بنانشان n = contour_points.ڊيگهائي;
  إذا (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    أرجع خطأ;

  لكل (بنانشان j = 0; j < 2; j++)
  {
    عائم contour_نقطو_t::* xp;
    مزدوج x1, x2, d1, d2;
    ثابت صحيح *اندريون;
    مزدوج *ٻاهريون;
    إذا (j == 0)
    {
      xp = &contour_نقطو_t::x;
      x1 = static_cast<مزدوج> (p1.x);
      x2 = static_cast<مزدوج> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      اندريون = x_deltas.arrayZ;
      ٻاهريون = interp_x_deltas.arrayZ;
    }
    وإلا
    {
      xp = &contour_نقطو_t::y;
      x1 = static_cast<مزدوج> (p1.y);
      x2 = static_cast<مزدوج> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      اندريون = y_deltas.arrayZ;
      ٻاهريون = interp_y_deltas.arrayZ;
    }

    إذا (x1 == x2)
    {
      إذا (d1 == d2)
      {
        لكل (بنانشان i = 0; i < n; i++)
          ٻاهريون[i] = d1;
      }
      وإلا
      {
        لكل (بنانشان i = 0; i < n; i++)
          ٻاهريون[i] = 0.0;
      }
      استمر;
    }

    إذا (x1 > x2)
    {
      hb_مٽا_سٽا (x1, x2);
      hb_مٽا_سٽا (d1, d2);
    }

    مزدوج scale = (d2 - d1) / (x2 - x1);
    لكل (بنانشان i = 0; i < n; i++)
    {
      مزدوج x = (مزدوج) (contour_points.arrayZ[i].*xp);
      مزدوج d;
      إذا (x <= x1)
        d = d1;
      وإلا إذا (x >= x2)
        d = d2;
      وإلا
        d = d1 + (x - x1) * scale;

      ٻاهريون[i] = d;
      مزدوج err = d - اندريون[i];
      إذا (err * err > tolerance_sq)
	أرجع خطأ;
    }
  }
  أرجع سچ;
}

ساكن منطقي can_iup_اندريون_between (ثابت hb_array_t<ثابت contour_نقطو_t> contour_points,
                                 ثابت hb_array_t<ثابت صحيح> x_deltas,
                                 ثابت hb_array_t<ثابت صحيح> y_deltas,
                                 ثابت contour_نقطو_t& p1, ثابت contour_نقطو_t& p2,
                                 صحيح p1_dx, صحيح p2_dx,
                                 صحيح p1_dy, صحيح p2_dy,
                                 مزدوج tolerance_sq,
				 hb_ويڪٽر_t<مزدوج> &interp_x_deltas, /* scratch */
				 hb_ويڪٽر_t<مزدوج> &interp_y_deltas /* scratch */)
{
  إذا (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    أرجع خطأ;

  بنانشان انگ = contour_points.ڊيگهائي;

  لكل (بنانشان i = 0; i < انگ; i++)
  {
    مزدوج dx = static_cast<مزدوج> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    مزدوج dy = static_cast<مزدوج> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    إذا (dx * dx + dy * dy > tolerance_sq)
      أرجع خطأ;
  }
  أرجع سچ;
}

ساكن منطقي _iup_contour_optimize_dp (ثابت contour_نقطو_ويڪٽر_t& contour_points,
                                      ثابت hb_ويڪٽر_t<صحيح>& x_deltas,
                                      ثابت hb_ويڪٽر_t<صحيح>& y_deltas,
                                      ثابت hb_iup_سيٽ_t& forced_سيٽ,
                                      مزدوج tolerance_sq,
                                      بنانشان lookback,
                                      hb_ويڪٽر_t<بنانشان>& costs, /*ٻاهريون*/
                                      hb_ويڪٽر_t<صحيح>& chain, /*ٻاهريون*/
				      hb_ويڪٽر_t<مزدوج> &interp_x_deltas_scratch,
				      hb_ويڪٽر_t<مزدوج> &interp_y_deltas_scratch)
{
  بنانشان n = contour_points.ڊيگهائي;
  إذا (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    أرجع خطأ;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  لكل (بنانشان i = 0; i < n; i++)
  {
    بنانشان بهترين_لاڳت = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = بهترين_لاڳت;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    إذا (i > 0 && forced_سيٽ.has (i - 1))
      استمر;

    صحيح lookback_اشاريو = hb_max ((صحيح) i - (صحيح) lookback + 1, -1);
    لكل (صحيح j = i - 2; j >= lookback_اشاريو; j--)
    {
      بنانشان لاڳت = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      بنانشان انگ_points = i - j - 1;
      بنانشان p1 = (j == -1 ? n - 1 : j);
      إذا (لاڳت < بهترين_لاڳت &&
          can_iup_اندريون_between (contour_points.as_array ().sub_array (j + 1, انگ_points),
                               x_deltas.as_array ().sub_array (j + 1, انگ_points),
                               y_deltas.as_array ().sub_array (j + 1, انگ_points),
                               contour_points.arrayZ[p1], contour_points.arrayZ[i],
                               x_deltas.arrayZ[p1], x_deltas.arrayZ[i],
                               y_deltas.arrayZ[p1], y_deltas.arrayZ[i],
                               tolerance_sq,
			       interp_x_deltas_scratch, interp_y_deltas_scratch))
      {
        بهترين_لاڳت = لاڳت;
        costs.arrayZ[i] = بهترين_لاڳت;
        chain.arrayZ[i] = j;
      }

      إذا (j > 0 && forced_سيٽ.has (j))
        اكسر;
    }
  }
  أرجع سچ;
}

ساكن منطقي _iup_contour_optimize (ثابت hb_array_t<ثابت contour_نقطو_t> contour_points,
                                   ثابت hb_array_t<ثابت صحيح> x_deltas,
                                   ثابت hb_array_t<ثابت صحيح> y_deltas,
                                   hb_array_t<منطقي> opt_indices, /*ٻاهريون*/
                                   مزدوج tolerance,
				   iup_scratch_t &scratch)
{
  بنانشان n = contour_points.ڊيگهائي;
  إذا (opt_indices.ڊيگهائي != n ||
      x_deltas.ڊيگهائي != n ||
      y_deltas.ڊيگهائي != n)
    أرجع خطأ;

  إذا (unlikely (n > hb_iup_سيٽ_t::صفحو_بٽ))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    لكل (بنانشان i = 0; i < n; i++)
      opt_indices.arrayZ[i] = سچ;
    أرجع سچ;
  }

  منطقي all_within_tolerance = سچ;
  مزدوج tolerance_sq = tolerance * tolerance;
  لكل (بنانشان i = 0; i < n; i++)
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
    أرجع سچ;

  /* If there's exactly one point, return it */
  إذا (n == 1)
  {
    opt_indices.arrayZ[0] = سچ;
    أرجع سچ;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  منطقي all_deltas_are_equal = سچ;
  لكل (بنانشان i = 1; i < n; i++)
    إذا (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = خطأ;
      اكسر;
    }

  إذا (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = سچ;
    أرجع سچ;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_سيٽ_t forced_سيٽ;
  iup_contour_bound_forced_سيٽ (contour_points, x_deltas, y_deltas, forced_سيٽ, tolerance);

  hb_ويڪٽر_t<بنانشان> &costs = scratch.costs.reset ();
  hb_ويڪٽر_t<صحيح> &chain = scratch.chain.reset ();

  إذا (!forced_سيٽ.is_empty ())
  {
    صحيح k = n - 1 - forced_سيٽ.get_max ();
    إذا (k < 0)
      أرجع خطأ;

    hb_ويڪٽر_t<صحيح> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_ويڪٽر_t<صحيح> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_نقطو_ويڪٽر_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_سيٽ_t rot_forced_سيٽ;
    إذا (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_سيٽ (forced_سيٽ, k, n, rot_forced_سيٽ))
      أرجع خطأ;

    إذا (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_سيٽ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      أرجع خطأ;

    hb_iup_سيٽ_t solution;
    صحيح اشاريو_2 = n - 1;
    طالما (اشاريو_2 != -1)
    {
      solution.جوڙيو (اشاريو_2);
      اشاريو_2 = chain.arrayZ[اشاريو_2];
    }

    إذا (solution.is_empty () ||
        forced_سيٽ.get_population () > solution.get_population ())
      أرجع خطأ;

    لكل (بنانشان i : solution)
      opt_indices.arrayZ[i] = سچ;

    hb_ويڪٽر_t<منطقي> &rot_indices = scratch.rot_indices.reset ();
    ثابت hb_array_t<ثابت منطقي> opt_indices_array (opt_indices.arrayZ, opt_indices.ڊيگهائي);
    إذا (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      أرجع خطأ;

    لكل (بنانشان i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  وإلا
  {
    hb_ويڪٽر_t<صحيح> repeat_x_deltas, repeat_y_deltas;
    contour_نقطو_ويڪٽر_t repeat_points;

    إذا (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      أرجع خطأ;

    بنانشان contour_نقطو_سائيز = hb_static_سائيز (contour_نقطو_t);
    لكل (بنانشان i = 0; i < n; i++)
    {
      hb_memcpy ((فراغ *) repeat_x_deltas.arrayZ, (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_x_deltas.arrayZ + n), (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_y_deltas.arrayZ, (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_y_deltas.arrayZ + n), (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_points.arrayZ, (ثابت فراغ *) contour_points.arrayZ, n * contour_نقطو_سائيز);
      hb_memcpy ((فراغ *) (repeat_points.arrayZ + n), (ثابت فراغ *) contour_points.arrayZ, n * contour_نقطو_سائيز);
    }

    إذا (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_سيٽ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      أرجع خطأ;

    بنانشان بهترين_لاڳت = n + 1;
    صحيح len = costs.ڊيگهائي;
    hb_iup_سيٽ_t بهترين_sol;
    لكل (صحيح شروعات = n - 1; شروعات < len; شروعات++)
    {
      hb_iup_سيٽ_t solution;
      صحيح i = شروعات;
      صحيح lookback = شروعات - (صحيح) n;
      طالما (i > lookback)
      {
        solution.جوڙيو (i % n);
        i = chain.arrayZ[i];
      }
      إذا (i == lookback)
      {
        بنانشان لاڳت_i = i < 0 ? 0 : costs.arrayZ[i];
        بنانشان لاڳت = costs.arrayZ[شروعات] - لاڳت_i;
        إذا (لاڳت <= بهترين_لاڳت)
        {
          بهترين_sol = solution;
          بهترين_لاڳت = لاڳت;
        }
      }
    }

    لكل (بنانشان i = 0; i < n; i++)
      إذا (بهترين_sol.has (i))
        opt_indices.arrayZ[i] = سچ;
  }
  أرجع سچ;
}

منطقي iup_delta_optimize (ثابت contour_نقطو_ويڪٽر_t& contour_points,
                         ثابت hb_ويڪٽر_t<صحيح>& x_deltas,
                         ثابت hb_ويڪٽر_t<صحيح>& y_deltas,
                         hb_ويڪٽر_t<منطقي>& opt_indices, /*ٻاهريون*/
			 iup_scratch_t &scratch,
                         مزدوج tolerance)
{
  إذا (!opt_indices.resize (contour_points.ڊيگهائي))
      أرجع خطأ;

  hb_ويڪٽر_t<بنانشان> &پڇاڙي_points = scratch.پڇاڙي_points.reset ();

  بنانشان ڳڻپ = contour_points.ڊيگهائي;
  إذا (unlikely (!پڇاڙي_points.alloc (ڳڻپ)))
    أرجع خطأ;

  إذا (ڳڻپ < 4)
  {
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      پڇاڙي_points.push (i);
  }
  وإلا
  {
    لكل (بنانشان i = 0; i < ڳڻپ - 4; i++)
      إذا (contour_points.arrayZ[i].is_پڇاڙي_نقطو)
        پڇاڙي_points.push (i);

    /* phantom points */
    لكل (بنانشان i = ڳڻپ - 4; i < ڳڻپ; i++)
      پڇاڙي_points.push (i);
  }

  إذا (پڇاڙي_points.اندريون_error ()) أرجع خطأ;

  بنانشان شروعات = 0;
  لكل (بنانشان پڇاڙي : پڇاڙي_points)
  {
    بنانشان len = پڇاڙي - شروعات + 1;
    إذا (!_iup_contour_optimize (contour_points.as_array ().sub_array (شروعات, len),
                                x_deltas.as_array ().sub_array (شروعات, len),
                                y_deltas.as_array ().sub_array (شروعات, len),
                                opt_indices.as_array ().sub_array (شروعات, len),
                                tolerance,
				scratch))
      أرجع خطأ;
    شروعات = پڇاڙي + 1;
  }
  أرجع سچ;
}
