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

#అన్తర్భావ "hb-subset-instancer-iup.hh"

#అన్తర్భావ "hb-bit-page.hh"

ఉపయోగించి hb_iup_సమితి_t = hb_బిట్టు_పుట_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr స్థిర గుర్తులేని MAX_LOOKBACK = 8;

స్థిర శూన్య iup_contour_bound_forced_సమితి (స్థిరరాశి hb_array_t<స్థిరరాశి contour_బిందువు_t> contour_points,
                                           స్థిరరాశి hb_array_t<స్థిరరాశి పూర్ణాంకం> x_deltas,
                                           స్థిరరాశి hb_array_t<స్థిరరాశి పూర్ణాంకం> y_deltas,
                                           hb_iup_సమితి_t& forced_సమితి, /*బహిర్గతం*/
                                           ద్విగుణ tolerance = 0.0)
{
  గుర్తులేని len = contour_points.నిడివి;
  గుర్తులేని తదుపరి_i = 0;
  కొరకు (పూర్ణాంకం i = len - 1; i >= 0; i--)
  {
    గుర్తులేని చివరి_i = (len + i - 1) % len;
    కొరకు (గుర్తులేని j = 0; j < 2; j++)
    {
      ద్విగుణ cj, lcj, ncj;
      ద్విగుణ dj, ldj, ndj;
      ఒకవేళ (j == 0)
      {
        cj = static_cast<ద్విగుణ> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<ద్విగుణ> (contour_points.arrayZ[చివరి_i].x);
        ldj = x_deltas.arrayZ[చివరి_i];
        ncj = static_cast<ద్విగుణ> (contour_points.arrayZ[తదుపరి_i].x);
        ndj = x_deltas.arrayZ[తదుపరి_i];
      }
      లేకపోతే
      {
        cj = static_cast<ద్విగుణ> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<ద్విగుణ> (contour_points.arrayZ[చివరి_i].y);
        ldj = y_deltas.arrayZ[చివరి_i];
        ncj = static_cast<ద్విగుణ> (contour_points.arrayZ[తదుపరి_i].y);
        ndj = y_deltas.arrayZ[తదుపరి_i];
      }

      ద్విగుణ c1, c2;
      ద్విగుణ d1, d2;
      ఒకవేళ (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      లేకపోతే
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      తార్కిక force = అబద్ధం;
      ఒకవేళ (c1 == c2)
      {
        ఒకవేళ (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = నిజం;
      }
      లేకపోతే ఒకవేళ (c1 <= cj && cj <= c2)
      {
        ఒకవేళ (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = నిజం;
      }
      లేకపోతే
      {
        ఒకవేళ (d1 != d2)
        {
          ఒకవేళ (cj < c1)
          {
            ఒకవేళ (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = నిజం;
          }
          లేకపోతే
          {
            ఒకవేళ (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = నిజం;
          }
        }
      }

      ఒకవేళ (force)
      {
        forced_సమితి.కూడు (i);
        ఆపు;
      }
    }
    తదుపరి_i = i;
  }
}

మూస <రకం_పేరు T,
          hb_enable_if (hb_is_trivially_copyable (T))>
స్థిర తార్కిక rotate_array (స్థిరరాశి hb_array_t<స్థిరరాశి T>& org_array,
                          పూర్ణాంకం k,
                          hb_సదిశ_t<T>& బహిర్గతం)
{
  గుర్తులేని n = org_array.నిడివి;
  ఒకవేళ (!n) తిరిగి నిజం;
  ఒకవేళ (unlikely (!బహిర్గతం.resize_dirty  (n)))
    తిరిగి అబద్ధం;

  గుర్తులేని అంశం_కొలత = hb_static_కొలత (T);
  ఒకవేళ (k < 0)
    k = n - (-k) % n;
  లేకపోతే
    k %= n;

  hb_memcpy ((శూన్య *) బహిర్గతం.arrayZ, (స్థిరరాశి శూన్య *) (org_array.arrayZ + n - k), k * అంశం_కొలత);
  hb_memcpy ((శూన్య *) (బహిర్గతం.arrayZ + k), (స్థిరరాశి శూన్య *) org_array.arrayZ, (n - k) * అంశం_కొలత);
  తిరిగి నిజం;
}

స్థిర తార్కిక rotate_సమితి (స్థిరరాశి hb_iup_సమితి_t& org_సమితి,
                        పూర్ణాంకం k,
                        గుర్తులేని n,
                        hb_iup_సమితి_t& బహిర్గతం)
{
  ఒకవేళ (!n) తిరిగి అబద్ధం;
  k %= n;
  ఒకవేళ (k < 0)
    k = n + k;

  ఒకవేళ (k == 0)
  {
    బహిర్గతం = org_సమితి;
  }
  లేకపోతే
  {
    కొరకు (గుర్తులేని v : org_సమితి)
      బహిర్గతం.కూడు ((v + k) % n);
  }
  తిరిగి నిజం;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
స్థిర తార్కిక _iup_segment (స్థిరరాశి hb_array_t<స్థిరరాశి contour_బిందువు_t> contour_points,
                          స్థిరరాశి hb_array_t<స్థిరరాశి పూర్ణాంకం> x_deltas,
                          స్థిరరాశి hb_array_t<స్థిరరాశి పూర్ణాంకం> y_deltas,
                          స్థిరరాశి contour_బిందువు_t& p1, స్థిరరాశి contour_బిందువు_t& p2,
                          పూర్ణాంకం p1_dx, పూర్ణాంకం p2_dx,
                          పూర్ణాంకం p1_dy, పూర్ణాంకం p2_dy,
			  ద్విగుణ tolerance_sq,
                          hb_సదిశ_t<ద్విగుణ>& interp_x_deltas, /*బహిర్గతం*/
                          hb_సదిశ_t<ద్విగుణ>& interp_y_deltas /*బహిర్గతం*/)
{
  గుర్తులేని n = contour_points.నిడివి;
  ఒకవేళ (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    తిరిగి అబద్ధం;

  కొరకు (గుర్తులేని j = 0; j < 2; j++)
  {
    దశాంశం contour_బిందువు_t::* xp;
    ద్విగుణ x1, x2, d1, d2;
    స్థిరరాశి పూర్ణాంకం *లోపల;
    ద్విగుణ *బహిర్గతం;
    ఒకవేళ (j == 0)
    {
      xp = &contour_బిందువు_t::x;
      x1 = static_cast<ద్విగుణ> (p1.x);
      x2 = static_cast<ద్విగుణ> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      లోపల = x_deltas.arrayZ;
      బహిర్గతం = interp_x_deltas.arrayZ;
    }
    లేకపోతే
    {
      xp = &contour_బిందువు_t::y;
      x1 = static_cast<ద్విగుణ> (p1.y);
      x2 = static_cast<ద్విగుణ> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      లోపల = y_deltas.arrayZ;
      బహిర్గతం = interp_y_deltas.arrayZ;
    }

    ఒకవేళ (x1 == x2)
    {
      ఒకవేళ (d1 == d2)
      {
        కొరకు (గుర్తులేని i = 0; i < n; i++)
          బహిర్గతం[i] = d1;
      }
      లేకపోతే
      {
        కొరకు (గుర్తులేని i = 0; i < n; i++)
          బహిర్గతం[i] = 0.0;
      }
      కొనసాగు;
    }

    ఒకవేళ (x1 > x2)
    {
      hb_మార్పిడి (x1, x2);
      hb_మార్పిడి (d1, d2);
    }

    ద్విగుణ scale = (d2 - d1) / (x2 - x1);
    కొరకు (గుర్తులేని i = 0; i < n; i++)
    {
      ద్విగుణ x = (ద్విగుణ) (contour_points.arrayZ[i].*xp);
      ద్విగుణ d;
      ఒకవేళ (x <= x1)
        d = d1;
      లేకపోతే ఒకవేళ (x >= x2)
        d = d2;
      లేకపోతే
        d = d1 + (x - x1) * scale;

      బహిర్గతం[i] = d;
      ద్విగుణ err = d - లోపల[i];
      ఒకవేళ (err * err > tolerance_sq)
	తిరిగి అబద్ధం;
    }
  }
  తిరిగి నిజం;
}

స్థిర తార్కిక can_iup_లోపల_between (స్థిరరాశి hb_array_t<స్థిరరాశి contour_బిందువు_t> contour_points,
                                 స్థిరరాశి hb_array_t<స్థిరరాశి పూర్ణాంకం> x_deltas,
                                 స్థిరరాశి hb_array_t<స్థిరరాశి పూర్ణాంకం> y_deltas,
                                 స్థిరరాశి contour_బిందువు_t& p1, స్థిరరాశి contour_బిందువు_t& p2,
                                 పూర్ణాంకం p1_dx, పూర్ణాంకం p2_dx,
                                 పూర్ణాంకం p1_dy, పూర్ణాంకం p2_dy,
                                 ద్విగుణ tolerance_sq,
				 hb_సదిశ_t<ద్విగుణ> &interp_x_deltas, /* scratch */
				 hb_సదిశ_t<ద్విగుణ> &interp_y_deltas /* scratch */)
{
  ఒకవేళ (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    తిరిగి అబద్ధం;

  గుర్తులేని అంకె = contour_points.నిడివి;

  కొరకు (గుర్తులేని i = 0; i < అంకె; i++)
  {
    ద్విగుణ dx = static_cast<ద్విగుణ> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    ద్విగుణ dy = static_cast<ద్విగుణ> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    ఒకవేళ (dx * dx + dy * dy > tolerance_sq)
      తిరిగి అబద్ధం;
  }
  తిరిగి నిజం;
}

స్థిర తార్కిక _iup_contour_optimize_dp (స్థిరరాశి contour_బిందువు_సదిశ_t& contour_points,
                                      స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& x_deltas,
                                      స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& y_deltas,
                                      స్థిరరాశి hb_iup_సమితి_t& forced_సమితి,
                                      ద్విగుణ tolerance_sq,
                                      గుర్తులేని lookback,
                                      hb_సదిశ_t<గుర్తులేని>& costs, /*బహిర్గతం*/
                                      hb_సదిశ_t<పూర్ణాంకం>& chain, /*బహిర్గతం*/
				      hb_సదిశ_t<ద్విగుణ> &interp_x_deltas_scratch,
				      hb_సదిశ_t<ద్విగుణ> &interp_y_deltas_scratch)
{
  గుర్తులేని n = contour_points.నిడివి;
  ఒకవేళ (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    తిరిగి అబద్ధం;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  కొరకు (గుర్తులేని i = 0; i < n; i++)
  {
    గుర్తులేని ఉత్తమ_ఖర్చు = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = ఉత్తమ_ఖర్చు;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    ఒకవేళ (i > 0 && forced_సమితి.has (i - 1))
      కొనసాగు;

    పూర్ణాంకం lookback_సూచీ = hb_max ((పూర్ణాంకం) i - (పూర్ణాంకం) lookback + 1, -1);
    కొరకు (పూర్ణాంకం j = i - 2; j >= lookback_సూచీ; j--)
    {
      గుర్తులేని ఖర్చు = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      గుర్తులేని అంకె_points = i - j - 1;
      గుర్తులేని p1 = (j == -1 ? n - 1 : j);
      ఒకవేళ (ఖర్చు < ఉత్తమ_ఖర్చు &&
          can_iup_లోపల_between (contour_points.as_array ().sub_array (j + 1, అంకె_points),
                               x_deltas.as_array ().sub_array (j + 1, అంకె_points),
                               y_deltas.as_array ().sub_array (j + 1, అంకె_points),
                               contour_points.arrayZ[p1], contour_points.arrayZ[i],
                               x_deltas.arrayZ[p1], x_deltas.arrayZ[i],
                               y_deltas.arrayZ[p1], y_deltas.arrayZ[i],
                               tolerance_sq,
			       interp_x_deltas_scratch, interp_y_deltas_scratch))
      {
        ఉత్తమ_ఖర్చు = ఖర్చు;
        costs.arrayZ[i] = ఉత్తమ_ఖర్చు;
        chain.arrayZ[i] = j;
      }

      ఒకవేళ (j > 0 && forced_సమితి.has (j))
        ఆపు;
    }
  }
  తిరిగి నిజం;
}

స్థిర తార్కిక _iup_contour_optimize (స్థిరరాశి hb_array_t<స్థిరరాశి contour_బిందువు_t> contour_points,
                                   స్థిరరాశి hb_array_t<స్థిరరాశి పూర్ణాంకం> x_deltas,
                                   స్థిరరాశి hb_array_t<స్థిరరాశి పూర్ణాంకం> y_deltas,
                                   hb_array_t<తార్కిక> opt_indices, /*బహిర్గతం*/
                                   ద్విగుణ tolerance,
				   iup_scratch_t &scratch)
{
  గుర్తులేని n = contour_points.నిడివి;
  ఒకవేళ (opt_indices.నిడివి != n ||
      x_deltas.నిడివి != n ||
      y_deltas.నిడివి != n)
    తిరిగి అబద్ధం;

  ఒకవేళ (unlikely (n > hb_iup_సమితి_t::పుట_బిట్లు))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    కొరకు (గుర్తులేని i = 0; i < n; i++)
      opt_indices.arrayZ[i] = నిజం;
    తిరిగి నిజం;
  }

  తార్కిక all_within_tolerance = నిజం;
  ద్విగుణ tolerance_sq = tolerance * tolerance;
  కొరకు (గుర్తులేని i = 0; i < n; i++)
  {
    పూర్ణాంకం dx = x_deltas.arrayZ[i];
    పూర్ణాంకం dy = y_deltas.arrayZ[i];
    ఒకవేళ ((ద్విగుణ) dx * dx + (ద్విగుణ) dy * dy > tolerance_sq)
    {
      all_within_tolerance = అబద్ధం;
      ఆపు;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  ఒకవేళ (all_within_tolerance)
    తిరిగి నిజం;

  /* If there's exactly one point, return it */
  ఒకవేళ (n == 1)
  {
    opt_indices.arrayZ[0] = నిజం;
    తిరిగి నిజం;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  తార్కిక all_deltas_are_equal = నిజం;
  కొరకు (గుర్తులేని i = 1; i < n; i++)
    ఒకవేళ (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = అబద్ధం;
      ఆపు;
    }

  ఒకవేళ (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = నిజం;
    తిరిగి నిజం;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_సమితి_t forced_సమితి;
  iup_contour_bound_forced_సమితి (contour_points, x_deltas, y_deltas, forced_సమితి, tolerance);

  hb_సదిశ_t<గుర్తులేని> &costs = scratch.costs.reset ();
  hb_సదిశ_t<పూర్ణాంకం> &chain = scratch.chain.reset ();

  ఒకవేళ (!forced_సమితి.is_empty ())
  {
    పూర్ణాంకం k = n - 1 - forced_సమితి.get_max ();
    ఒకవేళ (k < 0)
      తిరిగి అబద్ధం;

    hb_సదిశ_t<పూర్ణాంకం> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_సదిశ_t<పూర్ణాంకం> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_బిందువు_సదిశ_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_సమితి_t rot_forced_సమితి;
    ఒకవేళ (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_సమితి (forced_సమితి, k, n, rot_forced_సమితి))
      తిరిగి అబద్ధం;

    ఒకవేళ (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_సమితి, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      తిరిగి అబద్ధం;

    hb_iup_సమితి_t solution;
    పూర్ణాంకం సూచీ_2 = n - 1;
    అయితే (సూచీ_2 != -1)
    {
      solution.కూడు (సూచీ_2);
      సూచీ_2 = chain.arrayZ[సూచీ_2];
    }

    ఒకవేళ (solution.is_empty () ||
        forced_సమితి.get_population () > solution.get_population ())
      తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని i : solution)
      opt_indices.arrayZ[i] = నిజం;

    hb_సదిశ_t<తార్కిక> &rot_indices = scratch.rot_indices.reset ();
    స్థిరరాశి hb_array_t<స్థిరరాశి తార్కిక> opt_indices_array (opt_indices.arrayZ, opt_indices.నిడివి);
    ఒకవేళ (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  లేకపోతే
  {
    hb_సదిశ_t<పూర్ణాంకం> repeat_x_deltas, repeat_y_deltas;
    contour_బిందువు_సదిశ_t repeat_points;

    ఒకవేళ (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      తిరిగి అబద్ధం;

    గుర్తులేని contour_బిందువు_కొలత = hb_static_కొలత (contour_బిందువు_t);
    కొరకు (గుర్తులేని i = 0; i < n; i++)
    {
      hb_memcpy ((శూన్య *) repeat_x_deltas.arrayZ, (స్థిరరాశి శూన్య *) x_deltas.arrayZ, n * పరిమాణం (repeat_x_deltas[0]));
      hb_memcpy ((శూన్య *) (repeat_x_deltas.arrayZ + n), (స్థిరరాశి శూన్య *) x_deltas.arrayZ, n * పరిమాణం (repeat_x_deltas[0]));

      hb_memcpy ((శూన్య *) repeat_y_deltas.arrayZ, (స్థిరరాశి శూన్య *) y_deltas.arrayZ, n * పరిమాణం (repeat_x_deltas[0]));
      hb_memcpy ((శూన్య *) (repeat_y_deltas.arrayZ + n), (స్థిరరాశి శూన్య *) y_deltas.arrayZ, n * పరిమాణం (repeat_x_deltas[0]));

      hb_memcpy ((శూన్య *) repeat_points.arrayZ, (స్థిరరాశి శూన్య *) contour_points.arrayZ, n * contour_బిందువు_కొలత);
      hb_memcpy ((శూన్య *) (repeat_points.arrayZ + n), (స్థిరరాశి శూన్య *) contour_points.arrayZ, n * contour_బిందువు_కొలత);
    }

    ఒకవేళ (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_సమితి, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      తిరిగి అబద్ధం;

    గుర్తులేని ఉత్తమ_ఖర్చు = n + 1;
    పూర్ణాంకం len = costs.నిడివి;
    hb_iup_సమితి_t ఉత్తమ_sol;
    కొరకు (పూర్ణాంకం ప్రారంభం = n - 1; ప్రారంభం < len; ప్రారంభం++)
    {
      hb_iup_సమితి_t solution;
      పూర్ణాంకం i = ప్రారంభం;
      పూర్ణాంకం lookback = ప్రారంభం - (పూర్ణాంకం) n;
      అయితే (i > lookback)
      {
        solution.కూడు (i % n);
        i = chain.arrayZ[i];
      }
      ఒకవేళ (i == lookback)
      {
        గుర్తులేని ఖర్చు_i = i < 0 ? 0 : costs.arrayZ[i];
        గుర్తులేని ఖర్చు = costs.arrayZ[ప్రారంభం] - ఖర్చు_i;
        ఒకవేళ (ఖర్చు <= ఉత్తమ_ఖర్చు)
        {
          ఉత్తమ_sol = solution;
          ఉత్తమ_ఖర్చు = ఖర్చు;
        }
      }
    }

    కొరకు (గుర్తులేని i = 0; i < n; i++)
      ఒకవేళ (ఉత్తమ_sol.has (i))
        opt_indices.arrayZ[i] = నిజం;
  }
  తిరిగి నిజం;
}

తార్కిక iup_delta_optimize (స్థిరరాశి contour_బిందువు_సదిశ_t& contour_points,
                         స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& x_deltas,
                         స్థిరరాశి hb_సదిశ_t<పూర్ణాంకం>& y_deltas,
                         hb_సదిశ_t<తార్కిక>& opt_indices, /*బహిర్గతం*/
			 iup_scratch_t &scratch,
                         ద్విగుణ tolerance)
{
  ఒకవేళ (!opt_indices.resize (contour_points.నిడివి))
      తిరిగి అబద్ధం;

  hb_సదిశ_t<గుర్తులేని> &చివర_points = scratch.చివర_points.reset ();

  గుర్తులేని లెక్క = contour_points.నిడివి;
  ఒకవేళ (unlikely (!చివర_points.alloc (లెక్క)))
    తిరిగి అబద్ధం;

  ఒకవేళ (లెక్క < 4)
  {
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      చివర_points.push (i);
  }
  లేకపోతే
  {
    కొరకు (గుర్తులేని i = 0; i < లెక్క - 4; i++)
      ఒకవేళ (contour_points.arrayZ[i].is_చివర_బిందువు)
        చివర_points.push (i);

    /* phantom points */
    కొరకు (గుర్తులేని i = లెక్క - 4; i < లెక్క; i++)
      చివర_points.push (i);
  }

  ఒకవేళ (చివర_points.లోపల_error ()) తిరిగి అబద్ధం;

  గుర్తులేని ప్రారంభం = 0;
  కొరకు (గుర్తులేని చివర : చివర_points)
  {
    గుర్తులేని len = చివర - ప్రారంభం + 1;
    ఒకవేళ (!_iup_contour_optimize (contour_points.as_array ().sub_array (ప్రారంభం, len),
                                x_deltas.as_array ().sub_array (ప్రారంభం, len),
                                y_deltas.as_array ().sub_array (ప్రారంభం, len),
                                opt_indices.as_array ().sub_array (ప్రారంభం, len),
                                tolerance,
				scratch))
      తిరిగి అబద్ధం;
    ప్రారంభం = చివర + 1;
  }
  తిరిగి నిజం;
}
