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

استفاده hb_iup_مجموعه_t = hb_بیت_صفحه_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ساكن بیعلامت MAX_LOOKBACK = 8;

ساكن فراغ iup_contour_bound_forced_مجموعه (ثابت hb_array_t<ثابت contour_نقطه_t> contour_points,
                                           ثابت hb_array_t<ثابت صحيح> x_deltas,
                                           ثابت hb_array_t<ثابت صحيح> y_deltas,
                                           hb_iup_مجموعه_t& forced_مجموعه, /*بیرون*/
                                           مزدوج tolerance = 0.0)
{
  بیعلامت len = contour_points.درازی;
  بیعلامت بعدی_i = 0;
  لكل (صحيح i = len - 1; i >= 0; i--)
  {
    بیعلامت آخرین_i = (len + i - 1) % len;
    لكل (بیعلامت j = 0; j < 2; j++)
    {
      مزدوج cj, lcj, ncj;
      مزدوج dj, ldj, ndj;
      إذا (j == 0)
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[آخرین_i].x);
        ldj = x_deltas.arrayZ[آخرین_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[بعدی_i].x);
        ndj = x_deltas.arrayZ[بعدی_i];
      }
      وإلا
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[آخرین_i].y);
        ldj = y_deltas.arrayZ[آخرین_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[بعدی_i].y);
        ndj = y_deltas.arrayZ[بعدی_i];
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
          force = درست;
      }
      وإلا إذا (c1 <= cj && cj <= c2)
      {
        إذا (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = درست;
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
                force = درست;
          }
          وإلا
          {
            إذا (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = درست;
          }
        }
      }

      إذا (force)
      {
        forced_مجموعه.جمع_کن (i);
        اكسر;
      }
    }
    بعدی_i = i;
  }
}

قالب <نام_نوع T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ساكن منطقي rotate_array (ثابت hb_array_t<ثابت T>& org_array,
                          صحيح k,
                          hb_وکتور_t<T>& بیرون)
{
  بیعلامت n = org_array.درازی;
  إذا (!n) أرجع درست;
  إذا (unlikely (!بیرون.resize_dirty  (n)))
    أرجع خطأ;

  بیعلامت قلم_اندازه = hb_static_اندازه (T);
  إذا (k < 0)
    k = n - (-k) % n;
  وإلا
    k %= n;

  hb_memcpy ((فراغ *) بیرون.arrayZ, (ثابت فراغ *) (org_array.arrayZ + n - k), k * قلم_اندازه);
  hb_memcpy ((فراغ *) (بیرون.arrayZ + k), (ثابت فراغ *) org_array.arrayZ, (n - k) * قلم_اندازه);
  أرجع درست;
}

