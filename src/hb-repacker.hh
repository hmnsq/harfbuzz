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

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector.hh"
#ଅନ୍ତର୍ଭାଵ "graph/graph.hh"
#ଅନ୍ତର୍ଭାଵ "graph/gsubgpos-graph.hh"
#ଅନ୍ତର୍ଭାଵ "graph/serialize.hh"

using graph::graph_t;
using graph::graph_result_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

ସଂରଚନା lookup_size_t
{
  unsigned lookup_index;
  size_t size;
  unsigned ଅଙ୍କ_subtables;

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ ଶୂନ୍ୟ* a, ଅଚଳ ଶୂନ୍ୟ* b)
  {
    ଫେରନ୍ତୁ cmp ((ଅଚଳ lookup_size_t*) a,
                (ଅଚଳ lookup_size_t*) b);
  }

  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ lookup_size_t* a, ଅଚଳ lookup_size_t* b)
  {
    ଦ୍ୱିଗୁଣ subtables_per_byte_a = (ଦ୍ୱିଗୁଣ) a->ଅଙ୍କ_subtables / (ଦ୍ୱିଗୁଣ) a->size;
    ଦ୍ୱିଗୁଣ subtables_per_byte_b = (ଦ୍ୱିଗୁଣ) b->ଅଙ୍କ_subtables / (ଦ୍ୱିଗୁଣ) b->size;
    ଯଦି (subtables_per_byte_a == subtables_per_byte_b) {
      ଫେରନ୍ତୁ b->lookup_index - a->lookup_index;
    }

    ଦ୍ୱିଗୁଣ cmp = subtables_per_byte_b - subtables_per_byte_a;
    ଯଦି (cmp < 0) ଫେରନ୍ତୁ -1;
    ଯଦି (cmp > 0) ଫେରନ୍ତୁ 1;
    ଫେରନ୍ତୁ 0;
  }
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
graph_result_t<ଶୂନ୍ୟ> _presplit_subtables_if_needed (graph::gsubgpos_graph_context_t& ext_context)
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
  hb_set_t lookup_indices(ext_context.lookups.ଚାବିଗୁଡ଼ିକ ());
  ପାଇଁ (unsigned lookup_index : lookup_indices)
  {
    graph::Lookup* lookup = ext_context.lookups.get(lookup_index);
    TRY (lookup->split_subtables_if_needed (ext_context, lookup_index));
  }

  ଫେରନ୍ତୁ Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
