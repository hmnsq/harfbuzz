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

#अगर_अघोषित HB_DEPEND_तथ्य_HH
#घोषणा HB_DEPEND_तथ्य_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#समावेश "hb.hh"

#समावेश "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#अगर_अघोषित HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
#घोषणा HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
प्रकार क्रमागत {
  HB_SUBSET_DEPEND_EDGE_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_edge_flags_t;
#पूर्ण_अगर
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_edge_flags_t);

/* High bit of a context-set element marks it as an index into the sets array
 * rather than a raw glyph ID. */
#घोषणा HB_DEPEND_CONTEXT_SET_FLAG 0x80000000u

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
काष्ठा hb_depend_edge_t {
  hb_depend_edge_t() = मिटाओ;
  hb_depend_edge_t(hb_tag_t table_tag,
                   hb_codepoint_t dependent,
                   hb_tag_t layout_tag,
                   hb_codepoint_t ligature_set,
                   hb_codepoint_t context_set,
                   hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE) : table_tag(table_tag),
     dependent(dependent), layout_tag(layout_tag),
     ligature_set(ligature_set), context_set(context_set),
     flags(flags)
     {}

  bool चालक == (स्थिर hb_depend_edge_t &o) स्थिर
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    वापस table_tag == o.table_tag &&
           dependent == o.dependent &&
           layout_tag == o.layout_tag &&
           ligature_set == o.ligature_set &&
           context_set == o.context_set;
  }

  uint32_t hash () स्थिर
  {
    /* FNV-1a hash of all identity fields (excludes flags) */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (table_tag);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ hb_hash (dependent);
    current = current * 16777619;
    current = current ^ hb_hash (layout_tag);
    current = current * 16777619;
    current = current ^ hb_hash (ligature_set);
    current = current * 16777619;
    current = current ^ hb_hash (context_set);
    current = current * 16777619;
    वापस current;
  }

  hb_tag_t table_tag;
  hb_codepoint_t dependent;
  hb_tag_t layout_tag;
  hb_codepoint_t ligature_set;
  hb_codepoint_t context_set;
  hb_subset_depend_edge_flags_t flags;
};

/**
 * hb_depend_edge_key_t:
 *
 * Key structure for edge deduplication. Combines source glyph with edge record.
 * Uses the record's equality and hash operators for comparison.
 */
