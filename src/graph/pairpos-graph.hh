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

#ifndef GRAPH_PAIRPOS_GRAPH_HH
#define GRAPH_PAIRPOS_GRAPH_HH

#অন্তর্ভাব "split-helpers.hh"
#অন্তর্ভাব "coverage-graph.hh"
#অন্তর্ভাব "classdef-graph.hh"
#অন্তর্ভাব "../OT/Layout/GPOS/PairPos.hh"
#অন্তর্ভাব "../OT/Layout/GPOS/PosLookupSubTable.hh"

নামাকাশ graph {

কাঠামো PairPosFormat1 : সর্বজনীন OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    unsigned min_আকার = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_আকার;
    যদি (unlikely (vertex_len < min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    যদি (unlikely (vertex_len < min_আকার + pairSet.get_আকার () - pairSet.len.get_আকার()))
      ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  graph_ফলাফল_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_সূচক)
  {
    hb_set_t visited;

    TRY_ASSIGN (ধ্রুবক unsigned coverage_id, c.graph.সূচক_for_offset (this_সূচক, &coverage));
    ধ্রুবক unsigned coverage_আকার = c.graph.vertices_[coverage_id].table_আকার ();
    ধ্রুবক unsigned base_আকার = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_আকার;

    unsigned partial_coverage_আকার = 4;
    unsigned accumulated = base_আকার;
    hb_vector_t<unsigned> split_points;
    জন্য (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_সূচক, pair_set_graph_সূচক (c, this_সূচক, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_আকার (pair_set_সূচক, visited).মান_or (0) +
          SmallTypes::আকার_2; // for PairSet offset.
      partial_coverage_আকার += OT::HBUINT16::static_আকার;

      accumulated += accumulated_delta;
      unsigned যোগ = accumulated + hb_min (partial_coverage_আকার, coverage_আকার);

      যদি (যোগ >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_ফলাফল_t<শূন্য>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_আকার + accumulated_delta;
        partial_coverage_আকার = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      এই,
      this_সূচক,
    };

    ফেরত actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ব্যক্তিগত:

  কাঠামো split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_সূচক;

    unsigned original_গুনতি_2 ()
    {
      ফেরত thiz->pairSet.len;
    }

    graph_ফলাফল_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ফেরত thiz->clone_range (এই->c, এই->this_সূচক, start, end);
    }

    graph_ফলাফল_t<শূন্য> shrink (unsigned গুনতি)
    {
      ফেরত thiz->shrink (এই->c, এই->this_সূচক, গুনতি);
    }
  };

  graph_ফলাফল_t<শূন্য> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_সূচক,
                               unsigned গুনতি)
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_সূচক,
               গুনতি);
    unsigned old_গুনতি = pairSet.len;
    যদি (গুনতি >= old_গুনতি)
      ফেরত Ok();

    pairSet.len = গুনতি;
    c.graph.vertices_[this_সূচক].shrink_বাফার ((old_গুনতি - গুনতি) * SmallTypes::আকার_2);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_সূচক, &এই->coverage));

    unsigned coverage_আকার = coverage.vertex->table_আকার ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          ফেরত p.second < গুনতি;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    ফেরত Coverage::make_coverage (c, new_coverage, coverage.সূচক_2, coverage_আকার);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_ফলাফল_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_সূচক,
                                        unsigned start, unsigned end) ধ্রুবক
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_সূচক, start, end);

    unsigned অঙ্ক_pair_sets = end - start;
    unsigned মৌলিক_আকার = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_আকার
                          + অঙ্ক_pair_sets * SmallTypes::আকার_2;

    TRY_ASSIGN (unsigned pair_pos_মৌলিক_id, c.create_গ্রন্থি (মৌলিক_আকার));

    PairPosFormat1* pair_pos_মৌলিক = (PairPosFormat1*) c.graph.object (pair_pos_মৌলিক_id).শীর্ষ_2;
    pair_pos_মৌলিক->format = এই->format;
    pair_pos_মৌলিক->মান_Format_2[0] = এই->মান_Format_2[0];
    pair_pos_মৌলিক->মান_Format_2[1] = এই->মান_Format_2[1];
    pair_pos_মৌলিক->pairSet.len = অঙ্ক_pair_sets;

    জন্য (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_সূচক,
                                 &pairSet[i],
                                 pair_pos_মৌলিক_id,
                                 &pair_pos_মৌলিক->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_সূচক].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.সূচক_for_offset (this_সূচক, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_মৌলিক_id,
                                   2,
                                   start, end));

    ফেরত Ok(pair_pos_মৌলিক_id);
  }



  graph_ফলাফল_t<unsigned> pair_set_graph_সূচক (gsubgpos_graph_context_t& c, unsigned this_সূচক, unsigned i) ধ্রুবক
  {
    ফেরত c.graph.সূচক_for_offset (this_সূচক, &pairSet[i]);
  }
};

