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

#ಅನ್ತರ್ಭಾವ "hb-subset-instancer-iup.hh"

#ಅನ್ತರ್ಭಾವ "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ಸ್ಥಿರ unsigned MAX_LOOKBACK = 8;

ಸ್ಥಿರ ಶೂನ್ಯ _iup_contour_bound_forced_set (ಧ್ರುವ hb_array_t<ಧ್ರುವ contour_point_t> contour_points,
                                           ಧ್ರುವ hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> x_deltas,
                                           ಧ್ರುವ hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           ದ್ವಿಗುಣ tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    ಪ್ರತಿ (unsigned j = 0; j < 2; j++)
    {
      ದ್ವಿಗುಣ cj, lcj, ncj;
      ದ್ವಿಗುಣ dj, ldj, ndj;
      ಯದಿ (j == 0)
      {
        cj = static_cast<ದ್ವಿಗುಣ> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<ದ್ವಿಗುಣ> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<ದ್ವಿಗುಣ> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      ಅನ್ಯಥಾ
      {
        cj = static_cast<ದ್ವಿಗುಣ> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<ದ್ವಿಗುಣ> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<ದ್ವಿಗುಣ> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      ದ್ವಿಗುಣ c1, c2;
      ದ್ವಿಗುಣ d1, d2;
      ಯದಿ (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      ಅನ್ಯಥಾ
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      ತಾರ್ಕಿಕ force = ಅಸತ್ಯ;
      ಯದಿ (c1 == c2)
      {
        ಯದಿ (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = ಸತ್ಯ;
      }
      ಅನ್ಯಥಾ ಯದಿ (c1 <= cj && cj <= c2)
      {
        ಯದಿ (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = ಸತ್ಯ;
      }
      ಅನ್ಯಥಾ
      {
        ಯದಿ (d1 != d2)
        {
          ಯದಿ (cj < c1)
          {
            ಯದಿ (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = ಸತ್ಯ;
          }
          ಅನ್ಯಥಾ
          {
            ಯದಿ (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = ಸತ್ಯ;
          }
        }
      }

      ಯದಿ (force)
      {
        forced_set.add (i);
        ಭಙ್ಗ;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ಸ್ಥಿರ ತಾರ್ಕಿಕ rotate_array (ಧ್ರುವ hb_array_t<ಧ್ರುವ T>& org_array,
                          ಪೂರ್ಣಾಙ್ಕ k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  ಯದಿ (!n) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  ಯದಿ (unlikely (!out.resize_dirty  (n)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned item_size = hb_static_size (T);
  ಯದಿ (k < 0)
    k = n - (-k) % n;
  ಅನ್ಯಥಾ
    k %= n;

  hb_memcpy ((ಶೂನ್ಯ *) out.arrayZ, (ಧ್ರುವ ಶೂನ್ಯ *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((ಶೂನ್ಯ *) (out.arrayZ + k), (ಧ್ರುವ ಶೂನ್ಯ *) org_array.arrayZ, (n - k) * item_size);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ rotate_set (ಧ್ರುವ hb_iup_set_t& org_set,
                        ಪೂರ್ಣಾಙ್ಕ k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  ಯದಿ (!n) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  k %= n;
  ಯದಿ (k < 0)
    k = n + k;

  ಯದಿ (k == 0)
  {
    out = org_set;
  }
  ಅನ್ಯಥಾ
  {
    ಪ್ರತಿ (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ಸ್ಥಿರ ತಾರ್ಕಿಕ _iup_segment (ಧ್ರುವ hb_array_t<ಧ್ರುವ contour_point_t> contour_points,
                          ಧ್ರುವ hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> x_deltas,
                          ಧ್ರುವ hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> y_deltas,
                          ಧ್ರುವ contour_point_t& p1, ಧ್ರುವ contour_point_t& p2,
                          ಪೂರ್ಣಾಙ್ಕ p1_dx, ಪೂರ್ಣಾಙ್ಕ p2_dx,
                          ಪೂರ್ಣಾಙ್ಕ p1_dy, ಪೂರ್ಣಾಙ್ಕ p2_dy,
			  ದ್ವಿಗುಣ tolerance_sq,
                          hb_vector_t<ದ್ವಿಗುಣ>& interp_x_deltas, /* OUT */
                          hb_vector_t<ದ್ವಿಗುಣ>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  ಯದಿ (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಪ್ರತಿ (unsigned j = 0; j < 2; j++)
  {
    ಭಿನ್ನ contour_point_t::* xp;
    ದ್ವಿಗುಣ x1, x2, d1, d2;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *in;
    ದ್ವಿಗುಣ *out;
    ಯದಿ (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<ದ್ವಿಗುಣ> (p1.x);
      x2 = static_cast<ದ್ವಿಗುಣ> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    ಅನ್ಯಥಾ
    {
      xp = &contour_point_t::y;
      x1 = static_cast<ದ್ವಿಗುಣ> (p1.y);
      x2 = static_cast<ದ್ವಿಗುಣ> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    ಯದಿ (x1 == x2)
    {
      ಯದಿ (d1 == d2)
      {
        ಪ್ರತಿ (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      ಅನ್ಯಥಾ
      {
        ಪ್ರತಿ (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      ಅನುವರ್ತ;
    }

    ಯದಿ (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    ದ್ವಿಗುಣ scale = (d2 - d1) / (x2 - x1);
    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
    {
      ದ್ವಿಗುಣ x = (ದ್ವಿಗುಣ) (contour_points.arrayZ[i].*xp);
      ದ್ವಿಗುಣ d;
      ಯದಿ (x <= x1)
        d = d1;
      ಅನ್ಯಥಾ ಯದಿ (x >= x2)
        d = d2;
      ಅನ್ಯಥಾ
        d = d1 + (x - x1) * scale;

      out[i] = d;
      ದ್ವಿಗುಣ err = d - in[i];
      ಯದಿ (err * err > tolerance_sq)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ _can_iup_in_between (ಧ್ರುವ hb_array_t<ಧ್ರುವ contour_point_t> contour_points,
                                 ಧ್ರುವ hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> x_deltas,
                                 ಧ್ರುವ hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> y_deltas,
                                 ಧ್ರುವ contour_point_t& p1, ಧ್ರುವ contour_point_t& p2,
                                 ಪೂರ್ಣಾಙ್ಕ p1_dx, ಪೂರ್ಣಾಙ್ಕ p2_dx,
                                 ಪೂರ್ಣಾಙ್ಕ p1_dy, ಪೂರ್ಣಾಙ್ಕ p2_dy,
                                 ದ್ವಿಗುಣ tolerance_sq,
				 hb_vector_t<ದ್ವಿಗುಣ> &interp_x_deltas, /* scratch */
				 hb_vector_t<ದ್ವಿಗುಣ> &interp_y_deltas /* scratch */)
{
  ಯದಿ (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned ಅಂಕೆ = contour_points.length;

  ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ; i++)
  {
    ದ್ವಿಗುಣ dx = static_cast<ದ್ವಿಗುಣ> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    ದ್ವಿಗುಣ dy = static_cast<ದ್ವಿಗುಣ> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    ಯದಿ (dx * dx + dy * dy > tolerance_sq)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ _iup_contour_optimize_dp (ಧ್ರುವ contour_point_vector_t& contour_points,
                                      ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>& x_deltas,
                                      ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>& y_deltas,
                                      ಧ್ರುವ hb_iup_set_t& forced_set,
                                      ದ್ವಿಗುಣ tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>& chain, /* OUT */
				      hb_vector_t<ದ್ವಿಗುಣ> &interp_x_deltas_scratch,
				      hb_vector_t<ದ್ವಿಗುಣ> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  ಯದಿ (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  ಪ್ರತಿ (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    ಯದಿ (i > 0 && forced_set.has (i - 1))
      ಅನುವರ್ತ;

    ಪೂರ್ಣಾಙ್ಕ lookback_index = hb_max ((ಪೂರ್ಣಾಙ್ಕ) i - (ಪೂರ್ಣಾಙ್ಕ) lookback + 1, -1);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned ಅಂಕೆ_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      ಯದಿ (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, ಅಂಕೆ_points),
                               x_deltas.as_array ().sub_array (j + 1, ಅಂಕೆ_points),
                               y_deltas.as_array ().sub_array (j + 1, ಅಂಕೆ_points),
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

      ಯದಿ (j > 0 && forced_set.has (j))
        ಭಙ್ಗ;
    }
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ _iup_contour_optimize (ಧ್ರುವ hb_array_t<ಧ್ರುವ contour_point_t> contour_points,
                                   ಧ್ರುವ hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> x_deltas,
                                   ಧ್ರುವ hb_array_t<ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ> y_deltas,
                                   hb_array_t<ತಾರ್ಕಿಕ> opt_indices, /* OUT */
                                   ದ್ವಿಗುಣ tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  ಯದಿ (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ all_within_tolerance = ಸತ್ಯ;
  ದ್ವಿಗುಣ tolerance_sq = tolerance * tolerance;
  ಪ್ರತಿ (unsigned i = 0; i < n; i++)
  {
    ಪೂರ್ಣಾಙ್ಕ dx = x_deltas.arrayZ[i];
    ಪೂರ್ಣಾಙ್ಕ dy = y_deltas.arrayZ[i];
    ಯದಿ ((ದ್ವಿಗುಣ) dx * dx + (ದ್ವಿಗುಣ) dy * dy > tolerance_sq)
    {
      all_within_tolerance = ಅಸತ್ಯ;
      ಭಙ್ಗ;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  ಯದಿ (all_within_tolerance)
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  /* If there's exactly one point, return it */
  ಯದಿ (n == 1)
  {
    opt_indices.arrayZ[0] = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  ತಾರ್ಕಿಕ all_deltas_are_equal = ಸತ್ಯ;
  ಪ್ರತಿ (unsigned i = 1; i < n; i++)
    ಯದಿ (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = ಅಸತ್ಯ;
      ಭಙ್ಗ;
    }

  ಯದಿ (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> &chain = scratch.chain.reset ();

  ಯದಿ (!forced_set.is_empty ())
  {
    ಪೂರ್ಣಾಙ್ಕ k = n - 1 - forced_set.get_max ();
    ಯದಿ (k < 0)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    ಯದಿ (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    hb_iup_set_t solution;
    ಪೂರ್ಣಾಙ್ಕ index = n - 1;
    ಯಾವತ್ (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    ಯದಿ (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned i : solution)
      opt_indices.arrayZ[i] = ಸತ್ಯ;

    hb_vector_t<ತಾರ್ಕಿಕ> &rot_indices = scratch.rot_indices.reset ();
    ಧ್ರುವ hb_array_t<ಧ್ರುವ ತಾರ್ಕಿಕ> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    ಯದಿ (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  ಅನ್ಯಥಾ
  {
    hb_vector_t<ಪೂರ್ಣಾಙ್ಕ> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    ಯದಿ (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((ಶೂನ್ಯ *) repeat_x_deltas.arrayZ, (ಧ್ರುವ ಶೂನ್ಯ *) x_deltas.arrayZ, n * ಪರಿಮಾಣ (repeat_x_deltas[0]));
      hb_memcpy ((ಶೂನ್ಯ *) (repeat_x_deltas.arrayZ + n), (ಧ್ರುವ ಶೂನ್ಯ *) x_deltas.arrayZ, n * ಪರಿಮಾಣ (repeat_x_deltas[0]));

      hb_memcpy ((ಶೂನ್ಯ *) repeat_y_deltas.arrayZ, (ಧ್ರುವ ಶೂನ್ಯ *) y_deltas.arrayZ, n * ಪರಿಮಾಣ (repeat_x_deltas[0]));
      hb_memcpy ((ಶೂನ್ಯ *) (repeat_y_deltas.arrayZ + n), (ಧ್ರುವ ಶೂನ್ಯ *) y_deltas.arrayZ, n * ಪರಿಮಾಣ (repeat_x_deltas[0]));

      hb_memcpy ((ಶೂನ್ಯ *) repeat_points.arrayZ, (ಧ್ರುವ ಶೂನ್ಯ *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((ಶೂನ್ಯ *) (repeat_points.arrayZ + n), (ಧ್ರುವ ಶೂನ್ಯ *) contour_points.arrayZ, n * contour_point_size);
    }

    ಯದಿ (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned best_cost = n + 1;
    ಪೂರ್ಣಾಙ್ಕ len = costs.length;
    hb_iup_set_t best_sol;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      ಪೂರ್ಣಾಙ್ಕ i = start;
      ಪೂರ್ಣಾಙ್ಕ lookback = start - (ಪೂರ್ಣಾಙ್ಕ) n;
      ಯಾವತ್ (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      ಯದಿ (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        ಯದಿ (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    ಪ್ರತಿ (unsigned i = 0; i < n; i++)
      ಯದಿ (best_sol.has (i))
        opt_indices.arrayZ[i] = ಸತ್ಯ;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ತಾರ್ಕಿಕ iup_delta_optimize (ಧ್ರುವ contour_point_vector_t& contour_points,
                         ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>& x_deltas,
                         ಧ್ರುವ hb_vector_t<ಪೂರ್ಣಾಙ್ಕ>& y_deltas,
                         hb_vector_t<ತಾರ್ಕಿಕ>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         ದ್ವಿಗುಣ tolerance)
{
  ಯದಿ (!opt_indices.resize (contour_points.length))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned ಎಣಿಕೆ = contour_points.length;
  ಯದಿ (unlikely (!end_points.alloc (ಎಣಿಕೆ)))
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  ಯದಿ (ಎಣಿಕೆ < 4)
  {
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      end_points.push (i);
  }
  ಅನ್ಯಥಾ
  {
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ - 4; i++)
      ಯದಿ (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    ಪ್ರತಿ (unsigned i = ಎಣಿಕೆ - 4; i < ಎಣಿಕೆ; i++)
      end_points.push (i);
  }

  ಯದಿ (end_points.in_error ()) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

  unsigned start = 0;
  ಪ್ರತಿ (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    ಯದಿ (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    start = end + 1;
  }
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
