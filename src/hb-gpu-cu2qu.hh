/*
 * Copyright (C) 2026  Behdad Esfahbod
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Cubic-to-quadratic Bézier conversion.
 * Ported from fonttools cu2qu:
 * https://github.com/fonttools/fonttools/blob/main/Lib/fontTools/cu2qu/cu2qu.py
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
 *
 * Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_GPU_CU2QU_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_CU2QU_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-gpu-draw.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <cmath>


/* Maximum approximation error in font units.
 * 0.5 is well below one unit in any reasonable coordinate system. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_GPU_CU2QU_TOLERANCE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_CU2QU_TOLERANCE 0.5
#ਜੇ_ਅੰਤ

/* Maximum subdivision depth.  Each level halves the parameter interval;
 * at depth 10 the sub-curve spans 1/1024 of the original, so the
 * quadratic error (O(dt^4)) is reduced by a factor of ~10^12. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_GPU_CU2_QU_MAX_ਡੂੰਘਾਈ 10


ਬਣਤਰ hb_gpu_cu2qu_ਬਿੰਦੂ_t
{
  ਦੁੱਗਣਾ x, y;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_gpu_cu2qu_ਬਿੰਦੂ_t
hb_gpu_cu2qu_lerp (hb_gpu_cu2qu_ਬਿੰਦੂ_t a, hb_gpu_cu2qu_ਬਿੰਦੂ_t b, ਦੁੱਗਣਾ t)
{
  ਵਾਪਸ {a.x + (b.x - a.x) * t,
	  a.y + (b.y - a.y) * t};
}


/*
 * Check whether a cubic error curve stays within `tolerance` of the origin.
 */
ਸਥਿਰ ਤਾਰਕਿਕ
hb_gpu_cubic_farthest_fit_inside (hb_gpu_cu2qu_ਬਿੰਦੂ_t p0,
				  hb_gpu_cu2qu_ਬਿੰਦੂ_t p1,
				  hb_gpu_cu2qu_ਬਿੰਦੂ_t p2,
				  hb_gpu_cu2qu_ਬਿੰਦੂ_t p3,
				  ਦੁੱਗਣਾ tolerance, ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ)
{
  ਜੇ (hypot (p1.x, p1.y) <= tolerance &&
      hypot (p2.x, p2.y) <= tolerance)
    ਵਾਪਸ ਸੱਚ;

  ਜੇ (ਡੂੰਘਾਈ >= 8)
    ਵਾਪਸ ਝੂਠ;

  hb_gpu_cu2qu_ਬਿੰਦੂ_t mid = {(p0.x + 3 * (p1.x + p2.x) + p3.x) * 0.125,
			      (p0.y + 3 * (p1.y + p2.y) + p3.y) * 0.125};
  ਜੇ (hypot (mid.x, mid.y) > tolerance)
    ਵਾਪਸ ਝੂਠ;

  hb_gpu_cu2qu_ਬਿੰਦੂ_t d3 = {(p3.x + p2.x - p1.x - p0.x) * 0.125,
			     (p3.y + p2.y - p1.y - p0.y) * 0.125};

  ਵਾਪਸ hb_gpu_cubic_farthest_fit_inside (
    p0,
    hb_gpu_cu2qu_lerp (p0, p1, 0.5),
    {mid.x - d3.x, mid.y - d3.y},
    mid,
    tolerance, ਡੂੰਘਾਈ + 1
  ) && hb_gpu_cubic_farthest_fit_inside (
    mid,
    {mid.x + d3.x, mid.y + d3.y},
    hb_gpu_cu2qu_lerp (p2, p3, 0.5),
    p3,
    tolerance, ਡੂੰਘਾਈ + 1
  );
}


/*
 * Try to approximate a cubic Bézier with a single quadratic.
 */
ਸਥਿਰ ਤਾਰਕਿਕ
hb_gpu_cubic_approx_quadratic (hb_gpu_cu2qu_ਬਿੰਦੂ_t c0,
			       hb_gpu_cu2qu_ਬਿੰਦੂ_t c1,
			       hb_gpu_cu2qu_ਬਿੰਦੂ_t c2,
			       hb_gpu_cu2qu_ਬਿੰਦੂ_t c3,
			       ਦੁੱਗਣਾ tolerance,
			       hb_gpu_cu2qu_ਬਿੰਦੂ_t *q1)
{
  ਦੁੱਗਣਾ ax = c1.x - c0.x, ay = c1.y - c0.y;
  ਦੁੱਗਣਾ dx = c3.x - c2.x, dy = c3.y - c2.y;

  ਦੁੱਗਣਾ px = -ay, py = ax;

  ਦੁੱਗਣਾ denom = px * dx + py * dy;
  ਜੇ (fabs (denom) < 1e-12)
    ਵਾਪਸ ਝੂਠ;

  ਦੁੱਗਣਾ h = (px * (c0.x - c2.x) + py * (c0.y - c2.y)) / denom;

  q1->x = c2.x + dx * h;
  q1->y = c2.y + dy * h;

  hb_gpu_cu2qu_ਬਿੰਦੂ_t err1 = {c0.x + (q1->x - c0.x) * (2.0 / 3.0) - c1.x,
			       c0.y + (q1->y - c0.y) * (2.0 / 3.0) - c1.y};
  hb_gpu_cu2qu_ਬਿੰਦੂ_t err2 = {c3.x + (q1->x - c3.x) * (2.0 / 3.0) - c2.x,
			       c3.y + (q1->y - c3.y) * (2.0 / 3.0) - c2.y};

  ਵਾਪਸ hb_gpu_cubic_farthest_fit_inside ({0, 0}, err1, err2, {0, 0}, tolerance, 0);
}


