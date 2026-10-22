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

#جے_بنا_تعریف HB_DEPEND_انکڑے_HH
#تعریف_کرو HB_DEPEND_انکڑے_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#ضمن "hb.hh"

#ضمن "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#جے_بنا_تعریف HB_SUBSET_DEPEND_کنارا_FLAGS_T_DEFINED
#تعریف_کرو HB_SUBSET_DEPEND_کنارا_FLAGS_T_DEFINED
تعريفنوع تعداد {
  HB_SUBSET_DEPEND_کنارا_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_کنارا_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_کنارا_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_کنارا_flags_t;
#جے_انت
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_کنارا_flags_t);

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
بنية hb_depend_کنارا_t {
  hb_depend_کنارا_t() = مٹاؤ;
  hb_depend_کنارا_t(hb_ٹیگ_t جدول_ٹیگ_2,
                   hb_codepoint_t dependent,
                   hb_ٹیگ_t layout_ٹیگ,
                   hb_codepoint_t ligature_سیٹ_2,
                   hb_codepoint_t context_سیٹ,
                   hb_subset_depend_کنارا_flags_t flags = HB_SUBSET_DEPEND_کنارا_FLAG_NONE) : جدول_ٹیگ_2(جدول_ٹیگ_2),
     dependent(dependent), layout_ٹیگ(layout_ٹیگ),
     ligature_سیٹ_2(ligature_سیٹ_2), context_سیٹ(context_سیٹ),
     flags(flags)
     {}

  منطقي عامل == (ثابت hb_depend_کنارا_t &o) ثابت
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    أرجع جدول_ٹیگ_2 == o.جدول_ٹیگ_2 &&
           dependent == o.dependent &&
           layout_ٹیگ == o.layout_ٹیگ &&
           ligature_سیٹ_2 == o.ligature_سیٹ_2 &&
           context_سیٹ == o.context_سیٹ;
  }

  uint32_t hash () ثابت
  {
    /* FNV-1a hash of all identity fields (excludes flags) */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (جدول_ٹیگ_2);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ hb_hash (dependent);
    current = current * 16777619;
    current = current ^ hb_hash (layout_ٹیگ);
    current = current * 16777619;
    current = current ^ hb_hash (ligature_سیٹ_2);
    current = current * 16777619;
    current = current ^ hb_hash (context_سیٹ);
    current = current * 16777619;
    أرجع current;
  }

  hb_ٹیگ_t جدول_ٹیگ_2;
  hb_codepoint_t dependent;
  hb_ٹیگ_t layout_ٹیگ;
  hb_codepoint_t ligature_سیٹ_2;
  hb_codepoint_t context_سیٹ;
  hb_subset_depend_کنارا_flags_t flags;
};

/**
 * hb_depend_edge_key_t:
 *
 * Key structure for edge deduplication. Combines source glyph with edge record.
 * Uses the record's equality and hash operators for comparison.
 */
