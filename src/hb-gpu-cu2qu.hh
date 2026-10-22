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

#اگر_غیر_تعریف_شدہ HB_GPU_CU2QU_HH
#تعریف_کرو HB_GPU_CU2QU_HH

#شامل "hb-gpu-draw.hh"

#شامل <cmath>


/* Maximum approximation error in font units.
 * 0.5 is well below one unit in any reasonable coordinate system. */
#اگر_غیر_تعریف_شدہ HB_GPU_CU2QU_TOLERANCE
#تعریف_کرو HB_GPU_CU2QU_TOLERANCE 0.5
#اگر_ختم

/* Maximum subdivision depth.  Each level halves the parameter interval;
 * at depth 10 the sub-curve spans 1/1024 of the original, so the
 * quadratic error (O(dt^4)) is reduced by a factor of ~10^12. */
#تعریف_کرو HB_GPU_CU2_QU_MAX_گہرائی 10


ساخت hb_gpu_cu2qu_نقطہ_t
{
  دوہرا x, y;
};

ساکن اندرونی hb_gpu_cu2qu_نقطہ_t
hb_gpu_cu2qu_lerp (hb_gpu_cu2qu_نقطہ_t a, hb_gpu_cu2qu_نقطہ_t b, دوہرا t)
{
  واپس {a.x + (b.x - a.x) * t,
	  a.y + (b.y - a.y) * t};
}


/*
 * Check whether a cubic error curve stays within `tolerance` of the origin.
 */
ساکن منطقی
hb_gpu_cubic_farthest_fit_inside (hb_gpu_cu2qu_نقطہ_t p0,
				  hb_gpu_cu2qu_نقطہ_t p1,
				  hb_gpu_cu2qu_نقطہ_t p2,
				  hb_gpu_cu2qu_نقطہ_t p3,
				  دوہرا tolerance, غیرعلامتی گہرائی)
{
  اگر (hypot (p1.x, p1.y) <= tolerance &&
      hypot (p2.x, p2.y) <= tolerance)
    واپس سچ;

  اگر (گہرائی >= 8)
    واپس جھوٹ;

  hb_gpu_cu2qu_نقطہ_t mid = {(p0.x + 3 * (p1.x + p2.x) + p3.x) * 0.125,
			      (p0.y + 3 * (p1.y + p2.y) + p3.y) * 0.125};
  اگر (hypot (mid.x, mid.y) > tolerance)
    واپس جھوٹ;

  hb_gpu_cu2qu_نقطہ_t d3 = {(p3.x + p2.x - p1.x - p0.x) * 0.125,
			     (p3.y + p2.y - p1.y - p0.y) * 0.125};

  واپس hb_gpu_cubic_farthest_fit_inside (
    p0,
    hb_gpu_cu2qu_lerp (p0, p1, 0.5),
    {mid.x - d3.x, mid.y - d3.y},
    mid,
    tolerance, گہرائی + 1
  ) && hb_gpu_cubic_farthest_fit_inside (
    mid,
    {mid.x + d3.x, mid.y + d3.y},
    hb_gpu_cu2qu_lerp (p2, p3, 0.5),
    p3,
    tolerance, گہرائی + 1
  );
}


/*
 * Try to approximate a cubic Bézier with a single quadratic.
 */
