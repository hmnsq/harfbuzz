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

#جے_بنا_تعریف HB_REPACKER_HH
#تعریف_کرو HB_REPACKER_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-map.hh"
#ضمن "hb-vector.hh"
#ضمن "graph/graph.hh"
#ضمن "graph/gsubgpos-graph.hh"
#ضمن "graph/serialize.hh"

ورتوں گراف::گراف_t;
ورتوں گراف::گراف_نتیجہ_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

بنية lookup_ناپ_t
{
  بنانشانی lookup_اشاریہ;
  size_t ناپ;
  بنانشانی عدد_subtables;

  ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
  {
    أرجع cmp ((ثابت lookup_ناپ_t*) a,
                (ثابت lookup_ناپ_t*) b);
  }

  ساكن صحيح cmp (ثابت lookup_ناپ_t* a, ثابت lookup_ناپ_t* b)
  {
    مزدوج subtables_فی_byte_a = (مزدوج) a->عدد_subtables / (مزدوج) a->ناپ;
    مزدوج subtables_فی_byte_b = (مزدوج) b->عدد_subtables / (مزدوج) b->ناپ;
    إذا (subtables_فی_byte_a == subtables_فی_byte_b) {
      أرجع b->lookup_اشاریہ - a->lookup_اشاریہ;
    }

    مزدوج cmp = subtables_فی_byte_b - subtables_فی_byte_a;
    إذا (cmp < 0) أرجع -1;
    إذا (cmp > 0) أرجع 1;
    أرجع 0;
  }
};

ساكن مضمن
گراف_نتیجہ_t<فراغ> _presplit_subtables_if_needed (گراف::gsubgpos_گراف_context_t& ext_context)
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
  hb_سیٹ_t lookup_indices(ext_context.lookups.کلیدیں ());
  لكل (بنانشانی lookup_اشاریہ : lookup_indices)
  {
    گراف::Lookup* lookup = ext_context.lookups.get(lookup_اشاریہ);
    TRY (lookup->ونڈو_subtables_if_needed (ext_context, lookup_اشاریہ));
  }

  أرجع Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
