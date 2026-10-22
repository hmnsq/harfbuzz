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

#ଅନ୍ତର୍ଭାଵ "split-helpers.hh"
#ଅନ୍ତର୍ଭାଵ "coverage-graph.hh"
#ଅନ୍ତର୍ଭାଵ "classdef-graph.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GPOS/PairPos.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GPOS/PosLookupSubTable.hh"

ନାମାକାଶ graph {

ସଂରଚନା PairPosFormat1 : ସର୍ବସାଧାରଣ OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>
{
  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;
    ଯଦି (unlikely (vertex_len < min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ଯଦି (unlikely (vertex_len < min_size + pairSet.get_size () - pairSet.len.get_size()))
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ଅଚଳ unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    ଅଚଳ unsigned coverage_size = c.graph.vertices_[coverage_id].table_size ();
    ଅଚଳ unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;
    ପାଇଁ (unsigned i = 0; i < pairSet.len; i++)
    {
      TRY_ASSIGN (unsigned pair_set_index, pair_set_graph_index (c, this_index, i));
      unsigned accumulated_delta =
          c.graph.find_subgraph_size (pair_set_index, visited).ମୂଲ୍ୟ_or (0) +
          SmallTypes::size; // for PairSet offset.
      partial_coverage_size += OT::HBUINT16::static_size;

      accumulated += accumulated_delta;
      unsigned total = accumulated + hb_min (partial_coverage_size, coverage_size);

      ଯଦି (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<ଶୂନ୍ୟ>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 6;
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }

    split_context_t split_context {
      c,
      ଏହି,
      this_index,
    };

    ଫେରନ୍ତୁ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ବ୍ୟକ୍ତିଗତ:

  ସଂରଚନା split_context_t {
    gsubgpos_graph_context_t& c;
    PairPosFormat1* thiz;
    unsigned this_index;

    unsigned original_ଗଣନା_2 ()
    {
      ଫେରନ୍ତୁ thiz->pairSet.len;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ଫେରନ୍ତୁ thiz->clone_range (ଏହି->c, ଏହି->this_index, start, end);
    }

    graph_result_t<ଶୂନ୍ୟ> shrink (unsigned ଗଣନା_2)
    {
      ଫେରନ୍ତୁ thiz->shrink (ଏହି->c, ଏହି->this_index, ଗଣନା_2);
    }
  };

  graph_result_t<ଶୂନ୍ୟ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned ଗଣନା_2)
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat1 (%u) to [0, %u).",
               this_index,
               ଗଣନା_2);
    unsigned old_ଗଣନା = pairSet.len;
    ଯଦି (ଗଣନା_2 >= old_ଗଣନା)
      ଫେରନ୍ତୁ Ok();

    pairSet.len = ଗଣନା_2;
    c.graph.vertices_[this_index].shrink_buffer ((old_ଗଣନା - ଗଣନା_2) * SmallTypes::size);

    TRY_ASSIGN (auto coverage, c.graph.as_mutable_table<Coverage> (this_index, &ଏହି->coverage));

    unsigned coverage_size = coverage.vertex->table_size ();
    auto new_coverage =
        + hb_zip (coverage.table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          ଫେରନ୍ତୁ p.second < ଗଣନା_2;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    ଫେରନ୍ତୁ Coverage::make_coverage (c, new_coverage, coverage.index, coverage_size);
  }

  // Create a new PairPos including PairSet's from start (inclusive) to end (exclusive).
  // Returns object id of the new object.
  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ଅଚଳ
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat1 (%u) range [%u, %u).", this_index, start, end);

    unsigned ଅଙ୍କ_pair_sets = end - start;
    unsigned ମୌଳିକ_size = OT::Layout::GPOS_impl::PairPosFormat1_3<SmallTypes>::min_size
                          + ଅଙ୍କ_pair_sets * SmallTypes::size;

    TRY_ASSIGN (unsigned pair_pos_ମୌଳିକ_id, c.create_ଗଣ୍ଠି (ମୌଳିକ_size));

    PairPosFormat1* pair_pos_ମୌଳିକ = (PairPosFormat1*) c.graph.object (pair_pos_ମୌଳିକ_id).head;
    pair_pos_ମୌଳିକ->format = ଏହି->format;
    pair_pos_ମୌଳିକ->ମୂଲ୍ୟ_Format_2[0] = ଏହି->ମୂଲ୍ୟ_Format_2[0];
    pair_pos_ମୌଳିକ->ମୂଲ୍ୟ_Format_2[1] = ଏହି->ମୂଲ୍ୟ_Format_2[1];
    pair_pos_ମୌଳିକ->pairSet.len = ଅଙ୍କ_pair_sets;

    ପାଇଁ (unsigned i = start; i < end; i++)
    {
      TRY (c.graph.move_child<> (this_index,
                                 &pairSet[i],
                                 pair_pos_ମୌଳିକ_id,
                                 &pair_pos_ମୌଳିକ->pairSet[i - start]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY (Coverage::clone_coverage (c,
                                   coverage_id,
                                   pair_pos_ମୌଳିକ_id,
                                   2,
                                   start, end));

    ଫେରନ୍ତୁ Ok(pair_pos_ମୌଳିକ_id);
  }



  graph_result_t<unsigned> pair_set_graph_index (gsubgpos_graph_context_t& c, unsigned this_index, unsigned i) ଅଚଳ
  {
    ଫେରନ୍ତୁ c.graph.index_for_offset (this_index, &pairSet[i]);
  }
};

ସଂରଚନା PairPosFormat2 : ସର୍ବସାଧାରଣ OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>
{
  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    ଯଦି (unlikely (vertex_len < min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ଅଚଳ unsigned class1_ଗଣନା = class1_ଗଣନା_2;
    ଯଦି (unlikely (vertex_len < min_size + class1_ଗଣନା * get_class1_record_size ()))
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ଅଚଳ unsigned base_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size;
    TRY_ASSIGN (ଅଚଳ unsigned class_def_2_size, size_of (c, this_index, &classDef2));
    TRY_ASSIGN (ଅଚଳ Coverage* coverage, get_coverage (c, this_index));
    TRY_ASSIGN (ଅଚଳ ClassDef* class_def_1, get_class_def_1 (c, this_index));
    auto gid_and_class =
        + coverage->iter ()
        | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
          ଫେରନ୍ତୁ hb_codepoint_pair_t (gid, class_def_1->get_class (gid));
        })
        ;
    TRY_ASSIGN (class_def_size_estimator_t estimator, class_def_size_estimator_t::create (gid_and_class));

    ଅଚଳ unsigned class1_ଗଣନା = class1_ଗଣନା_2;
    ଅଚଳ unsigned class2_ଗଣନା = class2_ଗଣନା_2;
    ଅଚଳ unsigned class1_record_size = get_class1_record_size ();

    ଅଚଳ unsigned ମୂଲ୍ୟ_1_len = ମୂଲ୍ୟ_Format1.get_len ();
    ଅଚଳ unsigned ମୂଲ୍ୟ_2_len = ମୂଲ୍ୟ_Format2.get_len ();
    ଅଚଳ unsigned total_ମୂଲ୍ୟ_len = ମୂଲ୍ୟ_1_len + ମୂଲ୍ୟ_2_len;

    unsigned accumulated = base_size;
    unsigned coverage_size = 4;
    unsigned class_def_1_size = 4;
    unsigned max_coverage_size = coverage_size;
    unsigned max_class_def_1_size = class_def_1_size;

    hb_vector_t<unsigned> split_points;

    TRY_ASSIGN (auto device_tables, get_all_device_tables (c, this_index));
    hb_vector_t<unsigned> format1_device_table_indices = ମୂଲ୍ୟ_Format1.get_device_table_indices ();
    hb_vector_t<unsigned> format2_device_table_indices = ମୂଲ୍ୟ_Format2.get_device_table_indices ();
    ତାର୍କିକ has_device_tables = ତାର୍କିକ(format1_device_table_indices) || ତାର୍କିକ(format2_device_table_indices);

    hb_set_t visited;
    ପାଇଁ (unsigned i = 0; i < class1_ଗଣନା; i++)
    {
      unsigned accumulated_delta = class1_record_size;
      class_def_1_size = estimator.add_class_def_size (i);
      coverage_size = estimator.coverage_size ();
      max_coverage_size = hb_max (max_coverage_size, coverage_size);
      max_class_def_1_size = hb_max (max_class_def_1_size, class_def_1_size);

      ଯଦି (has_device_tables) {
        ପାଇଁ (unsigned j = 0; j < class2_ଗଣନା; j++)
        {
          unsigned value1_index = total_ମୂଲ୍ୟ_len * (class2_ଗଣନା * i + j);
          unsigned value2_index = value1_index + ମୂଲ୍ୟ_1_len;
          accumulated_delta += size_of_ମୂଲ୍ୟ_record_children (c,
                                                        device_tables,
                                                        format1_device_table_indices,
                                                        value1_index,
                                                        visited);
          accumulated_delta += size_of_ମୂଲ୍ୟ_record_children (c,
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
      ଯଦି (total >= (1 << 16))
      {
        split_points.push (i);
        TRY (graph_result_t<ଶୂନ୍ୟ>::from (split_points, ALLOCATION_FAILURE));
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
      ଏହି,
      this_index,
      class1_record_size,
      total_ମୂଲ୍ୟ_len,
      ମୂଲ୍ୟ_1_len,
      ମୂଲ୍ୟ_2_len,
      max_coverage_size,
      max_class_def_1_size,
      device_tables,
      format1_device_table_indices,
      format2_device_table_indices
    };

    ଫେରନ୍ତୁ actuate_subtable_split<split_context_t> (split_context, split_points);
  }
 ବ୍ୟକ୍ତିଗତ:

  ସଂରଚନା split_context_t
  {
    gsubgpos_graph_context_t& c;
    PairPosFormat2* thiz;
    unsigned this_index;
    unsigned class1_record_size;
    unsigned ମୂଲ୍ୟ_record_len;
    unsigned value1_record_len;
    unsigned value2_record_len;
    unsigned max_coverage_size;
    unsigned max_class_def_size;

    ଅଚଳ hb_hashmap_t<unsigned, unsigned>& device_tables;
    ଅଚଳ hb_vector_t<unsigned>& format1_device_table_indices;
    ଅଚଳ hb_vector_t<unsigned>& format2_device_table_indices;

    unsigned original_ଗଣନା_2 ()
    {
      ଫେରନ୍ତୁ thiz->class1_ଗଣନା_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ଫେରନ୍ତୁ thiz->clone_range (*ଏହି, start, end);
    }

    graph_result_t<ଶୂନ୍ୟ> shrink (unsigned ଗଣନା_2)
    {
      ଫେରନ୍ତୁ thiz->shrink (*ଏହି, ଗଣନା_2);
    }
  };

  size_t get_class1_record_size () ଅଚଳ
  {
    ଅଚଳ size_t class2_ଗଣନା = class2_ଗଣନା_2;
    ଫେରନ୍ତୁ
        class2_ଗଣନା * (ମୂଲ୍ୟ_Format1.get_size () + ମୂଲ୍ୟ_Format2.get_size ());
  }

  graph_result_t<unsigned> clone_range (split_context_t& split_context,
                                        unsigned start, unsigned end) ଅଚଳ
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "  Cloning PairPosFormat2 (%u) range [%u, %u).", split_context.this_index, start, end);

    graph_t& graph = split_context.c.graph;

    unsigned ଅଙ୍କ_records = end - start;
    unsigned ମୌଳିକ_size = OT::Layout::GPOS_impl::PairPosFormat2_4<SmallTypes>::min_size
                          + ଅଙ୍କ_records * split_context.class1_record_size;

    TRY_ASSIGN (unsigned pair_pos_ମୌଳିକ_id, split_context.c.create_ଗଣ୍ଠି (ମୌଳିକ_size));

    PairPosFormat2* pair_pos_ମୌଳିକ =
        (PairPosFormat2*) graph.object (pair_pos_ମୌଳିକ_id).head;
    pair_pos_ମୌଳିକ->format = ଏହି->format;
    pair_pos_ମୌଳିକ->ମୂଲ୍ୟ_Format1 = ଏହି->ମୂଲ୍ୟ_Format1;
    pair_pos_ମୌଳିକ->ମୂଲ୍ୟ_Format2 = ଏହି->ମୂଲ୍ୟ_Format2;
    pair_pos_ମୌଳିକ->class1_ଗଣନା_2 = ଅଙ୍କ_records;
    pair_pos_ମୌଳିକ->class2_ଗଣନା_2 = ଏହି->class2_ଗଣନା_2;
    TRY (clone_class1_records (split_context,
                               pair_pos_ମୌଳିକ_id,
                               start,
                               end));

    TRY_ASSIGN (unsigned coverage_id,
         graph.index_for_offset (split_context.this_index, &coverage));
    TRY_ASSIGN (unsigned class_def_1_id,
         graph.index_for_offset (split_context.this_index, &classDef1));

    auto& coverage_v = graph.vertices_[coverage_id];
    auto& class_def_1_v = graph.vertices_[class_def_1_id];
    ଅଚଳ Coverage* coverage_table = (ଅଚଳ Coverage*) coverage_v.obj().head;
    ଅଚଳ ClassDef* class_def_1_table = (ଅଚଳ ClassDef*) class_def_1_v.obj().head;
    ଯଦି (unlikely (!coverage_table || !class_def_1_table))
      ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    TRY (coverage_table->sanitize (coverage_v));
    TRY (class_def_1_table->sanitize (class_def_1_v));

    auto klass_map =
    + coverage_table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      ଫେରନ୍ତୁ hb_codepoint_pair_t (gid, class_def_1_table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      ଫେରନ୍ତୁ klass >= start && klass < end;
    }, hb_second)
    | hb_map_retains_sorting ([&] (hb_codepoint_pair_t gid_and_class) {
      // Classes must be from 0...N so subtract start
      ଫେରନ୍ତୁ hb_codepoint_pair_t (gid_and_class.first, gid_and_class.second - start);
    })
    ;

    TRY (Coverage::add_coverage (split_context.c,
                                 pair_pos_ମୌଳିକ_id,
                                 2,
                                 + klass_map | hb_map_retains_sorting (hb_first),
                                 split_context.max_coverage_size));

    // classDef1
    TRY (ClassDef::add_class_def (split_context.c,
                                  pair_pos_ମୌଳିକ_id,
                                  8,
                                  + klass_map,
                                  split_context.max_class_def_size));

    // classDef2
    TRY_ASSIGN (unsigned class_def_2_id,
         graph.index_for_offset (split_context.this_index, &classDef2));

    TRY(graph.vertices_[pair_pos_ମୌଳିକ_id].add_real_link (SmallTypes::size, class_def_2_id, 10));
    TRY(graph.vertices_[class_def_2_id].add_parent (pair_pos_ମୌଳିକ_id, ମିଥ୍ୟା));

    ଫେରନ୍ତୁ Ok(pair_pos_ମୌଳିକ_id);
  }

  graph_result_t<ଶୂନ୍ୟ> clone_class1_records (split_context_t& split_context,
                                             unsigned pair_pos_ମୌଳିକ_id,
                                             unsigned start, unsigned end) ଅଚଳ
  {
    PairPosFormat2* pair_pos_ମୌଳିକ =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_ମୌଳିକ_id).head;

    ଅକ୍ଷର* start_addr = ((ଅକ୍ଷର*)&values[0]) + start * split_context.class1_record_size;
    unsigned ଅଙ୍କ_records = end - start;
    hb_memcpy (&pair_pos_ମୌଳିକ->values[0],
            start_addr,
            ଅଙ୍କ_records * split_context.class1_record_size);

    ଯଦି (!split_context.format1_device_table_indices
        && !split_context.format2_device_table_indices)
      // No device tables to move over.
      ଫେରନ୍ତୁ Ok();

    unsigned class2_ଗଣନା = class2_ଗଣନା_2;
    ପାଇଁ (unsigned i = start; i < end; i++)
    {
      ପାଇଁ (unsigned j = 0; j < class2_ଗଣନା; j++)
      {
        unsigned value1_index = split_context.ମୂଲ୍ୟ_record_len * (class2_ଗଣନା * i + j);
        unsigned value2_index = value1_index + split_context.value1_record_len;

        unsigned new_value1_index = split_context.ମୂଲ୍ୟ_record_len * (class2_ଗଣନା * (i - start) + j);
        unsigned new_value2_index = new_value1_index + split_context.value1_record_len;

        TRY (transfer_device_tables (split_context,
                                     pair_pos_ମୌଳିକ_id,
                                     split_context.format1_device_table_indices,
                                     value1_index,
                                     new_value1_index));

        TRY (transfer_device_tables (split_context,
                                     pair_pos_ମୌଳିକ_id,
                                     split_context.format2_device_table_indices,
                                     value2_index,
                                     new_value2_index));
      }
    }
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<ଶୂନ୍ୟ> transfer_device_tables (split_context_t& split_context,
                                               unsigned pair_pos_ମୌଳିକ_id,
                                               ଅଚଳ hb_vector_t<unsigned>& device_table_indices,
                                               unsigned old_ମୂଲ୍ୟ_record_index,
                                               unsigned new_ମୂଲ୍ୟ_record_index) ଅଚଳ
  {
    PairPosFormat2* pair_pos_ମୌଳିକ =
        (PairPosFormat2*) split_context.c.graph.object (pair_pos_ମୌଳିକ_id).head;

    ପାଇଁ (unsigned i : device_table_indices)
    {
      OT::Offset16* record = (OT::Offset16*) &values[old_ମୂଲ୍ୟ_record_index + i];
      unsigned record_position = ((ଅକ୍ଷର*) record) - ((ଅକ୍ଷର*) ଏହି);
      ଯଦି (!split_context.device_tables.has (record_position)) ଜାରି;

      TRY (split_context.c.graph.move_child (
          split_context.this_index,
          record,
          pair_pos_ମୌଳିକ_id,
          (OT::Offset16*) &pair_pos_ମୌଳିକ->values[new_ମୂଲ୍ୟ_record_index + i]));
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    split_context.c.graph.vertices_[split_context.this_index].sort_real_links ();

    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<ଶୂନ୍ୟ> shrink (split_context_t& split_context,
                               unsigned ଗଣନା_2)
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "  Shrinking PairPosFormat2 (%u) to [0, %u).",
               split_context.this_index,
               ଗଣନା_2);
    unsigned old_ଗଣନା = class1_ଗଣନା_2;
    ଯଦି (ଗଣନା_2 >= old_ଗଣନା)
      ଫେରନ୍ତୁ Ok();

    graph_t& graph = split_context.c.graph;
    class1_ଗଣନା_2 = ଗଣନା_2;
    graph.vertices_[split_context.this_index].shrink_buffer(
        (old_ଗଣନା - ଗଣନା_2) * split_context.class1_record_size);

    TRY_ASSIGN (auto coverage,
         graph.as_mutable_table<Coverage> (split_context.this_index, &ଏହି->coverage));

    TRY_ASSIGN (auto class_def_1,
         graph.as_mutable_table<ClassDef> (split_context.this_index, &classDef1));

    auto klass_map =
    + coverage.table->iter ()
    | hb_map_retains_sorting ([&] (hb_codepoint_t gid) {
      ଫେରନ୍ତୁ hb_codepoint_pair_t (gid, class_def_1.table->get_class (gid));
    })
    | hb_filter ([&] (hb_codepoint_t klass) {
      ଫେରନ୍ତୁ klass < ଗଣନା_2;
    }, hb_second)
    ;

    auto new_coverage = + klass_map | hb_map_retains_sorting (hb_first);
    TRY (Coverage::make_coverage (split_context.c,
                                  + new_coverage,
                                  coverage.index,
                                  // existing ranges my not be kept, worst case size is a format 1
                                  // coverage table.
                                  4 + new_coverage.len() * 2));

    ଫେରନ୍ତୁ ClassDef::make_class_def (split_context.c,
                                     + klass_map,
                                     class_def_1.index,
                                     class_def_1.vertex->table_size ());
  }

  graph_result_t<hb_hashmap_t<unsigned, unsigned>>
  get_all_device_tables (gsubgpos_graph_context_t& c,
                         unsigned this_index) ଅଚଳ
  {
    ଅଚଳ auto& v = c.graph.vertices_[this_index];
    ଫେରନ୍ତୁ v.position_to_index_map ();
  }

  graph_result_t<ଅଚଳ Coverage*> get_coverage (gsubgpos_graph_context_t& c,
                                                unsigned this_index) ଅଚଳ
  {
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    auto& coverage_v = c.graph.vertices_[coverage_id];

    ଅଚଳ Coverage* coverage_table = (ଅଚଳ Coverage*) coverage_v.obj().head;
    ଯଦି (unlikely (!coverage_table)) ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    TRY(coverage_table->sanitize (coverage_v));

    ଫେରନ୍ତୁ coverage_table;
  }

  graph_result_t<ଅଚଳ ClassDef*> get_class_def_1 (gsubgpos_graph_context_t& c,
                                                   unsigned this_index) ଅଚଳ
  {
    TRY_ASSIGN (unsigned class_def_1_id, c.graph.index_for_offset (this_index, &classDef1));
    auto& class_def_1_v = c.graph.vertices_[class_def_1_id];

    ଅଚଳ ClassDef* class_def_1_table = (ଅଚଳ ClassDef*) class_def_1_v.obj().head;
    ଯଦି (unlikely (!class_def_1_table)) ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    TRY(class_def_1_table->sanitize (class_def_1_v));
    ଫେରନ୍ତୁ class_def_1_table;
  }

  unsigned size_of_ମୂଲ୍ୟ_record_children (gsubgpos_graph_context_t& c,
                                          ଅଚଳ hb_hashmap_t<unsigned, unsigned>& device_tables,
                                          ଅଚଳ hb_vector_t<unsigned> device_table_indices,
                                          unsigned ମୂଲ୍ୟ_record_index,
                                          hb_set_t& visited)
  {
    unsigned size = 0;
    ପାଇଁ (unsigned i : device_table_indices)
    {
      OT::Layout::GPOS_impl::ମୂଲ୍ୟ* record = &values[ମୂଲ୍ୟ_record_index + i];
      unsigned record_position = ((ଅକ୍ଷର*) record) - ((ଅକ୍ଷର*) ଏହି);
      unsigned* obj_idx;
      ଯଦି (!device_tables.has (record_position, &obj_idx)) ଜାରି;
      size += c.graph.find_subgraph_size (*obj_idx, visited).ମୂଲ୍ୟ_or (0);
    }
    ଫେରନ୍ତୁ size;
  }

  graph_result_t<unsigned> size_of (gsubgpos_graph_context_t& c,
                                    unsigned this_index,
                                    ଅଚଳ ଶୂନ୍ୟ* offset) ଅଚଳ
  {
    TRY_ASSIGN (ଅଚଳ unsigned id, c.graph.index_for_offset (this_index, offset));
    ଫେରନ୍ତୁ Ok(c.graph.vertices_[id].table_size ());
  }
};

ସଂରଚନା PairPos : ସର୍ବସାଧାରଣ OT::Layout::GPOS_impl::PairPos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1:
      hb_barrier ();
      ଫେରନ୍ତୁ ((PairPosFormat1*)(&u.format1))->split_subtables (c, this_index);
    ସ୍ଥିତି 2:
      hb_barrier ();
      ଫେରନ୍ତୁ ((PairPosFormat2*)(&u.format2))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: HB_FALLTHROUGH;
    ସ୍ଥିତି 4: HB_FALLTHROUGH;
      // 24-bit PairPos formats already have widened offsets; leave them unsplit.
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size();
    ଯଦି (unlikely (vertex_len < u.format.v.get_size ())) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1:
      hb_barrier ();
      ଫେରନ୍ତୁ ((PairPosFormat1*)(&u.format1))->sanitize (vertex);
    ସ୍ଥିତି 2:
      hb_barrier ();
      ଫେରନ୍ତୁ ((PairPosFormat2*)(&u.format2))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 3: HB_FALLTHROUGH;
    ସ୍ଥିତି 4: HB_FALLTHROUGH;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      // We don't handle format 3 and 4 here.
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_PAIRPOS_GRAPH_HH