/*
 * Split a cubic at t = 0.5 (de Casteljau).
 */
ਸਥਿਰ ਖਾਲੀ
hb_gpu_ਵੰਡੋ_cubic_half (hb_gpu_cu2qu_ਬਿੰਦੂ_t c0,
			 hb_gpu_cu2qu_ਬਿੰਦੂ_t c1,
			 hb_gpu_cu2qu_ਬਿੰਦੂ_t c2,
			 hb_gpu_cu2qu_ਬਿੰਦੂ_t c3,
			 hb_gpu_cu2qu_ਬਿੰਦੂ_t ਬਾਹਰਲਾ[7])
{
  hb_gpu_cu2qu_ਬਿੰਦੂ_t m01  = hb_gpu_cu2qu_lerp (c0,  c1,  0.5);
  hb_gpu_cu2qu_ਬਿੰਦੂ_t m12  = hb_gpu_cu2qu_lerp (c1,  c2,  0.5);
  hb_gpu_cu2qu_ਬਿੰਦੂ_t m23  = hb_gpu_cu2qu_lerp (c2,  c3,  0.5);
  hb_gpu_cu2qu_ਬਿੰਦੂ_t m012 = hb_gpu_cu2qu_lerp (m01, m12, 0.5);
  hb_gpu_cu2qu_ਬਿੰਦੂ_t m123 = hb_gpu_cu2qu_lerp (m12, m23, 0.5);
  hb_gpu_cu2qu_ਬਿੰਦੂ_t mid  = hb_gpu_cu2qu_lerp (m012, m123, 0.5);

  ਬਾਹਰਲਾ[0] = c0;   ਬਾਹਰਲਾ[1] = m01;  ਬਾਹਰਲਾ[2] = m012;
  ਬਾਹਰਲਾ[3] = mid;
  ਬਾਹਰਲਾ[4] = m123; ਬਾਹਰਲਾ[5] = m23;  ਬਾਹਰਲਾ[6] = c3;
}


/*
 * Recursively convert a cubic into quadratic segments.
 */
ਸਥਿਰ ਖਾਲੀ
hb_gpu_cubic_ਮੰਜ਼ਿਲ_quadratics (hb_gpu_draw_t *g,
			    hb_gpu_cu2qu_ਬਿੰਦੂ_t c0,
			    hb_gpu_cu2qu_ਬਿੰਦੂ_t c1,
			    hb_gpu_cu2qu_ਬਿੰਦੂ_t c2,
			    hb_gpu_cu2qu_ਬਿੰਦੂ_t c3,
			    ਦੁੱਗਣਾ tolerance, ਬਿਨਾਨਿਸ਼ਾਨੀ ਡੂੰਘਾਈ)
{
  hb_gpu_cu2qu_ਬਿੰਦੂ_t q1;
  ਜੇ (hb_gpu_cubic_approx_quadratic (c0, c1, c2, c3, tolerance, &q1))
  {
    g->acc_conic_ਮੰਜ਼ਿਲ (q1.x, q1.y, c3.x, c3.y);
    ਵਾਪਸ;
  }

  ਜੇ (ਡੂੰਘਾਈ >= HB_GPU_CU2_QU_MAX_ਡੂੰਘਾਈ)
  {
    g->acc_ਸਤਰ_ਮੰਜ਼ਿਲ (c3.x, c3.y);
    ਵਾਪਸ;
  }

  hb_gpu_cu2qu_ਬਿੰਦੂ_t pts[7];
  hb_gpu_ਵੰਡੋ_cubic_half (c0, c1, c2, c3, pts);
  hb_gpu_cubic_ਮੰਜ਼ਿਲ_quadratics (g, pts[0], pts[1], pts[2], pts[3], tolerance, ਡੂੰਘਾਈ + 1);
  hb_gpu_cubic_ਮੰਜ਼ਿਲ_quadratics (g, pts[3], pts[4], pts[5], pts[6], tolerance, ਡੂੰਘਾਈ + 1);
}


#ਜੇ_ਅੰਤ /* HB_GPU_CU2QU_HH */
