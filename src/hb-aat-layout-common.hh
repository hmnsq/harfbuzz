/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_AAT_LAYOUT_COMMON_HH
#define HB_AAT_LAYOUT_COMMON_HH

#অন্তর্ভাব "hb-aat-layout.hh"
#অন্তর্ভাব "hb-aat-map.hh"
#অন্তর্ভাব "hb-ot-layout-common.hh"
#অন্তর্ভাব "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-cache.hh"
#অন্তর্ভাব "hb-bit-set.hh"
#অন্তর্ভাব "hb-bit-page.hh"


নামাকাশ OT {
কাঠামো GDEF;
};

নামাকাশ AAT {

using নামাকাশ OT;

কাঠামো ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_আকার
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#অন্যথা
#define HB_AAT_TRANSITION_INLINE
#endif

কাঠামো hb_aat_scratch_t
{
  hb_aat_scratch_t () = পূর্বনির্ধারিত;
  hb_aat_scratch_t (ধ্রুবক hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    বাফার_glyph_set.set_relaxed (o.বাফার_glyph_set.get_relaxed ());
    o.বাফার_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    বাফার_glyph_set.set_relaxed (o.বাফার_glyph_set.get_relaxed ());
    o.বাফার_glyph_set.set_relaxed (nullptr);
    ফেরত *এই;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = বাফার_glyph_set.get_relaxed ();
    যদি (unlikely (!s))
      ফেরত;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_বাফার_glyph_set () ধ্রুবক
  {
    hb_bit_set_t *s = বাফার_glyph_set.get_acquire ();
    যদি (s && বাফার_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      ফেরত s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, আকার (hb_bit_set_t));
    যদি (unlikely (!s))
      ফেরত nullptr;
    s->init ();

    ফেরত s;
  }
  শূন্য destroy_বাফার_glyph_set (hb_bit_set_t *s) ধ্রুবক
  {
    যদি (unlikely (!s))
      ফেরত;
    যদি (বাফার_glyph_set.cmpexch (nullptr, s))
      ফেরত;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> বাফার_glyph_set;
};

গণনা { DELETED_GLYPH = 0xFFFF };

#define HB_বাফার_SCRATCH_FLAG_AAT_HAS_DELETED HB_বাফার_SCRATCH_FLAG_SHAPER0

কাঠামো hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, যৌক্তিক, HB_DEBUG_APPLY>
{
  ধ্রুবক অক্ষর *get_name () { ফেরত "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (ধ্রুবক T &obj, Ts&&... ds)
  { ফেরত obj.apply (এই, std::forward<Ts> (ds)...); }
  স্থির return_t default_return_মান () { ফেরত মিথ্যা; }
  যৌক্তিক stop_sublookup_iteration (return_t r) ধ্রুবক { ফেরত r; }

  ধ্রুবক hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_বাফার_t *বাফার;
  hb_sanitize_context_t sanitizer;
  ধ্রুবক ankr *ankr_table;
  ধ্রুবক OT::GDEF &gdef;
  যৌক্তিক has_glyph_classes;
  ধ্রুবক hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  যৌক্তিক বাফার_is_reversed = মিথ্যা;
  // Caches
  যৌক্তিক using_বাফার_glyph_set = মিথ্যা;
  hb_bit_set_t *বাফার_glyph_set = nullptr;
  ধ্রুবক hb_bit_set_t *first_set = nullptr;
  ধ্রুবক hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned পূর্ণসংখ্যা lookup_সূচক;

  HB_INTERNAL hb_aat_apply_context_t (ধ্রুবক hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_বাফার_t *বাফার_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL শূন্য set_ankr_table (ধ্রুবক AAT::ankr *ankr_table_);

  শূন্য set_lookup_সূচক (unsigned পূর্ণসংখ্যা i) { lookup_সূচক = i; }

  শূন্য reverse_বাফার ()
  {
    বাফার->reverse ();
    বাফার_is_reversed = !বাফার_is_reversed;
  }

  শূন্য setup_বাফার_glyph_set ()
  {
    using_বাফার_glyph_set = বাফার->len >= 4 && বাফার_glyph_set;

    যদি (likely (using_বাফার_glyph_set))
      বাফার->collect_codepoints (*বাফার_glyph_set);
  }
  যৌক্তিক বাফার_intersects_machine () ধ্রুবক
  {
    যদি (likely (using_বাফার_glyph_set))
      ফেরত বাফার_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    জন্য (unsigned i = 0; i < বাফার->len; i++)
      যদি (first_set->has (বাফার->info[i].codepoint))
	ফেরত সত্য;
    ফেরত মিথ্যা;
  }

  template <typename T>
  HB_NODISCARD যৌক্তিক output_glyphs (unsigned পূর্ণসংখ্যা গুনতি,
				   ধ্রুবক T *glyphs)
  {
    যদি (likely (using_বাফার_glyph_set))
      বাফার_glyph_set->add_array (glyphs, গুনতি);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
    {
      যৌক্তিক at_end_of_text = বাফার->idx == বাফার->len;
      যদি (at_end_of_text)
	যদি (unlikely (!বাফার->output_glyph (glyphs[i]))) ফেরত মিথ্যা;

      hb_glyph_info_t &info = at_end_of_text ? বাফার->prev () : বাফার->cur ();
      যদি (glyphs[i] == DELETED_GLYPH)
      {
        বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      অন্যথা
      {
#ifndef HB_NO_OT_LAYOUT
	যদি (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      যদি (!at_end_of_text)
	যদি (unlikely (!বাফার->output_glyph (glyphs[i]))) ফেরত মিথ্যা;
    }
    ফেরত সত্য;
  }

  HB_NODISCARD যৌক্তিক replace_glyph (hb_codepoint_t glyph)
  {
    যদি (glyph == DELETED_GLYPH)
    {
      বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&বাফার->cur());
    }

    যদি (likely (using_বাফার_glyph_set))
      বাফার_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    যদি (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&বাফার->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    ফেরত বাফার->replace_glyph (glyph);
  }

  HB_NODISCARD যৌক্তিক delete_glyph ()
  {
    বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&বাফার->cur());
    যদি (likely (using_বাফার_glyph_set))
      বাফার_glyph_set->add (DELETED_GLYPH);
    ফেরত বাফার->replace_glyph (DELETED_GLYPH);
  }

  শূন্য replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    বাফার->info[i].codepoint = glyph;
    যদি (glyph == DELETED_GLYPH)
    {
      বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&বাফার->info[i]);
    }
    যদি (likely (using_বাফার_glyph_set))
      বাফার_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    যদি (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&বাফার->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> কাঠামো Lookup;

template <typename T>
কাঠামো LookupFormat0
{
  friend কাঠামো Lookup<T>;

  ব্যক্তিগত:
  ধ্রুবক T* get_মান (hb_codepoint_t glyph_id, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    যদি (unlikely (glyph_id >= অঙ্ক_glyphs)) ফেরত nullptr;
    ফেরত &arrayZ[glyph_id];
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    glyphs.add_range (0, অঙ্ক_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, unsigned অঙ্ক_glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < অঙ্ক_glyphs; i++)
      যদি (filter (arrayZ[i]))
	glyphs.add (i);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (arrayZ.sanitize (c, c->get_অঙ্ক_glyphs ()));
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (arrayZ.sanitize (c, c->get_অঙ্ক_glyphs (), base));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  সর্বজনীন:
  DEFINE_আকার_UNBOUNDED (2);
};


template <typename T>
কাঠামো LookupSegmentSingle
{
  স্থির constexpr unsigned Termination_Word_গুনতি = 2u;

  পূর্ণসংখ্যা cmp (hb_codepoint_t g) ধ্রুবক
  { ফেরত g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুবক
  {
    যদি (first == DELETED_GLYPH) ফেরত;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    যদি (first == DELETED_GLYPH) ফেরত;
    যদি (!filter (value)) ফেরত;
    glyphs.add_range (first, last);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && value.sanitize (c));
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + T::static_আকার);
};

template <typename T>
কাঠামো LookupFormat2
{
  friend কাঠামো Lookup<T>;

  ব্যক্তিগত:
  ধ্রুবক T* get_মান (hb_codepoint_t glyph_id) ধ্রুবক
  {
    ধ্রুবক LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    ফেরত v ? &v->value : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুবক
  {
    unsigned গুনতি = segments.get_দৈর্ঘ্য ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    unsigned গুনতি = segments.get_দৈর্ঘ্য ();
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (segments.sanitize (c));
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (segments.sanitize (c, base));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_অনুসন্ধান_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, segments);
};

template <typename T>
কাঠামো LookupSegmentArray
{
  স্থির constexpr unsigned Termination_Word_গুনতি = 2u;

  ধ্রুবক T* get_মান (hb_codepoint_t glyph_id, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    ফেরত first <= glyph_id && glyph_id <= last ? &(base+মানসমূহ_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুবক
  {
    যদি (first == DELETED_GLYPH) ফেরত;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুবক শূন্য *base, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    যদি (first == DELETED_GLYPH) ফেরত;
    ধ্রুবক auto &values = base+মানসমূহ_Z;
    জন্য (hb_codepoint_t i = first; i <= last; i++)
      যদি (filter (values[i - first]))
	glyphs.add (i);
  }

  পূর্ণসংখ্যা cmp (hb_codepoint_t g) ধ্রুবক
  { ফেরত g < first ? -1 : g <= last ? 0 : +1; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  first <= last &&
		  মানসমূহ_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base, Ts&&... ds) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  first <= last &&
		  মানসমূহ_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		মানসমূহ_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (6);
};

template <typename T>
কাঠামো LookupFormat4
{
  friend কাঠামো Lookup<T>;

  ব্যক্তিগত:
  ধ্রুবক T* get_মান (hb_codepoint_t glyph_id) ধ্রুবক
  {
    ধ্রুবক LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    ফেরত v ? v->get_মান (glyph_id, এই) : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুবক
  {
    unsigned গুনতি = segments.get_দৈর্ঘ্য ();
    জন্য (unsigned i = 0; i < গুনতি; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    unsigned গুনতি = segments.get_দৈর্ঘ্য ();
    জন্য (unsigned i = 0; i < গুনতি; i++)
      segments[i].collect_glyphs_filtered (glyphs, এই, filter);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (segments.sanitize (c, এই));
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (segments.sanitize (c, এই, base));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_অনুসন্ধান_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, segments);
};

template <typename T>
কাঠামো LookupSingle
{
  স্থির constexpr unsigned Termination_Word_গুনতি = 1u;

  পূর্ণসংখ্যা cmp (hb_codepoint_t g) ধ্রুবক { ফেরত glyph.cmp (g); }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুবক
  {
    যদি (glyph == DELETED_GLYPH) ফেরত;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    যদি (glyph == DELETED_GLYPH) ফেরত;
    যদি (!filter (value)) ফেরত;
    glyphs.add (glyph);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && value.sanitize (c));
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  সর্বজনীন:
  DEFINE_আকার_STATIC (2 + T::static_আকার);
};

template <typename T>
কাঠামো LookupFormat6
{
  friend কাঠামো Lookup<T>;

  ব্যক্তিগত:
  ধ্রুবক T* get_মান (hb_codepoint_t glyph_id) ধ্রুবক
  {
    ধ্রুবক LookupSingle<T> *v = entries.bsearch (glyph_id);
    ফেরত v ? &v->value : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুবক
  {
    unsigned গুনতি = entries.get_দৈর্ঘ্য ();
    জন্য (unsigned i = 0; i < গুনতি; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    unsigned গুনতি = entries.get_দৈর্ঘ্য ();
    জন্য (unsigned i = 0; i < গুনতি; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (entries.sanitize (c));
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (entries.sanitize (c, base));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_অনুসন্ধান_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, entries);
};

template <typename T>
কাঠামো LookupFormat8
{
  friend কাঠামো Lookup<T>;

  ব্যক্তিগত:
  ধ্রুবক T* get_মান (hb_codepoint_t glyph_id) ধ্রুবক
  {
    ফেরত firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_গুনতি_2 ?
	   &মান_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুবক
  {
    যদি (unlikely (!glyph_গুনতি_2)) ফেরত;
    যদি (firstGlyph == DELETED_GLYPH) ফেরত;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_গুনতি_2 - 1);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    যদি (unlikely (!glyph_গুনতি_2)) ফেরত;
    যদি (firstGlyph == DELETED_GLYPH) ফেরত;
    ধ্রুবক T *p = মান_Array_Z.arrayZ;
    জন্য (unsigned i = 0; i < glyph_গুনতি_2; i++)
      যদি (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && মান_Array_Z.sanitize (c, glyph_গুনতি_2));
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && মান_Array_Z.sanitize (c, glyph_গুনতি_2, base));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_গুনতি_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		মান_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, মান_Array_Z);
};

template <typename T>
কাঠামো LookupFormat10
{
  friend কাঠামো Lookup<T>;

  ব্যক্তিগত:
  ধ্রুবক typename T::type get_মান_or_null (hb_codepoint_t glyph_id) ধ্রুবক
  {
    যদি (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_গুনতি_2))
      ফেরত Null (T);

    ধ্রুবক HBUINT8 *p = &মান_Array_Z[(glyph_id - firstGlyph) * মান_আকার];

    unsigned পূর্ণসংখ্যা v = 0;
    unsigned পূর্ণসংখ্যা গুনতি = মান_আকার;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      v = (v << 8) | *p++;

    ফেরত v;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুবক
  {
    যদি (unlikely (!glyph_গুনতি_2)) ফেরত;
    যদি (firstGlyph == DELETED_GLYPH) ফেরত;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_গুনতি_2 - 1);
  }

  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    যদি (unlikely (!glyph_গুনতি_2)) ফেরত;
    যদি (firstGlyph == DELETED_GLYPH) ফেরত;
    ধ্রুবক HBUINT8 *p = মান_Array_Z.arrayZ;
    জন্য (unsigned i = 0; i < glyph_গুনতি_2; i++)
    {
      unsigned পূর্ণসংখ্যা v = 0;
      unsigned পূর্ণসংখ্যা গুনতি = মান_আকার;
      জন্য (unsigned পূর্ণসংখ্যা j = 0; j < গুনতি; j++)
	v = (v << 8) | *p++;
      যদি (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  মান_আকার <= 4 &&
		  মান_Array_Z.sanitize (c, glyph_গুনতি_2 * মান_আকার));
  }

  সুরক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	মান_আকার;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_গুনতি_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		মান_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, মান_Array_Z);
};

template <typename T>
কাঠামো Lookup
{
  ধ্রুবক T* get_মান (hb_codepoint_t glyph_id, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); ফেরত u.format0.get_মান (glyph_id, অঙ্ক_glyphs);
    ক্ষেত্র 2: hb_barrier (); ফেরত u.format2.get_মান (glyph_id);
    ক্ষেত্র 4: hb_barrier (); ফেরত u.format4.get_মান (glyph_id);
    ক্ষেত্র 6: hb_barrier (); ফেরত u.format6.get_মান (glyph_id);
    ক্ষেত্র 8: hb_barrier (); ফেরত u.format8.get_মান (glyph_id);
    পূর্বনির্ধারিত:ফেরত nullptr;
    }
  }

  ধ্রুবক typename T::type get_মান_or_null (hb_codepoint_t glyph_id, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
      /* Format 10 cannot return a pointer. */
      ক্ষেত্র 10: hb_barrier (); ফেরত u.format10.get_মান_or_null (glyph_id);
      পূর্বনির্ধারিত:
      ধ্রুবক T *v = get_মান (glyph_id, অঙ্ক_glyphs);
      ফেরত v ? *v : Null (T);
    }
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs, unsigned পূর্ণসংখ্যা অঙ্ক_glyphs) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); u.format0.collect_glyphs (glyphs, অঙ্ক_glyphs); ফেরত;
    ক্ষেত্র 2: hb_barrier (); u.format2.collect_glyphs (glyphs); ফেরত;
    ক্ষেত্র 4: hb_barrier (); u.format4.collect_glyphs (glyphs); ফেরত;
    ক্ষেত্র 6: hb_barrier (); u.format6.collect_glyphs (glyphs); ফেরত;
    ক্ষেত্র 8: hb_barrier (); u.format8.collect_glyphs (glyphs); ফেরত;
    ক্ষেত্র 10: hb_barrier (); u.format10.collect_glyphs (glyphs); ফেরত;
    পূর্বনির্ধারিত:ফেরত;
    }
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, unsigned অঙ্ক_glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, অঙ্ক_glyphs, filter); ফেরত;
    ক্ষেত্র 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); ফেরত;
    ক্ষেত্র 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); ফেরত;
    ক্ষেত্র 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); ফেরত;
    ক্ষেত্র 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); ফেরত;
    ক্ষেত্র 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); ফেরত;
    পূর্বনির্ধারিত:ফেরত;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
			      unsigned পূর্ণসংখ্যা outOfRange) ধ্রুবক
  {
    ধ্রুবক T *v = get_মান (glyph_id, অঙ্ক_glyphs);
    ফেরত v ? *v : outOfRange;
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    ক্ষেত্র 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    ক্ষেত্র 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    ক্ষেত্র 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }
  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক শূন্য *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (!u.format.v.sanitize (c)) return_trace (মিথ্যা);
    hb_barrier ();
    নির্বাচন (u.format.v) {
    ক্ষেত্র 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    ক্ষেত্র 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    ক্ষেত্র 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    ক্ষেত্র 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    ক্ষেত্র 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    ক্ষেত্র 10: return_trace (মিথ্যা); /* We don't support format10 here currently. */
    পূর্বনির্ধারিত:return_trace (সত্য);
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  সর্বজনীন:
  DEFINE_আকার_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
কাঠামো Entry
{
  // This doesn't seem like it's ever called.
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_আকার, "");

    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		তথ্য;		/* Optional offsets to per-glyph tables. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4 + T::static_আকার);
};

template <>
কাঠামো Entry<শূন্য>
{
  // This does seem like it's ever called.
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  সর্বজনীন:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

গণনা Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_সারি = 3,
};

template <typename Types, typename Extra>
কাঠামো StateTable
{
  প্রকারলেখ typename Types::HBUINT HBUINT;
  প্রকারলেখ typename Types::HBUSHORT HBUSHORT;
  প্রকারলেখ typename Types::ClassTypeNarrow ClassType;

  গণনা State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_সারি = 1,
  };

  template <typename set_t, typename table_t>
  শূন্য collect_initial_glyphs (set_t &glyphs, unsigned অঙ্ক_glyphs, ধ্রুবক table_t &table) ধ্রুবক
  {
    unsigned অঙ্ক_classes = nClasses;

    যদি (unlikely (অঙ্ক_classes > hb_bit_page_t::BITS))
    {
      (এই+classTable).collect_glyphs (glyphs, অঙ্ক_glyphs);
      ফেরত;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    জন্য (unsigned i = 0; i < অঙ্ক_classes; i++)
    {
      ধ্রুবক auto &entry = get_entry (STATE_START_OF_TEXT, i);
      যদি (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	চালু;

      filter.add (i);
    }

    // And glyphs in those classes.

    যদি (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (এই+classTable).collect_glyphs_filtered (glyphs, অঙ্ক_glyphs, filter);
  }

  পূর্ণসংখ্যা new_state (unsigned পূর্ণসংখ্যা newState) ধ্রুবক
  { ফেরত Types::extended ? newState : ((পূর্ণসংখ্যা) newState - (পূর্ণসংখ্যা) stateArrayTable) / (পূর্ণসংখ্যা) nClasses; }

  unsigned পূর্ণসংখ্যা get_class (hb_codepoint_t glyph_id,
			  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) ধ্রুবক
  {
    unsigned klass;
    যদি (cache && cache->get (glyph_id, &klass)) ফেরত klass;
    যদি (unlikely (glyph_id == DELETED_GLYPH)) ফেরত CLASS_DELETED_GLYPH;
    klass = (এই+classTable).get_class (glyph_id, অঙ্ক_glyphs, CLASS_OUT_OF_BOUNDS);
    যদি (cache) cache->set (glyph_id, klass);
    ফেরত klass;
  }

  ধ্রুবক Entry<Extra> *get_entries () ধ্রুবক
  { ফেরত (এই+entryTable).arrayZ; }

  ধ্রুবক Entry<Extra> &get_entry (পূর্ণসংখ্যা state, unsigned পূর্ণসংখ্যা klass) ধ্রুবক
  {
    unsigned n_classes = nClasses;
    যদি (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    ধ্রুবক HBUSHORT *states = (এই+stateArrayTable).arrayZ;
    ধ্রুবক Entry<Extra> *entries = (এই+entryTable).arrayZ;

    unsigned পূর্ণসংখ্যা entry = states[state * n_classes + klass];
    DEBUG_বার্তা (APPLY, nullptr, "e%u", entry);

    ফেরত entries[entry];
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c,
		 unsigned পূর্ণসংখ্যা *অঙ্ক_entries_out = nullptr) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!(c->check_struct (এই) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, এই)))) return_trace (মিথ্যা);

    ধ্রুবক HBUSHORT *states = (এই+stateArrayTable).arrayZ;
    ধ্রুবক Entry<Extra> *entries = (এই+entryTable).arrayZ;

    unsigned পূর্ণসংখ্যা অঙ্ক_classes = nClasses;
    যদি (unlikely (hb_unsigned_mul_overflows (অঙ্ক_classes, states[0].static_আকার)))
      return_trace (মিথ্যা);
    unsigned পূর্ণসংখ্যা row_stride = অঙ্ক_classes * states[0].static_আকার;

    /* Apple 'kern' table has this peculiarity:
     *
     * "Because the stateTableOffset in the state table header is (strictly
     * speaking) redundant, some 'kern' tables use it to record an initial
     * state where that should not be StartOfText. To determine if this is
     * done, calculate what the stateTableOffset should be. If it's different
     * from the actual stateTableOffset, use it as the initial state."
     *
     * We implement this by calling the initial state zero, but allow *negative*
     * states if the start state indeed was not the first state.  Since the code
     * is shared, this will also apply to 'mort' table.  The 'kerx' / 'morx'
     * tables are not affected since those address states by index, not offset.
     */

    পূর্ণসংখ্যা min_state = 0;
    পূর্ণসংখ্যা max_state = 0;
    unsigned পূর্ণসংখ্যা অঙ্ক_entries = 0;

    পূর্ণসংখ্যা state_pos = 0;
    পূর্ণসংখ্যা state_neg = 0;
    unsigned পূর্ণসংখ্যা entry = 0;
    যখন (min_state < state_neg || state_pos <= max_state)
    {
      যদি (min_state < state_neg)
      {
	/* Negative states. */
	যদি (unlikely (hb_unsigned_mul_overflows (min_state, অঙ্ক_classes)))
	  return_trace (মিথ্যা);
	যদি (unlikely (!c->check_range (&states[min_state * অঙ্ক_classes],
				       -min_state,
				       row_stride)))
	  return_trace (মিথ্যা);
	যদি ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (মিথ্যা);
	{ /* Sweep new states. */
	  ধ্রুবক HBUSHORT *stop = &states[min_state * অঙ্ক_classes];
	  যদি (unlikely (stop > states))
	    return_trace (মিথ্যা);
	  জন্য (ধ্রুবক HBUSHORT *p = states; stop < p; p--)
	    অঙ্ক_entries = hb_max (অঙ্ক_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      যদি (state_pos <= max_state)
      {
	/* Positive states. */
	যদি (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (মিথ্যা);
	যদি ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (মিথ্যা);
	{ /* Sweep new states. */
	  যদি (unlikely (hb_unsigned_mul_overflows ((max_state + 1), অঙ্ক_classes)))
	    return_trace (মিথ্যা);
	  ধ্রুবক HBUSHORT *stop = &states[(max_state + 1) * অঙ্ক_classes];
	  যদি (unlikely (stop < states))
	    return_trace (মিথ্যা);
	  জন্য (ধ্রুবক HBUSHORT *p = &states[state_pos * অঙ্ক_classes]; p < stop; p++)
	    অঙ্ক_entries = hb_max (অঙ্ক_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      যদি (unlikely (!c->check_array (entries, অঙ্ক_entries)))
	return_trace (মিথ্যা);
      যদি ((c->max_ops -= অঙ্ক_entries - entry) <= 0)
	return_trace (মিথ্যা);
      { /* Sweep new entries. */
	ধ্রুবক Entry<Extra> *stop = &entries[অঙ্ক_entries];
	জন্য (ধ্রুবক Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  পূর্ণসংখ্যা newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = অঙ্ক_entries;
      }
    }

    যদি (অঙ্ক_entries_out)
      *অঙ্ক_entries_out = অঙ্ক_entries;

    return_trace (সত্য);
  }

  সুরক্ষিত:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  সর্বজনীন:
  DEFINE_আকার_STATIC (4 * আকার (HBUINT));
};

template <typename HBUCHAR>
কাঠামো ClassTable
{
  unsigned পূর্ণসংখ্যা get_class (hb_codepoint_t glyph_id, unsigned পূর্ণসংখ্যা outOfRange) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা i = glyph_id - firstGlyph;
    ফেরত i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned পূর্ণসংখ্যা get_class (hb_codepoint_t glyph_id,
			  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs HB_UNUSED,
			  unsigned পূর্ণসংখ্যা outOfRange) ধ্রুবক
  {
    ফেরত get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs, unsigned অঙ্ক_glyphs) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < classArray.len; i++)
      যদি (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, unsigned অঙ্ক_glyphs, ধ্রুবক filter_t &filter) ধ্রুবক
  {
    জন্য (unsigned i = 0; i < classArray.len; i++)
      যদি (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) && classArray.sanitize (c));
  }
  সুরক্ষিত:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, classArray);
};

কাঠামো SubtableGlyphCoverage
{
  যৌক্তিক sanitize (hb_sanitize_context_t *c, unsigned subtable_গুনতি) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    যদি (unlikely (!c->check_array (&subtableOffsets, subtable_গুনতি)))
      return_trace (মিথ্যা);

    unsigned bytes = (c->get_অঙ্ক_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    জন্য (unsigned i = 0; i < subtable_গুনতি; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      যদি (offset == 0 || offset == 0xFFFFFFFF)
        চালু;
      যদি (unlikely (!subtableOffsets[i].sanitize (c, এই, bytes)))
        return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }
  সুরক্ষিত:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (0, subtableOffsets);
};

কাঠামো ObsoleteTypes
{
  স্থির constexpr যৌক্তিক extended = মিথ্যা;
  প্রকারলেখ HBUINT16 HBUINT;
  প্রকারলেখ HBUINT8 HBUSHORT;
  প্রকারলেখ ClassTable<HBUINT8> ClassTypeNarrow;
  প্রকারলেখ ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  স্থির পূর্ণসংখ্যা offset_To_সূচক (int64_t offset,
			    ধ্রুবক শূন্য *base,
			    ধ্রুবক T *array)
  {
    int64_t array_offset = (ধ্রুবক অক্ষর *) array - (ধ্রুবক অক্ষর *) base;
    পূর্ণসংখ্যা bad_সূচক = INT_MAX / T::static_আকার;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    যদি (unlikely (offset < array_offset))
      ফেরত bad_সূচক;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t সূচক_2 = (offset - array_offset) / T::static_আকার;
    যদি (unlikely (সূচক_2 > bad_সূচক))
      ফেরত bad_সূচক;
    ফেরত সূচক_2;
  }
  template <typename T>
  স্থির পূর্ণসংখ্যা byte_Offset_To_সূচক (int64_t offset,
				ধ্রুবক শূন্য *base,
				ধ্রুবক T *array)
  {
    ফেরত offset_To_সূচক (offset, base, array);
  }
  template <typename T>
  স্থির পূর্ণসংখ্যা word_Offset_To_সূচক (int64_t offset,
				ধ্রুবক শূন্য *base,
				ধ্রুবক T *array)
  {
    ফেরত offset_To_সূচক (2 * offset, base, array);
  }
};
কাঠামো ExtendedTypes
{
  স্থির constexpr যৌক্তিক extended = সত্য;
  প্রকারলেখ HBUINT32 HBUINT;
  প্রকারলেখ HBUINT16 HBUSHORT;
  প্রকারলেখ Lookup<HBUINT16> ClassTypeNarrow;
  প্রকারলেখ Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  স্থির unsigned পূর্ণসংখ্যা offset_To_সূচক (unsigned পূর্ণসংখ্যা offset,
				     ধ্রুবক শূন্য *base HB_UNUSED,
				     ধ্রুবক T *array HB_UNUSED)
  {
    ফেরত offset;
  }
  template <typename T>
  স্থির unsigned পূর্ণসংখ্যা byte_Offset_To_সূচক (unsigned পূর্ণসংখ্যা offset,
					 ধ্রুবক শূন্য *base HB_UNUSED,
					 ধ্রুবক T *array HB_UNUSED)
  {
    ফেরত offset / 2;
  }
  template <typename T>
  স্থির unsigned পূর্ণসংখ্যা word_Offset_To_সূচক (unsigned পূর্ণসংখ্যা offset,
					 ধ্রুবক শূন্য *base HB_UNUSED,
					 ধ্রুবক T *array HB_UNUSED)
  {
    ফেরত offset;
  }
};

template <typename Types, typename Entry_তথ্য, typename Flags>
কাঠামো StateTableDriver
{
  using StateTableT = StateTable<Types, Entry_তথ্য>;
  using EntryT = Entry<Entry_তথ্য>;

  StateTableDriver (ধ্রুবক StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      অঙ্ক_glyphs (face_->get_অঙ্ক_glyphs ()) {}

  template <typename context_t>
  শূন্য drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_বাফার_t *বাফার = ac->বাফার;

    যদি (!c->in_place)
      বাফার->clear_output ();

    পূর্ণসংখ্যা state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->দৈর্ঘ্য > 1) ? &(*ac->range_flags)[0] : nullptr;
    ধ্রুবক যৌক্তিক start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    জন্য (বাফার->idx = 0; বাফার->successful;)
    {
      unsigned পূর্ণসংখ্যা klass = likely (বাফার->idx < বাফার->len) ?
			   machine.get_class (বাফার->cur().codepoint, অঙ্ক_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_বার্তা (APPLY, nullptr, "c%u at %u", klass, বাফার->idx);
      ধ্রুবক EntryT &entry = machine.get_entry (state, klass);
      ধ্রুবক পূর্ণসংখ্যা পরবর্তী_state = machine.new_state (entry.newState);

      যৌক্তিক is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      যৌক্তিক is_not_actionable = !c->table->is_actionable (entry);

      যদি (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	যদি (বাফার->idx < বাফার->len)
	{
	  unsigned cluster = বাফার->cur().cluster;
	  যখন (cluster < range->cluster_first)
	    range--;
	  যখন (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	যদি (!(range->flags & ac->subtable_flags))
	{
	  যদি (বাফার->idx == বাফার->len)
	    বিরতি;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (শূন্য) বাফার->পরবর্তী_glyph ();
	  চালু;
	}
      }
      অন্যথা
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	যৌক্তিক is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  পরবর্তী_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	যদি (is_null_transition)
	{
	  unsigned old_klass = klass;
	  করো
	  {
	    c->transition (বাফার, এই, entry);

	    যদি (বাফার->idx == বাফার->len || !বাফার->successful)
	      বিরতি;

	    (শূন্য) বাফার->পরবর্তী_glyph ();

	    klass = likely (বাফার->idx < বাফার->len) ?
		     machine.get_class (বাফার->cur().codepoint, অঙ্ক_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } যখন (klass == old_klass);

	  যদি (বাফার->idx == বাফার->len || !বাফার->successful)
	    বিরতি;

	  যাও resume;
	}
      }

      /* Conditions under which it's guaranteed safe-to-break before current glyph:
       *
       * 1. There was no action in this transition; and
       *
       * 2. If we break before current glyph, the results will be the same. That
       *    is guaranteed if:
       *
       *    2a. We were already in start-of-text state; or
       *
       *    2b. We are epsilon-transitioning to start-of-text state; or
       *
       *    2c. Starting from start-of-text state seeing current glyph:
       *
       *        2c'. There won't be any actions; and
       *
       *        2c". We would end up in the same state that we were going to end up
       *             in now, including whether epsilon-transitioning.
       *
       *    and
       *
       * 3. If we break before current glyph, there won't be any end-of-text action
       *    after previous glyph.
       *
       * This triples the transitions we need to look up, but is worth returning
       * granular unsafe-to-break results. See eg.:
       *
       *   https://github.com/harfbuzz/harfbuzz/issues/2860
       */
      ধ্রুবক EntryT *wouldbe_entry;
      যৌক্তিক is_safe_to_break =
      (
          /* 1. */
          !c->table->is_actionable (entry) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == StateTableT::STATE_START_OF_TEXT
              || ((entry.flags & Flags::DontAdvance) && পরবর্তী_state == StateTableT::STATE_START_OF_TEXT)
              || (
		    /* 2c. */
		    wouldbe_entry = &machine.get_entry(StateTableT::STATE_START_OF_TEXT, klass)
		    ,
		    /* 2c'. */
		    !c->table->is_actionable (*wouldbe_entry) &&
		    /* 2c". */
		    (
		      পরবর্তী_state == machine.new_state(wouldbe_entry->newState) &&
		      (entry.flags & Flags::DontAdvance) == (wouldbe_entry->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->table->is_actionable (machine.get_entry (state, CLASS_END_OF_TEXT))
      );

      যদি (!is_safe_to_break && বাফার->backtrack_len () && বাফার->idx < বাফার->len)
	বাফার->unsafe_to_break_from_outbuffer (বাফার->backtrack_len () - 1, বাফার->idx + 1);

      c->transition (বাফার, এই, entry);

      state = পরবর্তী_state;
      DEBUG_বার্তা (APPLY, nullptr, "s%d", state);

      যদি (বাফার->idx == বাফার->len)
	বিরতি;

      যদি (is_not_epsilon_transition || বাফার->max_ops-- <= 0)
	(শূন্য) বাফার->পরবর্তী_glyph ();
    }

    যদি (!c->in_place)
      বাফার->সমলয় ();
  }

  সর্বজনীন:
  ধ্রুবক StateTableT &machine;
  unsigned পূর্ণসংখ্যা অঙ্ক_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
