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
  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex, अचिन्हित class_मोजणी) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size();
    जर (unlikely (vertex_len < AnchorMatrix::min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();

    जर (unlikely (vertex_len < AnchorMatrix::min_size +
                  OT::Offset16::static_size * class_मोजणी * हे->rows))
      परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               अचिन्हित this_index,
                               अचिन्हित old_class_मोजणी,
                               अचिन्हित new_class_मोजणी)
  {
    जर (unlikely (new_class_मोजणी >= old_class_मोजणी)) परत Err(INVALID_ARGUMENT);
    स्वतः& v = c.graph.vertices_[this_index];
    स्थिरांक स्वतः& o = v.obj ();
    अचिन्हित base_मोजणी = rows;
    v.set_buffer (o.head,
      o.head +
      AnchorMatrix::min_size +
      OT::Offset16::static_size * base_मोजणी * new_class_मोजणी);

    // Reposition links into the new indexing scheme.
    साठी (स्वतः& link : v.real_links_writer ())
    {
      अचिन्हित index = (link.position - 2) / 2;
      अचिन्हित base = index / old_class_मोजणी;
      अचिन्हित klass = index % old_class_मोजणी;
      जर (unlikely (klass >= new_class_मोजणी))
        // should have already been removed
        परत Err(INVALID_ARGUMENT);

      अचिन्हित new_index = base * new_class_मोजणी + klass;

      link.position = (अक्षर*) &(हे->matrixZ[new_index]) - (अक्षर*) हे;
    }

    परत Ok();
  }

  graph_result_t<अचिन्हित> clone (gsubgpos_graph_context_t& c,
                                  अचिन्हित this_index,
                                  अचिन्हित start,
                                  अचिन्हित end,
                                  अचिन्हित class_मोजणी)
  {
    अचिन्हित base_मोजणी = rows;
    अचिन्हित new_class_मोजणी = end - start;
    अचिन्हित size = AnchorMatrix::min_size +
                    OT::Offset16::static_size * new_class_मोजणी * rows;
    TRY_ASSIGN (अचिन्हित मूळसंख्या_id, c.create_गाठ (size));
    AnchorMatrix* मूळसंख्या = (AnchorMatrix*) c.graph.object (मूळसंख्या_id).head;
    मूळसंख्या->rows = base_मोजणी;

    स्वतः& v = c.graph.vertices_[this_index];
    स्थिरांक स्वतः& o = v.obj ();

    पूर्णांक अंक_links = o.real_links.length;
    साठी (पूर्णांक i = 0; i < अंक_links; i++)
    {
      स्थिरांक स्वतः& link = o.real_links[i];
      अचिन्हित old_index = (link.position - 2) / OT::Offset16::static_size;
      अचिन्हित klass = old_index % class_मोजणी;
      जर (klass < start || klass >= end) सुरूठेवा;

      अचिन्हित base = old_index / class_मोजणी;
      अचिन्हित new_klass = klass - start;
      अचिन्हित new_index = base * new_class_मोजणी + new_klass;


      अचिन्हित child_idx = link.objidx;
      TRY (c.graph.add_link (&(मूळसंख्या->matrixZ[new_index]),
                             मूळसंख्या_id,
                             child_idx));

      स्वतः& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_index);

      v.remove_real_link_unordered (i);
      अंक_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    परत Ok(मूळसंख्या_id);
  }
};

