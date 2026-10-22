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

#അന്തര്ഭാവ "hb-aat-layout.hh"
#അന്തര്ഭാവ "hb-aat-map.hh"
#അന്തര്ഭാവ "hb-ot-layout-common.hh"
#അന്തര്ഭാവ "hb-ot-layout-gdef-table.hh"
#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-cache.hh"
#അന്തര്ഭാവ "hb-bit-set.hh"
#അന്തര്ഭാവ "hb-bit-page.hh"


നാമാകാശ OT {
ഘടന GDEF;
};

നാമാകാശ AAT {

using നാമാകാശ OT;

ഘടന ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#അല്ലെങ്കിൽ
#define HB_AAT_TRANSITION_INLINE
#endif

ഘടന hb_aat_scratch_t
{
  hb_aat_scratch_t () = സ്ഥിരസ്ഥിതി;
  hb_aat_scratch_t (സ്ഥിരം hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    തിരികെ *ഇത്;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    എങ്കിൽ (unlikely (!s))
      തിരികെ;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () സ്ഥിരം
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    എങ്കിൽ (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      തിരികെ s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, വലുപ്പം (hb_bit_set_t));
    എങ്കിൽ (unlikely (!s))
      തിരികെ nullptr;
    s->init ();

    തിരികെ s;
  }
  ശൂന്യം destroy_buffer_glyph_set (hb_bit_set_t *s) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!s))
      തിരികെ;
    എങ്കിൽ (buffer_glyph_set.cmpexch (nullptr, s))
      തിരികെ;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

