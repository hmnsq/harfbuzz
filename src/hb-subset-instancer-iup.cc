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

constexpr स्थिर अचिन्हित MAX_LOOKBACK = 8;

स्थिर शून्य _iup_contour_bound_forced_set (स्थिरांक hb_array_t<स्थिरांक contour_point_t> contour_points,
                                           स्थिरांक hb_array_t<स्थिरांक पूर्णांक> x_deltas,
                                           स्थिरांक hb_array_t<स्थिरांक पूर्णांक> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           द्विगुण tolerance = 0.0)
{
  अचिन्हित len = contour_points.length;
  अचिन्हित next_i = 0;
  साठी (पूर्णांक i = len - 1; i >= 0; i--)
  {
    अचिन्हित last_i = (len + i - 1) % len;
    साठी (अचिन्हित j = 0; j < 2; j++)
    {
      द्विगुण cj, lcj, ncj;
      द्विगुण dj, ldj, ndj;
      जर (j == 0)
      {
        cj = static_cast<द्विगुण> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<द्विगुण> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<द्विगुण> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      अन्यथा
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
      जर (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      अन्यथा
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      तार्किक force = असत्य;
      जर (c1 == c2)
      {
        जर (भ_असल (d1 - d2) > tolerance && भ_असल (dj) > tolerance)
          force = सत्य;
      }
      अन्यथा जर (c1 <= cj && cj <= c2)
      {
        जर (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = सत्य;
      }
      अन्यथा
      {
        जर (d1 != d2)
        {
          जर (cj < c1)
          {
            जर (भ_असल (dj) > tolerance &&
                भ_असल (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = सत्य;
          }
          अन्यथा
          {
            जर (भ_असल (dj) > tolerance &&
                भ_असल (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = सत्य;
          }
        }
      }

      जर (force)
      {
        forced_set.add (i);
        थांबा;
      }
    }
    next_i = i;
  }
}

ढाँचा <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
स्थिर तार्किक rotate_array (स्थिरांक hb_array_t<स्थिरांक T>& org_array,
                          पूर्णांक k,
                          hb_vector_t<T>& out)
{
  अचिन्हित n = org_array.length;
  जर (!n) परत सत्य;
  जर (unlikely (!out.resize_dirty  (n)))
    परत असत्य;

  अचिन्हित item_size = hb_static_size (T);
  जर (k < 0)
    k = n - (-k) % n;
  अन्यथा
    k %= n;

  hb_memcpy ((शून्य *) out.arrayZ, (स्थिरांक शून्य *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((शून्य *) (out.arrayZ + k), (स्थिरांक शून्य *) org_array.arrayZ, (n - k) * item_size);
  परत सत्य;
}

स्थिर तार्किक rotate_set (स्थिरांक hb_iup_set_t& org_set,
                        पूर्णांक k,
                        अचिन्हित n,
                        hb_iup_set_t& out)
{
  जर (!n) परत असत्य;
  k %= n;
  जर (k < 0)
    k = n + k;

  जर (k == 0)
  {
    out = org_set;
  }
  अन्यथा
  {
    साठी (अचिन्हित v : org_set)
      out.add ((v + k) % n);
  }
  परत सत्य;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
स्थिर तार्किक _iup_segment (स्थिरांक hb_array_t<स्थिरांक contour_point_t> contour_points,
                          स्थिरांक hb_array_t<स्थिरांक पूर्णांक> x_deltas,
                          स्थिरांक hb_array_t<स्थिरांक पूर्णांक> y_deltas,
                          स्थिरांक contour_point_t& p1, स्थिरांक contour_point_t& p2,
                          पूर्णांक p1_dx, पूर्णांक p2_dx,
                          पूर्णांक p1_dy, पूर्णांक p2_dy,
			  द्विगुण tolerance_sq,
                          hb_vector_t<द्विगुण>& interp_x_deltas, /* OUT */
                          hb_vector_t<द्विगुण>& interp_y_deltas /* OUT */)
{
  अचिन्हित n = contour_points.length;
  जर (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    परत असत्य;

  साठी (अचिन्हित j = 0; j < 2; j++)
  {
    दशांश contour_point_t::* xp;
    द्विगुण x1, x2, d1, d2;
    स्थिरांक पूर्णांक *in;
    द्विगुण *out;
    जर (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<द्विगुण> (p1.x);
      x2 = static_cast<द्विगुण> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    अन्यथा
    {
      xp = &contour_point_t::y;
      x1 = static_cast<द्विगुण> (p1.y);
      x2 = static_cast<द्विगुण> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    जर (x1 == x2)
    {
      जर (d1 == d2)
      {
        साठी (अचिन्हित i = 0; i < n; i++)
          out[i] = d1;
      }
      अन्यथा
      {
        साठी (अचिन्हित i = 0; i < n; i++)
          out[i] = 0.0;
      }
      सुरूठेवा;
    }

    जर (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    द्विगुण scale = (d2 - d1) / (x2 - x1);
    साठी (अचिन्हित i = 0; i < n; i++)
    {
      द्विगुण x = (द्विगुण) (contour_points.arrayZ[i].*xp);
      द्विगुण d;
      जर (x <= x1)
        d = d1;
      अन्यथा जर (x >= x2)
        d = d2;
      अन्यथा
        d = d1 + (x - x1) * scale;

      out[i] = d;
      द्विगुण err = d - in[i];
      जर (err * err > tolerance_sq)
	परत असत्य;
    }
  }
  परत सत्य;
}

स्थिर तार्किक _can_iup_in_between (स्थिरांक hb_array_t<स्थिरांक contour_point_t> contour_points,
                                 स्थिरांक hb_array_t<स्थिरांक पूर्णांक> x_deltas,
                                 स्थिरांक hb_array_t<स्थिरांक पूर्णांक> y_deltas,
                                 स्थिरांक contour_point_t& p1, स्थिरांक contour_point_t& p2,
                                 पूर्णांक p1_dx, पूर्णांक p2_dx,
                                 पूर्णांक p1_dy, पूर्णांक p2_dy,
                                 द्विगुण tolerance_sq,
				 hb_vector_t<द्विगुण> &interp_x_deltas, /* scratch */
				 hb_vector_t<द्विगुण> &interp_y_deltas /* scratch */)
{
  जर (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    परत असत्य;

  अचिन्हित अंक = contour_points.length;

  साठी (अचिन्हित i = 0; i < अंक; i++)
  {
    द्विगुण dx = static_cast<द्विगुण> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    द्विगुण dy = static_cast<द्विगुण> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    जर (dx * dx + dy * dy > tolerance_sq)
      परत असत्य;
  }
  परत सत्य;
}

स्थिर तार्किक _iup_contour_optimize_dp (स्थिरांक contour_point_vector_t& contour_points,
                                      स्थिरांक hb_vector_t<पूर्णांक>& x_deltas,
                                      स्थिरांक hb_vector_t<पूर्णांक>& y_deltas,
                                      स्थिरांक hb_iup_set_t& forced_set,
                                      द्विगुण tolerance_sq,
                                      अचिन्हित lookback,
                                      hb_vector_t<अचिन्हित>& costs, /* OUT */
                                      hb_vector_t<पूर्णांक>& chain, /* OUT */
				      hb_vector_t<द्विगुण> &interp_x_deltas_scratch,
				      hb_vector_t<द्विगुण> &interp_y_deltas_scratch)
{
  अचिन्हित n = contour_points.length;
  जर (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    परत असत्य;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  साठी (अचिन्हित i = 0; i < n; i++)
  {
    अचिन्हित best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    जर (i > 0 && forced_set.has (i - 1))
      सुरूठेवा;

    पूर्णांक lookback_index = hb_max ((पूर्णांक) i - (पूर्णांक) lookback + 1, -1);
    साठी (पूर्णांक j = i - 2; j >= lookback_index; j--)
    {
      अचिन्हित cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      अचिन्हित अंक_points = i - j - 1;
      अचिन्हित p1 = (j == -1 ? n - 1 : j);
      जर (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, अंक_points),
                               x_deltas.as_array ().sub_array (j + 1, अंक_points),
                               y_deltas.as_array ().sub_array (j + 1, अंक_points),
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

      जर (j > 0 && forced_set.has (j))
        थांबा;
    }
  }
  परत सत्य;
}

स्थिर तार्किक _iup_contour_optimize (स्थिरांक hb_array_t<स्थिरांक contour_point_t> contour_points,
                                   स्थिरांक hb_array_t<स्थिरांक पूर्णांक> x_deltas,
                                   स्थिरांक hb_array_t<स्थिरांक पूर्णांक> y_deltas,
                                   hb_array_t<तार्किक> opt_indices, /* OUT */
                                   द्विगुण tolerance,
				   iup_scratch_t &scratch)
{
  अचिन्हित n = contour_points.length;
  जर (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    परत असत्य;

  जर (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    साठी (अचिन्हित i = 0; i < n; i++)
      opt_indices.arrayZ[i] = सत्य;
    परत सत्य;
  }

  तार्किक all_within_tolerance = सत्य;
  द्विगुण tolerance_sq = tolerance * tolerance;
  साठी (अचिन्हित i = 0; i < n; i++)
  {
    पूर्णांक dx = x_deltas.arrayZ[i];
    पूर्णांक dy = y_deltas.arrayZ[i];
    जर ((द्विगुण) dx * dx + (द्विगुण) dy * dy > tolerance_sq)
    {
      all_within_tolerance = असत्य;
      थांबा;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  जर (all_within_tolerance)
    परत सत्य;

  /* If there's exactly one point, return it */
  जर (n == 1)
  {
    opt_indices.arrayZ[0] = सत्य;
    परत सत्य;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  तार्किक all_deltas_are_equal = सत्य;
  साठी (अचिन्हित i = 1; i < n; i++)
    जर (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = असत्य;
      थांबा;
    }

  जर (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = सत्य;
    परत सत्य;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<अचिन्हित> &costs = scratch.costs.reset ();
  hb_vector_t<पूर्णांक> &chain = scratch.chain.reset ();

  जर (!forced_set.is_empty ())
  {
    पूर्णांक k = n - 1 - forced_set.get_max ();
    जर (k < 0)
      परत असत्य;

    hb_vector_t<पूर्णांक> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<पूर्णांक> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    जर (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      परत असत्य;

    जर (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      परत असत्य;

    hb_iup_set_t solution;
    पूर्णांक index = n - 1;
    पर्यंत (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    जर (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      परत असत्य;

    साठी (अचिन्हित i : solution)
      opt_indices.arrayZ[i] = सत्य;

    hb_vector_t<तार्किक> &rot_indices = scratch.rot_indices.reset ();
    स्थिरांक hb_array_t<स्थिरांक तार्किक> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    जर (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      परत असत्य;

    साठी (अचिन्हित i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  अन्यथा
  {
    hb_vector_t<पूर्णांक> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    जर (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      परत असत्य;

    अचिन्हित contour_point_size = hb_static_size (contour_point_t);
    साठी (अचिन्हित i = 0; i < n; i++)
    {
      hb_memcpy ((शून्य *) repeat_x_deltas.arrayZ, (स्थिरांक शून्य *) x_deltas.arrayZ, n * आकार (repeat_x_deltas[0]));
      hb_memcpy ((शून्य *) (repeat_x_deltas.arrayZ + n), (स्थिरांक शून्य *) x_deltas.arrayZ, n * आकार (repeat_x_deltas[0]));

      hb_memcpy ((शून्य *) repeat_y_deltas.arrayZ, (स्थिरांक शून्य *) y_deltas.arrayZ, n * आकार (repeat_x_deltas[0]));
      hb_memcpy ((शून्य *) (repeat_y_deltas.arrayZ + n), (स्थिरांक शून्य *) y_deltas.arrayZ, n * आकार (repeat_x_deltas[0]));

      hb_memcpy ((शून्य *) repeat_points.arrayZ, (स्थिरांक शून्य *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((शून्य *) (repeat_points.arrayZ + n), (स्थिरांक शून्य *) contour_points.arrayZ, n * contour_point_size);
    }

    जर (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      परत असत्य;

    अचिन्हित best_cost = n + 1;
    पूर्णांक len = costs.length;
    hb_iup_set_t best_sol;
    साठी (पूर्णांक start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      पूर्णांक i = start;
      पूर्णांक lookback = start - (पूर्णांक) n;
      पर्यंत (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      जर (i == lookback)
      {
        अचिन्हित cost_i = i < 0 ? 0 : costs.arrayZ[i];
        अचिन्हित cost = costs.arrayZ[start] - cost_i;
        जर (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    साठी (अचिन्हित i = 0; i < n; i++)
      जर (best_sol.has (i))
        opt_indices.arrayZ[i] = सत्य;
  }
  परत सत्य;
}

तार्किक iup_delta_optimize (स्थिरांक contour_point_vector_t& contour_points,
                         स्थिरांक hb_vector_t<पूर्णांक>& x_deltas,
                         स्थिरांक hb_vector_t<पूर्णांक>& y_deltas,
                         hb_vector_t<तार्किक>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         द्विगुण tolerance)
{
  जर (!opt_indices.resize (contour_points.length))
      परत असत्य;

  hb_vector_t<अचिन्हित> &end_points = scratch.end_points.reset ();

  अचिन्हित मोजणी = contour_points.length;
  जर (unlikely (!end_points.alloc (मोजणी)))
    परत असत्य;

  जर (मोजणी < 4)
  {
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      end_points.push (i);
  }
  अन्यथा
  {
    साठी (अचिन्हित i = 0; i < मोजणी - 4; i++)
      जर (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    साठी (अचिन्हित i = मोजणी - 4; i < मोजणी; i++)
      end_points.push (i);
  }

  जर (end_points.in_error ()) परत असत्य;

  अचिन्हित start = 0;
  साठी (अचिन्हित end : end_points)
  {
    अचिन्हित len = end - start + 1;
    जर (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      परत असत्य;
    start = end + 1;
  }
  परत सत्य;
}
