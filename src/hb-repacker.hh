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

#నిర్వచితం_కాకపోతే HB_REPACKER_HH
#నిర్వచించు HB_REPACKER_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-vector.hh"
#అన్తర్భావ "graph/graph.hh"
#అన్తర్భావ "graph/gsubgpos-graph.hh"
#అన్తర్భావ "graph/serialize.hh"

ఉపయోగించి లేఖాచిత్రం::లేఖాచిత్రం_t;
ఉపయోగించి లేఖాచిత్రం::లేఖాచిత్రం_ఫలితం_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

నిర్మాణం lookup_కొలత_t
{
  గుర్తులేని lookup_సూచీ;
  size_t కొలత;
  గుర్తులేని అంకె_subtables;

  స్థిర పూర్ణాంకం cmp (స్థిరరాశి శూన్య* a, స్థిరరాశి శూన్య* b)
  {
    తిరిగి cmp ((స్థిరరాశి lookup_కొలత_t*) a,
                (స్థిరరాశి lookup_కొలత_t*) b);
  }

  స్థిర పూర్ణాంకం cmp (స్థిరరాశి lookup_కొలత_t* a, స్థిరరాశి lookup_కొలత_t* b)
  {
    ద్విగుణ subtables_చొప్పున_byte_a = (ద్విగుణ) a->అంకె_subtables / (ద్విగుణ) a->కొలత;
    ద్విగుణ subtables_చొప్పున_byte_b = (ద్విగుణ) b->అంకె_subtables / (ద్విగుణ) b->కొలత;
    ఒకవేళ (subtables_చొప్పున_byte_a == subtables_చొప్పున_byte_b) {
      తిరిగి b->lookup_సూచీ - a->lookup_సూచీ;
    }

    ద్విగుణ cmp = subtables_చొప్పున_byte_b - subtables_చొప్పున_byte_a;
    ఒకవేళ (cmp < 0) తిరిగి -1;
    ఒకవేళ (cmp > 0) తిరిగి 1;
    తిరిగి 0;
  }
};

స్థిర అంతఃవరుస
లేఖాచిత్రం_ఫలితం_t<శూన్య> _presplit_subtables_if_needed (లేఖాచిత్రం::gsubgpos_లేఖాచిత్రం_context_t& ext_context)
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
  hb_సమితి_t lookup_indices(ext_context.lookups.కీలకాలు ());
  కొరకు (గుర్తులేని lookup_సూచీ : lookup_indices)
  {
    లేఖాచిత్రం::Lookup* lookup = ext_context.lookups.get(lookup_సూచీ);
    TRY (lookup->విభజించు_subtables_if_needed (ext_context, lookup_సూచీ));
  }

  తిరిగి Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
