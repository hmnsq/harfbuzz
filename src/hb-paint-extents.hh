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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_PAINT_EXTENTS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_PAINT_EXTENTS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-geometry.hh"


ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ  hb_paint_extents_context_t hb_paint_extents_context_t;

ਬਣਤਰ hb_paint_extents_context_t
{
  ਖਾਲੀ clear ()
  {
    transforms.clear ();
    clips.clear ();
    groups.clear ();
    /* COLR scratch storage is zero-allocated and reused without running
     * constructors.  Distinguish that state from an explicitly requested
     * zero budget. */
    ਜੇ (unlikely (!budget_initialized))
    {
      budget = HB_BUDGET_DEFAULT;
      budget_initialized = ਸੱਚ;
    }
    recharge_budget ();

    transforms.push (hb_transform_t<>{});
    clips.push (hb_bounds_t<>{hb_bounds_t<>::UNBOUNDED});
    groups.push (hb_bounds_t<>{hb_bounds_t<>::ਖਾਲੀ_ਥਾਵਾਂ});
  }

  hb_paint_extents_context_t ()
  {
    clear ();
  }

  hb_extents_t<> get_extents ()
  {
    ਵਾਪਸ groups.ਪੂਛ().extents;
  }

  ਤਾਰਕਿਕ is_bounded ()
  {
    ਵਾਪਸ groups.ਪੂਛ().ਸਥਿਤੀ != hb_bounds_t<>::UNBOUNDED;
  }

  ਖਾਲੀ ਧੱਕੋ_transform (ਅਚਲ hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.ਪੂਛ ();
    t.multiply (trans);
    transforms.push (t);
  }

  ਖਾਲੀ ਕੱਢੋ_transform ()
  {
    transforms.ਕੱਢੋ ();
  }

  ਖਾਲੀ ਧੱਕੋ_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    ਅਚਲ hb_transform_t<> &t = transforms.ਪੂਛ ();
    t.transform_extents (extents);

    ਸਵੈਚਾਲਿਤ bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.ਪੂਛ ());

    clips.push (bounds);
  }

  ਖਾਲੀ ਕੱਢੋ_clip ()
  {
    clips.ਕੱਢੋ ();
  }

  ਖਾਲੀ ਧੱਕੋ_ਸਮੂਹ ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::ਖਾਲੀ_ਥਾਵਾਂ});
  }

  ਖਾਲੀ ਕੱਢੋ_ਸਮੂਹ (hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
  {
    ਅਚਲ hb_bounds_t<> src_bounds = groups.ਕੱਢੋ ();
    hb_bounds_t<> &backdrop_bounds = groups.ਪੂਛ ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    ਚੋਣ ((ਪੂਰਨ) ਬਹੁਲਕ)
    {
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR:
	backdrop_bounds.ਸਥਿਤੀ = hb_bounds_t<>::ਖਾਲੀ_ਥਾਵਾਂ;
	ਰੋਕੋ;
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਬਾਹਰਲਾ:
	backdrop_bounds = src_bounds;
	ਰੋਕੋ;
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਬਾਹਰਲਾ:
	ਰੋਕੋ;
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਵਿੱਚ:
      ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਵਿੱਚ:
	backdrop_bounds.intersect (src_bounds);
	ਰੋਕੋ;
      ਮੂਲ:
	backdrop_bounds.union_ (src_bounds);
	ਰੋਕੋ;
     }
  }

  ਖਾਲੀ paint ()
  {
    ਅਚਲ hb_bounds_t<> &clip = clips.ਪੂਛ ();
    hb_bounds_t<> &ਸਮੂਹ = groups.ਪੂਛ ();

    ਸਮੂਹ.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  ਤਾਰਕਿਕ budget_initialized = ਸੱਚ;
  int64_t budget_ਬਚਿਆ = HB_BUDGET_GLYPH;

  ਖਾਲੀ recharge_budget ()
  {
    budget_ਬਚਿਆ = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  ਸੁਰੱਖਿਅਤ:
  hb_ਸਦਿਸ਼_t<hb_transform_t<>> transforms;
  hb_ਸਦਿਸ਼_t<hb_bounds_t<>> clips;
  hb_ਸਦਿਸ਼_t<hb_bounds_t<>> groups;
};

HB_ਅੰਦਰੂਨੀ hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#ਜੇ_ਅੰਤ /* HB_PAINT_EXTENTS_HH */
