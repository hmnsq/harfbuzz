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

#অন্তর্ভাৱ "split-helpers.hh"
#অন্তর্ভাৱ "coverage-graph.hh"
#অন্তর্ভাৱ "../OT/Layout/GPOS/MarkBasePos.hh"
#অন্তর্ভাৱ "../OT/Layout/GPOS/PosLookupSubTable.hh"

নামাকাশ graph {

সংরচনা AnchorMatrix : সার্ৱজনিক OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_result_t<শূন্য> sanitize (ধ্রুৱ graph_t::vertex_t& vertex, unsigned class_গুনতি) ধ্রুৱ
  {
    size_t vertex_len = vertex.table_size();
    যদি (unlikely (vertex_len < AnchorMatrix::min_size)) ঘূৰাই Err(SANITIZE_FAILURE);
    hb_barrier ();

    যদি (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_গুনতি * এতত্->rows))
      ঘূৰাই Err(SANITIZE_FAILURE);
    ঘূৰাই Ok();
  }

  graph_result_t<শূন্য> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_class_গুনতি,
                               unsigned new_class_গুনতি)
  {
    যদি (unlikely (new_class_গুনতি >= old_class_গুনতি)) ঘূৰাই Err(INVALID_ARGUMENT);
    auto& v = c.graph.vertices_[this_index];
    ধ্রুৱ auto& o = v.obj ();
    unsigned base_গুনতি = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_গুনতি * new_class_গুনতি);

    // Reposition links into the new indexing scheme.
    বাবে (auto& link : v.real_links_writer ())
    {
      unsigned index = (link.position - 2) / 2;
      unsigned base = index / old_class_গুনতি;
      unsigned klass = index % old_class_গুনতি;
      যদি (unlikely (klass >= new_class_গুনতি))
        // should have already been removed
        ঘূৰাই Err(INVALID_ARGUMENT);

      unsigned new_index = base * new_class_গুনতি + klass;

      link.position = (ৱর্ণ*) &(এতত্->matrixZ[new_index]) - (ৱর্ণ*) এতত্;
    }

    ঘূৰাই Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  unsigned start,
                                  unsigned end,
                                  unsigned class_গুনতি)
  {
    unsigned base_গুনতি = rows;
    unsigned new_class_গুনতি = end - start;
    unsigned size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_গুনতি * rows;
    TRY_ASSIGN (unsigned মৌলিক_id, c.create_গ্ৰন্থি (size));
    AnchorMatrix* মৌলিক = (AnchorMatrix*) c.graph.object (মৌলিক_id).head;
    মৌলিক->rows = base_গুনতি;

    auto& v = c.graph.vertices_[this_index];
    ধ্রুৱ auto& o = v.obj ();

    পূর্ণাঙ্ক অঙ্ক_links = o.real_links.length;
    বাবে (পূর্ণাঙ্ক i = 0; i < অঙ্ক_links; i++)
    {
      ধ্রুৱ auto& link = o.real_links[i];
      unsigned old_index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = old_index % class_গুনতি;
      যদি (klass < start || klass >= end) অনুৱর্ত;

      unsigned base = old_index / class_গুনতি;
      unsigned new_klass = klass - start;
      unsigned new_index = base * new_class_গুনতি + new_klass;


      unsigned child_idx = link.objidx;
      TRY (c.graph.add_link (&(মৌলিক->matrixZ[new_index]),
                             মৌলিক_id,
                             child_idx));

      auto& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      অঙ্ক_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    ঘূৰাই Ok(মৌলিক_id);
  }
};

