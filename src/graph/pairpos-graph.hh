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

#अगर_अघोषित GRAPH_PAIRPOS_GRAPH_HH
#घोषणा GRAPH_PAIRPOS_GRAPH_HH

#समावेश "split-helpers.hh"
#समावेश "coverage-graph.hh"
#समावेश "classdef-graph.hh"
#समावेश "../OT/Layout/GPOS/PairPos.hh"
#समावेश "../OT/Layout/GPOS/PosLookupSubTable.hh"

namespace graph {

काष्ठा PairPosFormat1 : खुला OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार ();
    अचिन्हित min_आकार = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_आकार;
    अगर (unlikely (vertex_len < min_आकार)) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();

    अगर (unlikely (vertex_len < min_आकार + pairSet.get_आकार () - pairSet.len.get_आकार()))
      वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }

  graph_परिणाम_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_सूचकांक)
  {
    hb_set_t visited;

    TRY_ASSIGN (स्थिर अचिन्हित coverage_id, c.graph.सूचकांक_for_offset (this_सूचकांक, &coverage));
    स्थिर अचिन्हित coverage_आकार = c.graph.vertices_[coverage_id].table_आकार ();
    स्थिर अचिन्हित base_आकार = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_आकार;

    अचिन्हित partial_coverage_आकार = 4;
    अचिन्हित accumulated = base_आकार;
    hb_vector_t<अचिन्हित> split_points;
    क्रम (अचिन्हित i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (अचिन्हित pair_set_सूचकांक, pair_set_graph_सूचकांक (c, this_सूचकांक, i));
      अचिन्हित accumulated_delta =
          c.graph.find_subgraph_आकार (pair_set_सूचकांक, visited).मूल्य_or (0) +
          SmallTypes::आकार; // for PairSet offset.
      partial_coverage_आकार += OT::HBUINT16::static_आकार;

      accumulated += accumulated_delta;
      अचिन्हित योग = accumulated + hb_min (partial_coverage_आकार, coverage_आकार);

      अगर (योग >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_परिणाम_t<व्योम>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_आकार + accumulated_delta;
        partial_coverage_आकार = 6;
        visited.सफा (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      यह,
      this_सूचकांक,
    };

    वापस actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 निजी:

  काष्ठा split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    अचिन्हित this_सूचकांक;

    अचिन्हित original_गिनती_2 ()
    {
      वापस thiz->pairSet.len;
    }

    graph_परिणाम_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      वापस thiz->clone_range (यह->c, यह->this_सूचकांक, start, end);
    }

    graph_परिणाम_t<व्योम> shrink (अचिन्हित गिनती)
    {
      वापस thiz->shrink (यह->c, यह->this_सूचकांक, गिनती);
    }
  };

  graph_परिणाम_t<व्योम> shrink (gsubgpos_graph_context_t& c,
                               अचिन्हित this_सूचकांक,
                               अचिन्हित गिनती)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_सूचकांक,
               गिनती);
    अचिन्हित old_गिनती = pairSet.len;
    अगर (गिनती >= old_गिनती)
      वापस Ok();

    pairSet.len = गिनती;
    c.graph.vertices_[this_सूचकांक].shrink_बफर ((old_गिनती - गिनती) * SmallTypes::आकार);

    TRY_ASSIGN (स्वतः coverage, c.graph.as_mutable_table<Coverage> (this_सूचकांक, &यह->coverage));

    अचिन्हित coverage_आकार = coverage.vertex->table_आकार ();
    स्वतः new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<अचिन्हित, अचिन्हित> p) {
          वापस p.second < गिनती;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    वापस Coverage::make_coverage (c, new_coverage, coverage.सूचकांक_2, coverage_आकार);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_परिणाम_t<अचिन्हित> clone_range (gsubgpos_graph_context_t& c,
                                        अचिन्हित this_सूचकांक,
                                        अचिन्हित start, अचिन्हित end) स्थिर
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_सूचकांक, start, end);

    अचिन्हित अंक_pair_sets = end - start;
    अचिन्हित अभाज्य_आकार = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_आकार
                          + अंक_pair_sets * SmallTypes::आकार;

    TRY_ASSIGN (अचिन्हित pair_pos_अभाज्य_id, c.create_गाँठ (अभाज्य_आकार));

    PairPosFormat1* pair_pos_अभाज्य = (PairPosFormat1*) c.graph.object (pair_pos_अभाज्य_id).शीर्ष_2;
    pair_pos_अभाज्य->format = यह->format;
    pair_pos_अभाज्य->मूल्य_Format_2[0] = यह->मूल्य_Format_2[0];
    pair_pos_अभाज्य->मूल्य_Format_2[1] = यह->मूल्य_Format_2[1];
    pair_pos_अभाज्य->pairSet.len = अंक_pair_sets;

    क्रम (अचिन्हित i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_सूचकांक,
                                 &pairSet[i],
                                 pair_pos_अभाज्य_id,
                                 &pair_pos_अभाज्य->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_सूचकांक].sort_real_links ();

    TRY_ASSIGN (अचिन्हित coverage_id, c.graph.सूचकांक_for_offset (this_सूचकांक, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_अभाज्य_id,
                                   2,
                                   start, end));

    वापस Ok(pair_pos_अभाज्य_id);
  }



  graph_परिणाम_t<अचिन्हित> pair_set_graph_सूचकांक (gsubgpos_graph_context_t& c, अचिन्हित this_सूचकांक, अचिन्हित i) स्थिर
  {
    वापस c.graph.सूचकांक_for_offset (this_सूचकांक, &pairSet[i]);
  }
};