بنية hb_depend_کنارا_کلید_t {
  hb_codepoint_t سروت;
  hb_depend_کنارا_t ریکارڈ;

  hb_depend_کنارا_کلید_t () : سروت (0), ریکارڈ (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_کنارا_FLAG_NONE) {}

  hb_depend_کنارا_کلید_t (hb_codepoint_t سروت,
              hb_ٹیگ_t جدول_ٹیگ_2,
              hb_ٹیگ_t layout_ٹیگ,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_سیٹ_2,
              hb_codepoint_t context_سیٹ)
    : سروت (سروت),
      ریکارڈ (جدول_ٹیگ_2, dependent, layout_ٹیگ, ligature_سیٹ_2, context_سیٹ, HB_SUBSET_DEPEND_کنارا_FLAG_NONE) {}

  منطقي عامل == (ثابت hb_depend_کنارا_کلید_t &o) ثابت
  {
    أرجع سروت == o.سروت && ریکارڈ == o.ریکارڈ;
  }

  uint32_t hash () ثابت
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (سروت);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ ریکارڈ.hash ();
    current = current * 16777619;
    أرجع current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
بنية hb_depend_glyph_ریکارڈ_t {
  hb_سمتیہ_t<hb_depend_کنارا_t> dependencies;
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
  واضح hb_depend_lookup_revmap_t (منطقي بھریا) : بھریا (بھریا) {}
  hb_depend_lookup_revmap_t (ثابت hb_depend_lookup_revmap_t &o) : بھریا(o.بھریا),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : بھریا(o.بھریا),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& عامل = (ثابت hb_depend_lookup_revmap_t &o)
  {
    بھریا = o.بھریا;
    fv_indexes = o.fv_indexes;
    أرجع *هذا;
  }

  منطقي بھریا = سچ;
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
بنية hb_depend_انکڑے_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_سمتیہ_t<hb::unique_ptr<hb_سیٹ_t>> sets;

  hb_سمتیہ_t<hb_depend_glyph_ریکارڈ_t> glyph_dependencies;

  ثابت hb_سیٹ_t *get_سیٹ_from_اشاریہ (hb_codepoint_t اشاریہ_2)
  {
    إذا (اشاریہ_2 < sets.لمائی)
      أرجع sets[اشاریہ_2].get ();
    أرجع خالی_سوچک;
  }

  بنانشانی صحيح get_glyph_اندراج_گنتی (hb_codepoint_t gid) ثابت
  {
    إذا (gid < glyph_dependencies.لمائی)
      أرجع glyph_dependencies[gid].dependencies.لمائی;
    أرجع 0;
  }

  منطقي get_glyph_اندراج (hb_codepoint_t gid, بنانشانی صحيح اشاریہ_2,
                        hb_ٹیگ_t *جدول_ٹیگ_2, hb_codepoint_t *dependent,
                        hb_ٹیگ_t *layout_ٹیگ, hb_codepoint_t *ligature_سیٹ_2,
                        hb_codepoint_t *context_سیٹ, uint8_t *flags)
  {
    إذا (gid < glyph_dependencies.لمائی &&
        اشاریہ_2 < glyph_dependencies[gid].dependencies.لمائی) {
      خودکار &d = glyph_dependencies[gid].dependencies[اشاریہ_2];
      *جدول_ٹیگ_2 = d.جدول_ٹیگ_2;
      *dependent = d.dependent;
      *layout_ٹیگ = d.layout_ٹیگ;
      *ligature_سیٹ_2 = d.ligature_سیٹ_2;
      *context_سیٹ = d.context_سیٹ;
      إذا (flags) *flags = d.flags;
      أرجع سچ;
    }
    أرجع خطأ;
  }

  فراغ print ()
  {
    لكل (بنانشانی i = 0; i < glyph_dependencies.لمائی; i++) {
      خودکار &gd = glyph_dependencies[i];
      إذا (!gd.dependencies.لمائی)
        استمر;
      لکھو ("GID %u:\n", i);
      لكل (خودکار &d : gd.dependencies) {
        إذا (d.جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB) {
          لکھو ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_ٹیگ), d.dependent);
          إذا (d.ligature_سیٹ_2 != HB_CODEPOINT_INVALID)
            لکھو ("  (ligature)");
        } وإلا {
          لکھو ("  %c%c%c%c -> %u", HB_UNTAG(d.جدول_ٹیگ_2), d.dependent);
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
بنية hb_depend_انکڑے_builder_t
{
  hb_depend_انکڑے_builder_t (hb_depend_انکڑے_t &انکڑے_2)
    : current_context_سیٹ_اشاریہ (HB_CODEPOINT_INVALID),
      current_کنارا_flags (HB_SUBSET_DEPEND_کنارا_FLAG_NONE),
      انکڑے (انکڑے_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ثابت hb_سیٹ_t *get_سیٹ_from_اشاریہ (hb_codepoint_t اشاریہ_2)
  { أرجع انکڑے.get_سیٹ_from_اشاریہ (اشاریہ_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  فراغ discard_سیٹ (hb_codepoint_t سیٹ_اشاریہ)
  {
    إذا (سیٹ_اشاریہ >= انکڑے.sets.لمائی)
    {
      DEBUG_پیغام (SUBSET, خالی_سوچک, "Attempting to discard invalid set %u (max is %u)",
                 سیٹ_اشاریہ, انکڑے.sets.لمائی - 1);
      أرجع;
    }
    سیٹ_منزل_اشاریہ.del (انکڑے.sets[سیٹ_اشاریہ].get ());
    انکڑے.sets[سیٹ_اشاریہ]->clear ();
    check_success (free_سیٹ_list.ڈالو_or_fail (سیٹ_اشاریہ));
  }

  hb_codepoint_t new_سیٹ (ثابت hb_سیٹ_t &سیٹ)
  {
    hb_codepoint_t سیٹ_اشاریہ;

    إذا (free_سیٹ_list.لمائی > 0)
    {
      سیٹ_اشاریہ = free_سیٹ_list.نکالو ();
      انکڑے.sets[سیٹ_اشاریہ]->سیٹ (سیٹ);
      إذا (unlikely (انکڑے.sets[سیٹ_اشاریہ]->وچ_error ()))
	أرجع fail_invalid ();
    }
    وإلا
    {
      سیٹ_اشاریہ = انکڑے.sets.لمائی;
      hb_سیٹ_t *new_سیٹ = hb_سیٹ_create ();
      إذا (unlikely (!new_سیٹ))
	أرجع fail_invalid ();
      new_سیٹ->سیٹ (سیٹ);
      إذا (unlikely (new_سیٹ->وچ_error ()))
      {
	hb_سیٹ_destroy (new_سیٹ);
	أرجع fail_invalid ();
      }
      إذا (unlikely (!انکڑے.sets.ڈالو_or_fail (hb::unique_ptr<hb_سیٹ_t> {new_سیٹ})))
	أرجع fail_invalid ();
    }

    أرجع سیٹ_اشاریہ;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t لبھو_or_create_سیٹ (ثابت hb_سیٹ_t &سیٹ, منطقي *created = خالی_سوچک)
  {
    hb_codepoint_t *existing_idx = خالی_سوچک;
    إذا (سیٹ_منزل_اشاریہ.has (&سیٹ, &existing_idx))
    {
      إذا (created) *created = خطأ;
      أرجع *existing_idx;
    }

    hb_codepoint_t new_idx = new_سیٹ (سیٹ);
    إذا (unlikely (new_idx == HB_CODEPOINT_INVALID))
      أرجع HB_CODEPOINT_INVALID;

    إذا (unlikely (!سیٹ_منزل_اشاریہ.سیٹ (انکڑے.sets[new_idx].get (), new_idx)))
      أرجع fail_invalid ();
    إذا (created) *created = سچ;
    أرجع new_idx;
  }

  hb_codepoint_t لبھو_or_create_context_سیٹ (ثابت hb_سیٹ_t &سیٹ)
  { أرجع لبھو_or_create_سیٹ (سیٹ); }

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
    إذا ((!backtrack_sets || backtrack_sets->لمائی == 0) &&
        (!lookahead_sets || lookahead_sets->لمائی == 0))
      أرجع HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_سیٹ_t سدھا_requirements;

    إذا (backtrack_sets)
    {
      لكل (ثابت خودکار &پٹھ_سیٹ : *backtrack_sets)
      {
        إذا (پٹھ_سیٹ.get_population () == 1)
          سدھا_requirements.جوڑو (پٹھ_سیٹ.get_min ());
      }
    }

    إذا (lookahead_sets)
    {
      لكل (ثابت خودکار &look_سیٹ : *lookahead_sets)
      {
        إذا (look_سیٹ.get_population () == 1)
          سدھا_requirements.جوڑو (look_سیٹ.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_سیٹ_t context_elements;

    إذا (backtrack_sets)
    {
      لكل (ثابت خودکار &پٹھ_سیٹ : *backtrack_sets)
      {
        إذا (پٹھ_سیٹ.get_population () > 1)
        {
          hb_سیٹ_t filtered_سیٹ;
          filtered_سیٹ.سیٹ (پٹھ_سیٹ);
          filtered_سیٹ.گھٹاؤ (سدھا_requirements);
          إذا (unlikely (filtered_سیٹ.وچ_error ()))
            أرجع fail_invalid ();

          إذا (!filtered_سیٹ.is_empty ())
          {
            hb_codepoint_t سیٹ_idx = لبھو_or_create_context_سیٹ (filtered_سیٹ);
            إذا (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
              أرجع HB_CODEPOINT_INVALID;
            context_elements.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);
          }
        }
      }
    }

    إذا (lookahead_sets)
    {
      لكل (ثابت خودکار &look_سیٹ : *lookahead_sets)
      {
        إذا (look_سیٹ.get_population () > 1)
        {
          hb_سیٹ_t filtered_سیٹ;
          filtered_سیٹ.سیٹ (look_سیٹ);
          filtered_سیٹ.گھٹاؤ (سدھا_requirements);
          إذا (unlikely (filtered_سیٹ.وچ_error ()))
            أرجع fail_invalid ();

          إذا (!filtered_سیٹ.is_empty ())
          {
            hb_codepoint_t سیٹ_idx = لبھو_or_create_context_سیٹ (filtered_سیٹ);
            إذا (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
              أرجع HB_CODEPOINT_INVALID;
            context_elements.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);
          }
        }
      }
    }

    context_elements.union_ (سدھا_requirements);
    إذا (unlikely (سدھا_requirements.وچ_error () ||
                  context_elements.وچ_error ()))
      أرجع fail_invalid ();

    إذا (context_elements.is_empty ())
      أرجع HB_CODEPOINT_INVALID;

    أرجع لبھو_or_create_context_سیٹ (context_elements);
  }

  ساكن uint64_t encode_کنارا_ref (hb_codepoint_t سروت, بنانشانی صحيح اشاریہ_2)
  { أرجع (uint64_t (سروت) + 1) << 32 | اشاریہ_2; }

  ساكن فراغ decode_کنارا_ref (uint64_t ref,
			       hb_codepoint_t *سروت,
			       بنانشانی صحيح *اشاریہ_2)
  {
    *سروت = (ref >> 32) - 1;
    *اشاریہ_2 = ref;
  }

  uint32_t کنارا_hash (hb_codepoint_t سروت, ثابت hb_depend_کنارا_t &ریکارڈ) ثابت
  {
    hb_depend_کنارا_کلید_t key (سروت, ریکارڈ.جدول_ٹیگ_2, ریکارڈ.layout_ٹیگ,
			      ریکارڈ.dependent, ریکارڈ.ligature_سیٹ_2,
			      ریکارڈ.context_سیٹ);
    أرجع key.hash ();
  }

  منطقي resize_کنارا_slots ()
  {
    بنانشانی صحيح new_ناپ = کنارا_slots.لمائی ? کنارا_slots.لمائی * 2 : 8;
    إذا (unlikely (new_ناپ < کنارا_slots.لمائی))
      أرجع fail ();

    hb_سمتیہ_t<uint64_t> new_slots;
    إذا (unlikely (!new_slots.resize_exact (new_ناپ)))
      أرجع fail ();

    لكل (uint64_t ref : کنارا_slots)
    {
      إذا (!ref)
	استمر;

      hb_codepoint_t سروت;
      بنانشانی صحيح اشاریہ_2;
      decode_کنارا_ref (ref, &سروت, &اشاریہ_2);
      ثابت hb_depend_کنارا_t &ریکارڈ = انکڑے.glyph_dependencies[سروت].dependencies[اشاریہ_2];
      بنانشانی صحيح تھاں = کنارا_hash (سروت, ریکارڈ) & (new_ناپ - 1);
      بنانشانی صحيح قدم = 0;
      طالما (new_slots[تھاں])
	تھاں = (تھاں + ++قدم) & (new_ناپ - 1);
      new_slots[تھاں] = ref;
    }

    کنارا_slots = std::move (new_slots);
    أرجع سچ;
  }

  منطقي جوڑو_کنارا (hb_codepoint_t سروت, ثابت hb_depend_کنارا_t &ریکارڈ)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    إذا (unlikely (!کنارا_slots.لمائی ||
		  uint64_t (کنارا_population) * 3 / 2 >= کنارا_slots.لمائی - 1))
      إذا (unlikely (!resize_کنارا_slots ()))
	أرجع خطأ;

    بنانشانی صحيح تھاں = کنارا_hash (سروت, ریکارڈ) & (کنارا_slots.لمائی - 1);
    بنانشانی صحيح قدم = 0;
    طالما (uint64_t ref = کنارا_slots[تھاں])
    {
      hb_codepoint_t existing_سروت;
      بنانشانی صحيح existing_اشاریہ;
      decode_کنارا_ref (ref, &existing_سروت, &existing_اشاریہ);
      إذا (existing_سروت == سروت &&
	  انکڑے.glyph_dependencies[سروت].dependencies[existing_اشاریہ] == ریکارڈ)
	أرجع خطأ;
      تھاں = (تھاں + ++قدم) & (کنارا_slots.لمائی - 1);
    }

    خودکار &dependencies = انکڑے.glyph_dependencies[سروت].dependencies;
    بنانشانی صحيح اشاریہ_2 = dependencies.لمائی;
    إذا (unlikely (!dependencies.ڈالو_or_fail (ریکارڈ)))
      أرجع fail ();

    کنارا_slots[تھاں] = encode_کنارا_ref (سروت, اشاریہ_2);
    کنارا_population++;
    أرجع سچ;
  }

  منطقي جوڑو_depend_layout (hb_codepoint_t target, hb_ٹیگ_t جدول_ٹیگ_2,
                          hb_ٹیگ_t layout_ٹیگ,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_سیٹ = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_سیٹ = HB_CODEPOINT_INVALID,
                          hb_subset_depend_کنارا_flags_t flags = HB_SUBSET_DEPEND_کنارا_FLAG_NONE)
  {
    إذا (target >= انکڑے.glyph_dependencies.لمائی) {
      DEBUG_پیغام (SUBSET, خالی_سوچک, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(جدول_ٹیگ_2));
      أرجع خطأ;
    }

    أرجع جوڑو_کنارا (target, hb_depend_کنارا_t (جدول_ٹیگ_2, dependent, layout_ٹیگ,
					      lig_سیٹ, context_سیٹ, flags));
  }

  منطقي جوڑو_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_اشاریہ,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_سیٹ = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_سیٹ = HB_CODEPOINT_INVALID)
  {
    إذا (context_سیٹ == HB_CODEPOINT_INVALID)
      context_سیٹ = current_context_سیٹ_اشاریہ;
    hb_subset_depend_کنارا_flags_t flags = current_کنارا_flags;

    منطقي any_added = خطأ;
    لكل (uint64_t اندراج : get_lookup_features (lookup_اشاریہ)) {
      hb_ٹیگ_t t = (hb_ٹیگ_t) اندراج;
      إذا (جوڑو_depend_layout (target, HB_OT_ٹیگ_GSUB, t, dependent, lig_سیٹ, context_سیٹ, flags))
        any_added = سچ;
    }
    أرجع any_added;
  }

  منطقي init_lookup_features (بنانشانی lookup_گنتی)
  {
    أرجع check_success (lookup_feature_offsets.resize (lookup_گنتی + 1));
  }

  منطقي جوڑو_lookup_feature (hb_codepoint_t lookup_اشاریہ, hb_ٹیگ_t feature_ٹیگ)
  {
    إذا (feature_ٹیگ == HB_سیٹ_قدر_INVALID)
      أرجع سچ;

    إذا (unlikely (!lookup_feature_offsets.لمائی ||
		  lookup_اشاریہ >= lookup_feature_offsets.لمائی - 1))
      أرجع fail ();

    uint64_t اندراج = ((uint64_t) lookup_اشاریہ << 32) | feature_ٹیگ;
    أرجع check_success (lookup_features.ڈالو_or_fail (اندراج));
  }

  منطقي انت_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      أرجع a < b ? -1 : a > b ? 1 : 0;
    });

    بنانشانی اكتب = 0;
    لكل (uint64_t اندراج : lookup_features)
      إذا (!اكتب || اندراج != lookup_features[اكتب - 1])
	lookup_features[اكتب++] = اندراج;
    lookup_features.shrink (اكتب, خطأ);

    بنانشانی feature_اشاریہ = 0;
    بنانشانی lookup_گنتی = lookup_feature_offsets.لمائی - 1;
    لكل (بنانشانی lookup_اشاریہ = 0; lookup_اشاریہ < lookup_گنتی; lookup_اشاریہ++)
    {
      lookup_feature_offsets[lookup_اشاریہ] = feature_اشاریہ;
      طالما (feature_اشاریہ < lookup_features.لمائی &&
	     lookup_features[feature_اشاریہ] >> 32 == lookup_اشاریہ)
	feature_اشاریہ++;
    }
    lookup_feature_offsets[lookup_گنتی] = feature_اشاریہ;
    أرجع سچ;
  }

  hb_array_t<ثابت uint64_t> get_lookup_features (hb_codepoint_t lookup_اشاریہ) ثابت
  {
    إذا (unlikely (!lookup_feature_offsets.لمائی ||
		  lookup_اشاریہ >= lookup_feature_offsets.لمائی - 1))
      أرجع {};

    بنانشانی شروع = lookup_feature_offsets[lookup_اشاریہ];
    بنانشانی سرا = lookup_feature_offsets[lookup_اشاریہ + 1];
    أرجع lookup_features.as_array ().sub_array (شروع, سرا - شروع);
  }

  فراغ جوڑو_depend (hb_codepoint_t target, hb_ٹیگ_t جدول_ٹیگ_2,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_سیٹ = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_سیٹ = HB_CODEPOINT_INVALID,
                   hb_subset_depend_کنارا_flags_t flags = HB_SUBSET_DEPEND_کنارا_FLAG_NONE)
  {
    جوڑو_depend_layout (target, جدول_ٹیگ_2, HB_CODEPOINT_INVALID, dependent, lig_سیٹ,
                       context_سیٹ, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { أرجع hb_map_get (&nominal_glyphs, cp); }

  HB_اندرونی منطقي compile (hb_face_t *face);

  منطقي fail () { successful = خطأ; أرجع خطأ; }
  hb_codepoint_t fail_invalid () { fail (); أرجع HB_CODEPOINT_INVALID; }
  منطقي check_success (منطقي s) { successful = (successful && s); أرجع successful; }

  HB_اندرونی فراغ get_gsub_dependencies (hb_face_t *face);

  منطقي successful = سچ;
  hb_سیٹ_t unicodes;
  hb_map_t nominal_glyphs;
  hb_سمتیہ_t<uint64_t> lookup_features;
  hb_سمتیہ_t<بنانشانی> lookup_feature_offsets;
  hb_سمتیہ_t<uint64_t> کنارا_slots;
  بنانشانی صحيح کنارا_population = 0;
  hb_hashmap_t<ثابت hb_سیٹ_t*, hb_codepoint_t> سیٹ_منزل_اشاریہ;
  hb_سمتیہ_t<hb_codepoint_t> free_سیٹ_list;
  hb_codepoint_t current_context_سیٹ_اشاریہ;
  hb_subset_depend_کنارا_flags_t current_کنارا_flags;

  hb_depend_انکڑے_t &انکڑے;
};


#جے_انت /* HB_DEPEND_DATA_HH */
