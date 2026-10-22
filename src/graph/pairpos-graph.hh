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

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "classdef-graph.hh"
#ضمن "../OT/Layout/GPOS/PairPos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق graph {

بنية PairPosFormat1 : عام OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    إذا (unlikely (vertex_len < min_size)) إرجاع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      إرجاع Err(SANITIZE_FAILURE);
    إرجاع Ok();
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
    لكل (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).قيمة_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      إذا (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<فراغ>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      هذا,
      this_index,
    };

    إرجاع actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 خاص:

  بنية split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_تعداد_2 ()
    {
      إرجاع thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      إرجاع thiz->clone_range (هذا->c, هذا->this_index, start, end);
    }

    graph_result_t<فراغ> shrink (unsigned تعداد_2)
    {
      إرجاع thiz->shrink (هذا->c, هذا->this_index, تعداد_2);
    }
  };

  graph_result_t<فراغ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned تعداد_2)
  {
    DEBUG_رسالة (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               تعداد_2);
    unsigned old_تعداد = pairSet.len;
    إذا (تعداد_2 >= old_تعداد)
      إرجاع Ok();

    pairSet.len = تعداد_2;
    c.graph.vertices_[this_index].shrink_buffer ((old_تعداد - تعداد_2) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &هذا->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          إرجاع p.second < تعداد_2;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    إرجاع Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ثابت
  {
    DEBUG_رسالة (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned رقم_pair_sets = end - start;
    unsigned أولي_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + رقم_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_أولي_id, c.create_عقدة (أولي_size));

    PairPosFormat1* pair_pos_أولي = (PairPosFormat1*) c.graph.object (pair_pos_أولي_id).head;
    pair_pos_أولي->format = هذا->format;
    pair_pos_أولي->قيمة_Format_2[0] = هذا->قيمة_Format_2[0];
    pair_pos_أولي->قيمة_Format_2[1] = هذا->قيمة_Format_2[1];
    pair_pos_أولي->pairSet.len = رقم_pair_sets;

    لكل (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_أولي_id,
                                 &pair_pos_أولي->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_أولي_id,
                                   2,
                                   start, end));

    إرجاع Ok(pair_pos_أولي_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) ثابت
  {
    إرجاع c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

بنية PairPosFormat2 : عام OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    إذا (unlikely (vertex_len < min_size)) إرجاع Err(SANITIZE_FAILURE);
    hb_barrier ();

    ثابت unsigned class1_تعداد = class1_تعداد_2;
    إذا (unlikely (vertex_len < min_size + class1_تعداد * get_class1_record_size ()))
      إرجاع Err(SANITIZE_FAILURE);
    إرجاع Ok();
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
          إرجاع hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    ثابت unsigned class1_تعداد = class1_تعداد_2;
    ثابت unsigned class2_تعداد = class2_تعداد_2;
    ثابت unsigned class1_record_size = get_class1_record_size ();

    ثابت unsigned قيمة_1_len = قيمة_Format1.get_len ();
    ثابت unsigned قيمة_2_len = قيمة_Format2.get_len ();
    ثابت unsigned total_قيمة_len = قيمة_1_len + قيمة_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = قيمة_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = قيمة_Format2.get_device_table_indices ();
    منطقي has_device_tables = منطقي(format1_device_table_indices) || منطقي(format2_device_table_indices);

    hb_set_t visited;
    لكل (unsigned i = 0; i < class1_تعداد; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      إذا (has_device_tables) {
        لكل (unsigned j = 0; j < class2_تعداد; j++)
        {
          unsigned value1_index = total_قيمة_len * (class2_تعداد * i + j);
          unsigned value2_index = value1_index + قيمة_1_len;
          accumulated_delta += size_of_قيمة_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_قيمة_record_children (c,
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
      إذا (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<فراغ>::from (split_points, ALLOCATION_FAILURE));
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
      هذا,
      this_index,
      class1_record_size,
      total_قيمة_len,
      قيمة_1_len,
      قيمة_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    إرجاع actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 خاص:

  بنية split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned قيمة_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    ثابت hb_hashmap_t<unsigned, unsigned>& device_tables;
    ثابت hb_vector_t<unsigned>& format1_device_table_indices;
    ثابت hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_تعداد_2 ()
    {
      إرجاع thiz->class1_تعداد_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      إرجاع thiz->clone_range (*هذا, start, end);
    }

    graph_result_t<فراغ> shrink (unsigned تعداد_2)
    {
      إرجاع thiz->shrink (*هذا, تعداد_2);
    }
  };

  size_t get_class1_record_size () ثابت
  {
    ثابت size_t class2_تعداد = class2_تعداد_2;
    إرجاع
        class2_تعداد * (قيمة_Format1.get_size () + قيمة_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) ثابت
  {
    DEBUG_رسالة (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned رقم_records = end - start;
    unsigned أولي_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + رقم_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_أولي_id, split_context.c.create_عقدة (أولي_size));

    PairPosFormat2* pair_pos_أولي =
        (PairPosFormat2*) graph.object (pair_pos_أولي_id).head;
    pair_pos_أولي->format = هذا->format;
    pair_pos_أولي->قيمة_Format1 = هذا->قيمة_Format1;
    pair_pos_أولي->قيمة_Format2 = هذا->قيمة_Format2;
    pair_pos_أولي->class1_تعداد_2 = رقم_records;
    pair_pos_أولي->class2_تعداد_2 = هذا->class2_تعداد_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_أولي_id,
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
    إذا (unlikely (!coverage_table || !class_def_1_table))
      إرجاع Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      إرجاع hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      إرجاع klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      إرجاع hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_أولي_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_أولي_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_أولي_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_أولي_id, خطأ));

    إرجاع Ok(pair_pos_أولي_id);
  }

  graph_result_t<فراغ> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_أولي_id,
                                             unsigned start, unsigned end) ثابت
  {
    PairPosFormat2* pair_pos_أولي =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_أولي_id).head;

    حرف* start_addr = ((حرف*)&values[0]) + start * split_context.class1_record_size;
    unsigned رقم_records = end - start;
    hb_memcpy (&pair_pos_أولي->values[0],
            start_addr,
            رقم_records * split_context.class1_record_size);

    إذا (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      إرجاع Ok();

    unsigned class2_تعداد = class2_تعداد_2;
    لكل (unsigned i = start; i < end; i++)
    {
      لكل (unsigned j = 0; j < class2_تعداد; j++)
      {
        unsigned value1_index = split_context.قيمة_record_len * (class2_تعداد * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.قيمة_record_len * (class2_تعداد * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_أولي_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_أولي_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    إرجاع Ok();
  }

  graph_result_t<فراغ> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_أولي_id,
                                               ثابت hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_قيمة_record_index,
                                               unsigned new_قيمة_record_index) ثابت
  {
    PairPosFormat2* pair_pos_أولي =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_أولي_id).head;

    لكل (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_قيمة_record_index + i];
      unsigned record_position = ((حرف*) record) - ((حرف*) هذا);
      إذا (!split_context.device_tables.has (record_position)) استمر;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_أولي_id,
          (OT::Offset16*) &pair_pos_أولي->values[new_قيمة_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    إرجاع Ok();
  }

  graph_result_t<فراغ> shrink (split_context_t& split_context,
                               unsigned تعداد_2)
  {
    DEBUG_رسالة (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               تعداد_2);
    unsigned old_تعداد = class1_تعداد_2;
    إذا (تعداد_2 >= old_تعداد)
      إرجاع Ok();

    graph_t& graph = split_context.c.graph;
    class1_تعداد_2 = تعداد_2;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_تعداد - تعداد_2) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &هذا->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      إرجاع hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      إرجاع klass < تعداد_2;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    إرجاع ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) ثابت
  {
    ثابت auto& v = c.graph.vertices_[this_index];
    إرجاع v.position_to_index_map ();
  }

  graph_result_t<ثابت Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) ثابت
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    ثابت Coverage* coverage_table = (ثابت Coverage*) coverage_v.obj().head;
    إذا (unlikely (!coverage_table)) إرجاع Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    إرجاع coverage_table;
  }

  graph_result_t<ثابت ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) ثابت
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    ثابت ClassDef* class_def_1_table = (ثابت ClassDef*) class_def_1_v.obj().head;
    إذا (unlikely (!class_def_1_table)) إرجاع Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    إرجاع class_def_1_table;
  }

  unsigned size_of_قيمة_record_children (gsubgpos_graph_context_t& c,
                                          ثابت hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          ثابت hb_vector_t<unsigned> device_table_indices,
                                          unsigned قيمة_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    لكل (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::قيمة* record = &values[قيمة_record_index + i];
      unsigned record_position = ((حرف*) record) - ((حرف*) هذا);
      unsigned* obj_idx;
      إذا (!device_tables.has (record_position, &obj_idx)) استمر;
      size += c.graph.find_subgraph_size (*obj_idx, visited).قيمة_or (0);
    }
    إرجاع size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ثابت فراغ* offset) ثابت
  {
    TRY_ASSIGN (ثابت unsigned id, c.graph.index_for_offset (this_index, offset));
    إرجاع Ok(c.graph.vertices_[id].table_size ());
  }
};

بنية PairPos : عام OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      إرجاع ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    حالة 2:
      hb_barrier ();
      إرجاع ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    افتراضي:
      إرجاع Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size();
    إذا (unlikely (vertex_len < u.format.v.get_size ())) إرجاع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      إرجاع ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    حالة 2:
      hb_barrier ();
      إرجاع ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    حالة 3: HB_FALLTHROUGH;
    حالة 4: HB_FALLTHROUGH;
#endif
    افتراضي:
      // We don't handle format 3 and 4 here.
      إرجاع Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
