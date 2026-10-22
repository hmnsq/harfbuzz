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
  graph_result_t<शून्य> sanitize (अचर graph_t::vertex_t& vertex, अचिन्हित class_गन्ती) अचर
  {
    माप_प्रकार vertex_len = vertex.table_size();
    यदि (unlikely (vertex_len < AnchorMatrix::min_size)) फर्काऊ Err(SANITIZE_FAILURE);
    hb_barrier ();

    यदि (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_गन्ती * एतत्->rows))
      फर्काऊ Err(SANITIZE_FAILURE);
    फर्काऊ Ok();
  }

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               अचिन्हित this_index,
                               अचिन्हित old_class_गन्ती,
                               अचिन्हित new_class_गन्ती)
  {
    यदि (unlikely (new_class_गन्ती >= old_class_गन्ती)) फर्काऊ Err(INVALID_ARGUMENT);
    स्वतः& v = c.graph.vertices_[this_index];
    अचर स्वतः& o = v.obj ();
    अचिन्हित base_गन्ती = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_गन्ती * new_class_गन्ती);

    // Reposition links into the new indexing scheme.
    लागि (स्वतः& link : v.real_links_writer ())
    {
      अचिन्हित index = (link.position - 2) / 2;
      अचिन्हित base = index / old_class_गन्ती;
      अचिन्हित klass = index % old_class_गन्ती;
      यदि (unlikely (klass >= new_class_गन्ती))
        // should have already been removed
        फर्काऊ Err(INVALID_ARGUMENT);

      अचिन्हित new_index = base * new_class_गन्ती + klass;

      link.position = (वर्ण*) &(एतत्->matrixZ[new_index]) - (वर्ण*) एतत्;
    }

    फर्काऊ Ok();
  }

  graph_result_t<अचिन्हित> clone (gsubgpos_graph_context_t& c,
                                  अचिन्हित this_index,
                                  अचिन्हित start,
                                  अचिन्हित end,
                                  अचिन्हित class_गन्ती)
  {
    अचिन्हित base_गन्ती = rows;
    अचिन्हित new_class_गन्ती = end - start;
    अचिन्हित size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_गन्ती * rows;
    TRY_ASSIGN (अचिन्हित अविभाज्य_id, c.create_गाँठो (size));
    AnchorMatrix* अविभाज्य = (AnchorMatrix*) c.graph.object (अविभाज्य_id).head;
    अविभाज्य->rows = base_गन्ती;

    स्वतः& v = c.graph.vertices_[this_index];
    अचर स्वतः& o = v.obj ();

    पूर्णाङ्क अङ्क_links = o.real_links.length;
    लागि (पूर्णाङ्क i = 0; i < अङ्क_links; i++)
    {
      अचर स्वतः& link = o.real_links[i];
      अचिन्हित old_index = (link.position - 2) / OT::Offset16::static_size;
      अचिन्हित klass = old_index % class_गन्ती;
      यदि (klass < start || klass >= end) जारी;

      अचिन्हित base = old_index / class_गन्ती;
      अचिन्हित new_klass = klass - start;
      अचिन्हित new_index = base * new_class_गन्ती + new_klass;


      अचिन्हित child_idx = link.objidx;
      TRY (c.graph.add_link (&(अविभाज्य->matrixZ[new_index]),
                             अविभाज्य_id,
                             child_idx));

      स्वतः& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      अङ्क_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    फर्काऊ Ok(अविभाज्य_id);
  }
};