काष्ठा PairPosFormat2 : खुला OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार ();
    अचिन्हित min_आकार = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_आकार;
    अगर (unlikely (vertex_len < min_आकार)) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();

    स्थिर अचिन्हित class1_गिनती = class1_गिनती_2;
    अगर (unlikely (vertex_len < min_आकार + class1_गिनती * get_class1_record_आकार ()))
      वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }

  graph_परिणाम_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_सूचकांक)
  {
    स्थिर अचिन्हित base_आकार = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_आकार;
    TRY_ASSIGN (स्थिर अचिन्हित class_def_2_आकार, आकार_of (c, this_सूचकांक, &classDef2));
    TRY_ASSIGN (स्थिर Coverage* coverage, get_coverage (c, this_सूचकांक));
    TRY_ASSIGN (स्थिर ClassDef* class_def_1, get_class_def_1 (c, this_सूचकांक));
    स्वतः gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          वापस hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_आकार_estimator_t estimator, class_def_आकार_estimator_t::create (gid_and_class));

    स्थिर अचिन्हित class1_गिनती = class1_गिनती_2;
    स्थिर अचिन्हित class2_गिनती = class2_गिनती_2;
    स्थिर अचिन्हित class1_record_आकार = get_class1_record_आकार ();

    स्थिर अचिन्हित मूल्य_1_len = मूल्य_Format1.get_len ();
    स्थिर अचिन्हित मूल्य_2_len = मूल्य_Format2.get_len ();
    स्थिर अचिन्हित योग_मूल्य_len = मूल्य_1_len + मूल्य_2_len;

    अचिन्हित accumulated = base_आकार;
    अचिन्हित coverage_आकार = 4;
    अचिन्हित class_def_1_आकार = 4;
    अचिन्हित max_coverage_आकार = coverage_आकार;
    अचिन्हित max_class_def_1_आकार = class_def_1_आकार;

    hb_vector_t<अचिन्हित> split_points;

    TRY_ASSIGN (स्वतः device_tables, get_all_device_tables (c, this_सूचकांक));
    hb_vector_t<अचिन्हित> format1_device_table_indices = मूल्य_Format1.get_device_table_indices ();
    hb_vector_t<अचिन्हित> format2_device_table_indices = मूल्य_Format2.get_device_table_indices ();
    bool has_device_tables = bool(format1_device_table_indices) || bool(format2_device_table_indices);

    hb_set_t visited;
    क्रम (अचिन्हित i = 0; i < class1_गिनती; i++)
    {
      अचिन्हित accumulated_delta = class1_record_आकार;
      class_def_1_आकार = estimator.add_class_def_आकार (i);
      coverage_आकार = estimator.coverage_आकार ();
      max_coverage_आकार = hb_max (max_coverage_आकार, coverage_आकार);
      max_class_def_1_आकार = hb_max (max_class_def_1_आकार, class_def_1_आकार);

      अगर (has_device_tables) {
        क्रम (अचिन्हित j = 0; j < class2_गिनती; j++)
        {
          अचिन्हित value1_सूचकांक = योग_मूल्य_len * (class2_गिनती * i + j);
          अचिन्हित value2_सूचकांक = value1_सूचकांक + मूल्य_1_len;
          accumulated_delta += आकार_of_मूल्य_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_सूचकांक,
                                                        visited);
          accumulated_delta += आकार_of_मूल्य_record_children (c,
                                                        device_tables,
                                                        format2_device_table_indices,
                                                        value2_सूचकांक,
                                                        visited);
        }
      }

      accumulated += accumulated_delta;
      अचिन्हित योग = accumulated
                       + coverage_आकार + class_def_1_आकार + class_def_2_आकार
                       // The largest object will pack last and can exceed the size limit.
                       - hb_max (hb_max (coverage_आकार, class_def_1_आकार), class_def_2_आकार);
      अगर (योग >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_परिणाम_t<व्योम>::from (split_points, ALLOCATION_FAILURE));
        // split does not include i, so add the size for i when we reset the size counters.
        accumulated = base_आकार + accumulated_delta;

        estimator.reset();
        class_def_1_आकार = estimator.add_class_def_आकार(i);
        coverage_आकार = estimator.coverage_आकार();
        visited.सफा (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      यह,
      this_सूचकांक,
      class1_record_आकार,
      योग_मूल्य_len,
      मूल्य_1_len,
      मूल्य_2_len,
      max_coverage_आकार,
      max_class_def_1_आकार,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    वापस actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 निजी:

  काष्ठा split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    अचिन्हित this_सूचकांक;
    अचिन्हित class1_record_आकार;
    अचिन्हित मूल्य_record_len;
    अचिन्हित value1_record_len;
    अचिन्हित value2_record_len;
    अचिन्हित max_coverage_आकार;
    अचिन्हित max_class_def_आकार;

    स्थिर hb_hashmap_t<अचिन्हित, अचिन्हित>& device_tables;
    स्थिर hb_vector_t<अचिन्हित>& format1_device_table_indices;
    स्थिर hb_vector_t<अचिन्हित>& format2_device_table_indices;

    अचिन्हित original_गिनती_2 ()
    {
      वापस thiz->class1_गिनती_2;
    }

    graph_परिणाम_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      वापस thiz->clone_range (*यह, start, end);
    }

    graph_परिणाम_t<व्योम> shrink (अचिन्हित गिनती)
    {
      वापस thiz->shrink (*यह, गिनती);
    }
  };

  माप_प्रकार get_class1_record_आकार () स्थिर
  {
    स्थिर माप_प्रकार class2_गिनती = class2_गिनती_2;
    वापस
        class2_गिनती * (मूल्य_Format1.get_आकार () + मूल्य_Format2.get_आकार ());
  }

  graph_परिणाम_t<अचिन्हित> clone_range (split_context_t& split_context,
                                        अचिन्हित start, अचिन्हित end) स्थिर
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_सूचकांक, start, end);

    graph_t& graph = split_context.c.graph;

    अचिन्हित अंक_records = end - start;
    अचिन्हित अभाज्य_आकार = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_आकार
                          + अंक_records * split_context.class1_record_आकार;

    TRY_ASSIGN (अचिन्हित pair_pos_अभाज्य_id, split_context.c.create_गाँठ (अभाज्य_आकार));

    PairPosFormat2* pair_pos_अभाज्य =
        (PairPosFormat2*) graph.object (pair_pos_अभाज्य_id).शीर्ष_2;
    pair_pos_अभाज्य->format = यह->format;
    pair_pos_अभाज्य->मूल्य_Format1 = यह->मूल्य_Format1;
    pair_pos_अभाज्य->मूल्य_Format2 = यह->मूल्य_Format2;
    pair_pos_अभाज्य->class1_गिनती_2 = अंक_records;
    pair_pos_अभाज्य->class2_गिनती_2 = यह->class2_गिनती_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_अभाज्य_id,
                               start,
                               end));

    TRY_ASSIGN (अचिन्हित coverage_id,
         graph.सूचकांक_for_offset (split_context.this_सूचकांक, &coverage));
    TRY_ASSIGN (अचिन्हित class_def_1_id,
         graph.सूचकांक_for_offset (split_context.this_सूचकांक, &classDef1));

    स्वतः& coverage_v = graph.vertices_[coverage_id];
    स्वतः& class_def_1_v = graph.vertices_[class_def_1_id];
    स्थिर Coverage* coverage_table = (स्थिर Coverage*) coverage_v.obj().शीर्ष_2;
    स्थिर ClassDef* class_def_1_table = (स्थिर ClassDef*) class_def_1_v.obj().शीर्ष_2;
    अगर (unlikely (!coverage_table || !class_def_1_table))
      वापस Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    स्वतः klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      वापस hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      वापस klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      वापस hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_अभाज्य_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_आकार));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_अभाज्य_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_आकार));

    // classDef2
    TRY_ASSIGN (अचिन्हित class_def_2_id,
         graph.सूचकांक_for_offset (split_context.this_सूचकांक, &classDef2));

    TRY(graph.vertices_[pair_pos_अभाज्य_id].add_real_link (SmallTypes::आकार, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_अभाज्य_id, false));

    वापस Ok(pair_pos_अभाज्य_id);
  }

  graph_परिणाम_t<व्योम> clone_class1_records (split_context_t& split_context,
                                             अचिन्हित pair_pos_अभाज्य_id,
                                             अचिन्हित start, अचिन्हित end) स्थिर
  {
    PairPosFormat2* pair_pos_अभाज्य =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_अभाज्य_id).शीर्ष_2;

    अक्षर* start_addr = ((अक्षर*)&values[0]) + start * split_context.class1_record_आकार;
    अचिन्हित अंक_records = end - start;
    hb_memcpy (&pair_pos_अभाज्य->values[0],
            start_addr,
            अंक_records * split_context.class1_record_आकार);

    अगर (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      वापस Ok();

    अचिन्हित class2_गिनती = class2_गिनती_2;
    क्रम (अचिन्हित i = start; i < end; i++)
    {
      क्रम (अचिन्हित j = 0; j < class2_गिनती; j++)
      {
        अचिन्हित value1_सूचकांक = split_context.मूल्य_record_len * (class2_गिनती * i + j);
        अचिन्हित value2_सूचकांक = value1_सूचकांक + split_context.value1_record_len;

        अचिन्हित new_value1_सूचकांक = split_context.मूल्य_record_len * (class2_गिनती * (i - start) + j);
        अचिन्हित new_value2_सूचकांक = new_value1_सूचकांक + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_अभाज्य_id,
                                     split_context.format1_device_table_indices,
                                     value1_सूचकांक,
                                     new_value1_सूचकांक));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_अभाज्य_id,
                                     split_context.format2_device_table_indices,
                                     value2_सूचकांक,
                                     new_value2_सूचकांक));
      }
    }
    वापस Ok();
  }

  graph_परिणाम_t<व्योम> transfer_device_tables (split_context_t& split_context,
                                               अचिन्हित pair_pos_अभाज्य_id,
                                               स्थिर hb_vector_t<अचिन्हित>& device_table_indices,
                                               अचिन्हित old_मूल्य_record_सूचकांक,
                                               अचिन्हित new_मूल्य_record_सूचकांक) स्थिर
  {
    PairPosFormat2* pair_pos_अभाज्य =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_अभाज्य_id).शीर्ष_2;

    क्रम (अचिन्हित i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_मूल्य_record_सूचकांक + i];
      अचिन्हित record_position = ((अक्षर*) record) - ((अक्षर*) यह);
      अगर (!split_context.device_tables.has (record_position)) जारी;

      TRY (split_context.c.graph.move_child (
          split_context.this_सूचकांक,
          record,
          pair_pos_अभाज्य_id,
          (OT::Offset16*) &pair_pos_अभाज्य->values[new_मूल्य_record_सूचकांक + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_सूचकांक].sort_real_links ();

    वापस Ok();
  }

  graph_परिणाम_t<व्योम> shrink (split_context_t& split_context,
                               अचिन्हित गिनती)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_सूचकांक,
               गिनती);
    अचिन्हित old_गिनती = class1_गिनती_2;
    अगर (गिनती >= old_गिनती)
      वापस Ok();

    graph_t& graph = split_context.c.graph;
    class1_गिनती_2 = गिनती;
    graph.vertices_[split_context.this_सूचकांक].shrink_बफर(
        (old_गिनती - गिनती) * split_context.class1_record_आकार);

    TRY_ASSIGN (स्वतः coverage,
         graph.as_mutable_table<Coverage> (split_context.this_सूचकांक, &यह->coverage));

    TRY_ASSIGN (स्वतः class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_सूचकांक, &classDef1));

    स्वतः klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      वापस hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      वापस klass < गिनती;
    }, hb_second)
    ;

    स्वतः new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.सूचकांक_2,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    वापस ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.सूचकांक_2,
                                     class_def_1.vertex->table_आकार ());
  }

  graph_परिणाम_t<hb_hashmap_t<अचिन्हित, अचिन्हित>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         अचिन्हित this_सूचकांक) स्थिर
  {
    स्थिर स्वतः& v = c.graph.vertices_[this_सूचकांक];
    वापस v.position_to_सूचकांक_map ();
  }

  graph_परिणाम_t<स्थिर Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                अचिन्हित this_सूचकांक) स्थिर
  {
    TRY_ASSIGN (अचिन्हित coverage_id, c.graph.सूचकांक_for_offset (this_सूचकांक, &coverage));
    स्वतः& coverage_v = c.graph.vertices_[coverage_id];

    स्थिर Coverage* coverage_table = (स्थिर Coverage*) coverage_v.obj().शीर्ष_2;
    अगर (unlikely (!coverage_table)) वापस Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    वापस coverage_table;
  }

  graph_परिणाम_t<स्थिर ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   अचिन्हित this_सूचकांक) स्थिर
  {
    TRY_ASSIGN (अचिन्हित class_def_1_id, c.graph.सूचकांक_for_offset (this_सूचकांक, &classDef1));
    स्वतः& class_def_1_v = c.graph.vertices_[class_def_1_id];

    स्थिर ClassDef* class_def_1_table = (स्थिर ClassDef*) class_def_1_v.obj().शीर्ष_2;
    अगर (unlikely (!class_def_1_table)) वापस Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    वापस class_def_1_table;
  }

  अचिन्हित आकार_of_मूल्य_record_children (gsubgpos_graph_context_t& c,
                                          स्थिर hb_hashmap_t<अचिन्हित, अचिन्हित>& device_tables,
                                          स्थिर hb_vector_t<अचिन्हित> device_table_indices,
                                          अचिन्हित मूल्य_record_सूचकांक,
                                          hb_set_t& visited)
  {
    अचिन्हित आकार = 0;
    क्रम (अचिन्हित i : device_table_indices)
    {
      OT::Layout::GPOS_impl::मूल्य* record = &values[मूल्य_record_सूचकांक + i];
      अचिन्हित record_position = ((अक्षर*) record) - ((अक्षर*) यह);
      अचिन्हित* obj_idx;
      अगर (!device_tables.has (record_position, &obj_idx)) जारी;
      आकार += c.graph.find_subgraph_आकार (*obj_idx, visited).मूल्य_or (0);
    }
    वापस आकार;
  }

  graph_परिणाम_t<अचिन्हित> आकार_of (gsubgpos_graph_context_t& c,
                                    अचिन्हित this_सूचकांक,
                                    स्थिर व्योम* offset) स्थिर
  {
    TRY_ASSIGN (स्थिर अचिन्हित id, c.graph.सूचकांक_for_offset (this_सूचकांक, offset));
    वापस Ok(c.graph.vertices_[id].table_आकार ());
  }
};

काष्ठा PairPos : खुला OT::Layout::GPOS_impl::PairPos
{
  graph_परिणाम_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_सूचकांक)
  {
    चयन (u.format.v) {
    हाल 1:
      hb_barrier ();
      वापस ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_सूचकांक);
    हाल 2:
      hb_barrier ();
      वापस ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_सूचकांक);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: HB_FALLTHROUGH;
    हाल 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#पूर्ण_अगर
    शेष:
      वापस Ok(hb_vector_t<अचिन्हित> ());
    }
  }

  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार();
    अगर (unlikely (vertex_len < u.format.v.get_आकार ())) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();

    चयन (u.format.v) {
    हाल 1:
      hb_barrier ();
      वापस ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    हाल 2:
      hb_barrier ();
      वापस ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: HB_FALLTHROUGH;
    हाल 4: HB_FALLTHROUGH;
#पूर्ण_अगर
    शेष:
      // We don't handle format 3 and 4 here.
      वापस Err(SANITIZE_FAILURE);
    }
  }
};

}

#पूर्ण_अगर  // GRAPH_PAIRPOS_GRAPH_HH
