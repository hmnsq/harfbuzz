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

#അന്തര്ഭാവ "split-helpers.hh"
#അന്തര്ഭാവ "coverage-graph.hh"
#അന്തര്ഭാവ "../OT/Layout/GPOS/MarkBasePos.hh"
#അന്തര്ഭാവ "../OT/Layout/GPOS/PosLookupSubTable.hh"

നാമാകാശ graph {

ഘടന AnchorMatrix : പൊതു OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex, unsigned class_എണ്ണം) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size();
    എങ്കിൽ (unlikely (vertex_len < AnchorMatrix::min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();

    എങ്കിൽ (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_എണ്ണം * ഇത്->rows))
      തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  graph_result_t<ശൂന്യം> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_class_എണ്ണം,
                               unsigned new_class_എണ്ണം)
  {
    എങ്കിൽ (unlikely (new_class_എണ്ണം >= old_class_എണ്ണം)) തിരികെ Err(INVALID_ARGUMENT);
    auto& v = c.graph.vertices_[this_index];
    സ്ഥിരം auto& o = v.obj ();
    unsigned base_എണ്ണം = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_എണ്ണം * new_class_എണ്ണം);

    // Reposition links into the new indexing scheme.
    വേണ്ടി (auto& link : v.real_links_writer ())
    {
      unsigned index = (link.position - 2) / 2;
      unsigned base = index / old_class_എണ്ണം;
      unsigned klass = index % old_class_എണ്ണം;
      എങ്കിൽ (unlikely (klass >= new_class_എണ്ണം))
        // should have already been removed
        തിരികെ Err(INVALID_ARGUMENT);

      unsigned new_index = base * new_class_എണ്ണം + klass;

      link.position = (അക്ഷരം*) &(ഇത്->matrixZ[new_index]) - (അക്ഷരം*) ഇത്;
    }

    തിരികെ Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  unsigned start,
                                  unsigned end,
                                  unsigned class_എണ്ണം)
  {
    unsigned base_എണ്ണം = rows;
    unsigned new_class_എണ്ണം = end - start;
    unsigned size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_എണ്ണം * rows;
    TRY_ASSIGN (unsigned അഭാജ്യം_id, c.create_കണ്ണി (size));
    AnchorMatrix* അഭാജ്യം = (AnchorMatrix*) c.graph.object (അഭാജ്യം_id).head;
    അഭാജ്യം->rows = base_എണ്ണം;

    auto& v = c.graph.vertices_[this_index];
    സ്ഥിരം auto& o = v.obj ();

    പൂർണ്ണസംഖ്യ അക്കം_links = o.real_links.length;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_links; i++)
    {
      സ്ഥിരം auto& link = o.real_links[i];
      unsigned old_index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = old_index % class_എണ്ണം;
      എങ്കിൽ (klass < start || klass >= end) തുടരുക;

      unsigned base = old_index / class_എണ്ണം;
      unsigned new_klass = klass - start;
      unsigned new_index = base * new_class_എണ്ണം + new_klass;


      unsigned child_idx = link.objidx;
      TRY (c.graph.add_link (&(അഭാജ്യം->matrixZ[new_index]),
                             അഭാജ്യം_id,
                             child_idx));

      auto& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      അക്കം_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    തിരികെ Ok(അഭാജ്യം_id);
  }
};

