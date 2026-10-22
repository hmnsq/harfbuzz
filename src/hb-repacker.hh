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

#جيڪڏهن_وصف_نه_ٿيل HB_REPACKER_HH
#وصف_ڪريو HB_REPACKER_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-map.hh"
#ضمن "hb-vector.hh"
#ضمن "graph/graph.hh"
#ضمن "graph/gsubgpos-graph.hh"
#ضمن "graph/serialize.hh"

استعمال گراف::گراف_t;
استعمال گراف::گراف_نتيجو_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

بنية lookup_سائيز_t
{
  بنانشان lookup_اشاريو;
  size_t سائيز;
  بنانشان انگ_subtables;

  ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
  {
    أرجع cmp ((ثابت lookup_سائيز_t*) a,
                (ثابت lookup_سائيز_t*) b);
  }

  ساكن صحيح cmp (ثابت lookup_سائيز_t* a, ثابت lookup_سائيز_t* b)
  {
    مزدوج subtables_في_byte_a = (مزدوج) a->انگ_subtables / (مزدوج) a->سائيز;
    مزدوج subtables_في_byte_b = (مزدوج) b->انگ_subtables / (مزدوج) b->سائيز;
    إذا (subtables_في_byte_a == subtables_في_byte_b) {
      أرجع b->lookup_اشاريو - a->lookup_اشاريو;
    }

    مزدوج cmp = subtables_في_byte_b - subtables_في_byte_a;
    إذا (cmp < 0) أرجع -1;
    إذا (cmp > 0) أرجع 1;
    أرجع 0;
  }
};

ساكن مضمن
گراف_نتيجو_t<فراغ> _presplit_subtables_if_needed (گراف::gsubgpos_گراف_context_t& ext_context)
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
  hb_سيٽ_t lookup_indices(ext_context.lookups.ڪنجيون ());
  لكل (بنانشان lookup_اشاريو : lookup_indices)
  {
    گراف::Lookup* lookup = ext_context.lookups.get(lookup_اشاريو);
    TRY (lookup->ورهايو_subtables_if_needed (ext_context, lookup_اشاريو));
  }

  أرجع Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
