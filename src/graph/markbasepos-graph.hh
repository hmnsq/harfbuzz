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

#ifndef GRAPH_MARKBASEPOS_GRAPH_HH
#define GRAPH_MARKBASEPOS_GRAPH_HH

#ಅನ್ತರ್ಭಾವ "split-helpers.hh"
#ಅನ್ತರ್ಭಾವ "coverage-graph.hh"
#ಅನ್ತರ್ಭಾವ "../OT/Layout/GPOS/MarkBasePos.hh"
#ಅನ್ತರ್ಭಾವ "../OT/Layout/GPOS/PosLookupSubTable.hh"

ನಾಮಾಕಾಶ graph {

ಸಂರಚನಾ AnchorMatrix : ಸಾರ್ವಜನಿಕ OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex, unsigned class_ಎಣಿಕೆ) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size();
    ಯದಿ (unlikely (vertex_len < AnchorMatrix::min_size)) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ಯದಿ (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_ಎಣಿಕೆ * ಏತತ್->rows))
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<ಶೂನ್ಯ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_class_ಎಣಿಕೆ,
                               unsigned new_class_ಎಣಿಕೆ)
  {
    ಯದಿ (unlikely (new_class_ಎಣಿಕೆ >= old_class_ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ Err(INVALID_ARGUMENT);
    auto& v = c.graph.vertices_[this_index];
    ಧ್ರುವ auto& o = v.obj ();
    unsigned base_ಎಣಿಕೆ = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_ಎಣಿಕೆ * new_class_ಎಣಿಕೆ);

    // Reposition links into the new indexing scheme.
    ಪ್ರತಿ (auto& link : v.real_links_writer ())
    {
      unsigned index = (link.position - 2) / 2;
      unsigned base = index / old_class_ಎಣಿಕೆ;
      unsigned klass = index % old_class_ಎಣಿಕೆ;
      ಯದಿ (unlikely (klass >= new_class_ಎಣಿಕೆ))
        // should have already been removed
        ಪ್ರತ್ಯಾಗಮ Err(INVALID_ARGUMENT);

      unsigned new_index = base * new_class_ಎಣಿಕೆ + klass;

      link.position = (ವರ್ಣ*) &(ಏತತ್->matrixZ[new_index]) - (ವರ್ಣ*) ಏತತ್;
    }

    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  unsigned start,
                                  unsigned end,
                                  unsigned class_ಎಣಿಕೆ)
  {
    unsigned base_ಎಣಿಕೆ = rows;
    unsigned new_class_ಎಣಿಕೆ = end - start;
    unsigned size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_ಎಣಿಕೆ * rows;
    TRY_ASSIGN (unsigned ಅವಿಭಾಜ್ಯ_id, c.create_ಗಂಟು (size));
    AnchorMatrix* ಅವಿಭಾಜ್ಯ = (AnchorMatrix*) c.graph.object (ಅವಿಭಾಜ್ಯ_id).head;
    ಅವಿಭಾಜ್ಯ->rows = base_ಎಣಿಕೆ;

    auto& v = c.graph.vertices_[this_index];
    ಧ್ರುವ auto& o = v.obj ();

    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_links = o.real_links.length;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_links; i++)
    {
      ಧ್ರುವ auto& link = o.real_links[i];
      unsigned old_index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = old_index % class_ಎಣಿಕೆ;
      ಯದಿ (klass < start || klass >= end) ಅನುವರ್ತ;

      unsigned base = old_index / class_ಎಣಿಕೆ;
      unsigned new_klass = klass - start;
      unsigned new_index = base * new_class_ಎಣಿಕೆ + new_klass;


      unsigned child_idx = link.objidx;
      TRY (c.graph.add_link (&(ಅವಿಭಾಜ್ಯ->matrixZ[new_index]),
                             ಅವಿಭಾಜ್ಯ_id,
                             child_idx));

      auto& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      ಅಂಕೆ_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    ಪ್ರತ್ಯಾಗಮ Ok(ಅವಿಭಾಜ್ಯ_id);
  }
};

