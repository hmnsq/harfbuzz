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

#جيڪڏهن_وصف_نه_ٿيل HB_PAINT_EXTENTS_HH
#وصف_ڪريو HB_PAINT_EXTENTS_HH

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
      budget_initialized = سچ;
    }
    recharge_budget ();

    transforms.push (hb_transform_t<>{});
    clips.push (hb_bounds_t<>{hb_bounds_t<>::UNBOUNDED});
    groups.push (hb_bounds_t<>{hb_bounds_t<>::خالي_جايون});
  }

  hb_paint_extents_context_t ()
  {
    clear ();
  }

  hb_extents_t<> get_extents ()
  {
    أرجع groups.پڇ().extents;
  }

  منطقي is_bounded ()
  {
    أرجع groups.پڇ().حالت != hb_bounds_t<>::UNBOUNDED;
  }

  فراغ ڌڪو_transform (ثابت hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.پڇ ();
    t.multiply (trans);
    transforms.push (t);
  }

  فراغ ڪڍو_transform ()
  {
    transforms.ڪڍو ();
  }

  فراغ ڌڪو_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    ثابت hb_transform_t<> &t = transforms.پڇ ();
    t.transform_extents (extents);

    خودڪار bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.پڇ ());

    clips.push (bounds);
  }

  فراغ ڪڍو_clip ()
  {
    clips.ڪڍو ();
  }

  فراغ ڌڪو_گروهه ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::خالي_جايون});
  }

  فراغ ڪڍو_گروهه (hb_paint_مرکب_گهڻائي_t گهڻائي)
  {
    ثابت hb_bounds_t<> src_bounds = groups.ڪڍو ();
    hb_bounds_t<> &backdrop_bounds = groups.پڇ ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    بدل ((صحيح) گهڻائي)
    {
      حالة HB_PAINT_مرکب_گهڻائي_CLEAR:
	backdrop_bounds.حالت = hb_bounds_t<>::خالي_جايون;
	اكسر;
      حالة HB_PAINT_مرکب_گهڻائي_SRC:
      حالة HB_PAINT_مرکب_گهڻائي_SRC_ٻاهريون:
	backdrop_bounds = src_bounds;
	اكسر;
      حالة HB_PAINT_مرکب_گهڻائي_DEST:
      حالة HB_PAINT_مرکب_گهڻائي_DEST_ٻاهريون:
	اكسر;
      حالة HB_PAINT_مرکب_گهڻائي_SRC_اندريون:
      حالة HB_PAINT_مرکب_گهڻائي_DEST_اندريون:
	backdrop_bounds.intersect (src_bounds);
	اكسر;
      افتراضي:
	backdrop_bounds.union_ (src_bounds);
	اكسر;
     }
  }

  فراغ paint ()
  {
    ثابت hb_bounds_t<> &clip = clips.پڇ ();
    hb_bounds_t<> &گروهه = groups.پڇ ();

    گروهه.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  منطقي budget_initialized = سچ;
  int64_t budget_بچيل = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_بچيل = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  محمي:
  hb_ويڪٽر_t<hb_transform_t<>> transforms;
  hb_ويڪٽر_t<hb_bounds_t<>> clips;
  hb_ويڪٽر_t<hb_bounds_t<>> groups;
};

HB_اندروني hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#جيڪڏهن_پورو /* HB_PAINT_EXTENTS_HH */
