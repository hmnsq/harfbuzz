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

#اگر_غیر_تعریف_شدہ HB_REPACKER_HH
#تعریف_کرو HB_REPACKER_HH

#شامل "hb-open-type.hh"
#شامل "hb-map.hh"
#شامل "hb-vector.hh"
#شامل "graph/graph.hh"
#شامل "graph/gsubgpos-graph.hh"
#شامل "graph/serialize.hh"

استعمال گراف::گراف_t;
استعمال گراف::گراف_نتیجہ_t;

/*
 * For a detailed writeup on the overflow resolution algorithm see:
 * docs/repacker.md
 */

ساخت lookup_سائز_t
{
  غیرعلامتی lookup_اشاریہ;
  size_t سائز;
  غیرعلامتی عدد_subtables;

  ساکن صحیح cmp (ثابت خالی* a, ثابت خالی* b)
  {
    واپس cmp ((ثابت lookup_سائز_t*) a,
                (ثابت lookup_سائز_t*) b);
  }

  ساکن صحیح cmp (ثابت lookup_سائز_t* a, ثابت lookup_سائز_t* b)
  {
    دوہرا subtables_فی_byte_a = (دوہرا) a->عدد_subtables / (دوہرا) a->سائز;
    دوہرا subtables_فی_byte_b = (دوہرا) b->عدد_subtables / (دوہرا) b->سائز;
    اگر (subtables_فی_byte_a == subtables_فی_byte_b) {
      واپس b->lookup_اشاریہ - a->lookup_اشاریہ;
    }

    دوہرا cmp = subtables_فی_byte_b - subtables_فی_byte_a;
    اگر (cmp < 0) واپس -1;
    اگر (cmp > 0) واپس 1;
    واپس 0;
  }
};

ساکن اندرونی
گراف_نتیجہ_t<خالی> _presplit_subtables_if_needed (گراف::gsubgpos_گراف_context_t& ext_context)
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
  برائے (غیرعلامتی lookup_اشاریہ : lookup_indices)
  {
    گراف::Lookup* lookup = ext_context.lookups.get(lookup_اشاریہ);
    TRY (lookup->بانٹو_subtables_if_needed (ext_context, lookup_اشاریہ));
  }

  واپس Ok();
}

/*
 * Analyze the lookups in a GSUB/GPOS table and decide if any should be promoted
 * to extension lookups.
 */
ساکن اندرونی
گراف_نتیجہ_t<خالی> _promote_extensions_if_needed (گراف::gsubgpos_گراف_context_t& ext_context)
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
  اگر (!ext_context.lookups) واپس Ok();

  غیرعلامتی کل_lookup_جدول_sizes = 0;
  hb_سمتیہ_t<lookup_سائز_t> lookup_sizes;
  lookup_sizes.alloc (ext_context.lookups.get_population (), سچ);

  برائے (غیرعلامتی lookup_اشاریہ : ext_context.lookups.کلیدیں ())
  {
    ثابت خودکار& lookup_v = ext_context.گراف.vertices_[lookup_اشاریہ];
    کل_lookup_جدول_sizes += lookup_v.جدول_سائز ();

    ثابت گراف::Lookup* lookup = ext_context.lookups.get(lookup_اشاریہ);
    hb_سیٹ_t دیکھا;
    lookup_sizes.push (lookup_سائز_t {
        lookup_اشاریہ,
        ext_context.گراف.ڈھونڈو_subgraph_سائز (lookup_اشاریہ, دیکھا).قدر_or (0),
        lookup->نمبر_کا_subtables (),
      });
  }

  lookup_sizes.qsort ();

  size_t lookup_list_سائز = ext_context.گراف.vertices_[ext_context.lookup_list_اشاریہ].جدول_سائز ();
  size_t l2_l3_سائز = lookup_list_سائز + کل_lookup_جدول_sizes; // Lookup List + Lookups
  size_t l3_l4_سائز = کل_lookup_جدول_sizes; // Lookups + SubTables
  size_t l4_plus_سائز = 0; // SubTables + their descendants

  // Start by assuming all lookups are using extension subtables, this size will be removed later
  // if it's decided to not make a lookup extension.
  برائے (خودکار p : lookup_sizes)
  {
    // TODO(garretrieger): this overestimates the extension subtables size because some extension subtables may be
    //                     reused. However, we can't correct this until we have connected component analysis in place.
    غیرعلامتی subtables_سائز = p.عدد_subtables * 8;
    l3_l4_سائز += subtables_سائز;
    l4_plus_سائز += subtables_سائز;
  }

  منطقی layers_بھرا = جھوٹ;
  برائے (خودکار p : lookup_sizes)
  {
    ثابت گراف::Lookup* lookup = ext_context.lookups.get(p.lookup_اشاریہ);
    اگر (lookup->is_extension (ext_context.جدول_لیبل_2))
      // already an extension so size is counted by the loop above.
      جاری;

    اگر (!layers_بھرا)
    {
      size_t lookup_سائز = ext_context.گراف.vertices_[p.lookup_اشاریہ].جدول_سائز ();
      hb_سیٹ_t دیکھا;
      size_t subtables_سائز = ext_context.گراف.ڈھونڈو_subgraph_سائز (p.lookup_اشاریہ, دیکھا, 1).قدر_or (0) - lookup_سائز;
      size_t بقایا_سائز = p.سائز - subtables_سائز - lookup_سائز;

      l3_l4_سائز   += subtables_سائز;
      l3_l4_سائز   -= p.عدد_subtables * 8;
      l4_plus_سائز += subtables_سائز + بقایا_سائز;

      اگر (l2_l3_سائز < (1 << 16)
          && l3_l4_سائز < (1 << 16)
          && l4_plus_سائز < (1 << 16)) جاری; // this lookup fits within all layers groups

      layers_بھرا = سچ;
    }

    TRY (ext_context.lookups.get(p.lookup_اشاریہ)->make_extension (ext_context, p.lookup_اشاریہ));
  }

  واپس Ok();
}

