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

#ଅନ୍ତର୍ଭାଵ "split-helpers.hh"
#ଅନ୍ତର୍ଭାଵ "coverage-graph.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GPOS/MarkBasePos.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GPOS/PosLookupSubTable.hh"

ନାମାକାଶ graph {

ସଂରଚନା AnchorMatrix : ସର୍ବସାଧାରଣ OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex, unsigned class_ଗଣନା) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size();
    ଯଦି (unlikely (vertex_len < AnchorMatrix::min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ଯଦି (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_ଗଣନା * ଏହି->rows))
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<ଶୂନ୍ୟ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_class_ଗଣନା,
                               unsigned new_class_ଗଣନା)
  {
    ଯଦି (unlikely (new_class_ଗଣନା >= old_class_ଗଣନା)) ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);
    auto& v = c.graph.vertices_[this_index];
    ଅଚଳ auto& o = v.obj ();
    unsigned base_ଗଣନା = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_ଗଣନା * new_class_ଗଣନା);

    // Reposition links into the new indexing scheme.
    ପାଇଁ (auto& link : v.real_links_writer ())
    {
      unsigned index = (link.position - 2) / 2;
      unsigned base = index / old_class_ଗଣନା;
      unsigned klass = index % old_class_ଗଣନା;
      ଯଦି (unlikely (klass >= new_class_ଗଣନା))
        // should have already been removed
        ଫେରନ୍ତୁ Err(INVALID_ARGUMENT);

      unsigned new_index = base * new_class_ଗଣନା + klass;

      link.position = (ଅକ୍ଷର*) &(ଏହି->matrixZ[new_index]) - (ଅକ୍ଷର*) ଏହି;
    }

    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  unsigned start,
                                  unsigned end,
                                  unsigned class_ଗଣନା)
  {
    unsigned base_ଗଣନା = rows;
    unsigned new_class_ଗଣନା = end - start;
    unsigned size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_ଗଣନା * rows;
    TRY_ASSIGN (unsigned ମୌଳିକ_id, c.create_ଗଣ୍ଠି (size));
    AnchorMatrix* ମୌଳିକ = (AnchorMatrix*) c.graph.object (ମୌଳିକ_id).head;
    ମୌଳିକ->rows = base_ଗଣନା;

    auto& v = c.graph.vertices_[this_index];
    ଅଚଳ auto& o = v.obj ();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_links = o.real_links.length;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_links; i++)
    {
      ଅଚଳ auto& link = o.real_links[i];
      unsigned old_index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = old_index % class_ଗଣନା;
      ଯଦି (klass < start || klass >= end) ଜାରି;

      unsigned base = old_index / class_ଗଣନା;
      unsigned new_klass = klass - start;
      unsigned new_index = base * new_class_ଗଣନା + new_klass;


      unsigned child_idx = link.objidx;
      TRY (c.graph.add_link (&(ମୌଳିକ->matrixZ[new_index]),
                             ମୌଳିକ_id,
                             child_idx));

      auto& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      ଅଙ୍କ_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    ଫେରନ୍ତୁ Ok(ମୌଳିକ_id);
  }
};

