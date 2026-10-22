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

#അന്തര്ഭാവ "hb-subset-instancer-iup.hh"

#അന്തര്ഭാവ "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr സ്ഥിതം unsigned MAX_LOOKBACK = 8;

സ്ഥിതം ശൂന്യം _iup_contour_bound_forced_set (സ്ഥിരം hb_array_t<സ്ഥിരം contour_point_t> contour_points,
                                           സ്ഥിരം hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> x_deltas,
                                           സ്ഥിരം hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           ഇരട്ട tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    വേണ്ടി (unsigned j = 0; j < 2; j++)
    {
      ഇരട്ട cj, lcj, ncj;
      ഇരട്ട dj, ldj, ndj;
      എങ്കിൽ (j == 0)
      {
        cj = static_cast<ഇരട്ട> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<ഇരട്ട> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<ഇരട്ട> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      അല്ലെങ്കിൽ
      {
        cj = static_cast<ഇരട്ട> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<ഇരട്ട> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<ഇരട്ട> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      ഇരട്ട c1, c2;
      ഇരട്ട d1, d2;
      എങ്കിൽ (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      അല്ലെങ്കിൽ
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      തർക്കം force = അസത്യം;
      എങ്കിൽ (c1 == c2)
      {
        എങ്കിൽ (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = സത്യം;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (c1 <= cj && cj <= c2)
      {
        എങ്കിൽ (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = സത്യം;
      }
      അല്ലെങ്കിൽ
      {
        എങ്കിൽ (d1 != d2)
        {
          എങ്കിൽ (cj < c1)
          {
            എങ്കിൽ (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = സത്യം;
          }
          അല്ലെങ്കിൽ
          {
            എങ്കിൽ (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = സത്യം;
          }
        }
      }

      എങ്കിൽ (force)
      {
        forced_set.add (i);
        നിർത്തുക;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
സ്ഥിതം തർക്കം rotate_array (സ്ഥിരം hb_array_t<സ്ഥിരം T>& org_array,
                          പൂർണ്ണസംഖ്യ k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  എങ്കിൽ (!n) തിരികെ സത്യം;
  എങ്കിൽ (unlikely (!out.resize_dirty  (n)))
    തിരികെ അസത്യം;

  unsigned item_size = hb_static_size (T);
  എങ്കിൽ (k < 0)
    k = n - (-k) % n;
  അല്ലെങ്കിൽ
    k %= n;

  hb_memcpy ((ശൂന്യം *) out.arrayZ, (സ്ഥിരം ശൂന്യം *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((ശൂന്യം *) (out.arrayZ + k), (സ്ഥിരം ശൂന്യം *) org_array.arrayZ, (n - k) * item_size);
  തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം rotate_set (സ്ഥിരം hb_iup_set_t& org_set,
                        പൂർണ്ണസംഖ്യ k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  എങ്കിൽ (!n) തിരികെ അസത്യം;
  k %= n;
  എങ്കിൽ (k < 0)
    k = n + k;

  എങ്കിൽ (k == 0)
  {
    out = org_set;
  }
  അല്ലെങ്കിൽ
  {
    വേണ്ടി (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  തിരികെ സത്യം;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
സ്ഥിതം തർക്കം _iup_segment (സ്ഥിരം hb_array_t<സ്ഥിരം contour_point_t> contour_points,
                          സ്ഥിരം hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> x_deltas,
                          സ്ഥിരം hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> y_deltas,
                          സ്ഥിരം contour_point_t& p1, സ്ഥിരം contour_point_t& p2,
                          പൂർണ്ണസംഖ്യ p1_dx, പൂർണ്ണസംഖ്യ p2_dx,
                          പൂർണ്ണസംഖ്യ p1_dy, പൂർണ്ണസംഖ്യ p2_dy,
			  ഇരട്ട tolerance_sq,
                          hb_vector_t<ഇരട്ട>& interp_x_deltas, /* OUT */
                          hb_vector_t<ഇരട്ട>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  എങ്കിൽ (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    തിരികെ അസത്യം;

  വേണ്ടി (unsigned j = 0; j < 2; j++)
  {
    ദശാംശം contour_point_t::* xp;
    ഇരട്ട x1, x2, d1, d2;
    സ്ഥിരം പൂർണ്ണസംഖ്യ *in;
    ഇരട്ട *out;
    എങ്കിൽ (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<ഇരട്ട> (p1.x);
      x2 = static_cast<ഇരട്ട> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    അല്ലെങ്കിൽ
    {
      xp = &contour_point_t::y;
      x1 = static_cast<ഇരട്ട> (p1.y);
      x2 = static_cast<ഇരട്ട> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    എങ്കിൽ (x1 == x2)
    {
      എങ്കിൽ (d1 == d2)
      {
        വേണ്ടി (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      അല്ലെങ്കിൽ
      {
        വേണ്ടി (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      തുടരുക;
    }

    എങ്കിൽ (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    ഇരട്ട scale = (d2 - d1) / (x2 - x1);
    വേണ്ടി (unsigned i = 0; i < n; i++)
    {
      ഇരട്ട x = (ഇരട്ട) (contour_points.arrayZ[i].*xp);
      ഇരട്ട d;
      എങ്കിൽ (x <= x1)
        d = d1;
      അല്ലെങ്കിൽ എങ്കിൽ (x >= x2)
        d = d2;
      അല്ലെങ്കിൽ
        d = d1 + (x - x1) * scale;

      out[i] = d;
      ഇരട്ട err = d - in[i];
      എങ്കിൽ (err * err > tolerance_sq)
	തിരികെ അസത്യം;
    }
  }
  തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം _can_iup_in_between (സ്ഥിരം hb_array_t<സ്ഥിരം contour_point_t> contour_points,
                                 സ്ഥിരം hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> x_deltas,
                                 സ്ഥിരം hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> y_deltas,
                                 സ്ഥിരം contour_point_t& p1, സ്ഥിരം contour_point_t& p2,
                                 പൂർണ്ണസംഖ്യ p1_dx, പൂർണ്ണസംഖ്യ p2_dx,
                                 പൂർണ്ണസംഖ്യ p1_dy, പൂർണ്ണസംഖ്യ p2_dy,
                                 ഇരട്ട tolerance_sq,
				 hb_vector_t<ഇരട്ട> &interp_x_deltas, /* scratch */
				 hb_vector_t<ഇരട്ട> &interp_y_deltas /* scratch */)
{
  എങ്കിൽ (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    തിരികെ അസത്യം;

  unsigned അക്കം = contour_points.length;

  വേണ്ടി (unsigned i = 0; i < അക്കം; i++)
  {
    ഇരട്ട dx = static_cast<ഇരട്ട> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    ഇരട്ട dy = static_cast<ഇരട്ട> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    എങ്കിൽ (dx * dx + dy * dy > tolerance_sq)
      തിരികെ അസത്യം;
  }
  തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം _iup_contour_optimize_dp (സ്ഥിരം contour_point_vector_t& contour_points,
                                      സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>& x_deltas,
                                      സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>& y_deltas,
                                      സ്ഥിരം hb_iup_set_t& forced_set,
                                      ഇരട്ട tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<പൂർണ്ണസംഖ്യ>& chain, /* OUT */
				      hb_vector_t<ഇരട്ട> &interp_x_deltas_scratch,
				      hb_vector_t<ഇരട്ട> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  എങ്കിൽ (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    തിരികെ അസത്യം;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  വേണ്ടി (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    എങ്കിൽ (i > 0 && forced_set.has (i - 1))
      തുടരുക;

    പൂർണ്ണസംഖ്യ lookback_index = hb_max ((പൂർണ്ണസംഖ്യ) i - (പൂർണ്ണസംഖ്യ) lookback + 1, -1);
    വേണ്ടി (പൂർണ്ണസംഖ്യ j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned അക്കം_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      എങ്കിൽ (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, അക്കം_points),
                               x_deltas.as_array ().sub_array (j + 1, അക്കം_points),
                               y_deltas.as_array ().sub_array (j + 1, അക്കം_points),
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

      എങ്കിൽ (j > 0 && forced_set.has (j))
        നിർത്തുക;
    }
  }
  തിരികെ സത്യം;
}

സ്ഥിതം തർക്കം _iup_contour_optimize (സ്ഥിരം hb_array_t<സ്ഥിരം contour_point_t> contour_points,
                                   സ്ഥിരം hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> x_deltas,
                                   സ്ഥിരം hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> y_deltas,
                                   hb_array_t<തർക്കം> opt_indices, /* OUT */
                                   ഇരട്ട tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  എങ്കിൽ (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    തിരികെ അസത്യം;

  എങ്കിൽ (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    വേണ്ടി (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = സത്യം;
    തിരികെ സത്യം;
  }

  തർക്കം all_within_tolerance = സത്യം;
  ഇരട്ട tolerance_sq = tolerance * tolerance;
  വേണ്ടി (unsigned i = 0; i < n; i++)
  {
    പൂർണ്ണസംഖ്യ dx = x_deltas.arrayZ[i];
    പൂർണ്ണസംഖ്യ dy = y_deltas.arrayZ[i];
    എങ്കിൽ ((ഇരട്ട) dx * dx + (ഇരട്ട) dy * dy > tolerance_sq)
    {
      all_within_tolerance = അസത്യം;
      നിർത്തുക;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  എങ്കിൽ (all_within_tolerance)
    തിരികെ സത്യം;

  /* If there's exactly one point, return it */
  എങ്കിൽ (n == 1)
  {
    opt_indices.arrayZ[0] = സത്യം;
    തിരികെ സത്യം;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  തർക്കം all_deltas_are_equal = സത്യം;
  വേണ്ടി (unsigned i = 1; i < n; i++)
    എങ്കിൽ (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = അസത്യം;
      നിർത്തുക;
    }

  എങ്കിൽ (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = സത്യം;
    തിരികെ സത്യം;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<പൂർണ്ണസംഖ്യ> &chain = scratch.chain.reset ();

  എങ്കിൽ (!forced_set.is_empty ())
  {
    പൂർണ്ണസംഖ്യ k = n - 1 - forced_set.get_max ();
    എങ്കിൽ (k < 0)
      തിരികെ അസത്യം;

    hb_vector_t<പൂർണ്ണസംഖ്യ> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<പൂർണ്ണസംഖ്യ> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    എങ്കിൽ (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      തിരികെ അസത്യം;

    എങ്കിൽ (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      തിരികെ അസത്യം;

    hb_iup_set_t solution;
    പൂർണ്ണസംഖ്യ index = n - 1;
    ആകുമ്പോൾ (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    എങ്കിൽ (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      തിരികെ അസത്യം;

    വേണ്ടി (unsigned i : solution)
      opt_indices.arrayZ[i] = സത്യം;

    hb_vector_t<തർക്കം> &rot_indices = scratch.rot_indices.reset ();
    സ്ഥിരം hb_array_t<സ്ഥിരം തർക്കം> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    എങ്കിൽ (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      തിരികെ അസത്യം;

    വേണ്ടി (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  അല്ലെങ്കിൽ
  {
    hb_vector_t<പൂർണ്ണസംഖ്യ> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    എങ്കിൽ (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      തിരികെ അസത്യം;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    വേണ്ടി (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((ശൂന്യം *) repeat_x_deltas.arrayZ, (സ്ഥിരം ശൂന്യം *) x_deltas.arrayZ, n * വലുപ്പം (repeat_x_deltas[0]));
      hb_memcpy ((ശൂന്യം *) (repeat_x_deltas.arrayZ + n), (സ്ഥിരം ശൂന്യം *) x_deltas.arrayZ, n * വലുപ്പം (repeat_x_deltas[0]));

      hb_memcpy ((ശൂന്യം *) repeat_y_deltas.arrayZ, (സ്ഥിരം ശൂന്യം *) y_deltas.arrayZ, n * വലുപ്പം (repeat_x_deltas[0]));
      hb_memcpy ((ശൂന്യം *) (repeat_y_deltas.arrayZ + n), (സ്ഥിരം ശൂന്യം *) y_deltas.arrayZ, n * വലുപ്പം (repeat_x_deltas[0]));

      hb_memcpy ((ശൂന്യം *) repeat_points.arrayZ, (സ്ഥിരം ശൂന്യം *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((ശൂന്യം *) (repeat_points.arrayZ + n), (സ്ഥിരം ശൂന്യം *) contour_points.arrayZ, n * contour_point_size);
    }

    എങ്കിൽ (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      തിരികെ അസത്യം;

    unsigned best_cost = n + 1;
    പൂർണ്ണസംഖ്യ len = costs.length;
    hb_iup_set_t best_sol;
    വേണ്ടി (പൂർണ്ണസംഖ്യ start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      പൂർണ്ണസംഖ്യ i = start;
      പൂർണ്ണസംഖ്യ lookback = start - (പൂർണ്ണസംഖ്യ) n;
      ആകുമ്പോൾ (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      എങ്കിൽ (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        എങ്കിൽ (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    വേണ്ടി (unsigned i = 0; i < n; i++)
      എങ്കിൽ (best_sol.has (i))
        opt_indices.arrayZ[i] = സത്യം;
  }
  തിരികെ സത്യം;
}

തർക്കം iup_delta_optimize (സ്ഥിരം contour_point_vector_t& contour_points,
                         സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>& x_deltas,
                         സ്ഥിരം hb_vector_t<പൂർണ്ണസംഖ്യ>& y_deltas,
                         hb_vector_t<തർക്കം>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         ഇരട്ട tolerance)
{
  എങ്കിൽ (!opt_indices.resize (contour_points.length))
      തിരികെ അസത്യം;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned എണ്ണം = contour_points.length;
  എങ്കിൽ (unlikely (!end_points.alloc (എണ്ണം)))
    തിരികെ അസത്യം;

  എങ്കിൽ (എണ്ണം < 4)
  {
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      end_points.push (i);
  }
  അല്ലെങ്കിൽ
  {
    വേണ്ടി (unsigned i = 0; i < എണ്ണം - 4; i++)
      എങ്കിൽ (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    വേണ്ടി (unsigned i = എണ്ണം - 4; i < എണ്ണം; i++)
      end_points.push (i);
  }

  എങ്കിൽ (end_points.in_error ()) തിരികെ അസത്യം;

  unsigned start = 0;
  വേണ്ടി (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    എങ്കിൽ (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      തിരികെ അസത്യം;
    start = end + 1;
  }
  തിരികെ സത്യം;
}
