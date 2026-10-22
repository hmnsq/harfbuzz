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

#ضمن "split-helpers.hh"
#ضمن "coverage-graph.hh"
#ضمن "../OT/Layout/GPOS/MarkBasePos.hh"
#ضمن "../OT/Layout/GPOS/PosLookupSubTable.hh"

نطاق graph {

بنية AnchorMatrix : عام OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex, unsigned class_تعداد) ثابت
  {
    size_t vertex_len = vertex.table_size();
    إذا (unlikely (vertex_len < AnchorMatrix::min_size)) إرجاع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_تعداد * هذا->rows))
      إرجاع Err(SANITIZE_FAILURE);
    إرجاع Ok();
  }

  graph_result_t<فراغ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_class_تعداد,
                               unsigned new_class_تعداد)
  {
    إذا (unlikely (new_class_تعداد >= old_class_تعداد)) إرجاع Err(INVALID_ARGUMENT);
    auto& v = c.graph.vertices_[this_index];
    ثابت auto& o = v.obj ();
    unsigned base_تعداد = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_تعداد * new_class_تعداد);

    // Reposition links into the new indexing scheme.
    لكل (auto& link : v.real_links_writer ())
    {
      unsigned index = (link.position - 2) / 2;
      unsigned base = index / old_class_تعداد;
      unsigned klass = index % old_class_تعداد;
      إذا (unlikely (klass >= new_class_تعداد))
        // should have already been removed
        إرجاع Err(INVALID_ARGUMENT);

      unsigned new_index = base * new_class_تعداد + klass;

      link.position = (حرف*) &(هذا->matrixZ[new_index]) - (حرف*) هذا;
    }

    إرجاع Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  unsigned start,
                                  unsigned end,
                                  unsigned class_تعداد)
  {
    unsigned base_تعداد = rows;
    unsigned new_class_تعداد = end - start;
    unsigned size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_تعداد * rows;
    TRY_ASSIGN (unsigned أولي_id, c.create_عقدة (size));
    AnchorMatrix* أولي = (AnchorMatrix*) c.graph.object (أولي_id).head;
    أولي->rows = base_تعداد;

    auto& v = c.graph.vertices_[this_index];
    ثابت auto& o = v.obj ();

    صحيح رقم_links = o.real_links.length;
    لكل (صحيح i = 0; i < رقم_links; i++)
    {
      ثابت auto& link = o.real_links[i];
      unsigned old_index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = old_index % class_تعداد;
      إذا (klass < start || klass >= end) استمر;

      unsigned base = old_index / class_تعداد;
      unsigned new_klass = klass - start;
      unsigned new_index = base * new_class_تعداد + new_klass;


      unsigned child_idx = link.objidx;
      TRY (c.graph.add_link (&(أولي->matrixZ[new_index]),
                             أولي_id,
                             child_idx));

      auto& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      رقم_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    إرجاع Ok(أولي_id);
  }
};

