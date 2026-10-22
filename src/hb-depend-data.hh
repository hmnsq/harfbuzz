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

#که_نه_تعریف_شوی HB_DEPEND_معلومات_HH
#تعریف_کړه HB_DEPEND_معلومات_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#ضمن "hb.hh"

#ضمن "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#که_نه_تعریف_شوی HB_SUBSET_DEPEND_څنډه_FLAGS_T_DEFINED
#تعریف_کړه HB_SUBSET_DEPEND_څنډه_FLAGS_T_DEFINED
تعريفنوع تعداد {
  HB_SUBSET_DEPEND_څنډه_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_څنډه_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_څنډه_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_څنډه_flags_t;
#د_که_پای
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_څنډه_flags_t);

/* High bit of a context-set element marks it as an index into the sets array
 * rather than a raw glyph ID. */
#تعریف_کړه HB_DEPEND_CONTEXT_سېټ_FLAG 0x80000000u

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
بنية hb_depend_څنډه_t {
  hb_depend_څنډه_t() = ړنګول;
  hb_depend_څنډه_t(hb_نښان_t جدول_نښان_2,
                   hb_codepoint_t dependent,
                   hb_نښان_t layout_نښان,
                   hb_codepoint_t ligature_سېټ_2,
                   hb_codepoint_t context_سېټ,
                   hb_subset_depend_څنډه_flags_t flags = HB_SUBSET_DEPEND_څنډه_FLAG_NONE) : جدول_نښان_2(جدول_نښان_2),
     dependent(dependent), layout_نښان(layout_نښان),
     ligature_سېټ_2(ligature_سېټ_2), context_سېټ(context_سېټ),
     flags(flags)
     {}

  منطقي چلوونکی == (ثابت hb_depend_څنډه_t &o) ثابت
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    أرجع جدول_نښان_2 == o.جدول_نښان_2 &&
           dependent == o.dependent &&
           layout_نښان == o.layout_نښان &&
           ligature_سېټ_2 == o.ligature_سېټ_2 &&
           context_سېټ == o.context_سېټ;
  }

  uint32_t hash () ثابت
  {
    /* FNV-1a hash of all identity fields (excludes flags) */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (جدول_نښان_2);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ hb_hash (dependent);
    current = current * 16777619;
    current = current ^ hb_hash (layout_نښان);
    current = current * 16777619;
    current = current ^ hb_hash (ligature_سېټ_2);
    current = current * 16777619;
    current = current ^ hb_hash (context_سېټ);
    current = current * 16777619;
    أرجع current;
  }

  hb_نښان_t جدول_نښان_2;
  hb_codepoint_t dependent;
  hb_نښان_t layout_نښان;
  hb_codepoint_t ligature_سېټ_2;
  hb_codepoint_t context_سېټ;
  hb_subset_depend_څنډه_flags_t flags;
};

/**
 * hb_depend_edge_key_t:
 *
 * Key structure for edge deduplication. Combines source glyph with edge record.
 * Uses the record's equality and hash operators for comparison.
 */
