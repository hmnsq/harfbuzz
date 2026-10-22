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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_REPACKER_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_REPACKER_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "graph/graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "graph/gsubgpos-graph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "graph/serialize.hh"

ਵਰਤੋਂ ਗ੍ਰਾਫ::ਗ੍ਰਾਫ_t;
ਵਰਤੋਂ ਗ੍ਰਾਫ::ਗ੍ਰਾਫ_ਨਤੀਜਾ_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

ਬਣਤਰ lookup_ਮਾਪ_t
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ;
  size_t ਮਾਪ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_subtables;

  ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ* a, ਅਚਲ ਖਾਲੀ* b)
  {
    ਵਾਪਸ cmp ((ਅਚਲ lookup_ਮਾਪ_t*) a,
                (ਅਚਲ lookup_ਮਾਪ_t*) b);
  }

  ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ lookup_ਮਾਪ_t* a, ਅਚਲ lookup_ਮਾਪ_t* b)
  {
    ਦੁੱਗਣਾ subtables_ਪ੍ਰਤੀ_byte_a = (ਦੁੱਗਣਾ) a->ਅੰਕ_subtables / (ਦੁੱਗਣਾ) a->ਮਾਪ;
    ਦੁੱਗਣਾ subtables_ਪ੍ਰਤੀ_byte_b = (ਦੁੱਗਣਾ) b->ਅੰਕ_subtables / (ਦੁੱਗਣਾ) b->ਮਾਪ;
    ਜੇ (subtables_ਪ੍ਰਤੀ_byte_a == subtables_ਪ੍ਰਤੀ_byte_b) {
      ਵਾਪਸ b->lookup_ਸੂਚਕਾਂਕ - a->lookup_ਸੂਚਕਾਂਕ;
    }

    ਦੁੱਗਣਾ cmp = subtables_ਪ੍ਰਤੀ_byte_b - subtables_ਪ੍ਰਤੀ_byte_a;
    ਜੇ (cmp < 0) ਵਾਪਸ -1;
    ਜੇ (cmp > 0) ਵਾਪਸ 1;
    ਵਾਪਸ 0;
  }
};

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> _presplit_subtables_if_needed (ਗ੍ਰਾਫ::gsubgpos_ਗ੍ਰਾਫ_context_t& ext_context)
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
  hb_ਸੈੱਟ_t lookup_indices(ext_context.lookups.ਕੁੰਜੀਆਂ ());
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ : lookup_indices)
  {
    ਗ੍ਰਾਫ::Lookup* lookup = ext_context.lookups.get(lookup_ਸੂਚਕਾਂਕ);
    TRY (lookup->ਵੰਡੋ_subtables_if_needed (ext_context, lookup_ਸੂਚਕਾਂਕ));
  }

  ਵਾਪਸ Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ
ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ> _promote_extensions_if_needed (ਗ੍ਰਾਫ::gsubgpos_ਗ੍ਰਾਫ_context_t& ext_context)
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
  ਜੇ (!ext_context.lookups) ਵਾਪਸ Ok();

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_lookup_ਸਾਰਣੀ_sizes = 0;
  hb_ਸਦਿਸ਼_t<lookup_ਮਾਪ_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), ਸੱਚ);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ : ext_context.lookups.ਕੁੰਜੀਆਂ ())
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ& lookup_v = ext_context.ਗ੍ਰਾਫ.vertices_[lookup_ਸੂਚਕਾਂਕ];
    ਕੁੱਲ_lookup_ਸਾਰਣੀ_sizes += lookup_v.ਸਾਰਣੀ_ਮਾਪ ();

    ਅਚਲ ਗ੍ਰਾਫ::Lookup* lookup = ext_context.lookups.get(lookup_ਸੂਚਕਾਂਕ);
    hb_ਸੈੱਟ_t ਦੇਖਿਆ;
    lookup_sizes.push (lookup_ਮਾਪ_t {
        lookup_ਸੂਚਕਾਂਕ,
        ext_context.ਗ੍ਰਾਫ.ਲੱਭੋ_subgraph_ਮਾਪ (lookup_ਸੂਚਕਾਂਕ, ਦੇਖਿਆ).ਮੁੱਲ_or (0),
        lookup->ਸੰਖਿਆ_ਦਾ_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_ਮਾਪ = ext_context.ਗ੍ਰਾਫ.vertices_[ext_context.lookup_list_ਸੂਚਕਾਂਕ].ਸਾਰਣੀ_ਮਾਪ ();
  size_t l2_l3_ਮਾਪ = lookup_list_ਮਾਪ + ਕੁੱਲ_lookup_ਸਾਰਣੀ_sizes; // Lookup List + Lookups
  size_t l3_l4_ਮਾਪ = ਕੁੱਲ_lookup_ਸਾਰਣੀ_sizes; // Lookups + SubTables
  size_t l4_plus_ਮਾਪ = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  ਲਈ (ਸਵੈਚਾਲਿਤ p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    ਬਿਨਾਨਿਸ਼ਾਨੀ subtables_ਮਾਪ = p.ਅੰਕ_subtables * 8;
    l3_l4_ਮਾਪ += subtables_ਮਾਪ;
    l4_plus_ਮਾਪ += subtables_ਮਾਪ;
  }

  ਤਾਰਕਿਕ layers_ਭਰਿਆ = ਝੂਠ;
  ਲਈ (ਸਵੈਚਾਲਿਤ p : lookup_sizes)
  {
    ਅਚਲ ਗ੍ਰਾਫ::Lookup* lookup = ext_context.lookups.get(p.lookup_ਸੂਚਕਾਂਕ);
    ਜੇ (lookup->is_extension (ext_context.ਸਾਰਣੀ_ਟੈਗ_2))
      // already an extension so size is counted by the loop above.
      ਜਾਰੀ;

    ਜੇ (!layers_ਭਰਿਆ)
    {
      size_t lookup_ਮਾਪ = ext_context.ਗ੍ਰਾਫ.vertices_[p.lookup_ਸੂਚਕਾਂਕ].ਸਾਰਣੀ_ਮਾਪ ();
      hb_ਸੈੱਟ_t ਦੇਖਿਆ;
      size_t subtables_ਮਾਪ = ext_context.ਗ੍ਰਾਫ.ਲੱਭੋ_subgraph_ਮਾਪ (p.lookup_ਸੂਚਕਾਂਕ, ਦੇਖਿਆ, 1).ਮੁੱਲ_or (0) - lookup_ਮਾਪ;
      size_t ਬਚਿਆ_ਮਾਪ = p.ਮਾਪ - subtables_ਮਾਪ - lookup_ਮਾਪ;

      l3_l4_ਮਾਪ   += subtables_ਮਾਪ;
      l3_l4_ਮਾਪ   -= p.ਅੰਕ_subtables * 8;
      l4_plus_ਮਾਪ += subtables_ਮਾਪ + ਬਚਿਆ_ਮਾਪ;

      ਜੇ (l2_l3_ਮਾਪ < (1 << 16)
          && l3_l4_ਮਾਪ < (1 << 16)
          && l4_plus_ਮਾਪ < (1 << 16)) ਜਾਰੀ; // this lookup fits within all layers groups

      layers_ਭਰਿਆ = ਸੱਚ;
    }

    TRY (ext_context.lookups.get(p.lookup_ਸੂਚਕਾਂਕ)->make_extension (ext_context, p.lookup_ਸੂਚਕਾਂਕ));
  }

  ਵਾਪਸ Ok();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਤਾਰਕਿਕ> _try_isolating_subgraphs (ਅਚਲ hb_ਸਦਿਸ਼_t<ਗ੍ਰਾਫ::ਉਛਾਲ_ਰਿਕਾਰਡ_t>& overflows,
                                               ਗ੍ਰਾਫ_t& sorted_ਗ੍ਰਾਫ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ space = 0;
  hb_ਸੈੱਟ_t roots_ਮੰਜ਼ਿਲ_isolate;

  ਲਈ (ਪੂਰਨ i = overflows.ਲੰਮਾਈ - 1; i >= 0; i--)
  {
    ਅਚਲ ਗ੍ਰਾਫ::ਉਛਾਲ_ਰਿਕਾਰਡ_t& r = overflows[i];

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੜ੍ਹ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਛਾਲ_space = sorted_ਗ੍ਰਾਫ.space_for (r.ਮਾਪਾ, &ਜੜ੍ਹ);
    ਜੇ (!ਉਛਾਲ_space) ਜਾਰੀ;
    ਜੇ (sorted_ਗ੍ਰਾਫ.ਅੰਕ_roots_for_space_2 (ਉਛਾਲ_space) <= 1) ਜਾਰੀ;

    ਜੇ (!space) {
      space = ਉਛਾਲ_space;
    }

    ਜੇ (space == ਉਛਾਲ_space)
      roots_ਮੰਜ਼ਿਲ_isolate.ਜੋੜੋ(ਜੜ੍ਹ);
  }

  ਜੇ (!roots_ਮੰਜ਼ਿਲ_isolate) ਵਾਪਸ Ok(ਝੂਠ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਵੱਧ_ਤੋਂ_ਵੱਧ_ਮੰਜ਼ਿਲ_move = hb_max ((sorted_ਗ੍ਰਾਫ.ਅੰਕ_roots_for_space_2 (space) / 2u), 1u);
  ਜੇ (roots_ਮੰਜ਼ਿਲ_isolate.get_population () > ਵੱਧ_ਤੋਂ_ਵੱਧ_ਮੰਜ਼ਿਲ_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    ਪੂਰਨ extra = roots_ਮੰਜ਼ਿਲ_isolate.get_population () - ਵੱਧ_ਤੋਂ_ਵੱਧ_ਮੰਜ਼ਿਲ_move;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ id : sorted_ਗ੍ਰਾਫ.ordering_) {
      ਜੇ (!extra) ਰੋਕੋ;
      ਜੇ (roots_ਮੰਜ਼ਿਲ_isolate.has(id)) {
        roots_ਮੰਜ਼ਿਲ_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_ਗ੍ਰਾਫ.ਅੰਕ_roots_for_space_2 (space),
             roots_ਮੰਜ਼ਿਲ_isolate.get_population (),
             sorted_ਗ੍ਰਾਫ.ਅਗਲਾ_space ());
  TRY (sorted_ਗ੍ਰਾਫ.isolate_subgraph (roots_ਮੰਜ਼ਿਲ_isolate));
  TRY (sorted_ਗ੍ਰਾਫ.move_ਮੰਜ਼ਿਲ_new_space (roots_ਮੰਜ਼ਿਲ_isolate));

  ਵਾਪਸ Ok(ਸੱਚ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਤਾਰਕਿਕ> resolve_shared_ਉਛਾਲ(ਅਚਲ hb_ਸਦਿਸ਼_t<ਗ੍ਰਾਫ::ਉਛਾਲ_ਰਿਕਾਰਡ_t>& overflows,
                                              ਪੂਰਨ ਉਛਾਲ_ਸੂਚਕਾਂਕ,
                                              ਗ੍ਰਾਫ_t& sorted_ਗ੍ਰਾਫ)
{
  ਅਚਲ ਗ੍ਰਾਫ::ਉਛਾਲ_ਰਿਕਾਰਡ_t& r = overflows[ਉਛਾਲ_ਸੂਚਕਾਂਕ];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_ਸੈੱਟ_t parents;
  parents.ਜੋੜੋ(r.ਮਾਪਾ);
  ਲਈ (ਪੂਰਨ i = ਉਛਾਲ_ਸੂਚਕਾਂਕ - 1; i >= 0; i--) {
    ਅਚਲ ਗ੍ਰਾਫ::ਉਛਾਲ_ਰਿਕਾਰਡ_t& r2 = overflows[i];
    ਜੇ (r2.ਬੱਚਾ == r.ਬੱਚਾ) {
      parents.ਜੋੜੋ(r2.ਮਾਪਾ);
    }
  }

  ਸਵੈਚਾਲਿਤ ਨਤੀਜਾ = sorted_ਗ੍ਰਾਫ.duplicate(&parents, r.ਬੱਚਾ);
  ਜੇ (!ਨਤੀਜਾ.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    ਨਤੀਜਾ = sorted_ਗ੍ਰਾਫ.duplicate(&parents, r.ਬੱਚਾ);
  }

  ਜੇ (!ਨਤੀਜਾ.is_ok ()) ਵਾਪਸ Ok(ਝੂਠ);

  ਜੇ (parents.get_population() > 1) {
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
    sorted_ਗ੍ਰਾਫ.vertices_[*ਨਤੀਜਾ].give_max_ਤਰਜੀਹ();
  }

  ਵਾਪਸ Ok(ਸੱਚ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਤਾਰਕਿਕ> _process_overflows (ਅਚਲ hb_ਸਦਿਸ਼_t<ਗ੍ਰਾਫ::ਉਛਾਲ_ਰਿਕਾਰਡ_t>& overflows,
                                         hb_ਸੈੱਟ_t& ਤਰਜੀਹ_bumped_parents,
                                         ਗ੍ਰਾਫ_t& sorted_ਗ੍ਰਾਫ)
{
  ਤਾਰਕਿਕ resolution_attempted = ਝੂਠ;

  // Try resolving the furthest overflows first.
  ਲਈ (ਪੂਰਨ i = overflows.ਲੰਮਾਈ - 1; i >= 0; i--)
  {
    ਅਚਲ ਗ੍ਰਾਫ::ਉਛਾਲ_ਰਿਕਾਰਡ_t& r = overflows[i];
    ਜੇ (sorted_ਗ੍ਰਾਫ.vertices_[r.ਬੱਚਾ].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ resolved, resolve_shared_ਉਛਾਲ(overflows, i, sorted_ਗ੍ਰਾਫ));
      ਜੇ (resolved)
        ਵਾਪਸ Ok(ਸੱਚ);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    ਜੇ (sorted_ਗ੍ਰਾਫ.vertices_[r.ਬੱਚਾ].is_ਪੱਤੀ () && !ਤਰਜੀਹ_bumped_parents.has (r.ਮਾਪਾ))
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
      TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ raised, sorted_ਗ੍ਰਾਫ.raise_childrens_ਤਰਜੀਹ (r.ਮਾਪਾ));
      ਜੇ (raised) {
        ਤਰਜੀਹ_bumped_parents.ਜੋੜੋ (r.ਮਾਪਾ);
        resolution_attempted = ਸੱਚ;
      }
      ਜਾਰੀ;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  ਵਾਪਸ Ok(resolution_attempted);
}

ਸਤਰਅੰਦਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>
ਸੌਂਪੋ_spaces_and_ਛਾਂਟੋ (ਗ੍ਰਾਫ_t& sorted_ਗ੍ਰਾਫ /* IN/OUT */)
{
  DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Assigning spaces to 32 bit subgraphs.");
  TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ assigned, sorted_ਗ੍ਰਾਫ.ਸੌਂਪੋ_spaces ());
  ਜੇ (assigned)
    ਵਾਪਸ sorted_ਗ੍ਰਾਫ.ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ ();
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ sorted_ਗ੍ਰਾਫ.ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ_if_needed ();
}

ਸਤਰਅੰਦਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>
_gsub_gpos_specialization (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
                           ਤਾਰਕਿਕ always_recalculate_extensions,
                           ਗ੍ਰਾਫ_t& sorted_ਗ੍ਰਾਫ /* IN/OUT */)
{
  DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Applying GSUB/GPOS repacking specializations.");
  ਜੇ (!always_recalculate_extensions) ਵਾਪਸ ਸੌਂਪੋ_spaces_and_ਛਾਂਟੋ (sorted_ਗ੍ਰਾਫ);

  ਸਵੈਚਾਲਿਤ context_res = ਗ੍ਰਾਫ::gsubgpos_ਗ੍ਰਾਫ_context_t::create (ਸਾਰਣੀ_ਟੈਗ_2, sorted_ਗ੍ਰਾਫ);

  ਜੇ (unlikely (context_res.is_err())) {
    ਜੇ (context_res.error() == ਗ੍ਰਾਫ::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      ਵਾਪਸ ਸੌਂਪੋ_spaces_and_ਛਾਂਟੋ (sorted_ਗ੍ਰਾਫ);
    ਨਹੀਂਤਾਂ
      // All other errors are propagated.
      ਵਾਪਸ Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  ਸਵੈਚਾਲਿਤ& ext_context = *context_res;
  DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_ਗ੍ਰਾਫ.ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ_if_needed ());

  TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ will_ਉਛਾਲ, ਗ੍ਰਾਫ::will_ਉਛਾਲ (sorted_ਗ੍ਰਾਫ));
  ਜੇ (!will_ਉਛਾਲ) ਵਾਪਸ Ok();
  ਵਾਪਸ ਸੌਂਪੋ_spaces_and_ਛਾਂਟੋ (sorted_ਗ੍ਰਾਫ);
}

ਸਤਰਅੰਦਰ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>
hb_resolve_ਗ੍ਰਾਫ_overflows (hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਗੇੜ ,
                            ਤਾਰਕਿਕ always_recalculate_extensions,
                            ਗ੍ਰਾਫ_t& sorted_ਗ੍ਰਾਫ /* IN/OUT */)
{
  DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Repacking %c%c%c%c.", HB_UNTAG(ਸਾਰਣੀ_ਟੈਗ_2));
  TRY (sorted_ਗ੍ਰਾਫ.ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ ());

  TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ will_ਉਛਾਲ, ਗ੍ਰਾਫ::will_ਉਛਾਲ (sorted_ਗ੍ਰਾਫ));
  ਜੇ (!will_ਉਛਾਲ)
    ਵਾਪਸ Ok();

  ਤਾਰਕਿਕ is_gsub_or_gpos = (ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GPOS ||  ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB);
  ਜੇ (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (ਸਾਰਣੀ_ਟੈਗ_2, always_recalculate_extensions, sorted_ਗ੍ਰਾਫ));

  ਬਿਨਾਨਿਸ਼ਾਨੀ round = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_iterations = 0;
  hb_ਸਦਿਸ਼_t<ਗ੍ਰਾਫ::ਉਛਾਲ_ਰਿਕਾਰਡ_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  ਜਦੋਂ (round < max_ਗੇੜ && ਕੁੱਲ_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ overflows_exist, ਗ੍ਰਾਫ::will_ਉਛਾਲ (sorted_ਗ੍ਰਾਫ, &overflows));
    ਜੇ (!overflows_exist)
      ਰੋਕੋ;

    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_ਗ੍ਰਾਫ, overflows);

    ਕੁੱਲ_iterations++;
    hb_ਸੈੱਟ_t ਤਰਜੀਹ_bumped_parents;

    TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ isolated, _try_isolating_subgraphs (overflows, sorted_ਗ੍ਰਾਫ));
    ਜੇ (!isolated)
    {
      round++;
      TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ processed, _process_overflows (overflows, ਤਰਜੀਹ_bumped_parents, sorted_ਗ੍ਰਾਫ));
      ਜੇ (!processed)
      {
        DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "No resolution available :(");
        ਰੋਕੋ;
      }
    }

    TRY (sorted_ਗ੍ਰਾਫ.ਛਾਂਟੋ_ਸਭ_ਤੋਂ_ਥੋੜ੍ਹਾ_ਦੂਰੀ ());
  }

  TRY_ਸੌਂਪੋ (ਤਾਰਕਿਕ has_ਉਛਾਲ, ਗ੍ਰਾਫ::will_ਉਛਾਲ (sorted_ਗ੍ਰਾਫ));
  ਜੇ (unlikely (has_ਉਛਾਲ))
  {
    ਜੇ (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      ਵਾਪਸ hb_resolve_ਗ੍ਰਾਫ_overflows (ਸਾਰਣੀ_ਟੈਗ_2, max_ਗੇੜ, ਸੱਚ, sorted_ਗ੍ਰਾਫ);
    }

    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ, "Offset overflow resolution failed.");
    ਵਾਪਸ Err(ਗ੍ਰਾਫ::ਉਛਾਲ_RESOLUTION_FAILED);
  }

  ਵਾਪਸ Ok();
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
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਤਰਅੰਦਰ hb_blob_t*
hb_resolve_overflows (ਅਚਲ T& packed,
                      hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
                      ਬਿਨਾਨਿਸ਼ਾਨੀ max_ਗੇੜ = 32,
                      ਤਾਰਕਿਕ recalculate_extensions = ਝੂਠ) {
  ਸਵੈਚਾਲਿਤ ਗ੍ਰਾਫ_res = ਗ੍ਰਾਫ_t::create (packed);
  ਜੇ (!ਗ੍ਰਾਫ_res.is_ok ())
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "Graph creation failed, cause: %s", ਗ੍ਰਾਫ::to_string(ਗ੍ਰਾਫ_res.error()));
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
  ਗ੍ਰਾਫ_t sorted_ਗ੍ਰਾਫ = std::move (*ਗ੍ਰਾਫ_res);

  ਸਵੈਚਾਲਿਤ resolve_res = hb_resolve_ਗ੍ਰਾਫ_overflows (ਸਾਰਣੀ_ਟੈਗ_2, max_ਗੇੜ, recalculate_extensions, sorted_ਗ੍ਰਾਫ);
  ਜੇ (resolve_res.is_err ())
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "Overflow resolution failed, cause: %s", ਗ੍ਰਾਫ::to_string(resolve_res.error()));
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਵੈਚਾਲਿਤ serialize_res = ਗ੍ਰਾਫ::serialize (sorted_ਗ੍ਰਾਫ);
  ਜੇ (serialize_res.is_err ())
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET_REPACK, ਖਾਲੀ_ਸੂਚਕ,
               "Serialization failed, cause: %s", ਗ੍ਰਾਫ::to_string(serialize_res.error()));
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਵਾਪਸ *serialize_res;
}

#ਜੇ_ਅੰਤ /* HB_REPACKER_HH */
