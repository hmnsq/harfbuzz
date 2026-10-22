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

#अन्तर्भाव "hb-subset-instancer-iup.hh"

#अन्तर्भाव "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr स्थिर unsigned MAX_LOOKBACK = 8;

स्थिर शून्य _iup_contour_bound_forced_set (ध्रुव hb_array_t<ध्रुव contour_point_t> contour_points,
                                           ध्रुव hb_array_t<ध्रुव पूर्णाङ्क> x_deltas,
                                           ध्रुव hb_array_t<ध्रुव पूर्णाङ्क> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           द्विगुण tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  प्रति (पूर्णाङ्क i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    प्रति (unsigned j = 0; j < 2; j++)
    {
      द्विगुण cj, lcj, ncj;
      द्विगुण dj, ldj, ndj;
      जइ (j == 0)
      {
        cj = static_cast<द्विगुण> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<द्विगुण> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<द्विगुण> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      अण्णहा
      {
        cj = static_cast<द्विगुण> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<द्विगुण> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<द्विगुण> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      द्विगुण c1, c2;
      द्विगुण d1, d2;
      जइ (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      अण्णहा
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      तार्किक force = मिच्छ;
      जइ (c1 == c2)
      {
        जइ (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = सच्च;
      }
      अण्णहा जइ (c1 <= cj && cj <= c2)
      {
        जइ (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = सच्च;
      }
      अण्णहा
      {
        जइ (d1 != d2)
        {
          जइ (cj < c1)
          {
            जइ (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = सच्च;
          }
          अण्णहा
          {
            जइ (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = सच्च;
          }
        }
      }

      जइ (force)
      {
        forced_set.add (i);
        भङ्ग;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
स्थिर तार्किक rotate_array (ध्रुव hb_array_t<ध्रुव T>& org_array,
                          पूर्णाङ्क k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  जइ (!n) प्रत्यागम सच्च;
  जइ (unlikely (!out.resize_dirty  (n)))
    प्रत्यागम मिच्छ;

  unsigned item_size = hb_static_size (T);
  जइ (k < 0)
    k = n - (-k) % n;
  अण्णहा
    k %= n;

  hb_memcpy ((शून्य *) out.arrayZ, (ध्रुव शून्य *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((शून्य *) (out.arrayZ + k), (ध्रुव शून्य *) org_array.arrayZ, (n - k) * item_size);
  प्रत्यागम सच्च;
}

स्थिर तार्किक rotate_set (ध्रुव hb_iup_set_t& org_set,
                        पूर्णाङ्क k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  जइ (!n) प्रत्यागम मिच्छ;
  k %= n;
  जइ (k < 0)
    k = n + k;

  जइ (k == 0)
  {
    out = org_set;
  }
  अण्णहा
  {
    प्रति (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  प्रत्यागम सच्च;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
स्थिर तार्किक _iup_segment (ध्रुव hb_array_t<ध्रुव contour_point_t> contour_points,
                          ध्रुव hb_array_t<ध्रुव पूर्णाङ्क> x_deltas,
                          ध्रुव hb_array_t<ध्रुव पूर्णाङ्क> y_deltas,
                          ध्रुव contour_point_t& p1, ध्रुव contour_point_t& p2,
                          पूर्णाङ्क p1_dx, पूर्णाङ्क p2_dx,
                          पूर्णाङ्क p1_dy, पूर्णाङ्क p2_dy,
			  द्विगुण tolerance_sq,
                          hb_vector_t<द्विगुण>& interp_x_deltas, /* OUT */
                          hb_vector_t<द्विगुण>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  जइ (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    प्रत्यागम मिच्छ;

  प्रति (unsigned j = 0; j < 2; j++)
  {
    भिन्न contour_point_t::* xp;
    द्विगुण x1, x2, d1, d2;
    ध्रुव पूर्णाङ्क *in;
    द्विगुण *out;
    जइ (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<द्विगुण> (p1.x);
      x2 = static_cast<द्विगुण> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    अण्णहा
    {
      xp = &contour_point_t::y;
      x1 = static_cast<द्विगुण> (p1.y);
      x2 = static_cast<द्विगुण> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    जइ (x1 == x2)
    {
      जइ (d1 == d2)
      {
        प्रति (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      अण्णहा
      {
        प्रति (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      अनुवर्त;
    }

    जइ (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    द्विगुण scale = (d2 - d1) / (x2 - x1);
    प्रति (unsigned i = 0; i < n; i++)
    {
      द्विगुण x = (द्विगुण) (contour_points.arrayZ[i].*xp);
      द्विगुण d;
      जइ (x <= x1)
        d = d1;
      अण्णहा जइ (x >= x2)
        d = d2;
      अण्णहा
        d = d1 + (x - x1) * scale;

      out[i] = d;
      द्विगुण err = d - in[i];
      जइ (err * err > tolerance_sq)
	प्रत्यागम मिच्छ;
    }
  }
  प्रत्यागम सच्च;
}

स्थिर तार्किक _can_iup_in_between (ध्रुव hb_array_t<ध्रुव contour_point_t> contour_points,
                                 ध्रुव hb_array_t<ध्रुव पूर्णाङ्क> x_deltas,
                                 ध्रुव hb_array_t<ध्रुव पूर्णाङ्क> y_deltas,
                                 ध्रुव contour_point_t& p1, ध्रुव contour_point_t& p2,
                                 पूर्णाङ्क p1_dx, पूर्णाङ्क p2_dx,
                                 पूर्णाङ्क p1_dy, पूर्णाङ्क p2_dy,
                                 द्विगुण tolerance_sq,
				 hb_vector_t<द्विगुण> &interp_x_deltas, /* scratch */
				 hb_vector_t<द्विगुण> &interp_y_deltas /* scratch */)
{
  जइ (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    प्रत्यागम मिच्छ;

  unsigned अङ्कः = contour_points.length;

  प्रति (unsigned i = 0; i < अङ्कः; i++)
  {
    द्विगुण dx = static_cast<द्विगुण> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    द्विगुण dy = static_cast<द्विगुण> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    जइ (dx * dx + dy * dy > tolerance_sq)
      प्रत्यागम मिच्छ;
  }
  प्रत्यागम सच्च;
}

स्थिर तार्किक _iup_contour_optimize_dp (ध्रुव contour_point_vector_t& contour_points,
                                      ध्रुव hb_vector_t<पूर्णाङ्क>& x_deltas,
                                      ध्रुव hb_vector_t<पूर्णाङ्क>& y_deltas,
                                      ध्रुव hb_iup_set_t& forced_set,
                                      द्विगुण tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<पूर्णाङ्क>& chain, /* OUT */
				      hb_vector_t<द्विगुण> &interp_x_deltas_scratch,
				      hb_vector_t<द्विगुण> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  जइ (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    प्रत्यागम मिच्छ;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  प्रति (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    जइ (i > 0 && forced_set.has (i - 1))
      अनुवर्त;

    पूर्णाङ्क lookback_index = hb_max ((पूर्णाङ्क) i - (पूर्णाङ्क) lookback + 1, -1);
    प्रति (पूर्णाङ्क j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned अङ्कः_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      जइ (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, अङ्कः_points),
                               x_deltas.as_array ().sub_array (j + 1, अङ्कः_points),
                               y_deltas.as_array ().sub_array (j + 1, अङ्कः_points),
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

      जइ (j > 0 && forced_set.has (j))
        भङ्ग;
    }
  }
  प्रत्यागम सच्च;
}

स्थिर तार्किक _iup_contour_optimize (ध्रुव hb_array_t<ध्रुव contour_point_t> contour_points,
                                   ध्रुव hb_array_t<ध्रुव पूर्णाङ्क> x_deltas,
                                   ध्रुव hb_array_t<ध्रुव पूर्णाङ्क> y_deltas,
                                   hb_array_t<तार्किक> opt_indices, /* OUT */
                                   द्विगुण tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  जइ (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    प्रत्यागम मिच्छ;

  जइ (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    प्रति (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = सच्च;
    प्रत्यागम सच्च;
  }

  तार्किक all_within_tolerance = सच्च;
  द्विगुण tolerance_sq = tolerance * tolerance;
  प्रति (unsigned i = 0; i < n; i++)
  {
    पूर्णाङ्क dx = x_deltas.arrayZ[i];
    पूर्णाङ्क dy = y_deltas.arrayZ[i];
    जइ ((द्विगुण) dx * dx + (द्विगुण) dy * dy > tolerance_sq)
    {
      all_within_tolerance = मिच्छ;
      भङ्ग;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  जइ (all_within_tolerance)
    प्रत्यागम सच्च;

  /* If there's exactly one point, return it */
  जइ (n == 1)
  {
    opt_indices.arrayZ[0] = सच्च;
    प्रत्यागम सच्च;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  तार्किक all_deltas_are_equal = सच्च;
  प्रति (unsigned i = 1; i < n; i++)
    जइ (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = मिच्छ;
      भङ्ग;
    }

  जइ (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = सच्च;
    प्रत्यागम सच्च;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<पूर्णाङ्क> &chain = scratch.chain.reset ();

  जइ (!forced_set.is_empty ())
  {
    पूर्णाङ्क k = n - 1 - forced_set.get_max ();
    जइ (k < 0)
      प्रत्यागम मिच्छ;

    hb_vector_t<पूर्णाङ्क> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<पूर्णाङ्क> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    जइ (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      प्रत्यागम मिच्छ;

    जइ (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      प्रत्यागम मिच्छ;

    hb_iup_set_t solution;
    पूर्णाङ्क index = n - 1;
    जाव (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    जइ (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      प्रत्यागम मिच्छ;

    प्रति (unsigned i : solution)
      opt_indices.arrayZ[i] = सच्च;

    hb_vector_t<तार्किक> &rot_indices = scratch.rot_indices.reset ();
    ध्रुव hb_array_t<ध्रुव तार्किक> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    जइ (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      प्रत्यागम मिच्छ;

    प्रति (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  अण्णहा
  {
    hb_vector_t<पूर्णाङ्क> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    जइ (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      प्रत्यागम मिच्छ;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    प्रति (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((शून्य *) repeat_x_deltas.arrayZ, (ध्रुव शून्य *) x_deltas.arrayZ, n * परिमाण (repeat_x_deltas[0]));
      hb_memcpy ((शून्य *) (repeat_x_deltas.arrayZ + n), (ध्रुव शून्य *) x_deltas.arrayZ, n * परिमाण (repeat_x_deltas[0]));

      hb_memcpy ((शून्य *) repeat_y_deltas.arrayZ, (ध्रुव शून्य *) y_deltas.arrayZ, n * परिमाण (repeat_x_deltas[0]));
      hb_memcpy ((शून्य *) (repeat_y_deltas.arrayZ + n), (ध्रुव शून्य *) y_deltas.arrayZ, n * परिमाण (repeat_x_deltas[0]));

      hb_memcpy ((शून्य *) repeat_points.arrayZ, (ध्रुव शून्य *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((शून्य *) (repeat_points.arrayZ + n), (ध्रुव शून्य *) contour_points.arrayZ, n * contour_point_size);
    }

    जइ (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      प्रत्यागम मिच्छ;

    unsigned best_cost = n + 1;
    पूर्णाङ्क len = costs.length;
    hb_iup_set_t best_sol;
    प्रति (पूर्णाङ्क start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      पूर्णाङ्क i = start;
      पूर्णाङ्क lookback = start - (पूर्णाङ्क) n;
      जाव (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      जइ (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        जइ (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    प्रति (unsigned i = 0; i < n; i++)
      जइ (best_sol.has (i))
        opt_indices.arrayZ[i] = सच्च;
  }
  प्रत्यागम सच्च;
}

तार्किक iup_delta_optimize (ध्रुव contour_point_vector_t& contour_points,
                         ध्रुव hb_vector_t<पूर्णाङ्क>& x_deltas,
                         ध्रुव hb_vector_t<पूर्णाङ्क>& y_deltas,
                         hb_vector_t<तार्किक>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         द्विगुण tolerance)
{
  जइ (!opt_indices.resize (contour_points.length))
      प्रत्यागम मिच्छ;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned गणनम् = contour_points.length;
  जइ (unlikely (!end_points.alloc (गणनम्)))
    प्रत्यागम मिच्छ;

  जइ (गणनम् < 4)
  {
    प्रति (unsigned i = 0; i < गणनम्; i++)
      end_points.push (i);
  }
  अण्णहा
  {
    प्रति (unsigned i = 0; i < गणनम् - 4; i++)
      जइ (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    प्रति (unsigned i = गणनम् - 4; i < गणनम्; i++)
      end_points.push (i);
  }

  जइ (end_points.in_error ()) प्रत्यागम मिच्छ;

  unsigned start = 0;
  प्रति (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    जइ (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      प्रत्यागम मिच्छ;
    start = end + 1;
  }
  प्रत्यागम सच्च;
}
