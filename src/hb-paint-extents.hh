/*
 * Copyright © 2022 Behdad Esfahbod
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

#اگر_تعریف_نشده HB_PAINT_EXTENTS_HH
#تعریف_کن HB_PAINT_EXTENTS_HH

#ضمن "hb.hh"
#ضمن "hb-paint.h"

#ضمن "hb-geometry.hh"


تعريفنوع بنية  hb_paint_extents_context_t hb_paint_extents_context_t;

بنية hb_paint_extents_context_t
{
  فراغ clear ()
  {
    transforms.clear ();
    clips.clear ();
    groups.clear ();
    /* COLR scratch storage is zero-allocated and reused without running
     * constructors.  Distinguish that state from an explicitly requested
     * zero budget. */
    إذا (unlikely (!budget_initialized))
    {
      budget = HB_BUDGET_DEFAULT;
      budget_initialized = درست;
    }
    recharge_budget ();

    transforms.push (hb_transform_t<>{});
    clips.push (hb_bounds_t<>{hb_bounds_t<>::UNBOUNDED});
    groups.push (hb_bounds_t<>{hb_bounds_t<>::خالی});
  }

  hb_paint_extents_context_t ()
  {
    clear ();
  }

  hb_extents_t<> get_extents ()
  {
    أرجع groups.دم().extents;
  }

  منطقي is_bounded ()
  {
    أرجع groups.دم().وضعیت != hb_bounds_t<>::UNBOUNDED;
  }

  فراغ افزودن_transform (ثابت hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.دم ();
    t.multiply (trans);
    transforms.push (t);
  }

  فراغ برداشت_transform ()
  {
    transforms.برداشت ();
  }

  فراغ افزودن_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    ثابت hb_transform_t<> &t = transforms.دم ();
    t.transform_extents (extents);

    خودکار bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.دم ());

    clips.push (bounds);
  }

  فراغ برداشت_clip ()
  {
    clips.برداشت ();
  }

  فراغ افزودن_گروه ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::خالی});
  }

  فراغ برداشت_گروه (hb_paint_مرکب_نما_t نما)
  {
    ثابت hb_bounds_t<> src_bounds = groups.برداشت ();
    hb_bounds_t<> &backdrop_bounds = groups.دم ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    بدل ((صحيح) نما)
    {
      حالة HB_PAINT_مرکب_نما_CLEAR:
	backdrop_bounds.وضعیت = hb_bounds_t<>::خالی;
	اكسر;
      حالة HB_PAINT_مرکب_نما_SRC:
      حالة HB_PAINT_مرکب_نما_SRC_بیرون:
	backdrop_bounds = src_bounds;
	اكسر;
      حالة HB_PAINT_مرکب_نما_DEST:
      حالة HB_PAINT_مرکب_نما_DEST_بیرون:
	اكسر;
      حالة HB_PAINT_مرکب_نما_SRC_درون:
      حالة HB_PAINT_مرکب_نما_DEST_درون:
	backdrop_bounds.intersect (src_bounds);
	اكسر;
      افتراضي:
	backdrop_bounds.union_ (src_bounds);
	اكسر;
     }
  }

  فراغ paint ()
  {
    ثابت hb_bounds_t<> &clip = clips.دم ();
    hb_bounds_t<> &گروه = groups.دم ();

    گروه.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  منطقي budget_initialized = درست;
  int64_t budget_مانده = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_مانده = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  محمي:
  hb_وکتور_t<hb_transform_t<>> transforms;
  hb_وکتور_t<hb_bounds_t<>> clips;
  hb_وکتور_t<hb_bounds_t<>> groups;
};

HB_داخلی hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#پایان_اگر /* HB_PAINT_EXTENTS_HH */
