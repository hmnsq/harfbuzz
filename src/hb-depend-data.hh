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

#నిర్వచితం_కాకపోతే HB_DEPEND_దత్తాంశం_HH
#నిర్వచించు HB_DEPEND_దత్తాంశం_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#నిర్వచితం_కాకపోతే HB_SUBSET_DEPEND_అంచు_FLAGS_T_DEFINED
#నిర్వచించు HB_SUBSET_DEPEND_అంచు_FLAGS_T_DEFINED
రకనిర్వచనం గణన {
  HB_SUBSET_DEPEND_అంచు_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_అంచు_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_అంచు_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_అంచు_flags_t;
#అయితే_ముగింపు
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_అంచు_flags_t);

/* High bit of a context-set element marks it as an index into the sets array
 * rather than a raw glyph ID. */
#నిర్వచించు HB_DEPEND_CONTEXT_సమితి_FLAG 0x80000000u

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
నిర్మాణం hb_depend_అంచు_t {
  hb_depend_అంచు_t() = తొలగించు;
  hb_depend_అంచు_t(hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
                   hb_codepoint_t dependent,
                   hb_ట్యాగ్_t layout_ట్యాగ్,
                   hb_codepoint_t ligature_సమితి_2,
                   hb_codepoint_t context_సమితి,
                   hb_subset_depend_అంచు_flags_t flags = HB_SUBSET_DEPEND_అంచు_FLAG_NONE) : పట్టిక_ట్యాగ్_2(పట్టిక_ట్యాగ్_2),
     dependent(dependent), layout_ట్యాగ్(layout_ట్యాగ్),
     ligature_సమితి_2(ligature_సమితి_2), context_సమితి(context_సమితి),
     flags(flags)
     {}

  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_depend_అంచు_t &o) స్థిరరాశి
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    తిరిగి పట్టిక_ట్యాగ్_2 == o.పట్టిక_ట్యాగ్_2 &&
           dependent == o.dependent &&
           layout_ట్యాగ్ == o.layout_ట్యాగ్ &&
           ligature_సమితి_2 == o.ligature_సమితి_2 &&
           context_సమితి == o.context_సమితి;
  }

  uint32_t hash () స్థిరరాశి
  {
    /* FNV-1a hash of all identity fields (excludes flags) */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (పట్టిక_ట్యాగ్_2);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ hb_hash (dependent);
    current = current * 16777619;
    current = current ^ hb_hash (layout_ట్యాగ్);
    current = current * 16777619;
    current = current ^ hb_hash (ligature_సమితి_2);
    current = current * 16777619;
    current = current ^ hb_hash (context_సమితి);
    current = current * 16777619;
    తిరిగి current;
  }

  hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2;
  hb_codepoint_t dependent;
  hb_ట్యాగ్_t layout_ట్యాగ్;
  hb_codepoint_t ligature_సమితి_2;
  hb_codepoint_t context_సమితి;
  hb_subset_depend_అంచు_flags_t flags;
};

/**
 * hb_depend_edge_key_t:
 *
 * Key structure for edge deduplication. Combines source glyph with edge record.
 * Uses the record's equality and hash operators for comparison.
 */
నిర్మాణం hb_depend_అంచు_కీలకం_t {
  hb_codepoint_t ఆధారం;
  hb_depend_అంచు_t రికార్డు;

  hb_depend_అంచు_కీలకం_t () : ఆధారం (0), రికార్డు (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_అంచు_FLAG_NONE) {}

  hb_depend_అంచు_కీలకం_t (hb_codepoint_t ఆధారం,
              hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
              hb_ట్యాగ్_t layout_ట్యాగ్,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_సమితి_2,
              hb_codepoint_t context_సమితి)
    : ఆధారం (ఆధారం),
      రికార్డు (పట్టిక_ట్యాగ్_2, dependent, layout_ట్యాగ్, ligature_సమితి_2, context_సమితి, HB_SUBSET_DEPEND_అంచు_FLAG_NONE) {}

  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_depend_అంచు_కీలకం_t &o) స్థిరరాశి
  {
    తిరిగి ఆధారం == o.ఆధారం && రికార్డు == o.రికార్డు;
  }

  uint32_t hash () స్థిరరాశి
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (ఆధారం);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ రికార్డు.hash ();
    current = current * 16777619;
    తిరిగి current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
