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

#अगर_अघोषित GRAPH_MARKBASEPOS_GRAPH_HH
#घोषणा GRAPH_MARKBASEPOS_GRAPH_HH

#समावेश "split-helpers.hh"
#समावेश "coverage-graph.hh"
#समावेश "../OT/Layout/GPOS/MarkBasePos.hh"
#समावेश "../OT/Layout/GPOS/PosLookupSubTable.hh"

namespace graph {

काष्ठा AnchorMatrix : खुला OT::Layout::GPOS_impl::AnchorMatrix<SmallTypes>
{
  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex, अचिन्हित class_गिनती) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार();
    अगर (unlikely (vertex_len < AnchorMatrix::min_आकार)) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();

    अगर (unlikely (vertex_len < AnchorMatrix::min_आकार +
                  OT::Offset16::static_आकार * class_गिनती * यह->rows))
      वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }

  graph_परिणाम_t<व्योम> shrink (gsubgpos_graph_context_t& c,
                               अचिन्हित this_सूचकांक,
                               अचिन्हित old_class_गिनती,
                               अचिन्हित new_class_गिनती)
  {
    अगर (unlikely (new_class_गिनती >= old_class_गिनती)) वापस Err(INVALID_ARGUMENT);
    स्वतः& v = c.graph.vertices_[this_सूचकांक];
    स्थिर स्वतः& o = v.obj ();
    अचिन्हित base_गिनती = rows;
    v.set_बफर (o.शीर्ष_2,
      o.शीर्ष_2 +
      AnchorMatrix::min_आकार +
      OT::Offset16::static_आकार * base_गिनती * new_class_गिनती);

    // Reposition links into the new indexing scheme.
    क्रम (स्वतः& link : v.real_links_writer ())
    {
      अचिन्हित सूचकांक_2 = (link.position - 2) / 2;
      अचिन्हित base = सूचकांक_2 / old_class_गिनती;
      अचिन्हित klass = सूचकांक_2 % old_class_गिनती;
      अगर (unlikely (klass >= new_class_गिनती))
        // should have already been removed
        वापस Err(INVALID_ARGUMENT);

      अचिन्हित new_सूचकांक = base * new_class_गिनती + klass;

      link.position = (अक्षर*) &(यह->matrixZ[new_सूचकांक]) - (अक्षर*) यह;
    }

    वापस Ok();
  }

  graph_परिणाम_t<अचिन्हित> clone (gsubgpos_graph_context_t& c,
                                  अचिन्हित this_सूचकांक,
                                  अचिन्हित start,
                                  अचिन्हित end,
                                  अचिन्हित class_गिनती)
  {
    अचिन्हित base_गिनती = rows;
    अचिन्हित new_class_गिनती = end - start;
    अचिन्हित आकार = AnchorMatrix::min_आकार +
                    OT::Offset16::static_आकार * new_class_गिनती * rows;
    TRY_ASSIGN (अचिन्हित अभाज्य_id, c.create_गाँठ (आकार));
    AnchorMatrix* अभाज्य = (AnchorMatrix*) c.graph.object (अभाज्य_id).शीर्ष_2;
    अभाज्य->rows = base_गिनती;

    स्वतः& v = c.graph.vertices_[this_सूचकांक];
    स्थिर स्वतः& o = v.obj ();

    पूर्णांक अंक_links = o.real_links.लंबाई;
    क्रम (पूर्णांक i = 0; i < अंक_links; i++)
    {
      स्थिर स्वतः& link = o.real_links[i];
      अचिन्हित old_सूचकांक = (link.position - 2) / OT::Offset16::static_आकार;
      अचिन्हित klass = old_सूचकांक % class_गिनती;
      अगर (klass < start || klass >= end) जारी;

      अचिन्हित base = old_सूचकांक / class_गिनती;
      अचिन्हित new_klass = klass - start;
      अचिन्हित new_सूचकांक = base * new_class_गिनती + new_klass;


      अचिन्हित child_idx = link.objidx;
      TRY (c.graph.add_link (&(अभाज्य->matrixZ[new_सूचकांक]),
                             अभाज्य_id,
                             child_idx));

      स्वतः& child = c.graph.vertices_[child_idx];
      child.remove_parent (this_सूचकांक);

      v.remove_real_link_unordered (i);
      अंक_links--;
      i--;
    }

    // Restore correct ordering after the above removes, which mess up link ordering.
    v.sort_real_links ();

    वापस Ok(अभाज्य_id);
  }
};

