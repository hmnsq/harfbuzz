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

#अन्तर्भाव "split-helpers.hh"
#अन्तर्भाव "coverage-graph.hh"
#अन्तर्भाव "../OT/Layout/GPOS/MarkBasePos.hh"
#अन्तर्भाव "../OT/Layout/GPOS/PosLookupSubTable.hh"

नामाकाश graph {

संरचना AnchorMatrix : सार्वजनिक OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex, unsigned class_गणनम्) ध्रुव
  {
    size_t vertex_len = vertex.table_size();
    जइ (unlikely (vertex_len < AnchorMatrix::min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();

    जइ (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_गणनम् * एतत्->rows))
      प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_class_गणनम्,
                               unsigned new_class_गणनम्)
  {
    जइ (unlikely (new_class_गणनम् >= old_class_गणनम्)) प्रत्यागम Err(INVALID_ARGUMENT);
    auto& v = c.graph.vertices_[this_index];
    ध्रुव auto& o = v.obj ();
    unsigned base_गणनम् = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_गणनम् * new_class_गणनम्);

    // Reposition links into the new indexing scheme.
    प्रति (auto& link : v.real_links_writer ())
    {
      unsigned index = (link.position - 2) / 2;
      unsigned base = index / old_class_गणनम्;
      unsigned klass = index % old_class_गणनम्;
      जइ (unlikely (klass >= new_class_गणनम्))
        // should have already been removed
        प्रत्यागम Err(INVALID_ARGUMENT);

      unsigned new_index = base * new_class_गणनम् + klass;

      link.position = (वर्ण*) &(एतत्->matrixZ[new_index]) - (वर्ण*) एतत्;
    }

    प्रत्यागम Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  unsigned start,
                                  unsigned end,
                                  unsigned class_गणनम्)
  {
    unsigned base_गणनम् = rows;
    unsigned new_class_गणनम् = end - start;
    unsigned size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_गणनम् * rows;
    TRY_ASSIGN (unsigned अविभाज्यः_id, c.create_ग्रन्थिः (size));
    AnchorMatrix* अविभाज्यः = (AnchorMatrix*) c.graph.object (अविभाज्यः_id).head;
    अविभाज्यः->rows = base_गणनम्;

    auto& v = c.graph.vertices_[this_index];
    ध्रुव auto& o = v.obj ();

    पूर्णाङ्क अङ्कः_links = o.real_links.length;
    प्रति (पूर्णाङ्क i = 0; i < अङ्कः_links; i++)
    {
      ध्रुव auto& link = o.real_links[i];
      unsigned old_index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = old_index % class_गणनम्;
      जइ (klass < start || klass >= end) अनुवर्त;

      unsigned base = old_index / class_गणनम्;
      unsigned new_klass = klass - start;
      unsigned new_index = base * new_class_गणनम् + new_klass;


      unsigned child_idx = link.objidx;
      TRY (c.graph.add_link (&(अविभाज्यः->matrixZ[new_index]),
                             अविभाज्यः_id,
                             child_idx));

      auto& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      अङ्कः_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    प्रत्यागम Ok(अविभाज्यः_id);
  }
};