ഘടന MarkArray : പൊതു OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size();
    unsigned min_size = MarkArray::min_size;
    എങ്കിൽ (unlikely (vertex_len < min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();

    എങ്കിൽ (unlikely (vertex_len < get_size ())) തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  graph_result_t<ശൂന്യം> shrink (gsubgpos_graph_context_t& c,
                               സ്ഥിരം hb_hashmap_t<unsigned, unsigned>& mark_array_links,
                               unsigned this_index,
                               unsigned new_class_എണ്ണം)
  {
    auto& v = c.graph.vertices_[this_index];
    സ്ഥിരം auto& o = v.obj ();
    വേണ്ടി (സ്ഥിരം auto& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    unsigned new_index = 0;
    വേണ്ടി (സ്ഥിരം auto& record : ഇത്->iter ())
    {
      unsigned klass = record.klass;
      എങ്കിൽ (klass >= new_class_എണ്ണം) തുടരുക;

      (*ഇത്)[new_index].klass = klass;
      unsigned position = (അക്ഷരം*) &record.markAnchor - (അക്ഷരം*) ഇത്;
      unsigned* objidx;
      എങ്കിൽ (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        തുടരുക;
      }

      TRY (c.graph.add_link (&(*ഇത്)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    ഇത്->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    തിരികെ Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  സ്ഥിരം hb_hashmap_t<unsigned, unsigned>& pos_to_index,
                                  hb_set_t& marks,
                                  unsigned start_class)
  {
    unsigned size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (unsigned അഭാജ്യം_id, c.create_കണ്ണി (size));
    MarkArray* അഭാജ്യം = (MarkArray*) c.graph.object (അഭാജ്യം_id).head;
    അഭാജ്യം->len = marks.get_population ();


    unsigned i = 0;
    വേണ്ടി (hb_codepoint_t mark : marks)
    {
      (*അഭാജ്യം)[i].klass = (*ഇത്)[mark].klass - start_class;
      unsigned offset_pos = (അക്ഷരം*) &((*ഇത്)[mark].markAnchor) - (അക്ഷരം*) ഇത്;
      unsigned* anchor_index;
      എങ്കിൽ (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*ഇത്)[mark].markAnchor),
                                 അഭാജ്യം_id,
                                 &((*അഭാജ്യം)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    തിരികെ Ok(അഭാജ്യം_id);
  }
};

ഘടന MarkBasePosFormat1 : പൊതു OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    എങ്കിൽ (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (സ്ഥിരം unsigned base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    സ്ഥിരം unsigned base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    unsigned class_എണ്ണം = class_എണ്ണം_2;
    TRY_ASSIGN (auto base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_എണ്ണം));
    unsigned base_എണ്ണം = base_array.table->rows;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;

    വേണ്ടി (unsigned klass = 0; klass < class_എണ്ണം; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      unsigned accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_എണ്ണം;

      വേണ്ടി (unsigned objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).മൂല്യം_or (0);

      accumulated += accumulated_delta;
      unsigned total = accumulated + partial_coverage_size;

      എങ്കിൽ (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<ശൂന്യം>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (സ്ഥിരം unsigned mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (auto mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      ഇത്,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    തിരികെ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 സ്വകാര്യ:

  ഘടന class_info_t {
    hb_set_t marks;
    hb_vector_t<unsigned> child_indices;
  };

  ഘടന split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    unsigned this_index;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<unsigned, unsigned> mark_array_links;

    hb_set_t marks_for (unsigned start, unsigned end)
    {
      hb_set_t marks;
      വേണ്ടി (unsigned klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      തിരികെ marks;
    }

    unsigned original_എണ്ണം_2 ()
    {
      തിരികെ thiz->class_എണ്ണം_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      തിരികെ thiz->clone_range (*ഇത്, ഇത്->this_index, start, end);
    }

    graph_result_t<ശൂന്യം> shrink (unsigned എണ്ണം)
    {
      തിരികെ thiz->shrink (*ഇത്, ഇത്->this_index, എണ്ണം);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            unsigned this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    unsigned class_എണ്ണം = class_എണ്ണം_2;
    എങ്കിൽ (!class_എണ്ണം) തിരികെ class_to_info;

    എങ്കിൽ (unlikely (!class_to_info.resize (class_എണ്ണം)))
      തിരികെ Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (auto mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    unsigned mark_എണ്ണം = mark_array.table->len;
    വേണ്ടി (unsigned mark = 0; mark < mark_എണ്ണം; mark++)
    {
      unsigned klass = (*mark_array.table)[mark].get_class ();
      എങ്കിൽ (klass >= class_എണ്ണം) തുടരുക;
      class_to_info[klass].marks.add (mark);
    }

    വേണ്ടി (സ്ഥിരം auto& link : mark_array.vertex->obj ().real_links)
    {
      unsigned mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      unsigned klass = (*mark_array.table)[mark].get_class ();
      എങ്കിൽ (klass >= class_എണ്ണം) തുടരുക;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (unsigned base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    auto& base_array_v = c.graph.vertices_[base_array_id];

    വേണ്ടി (സ്ഥിരം auto& link : base_array_v.obj ().real_links)
    {
      unsigned index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = index % class_എണ്ണം;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    തിരികെ class_to_info;
  }

  graph_result_t<ശൂന്യം> shrink (split_context_t& sc,
                               unsigned this_index,
                               unsigned എണ്ണം)
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               എണ്ണം);

    unsigned old_എണ്ണം = class_എണ്ണം_2;
    എങ്കിൽ (എണ്ണം >= old_എണ്ണം)
      തിരികെ Ok();

    class_എണ്ണം_2 = എണ്ണം;

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, എണ്ണം);
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
                                                                     old_എണ്ണം));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_എണ്ണം,
                                   എണ്ണം));

    TRY_ASSIGN (auto mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   എണ്ണം));

    തിരികെ Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<unsigned> clone_range (split_context_t& sc,
                                        unsigned this_index,
                                        unsigned start, unsigned end) സ്ഥിരം
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    unsigned അഭാജ്യം_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (unsigned അഭാജ്യം_id, sc.c.create_കണ്ണി (അഭാജ്യം_size));

    MarkBasePosFormat1* അഭാജ്യം = (MarkBasePosFormat1*) graph.object (അഭാജ്യം_id).head;
    അഭാജ്യം->format = ഇത്->format;
    unsigned new_class_എണ്ണം = end - start;
    അഭാജ്യം->class_എണ്ണം_2 = new_class_എണ്ണം;

    TRY_ASSIGN (unsigned base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(അഭാജ്യം->baseCoverage), അഭാജ്യം_id, base_coverage_id));

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 അഭാജ്യം_id,
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
    TRY (graph.add_link (&(അഭാജ്യം->markArray), അഭാജ്യം_id, new_mark_array));

    unsigned class_എണ്ണം = class_എണ്ണം_2;
    TRY_ASSIGN (auto base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_എണ്ണം));

    TRY_ASSIGN (unsigned new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, ഇത്->class_എണ്ണം_2));
    TRY (graph.add_link (&(അഭാജ്യം->baseArray), അഭാജ്യം_id, new_base_array));

    തിരികെ Ok(അഭാജ്യം_id);
  }
};


ഘടന MarkBasePos : പൊതു OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1:
      hb_barrier ();
      തിരികെ ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    സ്ഥിരസ്ഥിതി:
      തിരികെ Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    എങ്കിൽ (unlikely (vertex_len < u.format.v.get_size ())) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();

    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1:
      hb_barrier ();
      തിരികെ ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2: HB_FALLTHROUGH;
#endif
    സ്ഥിരസ്ഥിതി:
      // We don't handle format 3 and 4 here.
      തിരികെ Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
