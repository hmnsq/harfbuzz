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

#شامل "hb-subset-instancer-iup.hh"

#شامل "hb-bit-page.hh"

استعمال hb_iup_سیٹ_t = hb_ثنائی_ہندسہ_صفحہ_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ساکن غیرعلامتی MAX_LOOKBACK = 8;

ساکن خالی iup_contour_bound_forced_سیٹ (ثابت hb_array_t<ثابت contour_نقطہ_t> contour_points,
                                           ثابت hb_array_t<ثابت صحیح> x_deltas,
                                           ثابت hb_array_t<ثابت صحیح> y_deltas,
                                           hb_iup_سیٹ_t& forced_سیٹ, /*برآمد*/
                                           دوہرا tolerance = 0.0)
{
  غیرعلامتی len = contour_points.لمبائی;
  غیرعلامتی اگلا_i = 0;
  برائے (صحیح i = len - 1; i >= 0; i--)
  {
    غیرعلامتی آخری_i = (len + i - 1) % len;
    برائے (غیرعلامتی j = 0; j < 2; j++)
    {
      دوہرا cj, lcj, ncj;
      دوہرا dj, ldj, ndj;
      اگر (j == 0)
      {
        cj = static_cast<دوہرا> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<دوہرا> (contour_points.arrayZ[آخری_i].x);
        ldj = x_deltas.arrayZ[آخری_i];
        ncj = static_cast<دوہرا> (contour_points.arrayZ[اگلا_i].x);
        ndj = x_deltas.arrayZ[اگلا_i];
      }
      ورنہ
      {
        cj = static_cast<دوہرا> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<دوہرا> (contour_points.arrayZ[آخری_i].y);
        ldj = y_deltas.arrayZ[آخری_i];
        ncj = static_cast<دوہرا> (contour_points.arrayZ[اگلا_i].y);
        ndj = y_deltas.arrayZ[اگلا_i];
      }

      دوہرا c1, c2;
      دوہرا d1, d2;
      اگر (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      ورنہ
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      منطقی force = جھوٹ;
      اگر (c1 == c2)
      {
        اگر (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = سچ;
      }
      ورنہ اگر (c1 <= cj && cj <= c2)
      {
        اگر (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = سچ;
      }
      ورنہ
      {
        اگر (d1 != d2)
        {
          اگر (cj < c1)
          {
            اگر (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = سچ;
          }
          ورنہ
          {
            اگر (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = سچ;
          }
        }
      }

      اگر (force)
      {
        forced_سیٹ.جوڑو (i);
        توڑو;
      }
    }
    اگلا_i = i;
  }
}

سانچہ <قسم_نام T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ساکن منطقی rotate_array (ثابت hb_array_t<ثابت T>& org_array,
                          صحیح k,
                          hb_سمتیہ_t<T>& برآمد)
{
  غیرعلامتی n = org_array.لمبائی;
  اگر (!n) واپس سچ;
  اگر (unlikely (!برآمد.resize_dirty  (n)))
    واپس جھوٹ;

  غیرعلامتی شے_سائز = hb_static_سائز (T);
  اگر (k < 0)
    k = n - (-k) % n;
  ورنہ
    k %= n;

  hb_memcpy ((خالی *) برآمد.arrayZ, (ثابت خالی *) (org_array.arrayZ + n - k), k * شے_سائز);
  hb_memcpy ((خالی *) (برآمد.arrayZ + k), (ثابت خالی *) org_array.arrayZ, (n - k) * شے_سائز);
  واپس سچ;
}

ساکن منطقی rotate_سیٹ (ثابت hb_iup_سیٹ_t& org_سیٹ,
                        صحیح k,
                        غیرعلامتی n,
                        hb_iup_سیٹ_t& برآمد)
{
  اگر (!n) واپس جھوٹ;
  k %= n;
  اگر (k < 0)
    k = n + k;

  اگر (k == 0)
  {
    برآمد = org_سیٹ;
  }
  ورنہ
  {
    برائے (غیرعلامتی v : org_سیٹ)
      برآمد.جوڑو ((v + k) % n);
  }
  واپس سچ;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ساکن منطقی _iup_segment (ثابت hb_array_t<ثابت contour_نقطہ_t> contour_points,
                          ثابت hb_array_t<ثابت صحیح> x_deltas,
                          ثابت hb_array_t<ثابت صحیح> y_deltas,
                          ثابت contour_نقطہ_t& p1, ثابت contour_نقطہ_t& p2,
                          صحیح p1_dx, صحیح p2_dx,
                          صحیح p1_dy, صحیح p2_dy,
			  دوہرا tolerance_sq,
                          hb_سمتیہ_t<دوہرا>& interp_x_deltas, /*برآمد*/
                          hb_سمتیہ_t<دوہرا>& interp_y_deltas /*برآمد*/)
{
  غیرعلامتی n = contour_points.لمبائی;
  اگر (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    واپس جھوٹ;

  برائے (غیرعلامتی j = 0; j < 2; j++)
  {
    اعشاری contour_نقطہ_t::* xp;
    دوہرا x1, x2, d1, d2;
    ثابت صحیح *اندر;
    دوہرا *برآمد;
    اگر (j == 0)
    {
      xp = &contour_نقطہ_t::x;
      x1 = static_cast<دوہرا> (p1.x);
      x2 = static_cast<دوہرا> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      اندر = x_deltas.arrayZ;
      برآمد = interp_x_deltas.arrayZ;
    }
    ورنہ
    {
      xp = &contour_نقطہ_t::y;
      x1 = static_cast<دوہرا> (p1.y);
      x2 = static_cast<دوہرا> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      اندر = y_deltas.arrayZ;
      برآمد = interp_y_deltas.arrayZ;
    }

    اگر (x1 == x2)
    {
      اگر (d1 == d2)
      {
        برائے (غیرعلامتی i = 0; i < n; i++)
          برآمد[i] = d1;
      }
      ورنہ
      {
        برائے (غیرعلامتی i = 0; i < n; i++)
          برآمد[i] = 0.0;
      }
      جاری;
    }

    اگر (x1 > x2)
    {
      hb_تبادلہ (x1, x2);
      hb_تبادلہ (d1, d2);
    }

    دوہرا scale = (d2 - d1) / (x2 - x1);
    برائے (غیرعلامتی i = 0; i < n; i++)
    {
      دوہرا x = (دوہرا) (contour_points.arrayZ[i].*xp);
      دوہرا d;
      اگر (x <= x1)
        d = d1;
      ورنہ اگر (x >= x2)
        d = d2;
      ورنہ
        d = d1 + (x - x1) * scale;

      برآمد[i] = d;
      دوہرا err = d - اندر[i];
      اگر (err * err > tolerance_sq)
	واپس جھوٹ;
    }
  }
  واپس سچ;
}

ساکن منطقی can_iup_اندر_between (ثابت hb_array_t<ثابت contour_نقطہ_t> contour_points,
                                 ثابت hb_array_t<ثابت صحیح> x_deltas,
                                 ثابت hb_array_t<ثابت صحیح> y_deltas,
                                 ثابت contour_نقطہ_t& p1, ثابت contour_نقطہ_t& p2,
                                 صحیح p1_dx, صحیح p2_dx,
                                 صحیح p1_dy, صحیح p2_dy,
                                 دوہرا tolerance_sq,
				 hb_سمتیہ_t<دوہرا> &interp_x_deltas, /* scratch */
				 hb_سمتیہ_t<دوہرا> &interp_y_deltas /* scratch */)
{
  اگر (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    واپس جھوٹ;

  غیرعلامتی عدد = contour_points.لمبائی;

  برائے (غیرعلامتی i = 0; i < عدد; i++)
  {
    دوہرا dx = static_cast<دوہرا> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    دوہرا dy = static_cast<دوہرا> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    اگر (dx * dx + dy * dy > tolerance_sq)
      واپس جھوٹ;
  }
  واپس سچ;
}

ساکن منطقی _iup_contour_optimize_dp (ثابت contour_نقطہ_سمتیہ_t& contour_points,
                                      ثابت hb_سمتیہ_t<صحیح>& x_deltas,
                                      ثابت hb_سمتیہ_t<صحیح>& y_deltas,
                                      ثابت hb_iup_سیٹ_t& forced_سیٹ,
                                      دوہرا tolerance_sq,
                                      غیرعلامتی lookback,
                                      hb_سمتیہ_t<غیرعلامتی>& costs, /*برآمد*/
                                      hb_سمتیہ_t<صحیح>& chain, /*برآمد*/
				      hb_سمتیہ_t<دوہرا> &interp_x_deltas_scratch,
				      hb_سمتیہ_t<دوہرا> &interp_y_deltas_scratch)
{
  غیرعلامتی n = contour_points.لمبائی;
  اگر (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    واپس جھوٹ;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  برائے (غیرعلامتی i = 0; i < n; i++)
  {
    غیرعلامتی بہترین_لاگت = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = بہترین_لاگت;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    اگر (i > 0 && forced_سیٹ.has (i - 1))
      جاری;

    صحیح lookback_اشاریہ = hb_max ((صحیح) i - (صحیح) lookback + 1, -1);
    برائے (صحیح j = i - 2; j >= lookback_اشاریہ; j--)
    {
      غیرعلامتی لاگت = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      غیرعلامتی عدد_points = i - j - 1;
      غیرعلامتی p1 = (j == -1 ? n - 1 : j);
      اگر (لاگت < بہترین_لاگت &&
          can_iup_اندر_between (contour_points.as_array ().sub_array (j + 1, عدد_points),
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

      اگر (j > 0 && forced_سیٹ.has (j))
        توڑو;
    }
  }
  واپس سچ;
}

ساکن منطقی _iup_contour_optimize (ثابت hb_array_t<ثابت contour_نقطہ_t> contour_points,
                                   ثابت hb_array_t<ثابت صحیح> x_deltas,
                                   ثابت hb_array_t<ثابت صحیح> y_deltas,
                                   hb_array_t<منطقی> opt_indices, /*برآمد*/
                                   دوہرا tolerance,
				   iup_scratch_t &scratch)
{
  غیرعلامتی n = contour_points.لمبائی;
  اگر (opt_indices.لمبائی != n ||
      x_deltas.لمبائی != n ||
      y_deltas.لمبائی != n)
    واپس جھوٹ;

  اگر (unlikely (n > hb_iup_سیٹ_t::صفحہ_بٹ))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    برائے (غیرعلامتی i = 0; i < n; i++)
      opt_indices.arrayZ[i] = سچ;
    واپس سچ;
  }

  منطقی all_within_tolerance = سچ;
  دوہرا tolerance_sq = tolerance * tolerance;
  برائے (غیرعلامتی i = 0; i < n; i++)
  {
    صحیح dx = x_deltas.arrayZ[i];
    صحیح dy = y_deltas.arrayZ[i];
    اگر ((دوہرا) dx * dx + (دوہرا) dy * dy > tolerance_sq)
    {
      all_within_tolerance = جھوٹ;
      توڑو;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  اگر (all_within_tolerance)
    واپس سچ;

  /* If there's exactly one point, return it */
  اگر (n == 1)
  {
    opt_indices.arrayZ[0] = سچ;
    واپس سچ;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  منطقی all_deltas_are_equal = سچ;
  برائے (غیرعلامتی i = 1; i < n; i++)
    اگر (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = جھوٹ;
      توڑو;
    }

  اگر (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = سچ;
    واپس سچ;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_سیٹ_t forced_سیٹ;
  iup_contour_bound_forced_سیٹ (contour_points, x_deltas, y_deltas, forced_سیٹ, tolerance);

  hb_سمتیہ_t<غیرعلامتی> &costs = scratch.costs.reset ();
  hb_سمتیہ_t<صحیح> &chain = scratch.chain.reset ();

  اگر (!forced_سیٹ.is_empty ())
  {
    صحیح k = n - 1 - forced_سیٹ.get_max ();
    اگر (k < 0)
      واپس جھوٹ;

    hb_سمتیہ_t<صحیح> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_سمتیہ_t<صحیح> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_نقطہ_سمتیہ_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_سیٹ_t rot_forced_سیٹ;
    اگر (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_سیٹ (forced_سیٹ, k, n, rot_forced_سیٹ))
      واپس جھوٹ;

    اگر (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_سیٹ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      واپس جھوٹ;

    hb_iup_سیٹ_t solution;
    صحیح اشاریہ_2 = n - 1;
    جب (اشاریہ_2 != -1)
    {
      solution.جوڑو (اشاریہ_2);
      اشاریہ_2 = chain.arrayZ[اشاریہ_2];
    }

    اگر (solution.is_empty () ||
        forced_سیٹ.get_population () > solution.get_population ())
      واپس جھوٹ;

    برائے (غیرعلامتی i : solution)
      opt_indices.arrayZ[i] = سچ;

    hb_سمتیہ_t<منطقی> &rot_indices = scratch.rot_indices.reset ();
    ثابت hb_array_t<ثابت منطقی> opt_indices_array (opt_indices.arrayZ, opt_indices.لمبائی);
    اگر (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      واپس جھوٹ;

    برائے (غیرعلامتی i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  ورنہ
  {
    hb_سمتیہ_t<صحیح> repeat_x_deltas, repeat_y_deltas;
    contour_نقطہ_سمتیہ_t repeat_points;

    اگر (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      واپس جھوٹ;

    غیرعلامتی contour_نقطہ_سائز = hb_static_سائز (contour_نقطہ_t);
    برائے (غیرعلامتی i = 0; i < n; i++)
    {
      hb_memcpy ((خالی *) repeat_x_deltas.arrayZ, (ثابت خالی *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((خالی *) (repeat_x_deltas.arrayZ + n), (ثابت خالی *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((خالی *) repeat_y_deltas.arrayZ, (ثابت خالی *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((خالی *) (repeat_y_deltas.arrayZ + n), (ثابت خالی *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((خالی *) repeat_points.arrayZ, (ثابت خالی *) contour_points.arrayZ, n * contour_نقطہ_سائز);
      hb_memcpy ((خالی *) (repeat_points.arrayZ + n), (ثابت خالی *) contour_points.arrayZ, n * contour_نقطہ_سائز);
    }

    اگر (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_سیٹ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      واپس جھوٹ;

    غیرعلامتی بہترین_لاگت = n + 1;
    صحیح len = costs.لمبائی;
    hb_iup_سیٹ_t بہترین_sol;
    برائے (صحیح آغاز = n - 1; آغاز < len; آغاز++)
    {
      hb_iup_سیٹ_t solution;
      صحیح i = آغاز;
      صحیح lookback = آغاز - (صحیح) n;
      جب (i > lookback)
      {
        solution.جوڑو (i % n);
        i = chain.arrayZ[i];
      }
      اگر (i == lookback)
      {
        غیرعلامتی لاگت_i = i < 0 ? 0 : costs.arrayZ[i];
        غیرعلامتی لاگت = costs.arrayZ[آغاز] - لاگت_i;
        اگر (لاگت <= بہترین_لاگت)
        {
          بہترین_sol = solution;
          بہترین_لاگت = لاگت;
        }
      }
    }

    برائے (غیرعلامتی i = 0; i < n; i++)
      اگر (بہترین_sol.has (i))
        opt_indices.arrayZ[i] = سچ;
  }
  واپس سچ;
}

منطقی iup_delta_optimize (ثابت contour_نقطہ_سمتیہ_t& contour_points,
                         ثابت hb_سمتیہ_t<صحیح>& x_deltas,
                         ثابت hb_سمتیہ_t<صحیح>& y_deltas,
                         hb_سمتیہ_t<منطقی>& opt_indices, /*برآمد*/
			 iup_scratch_t &scratch,
                         دوہرا tolerance)
{
  اگر (!opt_indices.resize (contour_points.لمبائی))
      واپس جھوٹ;

  hb_سمتیہ_t<غیرعلامتی> &انتہا_points = scratch.انتہا_points.reset ();

  غیرعلامتی گنتی = contour_points.لمبائی;
  اگر (unlikely (!انتہا_points.alloc (گنتی)))
    واپس جھوٹ;

  اگر (گنتی < 4)
  {
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      انتہا_points.push (i);
  }
  ورنہ
  {
    برائے (غیرعلامتی i = 0; i < گنتی - 4; i++)
      اگر (contour_points.arrayZ[i].is_انتہا_نقطہ)
        انتہا_points.push (i);

    /* phantom points */
    برائے (غیرعلامتی i = گنتی - 4; i < گنتی; i++)
      انتہا_points.push (i);
  }

  اگر (انتہا_points.اندر_error ()) واپس جھوٹ;

  غیرعلامتی آغاز = 0;
  برائے (غیرعلامتی انتہا : انتہا_points)
  {
    غیرعلامتی len = انتہا - آغاز + 1;
    اگر (!_iup_contour_optimize (contour_points.as_array ().sub_array (آغاز, len),
                                x_deltas.as_array ().sub_array (آغاز, len),
                                y_deltas.as_array ().sub_array (آغاز, len),
                                opt_indices.as_array ().sub_array (آغاز, len),
                                tolerance,
				scratch))
      واپس جھوٹ;
    آغاز = انتہا + 1;
  }
  واپس سچ;
}
