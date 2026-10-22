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

#अन्तर्भाव "split-helpers.hh"
#अन्तर्भाव "coverage-graph.hh"
#अन्तर्भाव "classdef-graph.hh"
#अन्तर्भाव "../OT/Layout/GPOS/PairPos.hh"
#अन्तर्भाव "../OT/Layout/GPOS/PosLookupSubTable.hh"

नामाकाश graph {

संरचना PairPosFormat1 : सार्वजनिक OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    जइ (unlikely (vertex_len < min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();

    जइ (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ध्रुव unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    ध्रुव unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    ध्रुव unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;
    प्रति (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).मानम्_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      जइ (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<शून्य>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      एतत्,
      this_index,
    };

    प्रत्यागम actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 गुप्त:

  संरचना split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_गणनम्_2 ()
    {
      प्रत्यागम thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      प्रत्यागम thiz->clone_range (एतत्->c, एतत्->this_index, start, end);
    }

    graph_result_t<शून्य> shrink (unsigned गणनम्)
    {
      प्रत्यागम thiz->shrink (एतत्->c, एतत्->this_index, गणनम्);
    }
  };

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned गणनम्)
  {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               गणनम्);
    unsigned old_गणनम् = pairSet.len;
    जइ (गणनम् >= old_गणनम्)
      प्रत्यागम Ok();

    pairSet.len = गणनम्;
    c.graph.vertices_[this_index].shrink_buffer ((old_गणनम् - गणनम्) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &एतत्->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          प्रत्यागम p.second < गणनम्;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    प्रत्यागम Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ध्रुव
  {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned अङ्कः_pair_sets = end - start;
    unsigned अविभाज्यः_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + अङ्कः_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_अविभाज्यः_id, c.create_ग्रन्थिः (अविभाज्यः_size));

    PairPosFormat1* pair_pos_अविभाज्यः = (PairPosFormat1*) c.graph.object (pair_pos_अविभाज्यः_id).head;
    pair_pos_अविभाज्यः->format = एतत्->format;
    pair_pos_अविभाज्यः->मानम्_Format_2[0] = एतत्->मानम्_Format_2[0];
    pair_pos_अविभाज्यः->मानम्_Format_2[1] = एतत्->मानम्_Format_2[1];
    pair_pos_अविभाज्यः->pairSet.len = अङ्कः_pair_sets;

    प्रति (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_अविभाज्यः_id,
                                 &pair_pos_अविभाज्यः->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_अविभाज्यः_id,
                                   2,
                                   start, end));

    प्रत्यागम Ok(pair_pos_अविभाज्यः_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) ध्रुव
  {
    प्रत्यागम c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

संरचना PairPosFormat2 : सार्वजनिक OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    जइ (unlikely (vertex_len < min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();

    ध्रुव unsigned class1_गणनम् = class1_गणनम्_2;
    जइ (unlikely (vertex_len < min_size + class1_गणनम् * get_class1_record_size ()))
      प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ध्रुव unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (ध्रुव unsigned class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (ध्रुव Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (ध्रुव ClassDef* class_def_1, get_class_def_1 (c, this_index));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          प्रत्यागम hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    ध्रुव unsigned class1_गणनम् = class1_गणनम्_2;
    ध्रुव unsigned class2_गणनम् = class2_गणनम्_2;
    ध्रुव unsigned class1_record_size = get_class1_record_size ();

    ध्रुव unsigned मानम्_1_len = मानम्_Format1.get_len ();
    ध्रुव unsigned मानम्_2_len = मानम्_Format2.get_len ();
    ध्रुव unsigned total_मानम्_len = मानम्_1_len + मानम्_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = मानम्_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = मानम्_Format2.get_device_table_indices ();
    तार्किक has_device_tables = तार्किक(format1_device_table_indices) || तार्किक(format2_device_table_indices);

    hb_set_t visited;
    प्रति (unsigned i = 0; i < class1_गणनम्; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      जइ (has_device_tables) {
        प्रति (unsigned j = 0; j < class2_गणनम्; j++)
        {
          unsigned value1_index = total_मानम्_len * (class2_गणनम् * i + j);
          unsigned value2_index = value1_index + मानम्_1_len;
          accumulated_delta += size_of_मानम्_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_मानम्_record_children (c,
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
      जइ (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<शून्य>::from (split_points, ALLOCATION_FAILURE));
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
      एतत्,
      this_index,
      class1_record_size,
      total_मानम्_len,
      मानम्_1_len,
      मानम्_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    प्रत्यागम actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 गुप्त:

  संरचना split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned मानम्_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    ध्रुव hb_hashmap_t<unsigned, unsigned>& device_tables;
    ध्रुव hb_vector_t<unsigned>& format1_device_table_indices;
    ध्रुव hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_गणनम्_2 ()
    {
      प्रत्यागम thiz->class1_गणनम्_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      प्रत्यागम thiz->clone_range (*एतत्, start, end);
    }

    graph_result_t<शून्य> shrink (unsigned गणनम्)
    {
      प्रत्यागम thiz->shrink (*एतत्, गणनम्);
    }
  };

  size_t get_class1_record_size () ध्रुव
  {
    ध्रुव size_t class2_गणनम् = class2_गणनम्_2;
    प्रत्यागम
        class2_गणनम् * (मानम्_Format1.get_size () + मानम्_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) ध्रुव
  {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned अङ्कः_records = end - start;
    unsigned अविभाज्यः_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + अङ्कः_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_अविभाज्यः_id, split_context.c.create_ग्रन्थिः (अविभाज्यः_size));

    PairPosFormat2* pair_pos_अविभाज्यः =
        (PairPosFormat2*) graph.object (pair_pos_अविभाज्यः_id).head;
    pair_pos_अविभाज्यः->format = एतत्->format;
    pair_pos_अविभाज्यः->मानम्_Format1 = एतत्->मानम्_Format1;
    pair_pos_अविभाज्यः->मानम्_Format2 = एतत्->मानम्_Format2;
    pair_pos_अविभाज्यः->class1_गणनम्_2 = अङ्कः_records;
    pair_pos_अविभाज्यः->class2_गणनम्_2 = एतत्->class2_गणनम्_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_अविभाज्यः_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    ध्रुव Coverage* coverage_table = (ध्रुव Coverage*) coverage_v.obj().head;
    ध्रुव ClassDef* class_def_1_table = (ध्रुव ClassDef*) class_def_1_v.obj().head;
    जइ (unlikely (!coverage_table || !class_def_1_table))
      प्रत्यागम Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      प्रत्यागम hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      प्रत्यागम klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      प्रत्यागम hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_अविभाज्यः_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_अविभाज्यः_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_अविभाज्यः_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_अविभाज्यः_id, मिच्छ));

    प्रत्यागम Ok(pair_pos_अविभाज्यः_id);
  }

  graph_result_t<शून्य> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_अविभाज्यः_id,
                                             unsigned start, unsigned end) ध्रुव
  {
    PairPosFormat2* pair_pos_अविभाज्यः =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_अविभाज्यः_id).head;

    वर्ण* start_addr = ((वर्ण*)&values[0]) + start * split_context.class1_record_size;
    unsigned अङ्कः_records = end - start;
    hb_memcpy (&pair_pos_अविभाज्यः->values[0],
            start_addr,
            अङ्कः_records * split_context.class1_record_size);

    जइ (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      प्रत्यागम Ok();

    unsigned class2_गणनम् = class2_गणनम्_2;
    प्रति (unsigned i = start; i < end; i++)
    {
      प्रति (unsigned j = 0; j < class2_गणनम्; j++)
      {
        unsigned value1_index = split_context.मानम्_record_len * (class2_गणनम् * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.मानम्_record_len * (class2_गणनम् * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_अविभाज्यः_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_अविभाज्यः_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_अविभाज्यः_id,
                                               ध्रुव hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_मानम्_record_index,
                                               unsigned new_मानम्_record_index) ध्रुव
  {
    PairPosFormat2* pair_pos_अविभाज्यः =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_अविभाज्यः_id).head;

    प्रति (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_मानम्_record_index + i];
      unsigned record_position = ((वर्ण*) record) - ((वर्ण*) एतत्);
      जइ (!split_context.device_tables.has (record_position)) अनुवर्त;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_अविभाज्यः_id,
          (OT::Offset16*) &pair_pos_अविभाज्यः->values[new_मानम्_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> shrink (split_context_t& split_context,
                               unsigned गणनम्)
  {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               गणनम्);
    unsigned old_गणनम् = class1_गणनम्_2;
    जइ (गणनम् >= old_गणनम्)
      प्रत्यागम Ok();

    graph_t& graph = split_context.c.graph;
    class1_गणनम्_2 = गणनम्;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_गणनम् - गणनम्) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &एतत्->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      प्रत्यागम hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      प्रत्यागम klass < गणनम्;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    प्रत्यागम ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) ध्रुव
  {
    ध्रुव auto& v = c.graph.vertices_[this_index];
    प्रत्यागम v.position_to_index_map ();
  }

  graph_result_t<ध्रुव Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) ध्रुव
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    ध्रुव Coverage* coverage_table = (ध्रुव Coverage*) coverage_v.obj().head;
    जइ (unlikely (!coverage_table)) प्रत्यागम Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    प्रत्यागम coverage_table;
  }

  graph_result_t<ध्रुव ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) ध्रुव
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    ध्रुव ClassDef* class_def_1_table = (ध्रुव ClassDef*) class_def_1_v.obj().head;
    जइ (unlikely (!class_def_1_table)) प्रत्यागम Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    प्रत्यागम class_def_1_table;
  }

  unsigned size_of_मानम्_record_children (gsubgpos_graph_context_t& c,
                                          ध्रुव hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          ध्रुव hb_vector_t<unsigned> device_table_indices,
                                          unsigned मानम्_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    प्रति (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::मानम्* record = &values[मानम्_record_index + i];
      unsigned record_position = ((वर्ण*) record) - ((वर्ण*) एतत्);
      unsigned* obj_idx;
      जइ (!device_tables.has (record_position, &obj_idx)) अनुवर्त;
      size += c.graph.find_subgraph_size (*obj_idx, visited).मानम्_or (0);
    }
    प्रत्यागम size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ध्रुव शून्य* offset) ध्रुव
  {
    TRY_ASSIGN (ध्रुव unsigned id, c.graph.index_for_offset (this_index, offset));
    प्रत्यागम Ok(c.graph.vertices_[id].table_size ());
  }
};

संरचना PairPos : सार्वजनिक OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    विकल्प (u.format.v) {
    पक्ष 1:
      hb_barrier ();
      प्रत्यागम ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    पक्ष 2:
      hb_barrier ();
      प्रत्यागम ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: HB_FALLTHROUGH;
    पक्ष 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    स्वभाव:
      प्रत्यागम Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size();
    जइ (unlikely (vertex_len < u.format.v.get_size ())) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();

    विकल्प (u.format.v) {
    पक्ष 1:
      hb_barrier ();
      प्रत्यागम ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    पक्ष 2:
      hb_barrier ();
      प्रत्यागम ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: HB_FALLTHROUGH;
    पक्ष 4: HB_FALLTHROUGH;
#endif
    स्वभाव:
      // We don't handle format 3 and 4 here.
      प्रत्यागम Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