संरचना MarkArray : सार्वजनिक OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<शून्य> sanitize (अचर graph_t::vertex_t& vertex) अचर
  {
    माप_प्रकार vertex_len = vertex.table_size();
    अचिन्हित min_size = MarkArray::min_size;
    यदि (unlikely (vertex_len < min_size)) फर्काऊ Err(SANITIZE_FAILURE);
    hb_barrier ();

    यदि (unlikely (vertex_len < get_size ())) फर्काऊ Err(SANITIZE_FAILURE);
    फर्काऊ Ok();
  }

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               अचर hb_hashmap_t<अचिन्हित, अचिन्हित>& mark_array_links,
                               अचिन्हित this_index,
                               अचिन्हित new_class_गन्ती)
  {
    स्वतः& v = c.graph.vertices_[this_index];
    अचर स्वतः& o = v.obj ();
    लागि (अचर स्वतः& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    अचिन्हित new_index = 0;
    लागि (अचर स्वतः& record : एतत्->iter ())
    {
      अचिन्हित klass = record.klass;
      यदि (klass >= new_class_गन्ती) जारी;

      (*एतत्)[new_index].klass = klass;
      अचिन्हित position = (वर्ण*) &record.markAnchor - (वर्ण*) एतत्;
      अचिन्हित* objidx;
      यदि (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        जारी;
      }

      TRY (c.graph.add_link (&(*एतत्)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    एतत्->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    फर्काऊ Ok();
  }

  graph_result_t<अचिन्हित> clone (gsubgpos_graph_context_t& c,
                                  अचिन्हित this_index,
                                  अचर hb_hashmap_t<अचिन्हित, अचिन्हित>& pos_to_index,
                                  hb_set_t& marks,
                                  अचिन्हित start_class)
  {
    अचिन्हित size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (अचिन्हित अविभाज्य_id, c.create_गाँठो (size));
    MarkArray* अविभाज्य = (MarkArray*) c.graph.object (अविभाज्य_id).head;
    अविभाज्य->len = marks.get_population ();


    अचिन्हित i = 0;
    लागि (hb_codepoint_t mark : marks)
    {
      (*अविभाज्य)[i].klass = (*एतत्)[mark].klass - start_class;
      अचिन्हित offset_pos = (वर्ण*) &((*एतत्)[mark].markAnchor) - (वर्ण*) एतत्;
      अचिन्हित* anchor_index;
      यदि (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*एतत्)[mark].markAnchor),
                                 अविभाज्य_id,
                                 &((*अविभाज्य)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    फर्काऊ Ok(अविभाज्य_id);
  }
};

संरचना MarkBasePosFormat1 : सार्वजनिक OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<शून्य> sanitize (अचर graph_t::vertex_t& vertex) अचर
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    यदि (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      फर्काऊ Err(SANITIZE_FAILURE);
    फर्काऊ Ok();
  }

  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (अचर अचिन्हित base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    अचर अचिन्हित base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    अचिन्हित class_गन्ती = class_गन्ती_2;
    TRY_ASSIGN (स्वतः base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_गन्ती));
    अचिन्हित base_गन्ती = base_array.table->rows;

    अचिन्हित partial_coverage_size = 4;
    अचिन्हित accumulated = base_size;
    hb_vector_t<अचिन्हित> split_points;

    लागि (अचिन्हित klass = 0; klass < class_गन्ती; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      अचिन्हित accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_गन्ती;

      लागि (अचिन्हित objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).मान_or (0);

      accumulated += accumulated_delta;
      अचिन्हित total = accumulated + partial_coverage_size;

      यदि (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<शून्य>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (अचर अचिन्हित mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (स्वतः mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      एतत्,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    फर्काऊ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 गुप्त:

  संरचना class_info_t {
    hb_set_t marks;
    hb_vector_t<अचिन्हित> child_indices;
  };

  संरचना split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    अचिन्हित this_index;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<अचिन्हित, अचिन्हित> mark_array_links;

    hb_set_t marks_for (अचिन्हित start, अचिन्हित end)
    {
      hb_set_t marks;
      लागि (अचिन्हित klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      फर्काऊ marks;
    }

    अचिन्हित original_गन्ती_2 ()
    {
      फर्काऊ thiz->class_गन्ती_2;
    }

    graph_result_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      फर्काऊ thiz->clone_range (*एतत्, एतत्->this_index, start, end);
    }

    graph_result_t<शून्य> shrink (अचिन्हित गन्ती)
    {
      फर्काऊ thiz->shrink (*एतत्, एतत्->this_index, गन्ती);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            अचिन्हित this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    अचिन्हित class_गन्ती = class_गन्ती_2;
    यदि (!class_गन्ती) फर्काऊ class_to_info;

    यदि (unlikely (!class_to_info.resize (class_गन्ती)))
      फर्काऊ Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (स्वतः mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    अचिन्हित mark_गन्ती = mark_array.table->len;
    लागि (अचिन्हित mark = 0; mark < mark_गन्ती; mark++)
    {
      अचिन्हित klass = (*mark_array.table)[mark].get_class ();
      यदि (klass >= class_गन्ती) जारी;
      class_to_info[klass].marks.add (mark);
    }

    लागि (अचर स्वतः& link : mark_array.vertex->obj ().real_links)
    {
      अचिन्हित mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      अचिन्हित klass = (*mark_array.table)[mark].get_class ();
      यदि (klass >= class_गन्ती) जारी;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (अचिन्हित base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    स्वतः& base_array_v = c.graph.vertices_[base_array_id];

    लागि (अचर स्वतः& link : base_array_v.obj ().real_links)
    {
      अचिन्हित index = (link.position - 2) / OT::Offset16::static_size;
      अचिन्हित klass = index % class_गन्ती;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    फर्काऊ class_to_info;
  }

  graph_result_t<शून्य> shrink (split_context_t& sc,
                               अचिन्हित this_index,
                               अचिन्हित गन्ती)
  {
    DEBUG_सन्देश (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               गन्ती);

    अचिन्हित old_गन्ती = class_गन्ती_2;
    यदि (गन्ती >= old_गन्ती)
      फर्काऊ Ok();

    class_गन्ती_2 = गन्ती;

    TRY_ASSIGN (स्वतः mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, गन्ती);
    स्वतः new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.index,
                                  4 + 2 * marks.get_population ()));


    TRY_ASSIGN (स्वतः base_array, sc.c.graph.as_mutable_table<AnchorMatrix> (this_index,
                                                                     &baseArray,
                                                                     old_गन्ती));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_गन्ती,
                                   गन्ती));

    TRY_ASSIGN (स्वतः mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   गन्ती));

    फर्काऊ Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<अचिन्हित> clone_range (split_context_t& sc,
                                        अचिन्हित this_index,
                                        अचिन्हित start, अचिन्हित end) अचर
  {
    DEBUG_सन्देश (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    अचिन्हित अविभाज्य_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (अचिन्हित अविभाज्य_id, sc.c.create_गाँठो (अविभाज्य_size));

    MarkBasePosFormat1* अविभाज्य = (MarkBasePosFormat1*) graph.object (अविभाज्य_id).head;
    अविभाज्य->format = एतत्->format;
    अचिन्हित new_class_गन्ती = end - start;
    अविभाज्य->class_गन्ती_2 = new_class_गन्ती;

    TRY_ASSIGN (अचिन्हित base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(अविभाज्य->baseCoverage), अविभाज्य_id, base_coverage_id));

    TRY_ASSIGN (स्वतः mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    स्वतः new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 अविभाज्य_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_ASSIGN (स्वतः mark_array,
         graph.as_mutable_table <MarkArray> (sc.this_index, &markArray));

    TRY_ASSIGN (अचिन्हित new_mark_array,
         mark_array.table->clone (sc.c,
                                  mark_array.index,
                                  sc.mark_array_links,
                                  marks,
                                  start));
    TRY (graph.add_link (&(अविभाज्य->markArray), अविभाज्य_id, new_mark_array));

    अचिन्हित class_गन्ती = class_गन्ती_2;
    TRY_ASSIGN (स्वतः base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_गन्ती));

    TRY_ASSIGN (अचिन्हित new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, एतत्->class_गन्ती_2));
    TRY (graph.add_link (&(अविभाज्य->baseArray), अविभाज्य_id, new_base_array));

    फर्काऊ Ok(अविभाज्य_id);
  }
};


संरचना MarkBasePos : सार्वजनिक OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    विकल्प (u.format.v) {
    पक्ष 1:
      hb_barrier ();
      फर्काऊ ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    स्वभाव:
      फर्काऊ Ok(hb_vector_t<अचिन्हित> ());
    }
  }

  graph_result_t<शून्य> sanitize (अचर graph_t::vertex_t& vertex) अचर
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    यदि (unlikely (vertex_len < u.format.v.get_size ())) फर्काऊ Err(SANITIZE_FAILURE);
    hb_barrier ();

    विकल्प (u.format.v) {
    पक्ष 1:
      hb_barrier ();
      फर्काऊ ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: HB_FALLTHROUGH;
#endif
    स्वभाव:
      // We don't handle format 3 and 4 here.
      फर्काऊ Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
