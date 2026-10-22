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

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_PAIRPOS_లేఖాచిత్రం_HH
#నిర్వచించు లేఖాచిత్రం_PAIRPOS_లేఖాచిత్రం_HH

#అన్తర్భావ "split-helpers.hh"
#అన్తర్భావ "coverage-graph.hh"
#అన్తర్భావ "classdef-graph.hh"
#అన్తర్భావ "../OT/Layout/GPOS/PairPos.hh"
#అన్తర్భావ "../OT/Layout/GPOS/PosLookupSubTable.hh"

నామాకాశ లేఖాచిత్రం {

నిర్మాణం PairPosFormat1 : బహిరంగ OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    గుర్తులేని min_కొలత = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_కొలత;
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత + pair_సమితి.get_కొలత () - pair_సమితి.len.get_కొలత()))
      తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> విభజించు_subtables (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                         గుర్తులేని this_సూచీ)
  {
    hb_సమితి_t సందర్శించిన;

    TRY_కేటాయించు (స్థిరరాశి గుర్తులేని coverage_id, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &coverage));
    స్థిరరాశి గుర్తులేని coverage_కొలత = c.లేఖాచిత్రం.vertices_[coverage_id].పట్టిక_కొలత ();
    స్థిరరాశి గుర్తులేని పునాది_కొలత = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_కొలత;

    గుర్తులేని partial_coverage_కొలత = 4;
    గుర్తులేని accumulated = పునాది_కొలత;
    hb_సదిశ_t<గుర్తులేని> విభజించు_points;
    కొరకు (గుర్తులేని i = 0; i < pair_సమితి.len; i++)
    {
      TRY_కేటాయించు (గుర్తులేని pair_సమితి_సూచీ, pair_సమితి_లేఖాచిత్రం_సూచీ (c, this_సూచీ, i));
      గుర్తులేని accumulated_delta =
          c.లేఖాచిత్రం.కనుగొను_subgraph_కొలత (pair_సమితి_సూచీ, సందర్శించిన).విలువ_or (0) +
          SmallTypes::కొలత; // for PairSet offset.
      partial_coverage_కొలత += OT::HBUINT16::static_కొలత;

      accumulated += accumulated_delta;
      గుర్తులేని మొత్తం = accumulated + hb_min (partial_coverage_కొలత, coverage_కొలత);

      ఒకవేళ (మొత్తం >= (1 << 16))
      {
        విభజించు_points.push (i);
        TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (విభజించు_points, కేటాయింపు_FAILURE));
        accumulated = పునాది_కొలత + accumulated_delta;
        partial_coverage_కొలత = 6;
        సందర్శించిన.clear (); // node sharing isn't allowed between splits.
      }
    }

    విభజించు_context_t విభజించు_context {
      c,
      ఇది,
      this_సూచీ,
    };

    తిరిగి actuate_subtable_విభజించు<విభజించు_context_t> (విభజించు_context, విభజించు_points);
  }

 అంతరంగ:

  నిర్మాణం విభజించు_context_t {
    gsubgpos_లేఖాచిత్రం_context_t& c;
    PairPosFormat1* thiz;
    గుర్తులేని this_సూచీ;

    గుర్తులేని original_లెక్క_2 ()
    {
      తిరిగి thiz->pair_సమితి.len;
    }

    లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone_range (గుర్తులేని ప్రారంభం, గుర్తులేని చివర)
    {
      తిరిగి thiz->clone_range (ఇది->c, ఇది->this_సూచీ, ప్రారంభం, చివర);
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (గుర్తులేని లెక్క)
    {
      తిరిగి thiz->shrink (ఇది->c, ఇది->this_సూచీ, లెక్క);
    }
  };

  లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (gsubgpos_లేఖాచిత్రం_context_t& c,
                               గుర్తులేని this_సూచీ,
                               గుర్తులేని లెక్క)
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_సూచీ,
               లెక్క);
    గుర్తులేని old_లెక్క = pair_సమితి.len;
    ఒకవేళ (లెక్క >= old_లెక్క)
      తిరిగి Ok();

    pair_సమితి.len = లెక్క;
    c.లేఖాచిత్రం.vertices_[this_సూచీ].shrink_బఫరు ((old_లెక్క - లెక్క) * SmallTypes::కొలత);

    TRY_కేటాయించు (స్వయంచాలక coverage, c.లేఖాచిత్రం.as_mutable_పట్టిక<Coverage> (this_సూచీ, &ఇది->coverage));

    గుర్తులేని coverage_కొలత = coverage.శీర్షం->పట్టిక_కొలత ();
    స్వయంచాలక new_coverage =
        + hb_zip (coverage.పట్టిక->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<గుర్తులేని, గుర్తులేని> p) {
          తిరిగి p.second < లెక్క;
        })
        | hb_map_retains_క్రమీకరణ (hb_మొదటి)
        ;

    తిరిగి Coverage::make_coverage (c, new_coverage, coverage.సూచీ_2, coverage_కొలత);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone_range (gsubgpos_లేఖాచిత్రం_context_t& c,
                                        గుర్తులేని this_సూచీ,
                                        గుర్తులేని ప్రారంభం, గుర్తులేని చివర) స్థిరరాశి
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_సూచీ, ప్రారంభం, చివర);

    గుర్తులేని అంకె_pair_sets = చివర - ప్రారంభం;
    గుర్తులేని ప్రధానసంఖ్య_కొలత = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_కొలత
                          + అంకె_pair_sets * SmallTypes::కొలత;

    TRY_కేటాయించు (గుర్తులేని pair_pos_ప్రధానసంఖ్య_id, c.create_కణుపు (ప్రధానసంఖ్య_కొలత));

    PairPosFormat1* pair_pos_ప్రధానసంఖ్య = (PairPosFormat1*) c.లేఖాచిత్రం.object (pair_pos_ప్రధానసంఖ్య_id).తల_2;
    pair_pos_ప్రధానసంఖ్య->format = ఇది->format;
    pair_pos_ప్రధానసంఖ్య->విలువ_Format_2[0] = ఇది->విలువ_Format_2[0];
    pair_pos_ప్రధానసంఖ్య->విలువ_Format_2[1] = ఇది->విలువ_Format_2[1];
    pair_pos_ప్రధానసంఖ్య->pair_సమితి.len = అంకె_pair_sets;

    కొరకు (గుర్తులేని i = ప్రారంభం; i < చివర; i++)
    {
      TRY (c.లేఖాచిత్రం.move_బిడ్డ<> (this_సూచీ,
                                 &pair_సమితి[i],
                                 pair_pos_ప్రధానసంఖ్య_id,
                                 &pair_pos_ప్రధానసంఖ్య->pair_సమితి[i - ప్రారంభం]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.లేఖాచిత్రం.vertices_[this_సూచీ].క్రమీకరించు_real_links ();

    TRY_కేటాయించు (గుర్తులేని coverage_id, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_ప్రధానసంఖ్య_id,
                                   2,
                                   ప్రారంభం, చివర));

    తిరిగి Ok(pair_pos_ప్రధానసంఖ్య_id);
  }



  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> pair_సమితి_లేఖాచిత్రం_సూచీ (gsubgpos_లేఖాచిత్రం_context_t& c, గుర్తులేని this_సూచీ, గుర్తులేని i) స్థిరరాశి
  {
    తిరిగి c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &pair_సమితి[i]);
  }
};

