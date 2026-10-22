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

#ضمن "hb-open-type.hh"
#ضمن "hb-map.hh"
#ضمن "hb-vector.hh"
#ضمن "graph/graph.hh"
#ضمن "graph/gsubgpos-graph.hh"
#ضمن "graph/serialize.hh"

using graph::graph_t;
using graph::graph_result_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

بنية lookup_size_t
{
  unsigned lookup_index;
  size_t size;
  unsigned عدد_subtables;

  ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
  {
    أرجع cmp ((ثابت lookup_size_t*) a,
                (ثابت lookup_size_t*) b);
  }

  ساكن صحيح cmp (ثابت lookup_size_t* a, ثابت lookup_size_t* b)
  {
    مزدوج subtables_per_byte_a = (مزدوج) a->عدد_subtables / (مزدوج) a->size;
    مزدوج subtables_per_byte_b = (مزدوج) b->عدد_subtables / (مزدوج) b->size;
    إذا (subtables_per_byte_a == subtables_per_byte_b) {
      أرجع b->lookup_index - a->lookup_index;
    }

    مزدوج cmp = subtables_per_byte_b - subtables_per_byte_a;
    إذا (cmp < 0) أرجع -1;
    إذا (cmp > 0) أرجع 1;
    أرجع 0;
  }
};

ساكن مضمن
graph_result_t<فراغ> _presplit_subtables_if_needed (graph::gsubgpos_graph_context_t& ext_context)
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
  hb_set_t lookup_indices(ext_context.lookups.کلیدیں ());
  لكل (unsigned lookup_index : lookup_indices)
  {
    graph::Lookup* lookup = ext_context.lookups.get(lookup_index);
    TRY (lookup->split_subtables_if_needed (ext_context, lookup_index));
  }

  أرجع Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