graph_result_t<ଶୂନ୍ୟ> _promote_extensions_if_needed (graph::gsubgpos_graph_context_t& ext_context)
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
  ଯଦି (!ext_context.lookups) ଫେରନ୍ତୁ Ok();

  unsigned total_lookup_table_sizes = 0;
  hb_vector_t<lookup_size_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), ସତ୍ୟ);

  ପାଇଁ (unsigned lookup_index : ext_context.lookups.ଚାବିଗୁଡ଼ିକ ())
  {
    ଅଚଳ auto& lookup_v = ext_context.graph.vertices_[lookup_index];
    total_lookup_table_sizes += lookup_v.table_size ();

    ଅଚଳ graph::Lookup* lookup = ext_context.lookups.get(lookup_index);
    hb_set_t visited;
    lookup_sizes.push (lookup_size_t {
        lookup_index,
        ext_context.graph.find_subgraph_size (lookup_index, visited).ମୂଲ୍ୟ_or (0),
        lookup->ସଂଖ୍ୟା_of_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_size = ext_context.graph.vertices_[ext_context.lookup_list_index].table_size ();
  size_t l2_l3_size = lookup_list_size + total_lookup_table_sizes; // Lookup List + Lookups
  size_t l3_l4_size = total_lookup_table_sizes; // Lookups + SubTables
  size_t l4_plus_size = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  ପାଇଁ (auto p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    unsigned subtables_size = p.ଅଙ୍କ_subtables * 8;
    l3_l4_size += subtables_size;
    l4_plus_size += subtables_size;
  }

  ତାର୍କିକ layers_full = ମିଥ୍ୟା;
  ପାଇଁ (auto p : lookup_sizes)
  {
    ଅଚଳ graph::Lookup* lookup = ext_context.lookups.get(p.lookup_index);
    ଯଦି (lookup->is_extension (ext_context.table_tag))
      // already an extension so size is counted by the loop above.
      ଜାରି;

    ଯଦି (!layers_full)
    {
      size_t lookup_size = ext_context.graph.vertices_[p.lookup_index].table_size ();
      hb_set_t visited;
      size_t subtables_size = ext_context.graph.find_subgraph_size (p.lookup_index, visited, 1).ମୂଲ୍ୟ_or (0) - lookup_size;
      size_t remaining_size = p.size - subtables_size - lookup_size;

      l3_l4_size   += subtables_size;
      l3_l4_size   -= p.ଅଙ୍କ_subtables * 8;
      l4_plus_size += subtables_size + remaining_size;

      ଯଦି (l2_l3_size < (1 << 16)
          && l3_l4_size < (1 << 16)
          && l4_plus_size < (1 << 16)) ଜାରି; // this lookup fits within all layers groups

      layers_full = ସତ୍ୟ;
    }

    TRY (ext_context.lookups.get(p.lookup_index)->make_extension (ext_context, p.lookup_index));
  }

  ଫେରନ୍ତୁ Ok();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
graph_result_t<ତାର୍କିକ> _try_isolating_subgraphs (ଅଚଳ hb_vector_t<graph::overflow_record_t>& overflows,
                                               graph_t& sorted_graph)
{
  unsigned space = 0;
  hb_set_t roots_to_isolate;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = overflows.length - 1; i >= 0; i--)
  {
    ଅଚଳ graph::overflow_record_t& r = overflows[i];

    unsigned ମୂଳ;
    unsigned overflow_space = sorted_graph.space_for (r.parent, &ମୂଳ);
    ଯଦି (!overflow_space) ଜାରି;
    ଯଦି (sorted_graph.ଅଙ୍କ_roots_for_space_2 (overflow_space) <= 1) ଜାରି;

    ଯଦି (!space) {
      space = overflow_space;
    }

    ଯଦି (space == overflow_space)
      roots_to_isolate.add(ମୂଳ);
  }

  ଯଦି (!roots_to_isolate) ଫେରନ୍ତୁ Ok(ମିଥ୍ୟା);

  unsigned maximum_to_move = hb_max ((sorted_graph.ଅଙ୍କ_roots_for_space_2 (space) / 2u), 1u);
  ଯଦି (roots_to_isolate.get_population () > maximum_to_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra = roots_to_isolate.get_population () - maximum_to_move;
    ପାଇଁ (unsigned id : sorted_graph.ordering_) {
      ଯଦି (!extra) ଭାଙ୍ଗ;
      ଯଦି (roots_to_isolate.has(id)) {
        roots_to_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_graph.ଅଙ୍କ_roots_for_space_2 (space),
             roots_to_isolate.get_population (),
             sorted_graph.next_space ());
  TRY (sorted_graph.isolate_subgraph (roots_to_isolate));
  TRY (sorted_graph.move_to_new_space (roots_to_isolate));

  ଫେରନ୍ତୁ Ok(ସତ୍ୟ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
graph_result_t<ତାର୍କିକ> _resolve_shared_overflow(ଅଚଳ hb_vector_t<graph::overflow_record_t>& overflows,
                                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା overflow_index,
                                              graph_t& sorted_graph)
{
  ଅଚଳ graph::overflow_record_t& r = overflows[overflow_index];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_set_t parents;
  parents.add(r.parent);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = overflow_index - 1; i >= 0; i--) {
    ଅଚଳ graph::overflow_record_t& r2 = overflows[i];
    ଯଦି (r2.child == r.child) {
      parents.add(r2.parent);
    }
  }

  auto result = sorted_graph.duplicate(&parents, r.child);
  ଯଦି (!result.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    result = sorted_graph.duplicate(&parents, r.child);
  }

  ଯଦି (!result.is_ok ()) ଫେରନ୍ତୁ Ok(ମିଥ୍ୟା);

  ଯଦି (parents.get_population() > 1) {
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
    sorted_graph.vertices_[*result].give_max_priority();
  }

  ଫେରନ୍ତୁ Ok(ସତ୍ୟ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
graph_result_t<ତାର୍କିକ> _process_overflows (ଅଚଳ hb_vector_t<graph::overflow_record_t>& overflows,
                                         hb_set_t& priority_bumped_parents,
                                         graph_t& sorted_graph)
{
  ତାର୍କିକ resolution_attempted = ମିଥ୍ୟା;

  // Try resolving the furthest overflows first.
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = overflows.length - 1; i >= 0; i--)
  {
    ଅଚଳ graph::overflow_record_t& r = overflows[i];
    ଯଦି (sorted_graph.vertices_[r.child].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_ASSIGN (ତାର୍କିକ resolved, _resolve_shared_overflow(overflows, i, sorted_graph));
      ଯଦି (resolved)
        ଫେରନ୍ତୁ Ok(ସତ୍ୟ);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    ଯଦି (sorted_graph.vertices_[r.child].is_leaf () && !priority_bumped_parents.has (r.parent))
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
      TRY_ASSIGN (ତାର୍କିକ raised, sorted_graph.raise_childrens_priority (r.parent));
      ଯଦି (raised) {
        priority_bumped_parents.add (r.parent);
        resolution_attempted = ସତ୍ୟ;
      }
      ଜାରି;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  ଫେରନ୍ତୁ Ok(resolution_attempted);
}

ଧାଡ଼ିଭିତରେ graph_result_t<ଶୂନ୍ୟ>
_assign_spaces_and_sort (graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Assigning spaces to 32 bit subgraphs.");
  TRY_ASSIGN (ତାର୍କିକ assigned, sorted_graph.assign_spaces ());
  ଯଦି (assigned)
    ଫେରନ୍ତୁ sorted_graph.sort_shortest_distance ();
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ sorted_graph.sort_shortest_distance_if_needed ();
}

ଧାଡ଼ିଭିତରେ graph_result_t<ଶୂନ୍ୟ>
_gsub_gpos_specialization (hb_tag_t table_tag,
                           ତାର୍କିକ always_recalculate_extensions,
                           graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Applying GSUB/GPOS repacking specializations.");
  ଯଦି (!always_recalculate_extensions) ଫେରନ୍ତୁ _assign_spaces_and_sort (sorted_graph);

  auto context_res = graph::gsubgpos_graph_context_t::create (table_tag, sorted_graph);

  ଯଦି (unlikely (context_res.is_err())) {
    ଯଦି (context_res.error() == graph::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      ଫେରନ୍ତୁ _assign_spaces_and_sort (sorted_graph);
    ଅନ୍ୟଥା
      // All other errors are propagated.
      ଫେରନ୍ତୁ Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  auto& ext_context = *context_res;
  DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_graph.sort_shortest_distance_if_needed ());

  TRY_ASSIGN (ତାର୍କିକ will_overflow, graph::will_overflow (sorted_graph));
  ଯଦି (!will_overflow) ଫେରନ୍ତୁ Ok();
  ଫେରନ୍ତୁ _assign_spaces_and_sort (sorted_graph);
}

ଧାଡ଼ିଭିତରେ graph_result_t<ଶୂନ୍ୟ>
hb_resolve_graph_overflows (hb_tag_t table_tag,
                            unsigned max_rounds ,
                            ତାର୍କିକ always_recalculate_extensions,
                            graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Repacking %c%c%c%c.", HB_UNTAG(table_tag));
  TRY (sorted_graph.sort_shortest_distance ());

  TRY_ASSIGN (ତାର୍କିକ will_overflow, graph::will_overflow (sorted_graph));
  ଯଦି (!will_overflow)
    ଫେରନ୍ତୁ Ok();

  ତାର୍କିକ is_gsub_or_gpos = (table_tag == HB_OT_TAG_GPOS ||  table_tag == HB_OT_TAG_GSUB);
  ଯଦି (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (table_tag, always_recalculate_extensions, sorted_graph));

  unsigned round = 0;
  unsigned total_iterations = 0;
  hb_vector_t<graph::overflow_record_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  ଯେତେବେଳେ (round < max_rounds && total_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_ASSIGN (ତାର୍କିକ overflows_exist, graph::will_overflow (sorted_graph, &overflows));
    ଯଦି (!overflows_exist)
      ଭାଙ୍ଗ;

    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_graph, overflows);

    total_iterations++;
    hb_set_t priority_bumped_parents;

    TRY_ASSIGN (ତାର୍କିକ isolated, _try_isolating_subgraphs (overflows, sorted_graph));
    ଯଦି (!isolated)
    {
      round++;
      TRY_ASSIGN (ତାର୍କିକ processed, _process_overflows (overflows, priority_bumped_parents, sorted_graph));
      ଯଦି (!processed)
      {
        DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "No resolution available :(");
        ଭାଙ୍ଗ;
      }
    }

    TRY (sorted_graph.sort_shortest_distance ());
  }

  TRY_ASSIGN (ତାର୍କିକ has_overflow, graph::will_overflow (sorted_graph));
  ଯଦି (unlikely (has_overflow))
  {
    ଯଦି (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      ଫେରନ୍ତୁ hb_resolve_graph_overflows (table_tag, max_rounds, ସତ୍ୟ, sorted_graph);
    }

    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Offset overflow resolution failed.");
    ଫେରନ୍ତୁ Err(graph::OVERFLOW_RESOLUTION_FAILED);
  }

  ଫେରନ୍ତୁ Ok();
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
ଧାଡ଼ିଭିତରେ hb_blob_t*
hb_resolve_overflows (ଅଚଳ T& packed,
                      hb_tag_t table_tag,
                      unsigned max_rounds = 32,
                      ତାର୍କିକ recalculate_extensions = ମିଥ୍ୟା) {
  auto graph_res = graph_t::create (packed);
  ଯଦି (!graph_res.is_ok ())
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "Graph creation failed, cause: %s", graph::to_string(graph_res.error()));
    ଫେରନ୍ତୁ nullptr;
  }
  graph_t sorted_graph = std::move (*graph_res);

  auto resolve_res = hb_resolve_graph_overflows (table_tag, max_rounds, recalculate_extensions, sorted_graph);
  ଯଦି (resolve_res.is_err ())
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "Overflow resolution failed, cause: %s", graph::to_string(resolve_res.error()));
    ଫେରନ୍ତୁ nullptr;
  }

  auto serialize_res = graph::serialize (sorted_graph);
  ଯଦି (serialize_res.is_err ())
  {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
               "Serialization failed, cause: %s", graph::to_string(serialize_res.error()));
    ଫେରନ୍ତୁ nullptr;
  }

  ଫେରନ୍ତୁ *serialize_res;
}

#endif /* HB_REPACKER_HH */
