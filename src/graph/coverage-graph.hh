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
#అన్తర్భావ "../OT/Layout/Common/Coverage.hh"

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_COVERAGE_లేఖాచిత్రం_HH
#నిర్వచించు లేఖాచిత్రం_COVERAGE_లేఖాచిత్రం_HH

నామాకాశ లేఖాచిత్రం {

మూస <రకం_పేరు Types>
స్థిర లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (
  స్థిరరాశి OT::Layout::Common::CoverageFormat1_3<Types>* thiz,
  స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం
) {
  size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత();
  constexpr గుర్తులేని min_కొలత = OT::Layout::Common::CoverageFormat1_3<Types>::min_కొలత;
  ఒకవేళ (unlikely (శీర్షం_len < min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
  hb_barrier ();
  ఒకవేళ (unlikely (శీర్షం_len < min_కొలత + thiz->glyphArray.get_కొలత () - thiz->glyphArray.len.get_కొలత ()))
    తిరిగి Err(SANITIZE_FAILURE);
  తిరిగి Ok();
}

మూస <రకం_పేరు Types>
స్థిర లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (
  స్థిరరాశి OT::Layout::Common::CoverageFormat2_4<Types>* thiz,
  స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం
) {
  size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత();
  constexpr గుర్తులేని min_కొలత = OT::Layout::Common::CoverageFormat2_4<Types>::min_కొలత;
  ఒకవేళ (unlikely (శీర్షం_len < min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
  hb_barrier ();
  ఒకవేళ (unlikely (శీర్షం_len < min_కొలత + thiz->range_రికార్డు.get_కొలత () - thiz->range_రికార్డు.len.get_కొలత ()))
    తిరిగి Err(SANITIZE_FAILURE);
  తిరిగి Ok();
}

నిర్మాణం Coverage : బహిరంగ OT::Layout::Common::Coverage
{
  స్థిర లేఖాచిత్రం_ఫలితం_t<శూన్య> clone_coverage (gsubgpos_లేఖాచిత్రం_context_t& c,
                                              గుర్తులేని coverage_id,
                                              గుర్తులేని new_మాతృ_id,
                                              గుర్తులేని link_position,
                                              గుర్తులేని ప్రారంభం, గుర్తులేని చివర)

  {
    గుర్తులేని coverage_కొలత = c.లేఖాచిత్రం.vertices_[coverage_id].పట్టిక_కొలత ();
    స్వయంచాలక& coverage_v = c.లేఖాచిత్రం.vertices_[coverage_id];
    స్థిరరాశి Coverage* coverage_పట్టిక = (స్థిరరాశి Coverage*) coverage_v.obj().తల_2;
    ఒకవేళ (unlikely (!coverage_పట్టిక))
      తిరిగి Err(INVALID_వాదం);
    TRY (coverage_పట్టిక->sanitize (coverage_v));

    స్వయంచాలక new_coverage =
        + hb_zip (coverage_పట్టిక->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<గుర్తులేని, గుర్తులేని> p) {
          తిరిగి p.second >= ప్రారంభం && p.second < చివర;
        })
        | hb_map_retains_క్రమీకరణ (hb_మొదటి)
        ;

    తిరిగి కూడు_coverage (c, new_మాతృ_id, link_position, new_coverage, coverage_కొలత);
  }

  మూస<రకం_పేరు It>
  స్థిర లేఖాచిత్రం_ఫలితం_t<శూన్య> కూడు_coverage (gsubgpos_లేఖాచిత్రం_context_t& c,
                                            గుర్తులేని మాతృ_id,
                                            గుర్తులేని link_position,
                                            It glyphs,
                                            గుర్తులేని max_size)
  {
    TRY_కేటాయించు (గుర్తులేని coverage_ప్రధానసంఖ్య_id, c.లేఖాచిత్రం.new_కణుపు (శూన్యసూచిక, శూన్యసూచిక));
    స్వయంచాలక& coverage_ప్రధానసంఖ్య_శీర్షం = c.లేఖాచిత్రం.vertices_[coverage_ప్రధానసంఖ్య_id];
    TRY (make_coverage (c, glyphs, coverage_ప్రధానసంఖ్య_id, max_size));

    TRY(c.లేఖాచిత్రం.vertices_[మాతృ_id].కూడు_real_link (SmallTypes::కొలత, coverage_ప్రధానసంఖ్య_id, link_position));
    TRY(coverage_ప్రధానసంఖ్య_శీర్షం.కూడు_మాతృ (మాతృ_id, అబద్ధం));

    తిరిగి Ok();
  }

  // Filter an existing coverage table to glyphs at indices [start, end) and replace it with the filtered version.
  స్థిర లేఖాచిత్రం_ఫలితం_t<శూన్య> filter_coverage (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                గుర్తులేని existing_coverage,
                                                గుర్తులేని ప్రారంభం, గుర్తులేని చివర) {
    గుర్తులేని coverage_కొలత = c.లేఖాచిత్రం.vertices_[existing_coverage].పట్టిక_కొలత ();
    స్వయంచాలక& coverage_v = c.లేఖాచిత్రం.vertices_[existing_coverage];
    స్థిరరాశి Coverage* coverage_పట్టిక = (స్థిరరాశి Coverage*) coverage_v.obj().తల_2;
    ఒకవేళ (unlikely (!coverage_పట్టిక))
      తిరిగి Err(INVALID_వాదం);
    TRY (coverage_పట్టిక->sanitize (coverage_v));

    స్వయంచాలక new_coverage =
        + hb_zip (coverage_పట్టిక->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<గుర్తులేని, గుర్తులేని> p) {
          తిరిగి p.second >= ప్రారంభం && p.second < చివర;
        })
        | hb_map_retains_క్రమీకరణ (hb_మొదటి)
        ;

    తిరిగి make_coverage (c, new_coverage, existing_coverage, coverage_కొలత * 2 + 100);
  }

  // Replace the coverage table at dest obj with one covering 'glyphs'.
  మూస<రకం_పేరు It>
  స్థిర లేఖాచిత్రం_ఫలితం_t<శూన్య> make_coverage (gsubgpos_లేఖాచిత్రం_context_t& c,
                                             It glyphs,
                                             గుర్తులేని dest_obj,
                                             గుర్తులేని max_size)
  {
    అక్షరం* బఫరు = (అక్షరం*) hb_calloc (1, max_size);
    ఒకవేళ (unlikely (!బఫరు))
      తిరిగి Err(కేటాయింపు_FAILURE);

    hb_serialize_context_t serializer (బఫరు, max_size);
    OT::Layout::Common::Coverage_serialize (&serializer, glyphs);
    serializer.చివర_serialize ();
    ఒకవేళ (unlikely (serializer.లోపల_error ()))
    {
      hb_free (బఫరు);
      తిరిగి Err(కేటాయింపు_FAILURE);
    }

    hb_బైట్లు_t coverage_copy = serializer.copy_బైట్లు ();
    ఒకవేళ (unlikely (!coverage_copy.arrayZ)) {
      hb_free (బఫరు);
      తిరిగి Err(కేటాయింపు_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    స్వయంచాలక res = c.కూడు_బఫరు ((అక్షరం *) coverage_copy.arrayZ);
    ఒకవేళ (unlikely (!res.is_ok ()))
    {
      hb_free (బఫరు);
      hb_free ((అక్షరం *) coverage_copy.arrayZ);
      తిరిగి res;
    }

    స్వయంచాలక& v = c.లేఖాచిత్రం.vertices_[dest_obj];
    అక్షరం* తల_2 = (అక్షరం *) coverage_copy.arrayZ;
    v.సమితి_బఫరు (తల_2, తల_2 + coverage_copy.నిడివి);

    hb_free (బఫరు);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (శీర్షం_len < OT::Layout::Common::Coverage::min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();
    ఎంపిక (u.format.v)
    {
    స్థితి 1: తిరిగి లేఖాచిత్రం::sanitize ((స్థిరరాశి OT::Layout::Common::CoverageFormat1_3<OT::Layout::SmallTypes>*) ఇది, శీర్షం);
    స్థితి 2: తిరిగి లేఖాచిత్రం::sanitize ((స్థిరరాశి OT::Layout::Common::CoverageFormat2_4<OT::Layout::SmallTypes>*) ఇది, శీర్షం);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: తిరిగి లేఖాచిత్రం::sanitize ((స్థిరరాశి OT::Layout::Common::CoverageFormat1_3<OT::Layout::MediumTypes>*) ఇది, శీర్షం);
    స్థితి 4: తిరిగి లేఖాచిత్రం::sanitize ((స్థిరరాశి OT::Layout::Common::CoverageFormat2_4<OT::Layout::MediumTypes>*) ఇది, శీర్షం);
#అయితే_ముగింపు
    అప్రమేయ: తిరిగి Err(SANITIZE_FAILURE);
    }
  }
};


}

#అయితే_ముగింపు  // GRAPH_COVERAGE_GRAPH_HH
