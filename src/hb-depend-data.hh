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

#ifndef HB_DEPEND_DATA_HH
#define HB_DEPEND_DATA_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#ifndef HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
#define HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
प्रकारव्याख्या गणना {
  HB_SUBSET_DEPEND_EDGE_FLAG_NONE                   = 0x00u,
  HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION  = 0x01u,
  HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT    = 0x02u,
} hb_subset_depend_edge_flags_t;
#endif
/* Apply operator overloads now that hb.hh (and thus hb-algs.hh) is available. */
HB_MARK_AS_FLAG_T (hb_subset_depend_edge_flags_t);

/* High bit of a context-set element marks it as an index into the sets array
 * rather than a raw glyph ID. */
#define HB_DEPEND_CONTEXT_SET_FLAG 0x80000000u

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
संरचना hb_depend_edge_t {
  hb_depend_edge_t() = delete;
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

  तार्किक चालक == (स्थिरांक hb_depend_edge_t &o) स्थिरांक
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    परत table_tag == o.table_tag &&
           dependent == o.dependent &&
           layout_tag == o.layout_tag &&
           ligature_set == o.ligature_set &&
           context_set == o.context_set;
  }

  uint32_t hash () स्थिरांक
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
    परत current;
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
संरचना hb_depend_edge_किल्ली_t {
  hb_codepoint_t source;
  hb_depend_edge_t record;

  hb_depend_edge_किल्ली_t () : source (0), record (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  hb_depend_edge_किल्ली_t (hb_codepoint_t source,
              hb_tag_t table_tag,
              hb_tag_t layout_tag,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_set,
              hb_codepoint_t context_set)
    : source (source),
      record (table_tag, dependent, layout_tag, ligature_set, context_set, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  तार्किक चालक == (स्थिरांक hb_depend_edge_किल्ली_t &o) स्थिरांक
  {
    परत source == o.source && record == o.record;
  }

  uint32_t hash () स्थिरांक
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (source);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ record.hash ();
    current = current * 16777619;
    परत current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
संरचना hb_depend_glyph_record_t {
  hb_vector_t<hb_depend_edge_t> dependencies;
};

/**
 * hb_depend_lookup_revmap_t:
 *
 * Internal structure tracking which features are associated with a lookup.
 * Used during GSUB dependency analysis to record the feature tags that
 * activate each lookup.
 */
संरचना hb_depend_lookup_revmap_t
{
  hb_depend_lookup_revmap_t () = पूर्वनिर्धारित;
  explicit hb_depend_lookup_revmap_t (तार्किक full) : full (full) {}
  hb_depend_lookup_revmap_t (स्थिरांक hb_depend_lookup_revmap_t &o) : full(o.full),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : full(o.full),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& चालक = (स्थिरांक hb_depend_lookup_revmap_t &o)
  {
    full = o.full;
    fv_indexes = o.fv_indexes;
    परत *हे;
  }

  तार्किक full = सत्य;
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
संरचना hb_depend_data_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_vector_t<hb::unique_ptr<hb_set_t>> sets;

  hb_vector_t<hb_depend_glyph_record_t> glyph_dependencies;

  स्थिरांक hb_set_t *get_set_from_index (hb_codepoint_t index)
  {
    जर (index < sets.length)
      परत sets[index].get ();
    परत nullptr;
  }

  अचिन्हित पूर्णांक get_glyph_entry_मोजणी (hb_codepoint_t gid) स्थिरांक
  {
    जर (gid < glyph_dependencies.length)
      परत glyph_dependencies[gid].dependencies.length;
    परत 0;
  }

  तार्किक get_glyph_entry (hb_codepoint_t gid, अचिन्हित पूर्णांक index,
                        hb_tag_t *table_tag, hb_codepoint_t *dependent,
                        hb_tag_t *layout_tag, hb_codepoint_t *ligature_set,
                        hb_codepoint_t *context_set, uint8_t *flags)
  {
    जर (gid < glyph_dependencies.length &&
        index < glyph_dependencies[gid].dependencies.length) {
      स्वतः &d = glyph_dependencies[gid].dependencies[index];
      *table_tag = d.table_tag;
      *dependent = d.dependent;
      *layout_tag = d.layout_tag;
      *ligature_set = d.ligature_set;
      *context_set = d.context_set;
      जर (flags) *flags = d.flags;
      परत सत्य;
    }
    परत असत्य;
  }

  शून्य print ()
  {
    साठी (अचिन्हित i = 0; i < glyph_dependencies.length; i++) {
      स्वतः &gd = glyph_dependencies[i];
      जर (!gd.dependencies.length)
        सुरूठेवा;
      म_लिखो ("GID %u:\n", i);
      साठी (स्वतः &d : gd.dependencies) {
        जर (d.table_tag == HB_OT_TAG_GSUB) {
          म_लिखो ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_tag), d.dependent);
          जर (d.ligature_set != HB_CODEPOINT_INVALID)
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
संरचना hb_depend_data_builder_t
{
  hb_depend_data_builder_t (hb_depend_data_t &data_)
    : current_context_set_index (HB_CODEPOINT_INVALID),
      current_edge_flags (HB_SUBSET_DEPEND_EDGE_FLAG_NONE),
      data (data_) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  स्थिरांक hb_set_t *get_set_from_index (hb_codepoint_t index)
  { परत data.get_set_from_index (index); }

  /* Discard an unused set that was allocated but had no edges added. */
  शून्य discard_set (hb_codepoint_t set_index)
  {
    जर (set_index >= data.sets.length)
    {
      DEBUG_संदेश (SUBSET, nullptr, "Attempting to discard invalid set %u (max is %u)",
                 set_index, data.sets.length - 1);
      परत;
    }
    set_to_index.del (data.sets[set_index].get ());
    data.sets[set_index]->clear ();
    check_success (free_set_list.ढकला_or_fail (set_index));
  }

  hb_codepoint_t new_set (स्थिरांक hb_set_t &set)
  {
    hb_codepoint_t set_index;

    जर (free_set_list.length > 0)
    {
      set_index = free_set_list.काढा ();
      data.sets[set_index]->set (set);
      जर (unlikely (data.sets[set_index]->in_error ()))
	परत fail_invalid ();
    }
    अन्यथा
    {
      set_index = data.sets.length;
      hb_set_t *new_set = hb_set_create ();
      जर (unlikely (!new_set))
	परत fail_invalid ();
      new_set->set (set);
      जर (unlikely (new_set->in_error ()))
      {
	hb_set_destroy (new_set);
	परत fail_invalid ();
      }
      जर (unlikely (!data.sets.ढकला_or_fail (hb::unique_ptr<hb_set_t> {new_set})))
	परत fail_invalid ();
    }

    परत set_index;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t find_or_create_set (स्थिरांक hb_set_t &set, तार्किक *created = nullptr)
  {
    hb_codepoint_t *existing_idx = nullptr;
    जर (set_to_index.has (&set, &existing_idx))
    {
      जर (created) *created = असत्य;
      परत *existing_idx;
    }

    hb_codepoint_t new_idx = new_set (set);
    जर (unlikely (new_idx == HB_CODEPOINT_INVALID))
      परत HB_CODEPOINT_INVALID;

    जर (unlikely (!set_to_index.set (data.sets[new_idx].get (), new_idx)))
      परत fail_invalid ();
    जर (created) *created = सत्य;
    परत new_idx;
  }

  hb_codepoint_t find_or_create_context_set (स्थिरांक hb_set_t &set)
  { परत find_or_create_set (set); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_set (स्थिरांक hb_vector_t<hb_set_t> *backtrack_sets,
                                     स्थिरांक hb_vector_t<hb_set_t> *lookahead_sets)
  {
    जर ((!backtrack_sets || backtrack_sets->length == 0) &&
        (!lookahead_sets || lookahead_sets->length == 0))
      परत HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_set_t direct_requirements;

    जर (backtrack_sets)
    {
      साठी (स्थिरांक स्वतः &back_set : *backtrack_sets)
      {
        जर (back_set.get_population () == 1)
          direct_requirements.add (back_set.get_min ());
      }
    }

    जर (lookahead_sets)
    {
      साठी (स्थिरांक स्वतः &look_set : *lookahead_sets)
      {
        जर (look_set.get_population () == 1)
          direct_requirements.add (look_set.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_set_t context_elements;

    जर (backtrack_sets)
    {
      साठी (स्थिरांक स्वतः &back_set : *backtrack_sets)
      {
        जर (back_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (back_set);
          filtered_set.subtract (direct_requirements);
          जर (unlikely (filtered_set.in_error ()))
            परत fail_invalid ();

          जर (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            जर (unlikely (set_idx == HB_CODEPOINT_INVALID))
              परत HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    जर (lookahead_sets)
    {
      साठी (स्थिरांक स्वतः &look_set : *lookahead_sets)
      {
        जर (look_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (look_set);
          filtered_set.subtract (direct_requirements);
          जर (unlikely (filtered_set.in_error ()))
            परत fail_invalid ();

          जर (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            जर (unlikely (set_idx == HB_CODEPOINT_INVALID))
              परत HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    context_elements.union_ (direct_requirements);
    जर (unlikely (direct_requirements.in_error () ||
                  context_elements.in_error ()))
      परत fail_invalid ();

    जर (context_elements.is_empty ())
      परत HB_CODEPOINT_INVALID;

    परत find_or_create_context_set (context_elements);
  }

  स्थिर uint64_t encode_edge_ref (hb_codepoint_t source, अचिन्हित पूर्णांक index)
  { परत (uint64_t (source) + 1) << 32 | index; }

  स्थिर शून्य decode_edge_ref (uint64_t ref,
			       hb_codepoint_t *source,
			       अचिन्हित पूर्णांक *index)
  {
    *source = (ref >> 32) - 1;
    *index = ref;
  }

  uint32_t edge_hash (hb_codepoint_t source, स्थिरांक hb_depend_edge_t &record) स्थिरांक
  {
    hb_depend_edge_किल्ली_t key (source, record.table_tag, record.layout_tag,
			      record.dependent, record.ligature_set,
			      record.context_set);
    परत key.hash ();
  }

  तार्किक resize_edge_slots ()
  {
    अचिन्हित पूर्णांक new_size = edge_slots.length ? edge_slots.length * 2 : 8;
    जर (unlikely (new_size < edge_slots.length))
      परत fail ();

    hb_vector_t<uint64_t> new_slots;
    जर (unlikely (!new_slots.resize_exact (new_size)))
      परत fail ();

    साठी (uint64_t ref : edge_slots)
    {
      जर (!ref)
	सुरूठेवा;

      hb_codepoint_t source;
      अचिन्हित पूर्णांक index;
      decode_edge_ref (ref, &source, &index);
      स्थिरांक hb_depend_edge_t &record = data.glyph_dependencies[source].dependencies[index];
      अचिन्हित पूर्णांक slot = edge_hash (source, record) & (new_size - 1);
      अचिन्हित पूर्णांक step = 0;
      पर्यंत (new_slots[slot])
	slot = (slot + ++step) & (new_size - 1);
      new_slots[slot] = ref;
    }

    edge_slots = std::move (new_slots);
    परत सत्य;
  }

  तार्किक add_edge (hb_codepoint_t source, स्थिरांक hb_depend_edge_t &record)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    जर (unlikely (!edge_slots.length ||
		  uint64_t (edge_population) * 3 / 2 >= edge_slots.length - 1))
      जर (unlikely (!resize_edge_slots ()))
	परत असत्य;

    अचिन्हित पूर्णांक slot = edge_hash (source, record) & (edge_slots.length - 1);
    अचिन्हित पूर्णांक step = 0;
    पर्यंत (uint64_t ref = edge_slots[slot])
    {
      hb_codepoint_t existing_source;
      अचिन्हित पूर्णांक existing_index;
      decode_edge_ref (ref, &existing_source, &existing_index);
      जर (existing_source == source &&
	  data.glyph_dependencies[source].dependencies[existing_index] == record)
	परत असत्य;
      slot = (slot + ++step) & (edge_slots.length - 1);
    }

    स्वतः &dependencies = data.glyph_dependencies[source].dependencies;
    अचिन्हित पूर्णांक index = dependencies.length;
    जर (unlikely (!dependencies.ढकला_or_fail (record)))
      परत fail ();

    edge_slots[slot] = encode_edge_ref (source, index);
    edge_population++;
    परत सत्य;
  }

  तार्किक add_depend_layout (hb_codepoint_t target, hb_tag_t table_tag,
                          hb_tag_t layout_tag,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                          hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    जर (target >= data.glyph_dependencies.length) {
      DEBUG_संदेश (SUBSET, nullptr, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(table_tag));
      परत असत्य;
    }

    परत add_edge (target, hb_depend_edge_t (table_tag, dependent, layout_tag,
					      lig_set, context_set, flags));
  }

  तार्किक add_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_index,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_set = HB_CODEPOINT_INVALID)
  {
    जर (context_set == HB_CODEPOINT_INVALID)
      context_set = current_context_set_index;
    hb_subset_depend_edge_flags_t flags = current_edge_flags;

    तार्किक any_added = असत्य;
    साठी (uint64_t entry : get_lookup_features (lookup_index)) {
      hb_tag_t t = (hb_tag_t) entry;
      जर (add_depend_layout (target, HB_OT_TAG_GSUB, t, dependent, lig_set, context_set, flags))
        any_added = सत्य;
    }
    परत any_added;
  }

  तार्किक init_lookup_features (अचिन्हित lookup_मोजणी)
  {
    परत check_success (lookup_feature_offsets.resize (lookup_मोजणी + 1));
  }

  तार्किक add_lookup_feature (hb_codepoint_t lookup_index, hb_tag_t feature_tag)
  {
    जर (feature_tag == HB_SET_मूल्य_INVALID)
      परत सत्य;

    जर (unlikely (!lookup_feature_offsets.length ||
		  lookup_index >= lookup_feature_offsets.length - 1))
      परत fail ();

    uint64_t entry = ((uint64_t) lookup_index << 32) | feature_tag;
    परत check_success (lookup_features.ढकला_or_fail (entry));
  }

  तार्किक finish_lookup_features ()
  {
    lookup_features.क्विक ([] (uint64_t a, uint64_t b) {
      परत a < b ? -1 : a > b ? 1 : 0;
    });

    अचिन्हित लिख = 0;
    साठी (uint64_t entry : lookup_features)
      जर (!लिख || entry != lookup_features[लिख - 1])
	lookup_features[लिख++] = entry;
    lookup_features.shrink (लिख, असत्य);

    अचिन्हित feature_index = 0;
    अचिन्हित lookup_मोजणी = lookup_feature_offsets.length - 1;
    साठी (अचिन्हित lookup_index = 0; lookup_index < lookup_मोजणी; lookup_index++)
    {
      lookup_feature_offsets[lookup_index] = feature_index;
      पर्यंत (feature_index < lookup_features.length &&
	     lookup_features[feature_index] >> 32 == lookup_index)
	feature_index++;
    }
    lookup_feature_offsets[lookup_मोजणी] = feature_index;
    परत सत्य;
  }

  hb_array_t<स्थिरांक uint64_t> get_lookup_features (hb_codepoint_t lookup_index) स्थिरांक
  {
    जर (unlikely (!lookup_feature_offsets.length ||
		  lookup_index >= lookup_feature_offsets.length - 1))
      परत {};

    अचिन्हित start = lookup_feature_offsets[lookup_index];
    अचिन्हित end = lookup_feature_offsets[lookup_index + 1];
    परत lookup_features.as_array ().sub_array (start, end - start);
  }

  शून्य add_depend (hb_codepoint_t target, hb_tag_t table_tag,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                   hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    add_depend_layout (target, table_tag, HB_CODEPOINT_INVALID, dependent, lig_set,
                       context_set, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { परत hb_map_get (&nominal_glyphs, cp); }

  HB_INTERNAL तार्किक compile (hb_face_t *face);

  तार्किक fail () { successful = असत्य; परत असत्य; }
  hb_codepoint_t fail_invalid () { fail (); परत HB_CODEPOINT_INVALID; }
  तार्किक check_success (तार्किक s) { successful = (successful && s); परत successful; }

  HB_INTERNAL शून्य get_gsub_dependencies (hb_face_t *face);

  तार्किक successful = सत्य;
  hb_set_t unicodes;
  hb_map_t nominal_glyphs;
  hb_vector_t<uint64_t> lookup_features;
  hb_vector_t<अचिन्हित> lookup_feature_offsets;
  hb_vector_t<uint64_t> edge_slots;
  अचिन्हित पूर्णांक edge_population = 0;
  hb_hashmap_t<स्थिरांक hb_set_t*, hb_codepoint_t> set_to_index;
  hb_vector_t<hb_codepoint_t> free_set_list;
  hb_codepoint_t current_context_set_index;
  hb_subset_depend_edge_flags_t current_edge_flags;

  hb_depend_data_t &data;
};


#endif /* HB_DEPEND_DATA_HH */
