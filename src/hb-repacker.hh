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

#अगर_अघोषित HB_REPACKER_HH
#घोषणा HB_REPACKER_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-map.hh"
#समावेश "hb-vector.hh"
#समावेश "graph/graph.hh"
#समावेश "graph/gsubgpos-graph.hh"
#समावेश "graph/serialize.hh"

using graph::graph_t;
using graph::graph_परिणाम_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

काष्ठा lookup_आकार_t
{
  अचिन्हित lookup_सूचकांक;
  माप_प्रकार आकार;
  अचिन्हित अंक_subtables;

  अटल पूर्णांक cmp (स्थिर व्योम* a, स्थिर व्योम* b)
  {
    वापस cmp ((स्थिर lookup_आकार_t*) a,
                (स्थिर lookup_आकार_t*) b);
  }

  अटल पूर्णांक cmp (स्थिर lookup_आकार_t* a, स्थिर lookup_आकार_t* b)
  {
    द्विगुन subtables_per_byte_a = (द्विगुन) a->अंक_subtables / (द्विगुन) a->आकार;
    द्विगुन subtables_per_byte_b = (द्विगुन) b->अंक_subtables / (द्विगुन) b->आकार;
    अगर (subtables_per_byte_a == subtables_per_byte_b) {
      वापस b->lookup_सूचकांक - a->lookup_सूचकांक;
    }

    द्विगुन cmp = subtables_per_byte_b - subtables_per_byte_a;
    अगर (cmp < 0) वापस -1;
    अगर (cmp > 0) वापस 1;
    वापस 0;
  }
};

अटल अंतरभूत
graph_परिणाम_t<व्योम> _presplit_subtables_if_needed (graph::gsubgpos_graph_context_t& ext_context)
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
  hb_set_t lookup_indices(ext_context.lookups.कुंजियाँ ());
  क्रम (अचिन्हित lookup_सूचकांक : lookup_indices)
  {
    graph::Lookup* lookup = ext_context.lookups.लो(lookup_सूचकांक);
    TRY (lookup->split_subtables_if_needed (ext_context, lookup_सूचकांक));
  }

  वापस Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
अटल अंतरभूत
graph_परिणाम_t<व्योम> _promote_extensions_if_needed (graph::gsubgpos_graph_context_t& ext_context)
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
  अगर (!ext_context.lookups) वापस Ok();

  अचिन्हित योग_lookup_table_sizes = 0;
  hb_vector_t<lookup_आकार_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), true);

  क्रम (अचिन्हित lookup_सूचकांक : ext_context.lookups.कुंजियाँ ())
  {
    स्थिर स्वतः& lookup_v = ext_context.graph.vertices_[lookup_सूचकांक];
    योग_lookup_table_sizes += lookup_v.table_आकार ();

    स्थिर graph::Lookup* lookup = ext_context.lookups.लो(lookup_सूचकांक);
    hb_set_t visited;
    lookup_sizes.push (lookup_आकार_t {
        lookup_सूचकांक,
        ext_context.graph.find_subgraph_आकार (lookup_सूचकांक, visited).मूल्य_or (0),
        lookup->संख्या_of_subtables (),
      });
  }

  lookup_sizes.क्विक ();

  माप_प्रकार lookup_list_आकार = ext_context.graph.vertices_[ext_context.lookup_list_सूचकांक].table_आकार ();
  माप_प्रकार l2_l3_आकार = lookup_list_आकार + योग_lookup_table_sizes; // Lookup List + Lookups
  माप_प्रकार l3_l4_आकार = योग_lookup_table_sizes; // Lookups + SubTables
  माप_प्रकार l4_plus_आकार = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  क्रम (स्वतः p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    अचिन्हित subtables_आकार = p.अंक_subtables * 8;
    l3_l4_आकार += subtables_आकार;
    l4_plus_आकार += subtables_आकार;
  }

  bool layers_full = false;
  क्रम (स्वतः p : lookup_sizes)
  {
    स्थिर graph::Lookup* lookup = ext_context.lookups.लो(p.lookup_सूचकांक);
    अगर (lookup->is_extension (ext_context.table_tag))
      // already an extension so size is counted by the loop above.
      जारी;

    अगर (!layers_full)
    {
      माप_प्रकार lookup_आकार = ext_context.graph.vertices_[p.lookup_सूचकांक].table_आकार ();
      hb_set_t visited;
      माप_प्रकार subtables_आकार = ext_context.graph.find_subgraph_आकार (p.lookup_सूचकांक, visited, 1).मूल्य_or (0) - lookup_आकार;
      माप_प्रकार remaining_आकार = p.आकार - subtables_आकार - lookup_आकार;

      l3_l4_आकार   += subtables_आकार;
      l3_l4_आकार   -= p.अंक_subtables * 8;
      l4_plus_आकार += subtables_आकार + remaining_आकार;

      अगर (l2_l3_आकार < (1 << 16)
          && l3_l4_आकार < (1 << 16)
          && l4_plus_आकार < (1 << 16)) जारी; // this lookup fits within all layers groups

      layers_full = true;
    }

    TRY (ext_context.lookups.लो(p.lookup_सूचकांक)->make_extension (ext_context, p.lookup_सूचकांक));
  }

  वापस Ok();
}

