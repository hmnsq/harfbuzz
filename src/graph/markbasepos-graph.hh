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

#অন্তর্ভাব "split-helpers.hh"
#অন্তর্ভাব "coverage-graph.hh"
#অন্তর্ভাব "../OT/Layout/GPOS/MarkBasePos.hh"
#অন্তর্ভাব "../OT/Layout/GPOS/PosLookupSubTable.hh"

নামাকাশ graph {

কাঠামো AnchorMatrix : সর্বজনীন OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex, unsigned class_গুনতি) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার();
    যদি (unlikely (vertex_len < AnchorMatrix::min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    যদি (unlikely (vertex_len < AnchorMatrix::min_আকার +
                  OT::Offset16::static_আকার * class_গুনতি * এই->rows))
      ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  graph_ফলাফল_t<শূন্য> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_সূচক,
                               unsigned old_class_গুনতি,
                               unsigned new_class_গুনতি)
  {
    যদি (unlikely (new_class_গুনতি >= old_class_গুনতি)) ফেরত Err(INVALID_ARGUMENT);
    auto& v = c.graph.vertices_[this_সূচক];
    ধ্রুবক auto& o = v.obj ();
    unsigned base_গুনতি = rows;
    v.set_বাফার (o.শীর্ষ_2,
      o.শীর্ষ_2 +
      AnchorMatrix::min_আকার +
      OT::Offset16::static_আকার * base_গুনতি * new_class_গুনতি);

    // Reposition links into the new indexing scheme.
    জন্য (auto& link : v.real_links_writer ())
    {
      unsigned সূচক_2 = (link.position - 2) / 2;
      unsigned base = সূচক_2 / old_class_গুনতি;
      unsigned klass = সূচক_2 % old_class_গুনতি;
      যদি (unlikely (klass >= new_class_গুনতি))
        // should have already been removed
        ফেরত Err(INVALID_ARGUMENT);

      unsigned new_সূচক = base * new_class_গুনতি + klass;

      link.position = (অক্ষর*) &(এই->matrixZ[new_সূচক]) - (অক্ষর*) এই;
    }

    ফেরত Ok();
  }

  graph_ফলাফল_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_সূচক,
                                  unsigned start,
                                  unsigned end,
                                  unsigned class_গুনতি)
  {
    unsigned base_গুনতি = rows;
    unsigned new_class_গুনতি = end - start;
    unsigned আকার_2 = AnchorMatrix::min_আকার +
                    OT::Offset16::static_আকার * new_class_গুনতি * rows;
    TRY_ASSIGN (unsigned মৌলিক_id, c.create_গ্রন্থি (আকার_2));
    AnchorMatrix* মৌলিক = (AnchorMatrix*) c.graph.object (মৌলিক_id).শীর্ষ_2;
    মৌলিক->rows = base_গুনতি;

    auto& v = c.graph.vertices_[this_সূচক];
    ধ্রুবক auto& o = v.obj ();

    পূর্ণসংখ্যা অঙ্ক_links = o.real_links.দৈর্ঘ্য;
    জন্য (পূর্ণসংখ্যা i = 0; i < অঙ্ক_links; i++)
    {
      ধ্রুবক auto& link = o.real_links[i];
      unsigned old_সূচক = (link.position - 2) / OT::Offset16::static_আকার;
      unsigned klass = old_সূচক % class_গুনতি;
      যদি (klass < start || klass >= end) চালু;

      unsigned base = old_সূচক / class_গুনতি;
      unsigned new_klass = klass - start;
      unsigned new_সূচক = base * new_class_গুনতি + new_klass;


      unsigned child_idx = link.objidx;
      TRY (c.graph.add_link (&(মৌলিক->matrixZ[new_সূচক]),
                             মৌলিক_id,
                             child_idx));

      auto& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_সূচক);

      v.remove_real_link_unordered (i);
      অঙ্ক_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    ফেরত Ok(মৌলিক_id);
  }
};

