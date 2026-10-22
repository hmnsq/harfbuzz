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

#اگر_غیر_تعریف_شدہ HB_DEPEND_ڈیٹا_HH
#تعریف_کرو HB_DEPEND_ڈیٹا_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#شامل "hb.hh"

#شامل "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#اگر_غیر_تعریف_شدہ HB_SUBSET_DEPEND_کنارہ_FLAGS_T_DEFINED
#تعریف_کرو HB_SUBSET_DEPEND_کنارہ_FLAGS_T_DEFINED
قسمتعریف شمار {
  HB_SUBSET_DEPEND_کنارہ_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_کنارہ_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_کنارہ_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_کنارہ_flags_t;
#اگر_ختم
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_کنارہ_flags_t);

/* High bit of a context-set element marks it as an index into the sets array
 * rather than a raw glyph ID. */
#تعریف_کرو HB_DEPEND_CONTEXT_سیٹ_FLAG 0x80000000u

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
ساخت hb_depend_کنارہ_t {
  hb_depend_کنارہ_t() = حذف;
  hb_depend_کنارہ_t(hb_لیبل_t جدول_لیبل_2,
                   hb_codepoint_t dependent,
                   hb_لیبل_t layout_لیبل,
                   hb_codepoint_t ligature_سیٹ_2,
                   hb_codepoint_t context_سیٹ,
                   hb_subset_depend_کنارہ_flags_t flags = HB_SUBSET_DEPEND_کنارہ_FLAG_NONE) : جدول_لیبل_2(جدول_لیبل_2),
     dependent(dependent), layout_لیبل(layout_لیبل),
     ligature_سیٹ_2(ligature_سیٹ_2), context_سیٹ(context_سیٹ),
     flags(flags)
     {}

  منطقی عامل == (ثابت hb_depend_کنارہ_t &o) ثابت
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    واپس جدول_لیبل_2 == o.جدول_لیبل_2 &&
           dependent == o.dependent &&
           layout_لیبل == o.layout_لیبل &&
           ligature_سیٹ_2 == o.ligature_سیٹ_2 &&
           context_سیٹ == o.context_سیٹ;
  }

  uint32_t hash () ثابت
  {
    /* FNV-1a hash of all identity fields (excludes flags) */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (جدول_لیبل_2);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ hb_hash (dependent);
    current = current * 16777619;
    current = current ^ hb_hash (layout_لیبل);
    current = current * 16777619;
    current = current ^ hb_hash (ligature_سیٹ_2);
    current = current * 16777619;
    current = current ^ hb_hash (context_سیٹ);
    current = current * 16777619;
    واپس current;
  }

  hb_لیبل_t جدول_لیبل_2;
  hb_codepoint_t dependent;
  hb_لیبل_t layout_لیبل;
  hb_codepoint_t ligature_سیٹ_2;
  hb_codepoint_t context_سیٹ;
  hb_subset_depend_کنارہ_flags_t flags;
};

/**
 * hb_depend_edge_key_t:
 *
 * Key structure for edge deduplication. Combines source glyph with edge record.
 * Uses the record's equality and hash operators for comparison.
 */