काष्ठा MarkArray : खुला OT::Layout::GPOS_impl::MarkArray<SmallTypes>
{
  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार();
    अचिन्हित min_आकार = MarkArray::min_आकार;
    अगर (unlikely (vertex_len < min_आकार)) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();

    अगर (unlikely (vertex_len < get_आकार ())) वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }

  graph_परिणाम_t<व्योम> shrink (gsubgpos_graph_context_t& c,
                               स्थिर hb_hashmap_t<अचिन्हित, अचिन्हित>& mark_array_links,
                               अचिन्हित this_सूचकांक,
                               अचिन्हित new_class_गिनती)
  {
    स्वतः& v = c.graph.vertices_[this_सूचकांक];
    स्थिर स्वतः& o = v.obj ();
    क्रम (स्थिर स्वतः& link : o.real_links)
      c.graph.vertices_[link.objidx].remove_parent (this_सूचकांक);
    v.clear_real_links ();

    अचिन्हित new_सूचकांक = 0;
    क्रम (स्थिर स्वतः& record : यह->iter ())
    {
      अचिन्हित klass = record.klass;
      अगर (klass >= new_class_गिनती) जारी;

      (*यह)[new_सूचकांक].klass = klass;
      अचिन्हित position = (अक्षर*) &record.markAnchor - (अक्षर*) यह;
      अचिन्हित* objidx;
      अगर (!mark_array_links.has (position, &objidx))
      {
        new_सूचकांक++;
        जारी;
      }

      TRY (c.graph.add_link (&(*यह)[new_सूचकांक].markAnchor, this_सूचकांक, *objidx));
      new_सूचकांक++;
    }

    यह->len = new_सूचकांक;
    v.set_बफर (o.शीर्ष_2, o.शीर्ष_2 + MarkArray::min_आकार +
                  OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_आकार * new_सूचकांक);

    वापस Ok();
  }

  graph_परिणाम_t<अचिन्हित> clone (gsubgpos_graph_context_t& c,
                                  अचिन्हित this_सूचकांक,
                                  स्थिर hb_hashmap_t<अचिन्हित, अचिन्हित>& pos_to_सूचकांक,
                                  hb_set_t& marks,
                                  अचिन्हित start_class)
  {
    अचिन्हित आकार = MarkArray::min_आकार +
                    OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_आकार *
                    marks.get_population ();
    TRY_ASSIGN (अचिन्हित अभाज्य_id, c.create_गाँठ (आकार));
    MarkArray* अभाज्य = (MarkArray*) c.graph.object (अभाज्य_id).शीर्ष_2;
    अभाज्य->len = marks.get_population ();


    अचिन्हित i = 0;
    क्रम (hb_codepoint_t mark : marks)
    {
      (*अभाज्य)[i].klass = (*यह)[mark].klass - start_class;
      अचिन्हित offset_pos = (अक्षर*) &((*यह)[mark].markAnchor) - (अक्षर*) यह;
      अचिन्हित* anchor_सूचकांक;
      अगर (pos_to_सूचकांक.has (offset_pos, &anchor_सूचकांक))
        TRY (c.graph.move_child (this_सूचकांक,
                                 &((*यह)[mark].markAnchor),
                                 अभाज्य_id,
                                 &((*अभाज्य)[i].markAnchor)));

      i++;
    }

    // Restore correct ordering of the vertex links which are disturbed by move_child.
    c.graph.vertices_[this_सूचकांक].sort_real_links ();

    वापस Ok(अभाज्य_id);
  }
};

