
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

#ضمن "graph.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubst.hh"
#ضمن "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#ضمن "../OT/Layout/GSUB/LigatureSet.hh"
#ضمن "../hb-ot-dual.hh"
#ضمن <algorithm>
#ضمن <utility>

نطاق graph {

بنية LigatureSet : عام OT::Layout::GSUB_impl::LigatureSet<SmallTypes>
{
  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    إذا (unlikely (vertex_len < OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t total_len = ligature.get_size() + OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size - ligature.len.get_size();
    إذا (unlikely (vertex_len < total_len)) {
      أرجع Err(SANITIZE_FAILURE);
    }
    أرجع Ok();
  }
};

بنية LigatureSubstFormat1 : عام OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    إذا (unlikely (vertex_len < min_size)) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    إذا (unlikely (vertex_len < min_size + ligatureSet.get_size() - ligatureSet.len.get_size()))
      أرجع Err(SANITIZE_FAILURE);
    أرجع Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    TRY_ASSIGN (auto split_points, compute_split_points(c, this_index));
    إذا (!split_points)
      أرجع Ok(hb_vector_t<unsigned> ());

    TRY_ASSIGN (unsigned total_ligas, total_نومبور_ligas(c, this_index));
    TRY_ASSIGN (hb_vector_t<unsigned> counts, liga_counts(c, this_index));
    split_context_t split_context {
      c,
      هذا,
      this_index,
      total_ligas,
      std::move (counts),
    };
    أرجع actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 خاص:
  graph_result_t<unsigned> total_نومبور_ligas(gsubgpos_graph_context_t& c, unsigned this_index) ثابت {
    unsigned total = 0;
    لكل (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      total += liga_set.table->ligature.len;
    }
    أرجع Ok(total);
  }

  graph_result_t<hb_vector_t<unsigned>> liga_counts(gsubgpos_graph_context_t& c, unsigned this_index) ثابت {
    hb_vector_t<unsigned> result;
    لكل (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      result.push(!liga_set.table ? 0 : liga_set.table->ligature.len);
    }
    أرجع graph_result_t<hb_vector_t<unsigned>>::from(std::move(result), ALLOCATION_FAILURE);
  }

  template <graph_t::vertex_mutability_t mutability>
  graph_result_t<hb_vector_t<unsigned>> ligature_index_to_object_id(ثابت graph_t::vertex_and_table_t<LigatureSet, mutability>& liga_set) ثابت {
    hb_vector_t<unsigned> map;
    إذا (!map.resize_exact(liga_set.table->ligature.len)) أرجع Err(ALLOCATION_FAILURE);

    لكل (unsigned i = 0; i < map.length; i++) {
      map[i] = HB_GRAPH_INVALID;
    }

    لكل (ثابت auto& l : liga_set.vertex->obj ().real_links) {
      إذا (l.position < 2) استمر;
      unsigned array_index = (l.position - 2) / 2;
      map[array_index] = l.objidx;
    }
    أرجع map;
  }

  graph_result_t<hb_vector_t<unsigned>> compute_split_points(gsubgpos_graph_context_t& c,
                                                             unsigned this_index) ثابت
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ثابت unsigned base_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    unsigned accumulated = base_size;

    unsigned ligature_index = 0;
    hb_vector_t<unsigned> split_points;
    لكل (unsigned i = 0; i < ligatureSet.len; i++)
    {
      accumulated += OT::HBUINT16::static_size; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size; // for ligature set table

      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      لكل (unsigned j = 0; j < liga_set.table->ligature.len; j++)
      {
        ثابت unsigned liga_id = index_to_id[j];
        إذا (liga_id == HB_GRAPH_INVALID) استمر; // no outgoing link, ignore
        ثابت unsigned liga_size = c.graph.vertices_[liga_id].table_size ();

        accumulated += OT::HBUINT16::static_size; // for ligature offset
        accumulated += liga_size; // for the ligature table

        إذا (accumulated >= (1 << 16))
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

    أرجع split_points;
  }

  بنية split_context_t
  {
    gsubgpos_graph_context_t& c;
    LigatureSubstFormat1* thiz;
    unsigned this_index;
    unsigned original_کيراءن;
    hb_vector_t<unsigned> liga_counts;

    unsigned original_کيراءن_2 ()
    {
      أرجع original_کيراءن;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      أرجع thiz->clone_range (c, this_index, liga_counts, start, end);
    }

    graph_result_t<فراغ> shrink (unsigned کيراءن)
    {
      أرجع thiz->shrink (c, this_index, original_کيراءن_2(), liga_counts, کيراءن);
    }
  };

  graph_result_t<hb_pair_t<unsigned, LigatureSet*>> new_liga_set(gsubgpos_graph_context_t& c, unsigned کيراءن) ثابت {
    unsigned ڤردان_size = OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size
                          + کيراءن * SmallTypes::size;

    TRY_ASSIGN (unsigned ڤردان_id, c.create_نود (ڤردان_size));

    LigatureSet* ڤردان = (LigatureSet*) c.graph.object (ڤردان_id).head;
    ڤردان->ligature.len = کيراءن;
    أرجع Ok(hb_pair(ڤردان_id, ڤردان));
  }

  فراغ clear_virtual_links (gsubgpos_graph_context_t& c, unsigned نود_index) ثابت
  {
    auto& v  = c.graph.vertices_[نود_index];
    لكل (ثابت auto& l : v.obj ().virtual_links)
    {
      auto& child = c.graph.vertices_[l.objidx];
      child.remove_parent(نود_index);
    }
    v.clear_virtual_links ();
  }

  graph_result_t<فراغ> add_virtual_link(gsubgpos_graph_context_t& c, unsigned from, unsigned to) ثابت {
    إذا (unlikely (from >= c.graph.vertices_.length || to >= c.graph.vertices_.length)) أرجع Err(OUT_OF_BOUNDS);
    TRY(c.graph.vertices_[to].add_parent (from, true));
    TRY(c.graph.vertices_[from].add_virtual_link (to));
    أرجع Ok();
  }

  hb_pair_t<unsigned, unsigned> current_liga_set_bounds (gsubgpos_graph_context_t& c,
                                                         unsigned liga_set_index,
                                                         ثابت hb_serialize_context_t::object_t& liga_set) ثابت
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    unsigned min_index = HB_GRAPH_INVALID;
    unsigned max_index = 0;
    لكل (ثابت auto& l : liga_set.real_links) {
      إذا (l.position < 2) استمر;

      unsigned liga_index = (l.position - 2) / 2;
      min_index = hb_min(min_index, liga_index);
      max_index = hb_max(max_index, liga_index);
    }
    أرجع hb_pair(min_index, max_index + 1);
  }

  فراغ compact_liga_set (gsubgpos_graph_context_t& c, LigatureSet* table, graph_t::vertex_t* v) ثابت
  {
    ثابت auto& obj = v->obj ();
    إذا (table->ligature.len <= obj.real_links.length) أرجع;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    unsigned to_remove = table->ligature.len - obj.real_links.length;
    unsigned new_position = SmallTypes::size;
    v->sort_real_links (); // for this to work we need to process links in order of position.
    لكل (auto& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::size;
    }

    table->ligature.len = obj.real_links.length;
    v->shrink_buffer (to_remove * SmallTypes::size);
  }

  graph_result_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_index,
                                        hb_vector_t<unsigned> liga_counts,
                                        unsigned start, unsigned end) ثابت
  {
    DEBUG_ڤسنن (SUBSET_REPACK, nullptr,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_index, start, end);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    unsigned ڤردان_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size
                          + ligatureSet.get_size() - ligatureSet.len.get_size();

    TRY_ASSIGN (unsigned liga_subst_ڤردان_id, c.create_نود (ڤردان_size));

    LigatureSubstFormat1* liga_subst_ڤردان = (LigatureSubstFormat1*) c.graph.object (liga_subst_ڤردان_id).head;
    liga_subst_ڤردان->format = هذا->format;
    liga_subst_ڤردان->ligatureSet.len = هذا->ligatureSet.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY_ASSIGN (unsigned coverage_ڤردان_id, c.graph.duplicate(coverage_id));
    auto& coverage_ڤردان_vertex = c.graph.vertices_[coverage_ڤردان_id];

    TRY(c.graph.vertices_[liga_subst_ڤردان_id].add_real_link (SmallTypes::size, coverage_ڤردان_id, 2));
    TRY(coverage_ڤردان_vertex.add_parent (liga_subst_ڤردان_id, خطأ));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    unsigned کيراءن = 0;
    unsigned liga_set_کيراءن = 0;
    unsigned liga_set_start = HB_GRAPH_INVALID;
    unsigned liga_set_end = 0; // inclusive
    لكل (unsigned i = 0; i < liga_counts.length; i++)
    {
      unsigned اڠک_ligas = liga_counts[i];

      unsigned current_start = کيراءن;
      unsigned current_end = کيراءن + اڠک_ligas;

      إذا (current_start >= end || start >= current_end) {
        // No intersection, so just skip
        کيراءن += اڠک_ligas;
        استمر;
      }

      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      unsigned liga_set_index = liga_set.index;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<unsigned, unsigned> liga_bounds = current_liga_set_bounds(c, liga_set_index, liga_set.vertex->obj ());
      current_start = hb_max(کيراءن + liga_bounds.first, current_start);
      current_end = hb_min(کيراءن + liga_bounds.second, current_end);

      unsigned liga_set_ڤردان_id;
      إذا (current_start >= start && current_end <= end) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_set_end = i;
        إذا (i < liga_set_start) liga_set_start = i;
        TRY_ASSIGN (liga_set_ڤردان_id, c.graph.move_child<> (this_index,
                              &ligatureSet[i],
                              liga_subst_ڤردان_id,
                              &liga_subst_ڤردان->ligatureSet[liga_set_کيراءن++]));
        compact_liga_set (c, liga_set.table, liga_set.vertex);
      }
      وإلا
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        unsigned start_index = hb_max(start, current_start) - کيراءن;
        unsigned end_index = hb_min(end, current_end) - کيراءن;
        unsigned liga_کيراءن = end_index - start_index;
        TRY_ASSIGN (auto result, new_liga_set(c, liga_کيراءن));
        liga_set_ڤردان_id = result.first;

        TRY (c.graph.move_children<OT::Offset16>(
          liga_set_index,
          2 + start_index * 2,
          2 + end_index * 2,
          liga_set_ڤردان_id,
          2));

        liga_set_end = i;
        إذا (i < liga_set_start) liga_set_start = i;
        TRY (c.graph.add_link(&liga_subst_ڤردان->ligatureSet[liga_set_کيراءن++], liga_subst_ڤردان_id, liga_set_ڤردان_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ثابت auto& liga_set_ڤردان = c.graph.vertices_[liga_set_ڤردان_id].obj ();
      clear_virtual_links(c, liga_set_ڤردان_id);
      TRY(add_virtual_link(c, liga_set_ڤردان_id, coverage_ڤردان_id));
      لكل (ثابت auto& l : liga_set_ڤردان.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(add_virtual_link(c, l.objidx, coverage_ڤردان_id));
      }

      کيراءن += اڠک_ligas;
    }

    c.graph.vertices_[liga_subst_ڤردان_id].shrink_buffer ((liga_subst_ڤردان->ligatureSet.len - liga_set_کيراءن) * SmallTypes::size);
    liga_subst_ڤردان->ligatureSet.len = liga_set_کيراءن;

    TRY (Coverage::filter_coverage (c,
                                    coverage_ڤردان_id,
                                    liga_set_start, liga_set_end + 1));

    أرجع Ok(liga_subst_ڤردان_id);
  }

  graph_result_t<فراغ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_کيراءن,
                               hb_vector_t<unsigned> liga_counts,
                               unsigned کيراءن)
  {
    DEBUG_ڤسنن (SUBSET_REPACK, nullptr,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_index,
               کيراءن);
    إذا (کيراءن >= old_کيراءن)
      أرجع Ok();

    hb_set_t retained_indices;
    unsigned new_liga_set_کيراءن = 0;
    لكل (unsigned i = 0; i < liga_counts.length; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_set.index);
      retained_indices.add(liga_set.index);

      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      لكل (unsigned i = 0; i < liga_set.table->ligature.len; i++) {
        unsigned liga_index = index_to_id[i];
        إذا (liga_index != HB_GRAPH_INVALID) {
          clear_virtual_links(c, liga_index);
          retained_indices.add(liga_index);
        }
      }

      unsigned اڠک_ligas = liga_counts[i];
      إذا (اڠک_ligas >= کيراءن) {
        // drop the trailing liga's from this set and all subsequent liga sets
        unsigned اڠک_ligas_to_remove = اڠک_ligas - کيراءن;
        new_liga_set_کيراءن = i + 1;
        c.graph.vertices_[liga_set.index].shrink_buffer (اڠک_ligas_to_remove * SmallTypes::size);
        liga_set.table->ligature.len = کيراءن;
        اكسر;
      } وإلا {
        کيراءن -= اڠک_ligas;
      }
    }

    // Adjust liga set array
    auto& this_vertex = c.graph.vertices_[this_index];
    this_vertex.shrink_buffer ((ligatureSet.len - new_liga_set_کيراءن) * SmallTypes::size);
    ligatureSet.len = new_liga_set_کيراءن;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ASSIGN (unsigned coverage_idx, c.graph.index_for_offset (this_index, &هذا->coverage));

    auto& coverage_v = c.graph.vertices_[coverage_idx];
    unsigned coverage_size = coverage_v.table_size ();
    ثابت Coverage* coverage_table = (ثابت Coverage*) coverage_v.obj ().head;

    إذا (coverage_v.is_shared ())
    {
      TRY_ASSIGN (coverage_idx, c.graph.remap_child (this_index, coverage_idx));
    }

    لكل (unsigned i : retained_indices.iter())
      TRY(add_virtual_link(c, i, coverage_idx));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          أرجع p.second < new_liga_set_کيراءن;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    أرجع Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_size);
  }
};

بنية LigatureSubst : عام OT::Layout::GSUB_impl::LigatureSubst
{

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    حالة 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#endif
    افتراضي:
      أرجع Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<فراغ> sanitize (ثابت graph_t::vertex_t& vertex) ثابت
  {
    size_t vertex_len = vertex.table_size ();
    إذا (unlikely (vertex_len < u.format.v.get_size ())) أرجع Err(SANITIZE_FAILURE);
    hb_barrier ();

    بدل (u.format.v) {
    حالة 1:
      hb_barrier ();
      أرجع ((LigatureSubstFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    حالة 2:  HB_FALLTHROUGH;
#endif
    افتراضي:
      // We don't handle format 2 here.
      أرجع Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_LIGATURE_GRAPH_HH