अटल अंतरभूत
graph_परिणाम_t<bool> _try_isolating_subgraphs (स्थिर hb_vector_t<graph::overflow_record_t>& overflows,
                                               graph_t& sorted_graph)
{
  अचिन्हित space = 0;
  hb_set_t roots_to_isolate;

  क्रम (पूर्णांक i = overflows.लंबाई - 1; i >= 0; i--)
  {
    स्थिर graph::overflow_record_t& r = overflows[i];

    अचिन्हित मूल;
    अचिन्हित overflow_space = sorted_graph.space_for (r.parent, &मूल);
    अगर (!overflow_space) जारी;
    अगर (sorted_graph.अंक_roots_for_space_2 (overflow_space) <= 1) जारी;

    अगर (!space) {
      space = overflow_space;
    }

    अगर (space == overflow_space)
      roots_to_isolate.add(मूल);
  }

  अगर (!roots_to_isolate) वापस Ok(false);

  अचिन्हित maximum_to_move = hb_max ((sorted_graph.अंक_roots_for_space_2 (space) / 2u), 1u);
  अगर (roots_to_isolate.get_population () > maximum_to_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    पूर्णांक extra = roots_to_isolate.get_population () - maximum_to_move;
    क्रम (अचिन्हित id : sorted_graph.ordering_) {
      अगर (!extra) अवरोध;
      अगर (roots_to_isolate.has(id)) {
        roots_to_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_संदेश (SUBSET_REPACK, nullptr,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_graph.अंक_roots_for_space_2 (space),
             roots_to_isolate.get_population (),
             sorted_graph.अग्र_space ());
  TRY (sorted_graph.isolate_subgraph (roots_to_isolate));
  TRY (sorted_graph.move_to_new_space (roots_to_isolate));

  वापस Ok(true);
}

अटल अंतरभूत
graph_परिणाम_t<bool> _resolve_shared_overflow(स्थिर hb_vector_t<graph::overflow_record_t>& overflows,
                                              पूर्णांक overflow_सूचकांक,
                                              graph_t& sorted_graph)
{
  स्थिर graph::overflow_record_t& r = overflows[overflow_सूचकांक];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_set_t parents;
  parents.add(r.parent);
  क्रम (पूर्णांक i = overflow_सूचकांक - 1; i >= 0; i--) {
    स्थिर graph::overflow_record_t& r2 = overflows[i];
    अगर (r2.child == r.child) {
      parents.add(r2.parent);
    }
  }

  स्वतः परिणाम = sorted_graph.duplicate(&parents, r.child);
  अगर (!परिणाम.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    परिणाम = sorted_graph.duplicate(&parents, r.child);
  }

  अगर (!परिणाम.is_ok ()) वापस Ok(false);

  अगर (parents.get_population() > 1) {
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
    sorted_graph.vertices_[*परिणाम].give_max_priority();
  }

  वापस Ok(true);
}

अटल अंतरभूत
graph_परिणाम_t<bool> _process_overflows (स्थिर hb_vector_t<graph::overflow_record_t>& overflows,
                                         hb_set_t& priority_bumped_parents,
                                         graph_t& sorted_graph)
{
  bool resolution_attempted = false;

  // Try resolving the furthest overflows first.
  क्रम (पूर्णांक i = overflows.लंबाई - 1; i >= 0; i--)
  {
    स्थिर graph::overflow_record_t& r = overflows[i];
    अगर (sorted_graph.vertices_[r.child].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_ASSIGN (bool resolved, _resolve_shared_overflow(overflows, i, sorted_graph));
      अगर (resolved)
        वापस Ok(true);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    अगर (sorted_graph.vertices_[r.child].is_leaf () && !priority_bumped_parents.has (r.parent))
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
      TRY_ASSIGN (bool raised, sorted_graph.raise_childrens_priority (r.parent));
      अगर (raised) {
        priority_bumped_parents.add (r.parent);
        resolution_attempted = true;
      }
      जारी;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  वापस Ok(resolution_attempted);
}

अंतरभूत graph_परिणाम_t<व्योम>
_assign_spaces_and_sort (graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_संदेश (SUBSET_REPACK, nullptr, "Assigning spaces to 32 bit subgraphs.");
  TRY_ASSIGN (bool assigned, sorted_graph.assign_spaces ());
  अगर (assigned)
    वापस sorted_graph.sort_shortest_distance ();
  अन्यथा
    वापस sorted_graph.sort_shortest_distance_if_needed ();
}

अंतरभूत graph_परिणाम_t<व्योम>
_gsub_gpos_specialization (hb_tag_t table_tag,
                           bool always_recalculate_extensions,
                           graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_संदेश (SUBSET_REPACK, nullptr, "Applying GSUB/GPOS repacking specializations.");
  अगर (!always_recalculate_extensions) वापस _assign_spaces_and_sort (sorted_graph);

  स्वतः context_res = graph::gsubgpos_graph_context_t::create (table_tag, sorted_graph);

  अगर (unlikely (context_res.is_err())) {
    अगर (context_res.error() == graph::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      वापस _assign_spaces_and_sort (sorted_graph);
    अन्यथा
      // All other errors are propagated.
      वापस Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  स्वतः& ext_context = *context_res;
  DEBUG_संदेश (SUBSET_REPACK, nullptr, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_संदेश (SUBSET_REPACK, nullptr, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_graph.sort_shortest_distance_if_needed ());

  TRY_ASSIGN (bool will_overflow, graph::will_overflow (sorted_graph));
  अगर (!will_overflow) वापस Ok();
  वापस _assign_spaces_and_sort (sorted_graph);
}

अंतरभूत graph_परिणाम_t<व्योम>
hb_resolve_graph_overflows (hb_tag_t table_tag,
                            अचिन्हित max_rounds ,
                            bool always_recalculate_extensions,
                            graph_t& sorted_graph /* IN/OUT */)
{
  DEBUG_संदेश (SUBSET_REPACK, nullptr, "Repacking %c%c%c%c.", HB_UNTAG(table_tag));
  TRY (sorted_graph.sort_shortest_distance ());

  TRY_ASSIGN (bool will_overflow, graph::will_overflow (sorted_graph));
  अगर (!will_overflow)
    वापस Ok();

  bool is_gsub_or_gpos = (table_tag == HB_OT_TAG_GPOS ||  table_tag == HB_OT_TAG_GSUB);
  अगर (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (table_tag, always_recalculate_extensions, sorted_graph));

  अचिन्हित round = 0;
  अचिन्हित योग_iterations = 0;
  hb_vector_t<graph::overflow_record_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  जबतक (round < max_rounds && योग_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_ASSIGN (bool overflows_exist, graph::will_overflow (sorted_graph, &overflows));
    अगर (!overflows_exist)
      अवरोध;

    DEBUG_संदेश (SUBSET_REPACK, nullptr, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_graph, overflows);

    योग_iterations++;
    hb_set_t priority_bumped_parents;

    TRY_ASSIGN (bool isolated, _try_isolating_subgraphs (overflows, sorted_graph));
    अगर (!isolated)
    {
      round++;
      TRY_ASSIGN (bool processed, _process_overflows (overflows, priority_bumped_parents, sorted_graph));
      अगर (!processed)
      {
        DEBUG_संदेश (SUBSET_REPACK, nullptr, "No resolution available :(");
        अवरोध;
      }
    }

    TRY (sorted_graph.sort_shortest_distance ());
  }

  TRY_ASSIGN (bool has_overflow, graph::will_overflow (sorted_graph));
  अगर (unlikely (has_overflow))
  {
    अगर (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      वापस hb_resolve_graph_overflows (table_tag, max_rounds, true, sorted_graph);
    }

    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Offset overflow resolution failed.");
    वापस Err(graph::OVERFLOW_RESOLUTION_FAILED);
  }

  वापस Ok();
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
ढाँचा<typename T>
अंतरभूत hb_blob_t*
hb_resolve_overflows (स्थिर T& packed,
                      hb_tag_t table_tag,
                      अचिन्हित max_rounds = 32,
                      bool recalculate_extensions = false) {
  स्वतः graph_res = graph_t::create (packed);
  अगर (!graph_res.is_ok ())
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "Graph creation failed, cause: %s", graph::to_string(graph_res.error()));
    वापस nullptr;
  }
  graph_t sorted_graph = std::move (*graph_res);

  स्वतः resolve_res = hb_resolve_graph_overflows (table_tag, max_rounds, recalculate_extensions, sorted_graph);
  अगर (resolve_res.is_err ())
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "Overflow resolution failed, cause: %s", graph::to_string(resolve_res.error()));
    वापस nullptr;
  }

  स्वतः serialize_res = graph::serialize (sorted_graph);
  अगर (serialize_res.is_err ())
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "Serialization failed, cause: %s", graph::to_string(serialize_res.error()));
    वापस nullptr;
  }

  वापस *serialize_res;
}

#पूर्ण_अगर /* HB_REPACKER_HH */