সংরচনা MarkArray : সার্ৱজনিক OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<শূন্য> sanitize (ধ্রুৱ graph_t::vertex_t& vertex) ধ্রুৱ
  {
    size_t vertex_len = vertex.table_size();
    unsigned min_size = MarkArray::min_size;
    যদি (unlikely (vertex_len < min_size)) ঘূৰাই Err(SANITIZE_FAILURE);
    hb_barrier ();

    যদি (unlikely (vertex_len < get_size ())) ঘূৰাই Err(SANITIZE_FAILURE);
    ঘূৰাই Ok();
  }

  graph_result_t<শূন্য> shrink (gsubgpos_graph_context_t& c,
                               ধ্রুৱ hb_hashmap_t<unsigned, unsigned>& mark_array_links,
                               unsigned this_index,
                               unsigned new_class_গুনতি)
  {
    auto& v = c.graph.vertices_[this_index];
    ধ্রুৱ auto& o = v.obj ();
    বাবে (ধ্রুৱ auto& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    unsigned new_index = 0;
    বাবে (ধ্রুৱ auto& record : এতত্->iter ())
    {
      unsigned klass = record.klass;
      যদি (klass >= new_class_গুনতি) অনুৱর্ত;

      (*এতত্)[new_index].klass = klass;
      unsigned position = (ৱর্ণ*) &record.markAnchor - (ৱর্ণ*) এতত্;
      unsigned* objidx;
      যদি (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        অনুৱর্ত;
      }

      TRY (c.graph.add_link (&(*এতত্)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    এতত্->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    ঘূৰাই Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  ধ্রুৱ hb_hashmap_t<unsigned, unsigned>& pos_to_index,
                                  hb_set_t& marks,
                                  unsigned start_class)
  {
    unsigned size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (unsigned মৌলিক_id, c.create_গ্ৰন্থি (size));
    MarkArray* মৌলিক = (MarkArray*) c.graph.object (মৌলিক_id).head;
    মৌলিক->len = marks.get_population ();


    unsigned i = 0;
    বাবে (hb_codepoint_t mark : marks)
    {
      (*মৌলিক)[i].klass = (*এতত্)[mark].klass - start_class;
      unsigned offset_pos = (ৱর্ণ*) &((*এতত্)[mark].markAnchor) - (ৱর্ণ*) এতত্;
      unsigned* anchor_index;
      যদি (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*এতত্)[mark].markAnchor),
                                 মৌলিক_id,
                                 &((*মৌলিক)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    ঘূৰাই Ok(মৌলিক_id);
  }
};

সংরচনা MarkBasePosFormat1 : সার্ৱজনিক OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<শূন্য> sanitize (ধ্রুৱ graph_t::vertex_t& vertex) ধ্রুৱ
  {
    size_t vertex_len = vertex.table_size ();
    যদি (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      ঘূৰাই Err(SANITIZE_FAILURE);
    ঘূৰাই Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ধ্রুৱ unsigned base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    ধ্রুৱ unsigned base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    unsigned class_গুনতি = class_গুনতি_2;
    TRY_ASSIGN (auto base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_গুনতি));
    unsigned base_গুনতি = base_array.table->rows;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;

    বাবে (unsigned klass = 0; klass < class_গুনতি; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      unsigned accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_গুনতি;

      বাবে (unsigned objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).মান_or (0);

      accumulated += accumulated_delta;
      unsigned total = accumulated + partial_coverage_size;

      যদি (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<শূন্য>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (ধ্রুৱ unsigned mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (auto mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      এতত্,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    ঘূৰাই actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 গুপ্ত:

  সংরচনা class_info_t {
    hb_set_t marks;
    hb_vector_t<unsigned> child_indices;
  };

  সংরচনা split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    unsigned this_index;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<unsigned, unsigned> mark_array_links;

    hb_set_t marks_for (unsigned start, unsigned end)
    {
      hb_set_t marks;
      বাবে (unsigned klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      ঘূৰাই marks;
    }

    unsigned original_গুনতি_2 ()
    {
      ঘূৰাই thiz->class_গুনতি_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ঘূৰাই thiz->clone_range (*এতত্, এতত্->this_index, start, end);
    }

    graph_result_t<শূন্য> shrink (unsigned গুনতি)
    {
      ঘূৰাই thiz->shrink (*এতত্, এতত্->this_index, গুনতি);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            unsigned this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    unsigned class_গুনতি = class_গুনতি_2;
    যদি (!class_গুনতি) ঘূৰাই class_to_info;

    যদি (unlikely (!class_to_info.resize (class_গুনতি)))
      ঘূৰাই Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (auto mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    unsigned mark_গুনতি = mark_array.table->len;
    বাবে (unsigned mark = 0; mark < mark_গুনতি; mark++)
    {
      unsigned klass = (*mark_array.table)[mark].get_class ();
      যদি (klass >= class_গুনতি) অনুৱর্ত;
      class_to_info[klass].marks.add (mark);
    }

    বাবে (ধ্রুৱ auto& link : mark_array.vertex->obj ().real_links)
    {
      unsigned mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      unsigned klass = (*mark_array.table)[mark].get_class ();
      যদি (klass >= class_গুনতি) অনুৱর্ত;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (unsigned base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    auto& base_array_v = c.graph.vertices_[base_array_id];

    বাবে (ধ্রুৱ auto& link : base_array_v.obj ().real_links)
    {
      unsigned index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = index % class_গুনতি;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    ঘূৰাই class_to_info;
  }

  graph_result_t<শূন্য> shrink (split_context_t& sc,
                               unsigned this_index,
                               unsigned গুনতি)
  {
    DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               গুনতি);

    unsigned old_গুনতি = class_গুনতি_2;
    যদি (গুনতি >= old_গুনতি)
      ঘূৰাই Ok();

    class_গুনতি_2 = গুনতি;

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, গুনতি);
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
                                                                     old_গুনতি));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_গুনতি,
                                   গুনতি));

    TRY_ASSIGN (auto mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   গুনতি));

    ঘূৰাই Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<unsigned> clone_range (split_context_t& sc,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ধ্রুৱ
  {
    DEBUG_বাৰ্তা (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    unsigned মৌলিক_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (unsigned মৌলিক_id, sc.c.create_গ্ৰন্থি (মৌলিক_size));

    MarkBasePosFormat1* মৌলিক = (MarkBasePosFormat1*) graph.object (মৌলিক_id).head;
    মৌলিক->format = এতত্->format;
    unsigned new_class_গুনতি = end - start;
    মৌলিক->class_গুনতি_2 = new_class_গুনতি;

    TRY_ASSIGN (unsigned base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(মৌলিক->baseCoverage), মৌলিক_id, base_coverage_id));

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
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
         graph.as_mutable_table <MarkArray> (sc.this_index, &markArray));

    TRY_ASSIGN (unsigned new_mark_array,
         mark_array.table->clone (sc.c,
                                  mark_array.index,
                                  sc.mark_array_links,
                                  marks,
                                  start));
    TRY (graph.add_link (&(মৌলিক->markArray), মৌলিক_id, new_mark_array));

    unsigned class_গুনতি = class_গুনতি_2;
    TRY_ASSIGN (auto base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_গুনতি));

    TRY_ASSIGN (unsigned new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, এতত্->class_গুনতি_2));
    TRY (graph.add_link (&(মৌলিক->baseArray), মৌলিক_id, new_base_array));

    ঘূৰাই Ok(মৌলিক_id);
  }
};


সংরচনা MarkBasePos : সার্ৱজনিক OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1:
      hb_barrier ();
      ঘূৰাই ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    স্ৱভাৱ:
      ঘূৰাই Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<শূন্য> sanitize (ধ্রুৱ graph_t::vertex_t& vertex) ধ্রুৱ
  {
    size_t vertex_len = vertex.table_size ();
    যদি (unlikely (vertex_len < u.format.v.get_size ())) ঘূৰাই Err(SANITIZE_FAILURE);
    hb_barrier ();

    ৱিকল্প (u.format.v) {
    পক্ষ 1:
      hb_barrier ();
      ঘূৰাই ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    পক্ষ 2: HB_FALLTHROUGH;
#endif
    স্ৱভাৱ:
      // We don't handle format 3 and 4 here.
      ঘূৰাই Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
