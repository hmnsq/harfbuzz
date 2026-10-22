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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEPEND_ਅੰਕੜੇ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEPEND_ਅੰਕੜੇ_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAGS_T_DEFINED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAGS_T_DEFINED
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ {
  HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_ਕਿਨਾਰਾ_flags_t;
#ਜੇ_ਅੰਤ
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_ਕਿਨਾਰਾ_flags_t);

/* High bit of a context-set element marks it as an index into the sets array
 * rather than a raw glyph ID. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG 0x80000000u

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
ਬਣਤਰ hb_depend_ਕਿਨਾਰਾ_t {
  hb_depend_ਕਿਨਾਰਾ_t() = ਮਿਟਾਓ;
  hb_depend_ਕਿਨਾਰਾ_t(hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
                   hb_codepoint_t dependent,
                   hb_ਟੈਗ_t layout_ਟੈਗ,
                   hb_codepoint_t ligature_ਸੈੱਟ_2,
                   hb_codepoint_t context_ਸੈੱਟ,
                   hb_subset_depend_ਕਿਨਾਰਾ_flags_t flags = HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_NONE) : ਸਾਰਣੀ_ਟੈਗ_2(ਸਾਰਣੀ_ਟੈਗ_2),
     dependent(dependent), layout_ਟੈਗ(layout_ਟੈਗ),
     ligature_ਸੈੱਟ_2(ligature_ਸੈੱਟ_2), context_ਸੈੱਟ(context_ਸੈੱਟ),
     flags(flags)
     {}

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_depend_ਕਿਨਾਰਾ_t &o) ਅਚਲ
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    ਵਾਪਸ ਸਾਰਣੀ_ਟੈਗ_2 == o.ਸਾਰਣੀ_ਟੈਗ_2 &&
           dependent == o.dependent &&
           layout_ਟੈਗ == o.layout_ਟੈਗ &&
           ligature_ਸੈੱਟ_2 == o.ligature_ਸੈੱਟ_2 &&
           context_ਸੈੱਟ == o.context_ਸੈੱਟ;
  }

  uint32_t hash () ਅਚਲ
  {
    /* FNV-1a hash of all identity fields (excludes flags) */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (ਸਾਰਣੀ_ਟੈਗ_2);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ hb_hash (dependent);
    current = current * 16777619;
    current = current ^ hb_hash (layout_ਟੈਗ);
    current = current * 16777619;
    current = current ^ hb_hash (ligature_ਸੈੱਟ_2);
    current = current * 16777619;
    current = current ^ hb_hash (context_ਸੈੱਟ);
    current = current * 16777619;
    ਵਾਪਸ current;
  }

  hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2;
  hb_codepoint_t dependent;
  hb_ਟੈਗ_t layout_ਟੈਗ;
  hb_codepoint_t ligature_ਸੈੱਟ_2;
  hb_codepoint_t context_ਸੈੱਟ;
  hb_subset_depend_ਕਿਨਾਰਾ_flags_t flags;
};

/**
 * hb_depend_edge_key_t:
 *
 * Key structure for edge deduplication. Combines source glyph with edge record.
 * Uses the record's equality and hash operators for comparison.
 */
ਬਣਤਰ hb_depend_ਕਿਨਾਰਾ_ਕੁੰਜੀ_t {
  hb_codepoint_t ਸਰੋਤ;
  hb_depend_ਕਿਨਾਰਾ_t ਰਿਕਾਰਡ;

  hb_depend_ਕਿਨਾਰਾ_ਕੁੰਜੀ_t () : ਸਰੋਤ (0), ਰਿਕਾਰਡ (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_NONE) {}

  hb_depend_ਕਿਨਾਰਾ_ਕੁੰਜੀ_t (hb_codepoint_t ਸਰੋਤ,
              hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
              hb_ਟੈਗ_t layout_ਟੈਗ,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_ਸੈੱਟ_2,
              hb_codepoint_t context_ਸੈੱਟ)
    : ਸਰੋਤ (ਸਰੋਤ),
      ਰਿਕਾਰਡ (ਸਾਰਣੀ_ਟੈਗ_2, dependent, layout_ਟੈਗ, ligature_ਸੈੱਟ_2, context_ਸੈੱਟ, HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_NONE) {}

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_depend_ਕਿਨਾਰਾ_ਕੁੰਜੀ_t &o) ਅਚਲ
  {
    ਵਾਪਸ ਸਰੋਤ == o.ਸਰੋਤ && ਰਿਕਾਰਡ == o.ਰਿਕਾਰਡ;
  }

  uint32_t hash () ਅਚਲ
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (ਸਰੋਤ);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ ਰਿਕਾਰਡ.hash ();
    current = current * 16777619;
    ਵਾਪਸ current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
