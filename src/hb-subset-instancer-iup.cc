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

#شامل "hb-subset-instancer-iup.hh"

#شامل "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ایستا unsigned MAX_LOOKBACK = 8;

ایستا تهی _iup_contour_bound_forced_set (ثابت hb_array_t<ثابت contour_point_t> contour_points,
                                           ثابت hb_array_t<ثابت صحیح> x_deltas,
                                           ثابت hb_array_t<ثابت صحیح> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           دوگانه tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  برای (صحیح i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    برای (unsigned j = 0; j < 2; j++)
    {
      دوگانه cj, lcj, ncj;
      دوگانه dj, ldj, ndj;
      اگر (j == 0)
      {
        cj = static_cast<دوگانه> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<دوگانه> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<دوگانه> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      وگرنه
      {
        cj = static_cast<دوگانه> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<دوگانه> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<دوگانه> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      دوگانه c1, c2;
      دوگانه d1, d2;
      اگر (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      وگرنه
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      منطقی force = نادرست;
      اگر (c1 == c2)
      {
        اگر (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = درست;
      }
      وگرنه اگر (c1 <= cj && cj <= c2)
      {
        اگر (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = درست;
      }
      وگرنه
      {
        اگر (d1 != d2)
        {
          اگر (cj < c1)
          {
            اگر (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = درست;
          }
          وگرنه
          {
            اگر (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = درست;
          }
        }
      }

      اگر (force)
      {
        forced_set.add (i);
        بشکن;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ایستا منطقی rotate_array (ثابت hb_array_t<ثابت T>& org_array,
                          صحیح k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  اگر (!n) بازگشت درست;
  اگر (unlikely (!out.resize_dirty  (n)))
    بازگشت نادرست;

  unsigned item_size = hb_static_size (T);
  اگر (k < 0)
    k = n - (-k) % n;
  وگرنه
    k %= n;

  hb_memcpy ((تهی *) out.arrayZ, (ثابت تهی *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((تهی *) (out.arrayZ + k), (ثابت تهی *) org_array.arrayZ, (n - k) * item_size);
  بازگشت درست;
}

ایستا منطقی rotate_set (ثابت hb_iup_set_t& org_set,
                        صحیح k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  اگر (!n) بازگشت نادرست;
  k %= n;
  اگر (k < 0)
    k = n + k;

  اگر (k == 0)
  {
    out = org_set;
  }
  وگرنه
  {
    برای (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  بازگشت درست;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ایستا منطقی _iup_segment (ثابت hb_array_t<ثابت contour_point_t> contour_points,
                          ثابت hb_array_t<ثابت صحیح> x_deltas,
                          ثابت hb_array_t<ثابت صحیح> y_deltas,
                          ثابت contour_point_t& p1, ثابت contour_point_t& p2,
                          صحیح p1_dx, صحیح p2_dx,
                          صحیح p1_dy, صحیح p2_dy,
			  دوگانه tolerance_sq,
                          hb_vector_t<دوگانه>& interp_x_deltas, /* OUT */
                          hb_vector_t<دوگانه>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  اگر (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    بازگشت نادرست;

  برای (unsigned j = 0; j < 2; j++)
  {
    اعشاری contour_point_t::* xp;
    دوگانه x1, x2, d1, d2;
    ثابت صحیح *in;
    دوگانه *out;
    اگر (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<دوگانه> (p1.x);
      x2 = static_cast<دوگانه> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    وگرنه
    {
      xp = &contour_point_t::y;
      x1 = static_cast<دوگانه> (p1.y);
      x2 = static_cast<دوگانه> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    اگر (x1 == x2)
    {
      اگر (d1 == d2)
      {
        برای (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      وگرنه
      {
        برای (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      ادامه;
    }

    اگر (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    دوگانه scale = (d2 - d1) / (x2 - x1);
    برای (unsigned i = 0; i < n; i++)
    {
      دوگانه x = (دوگانه) (contour_points.arrayZ[i].*xp);
      دوگانه d;
      اگر (x <= x1)
        d = d1;
      وگرنه اگر (x >= x2)
        d = d2;
      وگرنه
        d = d1 + (x - x1) * scale;

      out[i] = d;
      دوگانه err = d - in[i];
      اگر (err * err > tolerance_sq)
	بازگشت نادرست;
    }
  }
  بازگشت درست;
}

ایستا منطقی _can_iup_in_between (ثابت hb_array_t<ثابت contour_point_t> contour_points,
                                 ثابت hb_array_t<ثابت صحیح> x_deltas,
                                 ثابت hb_array_t<ثابت صحیح> y_deltas,
                                 ثابت contour_point_t& p1, ثابت contour_point_t& p2,
                                 صحیح p1_dx, صحیح p2_dx,
                                 صحیح p1_dy, صحیح p2_dy,
                                 دوگانه tolerance_sq,
				 hb_vector_t<دوگانه> &interp_x_deltas, /* scratch */
				 hb_vector_t<دوگانه> &interp_y_deltas /* scratch */)
{
  اگر (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    بازگشت نادرست;

  unsigned رقم = contour_points.length;

  برای (unsigned i = 0; i < رقم; i++)
  {
    دوگانه dx = static_cast<دوگانه> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    دوگانه dy = static_cast<دوگانه> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    اگر (dx * dx + dy * dy > tolerance_sq)
      بازگشت نادرست;
  }
  بازگشت درست;
}

ایستا منطقی _iup_contour_optimize_dp (ثابت contour_point_vector_t& contour_points,
                                      ثابت hb_vector_t<صحیح>& x_deltas,
                                      ثابت hb_vector_t<صحیح>& y_deltas,
                                      ثابت hb_iup_set_t& forced_set,
                                      دوگانه tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<صحیح>& chain, /* OUT */
				      hb_vector_t<دوگانه> &interp_x_deltas_scratch,
				      hb_vector_t<دوگانه> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  اگر (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    بازگشت نادرست;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  برای (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    اگر (i > 0 && forced_set.has (i - 1))
      ادامه;

    صحیح lookback_index = hb_max ((صحیح) i - (صحیح) lookback + 1, -1);
    برای (صحیح j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned رقم_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      اگر (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, رقم_points),
                               x_deltas.as_array ().sub_array (j + 1, رقم_points),
                               y_deltas.as_array ().sub_array (j + 1, رقم_points),
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

      اگر (j > 0 && forced_set.has (j))
        بشکن;
    }
  }
  بازگشت درست;
}

ایستا منطقی _iup_contour_optimize (ثابت hb_array_t<ثابت contour_point_t> contour_points,
                                   ثابت hb_array_t<ثابت صحیح> x_deltas,
                                   ثابت hb_array_t<ثابت صحیح> y_deltas,
                                   hb_array_t<منطقی> opt_indices, /* OUT */
                                   دوگانه tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  اگر (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    بازگشت نادرست;

  اگر (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    برای (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = درست;
    بازگشت درست;
  }

  منطقی all_within_tolerance = درست;
  دوگانه tolerance_sq = tolerance * tolerance;
  برای (unsigned i = 0; i < n; i++)
  {
    صحیح dx = x_deltas.arrayZ[i];
    صحیح dy = y_deltas.arrayZ[i];
    اگر ((دوگانه) dx * dx + (دوگانه) dy * dy > tolerance_sq)
    {
      all_within_tolerance = نادرست;
      بشکن;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  اگر (all_within_tolerance)
    بازگشت درست;

  /* If there's exactly one point, return it */
  اگر (n == 1)
  {
    opt_indices.arrayZ[0] = درست;
    بازگشت درست;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  منطقی all_deltas_are_equal = درست;
  برای (unsigned i = 1; i < n; i++)
    اگر (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = نادرست;
      بشکن;
    }

  اگر (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = درست;
    بازگشت درست;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<صحیح> &chain = scratch.chain.reset ();

  اگر (!forced_set.is_empty ())
  {
    صحیح k = n - 1 - forced_set.get_max ();
    اگر (k < 0)
      بازگشت نادرست;

    hb_vector_t<صحیح> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<صحیح> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    اگر (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      بازگشت نادرست;

    اگر (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      بازگشت نادرست;

    hb_iup_set_t solution;
    صحیح index = n - 1;
    وقتی (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    اگر (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      بازگشت نادرست;

    برای (unsigned i : solution)
      opt_indices.arrayZ[i] = درست;

    hb_vector_t<منطقی> &rot_indices = scratch.rot_indices.reset ();
    ثابت hb_array_t<ثابت منطقی> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    اگر (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      بازگشت نادرست;

    برای (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  وگرنه
  {
    hb_vector_t<صحیح> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    اگر (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      بازگشت نادرست;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    برای (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((تهی *) repeat_x_deltas.arrayZ, (ثابت تهی *) x_deltas.arrayZ, n * اندازه (repeat_x_deltas[0]));
      hb_memcpy ((تهی *) (repeat_x_deltas.arrayZ + n), (ثابت تهی *) x_deltas.arrayZ, n * اندازه (repeat_x_deltas[0]));

      hb_memcpy ((تهی *) repeat_y_deltas.arrayZ, (ثابت تهی *) y_deltas.arrayZ, n * اندازه (repeat_x_deltas[0]));
      hb_memcpy ((تهی *) (repeat_y_deltas.arrayZ + n), (ثابت تهی *) y_deltas.arrayZ, n * اندازه (repeat_x_deltas[0]));

      hb_memcpy ((تهی *) repeat_points.arrayZ, (ثابت تهی *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((تهی *) (repeat_points.arrayZ + n), (ثابت تهی *) contour_points.arrayZ, n * contour_point_size);
    }

    اگر (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      بازگشت نادرست;

    unsigned best_cost = n + 1;
    صحیح len = costs.length;
    hb_iup_set_t best_sol;
    برای (صحیح start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      صحیح i = start;
      صحیح lookback = start - (صحیح) n;
      وقتی (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      اگر (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        اگر (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    برای (unsigned i = 0; i < n; i++)
      اگر (best_sol.has (i))
        opt_indices.arrayZ[i] = درست;
  }
  بازگشت درست;
}

منطقی iup_delta_optimize (ثابت contour_point_vector_t& contour_points,
                         ثابت hb_vector_t<صحیح>& x_deltas,
                         ثابت hb_vector_t<صحیح>& y_deltas,
                         hb_vector_t<منطقی>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         دوگانه tolerance)
{
  اگر (!opt_indices.resize (contour_points.length))
      بازگشت نادرست;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned شمار = contour_points.length;
  اگر (unlikely (!end_points.alloc (شمار)))
    بازگشت نادرست;

  اگر (شمار < 4)
  {
    برای (unsigned i = 0; i < شمار; i++)
      end_points.push (i);
  }
  وگرنه
  {
    برای (unsigned i = 0; i < شمار - 4; i++)
      اگر (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    برای (unsigned i = شمار - 4; i < شمار; i++)
      end_points.push (i);
  }

  اگر (end_points.in_error ()) بازگشت نادرست;

  unsigned start = 0;
  برای (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    اگر (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      بازگشت نادرست;
    start = end + 1;
  }
  بازگشت درست;
}
