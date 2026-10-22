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

#ضمن "hb-subset-instancer-iup.hh"

#ضمن "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ساكن unsigned MAX_LOOKBACK = 8;

ساكن فراغ _iup_contour_bound_forced_set (ثابت hb_array_t<ثابت contour_point_t> contour_points,
                                           ثابت hb_array_t<ثابت صحيح> x_deltas,
                                           ثابت hb_array_t<ثابت صحيح> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           مزدوج tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  עבור (صحيح i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    עבור (unsigned j = 0; j < 2; j++)
    {
      مزدوج cj, lcj, ncj;
      مزدوج dj, ldj, ndj;
      אם (j == 0)
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      אחרת
      {
        cj = static_cast<مزدوج> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<مزدوج> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<مزدوج> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      مزدوج c1, c2;
      مزدوج d1, d2;
      אם (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      אחרת
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      منطقي force = שקר;
      אם (c1 == c2)
      {
        אם (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = אמת;
      }
      אחרת אם (c1 <= cj && cj <= c2)
      {
        אם (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = אמת;
      }
      אחרת
      {
        אם (d1 != d2)
        {
          אם (cj < c1)
          {
            אם (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = אמת;
          }
          אחרת
          {
            אם (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = אמת;
          }
        }
      }

      אם (force)
      {
        forced_set.add (i);
        اكسر;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ساكن منطقي rotate_array (ثابت hb_array_t<ثابت T>& org_array,
                          صحيح k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  אם (!n) החזר אמת;
  אם (unlikely (!out.resize_dirty  (n)))
    החזר שקר;

  unsigned item_size = hb_static_size (T);
  אם (k < 0)
    k = n - (-k) % n;
  אחרת
    k %= n;

  hb_memcpy ((فراغ *) out.arrayZ, (ثابت فراغ *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((فراغ *) (out.arrayZ + k), (ثابت فراغ *) org_array.arrayZ, (n - k) * item_size);
  החזר אמת;
}

ساكن منطقي rotate_set (ثابت hb_iup_set_t& org_set,
                        صحيح k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  אם (!n) החזר שקר;
  k %= n;
  אם (k < 0)
    k = n + k;

  אם (k == 0)
  {
    out = org_set;
  }
  אחרת
  {
    עבור (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  החזר אמת;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ساكن منطقي _iup_segment (ثابت hb_array_t<ثابت contour_point_t> contour_points,
                          ثابت hb_array_t<ثابت صحيح> x_deltas,
                          ثابت hb_array_t<ثابت صحيح> y_deltas,
                          ثابت contour_point_t& p1, ثابت contour_point_t& p2,
                          صحيح p1_dx, صحيح p2_dx,
                          صحيح p1_dy, صحيح p2_dy,
			  مزدوج tolerance_sq,
                          hb_vector_t<مزدوج>& interp_x_deltas, /* OUT */
                          hb_vector_t<مزدوج>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  אם (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    החזר שקר;

  עבור (unsigned j = 0; j < 2; j++)
  {
    عائم contour_point_t::* xp;
    مزدوج x1, x2, d1, d2;
    ثابت صحيح *in;
    مزدوج *out;
    אם (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<مزدوج> (p1.x);
      x2 = static_cast<مزدوج> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    אחרת
    {
      xp = &contour_point_t::y;
      x1 = static_cast<مزدوج> (p1.y);
      x2 = static_cast<مزدوج> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    אם (x1 == x2)
    {
      אם (d1 == d2)
      {
        עבור (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      אחרת
      {
        עבור (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      استمر;
    }

    אם (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    مزدوج scale = (d2 - d1) / (x2 - x1);
    עבור (unsigned i = 0; i < n; i++)
    {
      مزدوج x = (مزدوج) (contour_points.arrayZ[i].*xp);
      مزدوج d;
      אם (x <= x1)
        d = d1;
      אחרת אם (x >= x2)
        d = d2;
      אחרת
        d = d1 + (x - x1) * scale;

      out[i] = d;
      مزدوج err = d - in[i];
      אם (err * err > tolerance_sq)
	החזר שקר;
    }
  }
  החזר אמת;
}

ساكن منطقي _can_iup_in_between (ثابت hb_array_t<ثابت contour_point_t> contour_points,
                                 ثابت hb_array_t<ثابت صحيح> x_deltas,
                                 ثابت hb_array_t<ثابت صحيح> y_deltas,
                                 ثابت contour_point_t& p1, ثابت contour_point_t& p2,
                                 صحيح p1_dx, صحيح p2_dx,
                                 صحيح p1_dy, صحيح p2_dy,
                                 مزدوج tolerance_sq,
				 hb_vector_t<مزدوج> &interp_x_deltas, /* scratch */
				 hb_vector_t<مزدوج> &interp_y_deltas /* scratch */)
{
  אם (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    החזר שקר;

  unsigned num = contour_points.length;

  עבור (unsigned i = 0; i < num; i++)
  {
    مزدوج dx = static_cast<مزدوج> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    مزدوج dy = static_cast<مزدوج> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    אם (dx * dx + dy * dy > tolerance_sq)
      החזר שקר;
  }
  החזר אמת;
}

ساكن منطقي _iup_contour_optimize_dp (ثابت contour_point_vector_t& contour_points,
                                      ثابت hb_vector_t<صحيح>& x_deltas,
                                      ثابت hb_vector_t<صحيح>& y_deltas,
                                      ثابت hb_iup_set_t& forced_set,
                                      مزدوج tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<صحيح>& chain, /* OUT */
				      hb_vector_t<مزدوج> &interp_x_deltas_scratch,
				      hb_vector_t<مزدوج> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  אם (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    החזר שקר;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  עבור (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    אם (i > 0 && forced_set.has (i - 1))
      استمر;

    صحيح lookback_index = hb_max ((صحيح) i - (صحيح) lookback + 1, -1);
    עבור (صحيح j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned num_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      אם (cost < best_cost &&
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

      אם (j > 0 && forced_set.has (j))
        اكسر;
    }
  }
  החזר אמת;
}

ساكن منطقي _iup_contour_optimize (ثابت hb_array_t<ثابت contour_point_t> contour_points,
                                   ثابت hb_array_t<ثابت صحيح> x_deltas,
                                   ثابت hb_array_t<ثابت صحيح> y_deltas,
                                   hb_array_t<منطقي> opt_indices, /* OUT */
                                   مزدوج tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  אם (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    החזר שקר;

  אם (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    עבור (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = אמת;
    החזר אמת;
  }

  منطقي all_within_tolerance = אמת;
  مزدوج tolerance_sq = tolerance * tolerance;
  עבור (unsigned i = 0; i < n; i++)
  {
    صحيح dx = x_deltas.arrayZ[i];
    صحيح dy = y_deltas.arrayZ[i];
    אם ((مزدوج) dx * dx + (مزدوج) dy * dy > tolerance_sq)
    {
      all_within_tolerance = שקר;
      اكسر;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  אם (all_within_tolerance)
    החזר אמת;

  /* If there's exactly one point, return it */
  אם (n == 1)
  {
    opt_indices.arrayZ[0] = אמת;
    החזר אמת;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  منطقي all_deltas_are_equal = אמת;
  עבור (unsigned i = 1; i < n; i++)
    אם (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = שקר;
      اكسر;
    }

  אם (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = אמת;
    החזר אמת;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<صحيح> &chain = scratch.chain.reset ();

  אם (!forced_set.is_empty ())
  {
    صحيح k = n - 1 - forced_set.get_max ();
    אם (k < 0)
      החזר שקר;

    hb_vector_t<صحيح> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<صحيح> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    אם (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      החזר שקר;

    אם (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      החזר שקר;

    hb_iup_set_t solution;
    صحيح index = n - 1;
    כלעוד (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    אם (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      החזר שקר;

    עבור (unsigned i : solution)
      opt_indices.arrayZ[i] = אמת;

    hb_vector_t<منطقي> &rot_indices = scratch.rot_indices.reset ();
    ثابت hb_array_t<ثابت منطقي> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    אם (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      החזר שקר;

    עבור (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  אחרת
  {
    hb_vector_t<صحيح> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    אם (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      החזר שקר;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    עבור (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((فراغ *) repeat_x_deltas.arrayZ, (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_x_deltas.arrayZ + n), (ثابت فراغ *) x_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_y_deltas.arrayZ, (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));
      hb_memcpy ((فراغ *) (repeat_y_deltas.arrayZ + n), (ثابت فراغ *) y_deltas.arrayZ, n * حجم (repeat_x_deltas[0]));

      hb_memcpy ((فراغ *) repeat_points.arrayZ, (ثابت فراغ *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((فراغ *) (repeat_points.arrayZ + n), (ثابت فراغ *) contour_points.arrayZ, n * contour_point_size);
    }

    אם (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      החזר שקר;

    unsigned best_cost = n + 1;
    صحيح len = costs.length;
    hb_iup_set_t best_sol;
    עבור (صحيح start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      صحيح i = start;
      صحيح lookback = start - (صحيح) n;
      כלעוד (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      אם (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        אם (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    עבור (unsigned i = 0; i < n; i++)
      אם (best_sol.has (i))
        opt_indices.arrayZ[i] = אמת;
  }
  החזר אמת;
}

منطقي iup_delta_optimize (ثابت contour_point_vector_t& contour_points,
                         ثابت hb_vector_t<صحيح>& x_deltas,
                         ثابت hb_vector_t<صحيح>& y_deltas,
                         hb_vector_t<منطقي>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         مزدوج tolerance)
{
  אם (!opt_indices.resize (contour_points.length))
      החזר שקר;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned count = contour_points.length;
  אם (unlikely (!end_points.alloc (count)))
    החזר שקר;

  אם (count < 4)
  {
    עבור (unsigned i = 0; i < count; i++)
      end_points.push (i);
  }
  אחרת
  {
    עבור (unsigned i = 0; i < count - 4; i++)
      אם (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    עבור (unsigned i = count - 4; i < count; i++)
      end_points.push (i);
  }

  אם (end_points.in_error ()) החזר שקר;

  unsigned start = 0;
  עבור (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    אם (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      החזר שקר;
    start = end + 1;
  }
  החזר אמת;
}
