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

#ಅನ್ತರ್ಭಾವ "split-helpers.hh"
#ಅನ್ತರ್ಭಾವ "coverage-graph.hh"
#ಅನ್ತರ್ಭಾವ "classdef-graph.hh"
#ಅನ್ತರ್ಭಾವ "../OT/Layout/GPOS/PairPos.hh"
#ಅನ್ತರ್ಭಾವ "../OT/Layout/GPOS/PosLookupSubTable.hh"

ನಾಮಾಕಾಶ graph {

ಸಂರಚನಾ PairPosFormat1 : ಸಾರ್ವಜನಿಕ OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    ಯದಿ (unlikely (vertex_len < min_size)) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ಯದಿ (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ಧ್ರುವ unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    ಧ್ರುವ unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    ಧ್ರುವ unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;
    ಪ್ರತಿ (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).ಮೌಲ್ಯ_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      ಯದಿ (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<ಶೂನ್ಯ>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      ಏತತ್,
      this_index,
    };

    ಪ್ರತ್ಯಾಗಮ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ಗುಪ್ತ:

  ಸಂರಚನಾ split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_ಎಣಿಕೆ_2 ()
    {
      ಪ್ರತ್ಯಾಗಮ thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ಪ್ರತ್ಯಾಗಮ thiz->clone_range (ಏತತ್->c, ಏತತ್->this_index, start, end);
    }

    graph_result_t<ಶೂನ್ಯ> shrink (unsigned ಎಣಿಕೆ)
    {
      ಪ್ರತ್ಯಾಗಮ thiz->shrink (ಏತತ್->c, ಏತತ್->this_index, ಎಣಿಕೆ);
    }
  };

  graph_result_t<ಶೂನ್ಯ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned ಎಣಿಕೆ)
  {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               ಎಣಿಕೆ);
    unsigned old_ಎಣಿಕೆ = pairSet.len;
    ಯದಿ (ಎಣಿಕೆ >= old_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ Ok();

    pairSet.len = ಎಣಿಕೆ;
    c.graph.vertices_[this_index].shrink_buffer ((old_ಎಣಿಕೆ - ಎಣಿಕೆ) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &ಏತತ್->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          ಪ್ರತ್ಯಾಗಮ p.second < ಎಣಿಕೆ;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    ಪ್ರತ್ಯಾಗಮ Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ಧ್ರುವ
  {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned ಅಂಕೆ_pair_sets = end - start;
    unsigned ಅವಿಭಾಜ್ಯ_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + ಅಂಕೆ_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_ಅವಿಭಾಜ್ಯ_id, c.create_ಗಂಟು (ಅವಿಭಾಜ್ಯ_size));

    PairPosFormat1* pair_pos_ಅವಿಭಾಜ್ಯ = (PairPosFormat1*) c.graph.object (pair_pos_ಅವಿಭಾಜ್ಯ_id).head;
    pair_pos_ಅವಿಭಾಜ್ಯ->format = ಏತತ್->format;
    pair_pos_ಅವಿಭಾಜ್ಯ->ಮೌಲ್ಯ_Format_2[0] = ಏತತ್->ಮೌಲ್ಯ_Format_2[0];
    pair_pos_ಅವಿಭಾಜ್ಯ->ಮೌಲ್ಯ_Format_2[1] = ಏತತ್->ಮೌಲ್ಯ_Format_2[1];
    pair_pos_ಅವಿಭಾಜ್ಯ->pairSet.len = ಅಂಕೆ_pair_sets;

    ಪ್ರತಿ (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_ಅವಿಭಾಜ್ಯ_id,
                                 &pair_pos_ಅವಿಭಾಜ್ಯ->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_ಅವಿಭಾಜ್ಯ_id,
                                   2,
                                   start, end));

    ಪ್ರತ್ಯಾಗಮ Ok(pair_pos_ಅವಿಭಾಜ್ಯ_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

ಸಂರಚನಾ PairPosFormat2 : ಸಾರ್ವಜನಿಕ OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    ಯದಿ (unlikely (vertex_len < min_size)) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ಧ್ರುವ unsigned class1_ಎಣಿಕೆ = class1_ಎಣಿಕೆ_2;
    ಯದಿ (unlikely (vertex_len < min_size + class1_ಎಣಿಕೆ * get_class1_record_size ()))
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ಧ್ರುವ unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (ಧ್ರುವ unsigned class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (ಧ್ರುವ Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (ಧ್ರುವ ClassDef* class_def_1, get_class_def_1 (c, this_index));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          ಪ್ರತ್ಯಾಗಮ hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    ಧ್ರುವ unsigned class1_ಎಣಿಕೆ = class1_ಎಣಿಕೆ_2;
    ಧ್ರುವ unsigned class2_ಎಣಿಕೆ = class2_ಎಣಿಕೆ_2;
    ಧ್ರುವ unsigned class1_record_size = get_class1_record_size ();

    ಧ್ರುವ unsigned ಮೌಲ್ಯ_1_len = ಮೌಲ್ಯ_Format1.get_len ();
    ಧ್ರುವ unsigned ಮೌಲ್ಯ_2_len = ಮೌಲ್ಯ_Format2.get_len ();
    ಧ್ರುವ unsigned total_ಮೌಲ್ಯ_len = ಮೌಲ್ಯ_1_len + ಮೌಲ್ಯ_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = ಮೌಲ್ಯ_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = ಮೌಲ್ಯ_Format2.get_device_table_indices ();
    ತಾರ್ಕಿಕ has_device_tables = ತಾರ್ಕಿಕ(format1_device_table_indices) || ತಾರ್ಕಿಕ(format2_device_table_indices);

    hb_set_t visited;
    ಪ್ರತಿ (unsigned i = 0; i < class1_ಎಣಿಕೆ; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      ಯದಿ (has_device_tables) {
        ಪ್ರತಿ (unsigned j = 0; j < class2_ಎಣಿಕೆ; j++)
        {
          unsigned value1_index = total_ಮೌಲ್ಯ_len * (class2_ಎಣಿಕೆ * i + j);
          unsigned value2_index = value1_index + ಮೌಲ್ಯ_1_len;
          accumulated_delta += size_of_ಮೌಲ್ಯ_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_ಮೌಲ್ಯ_record_children (c,
                                                        device_tables,
                                                        format2_device_table_indices,
                                                        value2_index,
                                                        visited);
        }
      }

      accumulated += accumulated_delta;
      unsigned total = accumulated
                       + coverage_size + class_def_1_size + class_def_2_size
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_size, class_def_1_size), class_def_2_size);
      ಯದಿ (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<ಶೂನ್ಯ>::from (split_points, ALLOCATION_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = base_size + accumulated_delta;

        estimator.reset();
        class_def_1_size = estimator.add_class_def_size(i);
        coverage_size = estimator.coverage_size();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      ಏತತ್,
      this_index,
      class1_record_size,
      total_ಮೌಲ್ಯ_len,
      ಮೌಲ್ಯ_1_len,
      ಮೌಲ್ಯ_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    ಪ್ರತ್ಯಾಗಮ actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 ಗುಪ್ತ:

  ಸಂರಚನಾ split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned ಮೌಲ್ಯ_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    ಧ್ರುವ hb_hashmap_t<unsigned, unsigned>& device_tables;
    ಧ್ರುವ hb_vector_t<unsigned>& format1_device_table_indices;
    ಧ್ರುವ hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_ಎಣಿಕೆ_2 ()
    {
      ಪ್ರತ್ಯಾಗಮ thiz->class1_ಎಣಿಕೆ_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ಪ್ರತ್ಯಾಗಮ thiz->clone_range (*ಏತತ್, start, end);
    }

    graph_result_t<ಶೂನ್ಯ> shrink (unsigned ಎಣಿಕೆ)
    {
      ಪ್ರತ್ಯಾಗಮ thiz->shrink (*ಏತತ್, ಎಣಿಕೆ);
    }
  };

  size_t get_class1_record_size () ಧ್ರುವ
  {
    ಧ್ರುವ size_t class2_ಎಣಿಕೆ = class2_ಎಣಿಕೆ_2;
    ಪ್ರತ್ಯಾಗಮ
        class2_ಎಣಿಕೆ * (ಮೌಲ್ಯ_Format1.get_size () + ಮೌಲ್ಯ_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) ಧ್ರುವ
  {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned ಅಂಕೆ_records = end - start;
    unsigned ಅವಿಭಾಜ್ಯ_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + ಅಂಕೆ_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_ಅವಿಭಾಜ್ಯ_id, split_context.c.create_ಗಂಟು (ಅವಿಭಾಜ್ಯ_size));

    PairPosFormat2* pair_pos_ಅವಿಭಾಜ್ಯ =
        (PairPosFormat2*) graph.object (pair_pos_ಅವಿಭಾಜ್ಯ_id).head;
    pair_pos_ಅವಿಭಾಜ್ಯ->format = ಏತತ್->format;
    pair_pos_ಅವಿಭಾಜ್ಯ->ಮೌಲ್ಯ_Format1 = ಏತತ್->ಮೌಲ್ಯ_Format1;
    pair_pos_ಅವಿಭಾಜ್ಯ->ಮೌಲ್ಯ_Format2 = ಏತತ್->ಮೌಲ್ಯ_Format2;
    pair_pos_ಅವಿಭಾಜ್ಯ->class1_ಎಣಿಕೆ_2 = ಅಂಕೆ_records;
    pair_pos_ಅವಿಭಾಜ್ಯ->class2_ಎಣಿಕೆ_2 = ಏತತ್->class2_ಎಣಿಕೆ_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_ಅವಿಭಾಜ್ಯ_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    ಧ್ರುವ Coverage* coverage_table = (ಧ್ರುವ Coverage*) coverage_v.obj().head;
    ಧ್ರುವ ClassDef* class_def_1_table = (ಧ್ರುವ ClassDef*) class_def_1_v.obj().head;
    ಯದಿ (unlikely (!coverage_table || !class_def_1_table))
      ಪ್ರತ್ಯಾಗಮ Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      ಪ್ರತ್ಯಾಗಮ hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      ಪ್ರತ್ಯಾಗಮ klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      ಪ್ರತ್ಯಾಗಮ hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_ಅವಿಭಾಜ್ಯ_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_ಅವಿಭಾಜ್ಯ_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_ಅವಿಭಾಜ್ಯ_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_ಅವಿಭಾಜ್ಯ_id, ಅಸತ್ಯ));

    ಪ್ರತ್ಯಾಗಮ Ok(pair_pos_ಅವಿಭಾಜ್ಯ_id);
  }

  graph_result_t<ಶೂನ್ಯ> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_ಅವಿಭಾಜ್ಯ_id,
                                             unsigned start, unsigned end) ಧ್ರುವ
  {
    PairPosFormat2* pair_pos_ಅವಿಭಾಜ್ಯ =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_ಅವಿಭಾಜ್ಯ_id).head;

    ವರ್ಣ* start_addr = ((ವರ್ಣ*)&values[0]) + start * split_context.class1_record_size;
    unsigned ಅಂಕೆ_records = end - start;
    hb_memcpy (&pair_pos_ಅವಿಭಾಜ್ಯ->values[0],
            start_addr,
            ಅಂಕೆ_records * split_context.class1_record_size);

    ಯದಿ (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      ಪ್ರತ್ಯಾಗಮ Ok();

    unsigned class2_ಎಣಿಕೆ = class2_ಎಣಿಕೆ_2;
    ಪ್ರತಿ (unsigned i = start; i < end; i++)
    {
      ಪ್ರತಿ (unsigned j = 0; j < class2_ಎಣಿಕೆ; j++)
      {
        unsigned value1_index = split_context.ಮೌಲ್ಯ_record_len * (class2_ಎಣಿಕೆ * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.ಮೌಲ್ಯ_record_len * (class2_ಎಣಿಕೆ * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_ಅವಿಭಾಜ್ಯ_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_ಅವಿಭಾಜ್ಯ_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<ಶೂನ್ಯ> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_ಅವಿಭಾಜ್ಯ_id,
                                               ಧ್ರುವ hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_ಮೌಲ್ಯ_record_index,
                                               unsigned new_ಮೌಲ್ಯ_record_index) ಧ್ರುವ
  {
    PairPosFormat2* pair_pos_ಅವಿಭಾಜ್ಯ =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_ಅವಿಭಾಜ್ಯ_id).head;

    ಪ್ರತಿ (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_ಮೌಲ್ಯ_record_index + i];
      unsigned record_position = ((ವರ್ಣ*) record) - ((ವರ್ಣ*) ಏತತ್);
      ಯದಿ (!split_context.device_tables.has (record_position)) ಅನುವರ್ತ;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_ಅವಿಭಾಜ್ಯ_id,
          (OT::Offset16*) &pair_pos_ಅವಿಭಾಜ್ಯ->values[new_ಮೌಲ್ಯ_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<ಶೂನ್ಯ> shrink (split_context_t& split_context,
                               unsigned ಎಣಿಕೆ)
  {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               ಎಣಿಕೆ);
    unsigned old_ಎಣಿಕೆ = class1_ಎಣಿಕೆ_2;
    ಯದಿ (ಎಣಿಕೆ >= old_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ Ok();

    graph_t& graph = split_context.c.graph;
    class1_ಎಣಿಕೆ_2 = ಎಣಿಕೆ;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_ಎಣಿಕೆ - ಎಣಿಕೆ) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &ಏತತ್->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      ಪ್ರತ್ಯಾಗಮ hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      ಪ್ರತ್ಯಾಗಮ klass < ಎಣಿಕೆ;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    ಪ್ರತ್ಯಾಗಮ ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) ಧ್ರುವ
  {
    ಧ್ರುವ auto& v = c.graph.vertices_[this_index];
    ಪ್ರತ್ಯಾಗಮ v.position_to_index_map ();
  }

  graph_result_t<ಧ್ರುವ Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) ಧ್ರುವ
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    ಧ್ರುವ Coverage* coverage_table = (ಧ್ರುವ Coverage*) coverage_v.obj().head;
    ಯದಿ (unlikely (!coverage_table)) ಪ್ರತ್ಯಾಗಮ Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    ಪ್ರತ್ಯಾಗಮ coverage_table;
  }

  graph_result_t<ಧ್ರುವ ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) ಧ್ರುವ
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    ಧ್ರುವ ClassDef* class_def_1_table = (ಧ್ರುವ ClassDef*) class_def_1_v.obj().head;
    ಯದಿ (unlikely (!class_def_1_table)) ಪ್ರತ್ಯಾಗಮ Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    ಪ್ರತ್ಯಾಗಮ class_def_1_table;
  }

  unsigned size_of_ಮೌಲ್ಯ_record_children (gsubgpos_graph_context_t& c,
                                          ಧ್ರುವ hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          ಧ್ರುವ hb_vector_t<unsigned> device_table_indices,
                                          unsigned ಮೌಲ್ಯ_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    ಪ್ರತಿ (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::ಮೌಲ್ಯ* record = &values[ಮೌಲ್ಯ_record_index + i];
      unsigned record_position = ((ವರ್ಣ*) record) - ((ವರ್ಣ*) ಏತತ್);
      unsigned* obj_idx;
      ಯದಿ (!device_tables.has (record_position, &obj_idx)) ಅನುವರ್ತ;
      size += c.graph.find_subgraph_size (*obj_idx, visited).ಮೌಲ್ಯ_or (0);
    }
    ಪ್ರತ್ಯಾಗಮ size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ಧ್ರುವ ಶೂನ್ಯ* offset) ಧ್ರುವ
  {
    TRY_ASSIGN (ಧ್ರುವ unsigned id, c.graph.index_for_offset (this_index, offset));
    ಪ್ರತ್ಯಾಗಮ Ok(c.graph.vertices_[id].table_size ());
  }
};

ಸಂರಚನಾ PairPos : ಸಾರ್ವಜನಿಕ OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    ಪಕ್ಷ 2:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: HB_FALLTHROUGH;
    ಪಕ್ಷ 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size();
    ಯದಿ (unlikely (vertex_len < u.format.v.get_size ())) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    ಪಕ್ಷ 2:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 3: HB_FALLTHROUGH;
    ಪಕ್ಷ 4: HB_FALLTHROUGH;
#endif
    ಸ್ವಭಾವ:
      // We don't handle format 3 and 4 here.
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
