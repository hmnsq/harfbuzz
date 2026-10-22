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

#నిర్వచితం_కాకపోతే HB_PAINT_BOUNDED_HH
#నిర్వచించు HB_PAINT_BOUNDED_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-paint.h"

#అన్తర్భావ "hb-geometry.hh"


రకనిర్వచనం నిర్మాణం  hb_paint_bounded_context_t hb_paint_bounded_context_t;

నిర్మాణం hb_paint_bounded_context_t
{
  శూన్య clear ()
  {
    clips = 0;
    bounded = నిజం;
    groups.clear ();
  }

  hb_paint_bounded_context_t ()
  {
    clear ();
  }

  తార్కిక is_bounded ()
  {
    తిరిగి bounded;
  }

  శూన్య నెట్టు_clip ()
  {
    clips++;
  }

  శూన్య తీయి_clip ()
  {
    ఒకవేళ (clips == 0) తిరిగి;
    clips--;
  }

  శూన్య నెట్టు_సమూహం ()
  {
    groups.push (bounded);
    bounded = నిజం;
  }

  శూన్య తీయి_సమూహం (hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
  {
    స్థిరరాశి తార్కిక src_bounded = bounded;
    bounded = groups.తీయి ();
    తార్కిక &backdrop_bounded = bounded;

    // https://learn.microsoft.com/en-us/typography/opentype/spec/colr#format-32-paintcomposite
    ఎంపిక ((పూర్ణాంకం) బహుళకం)
    {
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_CLEAR:
	backdrop_bounded = నిజం;
	ఆపు;
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_బహిర్గతం:
	backdrop_bounded = src_bounded;
	ఆపు;
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_బహిర్గతం:
	ఆపు;
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_లోపల:
      స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_లోపల:
	backdrop_bounded = backdrop_bounded && src_bounded;
	ఆపు;
      అప్రమేయ:
	backdrop_bounded = backdrop_bounded || src_bounded;
	ఆపు;
     }
  }

  శూన్య paint ()
  {
    ఒకవేళ (!clips)
      bounded = అబద్ధం;
  }

  రక్షిత:
  తార్కిక bounded; // true if current drawing bounded
  గుర్తులేని clips; // number of active clips
  hb_సదిశ_t<తార్కిక> groups; // true if group bounded
};

HB_అంతర్గత hb_paint_funcs_t *
hb_paint_bounded_get_funcs ();


#అయితే_ముగింపు /* HB_PAINT_BOUNDED_HH */