എണ്ണൽ { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

ഘടന hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, തർക്കം, HB_DEBUG_APPLY>
{
  സ്ഥിരം അക്ഷരം *get_name () { തിരികെ "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (സ്ഥിരം T &obj, Ts&&... ds)
  { തിരികെ obj.apply (ഇത്, std::forward<Ts> (ds)...); }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ അസത്യം; }
  തർക്കം stop_sublookup_iteration (return_t r) സ്ഥിരം { തിരികെ r; }

  സ്ഥിരം hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  സ്ഥിരം ankr *ankr_table;
  സ്ഥിരം OT::GDEF &gdef;
  തർക്കം has_glyph_classes;
  സ്ഥിരം hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  തർക്കം buffer_is_reversed = അസത്യം;
  // Caches
  തർക്കം using_buffer_glyph_set = അസത്യം;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  സ്ഥിരം hb_bit_set_t *first_set = nullptr;
  സ്ഥിരം hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned പൂർണ്ണസംഖ്യ lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (സ്ഥിരം hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL ശൂന്യം set_ankr_table (സ്ഥിരം AAT::ankr *ankr_table_);

  ശൂന്യം set_lookup_index (unsigned പൂർണ്ണസംഖ്യ i) { lookup_index = i; }

  ശൂന്യം reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  ശൂന്യം setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    എങ്കിൽ (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  തർക്കം buffer_intersects_machine () സ്ഥിരം
  {
    എങ്കിൽ (likely (using_buffer_glyph_set))
      തിരികെ buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    വേണ്ടി (unsigned i = 0; i < buffer->len; i++)
      എങ്കിൽ (first_set->has (buffer->info[i].codepoint))
	തിരികെ സത്യം;
    തിരികെ അസത്യം;
  }

  template <typename T>
  HB_NODISCARD തർക്കം output_glyphs (unsigned പൂർണ്ണസംഖ്യ എണ്ണം,
				   സ്ഥിരം T *glyphs)
  {
    എങ്കിൽ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, എണ്ണം);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      തർക്കം at_end_of_text = buffer->idx == buffer->len;
      എങ്കിൽ (at_end_of_text)
	എങ്കിൽ (unlikely (!buffer->output_glyph (glyphs[i]))) തിരികെ അസത്യം;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      എങ്കിൽ (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      അല്ലെങ്കിൽ
      {
#ifndef HB_NO_OT_LAYOUT
	എങ്കിൽ (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      എങ്കിൽ (!at_end_of_text)
	എങ്കിൽ (unlikely (!buffer->output_glyph (glyphs[i]))) തിരികെ അസത്യം;
    }
    തിരികെ സത്യം;
  }

  HB_NODISCARD തർക്കം replace_glyph (hb_codepoint_t glyph)
  {
    എങ്കിൽ (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    എങ്കിൽ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    എങ്കിൽ (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    തിരികെ buffer->replace_glyph (glyph);
  }

  HB_NODISCARD തർക്കം delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    എങ്കിൽ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    തിരികെ buffer->replace_glyph (DELETED_GLYPH);
  }

  ശൂന്യം replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    എങ്കിൽ (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    എങ്കിൽ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    എങ്കിൽ (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> ഘടന Lookup;

template <typename T>
ഘടന LookupFormat0
{
  friend ഘടന Lookup<T>;

  സ്വകാര്യ:
  സ്ഥിരം T* get_മൂല്യം (hb_codepoint_t glyph_id, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (glyph_id >= അക്കം_glyphs)) തിരികെ nullptr;
    തിരികെ &arrayZ[glyph_id];
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    glyphs.add_range (0, അക്കം_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, unsigned അക്കം_glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < അക്കം_glyphs; i++)
      എങ്കിൽ (filter (arrayZ[i]))
	glyphs.add (i);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (arrayZ.sanitize (c, c->get_അക്കം_glyphs ()));
  }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (arrayZ.sanitize (c, c->get_അക്കം_glyphs (), base));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  പൊതു:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
ഘടന LookupSegmentSingle
{
  സ്ഥിതം constexpr unsigned Termination_Word_എണ്ണം = 2u;

  പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t g) സ്ഥിരം
  { തിരികെ g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs) സ്ഥിരം
  {
    എങ്കിൽ (first == DELETED_GLYPH) തിരികെ;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    എങ്കിൽ (first == DELETED_GLYPH) തിരികെ;
    എങ്കിൽ (!filter (value)) തിരികെ;
    glyphs.add_range (first, last);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && value.sanitize (c));
  }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  പൊതു:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
ഘടന LookupFormat2
{
  friend ഘടന Lookup<T>;

  സ്വകാര്യ:
  സ്ഥിരം T* get_മൂല്യം (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    സ്ഥിരം LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    തിരികെ v ? &v->value : nullptr;
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs) സ്ഥിരം
  {
    unsigned എണ്ണം = segments.get_length ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    unsigned എണ്ണം = segments.get_length ();
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (segments.sanitize (c));
  }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (segments.sanitize (c, base));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_തിരയൽ_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  പൊതു:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
ഘടന LookupSegmentArray
{
  സ്ഥിതം constexpr unsigned Termination_Word_എണ്ണം = 2u;

  സ്ഥിരം T* get_മൂല്യം (hb_codepoint_t glyph_id, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    തിരികെ first <= glyph_id && glyph_id <= last ? &(base+മൂല്യങ്ങൾ_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs) സ്ഥിരം
  {
    എങ്കിൽ (first == DELETED_GLYPH) തിരികെ;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, സ്ഥിരം ശൂന്യം *base, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    എങ്കിൽ (first == DELETED_GLYPH) തിരികെ;
    സ്ഥിരം auto &values = base+മൂല്യങ്ങൾ_Z;
    വേണ്ടി (hb_codepoint_t i = first; i <= last; i++)
      എങ്കിൽ (filter (values[i - first]))
	glyphs.add (i);
  }

  പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t g) സ്ഥിരം
  { തിരികെ g < first ? -1 : g <= last ? 0 : +1; }

  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  first <= last &&
		  മൂല്യങ്ങൾ_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base, Ts&&... ds) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  first <= last &&
		  മൂല്യങ്ങൾ_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		മൂല്യങ്ങൾ_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  പൊതു:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
ഘടന LookupFormat4
{
  friend ഘടന Lookup<T>;

  സ്വകാര്യ:
  സ്ഥിരം T* get_മൂല്യം (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    സ്ഥിരം LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    തിരികെ v ? v->get_മൂല്യം (glyph_id, ഇത്) : nullptr;
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs) സ്ഥിരം
  {
    unsigned എണ്ണം = segments.get_length ();
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    unsigned എണ്ണം = segments.get_length ();
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      segments[i].collect_glyphs_filtered (glyphs, ഇത്, filter);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (segments.sanitize (c, ഇത്));
  }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (segments.sanitize (c, ഇത്, base));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_തിരയൽ_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  പൊതു:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
ഘടന LookupSingle
{
  സ്ഥിതം constexpr unsigned Termination_Word_എണ്ണം = 1u;

  പൂർണ്ണസംഖ്യ cmp (hb_codepoint_t g) സ്ഥിരം { തിരികെ glyph.cmp (g); }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs) സ്ഥിരം
  {
    എങ്കിൽ (glyph == DELETED_GLYPH) തിരികെ;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    എങ്കിൽ (glyph == DELETED_GLYPH) തിരികെ;
    എങ്കിൽ (!filter (value)) തിരികെ;
    glyphs.add (glyph);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && value.sanitize (c));
  }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  പൊതു:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
ഘടന LookupFormat6
{
  friend ഘടന Lookup<T>;

  സ്വകാര്യ:
  സ്ഥിരം T* get_മൂല്യം (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    സ്ഥിരം LookupSingle<T> *v = entries.bsearch (glyph_id);
    തിരികെ v ? &v->value : nullptr;
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs) സ്ഥിരം
  {
    unsigned എണ്ണം = entries.get_length ();
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    unsigned എണ്ണം = entries.get_length ();
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (entries.sanitize (c));
  }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (entries.sanitize (c, base));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_തിരയൽ_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  പൊതു:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
ഘടന LookupFormat8
{
  friend ഘടന Lookup<T>;

  സ്വകാര്യ:
  സ്ഥിരം T* get_മൂല്യം (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    തിരികെ firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_എണ്ണം_2 ?
	   &മൂല്യം_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!glyph_എണ്ണം_2)) തിരികെ;
    എങ്കിൽ (firstGlyph == DELETED_GLYPH) തിരികെ;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_എണ്ണം_2 - 1);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!glyph_എണ്ണം_2)) തിരികെ;
    എങ്കിൽ (firstGlyph == DELETED_GLYPH) തിരികെ;
    സ്ഥിരം T *p = മൂല്യം_Array_Z.arrayZ;
    വേണ്ടി (unsigned i = 0; i < glyph_എണ്ണം_2; i++)
      എങ്കിൽ (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && മൂല്യം_Array_Z.sanitize (c, glyph_എണ്ണം_2));
  }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && മൂല്യം_Array_Z.sanitize (c, glyph_എണ്ണം_2, base));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_എണ്ണം_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		മൂല്യം_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  പൊതു:
  DEFINE_SIZE_ARRAY (6, മൂല്യം_Array_Z);
};

template <typename T>
ഘടന LookupFormat10
{
  friend ഘടന Lookup<T>;

  സ്വകാര്യ:
  സ്ഥിരം typename T::type get_മൂല്യം_or_null (hb_codepoint_t glyph_id) സ്ഥിരം
  {
    എങ്കിൽ (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_എണ്ണം_2))
      തിരികെ Null (T);

    സ്ഥിരം HBUINT8 *p = &മൂല്യം_Array_Z[(glyph_id - firstGlyph) * മൂല്യം_Size];

    unsigned പൂർണ്ണസംഖ്യ v = 0;
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = മൂല്യം_Size;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
      v = (v << 8) | *p++;

    തിരികെ v;
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!glyph_എണ്ണം_2)) തിരികെ;
    എങ്കിൽ (firstGlyph == DELETED_GLYPH) തിരികെ;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_എണ്ണം_2 - 1);
  }

  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!glyph_എണ്ണം_2)) തിരികെ;
    എങ്കിൽ (firstGlyph == DELETED_GLYPH) തിരികെ;
    സ്ഥിരം HBUINT8 *p = മൂല്യം_Array_Z.arrayZ;
    വേണ്ടി (unsigned i = 0; i < glyph_എണ്ണം_2; i++)
    {
      unsigned പൂർണ്ണസംഖ്യ v = 0;
      unsigned പൂർണ്ണസംഖ്യ എണ്ണം = മൂല്യം_Size;
      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; j < എണ്ണം; j++)
	v = (v << 8) | *p++;
      എങ്കിൽ (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  hb_barrier () &&
		  മൂല്യം_Size <= 4 &&
		  മൂല്യം_Array_Z.sanitize (c, glyph_എണ്ണം_2 * മൂല്യം_Size));
  }

  സംരക്ഷിത:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	മൂല്യം_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_എണ്ണം_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		മൂല്യം_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  പൊതു:
  DEFINE_SIZE_ARRAY (8, മൂല്യം_Array_Z);
};