संरचना MarkArray : सार्वजनिक OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size();
    unsigned min_size = MarkArray::min_size;
    जइ (unlikely (vertex_len < min_size)) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();

    जइ (unlikely (vertex_len < get_size ())) प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               ध्रुव hb_hashmap_t<unsigned, unsigned>& mark_array_links,
                               unsigned this_index,
                               unsigned new_class_गणनम्)
  {
    auto& v = c.graph.vertices_[this_index];
    ध्रुव auto& o = v.obj ();
    प्रति (ध्रुव auto& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    unsigned new_index = 0;
    प्रति (ध्रुव auto& record : एतत्->iter ())
    {
      unsigned klass = record.klass;
      जइ (klass >= new_class_गणनम्) अनुवर्त;

      (*एतत्)[new_index].klass = klass;
      unsigned position = (वर्ण*) &record.markAnchor - (वर्ण*) एतत्;
      unsigned* objidx;
      जइ (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        अनुवर्त;
      }

      TRY (c.graph.add_link (&(*एतत्)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    एतत्->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    प्रत्यागम Ok();
  }

  graph_result_t<unsigned> clone (gsubgpos_graph_context_t& c,
                                  unsigned this_index,
                                  ध्रुव hb_hashmap_t<unsigned, unsigned>& pos_to_index,
                                  hb_set_t& marks,
                                  unsigned start_class)
  {
    unsigned size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (unsigned अविभाज्यः_id, c.create_ग्रन्थिः (size));
    MarkArray* अविभाज्यः = (MarkArray*) c.graph.object (अविभाज्यः_id).head;
    अविभाज्यः->len = marks.get_population ();


    unsigned i = 0;
    प्रति (hb_codepoint_t mark : marks)
    {
      (*अविभाज्यः)[i].klass = (*एतत्)[mark].klass - start_class;
      unsigned offset_pos = (वर्ण*) &((*एतत्)[mark].markAnchor) - (वर्ण*) एतत्;
      unsigned* anchor_index;
      जइ (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*एतत्)[mark].markAnchor),
                                 अविभाज्यः_id,
                                 &((*अविभाज्यः)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    प्रत्यागम Ok(अविभाज्यः_id);
  }
};

संरचना MarkBasePosFormat1 : सार्वजनिक OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size ();
    जइ (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      प्रत्यागम Err(SANITIZE_FAILURE);
    प्रत्यागम Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (ध्रुव unsigned base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    ध्रुव unsigned base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    unsigned class_गणनम् = class_गणनम्_2;
    TRY_ASSIGN (auto base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_गणनम्));
    unsigned base_गणनम् = base_array.table->rows;

    unsigned partial_coverage_size = 4;
    unsigned accumulated = base_size;
    hb_vector_t<unsigned> split_points;

    प्रति (unsigned klass = 0; klass < class_गणनम्; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      unsigned accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_गणनम्;

      प्रति (unsigned objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).मानम्_or (0);

      accumulated += accumulated_delta;
      unsigned total = accumulated + partial_coverage_size;

      जइ (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<शून्य>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (ध्रुव unsigned mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (auto mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      एतत्,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    प्रत्यागम actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 गुप्त:

  संरचना class_info_t {
    hb_set_t marks;
    hb_vector_t<unsigned> child_indices;
  };

  संरचना split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    unsigned this_index;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<unsigned, unsigned> mark_array_links;

    hb_set_t marks_for (unsigned start, unsigned end)
    {
      hb_set_t marks;
      प्रति (unsigned klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      प्रत्यागम marks;
    }

    unsigned original_गणनम्_2 ()
    {
      प्रत्यागम thiz->class_गणनम्_2;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      प्रत्यागम thiz->clone_range (*एतत्, एतत्->this_index, start, end);
    }

    graph_result_t<शून्य> shrink (unsigned गणनम्)
    {
      प्रत्यागम thiz->shrink (*एतत्, एतत्->this_index, गणनम्);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            unsigned this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    unsigned class_गणनम् = class_गणनम्_2;
    जइ (!class_गणनम्) प्रत्यागम class_to_info;

    जइ (unlikely (!class_to_info.resize (class_गणनम्)))
      प्रत्यागम Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (auto mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    unsigned mark_गणनम् = mark_array.table->len;
    प्रति (unsigned mark = 0; mark < mark_गणनम्; mark++)
    {
      unsigned klass = (*mark_array.table)[mark].get_class ();
      जइ (klass >= class_गणनम्) अनुवर्त;
      class_to_info[klass].marks.add (mark);
    }

    प्रति (ध्रुव auto& link : mark_array.vertex->obj ().real_links)
    {
      unsigned mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      unsigned klass = (*mark_array.table)[mark].get_class ();
      जइ (klass >= class_गणनम्) अनुवर्त;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (unsigned base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    auto& base_array_v = c.graph.vertices_[base_array_id];

    प्रति (ध्रुव auto& link : base_array_v.obj ().real_links)
    {
      unsigned index = (link.position - 2) / OT::Offset16::static_size;
      unsigned klass = index % class_गणनम्;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    प्रत्यागम class_to_info;
  }

  graph_result_t<शून्य> shrink (split_context_t& sc,
                               unsigned this_index,
                               unsigned गणनम्)
  {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               गणनम्);

    unsigned old_गणनम् = class_गणनम्_2;
    जइ (गणनम् >= old_गणनम्)
      प्रत्यागम Ok();

    class_गणनम्_2 = गणनम्;

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, गणनम्);
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
                                                                     old_गणनम्));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_गणनम्,
                                   गणनम्));

    TRY_ASSIGN (auto mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   गणनम्));

    प्रत्यागम Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<unsigned> clone_range (split_context_t& sc,
                                        unsigned this_index,
                                        unsigned start, unsigned end) ध्रुव
  {
    DEBUG_सन्देशः (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    unsigned अविभाज्यः_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (unsigned अविभाज्यः_id, sc.c.create_ग्रन्थिः (अविभाज्यः_size));

    MarkBasePosFormat1* अविभाज्यः = (MarkBasePosFormat1*) graph.object (अविभाज्यः_id).head;
    अविभाज्यः->format = एतत्->format;
    unsigned new_class_गणनम् = end - start;
    अविभाज्यः->class_गणनम्_2 = new_class_गणनम्;

    TRY_ASSIGN (unsigned base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(अविभाज्यः->baseCoverage), अविभाज्यः_id, base_coverage_id));

    TRY_ASSIGN (auto mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    auto new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 अविभाज्यः_id,
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
    TRY (graph.add_link (&(अविभाज्यः->markArray), अविभाज्यः_id, new_mark_array));

    unsigned class_गणनम् = class_गणनम्_2;
    TRY_ASSIGN (auto base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_गणनम्));

    TRY_ASSIGN (unsigned new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, एतत्->class_गणनम्_2));
    TRY (graph.add_link (&(अविभाज्यः->baseArray), अविभाज्यः_id, new_base_array));

    प्रत्यागम Ok(अविभाज्यः_id);
  }
};


संरचना MarkBasePos : सार्वजनिक OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    विकल्प (u.format.v) {
    पक्ष 1:
      hb_barrier ();
      प्रत्यागम ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    स्वभाव:
      प्रत्यागम Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<शून्य> sanitize (ध्रुव graph_t::vertex_t& vertex) ध्रुव
  {
    size_t vertex_len = vertex.table_size ();
    जइ (unlikely (vertex_len < u.format.v.get_size ())) प्रत्यागम Err(SANITIZE_FAILURE);
    hb_barrier ();

    विकल्प (u.format.v) {
    पक्ष 1:
      hb_barrier ();
      प्रत्यागम ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: HB_FALLTHROUGH;
#endif
    स्वभाव:
      // We don't handle format 3 and 4 here.
      प्रत्यागम Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