స్థిర అంతఃవరుస
లేఖాచిత్రం_ఫలితం_t<శూన్య> _promote_extensions_if_needed (లేఖాచిత్రం::gsubgpos_లేఖాచిత్రం_context_t& ext_context)
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
  ఒకవేళ (!ext_context.lookups) తిరిగి Ok();

  గుర్తులేని మొత్తం_lookup_పట్టిక_sizes = 0;
  hb_సదిశ_t<lookup_కొలత_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), నిజం);

  కొరకు (గుర్తులేని lookup_సూచీ : ext_context.lookups.కీలకాలు ())
  {
    స్థిరరాశి స్వయంచాలక& lookup_v = ext_context.లేఖాచిత్రం.vertices_[lookup_సూచీ];
    మొత్తం_lookup_పట్టిక_sizes += lookup_v.పట్టిక_కొలత ();

    స్థిరరాశి లేఖాచిత్రం::Lookup* lookup = ext_context.lookups.get(lookup_సూచీ);
    hb_సమితి_t సందర్శించిన;
    lookup_sizes.push (lookup_కొలత_t {
        lookup_సూచీ,
        ext_context.లేఖాచిత్రం.కనుగొను_subgraph_కొలత (lookup_సూచీ, సందర్శించిన).విలువ_or (0),
        lookup->సంఖ్య_యొక్క_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_కొలత = ext_context.లేఖాచిత్రం.vertices_[ext_context.lookup_list_సూచీ].పట్టిక_కొలత ();
  size_t l2_l3_కొలత = lookup_list_కొలత + మొత్తం_lookup_పట్టిక_sizes; // Lookup List + Lookups
  size_t l3_l4_కొలత = మొత్తం_lookup_పట్టిక_sizes; // Lookups + SubTables
  size_t l4_plus_కొలత = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  కొరకు (స్వయంచాలక p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    గుర్తులేని subtables_కొలత = p.అంకె_subtables * 8;
    l3_l4_కొలత += subtables_కొలత;
    l4_plus_కొలత += subtables_కొలత;
  }

  తార్కిక layers_నిండిన = అబద్ధం;
  కొరకు (స్వయంచాలక p : lookup_sizes)
  {
    స్థిరరాశి లేఖాచిత్రం::Lookup* lookup = ext_context.lookups.get(p.lookup_సూచీ);
    ఒకవేళ (lookup->is_extension (ext_context.పట్టిక_ట్యాగ్_2))
      // already an extension so size is counted by the loop above.
      కొనసాగు;

    ఒకవేళ (!layers_నిండిన)
    {
      size_t lookup_కొలత = ext_context.లేఖాచిత్రం.vertices_[p.lookup_సూచీ].పట్టిక_కొలత ();
      hb_సమితి_t సందర్శించిన;
      size_t subtables_కొలత = ext_context.లేఖాచిత్రం.కనుగొను_subgraph_కొలత (p.lookup_సూచీ, సందర్శించిన, 1).విలువ_or (0) - lookup_కొలత;
      size_t మిగిలిన_కొలత = p.కొలత - subtables_కొలత - lookup_కొలత;

      l3_l4_కొలత   += subtables_కొలత;
      l3_l4_కొలత   -= p.అంకె_subtables * 8;
      l4_plus_కొలత += subtables_కొలత + మిగిలిన_కొలత;

      ఒకవేళ (l2_l3_కొలత < (1 << 16)
          && l3_l4_కొలత < (1 << 16)
          && l4_plus_కొలత < (1 << 16)) కొనసాగు; // this lookup fits within all layers groups

      layers_నిండిన = నిజం;
    }

    TRY (ext_context.lookups.get(p.lookup_సూచీ)->make_extension (ext_context, p.lookup_సూచీ));
  }

  తిరిగి Ok();
}

స్థిర అంతఃవరుస
లేఖాచిత్రం_ఫలితం_t<తార్కిక> _try_isolating_subgraphs (స్థిరరాశి hb_సదిశ_t<లేఖాచిత్రం::అధికప్రవాహం_రికార్డు_t>& overflows,
                                               లేఖాచిత్రం_t& sorted_లేఖాచిత్రం)
{
  గుర్తులేని space = 0;
  hb_సమితి_t roots_గమ్యం_isolate;

  కొరకు (పూర్ణాంకం i = overflows.నిడివి - 1; i >= 0; i--)
  {
    స్థిరరాశి లేఖాచిత్రం::అధికప్రవాహం_రికార్డు_t& r = overflows[i];

    గుర్తులేని మూలం;
    గుర్తులేని అధికప్రవాహం_space = sorted_లేఖాచిత్రం.space_for (r.మాతృ, &మూలం);
    ఒకవేళ (!అధికప్రవాహం_space) కొనసాగు;
    ఒకవేళ (sorted_లేఖాచిత్రం.అంకె_roots_for_space_2 (అధికప్రవాహం_space) <= 1) కొనసాగు;

    ఒకవేళ (!space) {
      space = అధికప్రవాహం_space;
    }

    ఒకవేళ (space == అధికప్రవాహం_space)
      roots_గమ్యం_isolate.కూడు(మూలం);
  }

  ఒకవేళ (!roots_గమ్యం_isolate) తిరిగి Ok(అబద్ధం);

  గుర్తులేని గరిష్ఠ_గమ్యం_move = hb_max ((sorted_లేఖాచిత్రం.అంకె_roots_for_space_2 (space) / 2u), 1u);
  ఒకవేళ (roots_గమ్యం_isolate.get_population () > గరిష్ఠ_గమ్యం_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    పూర్ణాంకం extra = roots_గమ్యం_isolate.get_population () - గరిష్ఠ_గమ్యం_move;
    కొరకు (గుర్తులేని id : sorted_లేఖాచిత్రం.ordering_) {
      ఒకవేళ (!extra) ఆపు;
      ఒకవేళ (roots_గమ్యం_isolate.has(id)) {
        roots_గమ్యం_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_లేఖాచిత్రం.అంకె_roots_for_space_2 (space),
             roots_గమ్యం_isolate.get_population (),
             sorted_లేఖాచిత్రం.తదుపరి_space ());
  TRY (sorted_లేఖాచిత్రం.isolate_subgraph (roots_గమ్యం_isolate));
  TRY (sorted_లేఖాచిత్రం.move_గమ్యం_new_space (roots_గమ్యం_isolate));

  తిరిగి Ok(నిజం);
}

స్థిర అంతఃవరుస
లేఖాచిత్రం_ఫలితం_t<తార్కిక> resolve_shared_అధికప్రవాహం(స్థిరరాశి hb_సదిశ_t<లేఖాచిత్రం::అధికప్రవాహం_రికార్డు_t>& overflows,
                                              పూర్ణాంకం అధికప్రవాహం_సూచీ,
                                              లేఖాచిత్రం_t& sorted_లేఖాచిత్రం)
{
  స్థిరరాశి లేఖాచిత్రం::అధికప్రవాహం_రికార్డు_t& r = overflows[అధికప్రవాహం_సూచీ];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_సమితి_t parents;
  parents.కూడు(r.మాతృ);
  కొరకు (పూర్ణాంకం i = అధికప్రవాహం_సూచీ - 1; i >= 0; i--) {
    స్థిరరాశి లేఖాచిత్రం::అధికప్రవాహం_రికార్డు_t& r2 = overflows[i];
    ఒకవేళ (r2.బిడ్డ == r.బిడ్డ) {
      parents.కూడు(r2.మాతృ);
    }
  }

  స్వయంచాలక ఫలితం = sorted_లేఖాచిత్రం.duplicate(&parents, r.బిడ్డ);
  ఒకవేళ (!ఫలితం.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    ఫలితం = sorted_లేఖాచిత్రం.duplicate(&parents, r.బిడ్డ);
  }

  ఒకవేళ (!ఫలితం.is_ok ()) తిరిగి Ok(అబద్ధం);

  ఒకవేళ (parents.get_population() > 1) {
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
    sorted_లేఖాచిత్రం.vertices_[*ఫలితం].give_max_ప్రాధాన్యత();
  }

  తిరిగి Ok(నిజం);
}

స్థిర అంతఃవరుస
లేఖాచిత్రం_ఫలితం_t<తార్కిక> _process_overflows (స్థిరరాశి hb_సదిశ_t<లేఖాచిత్రం::అధికప్రవాహం_రికార్డు_t>& overflows,
                                         hb_సమితి_t& ప్రాధాన్యత_bumped_parents,
                                         లేఖాచిత్రం_t& sorted_లేఖాచిత్రం)
{
  తార్కిక resolution_attempted = అబద్ధం;

  // Try resolving the furthest overflows first.
  కొరకు (పూర్ణాంకం i = overflows.నిడివి - 1; i >= 0; i--)
  {
    స్థిరరాశి లేఖాచిత్రం::అధికప్రవాహం_రికార్డు_t& r = overflows[i];
    ఒకవేళ (sorted_లేఖాచిత్రం.vertices_[r.బిడ్డ].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_కేటాయించు (తార్కిక resolved, resolve_shared_అధికప్రవాహం(overflows, i, sorted_లేఖాచిత్రం));
      ఒకవేళ (resolved)
        తిరిగి Ok(నిజం);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    ఒకవేళ (sorted_లేఖాచిత్రం.vertices_[r.బిడ్డ].is_ఆకు () && !ప్రాధాన్యత_bumped_parents.has (r.మాతృ))
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
      TRY_కేటాయించు (తార్కిక raised, sorted_లేఖాచిత్రం.raise_childrens_ప్రాధాన్యత (r.మాతృ));
      ఒకవేళ (raised) {
        ప్రాధాన్యత_bumped_parents.కూడు (r.మాతృ);
        resolution_attempted = నిజం;
      }
      కొనసాగు;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  తిరిగి Ok(resolution_attempted);
}

అంతఃవరుస లేఖాచిత్రం_ఫలితం_t<శూన్య>
కేటాయించు_spaces_and_క్రమీకరించు (లేఖాచిత్రం_t& sorted_లేఖాచిత్రం /* IN/OUT */)
{
  DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Assigning spaces to 32 bit subgraphs.");
  TRY_కేటాయించు (తార్కిక assigned, sorted_లేఖాచిత్రం.కేటాయించు_spaces ());
  ఒకవేళ (assigned)
    తిరిగి sorted_లేఖాచిత్రం.క్రమీకరించు_అతితక్కువ_దూరం ();
  లేకపోతే
    తిరిగి sorted_లేఖాచిత్రం.క్రమీకరించు_అతితక్కువ_దూరం_if_needed ();
}

అంతఃవరుస లేఖాచిత్రం_ఫలితం_t<శూన్య>
_gsub_gpos_specialization (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
                           తార్కిక always_recalculate_extensions,
                           లేఖాచిత్రం_t& sorted_లేఖాచిత్రం /* IN/OUT */)
{
  DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Applying GSUB/GPOS repacking specializations.");
  ఒకవేళ (!always_recalculate_extensions) తిరిగి కేటాయించు_spaces_and_క్రమీకరించు (sorted_లేఖాచిత్రం);

  స్వయంచాలక context_res = లేఖాచిత్రం::gsubgpos_లేఖాచిత్రం_context_t::create (పట్టిక_ట్యాగ్_2, sorted_లేఖాచిత్రం);

  ఒకవేళ (unlikely (context_res.is_err())) {
    ఒకవేళ (context_res.error() == లేఖాచిత్రం::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      తిరిగి కేటాయించు_spaces_and_క్రమీకరించు (sorted_లేఖాచిత్రం);
    లేకపోతే
      // All other errors are propagated.
      తిరిగి Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  స్వయంచాలక& ext_context = *context_res;
  DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_లేఖాచిత్రం.క్రమీకరించు_అతితక్కువ_దూరం_if_needed ());

  TRY_కేటాయించు (తార్కిక will_అధికప్రవాహం, లేఖాచిత్రం::will_అధికప్రవాహం (sorted_లేఖాచిత్రం));
  ఒకవేళ (!will_అధికప్రవాహం) తిరిగి Ok();
  తిరిగి కేటాయించు_spaces_and_క్రమీకరించు (sorted_లేఖాచిత్రం);
}

అంతఃవరుస లేఖాచిత్రం_ఫలితం_t<శూన్య>
hb_resolve_లేఖాచిత్రం_overflows (hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
                            గుర్తులేని max_విడతలు ,
                            తార్కిక always_recalculate_extensions,
                            లేఖాచిత్రం_t& sorted_లేఖాచిత్రం /* IN/OUT */)
{
  DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Repacking %c%c%c%c.", HB_UNTAG(పట్టిక_ట్యాగ్_2));
  TRY (sorted_లేఖాచిత్రం.క్రమీకరించు_అతితక్కువ_దూరం ());

  TRY_కేటాయించు (తార్కిక will_అధికప్రవాహం, లేఖాచిత్రం::will_అధికప్రవాహం (sorted_లేఖాచిత్రం));
  ఒకవేళ (!will_అధికప్రవాహం)
    తిరిగి Ok();

  తార్కిక is_gsub_or_gpos = (పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GPOS ||  పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB);
  ఒకవేళ (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (పట్టిక_ట్యాగ్_2, always_recalculate_extensions, sorted_లేఖాచిత్రం));

  గుర్తులేని round = 0;
  గుర్తులేని మొత్తం_iterations = 0;
  hb_సదిశ_t<లేఖాచిత్రం::అధికప్రవాహం_రికార్డు_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  అయితే (round < max_విడతలు && మొత్తం_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_కేటాయించు (తార్కిక overflows_exist, లేఖాచిత్రం::will_అధికప్రవాహం (sorted_లేఖాచిత్రం, &overflows));
    ఒకవేళ (!overflows_exist)
      ఆపు;

    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_లేఖాచిత్రం, overflows);

    మొత్తం_iterations++;
    hb_సమితి_t ప్రాధాన్యత_bumped_parents;

    TRY_కేటాయించు (తార్కిక isolated, _try_isolating_subgraphs (overflows, sorted_లేఖాచిత్రం));
    ఒకవేళ (!isolated)
    {
      round++;
      TRY_కేటాయించు (తార్కిక processed, _process_overflows (overflows, ప్రాధాన్యత_bumped_parents, sorted_లేఖాచిత్రం));
      ఒకవేళ (!processed)
      {
        DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "No resolution available :(");
        ఆపు;
      }
    }

    TRY (sorted_లేఖాచిత్రం.క్రమీకరించు_అతితక్కువ_దూరం ());
  }

  TRY_కేటాయించు (తార్కిక has_అధికప్రవాహం, లేఖాచిత్రం::will_అధికప్రవాహం (sorted_లేఖాచిత్రం));
  ఒకవేళ (unlikely (has_అధికప్రవాహం))
  {
    ఒకవేళ (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      తిరిగి hb_resolve_లేఖాచిత్రం_overflows (పట్టిక_ట్యాగ్_2, max_విడతలు, నిజం, sorted_లేఖాచిత్రం);
    }

    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక, "Offset overflow resolution failed.");
    తిరిగి Err(లేఖాచిత్రం::అధికప్రవాహం_RESOLUTION_FAILED);
  }

  తిరిగి Ok();
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
మూస<రకం_పేరు T>
అంతఃవరుస hb_blob_t*
hb_resolve_overflows (స్థిరరాశి T& packed,
                      hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
                      గుర్తులేని max_విడతలు = 32,
                      తార్కిక recalculate_extensions = అబద్ధం) {
  స్వయంచాలక లేఖాచిత్రం_res = లేఖాచిత్రం_t::create (packed);
  ఒకవేళ (!లేఖాచిత్రం_res.is_ok ())
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "Graph creation failed, cause: %s", లేఖాచిత్రం::to_string(లేఖాచిత్రం_res.error()));
    తిరిగి శూన్యసూచిక;
  }
  లేఖాచిత్రం_t sorted_లేఖాచిత్రం = std::move (*లేఖాచిత్రం_res);

  స్వయంచాలక resolve_res = hb_resolve_లేఖాచిత్రం_overflows (పట్టిక_ట్యాగ్_2, max_విడతలు, recalculate_extensions, sorted_లేఖాచిత్రం);
  ఒకవేళ (resolve_res.is_err ())
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "Overflow resolution failed, cause: %s", లేఖాచిత్రం::to_string(resolve_res.error()));
    తిరిగి శూన్యసూచిక;
  }

  స్వయంచాలక serialize_res = లేఖాచిత్రం::serialize (sorted_లేఖాచిత్రం);
  ఒకవేళ (serialize_res.is_err ())
  {
    DEBUG_సందేశం (SUBSET_REPACK, శూన్యసూచిక,
               "Serialization failed, cause: %s", లేఖాచిత్రం::to_string(serialize_res.error()));
    తిరిగి శూన్యసూచిక;
  }

  తిరిగి *serialize_res;
}

#అయితే_ముగింపు /* HB_REPACKER_HH */
