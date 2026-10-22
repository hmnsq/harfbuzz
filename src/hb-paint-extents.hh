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

#ifndef HB_PAINT_EXTENTS_HH
#define HB_PAINT_EXTENTS_HH

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-paint.h"

#অন্তর্ভাব "hb-geometry.hh"


প্রকারলেখ কাঠামো  hb_paint_extents_context_t hb_paint_extents_context_t;

কাঠামো hb_paint_extents_context_t
{
  শূন্য clear ()
  {
    transforms.clear ();
    clips.clear ();
    groups.clear ();
    /* COLR scratch storage is zero-allocated and reused without running
     * constructors.  Distinguish that state from an explicitly requested
     * zero budget. */
    যদি (unlikely (!budget_initialized))
    {
      budget = HB_BUDGET_DEFAULT;
      budget_initialized = সত্য;
    }
    recharge_budget ();

    transforms.push (hb_transform_t<>{});
    clips.push (hb_bounds_t<>{hb_bounds_t<>::UNBOUNDED});
    groups.push (hb_bounds_t<>{hb_bounds_t<>::EMPTY});
  }

  hb_paint_extents_context_t ()
  {
    clear ();
  }

  hb_extents_t<> get_extents ()
  {
    ফেরত groups.লেজ().extents;
  }

  যৌক্তিক is_bounded ()
  {
    ফেরত groups.লেজ().status != hb_bounds_t<>::UNBOUNDED;
  }

  শূন্য ঠেলো_transform (ধ্রুবক hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.লেজ ();
    t.multiply (trans);
    transforms.push (t);
  }

  শূন্য বের_করো_transform ()
  {
    transforms.বের_করো ();
  }

  শূন্য ঠেলো_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    ধ্রুবক hb_transform_t<> &t = transforms.লেজ ();
    t.transform_extents (extents);

    auto bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.লেজ ());

    clips.push (bounds);
  }

  শূন্য বের_করো_clip ()
  {
    clips.বের_করো ();
  }

  শূন্য ঠেলো_group ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::EMPTY});
  }

  শূন্য বের_করো_group (hb_paint_composite_mode_t mode)
  {
    ধ্রুবক hb_bounds_t<> src_bounds = groups.বের_করো ();
    hb_bounds_t<> &backdrop_bounds = groups.লেজ ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    নির্বাচন ((পূর্ণসংখ্যা) mode)
    {
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounds.status = hb_bounds_t<>::EMPTY;
	বিরতি;
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounds = src_bounds;
	বিরতি;
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	বিরতি;
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_SRC_IN:
      ক্ষেত্র HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounds.intersect (src_bounds);
	বিরতি;
      পূর্বনির্ধারিত:
	backdrop_bounds.union_ (src_bounds);
	বিরতি;
     }
  }

  শূন্য paint ()
  {
    ধ্রুবক hb_bounds_t<> &clip = clips.লেজ ();
    hb_bounds_t<> &group = groups.লেজ ();

    group.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  যৌক্তিক budget_initialized = সত্য;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  শূন্য recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  সুরক্ষিত:
  hb_vector_t<hb_transform_t<>> transforms;
  hb_vector_t<hb_bounds_t<>> clips;
  hb_vector_t<hb_bounds_t<>> groups;
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#endif /* HB_PAINT_EXTENTS_HH */
