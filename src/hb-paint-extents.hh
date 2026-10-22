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

#अगर_अघोषित HB_PAINT_EXTENTS_HH
#घोषणा HB_PAINT_EXTENTS_HH

#समावेश "hb.hh"
#समावेश "hb-paint.h"

#समावेश "hb-geometry.hh"


प्रकार काष्ठा  hb_paint_extents_context_t hb_paint_extents_context_t;

काष्ठा hb_paint_extents_context_t
{
  व्योम सफा ()
  {
    transforms.सफा ();
    clips.सफा ();
    groups.सफा ();
    /* COLR scratch storage is zero-allocated and reused without running
     * constructors.  Distinguish that state from an explicitly requested
     * zero budget. */
    अगर (unlikely (!budget_initialized))
    {
      budget = HB_BUDGET_DEFAULT;
      budget_initialized = true;
    }
    recharge_budget ();

    transforms.push (hb_transform_t<>{});
    clips.push (hb_bounds_t<>{hb_bounds_t<>::UNBOUNDED});
    groups.push (hb_bounds_t<>{hb_bounds_t<>::EMPTY});
  }

  hb_paint_extents_context_t ()
  {
    सफा ();
  }

  hb_extents_t<> get_extents ()
  {
    वापस groups.पुच्छ().extents;
  }

  bool is_bounded ()
  {
    वापस groups.पुच्छ().status != hb_bounds_t<>::UNBOUNDED;
  }

  व्योम धकेलो_transform (स्थिर hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.पुच्छ ();
    t.multiply (trans);
    transforms.push (t);
  }

  व्योम निकालो_transform ()
  {
    transforms.निकालो ();
  }

  व्योम धकेलो_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    स्थिर hb_transform_t<> &t = transforms.पुच्छ ();
    t.transform_extents (extents);

    स्वतः bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.पुच्छ ());

    clips.push (bounds);
  }

  व्योम निकालो_clip ()
  {
    clips.निकालो ();
  }

  व्योम धकेलो_group ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::EMPTY});
  }

  व्योम निकालो_group (hb_paint_composite_mode_t mode)
  {
    स्थिर hb_bounds_t<> src_bounds = groups.निकालो ();
    hb_bounds_t<> &backdrop_bounds = groups.पुच्छ ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    चयन ((पूर्णांक) mode)
    {
      हाल HB_PAINT_COMPOSITE_MODE_CLEAR:
	backdrop_bounds.status = hb_bounds_t<>::EMPTY;
	अवरोध;
      हाल HB_PAINT_COMPOSITE_MODE_SRC:
      हाल HB_PAINT_COMPOSITE_MODE_SRC_OUT:
	backdrop_bounds = src_bounds;
	अवरोध;
      हाल HB_PAINT_COMPOSITE_MODE_DEST:
      हाल HB_PAINT_COMPOSITE_MODE_DEST_OUT:
	अवरोध;
      हाल HB_PAINT_COMPOSITE_MODE_SRC_IN:
      हाल HB_PAINT_COMPOSITE_MODE_DEST_IN:
	backdrop_bounds.intersect (src_bounds);
	अवरोध;
      शेष:
	backdrop_bounds.union_ (src_bounds);
	अवरोध;
     }
  }

  व्योम paint ()
  {
    स्थिर hb_bounds_t<> &clip = clips.पुच्छ ();
    hb_bounds_t<> &group = groups.पुच्छ ();

    group.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  bool budget_initialized = true;
  int64_t budget_remaining = HB_BUDGET_GLYPH;

  व्योम recharge_budget ()
  {
    budget_remaining = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  रक्षित:
  hb_vector_t<hb_transform_t<>> transforms;
  hb_vector_t<hb_bounds_t<>> clips;
  hb_vector_t<hb_bounds_t<>> groups;
};

HB_INTERNAL hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#पूर्ण_अगर /* HB_PAINT_EXTENTS_HH */
