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
                                           const hb_array_t<const puur_naa_nka> x_deltas,
                                           const hb_array_t<const puur_naa_nka> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           dviguu_na tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  for (puur_naa_nka i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    for (unsigned j = 0; j < 2; j++)
    {
      dviguu_na cj, lcj, ncj;
      dviguu_na dj, ldj, ndj;
      ᱡᱩᱫᱤ (j == 0)
      {
        cj = static_cast<dviguu_na> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<dviguu_na> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<dviguu_na> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      ᱵᱟᱝ
      {
        cj = static_cast<dviguu_na> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<dviguu_na> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<dviguu_na> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      dviguu_na c1, c2;
      dviguu_na d1, d2;
      ᱡᱩᱫᱤ (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      ᱵᱟᱝ
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      bool force = false;
      ᱡᱩᱫᱤ (c1 == c2)
      {
        ᱡᱩᱫᱤ (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = true;
      }
      ᱵᱟᱝ ᱡᱩᱫᱤ (c1 <= cj && cj <= c2)
      {
        ᱡᱩᱫᱤ (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = true;
      }
      ᱵᱟᱝ
      {
        ᱡᱩᱫᱤ (d1 != d2)
        {
          ᱡᱩᱫᱤ (cj < c1)
          {
            ᱡᱩᱫᱤ (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = true;
          }
          ᱵᱟᱝ
          {
            ᱡᱩᱫᱤ (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = true;
          }
        }
      }

      ᱡᱩᱫᱤ (force)
      {
        forced_set.add (i);
        bha_nga;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
static bool rotate_array (const hb_array_t<const T>& org_array,
                          puur_naa_nka k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  ᱡᱩᱫᱤ (!n) ᱨᱩᱣᱟᱹᱲ true;
  ᱡᱩᱫᱤ (unlikely (!out.resize_dirty  (n)))
    ᱨᱩᱣᱟᱹᱲ false;

  unsigned item_size = hb_static_size (T);
  ᱡᱩᱫᱤ (k < 0)
    k = n - (-k) % n;
  ᱵᱟᱝ
    k %= n;

  hb_memcpy ((void *) out.arrayZ, (const void *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((void *) (out.arrayZ + k), (const void *) org_array.arrayZ, (n - k) * item_size);
  ᱨᱩᱣᱟᱹᱲ true;
}

static bool rotate_set (const hb_iup_set_t& org_set,
                        puur_naa_nka k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  ᱡᱩᱫᱤ (!n) ᱨᱩᱣᱟᱹᱲ false;
  k %= n;
  ᱡᱩᱫᱤ (k < 0)
    k = n + k;

  ᱡᱩᱫᱤ (k == 0)
  {
    out = org_set;
  }
  ᱵᱟᱝ
  {
    for (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  ᱨᱩᱣᱟᱹᱲ true;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
static bool _iup_segment (const hb_array_t<const contour_point_t> contour_points,
                          const hb_array_t<const puur_naa_nka> x_deltas,
                          const hb_array_t<const puur_naa_nka> y_deltas,
                          const contour_point_t& p1, const contour_point_t& p2,
                          puur_naa_nka p1_dx, puur_naa_nka p2_dx,
                          puur_naa_nka p1_dy, puur_naa_nka p2_dy,
			  dviguu_na tolerance_sq,
                          hb_vector_t<dviguu_na>& interp_x_deltas, /* OUT */
                          hb_vector_t<dviguu_na>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  ᱡᱩᱫᱤ (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    ᱨᱩᱣᱟᱹᱲ false;

  for (unsigned j = 0; j < 2; j++)
  {
    float contour_point_t::* xp;
    dviguu_na x1, x2, d1, d2;
    const puur_naa_nka *in;
    dviguu_na *out;
    ᱡᱩᱫᱤ (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<dviguu_na> (p1.x);
      x2 = static_cast<dviguu_na> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    ᱵᱟᱝ
    {
      xp = &contour_point_t::y;
      x1 = static_cast<dviguu_na> (p1.y);
      x2 = static_cast<dviguu_na> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    ᱡᱩᱫᱤ (x1 == x2)
    {
      ᱡᱩᱫᱤ (d1 == d2)
      {
        for (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      ᱵᱟᱝ
      {
        for (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      continue;
    }

    ᱡᱩᱫᱤ (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    dviguu_na scale = (d2 - d1) / (x2 - x1);
    for (unsigned i = 0; i < n; i++)
    {
      dviguu_na x = (dviguu_na) (contour_points.arrayZ[i].*xp);
      dviguu_na d;
      ᱡᱩᱫᱤ (x <= x1)
        d = d1;
      ᱵᱟᱝ ᱡᱩᱫᱤ (x >= x2)
        d = d2;
      ᱵᱟᱝ
        d = d1 + (x - x1) * scale;

      out[i] = d;
      dviguu_na err = d - in[i];
      ᱡᱩᱫᱤ (err * err > tolerance_sq)
	ᱨᱩᱣᱟᱹᱲ false;
    }
  }
  ᱨᱩᱣᱟᱹᱲ true;
}

static bool _can_iup_in_between (const hb_array_t<const contour_point_t> contour_points,
                                 const hb_array_t<const puur_naa_nka> x_deltas,
                                 const hb_array_t<const puur_naa_nka> y_deltas,
                                 const contour_point_t& p1, const contour_point_t& p2,
                                 puur_naa_nka p1_dx, puur_naa_nka p2_dx,
                                 puur_naa_nka p1_dy, puur_naa_nka p2_dy,
                                 dviguu_na tolerance_sq,
				 hb_vector_t<dviguu_na> &interp_x_deltas, /* scratch */
				 hb_vector_t<dviguu_na> &interp_y_deltas /* scratch */)
{
  ᱡᱩᱫᱤ (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    ᱨᱩᱣᱟᱹᱲ false;

  unsigned num = contour_points.length;

  for (unsigned i = 0; i < num; i++)
  {
    dviguu_na dx = static_cast<dviguu_na> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    dviguu_na dy = static_cast<dviguu_na> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    ᱡᱩᱫᱤ (dx * dx + dy * dy > tolerance_sq)
      ᱨᱩᱣᱟᱹᱲ false;
  }
  ᱨᱩᱣᱟᱹᱲ true;
}

static bool _iup_contour_optimize_dp (const contour_point_vector_t& contour_points,
                                      const hb_vector_t<puur_naa_nka>& x_deltas,
                                      const hb_vector_t<puur_naa_nka>& y_deltas,
                                      const hb_iup_set_t& forced_set,
                                      dviguu_na tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<puur_naa_nka>& chain, /* OUT */
				      hb_vector_t<dviguu_na> &interp_x_deltas_scratch,
				      hb_vector_t<dviguu_na> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  ᱡᱩᱫᱤ (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    ᱨᱩᱣᱟᱹᱲ false;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  for (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    ᱡᱩᱫᱤ (i > 0 && forced_set.has (i - 1))
      continue;

    puur_naa_nka lookback_index = hb_max ((puur_naa_nka) i - (puur_naa_nka) lookback + 1, -1);
    for (puur_naa_nka j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned num_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      ᱡᱩᱫᱤ (cost < best_cost &&
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

      ᱡᱩᱫᱤ (j > 0 && forced_set.has (j))
        bha_nga;
    }
  }
  ᱨᱩᱣᱟᱹᱲ true;
}

static bool _iup_contour_optimize (const hb_array_t<const contour_point_t> contour_points,
                                   const hb_array_t<const puur_naa_nka> x_deltas,
                                   const hb_array_t<const puur_naa_nka> y_deltas,
                                   hb_array_t<bool> opt_indices, /* OUT */
                                   dviguu_na tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  ᱡᱩᱫᱤ (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    ᱨᱩᱣᱟᱹᱲ false;

  ᱡᱩᱫᱤ (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    for (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = true;
    ᱨᱩᱣᱟᱹᱲ true;
  }

  bool all_within_tolerance = true;
  dviguu_na tolerance_sq = tolerance * tolerance;
  for (unsigned i = 0; i < n; i++)
  {
    puur_naa_nka dx = x_deltas.arrayZ[i];
    puur_naa_nka dy = y_deltas.arrayZ[i];
    ᱡᱩᱫᱤ ((dviguu_na) dx * dx + (dviguu_na) dy * dy > tolerance_sq)
    {
      all_within_tolerance = false;
      bha_nga;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  ᱡᱩᱫᱤ (all_within_tolerance)
    ᱨᱩᱣᱟᱹᱲ true;

  /* If there's exactly one point, return it */
  ᱡᱩᱫᱤ (n == 1)
  {
    opt_indices.arrayZ[0] = true;
    ᱨᱩᱣᱟᱹᱲ true;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  bool all_deltas_are_equal = true;
  for (unsigned i = 1; i < n; i++)
    ᱡᱩᱫᱤ (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = false;
      bha_nga;
    }

  ᱡᱩᱫᱤ (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = true;
    ᱨᱩᱣᱟᱹᱲ true;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<puur_naa_nka> &chain = scratch.chain.reset ();

  ᱡᱩᱫᱤ (!forced_set.is_empty ())
  {
    puur_naa_nka k = n - 1 - forced_set.get_max ();
    ᱡᱩᱫᱤ (k < 0)
      ᱨᱩᱣᱟᱹᱲ false;

    hb_vector_t<puur_naa_nka> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<puur_naa_nka> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    ᱡᱩᱫᱤ (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      ᱨᱩᱣᱟᱹᱲ false;

    ᱡᱩᱫᱤ (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ᱨᱩᱣᱟᱹᱲ false;

    hb_iup_set_t solution;
    puur_naa_nka index = n - 1;
    ᱡᱚᱠᱷᱚᱱ (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    ᱡᱩᱫᱤ (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      ᱨᱩᱣᱟᱹᱲ false;

    for (unsigned i : solution)
      opt_indices.arrayZ[i] = true;

    hb_vector_t<bool> &rot_indices = scratch.rot_indices.reset ();
    const hb_array_t<const bool> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    ᱡᱩᱫᱤ (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      ᱨᱩᱣᱟᱹᱲ false;

    for (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  ᱵᱟᱝ
  {
    hb_vector_t<puur_naa_nka> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    ᱡᱩᱫᱤ (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      ᱨᱩᱣᱟᱹᱲ false;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    for (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((void *) repeat_x_deltas.arrayZ, (const void *) x_deltas.arrayZ, n * parimaa_na (repeat_x_deltas[0]));
      hb_memcpy ((void *) (repeat_x_deltas.arrayZ + n), (const void *) x_deltas.arrayZ, n * parimaa_na (repeat_x_deltas[0]));

      hb_memcpy ((void *) repeat_y_deltas.arrayZ, (const void *) y_deltas.arrayZ, n * parimaa_na (repeat_x_deltas[0]));
      hb_memcpy ((void *) (repeat_y_deltas.arrayZ + n), (const void *) y_deltas.arrayZ, n * parimaa_na (repeat_x_deltas[0]));

      hb_memcpy ((void *) repeat_points.arrayZ, (const void *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((void *) (repeat_points.arrayZ + n), (const void *) contour_points.arrayZ, n * contour_point_size);
    }

    ᱡᱩᱫᱤ (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ᱨᱩᱣᱟᱹᱲ false;

    unsigned best_cost = n + 1;
    puur_naa_nka len = costs.length;
    hb_iup_set_t best_sol;
    for (puur_naa_nka start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      puur_naa_nka i = start;
      puur_naa_nka lookback = start - (puur_naa_nka) n;
      ᱡᱚᱠᱷᱚᱱ (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      ᱡᱩᱫᱤ (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        ᱡᱩᱫᱤ (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    for (unsigned i = 0; i < n; i++)
      ᱡᱩᱫᱤ (best_sol.has (i))
        opt_indices.arrayZ[i] = true;
  }
  ᱨᱩᱣᱟᱹᱲ true;
}

bool iup_delta_optimize (const contour_point_vector_t& contour_points,
                         const hb_vector_t<puur_naa_nka>& x_deltas,
                         const hb_vector_t<puur_naa_nka>& y_deltas,
                         hb_vector_t<bool>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         dviguu_na tolerance)
{
  ᱡᱩᱫᱤ (!opt_indices.resize (contour_points.length))
      ᱨᱩᱣᱟᱹᱲ false;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned count = contour_points.length;
  ᱡᱩᱫᱤ (unlikely (!end_points.alloc (count)))
    ᱨᱩᱣᱟᱹᱲ false;

  ᱡᱩᱫᱤ (count < 4)
  {
    for (unsigned i = 0; i < count; i++)
      end_points.push (i);
  }
  ᱵᱟᱝ
  {
    for (unsigned i = 0; i < count - 4; i++)
      ᱡᱩᱫᱤ (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    for (unsigned i = count - 4; i < count; i++)
      end_points.push (i);
  }

  ᱡᱩᱫᱤ (end_points.in_error ()) ᱨᱩᱣᱟᱹᱲ false;

  unsigned start = 0;
  for (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    ᱡᱩᱫᱤ (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      ᱨᱩᱣᱟᱹᱲ false;
    start = end + 1;
  }
  ᱨᱩᱣᱟᱹᱲ true;
}
