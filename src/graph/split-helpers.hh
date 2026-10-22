/*
 * Copyright © 2022  Google, Inc.
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
 *
 * Google Author(s): Garret Rieger
 */

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_విభజించు_HELPERS_HH
#నిర్వచించు లేఖాచిత్రం_విభజించు_HELPERS_HH

#అన్తర్భావ "graph-result.hh"

నామాకాశ లేఖాచిత్రం {

మూస<రకం_పేరు Context>
HB_అంతర్గత
లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> actuate_subtable_విభజించు (Context& విభజించు_context,
                                                              స్థిరరాశి hb_సదిశ_t<గుర్తులేని>& విభజించు_points)
{
  hb_సదిశ_t<గుర్తులేని> new_objects;
  ఒకవేళ (!విభజించు_points)
    తిరిగి Ok(new_objects);

  కొరకు (గుర్తులేని i = 0; i < విభజించు_points.నిడివి; i++)
  {
    గుర్తులేని ప్రారంభం = విభజించు_points[i];
    గుర్తులేని చివర = (i < విభజించు_points.నిడివి - 1)
                   ? విభజించు_points[i + 1]
                   : విభజించు_context.original_లెక్క_2 ();
    TRY_కేటాయించు (గుర్తులేని id, విభజించు_context.clone_range (ప్రారంభం, చివర));
    new_objects.push (id);
    TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (new_objects, కేటాయింపు_FAILURE));
  }

  TRY (విభజించు_context.shrink (విభజించు_points[0]));

  తిరిగి Ok(new_objects);
}

}

#అయితే_ముగింపు  // GRAPH_SPLIT_HELPERS_HH
