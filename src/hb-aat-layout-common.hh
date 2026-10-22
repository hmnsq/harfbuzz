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

#अन्तर्भाव "hb-aat-layout.hh"
#अन्तर्भाव "hb-aat-map.hh"
#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-ot-layout-gdef-table.hh"
#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-cache.hh"
#अन्तर्भाव "hb-bit-set.hh"
#अन्तर्भाव "hb-bit-page.hh"


नामाकाश OT {
संरचना GDEF;
};

नामाकाश AAT {

using नामाकाश OT;

संरचना ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#अण्णहा
#define HB_AAT_TRANSITION_INLINE
#endif

संरचना hb_aat_scratch_t
{
  hb_aat_scratch_t () = स्वभाव;
  hb_aat_scratch_t (ध्रुव hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    प्रत्यागम *एतत्;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    जइ (unlikely (!s))
      प्रत्यागम;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () ध्रुव
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    जइ (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      प्रत्यागम s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, परिमाण (hb_bit_set_t));
    जइ (unlikely (!s))
      प्रत्यागम nullptr;
    s->init ();

    प्रत्यागम s;
  }
  शून्य destroy_buffer_glyph_set (hb_bit_set_t *s) ध्रुव
  {
    जइ (unlikely (!s))
      प्रत्यागम;
    जइ (buffer_glyph_set.cmpexch (nullptr, s))
      प्रत्यागम;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

गणना { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

संरचना hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, तार्किक, HB_DEBUG_APPLY>
{
  ध्रुव वर्ण *get_name () { प्रत्यागम "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (ध्रुव T &obj, Ts&&... ds)
  { प्रत्यागम obj.apply (एतत्, std::forward<Ts> (ds)...); }
  स्थिर return_t default_return_मानम् () { प्रत्यागम मिच्छ; }
  तार्किक stop_sublookup_iteration (return_t r) ध्रुव { प्रत्यागम r; }

  ध्रुव hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  ध्रुव ankr *ankr_table;
  ध्रुव OT::GDEF &gdef;
  तार्किक has_glyph_classes;
  ध्रुव hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  तार्किक buffer_is_reversed = मिच्छ;
  // Caches
  तार्किक using_buffer_glyph_set = मिच्छ;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  ध्रुव hb_bit_set_t *first_set = nullptr;
  ध्रुव hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned पूर्णाङ्क lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (ध्रुव hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL शून्य set_ankr_table (ध्रुव AAT::ankr *ankr_table_);

  शून्य set_lookup_index (unsigned पूर्णाङ्क i) { lookup_index = i; }

  शून्य reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  शून्य setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    जइ (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  तार्किक buffer_intersects_machine () ध्रुव
  {
    जइ (likely (using_buffer_glyph_set))
      प्रत्यागम buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    प्रति (unsigned i = 0; i < buffer->len; i++)
      जइ (first_set->has (buffer->info[i].codepoint))
	प्रत्यागम सच्च;
    प्रत्यागम मिच्छ;
  }

  template <typename T>
  HB_NODISCARD तार्किक output_glyphs (unsigned पूर्णाङ्क गणनम्,
				   ध्रुव T *glyphs)
  {
    जइ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, गणनम्);
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      तार्किक at_end_of_text = buffer->idx == buffer->len;
      जइ (at_end_of_text)
	जइ (unlikely (!buffer->output_glyph (glyphs[i]))) प्रत्यागम मिच्छ;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      जइ (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      अण्णहा
      {
#ifndef HB_NO_OT_LAYOUT
	जइ (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      जइ (!at_end_of_text)
	जइ (unlikely (!buffer->output_glyph (glyphs[i]))) प्रत्यागम मिच्छ;
    }
    प्रत्यागम सच्च;
  }

  HB_NODISCARD तार्किक replace_glyph (hb_codepoint_t glyph)
  {
    जइ (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    जइ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    जइ (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    प्रत्यागम buffer->replace_glyph (glyph);
  }

  HB_NODISCARD तार्किक delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    जइ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    प्रत्यागम buffer->replace_glyph (DELETED_GLYPH);
  }

  शून्य replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    जइ (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    जइ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    जइ (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> संरचना Lookup;

template <typename T>
संरचना LookupFormat0
{
  friend संरचना Lookup<T>;

  गुप्त:
  ध्रुव T* get_मानम् (hb_codepoint_t glyph_id, unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  {
    जइ (unlikely (glyph_id >= अङ्कः_glyphs)) प्रत्यागम nullptr;
    प्रत्यागम &arrayZ[glyph_id];
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, unsigned अङ्कः_glyphs) ध्रुव
  {
    glyphs.add_range (0, अङ्कः_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, unsigned अङ्कः_glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    प्रति (unsigned i = 0; i < अङ्कः_glyphs; i++)
      जइ (filter (arrayZ[i]))
	glyphs.add (i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (arrayZ.sanitize (c, c->get_अङ्कः_glyphs ()));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (arrayZ.sanitize (c, c->get_अङ्कः_glyphs (), base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  सार्वजनिक:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
संरचना LookupSegmentSingle
{
  स्थिर constexpr unsigned Termination_Word_गणनम् = 2u;

  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव
  { प्रत्यागम g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) ध्रुव
  {
    जइ (first == DELETED_GLYPH) प्रत्यागम;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    जइ (first == DELETED_GLYPH) प्रत्यागम;
    जइ (!filter (value)) प्रत्यागम;
    glyphs.add_range (first, last);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
संरचना LookupFormat2
{
  friend संरचना Lookup<T>;

  गुप्त:
  ध्रुव T* get_मानम् (hb_codepoint_t glyph_id) ध्रुव
  {
    ध्रुव LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    प्रत्यागम v ? &v->value : nullptr;
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) ध्रुव
  {
    unsigned गणनम् = segments.get_length ();
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    unsigned गणनम् = segments.get_length ();
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (segments.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (segments.sanitize (c, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_अन्वेषणम्_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
संरचना LookupSegmentArray
{
  स्थिर constexpr unsigned Termination_Word_गणनम् = 2u;

  ध्रुव T* get_मानम् (hb_codepoint_t glyph_id, ध्रुव शून्य *base) ध्रुव
  {
    प्रत्यागम first <= glyph_id && glyph_id <= last ? &(base+मानानि_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) ध्रुव
  {
    जइ (first == DELETED_GLYPH) प्रत्यागम;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, ध्रुव शून्य *base, ध्रुव filter_t &filter) ध्रुव
  {
    जइ (first == DELETED_GLYPH) प्रत्यागम;
    ध्रुव auto &values = base+मानानि_Z;
    प्रति (hb_codepoint_t i = first; i <= last; i++)
      जइ (filter (values[i - first]))
	glyphs.add (i);
  }

  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव
  { प्रत्यागम g < first ? -1 : g <= last ? 0 : +1; }

  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  first <= last &&
		  मानानि_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base, Ts&&... ds) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  first <= last &&
		  मानानि_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		मानानि_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
संरचना LookupFormat4
{
  friend संरचना Lookup<T>;

  गुप्त:
  ध्रुव T* get_मानम् (hb_codepoint_t glyph_id) ध्रुव
  {
    ध्रुव LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    प्रत्यागम v ? v->get_मानम् (glyph_id, एतत्) : nullptr;
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) ध्रुव
  {
    unsigned गणनम् = segments.get_length ();
    प्रति (unsigned i = 0; i < गणनम्; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    unsigned गणनम् = segments.get_length ();
    प्रति (unsigned i = 0; i < गणनम्; i++)
      segments[i].collect_glyphs_filtered (glyphs, एतत्, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (segments.sanitize (c, एतत्));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (segments.sanitize (c, एतत्, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_अन्वेषणम्_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
संरचना LookupSingle
{
  स्थिर constexpr unsigned Termination_Word_गणनम् = 1u;

  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव { प्रत्यागम glyph.cmp (g); }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) ध्रुव
  {
    जइ (glyph == DELETED_GLYPH) प्रत्यागम;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    जइ (glyph == DELETED_GLYPH) प्रत्यागम;
    जइ (!filter (value)) प्रत्यागम;
    glyphs.add (glyph);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
संरचना LookupFormat6
{
  friend संरचना Lookup<T>;

  गुप्त:
  ध्रुव T* get_मानम् (hb_codepoint_t glyph_id) ध्रुव
  {
    ध्रुव LookupSingle<T> *v = entries.bsearch (glyph_id);
    प्रत्यागम v ? &v->value : nullptr;
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) ध्रुव
  {
    unsigned गणनम् = entries.get_length ();
    प्रति (unsigned i = 0; i < गणनम्; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    unsigned गणनम् = entries.get_length ();
    प्रति (unsigned i = 0; i < गणनम्; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (entries.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (entries.sanitize (c, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_अन्वेषणम्_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
संरचना LookupFormat8
{
  friend संरचना Lookup<T>;

  गुप्त:
  ध्रुव T* get_मानम् (hb_codepoint_t glyph_id) ध्रुव
  {
    प्रत्यागम firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_गणनम्_2 ?
	   &मानम्_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) ध्रुव
  {
    जइ (unlikely (!glyph_गणनम्_2)) प्रत्यागम;
    जइ (firstGlyph == DELETED_GLYPH) प्रत्यागम;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_गणनम्_2 - 1);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    जइ (unlikely (!glyph_गणनम्_2)) प्रत्यागम;
    जइ (firstGlyph == DELETED_GLYPH) प्रत्यागम;
    ध्रुव T *p = मानम्_Array_Z.arrayZ;
    प्रति (unsigned i = 0; i < glyph_गणनम्_2; i++)
      जइ (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && मानम्_Array_Z.sanitize (c, glyph_गणनम्_2));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && मानम्_Array_Z.sanitize (c, glyph_गणनम्_2, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_गणनम्_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		मानम्_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, मानम्_Array_Z);
};

template <typename T>
संरचना LookupFormat10
{
  friend संरचना Lookup<T>;

  गुप्त:
  ध्रुव typename T::type get_मानम्_or_null (hb_codepoint_t glyph_id) ध्रुव
  {
    जइ (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_गणनम्_2))
      प्रत्यागम Null (T);

    ध्रुव HBUINT8 *p = &मानम्_Array_Z[(glyph_id - firstGlyph) * मानम्_Size];

    unsigned पूर्णाङ्क v = 0;
    unsigned पूर्णाङ्क गणनम् = मानम्_Size;
    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      v = (v << 8) | *p++;

    प्रत्यागम v;
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) ध्रुव
  {
    जइ (unlikely (!glyph_गणनम्_2)) प्रत्यागम;
    जइ (firstGlyph == DELETED_GLYPH) प्रत्यागम;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_गणनम्_2 - 1);
  }

  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    जइ (unlikely (!glyph_गणनम्_2)) प्रत्यागम;
    जइ (firstGlyph == DELETED_GLYPH) प्रत्यागम;
    ध्रुव HBUINT8 *p = मानम्_Array_Z.arrayZ;
    प्रति (unsigned i = 0; i < glyph_गणनम्_2; i++)
    {
      unsigned पूर्णाङ्क v = 0;
      unsigned पूर्णाङ्क गणनम् = मानम्_Size;
      प्रति (unsigned पूर्णाङ्क j = 0; j < गणनम्; j++)
	v = (v << 8) | *p++;
      जइ (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  मानम्_Size <= 4 &&
		  मानम्_Array_Z.sanitize (c, glyph_गणनम्_2 * मानम्_Size));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	मानम्_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_गणनम्_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		मानम्_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, मानम्_Array_Z);
};

template <typename T>
संरचना Lookup
{
  ध्रुव T* get_मानम् (hb_codepoint_t glyph_id, unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); प्रत्यागम u.format0.get_मानम् (glyph_id, अङ्कः_glyphs);
    पक्ष 2: hb_barrier (); प्रत्यागम u.format2.get_मानम् (glyph_id);
    पक्ष 4: hb_barrier (); प्रत्यागम u.format4.get_मानम् (glyph_id);
    पक्ष 6: hb_barrier (); प्रत्यागम u.format6.get_मानम् (glyph_id);
    पक्ष 8: hb_barrier (); प्रत्यागम u.format8.get_मानम् (glyph_id);
    स्वभाव:प्रत्यागम nullptr;
    }
  }

  ध्रुव typename T::type get_मानम्_or_null (hb_codepoint_t glyph_id, unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  {
    विकल्प (u.format.v) {
      /* Format 10 cannot return a pointer. */
      पक्ष 10: hb_barrier (); प्रत्यागम u.format10.get_मानम्_or_null (glyph_id);
      स्वभाव:
      ध्रुव T *v = get_मानम् (glyph_id, अङ्कः_glyphs);
      प्रत्यागम v ? *v : Null (T);
    }
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, unsigned पूर्णाङ्क अङ्कः_glyphs) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); u.format0.collect_glyphs (glyphs, अङ्कः_glyphs); प्रत्यागम;
    पक्ष 2: hb_barrier (); u.format2.collect_glyphs (glyphs); प्रत्यागम;
    पक्ष 4: hb_barrier (); u.format4.collect_glyphs (glyphs); प्रत्यागम;
    पक्ष 6: hb_barrier (); u.format6.collect_glyphs (glyphs); प्रत्यागम;
    पक्ष 8: hb_barrier (); u.format8.collect_glyphs (glyphs); प्रत्यागम;
    पक्ष 10: hb_barrier (); u.format10.collect_glyphs (glyphs); प्रत्यागम;
    स्वभाव:प्रत्यागम;
    }
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, unsigned अङ्कः_glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, अङ्कः_glyphs, filter); प्रत्यागम;
    पक्ष 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); प्रत्यागम;
    पक्ष 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); प्रत्यागम;
    पक्ष 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); प्रत्यागम;
    पक्ष 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); प्रत्यागम;
    पक्ष 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); प्रत्यागम;
    स्वभाव:प्रत्यागम;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned पूर्णाङ्क अङ्कः_glyphs,
			      unsigned पूर्णाङ्क outOfRange) ध्रुव
  {
    ध्रुव T *v = get_मानम् (glyph_id, अङ्कः_glyphs);
    प्रत्यागम v ? *v : outOfRange;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!u.format.v.sanitize (c)) return_trace (मिच्छ);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    पक्ष 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    पक्ष 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    पक्ष 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    स्वभाव:return_trace (सच्च);
    }
  }
  तार्किक sanitize (hb_sanitize_context_t *c, ध्रुव शून्य *base) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (!u.format.v.sanitize (c)) return_trace (मिच्छ);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    पक्ष 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    पक्ष 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    पक्ष 10: return_trace (मिच्छ); /* We don't support format10 here currently. */
    स्वभाव:return_trace (सच्च);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
संरचना Entry
{
  // This doesn't seem like it's ever called.
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
संरचना Entry<शून्य>
{
  // This does seem like it's ever called.
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

गणना Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_पङ्क्तिः = 3,
};

template <typename Types, typename Extra>
संरचना StateTable
{
  प्रकारनिर्देश typename Types::HBUINT HBUINT;
  प्रकारनिर्देश typename Types::HBUSHORT HBUSHORT;
  प्रकारनिर्देश typename Types::ClassTypeNarrow ClassType;

  गणना State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_पङ्क्तिः = 1,
  };

  template <typename set_t, typename table_t>
  शून्य collect_initial_glyphs (set_t &glyphs, unsigned अङ्कः_glyphs, ध्रुव table_t &table) ध्रुव
  {
    unsigned अङ्कः_classes = nClasses;

    जइ (unlikely (अङ्कः_classes > hb_bit_page_t::BITS))
    {
      (एतत्+classTable).collect_glyphs (glyphs, अङ्कः_glyphs);
      प्रत्यागम;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    प्रति (unsigned i = 0; i < अङ्कः_classes; i++)
    {
      ध्रुव auto &entry = get_entry (STATE_START_OF_TEXT, i);
      जइ (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	अनुवर्त;

      filter.add (i);
    }

    // And glyphs in those classes.

    जइ (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (एतत्+classTable).collect_glyphs_filtered (glyphs, अङ्कः_glyphs, filter);
  }

  पूर्णाङ्क new_state (unsigned पूर्णाङ्क newState) ध्रुव
  { प्रत्यागम Types::extended ? newState : ((पूर्णाङ्क) newState - (पूर्णाङ्क) stateArrayTable) / (पूर्णाङ्क) nClasses; }

  unsigned पूर्णाङ्क get_class (hb_codepoint_t glyph_id,
			  unsigned पूर्णाङ्क अङ्कः_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) ध्रुव
  {
    unsigned klass;
    जइ (cache && cache->get (glyph_id, &klass)) प्रत्यागम klass;
    जइ (unlikely (glyph_id == DELETED_GLYPH)) प्रत्यागम CLASS_DELETED_GLYPH;
    klass = (एतत्+classTable).get_class (glyph_id, अङ्कः_glyphs, CLASS_OUT_OF_BOUNDS);
    जइ (cache) cache->set (glyph_id, klass);
    प्रत्यागम klass;
  }

  ध्रुव Entry<Extra> *get_entries () ध्रुव
  { प्रत्यागम (एतत्+entryTable).arrayZ; }

  ध्रुव Entry<Extra> &get_entry (पूर्णाङ्क state, unsigned पूर्णाङ्क klass) ध्रुव
  {
    unsigned n_classes = nClasses;
    जइ (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    ध्रुव HBUSHORT *states = (एतत्+stateArrayTable).arrayZ;
    ध्रुव Entry<Extra> *entries = (एतत्+entryTable).arrayZ;

    unsigned पूर्णाङ्क entry = states[state * n_classes + klass];
    DEBUG_सन्देशः (APPLY, nullptr, "e%u", entry);

    प्रत्यागम entries[entry];
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 unsigned पूर्णाङ्क *अङ्कः_entries_out = nullptr) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    जइ (unlikely (!(c->check_struct (एतत्) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, एतत्)))) return_trace (मिच्छ);

    ध्रुव HBUSHORT *states = (एतत्+stateArrayTable).arrayZ;
    ध्रुव Entry<Extra> *entries = (एतत्+entryTable).arrayZ;

    unsigned पूर्णाङ्क अङ्कः_classes = nClasses;
    जइ (unlikely (hb_unsigned_mul_overflows (अङ्कः_classes, states[0].static_size)))
      return_trace (मिच्छ);
    unsigned पूर्णाङ्क row_stride = अङ्कः_classes * states[0].static_size;

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

    पूर्णाङ्क min_state = 0;
    पूर्णाङ्क max_state = 0;
    unsigned पूर्णाङ्क अङ्कः_entries = 0;

    पूर्णाङ्क state_pos = 0;
    पूर्णाङ्क state_neg = 0;
    unsigned पूर्णाङ्क entry = 0;
    जाव (min_state < state_neg || state_pos <= max_state)
    {
      जइ (min_state < state_neg)
      {
	/* Negative states. */
	जइ (unlikely (hb_unsigned_mul_overflows (min_state, अङ्कः_classes)))
	  return_trace (मिच्छ);
	जइ (unlikely (!c->check_range (&states[min_state * अङ्कः_classes],
				       -min_state,
				       row_stride)))
	  return_trace (मिच्छ);
	जइ ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (मिच्छ);
	{ /* Sweep new states. */
	  ध्रुव HBUSHORT *stop = &states[min_state * अङ्कः_classes];
	  जइ (unlikely (stop > states))
	    return_trace (मिच्छ);
	  प्रति (ध्रुव HBUSHORT *p = states; stop < p; p--)
	    अङ्कः_entries = hb_max (अङ्कः_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      जइ (state_pos <= max_state)
      {
	/* Positive states. */
	जइ (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (मिच्छ);
	जइ ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (मिच्छ);
	{ /* Sweep new states. */
	  जइ (unlikely (hb_unsigned_mul_overflows ((max_state + 1), अङ्कः_classes)))
	    return_trace (मिच्छ);
	  ध्रुव HBUSHORT *stop = &states[(max_state + 1) * अङ्कः_classes];
	  जइ (unlikely (stop < states))
	    return_trace (मिच्छ);
	  प्रति (ध्रुव HBUSHORT *p = &states[state_pos * अङ्कः_classes]; p < stop; p++)
	    अङ्कः_entries = hb_max (अङ्कः_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      जइ (unlikely (!c->check_array (entries, अङ्कः_entries)))
	return_trace (मिच्छ);
      जइ ((c->max_ops -= अङ्कः_entries - entry) <= 0)
	return_trace (मिच्छ);
      { /* Sweep new entries. */
	ध्रुव Entry<Extra> *stop = &entries[अङ्कः_entries];
	प्रति (ध्रुव Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  पूर्णाङ्क newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = अङ्कः_entries;
      }
    }

    जइ (अङ्कः_entries_out)
      *अङ्कः_entries_out = अङ्कः_entries;

    return_trace (सच्च);
  }

  रक्षित:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 * परिमाण (HBUINT));
};

template <typename HBUCHAR>
संरचना ClassTable
{
  unsigned पूर्णाङ्क get_class (hb_codepoint_t glyph_id, unsigned पूर्णाङ्क outOfRange) ध्रुव
  {
    unsigned पूर्णाङ्क i = glyph_id - firstGlyph;
    प्रत्यागम i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned पूर्णाङ्क get_class (hb_codepoint_t glyph_id,
			  unsigned पूर्णाङ्क अङ्कः_glyphs HB_UNUSED,
			  unsigned पूर्णाङ्क outOfRange) ध्रुव
  {
    प्रत्यागम get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, unsigned अङ्कः_glyphs) ध्रुव
  {
    प्रति (unsigned i = 0; i < classArray.len; i++)
      जइ (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, unsigned अङ्कः_glyphs, ध्रुव filter_t &filter) ध्रुव
  {
    प्रति (unsigned i = 0; i < classArray.len; i++)
      जइ (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && classArray.sanitize (c));
  }
  रक्षित:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, classArray);
};

संरचना SubtableGlyphCoverage
{
  तार्किक sanitize (hb_sanitize_context_t *c, unsigned subtable_गणनम्) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    जइ (unlikely (!c->check_array (&subtableOffsets, subtable_गणनम्)))
      return_trace (मिच्छ);

    unsigned bytes = (c->get_अङ्कः_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    प्रति (unsigned i = 0; i < subtable_गणनम्; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      जइ (offset == 0 || offset == 0xFFFFFFFF)
        अनुवर्त;
      जइ (unlikely (!subtableOffsets[i].sanitize (c, एतत्, bytes)))
        return_trace (मिच्छ);
    }

    return_trace (सच्च);
  }
  रक्षित:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

संरचना ObsoleteTypes
{
  स्थिर constexpr तार्किक extended = मिच्छ;
  प्रकारनिर्देश HBUINT16 HBUINT;
  प्रकारनिर्देश HBUINT8 HBUSHORT;
  प्रकारनिर्देश ClassTable<HBUINT8> ClassTypeNarrow;
  प्रकारनिर्देश ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  स्थिर पूर्णाङ्क offsetToIndex (int64_t offset,
			    ध्रुव शून्य *base,
			    ध्रुव T *array)
  {
    int64_t array_offset = (ध्रुव वर्ण *) array - (ध्रुव वर्ण *) base;
    पूर्णाङ्क bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    जइ (unlikely (offset < array_offset))
      प्रत्यागम bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    जइ (unlikely (index > bad_index))
      प्रत्यागम bad_index;
    प्रत्यागम index;
  }
  template <typename T>
  स्थिर पूर्णाङ्क byteOffsetToIndex (int64_t offset,
				ध्रुव शून्य *base,
				ध्रुव T *array)
  {
    प्रत्यागम offsetToIndex (offset, base, array);
  }
  template <typename T>
  स्थिर पूर्णाङ्क wordOffsetToIndex (int64_t offset,
				ध्रुव शून्य *base,
				ध्रुव T *array)
  {
    प्रत्यागम offsetToIndex (2 * offset, base, array);
  }
};
संरचना ExtendedTypes
{
  स्थिर constexpr तार्किक extended = सच्च;
  प्रकारनिर्देश HBUINT32 HBUINT;
  प्रकारनिर्देश HBUINT16 HBUSHORT;
  प्रकारनिर्देश Lookup<HBUINT16> ClassTypeNarrow;
  प्रकारनिर्देश Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  स्थिर unsigned पूर्णाङ्क offsetToIndex (unsigned पूर्णाङ्क offset,
				     ध्रुव शून्य *base HB_UNUSED,
				     ध्रुव T *array HB_UNUSED)
  {
    प्रत्यागम offset;
  }
  template <typename T>
  स्थिर unsigned पूर्णाङ्क byteOffsetToIndex (unsigned पूर्णाङ्क offset,
					 ध्रुव शून्य *base HB_UNUSED,
					 ध्रुव T *array HB_UNUSED)
  {
    प्रत्यागम offset / 2;
  }
  template <typename T>
  स्थिर unsigned पूर्णाङ्क wordOffsetToIndex (unsigned पूर्णाङ्क offset,
					 ध्रुव शून्य *base HB_UNUSED,
					 ध्रुव T *array HB_UNUSED)
  {
    प्रत्यागम offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
संरचना StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (ध्रुव StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      अङ्कः_glyphs (face_->get_अङ्कः_glyphs ()) {}

  template <typename context_t>
  शून्य drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    जइ (!c->in_place)
      buffer->clear_output ();

    पूर्णाङ्क state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    ध्रुव तार्किक start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    प्रति (buffer->idx = 0; buffer->successful;)
    {
      unsigned पूर्णाङ्क klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, अङ्कः_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_सन्देशः (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      ध्रुव EntryT &entry = machine.get_entry (state, klass);
      ध्रुव पूर्णाङ्क next_state = machine.new_state (entry.newState);

      तार्किक is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      तार्किक is_not_actionable = !c->table->is_actionable (entry);

      जइ (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	जइ (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  जाव (cluster < range->cluster_first)
	    range--;
	  जाव (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	जइ (!(range->flags & ac->subtable_flags))
	{
	  जइ (buffer->idx == buffer->len)
	    भङ्ग;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (शून्य) buffer->next_glyph ();
	  अनुवर्त;
	}
      }
      अण्णहा
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	तार्किक is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	जइ (is_null_transition)
	{
	  unsigned old_klass = klass;
	  करेहि
	  {
	    c->transition (buffer, एतत्, entry);

	    जइ (buffer->idx == buffer->len || !buffer->successful)
	      भङ्ग;

	    (शून्य) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, अङ्कः_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } जाव (klass == old_klass);

	  जइ (buffer->idx == buffer->len || !buffer->successful)
	    भङ्ग;

	  गच्छ resume;
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
      ध्रुव EntryT *wouldbe_entry;
      तार्किक is_safe_to_break =
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

      जइ (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, एतत्, entry);

      state = next_state;
      DEBUG_सन्देशः (APPLY, nullptr, "s%d", state);

      जइ (buffer->idx == buffer->len)
	भङ्ग;

      जइ (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(शून्य) buffer->next_glyph ();
    }

    जइ (!c->in_place)
      buffer->समकाल ();
  }

  सार्वजनिक:
  ध्रुव StateTableT &machine;
  unsigned पूर्णाङ्क अङ्कः_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
