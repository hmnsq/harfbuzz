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

#جے_بنا_تعریف HB_PAINT_EXTENTS_HH
#تعریف_کرو HB_PAINT_EXTENTS_HH

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
    groups.push (hb_bounds_t<>{hb_bounds_t<>::خالی_تھاواں});
  }

  hb_paint_extents_context_t ()
  {
    clear ();
  }

  hb_extents_t<> get_extents ()
  {
    أرجع groups.پوچھ().extents;
  }

  منطقي is_bounded ()
  {
    أرجع groups.پوچھ().حالت != hb_bounds_t<>::UNBOUNDED;
  }

  فراغ ڈالو_transform (ثابت hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.پوچھ ();
    t.multiply (trans);
    transforms.push (t);
  }

  فراغ نکالو_transform ()
  {
    transforms.نکالو ();
  }

  فراغ ڈالو_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    ثابت hb_transform_t<> &t = transforms.پوچھ ();
    t.transform_extents (extents);

    خودکار bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.پوچھ ());

    clips.push (bounds);
  }

  فراغ نکالو_clip ()
  {
    clips.نکالو ();
  }

  فراغ ڈالو_گروہ ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::خالی_تھاواں});
  }

  فراغ نکالو_گروہ (hb_paint_مرکب_منوال_t منوال)
  {
    ثابت hb_bounds_t<> src_bounds = groups.نکالو ();
    hb_bounds_t<> &backdrop_bounds = groups.پوچھ ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    بدل ((صحيح) منوال)
    {
      حالة HB_PAINT_مرکب_منوال_CLEAR:
	backdrop_bounds.حالت = hb_bounds_t<>::خالی_تھاواں;
	اكسر;
      حالة HB_PAINT_مرکب_منوال_SRC:
      حالة HB_PAINT_مرکب_منوال_SRC_باہرلا:
	backdrop_bounds = src_bounds;
	اكسر;
      حالة HB_PAINT_مرکب_منوال_DEST:
      حالة HB_PAINT_مرکب_منوال_DEST_باہرلا:
	اكسر;
      حالة HB_PAINT_مرکب_منوال_SRC_وچ:
      حالة HB_PAINT_مرکب_منوال_DEST_وچ:
	backdrop_bounds.intersect (src_bounds);
	اكسر;
      افتراضي:
	backdrop_bounds.union_ (src_bounds);
	اكسر;
     }
  }

  فراغ paint ()
  {
    ثابت hb_bounds_t<> &clip = clips.پوچھ ();
    hb_bounds_t<> &گروہ = groups.پوچھ ();

    گروہ.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  منطقي budget_initialized = سچ;
  int64_t budget_بچیا = HB_BUDGET_GLYPH;

  فراغ recharge_budget ()
  {
    budget_بچیا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  محمي:
  hb_سمتیہ_t<hb_transform_t<>> transforms;
  hb_سمتیہ_t<hb_bounds_t<>> clips;
  hb_سمتیہ_t<hb_bounds_t<>> groups;
};

HB_اندرونی hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#جے_انت /* HB_PAINT_EXTENTS_HH */