ساکن اندرونی
گراف_نتیجہ_t<منطقی> _try_isolating_subgraphs (ثابت hb_سمتیہ_t<گراف::حد_سے_تجاوز_ریکارڈ_t>& overflows,
                                               گراف_t& sorted_گراف)
{
  غیرعلامتی space = 0;
  hb_سیٹ_t roots_مقصد_isolate;

  برائے (صحیح i = overflows.لمبائی - 1; i >= 0; i--)
  {
    ثابت گراف::حد_سے_تجاوز_ریکارڈ_t& r = overflows[i];

    غیرعلامتی جڑ;
    غیرعلامتی حد_سے_تجاوز_space = sorted_گراف.space_for (r.والد, &جڑ);
    اگر (!حد_سے_تجاوز_space) جاری;
    اگر (sorted_گراف.عدد_roots_for_space_2 (حد_سے_تجاوز_space) <= 1) جاری;

    اگر (!space) {
      space = حد_سے_تجاوز_space;
    }

    اگر (space == حد_سے_تجاوز_space)
      roots_مقصد_isolate.جوڑو(جڑ);
  }

  اگر (!roots_مقصد_isolate) واپس Ok(جھوٹ);

  غیرعلامتی زیادہ_سے_زیادہ_مقصد_move = hb_max ((sorted_گراف.عدد_roots_for_space_2 (space) / 2u), 1u);
  اگر (roots_مقصد_isolate.get_population () > زیادہ_سے_زیادہ_مقصد_move) {
    // Only move at most half of the roots in a space at a time.
    //
    // Note: this was ported from non-stable ids to stable ids. So to retain the same behaviour
    // with regards to which roots are removed from the set we need to remove them in the topological
    // order, not the object id order.
    صحیح extra = roots_مقصد_isolate.get_population () - زیادہ_سے_زیادہ_مقصد_move;
    برائے (غیرعلامتی id : sorted_گراف.ordering_) {
      اگر (!extra) توڑو;
      اگر (roots_مقصد_isolate.has(id)) {
        roots_مقصد_isolate.del(id);
        extra--;
      }
    }
  }

  DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
             "Overflow in space %u (%u roots). Moving %u roots to space %u.",
             space,
             sorted_گراف.عدد_roots_for_space_2 (space),
             roots_مقصد_isolate.get_population (),
             sorted_گراف.اگلا_space ());
  TRY (sorted_گراف.isolate_subgraph (roots_مقصد_isolate));
  TRY (sorted_گراف.move_مقصد_new_space (roots_مقصد_isolate));

  واپس Ok(سچ);
}

