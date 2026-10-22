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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-instancer-iup.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bit-page.hh"

ਵਰਤੋਂ hb_iup_ਸੈੱਟ_t = hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਪੰਨਾ_t;

/* This file is a straight port of the following:
 *
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/varLib/iup.py
 *
 * Where that file returns optimzied deltas vector, we return optimized
 * referenced point indices.
 */

constexpr ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ MAX_LOOKBACK = 8;

ਸਥਿਰ ਖਾਲੀ iup_contour_bound_forced_ਸੈੱਟ (ਅਚਲ hb_array_t<ਅਚਲ contour_ਬਿੰਦੂ_t> contour_points,
                                           ਅਚਲ hb_array_t<ਅਚਲ ਪੂਰਨ> x_deltas,
                                           ਅਚਲ hb_array_t<ਅਚਲ ਪੂਰਨ> y_deltas,
                                           hb_iup_ਸੈੱਟ_t& forced_ਸੈੱਟ, /*ਬਾਹਰਲਾ*/
                                           ਦੁੱਗਣਾ tolerance = 0.0)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ len = contour_points.ਲੰਮਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਗਲਾ_i = 0;
  ਲਈ (ਪੂਰਨ i = len - 1; i >= 0; i--)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਆਖਰੀ_i = (len + i - 1) % len;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < 2; j++)
    {
      ਦੁੱਗਣਾ cj, lcj, ncj;
      ਦੁੱਗਣਾ dj, ldj, ndj;
      ਜੇ (j == 0)
      {
        cj = static_cast<ਦੁੱਗਣਾ> (contour_points.arrayZ[i].x);
        dj = x_deltas.arrayZ[i];
        lcj = static_cast<ਦੁੱਗਣਾ> (contour_points.arrayZ[ਆਖਰੀ_i].x);
        ldj = x_deltas.arrayZ[ਆਖਰੀ_i];
        ncj = static_cast<ਦੁੱਗਣਾ> (contour_points.arrayZ[ਅਗਲਾ_i].x);
        ndj = x_deltas.arrayZ[ਅਗਲਾ_i];
      }
      ਨਹੀਂਤਾਂ
      {
        cj = static_cast<ਦੁੱਗਣਾ> (contour_points.arrayZ[i].y);
        dj = y_deltas.arrayZ[i];
        lcj = static_cast<ਦੁੱਗਣਾ> (contour_points.arrayZ[ਆਖਰੀ_i].y);
        ldj = y_deltas.arrayZ[ਆਖਰੀ_i];
        ncj = static_cast<ਦੁੱਗਣਾ> (contour_points.arrayZ[ਅਗਲਾ_i].y);
        ndj = y_deltas.arrayZ[ਅਗਲਾ_i];
      }

      ਦੁੱਗਣਾ c1, c2;
      ਦੁੱਗਣਾ d1, d2;
      ਜੇ (lcj <= ncj)
      {
        c1 = lcj;
        c2 = ncj;
        d1 = ldj;
        d2 = ndj;
      }
      ਨਹੀਂਤਾਂ
      {
        c1 = ncj;
        c2 = lcj;
        d1 = ndj;
        d2 = ldj;
      }

      ਤਾਰਕਿਕ force = ਝੂਠ;
      ਜੇ (c1 == c2)
      {
        ਜੇ (fabs (d1 - d2) > tolerance && fabs (dj) > tolerance)
          force = ਸੱਚ;
      }
      ਨਹੀਂਤਾਂ ਜੇ (c1 <= cj && cj <= c2)
      {
        ਜੇ (!(hb_min (d1, d2) - tolerance <= dj &&
              dj <= hb_max (d1, d2) + tolerance))
          force = ਸੱਚ;
      }
      ਨਹੀਂਤਾਂ
      {
        ਜੇ (d1 != d2)
        {
          ਜੇ (cj < c1)
          {
            ਜੇ (fabs (dj) > tolerance &&
                fabs (dj - d1) > tolerance &&
                ((dj - tolerance < d1) != (d1 < d2)))
                force = ਸੱਚ;
          }
          ਨਹੀਂਤਾਂ
          {
            ਜੇ (fabs (dj) > tolerance &&
                fabs (dj - d2) > tolerance &&
                ((d2 < dj + tolerance) != (d1 < d2)))
              force = ਸੱਚ;
          }
        }
      }

      ਜੇ (force)
      {
        forced_ਸੈੱਟ.ਜੋੜੋ (i);
        ਰੋਕੋ;
      }
    }
    ਅਗਲਾ_i = i;
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
          hb_enable_if (hb_is_trivially_copyable (T))>