ಸಂರಚನಾ MarkArray : ಸಾರ್ವಜನಿಕ OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size();
    unsigned min_size = MarkArray::min_size;
    ಯದಿ (unlikely (vertex_len < min_size)) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ಯದಿ (unlikely (vertex_len < get_size ())) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<ಶೂನ್ಯ> shrink (gsubgpos_graph_context_t& c,
                               ಧ್ರುವ hb_hashmap_t<unsigned, unsigned>& mark_array_links,
                               unsigned this_index,
                               unsigned new_class_ಎಣಿಕೆ)
  {
    auto& v = c.graph.vertices_[this_index];
    ಧ್ರುವ auto& o = v.obj ();
    ಪ್ರತಿ (ಧ್ರುವ auto& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    unsigned new_index = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& record : ಏತತ್->iter ())
    {
      unsigned klass = record.klass;
      ಯದಿ (klass >= new_class_ಎಣಿಕೆ) ಅನುವರ್ತ;

      (*ಏತತ್)[new_index].klass = klass;
      unsigned position = (ವರ್ಣ*) &record.markAnchor - (ವರ್ಣ*) ಏತತ್;
      unsigned* objidx;
      ಯದಿ (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        ಅನುವರ್ತ;
      }

      TRY (c.graph.add_link (&(*ಏತತ್)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    ಏತತ್->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  ಧ್ರುವ hb_hashmap_t<unsigned, unsigned>& pos_to_index,
                                  hb_set_t& marks,
                                  unsigned start_class)
  {
    unsigned size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (unsigned ಅವಿಭಾಜ್ಯ_id, c.create_ಗಂಟು (size));
    MarkArray* ಅವಿಭಾಜ್ಯ = (MarkArray*) c.graph.object (ಅವಿಭಾಜ್ಯ_id).head;
    ಅವಿಭಾಜ್ಯ->len = marks.get_population ();


    unsigned i = 0;
    ಪ್ರತಿ (hb_codepoint_t mark : marks)
    {
      (*ಅವಿಭಾಜ್ಯ)[i].klass = (*ಏತತ್)[mark].klass - start_class;
      unsigned offset_pos = (ವರ್ಣ*) &((*ಏತತ್)[mark].markAnchor) - (ವರ್ಣ*) ಏತತ್;
      unsigned* anchor_index;
      ಯದಿ (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*ಏತತ್)[mark].markAnchor),
                                 ಅವಿಭಾಜ್ಯ_id,
                                 &((*ಅವಿಭಾಜ್ಯ)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    ಪ್ರತ್ಯಾಗಮ Ok(ಅವಿಭಾಜ್ಯ_id);
  }
};

ಸಂರಚನಾ MarkBasePosFormat1 : ಸಾರ್ವಜನಿಕ OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size ();
    ಯದಿ (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ಧ್ರುವ unsigned base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    ಧ್ರುವ unsigned base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    unsigned class_ಎಣಿಕೆ = class_ಎಣಿಕೆ_2;
    TRY_ASSIGN (auto base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_ಎಣಿಕೆ));
    unsigned base_ಎಣಿಕೆ = base_array.table->rows;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;

    ಪ್ರತಿ (unsigned klass = 0; klass < class_ಎಣಿಕೆ; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      unsigned accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_ಎಣಿಕೆ;

      ಪ್ರತಿ (unsigned objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).ಮೌಲ್ಯ_or (0);

      accumulated += accumulated_delta;
      unsigned total = accumulated + partial_coverage_size;

      ಯದಿ (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<ಶೂನ್ಯ>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (ಧ್ರುವ unsigned mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (auto mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      ಏತತ್,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    ಪ್ರತ್ಯಾಗಮ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ಗುಪ್ತ:

  ಸಂರಚನಾ class_info_t {
    hb_set_t marks;
    hb_vector_t<unsigned> child_indices;
  };

  ಸಂರಚನಾ split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    unsigned this_index;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<unsigned, unsigned> mark_array_links;

    hb_set_t marks_for (unsigned start, unsigned end)
    {
      hb_set_t marks;
      ಪ್ರತಿ (unsigned klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      ಪ್ರತ್ಯಾಗಮ marks;
    }

    unsigned original_ಎಣಿಕೆ_2 ()
    {
      ಪ್ರತ್ಯಾಗಮ thiz->class_ಎಣಿಕೆ_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ಪ್ರತ್ಯಾಗಮ thiz->clone_range (*ಏತತ್, ಏತತ್->this_index, start, end);
    }

    graph_result_t<ಶೂನ್ಯ> shrink (unsigned ಎಣಿಕೆ)
    {
      ಪ್ರತ್ಯಾಗಮ thiz->shrink (*ಏತತ್, ಏತತ್->this_index, ಎಣಿಕೆ);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            unsigned this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    unsigned class_ಎಣಿಕೆ = class_ಎಣಿಕೆ_2;
    ಯದಿ (!class_ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ class_to_info;

    ಯದಿ (unlikely (!class_to_info.resize (class_ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (auto mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    unsigned mark_ಎಣಿಕೆ = mark_array.table->len;
    ಪ್ರತಿ (unsigned mark = 0; mark < mark_ಎಣಿಕೆ; mark++)
    {
      unsigned klass = (*mark_array.table)[mark].get_class ();
      ಯದಿ (klass >= class_ಎಣಿಕೆ) ಅನುವರ್ತ;
      class_to_info[klass].marks.add (mark);
    }

    ಪ್ರತಿ (ಧ್ರುವ auto& link : mark_array.vertex->obj ().real_links)
    {
      unsigned mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      unsigned klass = (*mark_array.table)[mark].get_class ();
      ಯದಿ (klass >= class_ಎಣಿಕೆ) ಅನುವರ್ತ;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (unsigned base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    auto& base_array_v = c.graph.vertices_[base_array_id];

    ಪ್ರತಿ (ಧ್ರುವ auto& link : base_array_v.obj ().real_links)
    {
      unsigned index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = index % class_ಎಣಿಕೆ;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    ಪ್ರತ್ಯಾಗಮ class_to_info;
  }

  graph_result_t<ಶೂನ್ಯ> shrink (split_context_t& sc,
                               unsigned this_index,
                               unsigned ಎಣಿಕೆ)
  {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               ಎಣಿಕೆ);

    unsigned old_ಎಣಿಕೆ = class_ಎಣಿಕೆ_2;
    ಯದಿ (ಎಣಿಕೆ >= old_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ Ok();

    class_ಎಣಿಕೆ_2 = ಎಣಿಕೆ;

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, ಎಣಿಕೆ);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.index,
                                  4 + 2 * marks.get_population ()));


    TRY_ASSIGN (auto base_array, sc.c.graph.as_mutable_table<AnchorMatrix> (this_index,
                                                                     &baseArray,
                                                                     old_ಎಣಿಕೆ));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_ಎಣಿಕೆ,
                                   ಎಣಿಕೆ));

    TRY_ASSIGN (auto mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   ಎಣಿಕೆ));

    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<unsigned> clone_range (split_context_t& sc,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ಧ್ರುವ
  {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    unsigned ಅವಿಭಾಜ್ಯ_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (unsigned ಅವಿಭಾಜ್ಯ_id, sc.c.create_ಗಂಟು (ಅವಿಭಾಜ್ಯ_size));

    MarkBasePosFormat1* ಅವಿಭಾಜ್ಯ = (MarkBasePosFormat1*) graph.object (ಅವಿಭಾಜ್ಯ_id).head;
    ಅವಿಭಾಜ್ಯ->format = ಏತತ್->format;
    unsigned new_class_ಎಣಿಕೆ = end - start;
    ಅವಿಭಾಜ್ಯ->class_ಎಣಿಕೆ_2 = new_class_ಎಣಿಕೆ;

    TRY_ASSIGN (unsigned base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(ಅವಿಭಾಜ್ಯ->baseCoverage), ಅವಿಭಾಜ್ಯ_id, base_coverage_id));

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 ಅವಿಭಾಜ್ಯ_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_ASSIGN (auto mark_array,
         graph.as_mutable_table <MarkArray> (sc.this_index, &markArray));

    TRY_ASSIGN (unsigned new_mark_array,
         mark_array.table->clone (sc.c,
                                  mark_array.index,
                                  sc.mark_array_links,
                                  marks,
                                  start));
    TRY (graph.add_link (&(ಅವಿಭಾಜ್ಯ->markArray), ಅವಿಭಾಜ್ಯ_id, new_mark_array));

    unsigned class_ಎಣಿಕೆ = class_ಎಣಿಕೆ_2;
    TRY_ASSIGN (auto base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_ಎಣಿಕೆ));

    TRY_ASSIGN (unsigned new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, ಏತತ್->class_ಎಣಿಕೆ_2));
    TRY (graph.add_link (&(ಅವಿಭಾಜ್ಯ->baseArray), ಅವಿಭಾಜ್ಯ_id, new_base_array));

    ಪ್ರತ್ಯಾಗಮ Ok(ಅವಿಭಾಜ್ಯ_id);
  }
};


ಸಂರಚನಾ MarkBasePos : ಸಾರ್ವಜನಿಕ OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size ();
    ಯದಿ (unlikely (vertex_len < u.format.v.get_size ())) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 2: HB_FALLTHROUGH;
#endif
    ಸ್ವಭಾವ:
      // We don't handle format 3 and 4 here.
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