काष्ठा MarkBasePosFormat1 : खुला OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>
{
  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार ();
    अगर (unlikely (vertex_len < MarkBasePosFormat1::static_आकार))
      वापस Err(SANITIZE_FAILURE);
    वापस Ok();
  }

  graph_परिणाम_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_सूचकांक)
  {
    hb_set_t visited;

    TRY_ASSIGN (स्थिर अचिन्हित base_coverage_id, c.graph.सूचकांक_for_offset (this_सूचकांक, &baseCoverage));

    स्थिर अचिन्हित base_आकार =
        OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::min_आकार +
        MarkArray::min_आकार +
        AnchorMatrix::min_आकार +
        c.graph.vertices_[base_coverage_id].table_आकार ();

    TRY_ASSIGN (hb_vector_t<class_info_t> class_to_info, get_class_info (c, this_सूचकांक));

    अचिन्हित class_गिनती = class_गिनती_2;
    TRY_ASSIGN (स्वतः base_array, c.graph.as_table<AnchorMatrix> (this_सूचकांक,
                                                          &baseArray,
                                                          class_गिनती));
    अचिन्हित base_गिनती = base_array.table->rows;

    अचिन्हित partial_coverage_आकार = 4;
    अचिन्हित accumulated = base_आकार;
    hb_vector_t<अचिन्हित> split_points;

    क्रम (अचिन्हित klass = 0; klass < class_गिनती; klass++)
    {
      class_info_t& info = class_to_info[klass];
      partial_coverage_आकार += OT::HBUINT16::static_आकार * info.marks.get_population ();
      अचिन्हित accumulated_delta =
          OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_आकार * info.marks.get_population () +
          OT::Offset16::static_आकार * base_गिनती;

      क्रम (अचिन्हित objidx : info.child_indices)
        accumulated_delta += c.graph.find_subgraph_आकार (objidx, visited).मूल्य_or (0);

      accumulated += accumulated_delta;
      अचिन्हित योग = accumulated + partial_coverage_आकार;

      अगर (योग >= (1 << 16))
      {
        split_points.push (klass);
        TRY (graph_परिणाम_t<व्योम>::from (split_points, ALLOCATION_FAILURE));
        accumulated = base_आकार + accumulated_delta;
        partial_coverage_आकार = 4 + OT::HBUINT16::static_आकार * info.marks.get_population ();
        visited.सफा (); // node sharing isn't allowed between splits.
      }
    }


    TRY_ASSIGN (स्थिर अचिन्हित mark_array_id, c.graph.सूचकांक_for_offset (this_सूचकांक, &markArray));
    TRY_ASSIGN (स्वतः mark_array_links, c.graph.vertices_[mark_array_id].position_to_सूचकांक_map ());

    split_context_t split_context {
      c,
      यह,
      this_सूचकांक,
      std::move (class_to_info),
      std::move (mark_array_links),
    };

    वापस actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 निजी:

  काष्ठा class_info_t {
    hb_set_t marks;
    hb_vector_t<अचिन्हित> child_indices;
  };

  काष्ठा split_context_t {
    gsubgpos_graph_context_t& c;
    MarkBasePosFormat1* thiz;
    अचिन्हित this_सूचकांक;
    hb_vector_t<class_info_t> class_to_info;
    hb_hashmap_t<अचिन्हित, अचिन्हित> mark_array_links;

    hb_set_t marks_for (अचिन्हित start, अचिन्हित end)
    {
      hb_set_t marks;
      क्रम (अचिन्हित klass = start; klass < end; klass++)
      {
        + class_to_info[klass].marks.iter ()
        | hb_sink (marks)
        ;
      }
      वापस marks;
    }

    अचिन्हित original_गिनती_2 ()
    {
      वापस thiz->class_गिनती_2;
    }

    graph_परिणाम_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      वापस thiz->clone_range (*यह, यह->this_सूचकांक, start, end);
    }

    graph_परिणाम_t<व्योम> shrink (अचिन्हित गिनती)
    {
      वापस thiz->shrink (*यह, यह->this_सूचकांक, गिनती);
    }
  };

  graph_परिणाम_t<hb_vector_t<class_info_t>> get_class_info (gsubgpos_graph_context_t& c,
                                                            अचिन्हित this_सूचकांक)
  {
    hb_vector_t<class_info_t> class_to_info;

    अचिन्हित class_गिनती = class_गिनती_2;
    अगर (!class_गिनती) वापस class_to_info;

    अगर (unlikely (!class_to_info.resize (class_गिनती)))
      वापस Err(ALLOCATION_FAILURE);

    TRY_ASSIGN (स्वतः mark_array, c.graph.as_table<MarkArray> (this_सूचकांक, &markArray));

    अचिन्हित mark_गिनती = mark_array.table->len;
    क्रम (अचिन्हित mark = 0; mark < mark_गिनती; mark++)
    {
      अचिन्हित klass = (*mark_array.table)[mark].get_class ();
      अगर (klass >= class_गिनती) जारी;
      class_to_info[klass].marks.add (mark);
    }

    क्रम (स्थिर स्वतः& link : mark_array.vertex->obj ().real_links)
    {
      अचिन्हित mark = (link.position - 2) /
                     OT::Layout::GPOS_impl::MarkRecord<SmallTypes>::static_आकार;
      अचिन्हित klass = (*mark_array.table)[mark].get_class ();
      अगर (klass >= class_गिनती) जारी;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    TRY_ASSIGN (अचिन्हित base_array_id,
         c.graph.सूचकांक_for_offset (this_सूचकांक, &baseArray));

    स्वतः& base_array_v = c.graph.vertices_[base_array_id];

    क्रम (स्थिर स्वतः& link : base_array_v.obj ().real_links)
    {
      अचिन्हित सूचकांक_2 = (link.position - 2) / OT::Offset16::static_आकार;
      अचिन्हित klass = सूचकांक_2 % class_गिनती;
      class_to_info[klass].child_indices.push (link.objidx);
    }

    वापस class_to_info;
  }

  graph_परिणाम_t<व्योम> shrink (split_context_t& sc,
                               अचिन्हित this_सूचकांक,
                               अचिन्हित गिनती)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking MarkBasePosFormat1 (%u) to [0, %u).",
               this_सूचकांक,
               गिनती);

    अचिन्हित old_गिनती = class_गिनती_2;
    अगर (गिनती >= old_गिनती)
      वापस Ok();

    class_गिनती_2 = गिनती;

    TRY_ASSIGN (स्वतः mark_coverage, sc.c.graph.as_mutable_table<Coverage> (this_सूचकांक,
                                                                    &markCoverage));

    hb_set_t marks = sc.marks_for (0, गिनती);
    स्वतः new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::make_coverage (sc.c, + new_coverage,
                                  mark_coverage.सूचकांक_2,
                                  4 + 2 * marks.get_population ()));


    TRY_ASSIGN (स्वतः base_array, sc.c.graph.as_mutable_table<AnchorMatrix> (this_सूचकांक,
                                                                     &baseArray,
                                                                     old_गिनती));

    TRY (base_array.table->shrink (sc.c,
                                   base_array.सूचकांक_2,
                                   old_गिनती,
                                   गिनती));

    TRY_ASSIGN (स्वतः mark_array, sc.c.graph.as_mutable_table<MarkArray> (this_सूचकांक,
                                                                  &markArray));

    TRY (mark_array.table->shrink (sc.c,
                                   sc.mark_array_links,
                                   mark_array.सूचकांक_2,
                                   गिनती));

    वापस Ok();
  }

  // Create a new MarkBasePos that has all of the data for classes from [start, end).
  graph_परिणाम_t<अचिन्हित> clone_range (split_context_t& sc,
                                        अचिन्हित this_सूचकांक,
                                        अचिन्हित start, अचिन्हित end) स्थिर
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning MarkBasePosFormat1 (%u) range [%u, %u).", this_सूचकांक, start, end);

    graph_t& graph = sc.c.graph;
    अचिन्हित अभाज्य_आकार = OT::Layout::GPOS_impl::MarkBasePosFormat1_2<SmallTypes>::static_आकार;

    TRY_ASSIGN (अचिन्हित अभाज्य_id, sc.c.create_गाँठ (अभाज्य_आकार));

    MarkBasePosFormat1* अभाज्य = (MarkBasePosFormat1*) graph.object (अभाज्य_id).शीर्ष_2;
    अभाज्य->format = यह->format;
    अचिन्हित new_class_गिनती = end - start;
    अभाज्य->class_गिनती_2 = new_class_गिनती;

    TRY_ASSIGN (अचिन्हित base_coverage_id,
         graph.सूचकांक_for_offset (sc.this_सूचकांक, &baseCoverage));

    TRY (graph.add_link (&(अभाज्य->baseCoverage), अभाज्य_id, base_coverage_id));

    TRY_ASSIGN (स्वतः mark_coverage, sc.c.graph.as_table<Coverage> (this_सूचकांक,
                                                            &markCoverage));

    hb_set_t marks = sc.marks_for (start, end);
    स्वतः new_coverage =
        + hb_enumerate (mark_coverage.table->iter ())
        | hb_filter (marks, hb_first)
        | hb_map_retains_sorting (hb_second)
        ;
    TRY (Coverage::add_coverage (sc.c,
                                 अभाज्य_id,
                                 2,
                                 + new_coverage,
                                 marks.get_population () * 2 + 4));

    TRY_ASSIGN (स्वतः mark_array,
         graph.as_mutable_table <MarkArray> (sc.this_सूचकांक, &markArray));

    TRY_ASSIGN (अचिन्हित new_mark_array,
         mark_array.table->clone (sc.c,
                                  mark_array.सूचकांक_2,
                                  sc.mark_array_links,
                                  marks,
                                  start));
    TRY (graph.add_link (&(अभाज्य->markArray), अभाज्य_id, new_mark_array));

    अचिन्हित class_गिनती = class_गिनती_2;
    TRY_ASSIGN (स्वतः base_array,
         graph.as_mutable_table<AnchorMatrix> (sc.this_सूचकांक, &baseArray, class_गिनती));

    TRY_ASSIGN (अचिन्हित new_base_array,
         base_array.table->clone (sc.c,
                                  base_array.सूचकांक_2,
                                  start, end, यह->class_गिनती_2));
    TRY (graph.add_link (&(अभाज्य->baseArray), अभाज्य_id, new_base_array));

    वापस Ok(अभाज्य_id);
  }
};