ساکن اندرونی
گراف_نتیجہ_t<منطقی> resolve_shared_حد_سے_تجاوز(ثابت hb_سمتیہ_t<گراف::حد_سے_تجاوز_ریکارڈ_t>& overflows,
                                              صحیح حد_سے_تجاوز_اشاریہ,
                                              گراف_t& sorted_گراف)
{
  ثابت گراف::حد_سے_تجاوز_ریکارڈ_t& r = overflows[حد_سے_تجاوز_اشاریہ];

  // Find all of the parents in overflowing links that link to this
  // same child node. We will then try duplicating the child node and
  // re-assigning all of these parents to the duplicate.
  hb_سیٹ_t parents;
  parents.جوڑو(r.والد);
  برائے (صحیح i = حد_سے_تجاوز_اشاریہ - 1; i >= 0; i--) {
    ثابت گراف::حد_سے_تجاوز_ریکارڈ_t& r2 = overflows[i];
    اگر (r2.بچہ == r.بچہ) {
      parents.جوڑو(r2.والد);
    }
  }

  خودکار نتیجہ = sorted_گراف.duplicate(&parents, r.بچہ);
  اگر (!نتیجہ.is_ok () && parents.get_population() > 2) {
    // All links to the child are overflowing, so we can't include all
    // in the duplication. Remove one parent from the duplication.
    // Remove the lowest index parent, which will be the closest to the child.
    parents.del(parents.get_min());
    نتیجہ = sorted_گراف.duplicate(&parents, r.بچہ);
  }

  اگر (!نتیجہ.is_ok ()) واپس Ok(جھوٹ);

  اگر (parents.get_population() > 1) {
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

  واپس Ok(سچ);
}

ساکن اندرونی
گراف_نتیجہ_t<منطقی> _process_overflows (ثابت hb_سمتیہ_t<گراف::حد_سے_تجاوز_ریکارڈ_t>& overflows,
                                         hb_سیٹ_t& ترجیح_bumped_parents,
                                         گراف_t& sorted_گراف)
{
  منطقی resolution_attempted = جھوٹ;

  // Try resolving the furthest overflows first.
  برائے (صحیح i = overflows.لمبائی - 1; i >= 0; i--)
  {
    ثابت گراف::حد_سے_تجاوز_ریکارڈ_t& r = overflows[i];
    اگر (sorted_گراف.vertices_[r.بچہ].is_shared ())
    {
      // The child object is shared, we may be able to eliminate the overflow
      // by duplicating it.
      TRY_تفویض (منطقی resolved, resolve_shared_حد_سے_تجاوز(overflows, i, sorted_گراف));
      اگر (resolved)
        واپس Ok(سچ);

      // Sometimes we can't duplicate a node which looks shared because it's not actually shared
      // (eg. all links from the same parent) in this case continue on to other resolution options.
    }

    اگر (sorted_گراف.vertices_[r.بچہ].is_برگ () && !ترجیح_bumped_parents.has (r.والد))
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
      TRY_تفویض (منطقی raised, sorted_گراف.raise_childrens_ترجیح (r.والد));
      اگر (raised) {
        ترجیح_bumped_parents.جوڑو (r.والد);
        resolution_attempted = سچ;
      }
      جاری;
    }

    // TODO(garretrieger): add additional offset resolution strategies
    // - Promotion to extension lookups.
    // - Table splitting.
  }

  واپس Ok(resolution_attempted);
}

اندرونی گراف_نتیجہ_t<خالی>
تفویض_spaces_and_ترتیب_دو (گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Assigning spaces to 32 bit subgraphs.");
  TRY_تفویض (منطقی assigned, sorted_گراف.تفویض_spaces ());
  اگر (assigned)
    واپس sorted_گراف.ترتیب_دو_مختصر_ترین_فاصلہ ();
  ورنہ
    واپس sorted_گراف.ترتیب_دو_مختصر_ترین_فاصلہ_if_needed ();
}

اندرونی گراف_نتیجہ_t<خالی>
_gsub_gpos_specialization (hb_لیبل_t جدول_لیبل_2,
                           منطقی always_recalculate_extensions,
                           گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Applying GSUB/GPOS repacking specializations.");
  اگر (!always_recalculate_extensions) واپس تفویض_spaces_and_ترتیب_دو (sorted_گراف);

  خودکار context_res = گراف::gsubgpos_گراف_context_t::create (جدول_لیبل_2, sorted_گراف);

  اگر (unlikely (context_res.is_err())) {
    اگر (context_res.error() == گراف::SANITIZE_FAILURE)
      // Sanitize failures are ignored, and we just skip splitting/extension promotion which needs a valid
      // GSUB/GPOS
      واپس تفویض_spaces_and_ترتیب_دو (sorted_گراف);
    ورنہ
      // All other errors are propagated.
      واپس Err(context_res.error());
  }

  // Otherwise we have a valid GSUB/GPOS table and can try extension promotion and splitting
  خودکار& ext_context = *context_res;
  DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Splitting subtables if needed.");
  TRY (_presplit_subtables_if_needed (ext_context));

  DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Promoting lookups to extensions if needed.");
  TRY (_promote_extensions_if_needed (ext_context));

  // an additional sorting is needed before assign_spaces () which requires
  // correct topological ordering to find space roots
  TRY (sorted_گراف.ترتیب_دو_مختصر_ترین_فاصلہ_if_needed ());

  TRY_تفویض (منطقی will_حد_سے_تجاوز, گراف::will_حد_سے_تجاوز (sorted_گراف));
  اگر (!will_حد_سے_تجاوز) واپس Ok();
  واپس تفویض_spaces_and_ترتیب_دو (sorted_گراف);
}

