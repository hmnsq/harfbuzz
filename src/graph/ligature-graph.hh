
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

#ಅನ್ತರ್ಭಾವ "graph.hh"
#ಅನ್ತರ್ಭಾವ "../OT/Layout/GSUB/LigatureSubst.hh"
#ಅನ್ತರ್ಭಾವ "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#ಅನ್ತರ್ಭಾವ "../OT/Layout/GSUB/LigatureSet.hh"
#ಅನ್ತರ್ಭಾವ "../hb-ot-dual.hh"
#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <utility>

ನಾಮಾಕಾಶ graph {

ಸಂರಚನಾ LigatureSet : ಸಾರ್ವಜನಿಕ OT::Layout::GSUB_impl::LigatureSet<SmallTypes>
{
  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size ();
    ಯದಿ (unlikely (vertex_len < OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size)) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t total_len = ligature.get_size() + OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size - ligature.len.get_size();
    ಯದಿ (unlikely (vertex_len < total_len)) {
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    }
    ಪ್ರತ್ಯಾಗಮ Ok();
  }
};

ಸಂರಚನಾ LigatureSubstFormat1 : ಸಾರ್ವಜನಿಕ OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    ಯದಿ (unlikely (vertex_len < min_size)) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ಯದಿ (unlikely (vertex_len < min_size + ligatureSet.get_size() - ligatureSet.len.get_size()))
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    TRY_ASSIGN (auto split_points, compute_split_points(c, this_index));
    ಯದಿ (!split_points)
      ಪ್ರತ್ಯಾಗಮ Ok(hb_vector_t<unsigned> ());

    TRY_ASSIGN (unsigned total_ligas, total_ಸಂಖ್ಯೆ_ligas(c, this_index));
    TRY_ASSIGN (hb_vector_t<unsigned> counts, liga_counts(c, this_index));
    split_context_t split_context {
      c,
      ಏತತ್,
      this_index,
      total_ligas,
      std::move (counts),
    };
    ಪ್ರತ್ಯಾಗಮ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ಗುಪ್ತ:
  graph_result_t<unsigned> total_ಸಂಖ್ಯೆ_ligas(gsubgpos_graph_context_t& c, unsigned this_index) ಧ್ರುವ {
    unsigned total = 0;
    ಪ್ರತಿ (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      total += liga_set.table->ligature.len;
    }
    ಪ್ರತ್ಯಾಗಮ Ok(total);
  }

  graph_result_t<hb_vector_t<unsigned>> liga_counts(gsubgpos_graph_context_t& c, unsigned this_index) ಧ್ರುವ {
    hb_vector_t<unsigned> result;
    ಪ್ರತಿ (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      result.push(!liga_set.table ? 0 : liga_set.table->ligature.len);
    }
    ಪ್ರತ್ಯಾಗಮ graph_result_t<hb_vector_t<unsigned>>::from(std::move(result), ALLOCATION_FAILURE);
  }

  template <graph_t::vertex_mutability_t mutability>
  graph_result_t<hb_vector_t<unsigned>> ligature_index_to_object_id(ಧ್ರುವ graph_t::vertex_and_table_t<LigatureSet, mutability>& liga_set) ಧ್ರುವ {
    hb_vector_t<unsigned> map;
    ಯದಿ (!map.resize_exact(liga_set.table->ligature.len)) ಪ್ರತ್ಯಾಗಮ Err(ALLOCATION_FAILURE);

    ಪ್ರತಿ (unsigned i = 0; i < map.length; i++) {
      map[i] = HB_GRAPH_INVALID;
    }

    ಪ್ರತಿ (ಧ್ರುವ auto& l : liga_set.vertex->obj ().real_links) {
      ಯದಿ (l.position < 2) ಅನುವರ್ತ;
      unsigned array_index = (l.position - 2) / 2;
      map[array_index] = l.objidx;
    }
    ಪ್ರತ್ಯಾಗಮ map;
  }

  graph_result_t<hb_vector_t<unsigned>> compute_split_points(gsubgpos_graph_context_t& c,
                                                             unsigned this_index) ಧ್ರುವ
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ಧ್ರುವ unsigned base_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    unsigned accumulated = base_size;

    unsigned ligature_index = 0;
    hb_vector_t<unsigned> split_points;
    ಪ್ರತಿ (unsigned i = 0; i < ligatureSet.len; i++)
    {
      accumulated += OT::HBUINT16::static_size; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size; // for ligature set table

      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      ಪ್ರತಿ (unsigned j = 0; j < liga_set.table->ligature.len; j++)
      {
        ಧ್ರುವ unsigned liga_id = index_to_id[j];
        ಯದಿ (liga_id == HB_GRAPH_INVALID) ಅನುವರ್ತ; // no outgoing link, ignore
        ಧ್ರುವ unsigned liga_size = c.graph.vertices_[liga_id].table_size ();

        accumulated += OT::HBUINT16::static_size; // for ligature offset
        accumulated += liga_size; // for the ligature table

        ಯದಿ (accumulated >= (1 << 16))
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

    ಪ್ರತ್ಯಾಗಮ split_points;
  }

  ಸಂರಚನಾ split_context_t
  {
    gsubgpos_graph_context_t& c;
    LigatureSubstFormat1* thiz;
    unsigned this_index;
    unsigned original_ಎಣಿಕೆ;
    hb_vector_t<unsigned> liga_counts;

    unsigned original_ಎಣಿಕೆ_2 ()
    {
      ಪ್ರತ್ಯಾಗಮ original_ಎಣಿಕೆ;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ಪ್ರತ್ಯಾಗಮ thiz->clone_range (c, this_index, liga_counts, start, end);
    }

    graph_result_t<ಶೂನ್ಯ> shrink (unsigned ಎಣಿಕೆ)
    {
      ಪ್ರತ್ಯಾಗಮ thiz->shrink (c, this_index, original_ಎಣಿಕೆ_2(), liga_counts, ಎಣಿಕೆ);
    }
  };

  graph_result_t<hb_pair_t<unsigned, LigatureSet*>> new_liga_set(gsubgpos_graph_context_t& c, unsigned ಎಣಿಕೆ) ಧ್ರುವ {
    unsigned ಅವಿಭಾಜ್ಯ_size = OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size
                          + ಎಣಿಕೆ * SmallTypes::size;

    TRY_ASSIGN (unsigned ಅವಿಭಾಜ್ಯ_id, c.create_ಗಂಟು (ಅವಿಭಾಜ್ಯ_size));

    LigatureSet* ಅವಿಭಾಜ್ಯ = (LigatureSet*) c.graph.object (ಅವಿಭಾಜ್ಯ_id).head;
    ಅವಿಭಾಜ್ಯ->ligature.len = ಎಣಿಕೆ;
    ಪ್ರತ್ಯಾಗಮ Ok(hb_pair(ಅವಿಭಾಜ್ಯ_id, ಅವಿಭಾಜ್ಯ));
  }

  ಶೂನ್ಯ clear_virtual_links (gsubgpos_graph_context_t& c, unsigned ಗಂಟು_index) ಧ್ರುವ
  {
    auto& v  = c.graph.vertices_[ಗಂಟು_index];
    ಪ್ರತಿ (ಧ್ರುವ auto& l : v.obj ().virtual_links)
    {
      auto& child = c.graph.vertices_[l.objidx];
      child.remove_parent(ಗಂಟು_index);
    }
    v.clear_virtual_links ();
  }

  graph_result_t<ಶೂನ್ಯ> add_virtual_link(gsubgpos_graph_context_t& c, unsigned from, unsigned to) ಧ್ರುವ {
    ಯದಿ (unlikely (from >= c.graph.vertices_.length || to >= c.graph.vertices_.length)) ಪ್ರತ್ಯಾಗಮ Err(OUT_OF_BOUNDS);
    TRY(c.graph.vertices_[to].add_parent (from, ಸತ್ಯ));
    TRY(c.graph.vertices_[from].add_virtual_link (to));
    ಪ್ರತ್ಯಾಗಮ Ok();
  }

  hb_pair_t<unsigned, unsigned> current_liga_set_bounds (gsubgpos_graph_context_t& c,
                                                         unsigned liga_set_index,
                                                         ಧ್ರುವ hb_serialize_context_t::object_t& liga_set) ಧ್ರುವ
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    unsigned min_index = HB_GRAPH_INVALID;
    unsigned max_index = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& l : liga_set.real_links) {
      ಯದಿ (l.position < 2) ಅನುವರ್ತ;

      unsigned liga_index = (l.position - 2) / 2;
      min_index = hb_min(min_index, liga_index);
      max_index = hb_max(max_index, liga_index);
    }
    ಪ್ರತ್ಯಾಗಮ hb_pair(min_index, max_index + 1);
  }

  ಶೂನ್ಯ compact_liga_set (gsubgpos_graph_context_t& c, LigatureSet* table, graph_t::vertex_t* v) ಧ್ರುವ
  {
    ಧ್ರುವ auto& obj = v->obj ();
    ಯದಿ (table->ligature.len <= obj.real_links.length) ಪ್ರತ್ಯಾಗಮ;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    unsigned to_remove = table->ligature.len - obj.real_links.length;
    unsigned new_position = SmallTypes::size;
    v->sort_real_links (); // for this to work we need to process links in order of position.
    ಪ್ರತಿ (auto& l : v->real_links_writer ())
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
                                        unsigned start, unsigned end) ಧ್ರುವ
  {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_index, start, end);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    unsigned ಅವಿಭಾಜ್ಯ_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size
                          + ligatureSet.get_size() - ligatureSet.len.get_size();

    TRY_ASSIGN (unsigned liga_subst_ಅವಿಭಾಜ್ಯ_id, c.create_ಗಂಟು (ಅವಿಭಾಜ್ಯ_size));

    LigatureSubstFormat1* liga_subst_ಅವಿಭಾಜ್ಯ = (LigatureSubstFormat1*) c.graph.object (liga_subst_ಅವಿಭಾಜ್ಯ_id).head;
    liga_subst_ಅವಿಭಾಜ್ಯ->format = ಏತತ್->format;
    liga_subst_ಅವಿಭಾಜ್ಯ->ligatureSet.len = ಏತತ್->ligatureSet.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY_ASSIGN (unsigned coverage_ಅವಿಭಾಜ್ಯ_id, c.graph.duplicate(coverage_id));
    auto& coverage_ಅವಿಭಾಜ್ಯ_vertex = c.graph.vertices_[coverage_ಅವಿಭಾಜ್ಯ_id];

    TRY(c.graph.vertices_[liga_subst_ಅವಿಭಾಜ್ಯ_id].add_real_link (SmallTypes::size, coverage_ಅವಿಭಾಜ್ಯ_id, 2));
    TRY(coverage_ಅವಿಭಾಜ್ಯ_vertex.add_parent (liga_subst_ಅವಿಭಾಜ್ಯ_id, ಅಸತ್ಯ));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    unsigned ಎಣಿಕೆ = 0;
    unsigned liga_set_ಎಣಿಕೆ = 0;
    unsigned liga_set_start = HB_GRAPH_INVALID;
    unsigned liga_set_end = 0; // inclusive
    ಪ್ರತಿ (unsigned i = 0; i < liga_counts.length; i++)
    {
      unsigned ಅಂಕೆ_ligas = liga_counts[i];

      unsigned current_start = ಎಣಿಕೆ;
      unsigned current_end = ಎಣಿಕೆ + ಅಂಕೆ_ligas;

      ಯದಿ (current_start >= end || start >= current_end) {
        // No intersection, so just skip
        ಎಣಿಕೆ += ಅಂಕೆ_ligas;
        ಅನುವರ್ತ;
      }

      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      unsigned liga_set_index = liga_set.index;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<unsigned, unsigned> liga_bounds = current_liga_set_bounds(c, liga_set_index, liga_set.vertex->obj ());
      current_start = hb_max(ಎಣಿಕೆ + liga_bounds.first, current_start);
      current_end = hb_min(ಎಣಿಕೆ + liga_bounds.second, current_end);

      unsigned liga_set_ಅವಿಭಾಜ್ಯ_id;
      ಯದಿ (current_start >= start && current_end <= end) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_set_end = i;
        ಯದಿ (i < liga_set_start) liga_set_start = i;
        TRY_ASSIGN (liga_set_ಅವಿಭಾಜ್ಯ_id, c.graph.move_child<> (this_index,
                              &ligatureSet[i],
                              liga_subst_ಅವಿಭಾಜ್ಯ_id,
                              &liga_subst_ಅವಿಭಾಜ್ಯ->ligatureSet[liga_set_ಎಣಿಕೆ++]));
        compact_liga_set (c, liga_set.table, liga_set.vertex);
      }
      ಅನ್ಯಥಾ
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        unsigned start_index = hb_max(start, current_start) - ಎಣಿಕೆ;
        unsigned end_index = hb_min(end, current_end) - ಎಣಿಕೆ;
        unsigned liga_ಎಣಿಕೆ = end_index - start_index;
        TRY_ASSIGN (auto result, new_liga_set(c, liga_ಎಣಿಕೆ));
        liga_set_ಅವಿಭಾಜ್ಯ_id = result.first;

        TRY (c.graph.move_children<OT::Offset16>(
          liga_set_index,
          2 + start_index * 2,
          2 + end_index * 2,
          liga_set_ಅವಿಭಾಜ್ಯ_id,
          2));

        liga_set_end = i;
        ಯದಿ (i < liga_set_start) liga_set_start = i;
        TRY (c.graph.add_link(&liga_subst_ಅವಿಭಾಜ್ಯ->ligatureSet[liga_set_ಎಣಿಕೆ++], liga_subst_ಅವಿಭಾಜ್ಯ_id, liga_set_ಅವಿಭಾಜ್ಯ_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ಧ್ರುವ auto& liga_set_ಅವಿಭಾಜ್ಯ = c.graph.vertices_[liga_set_ಅವಿಭಾಜ್ಯ_id].obj ();
      clear_virtual_links(c, liga_set_ಅವಿಭಾಜ್ಯ_id);
      TRY(add_virtual_link(c, liga_set_ಅವಿಭಾಜ್ಯ_id, coverage_ಅವಿಭಾಜ್ಯ_id));
      ಪ್ರತಿ (ಧ್ರುವ auto& l : liga_set_ಅವಿಭಾಜ್ಯ.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(add_virtual_link(c, l.objidx, coverage_ಅವಿಭಾಜ್ಯ_id));
      }

      ಎಣಿಕೆ += ಅಂಕೆ_ligas;
    }

    c.graph.vertices_[liga_subst_ಅವಿಭಾಜ್ಯ_id].shrink_buffer ((liga_subst_ಅವಿಭಾಜ್ಯ->ligatureSet.len - liga_set_ಎಣಿಕೆ) * SmallTypes::size);
    liga_subst_ಅವಿಭಾಜ್ಯ->ligatureSet.len = liga_set_ಎಣಿಕೆ;

    TRY (Coverage::filter_coverage (c,
                                    coverage_ಅವಿಭಾಜ್ಯ_id,
                                    liga_set_start, liga_set_end + 1));

    ಪ್ರತ್ಯಾಗಮ Ok(liga_subst_ಅವಿಭಾಜ್ಯ_id);
  }

  graph_result_t<ಶೂನ್ಯ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_ಎಣಿಕೆ,
                               hb_vector_t<unsigned> liga_counts,
                               unsigned ಎಣಿಕೆ)
  {
    DEBUG_ಸಂದೇಶ (SUBSET_REPACK, nullptr,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_index,
               ಎಣಿಕೆ);
    ಯದಿ (ಎಣಿಕೆ >= old_ಎಣಿಕೆ)
      ಪ್ರತ್ಯಾಗಮ Ok();

    hb_set_t retained_indices;
    unsigned new_liga_set_ಎಣಿಕೆ = 0;
    ಪ್ರತಿ (unsigned i = 0; i < liga_counts.length; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_set.index);
      retained_indices.add(liga_set.index);

      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      ಪ್ರತಿ (unsigned i = 0; i < liga_set.table->ligature.len; i++) {
        unsigned liga_index = index_to_id[i];
        ಯದಿ (liga_index != HB_GRAPH_INVALID) {
          clear_virtual_links(c, liga_index);
          retained_indices.add(liga_index);
        }
      }

      unsigned ಅಂಕೆ_ligas = liga_counts[i];
      ಯದಿ (ಅಂಕೆ_ligas >= ಎಣಿಕೆ) {
        // drop the trailing liga's from this set and all subsequent liga sets
        unsigned ಅಂಕೆ_ligas_to_remove = ಅಂಕೆ_ligas - ಎಣಿಕೆ;
        new_liga_set_ಎಣಿಕೆ = i + 1;
        c.graph.vertices_[liga_set.index].shrink_buffer (ಅಂಕೆ_ligas_to_remove * SmallTypes::size);
        liga_set.table->ligature.len = ಎಣಿಕೆ;
        ಭಙ್ಗ;
      } ಅನ್ಯಥಾ {
        ಎಣಿಕೆ -= ಅಂಕೆ_ligas;
      }
    }

    // Adjust liga set array
    auto& this_vertex = c.graph.vertices_[this_index];
    this_vertex.shrink_buffer ((ligatureSet.len - new_liga_set_ಎಣಿಕೆ) * SmallTypes::size);
    ligatureSet.len = new_liga_set_ಎಣಿಕೆ;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ASSIGN (unsigned coverage_idx, c.graph.index_for_offset (this_index, &ಏತತ್->coverage));

    auto& coverage_v = c.graph.vertices_[coverage_idx];
    unsigned coverage_size = coverage_v.table_size ();
    ಧ್ರುವ Coverage* coverage_table = (ಧ್ರುವ Coverage*) coverage_v.obj ().head;

    ಯದಿ (coverage_v.is_shared ())
    {
      TRY_ASSIGN (coverage_idx, c.graph.remap_child (this_index, coverage_idx));
    }

    ಪ್ರತಿ (unsigned i : retained_indices.iter())
      TRY(add_virtual_link(c, i, coverage_idx));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          ಪ್ರತ್ಯಾಗಮ p.second < new_liga_set_ಎಣಿಕೆ;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    ಪ್ರತ್ಯಾಗಮ Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_size);
  }
};

ಸಂರಚನಾ LigatureSubst : ಸಾರ್ವಜನಿಕ OT::Layout::GSUB_impl::LigatureSubst
{

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ ((LigatureSubstFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#endif
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<ಶೂನ್ಯ> sanitize (ಧ್ರುವ graph_t::vertex_t& vertex) ಧ್ರುವ
  {
    size_t vertex_len = vertex.table_size ();
    ಯದಿ (unlikely (vertex_len < u.format.v.get_size ())) ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 1:
      hb_barrier ();
      ಪ್ರತ್ಯಾಗಮ ((LigatureSubstFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ಪಕ್ಷ 2:  HB_FALLTHROUGH;
#endif
    ಸ್ವಭಾವ:
      // We don't handle format 2 here.
      ಪ್ರತ್ಯಾಗಮ Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_LIGATURE_GRAPH_HH