ساکن منطقی
hb_gpu_cubic_approx_quadratic (hb_gpu_cu2qu_نقطہ_t c0,
			       hb_gpu_cu2qu_نقطہ_t c1,
			       hb_gpu_cu2qu_نقطہ_t c2,
			       hb_gpu_cu2qu_نقطہ_t c3,
			       دوہرا tolerance,
			       hb_gpu_cu2qu_نقطہ_t *q1)
{
  دوہرا ax = c1.x - c0.x, ay = c1.y - c0.y;
  دوہرا dx = c3.x - c2.x, dy = c3.y - c2.y;

  دوہرا px = -ay, py = ax;

  دوہرا denom = px * dx + py * dy;
  اگر (fabs (denom) < 1e-12)
    واپس جھوٹ;

  دوہرا h = (px * (c0.x - c2.x) + py * (c0.y - c2.y)) / denom;

  q1->x = c2.x + dx * h;
  q1->y = c2.y + dy * h;

  hb_gpu_cu2qu_نقطہ_t err1 = {c0.x + (q1->x - c0.x) * (2.0 / 3.0) - c1.x,
			       c0.y + (q1->y - c0.y) * (2.0 / 3.0) - c1.y};
  hb_gpu_cu2qu_نقطہ_t err2 = {c3.x + (q1->x - c3.x) * (2.0 / 3.0) - c2.x,
			       c3.y + (q1->y - c3.y) * (2.0 / 3.0) - c2.y};

  واپس hb_gpu_cubic_farthest_fit_inside ({0, 0}, err1, err2, {0, 0}, tolerance, 0);
}


/*
 * Split a cubic at t = 0.5 (de Casteljau).
 */
ساکن خالی
hb_gpu_بانٹو_cubic_half (hb_gpu_cu2qu_نقطہ_t c0,
			 hb_gpu_cu2qu_نقطہ_t c1,
			 hb_gpu_cu2qu_نقطہ_t c2,
			 hb_gpu_cu2qu_نقطہ_t c3,
			 hb_gpu_cu2qu_نقطہ_t برآمد[7])
{
  hb_gpu_cu2qu_نقطہ_t m01  = hb_gpu_cu2qu_lerp (c0,  c1,  0.5);
  hb_gpu_cu2qu_نقطہ_t m12  = hb_gpu_cu2qu_lerp (c1,  c2,  0.5);
  hb_gpu_cu2qu_نقطہ_t m23  = hb_gpu_cu2qu_lerp (c2,  c3,  0.5);
  hb_gpu_cu2qu_نقطہ_t m012 = hb_gpu_cu2qu_lerp (m01, m12, 0.5);
  hb_gpu_cu2qu_نقطہ_t m123 = hb_gpu_cu2qu_lerp (m12, m23, 0.5);
  hb_gpu_cu2qu_نقطہ_t mid  = hb_gpu_cu2qu_lerp (m012, m123, 0.5);

  برآمد[0] = c0;   برآمد[1] = m01;  برآمد[2] = m012;
  برآمد[3] = mid;
  برآمد[4] = m123; برآمد[5] = m23;  برآمد[6] = c3;
}


/*
 * Recursively convert a cubic into quadratic segments.
 */
ساکن خالی
hb_gpu_cubic_مقصد_quadratics (hb_gpu_draw_t *g,
			    hb_gpu_cu2qu_نقطہ_t c0,
			    hb_gpu_cu2qu_نقطہ_t c1,
			    hb_gpu_cu2qu_نقطہ_t c2,
			    hb_gpu_cu2qu_نقطہ_t c3,
			    دوہرا tolerance, غیرعلامتی گہرائی)
{
  hb_gpu_cu2qu_نقطہ_t q1;
  اگر (hb_gpu_cubic_approx_quadratic (c0, c1, c2, c3, tolerance, &q1))
  {
    g->acc_conic_مقصد (q1.x, q1.y, c3.x, c3.y);
    واپس;
  }

  اگر (گہرائی >= HB_GPU_CU2_QU_MAX_گہرائی)
  {
    g->acc_سطر_مقصد (c3.x, c3.y);
    واپس;
  }

  hb_gpu_cu2qu_نقطہ_t pts[7];
  hb_gpu_بانٹو_cubic_half (c0, c1, c2, c3, pts);
  hb_gpu_cubic_مقصد_quadratics (g, pts[0], pts[1], pts[2], pts[3], tolerance, گہرائی + 1);
  hb_gpu_cubic_مقصد_quadratics (g, pts[3], pts[4], pts[5], pts[6], tolerance, گہرائی + 1);
}


#اگر_ختم /* HB_GPU_CU2QU_HH */
