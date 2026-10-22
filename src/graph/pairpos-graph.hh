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
    माप_प्रकार vertex_len = vertex.table_size ();
    अचिन्हित min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    यदि (unlikely (vertex_len < min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();

    यदि (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ध्रुव अचिन्हित coverage_id, c.graph.index_for_offset (this_index, &coverage));
    ध्रुव अचिन्हित coverage_size = c.graph.vertices_[coverage_id].table_size ();
    ध्रुव अचिन्हित base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    अचिन्हित partial_coverage_size = 4;
    अचिन्हित accumulated = base_size;
    hb_vector_t<अचिन्हित> split_points;
    प्रति (अचिन्हित i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (अचिन्हित pair_set_index, pair_set_graph_index (c, this_index, i));
      अचिन्हित accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).मूल्य_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      अचिन्हित total = accumulated + hb_min (partial_coverage_size, coverage_size);

      यदि (total >= (1 << 16))
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
    अचिन्हित this_index;

    अचिन्हित original_गिनती_2 ()
    {
      प्रत्यागम thiz->pairSet.len;
    }

    graph_result_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      प्रत्यागम thiz->clone_range (एतत्->c, एतत्->this_index, start, end);
    }

    graph_result_t<शून्य> shrink (अचिन्हित गिनती)
    {
      प्रत्यागम thiz->shrink (एतत्->c, एतत्->this_index, गिनती);
    }
  };

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               अचिन्हित this_index,
                               अचिन्हित गिनती)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               गिनती);
    अचिन्हित old_गिनती = pairSet.len;
    यदि (गिनती >= old_गिनती)
      प्रत्यागम Ok();

    pairSet.len = गिनती;
    c.graph.vertices_[this_index].shrink_buffer ((old_गिनती - गिनती) * SmallTypes::size);

    TRY_ASSIGN (स्वतः coverage, c.graph.as_mutable_table<Coverage> (this_index, &एतत्->coverage));

    अचिन्हित coverage_size = coverage.vertex->table_size ();
    स्वतः new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<अचिन्हित, अचिन्हित> p) {
          प्रत्यागम p.second < गिनती;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    प्रत्यागम Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<अचिन्हित> clone_range (gsubgpos_graph_context_t& c,
                                        अचिन्हित this_index,
                                        अचिन्हित start, अचिन्हित end) ध्रुव
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    अचिन्हित अंक_pair_sets = end - start;
    अचिन्हित अभाज्य_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + अंक_pair_sets * SmallTypes::size;

    TRY_ASSIGN (अचिन्हित pair_pos_अभाज्य_id, c.create_गाँठ (अभाज्य_size));

    PairPosFormat1* pair_pos_अभाज्य = (PairPosFormat1*) c.graph.object (pair_pos_अभाज्य_id).head;
    pair_pos_अभाज्य->format = एतत्->format;
    pair_pos_अभाज्य->मूल्य_Format_2[0] = एतत्->मूल्य_Format_2[0];
    pair_pos_अभाज्य->मूल्य_Format_2[1] = एतत्->मूल्य_Format_2[1];
    pair_pos_अभाज्य->pairSet.len = अंक_pair_sets;

    प्रति (अचिन्हित i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_अभाज्य_id,
                                 &pair_pos_अभाज्य->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (अचिन्हित coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_अभाज्य_id,
                                   2,
                                   start, end));

    प्रत्यागम Ok(pair_pos_अभाज्य_id);
  }



  graph_result_t<अचिन्हित> pair_set_graph_index (gsubgpos_graph_context_t& c, अचिन्हित this_index, अचिन्हित i) ध्रुव
  {
    प्रत्यागम c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

संरचना PairPosFormat2 : सार्वजनिक OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    अचिन्हित min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    यदि (unlikely (vertex_len < min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();

    ध्रुव अचिन्हित class1_गिनती = class1_गिनती_2;
    यदि (unlikely (vertex_len < min_size + class1_गिनती * get_class1_record_size ()))
      प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    ध्रुव अचिन्हित base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (ध्रुव अचिन्हित class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (ध्रुव Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (ध्रुव ClassDef* class_def_1, get_class_def_1 (c, this_index));
    स्वतः gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          प्रत्यागम hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    ध्रुव अचिन्हित class1_गिनती = class1_गिनती_2;
    ध्रुव अचिन्हित class2_गिनती = class2_गिनती_2;
    ध्रुव अचिन्हित class1_record_size = get_class1_record_size ();

    ध्रुव अचिन्हित मूल्य_1_len = मूल्य_Format1.get_len ();
    ध्रुव अचिन्हित मूल्य_2_len = मूल्य_Format2.get_len ();
    ध्रुव अचिन्हित total_मूल्य_len = मूल्य_1_len + मूल्य_2_len;

    अचिन्हित accumulated = base_size;
    अचिन्हित coverage_size = 4;
    अचिन्हित class_def_1_size = 4;
    अचिन्हित max_coverage_size = coverage_size;
    अचिन्हित max_class_def_1_size = class_def_1_size;

    hb_vector_t<अचिन्हित> split_points;

    TRY_ASSIGN (स्वतः device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<अचिन्हित> format1_device_table_indices = मूल्य_Format1.get_device_table_indices ();
    hb_vector_t<अचिन्हित> format2_device_table_indices = मूल्य_Format2.get_device_table_indices ();
    तार्किक has_device_tables = तार्किक(format1_device_table_indices) || तार्किक(format2_device_table_indices);

    hb_set_t visited;
    प्रति (अचिन्हित i = 0; i < class1_गिनती; i++)
    {
      अचिन्हित accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      यदि (has_device_tables) {
        प्रति (अचिन्हित j = 0; j < class2_गिनती; j++)
        {
          अचिन्हित value1_index = total_मूल्य_len * (class2_गिनती * i + j);
          अचिन्हित value2_index = value1_index + मूल्य_1_len;
          accumulated_delta += size_of_मूल्य_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_मूल्य_record_children (c,
                                                        device_tables,
                                                        format2_device_table_indices,
                                                        value2_index,
                                                        visited);
        }
      }

      accumulated += accumulated_delta;
      अचिन्हित total = accumulated
                       + coverage_size + class_def_1_size + class_def_2_size
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_size, class_def_1_size), class_def_2_size);
      यदि (total >= (1 << 16))
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
      total_मूल्य_len,
      मूल्य_1_len,
      मूल्य_2_len,
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
    अचिन्हित this_index;
    अचिन्हित class1_record_size;
    अचिन्हित मूल्य_record_len;
    अचिन्हित value1_record_len;
    अचिन्हित value2_record_len;
    अचिन्हित max_coverage_size;
    अचिन्हित max_class_def_size;

    ध्रुव hb_hashmap_t<अचिन्हित, अचिन्हित>& device_tables;
    ध्रुव hb_vector_t<अचिन्हित>& format1_device_table_indices;
    ध्रुव hb_vector_t<अचिन्हित>& format2_device_table_indices;

    अचिन्हित original_गिनती_2 ()
    {
      प्रत्यागम thiz->class1_गिनती_2;
    }

    graph_result_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      प्रत्यागम thiz->clone_range (*एतत्, start, end);
    }

    graph_result_t<शून्य> shrink (अचिन्हित गिनती)
    {
      प्रत्यागम thiz->shrink (*एतत्, गिनती);
    }
  };

  माप_प्रकार get_class1_record_size () ध्रुव
  {
    ध्रुव माप_प्रकार class2_गिनती = class2_गिनती_2;
    प्रत्यागम
        class2_गिनती * (मूल्य_Format1.get_size () + मूल्य_Format2.get_size ());
  }

  graph_result_t<अचिन्हित> clone_range (split_context_t& split_context,
                                        अचिन्हित start, अचिन्हित end) ध्रुव
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    अचिन्हित अंक_records = end - start;
    अचिन्हित अभाज्य_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + अंक_records * split_context.class1_record_size;

    TRY_ASSIGN (अचिन्हित pair_pos_अभाज्य_id, split_context.c.create_गाँठ (अभाज्य_size));

    PairPosFormat2* pair_pos_अभाज्य =
        (PairPosFormat2*) graph.object (pair_pos_अभाज्य_id).head;
    pair_pos_अभाज्य->format = एतत्->format;
    pair_pos_अभाज्य->मूल्य_Format1 = एतत्->मूल्य_Format1;
    pair_pos_अभाज्य->मूल्य_Format2 = एतत्->मूल्य_Format2;
    pair_pos_अभाज्य->class1_गिनती_2 = अंक_records;
    pair_pos_अभाज्य->class2_गिनती_2 = एतत्->class2_गिनती_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_अभाज्य_id,
                               start,
                               end));

    TRY_ASSIGN (अचिन्हित coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (अचिन्हित class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    स्वतः& coverage_v = graph.vertices_[coverage_id];
    स्वतः& class_def_1_v = graph.vertices_[class_def_1_id];
    ध्रुव Coverage* coverage_table = (ध्रुव Coverage*) coverage_v.obj().head;
    ध्रुव ClassDef* class_def_1_table = (ध्रुव ClassDef*) class_def_1_v.obj().head;
    यदि (unlikely (!coverage_table || !class_def_1_table))
      प्रत्यागम Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    स्वतः klass_map =
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
                                 pair_pos_अभाज्य_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_अभाज्य_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (अचिन्हित class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_अभाज्य_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_अभाज्य_id, असत्य));

    प्रत्यागम Ok(pair_pos_अभाज्य_id);
  }

  graph_result_t<शून्य> clone_class1_records (split_context_t& split_context,
                                             अचिन्हित pair_pos_अभाज्य_id,
                                             अचिन्हित start, अचिन्हित end) ध्रुव
  {
    PairPosFormat2* pair_pos_अभाज्य =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_अभाज्य_id).head;

    वर्ण* start_addr = ((वर्ण*)&values[0]) + start * split_context.class1_record_size;
    अचिन्हित अंक_records = end - start;
    hb_memcpy (&pair_pos_अभाज्य->values[0],
            start_addr,
            अंक_records * split_context.class1_record_size);

    यदि (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      प्रत्यागम Ok();

    अचिन्हित class2_गिनती = class2_गिनती_2;
    प्रति (अचिन्हित i = start; i < end; i++)
    {
      प्रति (अचिन्हित j = 0; j < class2_गिनती; j++)
      {
        अचिन्हित value1_index = split_context.मूल्य_record_len * (class2_गिनती * i + j);
        अचिन्हित value2_index = value1_index + split_context.value1_record_len;

        अचिन्हित new_value1_index = split_context.मूल्य_record_len * (class2_गिनती * (i - start) + j);
        अचिन्हित new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_अभाज्य_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_अभाज्य_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> transfer_device_tables (split_context_t& split_context,
                                               अचिन्हित pair_pos_अभाज्य_id,
                                               ध्रुव hb_vector_t<अचिन्हित>& device_table_indices,
                                               अचिन्हित old_मूल्य_record_index,
                                               अचिन्हित new_मूल्य_record_index) ध्रुव
  {
    PairPosFormat2* pair_pos_अभाज्य =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_अभाज्य_id).head;

    प्रति (अचिन्हित i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_मूल्य_record_index + i];
      अचिन्हित record_position = ((वर्ण*) record) - ((वर्ण*) एतत्);
      यदि (!split_context.device_tables.has (record_position)) अनुवर्त;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_अभाज्य_id,
          (OT::Offset16*) &pair_pos_अभाज्य->values[new_मूल्य_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> shrink (split_context_t& split_context,
                               अचिन्हित गिनती)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               गिनती);
    अचिन्हित old_गिनती = class1_गिनती_2;
    यदि (गिनती >= old_गिनती)
      प्रत्यागम Ok();

    graph_t& graph = split_context.c.graph;
    class1_गिनती_2 = गिनती;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_गिनती - गिनती) * split_context.class1_record_size);

    TRY_ASSIGN (स्वतः coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &एतत्->coverage));

    TRY_ASSIGN (स्वतः class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    स्वतः klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      प्रत्यागम hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      प्रत्यागम klass < गिनती;
    }, hb_second)
    ;

    स्वतः new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
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

  graph_result_t<hb_hashmap_t<अचिन्हित, अचिन्हित>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         अचिन्हित this_index) ध्रुव
  {
    ध्रुव स्वतः& v = c.graph.vertices_[this_index];
    प्रत्यागम v.position_to_index_map ();
  }

  graph_result_t<ध्रुव Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                अचिन्हित this_index) ध्रुव
  {
    TRY_ASSIGN (अचिन्हित coverage_id, c.graph.index_for_offset (this_index, &coverage));
    स्वतः& coverage_v = c.graph.vertices_[coverage_id];

    ध्रुव Coverage* coverage_table = (ध्रुव Coverage*) coverage_v.obj().head;
    यदि (unlikely (!coverage_table)) प्रत्यागम Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    प्रत्यागम coverage_table;
  }

  graph_result_t<ध्रुव ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   अचिन्हित this_index) ध्रुव
  {
    TRY_ASSIGN (अचिन्हित class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    स्वतः& class_def_1_v = c.graph.vertices_[class_def_1_id];

    ध्रुव ClassDef* class_def_1_table = (ध्रुव ClassDef*) class_def_1_v.obj().head;
    यदि (unlikely (!class_def_1_table)) प्रत्यागम Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    प्रत्यागम class_def_1_table;
  }

  अचिन्हित size_of_मूल्य_record_children (gsubgpos_graph_context_t& c,
                                          ध्रुव hb_hashmap_t<अचिन्हित, अचिन्हित>& device_tables,
                                          ध्रुव hb_vector_t<अचिन्हित> device_table_indices,
                                          अचिन्हित मूल्य_record_index,
                                          hb_set_t& visited)
  {
    अचिन्हित size = 0;
    प्रति (अचिन्हित i : device_table_indices)
    {
      OT::Layout::GPOS_impl::मूल्य* record = &values[मूल्य_record_index + i];
      अचिन्हित record_position = ((वर्ण*) record) - ((वर्ण*) एतत्);
      अचिन्हित* obj_idx;
      यदि (!device_tables.has (record_position, &obj_idx)) अनुवर्त;
      size += c.graph.find_subgraph_size (*obj_idx, visited).मूल्य_or (0);
    }
    प्रत्यागम size;
  }

  graph_result_t<अचिन्हित> size_of (gsubgpos_graph_context_t& c,
                                    अचिन्हित this_index,
                                    ध्रुव शून्य* offset) ध्रुव
  {
    TRY_ASSIGN (ध्रुव अचिन्हित id, c.graph.index_for_offset (this_index, offset));
    प्रत्यागम Ok(c.graph.vertices_[id].table_size ());
  }
};

संरचना PairPos : सार्वजनिक OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
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
      प्रत्यागम Ok(hb_vector_t<अचिन्हित> ());
    }
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    माप_प्रकार vertex_len = vertex.table_size();
    यदि (unlikely (vertex_len < u.format.v.get_size ())) प्रत्यागम Err(SANITIZE_FAILURE);
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
