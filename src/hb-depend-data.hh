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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#ifndef HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
#define HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ {
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
ಸಂರಚನಾ hb_depend_edge_t {
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

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_depend_edge_t &o) ಧ್ರುವ
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    ಪ್ರತ್ಯಾಗಮ table_tag == o.table_tag &&
           dependent == o.dependent &&
           layout_tag == o.layout_tag &&
           ligature_set == o.ligature_set &&
           context_set == o.context_set;
  }

  uint32_t hash () ಧ್ರುವ
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
    ಪ್ರತ್ಯಾಗಮ current;
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
ಸಂರಚನಾ hb_depend_edge_ಕೀಲಿ_t {
  hb_codepoint_t source;
  hb_depend_edge_t record;

  hb_depend_edge_ಕೀಲಿ_t () : source (0), record (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  hb_depend_edge_ಕೀಲಿ_t (hb_codepoint_t source,
              hb_tag_t table_tag,
              hb_tag_t layout_tag,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_set,
              hb_codepoint_t context_set)
    : source (source),
      record (table_tag, dependent, layout_tag, ligature_set, context_set, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_depend_edge_ಕೀಲಿ_t &o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ source == o.source && record == o.record;
  }

  uint32_t hash () ಧ್ರುವ
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (source);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ record.hash ();
    current = current * 16777619;
    ಪ್ರತ್ಯಾಗಮ current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
ಸಂರಚನಾ hb_depend_glyph_record_t {
  hb_vector_t<hb_depend_edge_t> dependencies;
};

/**
 * hb_depend_lookup_revmap_t:
 *
 * Internal structure tracking which features are associated with a lookup.
 * Used during GSUB dependency analysis to record the feature tags that
 * activate each lookup.
 */
ಸಂರಚನಾ hb_depend_lookup_revmap_t
{
  hb_depend_lookup_revmap_t () = ಸ್ವಭಾವ;
  explicit hb_depend_lookup_revmap_t (ತಾರ್ಕಿಕ full) : full (full) {}
  hb_depend_lookup_revmap_t (ಧ್ರುವ hb_depend_lookup_revmap_t &o) : full(o.full),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : full(o.full),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& operator = (ಧ್ರುವ hb_depend_lookup_revmap_t &o)
  {
    full = o.full;
    fv_indexes = o.fv_indexes;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  ತಾರ್ಕಿಕ full = ಸತ್ಯ;
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
ಸಂರಚನಾ hb_depend_data_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_vector_t<hb::unique_ptr<hb_set_t>> sets;

  hb_vector_t<hb_depend_glyph_record_t> glyph_dependencies;

  ಧ್ರುವ hb_set_t *get_set_from_index (hb_codepoint_t index)
  {
    ಯದಿ (index < sets.length)
      ಪ್ರತ್ಯಾಗಮ sets[index].get ();
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_glyph_entry_ಎಣಿಕೆ (hb_codepoint_t gid) ಧ್ರುವ
  {
    ಯದಿ (gid < glyph_dependencies.length)
      ಪ್ರತ್ಯಾಗಮ glyph_dependencies[gid].dependencies.length;
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  ತಾರ್ಕಿಕ get_glyph_entry (hb_codepoint_t gid, unsigned ಪೂರ್ಣಾಙ್ಕ index,
                        hb_tag_t *table_tag, hb_codepoint_t *dependent,
                        hb_tag_t *layout_tag, hb_codepoint_t *ligature_set,
                        hb_codepoint_t *context_set, uint8_t *flags)
  {
    ಯದಿ (gid < glyph_dependencies.length &&
        index < glyph_dependencies[gid].dependencies.length) {
      auto &d = glyph_dependencies[gid].dependencies[index];
      *table_tag = d.table_tag;
      *dependent = d.dependent;
      *layout_tag = d.layout_tag;
      *ligature_set = d.ligature_set;
      *context_set = d.context_set;
      ಯದಿ (flags) *flags = d.flags;
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ print ()
  {
    ಪ್ರತಿ (unsigned i = 0; i < glyph_dependencies.length; i++) {
      auto &gd = glyph_dependencies[i];
      ಯದಿ (!gd.dependencies.length)
        ಅನುವರ್ತ;
      printf ("GID %u:\n", i);
      ಪ್ರತಿ (auto &d : gd.dependencies) {
        ಯದಿ (d.table_tag == HB_OT_TAG_GSUB) {
          printf ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_tag), d.dependent);
          ಯದಿ (d.ligature_set != HB_CODEPOINT_INVALID)
            printf ("  (ligature)");
        } ಅನ್ಯಥಾ {
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
ಸಂರಚನಾ hb_depend_data_builder_t
{
  hb_depend_data_builder_t (hb_depend_data_t &data_)
    : current_context_set_index (HB_CODEPOINT_INVALID),
      current_edge_flags (HB_SUBSET_DEPEND_EDGE_FLAG_NONE),
      data (data_) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ಧ್ರುವ hb_set_t *get_set_from_index (hb_codepoint_t index)
  { ಪ್ರತ್ಯಾಗಮ data.get_set_from_index (index); }

  /* Discard an unused set that was allocated but had no edges added. */
  ಶೂನ್ಯ discard_set (hb_codepoint_t set_index)
  {
    ಯದಿ (set_index >= data.sets.length)
    {
      DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "Attempting to discard invalid set %u (max is %u)",
                 set_index, data.sets.length - 1);
      ಪ್ರತ್ಯಾಗಮ;
    }
    set_to_index.del (data.sets[set_index].get ());
    data.sets[set_index]->clear ();
    check_success (free_set_list.ತಳ್ಳು_or_fail (set_index));
  }

  hb_codepoint_t new_set (ಧ್ರುವ hb_set_t &set)
  {
    hb_codepoint_t set_index;

    ಯದಿ (free_set_list.length > 0)
    {
      set_index = free_set_list.ತೆಗೆ ();
      data.sets[set_index]->set (set);
      ಯದಿ (unlikely (data.sets[set_index]->in_error ()))
	ಪ್ರತ್ಯಾಗಮ fail_invalid ();
    }
    ಅನ್ಯಥಾ
    {
      set_index = data.sets.length;
      hb_set_t *new_set = hb_set_create ();
      ಯದಿ (unlikely (!new_set))
	ಪ್ರತ್ಯಾಗಮ fail_invalid ();
      new_set->set (set);
      ಯದಿ (unlikely (new_set->in_error ()))
      {
	hb_set_destroy (new_set);
	ಪ್ರತ್ಯಾಗಮ fail_invalid ();
      }
      ಯದಿ (unlikely (!data.sets.ತಳ್ಳು_or_fail (hb::unique_ptr<hb_set_t> {new_set})))
	ಪ್ರತ್ಯಾಗಮ fail_invalid ();
    }

    ಪ್ರತ್ಯಾಗಮ set_index;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t find_or_create_set (ಧ್ರುವ hb_set_t &set, ತಾರ್ಕಿಕ *created = nullptr)
  {
    hb_codepoint_t *existing_idx = nullptr;
    ಯದಿ (set_to_index.has (&set, &existing_idx))
    {
      ಯದಿ (created) *created = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ *existing_idx;
    }

    hb_codepoint_t new_idx = new_set (set);
    ಯದಿ (unlikely (new_idx == HB_CODEPOINT_INVALID))
      ಪ್ರತ್ಯಾಗಮ HB_CODEPOINT_INVALID;

    ಯದಿ (unlikely (!set_to_index.set (data.sets[new_idx].get (), new_idx)))
      ಪ್ರತ್ಯಾಗಮ fail_invalid ();
    ಯದಿ (created) *created = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ new_idx;
  }

  hb_codepoint_t find_or_create_context_set (ಧ್ರುವ hb_set_t &set)
  { ಪ್ರತ್ಯಾಗಮ find_or_create_set (set); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_set (ಧ್ರುವ hb_vector_t<hb_set_t> *backtrack_sets,
                                     ಧ್ರುವ hb_vector_t<hb_set_t> *lookahead_sets)
  {
    ಯದಿ ((!backtrack_sets || backtrack_sets->length == 0) &&
        (!lookahead_sets || lookahead_sets->length == 0))
      ಪ್ರತ್ಯಾಗಮ HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_set_t direct_requirements;

    ಯದಿ (backtrack_sets)
    {
      ಪ್ರತಿ (ಧ್ರುವ auto &back_set : *backtrack_sets)
      {
        ಯದಿ (back_set.get_population () == 1)
          direct_requirements.add (back_set.get_min ());
      }
    }

    ಯದಿ (lookahead_sets)
    {
      ಪ್ರತಿ (ಧ್ರುವ auto &look_set : *lookahead_sets)
      {
        ಯದಿ (look_set.get_population () == 1)
          direct_requirements.add (look_set.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_set_t context_elements;

    ಯದಿ (backtrack_sets)
    {
      ಪ್ರತಿ (ಧ್ರುವ auto &back_set : *backtrack_sets)
      {
        ಯದಿ (back_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (back_set);
          filtered_set.subtract (direct_requirements);
          ಯದಿ (unlikely (filtered_set.in_error ()))
            ಪ್ರತ್ಯಾಗಮ fail_invalid ();

          ಯದಿ (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            ಯದಿ (unlikely (set_idx == HB_CODEPOINT_INVALID))
              ಪ್ರತ್ಯಾಗಮ HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    ಯದಿ (lookahead_sets)
    {
      ಪ್ರತಿ (ಧ್ರುವ auto &look_set : *lookahead_sets)
      {
        ಯದಿ (look_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (look_set);
          filtered_set.subtract (direct_requirements);
          ಯದಿ (unlikely (filtered_set.in_error ()))
            ಪ್ರತ್ಯಾಗಮ fail_invalid ();

          ಯದಿ (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            ಯದಿ (unlikely (set_idx == HB_CODEPOINT_INVALID))
              ಪ್ರತ್ಯಾಗಮ HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    context_elements.union_ (direct_requirements);
    ಯದಿ (unlikely (direct_requirements.in_error () ||
                  context_elements.in_error ()))
      ಪ್ರತ್ಯಾಗಮ fail_invalid ();

    ಯದಿ (context_elements.is_empty ())
      ಪ್ರತ್ಯಾಗಮ HB_CODEPOINT_INVALID;

    ಪ್ರತ್ಯಾಗಮ find_or_create_context_set (context_elements);
  }

  ಸ್ಥಿರ uint64_t encode_edge_ref (hb_codepoint_t source, unsigned ಪೂರ್ಣಾಙ್ಕ index)
  { ಪ್ರತ್ಯಾಗಮ (uint64_t (source) + 1) << 32 | index; }

  ಸ್ಥಿರ ಶೂನ್ಯ decode_edge_ref (uint64_t ref,
			       hb_codepoint_t *source,
			       unsigned ಪೂರ್ಣಾಙ್ಕ *index)
  {
    *source = (ref >> 32) - 1;
    *index = ref;
  }

  uint32_t edge_hash (hb_codepoint_t source, ಧ್ರುವ hb_depend_edge_t &record) ಧ್ರುವ
  {
    hb_depend_edge_ಕೀಲಿ_t key (source, record.table_tag, record.layout_tag,
			      record.dependent, record.ligature_set,
			      record.context_set);
    ಪ್ರತ್ಯಾಗಮ key.hash ();
  }

  ತಾರ್ಕಿಕ resize_edge_slots ()
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ new_size = edge_slots.length ? edge_slots.length * 2 : 8;
    ಯದಿ (unlikely (new_size < edge_slots.length))
      ಪ್ರತ್ಯಾಗಮ fail ();

    hb_vector_t<uint64_t> new_slots;
    ಯದಿ (unlikely (!new_slots.resize_exact (new_size)))
      ಪ್ರತ್ಯಾಗಮ fail ();

    ಪ್ರತಿ (uint64_t ref : edge_slots)
    {
      ಯದಿ (!ref)
	ಅನುವರ್ತ;

      hb_codepoint_t source;
      unsigned ಪೂರ್ಣಾಙ್ಕ index;
      decode_edge_ref (ref, &source, &index);
      ಧ್ರುವ hb_depend_edge_t &record = data.glyph_dependencies[source].dependencies[index];
      unsigned ಪೂರ್ಣಾಙ್ಕ slot = edge_hash (source, record) & (new_size - 1);
      unsigned ಪೂರ್ಣಾಙ್ಕ step = 0;
      ಯಾವತ್ (new_slots[slot])
	slot = (slot + ++step) & (new_size - 1);
      new_slots[slot] = ref;
    }

    edge_slots = std::move (new_slots);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ add_edge (hb_codepoint_t source, ಧ್ರುವ hb_depend_edge_t &record)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    ಯದಿ (unlikely (!edge_slots.length ||
		  uint64_t (edge_population) * 3 / 2 >= edge_slots.length - 1))
      ಯದಿ (unlikely (!resize_edge_slots ()))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ಪೂರ್ಣಾಙ್ಕ slot = edge_hash (source, record) & (edge_slots.length - 1);
    unsigned ಪೂರ್ಣಾಙ್ಕ step = 0;
    ಯಾವತ್ (uint64_t ref = edge_slots[slot])
    {
      hb_codepoint_t existing_source;
      unsigned ಪೂರ್ಣಾಙ್ಕ existing_index;
      decode_edge_ref (ref, &existing_source, &existing_index);
      ಯದಿ (existing_source == source &&
	  data.glyph_dependencies[source].dependencies[existing_index] == record)
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      slot = (slot + ++step) & (edge_slots.length - 1);
    }

    auto &dependencies = data.glyph_dependencies[source].dependencies;
    unsigned ಪೂರ್ಣಾಙ್ಕ index = dependencies.length;
    ಯದಿ (unlikely (!dependencies.ತಳ್ಳು_or_fail (record)))
      ಪ್ರತ್ಯಾಗಮ fail ();

    edge_slots[slot] = encode_edge_ref (source, index);
    edge_population++;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ add_depend_layout (hb_codepoint_t target, hb_tag_t table_tag,
                          hb_tag_t layout_tag,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                          hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    ಯದಿ (target >= data.glyph_dependencies.length) {
      DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(table_tag));
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ add_edge (target, hb_depend_edge_t (table_tag, dependent, layout_tag,
					      lig_set, context_set, flags));
  }

  ತಾರ್ಕಿಕ add_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_index,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_set = HB_CODEPOINT_INVALID)
  {
    ಯದಿ (context_set == HB_CODEPOINT_INVALID)
      context_set = current_context_set_index;
    hb_subset_depend_edge_flags_t flags = current_edge_flags;

    ತಾರ್ಕಿಕ any_added = ಅಸತ್ಯ;
    ಪ್ರತಿ (uint64_t entry : get_lookup_features (lookup_index)) {
      hb_tag_t t = (hb_tag_t) entry;
      ಯದಿ (add_depend_layout (target, HB_OT_TAG_GSUB, t, dependent, lig_set, context_set, flags))
        any_added = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ any_added;
  }

  ತಾರ್ಕಿಕ init_lookup_features (unsigned lookup_ಎಣಿಕೆ)
  {
    ಪ್ರತ್ಯಾಗಮ check_success (lookup_feature_offsets.resize (lookup_ಎಣಿಕೆ + 1));
  }

  ತಾರ್ಕಿಕ add_lookup_feature (hb_codepoint_t lookup_index, hb_tag_t feature_tag)
  {
    ಯದಿ (feature_tag == HB_SET_ಮೌಲ್ಯ_INVALID)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಯದಿ (unlikely (!lookup_feature_offsets.length ||
		  lookup_index >= lookup_feature_offsets.length - 1))
      ಪ್ರತ್ಯಾಗಮ fail ();

    uint64_t entry = ((uint64_t) lookup_index << 32) | feature_tag;
    ಪ್ರತ್ಯಾಗಮ check_success (lookup_features.ತಳ್ಳು_or_fail (entry));
  }

  ತಾರ್ಕಿಕ finish_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      ಪ್ರತ್ಯಾಗಮ a < b ? -1 : a > b ? 1 : 0;
    });

    unsigned ಲಿಖ = 0;
    ಪ್ರತಿ (uint64_t entry : lookup_features)
      ಯದಿ (!ಲಿಖ || entry != lookup_features[ಲಿಖ - 1])
	lookup_features[ಲಿಖ++] = entry;
    lookup_features.shrink (ಲಿಖ, ಅಸತ್ಯ);

    unsigned feature_index = 0;
    unsigned lookup_ಎಣಿಕೆ = lookup_feature_offsets.length - 1;
    ಪ್ರತಿ (unsigned lookup_index = 0; lookup_index < lookup_ಎಣಿಕೆ; lookup_index++)
    {
      lookup_feature_offsets[lookup_index] = feature_index;
      ಯಾವತ್ (feature_index < lookup_features.length &&
	     lookup_features[feature_index] >> 32 == lookup_index)
	feature_index++;
    }
    lookup_feature_offsets[lookup_ಎಣಿಕೆ] = feature_index;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  hb_array_t<ಧ್ರುವ uint64_t> get_lookup_features (hb_codepoint_t lookup_index) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!lookup_feature_offsets.length ||
		  lookup_index >= lookup_feature_offsets.length - 1))
      ಪ್ರತ್ಯಾಗಮ {};

    unsigned start = lookup_feature_offsets[lookup_index];
    unsigned end = lookup_feature_offsets[lookup_index + 1];
    ಪ್ರತ್ಯಾಗಮ lookup_features.as_array ().sub_array (start, end - start);
  }

  ಶೂನ್ಯ add_depend (hb_codepoint_t target, hb_tag_t table_tag,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                   hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    add_depend_layout (target, table_tag, HB_CODEPOINT_INVALID, dependent, lig_set,
                       context_set, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { ಪ್ರತ್ಯಾಗಮ hb_map_get (&nominal_glyphs, cp); }

  HB_INTERNAL ತಾರ್ಕಿಕ compile (hb_face_t *face);

  ತಾರ್ಕಿಕ fail () { successful = ಅಸತ್ಯ; ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  hb_codepoint_t fail_invalid () { fail (); ಪ್ರತ್ಯಾಗಮ HB_CODEPOINT_INVALID; }
  ತಾರ್ಕಿಕ check_success (ತಾರ್ಕಿಕ s) { successful = (successful && s); ಪ್ರತ್ಯಾಗಮ successful; }

  HB_INTERNAL ಶೂನ್ಯ get_gsub_dependencies (hb_face_t *face);

  ತಾರ್ಕಿಕ successful = ಸತ್ಯ;
  hb_set_t unicodes;
  hb_map_t nominal_glyphs;
  hb_vector_t<uint64_t> lookup_features;
  hb_vector_t<unsigned> lookup_feature_offsets;
  hb_vector_t<uint64_t> edge_slots;
  unsigned ಪೂರ್ಣಾಙ್ಕ edge_population = 0;
  hb_hashmap_t<ಧ್ರುವ hb_set_t*, hb_codepoint_t> set_to_index;
  hb_vector_t<hb_codepoint_t> free_set_list;
  hb_codepoint_t current_context_set_index;
  hb_subset_depend_edge_flags_t current_edge_flags;

  hb_depend_data_t &data;
};


#endif /* HB_DEPEND_DATA_HH */
