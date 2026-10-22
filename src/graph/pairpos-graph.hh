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

সংরচনা PairPosFormat1 : সার্বজনিক OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<শূন্য> sanitize (ধ্রুব graph_t::vertex_t& vertex) ধ্রুব
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    যদি (unlikely (vertex_len < min_size)) প্রত্যাগম Err(SANITIZE_FAILURE);
    hb_barrier ();

    যদি (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      প্রত্যাগম Err(SANITIZE_FAILURE);
    প্রত্যাগম Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ধ্রুব unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    ধ্রুব unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    ধ্রুব unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;
    প্রতি (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).মান_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      যদি (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<শূন্য>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      এতত্,
      this_index,
    };

    প্রত্যাগম actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 গুপ্ত:

  সংরচনা split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_গুনতি_2 ()
    {
      প্রত্যাগম thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      প্রত্যাগম thiz->clone_range (এতত্->c, এতত্->this_index, start, end);
    }

    graph_result_t<শূন্য> shrink (unsigned গুনতি)
    {
      প্রত্যাগম thiz->shrink (এতত্->c, এতত্->this_index, গুনতি);
    }
  };

  graph_result_t<শূন্য> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned গুনতি)
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               গুনতি);
    unsigned old_গুনতি = pairSet.len;
    যদি (গুনতি >= old_গুনতি)
      প্রত্যাগম Ok();

    pairSet.len = গুনতি;
    c.graph.vertices_[this_index].shrink_buffer ((old_গুনতি - গুনতি) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &এতত্->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          প্রত্যাগম p.second < গুনতি;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    প্রত্যাগম Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ধ্রুব
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned অঙ্ক_pair_sets = end - start;
    unsigned মৌলিক_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + অঙ্ক_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_মৌলিক_id, c.create_গ্রন্থি (মৌলিক_size));

    PairPosFormat1* pair_pos_মৌলিক = (PairPosFormat1*) c.graph.object (pair_pos_মৌলিক_id).head;
    pair_pos_মৌলিক->format = এতত্->format;
    pair_pos_মৌলিক->মান_Format_2[0] = এতত্->মান_Format_2[0];
    pair_pos_মৌলিক->মান_Format_2[1] = এতত্->মান_Format_2[1];
    pair_pos_মৌলিক->pairSet.len = অঙ্ক_pair_sets;

    প্রতি (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_মৌলিক_id,
                                 &pair_pos_মৌলিক->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_মৌলিক_id,
                                   2,
                                   start, end));

    প্রত্যাগম Ok(pair_pos_মৌলিক_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) ধ্রুব
  {
    প্রত্যাগম c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

সংরচনা PairPosFormat2 : সার্বজনিক OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<শূন্য> sanitize (ধ্রুব graph_t::vertex_t& vertex) ধ্রুব
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    যদি (unlikely (vertex_len < min_size)) প্রত্যাগম Err(SANITIZE_FAILURE);
    hb_barrier ();

    ধ্রুব unsigned class1_গুনতি = class1_গুনতি_2;
    যদি (unlikely (vertex_len < min_size + class1_গুনতি * get_class1_record_size ()))
      প্রত্যাগম Err(SANITIZE_FAILURE);
    প্রত্যাগম Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ধ্রুব unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (ধ্রুব unsigned class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (ধ্রুব Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (ধ্রুব ClassDef* class_def_1, get_class_def_1 (c, this_index));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          প্রত্যাগম hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    ধ্রুব unsigned class1_গুনতি = class1_গুনতি_2;
    ধ্রুব unsigned class2_গুনতি = class2_গুনতি_2;
    ধ্রুব unsigned class1_record_size = get_class1_record_size ();

    ধ্রুব unsigned মান_1_len = মান_Format1.get_len ();
    ধ্রুব unsigned মান_2_len = মান_Format2.get_len ();
    ধ্রুব unsigned total_মান_len = মান_1_len + মান_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = মান_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = মান_Format2.get_device_table_indices ();
    তার্কিক has_device_tables = তার্কিক(format1_device_table_indices) || তার্কিক(format2_device_table_indices);

    hb_set_t visited;
    প্রতি (unsigned i = 0; i < class1_গুনতি; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      যদি (has_device_tables) {
        প্রতি (unsigned j = 0; j < class2_গুনতি; j++)
        {
          unsigned value1_index = total_মান_len * (class2_গুনতি * i + j);
          unsigned value2_index = value1_index + মান_1_len;
          accumulated_delta += size_of_মান_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_মান_record_children (c,
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
      যদি (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<শূন্য>::from (split_points, ALLOCATION_FAILURE));
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
      এতত্,
      this_index,
      class1_record_size,
      total_মান_len,
      মান_1_len,
      মান_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    প্রত্যাগম actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 গুপ্ত:

  সংরচনা split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned মান_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    ধ্রুব hb_hashmap_t<unsigned, unsigned>& device_tables;
    ধ্রুব hb_vector_t<unsigned>& format1_device_table_indices;
    ধ্রুব hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_গুনতি_2 ()
    {
      প্রত্যাগম thiz->class1_গুনতি_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      প্রত্যাগম thiz->clone_range (*এতত্, start, end);
    }

    graph_result_t<শূন্য> shrink (unsigned গুনতি)
    {
      প্রত্যাগম thiz->shrink (*এতত্, গুনতি);
    }
  };

  size_t get_class1_record_size () ধ্রুব
  {
    ধ্রুব size_t class2_গুনতি = class2_গুনতি_2;
    প্রত্যাগম
        class2_গুনতি * (মান_Format1.get_size () + মান_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) ধ্রুব
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned অঙ্ক_records = end - start;
    unsigned মৌলিক_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + অঙ্ক_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_মৌলিক_id, split_context.c.create_গ্রন্থি (মৌলিক_size));

    PairPosFormat2* pair_pos_মৌলিক =
        (PairPosFormat2*) graph.object (pair_pos_মৌলিক_id).head;
    pair_pos_মৌলিক->format = এতত্->format;
    pair_pos_মৌলিক->মান_Format1 = এতত্->মান_Format1;
    pair_pos_মৌলিক->মান_Format2 = এতত্->মান_Format2;
    pair_pos_মৌলিক->class1_গুনতি_2 = অঙ্ক_records;
    pair_pos_মৌলিক->class2_গুনতি_2 = এতত্->class2_গুনতি_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_মৌলিক_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    ধ্রুব Coverage* coverage_table = (ধ্রুব Coverage*) coverage_v.obj().head;
    ধ্রুব ClassDef* class_def_1_table = (ধ্রুব ClassDef*) class_def_1_v.obj().head;
    যদি (unlikely (!coverage_table || !class_def_1_table))
      প্রত্যাগম Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      প্রত্যাগম hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      প্রত্যাগম klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      প্রত্যাগম hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_মৌলিক_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_মৌলিক_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_মৌলিক_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_মৌলিক_id, অসত্য));

    প্রত্যাগম Ok(pair_pos_মৌলিক_id);
  }

  graph_result_t<শূন্য> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_মৌলিক_id,
                                             unsigned start, unsigned end) ধ্রুব
  {
    PairPosFormat2* pair_pos_মৌলিক =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_মৌলিক_id).head;

    বর্ণ* start_addr = ((বর্ণ*)&values[0]) + start * split_context.class1_record_size;
    unsigned অঙ্ক_records = end - start;
    hb_memcpy (&pair_pos_মৌলিক->values[0],
            start_addr,
            অঙ্ক_records * split_context.class1_record_size);

    যদি (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      প্রত্যাগম Ok();

    unsigned class2_গুনতি = class2_গুনতি_2;
    প্রতি (unsigned i = start; i < end; i++)
    {
      প্রতি (unsigned j = 0; j < class2_গুনতি; j++)
      {
        unsigned value1_index = split_context.মান_record_len * (class2_গুনতি * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.মান_record_len * (class2_গুনতি * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_মৌলিক_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_মৌলিক_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    প্রত্যাগম Ok();
  }

  graph_result_t<শূন্য> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_মৌলিক_id,
                                               ধ্রুব hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_মান_record_index,
                                               unsigned new_মান_record_index) ধ্রুব
  {
    PairPosFormat2* pair_pos_মৌলিক =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_মৌলিক_id).head;

    প্রতি (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_মান_record_index + i];
      unsigned record_position = ((বর্ণ*) record) - ((বর্ণ*) এতত্);
      যদি (!split_context.device_tables.has (record_position)) অনুবর্ত;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_মৌলিক_id,
          (OT::Offset16*) &pair_pos_মৌলিক->values[new_মান_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    প্রত্যাগম Ok();
  }

  graph_result_t<শূন্য> shrink (split_context_t& split_context,
                               unsigned গুনতি)
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               গুনতি);
    unsigned old_গুনতি = class1_গুনতি_2;
    যদি (গুনতি >= old_গুনতি)
      প্রত্যাগম Ok();

    graph_t& graph = split_context.c.graph;
    class1_গুনতি_2 = গুনতি;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_গুনতি - গুনতি) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &এতত্->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      প্রত্যাগম hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      প্রত্যাগম klass < গুনতি;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    প্রত্যাগম ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) ধ্রুব
  {
    ধ্রুব auto& v = c.graph.vertices_[this_index];
    প্রত্যাগম v.position_to_index_map ();
  }

  graph_result_t<ধ্রুব Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) ধ্রুব
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    ধ্রুব Coverage* coverage_table = (ধ্রুব Coverage*) coverage_v.obj().head;
    যদি (unlikely (!coverage_table)) প্রত্যাগম Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    প্রত্যাগম coverage_table;
  }

  graph_result_t<ধ্রুব ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) ধ্রুব
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    ধ্রুব ClassDef* class_def_1_table = (ধ্রুব ClassDef*) class_def_1_v.obj().head;
    যদি (unlikely (!class_def_1_table)) প্রত্যাগম Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    প্রত্যাগম class_def_1_table;
  }

  unsigned size_of_মান_record_children (gsubgpos_graph_context_t& c,
                                          ধ্রুব hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          ধ্রুব hb_vector_t<unsigned> device_table_indices,
                                          unsigned মান_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    প্রতি (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::মান* record = &values[মান_record_index + i];
      unsigned record_position = ((বর্ণ*) record) - ((বর্ণ*) এতত্);
      unsigned* obj_idx;
      যদি (!device_tables.has (record_position, &obj_idx)) অনুবর্ত;
      size += c.graph.find_subgraph_size (*obj_idx, visited).মান_or (0);
    }
    প্রত্যাগম size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ধ্রুব শূন্য* offset) ধ্রুব
  {
    TRY_ASSIGN (ধ্রুব unsigned id, c.graph.index_for_offset (this_index, offset));
    প্রত্যাগম Ok(c.graph.vertices_[id].table_size ());
  }
};

সংরচনা PairPos : সার্বজনিক OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    বিকল্প (u.format.v) {
    পক্ষ 1:
      hb_barrier ();
      প্রত্যাগম ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    পক্ষ 2:
      hb_barrier ();
      প্রত্যাগম ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: HB_FALLTHROUGH;
    পক্ষ 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    স্বভাব:
      প্রত্যাগম Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<শূন্য> sanitize (ধ্রুব graph_t::vertex_t& vertex) ধ্রুব
  {
    size_t vertex_len = vertex.table_size();
    যদি (unlikely (vertex_len < u.format.v.get_size ())) প্রত্যাগম Err(SANITIZE_FAILURE);
    hb_barrier ();

    বিকল্প (u.format.v) {
    পক্ষ 1:
      hb_barrier ();
      প্রত্যাগম ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    পক্ষ 2:
      hb_barrier ();
      প্রত্যাগম ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 3: HB_FALLTHROUGH;
    পক্ষ 4: HB_FALLTHROUGH;
#endif
    স্বভাব:
      // We don't handle format 3 and 4 here.
      প্রত্যাগম Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
