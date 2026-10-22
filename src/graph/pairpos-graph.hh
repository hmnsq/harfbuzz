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

#شامل "split-helpers.hh"
#شامل "coverage-graph.hh"
#شامل "classdef-graph.hh"
#شامل "../OT/Layout/GPOS/PairPos.hh"
#شامل "../OT/Layout/GPOS/PosLookupSubTable.hh"

فضاینام graph {

ساختار PairPosFormat1 : عمومی OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    اگر (unlikely (vertex_len < min_size)) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();

    اگر (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      بازگشت Err(SANITIZE_FAILURE);
    بازگشت Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ثابت unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    ثابت unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    ثابت unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;
    برای (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).مقدار_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      اگر (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<تهی>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      این,
      this_index,
    };

    بازگشت actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 خصوصی:

  ساختار split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_شمار_2 ()
    {
      بازگشت thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      بازگشت thiz->clone_range (این->c, این->this_index, start, end);
    }

    graph_result_t<تهی> shrink (unsigned شمار)
    {
      بازگشت thiz->shrink (این->c, این->this_index, شمار);
    }
  };

  graph_result_t<تهی> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned شمار)
  {
    DEBUG_پیام (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               شمار);
    unsigned old_شمار = pairSet.len;
    اگر (شمار >= old_شمار)
      بازگشت Ok();

    pairSet.len = شمار;
    c.graph.vertices_[this_index].shrink_buffer ((old_شمار - شمار) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &این->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          بازگشت p.second < شمار;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    بازگشت Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ثابت
  {
    DEBUG_پیام (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned رقم_pair_sets = end - start;
    unsigned اول_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + رقم_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_اول_id, c.create_گره (اول_size));

    PairPosFormat1* pair_pos_اول = (PairPosFormat1*) c.graph.object (pair_pos_اول_id).head;
    pair_pos_اول->format = این->format;
    pair_pos_اول->مقدار_Format_2[0] = این->مقدار_Format_2[0];
    pair_pos_اول->مقدار_Format_2[1] = این->مقدار_Format_2[1];
    pair_pos_اول->pairSet.len = رقم_pair_sets;

    برای (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_اول_id,
                                 &pair_pos_اول->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_اول_id,
                                   2,
                                   start, end));

    بازگشت Ok(pair_pos_اول_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) ثابت
  {
    بازگشت c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

ساختار PairPosFormat2 : عمومی OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    اگر (unlikely (vertex_len < min_size)) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();

    ثابت unsigned class1_شمار = class1_شمار_2;
    اگر (unlikely (vertex_len < min_size + class1_شمار * get_class1_record_size ()))
      بازگشت Err(SANITIZE_FAILURE);
    بازگشت Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ثابت unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (ثابت unsigned class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (ثابت Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (ثابت ClassDef* class_def_1, get_class_def_1 (c, this_index));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          بازگشت hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    ثابت unsigned class1_شمار = class1_شمار_2;
    ثابت unsigned class2_شمار = class2_شمار_2;
    ثابت unsigned class1_record_size = get_class1_record_size ();

    ثابت unsigned مقدار_1_len = مقدار_Format1.get_len ();
    ثابت unsigned مقدار_2_len = مقدار_Format2.get_len ();
    ثابت unsigned total_مقدار_len = مقدار_1_len + مقدار_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = مقدار_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = مقدار_Format2.get_device_table_indices ();
    منطقی has_device_tables = منطقی(format1_device_table_indices) || منطقی(format2_device_table_indices);

    hb_set_t visited;
    برای (unsigned i = 0; i < class1_شمار; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      اگر (has_device_tables) {
        برای (unsigned j = 0; j < class2_شمار; j++)
        {
          unsigned value1_index = total_مقدار_len * (class2_شمار * i + j);
          unsigned value2_index = value1_index + مقدار_1_len;
          accumulated_delta += size_of_مقدار_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_مقدار_record_children (c,
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
      اگر (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<تهی>::from (split_points, ALLOCATION_FAILURE));
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
      این,
      this_index,
      class1_record_size,
      total_مقدار_len,
      مقدار_1_len,
      مقدار_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    بازگشت actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 خصوصی:

  ساختار split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned مقدار_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    ثابت hb_hashmap_t<unsigned, unsigned>& device_tables;
    ثابت hb_vector_t<unsigned>& format1_device_table_indices;
    ثابت hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_شمار_2 ()
    {
      بازگشت thiz->class1_شمار_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      بازگشت thiz->clone_range (*این, start, end);
    }

    graph_result_t<تهی> shrink (unsigned شمار)
    {
      بازگشت thiz->shrink (*این, شمار);
    }
  };

  size_t get_class1_record_size () ثابت
  {
    ثابت size_t class2_شمار = class2_شمار_2;
    بازگشت
        class2_شمار * (مقدار_Format1.get_size () + مقدار_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) ثابت
  {
    DEBUG_پیام (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned رقم_records = end - start;
    unsigned اول_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + رقم_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_اول_id, split_context.c.create_گره (اول_size));

    PairPosFormat2* pair_pos_اول =
        (PairPosFormat2*) graph.object (pair_pos_اول_id).head;
    pair_pos_اول->format = این->format;
    pair_pos_اول->مقدار_Format1 = این->مقدار_Format1;
    pair_pos_اول->مقدار_Format2 = این->مقدار_Format2;
    pair_pos_اول->class1_شمار_2 = رقم_records;
    pair_pos_اول->class2_شمار_2 = این->class2_شمار_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_اول_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    ثابت Coverage* coverage_table = (ثابت Coverage*) coverage_v.obj().head;
    ثابت ClassDef* class_def_1_table = (ثابت ClassDef*) class_def_1_v.obj().head;
    اگر (unlikely (!coverage_table || !class_def_1_table))
      بازگشت Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      بازگشت hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      بازگشت klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      بازگشت hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_اول_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_اول_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_اول_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_اول_id, نادرست));

    بازگشت Ok(pair_pos_اول_id);
  }

  graph_result_t<تهی> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_اول_id,
                                             unsigned start, unsigned end) ثابت
  {
    PairPosFormat2* pair_pos_اول =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_اول_id).head;

    نویسه* start_addr = ((نویسه*)&values[0]) + start * split_context.class1_record_size;
    unsigned رقم_records = end - start;
    hb_memcpy (&pair_pos_اول->values[0],
            start_addr,
            رقم_records * split_context.class1_record_size);

    اگر (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      بازگشت Ok();

    unsigned class2_شمار = class2_شمار_2;
    برای (unsigned i = start; i < end; i++)
    {
      برای (unsigned j = 0; j < class2_شمار; j++)
      {
        unsigned value1_index = split_context.مقدار_record_len * (class2_شمار * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.مقدار_record_len * (class2_شمار * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_اول_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_اول_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    بازگشت Ok();
  }

  graph_result_t<تهی> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_اول_id,
                                               ثابت hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_مقدار_record_index,
                                               unsigned new_مقدار_record_index) ثابت
  {
    PairPosFormat2* pair_pos_اول =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_اول_id).head;

    برای (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_مقدار_record_index + i];
      unsigned record_position = ((نویسه*) record) - ((نویسه*) این);
      اگر (!split_context.device_tables.has (record_position)) ادامه;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_اول_id,
          (OT::Offset16*) &pair_pos_اول->values[new_مقدار_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    بازگشت Ok();
  }

  graph_result_t<تهی> shrink (split_context_t& split_context,
                               unsigned شمار)
  {
    DEBUG_پیام (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               شمار);
    unsigned old_شمار = class1_شمار_2;
    اگر (شمار >= old_شمار)
      بازگشت Ok();

    graph_t& graph = split_context.c.graph;
    class1_شمار_2 = شمار;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_شمار - شمار) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &این->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      بازگشت hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      بازگشت klass < شمار;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    بازگشت ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) ثابت
  {
    ثابت auto& v = c.graph.vertices_[this_index];
    بازگشت v.position_to_index_map ();
  }

  graph_result_t<ثابت Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) ثابت
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    ثابت Coverage* coverage_table = (ثابت Coverage*) coverage_v.obj().head;
    اگر (unlikely (!coverage_table)) بازگشت Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    بازگشت coverage_table;
  }

  graph_result_t<ثابت ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) ثابت
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    ثابت ClassDef* class_def_1_table = (ثابت ClassDef*) class_def_1_v.obj().head;
    اگر (unlikely (!class_def_1_table)) بازگشت Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    بازگشت class_def_1_table;
  }

  unsigned size_of_مقدار_record_children (gsubgpos_graph_context_t& c,
                                          ثابت hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          ثابت hb_vector_t<unsigned> device_table_indices,
                                          unsigned مقدار_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    برای (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::مقدار* record = &values[مقدار_record_index + i];
      unsigned record_position = ((نویسه*) record) - ((نویسه*) این);
      unsigned* obj_idx;
      اگر (!device_tables.has (record_position, &obj_idx)) ادامه;
      size += c.graph.find_subgraph_size (*obj_idx, visited).مقدار_or (0);
    }
    بازگشت size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ثابت تهی* offset) ثابت
  {
    TRY_ASSIGN (ثابت unsigned id, c.graph.index_for_offset (this_index, offset));
    بازگشت Ok(c.graph.vertices_[id].table_size ());
  }
};

ساختار PairPos : عمومی OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      بازگشت ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    حالت 2:
      hb_barrier ();
      بازگشت ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    حالت 3: HB_FALLTHROUGH;
    حالت 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    پیشفرض:
      بازگشت Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<تهی> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size();
    اگر (unlikely (vertex_len < u.format.v.get_size ())) بازگشت Err(SANITIZE_FAILURE);
    hb_barrier ();

    انتخاب (u.format.v) {
    حالت 1:
      hb_barrier ();
      بازگشت ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    حالت 2:
      hb_barrier ();
      بازگشت ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    حالت 3: HB_FALLTHROUGH;
    حالت 4: HB_FALLTHROUGH;
#endif
    پیشفرض:
      // We don't handle format 3 and 4 here.
      بازگشت Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