काष्ठा MarkBasePos : खुला OT::Layout::GPOS_impl::MarkBasePos
{
  graph_परिणाम_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_सूचकांक)
  {
    चयन (u.format.v) {
    हाल 1:
      hb_barrier ();
      वापस ((MarkBasePosFormat1*)(&u.format1))->split_subtables (c, this_सूचकांक);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 2: HB_FALLTHROUGH;
      // 24-bit MarkBasePos already has widened offsets; leave it unsplit.
#पूर्ण_अगर
    शेष:
      वापस Ok(hb_vector_t<अचिन्हित> ());
    }
  }

  graph_परिणाम_t<व्योम> sanitize (स्थिर graph_t::vertex_t& vertex) स्थिर
  {
    माप_प्रकार vertex_len = vertex.table_आकार ();
    अगर (unlikely (vertex_len < u.format.v.get_आकार ())) वापस Err(SANITIZE_FAILURE);
    hb_barrier ();

    चयन (u.format.v) {
    हाल 1:
      hb_barrier ();
      वापस ((MarkBasePosFormat1*)(&u.format1))->sanitize (vertex);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 2: HB_FALLTHROUGH;
#पूर्ण_अगर
    शेष:
      // We don't handle format 3 and 4 here.
      वापस Err(SANITIZE_FAILURE);
    }
  }
};


}

#पूर्ण_अगर  // GRAPH_MARKBASEPOS_GRAPH_HH
