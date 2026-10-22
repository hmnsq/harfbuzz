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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-paint.h"

#ಅನ್ತರ್ಭಾವ "hb-geometry.hh"


ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ  hb_paint_extents_context_t hb_paint_extents_context_t;

ಸಂರಚನಾ hb_paint_extents_context_t
{
  ಶೂನ್ಯ clear ()
  {
    transforms.clear ();
    clips.clear ();
    groups.clear ();
    /* COLR scratch storage is zero-allocated and reused without running
     * constructors.  Distinguish that state from an explicitly requested
     * zero budget. */
    ಯದಿ (unlikely (!budget_initialized))
    {
      budget = HB_BUDGET_DEFAULT;
      budget_initialized = ಸತ್ಯ;
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
    ಪ್ರತ್ಯಾಗಮ groups.tail().extents;
  }

  ತಾರ್ಕಿಕ is_bounded ()
  {
    ಪ್ರತ್ಯಾಗಮ groups.tail().status != hb_bounds_t<>::UNBOUNDED;
  }

  ಶೂನ್ಯ ತಳ್ಳು_transform (ಧ್ರುವ hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.tail ();
    t.multiply (trans);
    transforms.push (t);
  }

  ಶೂನ್ಯ ತೆಗೆ_transform ()
  {
    transforms.ತೆಗೆ ();
  }

  ಶೂನ್ಯ ತಳ್ಳು_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    ಧ್ರುವ hb_transform_t<> &t = transforms.tail ();
    t.transform_extents (extents);

    auto bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.tail ());

    clips.push (bounds);
  }

  ಶೂನ್ಯ ತೆಗೆ_clip ()
  {
    clips.ತೆಗೆ ();
  }

  ಶೂನ್ಯ ತಳ್ಳು_group ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::EMPTY});
  }

  ಶೂನ್ಯ ತೆಗೆ_group (hb_paint_composite_mode_t mode)
  {
    ಧ್ರುವ hb_bounds_t<> src_bounds = groups.ತೆಗೆ ();
    hb_bounds_t<> &backdrop_bounds = groups.tail ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    ವಿಕಲ್ಪ ((ಪೂರ್ಣಾಙ್ಕ) mode)
    {
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounds.status = hb_bounds_t<>::EMPTY;
	ಭಙ್ಗ;
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounds = src_bounds;
	ಭಙ್ಗ;
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	ಭಙ್ಗ;
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_SRC_IN:
      ಪಕ್ಷ HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounds.intersect (src_bounds);
	ಭಙ್ಗ;
      ಸ್ವಭಾವ:
	backdrop_bounds.union_ (src_bounds);
	ಭಙ್ಗ;
     }
  }

  ಶೂನ್ಯ paint ()
  {
    ಧ್ರುವ hb_bounds_t<> &clip = clips.tail ();
    hb_bounds_t<> &group = groups.tail ();

    group.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  ತಾರ್ಕಿಕ budget_initialized = ಸತ್ಯ;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  ಶೂನ್ಯ recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  ರಕ್ಷಿತ:
  hb_vector_t<hb_transform_t<>> transforms;
  hb_vector_t<hb_bounds_t<>> clips;
  hb_vector_t<hb_bounds_t<>> groups;
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#endif /* HB_PAINT_EXTENTS_HH */
