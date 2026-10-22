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

#جيڪڏهن_وصف_نه_ٿيل HB_DEPEND_معلومات_HH
#وصف_ڪريو HB_DEPEND_معلومات_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#ضمن "hb.hh"

#ضمن "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#جيڪڏهن_وصف_نه_ٿيل HB_SUBSET_DEPEND_ڪنارو_FLAGS_T_DEFINED
#وصف_ڪريو HB_SUBSET_DEPEND_ڪنارو_FLAGS_T_DEFINED
تعريفنوع تعداد {
  HB_SUBSET_DEPEND_ڪنارو_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_ڪنارو_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_ڪنارو_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_ڪنارو_flags_t;
#جيڪڏهن_پورو
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_ڪنارو_flags_t);

/* High bit of a context-set element marks it as an index into the sets array
 * rather than a raw glyph ID. */
#وصف_ڪريو HB_DEPEND_CONTEXT_سيٽ_FLAG 0x80000000u

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
بنية hb_depend_ڪنارو_t {
  hb_depend_ڪنارو_t() = ميٽيو;
  hb_depend_ڪنارو_t(hb_ٽيگ_t جدول_ٽيگ_2,
                   hb_codepoint_t dependent,
                   hb_ٽيگ_t layout_ٽيگ,
                   hb_codepoint_t ligature_سيٽ_2,
                   hb_codepoint_t context_سيٽ,
                   hb_subset_depend_ڪنارو_flags_t flags = HB_SUBSET_DEPEND_ڪنارو_FLAG_NONE) : جدول_ٽيگ_2(جدول_ٽيگ_2),
     dependent(dependent), layout_ٽيگ(layout_ٽيگ),
     ligature_سيٽ_2(ligature_سيٽ_2), context_سيٽ(context_سيٽ),
     flags(flags)
     {}

  منطقي عامل == (ثابت hb_depend_ڪنارو_t &o) ثابت
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    أرجع جدول_ٽيگ_2 == o.جدول_ٽيگ_2 &&
           dependent == o.dependent &&
           layout_ٽيگ == o.layout_ٽيگ &&
           ligature_سيٽ_2 == o.ligature_سيٽ_2 &&
           context_سيٽ == o.context_سيٽ;
  }

  uint32_t hash () ثابت
  {
    /* FNV-1a hash of all identity fields (excludes flags) */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (جدول_ٽيگ_2);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ hb_hash (dependent);
    current = current * 16777619;
    current = current ^ hb_hash (layout_ٽيگ);
    current = current * 16777619;
    current = current ^ hb_hash (ligature_سيٽ_2);
    current = current * 16777619;
    current = current ^ hb_hash (context_سيٽ);
    current = current * 16777619;
    أرجع current;
  }

  hb_ٽيگ_t جدول_ٽيگ_2;
  hb_codepoint_t dependent;
  hb_ٽيگ_t layout_ٽيگ;
  hb_codepoint_t ligature_سيٽ_2;
  hb_codepoint_t context_سيٽ;
  hb_subset_depend_ڪنارو_flags_t flags;
};

/**
 * hb_depend_edge_key_t:
 *
 * Key structure for edge deduplication. Combines source glyph with edge record.
 * Uses the record's equality and hash operators for comparison.
 */
