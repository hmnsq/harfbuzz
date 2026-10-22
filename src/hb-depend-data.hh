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

#ضمن "hb.hh"

#ضمن "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#ifndef HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
#define HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
تعريفنوع تعداد {
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
بنية hb_depend_edge_t {
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

  منطقي operator == (ثابت hb_depend_edge_t &o) ثابت
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    החזר table_tag == o.table_tag &&
           dependent == o.dependent &&
           layout_tag == o.layout_tag &&
           ligature_set == o.ligature_set &&
           context_set == o.context_set;
  }

  uint32_t hash () ثابت
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
    החזר current;
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
بنية hb_depend_edge_key_t {
  hb_codepoint_t source;
  hb_depend_edge_t record;

  hb_depend_edge_key_t () : source (0), record (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  hb_depend_edge_key_t (hb_codepoint_t source,
              hb_tag_t table_tag,
              hb_tag_t layout_tag,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_set,
              hb_codepoint_t context_set)
    : source (source),
      record (table_tag, dependent, layout_tag, ligature_set, context_set, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  منطقي operator == (ثابت hb_depend_edge_key_t &o) ثابت
  {
    החזר source == o.source && record == o.record;
  }

  uint32_t hash () ثابت
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (source);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ record.hash ();
    current = current * 16777619;
    החזר current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
بنية hb_depend_glyph_record_t {
  hb_vector_t<hb_depend_edge_t> dependencies;
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
  explicit hb_depend_lookup_revmap_t (منطقي full) : full (full) {}
  hb_depend_lookup_revmap_t (ثابت hb_depend_lookup_revmap_t &o) : full(o.full),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : full(o.full),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& operator = (ثابت hb_depend_lookup_revmap_t &o)
  {
    full = o.full;
    fv_indexes = o.fv_indexes;
    החזר *هذا;
  }

  منطقي full = אמת;
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
بنية hb_depend_data_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_vector_t<hb::unique_ptr<hb_set_t>> sets;

  hb_vector_t<hb_depend_glyph_record_t> glyph_dependencies;

  ثابت hb_set_t *get_set_from_index (hb_codepoint_t index)
  {
    אם (index < sets.length)
      החזר sets[index].get ();
    החזר nullptr;
  }

  unsigned صحيح get_glyph_entry_count (hb_codepoint_t gid) ثابت
  {
    אם (gid < glyph_dependencies.length)
      החזר glyph_dependencies[gid].dependencies.length;
    החזר 0;
  }

  منطقي get_glyph_entry (hb_codepoint_t gid, unsigned صحيح index,
                        hb_tag_t *table_tag, hb_codepoint_t *dependent,
                        hb_tag_t *layout_tag, hb_codepoint_t *ligature_set,
                        hb_codepoint_t *context_set, uint8_t *flags)
  {
    אם (gid < glyph_dependencies.length &&
        index < glyph_dependencies[gid].dependencies.length) {
      auto &d = glyph_dependencies[gid].dependencies[index];
      *table_tag = d.table_tag;
      *dependent = d.dependent;
      *layout_tag = d.layout_tag;
      *ligature_set = d.ligature_set;
      *context_set = d.context_set;
      אם (flags) *flags = d.flags;
      החזר אמת;
    }
    החזר שקר;
  }

  فراغ print ()
  {
    עבור (unsigned i = 0; i < glyph_dependencies.length; i++) {
      auto &gd = glyph_dependencies[i];
      אם (!gd.dependencies.length)
        استمر;
      printf ("GID %u:\n", i);
      עבור (auto &d : gd.dependencies) {
        אם (d.table_tag == HB_OT_TAG_GSUB) {
          printf ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_tag), d.dependent);
          אם (d.ligature_set != HB_CODEPOINT_INVALID)
            printf ("  (ligature)");
        } אחרת {
          printf ("  %c%c%c%c -> %u", HB_UNTAG(d.table_tag), d.dependent);
        }
        printf ("\n");
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
بنية hb_depend_data_builder_t
{
  hb_depend_data_builder_t (hb_depend_data_t &data_)
    : current_context_set_index (HB_CODEPOINT_INVALID),
      current_edge_flags (HB_SUBSET_DEPEND_EDGE_FLAG_NONE),
      data (data_) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ثابت hb_set_t *get_set_from_index (hb_codepoint_t index)
  { החזר data.get_set_from_index (index); }

  /* Discard an unused set that was allocated but had no edges added. */
  فراغ discard_set (hb_codepoint_t set_index)
  {
    אם (set_index >= data.sets.length)
    {
      DEBUG_MSG (SUBSET, nullptr, "Attempting to discard invalid set %u (max is %u)",
                 set_index, data.sets.length - 1);
      החזר;
    }
    set_to_index.del (data.sets[set_index].get ());
    data.sets[set_index]->clear ();
    check_success (free_set_list.push_or_fail (set_index));
  }

  hb_codepoint_t new_set (ثابت hb_set_t &set)
  {
    hb_codepoint_t set_index;

    אם (free_set_list.length > 0)
    {
      set_index = free_set_list.pop ();
      data.sets[set_index]->set (set);
      אם (unlikely (data.sets[set_index]->in_error ()))
	החזר fail_invalid ();
    }
    אחרת
    {
      set_index = data.sets.length;
      hb_set_t *new_set = hb_set_create ();
      אם (unlikely (!new_set))
	החזר fail_invalid ();
      new_set->set (set);
      אם (unlikely (new_set->in_error ()))
      {
	hb_set_destroy (new_set);
	החזר fail_invalid ();
      }
      אם (unlikely (!data.sets.push_or_fail (hb::unique_ptr<hb_set_t> {new_set})))
	החזר fail_invalid ();
    }

    החזר set_index;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t find_or_create_set (ثابت hb_set_t &set, منطقي *created = nullptr)
  {
    hb_codepoint_t *existing_idx = nullptr;
    אם (set_to_index.has (&set, &existing_idx))
    {
      אם (created) *created = שקר;
      החזר *existing_idx;
    }

    hb_codepoint_t new_idx = new_set (set);
    אם (unlikely (new_idx == HB_CODEPOINT_INVALID))
      החזר HB_CODEPOINT_INVALID;

    אם (unlikely (!set_to_index.set (data.sets[new_idx].get (), new_idx)))
      החזר fail_invalid ();
    אם (created) *created = אמת;
    החזר new_idx;
  }

  hb_codepoint_t find_or_create_context_set (ثابت hb_set_t &set)
  { החזר find_or_create_set (set); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_set (ثابت hb_vector_t<hb_set_t> *backtrack_sets,
                                     ثابت hb_vector_t<hb_set_t> *lookahead_sets)
  {
    אם ((!backtrack_sets || backtrack_sets->length == 0) &&
        (!lookahead_sets || lookahead_sets->length == 0))
      החזר HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_set_t direct_requirements;

    אם (backtrack_sets)
    {
      עבור (ثابت auto &back_set : *backtrack_sets)
      {
        אם (back_set.get_population () == 1)
          direct_requirements.add (back_set.get_min ());
      }
    }

    אם (lookahead_sets)
    {
      עבור (ثابت auto &look_set : *lookahead_sets)
      {
        אם (look_set.get_population () == 1)
          direct_requirements.add (look_set.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_set_t context_elements;

    אם (backtrack_sets)
    {
      עבור (ثابت auto &back_set : *backtrack_sets)
      {
        אם (back_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (back_set);
          filtered_set.subtract (direct_requirements);
          אם (unlikely (filtered_set.in_error ()))
            החזר fail_invalid ();

          אם (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            אם (unlikely (set_idx == HB_CODEPOINT_INVALID))
              החזר HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    אם (lookahead_sets)
    {
      עבור (ثابت auto &look_set : *lookahead_sets)
      {
        אם (look_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (look_set);
          filtered_set.subtract (direct_requirements);
          אם (unlikely (filtered_set.in_error ()))
            החזר fail_invalid ();

          אם (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            אם (unlikely (set_idx == HB_CODEPOINT_INVALID))
              החזר HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    context_elements.union_ (direct_requirements);
    אם (unlikely (direct_requirements.in_error () ||
                  context_elements.in_error ()))
      החזר fail_invalid ();

    אם (context_elements.is_empty ())
      החזר HB_CODEPOINT_INVALID;

    החזר find_or_create_context_set (context_elements);
  }

  ساكن uint64_t encode_edge_ref (hb_codepoint_t source, unsigned صحيح index)
  { החזר (uint64_t (source) + 1) << 32 | index; }

  ساكن فراغ decode_edge_ref (uint64_t ref,
			       hb_codepoint_t *source,
			       unsigned صحيح *index)
  {
    *source = (ref >> 32) - 1;
    *index = ref;
  }

  uint32_t edge_hash (hb_codepoint_t source, ثابت hb_depend_edge_t &record) ثابت
  {
    hb_depend_edge_key_t key (source, record.table_tag, record.layout_tag,
			      record.dependent, record.ligature_set,
			      record.context_set);
    החזר key.hash ();
  }

  منطقي resize_edge_slots ()
  {
    unsigned صحيح new_size = edge_slots.length ? edge_slots.length * 2 : 8;
    אם (unlikely (new_size < edge_slots.length))
      החזר fail ();

    hb_vector_t<uint64_t> new_slots;
    אם (unlikely (!new_slots.resize_exact (new_size)))
      החזר fail ();

    עבור (uint64_t ref : edge_slots)
    {
      אם (!ref)
	استمر;

      hb_codepoint_t source;
      unsigned صحيح index;
      decode_edge_ref (ref, &source, &index);
      ثابت hb_depend_edge_t &record = data.glyph_dependencies[source].dependencies[index];
      unsigned صحيح slot = edge_hash (source, record) & (new_size - 1);
      unsigned صحيح step = 0;
      כלעוד (new_slots[slot])
	slot = (slot + ++step) & (new_size - 1);
      new_slots[slot] = ref;
    }

    edge_slots = std::move (new_slots);
    החזר אמת;
  }

  منطقي add_edge (hb_codepoint_t source, ثابت hb_depend_edge_t &record)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    אם (unlikely (!edge_slots.length ||
		  uint64_t (edge_population) * 3 / 2 >= edge_slots.length - 1))
      אם (unlikely (!resize_edge_slots ()))
	החזר שקר;

    unsigned صحيح slot = edge_hash (source, record) & (edge_slots.length - 1);
    unsigned صحيح step = 0;
    כלעוד (uint64_t ref = edge_slots[slot])
    {
      hb_codepoint_t existing_source;
      unsigned صحيح existing_index;
      decode_edge_ref (ref, &existing_source, &existing_index);
      אם (existing_source == source &&
	  data.glyph_dependencies[source].dependencies[existing_index] == record)
	החזר שקר;
      slot = (slot + ++step) & (edge_slots.length - 1);
    }

    auto &dependencies = data.glyph_dependencies[source].dependencies;
    unsigned صحيح index = dependencies.length;
    אם (unlikely (!dependencies.push_or_fail (record)))
      החזר fail ();

    edge_slots[slot] = encode_edge_ref (source, index);
    edge_population++;
    החזר אמת;
  }

  منطقي add_depend_layout (hb_codepoint_t target, hb_tag_t table_tag,
                          hb_tag_t layout_tag,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                          hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    אם (target >= data.glyph_dependencies.length) {
      DEBUG_MSG (SUBSET, nullptr, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(table_tag));
      החזר שקר;
    }

    החזר add_edge (target, hb_depend_edge_t (table_tag, dependent, layout_tag,
					      lig_set, context_set, flags));
  }

  منطقي add_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_index,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_set = HB_CODEPOINT_INVALID)
  {
    אם (context_set == HB_CODEPOINT_INVALID)
      context_set = current_context_set_index;
    hb_subset_depend_edge_flags_t flags = current_edge_flags;

    منطقي any_added = שקר;
    עבור (uint64_t entry : get_lookup_features (lookup_index)) {
      hb_tag_t t = (hb_tag_t) entry;
      אם (add_depend_layout (target, HB_OT_TAG_GSUB, t, dependent, lig_set, context_set, flags))
        any_added = אמת;
    }
    החזר any_added;
  }

  منطقي init_lookup_features (unsigned lookup_count)
  {
    החזר check_success (lookup_feature_offsets.resize (lookup_count + 1));
  }

  منطقي add_lookup_feature (hb_codepoint_t lookup_index, hb_tag_t feature_tag)
  {
    אם (feature_tag == HB_SET_VALUE_INVALID)
      החזר אמת;

    אם (unlikely (!lookup_feature_offsets.length ||
		  lookup_index >= lookup_feature_offsets.length - 1))
      החזר fail ();

    uint64_t entry = ((uint64_t) lookup_index << 32) | feature_tag;
    החזר check_success (lookup_features.push_or_fail (entry));
  }

  منطقي finish_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      החזר a < b ? -1 : a > b ? 1 : 0;
    });

    unsigned اكتب = 0;
    עבור (uint64_t entry : lookup_features)
      אם (!اكتب || entry != lookup_features[اكتب - 1])
	lookup_features[اكتب++] = entry;
    lookup_features.shrink (اكتب, שקר);

    unsigned feature_index = 0;
    unsigned lookup_count = lookup_feature_offsets.length - 1;
    עבור (unsigned lookup_index = 0; lookup_index < lookup_count; lookup_index++)
    {
      lookup_feature_offsets[lookup_index] = feature_index;
      כלעוד (feature_index < lookup_features.length &&
	     lookup_features[feature_index] >> 32 == lookup_index)
	feature_index++;
    }
    lookup_feature_offsets[lookup_count] = feature_index;
    החזר אמת;
  }

  hb_array_t<ثابت uint64_t> get_lookup_features (hb_codepoint_t lookup_index) ثابت
  {
    אם (unlikely (!lookup_feature_offsets.length ||
		  lookup_index >= lookup_feature_offsets.length - 1))
      החזר {};

    unsigned start = lookup_feature_offsets[lookup_index];
    unsigned end = lookup_feature_offsets[lookup_index + 1];
    החזר lookup_features.as_array ().sub_array (start, end - start);
  }

  فراغ add_depend (hb_codepoint_t target, hb_tag_t table_tag,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                   hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    add_depend_layout (target, table_tag, HB_CODEPOINT_INVALID, dependent, lig_set,
                       context_set, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { החזר hb_map_get (&nominal_glyphs, cp); }

  HB_INTERNAL منطقي compile (hb_face_t *face);

  منطقي fail () { successful = שקר; החזר שקר; }
  hb_codepoint_t fail_invalid () { fail (); החזר HB_CODEPOINT_INVALID; }
  منطقي check_success (منطقي s) { successful = (successful && s); החזר successful; }

  HB_INTERNAL فراغ get_gsub_dependencies (hb_face_t *face);

  منطقي successful = אמת;
  hb_set_t unicodes;
  hb_map_t nominal_glyphs;
  hb_vector_t<uint64_t> lookup_features;
  hb_vector_t<unsigned> lookup_feature_offsets;
  hb_vector_t<uint64_t> edge_slots;
  unsigned صحيح edge_population = 0;
  hb_hashmap_t<ثابت hb_set_t*, hb_codepoint_t> set_to_index;
  hb_vector_t<hb_codepoint_t> free_set_list;
  hb_codepoint_t current_context_set_index;
  hb_subset_depend_edge_flags_t current_edge_flags;

  hb_depend_data_t &data;
};


#endif /* HB_DEPEND_DATA_HH */