నిర్మాణం hb_depend_glyph_రికార్డు_t {
  hb_సదిశ_t<hb_depend_అంచు_t> dependencies;
};

/**
 * hb_depend_lookup_revmap_t:
 *
 * Internal structure tracking which features are associated with a lookup.
 * Used during GSUB dependency analysis to record the feature tags that
 * activate each lookup.
 */
నిర్మాణం hb_depend_lookup_revmap_t
{
  hb_depend_lookup_revmap_t () = అప్రమేయ;
  స్పష్టమైన hb_depend_lookup_revmap_t (తార్కిక నిండిన) : నిండిన (నిండిన) {}
  hb_depend_lookup_revmap_t (స్థిరరాశి hb_depend_lookup_revmap_t &o) : నిండిన(o.నిండిన),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : నిండిన(o.నిండిన),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& క్రియాకారకం = (స్థిరరాశి hb_depend_lookup_revmap_t &o)
  {
    నిండిన = o.నిండిన;
    fv_indexes = o.fv_indexes;
    తిరిగి *ఇది;
  }

  తార్కిక నిండిన = నిజం;
  hb_సమితి_t fv_indexes;
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
నిర్మాణం hb_depend_దత్తాంశం_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_సదిశ_t<hb::unique_ptr<hb_సమితి_t>> sets;

  hb_సదిశ_t<hb_depend_glyph_రికార్డు_t> glyph_dependencies;

  స్థిరరాశి hb_సమితి_t *get_సమితి_from_సూచీ (hb_codepoint_t సూచీ_2)
  {
    ఒకవేళ (సూచీ_2 < sets.నిడివి)
      తిరిగి sets[సూచీ_2].get ();
    తిరిగి శూన్యసూచిక;
  }

  గుర్తులేని పూర్ణాంకం get_glyph_నమోదు_లెక్క (hb_codepoint_t gid) స్థిరరాశి
  {
    ఒకవేళ (gid < glyph_dependencies.నిడివి)
      తిరిగి glyph_dependencies[gid].dependencies.నిడివి;
    తిరిగి 0;
  }

  తార్కిక get_glyph_నమోదు (hb_codepoint_t gid, గుర్తులేని పూర్ణాంకం సూచీ_2,
                        hb_ట్యాగ్_t *పట్టిక_ట్యాగ్_2, hb_codepoint_t *dependent,
                        hb_ట్యాగ్_t *layout_ట్యాగ్, hb_codepoint_t *ligature_సమితి_2,
                        hb_codepoint_t *context_సమితి, uint8_t *flags)
  {
    ఒకవేళ (gid < glyph_dependencies.నిడివి &&
        సూచీ_2 < glyph_dependencies[gid].dependencies.నిడివి) {
      స్వయంచాలక &d = glyph_dependencies[gid].dependencies[సూచీ_2];
      *పట్టిక_ట్యాగ్_2 = d.పట్టిక_ట్యాగ్_2;
      *dependent = d.dependent;
      *layout_ట్యాగ్ = d.layout_ట్యాగ్;
      *ligature_సమితి_2 = d.ligature_సమితి_2;
      *context_సమితి = d.context_సమితి;
      ఒకవేళ (flags) *flags = d.flags;
      తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
  }

  శూన్య print ()
  {
    కొరకు (గుర్తులేని i = 0; i < glyph_dependencies.నిడివి; i++) {
      స్వయంచాలక &gd = glyph_dependencies[i];
      ఒకవేళ (!gd.dependencies.నిడివి)
        కొనసాగు;
      రాయి ("GID %u:\n", i);
      కొరకు (స్వయంచాలక &d : gd.dependencies) {
        ఒకవేళ (d.పట్టిక_ట్యాగ్_2 == HB_OT_ట్యాగ్_GSUB) {
          రాయి ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_ట్యాగ్), d.dependent);
          ఒకవేళ (d.ligature_సమితి_2 != HB_CODEPOINT_INVALID)
            రాయి ("  (ligature)");
        } లేకపోతే {
          రాయి ("  %c%c%c%c -> %u", HB_UNTAG(d.పట్టిక_ట్యాగ్_2), d.dependent);
        }
        రాయి ("\n");
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
నిర్మాణం hb_depend_దత్తాంశం_builder_t
{
  hb_depend_దత్తాంశం_builder_t (hb_depend_దత్తాంశం_t &దత్తాంశం_2)
    : current_context_సమితి_సూచీ (HB_CODEPOINT_INVALID),
      current_అంచు_flags (HB_SUBSET_DEPEND_అంచు_FLAG_NONE),
      దత్తాంశం (దత్తాంశం_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  స్థిరరాశి hb_సమితి_t *get_సమితి_from_సూచీ (hb_codepoint_t సూచీ_2)
  { తిరిగి దత్తాంశం.get_సమితి_from_సూచీ (సూచీ_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  శూన్య discard_సమితి (hb_codepoint_t సమితి_సూచీ)
  {
    ఒకవేళ (సమితి_సూచీ >= దత్తాంశం.sets.నిడివి)
    {
      DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "Attempting to discard invalid set %u (max is %u)",
                 సమితి_సూచీ, దత్తాంశం.sets.నిడివి - 1);
      తిరిగి;
    }
    సమితి_గమ్యం_సూచీ.del (దత్తాంశం.sets[సమితి_సూచీ].get ());
    దత్తాంశం.sets[సమితి_సూచీ]->clear ();
    check_success (free_సమితి_list.నెట్టు_or_fail (సమితి_సూచీ));
  }

  hb_codepoint_t new_సమితి (స్థిరరాశి hb_సమితి_t &సమితి)
  {
    hb_codepoint_t సమితి_సూచీ;

    ఒకవేళ (free_సమితి_list.నిడివి > 0)
    {
      సమితి_సూచీ = free_సమితి_list.తీయి ();
      దత్తాంశం.sets[సమితి_సూచీ]->సమితి (సమితి);
      ఒకవేళ (unlikely (దత్తాంశం.sets[సమితి_సూచీ]->లోపల_error ()))
	తిరిగి fail_invalid ();
    }
    లేకపోతే
    {
      సమితి_సూచీ = దత్తాంశం.sets.నిడివి;
      hb_సమితి_t *new_సమితి = hb_సమితి_create ();
      ఒకవేళ (unlikely (!new_సమితి))
	తిరిగి fail_invalid ();
      new_సమితి->సమితి (సమితి);
      ఒకవేళ (unlikely (new_సమితి->లోపల_error ()))
      {
	hb_సమితి_destroy (new_సమితి);
	తిరిగి fail_invalid ();
      }
      ఒకవేళ (unlikely (!దత్తాంశం.sets.నెట్టు_or_fail (hb::unique_ptr<hb_సమితి_t> {new_సమితి})))
	తిరిగి fail_invalid ();
    }

    తిరిగి సమితి_సూచీ;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t కనుగొను_or_create_సమితి (స్థిరరాశి hb_సమితి_t &సమితి, తార్కిక *created = శూన్యసూచిక)
  {
    hb_codepoint_t *existing_idx = శూన్యసూచిక;
    ఒకవేళ (సమితి_గమ్యం_సూచీ.has (&సమితి, &existing_idx))
    {
      ఒకవేళ (created) *created = అబద్ధం;
      తిరిగి *existing_idx;
    }

    hb_codepoint_t new_idx = new_సమితి (సమితి);
    ఒకవేళ (unlikely (new_idx == HB_CODEPOINT_INVALID))
      తిరిగి HB_CODEPOINT_INVALID;

    ఒకవేళ (unlikely (!సమితి_గమ్యం_సూచీ.సమితి (దత్తాంశం.sets[new_idx].get (), new_idx)))
      తిరిగి fail_invalid ();
    ఒకవేళ (created) *created = నిజం;
    తిరిగి new_idx;
  }

  hb_codepoint_t కనుగొను_or_create_context_సమితి (స్థిరరాశి hb_సమితి_t &సమితి)
  { తిరిగి కనుగొను_or_create_సమితి (సమితి); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_సమితి (స్థిరరాశి hb_సదిశ_t<hb_సమితి_t> *backtrack_sets,
                                     స్థిరరాశి hb_సదిశ_t<hb_సమితి_t> *lookahead_sets)
  {
    ఒకవేళ ((!backtrack_sets || backtrack_sets->నిడివి == 0) &&
        (!lookahead_sets || lookahead_sets->నిడివి == 0))
      తిరిగి HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_సమితి_t ప్రత్యక్ష_requirements;

    ఒకవేళ (backtrack_sets)
    {
      కొరకు (స్థిరరాశి స్వయంచాలక &వెనుక_సమితి : *backtrack_sets)
      {
        ఒకవేళ (వెనుక_సమితి.get_population () == 1)
          ప్రత్యక్ష_requirements.కూడు (వెనుక_సమితి.get_min ());
      }
    }

    ఒకవేళ (lookahead_sets)
    {
      కొరకు (స్థిరరాశి స్వయంచాలక &look_సమితి : *lookahead_sets)
      {
        ఒకవేళ (look_సమితి.get_population () == 1)
          ప్రత్యక్ష_requirements.కూడు (look_సమితి.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_సమితి_t context_elements;

    ఒకవేళ (backtrack_sets)
    {
      కొరకు (స్థిరరాశి స్వయంచాలక &వెనుక_సమితి : *backtrack_sets)
      {
        ఒకవేళ (వెనుక_సమితి.get_population () > 1)
        {
          hb_సమితి_t filtered_సమితి;
          filtered_సమితి.సమితి (వెనుక_సమితి);
          filtered_సమితి.వ్యవకలనం (ప్రత్యక్ష_requirements);
          ఒకవేళ (unlikely (filtered_సమితి.లోపల_error ()))
            తిరిగి fail_invalid ();

          ఒకవేళ (!filtered_సమితి.is_empty ())
          {
            hb_codepoint_t సమితి_idx = కనుగొను_or_create_context_సమితి (filtered_సమితి);
            ఒకవేళ (unlikely (సమితి_idx == HB_CODEPOINT_INVALID))
              తిరిగి HB_CODEPOINT_INVALID;
            context_elements.కూడు (HB_DEPEND_CONTEXT_సమితి_FLAG | సమితి_idx);
          }
        }
      }
    }

    ఒకవేళ (lookahead_sets)
    {
      కొరకు (స్థిరరాశి స్వయంచాలక &look_సమితి : *lookahead_sets)
      {
        ఒకవేళ (look_సమితి.get_population () > 1)
        {
          hb_సమితి_t filtered_సమితి;
          filtered_సమితి.సమితి (look_సమితి);
          filtered_సమితి.వ్యవకలనం (ప్రత్యక్ష_requirements);
          ఒకవేళ (unlikely (filtered_సమితి.లోపల_error ()))
            తిరిగి fail_invalid ();

          ఒకవేళ (!filtered_సమితి.is_empty ())
          {
            hb_codepoint_t సమితి_idx = కనుగొను_or_create_context_సమితి (filtered_సమితి);
            ఒకవేళ (unlikely (సమితి_idx == HB_CODEPOINT_INVALID))
              తిరిగి HB_CODEPOINT_INVALID;
            context_elements.కూడు (HB_DEPEND_CONTEXT_సమితి_FLAG | సమితి_idx);
          }
        }
      }
    }

    context_elements.union_ (ప్రత్యక్ష_requirements);
    ఒకవేళ (unlikely (ప్రత్యక్ష_requirements.లోపల_error () ||
                  context_elements.లోపల_error ()))
      తిరిగి fail_invalid ();

    ఒకవేళ (context_elements.is_empty ())
      తిరిగి HB_CODEPOINT_INVALID;

    తిరిగి కనుగొను_or_create_context_సమితి (context_elements);
  }

  స్థిర uint64_t encode_అంచు_ref (hb_codepoint_t ఆధారం, గుర్తులేని పూర్ణాంకం సూచీ_2)
  { తిరిగి (uint64_t (ఆధారం) + 1) << 32 | సూచీ_2; }

  స్థిర శూన్య decode_అంచు_ref (uint64_t ref,
			       hb_codepoint_t *ఆధారం,
			       గుర్తులేని పూర్ణాంకం *సూచీ_2)
  {
    *ఆధారం = (ref >> 32) - 1;
    *సూచీ_2 = ref;
  }

  uint32_t అంచు_hash (hb_codepoint_t ఆధారం, స్థిరరాశి hb_depend_అంచు_t &రికార్డు) స్థిరరాశి
  {
    hb_depend_అంచు_కీలకం_t key (ఆధారం, రికార్డు.పట్టిక_ట్యాగ్_2, రికార్డు.layout_ట్యాగ్,
			      రికార్డు.dependent, రికార్డు.ligature_సమితి_2,
			      రికార్డు.context_సమితి);
    తిరిగి key.hash ();
  }

  తార్కిక resize_అంచు_slots ()
  {
    గుర్తులేని పూర్ణాంకం new_కొలత = అంచు_slots.నిడివి ? అంచు_slots.నిడివి * 2 : 8;
    ఒకవేళ (unlikely (new_కొలత < అంచు_slots.నిడివి))
      తిరిగి fail ();

    hb_సదిశ_t<uint64_t> new_slots;
    ఒకవేళ (unlikely (!new_slots.resize_exact (new_కొలత)))
      తిరిగి fail ();

    కొరకు (uint64_t ref : అంచు_slots)
    {
      ఒకవేళ (!ref)
	కొనసాగు;

      hb_codepoint_t ఆధారం;
      గుర్తులేని పూర్ణాంకం సూచీ_2;
      decode_అంచు_ref (ref, &ఆధారం, &సూచీ_2);
      స్థిరరాశి hb_depend_అంచు_t &రికార్డు = దత్తాంశం.glyph_dependencies[ఆధారం].dependencies[సూచీ_2];
      గుర్తులేని పూర్ణాంకం స్థానం = అంచు_hash (ఆధారం, రికార్డు) & (new_కొలత - 1);
      గుర్తులేని పూర్ణాంకం అడుగు = 0;
      అయితే (new_slots[స్థానం])
	స్థానం = (స్థానం + ++అడుగు) & (new_కొలత - 1);
      new_slots[స్థానం] = ref;
    }

    అంచు_slots = std::move (new_slots);
    తిరిగి నిజం;
  }

  తార్కిక కూడు_అంచు (hb_codepoint_t ఆధారం, స్థిరరాశి hb_depend_అంచు_t &రికార్డు)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    ఒకవేళ (unlikely (!అంచు_slots.నిడివి ||
		  uint64_t (అంచు_population) * 3 / 2 >= అంచు_slots.నిడివి - 1))
      ఒకవేళ (unlikely (!resize_అంచు_slots ()))
	తిరిగి అబద్ధం;

    గుర్తులేని పూర్ణాంకం స్థానం = అంచు_hash (ఆధారం, రికార్డు) & (అంచు_slots.నిడివి - 1);
    గుర్తులేని పూర్ణాంకం అడుగు = 0;
    అయితే (uint64_t ref = అంచు_slots[స్థానం])
    {
      hb_codepoint_t existing_ఆధారం;
      గుర్తులేని పూర్ణాంకం existing_సూచీ;
      decode_అంచు_ref (ref, &existing_ఆధారం, &existing_సూచీ);
      ఒకవేళ (existing_ఆధారం == ఆధారం &&
	  దత్తాంశం.glyph_dependencies[ఆధారం].dependencies[existing_సూచీ] == రికార్డు)
	తిరిగి అబద్ధం;
      స్థానం = (స్థానం + ++అడుగు) & (అంచు_slots.నిడివి - 1);
    }

    స్వయంచాలక &dependencies = దత్తాంశం.glyph_dependencies[ఆధారం].dependencies;
    గుర్తులేని పూర్ణాంకం సూచీ_2 = dependencies.నిడివి;
    ఒకవేళ (unlikely (!dependencies.నెట్టు_or_fail (రికార్డు)))
      తిరిగి fail ();

    అంచు_slots[స్థానం] = encode_అంచు_ref (ఆధారం, సూచీ_2);
    అంచు_population++;
    తిరిగి నిజం;
  }

  తార్కిక కూడు_depend_layout (hb_codepoint_t target, hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
                          hb_ట్యాగ్_t layout_ట్యాగ్,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_సమితి = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_సమితి = HB_CODEPOINT_INVALID,
                          hb_subset_depend_అంచు_flags_t flags = HB_SUBSET_DEPEND_అంచు_FLAG_NONE)
  {
    ఒకవేళ (target >= దత్తాంశం.glyph_dependencies.నిడివి) {
      DEBUG_సందేశం (SUBSET, శూన్యసూచిక, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(పట్టిక_ట్యాగ్_2));
      తిరిగి అబద్ధం;
    }

    తిరిగి కూడు_అంచు (target, hb_depend_అంచు_t (పట్టిక_ట్యాగ్_2, dependent, layout_ట్యాగ్,
					      lig_సమితి, context_సమితి, flags));
  }

  తార్కిక కూడు_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_సూచీ,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_సమితి = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_సమితి = HB_CODEPOINT_INVALID)
  {
    ఒకవేళ (context_సమితి == HB_CODEPOINT_INVALID)
      context_సమితి = current_context_సమితి_సూచీ;
    hb_subset_depend_అంచు_flags_t flags = current_అంచు_flags;

    తార్కిక any_added = అబద్ధం;
    కొరకు (uint64_t నమోదు_2 : get_lookup_features (lookup_సూచీ)) {
      hb_ట్యాగ్_t t = (hb_ట్యాగ్_t) నమోదు_2;
      ఒకవేళ (కూడు_depend_layout (target, HB_OT_ట్యాగ్_GSUB, t, dependent, lig_సమితి, context_సమితి, flags))
        any_added = నిజం;
    }
    తిరిగి any_added;
  }

  తార్కిక init_lookup_features (గుర్తులేని lookup_లెక్క)
  {
    తిరిగి check_success (lookup_feature_offsets.resize (lookup_లెక్క + 1));
  }

  తార్కిక కూడు_lookup_feature (hb_codepoint_t lookup_సూచీ, hb_ట్యాగ్_t feature_ట్యాగ్)
  {
    ఒకవేళ (feature_ట్యాగ్ == HB_సమితి_విలువ_INVALID)
      తిరిగి నిజం;

    ఒకవేళ (unlikely (!lookup_feature_offsets.నిడివి ||
		  lookup_సూచీ >= lookup_feature_offsets.నిడివి - 1))
      తిరిగి fail ();

    uint64_t నమోదు_2 = ((uint64_t) lookup_సూచీ << 32) | feature_ట్యాగ్;
    తిరిగి check_success (lookup_features.నెట్టు_or_fail (నమోదు_2));
  }

  తార్కిక ముగింపు_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      తిరిగి a < b ? -1 : a > b ? 1 : 0;
    });

    గుర్తులేని లిఖ = 0;
    కొరకు (uint64_t నమోదు_2 : lookup_features)
      ఒకవేళ (!లిఖ || నమోదు_2 != lookup_features[లిఖ - 1])
	lookup_features[లిఖ++] = నమోదు_2;
    lookup_features.shrink (లిఖ, అబద్ధం);

    గుర్తులేని feature_సూచీ = 0;
    గుర్తులేని lookup_లెక్క = lookup_feature_offsets.నిడివి - 1;
    కొరకు (గుర్తులేని lookup_సూచీ = 0; lookup_సూచీ < lookup_లెక్క; lookup_సూచీ++)
    {
      lookup_feature_offsets[lookup_సూచీ] = feature_సూచీ;
      అయితే (feature_సూచీ < lookup_features.నిడివి &&
	     lookup_features[feature_సూచీ] >> 32 == lookup_సూచీ)
	feature_సూచీ++;
    }
    lookup_feature_offsets[lookup_లెక్క] = feature_సూచీ;
    తిరిగి నిజం;
  }

  hb_array_t<స్థిరరాశి uint64_t> get_lookup_features (hb_codepoint_t lookup_సూచీ) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!lookup_feature_offsets.నిడివి ||
		  lookup_సూచీ >= lookup_feature_offsets.నిడివి - 1))
      తిరిగి {};

    గుర్తులేని ప్రారంభం = lookup_feature_offsets[lookup_సూచీ];
    గుర్తులేని చివర = lookup_feature_offsets[lookup_సూచీ + 1];
    తిరిగి lookup_features.as_array ().sub_array (ప్రారంభం, చివర - ప్రారంభం);
  }

  శూన్య కూడు_depend (hb_codepoint_t target, hb_ట్యాగ్_t పట్టిక_ట్యాగ్_2,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_సమితి = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_సమితి = HB_CODEPOINT_INVALID,
                   hb_subset_depend_అంచు_flags_t flags = HB_SUBSET_DEPEND_అంచు_FLAG_NONE)
  {
    కూడు_depend_layout (target, పట్టిక_ట్యాగ్_2, HB_CODEPOINT_INVALID, dependent, lig_సమితి,
                       context_సమితి, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { తిరిగి hb_map_get (&nominal_glyphs, cp); }

  HB_అంతర్గత తార్కిక compile (hb_face_t *face);

  తార్కిక fail () { successful = అబద్ధం; తిరిగి అబద్ధం; }
  hb_codepoint_t fail_invalid () { fail (); తిరిగి HB_CODEPOINT_INVALID; }
  తార్కిక check_success (తార్కిక s) { successful = (successful && s); తిరిగి successful; }

  HB_అంతర్గత శూన్య get_gsub_dependencies (hb_face_t *face);

  తార్కిక successful = నిజం;
  hb_సమితి_t unicodes;
  hb_map_t nominal_glyphs;
  hb_సదిశ_t<uint64_t> lookup_features;
  hb_సదిశ_t<గుర్తులేని> lookup_feature_offsets;
  hb_సదిశ_t<uint64_t> అంచు_slots;
  గుర్తులేని పూర్ణాంకం అంచు_population = 0;
  hb_hashmap_t<స్థిరరాశి hb_సమితి_t*, hb_codepoint_t> సమితి_గమ్యం_సూచీ;
  hb_సదిశ_t<hb_codepoint_t> free_సమితి_list;
  hb_codepoint_t current_context_సమితి_సూచీ;
  hb_subset_depend_అంచు_flags_t current_అంచు_flags;

  hb_depend_దత్తాంశం_t &దత్తాంశం;
};


#అయితే_ముగింపు /* HB_DEPEND_DATA_HH */
