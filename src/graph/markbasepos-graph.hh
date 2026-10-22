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

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_MARKBASEPOS_లేఖాచిత్రం_HH
#నిర్వచించు లేఖాచిత్రం_MARKBASEPOS_లేఖాచిత్రం_HH

#అన్తర్భావ "split-helpers.hh"
#అన్తర్భావ "coverage-graph.hh"
#అన్తర్భావ "../OT/Layout/GPOS/MarkBasePos.hh"
#అన్తర్భావ "../OT/Layout/GPOS/PosLookupSubTable.hh"

నామాకాశ లేఖాచిత్రం {

నిర్మాణం Anchor_మాతృక : బహిరంగ OT::Layout::GPOS_impl::Anchor_మాతృక<SmallTypes>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం, గుర్తులేని class_లెక్క) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత();
    ఒకవేళ (unlikely (శీర్షం_len < Anchor_మాతృక::min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    ఒకవేళ (unlikely (శీర్షం_len < Anchor_మాతృక::min_కొలత +
                  OT::Offset16::static_కొలత * class_లెక్క * ఇది->అడ్డువరుసలు))
      తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (gsubgpos_లేఖాచిత్రం_context_t& c,
                               గుర్తులేని this_సూచీ,
                               గుర్తులేని old_class_లెక్క,
                               గుర్తులేని new_class_లెక్క)
  {
    ఒకవేళ (unlikely (new_class_లెక్క >= old_class_లెక్క)) తిరిగి Err(INVALID_వాదం);
    స్వయంచాలక& v = c.లేఖాచిత్రం.vertices_[this_సూచీ];
    స్థిరరాశి స్వయంచాలక& o = v.obj ();
    గుర్తులేని పునాది_లెక్క = అడ్డువరుసలు;
    v.సమితి_బఫరు (o.తల_2,
      o.తల_2 +
      Anchor_మాతృక::min_కొలత +
      OT::Offset16::static_కొలత * పునాది_లెక్క * new_class_లెక్క);

    // Reposition links into the new indexing scheme.
    కొరకు (స్వయంచాలక& link : v.real_links_writer ())
    {
      గుర్తులేని సూచీ_2 = (link.position - 2) / 2;
      గుర్తులేని పునాది = సూచీ_2 / old_class_లెక్క;
      గుర్తులేని klass = సూచీ_2 % old_class_లెక్క;
      ఒకవేళ (unlikely (klass >= new_class_లెక్క))
        // should have already been removed
        తిరిగి Err(INVALID_వాదం);

      గుర్తులేని new_సూచీ = పునాది * new_class_లెక్క + klass;

      link.position = (అక్షరం*) &(ఇది->మాతృక_Z[new_సూచీ]) - (అక్షరం*) ఇది;
    }

    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone (gsubgpos_లేఖాచిత్రం_context_t& c,
                                  గుర్తులేని this_సూచీ,
                                  గుర్తులేని ప్రారంభం,
                                  గుర్తులేని చివర,
                                  గుర్తులేని class_లెక్క)
  {
    గుర్తులేని పునాది_లెక్క = అడ్డువరుసలు;
    గుర్తులేని new_class_లెక్క = చివర - ప్రారంభం;
    గుర్తులేని కొలత = Anchor_మాతృక::min_కొలత +
                    OT::Offset16::static_కొలత * new_class_లెక్క * అడ్డువరుసలు;
    TRY_కేటాయించు (గుర్తులేని ప్రధానసంఖ్య_id, c.create_కణుపు (కొలత));
    Anchor_మాతృక* ప్రధానసంఖ్య = (Anchor_మాతృక*) c.లేఖాచిత్రం.object (ప్రధానసంఖ్య_id).తల_2;
    ప్రధానసంఖ్య->అడ్డువరుసలు = పునాది_లెక్క;

    స్వయంచాలక& v = c.లేఖాచిత్రం.vertices_[this_సూచీ];
    స్థిరరాశి స్వయంచాలక& o = v.obj ();

    పూర్ణాంకం అంకె_links = o.real_links.నిడివి;
    కొరకు (పూర్ణాంకం i = 0; i < అంకె_links; i++)
    {
      స్థిరరాశి స్వయంచాలక& link = o.real_links[i];
      గుర్తులేని old_సూచీ = (link.position - 2) / OT::Offset16::static_కొలత;
      గుర్తులేని klass = old_సూచీ % class_లెక్క;
      ఒకవేళ (klass < ప్రారంభం || klass >= చివర) కొనసాగు;

      గుర్తులేని పునాది = old_సూచీ / class_లెక్క;
      గుర్తులేని new_klass = klass - ప్రారంభం;
      గుర్తులేని new_సూచీ = పునాది * new_class_లెక్క + new_klass;


      గుర్తులేని బిడ్డ_idx = link.objidx;
      TRY (c.లేఖాచిత్రం.కూడు_link (&(ప్రధానసంఖ్య->మాతృక_Z[new_సూచీ]),
                             ప్రధానసంఖ్య_id,
                             బిడ్డ_idx));

      స్వయంచాలక& బిడ్డ = c.లేఖాచిత్రం.vertices_[బిడ్డ_idx];
      బిడ్డ.remove_మాతృ (this_సూచీ);

      v.remove_real_link_unordered (i);
      అంకె_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.క్రమీకరించు_real_links ();

    తిరిగి Ok(ప్రధానసంఖ్య_id);
  }
};

నిర్మాణం MarkArray : బహిరంగ OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత();
    గుర్తులేని min_కొలత = MarkArray::min_కొలత;
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    ఒకవేళ (unlikely (శీర్షం_len < get_కొలత ())) తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (gsubgpos_లేఖాచిత్రం_context_t& c,
                               స్థిరరాశి hb_hashmap_t<గుర్తులేని, గుర్తులేని>& mark_array_links,
                               గుర్తులేని this_సూచీ,
                               గుర్తులేని new_class_లెక్క)
  {
    స్వయంచాలక& v = c.లేఖాచిత్రం.vertices_[this_సూచీ];
    స్థిరరాశి స్వయంచాలక& o = v.obj ();
    కొరకు (స్థిరరాశి స్వయంచాలక& link : o.real_links)
      c.లేఖాచిత్రం.vertices_[link.objidx].remove_మాతృ (this_సూచీ);
    v.clear_real_links ();

    గుర్తులేని new_సూచీ = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& రికార్డు : ఇది->iter ())
    {
      గుర్తులేని klass = రికార్డు.klass;
      ఒకవేళ (klass >= new_class_లెక్క) కొనసాగు;

      (*ఇది)[new_సూచీ].klass = klass;
      గుర్తులేని position = (అక్షరం*) &రికార్డు.markAnchor - (అక్షరం*) ఇది;
      గుర్తులేని* objidx;
      ఒకవేళ (!mark_array_links.has (position, &objidx))
      {
        new_సూచీ++;
        కొనసాగు;
      }

      TRY (c.లేఖాచిత్రం.కూడు_link (&(*ఇది)[new_సూచీ].markAnchor, this_సూచీ, *objidx));
      new_సూచీ++;
    }

    ఇది->len = new_సూచీ;
    v.సమితి_బఫరు (o.తల_2, o.తల_2 + MarkArray::min_కొలత +
                  OT::Layout::GPOS_impl::Mark_రికార్డు<SmallTypes>::static_కొలత * new_సూచీ);

    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone (gsubgpos_లేఖాచిత్రం_context_t& c,
                                  గుర్తులేని this_సూచీ,
                                  స్థిరరాశి hb_hashmap_t<గుర్తులేని, గుర్తులేని>& pos_గమ్యం_సూచీ,
                                  hb_సమితి_t& marks,
                                  గుర్తులేని ప్రారంభం_class)
  {
    గుర్తులేని కొలత = MarkArray::min_కొలత +
                    OT::Layout::GPOS_impl::Mark_రికార్డు<SmallTypes>::static_కొలత *
                    marks.get_population ();
    TRY_కేటాయించు (గుర్తులేని ప్రధానసంఖ్య_id, c.create_కణుపు (కొలత));
    MarkArray* ప్రధానసంఖ్య = (MarkArray*) c.లేఖాచిత్రం.object (ప్రధానసంఖ్య_id).తల_2;
    ప్రధానసంఖ్య->len = marks.get_population ();


    గుర్తులేని i = 0;
    కొరకు (hb_codepoint_t mark : marks)
    {
      (*ప్రధానసంఖ్య)[i].klass = (*ఇది)[mark].klass - ప్రారంభం_class;
      గుర్తులేని స్థానభ్రంశం_pos = (అక్షరం*) &((*ఇది)[mark].markAnchor) - (అక్షరం*) ఇది;
      గుర్తులేని* anchor_సూచీ;
      ఒకవేళ (pos_గమ్యం_సూచీ.has (స్థానభ్రంశం_pos, &anchor_సూచీ))
        TRY (c.లేఖాచిత్రం.move_బిడ్డ (this_సూచీ,
                                 &((*ఇది)[mark].markAnchor),
                                 ప్రధానసంఖ్య_id,
                                 &((*ప్రధానసంఖ్య)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.లేఖాచిత్రం.vertices_[this_సూచీ].క్రమీకరించు_real_links ();

    తిరిగి Ok(ప్రధానసంఖ్య_id);
  }
};

నిర్మాణం Mark_పునాది_Pos_Format1 : బహిరంగ OT::Layout::GPOS_impl::Mark_పునాది_Pos_Format1_2<SmallTypes>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (శీర్షం_len < Mark_పునాది_Pos_Format1::static_కొలత))
      తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> విభజించు_subtables (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                         గుర్తులేని this_సూచీ)
  {
    hb_సమితి_t సందర్శించిన;

    TRY_కేటాయించు (స్థిరరాశి గుర్తులేని పునాది_coverage_id, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &పునాది_Coverage));

    స్థిరరాశి గుర్తులేని పునాది_కొలత =
        OT::Layout::GPOS_impl::Mark_పునాది_Pos_Format1_2<SmallTypes>::min_కొలత +
        MarkArray::min_కొలత +
        Anchor_మాతృక::min_కొలత +
        c.లేఖాచిత్రం.vertices_[పునాది_coverage_id].పట్టిక_కొలత ();

    TRY_కేటాయించు (hb_సదిశ_t<class_info_t> class_గమ్యం_info, get_class_info (c, this_సూచీ));

    గుర్తులేని class_లెక్క = class_లెక్క_2;
    TRY_కేటాయించు (స్వయంచాలక పునాది_array, c.లేఖాచిత్రం.as_పట్టిక<Anchor_మాతృక> (this_సూచీ,
                                                          &పునాది_Array,
                                                          class_లెక్క));
    గుర్తులేని పునాది_లెక్క = పునాది_array.పట్టిక->అడ్డువరుసలు;

    గుర్తులేని partial_coverage_కొలత = 4;
    గుర్తులేని accumulated = పునాది_కొలత;
    hb_సదిశ_t<గుర్తులేని> విభజించు_points;

    కొరకు (గుర్తులేని klass = 0; klass < class_లెక్క; klass++)
    {
      class_info_t& info = class_గమ్యం_info[klass];
      partial_coverage_కొలత += OT::HBUINT16::static_కొలత * info.marks.get_population ();
      గుర్తులేని accumulated_delta =
          OT::Layout::GPOS_impl::Mark_రికార్డు<SmallTypes>::static_కొలత * info.marks.get_population () +
          OT::Offset16::static_కొలత * పునాది_లెక్క;

      కొరకు (గుర్తులేని objidx : info.బిడ్డ_indices)
        accumulated_delta += c.లేఖాచిత్రం.కనుగొను_subgraph_కొలత (objidx, సందర్శించిన).విలువ_or (0);

      accumulated += accumulated_delta;
      గుర్తులేని మొత్తం = accumulated + partial_coverage_కొలత;

      ఒకవేళ (మొత్తం >= (1 << 16))
      {
        విభజించు_points.push (klass);
        TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (విభజించు_points, కేటాయింపు_FAILURE));
        accumulated = పునాది_కొలత + accumulated_delta;
        partial_coverage_కొలత = 4 + OT::HBUINT16::static_కొలత * info.marks.get_population ();
        సందర్శించిన.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_కేటాయించు (స్థిరరాశి గుర్తులేని mark_array_id, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &markArray));
    TRY_కేటాయించు (స్వయంచాలక mark_array_links, c.లేఖాచిత్రం.vertices_[mark_array_id].position_గమ్యం_సూచీ_map ());

    విభజించు_context_t విభజించు_context {
      c,
      ఇది,
      this_సూచీ,
      std::move (class_గమ్యం_info),
      std::move (mark_array_links),
    };

    తిరిగి actuate_subtable_విభజించు<విభజించు_context_t> (విభజించు_context, విభజించు_points);
  }

 అంతరంగ:

  నిర్మాణం class_info_t {
    hb_సమితి_t marks;
    hb_సదిశ_t<గుర్తులేని> బిడ్డ_indices;
  };

  నిర్మాణం విభజించు_context_t {
    gsubgpos_లేఖాచిత్రం_context_t& c;
    Mark_పునాది_Pos_Format1* thiz;
    గుర్తులేని this_సూచీ;
    hb_సదిశ_t<class_info_t> class_గమ్యం_info;
    hb_hashmap_t<గుర్తులేని, గుర్తులేని> mark_array_links;

    hb_సమితి_t marks_for (గుర్తులేని ప్రారంభం, గుర్తులేని చివర)
    {
      hb_సమితి_t marks;
      కొరకు (గుర్తులేని klass = ప్రారంభం; klass < చివర; klass++)
      {
        + class_గమ్యం_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      తిరిగి marks;
    }

    గుర్తులేని original_లెక్క_2 ()
    {
      తిరిగి thiz->class_లెక్క_2;
    }

    లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone_range (గుర్తులేని ప్రారంభం, గుర్తులేని చివర)
    {
      తిరిగి thiz->clone_range (*ఇది, ఇది->this_సూచీ, ప్రారంభం, చివర);
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (గుర్తులేని లెక్క)
    {
      తిరిగి thiz->shrink (*ఇది, ఇది->this_సూచీ, లెక్క);
    }
  };

  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<class_info_t>> get_class_info (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                            గుర్తులేని this_సూచీ)
  {
    hb_సదిశ_t<class_info_t> class_గమ్యం_info;

    గుర్తులేని class_లెక్క = class_లెక్క_2;
    ఒకవేళ (!class_లెక్క) తిరిగి class_గమ్యం_info;

    ఒకవేళ (unlikely (!class_గమ్యం_info.resize (class_లెక్క)))
      తిరిగి Err(కేటాయింపు_FAILURE);

    TRY_కేటాయించు (స్వయంచాలక mark_array, c.లేఖాచిత్రం.as_పట్టిక<MarkArray> (this_సూచీ, &markArray));

    గుర్తులేని mark_లెక్క = mark_array.పట్టిక->len;
    కొరకు (గుర్తులేని mark = 0; mark < mark_లెక్క; mark++)
    {
      గుర్తులేని klass = (*mark_array.పట్టిక)[mark].get_class ();
      ఒకవేళ (klass >= class_లెక్క) కొనసాగు;
      class_గమ్యం_info[klass].marks.కూడు (mark);
    }

    కొరకు (స్థిరరాశి స్వయంచాలక& link : mark_array.శీర్షం->obj ().real_links)
    {
      గుర్తులేని mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::Mark_రికార్డు<SmallTypes>::static_కొలత;
      గుర్తులేని klass = (*mark_array.పట్టిక)[mark].get_class ();
      ఒకవేళ (klass >= class_లెక్క) కొనసాగు;
      class_గమ్యం_info[klass].బిడ్డ_indices.push (link.objidx);
    }

    TRY_కేటాయించు (గుర్తులేని పునాది_array_id,
         c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &పునాది_Array));

    స్వయంచాలక& పునాది_array_v = c.లేఖాచిత్రం.vertices_[పునాది_array_id];

    కొరకు (స్థిరరాశి స్వయంచాలక& link : పునాది_array_v.obj ().real_links)
    {
      గుర్తులేని సూచీ_2 = (link.position - 2) / OT::Offset16::static_కొలత;
      గుర్తులేని klass = సూచీ_2 % class_లెక్క;
      class_గమ్యం_info[klass].బిడ్డ_indices.push (link.objidx);
    }

    తిరిగి class_గమ్యం_info;
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (విభజించు_context_t& sc,
                               గుర్తులేని this_సూచీ,
                               గుర్తులేని లెక్క)
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_సూచీ,
               లెక్క);

    గుర్తులేని old_లెక్క = class_లెక్క_2;
    ఒకవేళ (లెక్క >= old_లెక్క)
      తిరిగి Ok();

    class_లెక్క_2 = లెక్క;

    TRY_కేటాయించు (స్వయంచాలక mark_coverage, sc.c.లేఖాచిత్రం.as_mutable_పట్టిక<Coverage> (this_సూచీ,
                                                                    &markCoverage));

    hb_సమితి_t marks = sc.marks_for (0, లెక్క);
    స్వయంచాలక new_coverage =
        + hb_enumerate (mark_coverage.పట్టిక->iter ())
        | hb_filter (marks, hb_మొదటి)
        | hb_map_retains_క్రమీకరణ (hb_రెండవ)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.సూచీ_2,
                                  4 + 2 * marks.get_population ()));


    TRY_కేటాయించు (స్వయంచాలక పునాది_array, sc.c.లేఖాచిత్రం.as_mutable_పట్టిక<Anchor_మాతృక> (this_సూచీ,
                                                                     &పునాది_Array,
                                                                     old_లెక్క));

    TRY (పునాది_array.పట్టిక->shrink (sc.c,
                                   పునాది_array.సూచీ_2,
                                   old_లెక్క,
                                   లెక్క));

    TRY_కేటాయించు (స్వయంచాలక mark_array, sc.c.లేఖాచిత్రం.as_mutable_పట్టిక<MarkArray> (this_సూచీ,
                                                                  &markArray));

    TRY (mark_array.పట్టిక->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.సూచీ_2,
                                   లెక్క));

    తిరిగి Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone_range (విభజించు_context_t& sc,
                                        గుర్తులేని this_సూచీ,
                                        గుర్తులేని ప్రారంభం, గుర్తులేని చివర) స్థిరరాశి
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_సూచీ, ప్రారంభం, చివర);

    లేఖాచిత్రం_t& లేఖాచిత్రం = sc.c.లేఖాచిత్రం;
    గుర్తులేని ప్రధానసంఖ్య_కొలత = OT::Layout::GPOS_impl::Mark_పునాది_Pos_Format1_2<SmallTypes>::static_కొలత;

    TRY_కేటాయించు (గుర్తులేని ప్రధానసంఖ్య_id, sc.c.create_కణుపు (ప్రధానసంఖ్య_కొలత));

    Mark_పునాది_Pos_Format1* ప్రధానసంఖ్య = (Mark_పునాది_Pos_Format1*) లేఖాచిత్రం.object (ప్రధానసంఖ్య_id).తల_2;
    ప్రధానసంఖ్య->format = ఇది->format;
    గుర్తులేని new_class_లెక్క = చివర - ప్రారంభం;
    ప్రధానసంఖ్య->class_లెక్క_2 = new_class_లెక్క;

    TRY_కేటాయించు (గుర్తులేని పునాది_coverage_id,
         లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (sc.this_సూచీ, &పునాది_Coverage));

    TRY (లేఖాచిత్రం.కూడు_link (&(ప్రధానసంఖ్య->పునాది_Coverage), ప్రధానసంఖ్య_id, పునాది_coverage_id));

    TRY_కేటాయించు (స్వయంచాలక mark_coverage, sc.c.లేఖాచిత్రం.as_పట్టిక<Coverage> (this_సూచీ,
                                                            &markCoverage));

    hb_సమితి_t marks = sc.marks_for (ప్రారంభం, చివర);
    స్వయంచాలక new_coverage =
        + hb_enumerate (mark_coverage.పట్టిక->iter ())
        | hb_filter (marks, hb_మొదటి)
        | hb_map_retains_క్రమీకరణ (hb_రెండవ)
        ;
    TRY (Coverage::కూడు_coverage (sc.c,
                                 ప్రధానసంఖ్య_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_కేటాయించు (స్వయంచాలక mark_array,
         లేఖాచిత్రం.as_mutable_పట్టిక <MarkArray> (sc.this_సూచీ, &markArray));

    TRY_కేటాయించు (గుర్తులేని new_mark_array,
         mark_array.పట్టిక->clone (sc.c,
                                  mark_array.సూచీ_2,
                                  sc.mark_array_links,
                                  marks,
                                  ప్రారంభం));
    TRY (లేఖాచిత్రం.కూడు_link (&(ప్రధానసంఖ్య->markArray), ప్రధానసంఖ్య_id, new_mark_array));

    గుర్తులేని class_లెక్క = class_లెక్క_2;
    TRY_కేటాయించు (స్వయంచాలక పునాది_array,
         లేఖాచిత్రం.as_mutable_పట్టిక<Anchor_మాతృక> (sc.this_సూచీ, &పునాది_Array, class_లెక్క));

    TRY_కేటాయించు (గుర్తులేని new_పునాది_array,
         పునాది_array.పట్టిక->clone (sc.c,
                                  పునాది_array.సూచీ_2,
                                  ప్రారంభం, చివర, ఇది->class_లెక్క_2));
    TRY (లేఖాచిత్రం.కూడు_link (&(ప్రధానసంఖ్య->పునాది_Array), ప్రధానసంఖ్య_id, new_పునాది_array));

    తిరిగి Ok(ప్రధానసంఖ్య_id);
  }
};


నిర్మాణం Mark_పునాది_Pos : బహిరంగ OT::Layout::GPOS_impl::Mark_పునాది_Pos
{
  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> విభజించు_subtables (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                         గుర్తులేని this_సూచీ)
  {
    ఎంపిక (u.format.v) {
    స్థితి 1:
      hb_barrier ();
      తిరిగి ((Mark_పునాది_Pos_Format1*)(&u.format1))->విభజించు_subtables (c, this_సూచీ);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#అయితే_ముగింపు
    అప్రమేయ:
      తిరిగి Ok(hb_సదిశ_t<గుర్తులేని> ());
    }
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    ఒకవేళ (unlikely (శీర్షం_len < u.format.v.get_కొలత ())) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    ఎంపిక (u.format.v) {
    స్థితి 1:
      hb_barrier ();
      తిరిగి ((Mark_పునాది_Pos_Format1*)(&u.format1))->sanitize (శీర్షం);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 2: HB_FALLTHROUGH;
#అయితే_ముగింపు
    అప్రమేయ:
      // We don't handle format 3 and 4 here.
      తిరిగి Err(SANITIZE_FAILURE);
    }
  }
};


}

#అయితే_ముగింపు  // GRAPH_MARKBASEPOS_GRAPH_HH
