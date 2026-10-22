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

#اگر_غیر_تعریف_شدہ HB_PAINT_EXTENTS_HH
#تعریف_کرو HB_PAINT_EXTENTS_HH

#شامل "hb.hh"
#شامل "hb-paint.h"

#شامل "hb-geometry.hh"


قسمتعریف ساخت  hb_paint_extents_context_t hb_paint_extents_context_t;

ساخت hb_paint_extents_context_t
{
  خالی clear ()
  {
    transforms.clear ();
    clips.clear ();
    groups.clear ();
    /* COLR scratch storage is zero-allocated and reused without running
     * constructors.  Distinguish that state from an explicitly requested
     * zero budget. */
    اگر (unlikely (!budget_initialized))
    {
      budget = HB_BUDGET_DEFAULT;
      budget_initialized = سچ;
    }
    recharge_budget ();

    transforms.push (hb_transform_t<>{});
    clips.push (hb_bounds_t<>{hb_bounds_t<>::UNBOUNDED});
    groups.push (hb_bounds_t<>{hb_bounds_t<>::خالی_خانے});
  }

  hb_paint_extents_context_t ()
  {
    clear ();
  }

  hb_extents_t<> get_extents ()
  {
    واپس groups.دم().extents;
  }

  منطقی is_bounded ()
  {
    واپس groups.دم().حالت_2 != hb_bounds_t<>::UNBOUNDED;
  }

  خالی ڈالو_transform (ثابت hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.دم ();
    t.multiply (trans);
    transforms.push (t);
  }

  خالی نکالو_transform ()
  {
    transforms.نکالو ();
  }

  خالی ڈالو_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    ثابت hb_transform_t<> &t = transforms.دم ();
    t.transform_extents (extents);

    خودکار bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.دم ());

    clips.push (bounds);
  }

  خالی نکالو_clip ()
  {
    clips.نکالو ();
  }

  خالی ڈالو_گروہ ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::خالی_خانے});
  }

  خالی نکالو_گروہ (hb_paint_مرکب_منوال_t منوال)
  {
    ثابت hb_bounds_t<> src_bounds = groups.نکالو ();
    hb_bounds_t<> &backdrop_bounds = groups.دم ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    انتخاب ((صحیح) منوال)
    {
      حالت HB_PAINT_مرکب_منوال_CLEAR:
	backdrop_bounds.حالت_2 = hb_bounds_t<>::خالی_خانے;
	توڑو;
      حالت HB_PAINT_مرکب_منوال_SRC:
      حالت HB_PAINT_مرکب_منوال_SRC_برآمد:
	backdrop_bounds = src_bounds;
	توڑو;
      حالت HB_PAINT_مرکب_منوال_DEST:
      حالت HB_PAINT_مرکب_منوال_DEST_برآمد:
	توڑو;
      حالت HB_PAINT_مرکب_منوال_SRC_اندر:
      حالت HB_PAINT_مرکب_منوال_DEST_اندر:
	backdrop_bounds.intersect (src_bounds);
	توڑو;
      طےشدہ:
	backdrop_bounds.union_ (src_bounds);
	توڑو;
     }
  }

  خالی paint ()
  {
    ثابت hb_bounds_t<> &clip = clips.دم ();
    hb_bounds_t<> &گروہ = groups.دم ();

    گروہ.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  منطقی budget_initialized = سچ;
  int64_t budget_بقایا = HB_BUDGET_GLYPH;

  خالی recharge_budget ()
  {
    budget_بقایا = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  محفوظ:
  hb_سمتیہ_t<hb_transform_t<>> transforms;
  hb_سمتیہ_t<hb_bounds_t<>> clips;
  hb_سمتیہ_t<hb_bounds_t<>> groups;
};

HB_اندرونی hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#اگر_ختم /* HB_PAINT_EXTENTS_HH */
