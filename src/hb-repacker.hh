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

#که_نه_تعریف_شوی HB_REPACKER_HH
#تعریف_کړه HB_REPACKER_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-map.hh"
#ضمن "hb-vector.hh"
#ضمن "graph/graph.hh"
#ضمن "graph/gsubgpos-graph.hh"
#ضمن "graph/serialize.hh"

کارول ګراف::ګراف_t;
کارول ګراف::ګراف_پایله_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

بنية lookup_اندازه_t
{
  بینښه lookup_شاخص;
  size_t اندازه;
  بینښه عدد_subtables;

  ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
  {
    أرجع cmp ((ثابت lookup_اندازه_t*) a,
                (ثابت lookup_اندازه_t*) b);
  }

  ساكن صحيح cmp (ثابت lookup_اندازه_t* a, ثابت lookup_اندازه_t* b)
  {
    مزدوج subtables_فی_byte_a = (مزدوج) a->عدد_subtables / (مزدوج) a->اندازه;
    مزدوج subtables_فی_byte_b = (مزدوج) b->عدد_subtables / (مزدوج) b->اندازه;
    إذا (subtables_فی_byte_a == subtables_فی_byte_b) {
      أرجع b->lookup_شاخص - a->lookup_شاخص;
    }

    مزدوج cmp = subtables_فی_byte_b - subtables_فی_byte_a;
    إذا (cmp < 0) أرجع -1;
    إذا (cmp > 0) أرجع 1;
    أرجع 0;
  }
};

ساكن مضمن
ګراف_پایله_t<فراغ> _presplit_subtables_if_needed (ګراف::gsubgpos_ګراف_context_t& ext_context)
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
  hb_سېټ_t lookup_indices(ext_context.lookups.کلیګانې ());
  لكل (بینښه lookup_شاخص : lookup_indices)
  {
    ګراف::Lookup* lookup = ext_context.lookups.get(lookup_شاخص);
    TRY (lookup->وېشل_subtables_if_needed (ext_context, lookup_شاخص));
  }

  أرجع Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