ساكن مضمن
graph_result_t<فراغ> _promote_extensions_if_needed (graph::gsubgpos_graph_context_t& ext_context)
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
  إذا (!ext_context.lookups) أرجع Ok();

  unsigned total_lookup_table_sizes = 0;
  hb_vector_t<lookup_size_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), true);

  لكل (unsigned lookup_index : ext_context.lookups.کلیدیں ())
  {
    ثابت auto& lookup_v = ext_context.graph.vertices_[lookup_index];
    total_lookup_table_sizes += lookup_v.table_size ();

    ثابت graph::Lookup* lookup = ext_context.lookups.get(lookup_index);
    hb_set_t visited;
    lookup_sizes.push (lookup_size_t {
        lookup_index,
        ext_context.graph.find_subgraph_size (lookup_index, visited).قدر_or (0),
        lookup->نمبر_of_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_size = ext_context.graph.vertices_[ext_context.lookup_list_index].table_size ();
  size_t l2_l3_size = lookup_list_size + total_lookup_table_sizes; // Lookup List + Lookups
  size_t l3_l4_size = total_lookup_table_sizes; // Lookups + SubTables
  size_t l4_plus_size = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  لكل (auto p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    unsigned subtables_size = p.عدد_subtables * 8;
    l3_l4_size += subtables_size;
    l4_plus_size += subtables_size;
  }

  منطقي layers_full = خطأ;
  لكل (auto p : lookup_sizes)
  {
    ثابت graph::Lookup* lookup = ext_context.lookups.get(p.lookup_index);
    إذا (lookup->is_extension (ext_context.table_tag))
      // already an extension so size is counted by the loop above.
      استمر;

    إذا (!layers_full)
    {
      size_t lookup_size = ext_context.graph.vertices_[p.lookup_index].table_size ();
      hb_set_t visited;
      size_t subtables_size = ext_context.graph.find_subgraph_size (p.lookup_index, visited, 1).قدر_or (0) - lookup_size;
      size_t remaining_size = p.size - subtables_size - lookup_size;

      l3_l4_size   += subtables_size;
      l3_l4_size   -= p.عدد_subtables * 8;
      l4_plus_size += subtables_size + remaining_size;

      إذا (l2_l3_size < (1 << 16)
          && l3_l4_size < (1 << 16)
          && l4_plus_size < (1 << 16)) استمر; // this lookup fits within all layers groups

      layers_full = true;
    }

    TRY (ext_context.lookups.get(p.lookup_index)->make_extension (ext_context, p.lookup_index));
  }

  أرجع Ok();
}

ساكن مضمن
graph_result_t<منطقي> _try_isolating_subgraphs (ثابت hb_vector_t<graph::overflow_record_t>& overflows,
                                               graph_t& sorted_graph)
{
  unsigned space = 0;
  hb_set_t roots_to_isolate;

  لكل (صحيح i = overflows.length - 1; i >= 0; i--)
  {
    ثابت graph::overflow_record_t& r = overflows[i];

    unsigned جڑ;
    unsigned overflow_space = sorted_graph.space_for (r.parent, &جڑ);
    إذا (!overflow_space) استمر;
    إذا (sorted_graph.عدد_roots_for_space_2 (overflow_space) <= 1) استمر;

    إذا (!space) {
      space = overflow_space;
    }

    إذا (space == overflow_space)
      roots_to_isolate.add(جڑ);
  }

  إذا (!roots_to_isolate) أرجع Ok(خطأ);

  unsigned maximum_to_move = hb_max ((sorted_graph.عدد_roots_for_space_2 (space) / 2u), 1u);
  إذا (roots_to_isolate.get_population () > maximum_to_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    صحيح extra = roots_to_isolate.get_population () - maximum_to_move;
    لكل (unsigned id : sorted_graph.ordering_) {
      إذا (!extra) اكسر;
      إذا (roots_to_isolate.has(id)) {
        roots_to_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_پیغام (SUBSET_REPACK, nullptr,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_graph.عدد_roots_for_space_2 (space),
             roots_to_isolate.get_population (),
             sorted_graph.next_space ());
  TRY (sorted_graph.isolate_subgraph (roots_to_isolate));
  TRY (sorted_graph.move_to_new_space (roots_to_isolate));

  أرجع Ok(true);
}

ساكن مضمن
graph_result_t<منطقي> _resolve_shared_overflow(ثابت hb_vector_t<graph::overflow_record_t>& overflows,
                                              صحيح overflow_index,
                                              graph_t& sorted_graph)
{
  ثابت graph::overflow_record_t& r = overflows[overflow_index];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_set_t parents;
  parents.add(r.parent);
  لكل (صحيح i = overflow_index - 1; i >= 0; i--) {
    ثابت graph::overflow_record_t& r2 = overflows[i];
    إذا (r2.child == r.child) {
      parents.add(r2.parent);
    }
  }

  auto result = sorted_graph.duplicate(&parents, r.child);
  إذا (!result.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    result = sorted_graph.duplicate(&parents, r.child);
  }

  إذا (!result.is_ok ()) أرجع Ok(خطأ);

  إذا (parents.get_population() > 1) {
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

  أرجع Ok(true);
}

ساكن مضمن
graph_result_t<منطقي> _process_overflows (ثابت hb_vector_t<graph::overflow_record_t>& overflows,
                                         hb_set_t& priority_bumped_parents,
                                         graph_t& sorted_graph)
{
  منطقي resolution_attempted = خطأ;

  // Try resolving the furthest overflows first.
  لكل (صحيح i = overflows.length - 1; i >= 0; i--)
  {
    ثابت graph::overflow_record_t& r = overflows[i];
    إذا (sorted_graph.vertices_[r.child].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_ASSIGN (منطقي resolved, _resolve_shared_overflow(overflows, i, sorted_graph));
      إذا (resolved)
        أرجع Ok(true);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    إذا (sorted_graph.vertices_[r.child].is_leaf () && !priority_bumped_parents.has (r.parent))
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
      TRY_ASSIGN (منطقي raised, sorted_graph.raise_childrens_priority (r.parent));
      إذا (raised) {
        priority_bumped_parents.add (r.parent);
        resolution_attempted = true;
      }
      استمر;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  أرجع Ok(resolution_attempted);
}

مضمن graph_result_t<فراغ>
_assign_spaces_and_sort (graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, nullptr, "Assigning spaces to 32 bit subgraphs.");
  TRY_ASSIGN (منطقي assigned, sorted_graph.assign_spaces ());
  إذا (assigned)
    أرجع sorted_graph.sort_shortest_distance ();
  وإلا
    أرجع sorted_graph.sort_shortest_distance_if_needed ();
}

مضمن graph_result_t<فراغ>
_gsub_gpos_specialization (hb_tag_t table_tag,
                           منطقي always_recalculate_extensions,
                           graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, nullptr, "Applying GSUB/GPOS repacking specializations.");
  إذا (!always_recalculate_extensions) أرجع _assign_spaces_and_sort (sorted_graph);

  auto context_res = graph::gsubgpos_graph_context_t::create (table_tag, sorted_graph);

  إذا (unlikely (context_res.is_err())) {
    إذا (context_res.error() == graph::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      أرجع _assign_spaces_and_sort (sorted_graph);
    وإلا
      // All other errors are propagated.
      أرجع Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  auto& ext_context = *context_res;
  DEBUG_پیغام (SUBSET_REPACK, nullptr, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_پیغام (SUBSET_REPACK, nullptr, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_graph.sort_shortest_distance_if_needed ());

  TRY_ASSIGN (منطقي will_overflow, graph::will_overflow (sorted_graph));
  إذا (!will_overflow) أرجع Ok();
  أرجع _assign_spaces_and_sort (sorted_graph);
}

مضمن graph_result_t<فراغ>
hb_resolve_graph_overflows (hb_tag_t table_tag,
                            unsigned max_rounds ,
                            منطقي always_recalculate_extensions,
                            graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, nullptr, "Repacking %c%c%c%c.", HB_UNTAG(table_tag));
  TRY (sorted_graph.sort_shortest_distance ());

  TRY_ASSIGN (منطقي will_overflow, graph::will_overflow (sorted_graph));
  إذا (!will_overflow)
    أرجع Ok();

  منطقي is_gsub_or_gpos = (table_tag == HB_OT_TAG_GPOS ||  table_tag == HB_OT_TAG_GSUB);
  إذا (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (table_tag, always_recalculate_extensions, sorted_graph));

  unsigned round = 0;
  unsigned total_iterations = 0;
  hb_vector_t<graph::overflow_record_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  طالما (round < max_rounds && total_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_ASSIGN (منطقي overflows_exist, graph::will_overflow (sorted_graph, &overflows));
    إذا (!overflows_exist)
      اكسر;

    DEBUG_پیغام (SUBSET_REPACK, nullptr, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_graph, overflows);

    total_iterations++;
    hb_set_t priority_bumped_parents;

    TRY_ASSIGN (منطقي isolated, _try_isolating_subgraphs (overflows, sorted_graph));
    إذا (!isolated)
    {
      round++;
      TRY_ASSIGN (منطقي processed, _process_overflows (overflows, priority_bumped_parents, sorted_graph));
      إذا (!processed)
      {
        DEBUG_پیغام (SUBSET_REPACK, nullptr, "No resolution available :(");
        اكسر;
      }
    }

    TRY (sorted_graph.sort_shortest_distance ());
  }

  TRY_ASSIGN (منطقي has_overflow, graph::will_overflow (sorted_graph));
  إذا (unlikely (has_overflow))
  {
    إذا (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_پیغام (SUBSET_REPACK, nullptr, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      أرجع hb_resolve_graph_overflows (table_tag, max_rounds, true, sorted_graph);
    }

    DEBUG_پیغام (SUBSET_REPACK, nullptr, "Offset overflow resolution failed.");
    أرجع Err(graph::OVERFLOW_RESOLUTION_FAILED);
  }

  أرجع Ok();
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
مضمن hb_blob_t*
hb_resolve_overflows (ثابت T& packed,
                      hb_tag_t table_tag,
                      unsigned max_rounds = 32,
                      منطقي recalculate_extensions = خطأ) {
  auto graph_res = graph_t::create (packed);
  إذا (!graph_res.is_ok ())
  {
    DEBUG_پیغام (SUBSET_REPACK, nullptr,
               "Graph creation failed, cause: %s", graph::to_string(graph_res.error()));
    أرجع nullptr;
  }
  graph_t sorted_graph = std::move (*graph_res);

  auto resolve_res = hb_resolve_graph_overflows (table_tag, max_rounds, recalculate_extensions, sorted_graph);
  إذا (resolve_res.is_err ())
  {
    DEBUG_پیغام (SUBSET_REPACK, nullptr,
               "Overflow resolution failed, cause: %s", graph::to_string(resolve_res.error()));
    أرجع nullptr;
  }

  auto serialize_res = graph::serialize (sorted_graph);
  إذا (serialize_res.is_err ())
  {
    DEBUG_پیغام (SUBSET_REPACK, nullptr,
               "Serialization failed, cause: %s", graph::to_string(serialize_res.error()));
    أرجع nullptr;
  }

  أرجع *serialize_res;
}

#endif /* HB_REPACKER_HH */
