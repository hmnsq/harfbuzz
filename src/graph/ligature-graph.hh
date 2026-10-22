
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

#অন্তর্ভাব "graph.hh"
#অন্তর্ভাব "../OT/Layout/GSUB/LigatureSubst.hh"
#অন্তর্ভাব "../OT/Layout/GSUB/LigatureSubstFormat1.hh"
#অন্তর্ভাব "../OT/Layout/GSUB/LigatureSet.hh"
#অন্তর্ভাব "../hb-ot-dual.hh"
#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <utility>

নামাকাশ graph {

কাঠামো LigatureSet : সর্বজনীন OT::Layout::GSUB_impl::LigatureSet<SmallTypes>
{
  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    যদি (unlikely (vertex_len < OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    size_t যোগ_len = ligature.get_আকার() + OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_আকার - ligature.len.get_আকার();
    যদি (unlikely (vertex_len < যোগ_len)) {
      ফেরত Err(SANITIZE_FAILURE);
    }
    ফেরত Ok();
  }
};

কাঠামো LigatureSubstFormat1 : সর্বজনীন OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>
{
  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    unsigned min_আকার = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_আকার;
    যদি (unlikely (vertex_len < min_আকার)) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    যদি (unlikely (vertex_len < min_আকার + ligatureSet.get_আকার() - ligatureSet.len.get_আকার()))
      ফেরত Err(SANITIZE_FAILURE);
    ফেরত Ok();
  }

  graph_ফলাফল_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_সূচক)
  {
    TRY_ASSIGN (auto split_points, compute_split_points(c, this_সূচক));
    যদি (!split_points)
      ফেরত Ok(hb_vector_t<unsigned> ());

    TRY_ASSIGN (unsigned যোগ_ligas, যোগ_সংখ্যা_ligas(c, this_সূচক));
    TRY_ASSIGN (hb_vector_t<unsigned> counts, liga_counts(c, this_সূচক));
    split_context_t split_context {
      c,
      এই,
      this_সূচক,
      যোগ_ligas,
      std::move (counts),
    };
    ফেরত actuate_subtable_split<split_context_t> (split_context, split_points);
  }

 ব্যক্তিগত:
  graph_ফলাফল_t<unsigned> যোগ_সংখ্যা_ligas(gsubgpos_graph_context_t& c, unsigned this_সূচক) ধ্রুবক {
    unsigned যোগ = 0;
    জন্য (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_সূচক, &ligatureSet[i]));
      যোগ += liga_set.table->ligature.len;
    }
    ফেরত Ok(যোগ);
  }

  graph_ফলাফল_t<hb_vector_t<unsigned>> liga_counts(gsubgpos_graph_context_t& c, unsigned this_সূচক) ধ্রুবক {
    hb_vector_t<unsigned> ফলাফল;
    জন্য (unsigned i = 0; i < ligatureSet.len; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_সূচক, &ligatureSet[i]));
      ফলাফল.push(!liga_set.table ? 0 : liga_set.table->ligature.len);
    }
    ফেরত graph_ফলাফল_t<hb_vector_t<unsigned>>::from(std::move(ফলাফল), ALLOCATION_FAILURE);
  }

  template <graph_t::vertex_mutability_t mutability>
  graph_ফলাফল_t<hb_vector_t<unsigned>> ligature_সূচক_to_object_id(ধ্রুবক graph_t::vertex_and_table_t<LigatureSet, mutability>& liga_set) ধ্রুবক {
    hb_vector_t<unsigned> map;
    যদি (!map.resize_exact(liga_set.table->ligature.len)) ফেরত Err(ALLOCATION_FAILURE);

    জন্য (unsigned i = 0; i < map.দৈর্ঘ্য; i++) {
      map[i] = HB_GRAPH_INVALID;
    }

    জন্য (ধ্রুবক auto& l : liga_set.vertex->obj ().real_links) {
      যদি (l.position < 2) চালু;
      unsigned array_সূচক = (l.position - 2) / 2;
      map[array_সূচক] = l.objidx;
    }
    ফেরত map;
  }

  graph_ফলাফল_t<hb_vector_t<unsigned>> compute_split_points(gsubgpos_graph_context_t& c,
                                                             unsigned this_সূচক) ধ্রুবক
  {
    // For ligature subst coverage is always packed last, and as a result is where an overflow
    // will happen if there is one, so we can check the estimate length of the
    // LigatureSubstFormat1 -> Coverage offset length which is the sum of all data in the
    // retained sub graph except for the coverage table itself.
    ধ্রুবক unsigned base_আকার = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_আকার;
    unsigned accumulated = base_আকার;

    unsigned ligature_সূচক = 0;
    hb_vector_t<unsigned> split_points;
    জন্য (unsigned i = 0; i < ligatureSet.len; i++)
    {
      accumulated += OT::HBUINT16::static_আকার; // for ligature set offset
      accumulated += OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_আকার; // for ligature set table

      TRY_ASSIGN (auto liga_set, c.graph.as_table<LigatureSet>(this_সূচক, &ligatureSet[i]));

      // Finding the object id associated with an array index is O(n)
      // so to avoid O(n^2), precompute the mapping by scanning through
      // all links
      TRY_ASSIGN (auto সূচক_to_id, ligature_সূচক_to_object_id (liga_set));

      জন্য (unsigned j = 0; j < liga_set.table->ligature.len; j++)
      {
        ধ্রুবক unsigned liga_id = সূচক_to_id[j];
        যদি (liga_id == HB_GRAPH_INVALID) চালু; // no outgoing link, ignore
        ধ্রুবক unsigned liga_আকার = c.graph.vertices_[liga_id].table_আকার ();

        accumulated += OT::HBUINT16::static_আকার; // for ligature offset
        accumulated += liga_আকার; // for the ligature table

        যদি (accumulated >= (1 << 16))
        {
          split_points.push(ligature_সূচক);
          // We're going to split such that the current ligature will be in the new sub table.
          // That means we'll have one ligature subst (base_base), one ligature set, and one liga table
          accumulated = base_আকার + // for liga subst subtable
            (OT::HBUINT16::static_আকার * 2) + // for liga set and liga offset
            OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_আকার + // for liga set subtable
            liga_আকার; // for liga sub table
        }

        ligature_সূচক++;
      }
    }

    ফেরত split_points;
  }

  কাঠামো split_context_t
  {
    gsubgpos_graph_context_t& c;
    LigatureSubstFormat1* thiz;
    unsigned this_সূচক;
    unsigned original_গুনতি;
    hb_vector_t<unsigned> liga_counts;

    unsigned original_গুনতি_2 ()
    {
      ফেরত original_গুনতি;
    }

    graph_ফলাফল_t<unsigned> clone_range (unsigned start, unsigned end)
    {
      ফেরত thiz->clone_range (c, this_সূচক, liga_counts, start, end);
    }

    graph_ফলাফল_t<শূন্য> shrink (unsigned গুনতি)
    {
      ফেরত thiz->shrink (c, this_সূচক, original_গুনতি_2(), liga_counts, গুনতি);
    }
  };

  graph_ফলাফল_t<hb_pair_t<unsigned, LigatureSet*>> new_liga_set(gsubgpos_graph_context_t& c, unsigned গুনতি) ধ্রুবক {
    unsigned মৌলিক_আকার = OT::Layout::GSUB_impl::LigatureSet<SmallTypes>::min_আকার
                          + গুনতি * SmallTypes::আকার_2;

    TRY_ASSIGN (unsigned মৌলিক_id, c.create_গ্রন্থি (মৌলিক_আকার));

    LigatureSet* মৌলিক = (LigatureSet*) c.graph.object (মৌলিক_id).শীর্ষ_2;
    মৌলিক->ligature.len = গুনতি;
    ফেরত Ok(hb_pair(মৌলিক_id, মৌলিক));
  }

  শূন্য clear_virtual_links (gsubgpos_graph_context_t& c, unsigned গ্রন্থি_সূচক) ধ্রুবক
  {
    auto& v  = c.graph.vertices_[গ্রন্থি_সূচক];
    জন্য (ধ্রুবক auto& l : v.obj ().virtual_links)
    {
      auto& child = c.graph.vertices_[l.objidx];
      child.remove_parent(গ্রন্থি_সূচক);
    }
    v.clear_virtual_links ();
  }

  graph_ফলাফল_t<শূন্য> add_virtual_link(gsubgpos_graph_context_t& c, unsigned from, unsigned to) ধ্রুবক {
    যদি (unlikely (from >= c.graph.vertices_.দৈর্ঘ্য || to >= c.graph.vertices_.দৈর্ঘ্য)) ফেরত Err(OUT_OF_BOUNDS);
    TRY(c.graph.vertices_[to].add_parent (from, সত্য));
    TRY(c.graph.vertices_[from].add_virtual_link (to));
    ফেরত Ok();
  }

  hb_pair_t<unsigned, unsigned> current_liga_set_bounds (gsubgpos_graph_context_t& c,
                                                         unsigned liga_set_সূচক,
                                                         ধ্রুবক hb_serialize_context_t::object_t& liga_set) ধ্রুবক
  {
    // Finds the actual liga indices present in the liga set currently. Takes
    // into account those that have been removed by processing.
    unsigned min_সূচক = HB_GRAPH_INVALID;
    unsigned max_সূচক = 0;
    জন্য (ধ্রুবক auto& l : liga_set.real_links) {
      যদি (l.position < 2) চালু;

      unsigned liga_সূচক = (l.position - 2) / 2;
      min_সূচক = hb_min(min_সূচক, liga_সূচক);
      max_সূচক = hb_max(max_সূচক, liga_সূচক);
    }
    ফেরত hb_pair(min_সূচক, max_সূচক + 1);
  }

  শূন্য compact_liga_set (gsubgpos_graph_context_t& c, LigatureSet* table, graph_t::vertex_t* v) ধ্রুবক
  {
    ধ্রুবক auto& obj = v->obj ();
    যদি (table->ligature.len <= obj.real_links.দৈর্ঘ্য) ফেরত;

    // compact the remaining linked liga offsets into a continous array and shrink the node as needed.
    unsigned to_remove = table->ligature.len - obj.real_links.দৈর্ঘ্য;
    unsigned new_position = SmallTypes::আকার_2;
    v->sort_real_links (); // for this to work we need to process links in order of position.
    জন্য (auto& l : v->real_links_writer ())
    {
      l.position = new_position;
      new_position += SmallTypes::আকার_2;
    }

    table->ligature.len = obj.real_links.দৈর্ঘ্য;
    v->shrink_বাফার (to_remove * SmallTypes::আকার_2);
  }

  graph_ফলাফল_t<unsigned> clone_range (gsubgpos_graph_context_t& c,
                                        unsigned this_সূচক,
                                        hb_vector_t<unsigned> liga_counts,
                                        unsigned start, unsigned end) ধ্রুবক
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Cloning LigatureSubstFormat1 (%u) range [%u, %u).", this_সূচক, start, end);

    // Create an oversized new liga subst, we'll adjust the size down later. We don't know
    // the final size until we process it but we also need it to exist while we're processing
    // so that nodes can be moved to it as needed.
    unsigned মৌলিক_আকার = OT::Layout::GSUB_impl::LigatureSubstFormat1_2<SmallTypes>::min_আকার
                          + ligatureSet.get_আকার() - ligatureSet.len.get_আকার();

    TRY_ASSIGN (unsigned liga_subst_মৌলিক_id, c.create_গ্রন্থি (মৌলিক_আকার));

    LigatureSubstFormat1* liga_subst_মৌলিক = (LigatureSubstFormat1*) c.graph.object (liga_subst_মৌলিক_id).শীর্ষ_2;
    liga_subst_মৌলিক->format = এই->format;
    liga_subst_মৌলিক->ligatureSet.len = এই->ligatureSet.len;

    // Create a place holder coverage prime id since we need to add virtual links to it while
    // generating liga and liga sets. Afterwards it will be updated to have the correct coverage.
    TRY_ASSIGN (unsigned coverage_id, c.graph.সূচক_for_offset (this_সূচক, &coverage));
    TRY_ASSIGN (unsigned coverage_মৌলিক_id, c.graph.duplicate(coverage_id));
    auto& coverage_মৌলিক_vertex = c.graph.vertices_[coverage_মৌলিক_id];

    TRY(c.graph.vertices_[liga_subst_মৌলিক_id].add_real_link (SmallTypes::আকার_2, coverage_মৌলিক_id, 2));
    TRY(coverage_মৌলিক_vertex.add_parent (liga_subst_মৌলিক_id, মিথ্যা));

    // Locate all liga sets with ligas between start and end.
    // Clone or move them as needed.
    unsigned গুনতি = 0;
    unsigned liga_set_গুনতি = 0;
    unsigned liga_set_start = HB_GRAPH_INVALID;
    unsigned liga_set_end = 0; // inclusive
    জন্য (unsigned i = 0; i < liga_counts.দৈর্ঘ্য; i++)
    {
      unsigned অঙ্ক_ligas = liga_counts[i];

      unsigned current_start = গুনতি;
      unsigned current_end = গুনতি + অঙ্ক_ligas;

      যদি (current_start >= end || start >= current_end) {
        // No intersection, so just skip
        গুনতি += অঙ্ক_ligas;
        চালু;
      }

      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_সূচক, &ligatureSet[i]));

      unsigned liga_set_সূচক = liga_set.সূচক_2;
      // Bounds may need to be adjusted if some ligas have been previously removed.
      hb_pair_t<unsigned, unsigned> liga_bounds = current_liga_set_bounds(c, liga_set_সূচক, liga_set.vertex->obj ());
      current_start = hb_max(গুনতি + liga_bounds.first, current_start);
      current_end = hb_min(গুনতি + liga_bounds.second, current_end);

      unsigned liga_set_মৌলিক_id;
      যদি (current_start >= start && current_end <= end) {
        // This liga set is fully contined within [start, end)
        // We can move the entire ligaset to the new liga subset object.
        liga_set_end = i;
        যদি (i < liga_set_start) liga_set_start = i;
        TRY_ASSIGN (liga_set_মৌলিক_id, c.graph.move_child<> (this_সূচক,
                              &ligatureSet[i],
                              liga_subst_মৌলিক_id,
                              &liga_subst_মৌলিক->ligatureSet[liga_set_গুনতি++]));
        compact_liga_set (c, liga_set.table, liga_set.vertex);
      }
      অন্যথা
      {
        // This liga set partially overlaps [start, end). We'll need to create
        // a new liga set sub table and move the intersecting ligas to it.
        unsigned start_সূচক = hb_max(start, current_start) - গুনতি;
        unsigned end_সূচক = hb_min(end, current_end) - গুনতি;
        unsigned liga_গুনতি = end_সূচক - start_সূচক;
        TRY_ASSIGN (auto ফলাফল, new_liga_set(c, liga_গুনতি));
        liga_set_মৌলিক_id = ফলাফল.first;

        TRY (c.graph.move_children<OT::Offset16>(
          liga_set_সূচক,
          2 + start_সূচক * 2,
          2 + end_সূচক * 2,
          liga_set_মৌলিক_id,
          2));

        liga_set_end = i;
        যদি (i < liga_set_start) liga_set_start = i;
        TRY (c.graph.add_link(&liga_subst_মৌলিক->ligatureSet[liga_set_গুনতি++], liga_subst_মৌলিক_id, liga_set_মৌলিক_id));
      }

      // The new liga and all children set needs to have a virtual link to the new coverage table:
      ধ্রুবক auto& liga_set_মৌলিক = c.graph.vertices_[liga_set_মৌলিক_id].obj ();
      clear_virtual_links(c, liga_set_মৌলিক_id);
      TRY(add_virtual_link(c, liga_set_মৌলিক_id, coverage_মৌলিক_id));
      জন্য (ধ্রুবক auto& l : liga_set_মৌলিক.real_links) {
        clear_virtual_links(c, l.objidx);
        TRY(add_virtual_link(c, l.objidx, coverage_মৌলিক_id));
      }

      গুনতি += অঙ্ক_ligas;
    }

    c.graph.vertices_[liga_subst_মৌলিক_id].shrink_বাফার ((liga_subst_মৌলিক->ligatureSet.len - liga_set_গুনতি) * SmallTypes::আকার_2);
    liga_subst_মৌলিক->ligatureSet.len = liga_set_গুনতি;

    TRY (Coverage::filter_coverage (c,
                                    coverage_মৌলিক_id,
                                    liga_set_start, liga_set_end + 1));

    ফেরত Ok(liga_subst_মৌলিক_id);
  }

  graph_ফলাফল_t<শূন্য> shrink (gsubgpos_graph_context_t& c,
                               unsigned this_সূচক,
                               unsigned old_গুনতি,
                               hb_vector_t<unsigned> liga_counts,
                               unsigned গুনতি)
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "  Shrinking LigatureSubstFormat1 (%u) to [0, %u).",
               this_সূচক,
               গুনতি);
    যদি (গুনতি >= old_গুনতি)
      ফেরত Ok();

    hb_set_t retained_indices;
    unsigned new_liga_set_গুনতি = 0;
    জন্য (unsigned i = 0; i < liga_counts.দৈর্ঘ্য; i++)
    {
      TRY_ASSIGN (auto liga_set, c.graph.as_mutable_table<LigatureSet>(this_সূচক, &ligatureSet[i]));

      // We need the virtual links to coverage removed from all descendants on this liga subst.
      // If any are left when we try to mutate the coverage table later it will be unnessecarily
      // duplicated. Code later on will re-add the virtual links as needed (via retained_indices).
      clear_virtual_links(c, liga_set.সূচক_2);
      retained_indices.add(liga_set.সূচক_2);

      TRY_ASSIGN (auto সূচক_to_id, ligature_সূচক_to_object_id (liga_set));

      জন্য (unsigned i = 0; i < liga_set.table->ligature.len; i++) {
        unsigned liga_সূচক = সূচক_to_id[i];
        যদি (liga_সূচক != HB_GRAPH_INVALID) {
          clear_virtual_links(c, liga_সূচক);
          retained_indices.add(liga_সূচক);
        }
      }

      unsigned অঙ্ক_ligas = liga_counts[i];
      যদি (অঙ্ক_ligas >= গুনতি) {
        // drop the trailing liga's from this set and all subsequent liga sets
        unsigned অঙ্ক_ligas_to_remove = অঙ্ক_ligas - গুনতি;
        new_liga_set_গুনতি = i + 1;
        c.graph.vertices_[liga_set.সূচক_2].shrink_বাফার (অঙ্ক_ligas_to_remove * SmallTypes::আকার_2);
        liga_set.table->ligature.len = গুনতি;
        বিরতি;
      } অন্যথা {
        গুনতি -= অঙ্ক_ligas;
      }
    }

    // Adjust liga set array
    auto& this_vertex = c.graph.vertices_[this_সূচক];
    this_vertex.shrink_বাফার ((ligatureSet.len - new_liga_set_গুনতি) * SmallTypes::আকার_2);
    ligatureSet.len = new_liga_set_গুনতি;

    // Coverage matches the number of liga sets so rebuild as needed
    TRY_ASSIGN (unsigned coverage_idx, c.graph.সূচক_for_offset (this_সূচক, &এই->coverage));

    auto& coverage_v = c.graph.vertices_[coverage_idx];
    unsigned coverage_আকার = coverage_v.table_আকার ();
    ধ্রুবক Coverage* coverage_table = (ধ্রুবক Coverage*) coverage_v.obj ().শীর্ষ_2;

    যদি (coverage_v.is_shared ())
    {
      TRY_ASSIGN (coverage_idx, c.graph.remap_child (this_সূচক, coverage_idx));
    }

    জন্য (unsigned i : retained_indices.iter())
      TRY(add_virtual_link(c, i, coverage_idx));

    auto new_coverage =
        + hb_zip (coverage_table->iter (), hb_range ())
        | hb_filter ([&] (hb_pair_t<unsigned, unsigned> p) {
          ফেরত p.second < new_liga_set_গুনতি;
        })
        | hb_map_retains_sorting (hb_first)
        ;

    ফেরত Coverage::make_coverage (c, new_coverage, coverage_idx, coverage_আকার);
  }
};