بنية hb_depend_ڪنارو_ڪنجي_t {
  hb_codepoint_t ذريعو;
  hb_depend_ڪنارو_t رڪارڊ;

  hb_depend_ڪنارو_ڪنجي_t () : ذريعو (0), رڪارڊ (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_ڪنارو_FLAG_NONE) {}

  hb_depend_ڪنارو_ڪنجي_t (hb_codepoint_t ذريعو,
              hb_ٽيگ_t جدول_ٽيگ_2,
              hb_ٽيگ_t layout_ٽيگ,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_سيٽ_2,
              hb_codepoint_t context_سيٽ)
    : ذريعو (ذريعو),
      رڪارڊ (جدول_ٽيگ_2, dependent, layout_ٽيگ, ligature_سيٽ_2, context_سيٽ, HB_SUBSET_DEPEND_ڪنارو_FLAG_NONE) {}

  منطقي عامل == (ثابت hb_depend_ڪنارو_ڪنجي_t &o) ثابت
  {
    أرجع ذريعو == o.ذريعو && رڪارڊ == o.رڪارڊ;
  }

  uint32_t hash () ثابت
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (ذريعو);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ رڪارڊ.hash ();
    current = current * 16777619;
    أرجع current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
بنية hb_depend_glyph_رڪارڊ_t {
  hb_ويڪٽر_t<hb_depend_ڪنارو_t> dependencies;
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
  واضح hb_depend_lookup_revmap_t (منطقي ڀريل) : ڀريل (ڀريل) {}
  hb_depend_lookup_revmap_t (ثابت hb_depend_lookup_revmap_t &o) : ڀريل(o.ڀريل),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : ڀريل(o.ڀريل),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& عامل = (ثابت hb_depend_lookup_revmap_t &o)
  {
    ڀريل = o.ڀريل;
    fv_indexes = o.fv_indexes;
    أرجع *هذا;
  }

  منطقي ڀريل = سچ;
  hb_سيٽ_t fv_indexes;
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
  hb_ويڪٽر_t<hb::unique_ptr<hb_سيٽ_t>> sets;

  hb_ويڪٽر_t<hb_depend_glyph_رڪارڊ_t> glyph_dependencies;

  ثابت hb_سيٽ_t *get_سيٽ_from_اشاريو (hb_codepoint_t اشاريو_2)
  {
    إذا (اشاريو_2 < sets.ڊيگهائي)
      أرجع sets[اشاريو_2].get ();
    أرجع خالي_اشارو;
  }

  بنانشان صحيح get_glyph_داخلا_ڳڻپ (hb_codepoint_t gid) ثابت
  {
    إذا (gid < glyph_dependencies.ڊيگهائي)
      أرجع glyph_dependencies[gid].dependencies.ڊيگهائي;
    أرجع 0;
  }

  منطقي get_glyph_داخلا (hb_codepoint_t gid, بنانشان صحيح اشاريو_2,
                        hb_ٽيگ_t *جدول_ٽيگ_2, hb_codepoint_t *dependent,
                        hb_ٽيگ_t *layout_ٽيگ, hb_codepoint_t *ligature_سيٽ_2,
                        hb_codepoint_t *context_سيٽ, uint8_t *flags)
  {
    إذا (gid < glyph_dependencies.ڊيگهائي &&
        اشاريو_2 < glyph_dependencies[gid].dependencies.ڊيگهائي) {
      خودڪار &d = glyph_dependencies[gid].dependencies[اشاريو_2];
      *جدول_ٽيگ_2 = d.جدول_ٽيگ_2;
      *dependent = d.dependent;
      *layout_ٽيگ = d.layout_ٽيگ;
      *ligature_سيٽ_2 = d.ligature_سيٽ_2;
      *context_سيٽ = d.context_سيٽ;
      إذا (flags) *flags = d.flags;
      أرجع سچ;
    }
    أرجع خطأ;
  }

  فراغ print ()
  {
    لكل (بنانشان i = 0; i < glyph_dependencies.ڊيگهائي; i++) {
      خودڪار &gd = glyph_dependencies[i];
      إذا (!gd.dependencies.ڊيگهائي)
        استمر;
      لکو ("GID %u:\n", i);
      لكل (خودڪار &d : gd.dependencies) {
        إذا (d.جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB) {
          لکو ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_ٽيگ), d.dependent);
          إذا (d.ligature_سيٽ_2 != HB_CODEPOINT_INVALID)
            لکو ("  (ligature)");
        } وإلا {
          لکو ("  %c%c%c%c -> %u", HB_UNTAG(d.جدول_ٽيگ_2), d.dependent);
        }
        لکو ("\n");
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
    : current_context_سيٽ_اشاريو (HB_CODEPOINT_INVALID),
      current_ڪنارو_flags (HB_SUBSET_DEPEND_ڪنارو_FLAG_NONE),
      معلومات (معلومات_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ثابت hb_سيٽ_t *get_سيٽ_from_اشاريو (hb_codepoint_t اشاريو_2)
  { أرجع معلومات.get_سيٽ_from_اشاريو (اشاريو_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  فراغ discard_سيٽ (hb_codepoint_t سيٽ_اشاريو)
  {
    إذا (سيٽ_اشاريو >= معلومات.sets.ڊيگهائي)
    {
      DEBUG_نياپو (SUBSET, خالي_اشارو, "Attempting to discard invalid set %u (max is %u)",
                 سيٽ_اشاريو, معلومات.sets.ڊيگهائي - 1);
      أرجع;
    }
    سيٽ_منزل_اشاريو.del (معلومات.sets[سيٽ_اشاريو].get ());
    معلومات.sets[سيٽ_اشاريو]->clear ();
    check_success (free_سيٽ_list.ڌڪو_or_fail (سيٽ_اشاريو));
  }

  hb_codepoint_t new_سيٽ (ثابت hb_سيٽ_t &سيٽ)
  {
    hb_codepoint_t سيٽ_اشاريو;

    إذا (free_سيٽ_list.ڊيگهائي > 0)
    {
      سيٽ_اشاريو = free_سيٽ_list.ڪڍو ();
      معلومات.sets[سيٽ_اشاريو]->سيٽ (سيٽ);
      إذا (unlikely (معلومات.sets[سيٽ_اشاريو]->اندريون_error ()))
	أرجع fail_invalid ();
    }
    وإلا
    {
      سيٽ_اشاريو = معلومات.sets.ڊيگهائي;
      hb_سيٽ_t *new_سيٽ = hb_سيٽ_create ();
      إذا (unlikely (!new_سيٽ))
	أرجع fail_invalid ();
      new_سيٽ->سيٽ (سيٽ);
      إذا (unlikely (new_سيٽ->اندريون_error ()))
      {
	hb_سيٽ_destroy (new_سيٽ);
	أرجع fail_invalid ();
      }
      إذا (unlikely (!معلومات.sets.ڌڪو_or_fail (hb::unique_ptr<hb_سيٽ_t> {new_سيٽ})))
	أرجع fail_invalid ();
    }

    أرجع سيٽ_اشاريو;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t ڳوليو_or_create_سيٽ (ثابت hb_سيٽ_t &سيٽ, منطقي *created = خالي_اشارو)
  {
    hb_codepoint_t *existing_idx = خالي_اشارو;
    إذا (سيٽ_منزل_اشاريو.has (&سيٽ, &existing_idx))
    {
      إذا (created) *created = خطأ;
      أرجع *existing_idx;
    }

    hb_codepoint_t new_idx = new_سيٽ (سيٽ);
    إذا (unlikely (new_idx == HB_CODEPOINT_INVALID))
      أرجع HB_CODEPOINT_INVALID;

    إذا (unlikely (!سيٽ_منزل_اشاريو.سيٽ (معلومات.sets[new_idx].get (), new_idx)))
      أرجع fail_invalid ();
    إذا (created) *created = سچ;
    أرجع new_idx;
  }

  hb_codepoint_t ڳوليو_or_create_context_سيٽ (ثابت hb_سيٽ_t &سيٽ)
  { أرجع ڳوليو_or_create_سيٽ (سيٽ); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_سيٽ (ثابت hb_ويڪٽر_t<hb_سيٽ_t> *backtrack_sets,
                                     ثابت hb_ويڪٽر_t<hb_سيٽ_t> *lookahead_sets)
  {
    إذا ((!backtrack_sets || backtrack_sets->ڊيگهائي == 0) &&
        (!lookahead_sets || lookahead_sets->ڊيگهائي == 0))
      أرجع HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_سيٽ_t سڌو_requirements;

    إذا (backtrack_sets)
    {
      لكل (ثابت خودڪار &پٺ_سيٽ : *backtrack_sets)
      {
        إذا (پٺ_سيٽ.get_population () == 1)
          سڌو_requirements.جوڙيو (پٺ_سيٽ.get_min ());
      }
    }

    إذا (lookahead_sets)
    {
      لكل (ثابت خودڪار &look_سيٽ : *lookahead_sets)
      {
        إذا (look_سيٽ.get_population () == 1)
          سڌو_requirements.جوڙيو (look_سيٽ.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_سيٽ_t context_elements;

    إذا (backtrack_sets)
    {
      لكل (ثابت خودڪار &پٺ_سيٽ : *backtrack_sets)
      {
        إذا (پٺ_سيٽ.get_population () > 1)
        {
          hb_سيٽ_t filtered_سيٽ;
          filtered_سيٽ.سيٽ (پٺ_سيٽ);
          filtered_سيٽ.ڪٽيو (سڌو_requirements);
          إذا (unlikely (filtered_سيٽ.اندريون_error ()))
            أرجع fail_invalid ();

          إذا (!filtered_سيٽ.is_empty ())
          {
            hb_codepoint_t سيٽ_idx = ڳوليو_or_create_context_سيٽ (filtered_سيٽ);
            إذا (unlikely (سيٽ_idx == HB_CODEPOINT_INVALID))
              أرجع HB_CODEPOINT_INVALID;
            context_elements.جوڙيو (HB_DEPEND_CONTEXT_سيٽ_FLAG | سيٽ_idx);
          }
        }
      }
    }

    إذا (lookahead_sets)
    {
      لكل (ثابت خودڪار &look_سيٽ : *lookahead_sets)
      {
        إذا (look_سيٽ.get_population () > 1)
        {
          hb_سيٽ_t filtered_سيٽ;
          filtered_سيٽ.سيٽ (look_سيٽ);
          filtered_سيٽ.ڪٽيو (سڌو_requirements);
          إذا (unlikely (filtered_سيٽ.اندريون_error ()))
            أرجع fail_invalid ();

          إذا (!filtered_سيٽ.is_empty ())
          {
            hb_codepoint_t سيٽ_idx = ڳوليو_or_create_context_سيٽ (filtered_سيٽ);
            إذا (unlikely (سيٽ_idx == HB_CODEPOINT_INVALID))
              أرجع HB_CODEPOINT_INVALID;
            context_elements.جوڙيو (HB_DEPEND_CONTEXT_سيٽ_FLAG | سيٽ_idx);
          }
        }
      }
    }

    context_elements.union_ (سڌو_requirements);
    إذا (unlikely (سڌو_requirements.اندريون_error () ||
                  context_elements.اندريون_error ()))
      أرجع fail_invalid ();

    إذا (context_elements.is_empty ())
      أرجع HB_CODEPOINT_INVALID;

    أرجع ڳوليو_or_create_context_سيٽ (context_elements);
  }

  ساكن uint64_t encode_ڪنارو_ref (hb_codepoint_t ذريعو, بنانشان صحيح اشاريو_2)
  { أرجع (uint64_t (ذريعو) + 1) << 32 | اشاريو_2; }

  ساكن فراغ decode_ڪنارو_ref (uint64_t ref,
			       hb_codepoint_t *ذريعو,
			       بنانشان صحيح *اشاريو_2)
  {
    *ذريعو = (ref >> 32) - 1;
    *اشاريو_2 = ref;
  }

  uint32_t ڪنارو_hash (hb_codepoint_t ذريعو, ثابت hb_depend_ڪنارو_t &رڪارڊ) ثابت
  {
    hb_depend_ڪنارو_ڪنجي_t key (ذريعو, رڪارڊ.جدول_ٽيگ_2, رڪارڊ.layout_ٽيگ,
			      رڪارڊ.dependent, رڪارڊ.ligature_سيٽ_2,
			      رڪارڊ.context_سيٽ);
    أرجع key.hash ();
  }

  منطقي resize_ڪنارو_slots ()
  {
    بنانشان صحيح new_سائيز = ڪنارو_slots.ڊيگهائي ? ڪنارو_slots.ڊيگهائي * 2 : 8;
    إذا (unlikely (new_سائيز < ڪنارو_slots.ڊيگهائي))
      أرجع fail ();

    hb_ويڪٽر_t<uint64_t> new_slots;
    إذا (unlikely (!new_slots.resize_exact (new_سائيز)))
      أرجع fail ();

    لكل (uint64_t ref : ڪنارو_slots)
    {
      إذا (!ref)
	استمر;

      hb_codepoint_t ذريعو;
      بنانشان صحيح اشاريو_2;
      decode_ڪنارو_ref (ref, &ذريعو, &اشاريو_2);
      ثابت hb_depend_ڪنارو_t &رڪارڊ = معلومات.glyph_dependencies[ذريعو].dependencies[اشاريو_2];
      بنانشان صحيح جڳهه = ڪنارو_hash (ذريعو, رڪارڊ) & (new_سائيز - 1);
      بنانشان صحيح قدم = 0;
      طالما (new_slots[جڳهه])
	جڳهه = (جڳهه + ++قدم) & (new_سائيز - 1);
      new_slots[جڳهه] = ref;
    }

    ڪنارو_slots = std::move (new_slots);
    أرجع سچ;
  }

  منطقي جوڙيو_ڪنارو (hb_codepoint_t ذريعو, ثابت hb_depend_ڪنارو_t &رڪارڊ)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    إذا (unlikely (!ڪنارو_slots.ڊيگهائي ||
		  uint64_t (ڪنارو_population) * 3 / 2 >= ڪنارو_slots.ڊيگهائي - 1))
      إذا (unlikely (!resize_ڪنارو_slots ()))
	أرجع خطأ;

    بنانشان صحيح جڳهه = ڪنارو_hash (ذريعو, رڪارڊ) & (ڪنارو_slots.ڊيگهائي - 1);
    بنانشان صحيح قدم = 0;
    طالما (uint64_t ref = ڪنارو_slots[جڳهه])
    {
      hb_codepoint_t existing_ذريعو;
      بنانشان صحيح existing_اشاريو;
      decode_ڪنارو_ref (ref, &existing_ذريعو, &existing_اشاريو);
      إذا (existing_ذريعو == ذريعو &&
	  معلومات.glyph_dependencies[ذريعو].dependencies[existing_اشاريو] == رڪارڊ)
	أرجع خطأ;
      جڳهه = (جڳهه + ++قدم) & (ڪنارو_slots.ڊيگهائي - 1);
    }

    خودڪار &dependencies = معلومات.glyph_dependencies[ذريعو].dependencies;
    بنانشان صحيح اشاريو_2 = dependencies.ڊيگهائي;
    إذا (unlikely (!dependencies.ڌڪو_or_fail (رڪارڊ)))
      أرجع fail ();

    ڪنارو_slots[جڳهه] = encode_ڪنارو_ref (ذريعو, اشاريو_2);
    ڪنارو_population++;
    أرجع سچ;
  }

  منطقي جوڙيو_depend_layout (hb_codepoint_t target, hb_ٽيگ_t جدول_ٽيگ_2,
                          hb_ٽيگ_t layout_ٽيگ,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_سيٽ = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_سيٽ = HB_CODEPOINT_INVALID,
                          hb_subset_depend_ڪنارو_flags_t flags = HB_SUBSET_DEPEND_ڪنارو_FLAG_NONE)
  {
    إذا (target >= معلومات.glyph_dependencies.ڊيگهائي) {
      DEBUG_نياپو (SUBSET, خالي_اشارو, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(جدول_ٽيگ_2));
      أرجع خطأ;
    }

    أرجع جوڙيو_ڪنارو (target, hb_depend_ڪنارو_t (جدول_ٽيگ_2, dependent, layout_ٽيگ,
					      lig_سيٽ, context_سيٽ, flags));
  }

  منطقي جوڙيو_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_اشاريو,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_سيٽ = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_سيٽ = HB_CODEPOINT_INVALID)
  {
    إذا (context_سيٽ == HB_CODEPOINT_INVALID)
      context_سيٽ = current_context_سيٽ_اشاريو;
    hb_subset_depend_ڪنارو_flags_t flags = current_ڪنارو_flags;

    منطقي any_added = خطأ;
    لكل (uint64_t داخلا : get_lookup_features (lookup_اشاريو)) {
      hb_ٽيگ_t t = (hb_ٽيگ_t) داخلا;
      إذا (جوڙيو_depend_layout (target, HB_OT_ٽيگ_GSUB, t, dependent, lig_سيٽ, context_سيٽ, flags))
        any_added = سچ;
    }
    أرجع any_added;
  }

  منطقي init_lookup_features (بنانشان lookup_ڳڻپ)
  {
    أرجع check_success (lookup_feature_offsets.resize (lookup_ڳڻپ + 1));
  }

  منطقي جوڙيو_lookup_feature (hb_codepoint_t lookup_اشاريو, hb_ٽيگ_t feature_ٽيگ)
  {
    إذا (feature_ٽيگ == HB_سيٽ_قدر_INVALID)
      أرجع سچ;

    إذا (unlikely (!lookup_feature_offsets.ڊيگهائي ||
		  lookup_اشاريو >= lookup_feature_offsets.ڊيگهائي - 1))
      أرجع fail ();

    uint64_t داخلا = ((uint64_t) lookup_اشاريو << 32) | feature_ٽيگ;
    أرجع check_success (lookup_features.ڌڪو_or_fail (داخلا));
  }

  منطقي پڄاڻي_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      أرجع a < b ? -1 : a > b ? 1 : 0;
    });

    بنانشان اكتب = 0;
    لكل (uint64_t داخلا : lookup_features)
      إذا (!اكتب || داخلا != lookup_features[اكتب - 1])
	lookup_features[اكتب++] = داخلا;
    lookup_features.shrink (اكتب, خطأ);

    بنانشان feature_اشاريو = 0;
    بنانشان lookup_ڳڻپ = lookup_feature_offsets.ڊيگهائي - 1;
    لكل (بنانشان lookup_اشاريو = 0; lookup_اشاريو < lookup_ڳڻپ; lookup_اشاريو++)
    {
      lookup_feature_offsets[lookup_اشاريو] = feature_اشاريو;
      طالما (feature_اشاريو < lookup_features.ڊيگهائي &&
	     lookup_features[feature_اشاريو] >> 32 == lookup_اشاريو)
	feature_اشاريو++;
    }
    lookup_feature_offsets[lookup_ڳڻپ] = feature_اشاريو;
    أرجع سچ;
  }

  hb_array_t<ثابت uint64_t> get_lookup_features (hb_codepoint_t lookup_اشاريو) ثابت
  {
    إذا (unlikely (!lookup_feature_offsets.ڊيگهائي ||
		  lookup_اشاريو >= lookup_feature_offsets.ڊيگهائي - 1))
      أرجع {};

    بنانشان شروعات = lookup_feature_offsets[lookup_اشاريو];
    بنانشان پڇاڙي = lookup_feature_offsets[lookup_اشاريو + 1];
    أرجع lookup_features.as_array ().sub_array (شروعات, پڇاڙي - شروعات);
  }

  فراغ جوڙيو_depend (hb_codepoint_t target, hb_ٽيگ_t جدول_ٽيگ_2,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_سيٽ = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_سيٽ = HB_CODEPOINT_INVALID,
                   hb_subset_depend_ڪنارو_flags_t flags = HB_SUBSET_DEPEND_ڪنارو_FLAG_NONE)
  {
    جوڙيو_depend_layout (target, جدول_ٽيگ_2, HB_CODEPOINT_INVALID, dependent, lig_سيٽ,
                       context_سيٽ, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { أرجع hb_map_get (&nominal_glyphs, cp); }

  HB_اندروني منطقي compile (hb_face_t *face);

  منطقي fail () { successful = خطأ; أرجع خطأ; }
  hb_codepoint_t fail_invalid () { fail (); أرجع HB_CODEPOINT_INVALID; }
  منطقي check_success (منطقي s) { successful = (successful && s); أرجع successful; }

  HB_اندروني فراغ get_gsub_dependencies (hb_face_t *face);

  منطقي successful = سچ;
  hb_سيٽ_t unicodes;
  hb_map_t nominal_glyphs;
  hb_ويڪٽر_t<uint64_t> lookup_features;
  hb_ويڪٽر_t<بنانشان> lookup_feature_offsets;
  hb_ويڪٽر_t<uint64_t> ڪنارو_slots;
  بنانشان صحيح ڪنارو_population = 0;
  hb_hashmap_t<ثابت hb_سيٽ_t*, hb_codepoint_t> سيٽ_منزل_اشاريو;
  hb_ويڪٽر_t<hb_codepoint_t> free_سيٽ_list;
  hb_codepoint_t current_context_سيٽ_اشاريو;
  hb_subset_depend_ڪنارو_flags_t current_ڪنارو_flags;

  hb_depend_معلومات_t &معلومات;
};


#جيڪڏهن_پورو /* HB_DEPEND_DATA_HH */
