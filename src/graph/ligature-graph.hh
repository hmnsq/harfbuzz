
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

#അന്തര്ഭാവ "graph.hh"
#അന്തര്ഭാവ "../OT/Layout/GSUB/LigatureSubst.hh"
#അന്തര്ഭാവ "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#അന്തര്ഭാവ "../OT/Layout/GSUB/LigatureSet.hh"
#അന്തര്ഭാവ "../hb-ot-dual.hh"
#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <utility>

നാമാകാശ graph {

ഘടന LigatureSet : പൊതു OT::Layout::GSUB_impl::LigatureSet<SmallTypes>
{
  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    എങ്കിൽ (unlikely (vertex_len < OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t total_len = ligature.get_size() + OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size - ligature.len.get_size();
    എങ്കിൽ (unlikely (vertex_len < total_len)) {
      തിരികെ Err(SANITIZE_FAILURE);
    }
    തിരികെ Ok();
  }
};

ഘടന LigatureSubstFormat1 : പൊതു OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  graph_result_t<ശൂന്യം> sanitize (സ്ഥിരം graph_t::vertex_t& vertex) സ്ഥിരം
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    എങ്കിൽ (unlikely (vertex_len < min_size)) തിരികെ Err(SANITIZE_FAILURE);
    hb_barrier ();

    എങ്കിൽ (unlikely (vertex_len < min_size + ligatureSet.get_size() - ligatureSet.len.get_size()))
      തിരികെ Err(SANITIZE_FAILURE);
    തിരികെ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    TRY_ASSIGN (auto split_points, compute_split_points(c, this_index));
    എങ്കിൽ (!split_points)
      തിരികെ Ok(hb_vector_t<unsigned> ());

    TRY_ASSIGN (unsigned total_ligas, total_സംഖ്യ_ligas(c, this_index));
    TRY_ASSIGN (hb_vector_t<unsigned> counts, liga_counts(c, this_index));
    split_context_t split_context {
      c,
      ഇത്,
      this_index,
      total_ligas,
      std::move (counts),
    };
    തിരികെ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 സ്വകാര്യ:
  graph_result_t<unsigned> total_സംഖ്യ_ligas(gsubgpos_graph_context_t& c, unsigned this_index) സ്ഥിരം {
    unsigned total = 0;
    വേണ്ടി (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      total += liga_set.table->ligature.len;
    }
    തിരികെ Ok(total);
  }

  graph_result_t<hb_vector_t<unsigned>> liga_counts(gsubgpos_graph_context_t& c, unsigned this_index) സ്ഥിരം {
    hb_vector_t<unsigned> result;
    വേണ്ടി (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      result.push(!liga_set.table ? 0 : liga_set.table->ligature.len);
    }
    തിരികെ graph_result_t<hb_vector_t<unsigned>>::from(std::move(result), ALLOCATION_FAILURE);
  }

  template <graph_t::vertex_mutability_t mutability>
  graph_result_t<hb_vector_t<unsigned>> ligature_index_to_object_id(സ്ഥിരം graph_t::vertex_and_table_t<LigatureSet, mutability>& liga_set) സ്ഥിരം {
    hb_vector_t<unsigned> map;
    എങ്കിൽ (!map.resize_exact(liga_set.table->ligature.len)) തിരികെ Err(ALLOCATION_FAILURE);

    വേണ്ടി (unsigned i = 0; i < map.length; i++) {
      map[i] = HB_GRAPH_INVALID;
    }

    വേണ്ടി (സ്ഥിരം auto& l : liga_set.vertex->obj ().real_links) {
      എങ്കിൽ (l.position < 2) തുടരുക;
      unsigned array_index = (l.position - 2) / 2;
      map[array_index] = l.objidx;
    }
    തിരികെ map;
  }

  graph_result_t<hb_vector_t<unsigned>> compute_split_points(gsubgpos_graph_context_t& c,
                                                             unsigned this_index) സ്ഥിരം
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    സ്ഥിരം unsigned base_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    unsigned accumulated = base_size;

    unsigned ligature_index = 0;
    hb_vector_t<unsigned> split_points;
    വേണ്ടി (unsigned i = 0; i < ligatureSet.len; i++)
    {
      accumulated += OT::HBUINT16::static_size; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size; // for ligature set table

      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      വേണ്ടി (unsigned j = 0; j < liga_set.table->ligature.len; j++)
      {
        സ്ഥിരം unsigned liga_id = index_to_id[j];
        എങ്കിൽ (liga_id == HB_GRAPH_INVALID) തുടരുക; // no outgoing link, ignore
        സ്ഥിരം unsigned liga_size = c.graph.vertices_[liga_id].table_size ();

        accumulated += OT::HBUINT16::static_size; // for ligature offset
        accumulated += liga_size; // for the ligature table

        എങ്കിൽ (accumulated >= (1 << 16))
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

    തിരികെ split_points;
  }

  ഘടന split_context_t
  {
    gsubgpos_graph_context_t& c;
    LigatureSubstFormat1* thiz;
    unsigned this_index;
    unsigned original_എണ്ണം;
    hb_vector_t<unsigned> liga_counts;

    unsigned original_എണ്ണം_2 ()
    {
      തിരികെ original_എണ്ണം;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      തിരികെ thiz->clone_range (c, this_index, liga_counts, start, end);
    }

    graph_result_t<ശൂന്യം> shrink (unsigned എണ്ണം)
    {
      തിരികെ thiz->shrink (c, this_index, original_എണ്ണം_2(), liga_counts, എണ്ണം);
    }
  };

  graph_result_t<hb_pair_t<unsigned, LigatureSet*>> new_liga_set(gsubgpos_graph_context_t& c, unsigned എണ്ണം) സ്ഥിരം {
    unsigned അഭാജ്യം_size = OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size
                          + എണ്ണം * SmallTypes::size;

    TRY_ASSIGN (unsigned അഭാജ്യം_id, c.create_കണ്ണി (അഭാജ്യം_size));

    LigatureSet* അഭാജ്യം = (LigatureSet*) c.graph.object (അഭാജ്യം_id).head;
    അഭാജ്യം->ligature.len = എണ്ണം;
    തിരികെ Ok(hb_pair(അഭാജ്യം_id, അഭാജ്യം));
  }

  ശൂന്യം clear_virtual_links (gsubgpos_graph_context_t& c, unsigned കണ്ണി_index) സ്ഥിരം
  {
    auto& v  = c.graph.vertices_[കണ്ണി_index];
    വേണ്ടി (സ്ഥിരം auto& l : v.obj ().virtual_links)
    {
      auto& child = c.graph.vertices_[l.objidx];
      child.remove_parent(കണ്ണി_index);
    }
    v.clear_virtual_links ();
  }

  graph_result_t<ശൂന്യം> add_virtual_link(gsubgpos_graph_context_t& c, unsigned from, unsigned to) സ്ഥിരം {
    എങ്കിൽ (unlikely (from >= c.graph.vertices_.length || to >= c.graph.vertices_.length)) തിരികെ Err(OUT_OF_BOUNDS);
    TRY(c.graph.vertices_[to].add_parent (from, സത്യം));
    TRY(c.graph.vertices_[from].add_virtual_link (to));
    തിരികെ Ok();
  }

  hb_pair_t<unsigned, unsigned> current_liga_set_bounds (gsubgpos_graph_context_t& c,
                                                         unsigned liga_set_index,
                                                         സ്ഥിരം hb_serialize_context_t::object_t& liga_set) സ്ഥിരം
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    unsigned min_index = HB_GRAPH_INVALID;
    unsigned max_index = 0;
    വേണ്ടി (സ്ഥിരം auto& l : liga_set.real_links) {
      എങ്കിൽ (l.position < 2) തുടരുക;

      unsigned liga_index = (l.position - 2) / 2;
      min_index = hb_min(min_index, liga_index);
      max_index = hb_max(max_index, liga_index);
    }
    തിരികെ hb_pair(min_index, max_index + 1);
  }

  ശൂന്യം compact_liga_set (gsubgpos_graph_context_t& c, LigatureSet* table, graph_t::vertex_t* v) സ്ഥിരം
  {
    സ്ഥിരം auto& obj = v->obj ();
    എങ്കിൽ (table->ligature.len <= obj.real_links.length) തിരികെ;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    unsigned to_remove = table->ligature.len - obj.real_links.length;
    unsigned new_position = SmallTypes::size;
    v->sort_real_links (); // for this to work we need to process links in order of position.
    വേണ്ടി (auto& l : v->real_links_writer ())
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
                                        unsigned start, unsigned end) സ്ഥിരം
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_index, start, end);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    unsigned അഭാജ്യം_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size
                          + ligatureSet.get_size() - ligatureSet.len.get_size();

    TRY_ASSIGN (unsigned liga_subst_അഭാജ്യം_id, c.create_കണ്ണി (അഭാജ്യം_size));

    LigatureSubstFormat1* liga_subst_അഭാജ്യം = (LigatureSubstFormat1*) c.graph.object (liga_subst_അഭാജ്യം_id).head;
    liga_subst_അഭാജ്യം->format = ഇത്->format;
    liga_subst_അഭാജ്യം->ligatureSet.len = ഇത്->ligatureSet.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY_ASSIGN (unsigned coverage_അഭാജ്യം_id, c.graph.duplicate(coverage_id));
    auto& coverage_അഭാജ്യം_vertex = c.graph.vertices_[coverage_അഭാജ്യം_id];

    TRY(c.graph.vertices_[liga_subst_അഭാജ്യം_id].add_real_link (SmallTypes::size, coverage_അഭാജ്യം_id, 2));
    TRY(coverage_അഭാജ്യം_vertex.add_parent (liga_subst_അഭാജ്യം_id, അസത്യം));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    unsigned എണ്ണം = 0;
    unsigned liga_set_എണ്ണം = 0;
    unsigned liga_set_start = HB_GRAPH_INVALID;
    unsigned liga_set_end = 0; // inclusive
    വേണ്ടി (unsigned i = 0; i < liga_counts.length; i++)
    {
      unsigned അക്കം_ligas = liga_counts[i];

      unsigned current_start = എണ്ണം;
      unsigned current_end = എണ്ണം + അക്കം_ligas;

      എങ്കിൽ (current_start >= end || start >= current_end) {
        // No intersection, so just skip
        എണ്ണം += അക്കം_ligas;
        തുടരുക;
      }

      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      unsigned liga_set_index = liga_set.index;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<unsigned, unsigned> liga_bounds = current_liga_set_bounds(c, liga_set_index, liga_set.vertex->obj ());
      current_start = hb_max(എണ്ണം + liga_bounds.first, current_start);
      current_end = hb_min(എണ്ണം + liga_bounds.second, current_end);

      unsigned liga_set_അഭാജ്യം_id;
      എങ്കിൽ (current_start >= start && current_end <= end) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_set_end = i;
        എങ്കിൽ (i < liga_set_start) liga_set_start = i;
        TRY_ASSIGN (liga_set_അഭാജ്യം_id, c.graph.move_child<> (this_index,
                              &ligatureSet[i],
                              liga_subst_അഭാജ്യം_id,
                              &liga_subst_അഭാജ്യം->ligatureSet[liga_set_എണ്ണം++]));
        compact_liga_set (c, liga_set.table, liga_set.vertex);
      }
      അല്ലെങ്കിൽ
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        unsigned start_index = hb_max(start, current_start) - എണ്ണം;
        unsigned end_index = hb_min(end, current_end) - എണ്ണം;
        unsigned liga_എണ്ണം = end_index - start_index;
        TRY_ASSIGN (auto result, new_liga_set(c, liga_എണ്ണം));
        liga_set_അഭാജ്യം_id = result.first;

        TRY (c.graph.move_children<OT::Offset16>(
          liga_set_index,
          2 + start_index * 2,
          2 + end_index * 2,
          liga_set_അഭാജ്യം_id,
          2));

        liga_set_end = i;
        എങ്കിൽ (i < liga_set_start) liga_set_start = i;
        TRY (c.graph.add_link(&liga_subst_അഭാജ്യം->ligatureSet[liga_set_എണ്ണം++], liga_subst_അഭാജ്യം_id, liga_set_അഭാജ്യം_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      സ്ഥിരം auto& liga_set_അഭാജ്യം = c.graph.vertices_[liga_set_അഭാജ്യം_id].obj ();
      clear_virtual_links(c, liga_set_അഭാജ്യം_id);
      TRY(add_virtual_link(c, liga_set_അഭാജ്യം_id, coverage_അഭാജ്യം_id));
      വേണ്ടി (സ്ഥിരം auto& l : liga_set_അഭാജ്യം.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(add_virtual_link(c, l.objidx, coverage_അഭാജ്യം_id));
      }

      എണ്ണം += അക്കം_ligas;
    }

    c.graph.vertices_[liga_subst_അഭാജ്യം_id].shrink_buffer ((liga_subst_അഭാജ്യം->ligatureSet.len - liga_set_എണ്ണം) * SmallTypes::size);
    liga_subst_അഭാജ്യം->ligatureSet.len = liga_set_എണ്ണം;

    TRY (Coverage::filter_coverage (c,
                                    coverage_അഭാജ്യം_id,
                                    liga_set_start, liga_set_end + 1));

    തിരികെ Ok(liga_subst_അഭാജ്യം_id);
  }

  graph_result_t<ശൂന്യം> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_എണ്ണം,
                               hb_vector_t<unsigned> liga_counts,
                               unsigned എണ്ണം)
  {
    DEBUG_സന്ദേശം (SUBSET_REPACK, nullptr,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_index,
               എണ്ണം);
    എങ്കിൽ (എണ്ണം >= old_എണ്ണം)
      തിരികെ Ok();

    hb_set_t retained_indices;
    unsigned new_liga_set_എണ്ണം = 0;
    വേണ്ടി (unsigned i = 0; i < liga_counts.length; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_set.index);
      retained_indices.add(liga_set.index);

      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      വേണ്ടി (unsigned i = 0; i < liga_set.table->ligature.len; i++) {
        unsigned liga_index = index_to_id[i];
        എങ്കിൽ (liga_index != HB_GRAPH_INVALID) {
          clear_virtual_links(c, liga_index);
          retained_indices.add(liga_index);
        }
      }

      unsigned അക്കം_ligas = liga_counts[i];
      എങ്കിൽ (അക്കം_ligas >= എണ്ണം) {
        // drop the trailing liga's from this set and all subsequent liga sets
        unsigned അക്കം_ligas_to_remove = അക്കം_ligas - എണ്ണം;
        new_liga_set_എണ്ണം = i + 1;
        c.graph.vertices_[liga_set.index].shrink_buffer (അക്കം_ligas_to_remove * SmallTypes::size);
        liga_set.table->ligature.len = എണ്ണം;
        നിർത്തുക;
      } അല്ലെങ്കിൽ {
        എണ്ണം -= അക്കം_ligas;
      }
    }

    // Adjust liga set array
    auto& this_vertex = c.graph.vertices_[this_index];
    this_vertex.shrink_buffer ((ligatureSet.len - new_liga_set_എണ്ണം) * SmallTypes::size);
    ligatureSet.len = new_liga_set_എണ്ണം;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ASSIGN (unsigned coverage_idx, c.graph.index_for_offset (this_index, &ഇത്->coverage));

    auto& coverage_v = c.graph.vertices_[coverage_idx];
    unsigned coverage_size = coverage_v.table_size ();
    സ്ഥിരം Coverage* coverage_table = (സ്ഥിരം Coverage*) coverage_v.obj ().head;

    എങ്കിൽ (coverage_v.is_shared ())
    {
      TRY_ASSIGN (coverage_idx, c.graph.remap_child (this_index, coverage_idx));
    }

    വേണ്ടി (unsigned i : retained_indices.iter())
      TRY(add_virtual_link(c, i, coverage_idx));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          തിരികെ p.second < new_liga_set_എണ്ണം;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    തിരികെ Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_size);
  }
};

ഘടന LigatureSubst : പൊതു OT::Layout::GSUB_impl::LigatureSubst
{

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 1:
      hb_barrier ();
      തിരികെ ((LigatureSubstFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
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
      തിരികെ ((LigatureSubstFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    അവസ്ഥ 2:  HB_FALLTHROUGH;
#endif
    സ്ഥിരസ്ഥിതി:
      // We don't handle format 2 here.
      തിരികെ Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_LIGATURE_GRAPH_HH
