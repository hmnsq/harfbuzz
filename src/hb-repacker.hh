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

#اگر_تعریف_نشده HB_REPACKER_HH
#تعریف_کن HB_REPACKER_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-map.hh"
#ضمن "hb-vector.hh"
#ضمن "graph/graph.hh"
#ضمن "graph/gsubgpos-graph.hh"
#ضمن "graph/serialize.hh"

استفاده گراف::گراف_t;
استفاده گراف::گراف_نتیجه_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

بنية lookup_اندازه_t
{
  بیعلامت lookup_شاخص;
  size_t اندازه;
  بیعلامت رقم_subtables;

  ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
  {
    أرجع cmp ((ثابت lookup_اندازه_t*) a,
                (ثابت lookup_اندازه_t*) b);
  }

  ساكن صحيح cmp (ثابت lookup_اندازه_t* a, ثابت lookup_اندازه_t* b)
  {
    مزدوج subtables_در_هر_byte_a = (مزدوج) a->رقم_subtables / (مزدوج) a->اندازه;
    مزدوج subtables_در_هر_byte_b = (مزدوج) b->رقم_subtables / (مزدوج) b->اندازه;
    إذا (subtables_در_هر_byte_a == subtables_در_هر_byte_b) {
      أرجع b->lookup_شاخص - a->lookup_شاخص;
    }

    مزدوج cmp = subtables_در_هر_byte_b - subtables_در_هر_byte_a;
    إذا (cmp < 0) أرجع -1;
    إذا (cmp > 0) أرجع 1;
    أرجع 0;
  }
};

ساكن مضمن
گراف_نتیجه_t<فراغ> _presplit_subtables_if_needed (گراف::gsubgpos_گراف_context_t& ext_context)
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
  hb_مجموعه_t lookup_indices(ext_context.lookups.کلیدها ());
  لكل (بیعلامت lookup_شاخص : lookup_indices)
  {
    گراف::Lookup* lookup = ext_context.lookups.get(lookup_شاخص);
    TRY (lookup->تقسیم_subtables_if_needed (ext_context, lookup_شاخص));
  }

  أرجع Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