template <typename T>
ഘടന Lookup
{
  സ്ഥിരം T* get_മൂല്യം (hb_codepoint_t glyph_id, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); തിരികെ u.format0.get_മൂല്യം (glyph_id, അക്കം_glyphs);
    അവസ്ഥ 2: hb_barrier (); തിരികെ u.format2.get_മൂല്യം (glyph_id);
    അവസ്ഥ 4: hb_barrier (); തിരികെ u.format4.get_മൂല്യം (glyph_id);
    അവസ്ഥ 6: hb_barrier (); തിരികെ u.format6.get_മൂല്യം (glyph_id);
    അവസ്ഥ 8: hb_barrier (); തിരികെ u.format8.get_മൂല്യം (glyph_id);
    സ്ഥിരസ്ഥിതി:തിരികെ nullptr;
    }
  }

  സ്ഥിരം typename T::type get_മൂല്യം_or_null (hb_codepoint_t glyph_id, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
      /* Format 10 cannot return a pointer. */
      അവസ്ഥ 10: hb_barrier (); തിരികെ u.format10.get_മൂല്യം_or_null (glyph_id);
      സ്ഥിരസ്ഥിതി:
      സ്ഥിരം T *v = get_മൂല്യം (glyph_id, അക്കം_glyphs);
      തിരികെ v ? *v : Null (T);
    }
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs, unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); u.format0.collect_glyphs (glyphs, അക്കം_glyphs); തിരികെ;
    അവസ്ഥ 2: hb_barrier (); u.format2.collect_glyphs (glyphs); തിരികെ;
    അവസ്ഥ 4: hb_barrier (); u.format4.collect_glyphs (glyphs); തിരികെ;
    അവസ്ഥ 6: hb_barrier (); u.format6.collect_glyphs (glyphs); തിരികെ;
    അവസ്ഥ 8: hb_barrier (); u.format8.collect_glyphs (glyphs); തിരികെ;
    അവസ്ഥ 10: hb_barrier (); u.format10.collect_glyphs (glyphs); തിരികെ;
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, unsigned അക്കം_glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, അക്കം_glyphs, filter); തിരികെ;
    അവസ്ഥ 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); തിരികെ;
    അവസ്ഥ 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); തിരികെ;
    അവസ്ഥ 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); തിരികെ;
    അവസ്ഥ 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); തിരികെ;
    അവസ്ഥ 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); തിരികെ;
    സ്ഥിരസ്ഥിതി:തിരികെ;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
			      unsigned പൂർണ്ണസംഖ്യ outOfRange) സ്ഥിരം
  {
    സ്ഥിരം T *v = get_മൂല്യം (glyph_id, അക്കം_glyphs);
    തിരികെ v ? *v : outOfRange;
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    അവസ്ഥ 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    അവസ്ഥ 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    അവസ്ഥ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }
  തർക്കം sanitize (hb_sanitize_context_t *c, സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (!u.format.v.sanitize (c)) return_trace (അസത്യം);
    hb_barrier ();
    തിരഞ്ഞെടുപ്പ് (u.format.v) {
    അവസ്ഥ 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    അവസ്ഥ 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    അവസ്ഥ 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    അവസ്ഥ 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    അവസ്ഥ 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    അവസ്ഥ 10: return_trace (അസത്യം); /* We don't support format10 here currently. */
    സ്ഥിരസ്ഥിതി:return_trace (സത്യം);
    }
  }

  സംരക്ഷിത:
  സംഘം {
  ഘടന { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  പൊതു:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
ഘടന Entry
{
  // This doesn't seem like it's ever called.
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  പൊതു:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
ഘടന Entry<ശൂന്യം>
{
  // This does seem like it's ever called.
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്));
  }

  പൊതു:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  പൊതു:
  DEFINE_SIZE_STATIC (4);
};

