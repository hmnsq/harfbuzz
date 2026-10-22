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

#அந்தர்பாவ "split-helpers.hh"
#அந்தர்பாவ "coverage-graph.hh"
#அந்தர்பாவ "classdef-graph.hh"
#அந்தர்பாவ "../OT/Layout/GPOS/PairPos.hh"
#அந்தர்பாவ "../OT/Layout/GPOS/PosLookupSubTable.hh"

நாமாகாஶ graph {

கட்டமைப்பு PairPosFormat1 : பொது OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    எனில் (unlikely (vertex_len < min_size)) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();

    எனில் (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      திரும்பு Err(SANITIZE_FAILURE);
    திரும்பு Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (மாறிலி unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    மாறிலி unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    மாறிலி unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;
    க்கு (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).மதிப்பு_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      எனில் (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<வெறுமை>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      இது,
      this_index,
    };

    திரும்பு actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 தனிப்பட்ட:

  கட்டமைப்பு split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_மொத்தம்_2 ()
    {
      திரும்பு thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      திரும்பு thiz->clone_range (இது->c, இது->this_index, start, end);
    }

    graph_result_t<வெறுமை> shrink (unsigned மொத்தம்)
    {
      திரும்பு thiz->shrink (இது->c, இது->this_index, மொத்தம்);
    }
  };

  graph_result_t<வெறுமை> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned மொத்தம்)
  {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               மொத்தம்);
    unsigned old_மொத்தம் = pairSet.len;
    எனில் (மொத்தம் >= old_மொத்தம்)
      திரும்பு Ok();

    pairSet.len = மொத்தம்;
    c.graph.vertices_[this_index].shrink_buffer ((old_மொத்தம் - மொத்தம்) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &இது->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          திரும்பு p.second < மொத்தம்;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    திரும்பு Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) மாறிலி
  {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned இலக்கம்_pair_sets = end - start;
    unsigned பகாஎண்_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + இலக்கம்_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_பகாஎண்_id, c.create_முனை (பகாஎண்_size));

    PairPosFormat1* pair_pos_பகாஎண் = (PairPosFormat1*) c.graph.object (pair_pos_பகாஎண்_id).head;
    pair_pos_பகாஎண்->format = இது->format;
    pair_pos_பகாஎண்->மதிப்பு_Format_2[0] = இது->மதிப்பு_Format_2[0];
    pair_pos_பகாஎண்->மதிப்பு_Format_2[1] = இது->மதிப்பு_Format_2[1];
    pair_pos_பகாஎண்->pairSet.len = இலக்கம்_pair_sets;

    க்கு (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_பகாஎண்_id,
                                 &pair_pos_பகாஎண்->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_பகாஎண்_id,
                                   2,
                                   start, end));

    திரும்பு Ok(pair_pos_பகாஎண்_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) மாறிலி
  {
    திரும்பு c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

கட்டமைப்பு PairPosFormat2 : பொது OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    எனில் (unlikely (vertex_len < min_size)) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();

    மாறிலி unsigned class1_மொத்தம் = class1_மொத்தம்_2;
    எனில் (unlikely (vertex_len < min_size + class1_மொத்தம் * get_class1_record_size ()))
      திரும்பு Err(SANITIZE_FAILURE);
    திரும்பு Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    மாறிலி unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (மாறிலி unsigned class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (மாறிலி Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (மாறிலி ClassDef* class_def_1, get_class_def_1 (c, this_index));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          திரும்பு hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    மாறிலி unsigned class1_மொத்தம் = class1_மொத்தம்_2;
    மாறிலி unsigned class2_மொத்தம் = class2_மொத்தம்_2;
    மாறிலி unsigned class1_record_size = get_class1_record_size ();

    மாறிலி unsigned மதிப்பு_1_len = மதிப்பு_Format1.get_len ();
    மாறிலி unsigned மதிப்பு_2_len = மதிப்பு_Format2.get_len ();
    மாறிலி unsigned total_மதிப்பு_len = மதிப்பு_1_len + மதிப்பு_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = மதிப்பு_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = மதிப்பு_Format2.get_device_table_indices ();
    தருக்க has_device_tables = தருக்க(format1_device_table_indices) || தருக்க(format2_device_table_indices);

    hb_set_t visited;
    க்கு (unsigned i = 0; i < class1_மொத்தம்; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      எனில் (has_device_tables) {
        க்கு (unsigned j = 0; j < class2_மொத்தம்; j++)
        {
          unsigned value1_index = total_மதிப்பு_len * (class2_மொத்தம் * i + j);
          unsigned value2_index = value1_index + மதிப்பு_1_len;
          accumulated_delta += size_of_மதிப்பு_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_மதிப்பு_record_children (c,
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
      எனில் (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<வெறுமை>::from (split_points, ALLOCATION_FAILURE));
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
      இது,
      this_index,
      class1_record_size,
      total_மதிப்பு_len,
      மதிப்பு_1_len,
      மதிப்பு_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    திரும்பு actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 தனிப்பட்ட:

  கட்டமைப்பு split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned மதிப்பு_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    மாறிலி hb_hashmap_t<unsigned, unsigned>& device_tables;
    மாறிலி hb_vector_t<unsigned>& format1_device_table_indices;
    மாறிலி hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_மொத்தம்_2 ()
    {
      திரும்பு thiz->class1_மொத்தம்_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      திரும்பு thiz->clone_range (*இது, start, end);
    }

    graph_result_t<வெறுமை> shrink (unsigned மொத்தம்)
    {
      திரும்பு thiz->shrink (*இது, மொத்தம்);
    }
  };

  size_t get_class1_record_size () மாறிலி
  {
    மாறிலி size_t class2_மொத்தம் = class2_மொத்தம்_2;
    திரும்பு
        class2_மொத்தம் * (மதிப்பு_Format1.get_size () + மதிப்பு_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) மாறிலி
  {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned இலக்கம்_records = end - start;
    unsigned பகாஎண்_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + இலக்கம்_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_பகாஎண்_id, split_context.c.create_முனை (பகாஎண்_size));

    PairPosFormat2* pair_pos_பகாஎண் =
        (PairPosFormat2*) graph.object (pair_pos_பகாஎண்_id).head;
    pair_pos_பகாஎண்->format = இது->format;
    pair_pos_பகாஎண்->மதிப்பு_Format1 = இது->மதிப்பு_Format1;
    pair_pos_பகாஎண்->மதிப்பு_Format2 = இது->மதிப்பு_Format2;
    pair_pos_பகாஎண்->class1_மொத்தம்_2 = இலக்கம்_records;
    pair_pos_பகாஎண்->class2_மொத்தம்_2 = இது->class2_மொத்தம்_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_பகாஎண்_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    மாறிலி Coverage* coverage_table = (மாறிலி Coverage*) coverage_v.obj().head;
    மாறிலி ClassDef* class_def_1_table = (மாறிலி ClassDef*) class_def_1_v.obj().head;
    எனில் (unlikely (!coverage_table || !class_def_1_table))
      திரும்பு Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      திரும்பு hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      திரும்பு klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      திரும்பு hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_பகாஎண்_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_பகாஎண்_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_பகாஎண்_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_பகாஎண்_id, பொய்));

    திரும்பு Ok(pair_pos_பகாஎண்_id);
  }

  graph_result_t<வெறுமை> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_பகாஎண்_id,
                                             unsigned start, unsigned end) மாறிலி
  {
    PairPosFormat2* pair_pos_பகாஎண் =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_பகாஎண்_id).head;

    எழுத்து* start_addr = ((எழுத்து*)&values[0]) + start * split_context.class1_record_size;
    unsigned இலக்கம்_records = end - start;
    hb_memcpy (&pair_pos_பகாஎண்->values[0],
            start_addr,
            இலக்கம்_records * split_context.class1_record_size);

    எனில் (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      திரும்பு Ok();

    unsigned class2_மொத்தம் = class2_மொத்தம்_2;
    க்கு (unsigned i = start; i < end; i++)
    {
      க்கு (unsigned j = 0; j < class2_மொத்தம்; j++)
      {
        unsigned value1_index = split_context.மதிப்பு_record_len * (class2_மொத்தம் * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.மதிப்பு_record_len * (class2_மொத்தம் * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_பகாஎண்_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_பகாஎண்_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    திரும்பு Ok();
  }

  graph_result_t<வெறுமை> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_பகாஎண்_id,
                                               மாறிலி hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_மதிப்பு_record_index,
                                               unsigned new_மதிப்பு_record_index) மாறிலி
  {
    PairPosFormat2* pair_pos_பகாஎண் =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_பகாஎண்_id).head;

    க்கு (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_மதிப்பு_record_index + i];
      unsigned record_position = ((எழுத்து*) record) - ((எழுத்து*) இது);
      எனில் (!split_context.device_tables.has (record_position)) தொடர்;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_பகாஎண்_id,
          (OT::Offset16*) &pair_pos_பகாஎண்->values[new_மதிப்பு_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    திரும்பு Ok();
  }

  graph_result_t<வெறுமை> shrink (split_context_t& split_context,
                               unsigned மொத்தம்)
  {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               மொத்தம்);
    unsigned old_மொத்தம் = class1_மொத்தம்_2;
    எனில் (மொத்தம் >= old_மொத்தம்)
      திரும்பு Ok();

    graph_t& graph = split_context.c.graph;
    class1_மொத்தம்_2 = மொத்தம்;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_மொத்தம் - மொத்தம்) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &இது->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      திரும்பு hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      திரும்பு klass < மொத்தம்;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    திரும்பு ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) மாறிலி
  {
    மாறிலி auto& v = c.graph.vertices_[this_index];
    திரும்பு v.position_to_index_map ();
  }

  graph_result_t<மாறிலி Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) மாறிலி
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    மாறிலி Coverage* coverage_table = (மாறிலி Coverage*) coverage_v.obj().head;
    எனில் (unlikely (!coverage_table)) திரும்பு Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    திரும்பு coverage_table;
  }

  graph_result_t<மாறிலி ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) மாறிலி
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    மாறிலி ClassDef* class_def_1_table = (மாறிலி ClassDef*) class_def_1_v.obj().head;
    எனில் (unlikely (!class_def_1_table)) திரும்பு Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    திரும்பு class_def_1_table;
  }

  unsigned size_of_மதிப்பு_record_children (gsubgpos_graph_context_t& c,
                                          மாறிலி hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          மாறிலி hb_vector_t<unsigned> device_table_indices,
                                          unsigned மதிப்பு_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    க்கு (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::மதிப்பு* record = &values[மதிப்பு_record_index + i];
      unsigned record_position = ((எழுத்து*) record) - ((எழுத்து*) இது);
      unsigned* obj_idx;
      எனில் (!device_tables.has (record_position, &obj_idx)) தொடர்;
      size += c.graph.find_subgraph_size (*obj_idx, visited).மதிப்பு_or (0);
    }
    திரும்பு size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    மாறிலி வெறுமை* offset) மாறிலி
  {
    TRY_ASSIGN (மாறிலி unsigned id, c.graph.index_for_offset (this_index, offset));
    திரும்பு Ok(c.graph.vertices_[id].table_size ());
  }
};

கட்டமைப்பு PairPos : பொது OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    தேர்வு (u.format.v) {
    நிலை 1:
      hb_barrier ();
      திரும்பு ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    நிலை 2:
      hb_barrier ();
      திரும்பு ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: HB_FALLTHROUGH;
    நிலை 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    இயல்பு:
      திரும்பு Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size();
    எனில் (unlikely (vertex_len < u.format.v.get_size ())) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();

    தேர்வு (u.format.v) {
    நிலை 1:
      hb_barrier ();
      திரும்பு ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    நிலை 2:
      hb_barrier ();
      திரும்பு ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    நிலை 3: HB_FALLTHROUGH;
    நிலை 4: HB_FALLTHROUGH;
#endif
    இயல்பு:
      // We don't handle format 3 and 4 here.
      திரும்பு Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
