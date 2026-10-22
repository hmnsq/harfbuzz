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

#समावेश "hb-subset-instancer-iup.hh"

#समावेश "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr अटल अचिन्हित MAX_LOOKBACK = 8;

अटल व्योम _iup_contour_bound_forced_set (स्थिर hb_array_t<स्थिर contour_point_t> contour_points,
                                           स्थिर hb_array_t<स्थिर पूर्णांक> x_deltas,
                                           स्थिर hb_array_t<स्थिर पूर्णांक> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           द्विगुन tolerance = 0.0)
{
  अचिन्हित len = contour_points.लंबाई;
  अचिन्हित अग्र_i = 0;
  क्रम (पूर्णांक i = len - 1; i >= 0; i--)
  {
    अचिन्हित last_i = (len + i - 1) % len;
    क्रम (अचिन्हित j = 0; j < 2; j++)
    {
      द्विगुन cj, lcj, ncj;
      द्विगुन dj, ldj, ndj;
      अगर (j == 0)
      {
        cj = static_cast<द्विगुन> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<द्विगुन> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<द्विगुन> (contour_points.arrayZ[अग्र_i].x);
        ndj = x_deltas.arrayZ[अग्र_i];
      }
      अन्यथा
      {
        cj = static_cast<द्विगुन> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<द्विगुन> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<द्विगुन> (contour_points.arrayZ[अग्र_i].y);
        ndj = y_deltas.arrayZ[अग्र_i];
      }

      द्विगुन c1, c2;
      द्विगुन d1, d2;
      अगर (lcj <= ncj)
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

      bool force = false;
      अगर (c1 == c2)
      {
        अगर (भ_असल (d1 - d2) > tolerance && भ_असल (dj) > tolerance)
          force = true;
      }
      अन्यथा अगर (c1 <= cj && cj <= c2)
      {
        अगर (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = true;
      }
      अन्यथा
      {
        अगर (d1 != d2)
        {
          अगर (cj < c1)
          {
            अगर (भ_असल (dj) > tolerance &&
                भ_असल (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = true;
          }
          अन्यथा
          {
            अगर (भ_असल (dj) > tolerance &&
                भ_असल (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = true;
          }
        }
      }

      अगर (force)
      {
        forced_set.add (i);
        अवरोध;
      }
    }
    अग्र_i = i;
  }
}

ढाँचा <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
अटल bool rotate_array (स्थिर hb_array_t<स्थिर T>& org_array,
                          पूर्णांक k,
                          hb_vector_t<T>& out)
{
  अचिन्हित n = org_array.लंबाई;
  अगर (!n) वापस true;
  अगर (unlikely (!out.resize_dirty  (n)))
    वापस false;

  अचिन्हित item_आकार = hb_static_आकार (T);
  अगर (k < 0)
    k = n - (-k) % n;
  अन्यथा
    k %= n;

  hb_memcpy ((व्योम *) out.arrayZ, (स्थिर व्योम *) (org_array.arrayZ + n - k), k * item_आकार);
  hb_memcpy ((व्योम *) (out.arrayZ + k), (स्थिर व्योम *) org_array.arrayZ, (n - k) * item_आकार);
  वापस true;
}

अटल bool rotate_set (स्थिर hb_iup_set_t& org_set,
                        पूर्णांक k,
                        अचिन्हित n,
                        hb_iup_set_t& out)
{
  अगर (!n) वापस false;
  k %= n;
  अगर (k < 0)
    k = n + k;

  अगर (k == 0)
  {
    out = org_set;
  }
  अन्यथा
  {
    क्रम (अचिन्हित v : org_set)
      out.add ((v + k) % n);
  }
  वापस true;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
अटल bool _iup_segment (स्थिर hb_array_t<स्थिर contour_point_t> contour_points,
                          स्थिर hb_array_t<स्थिर पूर्णांक> x_deltas,
                          स्थिर hb_array_t<स्थिर पूर्णांक> y_deltas,
                          स्थिर contour_point_t& p1, स्थिर contour_point_t& p2,
                          पूर्णांक p1_dx, पूर्णांक p2_dx,
                          पूर्णांक p1_dy, पूर्णांक p2_dy,
			  द्विगुन tolerance_sq,
                          hb_vector_t<द्विगुन>& interp_x_deltas, /* OUT */
                          hb_vector_t<द्विगुन>& interp_y_deltas /* OUT */)
{
  अचिन्हित n = contour_points.लंबाई;
  अगर (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    वापस false;

  क्रम (अचिन्हित j = 0; j < 2; j++)
  {
    भग्न contour_point_t::* xp;
    द्विगुन x1, x2, d1, d2;
    स्थिर पूर्णांक *in;
    द्विगुन *out;
    अगर (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<द्विगुन> (p1.x);
      x2 = static_cast<द्विगुन> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    अन्यथा
    {
      xp = &contour_point_t::y;
      x1 = static_cast<द्विगुन> (p1.y);
      x2 = static_cast<द्विगुन> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    अगर (x1 == x2)
    {
      अगर (d1 == d2)
      {
        क्रम (अचिन्हित i = 0; i < n; i++)
          out[i] = d1;
      }
      अन्यथा
      {
        क्रम (अचिन्हित i = 0; i < n; i++)
          out[i] = 0.0;
      }
      जारी;
    }

    अगर (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    द्विगुन scale = (d2 - d1) / (x2 - x1);
    क्रम (अचिन्हित i = 0; i < n; i++)
    {
      द्विगुन x = (द्विगुन) (contour_points.arrayZ[i].*xp);
      द्विगुन d;
      अगर (x <= x1)
        d = d1;
      अन्यथा अगर (x >= x2)
        d = d2;
      अन्यथा
        d = d1 + (x - x1) * scale;

      out[i] = d;
      द्विगुन err = d - in[i];
      अगर (err * err > tolerance_sq)
	वापस false;
    }
  }
  वापस true;
}

अटल bool _can_iup_in_between (स्थिर hb_array_t<स्थिर contour_point_t> contour_points,
                                 स्थिर hb_array_t<स्थिर पूर्णांक> x_deltas,
                                 स्थिर hb_array_t<स्थिर पूर्णांक> y_deltas,
                                 स्थिर contour_point_t& p1, स्थिर contour_point_t& p2,
                                 पूर्णांक p1_dx, पूर्णांक p2_dx,
                                 पूर्णांक p1_dy, पूर्णांक p2_dy,
                                 द्विगुन tolerance_sq,
				 hb_vector_t<द्विगुन> &interp_x_deltas, /* scratch */
				 hb_vector_t<द्विगुन> &interp_y_deltas /* scratch */)
{
  अगर (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    वापस false;

  अचिन्हित अंक = contour_points.लंबाई;

  क्रम (अचिन्हित i = 0; i < अंक; i++)
  {
    द्विगुन dx = static_cast<द्विगुन> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    द्विगुन dy = static_cast<द्विगुन> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    अगर (dx * dx + dy * dy > tolerance_sq)
      वापस false;
  }
  वापस true;
}

अटल bool _iup_contour_optimize_dp (स्थिर contour_point_vector_t& contour_points,
                                      स्थिर hb_vector_t<पूर्णांक>& x_deltas,
                                      स्थिर hb_vector_t<पूर्णांक>& y_deltas,
                                      स्थिर hb_iup_set_t& forced_set,
                                      द्विगुन tolerance_sq,
                                      अचिन्हित lookback,
                                      hb_vector_t<अचिन्हित>& costs, /* OUT */
                                      hb_vector_t<पूर्णांक>& chain, /* OUT */
				      hb_vector_t<द्विगुन> &interp_x_deltas_scratch,
				      hb_vector_t<द्विगुन> &interp_y_deltas_scratch)
{
  अचिन्हित n = contour_points.लंबाई;
  अगर (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    वापस false;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  क्रम (अचिन्हित i = 0; i < n; i++)
  {
    अचिन्हित best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    अगर (i > 0 && forced_set.has (i - 1))
      जारी;

    पूर्णांक lookback_सूचकांक = hb_max ((पूर्णांक) i - (पूर्णांक) lookback + 1, -1);
    क्रम (पूर्णांक j = i - 2; j >= lookback_सूचकांक; j--)
    {
      अचिन्हित cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      अचिन्हित अंक_points = i - j - 1;
      अचिन्हित p1 = (j == -1 ? n - 1 : j);
      अगर (cost < best_cost &&
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

      अगर (j > 0 && forced_set.has (j))
        अवरोध;
    }
  }
  वापस true;
}

अटल bool _iup_contour_optimize (स्थिर hb_array_t<स्थिर contour_point_t> contour_points,
                                   स्थिर hb_array_t<स्थिर पूर्णांक> x_deltas,
                                   स्थिर hb_array_t<स्थिर पूर्णांक> y_deltas,
                                   hb_array_t<bool> opt_indices, /* OUT */
                                   द्विगुन tolerance,
				   iup_scratch_t &scratch)
{
  अचिन्हित n = contour_points.लंबाई;
  अगर (opt_indices.लंबाई != n ||
      x_deltas.लंबाई != n ||
      y_deltas.लंबाई != n)
    वापस false;

  अगर (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    क्रम (अचिन्हित i = 0; i < n; i++)
      opt_indices.arrayZ[i] = true;
    वापस true;
  }

  bool all_within_tolerance = true;
  द्विगुन tolerance_sq = tolerance * tolerance;
  क्रम (अचिन्हित i = 0; i < n; i++)
  {
    पूर्णांक dx = x_deltas.arrayZ[i];
    पूर्णांक dy = y_deltas.arrayZ[i];
    अगर ((द्विगुन) dx * dx + (द्विगुन) dy * dy > tolerance_sq)
    {
      all_within_tolerance = false;
      अवरोध;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  अगर (all_within_tolerance)
    वापस true;

  /* If there's exactly one point, return it */
  अगर (n == 1)
  {
    opt_indices.arrayZ[0] = true;
    वापस true;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  bool all_deltas_are_equal = true;
  क्रम (अचिन्हित i = 1; i < n; i++)
    अगर (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = false;
      अवरोध;
    }

  अगर (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = true;
    वापस true;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<अचिन्हित> &costs = scratch.costs.reset ();
  hb_vector_t<पूर्णांक> &chain = scratch.chain.reset ();

  अगर (!forced_set.is_empty ())
  {
    पूर्णांक k = n - 1 - forced_set.get_max ();
    अगर (k < 0)
      वापस false;

    hb_vector_t<पूर्णांक> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<पूर्णांक> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    अगर (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      वापस false;

    अगर (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      वापस false;

    hb_iup_set_t solution;
    पूर्णांक सूचकांक_2 = n - 1;
    जबतक (सूचकांक_2 != -1)
    {
      solution.add (सूचकांक_2);
      सूचकांक_2 = chain.arrayZ[सूचकांक_2];
    }

    अगर (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      वापस false;

    क्रम (अचिन्हित i : solution)
      opt_indices.arrayZ[i] = true;

    hb_vector_t<bool> &rot_indices = scratch.rot_indices.reset ();
    स्थिर hb_array_t<स्थिर bool> opt_indices_array (opt_indices.arrayZ, opt_indices.लंबाई);
    अगर (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      वापस false;

    क्रम (अचिन्हित i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  अन्यथा
  {
    hb_vector_t<पूर्णांक> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    अगर (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      वापस false;

    अचिन्हित contour_point_आकार = hb_static_आकार (contour_point_t);
    क्रम (अचिन्हित i = 0; i < n; i++)
    {
      hb_memcpy ((व्योम *) repeat_x_deltas.arrayZ, (स्थिर व्योम *) x_deltas.arrayZ, n * माप (repeat_x_deltas[0]));
      hb_memcpy ((व्योम *) (repeat_x_deltas.arrayZ + n), (स्थिर व्योम *) x_deltas.arrayZ, n * माप (repeat_x_deltas[0]));

      hb_memcpy ((व्योम *) repeat_y_deltas.arrayZ, (स्थिर व्योम *) y_deltas.arrayZ, n * माप (repeat_x_deltas[0]));
      hb_memcpy ((व्योम *) (repeat_y_deltas.arrayZ + n), (स्थिर व्योम *) y_deltas.arrayZ, n * माप (repeat_x_deltas[0]));

      hb_memcpy ((व्योम *) repeat_points.arrayZ, (स्थिर व्योम *) contour_points.arrayZ, n * contour_point_आकार);
      hb_memcpy ((व्योम *) (repeat_points.arrayZ + n), (स्थिर व्योम *) contour_points.arrayZ, n * contour_point_आकार);
    }

    अगर (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      वापस false;

    अचिन्हित best_cost = n + 1;
    पूर्णांक len = costs.लंबाई;
    hb_iup_set_t best_sol;
    क्रम (पूर्णांक start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      पूर्णांक i = start;
      पूर्णांक lookback = start - (पूर्णांक) n;
      जबतक (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      अगर (i == lookback)
      {
        अचिन्हित cost_i = i < 0 ? 0 : costs.arrayZ[i];
        अचिन्हित cost = costs.arrayZ[start] - cost_i;
        अगर (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    क्रम (अचिन्हित i = 0; i < n; i++)
      अगर (best_sol.has (i))
        opt_indices.arrayZ[i] = true;
  }
  वापस true;
}

bool iup_delta_optimize (स्थिर contour_point_vector_t& contour_points,
                         स्थिर hb_vector_t<पूर्णांक>& x_deltas,
                         स्थिर hb_vector_t<पूर्णांक>& y_deltas,
                         hb_vector_t<bool>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         द्विगुन tolerance)
{
  अगर (!opt_indices.resize (contour_points.लंबाई))
      वापस false;

  hb_vector_t<अचिन्हित> &end_points = scratch.end_points.reset ();

  अचिन्हित गिनती = contour_points.लंबाई;
  अगर (unlikely (!end_points.alloc (गिनती)))
    वापस false;

  अगर (गिनती < 4)
  {
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      end_points.push (i);
  }
  अन्यथा
  {
    क्रम (अचिन्हित i = 0; i < गिनती - 4; i++)
      अगर (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    क्रम (अचिन्हित i = गिनती - 4; i < गिनती; i++)
      end_points.push (i);
  }

  अगर (end_points.in_error ()) वापस false;

  अचिन्हित start = 0;
  क्रम (अचिन्हित end : end_points)
  {
    अचिन्हित len = end - start + 1;
    अगर (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      वापस false;
    start = end + 1;
  }
  वापस true;
}
