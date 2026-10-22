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

#അന്തര്ഭാവ "split-helpers.hh"
#അന്തര്ഭാവ "coverage-graph.hh"
#അന്തര്ഭാവ "classdef-graph.hh"
#അന്തര്ഭാവ "../OT/Layout/GPOS/PairPos.hh"
#അന്തര്ഭാവ "../OT/Layout/GPOS/PosLookupSubTable.hh"

നാമാകാശ graph {

ഘടന PairPosFormat1 : പൊതു OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    എങ്കിൽ (unlikely (vertex_len < min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();

    എങ്കിൽ (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (സ്ഥിരം unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    സ്ഥിരം unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    സ്ഥിരം unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;
    വേണ്ടി (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).മൂല്യം_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      എങ്കിൽ (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<ശൂന്യം>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      ഇത്,
      this_index,
    };

    തിരികെ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 സ്വകാര്യ:

  ഘടന split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_എണ്ണം_2 ()
    {
      തിരികെ thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      തിരികെ thiz->clone_range (ഇത്->c, ഇത്->this_index, start, end);
    }

    graph_result_t<ശൂന്യം> shrink (unsigned എണ്ണം)
    {
      തിരികെ thiz->shrink (ഇത്->c, ഇത്->this_index, എണ്ണം);
    }
  };

  graph_result_t<ശൂന്യം> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned എണ്ണം)
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               എണ്ണം);
    unsigned old_എണ്ണം = pairSet.len;
    എങ്കിൽ (എണ്ണം >= old_എണ്ണം)
      തിരികെ Ok();

    pairSet.len = എണ്ണം;
    c.graph.vertices_[this_index].shrink_buffer ((old_എണ്ണം - എണ്ണം) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &ഇത്->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          തിരികെ p.second < എണ്ണം;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    തിരികെ Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) സ്ഥിരം
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned അക്കം_pair_sets = end - start;
    unsigned അഭാജ്യം_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + അക്കം_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_അഭാജ്യം_id, c.create_കണ്ണി (അഭാജ്യം_size));

    PairPosFormat1* pair_pos_അഭാജ്യം = (PairPosFormat1*) c.graph.object (pair_pos_അഭാജ്യം_id).head;
    pair_pos_അഭാജ്യം->format = ഇത്->format;
    pair_pos_അഭാജ്യം->മൂല്യം_Format_2[0] = ഇത്->മൂല്യം_Format_2[0];
    pair_pos_അഭാജ്യം->മൂല്യം_Format_2[1] = ഇത്->മൂല്യം_Format_2[1];
    pair_pos_അഭാജ്യം->pairSet.len = അക്കം_pair_sets;

    വേണ്ടി (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_അഭാജ്യം_id,
                                 &pair_pos_അഭാജ്യം->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_അഭാജ്യം_id,
                                   2,
                                   start, end));

    തിരികെ Ok(pair_pos_അഭാജ്യം_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) സ്ഥിരം
  {
    തിരികെ c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

ഘടന PairPosFormat2 : പൊതു OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    എങ്കിൽ (unlikely (vertex_len < min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();

    സ്ഥിരം unsigned class1_എണ്ണം = class1_എണ്ണം_2;
    എങ്കിൽ (unlikely (vertex_len < min_size + class1_എണ്ണം * get_class1_record_size ()))
      തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    സ്ഥിരം unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (സ്ഥിരം unsigned class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (സ്ഥിരം Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (സ്ഥിരം ClassDef* class_def_1, get_class_def_1 (c, this_index));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          തിരികെ hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    സ്ഥിരം unsigned class1_എണ്ണം = class1_എണ്ണം_2;
    സ്ഥിരം unsigned class2_എണ്ണം = class2_എണ്ണം_2;
    സ്ഥിരം unsigned class1_record_size = get_class1_record_size ();

    സ്ഥിരം unsigned മൂല്യം_1_len = മൂല്യം_Format1.get_len ();
    സ്ഥിരം unsigned മൂല്യം_2_len = മൂല്യം_Format2.get_len ();
    സ്ഥിരം unsigned total_മൂല്യം_len = മൂല്യം_1_len + മൂല്യം_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = മൂല്യം_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = മൂല്യം_Format2.get_device_table_indices ();
    തർക്കം has_device_tables = തർക്കം(format1_device_table_indices) || തർക്കം(format2_device_table_indices);

    hb_set_t visited;
    വേണ്ടി (unsigned i = 0; i < class1_എണ്ണം; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      എങ്കിൽ (has_device_tables) {
        വേണ്ടി (unsigned j = 0; j < class2_എണ്ണം; j++)
        {
          unsigned value1_index = total_മൂല്യം_len * (class2_എണ്ണം * i + j);
          unsigned value2_index = value1_index + മൂല്യം_1_len;
          accumulated_delta += size_of_മൂല്യം_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_മൂല്യം_record_children (c,
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
      എങ്കിൽ (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<ശൂന്യം>::from (split_points, ALLOCATION_FAILURE));
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
      ഇത്,
      this_index,
      class1_record_size,
      total_മൂല്യം_len,
      മൂല്യം_1_len,
      മൂല്യം_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    തിരികെ actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 സ്വകാര്യ:

  ഘടന split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned മൂല്യം_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    സ്ഥിരം hb_hashmap_t<unsigned, unsigned>& device_tables;
    സ്ഥിരം hb_vector_t<unsigned>& format1_device_table_indices;
    സ്ഥിരം hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_എണ്ണം_2 ()
    {
      തിരികെ thiz->class1_എണ്ണം_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      തിരികെ thiz->clone_range (*ഇത്, start, end);
    }

    graph_result_t<ശൂന്യം> shrink (unsigned എണ്ണം)
    {
      തിരികെ thiz->shrink (*ഇത്, എണ്ണം);
    }
  };

  size_t get_class1_record_size () സ്ഥിരം
  {
    സ്ഥിരം size_t class2_എണ്ണം = class2_എണ്ണം_2;
    തിരികെ
        class2_എണ്ണം * (മൂല്യം_Format1.get_size () + മൂല്യം_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) സ്ഥിരം
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned അക്കം_records = end - start;
    unsigned അഭാജ്യം_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + അക്കം_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_അഭാജ്യം_id, split_context.c.create_കണ്ണി (അഭാജ്യം_size));

    PairPosFormat2* pair_pos_അഭാജ്യം =
        (PairPosFormat2*) graph.object (pair_pos_അഭാജ്യം_id).head;
    pair_pos_അഭാജ്യം->format = ഇത്->format;
    pair_pos_അഭാജ്യം->മൂല്യം_Format1 = ഇത്->മൂല്യം_Format1;
    pair_pos_അഭാജ്യം->മൂല്യം_Format2 = ഇത്->മൂല്യം_Format2;
    pair_pos_അഭാജ്യം->class1_എണ്ണം_2 = അക്കം_records;
    pair_pos_അഭാജ്യം->class2_എണ്ണം_2 = ഇത്->class2_എണ്ണം_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_അഭാജ്യം_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    സ്ഥിരം Coverage* coverage_table = (സ്ഥിരം Coverage*) coverage_v.obj().head;
    സ്ഥിരം ClassDef* class_def_1_table = (സ്ഥിരം ClassDef*) class_def_1_v.obj().head;
    എങ്കിൽ (unlikely (!coverage_table || !class_def_1_table))
      തിരികെ Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      തിരികെ hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      തിരികെ klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      തിരികെ hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_അഭാജ്യം_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_അഭാജ്യം_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_അഭാജ്യം_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_അഭാജ്യം_id, അസത്യം));

    തിരികെ Ok(pair_pos_അഭാജ്യം_id);
  }

  graph_result_t<ശൂന്യം> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_അഭാജ്യം_id,
                                             unsigned start, unsigned end) സ്ഥിരം
  {
    PairPosFormat2* pair_pos_അഭാജ്യം =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_അഭാജ്യം_id).head;

    അക്ഷരം* start_addr = ((അക്ഷരം*)&values[0]) + start * split_context.class1_record_size;
    unsigned അക്കം_records = end - start;
    hb_memcpy (&pair_pos_അഭാജ്യം->values[0],
            start_addr,
            അക്കം_records * split_context.class1_record_size);

    എങ്കിൽ (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      തിരികെ Ok();

    unsigned class2_എണ്ണം = class2_എണ്ണം_2;
    വേണ്ടി (unsigned i = start; i < end; i++)
    {
      വേണ്ടി (unsigned j = 0; j < class2_എണ്ണം; j++)
      {
        unsigned value1_index = split_context.മൂല്യം_record_len * (class2_എണ്ണം * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.മൂല്യം_record_len * (class2_എണ്ണം * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_അഭാജ്യം_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_അഭാജ്യം_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    തിരികെ Ok();
  }

  graph_result_t<ശൂന്യം> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_അഭാജ്യം_id,
                                               സ്ഥിരം hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_മൂല്യം_record_index,
                                               unsigned new_മൂല്യം_record_index) സ്ഥിരം
  {
    PairPosFormat2* pair_pos_അഭാജ്യം =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_അഭാജ്യം_id).head;

    വേണ്ടി (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_മൂല്യം_record_index + i];
      unsigned record_position = ((അക്ഷരം*) record) - ((അക്ഷരം*) ഇത്);
      എങ്കിൽ (!split_context.device_tables.has (record_position)) തുടരുക;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_അഭാജ്യം_id,
          (OT::Offset16*) &pair_pos_അഭാജ്യം->values[new_മൂല്യം_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    തിരികെ Ok();
  }

  graph_result_t<ശൂന്യം> shrink (split_context_t& split_context,
                               unsigned എണ്ണം)
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               എണ്ണം);
    unsigned old_എണ്ണം = class1_എണ്ണം_2;
    എങ്കിൽ (എണ്ണം >= old_എണ്ണം)
      തിരികെ Ok();

    graph_t& graph = split_context.c.graph;
    class1_എണ്ണം_2 = എണ്ണം;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_എണ്ണം - എണ്ണം) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &ഇത്->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      തിരികെ hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      തിരികെ klass < എണ്ണം;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    തിരികെ ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) സ്ഥിരം
  {
    സ്ഥിരം auto& v = c.graph.vertices_[this_index];
    തിരികെ v.position_to_index_map ();
  }

  graph_result_t<സ്ഥിരം Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) സ്ഥിരം
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    സ്ഥിരം Coverage* coverage_table = (സ്ഥിരം Coverage*) coverage_v.obj().head;
    എങ്കിൽ (unlikely (!coverage_table)) തിരികെ Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    തിരികെ coverage_table;
  }

  graph_result_t<സ്ഥിരം ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) സ്ഥിരം
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    സ്ഥിരം ClassDef* class_def_1_table = (സ്ഥിരം ClassDef*) class_def_1_v.obj().head;
    എങ്കിൽ (unlikely (!class_def_1_table)) തിരികെ Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    തിരികെ class_def_1_table;
  }

  unsigned size_of_മൂല്യം_record_children (gsubgpos_graph_context_t& c,
                                          സ്ഥിരം hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          സ്ഥിരം hb_vector_t<unsigned> device_table_indices,
                                          unsigned മൂല്യം_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    വേണ്ടി (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::മൂല്യം* record = &values[മൂല്യം_record_index + i];
      unsigned record_position = ((അക്ഷരം*) record) - ((അക്ഷരം*) ഇത്);
      unsigned* obj_idx;
      എങ്കിൽ (!device_tables.has (record_position, &obj_idx)) തുടരുക;
      size += c.graph.find_subgraph_size (*obj_idx, visited).മൂല്യം_or (0);
    }
    തിരികെ size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    സ്ഥിരം ശൂന്യം* offset) സ്ഥിരം
  {
    TRY_ASSIGN (സ്ഥിരം unsigned id, c.graph.index_for_offset (this_index, offset));
    തിരികെ Ok(c.graph.vertices_[id].table_size ());
  }
};

ഘടന PairPos : പൊതു OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1:
      hb_barrier ();
      തിരികെ ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    അവസ്ഥ 2:
      hb_barrier ();
      തിരികെ ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: HB_FALLTHROUGH;
    അവസ്ഥ 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    സ്ഥിരസ്ഥിതി:
      തിരികെ Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size();
    എങ്കിൽ (unlikely (vertex_len < u.format.v.get_size ())) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();

    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1:
      hb_barrier ();
      തിരികെ ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    അവസ്ഥ 2:
      hb_barrier ();
      തിരികെ ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 3: HB_FALLTHROUGH;
    അവസ്ഥ 4: HB_FALLTHROUGH;
#endif
    സ്ഥിരസ്ഥിതി:
      // We don't handle format 3 and 4 here.
      തിരികെ Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
