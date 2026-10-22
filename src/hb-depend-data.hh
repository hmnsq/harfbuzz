/*
 * Copyright © 2024  Adobe, Inc.
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
 * Adobe Author(s): Skef Iterum
 */

#اگر_تعریف_نشده HB_DEPEND_داده_HH
#تعریف_کن HB_DEPEND_داده_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#ضمن "hb.hh"

#ضمن "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#اگر_تعریف_نشده HB_SUBSET_DEPEND_یال_FLAGS_T_DEFINED
#تعریف_کن HB_SUBSET_DEPEND_یال_FLAGS_T_DEFINED
تعريفنوع تعداد {
  HB_SUBSET_DEPEND_یال_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_یال_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_یال_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_یال_flags_t;
#پایان_اگر
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_یال_flags_t);

/* High bit of a context-set element marks it as an index into the sets array
 * rather than a raw glyph ID. */
#تعریف_کن HB_DEPEND_CONTEXT_مجموعه_FLAG 0x80000000u

/**
 * hb_depend_edge_t:
 *
 * Internal structure representing a single dependency edge in the graph.
 * Records that glyph A depends on glyph B through a specific OpenType
 * mechanism (table_tag), with additional metadata:
 *
 * - table_tag: Source table (GSUB, glyf, CFF, COLR, MATH, VARC)
 * - dependent: Target glyph ID
 * - layout_tag: Feature tag (for GSUB), else 0
 * - ligature_set: Index into sets array for ligature components, else INVALID
 * - context_set: Index into sets array for context requirements, else INVALID
 * - flags: Edge flags (FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT)
 */
بنية hb_depend_یال_t {
  hb_depend_یال_t() = حذف;
  hb_depend_یال_t(hb_برچسب_t جدول_برچسب_2,
                   hb_codepoint_t dependent,
                   hb_برچسب_t layout_برچسب,
                   hb_codepoint_t ligature_مجموعه_2,
                   hb_codepoint_t context_مجموعه,
                   hb_subset_depend_یال_flags_t flags = HB_SUBSET_DEPEND_یال_FLAG_NONE) : جدول_برچسب_2(جدول_برچسب_2),
     dependent(dependent), layout_برچسب(layout_برچسب),
     ligature_مجموعه_2(ligature_مجموعه_2), context_مجموعه(context_مجموعه),
     flags(flags)
     {}

  منطقي عملگر == (ثابت hb_depend_یال_t &o) ثابت
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    أرجع جدول_برچسب_2 == o.جدول_برچسب_2 &&
           dependent == o.dependent &&
           layout_برچسب == o.layout_برچسب &&
           ligature_مجموعه_2 == o.ligature_مجموعه_2 &&
           context_مجموعه == o.context_مجموعه;
  }

  uint32_t hash () ثابت
  {
    /* FNV-1a hash of all identity fields (excludes flags) */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (جدول_برچسب_2);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ hb_hash (dependent);
    current = current * 16777619;
    current = current ^ hb_hash (layout_برچسب);
    current = current * 16777619;
    current = current ^ hb_hash (ligature_مجموعه_2);
    current = current * 16777619;
    current = current ^ hb_hash (context_مجموعه);
    current = current * 16777619;
    أرجع current;
  }

  hb_برچسب_t جدول_برچسب_2;
  hb_codepoint_t dependent;
  hb_برچسب_t layout_برچسب;
  hb_codepoint_t ligature_مجموعه_2;
  hb_codepoint_t context_مجموعه;
  hb_subset_depend_یال_flags_t flags;
};

/**
 * hb_depend_edge_key_t:
 *
 * Key structure for edge deduplication. Combines source glyph with edge record.
 * Uses the record's equality and hash operators for comparison.
 */
