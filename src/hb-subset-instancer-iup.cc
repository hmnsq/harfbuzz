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

#அந்தர்பாவ "hb-subset-instancer-iup.hh"

#அந்தர்பாவ "hb-bit-page.hh"

using hb_iup_set_t = hb_bit_page_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr நிலையான unsigned MAX_LOOKBACK = 8;

நிலையான வெறுமை _iup_contour_bound_forced_set (மாறிலி hb_array_t<மாறிலி contour_point_t> contour_points,
                                           மாறிலி hb_array_t<மாறிலி முழு> x_deltas,
                                           மாறிலி hb_array_t<மாறிலி முழு> y_deltas,
                                           hb_iup_set_t& forced_set, /* OUT */
                                           இரட்டை tolerance = 0.0)
{
  unsigned len = contour_points.length;
  unsigned next_i = 0;
  க்கு (முழு i = len - 1; i >= 0; i--)
  {
    unsigned last_i = (len + i - 1) % len;
    க்கு (unsigned j = 0; j < 2; j++)
    {
      இரட்டை cj, lcj, ncj;
      இரட்டை dj, ldj, ndj;
      எனில் (j == 0)
      {
        cj = static_cast<இரட்டை> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<இரட்டை> (contour_points.arrayZ[last_i].x);
        ldj = x_deltas.arrayZ[last_i];
        ncj = static_cast<இரட்டை> (contour_points.arrayZ[next_i].x);
        ndj = x_deltas.arrayZ[next_i];
      }
      இல்லையெனில்
      {
        cj = static_cast<இரட்டை> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<இரட்டை> (contour_points.arrayZ[last_i].y);
        ldj = y_deltas.arrayZ[last_i];
        ncj = static_cast<இரட்டை> (contour_points.arrayZ[next_i].y);
        ndj = y_deltas.arrayZ[next_i];
      }

      இரட்டை c1, c2;
      இரட்டை d1, d2;
      எனில் (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      இல்லையெனில்
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      தருக்க force = பொய்;
      எனில் (c1 == c2)
      {
        எனில் (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = உண்மை;
      }
      இல்லையெனில் எனில் (c1 <= cj && cj <= c2)
      {
        எனில் (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = உண்மை;
      }
      இல்லையெனில்
      {
        எனில் (d1 != d2)
        {
          எனில் (cj < c1)
          {
            எனில் (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = உண்மை;
          }
          இல்லையெனில்
          {
            எனில் (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = உண்மை;
          }
        }
      }

      எனில் (force)
      {
        forced_set.add (i);
        நிறுத்து;
      }
    }
    next_i = i;
  }
}

template <typename T,
          hb_enable_if (hb_is_trivially_copyable (T))>
நிலையான தருக்க rotate_array (மாறிலி hb_array_t<மாறிலி T>& org_array,
                          முழு k,
                          hb_vector_t<T>& out)
{
  unsigned n = org_array.length;
  எனில் (!n) திரும்பு உண்மை;
  எனில் (unlikely (!out.resize_dirty  (n)))
    திரும்பு பொய்;

  unsigned item_size = hb_static_size (T);
  எனில் (k < 0)
    k = n - (-k) % n;
  இல்லையெனில்
    k %= n;

  hb_memcpy ((வெறுமை *) out.arrayZ, (மாறிலி வெறுமை *) (org_array.arrayZ + n - k), k * item_size);
  hb_memcpy ((வெறுமை *) (out.arrayZ + k), (மாறிலி வெறுமை *) org_array.arrayZ, (n - k) * item_size);
  திரும்பு உண்மை;
}

நிலையான தருக்க rotate_set (மாறிலி hb_iup_set_t& org_set,
                        முழு k,
                        unsigned n,
                        hb_iup_set_t& out)
{
  எனில் (!n) திரும்பு பொய்;
  k %= n;
  எனில் (k < 0)
    k = n + k;

  எனில் (k == 0)
  {
    out = org_set;
  }
  இல்லையெனில்
  {
    க்கு (unsigned v : org_set)
      out.add ((v + k) % n);
  }
  திரும்பு உண்மை;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
நிலையான தருக்க _iup_segment (மாறிலி hb_array_t<மாறிலி contour_point_t> contour_points,
                          மாறிலி hb_array_t<மாறிலி முழு> x_deltas,
                          மாறிலி hb_array_t<மாறிலி முழு> y_deltas,
                          மாறிலி contour_point_t& p1, மாறிலி contour_point_t& p2,
                          முழு p1_dx, முழு p2_dx,
                          முழு p1_dy, முழு p2_dy,
			  இரட்டை tolerance_sq,
                          hb_vector_t<இரட்டை>& interp_x_deltas, /* OUT */
                          hb_vector_t<இரட்டை>& interp_y_deltas /* OUT */)
{
  unsigned n = contour_points.length;
  எனில் (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    திரும்பு பொய்;

  க்கு (unsigned j = 0; j < 2; j++)
  {
    தசம contour_point_t::* xp;
    இரட்டை x1, x2, d1, d2;
    மாறிலி முழு *in;
    இரட்டை *out;
    எனில் (j == 0)
    {
      xp = &contour_point_t::x;
      x1 = static_cast<இரட்டை> (p1.x);
      x2 = static_cast<இரட்டை> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      in = x_deltas.arrayZ;
      out = interp_x_deltas.arrayZ;
    }
    இல்லையெனில்
    {
      xp = &contour_point_t::y;
      x1 = static_cast<இரட்டை> (p1.y);
      x2 = static_cast<இரட்டை> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      in = y_deltas.arrayZ;
      out = interp_y_deltas.arrayZ;
    }

    எனில் (x1 == x2)
    {
      எனில் (d1 == d2)
      {
        க்கு (unsigned i = 0; i < n; i++)
          out[i] = d1;
      }
      இல்லையெனில்
      {
        க்கு (unsigned i = 0; i < n; i++)
          out[i] = 0.0;
      }
      தொடர்;
    }

    எனில் (x1 > x2)
    {
      hb_swap (x1, x2);
      hb_swap (d1, d2);
    }

    இரட்டை scale = (d2 - d1) / (x2 - x1);
    க்கு (unsigned i = 0; i < n; i++)
    {
      இரட்டை x = (இரட்டை) (contour_points.arrayZ[i].*xp);
      இரட்டை d;
      எனில் (x <= x1)
        d = d1;
      இல்லையெனில் எனில் (x >= x2)
        d = d2;
      இல்லையெனில்
        d = d1 + (x - x1) * scale;

      out[i] = d;
      இரட்டை err = d - in[i];
      எனில் (err * err > tolerance_sq)
	திரும்பு பொய்;
    }
  }
  திரும்பு உண்மை;
}

நிலையான தருக்க _can_iup_in_between (மாறிலி hb_array_t<மாறிலி contour_point_t> contour_points,
                                 மாறிலி hb_array_t<மாறிலி முழு> x_deltas,
                                 மாறிலி hb_array_t<மாறிலி முழு> y_deltas,
                                 மாறிலி contour_point_t& p1, மாறிலி contour_point_t& p2,
                                 முழு p1_dx, முழு p2_dx,
                                 முழு p1_dy, முழு p2_dy,
                                 இரட்டை tolerance_sq,
				 hb_vector_t<இரட்டை> &interp_x_deltas, /* scratch */
				 hb_vector_t<இரட்டை> &interp_y_deltas /* scratch */)
{
  எனில் (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    திரும்பு பொய்;

  unsigned இலக்கம் = contour_points.length;

  க்கு (unsigned i = 0; i < இலக்கம்; i++)
  {
    இரட்டை dx = static_cast<இரட்டை> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    இரட்டை dy = static_cast<இரட்டை> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    எனில் (dx * dx + dy * dy > tolerance_sq)
      திரும்பு பொய்;
  }
  திரும்பு உண்மை;
}

நிலையான தருக்க _iup_contour_optimize_dp (மாறிலி contour_point_vector_t& contour_points,
                                      மாறிலி hb_vector_t<முழு>& x_deltas,
                                      மாறிலி hb_vector_t<முழு>& y_deltas,
                                      மாறிலி hb_iup_set_t& forced_set,
                                      இரட்டை tolerance_sq,
                                      unsigned lookback,
                                      hb_vector_t<unsigned>& costs, /* OUT */
                                      hb_vector_t<முழு>& chain, /* OUT */
				      hb_vector_t<இரட்டை> &interp_x_deltas_scratch,
				      hb_vector_t<இரட்டை> &interp_y_deltas_scratch)
{
  unsigned n = contour_points.length;
  எனில் (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    திரும்பு பொய்;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  க்கு (unsigned i = 0; i < n; i++)
  {
    unsigned best_cost = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = best_cost;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    எனில் (i > 0 && forced_set.has (i - 1))
      தொடர்;

    முழு lookback_index = hb_max ((முழு) i - (முழு) lookback + 1, -1);
    க்கு (முழு j = i - 2; j >= lookback_index; j--)
    {
      unsigned cost = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      unsigned இலக்கம்_points = i - j - 1;
      unsigned p1 = (j == -1 ? n - 1 : j);
      எனில் (cost < best_cost &&
          _can_iup_in_between (contour_points.as_array ().sub_array (j + 1, இலக்கம்_points),
                               x_deltas.as_array ().sub_array (j + 1, இலக்கம்_points),
                               y_deltas.as_array ().sub_array (j + 1, இலக்கம்_points),
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

      எனில் (j > 0 && forced_set.has (j))
        நிறுத்து;
    }
  }
  திரும்பு உண்மை;
}

நிலையான தருக்க _iup_contour_optimize (மாறிலி hb_array_t<மாறிலி contour_point_t> contour_points,
                                   மாறிலி hb_array_t<மாறிலி முழு> x_deltas,
                                   மாறிலி hb_array_t<மாறிலி முழு> y_deltas,
                                   hb_array_t<தருக்க> opt_indices, /* OUT */
                                   இரட்டை tolerance,
				   iup_scratch_t &scratch)
{
  unsigned n = contour_points.length;
  எனில் (opt_indices.length != n ||
      x_deltas.length != n ||
      y_deltas.length != n)
    திரும்பு பொய்;

  எனில் (unlikely (n > hb_iup_set_t::PAGE_BITS))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    க்கு (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = உண்மை;
    திரும்பு உண்மை;
  }

  தருக்க all_within_tolerance = உண்மை;
  இரட்டை tolerance_sq = tolerance * tolerance;
  க்கு (unsigned i = 0; i < n; i++)
  {
    முழு dx = x_deltas.arrayZ[i];
    முழு dy = y_deltas.arrayZ[i];
    எனில் ((இரட்டை) dx * dx + (இரட்டை) dy * dy > tolerance_sq)
    {
      all_within_tolerance = பொய்;
      நிறுத்து;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  எனில் (all_within_tolerance)
    திரும்பு உண்மை;

  /* If there's exactly one point, return it */
  எனில் (n == 1)
  {
    opt_indices.arrayZ[0] = உண்மை;
    திரும்பு உண்மை;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  தருக்க all_deltas_are_equal = உண்மை;
  க்கு (unsigned i = 1; i < n; i++)
    எனில் (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = பொய்;
      நிறுத்து;
    }

  எனில் (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = உண்மை;
    திரும்பு உண்மை;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_set_t forced_set;
  _iup_contour_bound_forced_set (contour_points, x_deltas, y_deltas, forced_set, tolerance);

  hb_vector_t<unsigned> &costs = scratch.costs.reset ();
  hb_vector_t<முழு> &chain = scratch.chain.reset ();

  எனில் (!forced_set.is_empty ())
  {
    முழு k = n - 1 - forced_set.get_max ();
    எனில் (k < 0)
      திரும்பு பொய்;

    hb_vector_t<முழு> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_vector_t<முழு> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_point_vector_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_set_t rot_forced_set;
    எனில் (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_set (forced_set, k, n, rot_forced_set))
      திரும்பு பொய்;

    எனில் (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      திரும்பு பொய்;

    hb_iup_set_t solution;
    முழு index = n - 1;
    போது (index != -1)
    {
      solution.add (index);
      index = chain.arrayZ[index];
    }

    எனில் (solution.is_empty () ||
        forced_set.get_population () > solution.get_population ())
      திரும்பு பொய்;

    க்கு (unsigned i : solution)
      opt_indices.arrayZ[i] = உண்மை;

    hb_vector_t<தருக்க> &rot_indices = scratch.rot_indices.reset ();
    மாறிலி hb_array_t<மாறிலி தருக்க> opt_indices_array (opt_indices.arrayZ, opt_indices.length);
    எனில் (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      திரும்பு பொய்;

    க்கு (unsigned i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  இல்லையெனில்
  {
    hb_vector_t<முழு> repeat_x_deltas, repeat_y_deltas;
    contour_point_vector_t repeat_points;

    எனில் (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      திரும்பு பொய்;

    unsigned contour_point_size = hb_static_size (contour_point_t);
    க்கு (unsigned i = 0; i < n; i++)
    {
      hb_memcpy ((வெறுமை *) repeat_x_deltas.arrayZ, (மாறிலி வெறுமை *) x_deltas.arrayZ, n * அளவு (repeat_x_deltas[0]));
      hb_memcpy ((வெறுமை *) (repeat_x_deltas.arrayZ + n), (மாறிலி வெறுமை *) x_deltas.arrayZ, n * அளவு (repeat_x_deltas[0]));

      hb_memcpy ((வெறுமை *) repeat_y_deltas.arrayZ, (மாறிலி வெறுமை *) y_deltas.arrayZ, n * அளவு (repeat_x_deltas[0]));
      hb_memcpy ((வெறுமை *) (repeat_y_deltas.arrayZ + n), (மாறிலி வெறுமை *) y_deltas.arrayZ, n * அளவு (repeat_x_deltas[0]));

      hb_memcpy ((வெறுமை *) repeat_points.arrayZ, (மாறிலி வெறுமை *) contour_points.arrayZ, n * contour_point_size);
      hb_memcpy ((வெறுமை *) (repeat_points.arrayZ + n), (மாறிலி வெறுமை *) contour_points.arrayZ, n * contour_point_size);
    }

    எனில் (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_set, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      திரும்பு பொய்;

    unsigned best_cost = n + 1;
    முழு len = costs.length;
    hb_iup_set_t best_sol;
    க்கு (முழு start = n - 1; start < len; start++)
    {
      hb_iup_set_t solution;
      முழு i = start;
      முழு lookback = start - (முழு) n;
      போது (i > lookback)
      {
        solution.add (i % n);
        i = chain.arrayZ[i];
      }
      எனில் (i == lookback)
      {
        unsigned cost_i = i < 0 ? 0 : costs.arrayZ[i];
        unsigned cost = costs.arrayZ[start] - cost_i;
        எனில் (cost <= best_cost)
        {
          best_sol = solution;
          best_cost = cost;
        }
      }
    }

    க்கு (unsigned i = 0; i < n; i++)
      எனில் (best_sol.has (i))
        opt_indices.arrayZ[i] = உண்மை;
  }
  திரும்பு உண்மை;
}

தருக்க iup_delta_optimize (மாறிலி contour_point_vector_t& contour_points,
                         மாறிலி hb_vector_t<முழு>& x_deltas,
                         மாறிலி hb_vector_t<முழு>& y_deltas,
                         hb_vector_t<தருக்க>& opt_indices, /* OUT */
			 iup_scratch_t &scratch,
                         இரட்டை tolerance)
{
  எனில் (!opt_indices.resize (contour_points.length))
      திரும்பு பொய்;

  hb_vector_t<unsigned> &end_points = scratch.end_points.reset ();

  unsigned மொத்தம் = contour_points.length;
  எனில் (unlikely (!end_points.alloc (மொத்தம்)))
    திரும்பு பொய்;

  எனில் (மொத்தம் < 4)
  {
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      end_points.push (i);
  }
  இல்லையெனில்
  {
    க்கு (unsigned i = 0; i < மொத்தம் - 4; i++)
      எனில் (contour_points.arrayZ[i].is_end_point)
        end_points.push (i);

    /* phantom points */
    க்கு (unsigned i = மொத்தம் - 4; i < மொத்தம்; i++)
      end_points.push (i);
  }

  எனில் (end_points.in_error ()) திரும்பு பொய்;

  unsigned start = 0;
  க்கு (unsigned end : end_points)
  {
    unsigned len = end - start + 1;
    எனில் (!_iup_contour_optimize (contour_points.as_array ().sub_array (start, len),
                                x_deltas.as_array ().sub_array (start, len),
                                y_deltas.as_array ().sub_array (start, len),
                                opt_indices.as_array ().sub_array (start, len),
                                tolerance,
				scratch))
      திரும்பு பொய்;
    start = end + 1;
  }
  திரும்பு உண்மை;
}