काष्ठा hb_depend_edge_कुंजी_t {
  hb_codepoint_t source;
  hb_depend_edge_t record;

  hb_depend_edge_कुंजी_t () : source (0), record (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  hb_depend_edge_कुंजी_t (hb_codepoint_t source,
              hb_tag_t table_tag,
              hb_tag_t layout_tag,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_set,
              hb_codepoint_t context_set)
    : source (source),
      record (table_tag, dependent, layout_tag, ligature_set, context_set, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  bool चालक == (स्थिर hb_depend_edge_कुंजी_t &o) स्थिर
  {
    वापस source == o.source && record == o.record;
  }

  uint32_t hash () स्थिर
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (source);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ record.hash ();
    current = current * 16777619;
    वापस current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
काष्ठा hb_depend_glyph_record_t {
  hb_vector_t<hb_depend_edge_t> dependencies;
};

/**
 * hb_depend_lookup_revmap_t:
 *
 * Internal structure tracking which features are associated with a lookup.
 * Used during GSUB dependency analysis to record the feature tags that
 * activate each lookup.
 */
काष्ठा hb_depend_lookup_revmap_t
{
  hb_depend_lookup_revmap_t () = शेष;
  explicit hb_depend_lookup_revmap_t (bool full) : full (full) {}
  hb_depend_lookup_revmap_t (स्थिर hb_depend_lookup_revmap_t &o) : full(o.full),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : full(o.full),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& चालक = (स्थिर hb_depend_lookup_revmap_t &o)
  {
    full = o.full;
    fv_indexes = o.fv_indexes;
    वापस *यह;
  }

  bool full = true;
  hb_set_t fv_indexes;
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
काष्ठा hb_depend_तथ्य_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_vector_t<hb::unique_ptr<hb_set_t>> sets;

  hb_vector_t<hb_depend_glyph_record_t> glyph_dependencies;

  स्थिर hb_set_t *get_set_from_सूचकांक (hb_codepoint_t सूचकांक_2)
  {
    अगर (सूचकांक_2 < sets.लंबाई)
      वापस sets[सूचकांक_2].लो ();
    वापस nullptr;
  }

  अचिन्हित पूर्णांक get_glyph_entry_गिनती (hb_codepoint_t gid) स्थिर
  {
    अगर (gid < glyph_dependencies.लंबाई)
      वापस glyph_dependencies[gid].dependencies.लंबाई;
    वापस 0;
  }

  bool get_glyph_entry (hb_codepoint_t gid, अचिन्हित पूर्णांक सूचकांक_2,
                        hb_tag_t *table_tag, hb_codepoint_t *dependent,
                        hb_tag_t *layout_tag, hb_codepoint_t *ligature_set,
                        hb_codepoint_t *context_set, uint8_t *flags)
  {
    अगर (gid < glyph_dependencies.लंबाई &&
        सूचकांक_2 < glyph_dependencies[gid].dependencies.लंबाई) {
      स्वतः &d = glyph_dependencies[gid].dependencies[सूचकांक_2];
      *table_tag = d.table_tag;
      *dependent = d.dependent;
      *layout_tag = d.layout_tag;
      *ligature_set = d.ligature_set;
      *context_set = d.context_set;
      अगर (flags) *flags = d.flags;
      वापस true;
    }
    वापस false;
  }

  व्योम print ()
  {
    क्रम (अचिन्हित i = 0; i < glyph_dependencies.लंबाई; i++) {
      स्वतः &gd = glyph_dependencies[i];
      अगर (!gd.dependencies.लंबाई)
        जारी;
      म_लिखो ("GID %u:\n", i);
      क्रम (स्वतः &d : gd.dependencies) {
        अगर (d.table_tag == HB_OT_TAG_GSUB) {
          म_लिखो ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_tag), d.dependent);
          अगर (d.ligature_set != HB_CODEPOINT_INVALID)
            म_लिखो ("  (ligature)");
        } अन्यथा {
          म_लिखो ("  %c%c%c%c -> %u", HB_UNTAG(d.table_tag), d.dependent);
        }
        म_लिखो ("\n");
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
काष्ठा hb_depend_तथ्य_builder_t
{
  hb_depend_तथ्य_builder_t (hb_depend_तथ्य_t &तथ्य_2)
    : current_context_set_सूचकांक (HB_CODEPOINT_INVALID),
      current_edge_flags (HB_SUBSET_DEPEND_EDGE_FLAG_NONE),
      तथ्य (तथ्य_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  स्थिर hb_set_t *get_set_from_सूचकांक (hb_codepoint_t सूचकांक_2)
  { वापस तथ्य.get_set_from_सूचकांक (सूचकांक_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  व्योम discard_set (hb_codepoint_t set_सूचकांक)
  {
    अगर (set_सूचकांक >= तथ्य.sets.लंबाई)
    {
      DEBUG_संदेश (SUBSET, nullptr, "Attempting to discard invalid set %u (max is %u)",
                 set_सूचकांक, तथ्य.sets.लंबाई - 1);
      वापस;
    }
    set_to_सूचकांक.del (तथ्य.sets[set_सूचकांक].लो ());
    तथ्य.sets[set_सूचकांक]->सफा ();
    check_success (free_set_list.धकेलो_or_fail (set_सूचकांक));
  }

  hb_codepoint_t new_set (स्थिर hb_set_t &set)
  {
    hb_codepoint_t set_सूचकांक;

    अगर (free_set_list.लंबाई > 0)
    {
      set_सूचकांक = free_set_list.निकालो ();
      तथ्य.sets[set_सूचकांक]->set (set);
      अगर (unlikely (तथ्य.sets[set_सूचकांक]->in_error ()))
	वापस fail_invalid ();
    }
    अन्यथा
    {
      set_सूचकांक = तथ्य.sets.लंबाई;
      hb_set_t *new_set = hb_set_create ();
      अगर (unlikely (!new_set))
	वापस fail_invalid ();
      new_set->set (set);
      अगर (unlikely (new_set->in_error ()))
      {
	hb_set_destroy (new_set);
	वापस fail_invalid ();
      }
      अगर (unlikely (!तथ्य.sets.धकेलो_or_fail (hb::unique_ptr<hb_set_t> {new_set})))
	वापस fail_invalid ();
    }

    वापस set_सूचकांक;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t find_or_create_set (स्थिर hb_set_t &set, bool *created = nullptr)
  {
    hb_codepoint_t *existing_idx = nullptr;
    अगर (set_to_सूचकांक.has (&set, &existing_idx))
    {
      अगर (created) *created = false;
      वापस *existing_idx;
    }

    hb_codepoint_t new_idx = new_set (set);
    अगर (unlikely (new_idx == HB_CODEPOINT_INVALID))
      वापस HB_CODEPOINT_INVALID;

    अगर (unlikely (!set_to_सूचकांक.set (तथ्य.sets[new_idx].लो (), new_idx)))
      वापस fail_invalid ();
    अगर (created) *created = true;
    वापस new_idx;
  }

  hb_codepoint_t find_or_create_context_set (स्थिर hb_set_t &set)
  { वापस find_or_create_set (set); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_set (स्थिर hb_vector_t<hb_set_t> *backtrack_sets,
                                     स्थिर hb_vector_t<hb_set_t> *lookahead_sets)
  {
    अगर ((!backtrack_sets || backtrack_sets->लंबाई == 0) &&
        (!lookahead_sets || lookahead_sets->लंबाई == 0))
      वापस HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_set_t direct_requirements;

    अगर (backtrack_sets)
    {
      क्रम (स्थिर स्वतः &back_set : *backtrack_sets)
      {
        अगर (back_set.get_population () == 1)
          direct_requirements.add (back_set.get_min ());
      }
    }

    अगर (lookahead_sets)
    {
      क्रम (स्थिर स्वतः &look_set : *lookahead_sets)
      {
        अगर (look_set.get_population () == 1)
          direct_requirements.add (look_set.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_set_t context_elements;

    अगर (backtrack_sets)
    {
      क्रम (स्थिर स्वतः &back_set : *backtrack_sets)
      {
        अगर (back_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (back_set);
          filtered_set.subtract (direct_requirements);
          अगर (unlikely (filtered_set.in_error ()))
            वापस fail_invalid ();

          अगर (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            अगर (unlikely (set_idx == HB_CODEPOINT_INVALID))
              वापस HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    अगर (lookahead_sets)
    {
      क्रम (स्थिर स्वतः &look_set : *lookahead_sets)
      {
        अगर (look_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (look_set);
          filtered_set.subtract (direct_requirements);
          अगर (unlikely (filtered_set.in_error ()))
            वापस fail_invalid ();

          अगर (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            अगर (unlikely (set_idx == HB_CODEPOINT_INVALID))
              वापस HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    context_elements.union_ (direct_requirements);
    अगर (unlikely (direct_requirements.in_error () ||
                  context_elements.in_error ()))
      वापस fail_invalid ();

    अगर (context_elements.is_empty ())
      वापस HB_CODEPOINT_INVALID;

    वापस find_or_create_context_set (context_elements);
  }

  अटल uint64_t encode_edge_ref (hb_codepoint_t source, अचिन्हित पूर्णांक सूचकांक_2)
  { वापस (uint64_t (source) + 1) << 32 | सूचकांक_2; }

  अटल व्योम decode_edge_ref (uint64_t ref,
			       hb_codepoint_t *source,
			       अचिन्हित पूर्णांक *सूचकांक_2)
  {
    *source = (ref >> 32) - 1;
    *सूचकांक_2 = ref;
  }

  uint32_t edge_hash (hb_codepoint_t source, स्थिर hb_depend_edge_t &record) स्थिर
  {
    hb_depend_edge_कुंजी_t key (source, record.table_tag, record.layout_tag,
			      record.dependent, record.ligature_set,
			      record.context_set);
    वापस key.hash ();
  }

  bool resize_edge_slots ()
  {
    अचिन्हित पूर्णांक new_आकार = edge_slots.लंबाई ? edge_slots.लंबाई * 2 : 8;
    अगर (unlikely (new_आकार < edge_slots.लंबाई))
      वापस हार ();

    hb_vector_t<uint64_t> new_slots;
    अगर (unlikely (!new_slots.resize_exact (new_आकार)))
      वापस हार ();

    क्रम (uint64_t ref : edge_slots)
    {
      अगर (!ref)
	जारी;

      hb_codepoint_t source;
      अचिन्हित पूर्णांक सूचकांक_2;
      decode_edge_ref (ref, &source, &सूचकांक_2);
      स्थिर hb_depend_edge_t &record = तथ्य.glyph_dependencies[source].dependencies[सूचकांक_2];
      अचिन्हित पूर्णांक slot = edge_hash (source, record) & (new_आकार - 1);
      अचिन्हित पूर्णांक step = 0;
      जबतक (new_slots[slot])
	slot = (slot + ++step) & (new_आकार - 1);
      new_slots[slot] = ref;
    }

    edge_slots = std::move (new_slots);
    वापस true;
  }

  bool add_edge (hb_codepoint_t source, स्थिर hb_depend_edge_t &record)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    अगर (unlikely (!edge_slots.लंबाई ||
		  uint64_t (edge_population) * 3 / 2 >= edge_slots.लंबाई - 1))
      अगर (unlikely (!resize_edge_slots ()))
	वापस false;

    अचिन्हित पूर्णांक slot = edge_hash (source, record) & (edge_slots.लंबाई - 1);
    अचिन्हित पूर्णांक step = 0;
    जबतक (uint64_t ref = edge_slots[slot])
    {
      hb_codepoint_t existing_source;
      अचिन्हित पूर्णांक existing_सूचकांक;
      decode_edge_ref (ref, &existing_source, &existing_सूचकांक);
      अगर (existing_source == source &&
	  तथ्य.glyph_dependencies[source].dependencies[existing_सूचकांक] == record)
	वापस false;
      slot = (slot + ++step) & (edge_slots.लंबाई - 1);
    }

    स्वतः &dependencies = तथ्य.glyph_dependencies[source].dependencies;
    अचिन्हित पूर्णांक सूचकांक_2 = dependencies.लंबाई;
    अगर (unlikely (!dependencies.धकेलो_or_fail (record)))
      वापस हार ();

    edge_slots[slot] = encode_edge_ref (source, सूचकांक_2);
    edge_population++;
    वापस true;
  }

  bool add_depend_layout (hb_codepoint_t target, hb_tag_t table_tag,
                          hb_tag_t layout_tag,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                          hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    अगर (target >= तथ्य.glyph_dependencies.लंबाई) {
      DEBUG_संदेश (SUBSET, nullptr, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(table_tag));
      वापस false;
    }

    वापस add_edge (target, hb_depend_edge_t (table_tag, dependent, layout_tag,
					      lig_set, context_set, flags));
  }

  bool add_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_सूचकांक,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_set = HB_CODEPOINT_INVALID)
  {
    अगर (context_set == HB_CODEPOINT_INVALID)
      context_set = current_context_set_सूचकांक;
    hb_subset_depend_edge_flags_t flags = current_edge_flags;

    bool any_added = false;
    क्रम (uint64_t entry : get_lookup_features (lookup_सूचकांक)) {
      hb_tag_t t = (hb_tag_t) entry;
      अगर (add_depend_layout (target, HB_OT_TAG_GSUB, t, dependent, lig_set, context_set, flags))
        any_added = true;
    }
    वापस any_added;
  }

  bool init_lookup_features (अचिन्हित lookup_गिनती)
  {
    वापस check_success (lookup_feature_offsets.resize (lookup_गिनती + 1));
  }

  bool add_lookup_feature (hb_codepoint_t lookup_सूचकांक, hb_tag_t feature_tag)
  {
    अगर (feature_tag == HB_SET_मूल्य_INVALID)
      वापस true;

    अगर (unlikely (!lookup_feature_offsets.लंबाई ||
		  lookup_सूचकांक >= lookup_feature_offsets.लंबाई - 1))
      वापस हार ();

    uint64_t entry = ((uint64_t) lookup_सूचकांक << 32) | feature_tag;
    वापस check_success (lookup_features.धकेलो_or_fail (entry));
  }

  bool finish_lookup_features ()
  {
    lookup_features.क्विक ([] (uint64_t a, uint64_t b) {
      वापस a < b ? -1 : a > b ? 1 : 0;
    });

    अचिन्हित ग_लिखो = 0;
    क्रम (uint64_t entry : lookup_features)
      अगर (!ग_लिखो || entry != lookup_features[ग_लिखो - 1])
	lookup_features[ग_लिखो++] = entry;
    lookup_features.shrink (ग_लिखो, false);

    अचिन्हित feature_सूचकांक = 0;
    अचिन्हित lookup_गिनती = lookup_feature_offsets.लंबाई - 1;
    क्रम (अचिन्हित lookup_सूचकांक = 0; lookup_सूचकांक < lookup_गिनती; lookup_सूचकांक++)
    {
      lookup_feature_offsets[lookup_सूचकांक] = feature_सूचकांक;
      जबतक (feature_सूचकांक < lookup_features.लंबाई &&
	     lookup_features[feature_सूचकांक] >> 32 == lookup_सूचकांक)
	feature_सूचकांक++;
    }
    lookup_feature_offsets[lookup_गिनती] = feature_सूचकांक;
    वापस true;
  }

  hb_array_t<स्थिर uint64_t> get_lookup_features (hb_codepoint_t lookup_सूचकांक) स्थिर
  {
    अगर (unlikely (!lookup_feature_offsets.लंबाई ||
		  lookup_सूचकांक >= lookup_feature_offsets.लंबाई - 1))
      वापस {};

    अचिन्हित start = lookup_feature_offsets[lookup_सूचकांक];
    अचिन्हित end = lookup_feature_offsets[lookup_सूचकांक + 1];
    वापस lookup_features.as_array ().sub_array (start, end - start);
  }

  व्योम add_depend (hb_codepoint_t target, hb_tag_t table_tag,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                   hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    add_depend_layout (target, table_tag, HB_CODEPOINT_INVALID, dependent, lig_set,
                       context_set, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { वापस hb_map_get (&nominal_glyphs, cp); }

  HB_INTERNAL bool compile (hb_face_t *face);

  bool हार () { successful = false; वापस false; }
  hb_codepoint_t fail_invalid () { हार (); वापस HB_CODEPOINT_INVALID; }
  bool check_success (bool s) { successful = (successful && s); वापस successful; }

  HB_INTERNAL व्योम get_gsub_dependencies (hb_face_t *face);

  bool successful = true;
  hb_set_t unicodes;
  hb_map_t nominal_glyphs;
  hb_vector_t<uint64_t> lookup_features;
  hb_vector_t<अचिन्हित> lookup_feature_offsets;
  hb_vector_t<uint64_t> edge_slots;
  अचिन्हित पूर्णांक edge_population = 0;
  hb_hashmap_t<स्थिर hb_set_t*, hb_codepoint_t> set_to_सूचकांक;
  hb_vector_t<hb_codepoint_t> free_set_list;
  hb_codepoint_t current_context_set_सूचकांक;
  hb_subset_depend_edge_flags_t current_edge_flags;

  hb_depend_तथ्य_t &तथ्य;
};


#पूर्ण_अगर /* HB_DEPEND_DATA_HH */
