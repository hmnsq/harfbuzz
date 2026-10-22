
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

#ଅନ୍ତର୍ଭାଵ "graph.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GSUB/LigatureSubst.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#ଅନ୍ତର୍ଭାଵ "../OT/Layout/GSUB/LigatureSet.hh"
#ଅନ୍ତର୍ଭାଵ "../hb-ot-dual.hh"
#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <utility>

ନାମାକାଶ graph {

ସଂରଚନା LigatureSet : ସର୍ବସାଧାରଣ OT::Layout::GSUB_impl::LigatureSet<SmallTypes>
{
  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    ଯଦି (unlikely (vertex_len < OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t total_len = ligature.get_size() + OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size - ligature.len.get_size();
    ଯଦି (unlikely (vertex_len < total_len)) {
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    }
    ଫେରନ୍ତୁ Ok();
  }
};

ସଂରଚନା LigatureSubstFormat1 : ସର୍ବସାଧାରଣ OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    unsigned min_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    ଯଦି (unlikely (vertex_len < min_size)) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ଯଦି (unlikely (vertex_len < min_size + ligatureSet.get_size() - ligatureSet.len.get_size()))
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    ଫେରନ୍ତୁ Ok();
  }

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    TRY_ASSIGN (auto split_points, compute_split_points(c, this_index));
    ଯଦି (!split_points)
      ଫେରନ୍ତୁ Ok(hb_vector_t<unsigned> ());

    TRY_ASSIGN (unsigned total_ligas, total_ସଂଖ୍ୟା_ligas(c, this_index));
    TRY_ASSIGN (hb_vector_t<unsigned> counts, liga_counts(c, this_index));
    split_context_t split_context {
      c,
      ଏହି,
      this_index,
      total_ligas,
      std::move (counts),
    };
    ଫେରନ୍ତୁ actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ବ୍ୟକ୍ତିଗତ:
  graph_result_t<unsigned> total_ସଂଖ୍ୟା_ligas(gsubgpos_graph_context_t& c, unsigned this_index) ଅଚଳ {
    unsigned total = 0;
    ପାଇଁ (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      total += liga_set.table->ligature.len;
    }
    ଫେରନ୍ତୁ Ok(total);
  }

  graph_result_t<hb_vector_t<unsigned>> liga_counts(gsubgpos_graph_context_t& c, unsigned this_index) ଅଚଳ {
    hb_vector_t<unsigned> result;
    ପାଇଁ (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));
      result.push(!liga_set.table ? 0 : liga_set.table->ligature.len);
    }
    ଫେରନ୍ତୁ graph_result_t<hb_vector_t<unsigned>>::from(std::move(result), ALLOCATION_FAILURE);
  }

  template <graph_t::vertex_mutability_t mutability>
  graph_result_t<hb_vector_t<unsigned>> ligature_index_to_object_id(ଅଚଳ graph_t::vertex_and_table_t<LigatureSet, mutability>& liga_set) ଅଚଳ {
    hb_vector_t<unsigned> map;
    ଯଦି (!map.resize_exact(liga_set.table->ligature.len)) ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);

    ପାଇଁ (unsigned i = 0; i < map.length; i++) {
      map[i] = HB_GRAPH_INVALID;
    }

    ପାଇଁ (ଅଚଳ auto& l : liga_set.vertex->obj ().real_links) {
      ଯଦି (l.position < 2) ଜାରି;
      unsigned array_index = (l.position - 2) / 2;
      map[array_index] = l.objidx;
    }
    ଫେରନ୍ତୁ map;
  }

  graph_result_t<hb_vector_t<unsigned>> compute_split_points(gsubgpos_graph_context_t& c,
                                                             unsigned this_index) ଅଚଳ
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ଅଚଳ unsigned base_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size;
    unsigned accumulated = base_size;

    unsigned ligature_index = 0;
    hb_vector_t<unsigned> split_points;
    ପାଇଁ (unsigned i = 0; i < ligatureSet.len; i++)
    {
      accumulated += OT::HBUINT16::static_size; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size; // for ligature set table

      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_index, &ligatureSet[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      ପାଇଁ (unsigned j = 0; j < liga_set.table->ligature.len; j++)
      {
        ଅଚଳ unsigned liga_id = index_to_id[j];
        ଯଦି (liga_id == HB_GRAPH_INVALID) ଜାରି; // no outgoing link, ignore
        ଅଚଳ unsigned liga_size = c.graph.vertices_[liga_id].table_size ();

        accumulated += OT::HBUINT16::static_size; // for ligature offset
        accumulated += liga_size; // for the ligature table

        ଯଦି (accumulated >= (1 << 16))
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

    ଫେରନ୍ତୁ split_points;
  }

  ସଂରଚନା split_context_t
  {
    gsubgpos_graph_context_t& c;
    LigatureSubstFormat1* thiz;
    unsigned this_index;
    unsigned original_ଗଣନା;
    hb_vector_t<unsigned> liga_counts;

    unsigned original_ଗଣନା_2 ()
    {
      ଫେରନ୍ତୁ original_ଗଣନା;
    }

    graph_result_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ଫେରନ୍ତୁ thiz->clone_range (c, this_index, liga_counts, start, end);
    }

    graph_result_t<ଶୂନ୍ୟ> shrink (unsigned ଗଣନା_2)
    {
      ଫେରନ୍ତୁ thiz->shrink (c, this_index, original_ଗଣନା_2(), liga_counts, ଗଣନା_2);
    }
  };

  graph_result_t<hb_pair_t<unsigned, LigatureSet*>> new_liga_set(gsubgpos_graph_context_t& c, unsigned ଗଣନା_2) ଅଚଳ {
    unsigned ମୌଳିକ_size = OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_size
                          + ଗଣନା_2 * SmallTypes::size;

    TRY_ASSIGN (unsigned ମୌଳିକ_id, c.create_ଗଣ୍ଠି (ମୌଳିକ_size));

    LigatureSet* ମୌଳିକ = (LigatureSet*) c.graph.object (ମୌଳିକ_id).head;
    ମୌଳିକ->ligature.len = ଗଣନା_2;
    ଫେରନ୍ତୁ Ok(hb_pair(ମୌଳିକ_id, ମୌଳିକ));
  }

  ଶୂନ୍ୟ clear_virtual_links (gsubgpos_graph_context_t& c, unsigned ଗଣ୍ଠି_index) ଅଚଳ
  {
    auto& v  = c.graph.vertices_[ଗଣ୍ଠି_index];
    ପାଇଁ (ଅଚଳ auto& l : v.obj ().virtual_links)
    {
      auto& child = c.graph.vertices_[l.objidx];
      child.remove_parent(ଗଣ୍ଠି_index);
    }
    v.clear_virtual_links ();
  }

  graph_result_t<ଶୂନ୍ୟ> add_virtual_link(gsubgpos_graph_context_t& c, unsigned from, unsigned to) ଅଚଳ {
    ଯଦି (unlikely (from >= c.graph.vertices_.length || to >= c.graph.vertices_.length)) ଫେରନ୍ତୁ Err(OUT_OF_BOUNDS);
    TRY(c.graph.vertices_[to].add_parent (from, ସତ୍ୟ));
    TRY(c.graph.vertices_[from].add_virtual_link (to));
    ଫେରନ୍ତୁ Ok();
  }

  hb_pair_t<unsigned, unsigned> current_liga_set_bounds (gsubgpos_graph_context_t& c,
                                                         unsigned liga_set_index,
                                                         ଅଚଳ hb_serialize_context_t::object_t& liga_set) ଅଚଳ
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    unsigned min_index = HB_GRAPH_INVALID;
    unsigned max_index = 0;
    ପାଇଁ (ଅଚଳ auto& l : liga_set.real_links) {
      ଯଦି (l.position < 2) ଜାରି;

      unsigned liga_index = (l.position - 2) / 2;
      min_index = hb_min(min_index, liga_index);
      max_index = hb_max(max_index, liga_index);
    }
    ଫେରନ୍ତୁ hb_pair(min_index, max_index + 1);
  }

  ଶୂନ୍ୟ compact_liga_set (gsubgpos_graph_context_t& c, LigatureSet* table, graph_t::vertex_t* v) ଅଚଳ
  {
    ଅଚଳ auto& obj = v->obj ();
    ଯଦି (table->ligature.len <= obj.real_links.length) ଫେରନ୍ତୁ;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    unsigned to_remove = table->ligature.len - obj.real_links.length;
    unsigned new_position = SmallTypes::size;
    v->sort_real_links (); // for this to work we need to process links in order of position.
    ପାଇଁ (auto& l : v->real_links_writer ())
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
                                        unsigned start, unsigned end) ଅଚଳ
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_index, start, end);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    unsigned ମୌଳିକ_size = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_size
                          + ligatureSet.get_size() - ligatureSet.len.get_size();

    TRY_ASSIGN (unsigned liga_subst_ମୌଳିକ_id, c.create_ଗଣ୍ଠି (ମୌଳିକ_size));

    LigatureSubstFormat1* liga_subst_ମୌଳିକ = (LigatureSubstFormat1*) c.graph.object (liga_subst_ମୌଳିକ_id).head;
    liga_subst_ମୌଳିକ->format = ଏହି->format;
    liga_subst_ମୌଳିକ->ligatureSet.len = ଏହି->ligatureSet.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ASSIGN (unsigned coverage_id, c.graph.index_for_offset (this_index, &coverage));
    TRY_ASSIGN (unsigned coverage_ମୌଳିକ_id, c.graph.duplicate(coverage_id));
    auto& coverage_ମୌଳିକ_vertex = c.graph.vertices_[coverage_ମୌଳିକ_id];

    TRY(c.graph.vertices_[liga_subst_ମୌଳିକ_id].add_real_link (SmallTypes::size, coverage_ମୌଳିକ_id, 2));
    TRY(coverage_ମୌଳିକ_vertex.add_parent (liga_subst_ମୌଳିକ_id, ମିଥ୍ୟା));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    unsigned ଗଣନା_2 = 0;
    unsigned liga_set_ଗଣନା = 0;
    unsigned liga_set_start = HB_GRAPH_INVALID;
    unsigned liga_set_end = 0; // inclusive
    ପାଇଁ (unsigned i = 0; i < liga_counts.length; i++)
    {
      unsigned ଅଙ୍କ_ligas = liga_counts[i];

      unsigned current_start = ଗଣନା_2;
      unsigned current_end = ଗଣନା_2 + ଅଙ୍କ_ligas;

      ଯଦି (current_start >= end || start >= current_end) {
        // No intersection, so just skip
        ଗଣନା_2 += ଅଙ୍କ_ligas;
        ଜାରି;
      }

      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      unsigned liga_set_index = liga_set.index;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<unsigned, unsigned> liga_bounds = current_liga_set_bounds(c, liga_set_index, liga_set.vertex->obj ());
      current_start = hb_max(ଗଣନା_2 + liga_bounds.first, current_start);
      current_end = hb_min(ଗଣନା_2 + liga_bounds.second, current_end);

      unsigned liga_set_ମୌଳିକ_id;
      ଯଦି (current_start >= start && current_end <= end) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_set_end = i;
        ଯଦି (i < liga_set_start) liga_set_start = i;
        TRY_ASSIGN (liga_set_ମୌଳିକ_id, c.graph.move_child<> (this_index,
                              &ligatureSet[i],
                              liga_subst_ମୌଳିକ_id,
                              &liga_subst_ମୌଳିକ->ligatureSet[liga_set_ଗଣନା++]));
        compact_liga_set (c, liga_set.table, liga_set.vertex);
      }
      ଅନ୍ୟଥା
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        unsigned start_index = hb_max(start, current_start) - ଗଣନା_2;
        unsigned end_index = hb_min(end, current_end) - ଗଣନା_2;
        unsigned liga_ଗଣନା = end_index - start_index;
        TRY_ASSIGN (auto result, new_liga_set(c, liga_ଗଣନା));
        liga_set_ମୌଳିକ_id = result.first;

        TRY (c.graph.move_children<OT::Offset16>(
          liga_set_index,
          2 + start_index * 2,
          2 + end_index * 2,
          liga_set_ମୌଳିକ_id,
          2));

        liga_set_end = i;
        ଯଦି (i < liga_set_start) liga_set_start = i;
        TRY (c.graph.add_link(&liga_subst_ମୌଳିକ->ligatureSet[liga_set_ଗଣନା++], liga_subst_ମୌଳିକ_id, liga_set_ମୌଳିକ_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ଅଚଳ auto& liga_set_ମୌଳିକ = c.graph.vertices_[liga_set_ମୌଳିକ_id].obj ();
      clear_virtual_links(c, liga_set_ମୌଳିକ_id);
      TRY(add_virtual_link(c, liga_set_ମୌଳିକ_id, coverage_ମୌଳିକ_id));
      ପାଇଁ (ଅଚଳ auto& l : liga_set_ମୌଳିକ.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(add_virtual_link(c, l.objidx, coverage_ମୌଳିକ_id));
      }

      ଗଣନା_2 += ଅଙ୍କ_ligas;
    }

    c.graph.vertices_[liga_subst_ମୌଳିକ_id].shrink_buffer ((liga_subst_ମୌଳିକ->ligatureSet.len - liga_set_ଗଣନା) * SmallTypes::size);
    liga_subst_ମୌଳିକ->ligatureSet.len = liga_set_ଗଣନା;

    TRY (Coverage::filter_coverage (c,
                                    coverage_ମୌଳିକ_id,
                                    liga_set_start, liga_set_end + 1));

    ଫେରନ୍ତୁ Ok(liga_subst_ମୌଳିକ_id);
  }

  graph_result_t<ଶୂନ୍ୟ> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_index,
                               unsigned old_ଗଣନା,
                               hb_vector_t<unsigned> liga_counts,
                               unsigned ଗଣନା_2)
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_index,
               ଗଣନା_2);
    ଯଦି (ଗଣନା_2 >= old_ଗଣନା)
      ଫେରନ୍ତୁ Ok();

    hb_set_t retained_indices;
    unsigned new_liga_set_ଗଣନା = 0;
    ପାଇଁ (unsigned i = 0; i < liga_counts.length; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_index, &ligatureSet[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_set.index);
      retained_indices.add(liga_set.index);

      TRY_ASSIGN (auto index_to_id, ligature_index_to_object_id (liga_set));

      ପାଇଁ (unsigned i = 0; i < liga_set.table->ligature.len; i++) {
        unsigned liga_index = index_to_id[i];
        ଯଦି (liga_index != HB_GRAPH_INVALID) {
          clear_virtual_links(c, liga_index);
          retained_indices.add(liga_index);
        }
      }

      unsigned ଅଙ୍କ_ligas = liga_counts[i];
      ଯଦି (ଅଙ୍କ_ligas >= ଗଣନା_2) {
        // drop the trailing liga's from this set and all subsequent liga sets
        unsigned ଅଙ୍କ_ligas_to_remove = ଅଙ୍କ_ligas - ଗଣନା_2;
        new_liga_set_ଗଣନା = i + 1;
        c.graph.vertices_[liga_set.index].shrink_buffer (ଅଙ୍କ_ligas_to_remove * SmallTypes::size);
        liga_set.table->ligature.len = ଗଣନା_2;
        ଭାଙ୍ଗ;
      } ଅନ୍ୟଥା {
        ଗଣନା_2 -= ଅଙ୍କ_ligas;
      }
    }

    // Adjust liga set array
    auto& this_vertex = c.graph.vertices_[this_index];
    this_vertex.shrink_buffer ((ligatureSet.len - new_liga_set_ଗଣନା) * SmallTypes::size);
    ligatureSet.len = new_liga_set_ଗଣନା;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ASSIGN (unsigned coverage_idx, c.graph.index_for_offset (this_index, &ଏହି->coverage));

    auto& coverage_v = c.graph.vertices_[coverage_idx];
    unsigned coverage_size = coverage_v.table_size ();
    ଅଚଳ Coverage* coverage_table = (ଅଚଳ Coverage*) coverage_v.obj ().head;

    ଯଦି (coverage_v.is_shared ())
    {
      TRY_ASSIGN (coverage_idx, c.graph.remap_child (this_index, coverage_idx));
    }

    ପାଇଁ (unsigned i : retained_indices.iter())
      TRY(add_virtual_link(c, i, coverage_idx));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          ଫେରନ୍ତୁ p.second < new_liga_set_ଗଣନା;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    ଫେରନ୍ତୁ Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_size);
  }
};

ସଂରଚନା LigatureSubst : ସର୍ବସାଧାରଣ OT::Layout::GSUB_impl::LigatureSubst
{

  graph_result_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_index)
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1:
      hb_barrier ();
      ଫେରନ୍ତୁ ((LigatureSubstFormat1*)(&u.format1))->split_subtables (c, this_index);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_result_t<ଶୂନ୍ୟ> sanitize (ଅଚଳ graph_t::vertex_t& vertex) ଅଚଳ
  {
    size_t vertex_len = vertex.table_size ();
    ଯଦି (unlikely (vertex_len < u.format.v.get_size ())) ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    hb_barrier ();

    ବାଛ (u.format.v) {
    ସ୍ଥିତି 1:
      hb_barrier ();
      ଫେରନ୍ତୁ ((LigatureSubstFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 2:  HB_FALLTHROUGH;
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      // We don't handle format 2 here.
      ଫେରନ୍ତୁ Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_LIGATURE_GRAPH_HH
