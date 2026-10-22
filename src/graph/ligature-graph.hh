
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

#அந்தர்பாவ "graph.hh"
#அந்தர்பாவ "../OT/Layout/GSUB/LigatureSubst.hh"
#அந்தர்பாவ "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#அந்தர்பாவ "../OT/Layout/GSUB/LigatureSet.hh"
#அந்தர்பாவ "../hb-ot-dual.hh"
#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <utility>

நாமாகாஶ graph {

கட்டமைப்பு LigatureSet : பொது OT::Layout::GSUB_impl::LigatureSet<SmallTypes>
{
  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size ();
    எனில் (unlikely (vertex_len < OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size)) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t total_len = ligature.get_size() + OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size - ligature.len.get_size();
    எனில் (unlikely (vertex_len < total_len)) {
      திரும்பு Err(SANITIZE_FAILURE);
    }
    திரும்பு Ok();
  }
};

கட்டமைப்பு LigatureSubstFormat1 : பொது OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    எனில் (unlikely (vertex_len < min_size)) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();

    எனில் (unlikely (vertex_len < min_size + ligatureSet.get_size() - ligatureSet.len.get_size()))
      திரும்பு Err(SANITIZE_FAILURE);
    திரும்பு Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    TRY_ASSIGN (auto split_points, compute_split_points(c, this_index));
    எனில் (!split_points)
      திரும்பு Ok(hb_vector_t<unsigned> ());

    TRY_ASSIGN (unsigned total_ligas, total_எண்_ligas(c, this_index));
    TRY_ASSIGN (hb_vector_t<unsigned> counts, liga_counts(c, this_index));
    split_context_t split_context {
      c,
      இது,
      this_index,
      total_ligas,
      std::move (counts),
    };
    திரும்பு actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 தனிப்பட்ட:
  graph_result_t<unsigned> total_எண்_ligas(gsubgpos_graph_context_t& c, unsigned this_index) மாறிலி {
    unsigned total = 0;
    க்கு (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      total += liga_set.table->ligature.len;
    }
    திரும்பு Ok(total);
  }

  graph_result_t<hb_vector_t<unsigned>> liga_counts(gsubgpos_graph_context_t& c, unsigned this_index) மாறிலி {
    hb_vector_t<unsigned> result;
    க்கு (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      result.push(!liga_set.table ? 0 : liga_set.table->ligature.len);
    }
    திரும்பு graph_result_t<hb_vector_t<unsigned>>::from(std::move(result), ALLOCATION_FAILURE);
  }

  template <graph_t::vertex_mutability_t mutability>
  graph_result_t<hb_vector_t<unsigned>> ligature_index_to_object_id(மாறிலி graph_t::vertex_and_table_t<LigatureSet, mutability>& liga_set) மாறிலி {
    hb_vector_t<unsigned> map;
    எனில் (!map.resize_exact(liga_set.table->ligature.len)) திரும்பு Err(ALLOCATION_FAILURE);

    க்கு (unsigned i = 0; i < map.length; i++) {
      map[i] = HB_GRAPH_INVALID;
    }

    க்கு (மாறிலி auto& l : liga_set.vertex->obj ().real_links) {
      எனில் (l.position < 2) தொடர்;
      unsigned array_index = (l.position - 2) / 2;
      map[array_index] = l.objidx;
    }
    திரும்பு map;
  }

  graph_result_t<hb_vector_t<unsigned>> compute_split_points(gsubgpos_graph_context_t& c,
                                                             unsigned this_index) மாறிலி
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    மாறிலி unsigned base_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    unsigned accumulated = base_size;

    unsigned ligature_index = 0;
    hb_vector_t<unsigned> split_points;
    க்கு (unsigned i = 0; i < ligatureSet.len; i++)
    {
      accumulated += OT::HBUINT16::static_size; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size; // for ligature set table

      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      க்கு (unsigned j = 0; j < liga_set.table->ligature.len; j++)
      {
        மாறிலி unsigned liga_id = index_to_id[j];
        எனில் (liga_id == HB_GRAPH_INVALID) தொடர்; // no outgoing link, ignore
        மாறிலி unsigned liga_size = c.graph.vertices_[liga_id].table_size ();

        accumulated += OT::HBUINT16::static_size; // for ligature offset
        accumulated += liga_size; // for the ligature table

        எனில் (accumulated >= (1 << 16))
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

    திரும்பு split_points;
  }

  கட்டமைப்பு split_context_t
  {
    gsubgpos_graph_context_t& c;
    LigatureSubstFormat1* thiz;
    unsigned this_index;
    unsigned original_மொத்தம்;
    hb_vector_t<unsigned> liga_counts;

    unsigned original_மொத்தம்_2 ()
    {
      திரும்பு original_மொத்தம்;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      திரும்பு thiz->clone_range (c, this_index, liga_counts, start, end);
    }

    graph_result_t<வெறுமை> shrink (unsigned மொத்தம்)
    {
      திரும்பு thiz->shrink (c, this_index, original_மொத்தம்_2(), liga_counts, மொத்தம்);
    }
  };

  graph_result_t<hb_pair_t<unsigned, LigatureSet*>> new_liga_set(gsubgpos_graph_context_t& c, unsigned மொத்தம்) மாறிலி {
    unsigned பகாஎண்_size = OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size
                          + மொத்தம் * SmallTypes::size;

    TRY_ASSIGN (unsigned பகாஎண்_id, c.create_முனை (பகாஎண்_size));

    LigatureSet* பகாஎண் = (LigatureSet*) c.graph.object (பகாஎண்_id).head;
    பகாஎண்->ligature.len = மொத்தம்;
    திரும்பு Ok(hb_pair(பகாஎண்_id, பகாஎண்));
  }

  வெறுமை clear_virtual_links (gsubgpos_graph_context_t& c, unsigned முனை_index) மாறிலி
  {
    auto& v  = c.graph.vertices_[முனை_index];
    க்கு (மாறிலி auto& l : v.obj ().virtual_links)
    {
      auto& child = c.graph.vertices_[l.objidx];
      child.remove_parent(முனை_index);
    }
    v.clear_virtual_links ();
  }

  graph_result_t<வெறுமை> add_virtual_link(gsubgpos_graph_context_t& c, unsigned from, unsigned to) மாறிலி {
    எனில் (unlikely (from >= c.graph.vertices_.length || to >= c.graph.vertices_.length)) திரும்பு Err(OUT_OF_BOUNDS);
    TRY(c.graph.vertices_[to].add_parent (from, உண்மை));
    TRY(c.graph.vertices_[from].add_virtual_link (to));
    திரும்பு Ok();
  }

  hb_pair_t<unsigned, unsigned> current_liga_set_bounds (gsubgpos_graph_context_t& c,
                                                         unsigned liga_set_index,
                                                         மாறிலி hb_serialize_context_t::object_t& liga_set) மாறிலி
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    unsigned min_index = HB_GRAPH_INVALID;
    unsigned max_index = 0;
    க்கு (மாறிலி auto& l : liga_set.real_links) {
      எனில் (l.position < 2) தொடர்;

      unsigned liga_index = (l.position - 2) / 2;
      min_index = hb_min(min_index, liga_index);
      max_index = hb_max(max_index, liga_index);
    }
    திரும்பு hb_pair(min_index, max_index + 1);
  }

  வெறுமை compact_liga_set (gsubgpos_graph_context_t& c, LigatureSet* table, graph_t::vertex_t* v) மாறிலி
  {
    மாறிலி auto& obj = v->obj ();
    எனில் (table->ligature.len <= obj.real_links.length) திரும்பு;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    unsigned to_remove = table->ligature.len - obj.real_links.length;
    unsigned new_position = SmallTypes::size;
    v->sort_real_links (); // for this to work we need to process links in order of position.
    க்கு (auto& l : v->real_links_writer ())
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
                                        unsigned start, unsigned end) மாறிலி
  {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_index, start, end);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    unsigned பகாஎண்_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size
                          + ligatureSet.get_size() - ligatureSet.len.get_size();

    TRY_ASSIGN (unsigned liga_subst_பகாஎண்_id, c.create_முனை (பகாஎண்_size));

    LigatureSubstFormat1* liga_subst_பகாஎண் = (LigatureSubstFormat1*) c.graph.object (liga_subst_பகாஎண்_id).head;
    liga_subst_பகாஎண்->format = இது->format;
    liga_subst_பகாஎண்->ligatureSet.len = இது->ligatureSet.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY_ASSIGN (unsigned coverage_பகாஎண்_id, c.graph.duplicate(coverage_id));
    auto& coverage_பகாஎண்_vertex = c.graph.vertices_[coverage_பகாஎண்_id];

    TRY(c.graph.vertices_[liga_subst_பகாஎண்_id].add_real_link (SmallTypes::size, coverage_பகாஎண்_id, 2));
    TRY(coverage_பகாஎண்_vertex.add_parent (liga_subst_பகாஎண்_id, பொய்));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    unsigned மொத்தம் = 0;
    unsigned liga_set_மொத்தம் = 0;
    unsigned liga_set_start = HB_GRAPH_INVALID;
    unsigned liga_set_end = 0; // inclusive
    க்கு (unsigned i = 0; i < liga_counts.length; i++)
    {
      unsigned இலக்கம்_ligas = liga_counts[i];

      unsigned current_start = மொத்தம்;
      unsigned current_end = மொத்தம் + இலக்கம்_ligas;

      எனில் (current_start >= end || start >= current_end) {
        // No intersection, so just skip
        மொத்தம் += இலக்கம்_ligas;
        தொடர்;
      }

      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      unsigned liga_set_index = liga_set.index;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<unsigned, unsigned> liga_bounds = current_liga_set_bounds(c, liga_set_index, liga_set.vertex->obj ());
      current_start = hb_max(மொத்தம் + liga_bounds.first, current_start);
      current_end = hb_min(மொத்தம் + liga_bounds.second, current_end);

      unsigned liga_set_பகாஎண்_id;
      எனில் (current_start >= start && current_end <= end) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_set_end = i;
        எனில் (i < liga_set_start) liga_set_start = i;
        TRY_ASSIGN (liga_set_பகாஎண்_id, c.graph.move_child<> (this_index,
                              &ligatureSet[i],
                              liga_subst_பகாஎண்_id,
                              &liga_subst_பகாஎண்->ligatureSet[liga_set_மொத்தம்++]));
        compact_liga_set (c, liga_set.table, liga_set.vertex);
      }
      இல்லையெனில்
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        unsigned start_index = hb_max(start, current_start) - மொத்தம்;
        unsigned end_index = hb_min(end, current_end) - மொத்தம்;
        unsigned liga_மொத்தம் = end_index - start_index;
        TRY_ASSIGN (auto result, new_liga_set(c, liga_மொத்தம்));
        liga_set_பகாஎண்_id = result.first;

        TRY (c.graph.move_children<OT::Offset16>(
          liga_set_index,
          2 + start_index * 2,
          2 + end_index * 2,
          liga_set_பகாஎண்_id,
          2));

        liga_set_end = i;
        எனில் (i < liga_set_start) liga_set_start = i;
        TRY (c.graph.add_link(&liga_subst_பகாஎண்->ligatureSet[liga_set_மொத்தம்++], liga_subst_பகாஎண்_id, liga_set_பகாஎண்_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      மாறிலி auto& liga_set_பகாஎண் = c.graph.vertices_[liga_set_பகாஎண்_id].obj ();
      clear_virtual_links(c, liga_set_பகாஎண்_id);
      TRY(add_virtual_link(c, liga_set_பகாஎண்_id, coverage_பகாஎண்_id));
      க்கு (மாறிலி auto& l : liga_set_பகாஎண்.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(add_virtual_link(c, l.objidx, coverage_பகாஎண்_id));
      }

      மொத்தம் += இலக்கம்_ligas;
    }

    c.graph.vertices_[liga_subst_பகாஎண்_id].shrink_buffer ((liga_subst_பகாஎண்->ligatureSet.len - liga_set_மொத்தம்) * SmallTypes::size);
    liga_subst_பகாஎண்->ligatureSet.len = liga_set_மொத்தம்;

    TRY (Coverage::filter_coverage (c,
                                    coverage_பகாஎண்_id,
                                    liga_set_start, liga_set_end + 1));

    திரும்பு Ok(liga_subst_பகாஎண்_id);
  }

  graph_result_t<வெறுமை> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_மொத்தம்,
                               hb_vector_t<unsigned> liga_counts,
                               unsigned மொத்தம்)
  {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_index,
               மொத்தம்);
    எனில் (மொத்தம் >= old_மொத்தம்)
      திரும்பு Ok();

    hb_set_t retained_indices;
    unsigned new_liga_set_மொத்தம் = 0;
    க்கு (unsigned i = 0; i < liga_counts.length; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_set.index);
      retained_indices.add(liga_set.index);

      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      க்கு (unsigned i = 0; i < liga_set.table->ligature.len; i++) {
        unsigned liga_index = index_to_id[i];
        எனில் (liga_index != HB_GRAPH_INVALID) {
          clear_virtual_links(c, liga_index);
          retained_indices.add(liga_index);
        }
      }

      unsigned இலக்கம்_ligas = liga_counts[i];
      எனில் (இலக்கம்_ligas >= மொத்தம்) {
        // drop the trailing liga's from this set and all subsequent liga sets
        unsigned இலக்கம்_ligas_to_remove = இலக்கம்_ligas - மொத்தம்;
        new_liga_set_மொத்தம் = i + 1;
        c.graph.vertices_[liga_set.index].shrink_buffer (இலக்கம்_ligas_to_remove * SmallTypes::size);
        liga_set.table->ligature.len = மொத்தம்;
        நிறுத்து;
      } இல்லையெனில் {
        மொத்தம் -= இலக்கம்_ligas;
      }
    }

    // Adjust liga set array
    auto& this_vertex = c.graph.vertices_[this_index];
    this_vertex.shrink_buffer ((ligatureSet.len - new_liga_set_மொத்தம்) * SmallTypes::size);
    ligatureSet.len = new_liga_set_மொத்தம்;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ASSIGN (unsigned coverage_idx, c.graph.index_for_offset (this_index, &இது->coverage));

    auto& coverage_v = c.graph.vertices_[coverage_idx];
    unsigned coverage_size = coverage_v.table_size ();
    மாறிலி Coverage* coverage_table = (மாறிலி Coverage*) coverage_v.obj ().head;

    எனில் (coverage_v.is_shared ())
    {
      TRY_ASSIGN (coverage_idx, c.graph.remap_child (this_index, coverage_idx));
    }

    க்கு (unsigned i : retained_indices.iter())
      TRY(add_virtual_link(c, i, coverage_idx));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          திரும்பு p.second < new_liga_set_மொத்தம்;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    திரும்பு Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_size);
  }
};

கட்டமைப்பு LigatureSubst : பொது OT::Layout::GSUB_impl::LigatureSubst
{

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    தேர்வு (u.format.v) {
    நிலை 1:
      hb_barrier ();
      திரும்பு ((LigatureSubstFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    நிலை 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#endif
    இயல்பு:
      திரும்பு Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<வெறுமை> sanitize (மாறிலி graph_t::vertex_t& vertex) மாறிலி
  {
    size_t vertex_len = vertex.table_size ();
    எனில் (unlikely (vertex_len < u.format.v.get_size ())) திரும்பு Err(SANITIZE_FAILURE);
    hb_barrier ();

    தேர்வு (u.format.v) {
    நிலை 1:
      hb_barrier ();
      திரும்பு ((LigatureSubstFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    நிலை 2:  HB_FALLTHROUGH;
#endif
    இயல்பு:
      // We don't handle format 2 here.
      திரும்பு Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_LIGATURE_GRAPH_HH