ਸਥਿਰ ਤਾਰਕਿਕ rotate_array (ਅਚਲ hb_array_t<ਅਚਲ T>& org_array,
                          ਪੂਰਨ k,
                          hb_ਸਦਿਸ਼_t<T>& ਬਾਹਰਲਾ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ n = org_array.ਲੰਮਾਈ;
  ਜੇ (!n) ਵਾਪਸ ਸੱਚ;
  ਜੇ (unlikely (!ਬਾਹਰਲਾ.resize_dirty  (n)))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਸਤੂ_ਮਾਪ = hb_static_ਮਾਪ (T);
  ਜੇ (k < 0)
    k = n - (-k) % n;
  ਨਹੀਂਤਾਂ
    k %= n;

  hb_memcpy ((ਖਾਲੀ *) ਬਾਹਰਲਾ.arrayZ, (ਅਚਲ ਖਾਲੀ *) (org_array.arrayZ + n - k), k * ਵਸਤੂ_ਮਾਪ);
  hb_memcpy ((ਖਾਲੀ *) (ਬਾਹਰਲਾ.arrayZ + k), (ਅਚਲ ਖਾਲੀ *) org_array.arrayZ, (n - k) * ਵਸਤੂ_ਮਾਪ);
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ rotate_ਸੈੱਟ (ਅਚਲ hb_iup_ਸੈੱਟ_t& org_ਸੈੱਟ,
                        ਪੂਰਨ k,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ n,
                        hb_iup_ਸੈੱਟ_t& ਬਾਹਰਲਾ)
{
  ਜੇ (!n) ਵਾਪਸ ਝੂਠ;
  k %= n;
  ਜੇ (k < 0)
    k = n + k;

  ਜੇ (k == 0)
  {
    ਬਾਹਰਲਾ = org_ਸੈੱਟ;
  }
  ਨਹੀਂਤਾਂ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ v : org_ਸੈੱਟ)
      ਬਾਹਰਲਾ.ਜੋੜੋ ((v + k) % n);
  }
  ਵਾਪਸ ਸੱਚ;
}

/* Given two reference coordinates (start and end of contour_points array),
 * output interpolated deltas for points in between */