اندرونی گراف_نتیجہ_t<خالی>
hb_resolve_گراف_overflows (hb_لیبل_t جدول_لیبل_2,
                            غیرعلامتی max_دور ,
                            منطقی always_recalculate_extensions,
                            گراف_t& sorted_گراف /* IN/OUT */)
{
  DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Repacking %c%c%c%c.", HB_UNTAG(جدول_لیبل_2));
  TRY (sorted_گراف.ترتیب_دو_مختصر_ترین_فاصلہ ());

  TRY_تفویض (منطقی will_حد_سے_تجاوز, گراف::will_حد_سے_تجاوز (sorted_گراف));
  اگر (!will_حد_سے_تجاوز)
    واپس Ok();

  منطقی is_gsub_or_gpos = (جدول_لیبل_2 == HB_OT_لیبل_GPOS ||  جدول_لیبل_2 == HB_OT_لیبل_GSUB);
  اگر (is_gsub_or_gpos)
    TRY(_gsub_gpos_specialization (جدول_لیبل_2, always_recalculate_extensions, sorted_گراف));

  غیرعلامتی round = 0;
  غیرعلامتی کل_iterations = 0;
  hb_سمتیہ_t<گراف::حد_سے_تجاوز_ریکارڈ_t> overflows;
  // TODO(garretrieger): select a good limit for max rounds.
  جب (round < max_دور && کل_iterations < HB_REPACKER_MAX_ITERATIONS) {
    TRY_تفویض (منطقی overflows_exist, گراف::will_حد_سے_تجاوز (sorted_گراف, &overflows));
    اگر (!overflows_exist)
      توڑو;

    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "=== Overflow resolution round %u ===", round);
    print_overflows (sorted_گراف, overflows);

    کل_iterations++;
    hb_سیٹ_t ترجیح_bumped_parents;

    TRY_تفویض (منطقی isolated, _try_isolating_subgraphs (overflows, sorted_گراف));
    اگر (!isolated)
    {
      round++;
      TRY_تفویض (منطقی processed, _process_overflows (overflows, ترجیح_bumped_parents, sorted_گراف));
      اگر (!processed)
      {
        DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "No resolution available :(");
        توڑو;
      }
    }

    TRY (sorted_گراف.ترتیب_دو_مختصر_ترین_فاصلہ ());
  }

  TRY_تفویض (منطقی has_حد_سے_تجاوز, گراف::will_حد_سے_تجاوز (sorted_گراف));
  اگر (unlikely (has_حد_سے_تجاوز))
  {
    اگر (is_gsub_or_gpos && !always_recalculate_extensions) {
      // If this a GSUB/GPOS table and we didn't try to extension promotion and table splitting then
      // as a last ditch effort, re-run the repacker with it enabled.
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Failed to find a resolution. Re-running with extension promotion and table splitting enabled.");
      واپس hb_resolve_گراف_overflows (جدول_لیبل_2, max_دور, سچ, sorted_گراف);
    }

    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Offset overflow resolution failed.");
    واپس Err(گراف::حد_سے_تجاوز_RESOLUTION_FAILED);
  }

  واپس Ok();
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
سانچہ<قسم_نام T>
اندرونی hb_blob_t*
hb_resolve_overflows (ثابت T& packed,
                      hb_لیبل_t جدول_لیبل_2,
                      غیرعلامتی max_دور = 32,
                      منطقی recalculate_extensions = جھوٹ) {
  خودکار گراف_res = گراف_t::create (packed);
  اگر (!گراف_res.is_ok ())
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "Graph creation failed, cause: %s", گراف::to_string(گراف_res.error()));
    واپس خالی_اشاریہ;
  }
  گراف_t sorted_گراف = std::move (*گراف_res);

  خودکار resolve_res = hb_resolve_گراف_overflows (جدول_لیبل_2, max_دور, recalculate_extensions, sorted_گراف);
  اگر (resolve_res.is_err ())
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "Overflow resolution failed, cause: %s", گراف::to_string(resolve_res.error()));
    واپس خالی_اشاریہ;
  }

  خودکار serialize_res = گراف::serialize (sorted_گراف);
  اگر (serialize_res.is_err ())
  {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "Serialization failed, cause: %s", گراف::to_string(serialize_res.error()));
    واپس خالی_اشاریہ;
  }

  واپس *serialize_res;
}

#اگر_ختم /* HB_REPACKER_HH */