संरचना MarkArray : सार्वजनिक OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size();
    अचिन्हित min_size = MarkArray::min_size;
    जर (unlikely (vertex_len < min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();

    जर (unlikely (vertex_len < get_size ())) परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               स्थिरांक hb_hashmap_t<अचिन्हित, अचिन्हित>& mark_array_links,
                               अचिन्हित this_index,
                               अचिन्हित new_class_मोजणी)
  {
    स्वतः& v = c.graph.vertices_[this_index];
    स्थिरांक स्वतः& o = v.obj ();
    साठी (स्थिरांक स्वतः& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_index);
    v.clear_real_links ();

    अचिन्हित new_index = 0;
    साठी (स्थिरांक स्वतः& record : हे->iter ())
    {
      अचिन्हित klass = record.klass;
      जर (klass >= new_class_मोजणी) सुरूठेवा;

      (*हे)[new_index].klass = klass;
      अचिन्हित position = (अक्षर*) &record.markAnchor - (अक्षर*) हे;
      अचिन्हित* objidx;
      जर (!mark_array_links.has (position, &objidx))
      {
        new_index++;
        सुरूठेवा;
      }

      TRY (c.graph.add_link (&(*हे)[new_index].markAnchor, this_index, *objidx));
      new_index++;
    }

    हे->len = new_index;
    v.set_buffer (o.head, o.head + MarkArray::min_size +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * new_index);

    परत Ok();
  }

  graph_result_t<अचिन्हित> clone (gsubgpos_graph_context_t& c,
                                  अचिन्हित this_index,
                                  स्थिरांक hb_hashmap_t<अचिन्हित, अचिन्हित>& pos_to_index,
                                  hb_set_t& marks,
                                  अचिन्हित start_class)
  {
    अचिन्हित size = MarkArray::min_size +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size *
                    marks.get_population ();
    TRY_ASSIGN (अचिन्हित मूळसंख्या_id, c.create_गाठ (size));
    MarkArray* मूळसंख्या = (MarkArray*) c.graph.object (मूळसंख्या_id).head;
    मूळसंख्या->len = marks.get_population ();


    अचिन्हित i = 0;
    साठी (hb_codepoint_t mark : marks)
    {
      (*मूळसंख्या)[i].klass = (*हे)[mark].klass - start_class;
      अचिन्हित offset_pos = (अक्षर*) &((*हे)[mark].markAnchor) - (अक्षर*) हे;
      अचिन्हित* anchor_index;
      जर (pos_to_index.has (offset_pos, &anchor_index))
        TRY (c.graph.move_child (this_index,
                                 &((*हे)[mark].markAnchor),
                                 मूळसंख्या_id,
                                 &((*मूळसंख्या)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_index].sort_real_links ();

    परत Ok(मूळसंख्या_id);
  }
};

संरचना MarkBasePosFormat1 : सार्वजनिक OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    जर (unlikely (vertex_len < MarkBasePosFormat1::static_size))
      परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    hb_set_t visited;

    TRY_ASSIGN (स्थिरांक अचिन्हित base_coverage_id, c.graph.index_for_offset (this_index, &baseCoverage));

    स्थिरांक अचिन्हित base_size =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_size +
        MarkArray::min_size +
        AnchorMatrix::min_size +
        c.graph.vertices_[base_coverage_id].table_size ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_index));

    अचिन्हित class_मोजणी = class_मोजणी_2;
    TRY_ASSIGN (स्वतः base_array, c.graph.as_table<AnchorMatrix> (this_index,
                                                          &baseArray,
                                                          class_मोजणी));
    अचिन्हित base_मोजणी = base_array.table->rows;

    अचिन्हित partial_coverage_size = 4;
    अचिन्हित accumulated = base_size;
    hb_vector_t<अचिन्हित> split_points;

    साठी (अचिन्हित klass = 0; klass < class_मोजणी; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_size += OT::HBUINT16::static_size * info.marks.get_population ();
      अचिन्हित accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size * info.marks.get_population () +
          OT::Offset16::static_size * base_मोजणी;

      साठी (अचिन्हित objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_size (objidx, visited).मूल्य_or (0);

      accumulated += accumulated_delta;
      अचिन्हित total = accumulated + partial_coverage_size;

      जर (total >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_result_t<शून्य>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_size + accumulated_delta;
        partial_coverage_size = 4 + OT::HBUINT16::static_size * info.marks.get_population ();
        visited.clear (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (स्थिरांक अचिन्हित mark_array_id, c.graph.index_for_offset (this_index, &markArray));
    TRY_ASSIGN (स्वतः mark_array_links, c.graph.vertices_[mark_array_id].position_to_index_map ());

    split_context_t split_context {
      c,
      हे,
      this_index,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    परत actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 खाजगी:

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
      साठी (अचिन्हित klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      परत marks;
    }

    अचिन्हित original_मोजणी_2 ()
    {
      परत thiz->class_मोजणी_2;
    }

    graph_result_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      परत thiz->clone_range (*हे, हे->this_index, start, end);
    }

    graph_result_t<शून्य> shrink (अचिन्हित मोजणी)
    {
      परत thiz->shrink (*हे, हे->this_index, मोजणी);
    }
  };

  graph_result_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            अचिन्हित this_index)
  {
    hb_vector_t<class_info_t> class_to_info;

    अचिन्हित class_मोजणी = class_मोजणी_2;
    जर (!class_मोजणी) परत class_to_info;

    जर (unlikely (!class_to_info.resize (class_मोजणी)))
      परत Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (स्वतः mark_array, c.graph.as_table<MarkArray> (this_index, &markArray));

    अचिन्हित mark_मोजणी = mark_array.table->len;
    साठी (अचिन्हित mark = 0; mark < mark_मोजणी; mark++)
    {
      अचिन्हित klass = (*mark_array.table)[mark].get_class ();
      जर (klass >= class_मोजणी) सुरूठेवा;
      class_to_info[klass].marks.add (mark);
    }

    साठी (स्थिरांक स्वतः& link : mark_array.vertex->obj ().real_links)
    {
      अचिन्हित mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_size;
      अचिन्हित klass = (*mark_array.table)[mark].get_class ();
      जर (klass >= class_मोजणी) सुरूठेवा;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (अचिन्हित base_array_id,
         c.graph.index_for_offset (this_index, &baseArray));

    स्वतः& base_array_v = c.graph.vertices_[base_array_id];

    साठी (स्थिरांक स्वतः& link : base_array_v.obj ().real_links)
    {
      अचिन्हित index = (link.position - 2) / OT::Offset16::static_size;
      अचिन्हित klass = index % class_मोजणी;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    परत class_to_info;
  }

  graph_result_t<शून्य> shrink (split_context_t& sc,
                               अचिन्हित this_index,
                               अचिन्हित मोजणी)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_index,
               मोजणी);

    अचिन्हित old_मोजणी = class_मोजणी_2;
    जर (मोजणी >= old_मोजणी)
      परत Ok();

    class_मोजणी_2 = मोजणी;

    TRY_ASSIGN (स्वतः mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_index,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, मोजणी);
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
                                                                     old_मोजणी));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.index,
                                   old_मोजणी,
                                   मोजणी));

    TRY_ASSIGN (स्वतः mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_index,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.index,
                                   मोजणी));

    परत Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_result_t<अचिन्हित> clone_range (split_context_t& sc,
                                        अचिन्हित this_index,
                                        अचिन्हित start, अचिन्हित end) स्थिरांक
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_index, start, end);

    graph_t& graph = sc.c.graph;
    अचिन्हित मूळसंख्या_size = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_size;

    TRY_ASSIGN (अचिन्हित मूळसंख्या_id, sc.c.create_गाठ (मूळसंख्या_size));

    MarkBasePosFormat1* मूळसंख्या = (MarkBasePosFormat1*) graph.object (मूळसंख्या_id).head;
    मूळसंख्या->format = हे->format;
    अचिन्हित new_class_मोजणी = end - start;
    मूळसंख्या->class_मोजणी_2 = new_class_मोजणी;

    TRY_ASSIGN (अचिन्हित base_coverage_id,
         graph.index_for_offset (sc.this_index, &baseCoverage));

    TRY (graph.add_link (&(मूळसंख्या->baseCoverage), मूळसंख्या_id, base_coverage_id));

    TRY_ASSIGN (स्वतः mark_coverage, sc.c.graph.as_table<Coverage> (this_index,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    स्वतः new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 मूळसंख्या_id,
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
    TRY (graph.add_link (&(मूळसंख्या->markArray), मूळसंख्या_id, new_mark_array));

    अचिन्हित class_मोजणी = class_मोजणी_2;
    TRY_ASSIGN (स्वतः base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_index, &baseArray, class_मोजणी));

    TRY_ASSIGN (अचिन्हित new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.index,
                                  start, end, हे->class_मोजणी_2));
    TRY (graph.add_link (&(मूळसंख्या->baseArray), मूळसंख्या_id, new_base_array));

    परत Ok(मूळसंख्या_id);
  }
};


संरचना MarkBasePos : सार्वजनिक OT::Layout::GPOS_impl::MarkBasePos
{
  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    निवड (u.format.v) {
    स्थिती 1:
      hb_barrier ();
      परत ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    स्थिती 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#endif
    पूर्वनिर्धारित:
      परत Ok(hb_vector_t<अचिन्हित> ());
    }
  }

  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    जर (unlikely (vertex_len < u.format.v.get_size ())) परत Err(SANITIZE_FAILURE);
    hb_barrier ();

    निवड (u.format.v) {
    स्थिती 1:
      hb_barrier ();
      परत ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    स्थिती 2: HB_FALLTHROUGH;
#endif
    पूर्वनिर्धारित:
      // We don't handle format 3 and 4 here.
      परत Err(SANITIZE_FAILURE);
    }
  }
};


}

#endif  // GRAPH_MARKBASEPOS_GRAPH_HH