ساكن مضمن
گراف_نتیجه_t<فراغ> _promote_extensions_if_needed (گراف::gsubgpos_گراف_context_t& ext_context)
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

  بیعلامت مجموع_lookup_جدول_sizes = 0;
  hb_وکتور_t<lookup_اندازه_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), درست);

  لكل (بیعلامت lookup_شاخص : ext_context.lookups.کلیدها ())
  {
    ثابت خودکار& lookup_v = ext_context.گراف.vertices_[lookup_شاخص];
    مجموع_lookup_جدول_sizes += lookup_v.جدول_اندازه ();

    ثابت گراف::Lookup* lookup = ext_context.lookups.get(lookup_شاخص);
    hb_مجموعه_t دیدهشده;
    lookup_sizes.push (lookup_اندازه_t {
        lookup_شاخص,
        ext_context.گراف.پیدا_کن_subgraph_اندازه (lookup_شاخص, دیدهشده).مقدار_or (0),
        lookup->عدد_از_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_اندازه = ext_context.گراف.vertices_[ext_context.lookup_list_شاخص].جدول_اندازه ();
  size_t l2_l3_اندازه = lookup_list_اندازه + مجموع_lookup_جدول_sizes; // Lookup List + Lookups
  size_t l3_l4_اندازه = مجموع_lookup_جدول_sizes; // Lookups + SubTables
  size_t l4_plus_اندازه = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  لكل (خودکار p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    بیعلامت subtables_اندازه = p.رقم_subtables * 8;
    l3_l4_اندازه += subtables_اندازه;
    l4_plus_اندازه += subtables_اندازه;
  }

  منطقي layers_پر = خطأ;
  لكل (خودکار p : lookup_sizes)
  {
    ثابت گراف::Lookup* lookup = ext_context.lookups.get(p.lookup_شاخص);
    إذا (lookup->is_extension (ext_context.جدول_برچسب_2))
      // already an extension so size is counted by the loop above.
      استمر;

    إذا (!layers_پر)
    {
      size_t lookup_اندازه = ext_context.گراف.vertices_[p.lookup_شاخص].جدول_اندازه ();
      hb_مجموعه_t دیدهشده;
      size_t subtables_اندازه = ext_context.گراف.پیدا_کن_subgraph_اندازه (p.lookup_شاخص, دیدهشده, 1).مقدار_or (0) - lookup_اندازه;
      size_t مانده_اندازه = p.اندازه - subtables_اندازه - lookup_اندازه;

      l3_l4_اندازه   += subtables_اندازه;
      l3_l4_اندازه   -= p.رقم_subtables * 8;
      l4_plus_اندازه += subtables_اندازه + مانده_اندازه;

      إذا (l2_l3_اندازه < (1 << 16)
          && l3_l4_اندازه < (1 << 16)
          && l4_plus_اندازه < (1 << 16)) استمر; // this lookup fits within all layers groups

      layers_پر = درست;
    }

    TRY (ext_context.lookups.get(p.lookup_شاخص)->make_extension (ext_context, p.lookup_شاخص));
  }

  أرجع Ok();
}

ساكن مضمن
گراف_نتیجه_t<منطقي> _try_isolating_subgraphs (ثابت hb_وکتور_t<گراف::سرریز_رکورد_t>& overflows,
                                               گراف_t& sorted_گراف)
{
  بیعلامت space = 0;
  hb_مجموعه_t roots_مقصد_isolate;

  لكل (صحيح i = overflows.درازی - 1; i >= 0; i--)
  {
    ثابت گراف::سرریز_رکورد_t& r = overflows[i];

    بیعلامت ریشه;
    بیعلامت سرریز_space = sorted_گراف.space_for (r.والد, &ریشه);
    إذا (!سرریز_space) استمر;
    إذا (sorted_گراف.رقم_roots_for_space_2 (سرریز_space) <= 1) استمر;

    إذا (!space) {
      space = سرریز_space;
    }

    إذا (space == سرریز_space)
      roots_مقصد_isolate.جمع_کن(ریشه);
  }

  إذا (!roots_مقصد_isolate) أرجع Ok(خطأ);

  بیعلامت بیشینه_مقصد_move = hb_max ((sorted_گراف.رقم_roots_for_space_2 (space) / 2u), 1u);
  إذا (roots_مقصد_isolate.get_population () > بیشینه_مقصد_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    صحيح extra = roots_مقصد_isolate.get_population () - بیشینه_مقصد_move;
    لكل (بیعلامت id : sorted_گراف.ordering_) {
      إذا (!extra) اكسر;
      إذا (roots_مقصد_isolate.has(id)) {
        roots_مقصد_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_گراف.رقم_roots_for_space_2 (space),
             roots_مقصد_isolate.get_population (),
             sorted_گراف.بعدی_space ());
  TRY (sorted_گراف.isolate_subgraph (roots_مقصد_isolate));
  TRY (sorted_گراف.move_مقصد_new_space (roots_مقصد_isolate));

  أرجع Ok(درست);
}

ساكن مضمن
گراف_نتیجه_t<منطقي> resolve_shared_سرریز(ثابت hb_وکتور_t<گراف::سرریز_رکورد_t>& overflows,
                                              صحيح سرریز_شاخص,
                                              گراف_t& sorted_گراف)
{
  ثابت گراف::سرریز_رکورد_t& r = overflows[سرریز_شاخص];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_مجموعه_t parents;
  parents.جمع_کن(r.والد);
  لكل (صحيح i = سرریز_شاخص - 1; i >= 0; i--) {
    ثابت گراف::سرریز_رکورد_t& r2 = overflows[i];
    إذا (r2.فرزند == r.فرزند) {
      parents.جمع_کن(r2.والد);
    }
  }

  خودکار نتیجه = sorted_گراف.duplicate(&parents, r.فرزند);
  إذا (!نتیجه.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    نتیجه = sorted_گراف.duplicate(&parents, r.فرزند);
  }

  إذا (!نتیجه.is_ok ()) أرجع Ok(خطأ);

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
    sorted_گراف.vertices_[*نتیجه].give_max_اولویت();
  }

  أرجع Ok(درست);
}

ساكن مضمن
گراف_نتیجه_t<منطقي> _process_overflows (ثابت hb_وکتور_t<گراف::سرریز_رکورد_t>& overflows,
                                         hb_مجموعه_t& اولویت_bumped_parents,
                                         گراف_t& sorted_گراف)
{
  منطقي resolution_attempted = خطأ;

  // Try resolving the furthest overflows first.
  لكل (صحيح i = overflows.درازی - 1; i >= 0; i--)
  {
    ثابت گراف::سرریز_رکورد_t& r = overflows[i];
    إذا (sorted_گراف.vertices_[r.فرزند].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_تخصیص_بده (منطقي resolved, resolve_shared_سرریز(overflows, i, sorted_گراف));
      إذا (resolved)
        أرجع Ok(درست);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    إذا (sorted_گراف.vertices_[r.فرزند].is_برگ () && !اولویت_bumped_parents.has (r.والد))
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
      TRY_تخصیص_بده (منطقي raised, sorted_گراف.raise_childrens_اولویت (r.والد));
      إذا (raised) {
        اولویت_bumped_parents.جمع_کن (r.والد);
        resolution_attempted = درست;
      }
      استمر;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  أرجع Ok(resolution_attempted);
}

مضمن گراف_نتیجه_t<فراغ>
تخصیص_بده_spaces_and_مرتب_سازی (گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Assigning spaces to 32 bit subgraphs.");
  TRY_تخصیص_بده (منطقي assigned, sorted_گراف.تخصیص_بده_spaces ());
  إذا (assigned)
    أرجع sorted_گراف.مرتب_سازی_کوتاهترین_فاصله ();
  وإلا
    أرجع sorted_گراف.مرتب_سازی_کوتاهترین_فاصله_if_needed ();
}

مضمن گراف_نتیجه_t<فراغ>
_gsub_gpos_specialization (hb_برچسب_t جدول_برچسب_2,
                           منطقي always_recalculate_extensions,
                           گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Applying GSUB/GPOS repacking specializations.");
  إذا (!always_recalculate_extensions) أرجع تخصیص_بده_spaces_and_مرتب_سازی (sorted_گراف);

  خودکار context_res = گراف::gsubgpos_گراف_context_t::create (جدول_برچسب_2, sorted_گراف);

  إذا (unlikely (context_res.is_err())) {
    إذا (context_res.error() == گراف::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      أرجع تخصیص_بده_spaces_and_مرتب_سازی (sorted_گراف);
    وإلا
      // All other errors are propagated.
      أرجع Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  خودکار& ext_context = *context_res;
  DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_گراف.مرتب_سازی_کوتاهترین_فاصله_if_needed ());

  TRY_تخصیص_بده (منطقي will_سرریز, گراف::will_سرریز (sorted_گراف));
  إذا (!will_سرریز) أرجع Ok();
  أرجع تخصیص_بده_spaces_and_مرتب_سازی (sorted_گراف);
}

مضمن گراف_نتیجه_t<فراغ>
hb_resolve_گراف_overflows (hb_برچسب_t جدول_برچسب_2,
                            بیعلامت max_دورها ,
                            منطقي always_recalculate_extensions,
                            گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Repacking %c%c%c%c.", HB_UNTAG(جدول_برچسب_2));
  TRY (sorted_گراف.مرتب_سازی_کوتاهترین_فاصله ());

  TRY_تخصیص_بده (منطقي will_سرریز, گراف::will_سرریز (sorted_گراف));
  إذا (!will_سرریز)
    أرجع Ok();

  منطقي is_gsub_or_gpos = (جدول_برچسب_2 == HB_OT_برچسب_GPOS ||  جدول_برچسب_2 == HB_OT_برچسب_GSUB);
  إذا (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (جدول_برچسب_2, always_recalculate_extensions, sorted_گراف));

  بیعلامت round = 0;
  بیعلامت مجموع_iterations = 0;
  hb_وکتور_t<گراف::سرریز_رکورد_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  طالما (round < max_دورها && مجموع_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_تخصیص_بده (منطقي overflows_exist, گراف::will_سرریز (sorted_گراف, &overflows));
    إذا (!overflows_exist)
      اكسر;

    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_گراف, overflows);

    مجموع_iterations++;
    hb_مجموعه_t اولویت_bumped_parents;

    TRY_تخصیص_بده (منطقي isolated, _try_isolating_subgraphs (overflows, sorted_گراف));
    إذا (!isolated)
    {
      round++;
      TRY_تخصیص_بده (منطقي processed, _process_overflows (overflows, اولویت_bumped_parents, sorted_گراف));
      إذا (!processed)
      {
        DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "No resolution available :(");
        اكسر;
      }
    }

    TRY (sorted_گراف.مرتب_سازی_کوتاهترین_فاصله ());
  }

  TRY_تخصیص_بده (منطقي has_سرریز, گراف::will_سرریز (sorted_گراف));
  إذا (unlikely (has_سرریز))
  {
    إذا (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      أرجع hb_resolve_گراف_overflows (جدول_برچسب_2, max_دورها, درست, sorted_گراف);
    }

    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Offset overflow resolution failed.");
    أرجع Err(گراف::سرریز_RESOLUTION_FAILED);
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
قالب<نام_نوع T>
مضمن hb_blob_t*
hb_resolve_overflows (ثابت T& packed,
                      hb_برچسب_t جدول_برچسب_2,
                      بیعلامت max_دورها = 32,
                      منطقي recalculate_extensions = خطأ) {
  خودکار گراف_res = گراف_t::create (packed);
  إذا (!گراف_res.is_ok ())
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "Graph creation failed, cause: %s", گراف::to_string(گراف_res.error()));
    أرجع اشاره_تهی;
  }
  گراف_t sorted_گراف = std::move (*گراف_res);

  خودکار resolve_res = hb_resolve_گراف_overflows (جدول_برچسب_2, max_دورها, recalculate_extensions, sorted_گراف);
  إذا (resolve_res.is_err ())
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "Overflow resolution failed, cause: %s", گراف::to_string(resolve_res.error()));
    أرجع اشاره_تهی;
  }

  خودکار serialize_res = گراف::serialize (sorted_گراف);
  إذا (serialize_res.is_err ())
  {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "Serialization failed, cause: %s", گراف::to_string(serialize_res.error()));
    أرجع اشاره_تهی;
  }

  أرجع *serialize_res;
}

#پایان_اگر /* HB_REPACKER_HH */