నిర్మాణం PairPosFormat2 : బహిరంగ OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత ();
    గుర్తులేని min_కొలత = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_కొలత;
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత)) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    స్థిరరాశి గుర్తులేని class1_లెక్క = class1_లెక్క_2;
    ఒకవేళ (unlikely (శీర్షం_len < min_కొలత + class1_లెక్క * get_class1_రికార్డు_కొలత ()))
      తిరిగి Err(SANITIZE_FAILURE);
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> విభజించు_subtables (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                         గుర్తులేని this_సూచీ)
  {
    స్థిరరాశి గుర్తులేని పునాది_కొలత = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_కొలత;
    TRY_కేటాయించు (స్థిరరాశి గుర్తులేని class_def_2_కొలత, కొలత_యొక్క (c, this_సూచీ, &classDef2));
    TRY_కేటాయించు (స్థిరరాశి Coverage* coverage, get_coverage (c, this_సూచీ));
    TRY_కేటాయించు (స్థిరరాశి ClassDef* class_def_1, get_class_def_1 (c, this_సూచీ));
    స్వయంచాలక gid_and_class =
        + coverage->iter ()
        | hb_map_retains_క్రమీకరణ ([&] (hb_codepoint_t gid) {
          తిరిగి hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_కేటాయించు (class_def_కొలత_estimator_t estimator, class_def_కొలత_estimator_t::create (gid_and_class));

    స్థిరరాశి గుర్తులేని class1_లెక్క = class1_లెక్క_2;
    స్థిరరాశి గుర్తులేని class2_లెక్క = class2_లెక్క_2;
    స్థిరరాశి గుర్తులేని class1_రికార్డు_కొలత = get_class1_రికార్డు_కొలత ();

    స్థిరరాశి గుర్తులేని విలువ_1_len = విలువ_Format1.get_len ();
    స్థిరరాశి గుర్తులేని విలువ_2_len = విలువ_Format2.get_len ();
    స్థిరరాశి గుర్తులేని మొత్తం_విలువ_len = విలువ_1_len + విలువ_2_len;

    గుర్తులేని accumulated = పునాది_కొలత;
    గుర్తులేని coverage_కొలత = 4;
    గుర్తులేని class_def_1_కొలత = 4;
    గుర్తులేని max_coverage_కొలత = coverage_కొలత;
    గుర్తులేని max_class_def_1_కొలత = class_def_1_కొలత;

    hb_సదిశ_t<గుర్తులేని> విభజించు_points;

    TRY_కేటాయించు (స్వయంచాలక device_tables, get_all_device_tables (c, this_సూచీ));
    hb_సదిశ_t<గుర్తులేని> format1_device_పట్టిక_indices = విలువ_Format1.get_device_పట్టిక_indices ();
    hb_సదిశ_t<గుర్తులేని> format2_device_పట్టిక_indices = విలువ_Format2.get_device_పట్టిక_indices ();
    తార్కిక has_device_tables = తార్కిక(format1_device_పట్టిక_indices) || తార్కిక(format2_device_పట్టిక_indices);

    hb_సమితి_t సందర్శించిన;
    కొరకు (గుర్తులేని i = 0; i < class1_లెక్క; i++)
    {
      గుర్తులేని accumulated_delta = class1_రికార్డు_కొలత;
      class_def_1_కొలత = estimator.కూడు_class_def_కొలత (i);
      coverage_కొలత = estimator.coverage_కొలత ();
      max_coverage_కొలత = hb_max (max_coverage_కొలత, coverage_కొలత);
      max_class_def_1_కొలత = hb_max (max_class_def_1_కొలత, class_def_1_కొలత);

      ఒకవేళ (has_device_tables) {
        కొరకు (గుర్తులేని j = 0; j < class2_లెక్క; j++)
        {
          గుర్తులేని value1_సూచీ = మొత్తం_విలువ_len * (class2_లెక్క * i + j);
          గుర్తులేని value2_సూచీ = value1_సూచీ + విలువ_1_len;
          accumulated_delta += కొలత_యొక్క_విలువ_రికార్డు_children (c,
                                                        device_tables,
                                                        format1_device_పట్టిక_indices,
                                                        value1_సూచీ,
                                                        సందర్శించిన);
          accumulated_delta += కొలత_యొక్క_విలువ_రికార్డు_children (c,
                                                        device_tables,
                                                        format2_device_పట్టిక_indices,
                                                        value2_సూచీ,
                                                        సందర్శించిన);
        }
      }

      accumulated += accumulated_delta;
      గుర్తులేని మొత్తం = accumulated
                       + coverage_కొలత + class_def_1_కొలత + class_def_2_కొలత
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_కొలత, class_def_1_కొలత), class_def_2_కొలత);
      ఒకవేళ (మొత్తం >= (1 << 16))
      {
        విభజించు_points.push (i);
        TRY (లేఖాచిత్రం_ఫలితం_t<శూన్య>::from (విభజించు_points, కేటాయింపు_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = పునాది_కొలత + accumulated_delta;

        estimator.reset();
        class_def_1_కొలత = estimator.కూడు_class_def_కొలత(i);
        coverage_కొలత = estimator.coverage_కొలత();
        సందర్శించిన.clear (); // node sharing isn't allowed between splits.
      }
    }

    విభజించు_context_t విభజించు_context {
      c,
      ఇది,
      this_సూచీ,
      class1_రికార్డు_కొలత,
      మొత్తం_విలువ_len,
      విలువ_1_len,
      విలువ_2_len,
      max_coverage_కొలత,
      max_class_def_1_కొలత,
      device_tables,
      format1_device_పట్టిక_indices,
      format2_device_పట్టిక_indices
    };

    తిరిగి actuate_subtable_విభజించు<విభజించు_context_t> (విభజించు_context, విభజించు_points);
  }
 అంతరంగ:

  నిర్మాణం విభజించు_context_t
  {
    gsubgpos_లేఖాచిత్రం_context_t& c;
    PairPosFormat2* thiz;
    గుర్తులేని this_సూచీ;
    గుర్తులేని class1_రికార్డు_కొలత;
    గుర్తులేని విలువ_రికార్డు_len;
    గుర్తులేని value1_రికార్డు_len;
    గుర్తులేని value2_రికార్డు_len;
    గుర్తులేని max_coverage_కొలత;
    గుర్తులేని max_class_def_కొలత;

    స్థిరరాశి hb_hashmap_t<గుర్తులేని, గుర్తులేని>& device_tables;
    స్థిరరాశి hb_సదిశ_t<గుర్తులేని>& format1_device_పట్టిక_indices;
    స్థిరరాశి hb_సదిశ_t<గుర్తులేని>& format2_device_పట్టిక_indices;

    గుర్తులేని original_లెక్క_2 ()
    {
      తిరిగి thiz->class1_లెక్క_2;
    }

    లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone_range (గుర్తులేని ప్రారంభం, గుర్తులేని చివర)
    {
      తిరిగి thiz->clone_range (*ఇది, ప్రారంభం, చివర);
    }

    లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (గుర్తులేని లెక్క)
    {
      తిరిగి thiz->shrink (*ఇది, లెక్క);
    }
  };

  size_t get_class1_రికార్డు_కొలత () స్థిరరాశి
  {
    స్థిరరాశి size_t class2_లెక్క = class2_లెక్క_2;
    తిరిగి
        class2_లెక్క * (విలువ_Format1.get_కొలత () + విలువ_Format2.get_కొలత ());
  }

  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> clone_range (విభజించు_context_t& విభజించు_context,
                                        గుర్తులేని ప్రారంభం, గుర్తులేని చివర) స్థిరరాశి
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", విభజించు_context.this_సూచీ, ప్రారంభం, చివర);

    లేఖాచిత్రం_t& లేఖాచిత్రం = విభజించు_context.c.లేఖాచిత్రం;

    గుర్తులేని అంకె_రికార్డులు = చివర - ప్రారంభం;
    గుర్తులేని ప్రధానసంఖ్య_కొలత = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_కొలత
                          + అంకె_రికార్డులు * విభజించు_context.class1_రికార్డు_కొలత;

    TRY_కేటాయించు (గుర్తులేని pair_pos_ప్రధానసంఖ్య_id, విభజించు_context.c.create_కణుపు (ప్రధానసంఖ్య_కొలత));

    PairPosFormat2* pair_pos_ప్రధానసంఖ్య =
        (PairPosFormat2*) లేఖాచిత్రం.object (pair_pos_ప్రధానసంఖ్య_id).తల_2;
    pair_pos_ప్రధానసంఖ్య->format = ఇది->format;
    pair_pos_ప్రధానసంఖ్య->విలువ_Format1 = ఇది->విలువ_Format1;
    pair_pos_ప్రధానసంఖ్య->విలువ_Format2 = ఇది->విలువ_Format2;
    pair_pos_ప్రధానసంఖ్య->class1_లెక్క_2 = అంకె_రికార్డులు;
    pair_pos_ప్రధానసంఖ్య->class2_లెక్క_2 = ఇది->class2_లెక్క_2;
    TRY (clone_class1_రికార్డులు (విభజించు_context,
                               pair_pos_ప్రధానసంఖ్య_id,
                               ప్రారంభం,
                               చివర));

    TRY_కేటాయించు (గుర్తులేని coverage_id,
         లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (విభజించు_context.this_సూచీ, &coverage));
    TRY_కేటాయించు (గుర్తులేని class_def_1_id,
         లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (విభజించు_context.this_సూచీ, &classDef1));

    స్వయంచాలక& coverage_v = లేఖాచిత్రం.vertices_[coverage_id];
    స్వయంచాలక& class_def_1_v = లేఖాచిత్రం.vertices_[class_def_1_id];
    స్థిరరాశి Coverage* coverage_పట్టిక = (స్థిరరాశి Coverage*) coverage_v.obj().తల_2;
    స్థిరరాశి ClassDef* class_def_1_పట్టిక = (స్థిరరాశి ClassDef*) class_def_1_v.obj().తల_2;
    ఒకవేళ (unlikely (!coverage_పట్టిక || !class_def_1_పట్టిక))
      తిరిగి Err(INVALID_వాదం);
    TRY (coverage_పట్టిక->sanitize (coverage_v));
    TRY (class_def_1_పట్టిక->sanitize (class_def_1_v));

    స్వయంచాలక klass_map =
    + coverage_పట్టిక->iter ()
    | hb_map_retains_క్రమీకరణ ([&] (hb_codepoint_t gid) {
      తిరిగి hb_codepoint_pair_t (gid, class_def_1_పట్టిక->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      తిరిగి klass >= ప్రారంభం && klass < చివర;
    }, hb_రెండవ)
    | hb_map_retains_క్రమీకరణ ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      తిరిగి hb_codepoint_pair_t (gid_and_class.మొదటి, gid_and_class.second - ప్రారంభం);
    })
    ;

    TRY (Coverage::కూడు_coverage (విభజించు_context.c,
                                 pair_pos_ప్రధానసంఖ్య_id,
                                 2,
                                 + klass_map | hb_map_retains_క్రమీకరణ (hb_మొదటి),
                                 విభజించు_context.max_coverage_కొలత));

    // classDef1
    TRY (ClassDef::కూడు_class_def (విభజించు_context.c,
                                  pair_pos_ప్రధానసంఖ్య_id,
                                  8,
                                  + klass_map,
                                  విభజించు_context.max_class_def_కొలత));

    // classDef2
    TRY_కేటాయించు (గుర్తులేని class_def_2_id,
         లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (విభజించు_context.this_సూచీ, &classDef2));

    TRY(లేఖాచిత్రం.vertices_[pair_pos_ప్రధానసంఖ్య_id].కూడు_real_link (SmallTypes::కొలత, class_def_2_id, 10));
    TRY(లేఖాచిత్రం.vertices_[class_def_2_id].కూడు_మాతృ (pair_pos_ప్రధానసంఖ్య_id, అబద్ధం));

    తిరిగి Ok(pair_pos_ప్రధానసంఖ్య_id);
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> clone_class1_రికార్డులు (విభజించు_context_t& విభజించు_context,
                                             గుర్తులేని pair_pos_ప్రధానసంఖ్య_id,
                                             గుర్తులేని ప్రారంభం, గుర్తులేని చివర) స్థిరరాశి
  {
    PairPosFormat2* pair_pos_ప్రధానసంఖ్య =
        (PairPosFormat2*) విభజించు_context.c.లేఖాచిత్రం.object (pair_pos_ప్రధానసంఖ్య_id).తల_2;

    అక్షరం* ప్రారంభం_addr = ((అక్షరం*)&values[0]) + ప్రారంభం * విభజించు_context.class1_రికార్డు_కొలత;
    గుర్తులేని అంకె_రికార్డులు = చివర - ప్రారంభం;
    hb_memcpy (&pair_pos_ప్రధానసంఖ్య->values[0],
            ప్రారంభం_addr,
            అంకె_రికార్డులు * విభజించు_context.class1_రికార్డు_కొలత);

    ఒకవేళ (!విభజించు_context.format1_device_పట్టిక_indices
        && !విభజించు_context.format2_device_పట్టిక_indices)
      // No device tables to move over.
      తిరిగి Ok();

    గుర్తులేని class2_లెక్క = class2_లెక్క_2;
    కొరకు (గుర్తులేని i = ప్రారంభం; i < చివర; i++)
    {
      కొరకు (గుర్తులేని j = 0; j < class2_లెక్క; j++)
      {
        గుర్తులేని value1_సూచీ = విభజించు_context.విలువ_రికార్డు_len * (class2_లెక్క * i + j);
        గుర్తులేని value2_సూచీ = value1_సూచీ + విభజించు_context.value1_రికార్డు_len;

        గుర్తులేని new_value1_సూచీ = విభజించు_context.విలువ_రికార్డు_len * (class2_లెక్క * (i - ప్రారంభం) + j);
        గుర్తులేని new_value2_సూచీ = new_value1_సూచీ + విభజించు_context.value1_రికార్డు_len;

        TRY (transfer_device_tables (విభజించు_context,
                                     pair_pos_ప్రధానసంఖ్య_id,
                                     విభజించు_context.format1_device_పట్టిక_indices,
                                     value1_సూచీ,
                                     new_value1_సూచీ));

        TRY (transfer_device_tables (విభజించు_context,
                                     pair_pos_ప్రధానసంఖ్య_id,
                                     విభజించు_context.format2_device_పట్టిక_indices,
                                     value2_సూచీ,
                                     new_value2_సూచీ));
      }
    }
    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> transfer_device_tables (విభజించు_context_t& విభజించు_context,
                                               గుర్తులేని pair_pos_ప్రధానసంఖ్య_id,
                                               స్థిరరాశి hb_సదిశ_t<గుర్తులేని>& device_పట్టిక_indices,
                                               గుర్తులేని old_విలువ_రికార్డు_సూచీ,
                                               గుర్తులేని new_విలువ_రికార్డు_సూచీ) స్థిరరాశి
  {
    PairPosFormat2* pair_pos_ప్రధానసంఖ్య =
        (PairPosFormat2*) విభజించు_context.c.లేఖాచిత్రం.object (pair_pos_ప్రధానసంఖ్య_id).తల_2;

    కొరకు (గుర్తులేని i : device_పట్టిక_indices)
    {
      OT::Offset16* రికార్డు = (OT::Offset16*) &values[old_విలువ_రికార్డు_సూచీ + i];
      గుర్తులేని రికార్డు_position = ((అక్షరం*) రికార్డు) - ((అక్షరం*) ఇది);
      ఒకవేళ (!విభజించు_context.device_tables.has (రికార్డు_position)) కొనసాగు;

      TRY (విభజించు_context.c.లేఖాచిత్రం.move_బిడ్డ (
          విభజించు_context.this_సూచీ,
          రికార్డు,
          pair_pos_ప్రధానసంఖ్య_id,
          (OT::Offset16*) &pair_pos_ప్రధానసంఖ్య->values[new_విలువ_రికార్డు_సూచీ + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    విభజించు_context.c.లేఖాచిత్రం.vertices_[విభజించు_context.this_సూచీ].క్రమీకరించు_real_links ();

    తిరిగి Ok();
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> shrink (విభజించు_context_t& విభజించు_context,
                               గుర్తులేని లెక్క)
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               విభజించు_context.this_సూచీ,
               లెక్క);
    గుర్తులేని old_లెక్క = class1_లెక్క_2;
    ఒకవేళ (లెక్క >= old_లెక్క)
      తిరిగి Ok();

    లేఖాచిత్రం_t& లేఖాచిత్రం = విభజించు_context.c.లేఖాచిత్రం;
    class1_లెక్క_2 = లెక్క;
    లేఖాచిత్రం.vertices_[విభజించు_context.this_సూచీ].shrink_బఫరు(
        (old_లెక్క - లెక్క) * విభజించు_context.class1_రికార్డు_కొలత);

    TRY_కేటాయించు (స్వయంచాలక coverage,
         లేఖాచిత్రం.as_mutable_పట్టిక<Coverage> (విభజించు_context.this_సూచీ, &ఇది->coverage));

    TRY_కేటాయించు (స్వయంచాలక class_def_1,
         లేఖాచిత్రం.as_mutable_పట్టిక<ClassDef> (విభజించు_context.this_సూచీ, &classDef1));

    స్వయంచాలక klass_map =
    + coverage.పట్టిక->iter ()
    | hb_map_retains_క్రమీకరణ ([&] (hb_codepoint_t gid) {
      తిరిగి hb_codepoint_pair_t (gid, class_def_1.పట్టిక->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      తిరిగి klass < లెక్క;
    }, hb_రెండవ)
    ;

    స్వయంచాలక new_coverage = + klass_map | hb_map_retains_క్రమీకరణ (hb_మొదటి);
    TRY (Coverage::make_coverage (విభజించు_context.c,
                                  + new_coverage,
                                  coverage.సూచీ_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    తిరిగి ClassDef::make_class_def (విభజించు_context.c,
                                     + klass_map,
                                     class_def_1.సూచీ_2,
                                     class_def_1.శీర్షం->పట్టిక_కొలత ());
  }

  లేఖాచిత్రం_ఫలితం_t<hb_hashmap_t<గుర్తులేని, గుర్తులేని>>
  get_all_device_tables (gsubgpos_లేఖాచిత్రం_context_t& c,
                         గుర్తులేని this_సూచీ) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక& v = c.లేఖాచిత్రం.vertices_[this_సూచీ];
    తిరిగి v.position_గమ్యం_సూచీ_map ();
  }

  లేఖాచిత్రం_ఫలితం_t<స్థిరరాశి Coverage*> get_coverage (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                గుర్తులేని this_సూచీ) స్థిరరాశి
  {
    TRY_కేటాయించు (గుర్తులేని coverage_id, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &coverage));
    స్వయంచాలక& coverage_v = c.లేఖాచిత్రం.vertices_[coverage_id];

    స్థిరరాశి Coverage* coverage_పట్టిక = (స్థిరరాశి Coverage*) coverage_v.obj().తల_2;
    ఒకవేళ (unlikely (!coverage_పట్టిక)) తిరిగి Err(INVALID_వాదం);
    TRY(coverage_పట్టిక->sanitize (coverage_v));

    తిరిగి coverage_పట్టిక;
  }

  లేఖాచిత్రం_ఫలితం_t<స్థిరరాశి ClassDef*> get_class_def_1 (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                   గుర్తులేని this_సూచీ) స్థిరరాశి
  {
    TRY_కేటాయించు (గుర్తులేని class_def_1_id, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, &classDef1));
    స్వయంచాలక& class_def_1_v = c.లేఖాచిత్రం.vertices_[class_def_1_id];

    స్థిరరాశి ClassDef* class_def_1_పట్టిక = (స్థిరరాశి ClassDef*) class_def_1_v.obj().తల_2;
    ఒకవేళ (unlikely (!class_def_1_పట్టిక)) తిరిగి Err(INVALID_వాదం);
    TRY(class_def_1_పట్టిక->sanitize (class_def_1_v));
    తిరిగి class_def_1_పట్టిక;
  }

  గుర్తులేని కొలత_యొక్క_విలువ_రికార్డు_children (gsubgpos_లేఖాచిత్రం_context_t& c,
                                          స్థిరరాశి hb_hashmap_t<గుర్తులేని, గుర్తులేని>& device_tables,
                                          స్థిరరాశి hb_సదిశ_t<గుర్తులేని> device_పట్టిక_indices,
                                          గుర్తులేని విలువ_రికార్డు_సూచీ,
                                          hb_సమితి_t& సందర్శించిన)
  {
    గుర్తులేని కొలత = 0;
    కొరకు (గుర్తులేని i : device_పట్టిక_indices)
    {
      OT::Layout::GPOS_impl::విలువ* రికార్డు = &values[విలువ_రికార్డు_సూచీ + i];
      గుర్తులేని రికార్డు_position = ((అక్షరం*) రికార్డు) - ((అక్షరం*) ఇది);
      గుర్తులేని* obj_idx;
      ఒకవేళ (!device_tables.has (రికార్డు_position, &obj_idx)) కొనసాగు;
      కొలత += c.లేఖాచిత్రం.కనుగొను_subgraph_కొలత (*obj_idx, సందర్శించిన).విలువ_or (0);
    }
    తిరిగి కొలత;
  }

  లేఖాచిత్రం_ఫలితం_t<గుర్తులేని> కొలత_యొక్క (gsubgpos_లేఖాచిత్రం_context_t& c,
                                    గుర్తులేని this_సూచీ,
                                    స్థిరరాశి శూన్య* స్థానభ్రంశం) స్థిరరాశి
  {
    TRY_కేటాయించు (స్థిరరాశి గుర్తులేని id, c.లేఖాచిత్రం.సూచీ_for_స్థానభ్రంశం (this_సూచీ, స్థానభ్రంశం));
    తిరిగి Ok(c.లేఖాచిత్రం.vertices_[id].పట్టిక_కొలత ());
  }
};

నిర్మాణం PairPos : బహిరంగ OT::Layout::GPOS_impl::PairPos
{
  లేఖాచిత్రం_ఫలితం_t<hb_సదిశ_t<గుర్తులేని>> విభజించు_subtables (gsubgpos_లేఖాచిత్రం_context_t& c,
                                                         గుర్తులేని this_సూచీ)
  {
    ఎంపిక (u.format.v) {
    స్థితి 1:
      hb_barrier ();
      తిరిగి ((PairPosFormat1*)(&u.format1))->విభజించు_subtables (c, this_సూచీ);
    స్థితి 2:
      hb_barrier ();
      తిరిగి ((PairPosFormat2*)(&u.format2))->విభజించు_subtables (c, this_సూచీ);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: HB_FALLTHROUGH;
    స్థితి 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#అయితే_ముగింపు
    అప్రమేయ:
      తిరిగి Ok(hb_సదిశ_t<గుర్తులేని> ());
    }
  }

  లేఖాచిత్రం_ఫలితం_t<శూన్య> sanitize (స్థిరరాశి లేఖాచిత్రం_t::శీర్షం_t& శీర్షం) స్థిరరాశి
  {
    size_t శీర్షం_len = శీర్షం.పట్టిక_కొలత();
    ఒకవేళ (unlikely (శీర్షం_len < u.format.v.get_కొలత ())) తిరిగి Err(SANITIZE_FAILURE);
    hb_barrier ();

    ఎంపిక (u.format.v) {
    స్థితి 1:
      hb_barrier ();
      తిరిగి ((PairPosFormat1*)(&u.format1))->sanitize (శీర్షం);
    స్థితి 2:
      hb_barrier ();
      తిరిగి ((PairPosFormat2*)(&u.format2))->sanitize (శీర్షం);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    స్థితి 3: HB_FALLTHROUGH;
    స్థితి 4: HB_FALLTHROUGH;
#అయితే_ముగింపు
    అప్రమేయ:
      // We don't handle format 3 and 4 here.
      తిరిగి Err(SANITIZE_FAILURE);
    }
  }
};

}

#అయితే_ముగింపు  // GRAPH_PAIRPOS_GRAPH_HH