ସଂରଚନା MarkArray : ସର୍ବସାଧାରଣ OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size();
    unsigned min_size = MarkArray::min_size;
    ଯଦି (unlikely (vertex_len < min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ଯଦି (unlikely (vertex_len < get_size ())) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<ଶୂନ୍ୟ> shrink (gsubgpos_graph_context_t& c,
                               ଅଚଳ hb_hashmap_t<unsigned, unsigned>& mark_array_links,
                               unsigned this_index,
                               unsigned new_class_ଗଣନା)
  {
    auto& v = c.graph.vertices_[this_index];
    ଅଚଳ auto& o = v.obj ();
    ପାଇଁ (ଅଚଳ auto& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    unsigned new_index = 0;
    ପାଇଁ (ଅଚଳ auto& record : ଏହି->iter ())
    {
      unsigned klass = record.klass;
      ଯଦି (klass >= new_class_ଗଣନା) ଜାରି;

      (*ଏହି)[new_index].klass = klass;
      unsigned position = (ଅକ୍ଷର*) &record.markAnchor - (ଅକ୍ଷର*) ଏହି;
      unsigned* objidx;
      ଯଦି (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        ଜାରି;
      }

      TRY (c.graph.add_link (&(*ଏହି)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    ଏହି->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  ଅଚଳ hb_hashmap_t<unsigned, unsigned>& pos_to_index,
                                  hb_set_t& marks,
                                  unsigned start_class)
  {
    unsigned size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (unsigned ମୌଳିକ_id, c.create_ଗଣ୍ଠି (size));
    MarkArray* ମୌଳିକ = (MarkArray*) c.graph.object (ମୌଳିକ_id).head;
    ମୌଳିକ->len = marks.get_population ();


    unsigned i = 0;
    ପାଇଁ (hb_codepoint_t mark : marks)
    {
      (*ମୌଳିକ)[i].klass = (*ଏହି)[mark].klass - start_class;
      unsigned offset_pos = (ଅକ୍ଷର*) &((*ଏହି)[mark].markAnchor) - (ଅକ୍ଷର*) ଏହି;
      unsigned* anchor_index;
      ଯଦି (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*ଏହି)[mark].markAnchor),
                                 ମୌଳିକ_id,
                                 &((*ମୌଳିକ)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    ଫେରନ୍ତୁ Ok(ମୌଳିକ_id);
  }
};

ସଂରଚନା MarkBasePosFormat1 : ସର୍ବସାଧାରଣ OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    ଯଦି (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ଅଚଳ unsigned base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    ଅଚଳ unsigned base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    unsigned class_ଗଣନା = class_ଗଣନା_2;
    TRY_ASSIGN (auto base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_ଗଣନା));
    unsigned base_ଗଣନା = base_array.table->rows;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;

    ପାଇଁ (unsigned klass = 0; klass < class_ଗଣନା; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      unsigned accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_ଗଣନା;

      ପାଇଁ (unsigned objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).ମୂଲ୍ୟ_or (0);

      accumulated += accumulated_delta;
      unsigned total = accumulated + partial_coverage_size;

      ଯଦି (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<ଶୂନ୍ୟ>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (ଅଚଳ unsigned mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (auto mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      ଏହି,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    ଫେରନ୍ତୁ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ବ୍ୟକ୍ତିଗତ:

  ସଂରଚନା class_info_t {
    hb_set_t marks;
    hb_vector_t<unsigned> child_indices;
  };

  ସଂରଚନା split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    unsigned this_index;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<unsigned, unsigned> mark_array_links;

    hb_set_t marks_for (unsigned start, unsigned end)
    {
      hb_set_t marks;
      ପାଇଁ (unsigned klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      ଫେରନ୍ତୁ marks;
    }

    unsigned original_ଗଣନା_2 ()
    {
      ଫେରନ୍ତୁ thiz->class_ଗଣନା_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ଫେରନ୍ତୁ thiz->clone_range (*ଏହି, ଏହି->this_index, start, end);
    }

    graph_result_t<ଶୂନ୍ୟ> shrink (unsigned ଗଣନା_2)
    {
      ଫେରନ୍ତୁ thiz->shrink (*ଏହି, ଏହି->this_index, ଗଣନା_2);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            unsigned this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    unsigned class_ଗଣନା = class_ଗଣନା_2;
    ଯଦି (!class_ଗଣନା) ଫେରନ୍ତୁ class_to_info;

    ଯଦି (unlikely (!class_to_info.resize (class_ଗଣନା)))
      ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (auto mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    unsigned mark_ଗଣନା = mark_array.table->len;
    ପାଇଁ (unsigned mark = 0; mark < mark_ଗଣନା; mark++)
    {
      unsigned klass = (*mark_array.table)[mark].get_class ();
      ଯଦି (klass >= class_ଗଣନା) ଜାରି;
      class_to_info[klass].marks.add (mark);
    }

    ପାଇଁ (ଅଚଳ auto& link : mark_array.vertex->obj ().real_links)
    {
      unsigned mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      unsigned klass = (*mark_array.table)[mark].get_class ();
      ଯଦି (klass >= class_ଗଣନା) ଜାରି;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (unsigned base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    auto& base_array_v = c.graph.vertices_[base_array_id];

    ପାଇଁ (ଅଚଳ auto& link : base_array_v.obj ().real_links)
    {
      unsigned index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = index % class_ଗଣନା;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    ଫେରନ୍ତୁ class_to_info;
  }

  graph_result_t<ଶୂନ୍ୟ> shrink (split_context_t& sc,
                               unsigned this_index,
                               unsigned ଗଣନା_2)
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               ଗଣନା_2);

    unsigned old_ଗଣନା = class_ଗଣନା_2;
    ଯଦି (ଗଣନା_2 >= old_ଗଣନା)
      ଫେରନ୍ତୁ Ok();

    class_ଗଣନା_2 = ଗଣନା_2;

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, ଗଣନା_2);
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
                                                                     old_ଗଣନା));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_ଗଣନା,
                                   ଗଣନା_2));

    TRY_ASSIGN (auto mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   ଗଣନା_2));

    ଫେରନ୍ତୁ Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<unsigned> clone_range (split_context_t& sc,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ଅଚଳ
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    unsigned ମୌଳିକ_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (unsigned ମୌଳିକ_id, sc.c.create_ଗଣ୍ଠି (ମୌଳିକ_size));

    MarkBasePosFormat1* ମୌଳିକ = (MarkBasePosFormat1*) graph.object (ମୌଳିକ_id).head;
    ମୌଳିକ->format = ଏହି->format;
    unsigned new_class_ଗଣନା = end - start;
    ମୌଳିକ->class_ଗଣନା_2 = new_class_ଗଣନା;

    TRY_ASSIGN (unsigned base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(ମୌଳିକ->baseCoverage), ମୌଳିକ_id, base_coverage_id));

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 ମୌଳିକ_id,
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
    TRY (graph.add_link (&(ମୌଳିକ->markArray), ମୌଳିକ_id, new_mark_array));

    unsigned class_ଗଣନା = class_ଗଣନା_2;
    TRY_ASSIGN (auto base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_ଗଣନା));

    TRY_ASSIGN (unsigned new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, ଏହି->class_ଗଣନା_2));
    TRY (graph.add_link (&(ମୌଳିକ->baseArray), ମୌଳିକ_id, new_base_array));

    ଫେରନ୍ତୁ Ok(ମୌଳିକ_id);
  }
};


ସଂରଚନା MarkBasePos : ସର୍ବସାଧାରଣ OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1:
      hb_barrier ();
      ଫେରନ୍ତୁ ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    ଯଦି (unlikely (vertex_len < u.format.v.get_size ())) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1:
      hb_barrier ();
      ଫେରନ୍ତୁ ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 2: HB_FALLTHROUGH;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      // We don't handle format 3 and 4 here.
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