কাঠামো PairPosFormat2 : সর্বজনীন OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    unsigned min_আকার = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_আকার;
    যদি (unlikely (vertex_len < min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    ধ্রুবক unsigned class1_গুনতি = class1_গুনতি_2;
    যদি (unlikely (vertex_len < min_আকার + class1_গুনতি * get_class1_record_আকার ()))
      ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  graph_ফলাফল_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_সূচক)
  {
    ধ্রুবক unsigned base_আকার = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_আকার;
    TRY_ASSIGN (ধ্রুবক unsigned class_def_2_আকার, আকার_of (c, this_সূচক, &classDef2));
    TRY_ASSIGN (ধ্রুবক Coverage* coverage, get_coverage (c, this_সূচক));
    TRY_ASSIGN (ধ্রুবক ClassDef* class_def_1, get_class_def_1 (c, this_সূচক));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          ফেরত hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_আকার_estimator_t estimator, class_def_আকার_estimator_t::create (gid_and_class));

    ধ্রুবক unsigned class1_গুনতি = class1_গুনতি_2;
    ধ্রুবক unsigned class2_গুনতি = class2_গুনতি_2;
    ধ্রুবক unsigned class1_record_আকার = get_class1_record_আকার ();

    ধ্রুবক unsigned মান_1_len = মান_Format1.get_len ();
    ধ্রুবক unsigned মান_2_len = মান_Format2.get_len ();
    ধ্রুবক unsigned যোগ_মান_len = মান_1_len + মান_2_len;

    unsigned accumulated = base_আকার;
    unsigned coverage_আকার = 4;
    unsigned class_def_1_আকার = 4;
    unsigned max_coverage_আকার = coverage_আকার;
    unsigned max_class_def_1_আকার = class_def_1_আকার;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_সূচক));
    hb_vector_t<unsigned> format1_device_table_indices = মান_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = মান_Format2.get_device_table_indices ();
    যৌক্তিক has_device_tables = যৌক্তিক(format1_device_table_indices) || যৌক্তিক(format2_device_table_indices);

    hb_set_t visited;
    জন্য (unsigned i = 0; i < class1_গুনতি; i++)
    {
      unsigned accumulated_delta = class1_record_আকার;
      class_def_1_আকার = estimator.add_class_def_আকার (i);
      coverage_আকার = estimator.coverage_আকার ();
      max_coverage_আকার = hb_max (max_coverage_আকার, coverage_আকার);
      max_class_def_1_আকার = hb_max (max_class_def_1_আকার, class_def_1_আকার);

      যদি (has_device_tables) {
        জন্য (unsigned j = 0; j < class2_গুনতি; j++)
        {
          unsigned value1_সূচক = যোগ_মান_len * (class2_গুনতি * i + j);
          unsigned value2_সূচক = value1_সূচক + মান_1_len;
          accumulated_delta += আকার_of_মান_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_সূচক,
                                                        visited);
          accumulated_delta += আকার_of_মান_record_children (c,
                                                        device_tables,
                                                        format2_device_table_indices,
                                                        value2_সূচক,
                                                        visited);
        }
      }

      accumulated += accumulated_delta;
      unsigned যোগ = accumulated
                       + coverage_আকার + class_def_1_আকার + class_def_2_আকার
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_আকার, class_def_1_আকার), class_def_2_আকার);
      যদি (যোগ >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_ফলাফল_t<শূন্য>::from (split_points, ALLOCATION_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = base_আকার + accumulated_delta;

        estimator.reset();
        class_def_1_আকার = estimator.add_class_def_আকার(i);
        coverage_আকার = estimator.coverage_আকার();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      এই,
      this_সূচক,
      class1_record_আকার,
      যোগ_মান_len,
      মান_1_len,
      মান_2_len,
      max_coverage_আকার,
      max_class_def_1_আকার,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    ফেরত actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 ব্যক্তিগত:

  কাঠামো split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_সূচক;
    unsigned class1_record_আকার;
    unsigned মান_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_আকার;
    unsigned max_class_def_আকার;

    ধ্রুবক hb_hashmap_t<unsigned, unsigned>& device_tables;
    ধ্রুবক hb_vector_t<unsigned>& format1_device_table_indices;
    ধ্রুবক hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_গুনতি_2 ()
    {
      ফেরত thiz->class1_গুনতি_2;
    }

    graph_ফলাফল_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ফেরত thiz->clone_range (*এই, start, end);
    }

    graph_ফলাফল_t<শূন্য> shrink (unsigned গুনতি)
    {
      ফেরত thiz->shrink (*এই, গুনতি);
    }
  };

  size_t get_class1_record_আকার () ধ্রুবক
  {
    ধ্রুবক size_t class2_গুনতি = class2_গুনতি_2;
    ফেরত
        class2_গুনতি * (মান_Format1.get_আকার () + মান_Format2.get_আকার ());
  }

  graph_ফলাফল_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) ধ্রুবক
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_সূচক, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned অঙ্ক_records = end - start;
    unsigned মৌলিক_আকার = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_আকার
                          + অঙ্ক_records * split_context.class1_record_আকার;

    TRY_ASSIGN (unsigned pair_pos_মৌলিক_id, split_context.c.create_গ্রন্থি (মৌলিক_আকার));

    PairPosFormat2* pair_pos_মৌলিক =
        (PairPosFormat2*) graph.object (pair_pos_মৌলিক_id).শীর্ষ_2;
    pair_pos_মৌলিক->format = এই->format;
    pair_pos_মৌলিক->মান_Format1 = এই->মান_Format1;
    pair_pos_মৌলিক->মান_Format2 = এই->মান_Format2;
    pair_pos_মৌলিক->class1_গুনতি_2 = অঙ্ক_records;
    pair_pos_মৌলিক->class2_গুনতি_2 = এই->class2_গুনতি_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_মৌলিক_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.সূচক_for_offset (split_context.this_সূচক, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.সূচক_for_offset (split_context.this_সূচক, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    ধ্রুবক Coverage* coverage_table = (ধ্রুবক Coverage*) coverage_v.obj().শীর্ষ_2;
    ধ্রুবক ClassDef* class_def_1_table = (ধ্রুবক ClassDef*) class_def_1_v.obj().শীর্ষ_2;
    যদি (unlikely (!coverage_table || !class_def_1_table))
      ফেরত Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      ফেরত hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      ফেরত klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      ফেরত hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_মৌলিক_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_আকার));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_মৌলিক_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_আকার));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.সূচক_for_offset (split_context.this_সূচক, &classDef2));

    TRY(graph.vertices_[pair_pos_মৌলিক_id].add_real_link (SmallTypes::আকার_2, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_মৌলিক_id, মিথ্যা));

    ফেরত Ok(pair_pos_মৌলিক_id);
  }

  graph_ফলাফল_t<শূন্য> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_মৌলিক_id,
                                             unsigned start, unsigned end) ধ্রুবক
  {
    PairPosFormat2* pair_pos_মৌলিক =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_মৌলিক_id).শীর্ষ_2;

    অক্ষর* start_addr = ((অক্ষর*)&values[0]) + start * split_context.class1_record_আকার;
    unsigned অঙ্ক_records = end - start;
    hb_memcpy (&pair_pos_মৌলিক->values[0],
            start_addr,
            অঙ্ক_records * split_context.class1_record_আকার);

    যদি (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      ফেরত Ok();

    unsigned class2_গুনতি = class2_গুনতি_2;
    জন্য (unsigned i = start; i < end; i++)
    {
      জন্য (unsigned j = 0; j < class2_গুনতি; j++)
      {
        unsigned value1_সূচক = split_context.মান_record_len * (class2_গুনতি * i + j);
        unsigned value2_সূচক = value1_সূচক + split_context.value1_record_len;

        unsigned new_value1_সূচক = split_context.মান_record_len * (class2_গুনতি * (i - start) + j);
        unsigned new_value2_সূচক = new_value1_সূচক + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_মৌলিক_id,
                                     split_context.format1_device_table_indices,
                                     value1_সূচক,
                                     new_value1_সূচক));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_মৌলিক_id,
                                     split_context.format2_device_table_indices,
                                     value2_সূচক,
                                     new_value2_সূচক));
      }
    }
    ফেরত Ok();
  }

  graph_ফলাফল_t<শূন্য> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_মৌলিক_id,
                                               ধ্রুবক hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_মান_record_সূচক,
                                               unsigned new_মান_record_সূচক) ধ্রুবক
  {
    PairPosFormat2* pair_pos_মৌলিক =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_মৌলিক_id).শীর্ষ_2;

    জন্য (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_মান_record_সূচক + i];
      unsigned record_position = ((অক্ষর*) record) - ((অক্ষর*) এই);
      যদি (!split_context.device_tables.has (record_position)) চালু;

      TRY (split_context.c.graph.move_child (
          split_context.this_সূচক,
          record,
          pair_pos_মৌলিক_id,
          (OT::Offset16*) &pair_pos_মৌলিক->values[new_মান_record_সূচক + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_সূচক].sort_real_links ();

    ফেরত Ok();
  }

  graph_ফলাফল_t<শূন্য> shrink (split_context_t& split_context,
                               unsigned গুনতি)
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_সূচক,
               গুনতি);
    unsigned old_গুনতি = class1_গুনতি_2;
    যদি (গুনতি >= old_গুনতি)
      ফেরত Ok();

    graph_t& graph = split_context.c.graph;
    class1_গুনতি_2 = গুনতি;
    graph.vertices_[split_context.this_সূচক].shrink_বাফার(
        (old_গুনতি - গুনতি) * split_context.class1_record_আকার);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_সূচক, &এই->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_সূচক, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      ফেরত hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      ফেরত klass < গুনতি;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.সূচক_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    ফেরত ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.সূচক_2,
                                     class_def_1.vertex->table_আকার ());
  }

  graph_ফলাফল_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_সূচক) ধ্রুবক
  {
    ধ্রুবক auto& v = c.graph.vertices_[this_সূচক];
    ফেরত v.position_to_সূচক_map ();
  }

  graph_ফলাফল_t<ধ্রুবক Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_সূচক) ধ্রুবক
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.সূচক_for_offset (this_সূচক, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    ধ্রুবক Coverage* coverage_table = (ধ্রুবক Coverage*) coverage_v.obj().শীর্ষ_2;
    যদি (unlikely (!coverage_table)) ফেরত Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    ফেরত coverage_table;
  }

  graph_ফলাফল_t<ধ্রুবক ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_সূচক) ধ্রুবক
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.সূচক_for_offset (this_সূচক, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    ধ্রুবক ClassDef* class_def_1_table = (ধ্রুবক ClassDef*) class_def_1_v.obj().শীর্ষ_2;
    যদি (unlikely (!class_def_1_table)) ফেরত Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    ফেরত class_def_1_table;
  }

  unsigned আকার_of_মান_record_children (gsubgpos_graph_context_t& c,
                                          ধ্রুবক hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          ধ্রুবক hb_vector_t<unsigned> device_table_indices,
                                          unsigned মান_record_সূচক,
                                          hb_set_t& visited)
  {
    unsigned আকার_2 = 0;
    জন্য (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::মান* record = &values[মান_record_সূচক + i];
      unsigned record_position = ((অক্ষর*) record) - ((অক্ষর*) এই);
      unsigned* obj_idx;
      যদি (!device_tables.has (record_position, &obj_idx)) চালু;
      আকার_2 += c.graph.find_subgraph_আকার (*obj_idx, visited).মান_or (0);
    }
    ফেরত আকার_2;
  }

  graph_ফলাফল_t<unsigned> আকার_of (gsubgpos_graph_context_t& c,
                                    unsigned this_সূচক,
                                    ধ্রুবক শূন্য* offset) ধ্রুবক
  {
    TRY_ASSIGN (ধ্রুবক unsigned id, c.graph.সূচক_for_offset (this_সূচক, offset));
    ফেরত Ok(c.graph.vertices_[id].table_আকার ());
  }
};

কাঠামো PairPos : সর্বজনীন OT::Layout::GPOS_impl::PairPos
{
  graph_ফলাফল_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_সূচক)
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1:
      hb_barrier ();
      ফেরত ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_সূচক);
    ক্ষেত্র 2:
      hb_barrier ();
      ফেরত ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_সূচক);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: HB_FALLTHROUGH;
    ক্ষেত্র 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    পূর্বনির্ধারিত:
      ফেরত Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার();
    যদি (unlikely (vertex_len < u.format.v.get_আকার ())) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    নির্বাচন (u.format.v) {
    ক্ষেত্র 1:
      hb_barrier ();
      ফেরত ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    ক্ষেত্র 2:
      hb_barrier ();
      ফেরত ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 3: HB_FALLTHROUGH;
    ক্ষেত্র 4: HB_FALLTHROUGH;
#endif
    পূর্বনির্ধারিত:
      // We don't handle format 3 and 4 here.
      ফেরত Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