ساكن مضمن
ګراف_پایله_t<فراغ> _promote_extensions_if_needed (ګراف::gsubgpos_ګراف_context_t& ext_context)
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

  بینښه ټول_lookup_جدول_sizes = 0;
  hb_ویکټور_t<lookup_اندازه_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), ریښتیا);

  لكل (بینښه lookup_شاخص : ext_context.lookups.کلیګانې ())
  {
    ثابت خپلکاری& lookup_v = ext_context.ګراف.vertices_[lookup_شاخص];
    ټول_lookup_جدول_sizes += lookup_v.جدول_اندازه ();

    ثابت ګراف::Lookup* lookup = ext_context.lookups.get(lookup_شاخص);
    hb_سېټ_t لیدل_شوی;
    lookup_sizes.push (lookup_اندازه_t {
        lookup_شاخص,
        ext_context.ګراف.پیدا_کړه_subgraph_اندازه (lookup_شاخص, لیدل_شوی).ارزښت_or (0),
        lookup->شمېره_د_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_اندازه = ext_context.ګراف.vertices_[ext_context.lookup_list_شاخص].جدول_اندازه ();
  size_t l2_l3_اندازه = lookup_list_اندازه + ټول_lookup_جدول_sizes; // Lookup List + Lookups
  size_t l3_l4_اندازه = ټول_lookup_جدول_sizes; // Lookups + SubTables
  size_t l4_plus_اندازه = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  لكل (خپلکاری p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    بینښه subtables_اندازه = p.عدد_subtables * 8;
    l3_l4_اندازه += subtables_اندازه;
    l4_plus_اندازه += subtables_اندازه;
  }

  منطقي layers_ډک = خطأ;
  لكل (خپلکاری p : lookup_sizes)
  {
    ثابت ګراف::Lookup* lookup = ext_context.lookups.get(p.lookup_شاخص);
    إذا (lookup->is_extension (ext_context.جدول_نښان_2))
      // already an extension so size is counted by the loop above.
      استمر;

    إذا (!layers_ډک)
    {
      size_t lookup_اندازه = ext_context.ګراف.vertices_[p.lookup_شاخص].جدول_اندازه ();
      hb_سېټ_t لیدل_شوی;
      size_t subtables_اندازه = ext_context.ګراف.پیدا_کړه_subgraph_اندازه (p.lookup_شاخص, لیدل_شوی, 1).ارزښت_or (0) - lookup_اندازه;
      size_t پاتې_شوی_اندازه = p.اندازه - subtables_اندازه - lookup_اندازه;

      l3_l4_اندازه   += subtables_اندازه;
      l3_l4_اندازه   -= p.عدد_subtables * 8;
      l4_plus_اندازه += subtables_اندازه + پاتې_شوی_اندازه;

      إذا (l2_l3_اندازه < (1 << 16)
          && l3_l4_اندازه < (1 << 16)
          && l4_plus_اندازه < (1 << 16)) استمر; // this lookup fits within all layers groups

      layers_ډک = ریښتیا;
    }

    TRY (ext_context.lookups.get(p.lookup_شاخص)->make_extension (ext_context, p.lookup_شاخص));
  }

  أرجع Ok();
}

ساكن مضمن
ګراف_پایله_t<منطقي> _try_isolating_subgraphs (ثابت hb_ویکټور_t<ګراف::ډکېدنه_ریکارډ_t>& overflows,
                                               ګراف_t& sorted_ګراف)
{
  بینښه space = 0;
  hb_سېټ_t roots_منزل_isolate;

  لكل (صحيح i = overflows.اوږدتیا - 1; i >= 0; i--)
  {
    ثابت ګراف::ډکېدنه_ریکارډ_t& r = overflows[i];

    بینښه ریښه;
    بینښه ډکېدنه_space = sorted_ګراف.space_for (r.پلار, &ریښه);
    إذا (!ډکېدنه_space) استمر;
    إذا (sorted_ګراف.عدد_roots_for_space_2 (ډکېدنه_space) <= 1) استمر;

    إذا (!space) {
      space = ډکېدنه_space;
    }

    إذا (space == ډکېدنه_space)
      roots_منزل_isolate.جمع_کړه(ریښه);
  }

  إذا (!roots_منزل_isolate) أرجع Ok(خطأ);

  بینښه اعظمي_منزل_move = hb_max ((sorted_ګراف.عدد_roots_for_space_2 (space) / 2u), 1u);
  إذا (roots_منزل_isolate.get_population () > اعظمي_منزل_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    صحيح extra = roots_منزل_isolate.get_population () - اعظمي_منزل_move;
    لكل (بینښه id : sorted_ګراف.ordering_) {
      إذا (!extra) اكسر;
      إذا (roots_منزل_isolate.has(id)) {
        roots_منزل_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_ګراف.عدد_roots_for_space_2 (space),
             roots_منزل_isolate.get_population (),
             sorted_ګراف.بل_space ());
  TRY (sorted_ګراف.isolate_subgraph (roots_منزل_isolate));
  TRY (sorted_ګراف.move_منزل_new_space (roots_منزل_isolate));

  أرجع Ok(ریښتیا);
}

ساكن مضمن
ګراف_پایله_t<منطقي> resolve_shared_ډکېدنه(ثابت hb_ویکټور_t<ګراف::ډکېدنه_ریکارډ_t>& overflows,
                                              صحيح ډکېدنه_شاخص,
                                              ګراف_t& sorted_ګراف)
{
  ثابت ګراف::ډکېدنه_ریکارډ_t& r = overflows[ډکېدنه_شاخص];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_سېټ_t parents;
  parents.جمع_کړه(r.پلار);
  لكل (صحيح i = ډکېدنه_شاخص - 1; i >= 0; i--) {
    ثابت ګراف::ډکېدنه_ریکارډ_t& r2 = overflows[i];
    إذا (r2.ماشوم == r.ماشوم) {
      parents.جمع_کړه(r2.پلار);
    }
  }

  خپلکاری پایله = sorted_ګراف.duplicate(&parents, r.ماشوم);
  إذا (!پایله.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    پایله = sorted_ګراف.duplicate(&parents, r.ماشوم);
  }

  إذا (!پایله.is_ok ()) أرجع Ok(خطأ);

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
    sorted_ګراف.vertices_[*پایله].give_max_لومړیتوب();
  }

  أرجع Ok(ریښتیا);
}

ساكن مضمن
ګراف_پایله_t<منطقي> _process_overflows (ثابت hb_ویکټور_t<ګراف::ډکېدنه_ریکارډ_t>& overflows,
                                         hb_سېټ_t& لومړیتوب_bumped_parents,
                                         ګراف_t& sorted_ګراف)
{
  منطقي resolution_attempted = خطأ;

  // Try resolving the furthest overflows first.
  لكل (صحيح i = overflows.اوږدتیا - 1; i >= 0; i--)
  {
    ثابت ګراف::ډکېدنه_ریکارډ_t& r = overflows[i];
    إذا (sorted_ګراف.vertices_[r.ماشوم].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_ټاکل (منطقي resolved, resolve_shared_ډکېدنه(overflows, i, sorted_ګراف));
      إذا (resolved)
        أرجع Ok(ریښتیا);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    إذا (sorted_ګراف.vertices_[r.ماشوم].is_ونې_پاڼه () && !لومړیتوب_bumped_parents.has (r.پلار))
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
      TRY_ټاکل (منطقي raised, sorted_ګراف.raise_childrens_لومړیتوب (r.پلار));
      إذا (raised) {
        لومړیتوب_bumped_parents.جمع_کړه (r.پلار);
        resolution_attempted = ریښتیا;
      }
      استمر;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  أرجع Ok(resolution_attempted);
}

مضمن ګراف_پایله_t<فراغ>
ټاکل_spaces_and_ترتیبول (ګراف_t& sorted_ګراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Assigning spaces to 32 bit subgraphs.");
  TRY_ټاکل (منطقي assigned, sorted_ګراف.ټاکل_spaces ());
  إذا (assigned)
    أرجع sorted_ګراف.ترتیبول_تر_ټولو_لنډ_واټن ();
  وإلا
    أرجع sorted_ګراف.ترتیبول_تر_ټولو_لنډ_واټن_if_needed ();
}

مضمن ګراف_پایله_t<فراغ>
_gsub_gpos_specialization (hb_نښان_t جدول_نښان_2,
                           منطقي always_recalculate_extensions,
                           ګراف_t& sorted_ګراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Applying GSUB/GPOS repacking specializations.");
  إذا (!always_recalculate_extensions) أرجع ټاکل_spaces_and_ترتیبول (sorted_ګراف);

  خپلکاری context_res = ګراف::gsubgpos_ګراف_context_t::create (جدول_نښان_2, sorted_ګراف);

  إذا (unlikely (context_res.is_err())) {
    إذا (context_res.error() == ګراف::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      أرجع ټاکل_spaces_and_ترتیبول (sorted_ګراف);
    وإلا
      // All other errors are propagated.
      أرجع Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  خپلکاری& ext_context = *context_res;
  DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_ګراف.ترتیبول_تر_ټولو_لنډ_واټن_if_needed ());

  TRY_ټاکل (منطقي will_ډکېدنه, ګراف::will_ډکېدنه (sorted_ګراف));
  إذا (!will_ډکېدنه) أرجع Ok();
  أرجع ټاکل_spaces_and_ترتیبول (sorted_ګراف);
}

مضمن ګراف_پایله_t<فراغ>
hb_resolve_ګراف_overflows (hb_نښان_t جدول_نښان_2,
                            بینښه max_پړاوونه ,
                            منطقي always_recalculate_extensions,
                            ګراف_t& sorted_ګراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Repacking %c%c%c%c.", HB_UNTAG(جدول_نښان_2));
  TRY (sorted_ګراف.ترتیبول_تر_ټولو_لنډ_واټن ());

  TRY_ټاکل (منطقي will_ډکېدنه, ګراف::will_ډکېدنه (sorted_ګراف));
  إذا (!will_ډکېدنه)
    أرجع Ok();

  منطقي is_gsub_or_gpos = (جدول_نښان_2 == HB_OT_نښان_GPOS ||  جدول_نښان_2 == HB_OT_نښان_GSUB);
  إذا (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (جدول_نښان_2, always_recalculate_extensions, sorted_ګراف));

  بینښه round = 0;
  بینښه ټول_iterations = 0;
  hb_ویکټور_t<ګراف::ډکېدنه_ریکارډ_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  طالما (round < max_پړاوونه && ټول_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_ټاکل (منطقي overflows_exist, ګراف::will_ډکېدنه (sorted_ګراف, &overflows));
    إذا (!overflows_exist)
      اكسر;

    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_ګراف, overflows);

    ټول_iterations++;
    hb_سېټ_t لومړیتوب_bumped_parents;

    TRY_ټاکل (منطقي isolated, _try_isolating_subgraphs (overflows, sorted_ګراف));
    إذا (!isolated)
    {
      round++;
      TRY_ټاکل (منطقي processed, _process_overflows (overflows, لومړیتوب_bumped_parents, sorted_ګراف));
      إذا (!processed)
      {
        DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "No resolution available :(");
        اكسر;
      }
    }

    TRY (sorted_ګراف.ترتیبول_تر_ټولو_لنډ_واټن ());
  }

  TRY_ټاکل (منطقي has_ډکېدنه, ګراف::will_ډکېدنه (sorted_ګراف));
  إذا (unlikely (has_ډکېدنه))
  {
    إذا (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      أرجع hb_resolve_ګراف_overflows (جدول_نښان_2, max_پړاوونه, ریښتیا, sorted_ګراف);
    }

    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره, "Offset overflow resolution failed.");
    أرجع Err(ګراف::ډکېدنه_RESOLUTION_FAILED);
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
کينډۍ<ډول_نوم T>
مضمن hb_blob_t*
hb_resolve_overflows (ثابت T& packed,
                      hb_نښان_t جدول_نښان_2,
                      بینښه max_پړاوونه = 32,
                      منطقي recalculate_extensions = خطأ) {
  خپلکاری ګراف_res = ګراف_t::create (packed);
  إذا (!ګراف_res.is_ok ())
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "Graph creation failed, cause: %s", ګراف::to_string(ګراف_res.error()));
    أرجع تش_اشاره;
  }
  ګراف_t sorted_ګراف = std::move (*ګراف_res);

  خپلکاری resolve_res = hb_resolve_ګراف_overflows (جدول_نښان_2, max_پړاوونه, recalculate_extensions, sorted_ګراف);
  إذا (resolve_res.is_err ())
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "Overflow resolution failed, cause: %s", ګراف::to_string(resolve_res.error()));
    أرجع تش_اشاره;
  }

  خپلکاری serialize_res = ګراف::serialize (sorted_ګراف);
  إذا (serialize_res.is_err ())
  {
    DEBUG_پیغام (SUBSET_REPACK, تش_اشاره,
               "Serialization failed, cause: %s", ګراف::to_string(serialize_res.error()));
    أرجع تش_اشاره;
  }

  أرجع *serialize_res;
}

#د_که_پای /* HB_REPACKER_HH */