ساكن مضمن
گراف_نتیجہ_t<فراغ> _promote_extensions_if_needed (گراف::gsubgpos_گراف_context_t& ext_context)
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

  بنانشانی کل_lookup_جدول_sizes = 0;
  hb_سمتیہ_t<lookup_ناپ_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), سچ);

  لكل (بنانشانی lookup_اشاریہ : ext_context.lookups.کلیدیں ())
  {
    ثابت خودکار& lookup_v = ext_context.گراف.vertices_[lookup_اشاریہ];
    کل_lookup_جدول_sizes += lookup_v.جدول_ناپ ();

    ثابت گراف::Lookup* lookup = ext_context.lookups.get(lookup_اشاریہ);
    hb_سیٹ_t ویکھیا;
    lookup_sizes.push (lookup_ناپ_t {
        lookup_اشاریہ,
        ext_context.گراف.لبھو_subgraph_ناپ (lookup_اشاریہ, ویکھیا).قدر_or (0),
        lookup->نمبر_دا_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_ناپ = ext_context.گراف.vertices_[ext_context.lookup_list_اشاریہ].جدول_ناپ ();
  size_t l2_l3_ناپ = lookup_list_ناپ + کل_lookup_جدول_sizes; // Lookup List + Lookups
  size_t l3_l4_ناپ = کل_lookup_جدول_sizes; // Lookups + SubTables
  size_t l4_plus_ناپ = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  لكل (خودکار p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    بنانشانی subtables_ناپ = p.عدد_subtables * 8;
    l3_l4_ناپ += subtables_ناپ;
    l4_plus_ناپ += subtables_ناپ;
  }

  منطقي layers_بھریا = خطأ;
  لكل (خودکار p : lookup_sizes)
  {
    ثابت گراف::Lookup* lookup = ext_context.lookups.get(p.lookup_اشاریہ);
    إذا (lookup->is_extension (ext_context.جدول_ٹیگ_2))
      // already an extension so size is counted by the loop above.
      استمر;

    إذا (!layers_بھریا)
    {
      size_t lookup_ناپ = ext_context.گراف.vertices_[p.lookup_اشاریہ].جدول_ناپ ();
      hb_سیٹ_t ویکھیا;
      size_t subtables_ناپ = ext_context.گراف.لبھو_subgraph_ناپ (p.lookup_اشاریہ, ویکھیا, 1).قدر_or (0) - lookup_ناپ;
      size_t بچیا_ناپ = p.ناپ - subtables_ناپ - lookup_ناپ;

      l3_l4_ناپ   += subtables_ناپ;
      l3_l4_ناپ   -= p.عدد_subtables * 8;
      l4_plus_ناپ += subtables_ناپ + بچیا_ناپ;

      إذا (l2_l3_ناپ < (1 << 16)
          && l3_l4_ناپ < (1 << 16)
          && l4_plus_ناپ < (1 << 16)) استمر; // this lookup fits within all layers groups

      layers_بھریا = سچ;
    }

    TRY (ext_context.lookups.get(p.lookup_اشاریہ)->make_extension (ext_context, p.lookup_اشاریہ));
  }

  أرجع Ok();
}

ساكن مضمن
گراف_نتیجہ_t<منطقي> _try_isolating_subgraphs (ثابت hb_سمتیہ_t<گراف::حد_توں_ودھ_ریکارڈ_t>& overflows,
                                               گراف_t& sorted_گراف)
{
  بنانشانی space = 0;
  hb_سیٹ_t roots_منزل_isolate;

  لكل (صحيح i = overflows.لمائی - 1; i >= 0; i--)
  {
    ثابت گراف::حد_توں_ودھ_ریکارڈ_t& r = overflows[i];

    بنانشانی جڑ;
    بنانشانی حد_توں_ودھ_space = sorted_گراف.space_for (r.ماپا, &جڑ);
    إذا (!حد_توں_ودھ_space) استمر;
    إذا (sorted_گراف.عدد_roots_for_space_2 (حد_توں_ودھ_space) <= 1) استمر;

    إذا (!space) {
      space = حد_توں_ودھ_space;
    }

    إذا (space == حد_توں_ودھ_space)
      roots_منزل_isolate.جوڑو(جڑ);
  }

  إذا (!roots_منزل_isolate) أرجع Ok(خطأ);

  بنانشانی ودھ_توں_ودھ_منزل_move = hb_max ((sorted_گراف.عدد_roots_for_space_2 (space) / 2u), 1u);
  إذا (roots_منزل_isolate.get_population () > ودھ_توں_ودھ_منزل_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    صحيح extra = roots_منزل_isolate.get_population () - ودھ_توں_ودھ_منزل_move;
    لكل (بنانشانی id : sorted_گراف.ordering_) {
      إذا (!extra) اكسر;
      إذا (roots_منزل_isolate.has(id)) {
        roots_منزل_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_گراف.عدد_roots_for_space_2 (space),
             roots_منزل_isolate.get_population (),
             sorted_گراف.اگلا_space ());
  TRY (sorted_گراف.isolate_subgraph (roots_منزل_isolate));
  TRY (sorted_گراف.move_منزل_new_space (roots_منزل_isolate));

  أرجع Ok(سچ);
}

ساكن مضمن
گراف_نتیجہ_t<منطقي> resolve_shared_حد_توں_ودھ(ثابت hb_سمتیہ_t<گراف::حد_توں_ودھ_ریکارڈ_t>& overflows,
                                              صحيح حد_توں_ودھ_اشاریہ,
                                              گراف_t& sorted_گراف)
{
  ثابت گراف::حد_توں_ودھ_ریکارڈ_t& r = overflows[حد_توں_ودھ_اشاریہ];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_سیٹ_t parents;
  parents.جوڑو(r.ماپا);
  لكل (صحيح i = حد_توں_ودھ_اشاریہ - 1; i >= 0; i--) {
    ثابت گراف::حد_توں_ودھ_ریکارڈ_t& r2 = overflows[i];
    إذا (r2.بچہ == r.بچہ) {
      parents.جوڑو(r2.ماپا);
    }
  }

  خودکار نتیجہ = sorted_گراف.duplicate(&parents, r.بچہ);
  إذا (!نتیجہ.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    نتیجہ = sorted_گراف.duplicate(&parents, r.بچہ);
  }

  إذا (!نتیجہ.is_ok ()) أرجع Ok(خطأ);

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
    sorted_گراف.vertices_[*نتیجہ].give_max_ترجیح();
  }

  أرجع Ok(سچ);
}

ساكن مضمن
گراف_نتیجہ_t<منطقي> _process_overflows (ثابت hb_سمتیہ_t<گراف::حد_توں_ودھ_ریکارڈ_t>& overflows,
                                         hb_سیٹ_t& ترجیح_bumped_parents,
                                         گراف_t& sorted_گراف)
{
  منطقي resolution_attempted = خطأ;

  // Try resolving the furthest overflows first.
  لكل (صحيح i = overflows.لمائی - 1; i >= 0; i--)
  {
    ثابت گراف::حد_توں_ودھ_ریکارڈ_t& r = overflows[i];
    إذا (sorted_گراف.vertices_[r.بچہ].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_سونپو (منطقي resolved, resolve_shared_حد_توں_ودھ(overflows, i, sorted_گراف));
      إذا (resolved)
        أرجع Ok(سچ);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    إذا (sorted_گراف.vertices_[r.بچہ].is_پتی () && !ترجیح_bumped_parents.has (r.ماپا))
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
      TRY_سونپو (منطقي raised, sorted_گراف.raise_childrens_ترجیح (r.ماپا));
      إذا (raised) {
        ترجیح_bumped_parents.جوڑو (r.ماپا);
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

مضمن گراف_نتیجہ_t<فراغ>
سونپو_spaces_and_ترتیب_دیو (گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Assigning spaces to 32 bit subgraphs.");
  TRY_سونپو (منطقي assigned, sorted_گراف.سونپو_spaces ());
  إذا (assigned)
    أرجع sorted_گراف.ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ ();
  وإلا
    أرجع sorted_گراف.ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ_if_needed ();
}

مضمن گراف_نتیجہ_t<فراغ>
_gsub_gpos_specialization (hb_ٹیگ_t جدول_ٹیگ_2,
                           منطقي always_recalculate_extensions,
                           گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Applying GSUB/GPOS repacking specializations.");
  إذا (!always_recalculate_extensions) أرجع سونپو_spaces_and_ترتیب_دیو (sorted_گراف);

  خودکار context_res = گراف::gsubgpos_گراف_context_t::create (جدول_ٹیگ_2, sorted_گراف);

  إذا (unlikely (context_res.is_err())) {
    إذا (context_res.error() == گراف::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      أرجع سونپو_spaces_and_ترتیب_دیو (sorted_گراف);
    وإلا
      // All other errors are propagated.
      أرجع Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  خودکار& ext_context = *context_res;
  DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_گراف.ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ_if_needed ());

  TRY_سونپو (منطقي will_حد_توں_ودھ, گراف::will_حد_توں_ودھ (sorted_گراف));
  إذا (!will_حد_توں_ودھ) أرجع Ok();
  أرجع سونپو_spaces_and_ترتیب_دیو (sorted_گراف);
}

مضمن گراف_نتیجہ_t<فراغ>
hb_resolve_گراف_overflows (hb_ٹیگ_t جدول_ٹیگ_2,
                            بنانشانی max_گیڑ ,
                            منطقي always_recalculate_extensions,
                            گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Repacking %c%c%c%c.", HB_UNTAG(جدول_ٹیگ_2));
  TRY (sorted_گراف.ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ ());

  TRY_سونپو (منطقي will_حد_توں_ودھ, گراف::will_حد_توں_ودھ (sorted_گراف));
  إذا (!will_حد_توں_ودھ)
    أرجع Ok();

  منطقي is_gsub_or_gpos = (جدول_ٹیگ_2 == HB_OT_ٹیگ_GPOS ||  جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB);
  إذا (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (جدول_ٹیگ_2, always_recalculate_extensions, sorted_گراف));

  بنانشانی round = 0;
  بنانشانی کل_iterations = 0;
  hb_سمتیہ_t<گراف::حد_توں_ودھ_ریکارڈ_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  طالما (round < max_گیڑ && کل_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_سونپو (منطقي overflows_exist, گراف::will_حد_توں_ودھ (sorted_گراف, &overflows));
    إذا (!overflows_exist)
      اكسر;

    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_گراف, overflows);

    کل_iterations++;
    hb_سیٹ_t ترجیح_bumped_parents;

    TRY_سونپو (منطقي isolated, _try_isolating_subgraphs (overflows, sorted_گراف));
    إذا (!isolated)
    {
      round++;
      TRY_سونپو (منطقي processed, _process_overflows (overflows, ترجیح_bumped_parents, sorted_گراف));
      إذا (!processed)
      {
        DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "No resolution available :(");
        اكسر;
      }
    }

    TRY (sorted_گراف.ترتیب_دیو_سبھ_توں_تھوڑا_فاصلہ ());
  }

  TRY_سونپو (منطقي has_حد_توں_ودھ, گراف::will_حد_توں_ودھ (sorted_گراف));
  إذا (unlikely (has_حد_توں_ودھ))
  {
    إذا (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      أرجع hb_resolve_گراف_overflows (جدول_ٹیگ_2, max_گیڑ, سچ, sorted_گراف);
    }

    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Offset overflow resolution failed.");
    أرجع Err(گراف::حد_توں_ودھ_RESOLUTION_FAILED);
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
سانچا<قسم_ناں T>
مضمن hb_blob_t*
hb_resolve_overflows (ثابت T& packed,
                      hb_ٹیگ_t جدول_ٹیگ_2,
                      بنانشانی max_گیڑ = 32,
                      منطقي recalculate_extensions = خطأ) {
  خودکار گراف_res = گراف_t::create (packed);
  إذا (!گراف_res.is_ok ())
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "Graph creation failed, cause: %s", گراف::to_string(گراف_res.error()));
    أرجع خالی_سوچک;
  }
  گراف_t sorted_گراف = std::move (*گراف_res);

  خودکار resolve_res = hb_resolve_گراف_overflows (جدول_ٹیگ_2, max_گیڑ, recalculate_extensions, sorted_گراف);
  إذا (resolve_res.is_err ())
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "Overflow resolution failed, cause: %s", گراف::to_string(resolve_res.error()));
    أرجع خالی_سوچک;
  }

  خودکار serialize_res = گراف::serialize (sorted_گراف);
  إذا (serialize_res.is_err ())
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "Serialization failed, cause: %s", گراف::to_string(serialize_res.error()));
    أرجع خالی_سوچک;
  }

  أرجع *serialize_res;
}

#جے_انت /* HB_REPACKER_HH */