بنية hb_depend_یال_کلید_t {
  hb_codepoint_t منبع;
  hb_depend_یال_t رکورد;

  hb_depend_یال_کلید_t () : منبع (0), رکورد (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_یال_FLAG_NONE) {}

  hb_depend_یال_کلید_t (hb_codepoint_t منبع,
              hb_برچسب_t جدول_برچسب_2,
              hb_برچسب_t layout_برچسب,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_مجموعه_2,
              hb_codepoint_t context_مجموعه)
    : منبع (منبع),
      رکورد (جدول_برچسب_2, dependent, layout_برچسب, ligature_مجموعه_2, context_مجموعه, HB_SUBSET_DEPEND_یال_FLAG_NONE) {}

  منطقي عملگر == (ثابت hb_depend_یال_کلید_t &o) ثابت
  {
    أرجع منبع == o.منبع && رکورد == o.رکورد;
  }

  uint32_t hash () ثابت
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (منبع);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ رکورد.hash ();
    current = current * 16777619;
    أرجع current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
بنية hb_depend_glyph_رکورد_t {
  hb_وکتور_t<hb_depend_یال_t> dependencies;
};

/**
 * hb_depend_lookup_revmap_t:
 *
 * Internal structure tracking which features are associated with a lookup.
 * Used during GSUB dependency analysis to record the feature tags that
 * activate each lookup.
 */
بنية hb_depend_lookup_revmap_t
{
  hb_depend_lookup_revmap_t () = افتراضي;
  صریح hb_depend_lookup_revmap_t (منطقي پر) : پر (پر) {}
  hb_depend_lookup_revmap_t (ثابت hb_depend_lookup_revmap_t &o) : پر(o.پر),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : پر(o.پر),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& عملگر = (ثابت hb_depend_lookup_revmap_t &o)
  {
    پر = o.پر;
    fv_indexes = o.fv_indexes;
    أرجع *هذا;
  }

  منطقي پر = درست;
  hb_مجموعه_t fv_indexes;
};

/**
 * hb_depend_data_t:
 *
 * Persistent dependency graph data, retained for the lifetime of hb_subset_depend_t.
 * Contains only what is needed at query time:
 * - glyph_dependencies: Per-glyph edge lists
 * - sets: Ligature and context sets indexed by set ID
 *
 * Constructed via hb_depend_data_builder_t; immutable thereafter.
 */
بنية hb_depend_داده_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_وکتور_t<hb::unique_ptr<hb_مجموعه_t>> sets;

  hb_وکتور_t<hb_depend_glyph_رکورد_t> glyph_dependencies;

  ثابت hb_مجموعه_t *get_مجموعه_from_شاخص (hb_codepoint_t شاخص_2)
  {
    إذا (شاخص_2 < sets.درازی)
      أرجع sets[شاخص_2].get ();
    أرجع اشاره_تهی;
  }

  بیعلامت صحيح get_glyph_مدخل_شمار (hb_codepoint_t gid) ثابت
  {
    إذا (gid < glyph_dependencies.درازی)
      أرجع glyph_dependencies[gid].dependencies.درازی;
    أرجع 0;
  }

  منطقي get_glyph_مدخل (hb_codepoint_t gid, بیعلامت صحيح شاخص_2,
                        hb_برچسب_t *جدول_برچسب_2, hb_codepoint_t *dependent,
                        hb_برچسب_t *layout_برچسب, hb_codepoint_t *ligature_مجموعه_2,
                        hb_codepoint_t *context_مجموعه, uint8_t *flags)
  {
    إذا (gid < glyph_dependencies.درازی &&
        شاخص_2 < glyph_dependencies[gid].dependencies.درازی) {
      خودکار &d = glyph_dependencies[gid].dependencies[شاخص_2];
      *جدول_برچسب_2 = d.جدول_برچسب_2;
      *dependent = d.dependent;
      *layout_برچسب = d.layout_برچسب;
      *ligature_مجموعه_2 = d.ligature_مجموعه_2;
      *context_مجموعه = d.context_مجموعه;
      إذا (flags) *flags = d.flags;
      أرجع درست;
    }
    أرجع خطأ;
  }

  فراغ print ()
  {
    لكل (بیعلامت i = 0; i < glyph_dependencies.درازی; i++) {
      خودکار &gd = glyph_dependencies[i];
      إذا (!gd.dependencies.درازی)
        استمر;
      چاپ_کن ("GID %u:\n", i);
      لكل (خودکار &d : gd.dependencies) {
        إذا (d.جدول_برچسب_2 == HB_OT_برچسب_GSUB) {
          چاپ_کن ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_برچسب), d.dependent);
          إذا (d.ligature_مجموعه_2 != HB_CODEPOINT_INVALID)
            چاپ_کن ("  (ligature)");
        } وإلا {
          چاپ_کن ("  %c%c%c%c -> %u", HB_UNTAG(d.جدول_برچسب_2), d.dependent);
        }
        چاپ_کن ("\n");
      }
    }
  }
};

