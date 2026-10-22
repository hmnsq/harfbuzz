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
  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    अचिन्हित min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    जर (unlikely (vertex_len < min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();

    जर (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (स्थिरांक अचिन्हित coverage_id, c.graph.index_for_offset (this_index, &coverage));
    स्थिरांक अचिन्हित coverage_size = c.graph.vertices_[coverage_id].table_size ();
    स्थिरांक अचिन्हित base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    अचिन्हित partial_coverage_size = 4;
    अचिन्हित accumulated = base_size;
    hb_vector_t<अचिन्हित> split_points;
    साठी (अचिन्हित i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (अचिन्हित pair_set_index, pair_set_graph_index (c, this_index, i));
      अचिन्हित accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).मूल्य_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      अचिन्हित total = accumulated + hb_min (partial_coverage_size, coverage_size);

      जर (total >= (1 << 16))
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
      हे,
      this_index,
    };

    परत actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 खाजगी:

  संरचना split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    अचिन्हित this_index;

    अचिन्हित original_मोजणी_2 ()
    {
      परत thiz->pairSet.len;
    }

    graph_result_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      परत thiz->clone_range (हे->c, हे->this_index, start, end);
    }

    graph_result_t<शून्य> shrink (अचिन्हित मोजणी)
    {
      परत thiz->shrink (हे->c, हे->this_index, मोजणी);
    }
  };

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               अचिन्हित this_index,
                               अचिन्हित मोजणी)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               मोजणी);
    अचिन्हित old_मोजणी = pairSet.len;
    जर (मोजणी >= old_मोजणी)
      परत Ok();

    pairSet.len = मोजणी;
    c.graph.vertices_[this_index].shrink_buffer ((old_मोजणी - मोजणी) * SmallTypes::size);

    TRY_ASSIGN (स्वतः coverage, c.graph.as_mutable_table<Coverage> (this_index, &हे->coverage));

    अचिन्हित coverage_size = coverage.vertex->table_size ();
    स्वतः new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<अचिन्हित, अचिन्हित> p) {
          परत p.second < मोजणी;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    परत Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<अचिन्हित> clone_range (gsubgpos_graph_context_t& c,
                                        अचिन्हित this_index,
                                        अचिन्हित start, अचिन्हित end) स्थिरांक
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    अचिन्हित अंक_pair_sets = end - start;
    अचिन्हित मूळसंख्या_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + अंक_pair_sets * SmallTypes::size;

    TRY_ASSIGN (अचिन्हित pair_pos_मूळसंख्या_id, c.create_गाठ (मूळसंख्या_size));

    PairPosFormat1* pair_pos_मूळसंख्या = (PairPosFormat1*) c.graph.object (pair_pos_मूळसंख्या_id).head;
    pair_pos_मूळसंख्या->format = हे->format;
    pair_pos_मूळसंख्या->मूल्य_Format_2[0] = हे->मूल्य_Format_2[0];
    pair_pos_मूळसंख्या->मूल्य_Format_2[1] = हे->मूल्य_Format_2[1];
    pair_pos_मूळसंख्या->pairSet.len = अंक_pair_sets;

    साठी (अचिन्हित i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_मूळसंख्या_id,
                                 &pair_pos_मूळसंख्या->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (अचिन्हित coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_मूळसंख्या_id,
                                   2,
                                   start, end));

    परत Ok(pair_pos_मूळसंख्या_id);
  }



  graph_result_t<अचिन्हित> pair_set_graph_index (gsubgpos_graph_context_t& c, अचिन्हित this_index, अचिन्हित i) स्थिरांक
  {
    परत c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

संरचना PairPosFormat2 : सार्वजनिक OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    अचिन्हित min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    जर (unlikely (vertex_len < min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();

    स्थिरांक अचिन्हित class1_मोजणी = class1_मोजणी_2;
    जर (unlikely (vertex_len < min_size + class1_मोजणी * get_class1_record_size ()))
      परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    स्थिरांक अचिन्हित base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (स्थिरांक अचिन्हित class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (स्थिरांक Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (स्थिरांक ClassDef* class_def_1, get_class_def_1 (c, this_index));
    स्वतः gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          परत hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    स्थिरांक अचिन्हित class1_मोजणी = class1_मोजणी_2;
    स्थिरांक अचिन्हित class2_मोजणी = class2_मोजणी_2;
    स्थिरांक अचिन्हित class1_record_size = get_class1_record_size ();

    स्थिरांक अचिन्हित मूल्य_1_len = मूल्य_Format1.get_len ();
    स्थिरांक अचिन्हित मूल्य_2_len = मूल्य_Format2.get_len ();
    स्थिरांक अचिन्हित total_मूल्य_len = मूल्य_1_len + मूल्य_2_len;

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
    साठी (अचिन्हित i = 0; i < class1_मोजणी; i++)
    {
      अचिन्हित accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      जर (has_device_tables) {
        साठी (अचिन्हित j = 0; j < class2_मोजणी; j++)
        {
          अचिन्हित value1_index = total_मूल्य_len * (class2_मोजणी * i + j);
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
      जर (total >= (1 << 16))
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
      हे,
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

    परत actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 खाजगी:

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

    स्थिरांक hb_hashmap_t<अचिन्हित, अचिन्हित>& device_tables;
    स्थिरांक hb_vector_t<अचिन्हित>& format1_device_table_indices;
    स्थिरांक hb_vector_t<अचिन्हित>& format2_device_table_indices;

    अचिन्हित original_मोजणी_2 ()
    {
      परत thiz->class1_मोजणी_2;
    }

    graph_result_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      परत thiz->clone_range (*हे, start, end);
    }

    graph_result_t<शून्य> shrink (अचिन्हित मोजणी)
    {
      परत thiz->shrink (*हे, मोजणी);
    }
  };

  माप_प्रकार get_class1_record_size () स्थिरांक
  {
    स्थिरांक माप_प्रकार class2_मोजणी = class2_मोजणी_2;
    परत
        class2_मोजणी * (मूल्य_Format1.get_size () + मूल्य_Format2.get_size ());
  }

  graph_result_t<अचिन्हित> clone_range (split_context_t& split_context,
                                        अचिन्हित start, अचिन्हित end) स्थिरांक
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    अचिन्हित अंक_records = end - start;
    अचिन्हित मूळसंख्या_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + अंक_records * split_context.class1_record_size;

    TRY_ASSIGN (अचिन्हित pair_pos_मूळसंख्या_id, split_context.c.create_गाठ (मूळसंख्या_size));

    PairPosFormat2* pair_pos_मूळसंख्या =
        (PairPosFormat2*) graph.object (pair_pos_मूळसंख्या_id).head;
    pair_pos_मूळसंख्या->format = हे->format;
    pair_pos_मूळसंख्या->मूल्य_Format1 = हे->मूल्य_Format1;
    pair_pos_मूळसंख्या->मूल्य_Format2 = हे->मूल्य_Format2;
    pair_pos_मूळसंख्या->class1_मोजणी_2 = अंक_records;
    pair_pos_मूळसंख्या->class2_मोजणी_2 = हे->class2_मोजणी_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_मूळसंख्या_id,
                               start,
                               end));

    TRY_ASSIGN (अचिन्हित coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (अचिन्हित class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    स्वतः& coverage_v = graph.vertices_[coverage_id];
    स्वतः& class_def_1_v = graph.vertices_[class_def_1_id];
    स्थिरांक Coverage* coverage_table = (स्थिरांक Coverage*) coverage_v.obj().head;
    स्थिरांक ClassDef* class_def_1_table = (स्थिरांक ClassDef*) class_def_1_v.obj().head;
    जर (unlikely (!coverage_table || !class_def_1_table))
      परत Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    स्वतः klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      परत hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      परत klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      परत hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_मूळसंख्या_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_मूळसंख्या_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (अचिन्हित class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_मूळसंख्या_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_मूळसंख्या_id, असत्य));

    परत Ok(pair_pos_मूळसंख्या_id);
  }

  graph_result_t<शून्य> clone_class1_records (split_context_t& split_context,
                                             अचिन्हित pair_pos_मूळसंख्या_id,
                                             अचिन्हित start, अचिन्हित end) स्थिरांक
  {
    PairPosFormat2* pair_pos_मूळसंख्या =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_मूळसंख्या_id).head;

    अक्षर* start_addr = ((अक्षर*)&values[0]) + start * split_context.class1_record_size;
    अचिन्हित अंक_records = end - start;
    hb_memcpy (&pair_pos_मूळसंख्या->values[0],
            start_addr,
            अंक_records * split_context.class1_record_size);

    जर (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      परत Ok();

    अचिन्हित class2_मोजणी = class2_मोजणी_2;
    साठी (अचिन्हित i = start; i < end; i++)
    {
      साठी (अचिन्हित j = 0; j < class2_मोजणी; j++)
      {
        अचिन्हित value1_index = split_context.मूल्य_record_len * (class2_मोजणी * i + j);
        अचिन्हित value2_index = value1_index + split_context.value1_record_len;

        अचिन्हित new_value1_index = split_context.मूल्य_record_len * (class2_मोजणी * (i - start) + j);
        अचिन्हित new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_मूळसंख्या_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_मूळसंख्या_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    परत Ok();
  }

  graph_result_t<शून्य> transfer_device_tables (split_context_t& split_context,
                                               अचिन्हित pair_pos_मूळसंख्या_id,
                                               स्थिरांक hb_vector_t<अचिन्हित>& device_table_indices,
                                               अचिन्हित old_मूल्य_record_index,
                                               अचिन्हित new_मूल्य_record_index) स्थिरांक
  {
    PairPosFormat2* pair_pos_मूळसंख्या =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_मूळसंख्या_id).head;

    साठी (अचिन्हित i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_मूल्य_record_index + i];
      अचिन्हित record_position = ((अक्षर*) record) - ((अक्षर*) हे);
      जर (!split_context.device_tables.has (record_position)) सुरूठेवा;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_मूळसंख्या_id,
          (OT::Offset16*) &pair_pos_मूळसंख्या->values[new_मूल्य_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    परत Ok();
  }

  graph_result_t<शून्य> shrink (split_context_t& split_context,
                               अचिन्हित मोजणी)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               मोजणी);
    अचिन्हित old_मोजणी = class1_मोजणी_2;
    जर (मोजणी >= old_मोजणी)
      परत Ok();

    graph_t& graph = split_context.c.graph;
    class1_मोजणी_2 = मोजणी;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_मोजणी - मोजणी) * split_context.class1_record_size);

    TRY_ASSIGN (स्वतः coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &हे->coverage));

    TRY_ASSIGN (स्वतः class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    स्वतः klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      परत hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      परत klass < मोजणी;
    }, hb_second)
    ;

    स्वतः new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    परत ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<अचिन्हित, अचिन्हित>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         अचिन्हित this_index) स्थिरांक
  {
    स्थिरांक स्वतः& v = c.graph.vertices_[this_index];
    परत v.position_to_index_map ();
  }

  graph_result_t<स्थिरांक Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                अचिन्हित this_index) स्थिरांक
  {
    TRY_ASSIGN (अचिन्हित coverage_id, c.graph.index_for_offset (this_index, &coverage));
    स्वतः& coverage_v = c.graph.vertices_[coverage_id];

    स्थिरांक Coverage* coverage_table = (स्थिरांक Coverage*) coverage_v.obj().head;
    जर (unlikely (!coverage_table)) परत Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    परत coverage_table;
  }

  graph_result_t<स्थिरांक ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   अचिन्हित this_index) स्थिरांक
  {
    TRY_ASSIGN (अचिन्हित class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    स्वतः& class_def_1_v = c.graph.vertices_[class_def_1_id];

    स्थिरांक ClassDef* class_def_1_table = (स्थिरांक ClassDef*) class_def_1_v.obj().head;
    जर (unlikely (!class_def_1_table)) परत Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    परत class_def_1_table;
  }

  अचिन्हित size_of_मूल्य_record_children (gsubgpos_graph_context_t& c,
                                          स्थिरांक hb_hashmap_t<अचिन्हित, अचिन्हित>& device_tables,
                                          स्थिरांक hb_vector_t<अचिन्हित> device_table_indices,
                                          अचिन्हित मूल्य_record_index,
                                          hb_set_t& visited)
  {
    अचिन्हित size = 0;
    साठी (अचिन्हित i : device_table_indices)
    {
      OT::Layout::GPOS_impl::मूल्य* record = &values[मूल्य_record_index + i];
      अचिन्हित record_position = ((अक्षर*) record) - ((अक्षर*) हे);
      अचिन्हित* obj_idx;
      जर (!device_tables.has (record_position, &obj_idx)) सुरूठेवा;
      size += c.graph.find_subgraph_size (*obj_idx, visited).मूल्य_or (0);
    }
    परत size;
  }

  graph_result_t<अचिन्हित> size_of (gsubgpos_graph_context_t& c,
                                    अचिन्हित this_index,
                                    स्थिरांक शून्य* offset) स्थिरांक
  {
    TRY_ASSIGN (स्थिरांक अचिन्हित id, c.graph.index_for_offset (this_index, offset));
    परत Ok(c.graph.vertices_[id].table_size ());
  }
};

संरचना PairPos : सार्वजनिक OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    निवड (u.format.v) {
    स्थिती 1:
      hb_barrier ();
      परत ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    स्थिती 2:
      hb_barrier ();
      परत ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: HB_FALLTHROUGH;
    स्थिती 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    पूर्वनिर्धारित:
      परत Ok(hb_vector_t<अचिन्हित> ());
    }
  }

  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size();
    जर (unlikely (vertex_len < u.format.v.get_size ())) परत Err(SANITIZE_FAILURE);
    hb_barrier ();

    निवड (u.format.v) {
    स्थिती 1:
      hb_barrier ();
      परत ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    स्थिती 2:
      hb_barrier ();
      परत ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: HB_FALLTHROUGH;
    स्थिती 4: HB_FALLTHROUGH;
#endif
    पूर्वनिर्धारित:
      // We don't handle format 3 and 4 here.
      परत Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
