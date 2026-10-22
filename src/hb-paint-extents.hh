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

#నిర్వచితం_కాకపోతే HB_PAINT_EXTENTS_HH
#నిర్వచించు HB_PAINT_EXTENTS_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-paint.h"

#అన్తర్భావ "hb-geometry.hh"


రకనిర్వచనం నిర్మాణం  hb_paint_extents_context_t hb_paint_extents_context_t;

నిర్మాణం hb_paint_extents_context_t
{
  శూన్య clear ()
  {
    transforms.clear ();
    clips.clear ();
    groups.clear ();
    /* COLR scratch storage is zero-allocated and reused without running
     * constructors.  Distinguish that state from an explicitly requested
     * zero budget. */
    ఒకవేళ (unlikely (!budget_initialized))
    {
      budget = HB_BUDGET_DEFAULT;
      budget_initialized = నిజం;
    }
    recharge_budget ();

    transforms.push (hb_transform_t<>{});
    clips.push (hb_bounds_t<>{hb_bounds_t<>::UNBOUNDED});
    groups.push (hb_bounds_t<>{hb_bounds_t<>::ఖాళీలు});
  }

  hb_paint_extents_context_t ()
  {
    clear ();
  }

  hb_extents_t<> get_extents ()
  {
    తిరిగి groups.తోక().extents;
  }

  తార్కిక is_bounded ()
  {
    తిరిగి groups.తోక().స్థితి_2 != hb_bounds_t<>::UNBOUNDED;
  }

  శూన్య నెట్టు_transform (స్థిరరాశి hb_transform_t<> &trans)
  {
    hb_transform_t<> t = transforms.తోక ();
    t.multiply (trans);
    transforms.push (t);
  }

  శూన్య తీయి_transform ()
  {
    transforms.తీయి ();
  }

  శూన్య నెట్టు_clip (hb_extents_t<> extents)
  {
    /* Transform extents and push a new clip. */
    స్థిరరాశి hb_transform_t<> &t = transforms.తోక ();
    t.transform_extents (extents);

    స్వయంచాలక bounds = hb_bounds_t<> {extents};
    bounds.intersect (clips.తోక ());

    clips.push (bounds);
  }

  శూన్య తీయి_clip ()
  {
    clips.తీయి ();
  }

  శూన్య నెట్టు_సమూహం ()
  {
    groups.push (hb_bounds_t<> {hb_bounds_t<>::ఖాళీలు});
  }

  శూన్య తీయి_సమూహం (hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
  {
    స్థిరరాశి hb_bounds_t<> src_bounds = groups.తీయి ();
    hb_bounds_t<> &backdrop_bounds = groups.తోక ();

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    ఎంపిక ((పూర్ణాంకం) బహుళకం)
    {
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_CLEAR:
	backdrop_bounds.స్థితి_2 = hb_bounds_t<>::ఖాళీలు;
	ఆపు;
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_బహిర్గతం:
	backdrop_bounds = src_bounds;
	ఆపు;
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_బహిర్గతం:
	ఆపు;
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_లోపల:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_లోపల:
	backdrop_bounds.intersect (src_bounds);
	ఆపు;
      అప్రమేయ:
	backdrop_bounds.union_ (src_bounds);
	ఆపు;
     }
  }

  శూన్య paint ()
  {
    స్థిరరాశి hb_bounds_t<> &clip = clips.తోక ();
    hb_bounds_t<> &సమూహం = groups.తోక ();

    సమూహం.union_ (clip);
  }

  /* Cumulative work budget for the current session; reset by clear().
   * Outline traversal and consumed segments charge the same live
   * counter. */
  int64_t budget = HB_BUDGET_DEFAULT;
  తార్కిక budget_initialized = నిజం;
  int64_t budget_మిగిలిన = HB_BUDGET_GLYPH;

  శూన్య recharge_budget ()
  {
    budget_మిగిలిన = budget == HB_BUDGET_DEFAULT ?
		       HB_BUDGET_GLYPH : budget;
  }

  రక్షిత:
  hb_సదిశ_t<hb_transform_t<>> transforms;
  hb_సదిశ_t<hb_bounds_t<>> clips;
  hb_సదిశ_t<hb_bounds_t<>> groups;
};

HB_అంతర్గత hb_paint_funcs_t *
hb_paint_extents_get_funcs ();


#అయితే_ముగింపు /* HB_PAINT_EXTENTS_HH */