কাঠামো LigatureSubst : সর্বজনীন OT::Layout::GSUB_impl::LigatureSubst
{

  graph_ফলাফল_t<hb_vector_t<unsigned>> split_subtables (gsubgpos_graph_context_t& c,
                                                         unsigned this_সূচক)
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1:
      hb_barrier ();
      ফেরত ((LigatureSubstFormat1*)(&u.format1))->split_subtables (c, this_সূচক);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 2: HB_FALLTHROUGH;
      // 24-bit LigatureSubst already has widened offsets; leave it unsplit.
#endif
    পূর্বনির্ধারিত:
      ফেরত Ok(hb_vector_t<unsigned> ());
    }
  }

  graph_ফলাফল_t<শূন্য> sanitize (ধ্রুবক graph_t::vertex_t& vertex) ধ্রুবক
  {
    size_t vertex_len = vertex.table_আকার ();
    যদি (unlikely (vertex_len < u.format.v.get_আকার ())) ফেরত Err(SANITIZE_FAILURE);
    hb_barrier ();

    নির্বাচন (u.format.v) {
    ক্ষেত্র 1:
      hb_barrier ();
      ফেরত ((LigatureSubstFormat1*)(&u.format1))->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 2:  HB_FALLTHROUGH;
#endif
    পূর্বনির্ধারিত:
      // We don't handle format 2 here.
      ফেরত Err(SANITIZE_FAILURE);
    }
  }
};

}

#endif  // GRAPH_LIGATURE_GRAPH_HH
