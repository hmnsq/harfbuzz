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

#અન્તર્ભાવ "split-helpers.hh"
#અન્તર્ભાવ "coverage-graph.hh"
#અન્તર્ભાવ "classdef-graph.hh"
#અન્તર્ભાવ "../OT/Layout/GPOS/PairPos.hh"
#અન્તર્ભાવ "../OT/Layout/GPOS/PosLookupSubTable.hh"

નામાકાશ graph {

સંરચના PairPosFormat1 : જાહેર OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    જો (unlikely (vertex_len < min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();

    જો (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      પાછા Err(SANITIZE_FAILURE);
    પાછા Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (અચલ unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    અચલ unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    અચલ unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;
    માટે (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).કિંમત_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      જો (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<શૂન્ય>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      આ,
      this_index,
    };

    પાછા actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ખાનગી:

  સંરચના split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_ગણતરી_2 ()
    {
      પાછા thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      પાછા thiz->clone_range (આ->c, આ->this_index, start, end);
    }

    graph_result_t<શૂન્ય> shrink (unsigned ગણતરી)
    {
      પાછા thiz->shrink (આ->c, આ->this_index, ગણતરી);
    }
  };

  graph_result_t<શૂન્ય> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned ગણતરી)
  {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               ગણતરી);
    unsigned old_ગણતરી = pairSet.len;
    જો (ગણતરી >= old_ગણતરી)
      પાછા Ok();

    pairSet.len = ગણતરી;
    c.graph.vertices_[this_index].shrink_buffer ((old_ગણતરી - ગણતરી) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &આ->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          પાછા p.second < ગણતરી;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    પાછા Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) અચલ
  {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned અંક_pair_sets = end - start;
    unsigned અવિભાજ્ય_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + અંક_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_અવિભાજ્ય_id, c.create_ગાંઠ (અવિભાજ્ય_size));

    PairPosFormat1* pair_pos_અવિભાજ્ય = (PairPosFormat1*) c.graph.object (pair_pos_અવિભાજ્ય_id).head;
    pair_pos_અવિભાજ્ય->format = આ->format;
    pair_pos_અવિભાજ્ય->કિંમત_Format_2[0] = આ->કિંમત_Format_2[0];
    pair_pos_અવિભાજ્ય->કિંમત_Format_2[1] = આ->કિંમત_Format_2[1];
    pair_pos_અવિભાજ્ય->pairSet.len = અંક_pair_sets;

    માટે (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_અવિભાજ્ય_id,
                                 &pair_pos_અવિભાજ્ય->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_અવિભાજ્ય_id,
                                   2,
                                   start, end));

    પાછા Ok(pair_pos_અવિભાજ્ય_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) અચલ
  {
    પાછા c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

સંરચના PairPosFormat2 : જાહેર OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    જો (unlikely (vertex_len < min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();

    અચલ unsigned class1_ગણતરી = class1_ગણતરી_2;
    જો (unlikely (vertex_len < min_size + class1_ગણતરી * get_class1_record_size ()))
      પાછા Err(SANITIZE_FAILURE);
    પાછા Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    અચલ unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (અચલ unsigned class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (અચલ Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (અચલ ClassDef* class_def_1, get_class_def_1 (c, this_index));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          પાછા hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    અચલ unsigned class1_ગણતરી = class1_ગણતરી_2;
    અચલ unsigned class2_ગણતરી = class2_ગણતરી_2;
    અચલ unsigned class1_record_size = get_class1_record_size ();

    અચલ unsigned કિંમત_1_len = કિંમત_Format1.get_len ();
    અચલ unsigned કિંમત_2_len = કિંમત_Format2.get_len ();
    અચલ unsigned total_કિંમત_len = કિંમત_1_len + કિંમત_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = કિંમત_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = કિંમત_Format2.get_device_table_indices ();
    તાર્કિક has_device_tables = તાર્કિક(format1_device_table_indices) || તાર્કિક(format2_device_table_indices);

    hb_set_t visited;
    માટે (unsigned i = 0; i < class1_ગણતરી; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      જો (has_device_tables) {
        માટે (unsigned j = 0; j < class2_ગણતરી; j++)
        {
          unsigned value1_index = total_કિંમત_len * (class2_ગણતરી * i + j);
          unsigned value2_index = value1_index + કિંમત_1_len;
          accumulated_delta += size_of_કિંમત_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_કિંમત_record_children (c,
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
      જો (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<શૂન્ય>::from (split_points, ALLOCATION_FAILURE));
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
      આ,
      this_index,
      class1_record_size,
      total_કિંમત_len,
      કિંમત_1_len,
      કિંમત_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    પાછા actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 ખાનગી:

  સંરચના split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned કિંમત_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    અચલ hb_hashmap_t<unsigned, unsigned>& device_tables;
    અચલ hb_vector_t<unsigned>& format1_device_table_indices;
    અચલ hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_ગણતરી_2 ()
    {
      પાછા thiz->class1_ગણતરી_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      પાછા thiz->clone_range (*આ, start, end);
    }

    graph_result_t<શૂન્ય> shrink (unsigned ગણતરી)
    {
      પાછા thiz->shrink (*આ, ગણતરી);
    }
  };

  size_t get_class1_record_size () અચલ
  {
    અચલ size_t class2_ગણતરી = class2_ગણતરી_2;
    પાછા
        class2_ગણતરી * (કિંમત_Format1.get_size () + કિંમત_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) અચલ
  {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned અંક_records = end - start;
    unsigned અવિભાજ્ય_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + અંક_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_અવિભાજ્ય_id, split_context.c.create_ગાંઠ (અવિભાજ્ય_size));

    PairPosFormat2* pair_pos_અવિભાજ્ય =
        (PairPosFormat2*) graph.object (pair_pos_અવિભાજ્ય_id).head;
    pair_pos_અવિભાજ્ય->format = આ->format;
    pair_pos_અવિભાજ્ય->કિંમત_Format1 = આ->કિંમત_Format1;
    pair_pos_અવિભાજ્ય->કિંમત_Format2 = આ->કિંમત_Format2;
    pair_pos_અવિભાજ્ય->class1_ગણતરી_2 = અંક_records;
    pair_pos_અવિભાજ્ય->class2_ગણતરી_2 = આ->class2_ગણતરી_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_અવિભાજ્ય_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    અચલ Coverage* coverage_table = (અચલ Coverage*) coverage_v.obj().head;
    અચલ ClassDef* class_def_1_table = (અચલ ClassDef*) class_def_1_v.obj().head;
    જો (unlikely (!coverage_table || !class_def_1_table))
      પાછા Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      પાછા hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      પાછા klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      પાછા hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_અવિભાજ્ય_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_અવિભાજ્ય_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_અવિભાજ્ય_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_અવિભાજ્ય_id, ખોટું));

    પાછા Ok(pair_pos_અવિભાજ્ય_id);
  }

  graph_result_t<શૂન્ય> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_અવિભાજ્ય_id,
                                             unsigned start, unsigned end) અચલ
  {
    PairPosFormat2* pair_pos_અવિભાજ્ય =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_અવિભાજ્ય_id).head;

    અક્ષર* start_addr = ((અક્ષર*)&values[0]) + start * split_context.class1_record_size;
    unsigned અંક_records = end - start;
    hb_memcpy (&pair_pos_અવિભાજ્ય->values[0],
            start_addr,
            અંક_records * split_context.class1_record_size);

    જો (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      પાછા Ok();

    unsigned class2_ગણતરી = class2_ગણતરી_2;
    માટે (unsigned i = start; i < end; i++)
    {
      માટે (unsigned j = 0; j < class2_ગણતરી; j++)
      {
        unsigned value1_index = split_context.કિંમત_record_len * (class2_ગણતરી * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.કિંમત_record_len * (class2_ગણતરી * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_અવિભાજ્ય_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_અવિભાજ્ય_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    પાછા Ok();
  }

  graph_result_t<શૂન્ય> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_અવિભાજ્ય_id,
                                               અચલ hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_કિંમત_record_index,
                                               unsigned new_કિંમત_record_index) અચલ
  {
    PairPosFormat2* pair_pos_અવિભાજ્ય =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_અવિભાજ્ય_id).head;

    માટે (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_કિંમત_record_index + i];
      unsigned record_position = ((અક્ષર*) record) - ((અક્ષર*) આ);
      જો (!split_context.device_tables.has (record_position)) ચાલુ;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_અવિભાજ્ય_id,
          (OT::Offset16*) &pair_pos_અવિભાજ્ય->values[new_કિંમત_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    પાછા Ok();
  }

  graph_result_t<શૂન્ય> shrink (split_context_t& split_context,
                               unsigned ગણતરી)
  {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               ગણતરી);
    unsigned old_ગણતરી = class1_ગણતરી_2;
    જો (ગણતરી >= old_ગણતરી)
      પાછા Ok();

    graph_t& graph = split_context.c.graph;
    class1_ગણતરી_2 = ગણતરી;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_ગણતરી - ગણતરી) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &આ->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      પાછા hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      પાછા klass < ગણતરી;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    પાછા ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) અચલ
  {
    અચલ auto& v = c.graph.vertices_[this_index];
    પાછા v.position_to_index_map ();
  }

  graph_result_t<અચલ Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) અચલ
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    અચલ Coverage* coverage_table = (અચલ Coverage*) coverage_v.obj().head;
    જો (unlikely (!coverage_table)) પાછા Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    પાછા coverage_table;
  }

  graph_result_t<અચલ ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) અચલ
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    અચલ ClassDef* class_def_1_table = (અચલ ClassDef*) class_def_1_v.obj().head;
    જો (unlikely (!class_def_1_table)) પાછા Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    પાછા class_def_1_table;
  }

  unsigned size_of_કિંમત_record_children (gsubgpos_graph_context_t& c,
                                          અચલ hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          અચલ hb_vector_t<unsigned> device_table_indices,
                                          unsigned કિંમત_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    માટે (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::કિંમત* record = &values[કિંમત_record_index + i];
      unsigned record_position = ((અક્ષર*) record) - ((અક્ષર*) આ);
      unsigned* obj_idx;
      જો (!device_tables.has (record_position, &obj_idx)) ચાલુ;
      size += c.graph.find_subgraph_size (*obj_idx, visited).કિંમત_or (0);
    }
    પાછા size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    અચલ શૂન્ય* offset) અચલ
  {
    TRY_ASSIGN (અચલ unsigned id, c.graph.index_for_offset (this_index, offset));
    પાછા Ok(c.graph.vertices_[id].table_size ());
  }
};

સંરચના PairPos : જાહેર OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1:
      hb_barrier ();
      પાછા ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    સ્થિતિ 2:
      hb_barrier ();
      પાછા ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: HB_FALLTHROUGH;
    સ્થિતિ 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    મૂળભૂત:
      પાછા Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size();
    જો (unlikely (vertex_len < u.format.v.get_size ())) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();

    પસંદગી (u.format.v) {
    સ્થિતિ 1:
      hb_barrier ();
      પાછા ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    સ્થિતિ 2:
      hb_barrier ();
      પાછા ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: HB_FALLTHROUGH;
    સ્થિતિ 4: HB_FALLTHROUGH;
#endif
    મૂળભૂત:
      // We don't handle format 3 and 4 here.
      પાછા Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
