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

#include "hb-subset-instancer-iup.hh"

#include "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr static unsigned MAX_LOOKBACK = 8;

static void _iup_contour_bound_forced_set (const hb_array_t<const contour_point_t> contour_points,
                                           const hb_array_t<const số_nguyên> x_deltas,
                                           const hb_array_t<const số_nguyên> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           double tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  cho (số_nguyên i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    cho (unsigned j = 0; j < 2; j++)
    {
      double cj, lcj, ncj;
      double dj, ldj, ndj;
      nếu (j == 0)
      {
        cj = static_cast<double> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<double> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<double> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      không_thì
      {
        cj = static_cast<double> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<double> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<double> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      double c1, c2;
      double d1, d2;
      nếu (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      không_thì
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      bool force = false;
      nếu (c1 == c2)
      {
        nếu (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = true;
      }
      không_thì nếu (c1 <= cj && cj <= c2)
      {
        nếu (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = true;
      }
      không_thì
      {
        nếu (d1 != d2)
        {
          nếu (cj < c1)
          {
            nếu (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = true;
          }
          không_thì
          {
            nếu (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = true;
          }
        }
      }

      nếu (force)
      {
        forced_set.add (i);
        break;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
static bool rotate_array (const hb_array_t<const T>& org_array,
                          số_nguyên k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  nếu (!n) trả_về true;
  nếu (unlikely (!out.resize_dirty  (n)))
    trả_về false;

  unsigned item_size = hb_static_size (T);
  nếu (k < 0)
    k = n - (-k) % n;
  không_thì
    k %= n;

  hb_memcpy ((void *) out.arrayZ, (const void *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((void *) (out.arrayZ + k), (const void *) org_array.arrayZ, (n - k) * item_size);
  trả_về true;
}

static bool rotate_set (const hb_iup_set_t& org_set,
                        số_nguyên k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  nếu (!n) trả_về false;
  k %= n;
  nếu (k < 0)
    k = n + k;

  nếu (k == 0)
  {
    out = org_set;
  }
  không_thì
  {
    cho (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  trả_về true;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
static bool _iup_segment (const hb_array_t<const contour_point_t> contour_points,
                          const hb_array_t<const số_nguyên> x_deltas,
                          const hb_array_t<const số_nguyên> y_deltas,
                          const contour_point_t& p1, const contour_point_t& p2,
                          số_nguyên p1_dx, số_nguyên p2_dx,
                          số_nguyên p1_dy, số_nguyên p2_dy,
			  double tolerance_sq,
                          hb_vector_t<double>& interp_x_deltas, /* OUT */
                          hb_vector_t<double>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  nếu (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    trả_về false;

  cho (unsigned j = 0; j < 2; j++)
  {
    float contour_point_t::* xp;
    double x1, x2, d1, d2;
    const số_nguyên *in;
    double *out;
    nếu (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<double> (p1.x);
      x2 = static_cast<double> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    không_thì
    {
      xp = &contour_point_t::y;
      x1 = static_cast<double> (p1.y);
      x2 = static_cast<double> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    nếu (x1 == x2)
    {
      nếu (d1 == d2)
      {
        cho (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      không_thì
      {
        cho (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      continue;
    }

    nếu (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    double scale = (d2 - d1) / (x2 - x1);
    cho (unsigned i = 0; i < n; i++)
    {
      double x = (double) (contour_points.arrayZ[i].*xp);
      double d;
      nếu (x <= x1)
        d = d1;
      không_thì nếu (x >= x2)
        d = d2;
      không_thì
        d = d1 + (x - x1) * scale;

      out[i] = d;
      double err = d - in[i];
      nếu (err * err > tolerance_sq)
	trả_về false;
    }
  }
  trả_về true;
}

static bool _can_iup_in_between (const hb_array_t<const contour_point_t> contour_points,
                                 const hb_array_t<const số_nguyên> x_deltas,
                                 const hb_array_t<const số_nguyên> y_deltas,
                                 const contour_point_t& p1, const contour_point_t& p2,
                                 số_nguyên p1_dx, số_nguyên p2_dx,
                                 số_nguyên p1_dy, số_nguyên p2_dy,
                                 double tolerance_sq,
				 hb_vector_t<double> &interp_x_deltas, /* scratch */
				 hb_vector_t<double> &interp_y_deltas /* scratch */)
{
  nếu (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    trả_về false;

  unsigned num = contour_points.length;

  cho (unsigned i = 0; i < num; i++)
  {
    double dx = static_cast<double> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    double dy = static_cast<double> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    nếu (dx * dx + dy * dy > tolerance_sq)
      trả_về false;
  }
  trả_về true;
}

static bool _iup_contour_optimize_dp (const contour_point_vector_t& contour_points,
                                      const hb_vector_t<số_nguyên>& x_deltas,
                                      const hb_vector_t<số_nguyên>& y_deltas,
                                      const hb_iup_set_t& forced_set,
                                      double tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<số_nguyên>& chain, /* OUT */
				      hb_vector_t<double> &interp_x_deltas_scratch,
				      hb_vector_t<double> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  nếu (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    trả_về false;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  cho (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    nếu (i > 0 && forced_set.has (i - 1))
      continue;

    số_nguyên lookback_index = hb_max ((số_nguyên) i - (số_nguyên) lookback + 1, -1);
    cho (số_nguyên j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned num_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      nếu (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, num_points),
                               x_deltas.as_array ().sub_array (j + 1, num_points),
                               y_deltas.as_array ().sub_array (j + 1, num_points),
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

      nếu (j > 0 && forced_set.has (j))
        break;
    }
  }
  trả_về true;
}

static bool _iup_contour_optimize (const hb_array_t<const contour_point_t> contour_points,
                                   const hb_array_t<const số_nguyên> x_deltas,
                                   const hb_array_t<const số_nguyên> y_deltas,
                                   hb_array_t<bool> opt_indices, /* OUT */
                                   double tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  nếu (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    trả_về false;

  nếu (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    cho (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = true;
    trả_về true;
  }

  bool all_within_tolerance = true;
  double tolerance_sq = tolerance * tolerance;
  cho (unsigned i = 0; i < n; i++)
  {
    số_nguyên dx = x_deltas.arrayZ[i];
    số_nguyên dy = y_deltas.arrayZ[i];
    nếu ((double) dx * dx + (double) dy * dy > tolerance_sq)
    {
      all_within_tolerance = false;
      break;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  nếu (all_within_tolerance)
    trả_về true;

  /* If there's exactly one point, return it */
  nếu (n == 1)
  {
    opt_indices.arrayZ[0] = true;
    trả_về true;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  bool all_deltas_are_equal = true;
  cho (unsigned i = 1; i < n; i++)
    nếu (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = false;
      break;
    }

  nếu (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = true;
    trả_về true;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<số_nguyên> &chain = scratch.chain.reset ();

  nếu (!forced_set.is_empty ())
  {
    số_nguyên k = n - 1 - forced_set.get_max ();
    nếu (k < 0)
      trả_về false;

    hb_vector_t<số_nguyên> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<số_nguyên> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    nếu (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      trả_về false;

    nếu (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      trả_về false;

    hb_iup_set_t solution;
    số_nguyên index = n - 1;
    trong_khi (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    nếu (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      trả_về false;

    cho (unsigned i : solution)
      opt_indices.arrayZ[i] = true;

    hb_vector_t<bool> &rot_indices = scratch.rot_indices.reset ();
    const hb_array_t<const bool> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    nếu (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      trả_về false;

    cho (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  không_thì
  {
    hb_vector_t<số_nguyên> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    nếu (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      trả_về false;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    cho (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((void *) repeat_x_deltas.arrayZ, (const void *) x_deltas.arrayZ, n * sizeof (repeat_x_deltas[0]));
      hb_memcpy ((void *) (repeat_x_deltas.arrayZ + n), (const void *) x_deltas.arrayZ, n * sizeof (repeat_x_deltas[0]));

      hb_memcpy ((void *) repeat_y_deltas.arrayZ, (const void *) y_deltas.arrayZ, n * sizeof (repeat_x_deltas[0]));
      hb_memcpy ((void *) (repeat_y_deltas.arrayZ + n), (const void *) y_deltas.arrayZ, n * sizeof (repeat_x_deltas[0]));

      hb_memcpy ((void *) repeat_points.arrayZ, (const void *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((void *) (repeat_points.arrayZ + n), (const void *) contour_points.arrayZ, n * contour_point_size);
    }

    nếu (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      trả_về false;

    unsigned best_cost = n + 1;
    số_nguyên len = costs.length;
    hb_iup_set_t best_sol;
    cho (số_nguyên start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      số_nguyên i = start;
      số_nguyên lookback = start - (số_nguyên) n;
      trong_khi (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      nếu (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        nếu (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    cho (unsigned i = 0; i < n; i++)
      nếu (best_sol.has (i))
        opt_indices.arrayZ[i] = true;
  }
  trả_về true;
}

bool iup_delta_optimize (const contour_point_vector_t& contour_points,
                         const hb_vector_t<số_nguyên>& x_deltas,
                         const hb_vector_t<số_nguyên>& y_deltas,
                         hb_vector_t<bool>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         double tolerance)
{
  nếu (!opt_indices.resize (contour_points.length))
      trả_về false;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned count = contour_points.length;
  nếu (unlikely (!end_points.alloc (count)))
    trả_về false;

  nếu (count < 4)
  {
    cho (unsigned i = 0; i < count; i++)
      end_points.push (i);
  }
  không_thì
  {
    cho (unsigned i = 0; i < count - 4; i++)
      nếu (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    cho (unsigned i = count - 4; i < count; i++)
      end_points.push (i);
  }

  nếu (end_points.in_error ()) trả_về false;

  unsigned start = 0;
  cho (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    nếu (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      trả_về false;
    start = end + 1;
  }
  trả_về true;
}