ساكن منطقي rotate_مجموعه (ثابت hb_iup_مجموعه_t& org_مجموعه,
                        صحيح k,
                        بیعلامت n,
                        hb_iup_مجموعه_t& بیرون)
{
  إذا (!n) أرجع خطأ;
  k %= n;
  إذا (k < 0)
    k = n + k;

  إذا (k == 0)
  {
    بیرون = org_مجموعه;
  }
  وإلا
  {
    لكل (بیعلامت v : org_مجموعه)
      بیرون.جمع_کن ((v + k) % n);
  }
  أرجع درست;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ساكن منطقي _iup_segment (ثابت hb_array_t<ثابت contour_نقطه_t> contour_points,
                          ثابت hb_array_t<ثابت صحيح> x_deltas,
                          ثابت hb_array_t<ثابت صحيح> y_deltas,
                          ثابت contour_نقطه_t& p1, ثابت contour_نقطه_t& p2,
                          صحيح p1_dx, صحيح p2_dx,
                          صحيح p1_dy, صحيح p2_dy,
			  مزدوج tolerance_sq,
                          hb_وکتور_t<مزدوج>& interp_x_deltas, /*بیرون*/
                          hb_وکتور_t<مزدوج>& interp_y_deltas /*بیرون*/)
{
  بیعلامت n = contour_points.درازی;
  إذا (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    أرجع خطأ;

  لكل (بیعلامت j = 0; j < 2; j++)
  {
    عائم contour_نقطه_t::* xp;
    مزدوج x1, x2, d1, d2;
    ثابت صحيح *درون;
    مزدوج *بیرون;
    إذا (j == 0)
    {
      xp = &contour_نقطه_t::x;
      x1 = static_cast<مزدوج> (p1.x);
      x2 = static_cast<مزدوج> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      درون = x_deltas.arrayZ;
      بیرون = interp_x_deltas.arrayZ;
    }
    وإلا
    {
      xp = &contour_نقطه_t::y;
      x1 = static_cast<مزدوج> (p1.y);
      x2 = static_cast<مزدوج> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      درون = y_deltas.arrayZ;
      بیرون = interp_y_deltas.arrayZ;
    }

    إذا (x1 == x2)
    {
      إذا (d1 == d2)
      {
        لكل (بیعلامت i = 0; i < n; i++)
          بیرون[i] = d1;
      }
      وإلا
      {
        لكل (بیعلامت i = 0; i < n; i++)
          بیرون[i] = 0.0;
      }
      استمر;
    }

    إذا (x1 > x2)
    {
      hb_جابجا (x1, x2);
      hb_جابجا (d1, d2);
    }

    مزدوج scale = (d2 - d1) / (x2 - x1);
    لكل (بیعلامت i = 0; i < n; i++)
    {
      مزدوج x = (مزدوج) (contour_points.arrayZ[i].*xp);
      مزدوج d;
      إذا (x <= x1)
        d = d1;
      وإلا إذا (x >= x2)
        d = d2;
      وإلا
        d = d1 + (x - x1) * scale;

      بیرون[i] = d;
      مزدوج err = d - درون[i];
      إذا (err * err > tolerance_sq)
	أرجع خطأ;
    }
  }
  أرجع درست;
}

ساكن منطقي can_iup_درون_between (ثابت hb_array_t<ثابت contour_نقطه_t> contour_points,
                                 ثابت hb_array_t<ثابت صحيح> x_deltas,
                                 ثابت hb_array_t<ثابت صحيح> y_deltas,
                                 ثابت contour_نقطه_t& p1, ثابت contour_نقطه_t& p2,
                                 صحيح p1_dx, صحيح p2_dx,
                                 صحيح p1_dy, صحيح p2_dy,
                                 مزدوج tolerance_sq,
				 hb_وکتور_t<مزدوج> &interp_x_deltas, /* scratch */
				 hb_وکتور_t<مزدوج> &interp_y_deltas /* scratch */)
{
  إذا (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    أرجع خطأ;

  بیعلامت رقم = contour_points.درازی;

  لكل (بیعلامت i = 0; i < رقم; i++)
  {
    مزدوج dx = static_cast<مزدوج> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    مزدوج dy = static_cast<مزدوج> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    إذا (dx * dx + dy * dy > tolerance_sq)
      أرجع خطأ;
  }
  أرجع درست;
}

ساكن منطقي _iup_contour_optimize_dp (ثابت contour_نقطه_وکتور_t& contour_points,
                                      ثابت hb_وکتور_t<صحيح>& x_deltas,
                                      ثابت hb_وکتور_t<صحيح>& y_deltas,
                                      ثابت hb_iup_مجموعه_t& forced_مجموعه,
                                      مزدوج tolerance_sq,
                                      بیعلامت lookback,
                                      hb_وکتور_t<بیعلامت>& costs, /*بیرون*/
                                      hb_وکتور_t<صحيح>& chain, /*بیرون*/
				      hb_وکتور_t<مزدوج> &interp_x_deltas_scratch,
				      hb_وکتور_t<مزدوج> &interp_y_deltas_scratch)
{
  بیعلامت n = contour_points.درازی;
  إذا (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    أرجع خطأ;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  لكل (بیعلامت i = 0; i < n; i++)
  {
    بیعلامت بهترین_هزینه = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = بهترین_هزینه;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    إذا (i > 0 && forced_مجموعه.has (i - 1))
      استمر;

    صحيح lookback_شاخص = hb_max ((صحيح) i - (صحيح) lookback + 1, -1);
    لكل (صحيح j = i - 2; j >= lookback_شاخص; j--)
    {
      بیعلامت هزینه = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      بیعلامت رقم_points = i - j - 1;
      بیعلامت p1 = (j == -1 ? n - 1 : j);
      إذا (هزینه < بهترین_هزینه &&
          can_iup_درون_between (contour_points.as_array ().sub_array (j + 1, رقم_points),
                               x_deltas.as_array ().sub_array (j + 1, رقم_points),
                               y_deltas.as_array ().sub_array (j + 1, رقم_points),
                               contour_points.arrayZ[p1], contour_points.arrayZ[i],
                               x_deltas.arrayZ[p1], x_deltas.arrayZ[i],
                               y_deltas.arrayZ[p1], y_deltas.arrayZ[i],
                               tolerance_sq,
			       interp_x_deltas_scratch, interp_y_deltas_scratch))
      {
        بهترین_هزینه = هزینه;
        costs.arrayZ[i] = بهترین_هزینه;
        chain.arrayZ[i] = j;
      }

      إذا (j > 0 && forced_مجموعه.has (j))
        اكسر;
    }
  }
  أرجع درست;
}

ساكن منطقي _iup_contour_optimize (ثابت hb_array_t<ثابت contour_نقطه_t> contour_points,
                                   ثابت hb_array_t<ثابت صحيح> x_deltas,
                                   ثابت hb_array_t<ثابت صحيح> y_deltas,
                                   hb_array_t<منطقي> opt_indices, /*بیرون*/
                                   مزدوج tolerance,
				   iup_scratch_t &scratch)
{
  بیعلامت n = contour_points.درازی;
  إذا (opt_indices.درازی != n ||
      x_deltas.درازی != n ||
      y_deltas.درازی != n)
    أرجع خطأ;

  إذا (unlikely (n > hb_iup_مجموعه_t::صفحه_بیتها))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    لكل (بیعلامت i = 0; i < n; i++)
      opt_indices.arrayZ[i] = درست;
    أرجع درست;
  }

  منطقي all_within_tolerance = درست;
  مزدوج tolerance_sq = tolerance * tolerance;
  لكل (بیعلامت i = 0; i < n; i++)
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
    أرجع درست;

  /* If there's exactly one point, return it */
  إذا (n == 1)
  {
    opt_indices.arrayZ[0] = درست;
    أرجع درست;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  منطقي all_deltas_are_equal = درست;
  لكل (بیعلامت i = 1; i < n; i++)
    إذا (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = خطأ;
      اكسر;
    }

  إذا (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = درست;
    أرجع درست;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_مجموعه_t forced_مجموعه;
  iup_contour_bound_forced_مجموعه (contour_points, x_deltas, y_deltas, forced_مجموعه, tolerance);

  hb_وکتور_t<بیعلامت> &costs = scratch.costs.reset ();
  hb_وکتور_t<صحيح> &chain = scratch.chain.reset ();

  إذا (!forced_مجموعه.is_empty ())
  {
    صحيح k = n - 1 - forced_مجموعه.get_max ();
    إذا (k < 0)
      أرجع خطأ;

    hb_وکتور_t<صحيح> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_وکتور_t<صحيح> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_نقطه_وکتور_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_مجموعه_t rot_forced_مجموعه;
    إذا (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_مجموعه (forced_مجموعه, k, n, rot_forced_مجموعه))
      أرجع خطأ;

    إذا (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_مجموعه, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      أرجع خطأ;

    hb_iup_مجموعه_t solution;
    صحيح شاخص_2 = n - 1;
    طالما (شاخص_2 != -1)
    {
      solution.جمع_کن (شاخص_2);
      شاخص_2 = chain.arrayZ[شاخص_2];
    }

    إذا (solution.is_empty () ||
        forced_مجموعه.get_population () > solution.get_population ())
      أرجع خطأ;

    لكل (بیعلامت i : solution)
      opt_indices.arrayZ[i] = درست;

    hb_وکتور_t<منطقي> &rot_indices = scratch.rot_indices.reset ();
    ثابت hb_array_t<ثابت منطقي> opt_indices_array (opt_indices.arrayZ, opt_indices.درازی);
    إذا (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      أرجع خطأ;

    لكل (بیعلامت i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  وإلا
  {
    hb_وکتور_t<صحيح> repeat_x_deltas, repeat_y_deltas;
    contour_نقطه_وکتور_t repeat_points;

    إذا (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      أرجع خطأ;

    بیعلامت contour_نقطه_اندازه = hb_static_اندازه (contour_نقطه_t);
    لكل (بیعلامت i = 0; i < n; i++)
    {
      hb_memcpy ((فراغ *) repeat_x_deltas.arrayZ, (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_x_deltas.arrayZ + n), (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_y_deltas.arrayZ, (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_y_deltas.arrayZ + n), (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_points.arrayZ, (ثابت فراغ *) contour_points.arrayZ, n * contour_نقطه_اندازه);
      hb_memcpy ((فراغ *) (repeat_points.arrayZ + n), (ثابت فراغ *) contour_points.arrayZ, n * contour_نقطه_اندازه);
    }

    إذا (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_مجموعه, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      أرجع خطأ;

    بیعلامت بهترین_هزینه = n + 1;
    صحيح len = costs.درازی;
    hb_iup_مجموعه_t بهترین_sol;
    لكل (صحيح شروع = n - 1; شروع < len; شروع++)
    {
      hb_iup_مجموعه_t solution;
      صحيح i = شروع;
      صحيح lookback = شروع - (صحيح) n;
      طالما (i > lookback)
      {
        solution.جمع_کن (i % n);
        i = chain.arrayZ[i];
      }
      إذا (i == lookback)
      {
        بیعلامت هزینه_i = i < 0 ? 0 : costs.arrayZ[i];
        بیعلامت هزینه = costs.arrayZ[شروع] - هزینه_i;
        إذا (هزینه <= بهترین_هزینه)
        {
          بهترین_sol = solution;
          بهترین_هزینه = هزینه;
        }
      }
    }

    لكل (بیعلامت i = 0; i < n; i++)
      إذا (بهترین_sol.has (i))
        opt_indices.arrayZ[i] = درست;
  }
  أرجع درست;
}

منطقي iup_delta_optimize (ثابت contour_نقطه_وکتور_t& contour_points,
                         ثابت hb_وکتور_t<صحيح>& x_deltas,
                         ثابت hb_وکتور_t<صحيح>& y_deltas,
                         hb_وکتور_t<منطقي>& opt_indices, /*بیرون*/
			 iup_scratch_t &scratch,
                         مزدوج tolerance)
{
  إذا (!opt_indices.resize (contour_points.درازی))
      أرجع خطأ;

  hb_وکتور_t<بیعلامت> &انتها_points = scratch.انتها_points.reset ();

  بیعلامت شمار = contour_points.درازی;
  إذا (unlikely (!انتها_points.alloc (شمار)))
    أرجع خطأ;

  إذا (شمار < 4)
  {
    لكل (بیعلامت i = 0; i < شمار; i++)
      انتها_points.push (i);
  }
  وإلا
  {
    لكل (بیعلامت i = 0; i < شمار - 4; i++)
      إذا (contour_points.arrayZ[i].is_انتها_نقطه)
        انتها_points.push (i);

    /* phantom points */
    لكل (بیعلامت i = شمار - 4; i < شمار; i++)
      انتها_points.push (i);
  }

  إذا (انتها_points.درون_error ()) أرجع خطأ;

  بیعلامت شروع = 0;
  لكل (بیعلامت انتها : انتها_points)
  {
    بیعلامت len = انتها - شروع + 1;
    إذا (!_iup_contour_optimize (contour_points.as_array ().sub_array (شروع, len),
                                x_deltas.as_array ().sub_array (شروع, len),
                                y_deltas.as_array ().sub_array (شروع, len),
                                opt_indices.as_array ().sub_array (شروع, len),
                                tolerance,
				scratch))
      أرجع خطأ;
    شروع = انتها + 1;
  }
  أرجع درست;
}