بنية MarkArray : عام OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size();
    unsigned min_size = MarkArray::min_size;
    إذا (unlikely (vertex_len < min_size)) إرجاع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (vertex_len < get_size ())) إرجاع Err(SANITIZE_FAILURE);
    إرجاع Ok();
  }

  graph_result_t<فراغ> shrink (gsubgpos_graph_context_t& c,
                               ثابت hb_hashmap_t<unsigned, unsigned>& mark_array_links,
                               unsigned this_index,
                               unsigned new_class_تعداد)
  {
    auto& v = c.graph.vertices_[this_index];
    ثابت auto& o = v.obj ();
    لكل (ثابت auto& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    unsigned new_index = 0;
    لكل (ثابت auto& record : هذا->iter ())
    {
      unsigned klass = record.klass;
      إذا (klass >= new_class_تعداد) استمر;

      (*هذا)[new_index].klass = klass;
      unsigned position = (حرف*) &record.markAnchor - (حرف*) هذا;
      unsigned* objidx;
      إذا (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        استمر;
      }

      TRY (c.graph.add_link (&(*هذا)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    هذا->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    إرجاع Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  ثابت hb_hashmap_t<unsigned, unsigned>& pos_to_index,
                                  hb_set_t& marks,
                                  unsigned start_class)
  {
    unsigned size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (unsigned أولي_id, c.create_عقدة (size));
    MarkArray* أولي = (MarkArray*) c.graph.object (أولي_id).head;
    أولي->len = marks.get_population ();


    unsigned i = 0;
    لكل (hb_codepoint_t mark : marks)
    {
      (*أولي)[i].klass = (*هذا)[mark].klass - start_class;
      unsigned offset_pos = (حرف*) &((*هذا)[mark].markAnchor) - (حرف*) هذا;
      unsigned* anchor_index;
      إذا (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*هذا)[mark].markAnchor),
                                 أولي_id,
                                 &((*أولي)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    إرجاع Ok(أولي_id);
  }
};

بنية MarkBasePosFormat1 : عام OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    إذا (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      إرجاع Err(SANITIZE_FAILURE);
    إرجاع Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ثابت unsigned base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    ثابت unsigned base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    unsigned class_تعداد = class_تعداد_2;
    TRY_ASSIGN (auto base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_تعداد));
    unsigned base_تعداد = base_array.table->rows;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;

    لكل (unsigned klass = 0; klass < class_تعداد; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      unsigned accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_تعداد;

      لكل (unsigned objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).قيمة_or (0);

      accumulated += accumulated_delta;
      unsigned total = accumulated + partial_coverage_size;

      إذا (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<فراغ>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (ثابت unsigned mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (auto mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      هذا,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    إرجاع actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 خاص:

  بنية class_info_t {
    hb_set_t marks;
    hb_vector_t<unsigned> child_indices;
  };

  بنية split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    unsigned this_index;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<unsigned, unsigned> mark_array_links;

    hb_set_t marks_for (unsigned start, unsigned end)
    {
      hb_set_t marks;
      لكل (unsigned klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      إرجاع marks;
    }

    unsigned original_تعداد_2 ()
    {
      إرجاع thiz->class_تعداد_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      إرجاع thiz->clone_range (*هذا, هذا->this_index, start, end);
    }

    graph_result_t<فراغ> shrink (unsigned تعداد_2)
    {
      إرجاع thiz->shrink (*هذا, هذا->this_index, تعداد_2);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            unsigned this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    unsigned class_تعداد = class_تعداد_2;
    إذا (!class_تعداد) إرجاع class_to_info;

    إذا (unlikely (!class_to_info.resize (class_تعداد)))
      إرجاع Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (auto mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    unsigned mark_تعداد = mark_array.table->len;
    لكل (unsigned mark = 0; mark < mark_تعداد; mark++)
    {
      unsigned klass = (*mark_array.table)[mark].get_class ();
      إذا (klass >= class_تعداد) استمر;
      class_to_info[klass].marks.add (mark);
    }

    لكل (ثابت auto& link : mark_array.vertex->obj ().real_links)
    {
      unsigned mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      unsigned klass = (*mark_array.table)[mark].get_class ();
      إذا (klass >= class_تعداد) استمر;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (unsigned base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    auto& base_array_v = c.graph.vertices_[base_array_id];

    لكل (ثابت auto& link : base_array_v.obj ().real_links)
    {
      unsigned index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = index % class_تعداد;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    إرجاع class_to_info;
  }

  graph_result_t<فراغ> shrink (split_context_t& sc,
                               unsigned this_index,
                               unsigned تعداد_2)
  {
    DEBUG_رسالة (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               تعداد_2);

    unsigned old_تعداد = class_تعداد_2;
    إذا (تعداد_2 >= old_تعداد)
      إرجاع Ok();

    class_تعداد_2 = تعداد_2;

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, تعداد_2);
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
                                                                     old_تعداد));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_تعداد,
                                   تعداد_2));

    TRY_ASSIGN (auto mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   تعداد_2));

    إرجاع Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<unsigned> clone_range (split_context_t& sc,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ثابت
  {
    DEBUG_رسالة (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    unsigned أولي_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (unsigned أولي_id, sc.c.create_عقدة (أولي_size));

    MarkBasePosFormat1* أولي = (MarkBasePosFormat1*) graph.object (أولي_id).head;
    أولي->format = هذا->format;
    unsigned new_class_تعداد = end - start;
    أولي->class_تعداد_2 = new_class_تعداد;

    TRY_ASSIGN (unsigned base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(أولي->baseCoverage), أولي_id, base_coverage_id));

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 أولي_id,
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
    TRY (graph.add_link (&(أولي->markArray), أولي_id, new_mark_array));

    unsigned class_تعداد = class_تعداد_2;
    TRY_ASSIGN (auto base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_تعداد));

    TRY_ASSIGN (unsigned new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, هذا->class_تعداد_2));
    TRY (graph.add_link (&(أولي->baseArray), أولي_id, new_base_array));

    إرجاع Ok(أولي_id);
  }
};


بنية MarkBasePos : عام OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      إرجاع ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    افتراضي:
      إرجاع Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    إذا (unlikely (vertex_len < u.format.v.get_size ())) إرجاع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      إرجاع ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
#endif
    افتراضي:
      // We don't handle format 3 and 4 here.
      إرجاع Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