بنية hb_depend_څنډه_کلی_t {
  hb_codepoint_t سرچینه;
  hb_depend_څنډه_t ریکارډ;

  hb_depend_څنډه_کلی_t () : سرچینه (0), ریکارډ (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_څنډه_FLAG_NONE) {}

  hb_depend_څنډه_کلی_t (hb_codepoint_t سرچینه,
              hb_نښان_t جدول_نښان_2,
              hb_نښان_t layout_نښان,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_سېټ_2,
              hb_codepoint_t context_سېټ)
    : سرچینه (سرچینه),
      ریکارډ (جدول_نښان_2, dependent, layout_نښان, ligature_سېټ_2, context_سېټ, HB_SUBSET_DEPEND_څنډه_FLAG_NONE) {}

  منطقي چلوونکی == (ثابت hb_depend_څنډه_کلی_t &o) ثابت
  {
    أرجع سرچینه == o.سرچینه && ریکارډ == o.ریکارډ;
  }

  uint32_t hash () ثابت
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (سرچینه);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ ریکارډ.hash ();
    current = current * 16777619;
    أرجع current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
بنية hb_depend_glyph_ریکارډ_t {
  hb_ویکټور_t<hb_depend_څنډه_t> dependencies;
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
  څرګند hb_depend_lookup_revmap_t (منطقي ډک) : ډک (ډک) {}
  hb_depend_lookup_revmap_t (ثابت hb_depend_lookup_revmap_t &o) : ډک(o.ډک),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : ډک(o.ډک),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& چلوونکی = (ثابت hb_depend_lookup_revmap_t &o)
  {
    ډک = o.ډک;
    fv_indexes = o.fv_indexes;
    أرجع *هذا;
  }

  منطقي ډک = ریښتیا;
  hb_سېټ_t fv_indexes;
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
بنية hb_depend_معلومات_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_ویکټور_t<hb::unique_ptr<hb_سېټ_t>> sets;

  hb_ویکټور_t<hb_depend_glyph_ریکارډ_t> glyph_dependencies;

  ثابت hb_سېټ_t *get_سېټ_from_شاخص (hb_codepoint_t شاخص_2)
  {
    إذا (شاخص_2 < sets.اوږدتیا)
      أرجع sets[شاخص_2].get ();
    أرجع تش_اشاره;
  }

  بینښه صحيح get_glyph_ثبت_شمېر (hb_codepoint_t gid) ثابت
  {
    إذا (gid < glyph_dependencies.اوږدتیا)
      أرجع glyph_dependencies[gid].dependencies.اوږدتیا;
    أرجع 0;
  }

  منطقي get_glyph_ثبت (hb_codepoint_t gid, بینښه صحيح شاخص_2,
                        hb_نښان_t *جدول_نښان_2, hb_codepoint_t *dependent,
                        hb_نښان_t *layout_نښان, hb_codepoint_t *ligature_سېټ_2,
                        hb_codepoint_t *context_سېټ, uint8_t *flags)
  {
    إذا (gid < glyph_dependencies.اوږدتیا &&
        شاخص_2 < glyph_dependencies[gid].dependencies.اوږدتیا) {
      خپلکاری &d = glyph_dependencies[gid].dependencies[شاخص_2];
      *جدول_نښان_2 = d.جدول_نښان_2;
      *dependent = d.dependent;
      *layout_نښان = d.layout_نښان;
      *ligature_سېټ_2 = d.ligature_سېټ_2;
      *context_سېټ = d.context_سېټ;
      إذا (flags) *flags = d.flags;
      أرجع ریښتیا;
    }
    أرجع خطأ;
  }

  فراغ print ()
  {
    لكل (بینښه i = 0; i < glyph_dependencies.اوږدتیا; i++) {
      خپلکاری &gd = glyph_dependencies[i];
      إذا (!gd.dependencies.اوږدتیا)
        استمر;
      چاپ_کړه ("GID %u:\n", i);
      لكل (خپلکاری &d : gd.dependencies) {
        إذا (d.جدول_نښان_2 == HB_OT_نښان_GSUB) {
          چاپ_کړه ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_نښان), d.dependent);
          إذا (d.ligature_سېټ_2 != HB_CODEPOINT_INVALID)
            چاپ_کړه ("  (ligature)");
        } وإلا {
          چاپ_کړه ("  %c%c%c%c -> %u", HB_UNTAG(d.جدول_نښان_2), d.dependent);
        }
        چاپ_کړه ("\n");
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
بنية hb_depend_معلومات_builder_t
{
  hb_depend_معلومات_builder_t (hb_depend_معلومات_t &معلومات_2)
    : current_context_سېټ_شاخص (HB_CODEPOINT_INVALID),
      current_څنډه_flags (HB_SUBSET_DEPEND_څنډه_FLAG_NONE),
      معلومات (معلومات_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ثابت hb_سېټ_t *get_سېټ_from_شاخص (hb_codepoint_t شاخص_2)
  { أرجع معلومات.get_سېټ_from_شاخص (شاخص_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  فراغ discard_سېټ (hb_codepoint_t سېټ_شاخص)
  {
    إذا (سېټ_شاخص >= معلومات.sets.اوږدتیا)
    {
      DEBUG_پیغام (SUBSET, تش_اشاره, "Attempting to discard invalid set %u (max is %u)",
                 سېټ_شاخص, معلومات.sets.اوږدتیا - 1);
      أرجع;
    }
    سېټ_منزل_شاخص.del (معلومات.sets[سېټ_شاخص].get ());
    معلومات.sets[سېټ_شاخص]->clear ();
    check_success (free_سېټ_list.ټېل_or_fail (سېټ_شاخص));
  }

  hb_codepoint_t new_سېټ (ثابت hb_سېټ_t &سېټ)
  {
    hb_codepoint_t سېټ_شاخص;

    إذا (free_سېټ_list.اوږدتیا > 0)
    {
      سېټ_شاخص = free_سېټ_list.راوباسه ();
      معلومات.sets[سېټ_شاخص]->سېټ (سېټ);
      إذا (unlikely (معلومات.sets[سېټ_شاخص]->دننه_error ()))
	أرجع fail_invalid ();
    }
    وإلا
    {
      سېټ_شاخص = معلومات.sets.اوږدتیا;
      hb_سېټ_t *new_سېټ = hb_سېټ_create ();
      إذا (unlikely (!new_سېټ))
	أرجع fail_invalid ();
      new_سېټ->سېټ (سېټ);
      إذا (unlikely (new_سېټ->دننه_error ()))
      {
	hb_سېټ_destroy (new_سېټ);
	أرجع fail_invalid ();
      }
      إذا (unlikely (!معلومات.sets.ټېل_or_fail (hb::unique_ptr<hb_سېټ_t> {new_سېټ})))
	أرجع fail_invalid ();
    }

    أرجع سېټ_شاخص;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t پیدا_کړه_or_create_سېټ (ثابت hb_سېټ_t &سېټ, منطقي *created = تش_اشاره)
  {
    hb_codepoint_t *existing_idx = تش_اشاره;
    إذا (سېټ_منزل_شاخص.has (&سېټ, &existing_idx))
    {
      إذا (created) *created = خطأ;
      أرجع *existing_idx;
    }

    hb_codepoint_t new_idx = new_سېټ (سېټ);
    إذا (unlikely (new_idx == HB_CODEPOINT_INVALID))
      أرجع HB_CODEPOINT_INVALID;

    إذا (unlikely (!سېټ_منزل_شاخص.سېټ (معلومات.sets[new_idx].get (), new_idx)))
      أرجع fail_invalid ();
    إذا (created) *created = ریښتیا;
    أرجع new_idx;
  }

  hb_codepoint_t پیدا_کړه_or_create_context_سېټ (ثابت hb_سېټ_t &سېټ)
  { أرجع پیدا_کړه_or_create_سېټ (سېټ); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_سېټ (ثابت hb_ویکټور_t<hb_سېټ_t> *backtrack_sets,
                                     ثابت hb_ویکټور_t<hb_سېټ_t> *lookahead_sets)
  {
    إذا ((!backtrack_sets || backtrack_sets->اوږدتیا == 0) &&
        (!lookahead_sets || lookahead_sets->اوږدتیا == 0))
      أرجع HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_سېټ_t مستقیم_requirements;

    إذا (backtrack_sets)
    {
      لكل (ثابت خپلکاری &شا_سېټ : *backtrack_sets)
      {
        إذا (شا_سېټ.get_population () == 1)
          مستقیم_requirements.جمع_کړه (شا_سېټ.get_min ());
      }
    }

    إذا (lookahead_sets)
    {
      لكل (ثابت خپلکاری &look_سېټ : *lookahead_sets)
      {
        إذا (look_سېټ.get_population () == 1)
          مستقیم_requirements.جمع_کړه (look_سېټ.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_سېټ_t context_elements;

    إذا (backtrack_sets)
    {
      لكل (ثابت خپلکاری &شا_سېټ : *backtrack_sets)
      {
        إذا (شا_سېټ.get_population () > 1)
        {
          hb_سېټ_t filtered_سېټ;
          filtered_سېټ.سېټ (شا_سېټ);
          filtered_سېټ.کمول (مستقیم_requirements);
          إذا (unlikely (filtered_سېټ.دننه_error ()))
            أرجع fail_invalid ();

          إذا (!filtered_سېټ.is_empty ())
          {
            hb_codepoint_t سېټ_idx = پیدا_کړه_or_create_context_سېټ (filtered_سېټ);
            إذا (unlikely (سېټ_idx == HB_CODEPOINT_INVALID))
              أرجع HB_CODEPOINT_INVALID;
            context_elements.جمع_کړه (HB_DEPEND_CONTEXT_سېټ_FLAG | سېټ_idx);
          }
        }
      }
    }

    إذا (lookahead_sets)
    {
      لكل (ثابت خپلکاری &look_سېټ : *lookahead_sets)
      {
        إذا (look_سېټ.get_population () > 1)
        {
          hb_سېټ_t filtered_سېټ;
          filtered_سېټ.سېټ (look_سېټ);
          filtered_سېټ.کمول (مستقیم_requirements);
          إذا (unlikely (filtered_سېټ.دننه_error ()))
            أرجع fail_invalid ();

          إذا (!filtered_سېټ.is_empty ())
          {
            hb_codepoint_t سېټ_idx = پیدا_کړه_or_create_context_سېټ (filtered_سېټ);
            إذا (unlikely (سېټ_idx == HB_CODEPOINT_INVALID))
              أرجع HB_CODEPOINT_INVALID;
            context_elements.جمع_کړه (HB_DEPEND_CONTEXT_سېټ_FLAG | سېټ_idx);
          }
        }
      }
    }

    context_elements.union_ (مستقیم_requirements);
    إذا (unlikely (مستقیم_requirements.دننه_error () ||
                  context_elements.دننه_error ()))
      أرجع fail_invalid ();

    إذا (context_elements.is_empty ())
      أرجع HB_CODEPOINT_INVALID;

    أرجع پیدا_کړه_or_create_context_سېټ (context_elements);
  }

  ساكن uint64_t encode_څنډه_ref (hb_codepoint_t سرچینه, بینښه صحيح شاخص_2)
  { أرجع (uint64_t (سرچینه) + 1) << 32 | شاخص_2; }

  ساكن فراغ decode_څنډه_ref (uint64_t ref,
			       hb_codepoint_t *سرچینه,
			       بینښه صحيح *شاخص_2)
  {
    *سرچینه = (ref >> 32) - 1;
    *شاخص_2 = ref;
  }

  uint32_t څنډه_hash (hb_codepoint_t سرچینه, ثابت hb_depend_څنډه_t &ریکارډ) ثابت
  {
    hb_depend_څنډه_کلی_t key (سرچینه, ریکارډ.جدول_نښان_2, ریکارډ.layout_نښان,
			      ریکارډ.dependent, ریکارډ.ligature_سېټ_2,
			      ریکارډ.context_سېټ);
    أرجع key.hash ();
  }

  منطقي resize_څنډه_slots ()
  {
    بینښه صحيح new_اندازه = څنډه_slots.اوږدتیا ? څنډه_slots.اوږدتیا * 2 : 8;
    إذا (unlikely (new_اندازه < څنډه_slots.اوږدتیا))
      أرجع fail ();

    hb_ویکټور_t<uint64_t> new_slots;
    إذا (unlikely (!new_slots.resize_exact (new_اندازه)))
      أرجع fail ();

    لكل (uint64_t ref : څنډه_slots)
    {
      إذا (!ref)
	استمر;

      hb_codepoint_t سرچینه;
      بینښه صحيح شاخص_2;
      decode_څنډه_ref (ref, &سرچینه, &شاخص_2);
      ثابت hb_depend_څنډه_t &ریکارډ = معلومات.glyph_dependencies[سرچینه].dependencies[شاخص_2];
      بینښه صحيح خونه = څنډه_hash (سرچینه, ریکارډ) & (new_اندازه - 1);
      بینښه صحيح ګام = 0;
      طالما (new_slots[خونه])
	خونه = (خونه + ++ګام) & (new_اندازه - 1);
      new_slots[خونه] = ref;
    }

    څنډه_slots = std::move (new_slots);
    أرجع ریښتیا;
  }

  منطقي جمع_کړه_څنډه (hb_codepoint_t سرچینه, ثابت hb_depend_څنډه_t &ریکارډ)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    إذا (unlikely (!څنډه_slots.اوږدتیا ||
		  uint64_t (څنډه_population) * 3 / 2 >= څنډه_slots.اوږدتیا - 1))
      إذا (unlikely (!resize_څنډه_slots ()))
	أرجع خطأ;

    بینښه صحيح خونه = څنډه_hash (سرچینه, ریکارډ) & (څنډه_slots.اوږدتیا - 1);
    بینښه صحيح ګام = 0;
    طالما (uint64_t ref = څنډه_slots[خونه])
    {
      hb_codepoint_t existing_سرچینه;
      بینښه صحيح existing_شاخص;
      decode_څنډه_ref (ref, &existing_سرچینه, &existing_شاخص);
      إذا (existing_سرچینه == سرچینه &&
	  معلومات.glyph_dependencies[سرچینه].dependencies[existing_شاخص] == ریکارډ)
	أرجع خطأ;
      خونه = (خونه + ++ګام) & (څنډه_slots.اوږدتیا - 1);
    }

    خپلکاری &dependencies = معلومات.glyph_dependencies[سرچینه].dependencies;
    بینښه صحيح شاخص_2 = dependencies.اوږدتیا;
    إذا (unlikely (!dependencies.ټېل_or_fail (ریکارډ)))
      أرجع fail ();

    څنډه_slots[خونه] = encode_څنډه_ref (سرچینه, شاخص_2);
    څنډه_population++;
    أرجع ریښتیا;
  }

  منطقي جمع_کړه_depend_layout (hb_codepoint_t target, hb_نښان_t جدول_نښان_2,
                          hb_نښان_t layout_نښان,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_سېټ = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_سېټ = HB_CODEPOINT_INVALID,
                          hb_subset_depend_څنډه_flags_t flags = HB_SUBSET_DEPEND_څنډه_FLAG_NONE)
  {
    إذا (target >= معلومات.glyph_dependencies.اوږدتیا) {
      DEBUG_پیغام (SUBSET, تش_اشاره, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(جدول_نښان_2));
      أرجع خطأ;
    }

    أرجع جمع_کړه_څنډه (target, hb_depend_څنډه_t (جدول_نښان_2, dependent, layout_نښان,
					      lig_سېټ, context_سېټ, flags));
  }

  منطقي جمع_کړه_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_شاخص,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_سېټ = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_سېټ = HB_CODEPOINT_INVALID)
  {
    إذا (context_سېټ == HB_CODEPOINT_INVALID)
      context_سېټ = current_context_سېټ_شاخص;
    hb_subset_depend_څنډه_flags_t flags = current_څنډه_flags;

    منطقي any_added = خطأ;
    لكل (uint64_t ثبت : get_lookup_features (lookup_شاخص)) {
      hb_نښان_t t = (hb_نښان_t) ثبت;
      إذا (جمع_کړه_depend_layout (target, HB_OT_نښان_GSUB, t, dependent, lig_سېټ, context_سېټ, flags))
        any_added = ریښتیا;
    }
    أرجع any_added;
  }

  منطقي init_lookup_features (بینښه lookup_شمېر)
  {
    أرجع check_success (lookup_feature_offsets.resize (lookup_شمېر + 1));
  }

  منطقي جمع_کړه_lookup_feature (hb_codepoint_t lookup_شاخص, hb_نښان_t feature_نښان)
  {
    إذا (feature_نښان == HB_سېټ_ارزښت_INVALID)
      أرجع ریښتیا;

    إذا (unlikely (!lookup_feature_offsets.اوږدتیا ||
		  lookup_شاخص >= lookup_feature_offsets.اوږدتیا - 1))
      أرجع fail ();

    uint64_t ثبت = ((uint64_t) lookup_شاخص << 32) | feature_نښان;
    أرجع check_success (lookup_features.ټېل_or_fail (ثبت));
  }

  منطقي پای_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      أرجع a < b ? -1 : a > b ? 1 : 0;
    });

    بینښه اكتب = 0;
    لكل (uint64_t ثبت : lookup_features)
      إذا (!اكتب || ثبت != lookup_features[اكتب - 1])
	lookup_features[اكتب++] = ثبت;
    lookup_features.shrink (اكتب, خطأ);

    بینښه feature_شاخص = 0;
    بینښه lookup_شمېر = lookup_feature_offsets.اوږدتیا - 1;
    لكل (بینښه lookup_شاخص = 0; lookup_شاخص < lookup_شمېر; lookup_شاخص++)
    {
      lookup_feature_offsets[lookup_شاخص] = feature_شاخص;
      طالما (feature_شاخص < lookup_features.اوږدتیا &&
	     lookup_features[feature_شاخص] >> 32 == lookup_شاخص)
	feature_شاخص++;
    }
    lookup_feature_offsets[lookup_شمېر] = feature_شاخص;
    أرجع ریښتیا;
  }

  hb_array_t<ثابت uint64_t> get_lookup_features (hb_codepoint_t lookup_شاخص) ثابت
  {
    إذا (unlikely (!lookup_feature_offsets.اوږدتیا ||
		  lookup_شاخص >= lookup_feature_offsets.اوږدتیا - 1))
      أرجع {};

    بینښه پیل = lookup_feature_offsets[lookup_شاخص];
    بینښه پای_ټکی = lookup_feature_offsets[lookup_شاخص + 1];
    أرجع lookup_features.as_array ().sub_array (پیل, پای_ټکی - پیل);
  }

  فراغ جمع_کړه_depend (hb_codepoint_t target, hb_نښان_t جدول_نښان_2,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_سېټ = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_سېټ = HB_CODEPOINT_INVALID,
                   hb_subset_depend_څنډه_flags_t flags = HB_SUBSET_DEPEND_څنډه_FLAG_NONE)
  {
    جمع_کړه_depend_layout (target, جدول_نښان_2, HB_CODEPOINT_INVALID, dependent, lig_سېټ,
                       context_سېټ, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { أرجع hb_map_get (&nominal_glyphs, cp); }

  HB_دننی منطقي compile (hb_face_t *face);

  منطقي fail () { successful = خطأ; أرجع خطأ; }
  hb_codepoint_t fail_invalid () { fail (); أرجع HB_CODEPOINT_INVALID; }
  منطقي check_success (منطقي s) { successful = (successful && s); أرجع successful; }

  HB_دننی فراغ get_gsub_dependencies (hb_face_t *face);

  منطقي successful = ریښتیا;
  hb_سېټ_t unicodes;
  hb_map_t nominal_glyphs;
  hb_ویکټور_t<uint64_t> lookup_features;
  hb_ویکټور_t<بینښه> lookup_feature_offsets;
  hb_ویکټور_t<uint64_t> څنډه_slots;
  بینښه صحيح څنډه_population = 0;
  hb_hashmap_t<ثابت hb_سېټ_t*, hb_codepoint_t> سېټ_منزل_شاخص;
  hb_ویکټور_t<hb_codepoint_t> free_سېټ_list;
  hb_codepoint_t current_context_سېټ_شاخص;
  hb_subset_depend_څنډه_flags_t current_څنډه_flags;

  hb_depend_معلومات_t &معلومات;
};


#د_که_پای /* HB_DEPEND_DATA_HH */
