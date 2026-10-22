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

#అన్తర్భావ "gsubgpos-graph.hh"

నామాకాశ లేఖాచిత్రం {

లేఖాచిత్రం_ఫలితం_t<gsubgpos_లేఖాచిత్రం_context_t> gsubgpos_లేఖాచిత్రం_context_t::create (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_3,
                                                                           లేఖాచిత్రం_t& లేఖాచిత్రం_2)
{
  gsubgpos_లేఖాచిత్రం_context_t context (పట్టిక_ట్యాగ్_3, లేఖాచిత్రం_2);

  ఒకవేళ (unlikely (పట్టిక_ట్యాగ్_3 != HB_OT_ట్యాగ్_GPOS
                && పట్టిక_ట్యాగ్_3 != HB_OT_ట్యాగ్_GSUB))
    తిరిగి Err(INVALID_వాదం);

  TRY_కేటాయించు (స్థిరరాశి GSTAR* gstar, లేఖాచిత్రం::GSTAR::లేఖాచిత్రం_గమ్యం_gstar (context.లేఖాచిత్రం));
  TRY(gstar->కనుగొను_lookups (context.లేఖాచిత్రం, context.lookups));
  TRY_కేటాయించు (context.lookup_list_సూచీ, gstar->get_lookup_list_సూచీ (context.లేఖాచిత్రం));

  తిరిగి context;
}

gsubgpos_లేఖాచిత్రం_context_t::gsubgpos_లేఖాచిత్రం_context_t (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_3,
                                                    లేఖాచిత్రం_t& లేఖాచిత్రం_2)
    : పట్టిక_ట్యాగ్_2 (పట్టిక_ట్యాగ్_3),
      లేఖాచిత్రం (లేఖాచిత్రం_2),
      lookup_list_సూచీ (0),
      lookups ()
{}

లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> gsubgpos_లేఖాచిత్రం_context_t::create_కణుపు (గుర్తులేని కొలత)
{
  అక్షరం* బఫరు = (అక్షరం*) hb_calloc (1, కొలత);
  ఒకవేళ (unlikely (!బఫరు))
    తిరిగి Err(కేటాయింపు_FAILURE);

  స్వయంచాలక res = కూడు_బఫరు (బఫరు);
  ఒకవేళ (unlikely (!res.is_ok ())) {
    // Allocation did not get stored for freeing later.
    hb_free (బఫరు);
    తిరిగి Err(res.error ());
  }

  తిరిగి లేఖాచిత్రం.new_కణుపు (బఫరు, బఫరు + కొలత);
}

గుర్తులేని gsubgpos_లేఖాచిత్రం_context_t::అంకె_non_ext_subtables ()  {
  గుర్తులేని లెక్క = 0;
  కొరకు (స్వయంచాలక l : lookups.values ())
  {
    ఒకవేళ (l->is_extension (పట్టిక_ట్యాగ్_2)) కొనసాగు;
    లెక్క += l->సంఖ్య_యొక్క_subtables ();
  }
  తిరిగి లెక్క;
}

}