ਬਣਤਰ hb_depend_glyph_ਰਿਕਾਰਡ_t {
  hb_ਸਦਿਸ਼_t<hb_depend_ਕਿਨਾਰਾ_t> dependencies;
};

/**
 * hb_depend_lookup_revmap_t:
 *
 * Internal structure tracking which features are associated with a lookup.
 * Used during GSUB dependency analysis to record the feature tags that
 * activate each lookup.
 */
ਬਣਤਰ hb_depend_lookup_revmap_t
{
  hb_depend_lookup_revmap_t () = ਮੂਲ;
  ਸਪਸ਼ਟ hb_depend_lookup_revmap_t (ਤਾਰਕਿਕ ਭਰਿਆ) : ਭਰਿਆ (ਭਰਿਆ) {}
  hb_depend_lookup_revmap_t (ਅਚਲ hb_depend_lookup_revmap_t &o) : ਭਰਿਆ(o.ਭਰਿਆ),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : ਭਰਿਆ(o.ਭਰਿਆ),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_depend_lookup_revmap_t &o)
  {
    ਭਰਿਆ = o.ਭਰਿਆ;
    fv_indexes = o.fv_indexes;
    ਵਾਪਸ *ਇਹ;
  }

  ਤਾਰਕਿਕ ਭਰਿਆ = ਸੱਚ;
  hb_ਸੈੱਟ_t fv_indexes;
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
ਬਣਤਰ hb_depend_ਅੰਕੜੇ_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_ਸਦਿਸ਼_t<hb::unique_ptr<hb_ਸੈੱਟ_t>> sets;

  hb_ਸਦਿਸ਼_t<hb_depend_glyph_ਰਿਕਾਰਡ_t> glyph_dependencies;

  ਅਚਲ hb_ਸੈੱਟ_t *get_ਸੈੱਟ_from_ਸੂਚਕਾਂਕ (hb_codepoint_t ਸੂਚਕਾਂਕ_2)
  {
    ਜੇ (ਸੂਚਕਾਂਕ_2 < sets.ਲੰਮਾਈ)
      ਵਾਪਸ sets[ਸੂਚਕਾਂਕ_2].get ();
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_glyph_ਇੰਦਰਾਜ_ਗਿਣਤੀ (hb_codepoint_t gid) ਅਚਲ
  {
    ਜੇ (gid < glyph_dependencies.ਲੰਮਾਈ)
      ਵਾਪਸ glyph_dependencies[gid].dependencies.ਲੰਮਾਈ;
    ਵਾਪਸ 0;
  }

  ਤਾਰਕਿਕ get_glyph_ਇੰਦਰਾਜ (hb_codepoint_t gid, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2,
                        hb_ਟੈਗ_t *ਸਾਰਣੀ_ਟੈਗ_2, hb_codepoint_t *dependent,
                        hb_ਟੈਗ_t *layout_ਟੈਗ, hb_codepoint_t *ligature_ਸੈੱਟ_2,
                        hb_codepoint_t *context_ਸੈੱਟ, uint8_t *flags)
  {
    ਜੇ (gid < glyph_dependencies.ਲੰਮਾਈ &&
        ਸੂਚਕਾਂਕ_2 < glyph_dependencies[gid].dependencies.ਲੰਮਾਈ) {
      ਸਵੈਚਾਲਿਤ &d = glyph_dependencies[gid].dependencies[ਸੂਚਕਾਂਕ_2];
      *ਸਾਰਣੀ_ਟੈਗ_2 = d.ਸਾਰਣੀ_ਟੈਗ_2;
      *dependent = d.dependent;
      *layout_ਟੈਗ = d.layout_ਟੈਗ;
      *ligature_ਸੈੱਟ_2 = d.ligature_ਸੈੱਟ_2;
      *context_ਸੈੱਟ = d.context_ਸੈੱਟ;
      ਜੇ (flags) *flags = d.flags;
      ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
  }

  ਖਾਲੀ print ()
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < glyph_dependencies.ਲੰਮਾਈ; i++) {
      ਸਵੈਚਾਲਿਤ &gd = glyph_dependencies[i];
      ਜੇ (!gd.dependencies.ਲੰਮਾਈ)
        ਜਾਰੀ;
      ਲਿਖੋ ("GID %u:\n", i);
      ਲਈ (ਸਵੈਚਾਲਿਤ &d : gd.dependencies) {
        ਜੇ (d.ਸਾਰਣੀ_ਟੈਗ_2 == HB_OT_ਟੈਗ_GSUB) {
          ਲਿਖੋ ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_ਟੈਗ), d.dependent);
          ਜੇ (d.ligature_ਸੈੱਟ_2 != HB_CODEPOINT_INVALID)
            ਲਿਖੋ ("  (ligature)");
        } ਨਹੀਂਤਾਂ {
          ਲਿਖੋ ("  %c%c%c%c -> %u", HB_UNTAG(d.ਸਾਰਣੀ_ਟੈਗ_2), d.dependent);
        }
        ਲਿਖੋ ("\n");
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
ਬਣਤਰ hb_depend_ਅੰਕੜੇ_builder_t
{
  hb_depend_ਅੰਕੜੇ_builder_t (hb_depend_ਅੰਕੜੇ_t &ਅੰਕੜੇ_2)
    : current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ (HB_CODEPOINT_INVALID),
      current_ਕਿਨਾਰਾ_flags (HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_NONE),
      ਅੰਕੜੇ (ਅੰਕੜੇ_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ਅਚਲ hb_ਸੈੱਟ_t *get_ਸੈੱਟ_from_ਸੂਚਕਾਂਕ (hb_codepoint_t ਸੂਚਕਾਂਕ_2)
  { ਵਾਪਸ ਅੰਕੜੇ.get_ਸੈੱਟ_from_ਸੂਚਕਾਂਕ (ਸੂਚਕਾਂਕ_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  ਖਾਲੀ discard_ਸੈੱਟ (hb_codepoint_t ਸੈੱਟ_ਸੂਚਕਾਂਕ)
  {
    ਜੇ (ਸੈੱਟ_ਸੂਚਕਾਂਕ >= ਅੰਕੜੇ.sets.ਲੰਮਾਈ)
    {
      DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "Attempting to discard invalid set %u (max is %u)",
                 ਸੈੱਟ_ਸੂਚਕਾਂਕ, ਅੰਕੜੇ.sets.ਲੰਮਾਈ - 1);
      ਵਾਪਸ;
    }
    ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.del (ਅੰਕੜੇ.sets[ਸੈੱਟ_ਸੂਚਕਾਂਕ].get ());
    ਅੰਕੜੇ.sets[ਸੈੱਟ_ਸੂਚਕਾਂਕ]->clear ();
    check_success (free_ਸੈੱਟ_list.ਧੱਕੋ_or_fail (ਸੈੱਟ_ਸੂਚਕਾਂਕ));
  }

  hb_codepoint_t new_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t &ਸੈੱਟ)
  {
    hb_codepoint_t ਸੈੱਟ_ਸੂਚਕਾਂਕ;

    ਜੇ (free_ਸੈੱਟ_list.ਲੰਮਾਈ > 0)
    {
      ਸੈੱਟ_ਸੂਚਕਾਂਕ = free_ਸੈੱਟ_list.ਕੱਢੋ ();
      ਅੰਕੜੇ.sets[ਸੈੱਟ_ਸੂਚਕਾਂਕ]->ਸੈੱਟ (ਸੈੱਟ);
      ਜੇ (unlikely (ਅੰਕੜੇ.sets[ਸੈੱਟ_ਸੂਚਕਾਂਕ]->ਵਿੱਚ_error ()))
	ਵਾਪਸ fail_invalid ();
    }
    ਨਹੀਂਤਾਂ
    {
      ਸੈੱਟ_ਸੂਚਕਾਂਕ = ਅੰਕੜੇ.sets.ਲੰਮਾਈ;
      hb_ਸੈੱਟ_t *new_ਸੈੱਟ = hb_ਸੈੱਟ_create ();
      ਜੇ (unlikely (!new_ਸੈੱਟ))
	ਵਾਪਸ fail_invalid ();
      new_ਸੈੱਟ->ਸੈੱਟ (ਸੈੱਟ);
      ਜੇ (unlikely (new_ਸੈੱਟ->ਵਿੱਚ_error ()))
      {
	hb_ਸੈੱਟ_destroy (new_ਸੈੱਟ);
	ਵਾਪਸ fail_invalid ();
      }
      ਜੇ (unlikely (!ਅੰਕੜੇ.sets.ਧੱਕੋ_or_fail (hb::unique_ptr<hb_ਸੈੱਟ_t> {new_ਸੈੱਟ})))
	ਵਾਪਸ fail_invalid ();
    }

    ਵਾਪਸ ਸੈੱਟ_ਸੂਚਕਾਂਕ;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t ਲੱਭੋ_or_create_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t &ਸੈੱਟ, ਤਾਰਕਿਕ *created = ਖਾਲੀ_ਸੂਚਕ)
  {
    hb_codepoint_t *existing_idx = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.has (&ਸੈੱਟ, &existing_idx))
    {
      ਜੇ (created) *created = ਝੂਠ;
      ਵਾਪਸ *existing_idx;
    }

    hb_codepoint_t new_idx = new_ਸੈੱਟ (ਸੈੱਟ);
    ਜੇ (unlikely (new_idx == HB_CODEPOINT_INVALID))
      ਵਾਪਸ HB_CODEPOINT_INVALID;

    ਜੇ (unlikely (!ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ.ਸੈੱਟ (ਅੰਕੜੇ.sets[new_idx].get (), new_idx)))
      ਵਾਪਸ fail_invalid ();
    ਜੇ (created) *created = ਸੱਚ;
    ਵਾਪਸ new_idx;
  }

  hb_codepoint_t ਲੱਭੋ_or_create_context_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t &ਸੈੱਟ)
  { ਵਾਪਸ ਲੱਭੋ_or_create_ਸੈੱਟ (ਸੈੱਟ); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_ਸੈੱਟ (ਅਚਲ hb_ਸਦਿਸ਼_t<hb_ਸੈੱਟ_t> *backtrack_sets,
                                     ਅਚਲ hb_ਸਦਿਸ਼_t<hb_ਸੈੱਟ_t> *lookahead_sets)
  {
    ਜੇ ((!backtrack_sets || backtrack_sets->ਲੰਮਾਈ == 0) &&
        (!lookahead_sets || lookahead_sets->ਲੰਮਾਈ == 0))
      ਵਾਪਸ HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_ਸੈੱਟ_t ਸਿੱਧਾ_requirements;

    ਜੇ (backtrack_sets)
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਪਿੱਠ_ਸੈੱਟ : *backtrack_sets)
      {
        ਜੇ (ਪਿੱਠ_ਸੈੱਟ.get_population () == 1)
          ਸਿੱਧਾ_requirements.ਜੋੜੋ (ਪਿੱਠ_ਸੈੱਟ.get_min ());
      }
    }

    ਜੇ (lookahead_sets)
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &look_ਸੈੱਟ : *lookahead_sets)
      {
        ਜੇ (look_ਸੈੱਟ.get_population () == 1)
          ਸਿੱਧਾ_requirements.ਜੋੜੋ (look_ਸੈੱਟ.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_ਸੈੱਟ_t context_elements;

    ਜੇ (backtrack_sets)
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &ਪਿੱਠ_ਸੈੱਟ : *backtrack_sets)
      {
        ਜੇ (ਪਿੱਠ_ਸੈੱਟ.get_population () > 1)
        {
          hb_ਸੈੱਟ_t filtered_ਸੈੱਟ;
          filtered_ਸੈੱਟ.ਸੈੱਟ (ਪਿੱਠ_ਸੈੱਟ);
          filtered_ਸੈੱਟ.ਘਟਾਓ (ਸਿੱਧਾ_requirements);
          ਜੇ (unlikely (filtered_ਸੈੱਟ.ਵਿੱਚ_error ()))
            ਵਾਪਸ fail_invalid ();

          ਜੇ (!filtered_ਸੈੱਟ.is_empty ())
          {
            hb_codepoint_t ਸੈੱਟ_idx = ਲੱਭੋ_or_create_context_ਸੈੱਟ (filtered_ਸੈੱਟ);
            ਜੇ (unlikely (ਸੈੱਟ_idx == HB_CODEPOINT_INVALID))
              ਵਾਪਸ HB_CODEPOINT_INVALID;
            context_elements.ਜੋੜੋ (HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG | ਸੈੱਟ_idx);
          }
        }
      }
    }

    ਜੇ (lookahead_sets)
    {
      ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &look_ਸੈੱਟ : *lookahead_sets)
      {
        ਜੇ (look_ਸੈੱਟ.get_population () > 1)
        {
          hb_ਸੈੱਟ_t filtered_ਸੈੱਟ;
          filtered_ਸੈੱਟ.ਸੈੱਟ (look_ਸੈੱਟ);
          filtered_ਸੈੱਟ.ਘਟਾਓ (ਸਿੱਧਾ_requirements);
          ਜੇ (unlikely (filtered_ਸੈੱਟ.ਵਿੱਚ_error ()))
            ਵਾਪਸ fail_invalid ();

          ਜੇ (!filtered_ਸੈੱਟ.is_empty ())
          {
            hb_codepoint_t ਸੈੱਟ_idx = ਲੱਭੋ_or_create_context_ਸੈੱਟ (filtered_ਸੈੱਟ);
            ਜੇ (unlikely (ਸੈੱਟ_idx == HB_CODEPOINT_INVALID))
              ਵਾਪਸ HB_CODEPOINT_INVALID;
            context_elements.ਜੋੜੋ (HB_DEPEND_CONTEXT_ਸੈੱਟ_FLAG | ਸੈੱਟ_idx);
          }
        }
      }
    }

    context_elements.union_ (ਸਿੱਧਾ_requirements);
    ਜੇ (unlikely (ਸਿੱਧਾ_requirements.ਵਿੱਚ_error () ||
                  context_elements.ਵਿੱਚ_error ()))
      ਵਾਪਸ fail_invalid ();

    ਜੇ (context_elements.is_empty ())
      ਵਾਪਸ HB_CODEPOINT_INVALID;

    ਵਾਪਸ ਲੱਭੋ_or_create_context_ਸੈੱਟ (context_elements);
  }

  ਸਥਿਰ uint64_t encode_ਕਿਨਾਰਾ_ref (hb_codepoint_t ਸਰੋਤ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2)
  { ਵਾਪਸ (uint64_t (ਸਰੋਤ) + 1) << 32 | ਸੂਚਕਾਂਕ_2; }

  ਸਥਿਰ ਖਾਲੀ decode_ਕਿਨਾਰਾ_ref (uint64_t ref,
			       hb_codepoint_t *ਸਰੋਤ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸੂਚਕਾਂਕ_2)
  {
    *ਸਰੋਤ = (ref >> 32) - 1;
    *ਸੂਚਕਾਂਕ_2 = ref;
  }

  uint32_t ਕਿਨਾਰਾ_hash (hb_codepoint_t ਸਰੋਤ, ਅਚਲ hb_depend_ਕਿਨਾਰਾ_t &ਰਿਕਾਰਡ) ਅਚਲ
  {
    hb_depend_ਕਿਨਾਰਾ_ਕੁੰਜੀ_t key (ਸਰੋਤ, ਰਿਕਾਰਡ.ਸਾਰਣੀ_ਟੈਗ_2, ਰਿਕਾਰਡ.layout_ਟੈਗ,
			      ਰਿਕਾਰਡ.dependent, ਰਿਕਾਰਡ.ligature_ਸੈੱਟ_2,
			      ਰਿਕਾਰਡ.context_ਸੈੱਟ);
    ਵਾਪਸ key.hash ();
  }

  ਤਾਰਕਿਕ resize_ਕਿਨਾਰਾ_slots ()
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_ਮਾਪ = ਕਿਨਾਰਾ_slots.ਲੰਮਾਈ ? ਕਿਨਾਰਾ_slots.ਲੰਮਾਈ * 2 : 8;
    ਜੇ (unlikely (new_ਮਾਪ < ਕਿਨਾਰਾ_slots.ਲੰਮਾਈ))
      ਵਾਪਸ fail ();

    hb_ਸਦਿਸ਼_t<uint64_t> new_slots;
    ਜੇ (unlikely (!new_slots.resize_exact (new_ਮਾਪ)))
      ਵਾਪਸ fail ();

    ਲਈ (uint64_t ref : ਕਿਨਾਰਾ_slots)
    {
      ਜੇ (!ref)
	ਜਾਰੀ;

      hb_codepoint_t ਸਰੋਤ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2;
      decode_ਕਿਨਾਰਾ_ref (ref, &ਸਰੋਤ, &ਸੂਚਕਾਂਕ_2);
      ਅਚਲ hb_depend_ਕਿਨਾਰਾ_t &ਰਿਕਾਰਡ = ਅੰਕੜੇ.glyph_dependencies[ਸਰੋਤ].dependencies[ਸੂਚਕਾਂਕ_2];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਥਾਂ = ਕਿਨਾਰਾ_hash (ਸਰੋਤ, ਰਿਕਾਰਡ) & (new_ਮਾਪ - 1);
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕਦਮ = 0;
      ਜਦੋਂ (new_slots[ਥਾਂ])
	ਥਾਂ = (ਥਾਂ + ++ਕਦਮ) & (new_ਮਾਪ - 1);
      new_slots[ਥਾਂ] = ref;
    }

    ਕਿਨਾਰਾ_slots = std::move (new_slots);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ ਜੋੜੋ_ਕਿਨਾਰਾ (hb_codepoint_t ਸਰੋਤ, ਅਚਲ hb_depend_ਕਿਨਾਰਾ_t &ਰਿਕਾਰਡ)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    ਜੇ (unlikely (!ਕਿਨਾਰਾ_slots.ਲੰਮਾਈ ||
		  uint64_t (ਕਿਨਾਰਾ_population) * 3 / 2 >= ਕਿਨਾਰਾ_slots.ਲੰਮਾਈ - 1))
      ਜੇ (unlikely (!resize_ਕਿਨਾਰਾ_slots ()))
	ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਥਾਂ = ਕਿਨਾਰਾ_hash (ਸਰੋਤ, ਰਿਕਾਰਡ) & (ਕਿਨਾਰਾ_slots.ਲੰਮਾਈ - 1);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕਦਮ = 0;
    ਜਦੋਂ (uint64_t ref = ਕਿਨਾਰਾ_slots[ਥਾਂ])
    {
      hb_codepoint_t existing_ਸਰੋਤ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ existing_ਸੂਚਕਾਂਕ;
      decode_ਕਿਨਾਰਾ_ref (ref, &existing_ਸਰੋਤ, &existing_ਸੂਚਕਾਂਕ);
      ਜੇ (existing_ਸਰੋਤ == ਸਰੋਤ &&
	  ਅੰਕੜੇ.glyph_dependencies[ਸਰੋਤ].dependencies[existing_ਸੂਚਕਾਂਕ] == ਰਿਕਾਰਡ)
	ਵਾਪਸ ਝੂਠ;
      ਥਾਂ = (ਥਾਂ + ++ਕਦਮ) & (ਕਿਨਾਰਾ_slots.ਲੰਮਾਈ - 1);
    }

    ਸਵੈਚਾਲਿਤ &dependencies = ਅੰਕੜੇ.glyph_dependencies[ਸਰੋਤ].dependencies;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸੂਚਕਾਂਕ_2 = dependencies.ਲੰਮਾਈ;
    ਜੇ (unlikely (!dependencies.ਧੱਕੋ_or_fail (ਰਿਕਾਰਡ)))
      ਵਾਪਸ fail ();

    ਕਿਨਾਰਾ_slots[ਥਾਂ] = encode_ਕਿਨਾਰਾ_ref (ਸਰੋਤ, ਸੂਚਕਾਂਕ_2);
    ਕਿਨਾਰਾ_population++;
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ ਜੋੜੋ_depend_layout (hb_codepoint_t target, hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
                          hb_ਟੈਗ_t layout_ਟੈਗ,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_ਸੈੱਟ = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_ਸੈੱਟ = HB_CODEPOINT_INVALID,
                          hb_subset_depend_ਕਿਨਾਰਾ_flags_t flags = HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_NONE)
  {
    ਜੇ (target >= ਅੰਕੜੇ.glyph_dependencies.ਲੰਮਾਈ) {
      DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(ਸਾਰਣੀ_ਟੈਗ_2));
      ਵਾਪਸ ਝੂਠ;
    }

    ਵਾਪਸ ਜੋੜੋ_ਕਿਨਾਰਾ (target, hb_depend_ਕਿਨਾਰਾ_t (ਸਾਰਣੀ_ਟੈਗ_2, dependent, layout_ਟੈਗ,
					      lig_ਸੈੱਟ, context_ਸੈੱਟ, flags));
  }

  ਤਾਰਕਿਕ ਜੋੜੋ_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_ਸੂਚਕਾਂਕ,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_ਸੈੱਟ = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_ਸੈੱਟ = HB_CODEPOINT_INVALID)
  {
    ਜੇ (context_ਸੈੱਟ == HB_CODEPOINT_INVALID)
      context_ਸੈੱਟ = current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ;
    hb_subset_depend_ਕਿਨਾਰਾ_flags_t flags = current_ਕਿਨਾਰਾ_flags;

    ਤਾਰਕਿਕ any_added = ਝੂਠ;
    ਲਈ (uint64_t ਇੰਦਰਾਜ : get_lookup_features (lookup_ਸੂਚਕਾਂਕ)) {
      hb_ਟੈਗ_t t = (hb_ਟੈਗ_t) ਇੰਦਰਾਜ;
      ਜੇ (ਜੋੜੋ_depend_layout (target, HB_OT_ਟੈਗ_GSUB, t, dependent, lig_ਸੈੱਟ, context_ਸੈੱਟ, flags))
        any_added = ਸੱਚ;
    }
    ਵਾਪਸ any_added;
  }

  ਤਾਰਕਿਕ init_lookup_features (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਗਿਣਤੀ)
  {
    ਵਾਪਸ check_success (lookup_feature_offsets.resize (lookup_ਗਿਣਤੀ + 1));
  }

  ਤਾਰਕਿਕ ਜੋੜੋ_lookup_feature (hb_codepoint_t lookup_ਸੂਚਕਾਂਕ, hb_ਟੈਗ_t feature_ਟੈਗ)
  {
    ਜੇ (feature_ਟੈਗ == HB_ਸੈੱਟ_ਮੁੱਲ_INVALID)
      ਵਾਪਸ ਸੱਚ;

    ਜੇ (unlikely (!lookup_feature_offsets.ਲੰਮਾਈ ||
		  lookup_ਸੂਚਕਾਂਕ >= lookup_feature_offsets.ਲੰਮਾਈ - 1))
      ਵਾਪਸ fail ();

    uint64_t ਇੰਦਰਾਜ = ((uint64_t) lookup_ਸੂਚਕਾਂਕ << 32) | feature_ਟੈਗ;
    ਵਾਪਸ check_success (lookup_features.ਧੱਕੋ_or_fail (ਇੰਦਰਾਜ));
  }

  ਤਾਰਕਿਕ ਅੰਤ_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      ਵਾਪਸ a < b ? -1 : a > b ? 1 : 0;
    });

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲਿਖ = 0;
    ਲਈ (uint64_t ਇੰਦਰਾਜ : lookup_features)
      ਜੇ (!ਲਿਖ || ਇੰਦਰਾਜ != lookup_features[ਲਿਖ - 1])
	lookup_features[ਲਿਖ++] = ਇੰਦਰਾਜ;
    lookup_features.shrink (ਲਿਖ, ਝੂਠ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ feature_ਸੂਚਕਾਂਕ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਗਿਣਤੀ = lookup_feature_offsets.ਲੰਮਾਈ - 1;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ lookup_ਸੂਚਕਾਂਕ = 0; lookup_ਸੂਚਕਾਂਕ < lookup_ਗਿਣਤੀ; lookup_ਸੂਚਕਾਂਕ++)
    {
      lookup_feature_offsets[lookup_ਸੂਚਕਾਂਕ] = feature_ਸੂਚਕਾਂਕ;
      ਜਦੋਂ (feature_ਸੂਚਕਾਂਕ < lookup_features.ਲੰਮਾਈ &&
	     lookup_features[feature_ਸੂਚਕਾਂਕ] >> 32 == lookup_ਸੂਚਕਾਂਕ)
	feature_ਸੂਚਕਾਂਕ++;
    }
    lookup_feature_offsets[lookup_ਗਿਣਤੀ] = feature_ਸੂਚਕਾਂਕ;
    ਵਾਪਸ ਸੱਚ;
  }

  hb_array_t<ਅਚਲ uint64_t> get_lookup_features (hb_codepoint_t lookup_ਸੂਚਕਾਂਕ) ਅਚਲ
  {
    ਜੇ (unlikely (!lookup_feature_offsets.ਲੰਮਾਈ ||
		  lookup_ਸੂਚਕਾਂਕ >= lookup_feature_offsets.ਲੰਮਾਈ - 1))
      ਵਾਪਸ {};

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = lookup_feature_offsets[lookup_ਸੂਚਕਾਂਕ];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = lookup_feature_offsets[lookup_ਸੂਚਕਾਂਕ + 1];
    ਵਾਪਸ lookup_features.as_array ().sub_array (ਸ਼ੁਰੂ, ਸਿਰਾ - ਸ਼ੁਰੂ);
  }

  ਖਾਲੀ ਜੋੜੋ_depend (hb_codepoint_t target, hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_ਸੈੱਟ = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_ਸੈੱਟ = HB_CODEPOINT_INVALID,
                   hb_subset_depend_ਕਿਨਾਰਾ_flags_t flags = HB_SUBSET_DEPEND_ਕਿਨਾਰਾ_FLAG_NONE)
  {
    ਜੋੜੋ_depend_layout (target, ਸਾਰਣੀ_ਟੈਗ_2, HB_CODEPOINT_INVALID, dependent, lig_ਸੈੱਟ,
                       context_ਸੈੱਟ, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { ਵਾਪਸ hb_map_get (&nominal_glyphs, cp); }

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ compile (hb_face_t *face);

  ਤਾਰਕਿਕ fail () { successful = ਝੂਠ; ਵਾਪਸ ਝੂਠ; }
  hb_codepoint_t fail_invalid () { fail (); ਵਾਪਸ HB_CODEPOINT_INVALID; }
  ਤਾਰਕਿਕ check_success (ਤਾਰਕਿਕ s) { successful = (successful && s); ਵਾਪਸ successful; }

  HB_ਅੰਦਰੂਨੀ ਖਾਲੀ get_gsub_dependencies (hb_face_t *face);

  ਤਾਰਕਿਕ successful = ਸੱਚ;
  hb_ਸੈੱਟ_t unicodes;
  hb_map_t nominal_glyphs;
  hb_ਸਦਿਸ਼_t<uint64_t> lookup_features;
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> lookup_feature_offsets;
  hb_ਸਦਿਸ਼_t<uint64_t> ਕਿਨਾਰਾ_slots;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਕਿਨਾਰਾ_population = 0;
  hb_hashmap_t<ਅਚਲ hb_ਸੈੱਟ_t*, hb_codepoint_t> ਸੈੱਟ_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ;
  hb_ਸਦਿਸ਼_t<hb_codepoint_t> free_ਸੈੱਟ_list;
  hb_codepoint_t current_context_ਸੈੱਟ_ਸੂਚਕਾਂਕ;
  hb_subset_depend_ਕਿਨਾਰਾ_flags_t current_ਕਿਨਾਰਾ_flags;

  hb_depend_ਅੰਕੜੇ_t &ਅੰਕੜੇ;
};


#ਜੇ_ਅੰਤ /* HB_DEPEND_DATA_HH */
