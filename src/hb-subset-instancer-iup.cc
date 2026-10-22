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

#অন্তর্ভাব "hb-subset-instancer-iup.hh"

#অন্তর্ভাব "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr স্থির unsigned MAX_LOOKBACK = 8;

স্থির শূন্য _iup_contour_bound_forced_set (ধ্রুবক hb_array_t<ধ্রুবক contour_point_t> contour_points,
                                           ধ্রুবক hb_array_t<ধ্রুবক পূর্ণসংখ্যা> x_deltas,
                                           ধ্রুবক hb_array_t<ধ্রুবক পূর্ণসংখ্যা> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           দ্বিগুণ tolerance = 0.0)
{
  unsigned len = contour_points.দৈর্ঘ্য;
  unsigned পরবর্তী_i = 0;
  জন্য (পূর্ণসংখ্যা i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    জন্য (unsigned j = 0; j < 2; j++)
    {
      দ্বিগুণ cj, lcj, ncj;
      দ্বিগুণ dj, ldj, ndj;
      যদি (j == 0)
      {
        cj = static_cast<দ্বিগুণ> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<দ্বিগুণ> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<দ্বিগুণ> (contour_points.arrayZ[পরবর্তী_i].x);
        ndj = x_deltas.arrayZ[পরবর্তী_i];
      }
      অন্যথা
      {
        cj = static_cast<দ্বিগুণ> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<দ্বিগুণ> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<দ্বিগুণ> (contour_points.arrayZ[পরবর্তী_i].y);
        ndj = y_deltas.arrayZ[পরবর্তী_i];
      }

      দ্বিগুণ c1, c2;
      দ্বিগুণ d1, d2;
      যদি (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      অন্যথা
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      যৌক্তিক force = মিথ্যা;
      যদি (c1 == c2)
      {
        যদি (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = সত্য;
      }
      অন্যথা যদি (c1 <= cj && cj <= c2)
      {
        যদি (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = সত্য;
      }
      অন্যথা
      {
        যদি (d1 != d2)
        {
          যদি (cj < c1)
          {
            যদি (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = সত্য;
          }
          অন্যথা
          {
            যদি (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = সত্য;
          }
        }
      }

      যদি (force)
      {
        forced_set.add (i);
        বিরতি;
      }
    }
    পরবর্তী_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
স্থির যৌক্তিক rotate_array (ধ্রুবক hb_array_t<ধ্রুবক T>& org_array,
                          পূর্ণসংখ্যা k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.দৈর্ঘ্য;
  যদি (!n) ফেরত সত্য;
  যদি (unlikely (!out.resize_dirty  (n)))
    ফেরত মিথ্যা;

  unsigned item_আকার = hb_static_আকার (T);
  যদি (k < 0)
    k = n - (-k) % n;
  অন্যথা
    k %= n;

  hb_memcpy ((শূন্য *) out.arrayZ, (ধ্রুবক শূন্য *) (org_array.arrayZ + n - k), k * item_আকার);
  hb_memcpy ((শূন্য *) (out.arrayZ + k), (ধ্রুবক শূন্য *) org_array.arrayZ, (n - k) * item_আকার);
  ফেরত সত্য;
}

স্থির যৌক্তিক rotate_set (ধ্রুবক hb_iup_set_t& org_set,
                        পূর্ণসংখ্যা k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  যদি (!n) ফেরত মিথ্যা;
  k %= n;
  যদি (k < 0)
    k = n + k;

  যদি (k == 0)
  {
    out = org_set;
  }
  অন্যথা
  {
    জন্য (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  ফেরত সত্য;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
স্থির যৌক্তিক _iup_segment (ধ্রুবক hb_array_t<ধ্রুবক contour_point_t> contour_points,
                          ধ্রুবক hb_array_t<ধ্রুবক পূর্ণসংখ্যা> x_deltas,
                          ধ্রুবক hb_array_t<ধ্রুবক পূর্ণসংখ্যা> y_deltas,
                          ধ্রুবক contour_point_t& p1, ধ্রুবক contour_point_t& p2,
                          পূর্ণসংখ্যা p1_dx, পূর্ণসংখ্যা p2_dx,
                          পূর্ণসংখ্যা p1_dy, পূর্ণসংখ্যা p2_dy,
			  দ্বিগুণ tolerance_sq,
                          hb_vector_t<দ্বিগুণ>& interp_x_deltas, /* OUT */
                          hb_vector_t<দ্বিগুণ>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.দৈর্ঘ্য;
  যদি (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    ফেরত মিথ্যা;

  জন্য (unsigned j = 0; j < 2; j++)
  {
    দশমিক contour_point_t::* xp;
    দ্বিগুণ x1, x2, d1, d2;
    ধ্রুবক পূর্ণসংখ্যা *in;
    দ্বিগুণ *out;
    যদি (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<দ্বিগুণ> (p1.x);
      x2 = static_cast<দ্বিগুণ> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    অন্যথা
    {
      xp = &contour_point_t::y;
      x1 = static_cast<দ্বিগুণ> (p1.y);
      x2 = static_cast<দ্বিগুণ> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    যদি (x1 == x2)
    {
      যদি (d1 == d2)
      {
        জন্য (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      অন্যথা
      {
        জন্য (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      চালু;
    }

    যদি (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    দ্বিগুণ scale = (d2 - d1) / (x2 - x1);
    জন্য (unsigned i = 0; i < n; i++)
    {
      দ্বিগুণ x = (দ্বিগুণ) (contour_points.arrayZ[i].*xp);
      দ্বিগুণ d;
      যদি (x <= x1)
        d = d1;
      অন্যথা যদি (x >= x2)
        d = d2;
      অন্যথা
        d = d1 + (x - x1) * scale;

      out[i] = d;
      দ্বিগুণ err = d - in[i];
      যদি (err * err > tolerance_sq)
	ফেরত মিথ্যা;
    }
  }
  ফেরত সত্য;
}

স্থির যৌক্তিক _can_iup_in_between (ধ্রুবক hb_array_t<ধ্রুবক contour_point_t> contour_points,
                                 ধ্রুবক hb_array_t<ধ্রুবক পূর্ণসংখ্যা> x_deltas,
                                 ধ্রুবক hb_array_t<ধ্রুবক পূর্ণসংখ্যা> y_deltas,
                                 ধ্রুবক contour_point_t& p1, ধ্রুবক contour_point_t& p2,
                                 পূর্ণসংখ্যা p1_dx, পূর্ণসংখ্যা p2_dx,
                                 পূর্ণসংখ্যা p1_dy, পূর্ণসংখ্যা p2_dy,
                                 দ্বিগুণ tolerance_sq,
				 hb_vector_t<দ্বিগুণ> &interp_x_deltas, /* scratch */
				 hb_vector_t<দ্বিগুণ> &interp_y_deltas /* scratch */)
{
  যদি (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    ফেরত মিথ্যা;

  unsigned অঙ্ক = contour_points.দৈর্ঘ্য;

  জন্য (unsigned i = 0; i < অঙ্ক; i++)
  {
    দ্বিগুণ dx = static_cast<দ্বিগুণ> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    দ্বিগুণ dy = static_cast<দ্বিগুণ> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    যদি (dx * dx + dy * dy > tolerance_sq)
      ফেরত মিথ্যা;
  }
  ফেরত সত্য;
}

স্থির যৌক্তিক _iup_contour_optimize_dp (ধ্রুবক contour_point_vector_t& contour_points,
                                      ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>& x_deltas,
                                      ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>& y_deltas,
                                      ধ্রুবক hb_iup_set_t& forced_set,
                                      দ্বিগুণ tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<পূর্ণসংখ্যা>& chain, /* OUT */
				      hb_vector_t<দ্বিগুণ> &interp_x_deltas_scratch,
				      hb_vector_t<দ্বিগুণ> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.দৈর্ঘ্য;
  যদি (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    ফেরত মিথ্যা;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  জন্য (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    যদি (i > 0 && forced_set.has (i - 1))
      চালু;

    পূর্ণসংখ্যা lookback_সূচক = hb_max ((পূর্ণসংখ্যা) i - (পূর্ণসংখ্যা) lookback + 1, -1);
    জন্য (পূর্ণসংখ্যা j = i - 2; j >= lookback_সূচক; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned অঙ্ক_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      যদি (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, অঙ্ক_points),
                               x_deltas.as_array ().sub_array (j + 1, অঙ্ক_points),
                               y_deltas.as_array ().sub_array (j + 1, অঙ্ক_points),
                               contour_points.arrayZ[p1], contour_points.arrayZ[i],
                               x_deltas.arrayZ[p1], x_deltas.arrayZ[i],
                               y_deltas.arrayZ[p1], y_deltas.arrayZ[i],
                               tolerance_sq,
			       interp_x_deltas_scratch, interp_y_deltas_scratch))
      {
        best_cost = cost;
        costs.arrayZ[i] = best_cost;
        chain.arrayZ[i] = j;
      }

      যদি (j > 0 && forced_set.has (j))
        বিরতি;
    }
  }
  ফেরত সত্য;
}

স্থির যৌক্তিক _iup_contour_optimize (ধ্রুবক hb_array_t<ধ্রুবক contour_point_t> contour_points,
                                   ধ্রুবক hb_array_t<ধ্রুবক পূর্ণসংখ্যা> x_deltas,
                                   ধ্রুবক hb_array_t<ধ্রুবক পূর্ণসংখ্যা> y_deltas,
                                   hb_array_t<যৌক্তিক> opt_indices, /* OUT */
                                   দ্বিগুণ tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.দৈর্ঘ্য;
  যদি (opt_indices.দৈর্ঘ্য != n ||
      x_deltas.দৈর্ঘ্য != n ||
      y_deltas.দৈর্ঘ্য != n)
    ফেরত মিথ্যা;

  যদি (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    জন্য (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = সত্য;
    ফেরত সত্য;
  }

  যৌক্তিক all_within_tolerance = সত্য;
  দ্বিগুণ tolerance_sq = tolerance * tolerance;
  জন্য (unsigned i = 0; i < n; i++)
  {
    পূর্ণসংখ্যা dx = x_deltas.arrayZ[i];
    পূর্ণসংখ্যা dy = y_deltas.arrayZ[i];
    যদি ((দ্বিগুণ) dx * dx + (দ্বিগুণ) dy * dy > tolerance_sq)
    {
      all_within_tolerance = মিথ্যা;
      বিরতি;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  যদি (all_within_tolerance)
    ফেরত সত্য;

  /* If there's exactly one point, return it */
  যদি (n == 1)
  {
    opt_indices.arrayZ[0] = সত্য;
    ফেরত সত্য;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  যৌক্তিক all_deltas_are_equal = সত্য;
  জন্য (unsigned i = 1; i < n; i++)
    যদি (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = মিথ্যা;
      বিরতি;
    }

  যদি (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = সত্য;
    ফেরত সত্য;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<পূর্ণসংখ্যা> &chain = scratch.chain.reset ();

  যদি (!forced_set.is_empty ())
  {
    পূর্ণসংখ্যা k = n - 1 - forced_set.get_max ();
    যদি (k < 0)
      ফেরত মিথ্যা;

    hb_vector_t<পূর্ণসংখ্যা> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<পূর্ণসংখ্যা> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    যদি (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      ফেরত মিথ্যা;

    যদি (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ফেরত মিথ্যা;

    hb_iup_set_t solution;
    পূর্ণসংখ্যা সূচক_2 = n - 1;
    যখন (সূচক_2 != -1)
    {
      solution.add (সূচক_2);
      সূচক_2 = chain.arrayZ[সূচক_2];
    }

    যদি (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      ফেরত মিথ্যা;

    জন্য (unsigned i : solution)
      opt_indices.arrayZ[i] = সত্য;

    hb_vector_t<যৌক্তিক> &rot_indices = scratch.rot_indices.reset ();
    ধ্রুবক hb_array_t<ধ্রুবক যৌক্তিক> opt_indices_array (opt_indices.arrayZ, opt_indices.দৈর্ঘ্য);
    যদি (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      ফেরত মিথ্যা;

    জন্য (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  অন্যথা
  {
    hb_vector_t<পূর্ণসংখ্যা> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    যদি (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      ফেরত মিথ্যা;

    unsigned contour_point_আকার = hb_static_আকার (contour_point_t);
    জন্য (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((শূন্য *) repeat_x_deltas.arrayZ, (ধ্রুবক শূন্য *) x_deltas.arrayZ, n * আকার (repeat_x_deltas[0]));
      hb_memcpy ((শূন্য *) (repeat_x_deltas.arrayZ + n), (ধ্রুবক শূন্য *) x_deltas.arrayZ, n * আকার (repeat_x_deltas[0]));

      hb_memcpy ((শূন্য *) repeat_y_deltas.arrayZ, (ধ্রুবক শূন্য *) y_deltas.arrayZ, n * আকার (repeat_x_deltas[0]));
      hb_memcpy ((শূন্য *) (repeat_y_deltas.arrayZ + n), (ধ্রুবক শূন্য *) y_deltas.arrayZ, n * আকার (repeat_x_deltas[0]));

      hb_memcpy ((শূন্য *) repeat_points.arrayZ, (ধ্রুবক শূন্য *) contour_points.arrayZ, n * contour_point_আকার);
      hb_memcpy ((শূন্য *) (repeat_points.arrayZ + n), (ধ্রুবক শূন্য *) contour_points.arrayZ, n * contour_point_আকার);
    }

    যদি (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ফেরত মিথ্যা;

    unsigned best_cost = n + 1;
    পূর্ণসংখ্যা len = costs.দৈর্ঘ্য;
    hb_iup_set_t best_sol;
    জন্য (পূর্ণসংখ্যা start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      পূর্ণসংখ্যা i = start;
      পূর্ণসংখ্যা lookback = start - (পূর্ণসংখ্যা) n;
      যখন (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      যদি (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        যদি (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    জন্য (unsigned i = 0; i < n; i++)
      যদি (best_sol.has (i))
        opt_indices.arrayZ[i] = সত্য;
  }
  ফেরত সত্য;
}

যৌক্তিক iup_delta_optimize (ধ্রুবক contour_point_vector_t& contour_points,
                         ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>& x_deltas,
                         ধ্রুবক hb_vector_t<পূর্ণসংখ্যা>& y_deltas,
                         hb_vector_t<যৌক্তিক>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         দ্বিগুণ tolerance)
{
  যদি (!opt_indices.resize (contour_points.দৈর্ঘ্য))
      ফেরত মিথ্যা;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned গুনতি = contour_points.দৈর্ঘ্য;
  যদি (unlikely (!end_points.alloc (গুনতি)))
    ফেরত মিথ্যা;

  যদি (গুনতি < 4)
  {
    জন্য (unsigned i = 0; i < গুনতি; i++)
      end_points.push (i);
  }
  অন্যথা
  {
    জন্য (unsigned i = 0; i < গুনতি - 4; i++)
      যদি (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    জন্য (unsigned i = গুনতি - 4; i < গুনতি; i++)
      end_points.push (i);
  }

  যদি (end_points.in_error ()) ফেরত মিথ্যা;

  unsigned start = 0;
  জন্য (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    যদি (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      ফেরত মিথ্যা;
    start = end + 1;
  }
  ফেরত সত্য;
}