কাঠামো MarkArray : সর্বজনীন OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার();
    unsigned min_আকার = MarkArray::min_আকার;
    যদি (unlikely (vertex_len < min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    যদি (unlikely (vertex_len < get_আকার ())) ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  graph_ফলাফল_t<শূন্য> shrink (gsubgpos_graph_context_t& c,
                               ধ্রুবক hb_hashmap_t<unsigned, unsigned>& mark_array_links,
                               unsigned this_সূচক,
                               unsigned new_class_গুনতি)
  {
    auto& v = c.graph.vertices_[this_সূচক];
    ধ্রুবক auto& o = v.obj ();
    জন্য (ধ্রুবক auto& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_সূচক);
    v.clear_real_links ();

    unsigned new_সূচক = 0;
    জন্য (ধ্রুবক auto& record : এই->iter ())
    {
      unsigned klass = record.klass;
      যদি (klass >= new_class_গুনতি) চালু;

      (*এই)[new_সূচক].klass = klass;
      unsigned position = (অক্ষর*) &record.markAnchor - (অক্ষর*) এই;
      unsigned* objidx;
      যদি (!mark_array_links.has (position, &objidx))
      {
        new_সূচক++;
        চালু;
      }

      TRY (c.graph.add_link (&(*এই)[new_সূচক].markAnchor, this_সূচক, *objidx));
      new_সূচক++;
    }

    এই->len = new_সূচক;
    v.set_বাফার (o.শীর্ষ_2, o.শীর্ষ_2 + MarkArray::min_আকার +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_আকার * new_সূচক);

    ফেরত Ok();
  }

  graph_ফলাফল_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_সূচক,
                                  ধ্রুবক hb_hashmap_t<unsigned, unsigned>& pos_to_সূচক,
                                  hb_set_t& marks,
                                  unsigned start_class)
  {
    unsigned আকার_2 = MarkArray::min_আকার +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_আকার *
                    marks.get_population ();
    TRY_ASSIGN (unsigned মৌলিক_id, c.create_গ্রন্থি (আকার_2));
    MarkArray* মৌলিক = (MarkArray*) c.graph.object (মৌলিক_id).শীর্ষ_2;
    মৌলিক->len = marks.get_population ();


    unsigned i = 0;
    জন্য (hb_codepoint_t mark : marks)
    {
      (*মৌলিক)[i].klass = (*এই)[mark].klass - start_class;
      unsigned offset_pos = (অক্ষর*) &((*এই)[mark].markAnchor) - (অক্ষর*) এই;
      unsigned* anchor_সূচক;
      যদি (pos_to_সূচক.has (offset_pos, &anchor_সূচক))
        TRY (c.graph.move_child (this_সূচক,
                                 &((*এই)[mark].markAnchor),
                                 মৌলিক_id,
                                 &((*মৌলিক)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_সূচক].sort_real_links ();

    ফেরত Ok(মৌলিক_id);
  }
};

কাঠামো MarkBasePosFormat1 : সর্বজনীন OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    যদি (unlikely (vertex_len < MarkBasePosFormat1::static_আকার))
      ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  graph_ফলাফল_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_সূচক)
  {
    hb_set_t visited;

    TRY_ASSIGN (ধ্রুবক unsigned base_coverage_id, c.graph.সূচক_for_offset (this_সূচক, &baseCoverage));

    ধ্রুবক unsigned base_আকার =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_আকার +
        MarkArray::min_আকার +
        AnchorMatrix::min_আকার +
        c.graph.vertices_[base_coverage_id].table_আকার ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_সূচক));

    unsigned class_গুনতি = class_গুনতি_2;
    TRY_ASSIGN (auto base_array, c.graph.as_table<AnchorMatrix> (this_সূচক,
                                                          &baseArray,
                                                          class_গুনতি));
    unsigned base_গুনতি = base_array.table->rows;

    unsigned partial_coverage_আকার = 4;
    unsigned accumulated = base_আকার;
    hb_vector_t<unsigned> split_points;

    জন্য (unsigned klass = 0; klass < class_গুনতি; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_আকার += OT::HBUINT16::static_আকার * info.marks.get_population ();
      unsigned accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_আকার * info.marks.get_population () +
          OT::Offset16::static_আকার * base_গুনতি;

      জন্য (unsigned objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_আকার (objidx, visited).মান_or (0);

      accumulated += accumulated_delta;
      unsigned যোগ = accumulated + partial_coverage_আকার;

      যদি (যোগ >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_ফলাফল_t<শূন্য>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_আকার + accumulated_delta;
        partial_coverage_আকার = 4 + OT::HBUINT16::static_আকার * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (ধ্রুবক unsigned mark_array_id, c.graph.সূচক_for_offset (this_সূচক, &markArray));
    TRY_ASSIGN (auto mark_array_links, c.graph.vertices_[mark_array_id].position_to_সূচক_map ());

    split_context_t split_context {
      c,
      এই,
      this_সূচক,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    ফেরত actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ব্যক্তিগত:

  কাঠামো class_info_t {
    hb_set_t marks;
    hb_vector_t<unsigned> child_indices;
  };

  কাঠামো split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    unsigned this_সূচক;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<unsigned, unsigned> mark_array_links;

    hb_set_t marks_for (unsigned start, unsigned end)
    {
      hb_set_t marks;
      জন্য (unsigned klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      ফেরত marks;
    }

    unsigned original_গুনতি_2 ()
    {
      ফেরত thiz->class_গুনতি_2;
    }

    graph_ফলাফল_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ফেরত thiz->clone_range (*এই, এই->this_সূচক, start, end);
    }

    graph_ফলাফল_t<শূন্য> shrink (unsigned গুনতি)
    {
      ফেরত thiz->shrink (*এই, এই->this_সূচক, গুনতি);
    }
  };

  graph_ফলাফল_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            unsigned this_সূচক)
  {
    hb_vector_t<class_info_t> class_to_info;

    unsigned class_গুনতি = class_গুনতি_2;
    যদি (!class_গুনতি) ফেরত class_to_info;

    যদি (unlikely (!class_to_info.resize (class_গুনতি)))
      ফেরত Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (auto mark_array, c.graph.as_table<MarkArray> (this_সূচক, &markArray));

    unsigned mark_গুনতি = mark_array.table->len;
    জন্য (unsigned mark = 0; mark < mark_গুনতি; mark++)
    {
      unsigned klass = (*mark_array.table)[mark].get_class ();
      যদি (klass >= class_গুনতি) চালু;
      class_to_info[klass].marks.add (mark);
    }

    জন্য (ধ্রুবক auto& link : mark_array.vertex->obj ().real_links)
    {
      unsigned mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_আকার;
      unsigned klass = (*mark_array.table)[mark].get_class ();
      যদি (klass >= class_গুনতি) চালু;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (unsigned base_array_id,
         c.graph.সূচক_for_offset (this_সূচক, &baseArray));

    auto& base_array_v = c.graph.vertices_[base_array_id];

    জন্য (ধ্রুবক auto& link : base_array_v.obj ().real_links)
    {
      unsigned সূচক_2 = (link.position - 2) / OT::Offset16::static_আকার;
      unsigned klass = সূচক_2 % class_গুনতি;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    ফেরত class_to_info;
  }

  graph_ফলাফল_t<শূন্য> shrink (split_context_t& sc,
                               unsigned this_সূচক,
                               unsigned গুনতি)
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_সূচক,
               গুনতি);

    unsigned old_গুনতি = class_গুনতি_2;
    যদি (গুনতি >= old_গুনতি)
      ফেরত Ok();

    class_গুনতি_2 = গুনতি;

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_সূচক,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, গুনতি);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.সূচক_2,
                                  4 + 2 * marks.get_population ()));


    TRY_ASSIGN (auto base_array, sc.c.graph.as_mutable_table<AnchorMatrix> (this_সূচক,
                                                                     &baseArray,
                                                                     old_গুনতি));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.সূচক_2,
                                   old_গুনতি,
                                   গুনতি));

    TRY_ASSIGN (auto mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_সূচক,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.সূচক_2,
                                   গুনতি));

    ফেরত Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_ফলাফল_t<unsigned> clone_range (split_context_t& sc,
                                        unsigned this_সূচক,
                                        unsigned start, unsigned end) ধ্রুবক
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_সূচক, start, end);

    graph_t& graph = sc.c.graph;
    unsigned মৌলিক_আকার = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_আকার;

    TRY_ASSIGN (unsigned মৌলিক_id, sc.c.create_গ্রন্থি (মৌলিক_আকার));

    MarkBasePosFormat1* মৌলিক = (MarkBasePosFormat1*) graph.object (মৌলিক_id).শীর্ষ_2;
    মৌলিক->format = এই->format;
    unsigned new_class_গুনতি = end - start;
    মৌলিক->class_গুনতি_2 = new_class_গুনতি;

    TRY_ASSIGN (unsigned base_coverage_id,
         graph.সূচক_for_offset (sc.this_সূচক, &baseCoverage));

    TRY (graph.add_link (&(মৌলিক->baseCoverage), মৌলিক_id, base_coverage_id));

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_table<Coverage> (this_সূচক,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 মৌলিক_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_ASSIGN (auto mark_array,
         graph.as_mutable_table <MarkArray> (sc.this_সূচক, &markArray));

    TRY_ASSIGN (unsigned new_mark_array,
         mark_array.table->clone (sc.c,
                                  mark_array.সূচক_2,
                                  sc.mark_array_links,
                                  marks,
                                  start));
    TRY (graph.add_link (&(মৌলিক->markArray), মৌলিক_id, new_mark_array));

    unsigned class_গুনতি = class_গুনতি_2;
    TRY_ASSIGN (auto base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_সূচক, &baseArray, class_গুনতি));

    TRY_ASSIGN (unsigned new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.সূচক_2,
                                  start, end, এই->class_গুনতি_2));
    TRY (graph.add_link (&(মৌলিক->baseArray), মৌলিক_id, new_base_array));

    ফেরত Ok(মৌলিক_id);
  }
};


কাঠামো MarkBasePos : সর্বজনীন OT::Layout::GPOS_impl::MarkBasePos
{
  graph_ফলাফল_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_সূচক)
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1:
      hb_barrier ();
      ফেরত ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_সূচক);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    পূর্বনির্ধারিত:
      ফেরত Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    যদি (unlikely (vertex_len < u.format.v.get_আকার ())) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    নির্বাচন (u.format.v) {
    ক্ষেত্র 1:
      hb_barrier ();
      ফেরত ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 2: HB_FALLTHROUGH;
#endif
    পূর্বনির্ধারিত:
      // We don't handle format 3 and 4 here.
      ফেরত Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
