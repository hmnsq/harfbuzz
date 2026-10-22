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

ورتوں hb_iup_سیٹ_t = hb_ثنائی_ہندسہ_ورقہ_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ساكن بنانشانی MAX_LOOKBACK = 8;

ساكن فراغ iup_contour_bound_forced_سیٹ (ثابت hb_array_t<ثابت contour_نقطہ_t> contour_points,
                                           ثابت hb_array_t<ثابت صحيح> x_deltas,
                                           ثابت hb_array_t<ثابت صحيح> y_deltas,
                                           hb_iup_سیٹ_t& forced_سیٹ, /*باہرلا*/
                                           مزدوج tolerance = 0.0)
{
  بنانشانی len = contour_points.لمائی;
  بنانشانی اگلا_i = 0;
  لكل (صحيح i = len - 1; i >= 0; i--)
  {
    بنانشانی آخری_i = (len + i - 1) % len;
    لكل (بنانشانی j = 0; j < 2; j++)
    {
      مزدوج cj, lcj, ncj;
      مزدوج dj, ldj, ndj;
      إذا (j == 0)
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[آخری_i].x);
        ldj = x_deltas.arrayZ[آخری_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[اگلا_i].x);
        ndj = x_deltas.arrayZ[اگلا_i];
      }
      وإلا
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[آخری_i].y);
        ldj = y_deltas.arrayZ[آخری_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[اگلا_i].y);
        ndj = y_deltas.arrayZ[اگلا_i];
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
        forced_سیٹ.جوڑو (i);
        اكسر;
      }
    }
    اگلا_i = i;
  }
}

سانچا <قسم_ناں T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ساكن منطقي rotate_array (ثابت hb_array_t<ثابت T>& org_array,
                          صحيح k,
                          hb_سمتیہ_t<T>& باہرلا)
{
  بنانشانی n = org_array.لمائی;
  إذا (!n) أرجع سچ;
  إذا (unlikely (!باہرلا.resize_dirty  (n)))
    أرجع خطأ;

  بنانشانی شے_ناپ = hb_static_ناپ (T);
  إذا (k < 0)
    k = n - (-k) % n;
  وإلا
    k %= n;

  hb_memcpy ((فراغ *) باہرلا.arrayZ, (ثابت فراغ *) (org_array.arrayZ + n - k), k * شے_ناپ);
  hb_memcpy ((فراغ *) (باہرلا.arrayZ + k), (ثابت فراغ *) org_array.arrayZ, (n - k) * شے_ناپ);
  أرجع سچ;
}

