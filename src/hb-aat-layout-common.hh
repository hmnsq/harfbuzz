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

#ଅନ୍ତର୍ଭାଵ "hb-aat-layout.hh"
#ଅନ୍ତର୍ଭାଵ "hb-aat-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-layout-gdef-table.hh"
#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cache.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bit-set.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bit-page.hh"


ନାମାକାଶ OT {
ସଂରଚନା GDEF;
};

ନାମାକାଶ AAT {

using ନାମାକାଶ OT;

ସଂରଚନା ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#ଅନ୍ୟଥା
#define HB_AAT_TRANSITION_INLINE
#endif

ସଂରଚନା hb_aat_scratch_t
{
  hb_aat_scratch_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_aat_scratch_t (ଅଚଳ hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    ଫେରନ୍ତୁ *ଏହି;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    ଯଦି (unlikely (!s))
      ଫେରନ୍ତୁ;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () ଅଚଳ
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    ଯଦି (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      ଫେରନ୍ତୁ s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, ଆକାର (hb_bit_set_t));
    ଯଦି (unlikely (!s))
      ଫେରନ୍ତୁ nullptr;
    s->init ();

    ଫେରନ୍ତୁ s;
  }
  ଶୂନ୍ୟ destroy_buffer_glyph_set (hb_bit_set_t *s) ଅଚଳ
  {
    ଯଦି (unlikely (!s))
      ଫେରନ୍ତୁ;
    ଯଦି (buffer_glyph_set.cmpexch (nullptr, s))
      ଫେରନ୍ତୁ;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

ଗଣନା { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

ସଂରଚନା hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, ତାର୍କିକ, HB_DEBUG_APPLY>
{
  ଅଚଳ ଅକ୍ଷର *get_name () { ଫେରନ୍ତୁ "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (ଅଚଳ T &obj, Ts&&... ds)
  { ଫେରନ୍ତୁ obj.apply (ଏହି, std::forward<Ts> (ds)...); }
  ସ୍ଥିର return_t default_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  ତାର୍କିକ stop_sublookup_iteration (return_t r) ଅଚଳ { ଫେରନ୍ତୁ r; }

  ଅଚଳ hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  ଅଚଳ ankr *ankr_table;
  ଅଚଳ OT::GDEF &gdef;
  ତାର୍କିକ has_glyph_classes;
  ଅଚଳ hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  ତାର୍କିକ buffer_is_reversed = ମିଥ୍ୟା;
  // Caches
  ତାର୍କିକ using_buffer_glyph_set = ମିଥ୍ୟା;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  ଅଚଳ hb_bit_set_t *first_set = nullptr;
  ଅଚଳ hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (ଅଚଳ hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL ଶୂନ୍ୟ set_ankr_table (ଅଚଳ AAT::ankr *ankr_table_);

  ଶୂନ୍ୟ set_lookup_index (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) { lookup_index = i; }

  ଶୂନ୍ୟ reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  ଶୂନ୍ୟ setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    ଯଦି (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  ତାର୍କିକ buffer_intersects_machine () ଅଚଳ
  {
    ଯଦି (likely (using_buffer_glyph_set))
      ଫେରନ୍ତୁ buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    ପାଇଁ (unsigned i = 0; i < buffer->len; i++)
      ଯଦି (first_set->has (buffer->info[i].codepoint))
	ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  template <typename T>
  HB_NODISCARD ତାର୍କିକ output_glyphs (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
				   ଅଚଳ T *glyphs)
  {
    ଯଦି (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, ଗଣନା_2);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      ତାର୍କିକ at_end_of_text = buffer->idx == buffer->len;
      ଯଦି (at_end_of_text)
	ଯଦି (unlikely (!buffer->output_glyph (glyphs[i]))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      ଯଦି (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      ଅନ୍ୟଥା
      {
#ifndef HB_NO_OT_LAYOUT
	ଯଦି (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      ଯଦି (!at_end_of_text)
	ଯଦି (unlikely (!buffer->output_glyph (glyphs[i]))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  HB_NODISCARD ତାର୍କିକ replace_glyph (hb_codepoint_t glyph)
  {
    ଯଦି (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    ଯଦି (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    ଯଦି (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    ଫେରନ୍ତୁ buffer->replace_glyph (glyph);
  }

  HB_NODISCARD ତାର୍କିକ delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    ଯଦି (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    ଫେରନ୍ତୁ buffer->replace_glyph (DELETED_GLYPH);
  }

  ଶୂନ୍ୟ replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    ଯଦି (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    ଯଦି (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    ଯଦି (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> ସଂରଚନା Lookup;

template <typename T>
ସଂରଚନା LookupFormat0
{
  friend ସଂରଚନା Lookup<T>;

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ T* get_ମୂଲ୍ୟ (hb_codepoint_t glyph_id, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ଯଦି (unlikely (glyph_id >= ଅଙ୍କ_glyphs)) ଫେରନ୍ତୁ nullptr;
    ଫେରନ୍ତୁ &arrayZ[glyph_id];
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    glyphs.add_range (0, ଅଙ୍କ_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, unsigned ଅଙ୍କ_glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_glyphs; i++)
      ଯଦି (filter (arrayZ[i]))
	glyphs.add (i);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (arrayZ.sanitize (c, c->get_ଅଙ୍କ_glyphs ()));
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (arrayZ.sanitize (c, c->get_ଅଙ୍କ_glyphs (), base));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
ସଂରଚନା LookupSegmentSingle
{
  ସ୍ଥିର constexpr unsigned Termination_Word_ଗଣନା = 2u;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs) ଅଚଳ
  {
    ଯଦି (first == DELETED_GLYPH) ଫେରନ୍ତୁ;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    ଯଦି (first == DELETED_GLYPH) ଫେରନ୍ତୁ;
    ଯଦି (!filter (value)) ଫେରନ୍ତୁ;
    glyphs.add_range (first, last);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && value.sanitize (c));
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
ସଂରଚନା LookupFormat2
{
  friend ସଂରଚନା Lookup<T>;

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ T* get_ମୂଲ୍ୟ (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ଅଚଳ LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    ଫେରନ୍ତୁ v ? &v->value : nullptr;
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs) ଅଚଳ
  {
    unsigned ଗଣନା_2 = segments.get_length ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    unsigned ଗଣନା_2 = segments.get_length ();
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (segments.sanitize (c));
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (segments.sanitize (c, base));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_ସନ୍ଧାନ_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
ସଂରଚନା LookupSegmentArray
{
  ସ୍ଥିର constexpr unsigned Termination_Word_ଗଣନା = 2u;

  ଅଚଳ T* get_ମୂଲ୍ୟ (hb_codepoint_t glyph_id, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    ଫେରନ୍ତୁ first <= glyph_id && glyph_id <= last ? &(base+ମୂଲ୍ୟଗୁଡ଼ିକ_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs) ଅଚଳ
  {
    ଯଦି (first == DELETED_GLYPH) ଫେରନ୍ତୁ;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, ଅଚଳ ଶୂନ୍ୟ *base, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    ଯଦି (first == DELETED_GLYPH) ଫେରନ୍ତୁ;
    ଅଚଳ auto &values = base+ମୂଲ୍ୟଗୁଡ଼ିକ_Z;
    ପାଇଁ (hb_codepoint_t i = first; i <= last; i++)
      ଯଦି (filter (values[i - first]))
	glyphs.add (i);
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ
  { ଫେରନ୍ତୁ g < first ? -1 : g <= last ? 0 : +1; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  first <= last &&
		  ମୂଲ୍ୟଗୁଡ଼ିକ_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base, Ts&&... ds) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  first <= last &&
		  ମୂଲ୍ୟଗୁଡ଼ିକ_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		ମୂଲ୍ୟଗୁଡ଼ିକ_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
ସଂରଚନା LookupFormat4
{
  friend ସଂରଚନା Lookup<T>;

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ T* get_ମୂଲ୍ୟ (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ଅଚଳ LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    ଫେରନ୍ତୁ v ? v->get_ମୂଲ୍ୟ (glyph_id, ଏହି) : nullptr;
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs) ଅଚଳ
  {
    unsigned ଗଣନା_2 = segments.get_length ();
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    unsigned ଗଣନା_2 = segments.get_length ();
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      segments[i].collect_glyphs_filtered (glyphs, ଏହି, filter);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (segments.sanitize (c, ଏହି));
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (segments.sanitize (c, ଏହି, base));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_ସନ୍ଧାନ_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
ସଂରଚନା LookupSingle
{
  ସ୍ଥିର constexpr unsigned Termination_Word_ଗଣନା = 1u;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ glyph.cmp (g); }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs) ଅଚଳ
  {
    ଯଦି (glyph == DELETED_GLYPH) ଫେରନ୍ତୁ;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    ଯଦି (glyph == DELETED_GLYPH) ଫେରନ୍ତୁ;
    ଯଦି (!filter (value)) ଫେରନ୍ତୁ;
    glyphs.add (glyph);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && value.sanitize (c));
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
ସଂରଚନା LookupFormat6
{
  friend ସଂରଚନା Lookup<T>;

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ T* get_ମୂଲ୍ୟ (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ଅଚଳ LookupSingle<T> *v = entries.bsearch (glyph_id);
    ଫେରନ୍ତୁ v ? &v->value : nullptr;
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs) ଅଚଳ
  {
    unsigned ଗଣନା_2 = entries.get_length ();
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    unsigned ଗଣନା_2 = entries.get_length ();
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (entries.sanitize (c));
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (entries.sanitize (c, base));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_ସନ୍ଧାନ_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
ସଂରଚନା LookupFormat8
{
  friend ସଂରଚନା Lookup<T>;

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ T* get_ମୂଲ୍ୟ (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ଫେରନ୍ତୁ firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_ଗଣନା_2 ?
	   &ମୂଲ୍ୟ_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs) ଅଚଳ
  {
    ଯଦି (unlikely (!glyph_ଗଣନା_2)) ଫେରନ୍ତୁ;
    ଯଦି (firstGlyph == DELETED_GLYPH) ଫେରନ୍ତୁ;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_ଗଣନା_2 - 1);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    ଯଦି (unlikely (!glyph_ଗଣନା_2)) ଫେରନ୍ତୁ;
    ଯଦି (firstGlyph == DELETED_GLYPH) ଫେରନ୍ତୁ;
    ଅଚଳ T *p = ମୂଲ୍ୟ_Array_Z.arrayZ;
    ପାଇଁ (unsigned i = 0; i < glyph_ଗଣନା_2; i++)
      ଯଦି (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && ମୂଲ୍ୟ_Array_Z.sanitize (c, glyph_ଗଣନା_2));
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && ମୂଲ୍ୟ_Array_Z.sanitize (c, glyph_ଗଣନା_2, base));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ଗଣନା_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		ମୂଲ୍ୟ_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (6, ମୂଲ୍ୟ_Array_Z);
};

template <typename T>
ସଂରଚନା LookupFormat10
{
  friend ସଂରଚନା Lookup<T>;

  ବ୍ୟକ୍ତିଗତ:
  ଅଚଳ typename T::type get_ମୂଲ୍ୟ_or_null (hb_codepoint_t glyph_id) ଅଚଳ
  {
    ଯଦି (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_ଗଣନା_2))
      ଫେରନ୍ତୁ Null (T);

    ଅଚଳ HBUINT8 *p = &ମୂଲ୍ୟ_Array_Z[(glyph_id - firstGlyph) * ମୂଲ୍ୟ_Size];

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = ମୂଲ୍ୟ_Size;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      v = (v << 8) | *p++;

    ଫେରନ୍ତୁ v;
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs) ଅଚଳ
  {
    ଯଦି (unlikely (!glyph_ଗଣନା_2)) ଫେରନ୍ତୁ;
    ଯଦି (firstGlyph == DELETED_GLYPH) ଫେରନ୍ତୁ;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_ଗଣନା_2 - 1);
  }

  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    ଯଦି (unlikely (!glyph_ଗଣନା_2)) ଫେରନ୍ତୁ;
    ଯଦି (firstGlyph == DELETED_GLYPH) ଫେରନ୍ତୁ;
    ଅଚଳ HBUINT8 *p = ମୂଲ୍ୟ_Array_Z.arrayZ;
    ପାଇଁ (unsigned i = 0; i < glyph_ଗଣନା_2; i++)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = 0;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = ମୂଲ୍ୟ_Size;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < ଗଣନା_2; j++)
	v = (v << 8) | *p++;
      ଯଦି (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  ମୂଲ୍ୟ_Size <= 4 &&
		  ମୂଲ୍ୟ_Array_Z.sanitize (c, glyph_ଗଣନା_2 * ମୂଲ୍ୟ_Size));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	ମୂଲ୍ୟ_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ଗଣନା_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		ମୂଲ୍ୟ_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, ମୂଲ୍ୟ_Array_Z);
};

template <typename T>
ସଂରଚନା Lookup
{
  ଅଚଳ T* get_ମୂଲ୍ୟ (hb_codepoint_t glyph_id, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); ଫେରନ୍ତୁ u.format0.get_ମୂଲ୍ୟ (glyph_id, ଅଙ୍କ_glyphs);
    ସ୍ଥିତି 2: hb_barrier (); ଫେରନ୍ତୁ u.format2.get_ମୂଲ୍ୟ (glyph_id);
    ସ୍ଥିତି 4: hb_barrier (); ଫେରନ୍ତୁ u.format4.get_ମୂଲ୍ୟ (glyph_id);
    ସ୍ଥିତି 6: hb_barrier (); ଫେରନ୍ତୁ u.format6.get_ମୂଲ୍ୟ (glyph_id);
    ସ୍ଥିତି 8: hb_barrier (); ଫେରନ୍ତୁ u.format8.get_ମୂଲ୍ୟ (glyph_id);
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ nullptr;
    }
  }

  ଅଚଳ typename T::type get_ମୂଲ୍ୟ_or_null (hb_codepoint_t glyph_id, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v) {
      /* Format 10 cannot return a pointer. */
      ସ୍ଥିତି 10: hb_barrier (); ଫେରନ୍ତୁ u.format10.get_ମୂଲ୍ୟ_or_null (glyph_id);
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଅଚଳ T *v = get_ମୂଲ୍ୟ (glyph_id, ଅଙ୍କ_glyphs);
      ଫେରନ୍ତୁ v ? *v : Null (T);
    }
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); u.format0.collect_glyphs (glyphs, ଅଙ୍କ_glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 2: hb_barrier (); u.format2.collect_glyphs (glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 4: hb_barrier (); u.format4.collect_glyphs (glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 6: hb_barrier (); u.format6.collect_glyphs (glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 8: hb_barrier (); u.format8.collect_glyphs (glyphs); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 10: hb_barrier (); u.format10.collect_glyphs (glyphs); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, unsigned ଅଙ୍କ_glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, ଅଙ୍କ_glyphs, filter); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); ଫେରନ୍ତୁ;
    ସ୍ଥିତି 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:ଫେରନ୍ତୁ;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outOfRange) ଅଚଳ
  {
    ଅଚଳ T *v = get_ମୂଲ୍ୟ (glyph_id, ଅଙ୍କ_glyphs);
    ଫେରନ୍ତୁ v ? *v : outOfRange;
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    ସ୍ଥିତି 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    ସ୍ଥିତି 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    ସ୍ଥିତି 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, ଅଚଳ ଶୂନ୍ୟ *base) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (!u.format.v.sanitize (c)) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.format.v) {
    ସ୍ଥିତି 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    ସ୍ଥିତି 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    ସ୍ଥିତି 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    ସ୍ଥିତି 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    ସ୍ଥିତି 10: return_trace (ମିଥ୍ୟା); /* We don't support format10 here currently. */
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:return_trace (ସତ୍ୟ);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
ସଂରଚନା Entry
{
  // This doesn't seem like it's ever called.
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
ସଂରଚନା Entry<ଶୂନ୍ୟ>
{
  // This does seem like it's ever called.
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4);
};

ଗଣନା Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_ଧାଡ଼ି = 3,
};

template <typename Types, typename Extra>
ସଂରଚନା StateTable
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUINT HBUINT;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::HBUSHORT HBUSHORT;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::ClassTypeNarrow ClassType;

  ଗଣନା State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_ଧାଡ଼ି = 1,
  };

  template <typename set_t, typename table_t>
  ଶୂନ୍ୟ collect_initial_glyphs (set_t &glyphs, unsigned ଅଙ୍କ_glyphs, ଅଚଳ table_t &table) ଅଚଳ
  {
    unsigned ଅଙ୍କ_classes = nClasses;

    ଯଦି (unlikely (ଅଙ୍କ_classes > hb_bit_page_t::BITS))
    {
      (ଏହି+classTable).collect_glyphs (glyphs, ଅଙ୍କ_glyphs);
      ଫେରନ୍ତୁ;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_classes; i++)
    {
      ଅଚଳ auto &entry = get_entry (STATE_START_OF_TEXT, i);
      ଯଦି (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	ଜାରି;

      filter.add (i);
    }

    // And glyphs in those classes.

    ଯଦି (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (ଏହି+classTable).collect_glyphs_filtered (glyphs, ଅଙ୍କ_glyphs, filter);
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_state (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା newState) ଅଚଳ
  { ଫେରନ୍ତୁ Types::extended ? newState : ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) newState - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) stateArrayTable) / (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) nClasses; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_class (hb_codepoint_t glyph_id,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) ଅଚଳ
  {
    unsigned klass;
    ଯଦି (cache && cache->get (glyph_id, &klass)) ଫେରନ୍ତୁ klass;
    ଯଦି (unlikely (glyph_id == DELETED_GLYPH)) ଫେରନ୍ତୁ CLASS_DELETED_GLYPH;
    klass = (ଏହି+classTable).get_class (glyph_id, ଅଙ୍କ_glyphs, CLASS_OUT_OF_BOUNDS);
    ଯଦି (cache) cache->set (glyph_id, klass);
    ଫେରନ୍ତୁ klass;
  }

  ଅଚଳ Entry<Extra> *get_entries () ଅଚଳ
  { ଫେରନ୍ତୁ (ଏହି+entryTable).arrayZ; }

  ଅଚଳ Entry<Extra> &get_entry (ପୂର୍ଣ୍ଣସଂଖ୍ୟା state, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass) ଅଚଳ
  {
    unsigned n_classes = nClasses;
    ଯଦି (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    ଅଚଳ HBUSHORT *states = (ଏହି+stateArrayTable).arrayZ;
    ଅଚଳ Entry<Extra> *entries = (ଏହି+entryTable).arrayZ;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry = states[state * n_classes + klass];
    DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "e%u", entry);

    ଫେରନ୍ତୁ entries[entry];
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c,
		 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ_entries_out = nullptr) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!(c->check_struct (ଏହି) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, ଏହି)))) return_trace (ମିଥ୍ୟା);

    ଅଚଳ HBUSHORT *states = (ଏହି+stateArrayTable).arrayZ;
    ଅଚଳ Entry<Extra> *entries = (ଏହି+entryTable).arrayZ;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_classes = nClasses;
    ଯଦି (unlikely (hb_unsigned_mul_overflows (ଅଙ୍କ_classes, states[0].static_size)))
      return_trace (ମିଥ୍ୟା);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା row_stride = ଅଙ୍କ_classes * states[0].static_size;

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

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_state = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_state = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_entries = 0;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା state_pos = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା state_neg = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry = 0;
    ଯେତେବେଳେ (min_state < state_neg || state_pos <= max_state)
    {
      ଯଦି (min_state < state_neg)
      {
	/* Negative states. */
	ଯଦି (unlikely (hb_unsigned_mul_overflows (min_state, ଅଙ୍କ_classes)))
	  return_trace (ମିଥ୍ୟା);
	ଯଦି (unlikely (!c->check_range (&states[min_state * ଅଙ୍କ_classes],
				       -min_state,
				       row_stride)))
	  return_trace (ମିଥ୍ୟା);
	ଯଦି ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (ମିଥ୍ୟା);
	{ /* Sweep new states. */
	  ଅଚଳ HBUSHORT *stop = &states[min_state * ଅଙ୍କ_classes];
	  ଯଦି (unlikely (stop > states))
	    return_trace (ମିଥ୍ୟା);
	  ପାଇଁ (ଅଚଳ HBUSHORT *p = states; stop < p; p--)
	    ଅଙ୍କ_entries = hb_max (ଅଙ୍କ_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      ଯଦି (state_pos <= max_state)
      {
	/* Positive states. */
	ଯଦି (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (ମିଥ୍ୟା);
	ଯଦି ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (ମିଥ୍ୟା);
	{ /* Sweep new states. */
	  ଯଦି (unlikely (hb_unsigned_mul_overflows ((max_state + 1), ଅଙ୍କ_classes)))
	    return_trace (ମିଥ୍ୟା);
	  ଅଚଳ HBUSHORT *stop = &states[(max_state + 1) * ଅଙ୍କ_classes];
	  ଯଦି (unlikely (stop < states))
	    return_trace (ମିଥ୍ୟା);
	  ପାଇଁ (ଅଚଳ HBUSHORT *p = &states[state_pos * ଅଙ୍କ_classes]; p < stop; p++)
	    ଅଙ୍କ_entries = hb_max (ଅଙ୍କ_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      ଯଦି (unlikely (!c->check_array (entries, ଅଙ୍କ_entries)))
	return_trace (ମିଥ୍ୟା);
      ଯଦି ((c->max_ops -= ଅଙ୍କ_entries - entry) <= 0)
	return_trace (ମିଥ୍ୟା);
      { /* Sweep new entries. */
	ଅଚଳ Entry<Extra> *stop = &entries[ଅଙ୍କ_entries];
	ପାଇଁ (ଅଚଳ Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  ପୂର୍ଣ୍ଣସଂଖ୍ୟା newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = ଅଙ୍କ_entries;
      }
    }

    ଯଦି (ଅଙ୍କ_entries_out)
      *ଅଙ୍କ_entries_out = ଅଙ୍କ_entries;

    return_trace (ସତ୍ୟ);
  }

  ସୁରକ୍ଷିତ:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (4 * ଆକାର (HBUINT));
};

template <typename HBUCHAR>
ସଂରଚନା ClassTable
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_class (hb_codepoint_t glyph_id, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outOfRange) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = glyph_id - firstGlyph;
    ଫେରନ୍ତୁ i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_class (hb_codepoint_t glyph_id,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs HB_UNUSED,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା outOfRange) ଅଚଳ
  {
    ଫେରନ୍ତୁ get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  ଶୂନ୍ୟ collect_glyphs (set_t &glyphs, unsigned ଅଙ୍କ_glyphs) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < classArray.len; i++)
      ଯଦି (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  ଶୂନ୍ୟ collect_glyphs_filtered (set_t &glyphs, unsigned ଅଙ୍କ_glyphs, ଅଚଳ filter_t &filter) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < classArray.len; i++)
      ଯଦି (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) && classArray.sanitize (c));
  }
  ସୁରକ୍ଷିତ:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, classArray);
};

ସଂରଚନା SubtableGlyphCoverage
{
  ତାର୍କିକ sanitize (hb_sanitize_context_t *c, unsigned subtable_ଗଣନା) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    ଯଦି (unlikely (!c->check_array (&subtableOffsets, subtable_ଗଣନା)))
      return_trace (ମିଥ୍ୟା);

    unsigned bytes = (c->get_ଅଙ୍କ_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    ପାଇଁ (unsigned i = 0; i < subtable_ଗଣନା; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      ଯଦି (offset == 0 || offset == 0xFFFFFFFF)
        ଜାରି;
      ଯଦି (unlikely (!subtableOffsets[i].sanitize (c, ଏହି, bytes)))
        return_trace (ମିଥ୍ୟା);
    }

    return_trace (ସତ୍ୟ);
  }
  ସୁରକ୍ଷିତ:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

ସଂରଚନା ObsoleteTypes
{
  ସ୍ଥିର constexpr ତାର୍କିକ extended = ମିଥ୍ୟା;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା HBUINT16 HBUINT;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା HBUINT8 HBUSHORT;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ClassTable<HBUINT8> ClassTypeNarrow;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା offsetToIndex (int64_t offset,
			    ଅଚଳ ଶୂନ୍ୟ *base,
			    ଅଚଳ T *array)
  {
    int64_t array_offset = (ଅଚଳ ଅକ୍ଷର *) array - (ଅଚଳ ଅକ୍ଷର *) base;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    ଯଦି (unlikely (offset < array_offset))
      ଫେରନ୍ତୁ bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    ଯଦି (unlikely (index > bad_index))
      ଫେରନ୍ତୁ bad_index;
    ଫେରନ୍ତୁ index;
  }
  template <typename T>
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା byteOffsetToIndex (int64_t offset,
				ଅଚଳ ଶୂନ୍ୟ *base,
				ଅଚଳ T *array)
  {
    ଫେରନ୍ତୁ offsetToIndex (offset, base, array);
  }
  template <typename T>
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା wordOffsetToIndex (int64_t offset,
				ଅଚଳ ଶୂନ୍ୟ *base,
				ଅଚଳ T *array)
  {
    ଫେରନ୍ତୁ offsetToIndex (2 * offset, base, array);
  }
};
ସଂରଚନା ExtendedTypes
{
  ସ୍ଥିର constexpr ତାର୍କିକ extended = ସତ୍ୟ;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା HBUINT32 HBUINT;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା HBUINT16 HBUSHORT;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା Lookup<HBUINT16> ClassTypeNarrow;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offsetToIndex (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset,
				     ଅଚଳ ଶୂନ୍ୟ *base HB_UNUSED,
				     ଅଚଳ T *array HB_UNUSED)
  {
    ଫେରନ୍ତୁ offset;
  }
  template <typename T>
  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା byteOffsetToIndex (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset,
					 ଅଚଳ ଶୂନ୍ୟ *base HB_UNUSED,
					 ଅଚଳ T *array HB_UNUSED)
  {
    ଫେରନ୍ତୁ offset / 2;
  }
  template <typename T>
  ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା wordOffsetToIndex (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset,
					 ଅଚଳ ଶୂନ୍ୟ *base HB_UNUSED,
					 ଅଚଳ T *array HB_UNUSED)
  {
    ଫେରନ୍ତୁ offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
ସଂରଚନା StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (ଅଚଳ StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      ଅଙ୍କ_glyphs (face_->get_ଅଙ୍କ_glyphs ()) {}

  template <typename context_t>
  ଶୂନ୍ୟ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    ଯଦି (!c->in_place)
      buffer->clear_output ();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    ଅଚଳ ତାର୍କିକ start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    ପାଇଁ (buffer->idx = 0; buffer->successful;)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, ଅଙ୍କ_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      ଅଚଳ EntryT &entry = machine.get_entry (state, klass);
      ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_state = machine.new_state (entry.newState);

      ତାର୍କିକ is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      ତାର୍କିକ is_not_actionable = !c->table->is_actionable (entry);

      ଯଦି (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	ଯଦି (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  ଯେତେବେଳେ (cluster < range->cluster_first)
	    range--;
	  ଯେତେବେଳେ (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	ଯଦି (!(range->flags & ac->subtable_flags))
	{
	  ଯଦି (buffer->idx == buffer->len)
	    ଭାଙ୍ଗ;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (ଶୂନ୍ୟ) buffer->next_glyph ();
	  ଜାରି;
	}
      }
      ଅନ୍ୟଥା
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	ତାର୍କିକ is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	ଯଦି (is_null_transition)
	{
	  unsigned old_klass = klass;
	  କର
	  {
	    c->transition (buffer, ଏହି, entry);

	    ଯଦି (buffer->idx == buffer->len || !buffer->successful)
	      ଭାଙ୍ଗ;

	    (ଶୂନ୍ୟ) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, ଅଙ୍କ_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } ଯେତେବେଳେ (klass == old_klass);

	  ଯଦି (buffer->idx == buffer->len || !buffer->successful)
	    ଭାଙ୍ଗ;

	  ଯାଅ resume;
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
      ଅଚଳ EntryT *wouldbe_entry;
      ତାର୍କିକ is_safe_to_break =
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

      ଯଦି (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, ଏହି, entry);

      state = next_state;
      DEBUG_ସନ୍ଦେଶ (APPLY, nullptr, "s%d", state);

      ଯଦି (buffer->idx == buffer->len)
	ଭାଙ୍ଗ;

      ଯଦି (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(ଶୂନ୍ୟ) buffer->next_glyph ();
    }

    ଯଦି (!c->in_place)
      buffer->ସମକାଳ ();
  }

  ସର୍ବସାଧାରଣ:
  ଅଚଳ StateTableT &machine;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