ساكن مضمن
گراف_نتيجو_t<فراغ> _promote_extensions_if_needed (گراف::gsubgpos_گراف_context_t& ext_context)
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

  بنانشان ڪل_lookup_جدول_sizes = 0;
  hb_ويڪٽر_t<lookup_سائيز_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), سچ);

  لكل (بنانشان lookup_اشاريو : ext_context.lookups.ڪنجيون ())
  {
    ثابت خودڪار& lookup_v = ext_context.گراف.vertices_[lookup_اشاريو];
    ڪل_lookup_جدول_sizes += lookup_v.جدول_سائيز ();

    ثابت گراف::Lookup* lookup = ext_context.lookups.get(lookup_اشاريو);
    hb_سيٽ_t گهميل;
    lookup_sizes.push (lookup_سائيز_t {
        lookup_اشاريو,
        ext_context.گراف.ڳوليو_subgraph_سائيز (lookup_اشاريو, گهميل).قدر_or (0),
        lookup->عدد_جو_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_سائيز = ext_context.گراف.vertices_[ext_context.lookup_list_اشاريو].جدول_سائيز ();
  size_t l2_l3_سائيز = lookup_list_سائيز + ڪل_lookup_جدول_sizes; // Lookup List + Lookups
  size_t l3_l4_سائيز = ڪل_lookup_جدول_sizes; // Lookups + SubTables
  size_t l4_plus_سائيز = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  لكل (خودڪار p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    بنانشان subtables_سائيز = p.انگ_subtables * 8;
    l3_l4_سائيز += subtables_سائيز;
    l4_plus_سائيز += subtables_سائيز;
  }

  منطقي layers_ڀريل = خطأ;
  لكل (خودڪار p : lookup_sizes)
  {
    ثابت گراف::Lookup* lookup = ext_context.lookups.get(p.lookup_اشاريو);
    إذا (lookup->is_extension (ext_context.جدول_ٽيگ_2))
      // already an extension so size is counted by the loop above.
      استمر;

    إذا (!layers_ڀريل)
    {
      size_t lookup_سائيز = ext_context.گراف.vertices_[p.lookup_اشاريو].جدول_سائيز ();
      hb_سيٽ_t گهميل;
      size_t subtables_سائيز = ext_context.گراف.ڳوليو_subgraph_سائيز (p.lookup_اشاريو, گهميل, 1).قدر_or (0) - lookup_سائيز;
      size_t بچيل_سائيز = p.سائيز - subtables_سائيز - lookup_سائيز;

      l3_l4_سائيز   += subtables_سائيز;
      l3_l4_سائيز   -= p.انگ_subtables * 8;
      l4_plus_سائيز += subtables_سائيز + بچيل_سائيز;

      إذا (l2_l3_سائيز < (1 << 16)
          && l3_l4_سائيز < (1 << 16)
          && l4_plus_سائيز < (1 << 16)) استمر; // this lookup fits within all layers groups

      layers_ڀريل = سچ;
    }

    TRY (ext_context.lookups.get(p.lookup_اشاريو)->make_extension (ext_context, p.lookup_اشاريو));
  }

  أرجع Ok();
}

ساكن مضمن
گراف_نتيجو_t<منطقي> _try_isolating_subgraphs (ثابت hb_ويڪٽر_t<گراف::حد_کان_مٿي_رڪارڊ_t>& overflows,
                                               گراف_t& sorted_گراف)
{
  بنانشان space = 0;
  hb_سيٽ_t roots_منزل_isolate;

  لكل (صحيح i = overflows.ڊيگهائي - 1; i >= 0; i--)
  {
    ثابت گراف::حد_کان_مٿي_رڪارڊ_t& r = overflows[i];

    بنانشان پاڙ;
    بنانشان حد_کان_مٿي_space = sorted_گراف.space_for (r.والد, &پاڙ);
    إذا (!حد_کان_مٿي_space) استمر;
    إذا (sorted_گراف.انگ_roots_for_space_2 (حد_کان_مٿي_space) <= 1) استمر;

    إذا (!space) {
      space = حد_کان_مٿي_space;
    }

    إذا (space == حد_کان_مٿي_space)
      roots_منزل_isolate.جوڙيو(پاڙ);
  }

  إذا (!roots_منزل_isolate) أرجع Ok(خطأ);

  بنانشان وڌ_کان_وڌ_منزل_move = hb_max ((sorted_گراف.انگ_roots_for_space_2 (space) / 2u), 1u);
  إذا (roots_منزل_isolate.get_population () > وڌ_کان_وڌ_منزل_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    صحيح extra = roots_منزل_isolate.get_population () - وڌ_کان_وڌ_منزل_move;
    لكل (بنانشان id : sorted_گراف.ordering_) {
      إذا (!extra) اكسر;
      إذا (roots_منزل_isolate.has(id)) {
        roots_منزل_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_گراف.انگ_roots_for_space_2 (space),
             roots_منزل_isolate.get_population (),
             sorted_گراف.ايندڙ_space ());
  TRY (sorted_گراف.isolate_subgraph (roots_منزل_isolate));
  TRY (sorted_گراف.move_منزل_new_space (roots_منزل_isolate));

  أرجع Ok(سچ);
}

ساكن مضمن
گراف_نتيجو_t<منطقي> resolve_shared_حد_کان_مٿي(ثابت hb_ويڪٽر_t<گراف::حد_کان_مٿي_رڪارڊ_t>& overflows,
                                              صحيح حد_کان_مٿي_اشاريو,
                                              گراف_t& sorted_گراف)
{
  ثابت گراف::حد_کان_مٿي_رڪارڊ_t& r = overflows[حد_کان_مٿي_اشاريو];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_سيٽ_t parents;
  parents.جوڙيو(r.والد);
  لكل (صحيح i = حد_کان_مٿي_اشاريو - 1; i >= 0; i--) {
    ثابت گراف::حد_کان_مٿي_رڪارڊ_t& r2 = overflows[i];
    إذا (r2.ٻار == r.ٻار) {
      parents.جوڙيو(r2.والد);
    }
  }

  خودڪار نتيجو = sorted_گراف.duplicate(&parents, r.ٻار);
  إذا (!نتيجو.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    نتيجو = sorted_گراف.duplicate(&parents, r.ٻار);
  }

  إذا (!نتيجو.is_ok ()) أرجع Ok(خطأ);

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
    sorted_گراف.vertices_[*نتيجو].give_max_ترجيح();
  }

  أرجع Ok(سچ);
}

ساكن مضمن
گراف_نتيجو_t<منطقي> _process_overflows (ثابت hb_ويڪٽر_t<گراف::حد_کان_مٿي_رڪارڊ_t>& overflows,
                                         hb_سيٽ_t& ترجيح_bumped_parents,
                                         گراف_t& sorted_گراف)
{
  منطقي resolution_attempted = خطأ;

  // Try resolving the furthest overflows first.
  لكل (صحيح i = overflows.ڊيگهائي - 1; i >= 0; i--)
  {
    ثابت گراف::حد_کان_مٿي_رڪارڊ_t& r = overflows[i];
    إذا (sorted_گراف.vertices_[r.ٻار].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_سونپيو (منطقي resolved, resolve_shared_حد_کان_مٿي(overflows, i, sorted_گراف));
      إذا (resolved)
        أرجع Ok(سچ);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    إذا (sorted_گراف.vertices_[r.ٻار].is_پن () && !ترجيح_bumped_parents.has (r.والد))
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
      TRY_سونپيو (منطقي raised, sorted_گراف.raise_childrens_ترجيح (r.والد));
      إذا (raised) {
        ترجيح_bumped_parents.جوڙيو (r.والد);
        resolution_attempted = سچ;
      }
      استمر;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  أرجع Ok(resolution_attempted);
}

مضمن گراف_نتيجو_t<فراغ>
سونپيو_spaces_and_ترتيب_ڏيو (گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Assigning spaces to 32 bit subgraphs.");
  TRY_سونپيو (منطقي assigned, sorted_گراف.سونپيو_spaces ());
  إذا (assigned)
    أرجع sorted_گراف.ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو ();
  وإلا
    أرجع sorted_گراف.ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو_if_needed ();
}

مضمن گراف_نتيجو_t<فراغ>
_gsub_gpos_specialization (hb_ٽيگ_t جدول_ٽيگ_2,
                           منطقي always_recalculate_extensions,
                           گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Applying GSUB/GPOS repacking specializations.");
  إذا (!always_recalculate_extensions) أرجع سونپيو_spaces_and_ترتيب_ڏيو (sorted_گراف);

  خودڪار context_res = گراف::gsubgpos_گراف_context_t::create (جدول_ٽيگ_2, sorted_گراف);

  إذا (unlikely (context_res.is_err())) {
    إذا (context_res.error() == گراف::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      أرجع سونپيو_spaces_and_ترتيب_ڏيو (sorted_گراف);
    وإلا
      // All other errors are propagated.
      أرجع Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  خودڪار& ext_context = *context_res;
  DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_گراف.ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو_if_needed ());

  TRY_سونپيو (منطقي will_حد_کان_مٿي, گراف::will_حد_کان_مٿي (sorted_گراف));
  إذا (!will_حد_کان_مٿي) أرجع Ok();
  أرجع سونپيو_spaces_and_ترتيب_ڏيو (sorted_گراف);
}

مضمن گراف_نتيجو_t<فراغ>
hb_resolve_گراف_overflows (hb_ٽيگ_t جدول_ٽيگ_2,
                            بنانشان max_ڦيرا ,
                            منطقي always_recalculate_extensions,
                            گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Repacking %c%c%c%c.", HB_UNTAG(جدول_ٽيگ_2));
  TRY (sorted_گراف.ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو ());

  TRY_سونپيو (منطقي will_حد_کان_مٿي, گراف::will_حد_کان_مٿي (sorted_گراف));
  إذا (!will_حد_کان_مٿي)
    أرجع Ok();

  منطقي is_gsub_or_gpos = (جدول_ٽيگ_2 == HB_OT_ٽيگ_GPOS ||  جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB);
  إذا (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (جدول_ٽيگ_2, always_recalculate_extensions, sorted_گراف));

  بنانشان round = 0;
  بنانشان ڪل_iterations = 0;
  hb_ويڪٽر_t<گراف::حد_کان_مٿي_رڪارڊ_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  طالما (round < max_ڦيرا && ڪل_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_سونپيو (منطقي overflows_exist, گراف::will_حد_کان_مٿي (sorted_گراف, &overflows));
    إذا (!overflows_exist)
      اكسر;

    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_گراف, overflows);

    ڪل_iterations++;
    hb_سيٽ_t ترجيح_bumped_parents;

    TRY_سونپيو (منطقي isolated, _try_isolating_subgraphs (overflows, sorted_گراف));
    إذا (!isolated)
    {
      round++;
      TRY_سونپيو (منطقي processed, _process_overflows (overflows, ترجيح_bumped_parents, sorted_گراف));
      إذا (!processed)
      {
        DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "No resolution available :(");
        اكسر;
      }
    }

    TRY (sorted_گراف.ترتيب_ڏيو_سڀ_کان_ٿورو_مفاصلو ());
  }

  TRY_سونپيو (منطقي has_حد_کان_مٿي, گراف::will_حد_کان_مٿي (sorted_گراف));
  إذا (unlikely (has_حد_کان_مٿي))
  {
    إذا (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      أرجع hb_resolve_گراف_overflows (جدول_ٽيگ_2, max_ڦيرا, سچ, sorted_گراف);
    }

    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو, "Offset overflow resolution failed.");
    أرجع Err(گراف::حد_کان_مٿي_RESOLUTION_FAILED);
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
سانچو<قسم_نالو T>
مضمن hb_blob_t*
hb_resolve_overflows (ثابت T& packed,
                      hb_ٽيگ_t جدول_ٽيگ_2,
                      بنانشان max_ڦيرا = 32,
                      منطقي recalculate_extensions = خطأ) {
  خودڪار گراف_res = گراف_t::create (packed);
  إذا (!گراف_res.is_ok ())
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "Graph creation failed, cause: %s", گراف::to_string(گراف_res.error()));
    أرجع خالي_اشارو;
  }
  گراف_t sorted_گراف = std::move (*گراف_res);

  خودڪار resolve_res = hb_resolve_گراف_overflows (جدول_ٽيگ_2, max_ڦيرا, recalculate_extensions, sorted_گراف);
  إذا (resolve_res.is_err ())
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "Overflow resolution failed, cause: %s", گراف::to_string(resolve_res.error()));
    أرجع خالي_اشارو;
  }

  خودڪار serialize_res = گراف::serialize (sorted_گراف);
  إذا (serialize_res.is_err ())
  {
    DEBUG_نياپو (SUBSET_REPACK, خالي_اشارو,
               "Serialization failed, cause: %s", گراف::to_string(serialize_res.error()));
    أرجع خالي_اشارو;
  }

  أرجع *serialize_res;
}

#جيڪڏهن_پورو /* HB_REPACKER_HH */