ساخت hb_depend_کنارہ_کلید_t {
  hb_codepoint_t ماخذ;
  hb_depend_کنارہ_t ریکارڈ;

  hb_depend_کنارہ_کلید_t () : ماخذ (0), ریکارڈ (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_کنارہ_FLAG_NONE) {}

  hb_depend_کنارہ_کلید_t (hb_codepoint_t ماخذ,
              hb_لیبل_t جدول_لیبل_2,
              hb_لیبل_t layout_لیبل,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_سیٹ_2,
              hb_codepoint_t context_سیٹ)
    : ماخذ (ماخذ),
      ریکارڈ (جدول_لیبل_2, dependent, layout_لیبل, ligature_سیٹ_2, context_سیٹ, HB_SUBSET_DEPEND_کنارہ_FLAG_NONE) {}

  منطقی عامل == (ثابت hb_depend_کنارہ_کلید_t &o) ثابت
  {
    واپس ماخذ == o.ماخذ && ریکارڈ == o.ریکارڈ;
  }

  uint32_t hash () ثابت
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (ماخذ);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ ریکارڈ.hash ();
    current = current * 16777619;
    واپس current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
ساخت hb_depend_glyph_ریکارڈ_t {
  hb_سمتیہ_t<hb_depend_کنارہ_t> dependencies;
};

/**
 * hb_depend_lookup_revmap_t:
 *
 * Internal structure tracking which features are associated with a lookup.
 * Used during GSUB dependency analysis to record the feature tags that
 * activate each lookup.
 */
ساخت hb_depend_lookup_revmap_t
{
  hb_depend_lookup_revmap_t () = طےشدہ;
  صریح hb_depend_lookup_revmap_t (منطقی بھرا) : بھرا (بھرا) {}
  hb_depend_lookup_revmap_t (ثابت hb_depend_lookup_revmap_t &o) : بھرا(o.بھرا),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : بھرا(o.بھرا),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& عامل = (ثابت hb_depend_lookup_revmap_t &o)
  {
    بھرا = o.بھرا;
    fv_indexes = o.fv_indexes;
    واپس *یہ;
  }

  منطقی بھرا = سچ;
  hb_سیٹ_t fv_indexes;
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
ساخت hb_depend_ڈیٹا_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_سمتیہ_t<hb::unique_ptr<hb_سیٹ_t>> sets;

  hb_سمتیہ_t<hb_depend_glyph_ریکارڈ_t> glyph_dependencies;

  ثابت hb_سیٹ_t *get_سیٹ_from_اشاریہ (hb_codepoint_t اشاریہ_2)
  {
    اگر (اشاریہ_2 < sets.لمبائی)
      واپس sets[اشاریہ_2].get ();
    واپس خالی_اشاریہ;
  }

  غیرعلامتی صحیح get_glyph_اندراج_گنتی (hb_codepoint_t gid) ثابت
  {
    اگر (gid < glyph_dependencies.لمبائی)
      واپس glyph_dependencies[gid].dependencies.لمبائی;
    واپس 0;
  }

  منطقی get_glyph_اندراج (hb_codepoint_t gid, غیرعلامتی صحیح اشاریہ_2,
                        hb_لیبل_t *جدول_لیبل_2, hb_codepoint_t *dependent,
                        hb_لیبل_t *layout_لیبل, hb_codepoint_t *ligature_سیٹ_2,
                        hb_codepoint_t *context_سیٹ, uint8_t *flags)
  {
    اگر (gid < glyph_dependencies.لمبائی &&
        اشاریہ_2 < glyph_dependencies[gid].dependencies.لمبائی) {
      خودکار &d = glyph_dependencies[gid].dependencies[اشاریہ_2];
      *جدول_لیبل_2 = d.جدول_لیبل_2;
      *dependent = d.dependent;
      *layout_لیبل = d.layout_لیبل;
      *ligature_سیٹ_2 = d.ligature_سیٹ_2;
      *context_سیٹ = d.context_سیٹ;
      اگر (flags) *flags = d.flags;
      واپس سچ;
    }
    واپس جھوٹ;
  }

  خالی print ()
  {
    برائے (غیرعلامتی i = 0; i < glyph_dependencies.لمبائی; i++) {
      خودکار &gd = glyph_dependencies[i];
      اگر (!gd.dependencies.لمبائی)
        جاری;
      لکھو ("GID %u:\n", i);
      برائے (خودکار &d : gd.dependencies) {
        اگر (d.جدول_لیبل_2 == HB_OT_لیبل_GSUB) {
          لکھو ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_لیبل), d.dependent);
          اگر (d.ligature_سیٹ_2 != HB_CODEPOINT_INVALID)
            لکھو ("  (ligature)");
        } ورنہ {
          لکھو ("  %c%c%c%c -> %u", HB_UNTAG(d.جدول_لیبل_2), d.dependent);
        }
        لکھو ("\n");
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
ساخت hb_depend_ڈیٹا_builder_t
{
  hb_depend_ڈیٹا_builder_t (hb_depend_ڈیٹا_t &ڈیٹا_2)
    : current_context_سیٹ_اشاریہ (HB_CODEPOINT_INVALID),
      current_کنارہ_flags (HB_SUBSET_DEPEND_کنارہ_FLAG_NONE),
      ڈیٹا (ڈیٹا_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ثابت hb_سیٹ_t *get_سیٹ_from_اشاریہ (hb_codepoint_t اشاریہ_2)
  { واپس ڈیٹا.get_سیٹ_from_اشاریہ (اشاریہ_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  خالی discard_سیٹ (hb_codepoint_t سیٹ_اشاریہ)
  {
    اگر (سیٹ_اشاریہ >= ڈیٹا.sets.لمبائی)
    {
      DEBUG_پیغام (SUBSET, خالی_اشاریہ, "Attempting to discard invalid set %u (max is %u)",
                 سیٹ_اشاریہ, ڈیٹا.sets.لمبائی - 1);
      واپس;
    }
    سیٹ_مقصد_اشاریہ.del (ڈیٹا.sets[سیٹ_اشاریہ].get ());
    ڈیٹا.sets[سیٹ_اشاریہ]->clear ();
    check_success (free_سیٹ_list.ڈالو_or_fail (سیٹ_اشاریہ));
  }

  hb_codepoint_t new_سیٹ (ثابت hb_سیٹ_t &سیٹ)
  {
    hb_codepoint_t سیٹ_اشاریہ;

    اگر (free_سیٹ_list.لمبائی > 0)
    {
      سیٹ_اشاریہ = free_سیٹ_list.نکالو ();
      ڈیٹا.sets[سیٹ_اشاریہ]->سیٹ (سیٹ);
      اگر (unlikely (ڈیٹا.sets[سیٹ_اشاریہ]->اندر_error ()))
	واپس fail_invalid ();
    }
    ورنہ
    {
      سیٹ_اشاریہ = ڈیٹا.sets.لمبائی;
      hb_سیٹ_t *new_سیٹ = hb_سیٹ_create ();
      اگر (unlikely (!new_سیٹ))
	واپس fail_invalid ();
      new_سیٹ->سیٹ (سیٹ);
      اگر (unlikely (new_سیٹ->اندر_error ()))
      {
	hb_سیٹ_destroy (new_سیٹ);
	واپس fail_invalid ();
      }
      اگر (unlikely (!ڈیٹا.sets.ڈالو_or_fail (hb::unique_ptr<hb_سیٹ_t> {new_سیٹ})))
	واپس fail_invalid ();
    }

    واپس سیٹ_اشاریہ;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t ڈھونڈو_or_create_سیٹ (ثابت hb_سیٹ_t &سیٹ, منطقی *created = خالی_اشاریہ)
  {
    hb_codepoint_t *existing_idx = خالی_اشاریہ;
    اگر (سیٹ_مقصد_اشاریہ.has (&سیٹ, &existing_idx))
    {
      اگر (created) *created = جھوٹ;
      واپس *existing_idx;
    }

    hb_codepoint_t new_idx = new_سیٹ (سیٹ);
    اگر (unlikely (new_idx == HB_CODEPOINT_INVALID))
      واپس HB_CODEPOINT_INVALID;

    اگر (unlikely (!سیٹ_مقصد_اشاریہ.سیٹ (ڈیٹا.sets[new_idx].get (), new_idx)))
      واپس fail_invalid ();
    اگر (created) *created = سچ;
    واپس new_idx;
  }

  hb_codepoint_t ڈھونڈو_or_create_context_سیٹ (ثابت hb_سیٹ_t &سیٹ)
  { واپس ڈھونڈو_or_create_سیٹ (سیٹ); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_سیٹ (ثابت hb_سمتیہ_t<hb_سیٹ_t> *backtrack_sets,
                                     ثابت hb_سمتیہ_t<hb_سیٹ_t> *lookahead_sets)
  {
    اگر ((!backtrack_sets || backtrack_sets->لمبائی == 0) &&
        (!lookahead_sets || lookahead_sets->لمبائی == 0))
      واپس HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_سیٹ_t براہ_راست_requirements;

    اگر (backtrack_sets)
    {
      برائے (ثابت خودکار &عقب_سیٹ : *backtrack_sets)
      {
        اگر (عقب_سیٹ.get_population () == 1)
          براہ_راست_requirements.جوڑو (عقب_سیٹ.get_min ());
      }
    }

    اگر (lookahead_sets)
    {
      برائے (ثابت خودکار &look_سیٹ : *lookahead_sets)
      {
        اگر (look_سیٹ.get_population () == 1)
          براہ_راست_requirements.جوڑو (look_سیٹ.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_سیٹ_t context_elements;

    اگر (backtrack_sets)
    {
      برائے (ثابت خودکار &عقب_سیٹ : *backtrack_sets)
      {
        اگر (عقب_سیٹ.get_population () > 1)
        {
          hb_سیٹ_t filtered_سیٹ;
          filtered_سیٹ.سیٹ (عقب_سیٹ);
          filtered_سیٹ.تفریق (براہ_راست_requirements);
          اگر (unlikely (filtered_سیٹ.اندر_error ()))
            واپس fail_invalid ();

          اگر (!filtered_سیٹ.is_empty ())
          {
            hb_codepoint_t سیٹ_idx = ڈھونڈو_or_create_context_سیٹ (filtered_سیٹ);
            اگر (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
              واپس HB_CODEPOINT_INVALID;
            context_elements.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);
          }
        }
      }
    }

    اگر (lookahead_sets)
    {
      برائے (ثابت خودکار &look_سیٹ : *lookahead_sets)
      {
        اگر (look_سیٹ.get_population () > 1)
        {
          hb_سیٹ_t filtered_سیٹ;
          filtered_سیٹ.سیٹ (look_سیٹ);
          filtered_سیٹ.تفریق (براہ_راست_requirements);
          اگر (unlikely (filtered_سیٹ.اندر_error ()))
            واپس fail_invalid ();

          اگر (!filtered_سیٹ.is_empty ())
          {
            hb_codepoint_t سیٹ_idx = ڈھونڈو_or_create_context_سیٹ (filtered_سیٹ);
            اگر (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
              واپس HB_CODEPOINT_INVALID;
            context_elements.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);
          }
        }
      }
    }

    context_elements.union_ (براہ_راست_requirements);
    اگر (unlikely (براہ_راست_requirements.اندر_error () ||
                  context_elements.اندر_error ()))
      واپس fail_invalid ();

    اگر (context_elements.is_empty ())
      واپس HB_CODEPOINT_INVALID;

    واپس ڈھونڈو_or_create_context_سیٹ (context_elements);
  }

  ساکن uint64_t encode_کنارہ_ref (hb_codepoint_t ماخذ, غیرعلامتی صحیح اشاریہ_2)
  { واپس (uint64_t (ماخذ) + 1) << 32 | اشاریہ_2; }

  ساکن خالی decode_کنارہ_ref (uint64_t ref,
			       hb_codepoint_t *ماخذ,
			       غیرعلامتی صحیح *اشاریہ_2)
  {
    *ماخذ = (ref >> 32) - 1;
    *اشاریہ_2 = ref;
  }

  uint32_t کنارہ_hash (hb_codepoint_t ماخذ, ثابت hb_depend_کنارہ_t &ریکارڈ) ثابت
  {
    hb_depend_کنارہ_کلید_t key (ماخذ, ریکارڈ.جدول_لیبل_2, ریکارڈ.layout_لیبل,
			      ریکارڈ.dependent, ریکارڈ.ligature_سیٹ_2,
			      ریکارڈ.context_سیٹ);
    واپس key.hash ();
  }

  منطقی resize_کنارہ_slots ()
  {
    غیرعلامتی صحیح new_سائز = کنارہ_slots.لمبائی ? کنارہ_slots.لمبائی * 2 : 8;
    اگر (unlikely (new_سائز < کنارہ_slots.لمبائی))
      واپس fail ();

    hb_سمتیہ_t<uint64_t> new_slots;
    اگر (unlikely (!new_slots.resize_exact (new_سائز)))
      واپس fail ();

    برائے (uint64_t ref : کنارہ_slots)
    {
      اگر (!ref)
	جاری;

      hb_codepoint_t ماخذ;
      غیرعلامتی صحیح اشاریہ_2;
      decode_کنارہ_ref (ref, &ماخذ, &اشاریہ_2);
      ثابت hb_depend_کنارہ_t &ریکارڈ = ڈیٹا.glyph_dependencies[ماخذ].dependencies[اشاریہ_2];
      غیرعلامتی صحیح خانہ = کنارہ_hash (ماخذ, ریکارڈ) & (new_سائز - 1);
      غیرعلامتی صحیح قدم = 0;
      جب (new_slots[خانہ])
	خانہ = (خانہ + ++قدم) & (new_سائز - 1);
      new_slots[خانہ] = ref;
    }

    کنارہ_slots = std::move (new_slots);
    واپس سچ;
  }

  منطقی جوڑو_کنارہ (hb_codepoint_t ماخذ, ثابت hb_depend_کنارہ_t &ریکارڈ)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    اگر (unlikely (!کنارہ_slots.لمبائی ||
		  uint64_t (کنارہ_population) * 3 / 2 >= کنارہ_slots.لمبائی - 1))
      اگر (unlikely (!resize_کنارہ_slots ()))
	واپس جھوٹ;

    غیرعلامتی صحیح خانہ = کنارہ_hash (ماخذ, ریکارڈ) & (کنارہ_slots.لمبائی - 1);
    غیرعلامتی صحیح قدم = 0;
    جب (uint64_t ref = کنارہ_slots[خانہ])
    {
      hb_codepoint_t existing_ماخذ;
      غیرعلامتی صحیح existing_اشاریہ;
      decode_کنارہ_ref (ref, &existing_ماخذ, &existing_اشاریہ);
      اگر (existing_ماخذ == ماخذ &&
	  ڈیٹا.glyph_dependencies[ماخذ].dependencies[existing_اشاریہ] == ریکارڈ)
	واپس جھوٹ;
      خانہ = (خانہ + ++قدم) & (کنارہ_slots.لمبائی - 1);
    }

    خودکار &dependencies = ڈیٹا.glyph_dependencies[ماخذ].dependencies;
    غیرعلامتی صحیح اشاریہ_2 = dependencies.لمبائی;
    اگر (unlikely (!dependencies.ڈالو_or_fail (ریکارڈ)))
      واپس fail ();

    کنارہ_slots[خانہ] = encode_کنارہ_ref (ماخذ, اشاریہ_2);
    کنارہ_population++;
    واپس سچ;
  }

  منطقی جوڑو_depend_layout (hb_codepoint_t target, hb_لیبل_t جدول_لیبل_2,
                          hb_لیبل_t layout_لیبل,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_سیٹ = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_سیٹ = HB_CODEPOINT_INVALID,
                          hb_subset_depend_کنارہ_flags_t flags = HB_SUBSET_DEPEND_کنارہ_FLAG_NONE)
  {
    اگر (target >= ڈیٹا.glyph_dependencies.لمبائی) {
      DEBUG_پیغام (SUBSET, خالی_اشاریہ, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(جدول_لیبل_2));
      واپس جھوٹ;
    }

    واپس جوڑو_کنارہ (target, hb_depend_کنارہ_t (جدول_لیبل_2, dependent, layout_لیبل,
					      lig_سیٹ, context_سیٹ, flags));
  }

  منطقی جوڑو_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_اشاریہ,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_سیٹ = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_سیٹ = HB_CODEPOINT_INVALID)
  {
    اگر (context_سیٹ == HB_CODEPOINT_INVALID)
      context_سیٹ = current_context_سیٹ_اشاریہ;
    hb_subset_depend_کنارہ_flags_t flags = current_کنارہ_flags;

    منطقی any_added = جھوٹ;
    برائے (uint64_t اندراج_2 : get_lookup_features (lookup_اشاریہ)) {
      hb_لیبل_t t = (hb_لیبل_t) اندراج_2;
      اگر (جوڑو_depend_layout (target, HB_OT_لیبل_GSUB, t, dependent, lig_سیٹ, context_سیٹ, flags))
        any_added = سچ;
    }
    واپس any_added;
  }

  منطقی init_lookup_features (غیرعلامتی lookup_گنتی)
  {
    واپس check_success (lookup_feature_offsets.resize (lookup_گنتی + 1));
  }

  منطقی جوڑو_lookup_feature (hb_codepoint_t lookup_اشاریہ, hb_لیبل_t feature_لیبل)
  {
    اگر (feature_لیبل == HB_سیٹ_قدر_INVALID)
      واپس سچ;

    اگر (unlikely (!lookup_feature_offsets.لمبائی ||
		  lookup_اشاریہ >= lookup_feature_offsets.لمبائی - 1))
      واپس fail ();

    uint64_t اندراج_2 = ((uint64_t) lookup_اشاریہ << 32) | feature_لیبل;
    واپس check_success (lookup_features.ڈالو_or_fail (اندراج_2));
  }

  منطقی اختتام_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      واپس a < b ? -1 : a > b ? 1 : 0;
    });

    غیرعلامتی اكتب = 0;
    برائے (uint64_t اندراج_2 : lookup_features)
      اگر (!اكتب || اندراج_2 != lookup_features[اكتب - 1])
	lookup_features[اكتب++] = اندراج_2;
    lookup_features.shrink (اكتب, جھوٹ);

    غیرعلامتی feature_اشاریہ = 0;
    غیرعلامتی lookup_گنتی = lookup_feature_offsets.لمبائی - 1;
    برائے (غیرعلامتی lookup_اشاریہ = 0; lookup_اشاریہ < lookup_گنتی; lookup_اشاریہ++)
    {
      lookup_feature_offsets[lookup_اشاریہ] = feature_اشاریہ;
      جب (feature_اشاریہ < lookup_features.لمبائی &&
	     lookup_features[feature_اشاریہ] >> 32 == lookup_اشاریہ)
	feature_اشاریہ++;
    }
    lookup_feature_offsets[lookup_گنتی] = feature_اشاریہ;
    واپس سچ;
  }

  hb_array_t<ثابت uint64_t> get_lookup_features (hb_codepoint_t lookup_اشاریہ) ثابت
  {
    اگر (unlikely (!lookup_feature_offsets.لمبائی ||
		  lookup_اشاریہ >= lookup_feature_offsets.لمبائی - 1))
      واپس {};

    غیرعلامتی آغاز = lookup_feature_offsets[lookup_اشاریہ];
    غیرعلامتی انتہا = lookup_feature_offsets[lookup_اشاریہ + 1];
    واپس lookup_features.as_array ().sub_array (آغاز, انتہا - آغاز);
  }

  خالی جوڑو_depend (hb_codepoint_t target, hb_لیبل_t جدول_لیبل_2,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_سیٹ = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_سیٹ = HB_CODEPOINT_INVALID,
                   hb_subset_depend_کنارہ_flags_t flags = HB_SUBSET_DEPEND_کنارہ_FLAG_NONE)
  {
    جوڑو_depend_layout (target, جدول_لیبل_2, HB_CODEPOINT_INVALID, dependent, lig_سیٹ,
                       context_سیٹ, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { واپس hb_map_get (&nominal_glyphs, cp); }

  HB_اندرونی منطقی compile (hb_face_t *face);

  منطقی fail () { successful = جھوٹ; واپس جھوٹ; }
  hb_codepoint_t fail_invalid () { fail (); واپس HB_CODEPOINT_INVALID; }
  منطقی check_success (منطقی s) { successful = (successful && s); واپس successful; }

  HB_اندرونی خالی get_gsub_dependencies (hb_face_t *face);

  منطقی successful = سچ;
  hb_سیٹ_t unicodes;
  hb_map_t nominal_glyphs;
  hb_سمتیہ_t<uint64_t> lookup_features;
  hb_سمتیہ_t<غیرعلامتی> lookup_feature_offsets;
  hb_سمتیہ_t<uint64_t> کنارہ_slots;
  غیرعلامتی صحیح کنارہ_population = 0;
  hb_hashmap_t<ثابت hb_سیٹ_t*, hb_codepoint_t> سیٹ_مقصد_اشاریہ;
  hb_سمتیہ_t<hb_codepoint_t> free_سیٹ_list;
  hb_codepoint_t current_context_سیٹ_اشاریہ;
  hb_subset_depend_کنارہ_flags_t current_کنارہ_flags;

  hb_depend_ڈیٹا_t &ڈیٹا;
};


#اگر_ختم /* HB_DEPEND_DATA_HH */
