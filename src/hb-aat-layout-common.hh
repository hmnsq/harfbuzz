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

#অন্তর্ভাৱ "hb-aat-layout.hh"
#অন্তর্ভাৱ "hb-aat-map.hh"
#অন্তর্ভাৱ "hb-ot-layout-common.hh"
#অন্তর্ভাৱ "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাৱ "hb-open-type.hh"
#অন্তর্ভাৱ "hb-cache.hh"
#অন্তর্ভাৱ "hb-bit-set.hh"
#অন্তর্ভাৱ "hb-bit-page.hh"


নামাকাশ OT {
সংরচনা GDEF;
};

নামাকাশ AAT {

using নামাকাশ OT;

সংরচনা ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#অন্যথা
#define HB_AAT_TRANSITION_INLINE
#endif

সংরচনা hb_aat_scratch_t
{
  hb_aat_scratch_t () = স্ৱভাৱ;
  hb_aat_scratch_t (ধ্রুৱ hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    ঘূৰাই *এতত্;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    যদি (unlikely (!s))
      ঘূৰাই;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () ধ্রুৱ
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    যদি (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      ঘূৰাই s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, পরিমাণ (hb_bit_set_t));
    যদি (unlikely (!s))
      ঘূৰাই nullptr;
    s->init ();

    ঘূৰাই s;
  }
  শূন্য destroy_buffer_glyph_set (hb_bit_set_t *s) ধ্রুৱ
  {
    যদি (unlikely (!s))
      ঘূৰাই;
    যদি (buffer_glyph_set.cmpexch (nullptr, s))
      ঘূৰাই;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

গণনা { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

সংরচনা hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, তার্কিক, HB_DEBUG_APPLY>
{
  ধ্রুৱ ৱর্ণ *get_name () { ঘূৰাই "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (ধ্রুৱ T &obj, Ts&&... ds)
  { ঘূৰাই obj.apply (এতত্, std::forward<Ts> (ds)...); }
  স্থির return_t default_return_মান () { ঘূৰাই মিছা; }
  তার্কিক stop_sublookup_iteration (return_t r) ধ্রুৱ { ঘূৰাই r; }

  ধ্রুৱ hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  ধ্রুৱ ankr *ankr_table;
  ধ্রুৱ OT::GDEF &gdef;
  তার্কিক has_glyph_classes;
  ধ্রুৱ hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  তার্কিক buffer_is_reversed = মিছা;
  // Caches
  তার্কিক using_buffer_glyph_set = মিছা;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  ধ্রুৱ hb_bit_set_t *first_set = nullptr;
  ধ্রুৱ hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned পূর্ণাঙ্ক lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (ধ্রুৱ hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL শূন্য set_ankr_table (ধ্রুৱ AAT::ankr *ankr_table_);

  শূন্য set_lookup_index (unsigned পূর্ণাঙ্ক i) { lookup_index = i; }

  শূন্য reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  শূন্য setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    যদি (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  তার্কিক buffer_intersects_machine () ধ্রুৱ
  {
    যদি (likely (using_buffer_glyph_set))
      ঘূৰাই buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    বাবে (unsigned i = 0; i < buffer->len; i++)
      যদি (first_set->has (buffer->info[i].codepoint))
	ঘূৰাই সঁচা;
    ঘূৰাই মিছা;
  }

  template <typename T>
  HB_NODISCARD তার্কিক output_glyphs (unsigned পূর্ণাঙ্ক গুনতি,
				   ধ্রুৱ T *glyphs)
  {
    যদি (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, গুনতি);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      তার্কিক at_end_of_text = buffer->idx == buffer->len;
      যদি (at_end_of_text)
	যদি (unlikely (!buffer->output_glyph (glyphs[i]))) ঘূৰাই মিছা;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      যদি (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
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
	যদি (unlikely (!buffer->output_glyph (glyphs[i]))) ঘূৰাই মিছা;
    }
    ঘূৰাই সঁচা;
  }

  HB_NODISCARD তার্কিক replace_glyph (hb_codepoint_t glyph)
  {
    যদি (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    যদি (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    যদি (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    ঘূৰাই buffer->replace_glyph (glyph);
  }

  HB_NODISCARD তার্কিক delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    যদি (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    ঘূৰাই buffer->replace_glyph (DELETED_GLYPH);
  }

  শূন্য replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    যদি (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    যদি (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    যদি (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> সংরচনা Lookup;

template <typename T>
সংরচনা LookupFormat0
{
  friend সংরচনা Lookup<T>;

  গুপ্ত:
  ধ্রুৱ T* get_মান (hb_codepoint_t glyph_id, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    যদি (unlikely (glyph_id >= অঙ্ক_glyphs)) ঘূৰাই nullptr;
    ঘূৰাই &arrayZ[glyph_id];
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs, unsigned অঙ্ক_glyphs) ধ্রুৱ
  {
    glyphs.add_range (0, অঙ্ক_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, unsigned অঙ্ক_glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    বাবে (unsigned i = 0; i < অঙ্ক_glyphs; i++)
      যদি (filter (arrayZ[i]))
	glyphs.add (i);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (arrayZ.sanitize (c, c->get_অঙ্ক_glyphs ()));
  }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (arrayZ.sanitize (c, c->get_অঙ্ক_glyphs (), base));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  সার্ৱজনিক:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
সংরচনা LookupSegmentSingle
{
  স্থির constexpr unsigned Termination_Word_গুনতি = 2u;

  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুৱ
  { ঘূৰাই g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুৱ
  {
    যদি (first == DELETED_GLYPH) ঘূৰাই;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    যদি (first == DELETED_GLYPH) ঘূৰাই;
    যদি (!filter (value)) ঘূৰাই;
    glyphs.add_range (first, last);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && value.sanitize (c));
  }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
সংরচনা LookupFormat2
{
  friend সংরচনা Lookup<T>;

  গুপ্ত:
  ধ্রুৱ T* get_মান (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    ধ্রুৱ LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    ঘূৰাই v ? &v->value : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুৱ
  {
    unsigned গুনতি = segments.get_length ();
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    unsigned গুনতি = segments.get_length ();
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (segments.sanitize (c));
  }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (segments.sanitize (c, base));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_অনুসন্ধান_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
সংরচনা LookupSegmentArray
{
  স্থির constexpr unsigned Termination_Word_গুনতি = 2u;

  ধ্রুৱ T* get_মান (hb_codepoint_t glyph_id, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    ঘূৰাই first <= glyph_id && glyph_id <= last ? &(base+মানসমূহ_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুৱ
  {
    যদি (first == DELETED_GLYPH) ঘূৰাই;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুৱ শূন্য *base, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    যদি (first == DELETED_GLYPH) ঘূৰাই;
    ধ্রুৱ auto &values = base+মানসমূহ_Z;
    বাবে (hb_codepoint_t i = first; i <= last; i++)
      যদি (filter (values[i - first]))
	glyphs.add (i);
  }

  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুৱ
  { ঘূৰাই g < first ? -1 : g <= last ? 0 : +1; }

  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  first <= last &&
		  মানসমূহ_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base, Ts&&... ds) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  first <= last &&
		  মানসমূহ_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		মানসমূহ_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
সংরচনা LookupFormat4
{
  friend সংরচনা Lookup<T>;

  গুপ্ত:
  ধ্রুৱ T* get_মান (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    ধ্রুৱ LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    ঘূৰাই v ? v->get_মান (glyph_id, এতত্) : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুৱ
  {
    unsigned গুনতি = segments.get_length ();
    বাবে (unsigned i = 0; i < গুনতি; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    unsigned গুনতি = segments.get_length ();
    বাবে (unsigned i = 0; i < গুনতি; i++)
      segments[i].collect_glyphs_filtered (glyphs, এতত্, filter);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (segments.sanitize (c, এতত্));
  }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (segments.sanitize (c, এতত্, base));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_অনুসন্ধান_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
সংরচনা LookupSingle
{
  স্থির constexpr unsigned Termination_Word_গুনতি = 1u;

  পূর্ণাঙ্ক cmp (hb_codepoint_t g) ধ্রুৱ { ঘূৰাই glyph.cmp (g); }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুৱ
  {
    যদি (glyph == DELETED_GLYPH) ঘূৰাই;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    যদি (glyph == DELETED_GLYPH) ঘূৰাই;
    যদি (!filter (value)) ঘূৰাই;
    glyphs.add (glyph);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && value.sanitize (c));
  }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
সংরচনা LookupFormat6
{
  friend সংরচনা Lookup<T>;

  গুপ্ত:
  ধ্রুৱ T* get_মান (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    ধ্রুৱ LookupSingle<T> *v = entries.bsearch (glyph_id);
    ঘূৰাই v ? &v->value : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুৱ
  {
    unsigned গুনতি = entries.get_length ();
    বাবে (unsigned i = 0; i < গুনতি; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    unsigned গুনতি = entries.get_length ();
    বাবে (unsigned i = 0; i < গুনতি; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (entries.sanitize (c));
  }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (entries.sanitize (c, base));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_অনুসন্ধান_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
সংরচনা LookupFormat8
{
  friend সংরচনা Lookup<T>;

  গুপ্ত:
  ধ্রুৱ T* get_মান (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    ঘূৰাই firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_গুনতি_2 ?
	   &মান_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুৱ
  {
    যদি (unlikely (!glyph_গুনতি_2)) ঘূৰাই;
    যদি (firstGlyph == DELETED_GLYPH) ঘূৰাই;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_গুনতি_2 - 1);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    যদি (unlikely (!glyph_গুনতি_2)) ঘূৰাই;
    যদি (firstGlyph == DELETED_GLYPH) ঘূৰাই;
    ধ্রুৱ T *p = মান_Array_Z.arrayZ;
    বাবে (unsigned i = 0; i < glyph_গুনতি_2; i++)
      যদি (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && মান_Array_Z.sanitize (c, glyph_গুনতি_2));
  }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && মান_Array_Z.sanitize (c, glyph_গুনতি_2, base));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_গুনতি_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		মান_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, মান_Array_Z);
};

template <typename T>
সংরচনা LookupFormat10
{
  friend সংরচনা Lookup<T>;

  গুপ্ত:
  ধ্রুৱ typename T::type get_মান_or_null (hb_codepoint_t glyph_id) ধ্রুৱ
  {
    যদি (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_গুনতি_2))
      ঘূৰাই Null (T);

    ধ্রুৱ HBUINT8 *p = &মান_Array_Z[(glyph_id - firstGlyph) * মান_Size];

    unsigned পূর্ণাঙ্ক v = 0;
    unsigned পূর্ণাঙ্ক গুনতি = মান_Size;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      v = (v << 8) | *p++;

    ঘূৰাই v;
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs) ধ্রুৱ
  {
    যদি (unlikely (!glyph_গুনতি_2)) ঘূৰাই;
    যদি (firstGlyph == DELETED_GLYPH) ঘূৰাই;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_গুনতি_2 - 1);
  }

  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    যদি (unlikely (!glyph_গুনতি_2)) ঘূৰাই;
    যদি (firstGlyph == DELETED_GLYPH) ঘূৰাই;
    ধ্রুৱ HBUINT8 *p = মান_Array_Z.arrayZ;
    বাবে (unsigned i = 0; i < glyph_গুনতি_2; i++)
    {
      unsigned পূর্ণাঙ্ক v = 0;
      unsigned পূর্ণাঙ্ক গুনতি = মান_Size;
      বাবে (unsigned পূর্ণাঙ্ক j = 0; j < গুনতি; j++)
	v = (v << 8) | *p++;
      যদি (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  মান_Size <= 4 &&
		  মান_Array_Z.sanitize (c, glyph_গুনতি_2 * মান_Size));
  }

  রক্ষিত:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	মান_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_গুনতি_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		মান_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (8, মান_Array_Z);
};

template <typename T>
সংরচনা Lookup
{
  ধ্রুৱ T* get_মান (hb_codepoint_t glyph_id, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); ঘূৰাই u.format0.get_মান (glyph_id, অঙ্ক_glyphs);
    পক্ষ 2: hb_barrier (); ঘূৰাই u.format2.get_মান (glyph_id);
    পক্ষ 4: hb_barrier (); ঘূৰাই u.format4.get_মান (glyph_id);
    পক্ষ 6: hb_barrier (); ঘূৰাই u.format6.get_মান (glyph_id);
    পক্ষ 8: hb_barrier (); ঘূৰাই u.format8.get_মান (glyph_id);
    স্ৱভাৱ:ঘূৰাই nullptr;
    }
  }

  ধ্রুৱ typename T::type get_মান_or_null (hb_codepoint_t glyph_id, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
      /* Format 10 cannot return a pointer. */
      পক্ষ 10: hb_barrier (); ঘূৰাই u.format10.get_মান_or_null (glyph_id);
      স্ৱভাৱ:
      ধ্রুৱ T *v = get_মান (glyph_id, অঙ্ক_glyphs);
      ঘূৰাই v ? *v : Null (T);
    }
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs, unsigned পূর্ণাঙ্ক অঙ্ক_glyphs) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); u.format0.collect_glyphs (glyphs, অঙ্ক_glyphs); ঘূৰাই;
    পক্ষ 2: hb_barrier (); u.format2.collect_glyphs (glyphs); ঘূৰাই;
    পক্ষ 4: hb_barrier (); u.format4.collect_glyphs (glyphs); ঘূৰাই;
    পক্ষ 6: hb_barrier (); u.format6.collect_glyphs (glyphs); ঘূৰাই;
    পক্ষ 8: hb_barrier (); u.format8.collect_glyphs (glyphs); ঘূৰাই;
    পক্ষ 10: hb_barrier (); u.format10.collect_glyphs (glyphs); ঘূৰাই;
    স্ৱভাৱ:ঘূৰাই;
    }
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, unsigned অঙ্ক_glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, অঙ্ক_glyphs, filter); ঘূৰাই;
    পক্ষ 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); ঘূৰাই;
    পক্ষ 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); ঘূৰাই;
    পক্ষ 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); ঘূৰাই;
    পক্ষ 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); ঘূৰাই;
    পক্ষ 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); ঘূৰাই;
    স্ৱভাৱ:ঘূৰাই;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned পূর্ণাঙ্ক অঙ্ক_glyphs,
			      unsigned পূর্ণাঙ্ক outOfRange) ধ্রুৱ
  {
    ধ্রুৱ T *v = get_মান (glyph_id, অঙ্ক_glyphs);
    ঘূৰাই v ? *v : outOfRange;
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (মিছা);
    hb_barrier ();
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    পক্ষ 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    পক্ষ 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    পক্ষ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    স্ৱভাৱ:return_trace (সঁচা);
    }
  }
  তার্কিক sanitize (hb_sanitize_context_t *c, ধ্রুৱ শূন্য *base) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (!u.format.v.sanitize (c)) return_trace (মিছা);
    hb_barrier ();
    ৱিকল্প (u.format.v) {
    পক্ষ 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    পক্ষ 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    পক্ষ 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    পক্ষ 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    পক্ষ 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    পক্ষ 10: return_trace (মিছা); /* We don't support format10 here currently. */
    স্ৱভাৱ:return_trace (সঁচা);
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  সার্ৱজনিক:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
সংরচনা Entry
{
  // This doesn't seem like it's ever called.
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (এতত্));
  }

  সার্ৱজনিক:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
সংরচনা Entry<শূন্য>
{
  // This does seem like it's ever called.
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  সার্ৱজনিক:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

গণনা Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_সাৰি = 3,
};

template <typename Types, typename Extra>
সংরচনা StateTable
{
  প্রকারনির্দেশ typename Types::HBUINT HBUINT;
  প্রকারনির্দেশ typename Types::HBUSHORT HBUSHORT;
  প্রকারনির্দেশ typename Types::ClassTypeNarrow ClassType;

  গণনা State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_সাৰি = 1,
  };

  template <typename set_t, typename table_t>
  শূন্য collect_initial_glyphs (set_t &glyphs, unsigned অঙ্ক_glyphs, ধ্রুৱ table_t &table) ধ্রুৱ
  {
    unsigned অঙ্ক_classes = nClasses;

    যদি (unlikely (অঙ্ক_classes > hb_bit_page_t::BITS))
    {
      (এতত্+classTable).collect_glyphs (glyphs, অঙ্ক_glyphs);
      ঘূৰাই;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    বাবে (unsigned i = 0; i < অঙ্ক_classes; i++)
    {
      ধ্রুৱ auto &entry = get_entry (STATE_START_OF_TEXT, i);
      যদি (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	অনুৱর্ত;

      filter.add (i);
    }

    // And glyphs in those classes.

    যদি (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (এতত্+classTable).collect_glyphs_filtered (glyphs, অঙ্ক_glyphs, filter);
  }

  পূর্ণাঙ্ক new_state (unsigned পূর্ণাঙ্ক newState) ধ্রুৱ
  { ঘূৰাই Types::extended ? newState : ((পূর্ণাঙ্ক) newState - (পূর্ণাঙ্ক) stateArrayTable) / (পূর্ণাঙ্ক) nClasses; }

  unsigned পূর্ণাঙ্ক get_class (hb_codepoint_t glyph_id,
			  unsigned পূর্ণাঙ্ক অঙ্ক_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) ধ্রুৱ
  {
    unsigned klass;
    যদি (cache && cache->get (glyph_id, &klass)) ঘূৰাই klass;
    যদি (unlikely (glyph_id == DELETED_GLYPH)) ঘূৰাই CLASS_DELETED_GLYPH;
    klass = (এতত্+classTable).get_class (glyph_id, অঙ্ক_glyphs, CLASS_OUT_OF_BOUNDS);
    যদি (cache) cache->set (glyph_id, klass);
    ঘূৰাই klass;
  }

  ধ্রুৱ Entry<Extra> *get_entries () ধ্রুৱ
  { ঘূৰাই (এতত্+entryTable).arrayZ; }

  ধ্রুৱ Entry<Extra> &get_entry (পূর্ণাঙ্ক state, unsigned পূর্ণাঙ্ক klass) ধ্রুৱ
  {
    unsigned n_classes = nClasses;
    যদি (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    ধ্রুৱ HBUSHORT *states = (এতত্+stateArrayTable).arrayZ;
    ধ্রুৱ Entry<Extra> *entries = (এতত্+entryTable).arrayZ;

    unsigned পূর্ণাঙ্ক entry = states[state * n_classes + klass];
    DEBUG_বাৰ্তা (APPLY, nullptr, "e%u", entry);

    ঘূৰাই entries[entry];
  }

  তার্কিক sanitize (hb_sanitize_context_t *c,
		 unsigned পূর্ণাঙ্ক *অঙ্ক_entries_out = nullptr) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!(c->check_struct (এতত্) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, এতত্)))) return_trace (মিছা);

    ধ্রুৱ HBUSHORT *states = (এতত্+stateArrayTable).arrayZ;
    ধ্রুৱ Entry<Extra> *entries = (এতত্+entryTable).arrayZ;

    unsigned পূর্ণাঙ্ক অঙ্ক_classes = nClasses;
    যদি (unlikely (hb_unsigned_mul_overflows (অঙ্ক_classes, states[0].static_size)))
      return_trace (মিছা);
    unsigned পূর্ণাঙ্ক row_stride = অঙ্ক_classes * states[0].static_size;

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

    পূর্ণাঙ্ক min_state = 0;
    পূর্ণাঙ্ক max_state = 0;
    unsigned পূর্ণাঙ্ক অঙ্ক_entries = 0;

    পূর্ণাঙ্ক state_pos = 0;
    পূর্ণাঙ্ক state_neg = 0;
    unsigned পূর্ণাঙ্ক entry = 0;
    যেতিয়ালৈ (min_state < state_neg || state_pos <= max_state)
    {
      যদি (min_state < state_neg)
      {
	/* Negative states. */
	যদি (unlikely (hb_unsigned_mul_overflows (min_state, অঙ্ক_classes)))
	  return_trace (মিছা);
	যদি (unlikely (!c->check_range (&states[min_state * অঙ্ক_classes],
				       -min_state,
				       row_stride)))
	  return_trace (মিছা);
	যদি ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (মিছা);
	{ /* Sweep new states. */
	  ধ্রুৱ HBUSHORT *stop = &states[min_state * অঙ্ক_classes];
	  যদি (unlikely (stop > states))
	    return_trace (মিছা);
	  বাবে (ধ্রুৱ HBUSHORT *p = states; stop < p; p--)
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
	  return_trace (মিছা);
	যদি ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (মিছা);
	{ /* Sweep new states. */
	  যদি (unlikely (hb_unsigned_mul_overflows ((max_state + 1), অঙ্ক_classes)))
	    return_trace (মিছা);
	  ধ্রুৱ HBUSHORT *stop = &states[(max_state + 1) * অঙ্ক_classes];
	  যদি (unlikely (stop < states))
	    return_trace (মিছা);
	  বাবে (ধ্রুৱ HBUSHORT *p = &states[state_pos * অঙ্ক_classes]; p < stop; p++)
	    অঙ্ক_entries = hb_max (অঙ্ক_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      যদি (unlikely (!c->check_array (entries, অঙ্ক_entries)))
	return_trace (মিছা);
      যদি ((c->max_ops -= অঙ্ক_entries - entry) <= 0)
	return_trace (মিছা);
      { /* Sweep new entries. */
	ধ্রুৱ Entry<Extra> *stop = &entries[অঙ্ক_entries];
	বাবে (ধ্রুৱ Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  পূর্ণাঙ্ক newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = অঙ্ক_entries;
      }
    }

    যদি (অঙ্ক_entries_out)
      *অঙ্ক_entries_out = অঙ্ক_entries;

    return_trace (সঁচা);
  }

  রক্ষিত:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4 * পরিমাণ (HBUINT));
};

template <typename HBUCHAR>
সংরচনা ClassTable
{
  unsigned পূর্ণাঙ্ক get_class (hb_codepoint_t glyph_id, unsigned পূর্ণাঙ্ক outOfRange) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক i = glyph_id - firstGlyph;
    ঘূৰাই i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned পূর্ণাঙ্ক get_class (hb_codepoint_t glyph_id,
			  unsigned পূর্ণাঙ্ক অঙ্ক_glyphs HB_UNUSED,
			  unsigned পূর্ণাঙ্ক outOfRange) ধ্রুৱ
  {
    ঘূৰাই get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  শূন্য collect_glyphs (set_t &glyphs, unsigned অঙ্ক_glyphs) ধ্রুৱ
  {
    বাবে (unsigned i = 0; i < classArray.len; i++)
      যদি (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  শূন্য collect_glyphs_filtered (set_t &glyphs, unsigned অঙ্ক_glyphs, ধ্রুৱ filter_t &filter) ধ্রুৱ
  {
    বাবে (unsigned i = 0; i < classArray.len; i++)
      যদি (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) && classArray.sanitize (c));
  }
  রক্ষিত:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, classArray);
};

সংরচনা SubtableGlyphCoverage
{
  তার্কিক sanitize (hb_sanitize_context_t *c, unsigned subtable_গুনতি) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);

    যদি (unlikely (!c->check_array (&subtableOffsets, subtable_গুনতি)))
      return_trace (মিছা);

    unsigned bytes = (c->get_অঙ্ক_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    বাবে (unsigned i = 0; i < subtable_গুনতি; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      যদি (offset == 0 || offset == 0xFFFFFFFF)
        অনুৱর্ত;
      যদি (unlikely (!subtableOffsets[i].sanitize (c, এতত্, bytes)))
        return_trace (মিছা);
    }

    return_trace (সঁচা);
  }
  রক্ষিত:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

সংরচনা ObsoleteTypes
{
  স্থির constexpr তার্কিক extended = মিছা;
  প্রকারনির্দেশ HBUINT16 HBUINT;
  প্রকারনির্দেশ HBUINT8 HBUSHORT;
  প্রকারনির্দেশ ClassTable<HBUINT8> ClassTypeNarrow;
  প্রকারনির্দেশ ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  স্থির পূর্ণাঙ্ক offsetToIndex (int64_t offset,
			    ধ্রুৱ শূন্য *base,
			    ধ্রুৱ T *array)
  {
    int64_t array_offset = (ধ্রুৱ ৱর্ণ *) array - (ধ্রুৱ ৱর্ণ *) base;
    পূর্ণাঙ্ক bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    যদি (unlikely (offset < array_offset))
      ঘূৰাই bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    যদি (unlikely (index > bad_index))
      ঘূৰাই bad_index;
    ঘূৰাই index;
  }
  template <typename T>
  স্থির পূর্ণাঙ্ক byteOffsetToIndex (int64_t offset,
				ধ্রুৱ শূন্য *base,
				ধ্রুৱ T *array)
  {
    ঘূৰাই offsetToIndex (offset, base, array);
  }
  template <typename T>
  স্থির পূর্ণাঙ্ক wordOffsetToIndex (int64_t offset,
				ধ্রুৱ শূন্য *base,
				ধ্রুৱ T *array)
  {
    ঘূৰাই offsetToIndex (2 * offset, base, array);
  }
};
সংরচনা ExtendedTypes
{
  স্থির constexpr তার্কিক extended = সঁচা;
  প্রকারনির্দেশ HBUINT32 HBUINT;
  প্রকারনির্দেশ HBUINT16 HBUSHORT;
  প্রকারনির্দেশ Lookup<HBUINT16> ClassTypeNarrow;
  প্রকারনির্দেশ Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  স্থির unsigned পূর্ণাঙ্ক offsetToIndex (unsigned পূর্ণাঙ্ক offset,
				     ধ্রুৱ শূন্য *base HB_UNUSED,
				     ধ্রুৱ T *array HB_UNUSED)
  {
    ঘূৰাই offset;
  }
  template <typename T>
  স্থির unsigned পূর্ণাঙ্ক byteOffsetToIndex (unsigned পূর্ণাঙ্ক offset,
					 ধ্রুৱ শূন্য *base HB_UNUSED,
					 ধ্রুৱ T *array HB_UNUSED)
  {
    ঘূৰাই offset / 2;
  }
  template <typename T>
  স্থির unsigned পূর্ণাঙ্ক wordOffsetToIndex (unsigned পূর্ণাঙ্ক offset,
					 ধ্রুৱ শূন্য *base HB_UNUSED,
					 ধ্রুৱ T *array HB_UNUSED)
  {
    ঘূৰাই offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
সংরচনা StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (ধ্রুৱ StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      অঙ্ক_glyphs (face_->get_অঙ্ক_glyphs ()) {}

  template <typename context_t>
  শূন্য drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    যদি (!c->in_place)
      buffer->clear_output ();

    পূর্ণাঙ্ক state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    ধ্রুৱ তার্কিক start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    বাবে (buffer->idx = 0; buffer->successful;)
    {
      unsigned পূর্ণাঙ্ক klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, অঙ্ক_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_বাৰ্তা (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      ধ্রুৱ EntryT &entry = machine.get_entry (state, klass);
      ধ্রুৱ পূর্ণাঙ্ক next_state = machine.new_state (entry.newState);

      তার্কিক is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      তার্কিক is_not_actionable = !c->table->is_actionable (entry);

      যদি (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	যদি (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  যেতিয়ালৈ (cluster < range->cluster_first)
	    range--;
	  যেতিয়ালৈ (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	যদি (!(range->flags & ac->subtable_flags))
	{
	  যদি (buffer->idx == buffer->len)
	    ভঙ্গ;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (শূন্য) buffer->next_glyph ();
	  অনুৱর্ত;
	}
      }
      অন্যথা
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	তার্কিক is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	যদি (is_null_transition)
	{
	  unsigned old_klass = klass;
	  কৰা
	  {
	    c->transition (buffer, এতত্, entry);

	    যদি (buffer->idx == buffer->len || !buffer->successful)
	      ভঙ্গ;

	    (শূন্য) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, অঙ্ক_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } যেতিয়ালৈ (klass == old_klass);

	  যদি (buffer->idx == buffer->len || !buffer->successful)
	    ভঙ্গ;

	  গচ্ছ resume;
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
      ধ্রুৱ EntryT *wouldbe_entry;
      তার্কিক is_safe_to_break =
      (
          /* 1. */
          !c->table->is_actionable (entry) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == StateTableT::STATE_START_OF_TEXT
              || ((entry.flags & Flags::DontAdvance) && next_state == StateTableT::STATE_START_OF_TEXT)
              || (
		    /* 2c. */
		    wouldbe_entry = &machine.get_entry(StateTableT::STATE_START_OF_TEXT, klass)
		    ,
		    /* 2c'. */
		    !c->table->is_actionable (*wouldbe_entry) &&
		    /* 2c". */
		    (
		      next_state == machine.new_state(wouldbe_entry->newState) &&
		      (entry.flags & Flags::DontAdvance) == (wouldbe_entry->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->table->is_actionable (machine.get_entry (state, CLASS_END_OF_TEXT))
      );

      যদি (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, এতত্, entry);

      state = next_state;
      DEBUG_বাৰ্তা (APPLY, nullptr, "s%d", state);

      যদি (buffer->idx == buffer->len)
	ভঙ্গ;

      যদি (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(শূন্য) buffer->next_glyph ();
    }

    যদি (!c->in_place)
      buffer->সমকাল ();
  }

  সার্ৱজনিক:
  ধ্রুৱ StateTableT &machine;
  unsigned পূর্ণাঙ্ক অঙ্ক_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
