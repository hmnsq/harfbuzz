
/*
 * Copyright © 2025  Google, Inc.
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

#ifndef GRAPH_LIGATURE_GRAPH_HH
#define GRAPH_LIGATURE_GRAPH_HH

#अन्तर्भाव "graph.hh"
#अन्तर्भाव "../OT/Layout/GSUB/LigatureSubst.hh"
#अन्तर्भाव "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#अन्तर्भाव "../OT/Layout/GSUB/LigatureSet.hh"
#अन्तर्भाव "../hb-ot-dual.hh"
#अन्तर्भाव <algorithm>
#अन्तर्भाव <utility>

नामाकाश graph {

संरचना LigatureSet : सार्वजनिक OT::Layout::GSUB_impl::LigatureSet<SmallTypes>
{
  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    जर (unlikely (vertex_len < OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();

    माप_प्रकार total_len = ligature.get_size() + OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size - ligature.len.get_size();
    जर (unlikely (vertex_len < total_len)) {
      परत Err(SANITIZE_FAILURE);
    }
    परत Ok();
  }
};

संरचना LigatureSubstFormat1 : सार्वजनिक OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  graph_result_t<शून्य> sanitize (स्थिरांक graph_t::vertex_t& vertex) स्थिरांक
  {
    माप_प्रकार vertex_len = vertex.table_size ();
    अचिन्हित min_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    जर (unlikely (vertex_len < min_size)) परत Err(SANITIZE_FAILURE);
    hb_barrier ();

    जर (unlikely (vertex_len < min_size + ligatureSet.get_size() - ligatureSet.len.get_size()))
      परत Err(SANITIZE_FAILURE);
    परत Ok();
  }

  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    TRY_ASSIGN (स्वतः split_points, compute_split_points(c, this_index));
    जर (!split_points)
      परत Ok(hb_vector_t<अचिन्हित> ());

    TRY_ASSIGN (अचिन्हित total_ligas, total_संख्या_ligas(c, this_index));
    TRY_ASSIGN (hb_vector_t<अचिन्हित> counts, liga_counts(c, this_index));
    split_context_t split_context {
      c,
      हे,
      this_index,
      total_ligas,
      std::move (counts),
    };
    परत actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 खाजगी:
  graph_result_t<अचिन्हित> total_संख्या_ligas(gsubgpos_graph_context_t& c, अचिन्हित this_index) स्थिरांक {
    अचिन्हित total = 0;
    साठी (अचिन्हित i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (स्वतः liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      total += liga_set.table->ligature.len;
    }
    परत Ok(total);
  }

  graph_result_t<hb_vector_t<अचिन्हित>> liga_counts(gsubgpos_graph_context_t& c, अचिन्हित this_index) स्थिरांक {
    hb_vector_t<अचिन्हित> result;
    साठी (अचिन्हित i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (स्वतः liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      result.push(!liga_set.table ? 0 : liga_set.table->ligature.len);
    }
    परत graph_result_t<hb_vector_t<अचिन्हित>>::from(std::move(result), ALLOCATION_FAILURE);
  }

  ढाँचा <graph_t::vertex_mutability_t mutability>
  graph_result_t<hb_vector_t<अचिन्हित>> ligature_index_to_object_id(स्थिरांक graph_t::vertex_and_table_t<LigatureSet, mutability>& liga_set) स्थिरांक {
    hb_vector_t<अचिन्हित> map;
    जर (!map.resize_exact(liga_set.table->ligature.len)) परत Err(ALLOCATION_FAILURE);

    साठी (अचिन्हित i = 0; i < map.length; i++) {
      map[i] = HB_GRAPH_INVALID;
    }

    साठी (स्थिरांक स्वतः& l : liga_set.vertex->obj ().real_links) {
      जर (l.position < 2) सुरूठेवा;
      अचिन्हित array_index = (l.position - 2) / 2;
      map[array_index] = l.objidx;
    }
    परत map;
  }

  graph_result_t<hb_vector_t<अचिन्हित>> compute_split_points(gsubgpos_graph_context_t& c,
                                                             अचिन्हित this_index) स्थिरांक
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    स्थिरांक अचिन्हित base_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    अचिन्हित accumulated = base_size;

    अचिन्हित ligature_index = 0;
    hb_vector_t<अचिन्हित> split_points;
    साठी (अचिन्हित i = 0; i < ligatureSet.len; i++)
    {
      accumulated += OT::HBUINT16::static_size; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size; // for ligature set table

      TRY_ASSIGN (स्वतः liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ASSIGN (स्वतः index_to_id, ligature_index_to_object_id (liga_set));

      साठी (अचिन्हित j = 0; j < liga_set.table->ligature.len; j++)
      {
        स्थिरांक अचिन्हित liga_id = index_to_id[j];
        जर (liga_id == HB_GRAPH_INVALID) सुरूठेवा; // no outgoing link, ignore
        स्थिरांक अचिन्हित liga_size = c.graph.vertices_[liga_id].table_size ();

        accumulated += OT::HBUINT16::static_size; // for ligature offset
        accumulated += liga_size; // for the ligature table

        जर (accumulated >= (1 << 16))
        {
          split_points.push(ligature_index);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = base_size + // for liga subst subtable
            (OT::HBUINT16::static_size * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size + // for liga set subtable
            liga_size; // for liga sub table
        }

        ligature_index++;
      }
    }

    परत split_points;
  }

  संरचना split_context_t
  {
    gsubgpos_graph_context_t& c;
    LigatureSubstFormat1* thiz;
    अचिन्हित this_index;
    अचिन्हित original_मोजणी;
    hb_vector_t<अचिन्हित> liga_counts;

    अचिन्हित original_मोजणी_2 ()
    {
      परत original_मोजणी;
    }

    graph_result_t<अचिन्हित> clone_range (अचिन्हित start, अचिन्हित end)
    {
      परत thiz->clone_range (c, this_index, liga_counts, start, end);
    }

    graph_result_t<शून्य> shrink (अचिन्हित मोजणी)
    {
      परत thiz->shrink (c, this_index, original_मोजणी_2(), liga_counts, मोजणी);
    }
  };

  graph_result_t<hb_pair_t<अचिन्हित, LigatureSet*>> new_liga_set(gsubgpos_graph_context_t& c, अचिन्हित मोजणी) स्थिरांक {
    अचिन्हित मूळसंख्या_size = OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size
                          + मोजणी * SmallTypes::size;

    TRY_ASSIGN (अचिन्हित मूळसंख्या_id, c.create_गाठ (मूळसंख्या_size));

    LigatureSet* मूळसंख्या = (LigatureSet*) c.graph.object (मूळसंख्या_id).head;
    मूळसंख्या->ligature.len = मोजणी;
    परत Ok(hb_pair(मूळसंख्या_id, मूळसंख्या));
  }

  शून्य clear_virtual_links (gsubgpos_graph_context_t& c, अचिन्हित गाठ_index) स्थिरांक
  {
    स्वतः& v  = c.graph.vertices_[गाठ_index];
    साठी (स्थिरांक स्वतः& l : v.obj ().virtual_links)
    {
      स्वतः& child = c.graph.vertices_[l.objidx];
      child.remove_parent(गाठ_index);
    }
    v.clear_virtual_links ();
  }

  graph_result_t<शून्य> add_virtual_link(gsubgpos_graph_context_t& c, अचिन्हित from, अचिन्हित to) स्थिरांक {
    जर (unlikely (from >= c.graph.vertices_.length || to >= c.graph.vertices_.length)) परत Err(OUT_OF_BOUNDS);
    TRY(c.graph.vertices_[to].add_parent (from, सत्य));
    TRY(c.graph.vertices_[from].add_virtual_link (to));
    परत Ok();
  }

  hb_pair_t<अचिन्हित, अचिन्हित> current_liga_set_bounds (gsubgpos_graph_context_t& c,
                                                         अचिन्हित liga_set_index,
                                                         स्थिरांक hb_serialize_context_t::object_t& liga_set) स्थिरांक
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    अचिन्हित min_index = HB_GRAPH_INVALID;
    अचिन्हित max_index = 0;
    साठी (स्थिरांक स्वतः& l : liga_set.real_links) {
      जर (l.position < 2) सुरूठेवा;

      अचिन्हित liga_index = (l.position - 2) / 2;
      min_index = hb_min(min_index, liga_index);
      max_index = hb_max(max_index, liga_index);
    }
    परत hb_pair(min_index, max_index + 1);
  }

  शून्य compact_liga_set (gsubgpos_graph_context_t& c, LigatureSet* table, graph_t::vertex_t* v) स्थिरांक
  {
    स्थिरांक स्वतः& obj = v->obj ();
    जर (table->ligature.len <= obj.real_links.length) परत;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    अचिन्हित to_remove = table->ligature.len - obj.real_links.length;
    अचिन्हित new_position = SmallTypes::size;
    v->sort_real_links (); // for this to work we need to process links in order of position.
    साठी (स्वतः& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::size;
    }

    table->ligature.len = obj.real_links.length;
    v->shrink_buffer (to_remove * SmallTypes::size);
  }

  graph_result_t<अचिन्हित> clone_range (gsubgpos_graph_context_t& c,
                                        अचिन्हित this_index,
                                        hb_vector_t<अचिन्हित> liga_counts,
                                        अचिन्हित start, अचिन्हित end) स्थिरांक
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_index, start, end);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    अचिन्हित मूळसंख्या_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size
                          + ligatureSet.get_size() - ligatureSet.len.get_size();

    TRY_ASSIGN (अचिन्हित liga_subst_मूळसंख्या_id, c.create_गाठ (मूळसंख्या_size));

    LigatureSubstFormat1* liga_subst_मूळसंख्या = (LigatureSubstFormat1*) c.graph.object (liga_subst_मूळसंख्या_id).head;
    liga_subst_मूळसंख्या->format = हे->format;
    liga_subst_मूळसंख्या->ligatureSet.len = हे->ligatureSet.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ASSIGN (अचिन्हित coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY_ASSIGN (अचिन्हित coverage_मूळसंख्या_id, c.graph.duplicate(coverage_id));
    स्वतः& coverage_मूळसंख्या_vertex = c.graph.vertices_[coverage_मूळसंख्या_id];

    TRY(c.graph.vertices_[liga_subst_मूळसंख्या_id].add_real_link (SmallTypes::size, coverage_मूळसंख्या_id, 2));
    TRY(coverage_मूळसंख्या_vertex.add_parent (liga_subst_मूळसंख्या_id, असत्य));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    अचिन्हित मोजणी = 0;
    अचिन्हित liga_set_मोजणी = 0;
    अचिन्हित liga_set_start = HB_GRAPH_INVALID;
    अचिन्हित liga_set_end = 0; // inclusive
    साठी (अचिन्हित i = 0; i < liga_counts.length; i++)
    {
      अचिन्हित अंक_ligas = liga_counts[i];

      अचिन्हित current_start = मोजणी;
      अचिन्हित current_end = मोजणी + अंक_ligas;

      जर (current_start >= end || start >= current_end) {
        // No intersection, so just skip
        मोजणी += अंक_ligas;
        सुरूठेवा;
      }

      TRY_ASSIGN (स्वतः liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      अचिन्हित liga_set_index = liga_set.index;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<अचिन्हित, अचिन्हित> liga_bounds = current_liga_set_bounds(c, liga_set_index, liga_set.vertex->obj ());
      current_start = hb_max(मोजणी + liga_bounds.first, current_start);
      current_end = hb_min(मोजणी + liga_bounds.second, current_end);

      अचिन्हित liga_set_मूळसंख्या_id;
      जर (current_start >= start && current_end <= end) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_set_end = i;
        जर (i < liga_set_start) liga_set_start = i;
        TRY_ASSIGN (liga_set_मूळसंख्या_id, c.graph.move_child<> (this_index,
                              &ligatureSet[i],
                              liga_subst_मूळसंख्या_id,
                              &liga_subst_मूळसंख्या->ligatureSet[liga_set_मोजणी++]));
        compact_liga_set (c, liga_set.table, liga_set.vertex);
      }
      अन्यथा
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        अचिन्हित start_index = hb_max(start, current_start) - मोजणी;
        अचिन्हित end_index = hb_min(end, current_end) - मोजणी;
        अचिन्हित liga_मोजणी = end_index - start_index;
        TRY_ASSIGN (स्वतः result, new_liga_set(c, liga_मोजणी));
        liga_set_मूळसंख्या_id = result.first;

        TRY (c.graph.move_children<OT::Offset16>(
          liga_set_index,
          2 + start_index * 2,
          2 + end_index * 2,
          liga_set_मूळसंख्या_id,
          2));

        liga_set_end = i;
        जर (i < liga_set_start) liga_set_start = i;
        TRY (c.graph.add_link(&liga_subst_मूळसंख्या->ligatureSet[liga_set_मोजणी++], liga_subst_मूळसंख्या_id, liga_set_मूळसंख्या_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      स्थिरांक स्वतः& liga_set_मूळसंख्या = c.graph.vertices_[liga_set_मूळसंख्या_id].obj ();
      clear_virtual_links(c, liga_set_मूळसंख्या_id);
      TRY(add_virtual_link(c, liga_set_मूळसंख्या_id, coverage_मूळसंख्या_id));
      साठी (स्थिरांक स्वतः& l : liga_set_मूळसंख्या.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(add_virtual_link(c, l.objidx, coverage_मूळसंख्या_id));
      }

      मोजणी += अंक_ligas;
    }

    c.graph.vertices_[liga_subst_मूळसंख्या_id].shrink_buffer ((liga_subst_मूळसंख्या->ligatureSet.len - liga_set_मोजणी) * SmallTypes::size);
    liga_subst_मूळसंख्या->ligatureSet.len = liga_set_मोजणी;

    TRY (Coverage::filter_coverage (c,
                                    coverage_मूळसंख्या_id,
                                    liga_set_start, liga_set_end + 1));

    परत Ok(liga_subst_मूळसंख्या_id);
  }

  graph_result_t<शून्य> shrink (gsubgpos_graph_context_t& c,
                               अचिन्हित this_index,
                               अचिन्हित old_मोजणी,
                               hb_vector_t<अचिन्हित> liga_counts,
                               अचिन्हित मोजणी)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_index,
               मोजणी);
    जर (मोजणी >= old_मोजणी)
      परत Ok();

    hb_set_t retained_indices;
    अचिन्हित new_liga_set_मोजणी = 0;
    साठी (अचिन्हित i = 0; i < liga_counts.length; i++)
    {
      TRY_ASSIGN (स्वतः liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_set.index);
      retained_indices.add(liga_set.index);

      TRY_ASSIGN (स्वतः index_to_id, ligature_index_to_object_id (liga_set));

      साठी (अचिन्हित i = 0; i < liga_set.table->ligature.len; i++) {
        अचिन्हित liga_index = index_to_id[i];
        जर (liga_index != HB_GRAPH_INVALID) {
          clear_virtual_links(c, liga_index);
          retained_indices.add(liga_index);
        }
      }

      अचिन्हित अंक_ligas = liga_counts[i];
      जर (अंक_ligas >= मोजणी) {
        // drop the trailing liga's from this set and all subsequent liga sets
        अचिन्हित अंक_ligas_to_remove = अंक_ligas - मोजणी;
        new_liga_set_मोजणी = i + 1;
        c.graph.vertices_[liga_set.index].shrink_buffer (अंक_ligas_to_remove * SmallTypes::size);
        liga_set.table->ligature.len = मोजणी;
        थांबा;
      } अन्यथा {
        मोजणी -= अंक_ligas;
      }
    }

    // Adjust liga set array
    स्वतः& this_vertex = c.graph.vertices_[this_index];
    this_vertex.shrink_buffer ((ligatureSet.len - new_liga_set_मोजणी) * SmallTypes::size);
    ligatureSet.len = new_liga_set_मोजणी;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ASSIGN (अचिन्हित coverage_idx, c.graph.index_for_offset (this_index, &हे->coverage));

    स्वतः& coverage_v = c.graph.vertices_[coverage_idx];
    अचिन्हित coverage_size = coverage_v.table_size ();
    स्थिरांक Coverage* coverage_table = (स्थिरांक Coverage*) coverage_v.obj ().head;

    जर (coverage_v.is_shared ())
    {
      TRY_ASSIGN (coverage_idx, c.graph.remap_child (this_index, coverage_idx));
    }

    साठी (अचिन्हित i : retained_indices.iter())
      TRY(add_virtual_link(c, i, coverage_idx));

    स्वतः new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<अचिन्हित, अचिन्हित> p) {
          परत p.second < new_liga_set_मोजणी;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    परत Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_size);
  }
};

संरचना LigatureSubst : सार्वजनिक OT::Layout::GSUB_impl::LigatureSubst
{

  graph_result_t<hb_vector_t<अचिन्हित>> split_subtables (gsubgpos_graph_context_t& c,
                                                         अचिन्हित this_index)
  {
    निवड (u.format.v) {
    स्थिती 1:
      hb_barrier ();
      परत ((LigatureSubstFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    स्थिती 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
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
      परत ((LigatureSubstFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    स्थिती 2:  HB_FALLTHROUGH;
#endif
    पूर्वनिर्धारित:
      // We don't handle format 2 here.
      परत Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_LIGATURE_GRAPH_HH