ساكن منطقي rotate_سیٹ (ثابت hb_iup_سیٹ_t& org_سیٹ,
                        صحيح k,
                        بنانشانی n,
                        hb_iup_سیٹ_t& باہرلا)
{
  إذا (!n) أرجع خطأ;
  k %= n;
  إذا (k < 0)
    k = n + k;

  إذا (k == 0)
  {
    باہرلا = org_سیٹ;
  }
  وإلا
  {
    لكل (بنانشانی v : org_سیٹ)
      باہرلا.جوڑو ((v + k) % n);
  }
  أرجع سچ;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ساكن منطقي _iup_segment (ثابت hb_array_t<ثابت contour_نقطہ_t> contour_points,
                          ثابت hb_array_t<ثابت صحيح> x_deltas,
                          ثابت hb_array_t<ثابت صحيح> y_deltas,
                          ثابت contour_نقطہ_t& p1, ثابت contour_نقطہ_t& p2,
                          صحيح p1_dx, صحيح p2_dx,
                          صحيح p1_dy, صحيح p2_dy,
			  مزدوج tolerance_sq,
                          hb_سمتیہ_t<مزدوج>& interp_x_deltas, /*باہرلا*/
                          hb_سمتیہ_t<مزدوج>& interp_y_deltas /*باہرلا*/)
{
  بنانشانی n = contour_points.لمائی;
  إذا (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    أرجع خطأ;

  لكل (بنانشانی j = 0; j < 2; j++)
  {
    عائم contour_نقطہ_t::* xp;
    مزدوج x1, x2, d1, d2;
    ثابت صحيح *وچ;
    مزدوج *باہرلا;
    إذا (j == 0)
    {
      xp = &contour_نقطہ_t::x;
      x1 = static_cast<مزدوج> (p1.x);
      x2 = static_cast<مزدوج> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      وچ = x_deltas.arrayZ;
      باہرلا = interp_x_deltas.arrayZ;
    }
    وإلا
    {
      xp = &contour_نقطہ_t::y;
      x1 = static_cast<مزدوج> (p1.y);
      x2 = static_cast<مزدوج> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      وچ = y_deltas.arrayZ;
      باہرلا = interp_y_deltas.arrayZ;
    }

    إذا (x1 == x2)
    {
      إذا (d1 == d2)
      {
        لكل (بنانشانی i = 0; i < n; i++)
          باہرلا[i] = d1;
      }
      وإلا
      {
        لكل (بنانشانی i = 0; i < n; i++)
          باہرلا[i] = 0.0;
      }
      استمر;
    }

    إذا (x1 > x2)
    {
      hb_وٹاندرا (x1, x2);
      hb_وٹاندرا (d1, d2);
    }

    مزدوج scale = (d2 - d1) / (x2 - x1);
    لكل (بنانشانی i = 0; i < n; i++)
    {
      مزدوج x = (مزدوج) (contour_points.arrayZ[i].*xp);
      مزدوج d;
      إذا (x <= x1)
        d = d1;
      وإلا إذا (x >= x2)
        d = d2;
      وإلا
        d = d1 + (x - x1) * scale;

      باہرلا[i] = d;
      مزدوج err = d - وچ[i];
      إذا (err * err > tolerance_sq)
	أرجع خطأ;
    }
  }
  أرجع سچ;
}

ساكن منطقي can_iup_وچ_between (ثابت hb_array_t<ثابت contour_نقطہ_t> contour_points,
                                 ثابت hb_array_t<ثابت صحيح> x_deltas,
                                 ثابت hb_array_t<ثابت صحيح> y_deltas,
                                 ثابت contour_نقطہ_t& p1, ثابت contour_نقطہ_t& p2,
                                 صحيح p1_dx, صحيح p2_dx,
                                 صحيح p1_dy, صحيح p2_dy,
                                 مزدوج tolerance_sq,
				 hb_سمتیہ_t<مزدوج> &interp_x_deltas, /* scratch */
				 hb_سمتیہ_t<مزدوج> &interp_y_deltas /* scratch */)
{
  إذا (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    أرجع خطأ;

  بنانشانی عدد = contour_points.لمائی;

  لكل (بنانشانی i = 0; i < عدد; i++)
  {
    مزدوج dx = static_cast<مزدوج> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    مزدوج dy = static_cast<مزدوج> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    إذا (dx * dx + dy * dy > tolerance_sq)
      أرجع خطأ;
  }
  أرجع سچ;
}

ساكن منطقي _iup_contour_optimize_dp (ثابت contour_نقطہ_سمتیہ_t& contour_points,
                                      ثابت hb_سمتیہ_t<صحيح>& x_deltas,
                                      ثابت hb_سمتیہ_t<صحيح>& y_deltas,
                                      ثابت hb_iup_سیٹ_t& forced_سیٹ,
                                      مزدوج tolerance_sq,
                                      بنانشانی lookback,
                                      hb_سمتیہ_t<بنانشانی>& costs, /*باہرلا*/
                                      hb_سمتیہ_t<صحيح>& chain, /*باہرلا*/
				      hb_سمتیہ_t<مزدوج> &interp_x_deltas_scratch,
				      hb_سمتیہ_t<مزدوج> &interp_y_deltas_scratch)
{
  بنانشانی n = contour_points.لمائی;
  إذا (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    أرجع خطأ;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  لكل (بنانشانی i = 0; i < n; i++)
  {
    بنانشانی بہترین_لاگت = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = بہترین_لاگت;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    إذا (i > 0 && forced_سیٹ.has (i - 1))
      استمر;

    صحيح lookback_اشاریہ = hb_max ((صحيح) i - (صحيح) lookback + 1, -1);
    لكل (صحيح j = i - 2; j >= lookback_اشاریہ; j--)
    {
      بنانشانی لاگت = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      بنانشانی عدد_points = i - j - 1;
      بنانشانی p1 = (j == -1 ? n - 1 : j);
      إذا (لاگت < بہترین_لاگت &&
          can_iup_وچ_between (contour_points.as_array ().sub_array (j + 1, عدد_points),
                               x_deltas.as_array ().sub_array (j + 1, عدد_points),
                               y_deltas.as_array ().sub_array (j + 1, عدد_points),
                               contour_points.arrayZ[p1], contour_points.arrayZ[i],
                               x_deltas.arrayZ[p1], x_deltas.arrayZ[i],
                               y_deltas.arrayZ[p1], y_deltas.arrayZ[i],
                               tolerance_sq,
			       interp_x_deltas_scratch, interp_y_deltas_scratch))
      {
        بہترین_لاگت = لاگت;
        costs.arrayZ[i] = بہترین_لاگت;
        chain.arrayZ[i] = j;
      }

      إذا (j > 0 && forced_سیٹ.has (j))
        اكسر;
    }
  }
  أرجع سچ;
}

ساكن منطقي _iup_contour_optimize (ثابت hb_array_t<ثابت contour_نقطہ_t> contour_points,
                                   ثابت hb_array_t<ثابت صحيح> x_deltas,
                                   ثابت hb_array_t<ثابت صحيح> y_deltas,
                                   hb_array_t<منطقي> opt_indices, /*باہرلا*/
                                   مزدوج tolerance,
				   iup_scratch_t &scratch)
{
  بنانشانی n = contour_points.لمائی;
  إذا (opt_indices.لمائی != n ||
      x_deltas.لمائی != n ||
      y_deltas.لمائی != n)
    أرجع خطأ;

  إذا (unlikely (n > hb_iup_سیٹ_t::ورقہ_بٹ))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    لكل (بنانشانی i = 0; i < n; i++)
      opt_indices.arrayZ[i] = سچ;
    أرجع سچ;
  }

  منطقي all_within_tolerance = سچ;
  مزدوج tolerance_sq = tolerance * tolerance;
  لكل (بنانشانی i = 0; i < n; i++)
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
  لكل (بنانشانی i = 1; i < n; i++)
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
  hb_iup_سیٹ_t forced_سیٹ;
  iup_contour_bound_forced_سیٹ (contour_points, x_deltas, y_deltas, forced_سیٹ, tolerance);

  hb_سمتیہ_t<بنانشانی> &costs = scratch.costs.reset ();
  hb_سمتیہ_t<صحيح> &chain = scratch.chain.reset ();

  إذا (!forced_سیٹ.is_empty ())
  {
    صحيح k = n - 1 - forced_سیٹ.get_max ();
    إذا (k < 0)
      أرجع خطأ;

    hb_سمتیہ_t<صحيح> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_سمتیہ_t<صحيح> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_نقطہ_سمتیہ_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_سیٹ_t rot_forced_سیٹ;
    إذا (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_سیٹ (forced_سیٹ, k, n, rot_forced_سیٹ))
      أرجع خطأ;

    إذا (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_سیٹ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      أرجع خطأ;

    hb_iup_سیٹ_t solution;
    صحيح اشاریہ_2 = n - 1;
    طالما (اشاریہ_2 != -1)
    {
      solution.جوڑو (اشاریہ_2);
      اشاریہ_2 = chain.arrayZ[اشاریہ_2];
    }

    إذا (solution.is_empty () ||
        forced_سیٹ.get_population () > solution.get_population ())
      أرجع خطأ;

    لكل (بنانشانی i : solution)
      opt_indices.arrayZ[i] = سچ;

    hb_سمتیہ_t<منطقي> &rot_indices = scratch.rot_indices.reset ();
    ثابت hb_array_t<ثابت منطقي> opt_indices_array (opt_indices.arrayZ, opt_indices.لمائی);
    إذا (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      أرجع خطأ;

    لكل (بنانشانی i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  وإلا
  {
    hb_سمتیہ_t<صحيح> repeat_x_deltas, repeat_y_deltas;
    contour_نقطہ_سمتیہ_t repeat_points;

    إذا (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      أرجع خطأ;

    بنانشانی contour_نقطہ_ناپ = hb_static_ناپ (contour_نقطہ_t);
    لكل (بنانشانی i = 0; i < n; i++)
    {
      hb_memcpy ((فراغ *) repeat_x_deltas.arrayZ, (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_x_deltas.arrayZ + n), (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_y_deltas.arrayZ, (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_y_deltas.arrayZ + n), (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_points.arrayZ, (ثابت فراغ *) contour_points.arrayZ, n * contour_نقطہ_ناپ);
      hb_memcpy ((فراغ *) (repeat_points.arrayZ + n), (ثابت فراغ *) contour_points.arrayZ, n * contour_نقطہ_ناپ);
    }

    إذا (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_سیٹ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      أرجع خطأ;

    بنانشانی بہترین_لاگت = n + 1;
    صحيح len = costs.لمائی;
    hb_iup_سیٹ_t بہترین_sol;
    لكل (صحيح شروع = n - 1; شروع < len; شروع++)
    {
      hb_iup_سیٹ_t solution;
      صحيح i = شروع;
      صحيح lookback = شروع - (صحيح) n;
      طالما (i > lookback)
      {
        solution.جوڑو (i % n);
        i = chain.arrayZ[i];
      }
      إذا (i == lookback)
      {
        بنانشانی لاگت_i = i < 0 ? 0 : costs.arrayZ[i];
        بنانشانی لاگت = costs.arrayZ[شروع] - لاگت_i;
        إذا (لاگت <= بہترین_لاگت)
        {
          بہترین_sol = solution;
          بہترین_لاگت = لاگت;
        }
      }
    }

    لكل (بنانشانی i = 0; i < n; i++)
      إذا (بہترین_sol.has (i))
        opt_indices.arrayZ[i] = سچ;
  }
  أرجع سچ;
}

منطقي iup_delta_optimize (ثابت contour_نقطہ_سمتیہ_t& contour_points,
                         ثابت hb_سمتیہ_t<صحيح>& x_deltas,
                         ثابت hb_سمتیہ_t<صحيح>& y_deltas,
                         hb_سمتیہ_t<منطقي>& opt_indices, /*باہرلا*/
			 iup_scratch_t &scratch,
                         مزدوج tolerance)
{
  إذا (!opt_indices.resize (contour_points.لمائی))
      أرجع خطأ;

  hb_سمتیہ_t<بنانشانی> &سرا_points = scratch.سرا_points.reset ();

  بنانشانی گنتی = contour_points.لمائی;
  إذا (unlikely (!سرا_points.alloc (گنتی)))
    أرجع خطأ;

  إذا (گنتی < 4)
  {
    لكل (بنانشانی i = 0; i < گنتی; i++)
      سرا_points.push (i);
  }
  وإلا
  {
    لكل (بنانشانی i = 0; i < گنتی - 4; i++)
      إذا (contour_points.arrayZ[i].is_سرا_نقطہ)
        سرا_points.push (i);

    /* phantom points */
    لكل (بنانشانی i = گنتی - 4; i < گنتی; i++)
      سرا_points.push (i);
  }

  إذا (سرا_points.وچ_error ()) أرجع خطأ;

  بنانشانی شروع = 0;
  لكل (بنانشانی سرا : سرا_points)
  {
    بنانشانی len = سرا - شروع + 1;
    إذا (!_iup_contour_optimize (contour_points.as_array ().sub_array (شروع, len),
                                x_deltas.as_array ().sub_array (شروع, len),
                                y_deltas.as_array ().sub_array (شروع, len),
                                opt_indices.as_array ().sub_array (شروع, len),
                                tolerance,
				scratch))
      أرجع خطأ;
    شروع = سرا + 1;
  }
  أرجع سچ;
}
