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

#అన్తర్భావ "graph.hh"
#అన్తర్భావ "../hb-ot-layout-gsubgpos.hh"

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_GSUBGPOS_CONTEXT_HH
#నిర్వచించు లేఖాచిత్రం_GSUBGPOS_CONTEXT_HH

నామాకాశ లేఖాచిత్రం {

నిర్మాణం Lookup;

నిర్మాణం gsubgpos_లేఖాచిత్రం_context_t
{
  HB_అంతర్గత స్థిర లేఖాచిత్రం_ఫలితం_t<gsubgpos_లేఖాచిత్రం_context_t> create (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_3,
                                                                      లేఖాచిత్రం_t& లేఖాచిత్రం_2);

  hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2;
  లేఖాచిత్రం_t& లేఖాచిత్రం;
  గుర్తులేని lookup_list_సూచీ;
  hb_hashmap_t<గుర్తులేని, లేఖాచిత్రం::Lookup*> lookups;
  hb_hashmap_t<గుర్తులేని, గుర్తులేని> subtable_గమ్యం_extension;
  hb_hashmap_t<గుర్తులేని, hb_సదిశ_t<గుర్తులేని>> విభజించు_subtables;


  HB_అంతర్గత లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> create_కణుపు (గుర్తులేని కొలత);

  లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_బఫరు (అక్షరం* బఫరు)
  {
    తిరిగి లేఖాచిత్రం.కూడు_బఫరు (బఫరు);
  }

 అంతరంగ:
  HB_అంతర్గత gsubgpos_లేఖాచిత్రం_context_t (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_3,
                                        లేఖాచిత్రం_t& లేఖాచిత్రం_2);

  HB_అంతర్గత గుర్తులేని అంకె_non_ext_subtables ();
};

}

#అయితే_ముగింపు  // GRAPH_GSUBGPOS_CONTEXT
