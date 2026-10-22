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

#ଅନ୍ତର୍ଭାଵ "hb-subset-instancer-iup.hh"

#ଅନ୍ତର୍ଭାଵ "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ସ୍ଥିର unsigned MAX_LOOKBACK = 8;

ସ୍ଥିର ଶୂନ୍ୟ _iup_contour_bound_forced_set (ଅଚଳ hb_array_t<ଅଚଳ contour_point_t> contour_points,
                                           ଅଚଳ hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> x_deltas,
                                           ଅଚଳ hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           ଦ୍ୱିଗୁଣ tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    ପାଇଁ (unsigned j = 0; j < 2; j++)
    {
      ଦ୍ୱିଗୁଣ cj, lcj, ncj;
      ଦ୍ୱିଗୁଣ dj, ldj, ndj;
      ଯଦି (j == 0)
      {
        cj = static_cast<ଦ୍ୱିଗୁଣ> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<ଦ୍ୱିଗୁଣ> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<ଦ୍ୱିଗୁଣ> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      ଅନ୍ୟଥା
      {
        cj = static_cast<ଦ୍ୱିଗୁଣ> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<ଦ୍ୱିଗୁଣ> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<ଦ୍ୱିଗୁଣ> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      ଦ୍ୱିଗୁଣ c1, c2;
      ଦ୍ୱିଗୁଣ d1, d2;
      ଯଦି (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      ଅନ୍ୟଥା
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      ତାର୍କିକ force = ମିଥ୍ୟା;
      ଯଦି (c1 == c2)
      {
        ଯଦି (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = ସତ୍ୟ;
      }
      ଅନ୍ୟଥା ଯଦି (c1 <= cj && cj <= c2)
      {
        ଯଦି (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = ସତ୍ୟ;
      }
      ଅନ୍ୟଥା
      {
        ଯଦି (d1 != d2)
        {
          ଯଦି (cj < c1)
          {
            ଯଦି (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = ସତ୍ୟ;
          }
          ଅନ୍ୟଥା
          {
            ଯଦି (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = ସତ୍ୟ;
          }
        }
      }

      ଯଦି (force)
      {
        forced_set.add (i);
        ଭାଙ୍ଗ;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ସ୍ଥିର ତାର୍କିକ rotate_array (ଅଚଳ hb_array_t<ଅଚଳ T>& org_array,
                          ପୂର୍ଣ୍ଣସଂଖ୍ୟା k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  ଯଦି (!n) ଫେରନ୍ତୁ ସତ୍ୟ;
  ଯଦି (unlikely (!out.resize_dirty  (n)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned item_size = hb_static_size (T);
  ଯଦି (k < 0)
    k = n - (-k) % n;
  ଅନ୍ୟଥା
    k %= n;

  hb_memcpy ((ଶୂନ୍ୟ *) out.arrayZ, (ଅଚଳ ଶୂନ୍ୟ *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((ଶୂନ୍ୟ *) (out.arrayZ + k), (ଅଚଳ ଶୂନ୍ୟ *) org_array.arrayZ, (n - k) * item_size);
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ତାର୍କିକ rotate_set (ଅଚଳ hb_iup_set_t& org_set,
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  ଯଦି (!n) ଫେରନ୍ତୁ ମିଥ୍ୟା;
  k %= n;
  ଯଦି (k < 0)
    k = n + k;

  ଯଦି (k == 0)
  {
    out = org_set;
  }
  ଅନ୍ୟଥା
  {
    ପାଇଁ (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ସ୍ଥିର ତାର୍କିକ _iup_segment (ଅଚଳ hb_array_t<ଅଚଳ contour_point_t> contour_points,
                          ଅଚଳ hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> x_deltas,
                          ଅଚଳ hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> y_deltas,
                          ଅଚଳ contour_point_t& p1, ଅଚଳ contour_point_t& p2,
                          ପୂର୍ଣ୍ଣସଂଖ୍ୟା p1_dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା p2_dx,
                          ପୂର୍ଣ୍ଣସଂଖ୍ୟା p1_dy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା p2_dy,
			  ଦ୍ୱିଗୁଣ tolerance_sq,
                          hb_vector_t<ଦ୍ୱିଗୁଣ>& interp_x_deltas, /* OUT */
                          hb_vector_t<ଦ୍ୱିଗୁଣ>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  ଯଦି (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ପାଇଁ (unsigned j = 0; j < 2; j++)
  {
    ଦଶମିକ contour_point_t::* xp;
    ଦ୍ୱିଗୁଣ x1, x2, d1, d2;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *in;
    ଦ୍ୱିଗୁଣ *out;
    ଯଦି (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<ଦ୍ୱିଗୁଣ> (p1.x);
      x2 = static_cast<ଦ୍ୱିଗୁଣ> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    ଅନ୍ୟଥା
    {
      xp = &contour_point_t::y;
      x1 = static_cast<ଦ୍ୱିଗୁଣ> (p1.y);
      x2 = static_cast<ଦ୍ୱିଗୁଣ> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    ଯଦି (x1 == x2)
    {
      ଯଦି (d1 == d2)
      {
        ପାଇଁ (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      ଅନ୍ୟଥା
      {
        ପାଇଁ (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      ଜାରି;
    }

    ଯଦି (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    ଦ୍ୱିଗୁଣ scale = (d2 - d1) / (x2 - x1);
    ପାଇଁ (unsigned i = 0; i < n; i++)
    {
      ଦ୍ୱିଗୁଣ x = (ଦ୍ୱିଗୁଣ) (contour_points.arrayZ[i].*xp);
      ଦ୍ୱିଗୁଣ d;
      ଯଦି (x <= x1)
        d = d1;
      ଅନ୍ୟଥା ଯଦି (x >= x2)
        d = d2;
      ଅନ୍ୟଥା
        d = d1 + (x - x1) * scale;

      out[i] = d;
      ଦ୍ୱିଗୁଣ err = d - in[i];
      ଯଦି (err * err > tolerance_sq)
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ତାର୍କିକ _can_iup_in_between (ଅଚଳ hb_array_t<ଅଚଳ contour_point_t> contour_points,
                                 ଅଚଳ hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> x_deltas,
                                 ଅଚଳ hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> y_deltas,
                                 ଅଚଳ contour_point_t& p1, ଅଚଳ contour_point_t& p2,
                                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା p1_dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା p2_dx,
                                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା p1_dy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା p2_dy,
                                 ଦ୍ୱିଗୁଣ tolerance_sq,
				 hb_vector_t<ଦ୍ୱିଗୁଣ> &interp_x_deltas, /* scratch */
				 hb_vector_t<ଦ୍ୱିଗୁଣ> &interp_y_deltas /* scratch */)
{
  ଯଦି (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned ଅଙ୍କ = contour_points.length;

  ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ; i++)
  {
    ଦ୍ୱିଗୁଣ dx = static_cast<ଦ୍ୱିଗୁଣ> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    ଦ୍ୱିଗୁଣ dy = static_cast<ଦ୍ୱିଗୁଣ> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    ଯଦି (dx * dx + dy * dy > tolerance_sq)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ତାର୍କିକ _iup_contour_optimize_dp (ଅଚଳ contour_point_vector_t& contour_points,
                                      ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& x_deltas,
                                      ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& y_deltas,
                                      ଅଚଳ hb_iup_set_t& forced_set,
                                      ଦ୍ୱିଗୁଣ tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& chain, /* OUT */
				      hb_vector_t<ଦ୍ୱିଗୁଣ> &interp_x_deltas_scratch,
				      hb_vector_t<ଦ୍ୱିଗୁଣ> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  ଯଦି (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  ପାଇଁ (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    ଯଦି (i > 0 && forced_set.has (i - 1))
      ଜାରି;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookback_index = hb_max ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) lookback + 1, -1);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned ଅଙ୍କ_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      ଯଦି (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, ଅଙ୍କ_points),
                               x_deltas.as_array ().sub_array (j + 1, ଅଙ୍କ_points),
                               y_deltas.as_array ().sub_array (j + 1, ଅଙ୍କ_points),
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

      ଯଦି (j > 0 && forced_set.has (j))
        ଭାଙ୍ଗ;
    }
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର ତାର୍କିକ _iup_contour_optimize (ଅଚଳ hb_array_t<ଅଚଳ contour_point_t> contour_points,
                                   ଅଚଳ hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> x_deltas,
                                   ଅଚଳ hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> y_deltas,
                                   hb_array_t<ତାର୍କିକ> opt_indices, /* OUT */
                                   ଦ୍ୱିଗୁଣ tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  ଯଦି (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    ପାଇଁ (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = ସତ୍ୟ;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ all_within_tolerance = ସତ୍ୟ;
  ଦ୍ୱିଗୁଣ tolerance_sq = tolerance * tolerance;
  ପାଇଁ (unsigned i = 0; i < n; i++)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx = x_deltas.arrayZ[i];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy = y_deltas.arrayZ[i];
    ଯଦି ((ଦ୍ୱିଗୁଣ) dx * dx + (ଦ୍ୱିଗୁଣ) dy * dy > tolerance_sq)
    {
      all_within_tolerance = ମିଥ୍ୟା;
      ଭାଙ୍ଗ;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  ଯଦି (all_within_tolerance)
    ଫେରନ୍ତୁ ସତ୍ୟ;

  /* If there's exactly one point, return it */
  ଯଦି (n == 1)
  {
    opt_indices.arrayZ[0] = ସତ୍ୟ;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  ତାର୍କିକ all_deltas_are_equal = ସତ୍ୟ;
  ପାଇଁ (unsigned i = 1; i < n; i++)
    ଯଦି (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = ମିଥ୍ୟା;
      ଭାଙ୍ଗ;
    }

  ଯଦି (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = ସତ୍ୟ;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &chain = scratch.chain.reset ();

  ଯଦି (!forced_set.is_empty ())
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = n - 1 - forced_set.get_max ();
    ଯଦି (k < 0)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    ଯଦି (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    hb_iup_set_t solution;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = n - 1;
    ଯେତେବେଳେ (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    ଯଦି (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned i : solution)
      opt_indices.arrayZ[i] = ସତ୍ୟ;

    hb_vector_t<ତାର୍କିକ> &rot_indices = scratch.rot_indices.reset ();
    ଅଚଳ hb_array_t<ଅଚଳ ତାର୍କିକ> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    ଯଦି (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପାଇଁ (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  ଅନ୍ୟଥା
  {
    hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    ଯଦି (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    ପାଇଁ (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((ଶୂନ୍ୟ *) repeat_x_deltas.arrayZ, (ଅଚଳ ଶୂନ୍ୟ *) x_deltas.arrayZ, n * ଆକାର (repeat_x_deltas[0]));
      hb_memcpy ((ଶୂନ୍ୟ *) (repeat_x_deltas.arrayZ + n), (ଅଚଳ ଶୂନ୍ୟ *) x_deltas.arrayZ, n * ଆକାର (repeat_x_deltas[0]));

      hb_memcpy ((ଶୂନ୍ୟ *) repeat_y_deltas.arrayZ, (ଅଚଳ ଶୂନ୍ୟ *) y_deltas.arrayZ, n * ଆକାର (repeat_x_deltas[0]));
      hb_memcpy ((ଶୂନ୍ୟ *) (repeat_y_deltas.arrayZ + n), (ଅଚଳ ଶୂନ୍ୟ *) y_deltas.arrayZ, n * ଆକାର (repeat_x_deltas[0]));

      hb_memcpy ((ଶୂନ୍ୟ *) repeat_points.arrayZ, (ଅଚଳ ଶୂନ୍ୟ *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((ଶୂନ୍ୟ *) (repeat_points.arrayZ + n), (ଅଚଳ ଶୂନ୍ୟ *) contour_points.arrayZ, n * contour_point_size);
    }

    ଯଦି (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned best_cost = n + 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = costs.length;
    hb_iup_set_t best_sol;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookback = start - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) n;
      ଯେତେବେଳେ (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      ଯଦି (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        ଯଦି (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    ପାଇଁ (unsigned i = 0; i < n; i++)
      ଯଦି (best_sol.has (i))
        opt_indices.arrayZ[i] = ସତ୍ୟ;
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

ତାର୍କିକ iup_delta_optimize (ଅଚଳ contour_point_vector_t& contour_points,
                         ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& x_deltas,
                         ଅଚଳ hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& y_deltas,
                         hb_vector_t<ତାର୍କିକ>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         ଦ୍ୱିଗୁଣ tolerance)
{
  ଯଦି (!opt_indices.resize (contour_points.length))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned ଗଣନା_2 = contour_points.length;
  ଯଦି (unlikely (!end_points.alloc (ଗଣନା_2)))
    ଫେରନ୍ତୁ ମିଥ୍ୟା;

  ଯଦି (ଗଣନା_2 < 4)
  {
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      end_points.push (i);
  }
  ଅନ୍ୟଥା
  {
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2 - 4; i++)
      ଯଦି (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    ପାଇଁ (unsigned i = ଗଣନା_2 - 4; i < ଗଣନା_2; i++)
      end_points.push (i);
  }

  ଯଦି (end_points.in_error ()) ଫେରନ୍ତୁ ମିଥ୍ୟା;

  unsigned start = 0;
  ପାଇଁ (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    ଯଦି (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    start = end + 1;
  }
  ଫେରନ୍ତୁ ସତ୍ୟ;
}