എണ്ണൽ Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_വരി = 3,
};

template <typename Types, typename Extra>
ഘടന StateTable
{
  തരനിർവചനം typename Types::HBUINT HBUINT;
  തരനിർവചനം typename Types::HBUSHORT HBUSHORT;
  തരനിർവചനം typename Types::ClassTypeNarrow ClassType;

  എണ്ണൽ State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_വരി = 1,
  };

  template <typename set_t, typename table_t>
  ശൂന്യം collect_initial_glyphs (set_t &glyphs, unsigned അക്കം_glyphs, സ്ഥിരം table_t &table) സ്ഥിരം
  {
    unsigned അക്കം_classes = nClasses;

    എങ്കിൽ (unlikely (അക്കം_classes > hb_bit_page_t::BITS))
    {
      (ഇത്+classTable).collect_glyphs (glyphs, അക്കം_glyphs);
      തിരികെ;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    വേണ്ടി (unsigned i = 0; i < അക്കം_classes; i++)
    {
      സ്ഥിരം auto &entry = get_entry (STATE_START_OF_TEXT, i);
      എങ്കിൽ (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	തുടരുക;

      filter.add (i);
    }

    // And glyphs in those classes.

    എങ്കിൽ (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (ഇത്+classTable).collect_glyphs_filtered (glyphs, അക്കം_glyphs, filter);
  }

  പൂർണ്ണസംഖ്യ new_state (unsigned പൂർണ്ണസംഖ്യ newState) സ്ഥിരം
  { തിരികെ Types::extended ? newState : ((പൂർണ്ണസംഖ്യ) newState - (പൂർണ്ണസംഖ്യ) stateArrayTable) / (പൂർണ്ണസംഖ്യ) nClasses; }

  unsigned പൂർണ്ണസംഖ്യ get_class (hb_codepoint_t glyph_id,
			  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) സ്ഥിരം
  {
    unsigned klass;
    എങ്കിൽ (cache && cache->get (glyph_id, &klass)) തിരികെ klass;
    എങ്കിൽ (unlikely (glyph_id == DELETED_GLYPH)) തിരികെ CLASS_DELETED_GLYPH;
    klass = (ഇത്+classTable).get_class (glyph_id, അക്കം_glyphs, CLASS_OUT_OF_BOUNDS);
    എങ്കിൽ (cache) cache->set (glyph_id, klass);
    തിരികെ klass;
  }

  സ്ഥിരം Entry<Extra> *get_entries () സ്ഥിരം
  { തിരികെ (ഇത്+entryTable).arrayZ; }

  സ്ഥിരം Entry<Extra> &get_entry (പൂർണ്ണസംഖ്യ state, unsigned പൂർണ്ണസംഖ്യ klass) സ്ഥിരം
  {
    unsigned n_classes = nClasses;
    എങ്കിൽ (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    സ്ഥിരം HBUSHORT *states = (ഇത്+stateArrayTable).arrayZ;
    സ്ഥിരം Entry<Extra> *entries = (ഇത്+entryTable).arrayZ;

    unsigned പൂർണ്ണസംഖ്യ entry = states[state * n_classes + klass];
    DEBUG_സന്ദേശം (APPLY, nullptr, "e%u", entry);

    തിരികെ entries[entry];
  }

  തർക്കം sanitize (hb_sanitize_context_t *c,
		 unsigned പൂർണ്ണസംഖ്യ *അക്കം_entries_out = nullptr) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    എങ്കിൽ (unlikely (!(c->check_struct (ഇത്) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, ഇത്)))) return_trace (അസത്യം);

    സ്ഥിരം HBUSHORT *states = (ഇത്+stateArrayTable).arrayZ;
    സ്ഥിരം Entry<Extra> *entries = (ഇത്+entryTable).arrayZ;

    unsigned പൂർണ്ണസംഖ്യ അക്കം_classes = nClasses;
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (അക്കം_classes, states[0].static_size)))
      return_trace (അസത്യം);
    unsigned പൂർണ്ണസംഖ്യ row_stride = അക്കം_classes * states[0].static_size;

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

    പൂർണ്ണസംഖ്യ min_state = 0;
    പൂർണ്ണസംഖ്യ max_state = 0;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_entries = 0;

    പൂർണ്ണസംഖ്യ state_pos = 0;
    പൂർണ്ണസംഖ്യ state_neg = 0;
    unsigned പൂർണ്ണസംഖ്യ entry = 0;
    ആകുമ്പോൾ (min_state < state_neg || state_pos <= max_state)
    {
      എങ്കിൽ (min_state < state_neg)
      {
	/* Negative states. */
	എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (min_state, അക്കം_classes)))
	  return_trace (അസത്യം);
	എങ്കിൽ (unlikely (!c->check_range (&states[min_state * അക്കം_classes],
				       -min_state,
				       row_stride)))
	  return_trace (അസത്യം);
	എങ്കിൽ ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (അസത്യം);
	{ /* Sweep new states. */
	  സ്ഥിരം HBUSHORT *stop = &states[min_state * അക്കം_classes];
	  എങ്കിൽ (unlikely (stop > states))
	    return_trace (അസത്യം);
	  വേണ്ടി (സ്ഥിരം HBUSHORT *p = states; stop < p; p--)
	    അക്കം_entries = hb_max (അക്കം_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      എങ്കിൽ (state_pos <= max_state)
      {
	/* Positive states. */
	എങ്കിൽ (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (അസത്യം);
	എങ്കിൽ ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (അസത്യം);
	{ /* Sweep new states. */
	  എങ്കിൽ (unlikely (hb_unsigned_mul_overflows ((max_state + 1), അക്കം_classes)))
	    return_trace (അസത്യം);
	  സ്ഥിരം HBUSHORT *stop = &states[(max_state + 1) * അക്കം_classes];
	  എങ്കിൽ (unlikely (stop < states))
	    return_trace (അസത്യം);
	  വേണ്ടി (സ്ഥിരം HBUSHORT *p = &states[state_pos * അക്കം_classes]; p < stop; p++)
	    അക്കം_entries = hb_max (അക്കം_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      എങ്കിൽ (unlikely (!c->check_array (entries, അക്കം_entries)))
	return_trace (അസത്യം);
      എങ്കിൽ ((c->max_ops -= അക്കം_entries - entry) <= 0)
	return_trace (അസത്യം);
      { /* Sweep new entries. */
	സ്ഥിരം Entry<Extra> *stop = &entries[അക്കം_entries];
	വേണ്ടി (സ്ഥിരം Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  പൂർണ്ണസംഖ്യ newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = അക്കം_entries;
      }
    }

    എങ്കിൽ (അക്കം_entries_out)
      *അക്കം_entries_out = അക്കം_entries;

    return_trace (സത്യം);
  }

  സംരക്ഷിത:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  പൊതു:
  DEFINE_SIZE_STATIC (4 * വലുപ്പം (HBUINT));
};

template <typename HBUCHAR>
ഘടന ClassTable
{
  unsigned പൂർണ്ണസംഖ്യ get_class (hb_codepoint_t glyph_id, unsigned പൂർണ്ണസംഖ്യ outOfRange) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i = glyph_id - firstGlyph;
    തിരികെ i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned പൂർണ്ണസംഖ്യ get_class (hb_codepoint_t glyph_id,
			  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs HB_UNUSED,
			  unsigned പൂർണ്ണസംഖ്യ outOfRange) സ്ഥിരം
  {
    തിരികെ get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  ശൂന്യം collect_glyphs (set_t &glyphs, unsigned അക്കം_glyphs) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < classArray.len; i++)
      എങ്കിൽ (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  ശൂന്യം collect_glyphs_filtered (set_t &glyphs, unsigned അക്കം_glyphs, സ്ഥിരം filter_t &filter) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < classArray.len; i++)
      എങ്കിൽ (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) && classArray.sanitize (c));
  }
  സംരക്ഷിത:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  പൊതു:
  DEFINE_SIZE_ARRAY (4, classArray);
};

ഘടന SubtableGlyphCoverage
{
  തർക്കം sanitize (hb_sanitize_context_t *c, unsigned subtable_എണ്ണം) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    എങ്കിൽ (unlikely (!c->check_array (&subtableOffsets, subtable_എണ്ണം)))
      return_trace (അസത്യം);

    unsigned bytes = (c->get_അക്കം_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    വേണ്ടി (unsigned i = 0; i < subtable_എണ്ണം; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      എങ്കിൽ (offset == 0 || offset == 0xFFFFFFFF)
        തുടരുക;
      എങ്കിൽ (unlikely (!subtableOffsets[i].sanitize (c, ഇത്, bytes)))
        return_trace (അസത്യം);
    }

    return_trace (സത്യം);
  }
  സംരക്ഷിത:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  പൊതു:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

ഘടന ObsoleteTypes
{
  സ്ഥിതം constexpr തർക്കം extended = അസത്യം;
  തരനിർവചനം HBUINT16 HBUINT;
  തരനിർവചനം HBUINT8 HBUSHORT;
  തരനിർവചനം ClassTable<HBUINT8> ClassTypeNarrow;
  തരനിർവചനം ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  സ്ഥിതം പൂർണ്ണസംഖ്യ offsetToIndex (int64_t offset,
			    സ്ഥിരം ശൂന്യം *base,
			    സ്ഥിരം T *array)
  {
    int64_t array_offset = (സ്ഥിരം അക്ഷരം *) array - (സ്ഥിരം അക്ഷരം *) base;
    പൂർണ്ണസംഖ്യ bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    എങ്കിൽ (unlikely (offset < array_offset))
      തിരികെ bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    എങ്കിൽ (unlikely (index > bad_index))
      തിരികെ bad_index;
    തിരികെ index;
  }
  template <typename T>
  സ്ഥിതം പൂർണ്ണസംഖ്യ byteOffsetToIndex (int64_t offset,
				സ്ഥിരം ശൂന്യം *base,
				സ്ഥിരം T *array)
  {
    തിരികെ offsetToIndex (offset, base, array);
  }
  template <typename T>
  സ്ഥിതം പൂർണ്ണസംഖ്യ wordOffsetToIndex (int64_t offset,
				സ്ഥിരം ശൂന്യം *base,
				സ്ഥിരം T *array)
  {
    തിരികെ offsetToIndex (2 * offset, base, array);
  }
};
ഘടന ExtendedTypes
{
  സ്ഥിതം constexpr തർക്കം extended = സത്യം;
  തരനിർവചനം HBUINT32 HBUINT;
  തരനിർവചനം HBUINT16 HBUSHORT;
  തരനിർവചനം Lookup<HBUINT16> ClassTypeNarrow;
  തരനിർവചനം Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ offsetToIndex (unsigned പൂർണ്ണസംഖ്യ offset,
				     സ്ഥിരം ശൂന്യം *base HB_UNUSED,
				     സ്ഥിരം T *array HB_UNUSED)
  {
    തിരികെ offset;
  }
  template <typename T>
  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ byteOffsetToIndex (unsigned പൂർണ്ണസംഖ്യ offset,
					 സ്ഥിരം ശൂന്യം *base HB_UNUSED,
					 സ്ഥിരം T *array HB_UNUSED)
  {
    തിരികെ offset / 2;
  }
  template <typename T>
  സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ wordOffsetToIndex (unsigned പൂർണ്ണസംഖ്യ offset,
					 സ്ഥിരം ശൂന്യം *base HB_UNUSED,
					 സ്ഥിരം T *array HB_UNUSED)
  {
    തിരികെ offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
ഘടന StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (സ്ഥിരം StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      അക്കം_glyphs (face_->get_അക്കം_glyphs ()) {}

  template <typename context_t>
  ശൂന്യം drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    എങ്കിൽ (!c->in_place)
      buffer->clear_output ();

    പൂർണ്ണസംഖ്യ state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    സ്ഥിരം തർക്കം start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    വേണ്ടി (buffer->idx = 0; buffer->successful;)
    {
      unsigned പൂർണ്ണസംഖ്യ klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, അക്കം_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_സന്ദേശം (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      സ്ഥിരം EntryT &entry = machine.get_entry (state, klass);
      സ്ഥിരം പൂർണ്ണസംഖ്യ next_state = machine.new_state (entry.newState);

      തർക്കം is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      തർക്കം is_not_actionable = !c->table->is_actionable (entry);

      എങ്കിൽ (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	എങ്കിൽ (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  ആകുമ്പോൾ (cluster < range->cluster_first)
	    range--;
	  ആകുമ്പോൾ (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	എങ്കിൽ (!(range->flags & ac->subtable_flags))
	{
	  എങ്കിൽ (buffer->idx == buffer->len)
	    നിർത്തുക;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (ശൂന്യം) buffer->next_glyph ();
	  തുടരുക;
	}
      }
      അല്ലെങ്കിൽ
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	തർക്കം is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	എങ്കിൽ (is_null_transition)
	{
	  unsigned old_klass = klass;
	  ചെയ്യുക
	  {
	    c->transition (buffer, ഇത്, entry);

	    എങ്കിൽ (buffer->idx == buffer->len || !buffer->successful)
	      നിർത്തുക;

	    (ശൂന്യം) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, അക്കം_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } ആകുമ്പോൾ (klass == old_klass);

	  എങ്കിൽ (buffer->idx == buffer->len || !buffer->successful)
	    നിർത്തുക;

	  പോകുക resume;
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
      സ്ഥിരം EntryT *wouldbe_entry;
      തർക്കം is_safe_to_break =
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

      എങ്കിൽ (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, ഇത്, entry);

      state = next_state;
      DEBUG_സന്ദേശം (APPLY, nullptr, "s%d", state);

      എങ്കിൽ (buffer->idx == buffer->len)
	നിർത്തുക;

      എങ്കിൽ (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(ശൂന്യം) buffer->next_glyph ();
    }

    എങ്കിൽ (!c->in_place)
      buffer->സമന്വയം ();
  }

  പൊതു:
  സ്ഥിരം StateTableT &machine;
  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
