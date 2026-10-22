/*
 * Copyright © 2020  Google, Inc.
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

#ifndef HB_REPACKER_HH
#define HB_REPACKER_HH

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-map.hh"
#অন্তর্ভাব "hb-vector.hh"
#অন্তর্ভাব "graph/graph.hh"
#অন্তর্ভাব "graph/gsubgpos-graph.hh"
#অন্তর্ভাব "graph/serialize.hh"

using graph::graph_t;
using graph::graph_ফলাফল_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

কাঠামো lookup_আকার_t
{
  unsigned lookup_সূচক;
  size_t আকার_2;
  unsigned অঙ্ক_subtables;

  স্থির পূর্ণসংখ্যা cmp (ধ্রুবক শূন্য* a, ধ্রুবক শূন্য* b)
  {
    ফেরত cmp ((ধ্রুবক lookup_আকার_t*) a,
                (ধ্রুবক lookup_আকার_t*) b);
  }

  স্থির পূর্ণসংখ্যা cmp (ধ্রুবক lookup_আকার_t* a, ধ্রুবক lookup_আকার_t* b)
  {
    দ্বিগুণ subtables_per_byte_a = (দ্বিগুণ) a->অঙ্ক_subtables / (দ্বিগুণ) a->আকার_2;
    দ্বিগুণ subtables_per_byte_b = (দ্বিগুণ) b->অঙ্ক_subtables / (দ্বিগুণ) b->আকার_2;
    যদি (subtables_per_byte_a == subtables_per_byte_b) {
      ফেরত b->lookup_সূচক - a->lookup_সূচক;
    }

    দ্বিগুণ cmp = subtables_per_byte_b - subtables_per_byte_a;
    যদি (cmp < 0) ফেরত -1;
    যদি (cmp > 0) ফেরত 1;
    ফেরত 0;
  }
};

স্থির অন্তঃরেখ
graph_ফলাফল_t<শূন্য> _presplit_subtables_if_needed (graph::gsubgpos_graph_context_t& ext_context)
{
  // For each lookup this will check the size of subtables and split them as needed
  // so that no subtable is at risk of overflowing. (where we support splitting for
  // that subtable type).
  //
  // TODO(grieger): de-dup newly added nodes as necessary. Probably just want a full de-dup
  //                pass after this processing is done. Not super necessary as splits are
  //                only done where overflow is likely, so de-dup probably will get undone
  //                later anyways.

  // The loop below can modify the contents of ext_context.lookups if new subtables are added
  // to a lookup during a split. So save the initial set of lookup indices so the iteration doesn't
  // risk access free'd memory if ext_context.lookups gets resized.
  hb_set_t lookup_indices(ext_context.lookups.চাবিগুলি ());
  জন্য (unsigned lookup_সূচক : lookup_indices)
  {
    graph::Lookup* lookup = ext_context.lookups.get(lookup_সূচক);
    TRY (lookup->split_subtables_if_needed (ext_context, lookup_সূচক));
  }

  ফেরত Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
স্থির অন্তঃরেখ
graph_ফলাফল_t<শূন্য> _promote_extensions_if_needed (graph::gsubgpos_graph_context_t& ext_context)
{
  // Simple Algorithm (v1, current):
  // 1. Calculate how many bytes each non-extension lookup consumes.
  // 2. Select up to 64k of those to remain as non-extension (greedy, highest subtables per byte first)
  // 3. Promote the rest.
  //
  // Advanced Algorithm (v2, not implemented):
  // 1. Perform connected component analysis using lookups as roots.
  // 2. Compute size of each connected component.
  // 3. Select up to 64k worth of connected components to remain as non-extensions.
  //    (greedy, highest subtables per byte first)
  // 4. Promote the rest.

  // TODO(garretrieger): support extension demotion, then consider all lookups. Requires advanced algo.
  // TODO(garretrieger): also support extension promotion during iterative resolution phase, then
  //                     we can use a less conservative threshold here.
  // TODO(grieger): skip this for the 24 bit case.
  যদি (!ext_context.lookups) ফেরত Ok();

  unsigned যোগ_lookup_table_sizes = 0;
  hb_vector_t<lookup_আকার_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), সত্য);

  জন্য (unsigned lookup_সূচক : ext_context.lookups.চাবিগুলি ())
  {
    ধ্রুবক auto& lookup_v = ext_context.graph.vertices_[lookup_সূচক];
    যোগ_lookup_table_sizes += lookup_v.table_আকার ();

    ধ্রুবক graph::Lookup* lookup = ext_context.lookups.get(lookup_সূচক);
    hb_set_t visited;
    lookup_sizes.push (lookup_আকার_t {
        lookup_সূচক,
        ext_context.graph.find_subgraph_আকার (lookup_সূচক, visited).মান_or (0),
        lookup->সংখ্যা_of_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_আকার = ext_context.graph.vertices_[ext_context.lookup_list_সূচক].table_আকার ();
  size_t l2_l3_আকার = lookup_list_আকার + যোগ_lookup_table_sizes; // Lookup List + Lookups
  size_t l3_l4_আকার = যোগ_lookup_table_sizes; // Lookups + SubTables
  size_t l4_plus_আকার = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  জন্য (auto p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    unsigned subtables_আকার = p.অঙ্ক_subtables * 8;
    l3_l4_আকার += subtables_আকার;
    l4_plus_আকার += subtables_আকার;
  }

  যৌক্তিক layers_full = মিথ্যা;
  জন্য (auto p : lookup_sizes)
  {
    ধ্রুবক graph::Lookup* lookup = ext_context.lookups.get(p.lookup_সূচক);
    যদি (lookup->is_extension (ext_context.table_tag))
      // already an extension so size is counted by the loop above.
      চালু;

    যদি (!layers_full)
    {
      size_t lookup_আকার = ext_context.graph.vertices_[p.lookup_সূচক].table_আকার ();
      hb_set_t visited;
      size_t subtables_আকার = ext_context.graph.find_subgraph_আকার (p.lookup_সূচক, visited, 1).মান_or (0) - lookup_আকার;
      size_t remaining_আকার = p.আকার_2 - subtables_আকার - lookup_আকার;

      l3_l4_আকার   += subtables_আকার;
      l3_l4_আকার   -= p.অঙ্ক_subtables * 8;
      l4_plus_আকার += subtables_আকার + remaining_আকার;

      যদি (l2_l3_আকার < (1 << 16)
          && l3_l4_আকার < (1 << 16)
          && l4_plus_আকার < (1 << 16)) চালু; // this lookup fits within all layers groups

      layers_full = সত্য;
    }

    TRY (ext_context.lookups.get(p.lookup_সূচক)->make_extension (ext_context, p.lookup_সূচক));
  }

  ফেরত Ok();
}

স্থির অন্তঃরেখ
graph_ফলাফল_t<যৌক্তিক> _try_isolating_subgraphs (ধ্রুবক hb_vector_t<graph::overflow_record_t>& overflows,
                                               graph_t& sorted_graph)
{
  unsigned space = 0;
  hb_set_t roots_to_isolate;

  জন্য (পূর্ণসংখ্যা i = overflows.দৈর্ঘ্য - 1; i >= 0; i--)
  {
    ধ্রুবক graph::overflow_record_t& r = overflows[i];

    unsigned মূল;
    unsigned overflow_space = sorted_graph.space_for (r.parent, &মূল);
    যদি (!overflow_space) চালু;
    যদি (sorted_graph.অঙ্ক_roots_for_space_2 (overflow_space) <= 1) চালু;

    যদি (!space) {
      space = overflow_space;
    }

    যদি (space == overflow_space)
      roots_to_isolate.add(মূল);
  }

  যদি (!roots_to_isolate) ফেরত Ok(মিথ্যা);

  unsigned maximum_to_move = hb_max ((sorted_graph.অঙ্ক_roots_for_space_2 (space) / 2u), 1u);
  যদি (roots_to_isolate.get_population () > maximum_to_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    পূর্ণসংখ্যা extra = roots_to_isolate.get_population () - maximum_to_move;
    জন্য (unsigned id : sorted_graph.ordering_) {
      যদি (!extra) বিরতি;
      যদি (roots_to_isolate.has(id)) {
        roots_to_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_বার্তা (SUBSET_REPACK, nullptr,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_graph.অঙ্ক_roots_for_space_2 (space),
             roots_to_isolate.get_population (),
             sorted_graph.পরবর্তী_space ());
  TRY (sorted_graph.isolate_subgraph (roots_to_isolate));
  TRY (sorted_graph.move_to_new_space (roots_to_isolate));

  ফেরত Ok(সত্য);
}

স্থির অন্তঃরেখ
graph_ফলাফল_t<যৌক্তিক> _resolve_shared_overflow(ধ্রুবক hb_vector_t<graph::overflow_record_t>& overflows,
                                              পূর্ণসংখ্যা overflow_সূচক,
                                              graph_t& sorted_graph)
{
  ধ্রুবক graph::overflow_record_t& r = overflows[overflow_সূচক];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_set_t parents;
  parents.add(r.parent);
  জন্য (পূর্ণসংখ্যা i = overflow_সূচক - 1; i >= 0; i--) {
    ধ্রুবক graph::overflow_record_t& r2 = overflows[i];
    যদি (r2.child == r.child) {
      parents.add(r2.parent);
    }
  }

  auto ফলাফল = sorted_graph.duplicate(&parents, r.child);
  যদি (!ফলাফল.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    ফলাফল = sorted_graph.duplicate(&parents, r.child);
  }

  যদি (!ফলাফল.is_ok ()) ফেরত Ok(মিথ্যা);

  যদি (parents.get_population() > 1) {
    // If the duplicated node has more than one parent pre-emptively raise it's priority to the maximum.
    // This will place it close to the parents. Node's with only one parent, don't need this as normal overflow
    // resolution will raise priority if needed.
    //
    // Reasoning: most of the parents to this child are likely at the same layer in the graph. Duplicating
    // the child will theoretically allow it to be placed closer to it's parents. However, due to the shortest
    // distance sort by default it's placement will remain in the same layer, thus it will remain in roughly the
    // same position (and distance from parents) as the original child node. The overflow resolution will attempt
    // to move nodes closer, but only for non-shared nodes. Since this node is shared, it will simply be given
    // further duplication which defeats the attempt to duplicate with multiple parents. To fix this we
    // pre-emptively raise priority now which allows the duplicated node to pack into the same layer as it's parents.
    sorted_graph.vertices_[*ফলাফল].give_max_priority();
  }

  ফেরত Ok(সত্য);
}

স্থির অন্তঃরেখ
graph_ফলাফল_t<যৌক্তিক> _process_overflows (ধ্রুবক hb_vector_t<graph::overflow_record_t>& overflows,
                                         hb_set_t& priority_bumped_parents,
                                         graph_t& sorted_graph)
{
  যৌক্তিক resolution_attempted = মিথ্যা;

  // Try resolving the furthest overflows first.
  জন্য (পূর্ণসংখ্যা i = overflows.দৈর্ঘ্য - 1; i >= 0; i--)
  {
    ধ্রুবক graph::overflow_record_t& r = overflows[i];
    যদি (sorted_graph.vertices_[r.child].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_ASSIGN (যৌক্তিক resolved, _resolve_shared_overflow(overflows, i, sorted_graph));
      যদি (resolved)
        ফেরত Ok(সত্য);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    যদি (sorted_graph.vertices_[r.child].is_leaf () && !priority_bumped_parents.has (r.parent))
    {
      // This object is too far from it's parent, attempt to move it closer.
      //
      // TODO(garretrieger): initially limiting this to leaf's since they can be
      //                     moved closer with fewer consequences. However, this can
      //                     likely can be used for non-leafs as well.
      // TODO(garretrieger): also try lowering priority of the parent. Make it
      //                     get placed further up in the ordering, closer to it's children.
      //                     this is probably preferable if the total size of the parent object
      //                     is < then the total size of the children (and the parent can be moved).
      //                     Since in that case moving the parent will cause a smaller increase in
      //                     the length of other offsets.
      TRY_ASSIGN (যৌক্তিক raised, sorted_graph.raise_childrens_priority (r.parent));
      যদি (raised) {
        priority_bumped_parents.add (r.parent);
        resolution_attempted = সত্য;
      }
      চালু;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  ফেরত Ok(resolution_attempted);
}

অন্তঃরেখ graph_ফলাফল_t<শূন্য>
_assign_spaces_and_sort (graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Assigning spaces to 32 bit subgraphs.");
  TRY_ASSIGN (যৌক্তিক assigned, sorted_graph.assign_spaces ());
  যদি (assigned)
    ফেরত sorted_graph.sort_shortest_distance ();
  অন্যথা
    ফেরত sorted_graph.sort_shortest_distance_if_needed ();
}

অন্তঃরেখ graph_ফলাফল_t<শূন্য>
_gsub_gpos_specialization (hb_tag_t table_tag,
                           যৌক্তিক always_recalculate_extensions,
                           graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Applying GSUB/GPOS repacking specializations.");
  যদি (!always_recalculate_extensions) ফেরত _assign_spaces_and_sort (sorted_graph);

  auto context_res = graph::gsubgpos_graph_context_t::create (table_tag, sorted_graph);

  যদি (unlikely (context_res.is_err())) {
    যদি (context_res.error() == graph::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      ফেরত _assign_spaces_and_sort (sorted_graph);
    অন্যথা
      // All other errors are propagated.
      ফেরত Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  auto& ext_context = *context_res;
  DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_graph.sort_shortest_distance_if_needed ());

  TRY_ASSIGN (যৌক্তিক will_overflow, graph::will_overflow (sorted_graph));
  যদি (!will_overflow) ফেরত Ok();
  ফেরত _assign_spaces_and_sort (sorted_graph);
}

অন্তঃরেখ graph_ফলাফল_t<শূন্য>
hb_resolve_graph_overflows (hb_tag_t table_tag,
                            unsigned max_rounds ,
                            যৌক্তিক always_recalculate_extensions,
                            graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Repacking %c%c%c%c.", HB_UNTAG(table_tag));
  TRY (sorted_graph.sort_shortest_distance ());

  TRY_ASSIGN (যৌক্তিক will_overflow, graph::will_overflow (sorted_graph));
  যদি (!will_overflow)
    ফেরত Ok();

  যৌক্তিক is_gsub_or_gpos = (table_tag == HB_OT_TAG_GPOS ||  table_tag == HB_OT_TAG_GSUB);
  যদি (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (table_tag, always_recalculate_extensions, sorted_graph));

  unsigned round = 0;
  unsigned যোগ_iterations = 0;
  hb_vector_t<graph::overflow_record_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  যখন (round < max_rounds && যোগ_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_ASSIGN (যৌক্তিক overflows_exist, graph::will_overflow (sorted_graph, &overflows));
    যদি (!overflows_exist)
      বিরতি;

    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_graph, overflows);

    যোগ_iterations++;
    hb_set_t priority_bumped_parents;

    TRY_ASSIGN (যৌক্তিক isolated, _try_isolating_subgraphs (overflows, sorted_graph));
    যদি (!isolated)
    {
      round++;
      TRY_ASSIGN (যৌক্তিক processed, _process_overflows (overflows, priority_bumped_parents, sorted_graph));
      যদি (!processed)
      {
        DEBUG_বার্তা (SUBSET_REPACK, nullptr, "No resolution available :(");
        বিরতি;
      }
    }

    TRY (sorted_graph.sort_shortest_distance ());
  }

  TRY_ASSIGN (যৌক্তিক has_overflow, graph::will_overflow (sorted_graph));
  যদি (unlikely (has_overflow))
  {
    যদি (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      ফেরত hb_resolve_graph_overflows (table_tag, max_rounds, সত্য, sorted_graph);
    }

    DEBUG_বার্তা (SUBSET_REPACK, nullptr, "Offset overflow resolution failed.");
    ফেরত Err(graph::OVERFLOW_RESOLUTION_FAILED);
  }

  ফেরত Ok();
}

/*
 * Attempts to modify the topological sorting of the provided object graph to
 * eliminate offset overflows in the links between objects of the graph. If a
 * non-overflowing ordering is found the updated graph is serialized it into the
 * provided serialization context.
 *
 * If necessary the structure of the graph may be modified in ways that do not
 * affect the functionality of the graph. For example shared objects may be
 * duplicated.
 *
 * For a detailed writeup describing how the algorithm operates see:
 * docs/repacker.md
 */
template<typename T>
অন্তঃরেখ hb_blob_t*
hb_resolve_overflows (ধ্রুবক T& packed,
                      hb_tag_t table_tag,
                      unsigned max_rounds = 32,
                      যৌক্তিক recalculate_extensions = মিথ্যা) {
  auto graph_res = graph_t::create (packed);
  যদি (!graph_res.is_ok ())
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "Graph creation failed, cause: %s", graph::to_string(graph_res.error()));
    ফেরত nullptr;
  }
  graph_t sorted_graph = std::move (*graph_res);

  auto resolve_res = hb_resolve_graph_overflows (table_tag, max_rounds, recalculate_extensions, sorted_graph);
  যদি (resolve_res.is_err ())
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "Overflow resolution failed, cause: %s", graph::to_string(resolve_res.error()));
    ফেরত nullptr;
  }

  auto serialize_res = graph::serialize (sorted_graph);
  যদি (serialize_res.is_err ())
  {
    DEBUG_বার্তা (SUBSET_REPACK, nullptr,
               "Serialization failed, cause: %s", graph::to_string(serialize_res.error()));
    ফেরত nullptr;
  }

  ফেরত *serialize_res;
}

#endif /* HB_REPACKER_HH */