ਸਥਿਰ ਤਾਰਕਿਕ _iup_segment (ਅਚਲ hb_array_t<ਅਚਲ contour_ਬਿੰਦੂ_t> contour_points,
                          ਅਚਲ hb_array_t<ਅਚਲ ਪੂਰਨ> x_deltas,
                          ਅਚਲ hb_array_t<ਅਚਲ ਪੂਰਨ> y_deltas,
                          ਅਚਲ contour_ਬਿੰਦੂ_t& p1, ਅਚਲ contour_ਬਿੰਦੂ_t& p2,
                          ਪੂਰਨ p1_dx, ਪੂਰਨ p2_dx,
                          ਪੂਰਨ p1_dy, ਪੂਰਨ p2_dy,
			  ਦੁੱਗਣਾ tolerance_sq,
                          hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ>& interp_x_deltas, /*ਬਾਹਰਲਾ*/
                          hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ>& interp_y_deltas /*ਬਾਹਰਲਾ*/)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ n = contour_points.ਲੰਮਾਈ;
  ਜੇ (unlikely (!interp_x_deltas.resize_dirty  (n) ||
                !interp_y_deltas.resize_dirty  (n)))
    ਵਾਪਸ ਝੂਠ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ j = 0; j < 2; j++)
  {
    ਦਸ਼ਮਲਵ contour_ਬਿੰਦੂ_t::* xp;
    ਦੁੱਗਣਾ x1, x2, d1, d2;
    ਅਚਲ ਪੂਰਨ *ਵਿੱਚ;
    ਦੁੱਗਣਾ *ਬਾਹਰਲਾ;
    ਜੇ (j == 0)
    {
      xp = &contour_ਬਿੰਦੂ_t::x;
      x1 = static_cast<ਦੁੱਗਣਾ> (p1.x);
      x2 = static_cast<ਦੁੱਗਣਾ> (p2.x);
      d1 = p1_dx;
      d2 = p2_dx;
      ਵਿੱਚ = x_deltas.arrayZ;
      ਬਾਹਰਲਾ = interp_x_deltas.arrayZ;
    }
    ਨਹੀਂਤਾਂ
    {
      xp = &contour_ਬਿੰਦੂ_t::y;
      x1 = static_cast<ਦੁੱਗਣਾ> (p1.y);
      x2 = static_cast<ਦੁੱਗਣਾ> (p2.y);
      d1 = p1_dy;
      d2 = p2_dy;
      ਵਿੱਚ = y_deltas.arrayZ;
      ਬਾਹਰਲਾ = interp_y_deltas.arrayZ;
    }

    ਜੇ (x1 == x2)
    {
      ਜੇ (d1 == d2)
      {
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
          ਬਾਹਰਲਾ[i] = d1;
      }
      ਨਹੀਂਤਾਂ
      {
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
          ਬਾਹਰਲਾ[i] = 0.0;
      }
      ਜਾਰੀ;
    }

    ਜੇ (x1 > x2)
    {
      hb_ਅਦਲਾ_ਬਦਲੀ (x1, x2);
      hb_ਅਦਲਾ_ਬਦਲੀ (d1, d2);
    }

    ਦੁੱਗਣਾ scale = (d2 - d1) / (x2 - x1);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
    {
      ਦੁੱਗਣਾ x = (ਦੁੱਗਣਾ) (contour_points.arrayZ[i].*xp);
      ਦੁੱਗਣਾ d;
      ਜੇ (x <= x1)
        d = d1;
      ਨਹੀਂਤਾਂ ਜੇ (x >= x2)
        d = d2;
      ਨਹੀਂਤਾਂ
        d = d1 + (x - x1) * scale;

      ਬਾਹਰਲਾ[i] = d;
      ਦੁੱਗਣਾ err = d - ਵਿੱਚ[i];
      ਜੇ (err * err > tolerance_sq)
	ਵਾਪਸ ਝੂਠ;
    }
  }
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ can_iup_ਵਿੱਚ_between (ਅਚਲ hb_array_t<ਅਚਲ contour_ਬਿੰਦੂ_t> contour_points,
                                 ਅਚਲ hb_array_t<ਅਚਲ ਪੂਰਨ> x_deltas,
                                 ਅਚਲ hb_array_t<ਅਚਲ ਪੂਰਨ> y_deltas,
                                 ਅਚਲ contour_ਬਿੰਦੂ_t& p1, ਅਚਲ contour_ਬਿੰਦੂ_t& p2,
                                 ਪੂਰਨ p1_dx, ਪੂਰਨ p2_dx,
                                 ਪੂਰਨ p1_dy, ਪੂਰਨ p2_dy,
                                 ਦੁੱਗਣਾ tolerance_sq,
				 hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> &interp_x_deltas, /* scratch */
				 hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> &interp_y_deltas /* scratch */)
{
  ਜੇ (!_iup_segment (contour_points, x_deltas, y_deltas,
                     p1, p2, p1_dx, p2_dx, p1_dy, p2_dy,
		     tolerance_sq,
                     interp_x_deltas, interp_y_deltas))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ = contour_points.ਲੰਮਾਈ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ; i++)
  {
    ਦੁੱਗਣਾ dx = static_cast<ਦੁੱਗਣਾ> (x_deltas.arrayZ[i]) - interp_x_deltas.arrayZ[i];
    ਦੁੱਗਣਾ dy = static_cast<ਦੁੱਗਣਾ> (y_deltas.arrayZ[i]) - interp_y_deltas.arrayZ[i];

    ਜੇ (dx * dx + dy * dy > tolerance_sq)
      ਵਾਪਸ ਝੂਠ;
  }
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ _iup_contour_optimize_dp (ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t& contour_points,
                                      ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& x_deltas,
                                      ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& y_deltas,
                                      ਅਚਲ hb_iup_ਸੈੱਟ_t& forced_ਸੈੱਟ,
                                      ਦੁੱਗਣਾ tolerance_sq,
                                      ਬਿਨਾਨਿਸ਼ਾਨੀ lookback,
                                      hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& costs, /*ਬਾਹਰਲਾ*/
                                      hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& chain, /*ਬਾਹਰਲਾ*/
				      hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> &interp_x_deltas_scratch,
				      hb_ਸਦਿਸ਼_t<ਦੁੱਗਣਾ> &interp_y_deltas_scratch)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ n = contour_points.ਲੰਮਾਈ;
  ਜੇ (unlikely (!costs.resize_dirty  (n) ||
                !chain.resize_dirty  (n)))
    ਵਾਪਸ ਝੂਠ;

  lookback = hb_min (lookback, MAX_LOOKBACK);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_ਲਾਗਤ = (i == 0 ? 1 : costs.arrayZ[i-1] + 1);

    costs.arrayZ[i] = ਉੱਤਮ_ਲਾਗਤ;
    chain.arrayZ[i] = (i == 0 ? -1 : i - 1);

    ਜੇ (i > 0 && forced_ਸੈੱਟ.has (i - 1))
      ਜਾਰੀ;

    ਪੂਰਨ lookback_ਸੂਚਕਾਂਕ = hb_max ((ਪੂਰਨ) i - (ਪੂਰਨ) lookback + 1, -1);
    ਲਈ (ਪੂਰਨ j = i - 2; j >= lookback_ਸੂਚਕਾਂਕ; j--)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ = j == -1 ? 1 : costs.arrayZ[j] + 1;
      /* num points between i and j */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_points = i - j - 1;
      ਬਿਨਾਨਿਸ਼ਾਨੀ p1 = (j == -1 ? n - 1 : j);
      ਜੇ (ਲਾਗਤ < ਉੱਤਮ_ਲਾਗਤ &&
          can_iup_ਵਿੱਚ_between (contour_points.as_array ().sub_array (j + 1, ਅੰਕ_points),
                               x_deltas.as_array ().sub_array (j + 1, ਅੰਕ_points),
                               y_deltas.as_array ().sub_array (j + 1, ਅੰਕ_points),
                               contour_points.arrayZ[p1], contour_points.arrayZ[i],
                               x_deltas.arrayZ[p1], x_deltas.arrayZ[i],
                               y_deltas.arrayZ[p1], y_deltas.arrayZ[i],
                               tolerance_sq,
			       interp_x_deltas_scratch, interp_y_deltas_scratch))
      {
        ਉੱਤਮ_ਲਾਗਤ = ਲਾਗਤ;
        costs.arrayZ[i] = ਉੱਤਮ_ਲਾਗਤ;
        chain.arrayZ[i] = j;
      }

      ਜੇ (j > 0 && forced_ਸੈੱਟ.has (j))
        ਰੋਕੋ;
    }
  }
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਤਾਰਕਿਕ _iup_contour_optimize (ਅਚਲ hb_array_t<ਅਚਲ contour_ਬਿੰਦੂ_t> contour_points,
                                   ਅਚਲ hb_array_t<ਅਚਲ ਪੂਰਨ> x_deltas,
                                   ਅਚਲ hb_array_t<ਅਚਲ ਪੂਰਨ> y_deltas,
                                   hb_array_t<ਤਾਰਕਿਕ> opt_indices, /*ਬਾਹਰਲਾ*/
                                   ਦੁੱਗਣਾ tolerance,
				   iup_scratch_t &scratch)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ n = contour_points.ਲੰਮਾਈ;
  ਜੇ (opt_indices.ਲੰਮਾਈ != n ||
      x_deltas.ਲੰਮਾਈ != n ||
      y_deltas.ਲੰਮਾਈ != n)
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (unlikely (n > hb_iup_ਸੈੱਟ_t::ਪੰਨਾ_ਬਿੱਟ))
  {
    /* The forced and solution sets cannot hold this contour. Preserve
     * its explicit deltas instead of leaving every reference false. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
      opt_indices.arrayZ[i] = ਸੱਚ;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ all_within_tolerance = ਸੱਚ;
  ਦੁੱਗਣਾ tolerance_sq = tolerance * tolerance;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
  {
    ਪੂਰਨ dx = x_deltas.arrayZ[i];
    ਪੂਰਨ dy = y_deltas.arrayZ[i];
    ਜੇ ((ਦੁੱਗਣਾ) dx * dx + (ਦੁੱਗਣਾ) dy * dy > tolerance_sq)
    {
      all_within_tolerance = ਝੂਠ;
      ਰੋਕੋ;
    }
  }

  /* If all are within tolerance distance, do nothing, opt_indices is
   * initilized to false */
  ਜੇ (all_within_tolerance)
    ਵਾਪਸ ਸੱਚ;

  /* If there's exactly one point, return it */
  ਜੇ (n == 1)
  {
    opt_indices.arrayZ[0] = ਸੱਚ;
    ਵਾਪਸ ਸੱਚ;
  }

  /* If all deltas are exactly the same, return just one (the first one) */
  ਤਾਰਕਿਕ all_deltas_are_equal = ਸੱਚ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < n; i++)
    ਜੇ (x_deltas.arrayZ[i] != x_deltas.arrayZ[0] ||
        y_deltas.arrayZ[i] != y_deltas.arrayZ[0])
    {
      all_deltas_are_equal = ਝੂਠ;
      ਰੋਕੋ;
    }

  ਜੇ (all_deltas_are_equal)
  {
    opt_indices.arrayZ[0] = ਸੱਚ;
    ਵਾਪਸ ਸੱਚ;
  }

  /* else, solve the general problem using Dynamic Programming */
  hb_iup_ਸੈੱਟ_t forced_ਸੈੱਟ;
  iup_contour_bound_forced_ਸੈੱਟ (contour_points, x_deltas, y_deltas, forced_ਸੈੱਟ, tolerance);

  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &costs = scratch.costs.reset ();
  hb_ਸਦਿਸ਼_t<ਪੂਰਨ> &chain = scratch.chain.reset ();

  ਜੇ (!forced_ਸੈੱਟ.is_empty ())
  {
    ਪੂਰਨ k = n - 1 - forced_ਸੈੱਟ.get_max ();
    ਜੇ (k < 0)
      ਵਾਪਸ ਝੂਠ;

    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> &rot_x_deltas = scratch.rot_x_deltas.reset ();
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> &rot_y_deltas = scratch.rot_y_deltas.reset ();
    contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &rot_points = scratch.rot_points;
    rot_points.reset ();
    hb_iup_ਸੈੱਟ_t rot_forced_ਸੈੱਟ;
    ਜੇ (!rotate_array (contour_points, k, rot_points) ||
        !rotate_array (x_deltas, k, rot_x_deltas) ||
        !rotate_array (y_deltas, k, rot_y_deltas) ||
        !rotate_ਸੈੱਟ (forced_ਸੈੱਟ, k, n, rot_forced_ਸੈੱਟ))
      ਵਾਪਸ ਝੂਠ;

    ਜੇ (!_iup_contour_optimize_dp (rot_points, rot_x_deltas, rot_y_deltas,
                                   rot_forced_ਸੈੱਟ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ਵਾਪਸ ਝੂਠ;

    hb_iup_ਸੈੱਟ_t solution;
    ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = n - 1;
    ਜਦੋਂ (ਸੂਚਕਾਂਕ_2 != -1)
    {
      solution.ਜੋੜੋ (ਸੂਚਕਾਂਕ_2);
      ਸੂਚਕਾਂਕ_2 = chain.arrayZ[ਸੂਚਕਾਂਕ_2];
    }

    ਜੇ (solution.is_empty () ||
        forced_ਸੈੱਟ.get_population () > solution.get_population ())
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i : solution)
      opt_indices.arrayZ[i] = ਸੱਚ;

    hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ> &rot_indices = scratch.rot_indices.reset ();
    ਅਚਲ hb_array_t<ਅਚਲ ਤਾਰਕਿਕ> opt_indices_array (opt_indices.arrayZ, opt_indices.ਲੰਮਾਈ);
    ਜੇ (unlikely (!rotate_array (opt_indices_array, -k, rot_indices)))
      ਵਾਪਸ ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
      opt_indices.arrayZ[i] = rot_indices.arrayZ[i];
  }
  ਨਹੀਂਤਾਂ
  {
    hb_ਸਦਿਸ਼_t<ਪੂਰਨ> repeat_x_deltas, repeat_y_deltas;
    contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t repeat_points;

    ਜੇ (unlikely (!repeat_x_deltas.resize_dirty  (n * 2) ||
                  !repeat_y_deltas.resize_dirty  (n * 2) ||
                  !repeat_points.resize_dirty  (n * 2)))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ contour_ਬਿੰਦੂ_ਮਾਪ = hb_static_ਮਾਪ (contour_ਬਿੰਦੂ_t);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
    {
      hb_memcpy ((ਖਾਲੀ *) repeat_x_deltas.arrayZ, (ਅਚਲ ਖਾਲੀ *) x_deltas.arrayZ, n * ਆਕਾਰ (repeat_x_deltas[0]));
      hb_memcpy ((ਖਾਲੀ *) (repeat_x_deltas.arrayZ + n), (ਅਚਲ ਖਾਲੀ *) x_deltas.arrayZ, n * ਆਕਾਰ (repeat_x_deltas[0]));

      hb_memcpy ((ਖਾਲੀ *) repeat_y_deltas.arrayZ, (ਅਚਲ ਖਾਲੀ *) y_deltas.arrayZ, n * ਆਕਾਰ (repeat_x_deltas[0]));
      hb_memcpy ((ਖਾਲੀ *) (repeat_y_deltas.arrayZ + n), (ਅਚਲ ਖਾਲੀ *) y_deltas.arrayZ, n * ਆਕਾਰ (repeat_x_deltas[0]));

      hb_memcpy ((ਖਾਲੀ *) repeat_points.arrayZ, (ਅਚਲ ਖਾਲੀ *) contour_points.arrayZ, n * contour_ਬਿੰਦੂ_ਮਾਪ);
      hb_memcpy ((ਖਾਲੀ *) (repeat_points.arrayZ + n), (ਅਚਲ ਖਾਲੀ *) contour_points.arrayZ, n * contour_ਬਿੰਦੂ_ਮਾਪ);
    }

    ਜੇ (!_iup_contour_optimize_dp (repeat_points, repeat_x_deltas, repeat_y_deltas,
                                   forced_ਸੈੱਟ, tolerance_sq, n,
                                   costs, chain,
				   scratch.interp_x_deltas, scratch.interp_y_deltas))
      ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਉੱਤਮ_ਲਾਗਤ = n + 1;
    ਪੂਰਨ len = costs.ਲੰਮਾਈ;
    hb_iup_ਸੈੱਟ_t ਉੱਤਮ_sol;
    ਲਈ (ਪੂਰਨ ਸ਼ੁਰੂ = n - 1; ਸ਼ੁਰੂ < len; ਸ਼ੁਰੂ++)
    {
      hb_iup_ਸੈੱਟ_t solution;
      ਪੂਰਨ i = ਸ਼ੁਰੂ;
      ਪੂਰਨ lookback = ਸ਼ੁਰੂ - (ਪੂਰਨ) n;
      ਜਦੋਂ (i > lookback)
      {
        solution.ਜੋੜੋ (i % n);
        i = chain.arrayZ[i];
      }
      ਜੇ (i == lookback)
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ_i = i < 0 ? 0 : costs.arrayZ[i];
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਾਗਤ = costs.arrayZ[ਸ਼ੁਰੂ] - ਲਾਗਤ_i;
        ਜੇ (ਲਾਗਤ <= ਉੱਤਮ_ਲਾਗਤ)
        {
          ਉੱਤਮ_sol = solution;
          ਉੱਤਮ_ਲਾਗਤ = ਲਾਗਤ;
        }
      }
    }

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
      ਜੇ (ਉੱਤਮ_sol.has (i))
        opt_indices.arrayZ[i] = ਸੱਚ;
  }
  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ iup_delta_optimize (ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t& contour_points,
                         ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& x_deltas,
                         ਅਚਲ hb_ਸਦਿਸ਼_t<ਪੂਰਨ>& y_deltas,
                         hb_ਸਦਿਸ਼_t<ਤਾਰਕਿਕ>& opt_indices, /*ਬਾਹਰਲਾ*/
			 iup_scratch_t &scratch,
                         ਦੁੱਗਣਾ tolerance)
{
  ਜੇ (!opt_indices.resize (contour_points.ਲੰਮਾਈ))
      ਵਾਪਸ ਝੂਠ;

  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> &ਸਿਰਾ_points = scratch.ਸਿਰਾ_points.reset ();

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = contour_points.ਲੰਮਾਈ;
  ਜੇ (unlikely (!ਸਿਰਾ_points.alloc (ਗਿਣਤੀ_2)))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (ਗਿਣਤੀ_2 < 4)
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਸਿਰਾ_points.push (i);
  }
  ਨਹੀਂਤਾਂ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2 - 4; i++)
      ਜੇ (contour_points.arrayZ[i].is_ਸਿਰਾ_ਬਿੰਦੂ)
        ਸਿਰਾ_points.push (i);

    /* phantom points */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = ਗਿਣਤੀ_2 - 4; i < ਗਿਣਤੀ_2; i++)
      ਸਿਰਾ_points.push (i);
  }

  ਜੇ (ਸਿਰਾ_points.ਵਿੱਚ_error ()) ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ : ਸਿਰਾ_points)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ len = ਸਿਰਾ - ਸ਼ੁਰੂ + 1;
    ਜੇ (!_iup_contour_optimize (contour_points.as_array ().sub_array (ਸ਼ੁਰੂ, len),
                                x_deltas.as_array ().sub_array (ਸ਼ੁਰੂ, len),
                                y_deltas.as_array ().sub_array (ਸ਼ੁਰੂ, len),
                                opt_indices.as_array ().sub_array (ਸ਼ੁਰੂ, len),
                                tolerance,
				scratch))
      ਵਾਪਸ ਝੂਠ;
    ਸ਼ੁਰੂ = ਸਿਰਾ + 1;
  }
  ਵਾਪਸ ਸੱਚ;
}
