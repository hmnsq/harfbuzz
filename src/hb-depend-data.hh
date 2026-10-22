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

#ifndef HB_DEPEND_তথ্য_HH
#define HB_DEPEND_তথ্য_HH

/* This file exists to break include cycles: hb-ot-layout-gsubgpos.hh needs
 * hb_depend_data_builder_t for hb_depend_context_t, but hb-depend.hh pulls
 * in table headers that include hb-ot-layout-gsubgpos.hh. */

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-multimap.hh"

/* hb_subset_depend_edge_flags_t is defined in hb-depend.h (public header).
 * We redeclare it here under the same include-guard so internal code that
 * includes hb-depend-data.hh without going through hb-depend.h still gets
 * the type. The definitions must stay in sync. */
#ifndef HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
#define HB_SUBSET_DEPEND_EDGE_FLAGS_T_DEFINED
প্রকারলেখ গণনা {
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
কাঠামো hb_depend_edge_t {
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

  যৌক্তিক operator == (ধ্রুবক hb_depend_edge_t &o) ধ্রুবক
  {
    /* NOTE: flags intentionally excluded from equality comparison.
     * Flags are metadata about how an edge was discovered (e.g.,
     * FROM_CONTEXT_POSITION, FROM_NESTED_CONTEXT), not part of the
     * edge's identity. Multiple discoveries of the same edge via
     * different paths should be treated as duplicates. */
    ফেরত table_tag == o.table_tag &&
           dependent == o.dependent &&
           layout_tag == o.layout_tag &&
           ligature_set == o.ligature_set &&
           context_set == o.context_set;
  }

  uint32_t hash () ধ্রুবক
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
    ফেরত current;
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
কাঠামো hb_depend_edge_চাবি_t {
  hb_codepoint_t source;
  hb_depend_edge_t record;

  hb_depend_edge_চাবি_t () : source (0), record (0, 0, 0, HB_CODEPOINT_INVALID, HB_CODEPOINT_INVALID, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  hb_depend_edge_চাবি_t (hb_codepoint_t source,
              hb_tag_t table_tag,
              hb_tag_t layout_tag,
              hb_codepoint_t dependent,
              hb_codepoint_t ligature_set,
              hb_codepoint_t context_set)
    : source (source),
      record (table_tag, dependent, layout_tag, ligature_set, context_set, HB_SUBSET_DEPEND_EDGE_FLAG_NONE) {}

  যৌক্তিক operator == (ধ্রুবক hb_depend_edge_চাবি_t &o) ধ্রুবক
  {
    ফেরত source == o.source && record == o.record;
  }

  uint32_t hash () ধ্রুবক
  {
    /* Combine source hash with record hash */
    uint32_t current = 0x84222325;  /* FNV-1a offset basis */
    current = current ^ hb_hash (source);
    current = current * 16777619;   /* FNV-1a prime */
    current = current ^ record.hash ();
    current = current * 16777619;
    ফেরত current;
  }
};

/**
 * hb_depend_glyph_record_t:
 *
 * Internal structure holding all dependency edges for a single glyph.
 */
কাঠামো hb_depend_glyph_record_t {
  hb_vector_t<hb_depend_edge_t> dependencies;
};

/**
 * hb_depend_lookup_revmap_t:
 *
 * Internal structure tracking which features are associated with a lookup.
 * Used during GSUB dependency analysis to record the feature tags that
 * activate each lookup.
 */
কাঠামো hb_depend_lookup_revmap_t
{
  hb_depend_lookup_revmap_t () = পূর্বনির্ধারিত;
  explicit hb_depend_lookup_revmap_t (যৌক্তিক full) : full (full) {}
  hb_depend_lookup_revmap_t (ধ্রুবক hb_depend_lookup_revmap_t &o) : full(o.full),
                                                                     fv_indexes(o.fv_indexes) {}
  hb_depend_lookup_revmap_t (hb_depend_lookup_revmap_t &&o) : full(o.full),
                                                                fv_indexes(std::move(o.fv_indexes)) {}
  hb_depend_lookup_revmap_t& operator = (ধ্রুবক hb_depend_lookup_revmap_t &o)
  {
    full = o.full;
    fv_indexes = o.fv_indexes;
    ফেরত *এই;
  }

  যৌক্তিক full = সত্য;
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
কাঠামো hb_depend_তথ্য_t
{
  /* Set storage: vector of heap-allocated sets (both ligature and context sets).
   * Using unique_ptr follows HarfBuzz pattern and provides stable pointers. */
  hb_vector_t<hb::unique_ptr<hb_set_t>> sets;

  hb_vector_t<hb_depend_glyph_record_t> glyph_dependencies;

  ধ্রুবক hb_set_t *get_set_from_সূচক (hb_codepoint_t সূচক_2)
  {
    যদি (সূচক_2 < sets.দৈর্ঘ্য)
      ফেরত sets[সূচক_2].get ();
    ফেরত nullptr;
  }

  unsigned পূর্ণসংখ্যা get_glyph_entry_গুনতি (hb_codepoint_t gid) ধ্রুবক
  {
    যদি (gid < glyph_dependencies.দৈর্ঘ্য)
      ফেরত glyph_dependencies[gid].dependencies.দৈর্ঘ্য;
    ফেরত 0;
  }

  যৌক্তিক get_glyph_entry (hb_codepoint_t gid, unsigned পূর্ণসংখ্যা সূচক_2,
                        hb_tag_t *table_tag, hb_codepoint_t *dependent,
                        hb_tag_t *layout_tag, hb_codepoint_t *ligature_set,
                        hb_codepoint_t *context_set, uint8_t *flags)
  {
    যদি (gid < glyph_dependencies.দৈর্ঘ্য &&
        সূচক_2 < glyph_dependencies[gid].dependencies.দৈর্ঘ্য) {
      auto &d = glyph_dependencies[gid].dependencies[সূচক_2];
      *table_tag = d.table_tag;
      *dependent = d.dependent;
      *layout_tag = d.layout_tag;
      *ligature_set = d.ligature_set;
      *context_set = d.context_set;
      যদি (flags) *flags = d.flags;
      ফেরত সত্য;
    }
    ফেরত মিথ্যা;
  }

  শূন্য print ()
  {
    জন্য (unsigned i = 0; i < glyph_dependencies.দৈর্ঘ্য; i++) {
      auto &gd = glyph_dependencies[i];
      যদি (!gd.dependencies.দৈর্ঘ্য)
        চালু;
      printf ("GID %u:\n", i);
      জন্য (auto &d : gd.dependencies) {
        যদি (d.table_tag == HB_OT_TAG_GSUB) {
          printf ("  layout %c%c%c%c -> %u", HB_UNTAG(d.layout_tag), d.dependent);
          যদি (d.ligature_set != HB_CODEPOINT_INVALID)
            printf ("  (ligature)");
        } অন্যথা {
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
কাঠামো hb_depend_তথ্য_builder_t
{
  hb_depend_তথ্য_builder_t (hb_depend_তথ্য_t &তথ্য_2)
    : current_context_set_সূচক (HB_CODEPOINT_INVALID),
      current_edge_flags (HB_SUBSET_DEPEND_EDGE_FLAG_NONE),
      তথ্য (তথ্য_2) {}

  /* Forward to data for use during construction (e.g. from hb_depend_context_t) */
  ধ্রুবক hb_set_t *get_set_from_সূচক (hb_codepoint_t সূচক_2)
  { ফেরত তথ্য.get_set_from_সূচক (সূচক_2); }

  /* Discard an unused set that was allocated but had no edges added. */
  শূন্য discard_set (hb_codepoint_t set_সূচক)
  {
    যদি (set_সূচক >= তথ্য.sets.দৈর্ঘ্য)
    {
      DEBUG_বার্তা (SUBSET, nullptr, "Attempting to discard invalid set %u (max is %u)",
                 set_সূচক, তথ্য.sets.দৈর্ঘ্য - 1);
      ফেরত;
    }
    set_to_সূচক.del (তথ্য.sets[set_সূচক].get ());
    তথ্য.sets[set_সূচক]->clear ();
    check_success (free_set_list.ঠেলো_or_fail (set_সূচক));
  }

  hb_codepoint_t new_set (ধ্রুবক hb_set_t &set)
  {
    hb_codepoint_t set_সূচক;

    যদি (free_set_list.দৈর্ঘ্য > 0)
    {
      set_সূচক = free_set_list.বের_করো ();
      তথ্য.sets[set_সূচক]->set (set);
      যদি (unlikely (তথ্য.sets[set_সূচক]->in_error ()))
	ফেরত fail_invalid ();
    }
    অন্যথা
    {
      set_সূচক = তথ্য.sets.দৈর্ঘ্য;
      hb_set_t *new_set = hb_set_create ();
      যদি (unlikely (!new_set))
	ফেরত fail_invalid ();
      new_set->set (set);
      যদি (unlikely (new_set->in_error ()))
      {
	hb_set_destroy (new_set);
	ফেরত fail_invalid ();
      }
      যদি (unlikely (!তথ্য.sets.ঠেলো_or_fail (hb::unique_ptr<hb_set_t> {new_set})))
	ফেরত fail_invalid ();
    }

    ফেরত set_সূচক;
  }

  /* Find an existing dependency set with the same contents, or create one. */
  hb_codepoint_t find_or_create_set (ধ্রুবক hb_set_t &set, যৌক্তিক *created = nullptr)
  {
    hb_codepoint_t *existing_idx = nullptr;
    যদি (set_to_সূচক.has (&set, &existing_idx))
    {
      যদি (created) *created = মিথ্যা;
      ফেরত *existing_idx;
    }

    hb_codepoint_t new_idx = new_set (set);
    যদি (unlikely (new_idx == HB_CODEPOINT_INVALID))
      ফেরত HB_CODEPOINT_INVALID;

    যদি (unlikely (!set_to_সূচক.set (তথ্য.sets[new_idx].get (), new_idx)))
      ফেরত fail_invalid ();
    যদি (created) *created = সত্য;
    ফেরত new_idx;
  }

  hb_codepoint_t find_or_create_context_set (ধ্রুবক hb_set_t &set)
  { ফেরত find_or_create_set (set); }

  /* Build a context set from context information.
   * Encodes backtrack and lookahead requirements as a flattened set.
   * Returns HB_CODEPOINT_INVALID if no context.
   *
   * To ensure canonical encoding and avoid redundancy:
   * 1. First pass: collect all direct (single-glyph) requirements
   * 2. Second pass: create disjunction sets, subtracting direct requirements
   * 3. Combine: add direct requirements and filtered disjunction references
   */
  hb_codepoint_t build_context_set (ধ্রুবক hb_vector_t<hb_set_t> *backtrack_sets,
                                     ধ্রুবক hb_vector_t<hb_set_t> *lookahead_sets)
  {
    যদি ((!backtrack_sets || backtrack_sets->দৈর্ঘ্য == 0) &&
        (!lookahead_sets || lookahead_sets->দৈর্ঘ্য == 0))
      ফেরত HB_CODEPOINT_INVALID;

    /* First pass: collect all direct (single-glyph) requirements */
    hb_set_t direct_requirements;

    যদি (backtrack_sets)
    {
      জন্য (ধ্রুবক auto &back_set : *backtrack_sets)
      {
        যদি (back_set.get_population () == 1)
          direct_requirements.add (back_set.get_min ());
      }
    }

    যদি (lookahead_sets)
    {
      জন্য (ধ্রুবক auto &look_set : *lookahead_sets)
      {
        যদি (look_set.get_population () == 1)
          direct_requirements.add (look_set.get_min ());
      }
    }

    /* Second pass: create disjunction sets, filtering out direct requirements */
    hb_set_t context_elements;

    যদি (backtrack_sets)
    {
      জন্য (ধ্রুবক auto &back_set : *backtrack_sets)
      {
        যদি (back_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (back_set);
          filtered_set.subtract (direct_requirements);
          যদি (unlikely (filtered_set.in_error ()))
            ফেরত fail_invalid ();

          যদি (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
              ফেরত HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    যদি (lookahead_sets)
    {
      জন্য (ধ্রুবক auto &look_set : *lookahead_sets)
      {
        যদি (look_set.get_population () > 1)
        {
          hb_set_t filtered_set;
          filtered_set.set (look_set);
          filtered_set.subtract (direct_requirements);
          যদি (unlikely (filtered_set.in_error ()))
            ফেরত fail_invalid ();

          যদি (!filtered_set.is_empty ())
          {
            hb_codepoint_t set_idx = find_or_create_context_set (filtered_set);
            যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
              ফেরত HB_CODEPOINT_INVALID;
            context_elements.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
          }
        }
      }
    }

    context_elements.union_ (direct_requirements);
    যদি (unlikely (direct_requirements.in_error () ||
                  context_elements.in_error ()))
      ফেরত fail_invalid ();

    যদি (context_elements.is_empty ())
      ফেরত HB_CODEPOINT_INVALID;

    ফেরত find_or_create_context_set (context_elements);
  }

  স্থির uint64_t encode_edge_ref (hb_codepoint_t source, unsigned পূর্ণসংখ্যা সূচক_2)
  { ফেরত (uint64_t (source) + 1) << 32 | সূচক_2; }

  স্থির শূন্য decode_edge_ref (uint64_t ref,
			       hb_codepoint_t *source,
			       unsigned পূর্ণসংখ্যা *সূচক_2)
  {
    *source = (ref >> 32) - 1;
    *সূচক_2 = ref;
  }

  uint32_t edge_hash (hb_codepoint_t source, ধ্রুবক hb_depend_edge_t &record) ধ্রুবক
  {
    hb_depend_edge_চাবি_t key (source, record.table_tag, record.layout_tag,
			      record.dependent, record.ligature_set,
			      record.context_set);
    ফেরত key.hash ();
  }

  যৌক্তিক resize_edge_slots ()
  {
    unsigned পূর্ণসংখ্যা new_আকার = edge_slots.দৈর্ঘ্য ? edge_slots.দৈর্ঘ্য * 2 : 8;
    যদি (unlikely (new_আকার < edge_slots.দৈর্ঘ্য))
      ফেরত fail ();

    hb_vector_t<uint64_t> new_slots;
    যদি (unlikely (!new_slots.resize_exact (new_আকার)))
      ফেরত fail ();

    জন্য (uint64_t ref : edge_slots)
    {
      যদি (!ref)
	চালু;

      hb_codepoint_t source;
      unsigned পূর্ণসংখ্যা সূচক_2;
      decode_edge_ref (ref, &source, &সূচক_2);
      ধ্রুবক hb_depend_edge_t &record = তথ্য.glyph_dependencies[source].dependencies[সূচক_2];
      unsigned পূর্ণসংখ্যা slot = edge_hash (source, record) & (new_আকার - 1);
      unsigned পূর্ণসংখ্যা step = 0;
      যখন (new_slots[slot])
	slot = (slot + ++step) & (new_আকার - 1);
      new_slots[slot] = ref;
    }

    edge_slots = std::move (new_slots);
    ফেরত সত্য;
  }

  যৌক্তিক add_edge (hb_codepoint_t source, ধ্রুবক hb_depend_edge_t &record)
  {
    /* Store only a source and per-glyph edge index in each bucket.  The edge
     * itself already lives in the output vector, where it can be compared to
     * resolve hash collisions exactly.  Adding one to the encoded source
     * reserves zero as the empty-bucket value. */
    যদি (unlikely (!edge_slots.দৈর্ঘ্য ||
		  uint64_t (edge_population) * 3 / 2 >= edge_slots.দৈর্ঘ্য - 1))
      যদি (unlikely (!resize_edge_slots ()))
	ফেরত মিথ্যা;

    unsigned পূর্ণসংখ্যা slot = edge_hash (source, record) & (edge_slots.দৈর্ঘ্য - 1);
    unsigned পূর্ণসংখ্যা step = 0;
    যখন (uint64_t ref = edge_slots[slot])
    {
      hb_codepoint_t existing_source;
      unsigned পূর্ণসংখ্যা existing_সূচক;
      decode_edge_ref (ref, &existing_source, &existing_সূচক);
      যদি (existing_source == source &&
	  তথ্য.glyph_dependencies[source].dependencies[existing_সূচক] == record)
	ফেরত মিথ্যা;
      slot = (slot + ++step) & (edge_slots.দৈর্ঘ্য - 1);
    }

    auto &dependencies = তথ্য.glyph_dependencies[source].dependencies;
    unsigned পূর্ণসংখ্যা সূচক_2 = dependencies.দৈর্ঘ্য;
    যদি (unlikely (!dependencies.ঠেলো_or_fail (record)))
      ফেরত fail ();

    edge_slots[slot] = encode_edge_ref (source, সূচক_2);
    edge_population++;
    ফেরত সত্য;
  }

  যৌক্তিক add_depend_layout (hb_codepoint_t target, hb_tag_t table_tag,
                          hb_tag_t layout_tag,
                          hb_codepoint_t dependent,
                          hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                          hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                          hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    যদি (target >= তথ্য.glyph_dependencies.দৈর্ঘ্য) {
      DEBUG_বার্তা (SUBSET, nullptr, "Dependency glyph %u for %c%c%c%c too large",
                 target, HB_UNTAG(table_tag));
      ফেরত মিথ্যা;
    }

    ফেরত add_edge (target, hb_depend_edge_t (table_tag, dependent, layout_tag,
					      lig_set, context_set, flags));
  }

  যৌক্তিক add_gsub_lookup (hb_codepoint_t target, hb_codepoint_t lookup_সূচক,
                        hb_codepoint_t dependent,
                        hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                        hb_codepoint_t context_set = HB_CODEPOINT_INVALID)
  {
    যদি (context_set == HB_CODEPOINT_INVALID)
      context_set = current_context_set_সূচক;
    hb_subset_depend_edge_flags_t flags = current_edge_flags;

    যৌক্তিক any_added = মিথ্যা;
    জন্য (uint64_t entry : get_lookup_features (lookup_সূচক)) {
      hb_tag_t t = (hb_tag_t) entry;
      যদি (add_depend_layout (target, HB_OT_TAG_GSUB, t, dependent, lig_set, context_set, flags))
        any_added = সত্য;
    }
    ফেরত any_added;
  }

  যৌক্তিক init_lookup_features (unsigned lookup_গুনতি)
  {
    ফেরত check_success (lookup_feature_offsets.resize (lookup_গুনতি + 1));
  }

  যৌক্তিক add_lookup_feature (hb_codepoint_t lookup_সূচক, hb_tag_t feature_tag)
  {
    যদি (feature_tag == HB_SET_মান_INVALID)
      ফেরত সত্য;

    যদি (unlikely (!lookup_feature_offsets.দৈর্ঘ্য ||
		  lookup_সূচক >= lookup_feature_offsets.দৈর্ঘ্য - 1))
      ফেরত fail ();

    uint64_t entry = ((uint64_t) lookup_সূচক << 32) | feature_tag;
    ফেরত check_success (lookup_features.ঠেলো_or_fail (entry));
  }

  যৌক্তিক finish_lookup_features ()
  {
    lookup_features.qsort ([] (uint64_t a, uint64_t b) {
      ফেরত a < b ? -1 : a > b ? 1 : 0;
    });

    unsigned লিখ = 0;
    জন্য (uint64_t entry : lookup_features)
      যদি (!লিখ || entry != lookup_features[লিখ - 1])
	lookup_features[লিখ++] = entry;
    lookup_features.shrink (লিখ, মিথ্যা);

    unsigned feature_সূচক = 0;
    unsigned lookup_গুনতি = lookup_feature_offsets.দৈর্ঘ্য - 1;
    জন্য (unsigned lookup_সূচক = 0; lookup_সূচক < lookup_গুনতি; lookup_সূচক++)
    {
      lookup_feature_offsets[lookup_সূচক] = feature_সূচক;
      যখন (feature_সূচক < lookup_features.দৈর্ঘ্য &&
	     lookup_features[feature_সূচক] >> 32 == lookup_সূচক)
	feature_সূচক++;
    }
    lookup_feature_offsets[lookup_গুনতি] = feature_সূচক;
    ফেরত সত্য;
  }

  hb_array_t<ধ্রুবক uint64_t> get_lookup_features (hb_codepoint_t lookup_সূচক) ধ্রুবক
  {
    যদি (unlikely (!lookup_feature_offsets.দৈর্ঘ্য ||
		  lookup_সূচক >= lookup_feature_offsets.দৈর্ঘ্য - 1))
      ফেরত {};

    unsigned start = lookup_feature_offsets[lookup_সূচক];
    unsigned end = lookup_feature_offsets[lookup_সূচক + 1];
    ফেরত lookup_features.as_array ().sub_array (start, end - start);
  }

  শূন্য add_depend (hb_codepoint_t target, hb_tag_t table_tag,
                   hb_codepoint_t dependent,
                   hb_codepoint_t lig_set = HB_CODEPOINT_INVALID,
                   hb_codepoint_t context_set = HB_CODEPOINT_INVALID,
                   hb_subset_depend_edge_flags_t flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE)
  {
    add_depend_layout (target, table_tag, HB_CODEPOINT_INVALID, dependent, lig_set,
                       context_set, flags);
  }

  hb_codepoint_t get_nominal_glyph (hb_codepoint_t cp)
  { ফেরত hb_map_get (&nominal_glyphs, cp); }

  HB_INTERNAL যৌক্তিক compile (hb_face_t *face);

  যৌক্তিক fail () { successful = মিথ্যা; ফেরত মিথ্যা; }
  hb_codepoint_t fail_invalid () { fail (); ফেরত HB_CODEPOINT_INVALID; }
  যৌক্তিক check_success (যৌক্তিক s) { successful = (successful && s); ফেরত successful; }

  HB_INTERNAL শূন্য get_gsub_dependencies (hb_face_t *face);

  যৌক্তিক successful = সত্য;
  hb_set_t unicodes;
  hb_map_t nominal_glyphs;
  hb_vector_t<uint64_t> lookup_features;
  hb_vector_t<unsigned> lookup_feature_offsets;
  hb_vector_t<uint64_t> edge_slots;
  unsigned পূর্ণসংখ্যা edge_population = 0;
  hb_hashmap_t<ধ্রুবক hb_set_t*, hb_codepoint_t> set_to_সূচক;
  hb_vector_t<hb_codepoint_t> free_set_list;
  hb_codepoint_t current_context_set_সূচক;
  hb_subset_depend_edge_flags_t current_edge_flags;

  hb_depend_তথ্য_t &তথ্য;
};


#endif /* HB_DEPEND_DATA_HH */