/**
 * hb_depend_data_builder_t:
 *
 * Temporary builder for constructing hb_depend_data_t. Holds all state
 * needed during graph extraction; goes out of scope (and is freed) when
 * construction is complete, leaving only hb_depend_data_t.
 *
 * Temporary state (freed on destruction):
 * - lookup_features: Packed lookup index to feature tag mapping
 * - lookup_feature_offsets: Per-lookup offsets into lookup_features
 * - edge_slots: Compact edge deduplication table
 * - set_to_index: Content-based dependency set deduplication map
 * - free_set_list: Indices of freed sets available for reuse
 * - current_context_set_index: Context requirements for current rule
 * - current_edge_flags: Flags to apply to edges being recorded
 */
بنية hb_depend_داده_builder_t
{
  hb_depend_داده_builder_t (hb_depend_داده_t &داده_2)
    : current_context_مجموعه_شاخص (HB_CODEPOINT_INVALID),
      current_یال_flags (HB_SUBSET_DEPEND_یال_FLAG_NONE),
      داده (داده_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ثابت hb_مجموعه_t *get_مجموعه_from_شاخص (hb_codepoint_t شاخص_2)
  { أرجع داده.get_مجموعه_from_شاخص (شاخص_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  فراغ discard_مجموعه (hb_codepoint_t مجموعه_شاخص)
  {
    إذا (مجموعه_شاخص >= داده.sets.درازی)
    {
      DEBUG_پیام (SUBSET, اشاره_تهی, "Attempting to discard invalid set %u (max is %u)",
                 مجموعه_شاخص, داده.sets.درازی - 1);
      أرجع;
    }
    مجموعه_مقصد_شاخص.del (داده.sets[مجموعه_شاخص].get ());
    داده.sets[مجموعه_شاخص]->clear ();
    check_success (free_مجموعه_list.افزودن_or_fail (مجموعه_شاخص));
  }

  hb_codepoint_t new_مجموعه (ثابت hb_مجموعه_t &مجموعه)
  {
    hb_codepoint_t مجموعه_شاخص;

    إذا (free_مجموعه_list.درازی > 0)
    {
      مجموعه_شاخص = free_مجموعه_list.برداشت ();
      داده.sets[مجموعه_شاخص]->مجموعه (مجموعه);
      إذا (unlikely (داده.sets[مجموعه_شاخص]->درون_error ()))
	أرجع fail_invalid ();
    }
    وإلا
    {
      مجموعه_شاخص = داده.sets.درازی;
      hb_مجموعه_t *new_مجموعه = hb_مجموعه_create ();
      إذا (unlikely (!new_مجموعه))
	أرجع fail_invalid ();
      new_مجموعه->مجموعه (مجموعه);
      إذا (unlikely (new_مجموعه->درون_error ()))
      {
	hb_مجموعه_destroy (new_مجموعه);
	أرجع fail_invalid ();
      }
      إذا (unlikely (!داده.sets.افزودن_or_fail (hb::unique_ptr<hb_مجموعه_t> {new_مجموعه})))
	أرجع fail_invalid ();
    }

    أرجع مجموعه_شاخص;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t پیدا_کن_or_create_مجموعه (ثابت hb_مجموعه_t &مجموعه, منطقي *created = اشاره_تهی)
  {
    hb_codepoint_t *existing_idx = اشاره_تهی;
    إذا (مجموعه_مقصد_شاخص.has (&مجموعه, &existing_idx))
    {
      إذا (created) *created = خطأ;
      أرجع *existing_idx;
    }

    hb_codepoint_t new_idx = new_مجموعه (مجموعه);
    إذا (unlikely (new_idx == HB_CODEPOINT_INVALID))
      أرجع HB_CODEPOINT_INVALID;

    إذا (unlikely (!مجموعه_مقصد_شاخص.مجموعه (داده.sets[new_idx].get (), new_idx)))
      أرجع fail_invalid ();
    إذا (created) *created = درست;
    أرجع new_idx;
  }

  hb_codepoint_t پیدا_کن_or_create_context_مجموعه (ثابت hb_مجموعه_t &مجموعه)
  { أرجع پیدا_کن_or_create_مجموعه (مجموعه); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_مجموعه (ثابت hb_وکتور_t<hb_مجموعه_t> *backtrack_sets,
                                     ثابت hb_وکتور_t<hb_مجموعه_t> *lookahead_sets)
  {
    إذا ((!backtrack_sets || backtrack_sets->درازی == 0) &&
        (!lookahead_sets || lookahead_sets->درازی == 0))
      أرجع HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_مجموعه_t مستقیم_requirements;

    إذا (backtrack_sets)
    {
      لكل (ثابت خودکار &پشت_مجموعه : *backtrack_sets)
      {
        إذا (پشت_مجموعه.get_population () == 1)
          مستقیم_requirements.جمع_کن (پشت_مجموعه.get_min ());
      }
    }

    إذا (lookahead_sets)
    {
      لكل (ثابت خودکار &look_مجموعه : *lookahead_sets)
      {
        إذا (look_مجموعه.get_population () == 1)
          مستقیم_requirements.جمع_کن (look_مجموعه.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_مجموعه_t context_elements;

    إذا (backtrack_sets)
    {
      لكل (ثابت خودکار &پشت_مجموعه : *backtrack_sets)
      {
        إذا (پشت_مجموعه.get_population () > 1)
        {
          hb_مجموعه_t filtered_مجموعه;
          filtered_مجموعه.مجموعه (پشت_مجموعه);
          filtered_مجموعه.تفریق (مستقیم_requirements);
          إذا (unlikely (filtered_مجموعه.درون_error ()))
            أرجع fail_invalid ();

          إذا (!filtered_مجموعه.is_empty ())
          {
            hb_codepoint_t مجموعه_idx = پیدا_کن_or_create_context_مجموعه (filtered_مجموعه);
            إذا (unlikely (مجموعه_idx == HB_CODEPOINT_INVALID))
              أرجع HB_CODEPOINT_INVALID;
            context_elements.جمع_کن (HB_DEPEND_CONTEXT_مجموعه_FLAG | مجموعه_idx);
          }
        }
      }
    }

    إذا (lookahead_sets)
    {
      لكل (ثابت خودکار &look_مجموعه : *lookahead_sets)
      {
        إذا (look_مجموعه.get_population () > 1)
        {
          hb_مجموعه_t filtered_مجموعه;
          filtered_مجموعه.مجموعه (look_مجموعه);
          filtered_مجموعه.تفریق (مستقیم_requirements);
          إذا (unlikely (filtered_مجموعه.درون_error ()))
            أرجع fail_invalid ();

          إذا (!filtered_مجموعه.is_empty ())
          {
            hb_codepoint_t مجموعه_idx = پیدا_کن_or_create_context_مجموعه (filtered_مجموعه);
            إذا (unlikely (مجموعه_idx == HB_CODEPOINT_INVALID))
              أرجع HB_CODEPOINT_INVALID;
            context_elements.جمع_کن (HB_DEPEND_CONTEXT_مجموعه_FLAG | مجموعه_idx);
          }
        }
      }
    }

    context_elements.union_ (مستقیم_requirements);
    إذا (unlikely (مستقیم_requirements.درون_error () ||
                  context_elements.درون_error ()))
      أرجع fail_invalid ();

    إذا (context_elements.is_empty ())
      أرجع HB_CODEPOINT_INVALID;

    أرجع پیدا_کن_or_create_context_مجموعه (context_elements);
  }

  ساكن uint64_t encode_یال_ref (hb_codepoint_t منبع, بیعلامت صحيح شاخص_2)
  { أرجع (uint64_t (منبع) + 1) << 32 | شاخص_2; }

  ساكن فراغ decode_یال_ref (uint64_t ref,
			       hb_codepoint_t *منبع,
			       بیعلامت صحيح *شاخص_2)
  {
    *منبع = (ref >> 32) - 1;
    *شاخص_2 = ref;
  }

  uint32_t یال_hash (hb_codepoint_t منبع, ثابت hb_depend_یال_t &رکورد) ثابت
  {
    hb_depend_یال_کلید_t key (منبع, رکورد.جدول_برچسب_2, رکورد.layout_برچسب,
			      رکورد.dependent, رکورد.ligature_مجموعه_2,
			      رکورد.context_مجموعه);
    أرجع key.hash ();
  }

  منطقي resize_یال_slots ()
  {
    بیعلامت صحيح new_اندازه = یال_slots.درازی ? یال_slots.درازی * 2 : 8;
    إذا (unlikely (new_اندازه < یال_slots.درازی))
      أرجع fail ();

    hb_وکتور_t<uint64_t> new_slots;
    إذا (unlikely (!new_slots.resize_exact (new_اندازه)))
      أرجع fail ();

    لكل (uint64_t ref : یال_slots)
    {
      إذا (!ref)
	استمر;

      hb_codepoint_t منبع;
      بیعلامت صحيح شاخص_2;
      decode_یال_ref (ref, &منبع, &شاخص_2);
      ثابت hb_depend_یال_t &رکورد = داده.glyph_dependencies[منبع].dependencies[شاخص_2];
      بیعلامت صحيح جایگاه = یال_hash (منبع, رکورد) & (new_اندازه - 1);
      بیعلامت صحيح گام = 0;
      طالما (new_slots[جایگاه])
	جایگاه = (جایگاه + ++گام) & (new_اندازه - 1);
      new_slots[جایگاه] = ref;
    }

    یال_slots = std::move (new_slots);
    أرجع درست;
  }

  منطقي جمع_کن_یال (hb_codepoint_t منبع, ثابت hb_depend_یال_t &رکورد)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    إذا (unlikely (!یال_slots.درازی ||
		  uint64_t (یال_population) * 3 / 2 >= یال_slots.درازی - 1))
      إذا (unlikely (!resize_یال_slots ()))
	أرجع خطأ;

    بیعلامت صحيح جایگاه = یال_hash (منبع, رکورد) & (یال_slots.درازی - 1);
    بیعلامت صحيح گام = 0;
    طالما (uint64_t ref = یال_slots[جایگاه])
    {
      hb_codepoint_t existing_منبع;
      بیعلامت صحيح existing_شاخص;
      decode_یال_ref (ref, &existing_منبع, &existing_شاخص);
      إذا (existing_منبع == منبع &&
	  داده.glyph_dependencies[منبع].dependencies[existing_شاخص] == رکورد)
	أرجع خطأ;
      جایگاه = (جایگاه + ++گام) & (یال_slots.درازی - 1);
    }

    خودکار &dependencies = داده.glyph_dependencies[منبع].dependencies;
    بیعلامت صحيح شاخص_2 = dependencies.درازی;
    إذا (unlikely (!dependencies.افزودن_or_fail (رکورد)))
      أرجع fail ();

    یال_slots[جایگاه] = encode_یال_ref (منبع, شاخص_2);
    یال_population++;
    أرجع درست;
  }

  منطقي جمع_کن_depend_layout (hb_codepoint_t target, hb_برچسب_t جدول_برچسب_2,
                          hb_برچسب_t layout_برچسب,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_مجموعه = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_مجموعه = HB_CODEPOINT_INVALID,
                          hb_subset_depend_یال_flags_t flags = HB_SUBSET_DEPEND_یال_FLAG_NONE)
  {
    إذا (target >= داده.glyph_dependencies.درازی) {
      DEBUG_پیام (SUBSET, اشاره_تهی, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(جدول_برچسب_2));
      أرجع خطأ;
    }

    أرجع جمع_کن_یال (target, hb_depend_یال_t (جدول_برچسب_2, dependent, layout_برچسب,
					      lig_مجموعه, context_مجموعه, flags));
  }

  منطقي جمع_کن_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_شاخص,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_مجموعه = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_مجموعه = HB_CODEPOINT_INVALID)
  {
    إذا (context_مجموعه == HB_CODEPOINT_INVALID)
      context_مجموعه = current_context_مجموعه_شاخص;
    hb_subset_depend_یال_flags_t flags = current_یال_flags;

    منطقي any_added = خطأ;
    لكل (uint64_t مدخل : get_lookup_features (lookup_شاخص)) {
      hb_برچسب_t t = (hb_برچسب_t) مدخل;
      إذا (جمع_کن_depend_layout (target, HB_OT_برچسب_GSUB, t, dependent, lig_مجموعه, context_مجموعه, flags))
        any_added = درست;
    }
    أرجع any_added;
  }

  منطقي init_lookup_features (بیعلامت lookup_شمار)
  {
    أرجع check_success (lookup_feature_offsets.resize (lookup_شمار + 1));
  }

  منطقي جمع_کن_lookup_feature (hb_codepoint_t lookup_شاخص, hb_برچسب_t feature_برچسب)
  {
    إذا (feature_برچسب == HB_مجموعه_مقدار_INVALID)
      أرجع درست;

    إذا (unlikely (!lookup_feature_offsets.درازی ||
		  lookup_شاخص >= lookup_feature_offsets.درازی - 1))
      أرجع fail ();

    uint64_t مدخل = ((uint64_t) lookup_شاخص << 32) | feature_برچسب;
    أرجع check_success (lookup_features.افزودن_or_fail (مدخل));
  }

  منطقي پایان_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      أرجع a < b ? -1 : a > b ? 1 : 0;
    });

    بیعلامت اكتب = 0;
    لكل (uint64_t مدخل : lookup_features)
      إذا (!اكتب || مدخل != lookup_features[اكتب - 1])
	lookup_features[اكتب++] = مدخل;
    lookup_features.shrink (اكتب, خطأ);

    بیعلامت feature_شاخص = 0;
    بیعلامت lookup_شمار = lookup_feature_offsets.درازی - 1;
    لكل (بیعلامت lookup_شاخص = 0; lookup_شاخص < lookup_شمار; lookup_شاخص++)
    {
      lookup_feature_offsets[lookup_شاخص] = feature_شاخص;
      طالما (feature_شاخص < lookup_features.درازی &&
	     lookup_features[feature_شاخص] >> 32 == lookup_شاخص)
	feature_شاخص++;
    }
    lookup_feature_offsets[lookup_شمار] = feature_شاخص;
    أرجع درست;
  }

  hb_array_t<ثابت uint64_t> get_lookup_features (hb_codepoint_t lookup_شاخص) ثابت
  {
    إذا (unlikely (!lookup_feature_offsets.درازی ||
		  lookup_شاخص >= lookup_feature_offsets.درازی - 1))
      أرجع {};

    بیعلامت شروع = lookup_feature_offsets[lookup_شاخص];
    بیعلامت انتها = lookup_feature_offsets[lookup_شاخص + 1];
    أرجع lookup_features.as_array ().sub_array (شروع, انتها - شروع);
  }

  فراغ جمع_کن_depend (hb_codepoint_t target, hb_برچسب_t جدول_برچسب_2,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_مجموعه = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_مجموعه = HB_CODEPOINT_INVALID,
                   hb_subset_depend_یال_flags_t flags = HB_SUBSET_DEPEND_یال_FLAG_NONE)
  {
    جمع_کن_depend_layout (target, جدول_برچسب_2, HB_CODEPOINT_INVALID, dependent, lig_مجموعه,
                       context_مجموعه, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { أرجع hb_map_get (&nominal_glyphs, cp); }

  HB_داخلی منطقي compile (hb_face_t *face);

  منطقي fail () { successful = خطأ; أرجع خطأ; }
  hb_codepoint_t fail_invalid () { fail (); أرجع HB_CODEPOINT_INVALID; }
  منطقي check_success (منطقي s) { successful = (successful && s); أرجع successful; }

  HB_داخلی فراغ get_gsub_dependencies (hb_face_t *face);

  منطقي successful = درست;
  hb_مجموعه_t unicodes;
  hb_map_t nominal_glyphs;
  hb_وکتور_t<uint64_t> lookup_features;
  hb_وکتور_t<بیعلامت> lookup_feature_offsets;
  hb_وکتور_t<uint64_t> یال_slots;
  بیعلامت صحيح یال_population = 0;
  hb_hashmap_t<ثابت hb_مجموعه_t*, hb_codepoint_t> مجموعه_مقصد_شاخص;
  hb_وکتور_t<hb_codepoint_t> free_مجموعه_list;
  hb_codepoint_t current_context_مجموعه_شاخص;
  hb_subset_depend_یال_flags_t current_یال_flags;

  hb_depend_داده_t &داده;
};


#پایان_اگر /* HB_DEPEND_DATA_HH */
