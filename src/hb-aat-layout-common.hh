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

#அந்தர்பாவ "hb-aat-layout.hh"
#அந்தர்பாவ "hb-aat-map.hh"
#அந்தர்பாவ "hb-ot-layout-common.hh"
#அந்தர்பாவ "hb-ot-layout-gdef-table.hh"
#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-cache.hh"
#அந்தர்பாவ "hb-bit-set.hh"
#அந்தர்பாவ "hb-bit-page.hh"


நாமாகாஶ OT {
கட்டமைப்பு GDEF;
};

நாமாகாஶ AAT {

using நாமாகாஶ OT;

கட்டமைப்பு ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#இல்லையெனில்
#define HB_AAT_TRANSITION_INLINE
#endif

கட்டமைப்பு hb_aat_scratch_t
{
  hb_aat_scratch_t () = இயல்பு;
  hb_aat_scratch_t (மாறிலி hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    திரும்பு *இது;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    எனில் (unlikely (!s))
      திரும்பு;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () மாறிலி
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    எனில் (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      திரும்பு s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, அளவு (hb_bit_set_t));
    எனில் (unlikely (!s))
      திரும்பு nullptr;
    s->init ();

    திரும்பு s;
  }
  வெறுமை destroy_buffer_glyph_set (hb_bit_set_t *s) மாறிலி
  {
    எனில் (unlikely (!s))
      திரும்பு;
    எனில் (buffer_glyph_set.cmpexch (nullptr, s))
      திரும்பு;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

எண்ணிக்கை { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

கட்டமைப்பு hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, தருக்க, HB_DEBUG_APPLY>
{
  மாறிலி எழுத்து *get_name () { திரும்பு "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (மாறிலி T &obj, Ts&&... ds)
  { திரும்பு obj.apply (இது, std::forward<Ts> (ds)...); }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு பொய்; }
  தருக்க stop_sublookup_iteration (return_t r) மாறிலி { திரும்பு r; }

  மாறிலி hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  மாறிலி ankr *ankr_table;
  மாறிலி OT::GDEF &gdef;
  தருக்க has_glyph_classes;
  மாறிலி hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  தருக்க buffer_is_reversed = பொய்;
  // Caches
  தருக்க using_buffer_glyph_set = பொய்;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  மாறிலி hb_bit_set_t *first_set = nullptr;
  மாறிலி hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned முழு lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (மாறிலி hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL வெறுமை set_ankr_table (மாறிலி AAT::ankr *ankr_table_);

  வெறுமை set_lookup_index (unsigned முழு i) { lookup_index = i; }

  வெறுமை reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  வெறுமை setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    எனில் (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  தருக்க buffer_intersects_machine () மாறிலி
  {
    எனில் (likely (using_buffer_glyph_set))
      திரும்பு buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    க்கு (unsigned i = 0; i < buffer->len; i++)
      எனில் (first_set->has (buffer->info[i].codepoint))
	திரும்பு உண்மை;
    திரும்பு பொய்;
  }

  template <typename T>
  HB_NODISCARD தருக்க output_glyphs (unsigned முழு மொத்தம்,
				   மாறிலி T *glyphs)
  {
    எனில் (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, மொத்தம்);
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      தருக்க at_end_of_text = buffer->idx == buffer->len;
      எனில் (at_end_of_text)
	எனில் (unlikely (!buffer->output_glyph (glyphs[i]))) திரும்பு பொய்;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      எனில் (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      இல்லையெனில்
      {
#ifndef HB_NO_OT_LAYOUT
	எனில் (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      எனில் (!at_end_of_text)
	எனில் (unlikely (!buffer->output_glyph (glyphs[i]))) திரும்பு பொய்;
    }
    திரும்பு உண்மை;
  }

  HB_NODISCARD தருக்க replace_glyph (hb_codepoint_t glyph)
  {
    எனில் (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    எனில் (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    எனில் (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    திரும்பு buffer->replace_glyph (glyph);
  }

  HB_NODISCARD தருக்க delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    எனில் (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    திரும்பு buffer->replace_glyph (DELETED_GLYPH);
  }

  வெறுமை replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    எனில் (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    எனில் (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    எனில் (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> கட்டமைப்பு Lookup;

template <typename T>
கட்டமைப்பு LookupFormat0
{
  friend கட்டமைப்பு Lookup<T>;

  தனிப்பட்ட:
  மாறிலி T* get_மதிப்பு (hb_codepoint_t glyph_id, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    எனில் (unlikely (glyph_id >= இலக்கம்_glyphs)) திரும்பு nullptr;
    திரும்பு &arrayZ[glyph_id];
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs, unsigned இலக்கம்_glyphs) மாறிலி
  {
    glyphs.add_range (0, இலக்கம்_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, unsigned இலக்கம்_glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    க்கு (unsigned i = 0; i < இலக்கம்_glyphs; i++)
      எனில் (filter (arrayZ[i]))
	glyphs.add (i);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (arrayZ.sanitize (c, c->get_இலக்கம்_glyphs ()));
  }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (arrayZ.sanitize (c, c->get_இலக்கம்_glyphs (), base));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  பொது:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
கட்டமைப்பு LookupSegmentSingle
{
  நிலையான constexpr unsigned Termination_Word_மொத்தம் = 2u;

  முழு cmp (hb_codepoint_t g) மாறிலி
  { திரும்பு g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs) மாறிலி
  {
    எனில் (first == DELETED_GLYPH) திரும்பு;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    எனில் (first == DELETED_GLYPH) திரும்பு;
    எனில் (!filter (value)) திரும்பு;
    glyphs.add_range (first, last);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && value.sanitize (c));
  }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  பொது:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
கட்டமைப்பு LookupFormat2
{
  friend கட்டமைப்பு Lookup<T>;

  தனிப்பட்ட:
  மாறிலி T* get_மதிப்பு (hb_codepoint_t glyph_id) மாறிலி
  {
    மாறிலி LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    திரும்பு v ? &v->value : nullptr;
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs) மாறிலி
  {
    unsigned மொத்தம் = segments.get_length ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    unsigned மொத்தம் = segments.get_length ();
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (segments.sanitize (c));
  }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (segments.sanitize (c, base));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_தேடல்_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  பொது:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
கட்டமைப்பு LookupSegmentArray
{
  நிலையான constexpr unsigned Termination_Word_மொத்தம் = 2u;

  மாறிலி T* get_மதிப்பு (hb_codepoint_t glyph_id, மாறிலி வெறுமை *base) மாறிலி
  {
    திரும்பு first <= glyph_id && glyph_id <= last ? &(base+மதிப்புகள்_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs) மாறிலி
  {
    எனில் (first == DELETED_GLYPH) திரும்பு;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, மாறிலி வெறுமை *base, மாறிலி filter_t &filter) மாறிலி
  {
    எனில் (first == DELETED_GLYPH) திரும்பு;
    மாறிலி auto &values = base+மதிப்புகள்_Z;
    க்கு (hb_codepoint_t i = first; i <= last; i++)
      எனில் (filter (values[i - first]))
	glyphs.add (i);
  }

  முழு cmp (hb_codepoint_t g) மாறிலி
  { திரும்பு g < first ? -1 : g <= last ? 0 : +1; }

  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  first <= last &&
		  மதிப்புகள்_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base, Ts&&... ds) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  first <= last &&
		  மதிப்புகள்_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		மதிப்புகள்_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  பொது:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
கட்டமைப்பு LookupFormat4
{
  friend கட்டமைப்பு Lookup<T>;

  தனிப்பட்ட:
  மாறிலி T* get_மதிப்பு (hb_codepoint_t glyph_id) மாறிலி
  {
    மாறிலி LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    திரும்பு v ? v->get_மதிப்பு (glyph_id, இது) : nullptr;
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs) மாறிலி
  {
    unsigned மொத்தம் = segments.get_length ();
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    unsigned மொத்தம் = segments.get_length ();
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      segments[i].collect_glyphs_filtered (glyphs, இது, filter);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (segments.sanitize (c, இது));
  }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (segments.sanitize (c, இது, base));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_தேடல்_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  பொது:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
கட்டமைப்பு LookupSingle
{
  நிலையான constexpr unsigned Termination_Word_மொத்தம் = 1u;

  முழு cmp (hb_codepoint_t g) மாறிலி { திரும்பு glyph.cmp (g); }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs) மாறிலி
  {
    எனில் (glyph == DELETED_GLYPH) திரும்பு;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    எனில் (glyph == DELETED_GLYPH) திரும்பு;
    எனில் (!filter (value)) திரும்பு;
    glyphs.add (glyph);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && value.sanitize (c));
  }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  பொது:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
கட்டமைப்பு LookupFormat6
{
  friend கட்டமைப்பு Lookup<T>;

  தனிப்பட்ட:
  மாறிலி T* get_மதிப்பு (hb_codepoint_t glyph_id) மாறிலி
  {
    மாறிலி LookupSingle<T> *v = entries.bsearch (glyph_id);
    திரும்பு v ? &v->value : nullptr;
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs) மாறிலி
  {
    unsigned மொத்தம் = entries.get_length ();
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    unsigned மொத்தம் = entries.get_length ();
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (entries.sanitize (c));
  }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (entries.sanitize (c, base));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_தேடல்_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  பொது:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
கட்டமைப்பு LookupFormat8
{
  friend கட்டமைப்பு Lookup<T>;

  தனிப்பட்ட:
  மாறிலி T* get_மதிப்பு (hb_codepoint_t glyph_id) மாறிலி
  {
    திரும்பு firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_மொத்தம்_2 ?
	   &மதிப்பு_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs) மாறிலி
  {
    எனில் (unlikely (!glyph_மொத்தம்_2)) திரும்பு;
    எனில் (firstGlyph == DELETED_GLYPH) திரும்பு;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_மொத்தம்_2 - 1);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    எனில் (unlikely (!glyph_மொத்தம்_2)) திரும்பு;
    எனில் (firstGlyph == DELETED_GLYPH) திரும்பு;
    மாறிலி T *p = மதிப்பு_Array_Z.arrayZ;
    க்கு (unsigned i = 0; i < glyph_மொத்தம்_2; i++)
      எனில் (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && மதிப்பு_Array_Z.sanitize (c, glyph_மொத்தம்_2));
  }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && மதிப்பு_Array_Z.sanitize (c, glyph_மொத்தம்_2, base));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_மொத்தம்_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		மதிப்பு_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  பொது:
  DEFINE_SIZE_ARRAY (6, மதிப்பு_Array_Z);
};

template <typename T>
கட்டமைப்பு LookupFormat10
{
  friend கட்டமைப்பு Lookup<T>;

  தனிப்பட்ட:
  மாறிலி typename T::type get_மதிப்பு_or_null (hb_codepoint_t glyph_id) மாறிலி
  {
    எனில் (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_மொத்தம்_2))
      திரும்பு Null (T);

    மாறிலி HBUINT8 *p = &மதிப்பு_Array_Z[(glyph_id - firstGlyph) * மதிப்பு_Size];

    unsigned முழு v = 0;
    unsigned முழு மொத்தம் = மதிப்பு_Size;
    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
      v = (v << 8) | *p++;

    திரும்பு v;
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs) மாறிலி
  {
    எனில் (unlikely (!glyph_மொத்தம்_2)) திரும்பு;
    எனில் (firstGlyph == DELETED_GLYPH) திரும்பு;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_மொத்தம்_2 - 1);
  }

  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    எனில் (unlikely (!glyph_மொத்தம்_2)) திரும்பு;
    எனில் (firstGlyph == DELETED_GLYPH) திரும்பு;
    மாறிலி HBUINT8 *p = மதிப்பு_Array_Z.arrayZ;
    க்கு (unsigned i = 0; i < glyph_மொத்தம்_2; i++)
    {
      unsigned முழு v = 0;
      unsigned முழு மொத்தம் = மதிப்பு_Size;
      க்கு (unsigned முழு j = 0; j < மொத்தம்; j++)
	v = (v << 8) | *p++;
      எனில் (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  மதிப்பு_Size <= 4 &&
		  மதிப்பு_Array_Z.sanitize (c, glyph_மொத்தம்_2 * மதிப்பு_Size));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	மதிப்பு_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_மொத்தம்_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		மதிப்பு_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  பொது:
  DEFINE_SIZE_ARRAY (8, மதிப்பு_Array_Z);
};

template <typename T>
கட்டமைப்பு Lookup
{
  மாறிலி T* get_மதிப்பு (hb_codepoint_t glyph_id, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); திரும்பு u.format0.get_மதிப்பு (glyph_id, இலக்கம்_glyphs);
    நிலை 2: hb_barrier (); திரும்பு u.format2.get_மதிப்பு (glyph_id);
    நிலை 4: hb_barrier (); திரும்பு u.format4.get_மதிப்பு (glyph_id);
    நிலை 6: hb_barrier (); திரும்பு u.format6.get_மதிப்பு (glyph_id);
    நிலை 8: hb_barrier (); திரும்பு u.format8.get_மதிப்பு (glyph_id);
    இயல்பு:திரும்பு nullptr;
    }
  }

  மாறிலி typename T::type get_மதிப்பு_or_null (hb_codepoint_t glyph_id, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (u.format.v) {
      /* Format 10 cannot return a pointer. */
      நிலை 10: hb_barrier (); திரும்பு u.format10.get_மதிப்பு_or_null (glyph_id);
      இயல்பு:
      மாறிலி T *v = get_மதிப்பு (glyph_id, இலக்கம்_glyphs);
      திரும்பு v ? *v : Null (T);
    }
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs, unsigned முழு இலக்கம்_glyphs) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); u.format0.collect_glyphs (glyphs, இலக்கம்_glyphs); திரும்பு;
    நிலை 2: hb_barrier (); u.format2.collect_glyphs (glyphs); திரும்பு;
    நிலை 4: hb_barrier (); u.format4.collect_glyphs (glyphs); திரும்பு;
    நிலை 6: hb_barrier (); u.format6.collect_glyphs (glyphs); திரும்பு;
    நிலை 8: hb_barrier (); u.format8.collect_glyphs (glyphs); திரும்பு;
    நிலை 10: hb_barrier (); u.format10.collect_glyphs (glyphs); திரும்பு;
    இயல்பு:திரும்பு;
    }
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, unsigned இலக்கம்_glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, இலக்கம்_glyphs, filter); திரும்பு;
    நிலை 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); திரும்பு;
    நிலை 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); திரும்பு;
    நிலை 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); திரும்பு;
    நிலை 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); திரும்பு;
    நிலை 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); திரும்பு;
    இயல்பு:திரும்பு;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned முழு இலக்கம்_glyphs,
			      unsigned முழு outOfRange) மாறிலி
  {
    மாறிலி T *v = get_மதிப்பு (glyph_id, இலக்கம்_glyphs);
    திரும்பு v ? *v : outOfRange;
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    நிலை 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    நிலை 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    நிலை 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    நிலை 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    இயல்பு:return_trace (உண்மை);
    }
  }
  தருக்க sanitize (hb_sanitize_context_t *c, மாறிலி வெறுமை *base) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (!u.format.v.sanitize (c)) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.format.v) {
    நிலை 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    நிலை 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    நிலை 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    நிலை 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    நிலை 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    நிலை 10: return_trace (பொய்); /* We don't support format10 here currently. */
    இயல்பு:return_trace (உண்மை);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  பொது:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
கட்டமைப்பு Entry
{
  // This doesn't seem like it's ever called.
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (இது));
  }

  பொது:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  பொது:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
கட்டமைப்பு Entry<வெறுமை>
{
  // This does seem like it's ever called.
  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  பொது:
  DEFINE_SIZE_STATIC (4);
};

எண்ணிக்கை Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_வரி = 3,
};

template <typename Types, typename Extra>
கட்டமைப்பு StateTable
{
  வகைவரை typename Types::HBUINT HBUINT;
  வகைவரை typename Types::HBUSHORT HBUSHORT;
  வகைவரை typename Types::ClassTypeNarrow ClassType;

  எண்ணிக்கை State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_வரி = 1,
  };

  template <typename set_t, typename table_t>
  வெறுமை collect_initial_glyphs (set_t &glyphs, unsigned இலக்கம்_glyphs, மாறிலி table_t &table) மாறிலி
  {
    unsigned இலக்கம்_classes = nClasses;

    எனில் (unlikely (இலக்கம்_classes > hb_bit_page_t::BITS))
    {
      (இது+classTable).collect_glyphs (glyphs, இலக்கம்_glyphs);
      திரும்பு;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    க்கு (unsigned i = 0; i < இலக்கம்_classes; i++)
    {
      மாறிலி auto &entry = get_entry (STATE_START_OF_TEXT, i);
      எனில் (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	தொடர்;

      filter.add (i);
    }

    // And glyphs in those classes.

    எனில் (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (இது+classTable).collect_glyphs_filtered (glyphs, இலக்கம்_glyphs, filter);
  }

  முழு new_state (unsigned முழு newState) மாறிலி
  { திரும்பு Types::extended ? newState : ((முழு) newState - (முழு) stateArrayTable) / (முழு) nClasses; }

  unsigned முழு get_class (hb_codepoint_t glyph_id,
			  unsigned முழு இலக்கம்_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) மாறிலி
  {
    unsigned klass;
    எனில் (cache && cache->get (glyph_id, &klass)) திரும்பு klass;
    எனில் (unlikely (glyph_id == DELETED_GLYPH)) திரும்பு CLASS_DELETED_GLYPH;
    klass = (இது+classTable).get_class (glyph_id, இலக்கம்_glyphs, CLASS_OUT_OF_BOUNDS);
    எனில் (cache) cache->set (glyph_id, klass);
    திரும்பு klass;
  }

  மாறிலி Entry<Extra> *get_entries () மாறிலி
  { திரும்பு (இது+entryTable).arrayZ; }

  மாறிலி Entry<Extra> &get_entry (முழு state, unsigned முழு klass) மாறிலி
  {
    unsigned n_classes = nClasses;
    எனில் (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    மாறிலி HBUSHORT *states = (இது+stateArrayTable).arrayZ;
    மாறிலி Entry<Extra> *entries = (இது+entryTable).arrayZ;

    unsigned முழு entry = states[state * n_classes + klass];
    DEBUG_செய்தி (APPLY, nullptr, "e%u", entry);

    திரும்பு entries[entry];
  }

  தருக்க sanitize (hb_sanitize_context_t *c,
		 unsigned முழு *இலக்கம்_entries_out = nullptr) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!(c->check_struct (இது) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, இது)))) return_trace (பொய்);

    மாறிலி HBUSHORT *states = (இது+stateArrayTable).arrayZ;
    மாறிலி Entry<Extra> *entries = (இது+entryTable).arrayZ;

    unsigned முழு இலக்கம்_classes = nClasses;
    எனில் (unlikely (hb_unsigned_mul_overflows (இலக்கம்_classes, states[0].static_size)))
      return_trace (பொய்);
    unsigned முழு row_stride = இலக்கம்_classes * states[0].static_size;

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

    முழு min_state = 0;
    முழு max_state = 0;
    unsigned முழு இலக்கம்_entries = 0;

    முழு state_pos = 0;
    முழு state_neg = 0;
    unsigned முழு entry = 0;
    போது (min_state < state_neg || state_pos <= max_state)
    {
      எனில் (min_state < state_neg)
      {
	/* Negative states. */
	எனில் (unlikely (hb_unsigned_mul_overflows (min_state, இலக்கம்_classes)))
	  return_trace (பொய்);
	எனில் (unlikely (!c->check_range (&states[min_state * இலக்கம்_classes],
				       -min_state,
				       row_stride)))
	  return_trace (பொய்);
	எனில் ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (பொய்);
	{ /* Sweep new states. */
	  மாறிலி HBUSHORT *stop = &states[min_state * இலக்கம்_classes];
	  எனில் (unlikely (stop > states))
	    return_trace (பொய்);
	  க்கு (மாறிலி HBUSHORT *p = states; stop < p; p--)
	    இலக்கம்_entries = hb_max (இலக்கம்_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      எனில் (state_pos <= max_state)
      {
	/* Positive states. */
	எனில் (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (பொய்);
	எனில் ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (பொய்);
	{ /* Sweep new states. */
	  எனில் (unlikely (hb_unsigned_mul_overflows ((max_state + 1), இலக்கம்_classes)))
	    return_trace (பொய்);
	  மாறிலி HBUSHORT *stop = &states[(max_state + 1) * இலக்கம்_classes];
	  எனில் (unlikely (stop < states))
	    return_trace (பொய்);
	  க்கு (மாறிலி HBUSHORT *p = &states[state_pos * இலக்கம்_classes]; p < stop; p++)
	    இலக்கம்_entries = hb_max (இலக்கம்_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      எனில் (unlikely (!c->check_array (entries, இலக்கம்_entries)))
	return_trace (பொய்);
      எனில் ((c->max_ops -= இலக்கம்_entries - entry) <= 0)
	return_trace (பொய்);
      { /* Sweep new entries. */
	மாறிலி Entry<Extra> *stop = &entries[இலக்கம்_entries];
	க்கு (மாறிலி Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  முழு newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = இலக்கம்_entries;
      }
    }

    எனில் (இலக்கம்_entries_out)
      *இலக்கம்_entries_out = இலக்கம்_entries;

    return_trace (உண்மை);
  }

  பாதுகாக்கப்பட்ட:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  பொது:
  DEFINE_SIZE_STATIC (4 * அளவு (HBUINT));
};

template <typename HBUCHAR>
கட்டமைப்பு ClassTable
{
  unsigned முழு get_class (hb_codepoint_t glyph_id, unsigned முழு outOfRange) மாறிலி
  {
    unsigned முழு i = glyph_id - firstGlyph;
    திரும்பு i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned முழு get_class (hb_codepoint_t glyph_id,
			  unsigned முழு இலக்கம்_glyphs HB_UNUSED,
			  unsigned முழு outOfRange) மாறிலி
  {
    திரும்பு get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  வெறுமை collect_glyphs (set_t &glyphs, unsigned இலக்கம்_glyphs) மாறிலி
  {
    க்கு (unsigned i = 0; i < classArray.len; i++)
      எனில் (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  வெறுமை collect_glyphs_filtered (set_t &glyphs, unsigned இலக்கம்_glyphs, மாறிலி filter_t &filter) மாறிலி
  {
    க்கு (unsigned i = 0; i < classArray.len; i++)
      எனில் (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) && classArray.sanitize (c));
  }
  பாதுகாக்கப்பட்ட:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  பொது:
  DEFINE_SIZE_ARRAY (4, classArray);
};

கட்டமைப்பு SubtableGlyphCoverage
{
  தருக்க sanitize (hb_sanitize_context_t *c, unsigned subtable_மொத்தம்) மாறிலி
  {
    TRACE_SANITIZE (இது);

    எனில் (unlikely (!c->check_array (&subtableOffsets, subtable_மொத்தம்)))
      return_trace (பொய்);

    unsigned bytes = (c->get_இலக்கம்_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    க்கு (unsigned i = 0; i < subtable_மொத்தம்; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      எனில் (offset == 0 || offset == 0xFFFFFFFF)
        தொடர்;
      எனில் (unlikely (!subtableOffsets[i].sanitize (c, இது, bytes)))
        return_trace (பொய்);
    }

    return_trace (உண்மை);
  }
  பாதுகாக்கப்பட்ட:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  பொது:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

கட்டமைப்பு ObsoleteTypes
{
  நிலையான constexpr தருக்க extended = பொய்;
  வகைவரை HBUINT16 HBUINT;
  வகைவரை HBUINT8 HBUSHORT;
  வகைவரை ClassTable<HBUINT8> ClassTypeNarrow;
  வகைவரை ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  நிலையான முழு offsetToIndex (int64_t offset,
			    மாறிலி வெறுமை *base,
			    மாறிலி T *array)
  {
    int64_t array_offset = (மாறிலி எழுத்து *) array - (மாறிலி எழுத்து *) base;
    முழு bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    எனில் (unlikely (offset < array_offset))
      திரும்பு bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    எனில் (unlikely (index > bad_index))
      திரும்பு bad_index;
    திரும்பு index;
  }
  template <typename T>
  நிலையான முழு byteOffsetToIndex (int64_t offset,
				மாறிலி வெறுமை *base,
				மாறிலி T *array)
  {
    திரும்பு offsetToIndex (offset, base, array);
  }
  template <typename T>
  நிலையான முழு wordOffsetToIndex (int64_t offset,
				மாறிலி வெறுமை *base,
				மாறிலி T *array)
  {
    திரும்பு offsetToIndex (2 * offset, base, array);
  }
};
கட்டமைப்பு ExtendedTypes
{
  நிலையான constexpr தருக்க extended = உண்மை;
  வகைவரை HBUINT32 HBUINT;
  வகைவரை HBUINT16 HBUSHORT;
  வகைவரை Lookup<HBUINT16> ClassTypeNarrow;
  வகைவரை Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  நிலையான unsigned முழு offsetToIndex (unsigned முழு offset,
				     மாறிலி வெறுமை *base HB_UNUSED,
				     மாறிலி T *array HB_UNUSED)
  {
    திரும்பு offset;
  }
  template <typename T>
  நிலையான unsigned முழு byteOffsetToIndex (unsigned முழு offset,
					 மாறிலி வெறுமை *base HB_UNUSED,
					 மாறிலி T *array HB_UNUSED)
  {
    திரும்பு offset / 2;
  }
  template <typename T>
  நிலையான unsigned முழு wordOffsetToIndex (unsigned முழு offset,
					 மாறிலி வெறுமை *base HB_UNUSED,
					 மாறிலி T *array HB_UNUSED)
  {
    திரும்பு offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
கட்டமைப்பு StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (மாறிலி StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      இலக்கம்_glyphs (face_->get_இலக்கம்_glyphs ()) {}

  template <typename context_t>
  வெறுமை drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    எனில் (!c->in_place)
      buffer->clear_output ();

    முழு state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    மாறிலி தருக்க start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    க்கு (buffer->idx = 0; buffer->successful;)
    {
      unsigned முழு klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, இலக்கம்_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_செய்தி (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      மாறிலி EntryT &entry = machine.get_entry (state, klass);
      மாறிலி முழு next_state = machine.new_state (entry.newState);

      தருக்க is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      தருக்க is_not_actionable = !c->table->is_actionable (entry);

      எனில் (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	எனில் (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  போது (cluster < range->cluster_first)
	    range--;
	  போது (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	எனில் (!(range->flags & ac->subtable_flags))
	{
	  எனில் (buffer->idx == buffer->len)
	    நிறுத்து;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (வெறுமை) buffer->next_glyph ();
	  தொடர்;
	}
      }
      இல்லையெனில்
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	தருக்க is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	எனில் (is_null_transition)
	{
	  unsigned old_klass = klass;
	  செய்
	  {
	    c->transition (buffer, இது, entry);

	    எனில் (buffer->idx == buffer->len || !buffer->successful)
	      நிறுத்து;

	    (வெறுமை) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, இலக்கம்_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } போது (klass == old_klass);

	  எனில் (buffer->idx == buffer->len || !buffer->successful)
	    நிறுத்து;

	  செல் resume;
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
      மாறிலி EntryT *wouldbe_entry;
      தருக்க is_safe_to_break =
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

      எனில் (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, இது, entry);

      state = next_state;
      DEBUG_செய்தி (APPLY, nullptr, "s%d", state);

      எனில் (buffer->idx == buffer->len)
	நிறுத்து;

      எனில் (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(வெறுமை) buffer->next_glyph ();
    }

    எனில் (!c->in_place)
      buffer->ஒத்திசை ();
  }

  பொது:
  மாறிலி StateTableT &machine;
  unsigned முழு இலக்கம்_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
