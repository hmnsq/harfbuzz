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

#અન્તર્ભાવ "hb-subset-instancer-iup.hh"

#અન્તર્ભાવ "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr સ્થિર unsigned MAX_LOOKBACK = 8;

સ્થિર શૂન્ય _iup_contour_bound_forced_set (અચલ hb_array_t<અચલ contour_point_t> contour_points,
                                           અચલ hb_array_t<અચલ પૂર્ણાંક> x_deltas,
                                           અચલ hb_array_t<અચલ પૂર્ણાંક> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           દ્વિગુણ tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  માટે (પૂર્ણાંક i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    માટે (unsigned j = 0; j < 2; j++)
    {
      દ્વિગુણ cj, lcj, ncj;
      દ્વિગુણ dj, ldj, ndj;
      જો (j == 0)
      {
        cj = static_cast<દ્વિગુણ> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<દ્વિગુણ> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<દ્વિગુણ> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      નહીંતર
      {
        cj = static_cast<દ્વિગુણ> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<દ્વિગુણ> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<દ્વિગુણ> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      દ્વિગુણ c1, c2;
      દ્વિગુણ d1, d2;
      જો (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      નહીંતર
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      તાર્કિક force = ખોટું;
      જો (c1 == c2)
      {
        જો (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = સાચું;
      }
      નહીંતર જો (c1 <= cj && cj <= c2)
      {
        જો (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = સાચું;
      }
      નહીંતર
      {
        જો (d1 != d2)
        {
          જો (cj < c1)
          {
            જો (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = સાચું;
          }
          નહીંતર
          {
            જો (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = સાચું;
          }
        }
      }

      જો (force)
      {
        forced_set.add (i);
        તોડો;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
સ્થિર તાર્કિક rotate_array (અચલ hb_array_t<અચલ T>& org_array,
                          પૂર્ણાંક k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  જો (!n) પાછા સાચું;
  જો (unlikely (!out.resize_dirty  (n)))
    પાછા ખોટું;

  unsigned item_size = hb_static_size (T);
  જો (k < 0)
    k = n - (-k) % n;
  નહીંતર
    k %= n;

  hb_memcpy ((શૂન્ય *) out.arrayZ, (અચલ શૂન્ય *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((શૂન્ય *) (out.arrayZ + k), (અચલ શૂન્ય *) org_array.arrayZ, (n - k) * item_size);
  પાછા સાચું;
}

સ્થિર તાર્કિક rotate_set (અચલ hb_iup_set_t& org_set,
                        પૂર્ણાંક k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  જો (!n) પાછા ખોટું;
  k %= n;
  જો (k < 0)
    k = n + k;

  જો (k == 0)
  {
    out = org_set;
  }
  નહીંતર
  {
    માટે (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  પાછા સાચું;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
સ્થિર તાર્કિક _iup_segment (અચલ hb_array_t<અચલ contour_point_t> contour_points,
                          અચલ hb_array_t<અચલ પૂર્ણાંક> x_deltas,
                          અચલ hb_array_t<અચલ પૂર્ણાંક> y_deltas,
                          અચલ contour_point_t& p1, અચલ contour_point_t& p2,
                          પૂર્ણાંક p1_dx, પૂર્ણાંક p2_dx,
                          પૂર્ણાંક p1_dy, પૂર્ણાંક p2_dy,
			  દ્વિગુણ tolerance_sq,
                          hb_vector_t<દ્વિગુણ>& interp_x_deltas, /* OUT */
                          hb_vector_t<દ્વિગુણ>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  જો (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    પાછા ખોટું;

  માટે (unsigned j = 0; j < 2; j++)
  {
    દશાંશ contour_point_t::* xp;
    દ્વિગુણ x1, x2, d1, d2;
    અચલ પૂર્ણાંક *in;
    દ્વિગુણ *out;
    જો (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<દ્વિગુણ> (p1.x);
      x2 = static_cast<દ્વિગુણ> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    નહીંતર
    {
      xp = &contour_point_t::y;
      x1 = static_cast<દ્વિગુણ> (p1.y);
      x2 = static_cast<દ્વિગુણ> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    જો (x1 == x2)
    {
      જો (d1 == d2)
      {
        માટે (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      નહીંતર
      {
        માટે (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      ચાલુ;
    }

    જો (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    દ્વિગુણ scale = (d2 - d1) / (x2 - x1);
    માટે (unsigned i = 0; i < n; i++)
    {
      દ્વિગુણ x = (દ્વિગુણ) (contour_points.arrayZ[i].*xp);
      દ્વિગુણ d;
      જો (x <= x1)
        d = d1;
      નહીંતર જો (x >= x2)
        d = d2;
      નહીંતર
        d = d1 + (x - x1) * scale;

      out[i] = d;
      દ્વિગુણ err = d - in[i];
      જો (err * err > tolerance_sq)
	પાછા ખોટું;
    }
  }
  પાછા સાચું;
}

સ્થિર તાર્કિક _can_iup_in_between (અચલ hb_array_t<અચલ contour_point_t> contour_points,
                                 અચલ hb_array_t<અચલ પૂર્ણાંક> x_deltas,
                                 અચલ hb_array_t<અચલ પૂર્ણાંક> y_deltas,
                                 અચલ contour_point_t& p1, અચલ contour_point_t& p2,
                                 પૂર્ણાંક p1_dx, પૂર્ણાંક p2_dx,
                                 પૂર્ણાંક p1_dy, પૂર્ણાંક p2_dy,
                                 દ્વિગુણ tolerance_sq,
				 hb_vector_t<દ્વિગુણ> &interp_x_deltas, /* scratch */
				 hb_vector_t<દ્વિગુણ> &interp_y_deltas /* scratch */)
{
  જો (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    પાછા ખોટું;

  unsigned અંક = contour_points.length;

  માટે (unsigned i = 0; i < અંક; i++)
  {
    દ્વિગુણ dx = static_cast<દ્વિગુણ> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    દ્વિગુણ dy = static_cast<દ્વિગુણ> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    જો (dx * dx + dy * dy > tolerance_sq)
      પાછા ખોટું;
  }
  પાછા સાચું;
}

સ્થિર તાર્કિક _iup_contour_optimize_dp (અચલ contour_point_vector_t& contour_points,
                                      અચલ hb_vector_t<પૂર્ણાંક>& x_deltas,
                                      અચલ hb_vector_t<પૂર્ણાંક>& y_deltas,
                                      અચલ hb_iup_set_t& forced_set,
                                      દ્વિગુણ tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<પૂર્ણાંક>& chain, /* OUT */
				      hb_vector_t<દ્વિગુણ> &interp_x_deltas_scratch,
				      hb_vector_t<દ્વિગુણ> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  જો (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    પાછા ખોટું;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  માટે (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    જો (i > 0 && forced_set.has (i - 1))
      ચાલુ;

    પૂર્ણાંક lookback_index = hb_max ((પૂર્ણાંક) i - (પૂર્ણાંક) lookback + 1, -1);
    માટે (પૂર્ણાંક j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned અંક_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      જો (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, અંક_points),
                               x_deltas.as_array ().sub_array (j + 1, અંક_points),
                               y_deltas.as_array ().sub_array (j + 1, અંક_points),
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

      જો (j > 0 && forced_set.has (j))
        તોડો;
    }
  }
  પાછા સાચું;
}

સ્થિર તાર્કિક _iup_contour_optimize (અચલ hb_array_t<અચલ contour_point_t> contour_points,
                                   અચલ hb_array_t<અચલ પૂર્ણાંક> x_deltas,
                                   અચલ hb_array_t<અચલ પૂર્ણાંક> y_deltas,
                                   hb_array_t<તાર્કિક> opt_indices, /* OUT */
                                   દ્વિગુણ tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  જો (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    પાછા ખોટું;

  જો (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    માટે (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = સાચું;
    પાછા સાચું;
  }

  તાર્કિક all_within_tolerance = સાચું;
  દ્વિગુણ tolerance_sq = tolerance * tolerance;
  માટે (unsigned i = 0; i < n; i++)
  {
    પૂર્ણાંક dx = x_deltas.arrayZ[i];
    પૂર્ણાંક dy = y_deltas.arrayZ[i];
    જો ((દ્વિગુણ) dx * dx + (દ્વિગુણ) dy * dy > tolerance_sq)
    {
      all_within_tolerance = ખોટું;
      તોડો;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  જો (all_within_tolerance)
    પાછા સાચું;

  /* If there's exactly one point, return it */
  જો (n == 1)
  {
    opt_indices.arrayZ[0] = સાચું;
    પાછા સાચું;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  તાર્કિક all_deltas_are_equal = સાચું;
  માટે (unsigned i = 1; i < n; i++)
    જો (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = ખોટું;
      તોડો;
    }

  જો (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = સાચું;
    પાછા સાચું;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<પૂર્ણાંક> &chain = scratch.chain.reset ();

  જો (!forced_set.is_empty ())
  {
    પૂર્ણાંક k = n - 1 - forced_set.get_max ();
    જો (k < 0)
      પાછા ખોટું;

    hb_vector_t<પૂર્ણાંક> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<પૂર્ણાંક> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    જો (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      પાછા ખોટું;

    જો (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      પાછા ખોટું;

    hb_iup_set_t solution;
    પૂર્ણાંક index = n - 1;
    જ્યારે (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    જો (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      પાછા ખોટું;

    માટે (unsigned i : solution)
      opt_indices.arrayZ[i] = સાચું;

    hb_vector_t<તાર્કિક> &rot_indices = scratch.rot_indices.reset ();
    અચલ hb_array_t<અચલ તાર્કિક> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    જો (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      પાછા ખોટું;

    માટે (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  નહીંતર
  {
    hb_vector_t<પૂર્ણાંક> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    જો (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      પાછા ખોટું;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    માટે (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((શૂન્ય *) repeat_x_deltas.arrayZ, (અચલ શૂન્ય *) x_deltas.arrayZ, n * કદ (repeat_x_deltas[0]));
      hb_memcpy ((શૂન્ય *) (repeat_x_deltas.arrayZ + n), (અચલ શૂન્ય *) x_deltas.arrayZ, n * કદ (repeat_x_deltas[0]));

      hb_memcpy ((શૂન્ય *) repeat_y_deltas.arrayZ, (અચલ શૂન્ય *) y_deltas.arrayZ, n * કદ (repeat_x_deltas[0]));
      hb_memcpy ((શૂન્ય *) (repeat_y_deltas.arrayZ + n), (અચલ શૂન્ય *) y_deltas.arrayZ, n * કદ (repeat_x_deltas[0]));

      hb_memcpy ((શૂન્ય *) repeat_points.arrayZ, (અચલ શૂન્ય *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((શૂન્ય *) (repeat_points.arrayZ + n), (અચલ શૂન્ય *) contour_points.arrayZ, n * contour_point_size);
    }

    જો (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      પાછા ખોટું;

    unsigned best_cost = n + 1;
    પૂર્ણાંક len = costs.length;
    hb_iup_set_t best_sol;
    માટે (પૂર્ણાંક start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      પૂર્ણાંક i = start;
      પૂર્ણાંક lookback = start - (પૂર્ણાંક) n;
      જ્યારે (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      જો (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        જો (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    માટે (unsigned i = 0; i < n; i++)
      જો (best_sol.has (i))
        opt_indices.arrayZ[i] = સાચું;
  }
  પાછા સાચું;
}

તાર્કિક iup_delta_optimize (અચલ contour_point_vector_t& contour_points,
                         અચલ hb_vector_t<પૂર્ણાંક>& x_deltas,
                         અચલ hb_vector_t<પૂર્ણાંક>& y_deltas,
                         hb_vector_t<તાર્કિક>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         દ્વિગુણ tolerance)
{
  જો (!opt_indices.resize (contour_points.length))
      પાછા ખોટું;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned ગણતરી = contour_points.length;
  જો (unlikely (!end_points.alloc (ગણતરી)))
    પાછા ખોટું;

  જો (ગણતરી < 4)
  {
    માટે (unsigned i = 0; i < ગણતરી; i++)
      end_points.push (i);
  }
  નહીંતર
  {
    માટે (unsigned i = 0; i < ગણતરી - 4; i++)
      જો (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    માટે (unsigned i = ગણતરી - 4; i < ગણતરી; i++)
      end_points.push (i);
  }

  જો (end_points.in_error ()) પાછા ખોટું;

  unsigned start = 0;
  માટે (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    જો (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      પાછા ખોટું;
    start = end + 1;
  }
  પાછા સાચું;
}
