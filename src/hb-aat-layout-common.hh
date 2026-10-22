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

#અન્તર્ભાવ "hb-aat-layout.hh"
#અન્તર્ભાવ "hb-aat-map.hh"
#અન્તર્ભાવ "hb-ot-layout-common.hh"
#અન્તર્ભાવ "hb-ot-layout-gdef-table.hh"
#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-cache.hh"
#અન્તર્ભાવ "hb-bit-set.hh"
#અન્તર્ભાવ "hb-bit-page.hh"


નામાકાશ OT {
સંરચના GDEF;
};

નામાકાશ AAT {

using નામાકાશ OT;

સંરચના ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#નહીંતર
#define HB_AAT_TRANSITION_INLINE
#endif

સંરચના hb_aat_scratch_t
{
  hb_aat_scratch_t () = મૂળભૂત;
  hb_aat_scratch_t (અચલ hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    પાછા *આ;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    જો (unlikely (!s))
      પાછા;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () અચલ
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    જો (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      પાછા s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, કદ (hb_bit_set_t));
    જો (unlikely (!s))
      પાછા nullptr;
    s->init ();

    પાછા s;
  }
  શૂન્ય destroy_buffer_glyph_set (hb_bit_set_t *s) અચલ
  {
    જો (unlikely (!s))
      પાછા;
    જો (buffer_glyph_set.cmpexch (nullptr, s))
      પાછા;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

ગણના { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

સંરચના hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, તાર્કિક, HB_DEBUG_APPLY>
{
  અચલ અક્ષર *get_name () { પાછા "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (અચલ T &obj, Ts&&... ds)
  { પાછા obj.apply (આ, std::forward<Ts> (ds)...); }
  સ્થિર return_t default_return_કિંમત () { પાછા ખોટું; }
  તાર્કિક stop_sublookup_iteration (return_t r) અચલ { પાછા r; }

  અચલ hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  અચલ ankr *ankr_table;
  અચલ OT::GDEF &gdef;
  તાર્કિક has_glyph_classes;
  અચલ hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  તાર્કિક buffer_is_reversed = ખોટું;
  // Caches
  તાર્કિક using_buffer_glyph_set = ખોટું;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  અચલ hb_bit_set_t *first_set = nullptr;
  અચલ hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned પૂર્ણાંક lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (અચલ hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL શૂન્ય set_ankr_table (અચલ AAT::ankr *ankr_table_);

  શૂન્ય set_lookup_index (unsigned પૂર્ણાંક i) { lookup_index = i; }

  શૂન્ય reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  શૂન્ય setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    જો (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  તાર્કિક buffer_intersects_machine () અચલ
  {
    જો (likely (using_buffer_glyph_set))
      પાછા buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    માટે (unsigned i = 0; i < buffer->len; i++)
      જો (first_set->has (buffer->info[i].codepoint))
	પાછા સાચું;
    પાછા ખોટું;
  }

  template <typename T>
  HB_NODISCARD તાર્કિક output_glyphs (unsigned પૂર્ણાંક ગણતરી,
				   અચલ T *glyphs)
  {
    જો (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, ગણતરી);
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      તાર્કિક at_end_of_text = buffer->idx == buffer->len;
      જો (at_end_of_text)
	જો (unlikely (!buffer->output_glyph (glyphs[i]))) પાછા ખોટું;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      જો (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      નહીંતર
      {
#ifndef HB_NO_OT_LAYOUT
	જો (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      જો (!at_end_of_text)
	જો (unlikely (!buffer->output_glyph (glyphs[i]))) પાછા ખોટું;
    }
    પાછા સાચું;
  }

  HB_NODISCARD તાર્કિક replace_glyph (hb_codepoint_t glyph)
  {
    જો (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    જો (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    જો (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    પાછા buffer->replace_glyph (glyph);
  }

  HB_NODISCARD તાર્કિક delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    જો (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    પાછા buffer->replace_glyph (DELETED_GLYPH);
  }

  શૂન્ય replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    જો (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    જો (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    જો (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> સંરચના Lookup;

template <typename T>
સંરચના LookupFormat0
{
  friend સંરચના Lookup<T>;

  ખાનગી:
  અચલ T* get_કિંમત (hb_codepoint_t glyph_id, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    જો (unlikely (glyph_id >= અંક_glyphs)) પાછા nullptr;
    પાછા &arrayZ[glyph_id];
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs, unsigned અંક_glyphs) અચલ
  {
    glyphs.add_range (0, અંક_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, unsigned અંક_glyphs, અચલ filter_t &filter) અચલ
  {
    માટે (unsigned i = 0; i < અંક_glyphs; i++)
      જો (filter (arrayZ[i]))
	glyphs.add (i);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (arrayZ.sanitize (c, c->get_અંક_glyphs ()));
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (arrayZ.sanitize (c, c->get_અંક_glyphs (), base));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  જાહેર:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
સંરચના LookupSegmentSingle
{
  સ્થિર constexpr unsigned Termination_Word_ગણતરી = 2u;

  પૂર્ણાંક cmp (hb_codepoint_t g) અચલ
  { પાછા g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs) અચલ
  {
    જો (first == DELETED_GLYPH) પાછા;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, અચલ filter_t &filter) અચલ
  {
    જો (first == DELETED_GLYPH) પાછા;
    જો (!filter (value)) પાછા;
    glyphs.add_range (first, last);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && value.sanitize (c));
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  જાહેર:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
સંરચના LookupFormat2
{
  friend સંરચના Lookup<T>;

  ખાનગી:
  અચલ T* get_કિંમત (hb_codepoint_t glyph_id) અચલ
  {
    અચલ LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    પાછા v ? &v->value : nullptr;
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs) અચલ
  {
    unsigned ગણતરી = segments.get_length ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, અચલ filter_t &filter) અચલ
  {
    unsigned ગણતરી = segments.get_length ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (segments.sanitize (c));
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (segments.sanitize (c, base));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_શોધ_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  જાહેર:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
સંરચના LookupSegmentArray
{
  સ્થિર constexpr unsigned Termination_Word_ગણતરી = 2u;

  અચલ T* get_કિંમત (hb_codepoint_t glyph_id, અચલ શૂન્ય *base) અચલ
  {
    પાછા first <= glyph_id && glyph_id <= last ? &(base+કિંમતો_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs) અચલ
  {
    જો (first == DELETED_GLYPH) પાછા;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, અચલ શૂન્ય *base, અચલ filter_t &filter) અચલ
  {
    જો (first == DELETED_GLYPH) પાછા;
    અચલ auto &values = base+કિંમતો_Z;
    માટે (hb_codepoint_t i = first; i <= last; i++)
      જો (filter (values[i - first]))
	glyphs.add (i);
  }

  પૂર્ણાંક cmp (hb_codepoint_t g) અચલ
  { પાછા g < first ? -1 : g <= last ? 0 : +1; }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  first <= last &&
		  કિંમતો_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base, Ts&&... ds) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  first <= last &&
		  કિંમતો_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		કિંમતો_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
સંરચના LookupFormat4
{
  friend સંરચના Lookup<T>;

  ખાનગી:
  અચલ T* get_કિંમત (hb_codepoint_t glyph_id) અચલ
  {
    અચલ LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    પાછા v ? v->get_કિંમત (glyph_id, આ) : nullptr;
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs) અચલ
  {
    unsigned ગણતરી = segments.get_length ();
    માટે (unsigned i = 0; i < ગણતરી; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, અચલ filter_t &filter) અચલ
  {
    unsigned ગણતરી = segments.get_length ();
    માટે (unsigned i = 0; i < ગણતરી; i++)
      segments[i].collect_glyphs_filtered (glyphs, આ, filter);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (segments.sanitize (c, આ));
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (segments.sanitize (c, આ, base));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_શોધ_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  જાહેર:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
સંરચના LookupSingle
{
  સ્થિર constexpr unsigned Termination_Word_ગણતરી = 1u;

  પૂર્ણાંક cmp (hb_codepoint_t g) અચલ { પાછા glyph.cmp (g); }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs) અચલ
  {
    જો (glyph == DELETED_GLYPH) પાછા;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, અચલ filter_t &filter) અચલ
  {
    જો (glyph == DELETED_GLYPH) પાછા;
    જો (!filter (value)) પાછા;
    glyphs.add (glyph);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && value.sanitize (c));
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  જાહેર:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
સંરચના LookupFormat6
{
  friend સંરચના Lookup<T>;

  ખાનગી:
  અચલ T* get_કિંમત (hb_codepoint_t glyph_id) અચલ
  {
    અચલ LookupSingle<T> *v = entries.bsearch (glyph_id);
    પાછા v ? &v->value : nullptr;
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs) અચલ
  {
    unsigned ગણતરી = entries.get_length ();
    માટે (unsigned i = 0; i < ગણતરી; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, અચલ filter_t &filter) અચલ
  {
    unsigned ગણતરી = entries.get_length ();
    માટે (unsigned i = 0; i < ગણતરી; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (entries.sanitize (c));
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (entries.sanitize (c, base));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_શોધ_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  જાહેર:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
સંરચના LookupFormat8
{
  friend સંરચના Lookup<T>;

  ખાનગી:
  અચલ T* get_કિંમત (hb_codepoint_t glyph_id) અચલ
  {
    પાછા firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_ગણતરી_2 ?
	   &કિંમત_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs) અચલ
  {
    જો (unlikely (!glyph_ગણતરી_2)) પાછા;
    જો (firstGlyph == DELETED_GLYPH) પાછા;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_ગણતરી_2 - 1);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, અચલ filter_t &filter) અચલ
  {
    જો (unlikely (!glyph_ગણતરી_2)) પાછા;
    જો (firstGlyph == DELETED_GLYPH) પાછા;
    અચલ T *p = કિંમત_Array_Z.arrayZ;
    માટે (unsigned i = 0; i < glyph_ગણતરી_2; i++)
      જો (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && કિંમત_Array_Z.sanitize (c, glyph_ગણતરી_2));
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && કિંમત_Array_Z.sanitize (c, glyph_ગણતરી_2, base));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ગણતરી_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		કિંમત_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  જાહેર:
  DEFINE_SIZE_ARRAY (6, કિંમત_Array_Z);
};

template <typename T>
સંરચના LookupFormat10
{
  friend સંરચના Lookup<T>;

  ખાનગી:
  અચલ typename T::type get_કિંમત_or_null (hb_codepoint_t glyph_id) અચલ
  {
    જો (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_ગણતરી_2))
      પાછા Null (T);

    અચલ HBUINT8 *p = &કિંમત_Array_Z[(glyph_id - firstGlyph) * કિંમત_Size];

    unsigned પૂર્ણાંક v = 0;
    unsigned પૂર્ણાંક ગણતરી = કિંમત_Size;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      v = (v << 8) | *p++;

    પાછા v;
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs) અચલ
  {
    જો (unlikely (!glyph_ગણતરી_2)) પાછા;
    જો (firstGlyph == DELETED_GLYPH) પાછા;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_ગણતરી_2 - 1);
  }

  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, અચલ filter_t &filter) અચલ
  {
    જો (unlikely (!glyph_ગણતરી_2)) પાછા;
    જો (firstGlyph == DELETED_GLYPH) પાછા;
    અચલ HBUINT8 *p = કિંમત_Array_Z.arrayZ;
    માટે (unsigned i = 0; i < glyph_ગણતરી_2; i++)
    {
      unsigned પૂર્ણાંક v = 0;
      unsigned પૂર્ણાંક ગણતરી = કિંમત_Size;
      માટે (unsigned પૂર્ણાંક j = 0; j < ગણતરી; j++)
	v = (v << 8) | *p++;
      જો (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  કિંમત_Size <= 4 &&
		  કિંમત_Array_Z.sanitize (c, glyph_ગણતરી_2 * કિંમત_Size));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	કિંમત_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ગણતરી_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		કિંમત_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  જાહેર:
  DEFINE_SIZE_ARRAY (8, કિંમત_Array_Z);
};

template <typename T>
સંરચના Lookup
{
  અચલ T* get_કિંમત (hb_codepoint_t glyph_id, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_કિંમત (glyph_id, અંક_glyphs);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_કિંમત (glyph_id);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_કિંમત (glyph_id);
    સ્થિતિ 6: hb_barrier (); પાછા u.format6.get_કિંમત (glyph_id);
    સ્થિતિ 8: hb_barrier (); પાછા u.format8.get_કિંમત (glyph_id);
    મૂળભૂત:પાછા nullptr;
    }
  }

  અચલ typename T::type get_કિંમત_or_null (hb_codepoint_t glyph_id, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પસંદગી (u.format.v) {
      /* Format 10 cannot return a pointer. */
      સ્થિતિ 10: hb_barrier (); પાછા u.format10.get_કિંમત_or_null (glyph_id);
      મૂળભૂત:
      અચલ T *v = get_કિંમત (glyph_id, અંક_glyphs);
      પાછા v ? *v : Null (T);
    }
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs, unsigned પૂર્ણાંક અંક_glyphs) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); u.format0.collect_glyphs (glyphs, અંક_glyphs); પાછા;
    સ્થિતિ 2: hb_barrier (); u.format2.collect_glyphs (glyphs); પાછા;
    સ્થિતિ 4: hb_barrier (); u.format4.collect_glyphs (glyphs); પાછા;
    સ્થિતિ 6: hb_barrier (); u.format6.collect_glyphs (glyphs); પાછા;
    સ્થિતિ 8: hb_barrier (); u.format8.collect_glyphs (glyphs); પાછા;
    સ્થિતિ 10: hb_barrier (); u.format10.collect_glyphs (glyphs); પાછા;
    મૂળભૂત:પાછા;
    }
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, unsigned અંક_glyphs, અચલ filter_t &filter) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, અંક_glyphs, filter); પાછા;
    સ્થિતિ 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); પાછા;
    સ્થિતિ 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); પાછા;
    સ્થિતિ 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); પાછા;
    સ્થિતિ 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); પાછા;
    સ્થિતિ 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); પાછા;
    મૂળભૂત:પાછા;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned પૂર્ણાંક અંક_glyphs,
			      unsigned પૂર્ણાંક outOfRange) અચલ
  {
    અચલ T *v = get_કિંમત (glyph_id, અંક_glyphs);
    પાછા v ? *v : outOfRange;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    સ્થિતિ 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    સ્થિતિ 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    સ્થિતિ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    મૂળભૂત:return_trace (સાચું);
    }
  }
  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    સ્થિતિ 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    સ્થિતિ 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    સ્થિતિ 10: return_trace (ખોટું); /* We don't support format10 here currently. */
    મૂળભૂત:return_trace (સાચું);
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
સંરચના Entry
{
  // This doesn't seem like it's ever called.
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  જાહેર:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
સંરચના Entry<શૂન્ય>
{
  // This does seem like it's ever called.
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  જાહેર:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

ગણના Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_લીટી = 3,
};

template <typename Types, typename Extra>
સંરચના StateTable
{
  પ્રકારવ્યાખ્યા typename Types::HBUINT HBUINT;
  પ્રકારવ્યાખ્યા typename Types::HBUSHORT HBUSHORT;
  પ્રકારવ્યાખ્યા typename Types::ClassTypeNarrow ClassType;

  ગણના State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_લીટી = 1,
  };

  template <typename set_t, typename table_t>
  શૂન્ય collect_initial_glyphs (set_t &glyphs, unsigned અંક_glyphs, અચલ table_t &table) અચલ
  {
    unsigned અંક_classes = nClasses;

    જો (unlikely (અંક_classes > hb_bit_page_t::BITS))
    {
      (આ+classTable).collect_glyphs (glyphs, અંક_glyphs);
      પાછા;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    માટે (unsigned i = 0; i < અંક_classes; i++)
    {
      અચલ auto &entry = get_entry (STATE_START_OF_TEXT, i);
      જો (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	ચાલુ;

      filter.add (i);
    }

    // And glyphs in those classes.

    જો (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (આ+classTable).collect_glyphs_filtered (glyphs, અંક_glyphs, filter);
  }

  પૂર્ણાંક new_state (unsigned પૂર્ણાંક newState) અચલ
  { પાછા Types::extended ? newState : ((પૂર્ણાંક) newState - (પૂર્ણાંક) stateArrayTable) / (પૂર્ણાંક) nClasses; }

  unsigned પૂર્ણાંક get_class (hb_codepoint_t glyph_id,
			  unsigned પૂર્ણાંક અંક_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) અચલ
  {
    unsigned klass;
    જો (cache && cache->get (glyph_id, &klass)) પાછા klass;
    જો (unlikely (glyph_id == DELETED_GLYPH)) પાછા CLASS_DELETED_GLYPH;
    klass = (આ+classTable).get_class (glyph_id, અંક_glyphs, CLASS_OUT_OF_BOUNDS);
    જો (cache) cache->set (glyph_id, klass);
    પાછા klass;
  }

  અચલ Entry<Extra> *get_entries () અચલ
  { પાછા (આ+entryTable).arrayZ; }

  અચલ Entry<Extra> &get_entry (પૂર્ણાંક state, unsigned પૂર્ણાંક klass) અચલ
  {
    unsigned n_classes = nClasses;
    જો (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    અચલ HBUSHORT *states = (આ+stateArrayTable).arrayZ;
    અચલ Entry<Extra> *entries = (આ+entryTable).arrayZ;

    unsigned પૂર્ણાંક entry = states[state * n_classes + klass];
    DEBUG_સંદેશ (APPLY, nullptr, "e%u", entry);

    પાછા entries[entry];
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 unsigned પૂર્ણાંક *અંક_entries_out = nullptr) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!(c->check_struct (આ) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, આ)))) return_trace (ખોટું);

    અચલ HBUSHORT *states = (આ+stateArrayTable).arrayZ;
    અચલ Entry<Extra> *entries = (આ+entryTable).arrayZ;

    unsigned પૂર્ણાંક અંક_classes = nClasses;
    જો (unlikely (hb_unsigned_mul_overflows (અંક_classes, states[0].static_size)))
      return_trace (ખોટું);
    unsigned પૂર્ણાંક row_stride = અંક_classes * states[0].static_size;

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

    પૂર્ણાંક min_state = 0;
    પૂર્ણાંક max_state = 0;
    unsigned પૂર્ણાંક અંક_entries = 0;

    પૂર્ણાંક state_pos = 0;
    પૂર્ણાંક state_neg = 0;
    unsigned પૂર્ણાંક entry = 0;
    જ્યારે (min_state < state_neg || state_pos <= max_state)
    {
      જો (min_state < state_neg)
      {
	/* Negative states. */
	જો (unlikely (hb_unsigned_mul_overflows (min_state, અંક_classes)))
	  return_trace (ખોટું);
	જો (unlikely (!c->check_range (&states[min_state * અંક_classes],
				       -min_state,
				       row_stride)))
	  return_trace (ખોટું);
	જો ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (ખોટું);
	{ /* Sweep new states. */
	  અચલ HBUSHORT *stop = &states[min_state * અંક_classes];
	  જો (unlikely (stop > states))
	    return_trace (ખોટું);
	  માટે (અચલ HBUSHORT *p = states; stop < p; p--)
	    અંક_entries = hb_max (અંક_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      જો (state_pos <= max_state)
      {
	/* Positive states. */
	જો (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (ખોટું);
	જો ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (ખોટું);
	{ /* Sweep new states. */
	  જો (unlikely (hb_unsigned_mul_overflows ((max_state + 1), અંક_classes)))
	    return_trace (ખોટું);
	  અચલ HBUSHORT *stop = &states[(max_state + 1) * અંક_classes];
	  જો (unlikely (stop < states))
	    return_trace (ખોટું);
	  માટે (અચલ HBUSHORT *p = &states[state_pos * અંક_classes]; p < stop; p++)
	    અંક_entries = hb_max (અંક_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      જો (unlikely (!c->check_array (entries, અંક_entries)))
	return_trace (ખોટું);
      જો ((c->max_ops -= અંક_entries - entry) <= 0)
	return_trace (ખોટું);
      { /* Sweep new entries. */
	અચલ Entry<Extra> *stop = &entries[અંક_entries];
	માટે (અચલ Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  પૂર્ણાંક newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = અંક_entries;
      }
    }

    જો (અંક_entries_out)
      *અંક_entries_out = અંક_entries;

    return_trace (સાચું);
  }

  સુરક્ષિત:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  જાહેર:
  DEFINE_SIZE_STATIC (4 * કદ (HBUINT));
};

template <typename HBUCHAR>
સંરચના ClassTable
{
  unsigned પૂર્ણાંક get_class (hb_codepoint_t glyph_id, unsigned પૂર્ણાંક outOfRange) અચલ
  {
    unsigned પૂર્ણાંક i = glyph_id - firstGlyph;
    પાછા i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned પૂર્ણાંક get_class (hb_codepoint_t glyph_id,
			  unsigned પૂર્ણાંક અંક_glyphs HB_UNUSED,
			  unsigned પૂર્ણાંક outOfRange) અચલ
  {
    પાછા get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  શૂન્ય collect_glyphs (set_t &glyphs, unsigned અંક_glyphs) અચલ
  {
    માટે (unsigned i = 0; i < classArray.len; i++)
      જો (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  શૂન્ય collect_glyphs_filtered (set_t &glyphs, unsigned અંક_glyphs, અચલ filter_t &filter) અચલ
  {
    માટે (unsigned i = 0; i < classArray.len; i++)
      જો (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && classArray.sanitize (c));
  }
  સુરક્ષિત:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  જાહેર:
  DEFINE_SIZE_ARRAY (4, classArray);
};

સંરચના SubtableGlyphCoverage
{
  તાર્કિક sanitize (hb_sanitize_context_t *c, unsigned subtable_ગણતરી) અચલ
  {
    TRACE_SANITIZE (આ);

    જો (unlikely (!c->check_array (&subtableOffsets, subtable_ગણતરી)))
      return_trace (ખોટું);

    unsigned bytes = (c->get_અંક_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    માટે (unsigned i = 0; i < subtable_ગણતરી; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      જો (offset == 0 || offset == 0xFFFFFFFF)
        ચાલુ;
      જો (unlikely (!subtableOffsets[i].sanitize (c, આ, bytes)))
        return_trace (ખોટું);
    }

    return_trace (સાચું);
  }
  સુરક્ષિત:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  જાહેર:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

સંરચના ObsoleteTypes
{
  સ્થિર constexpr તાર્કિક extended = ખોટું;
  પ્રકારવ્યાખ્યા HBUINT16 HBUINT;
  પ્રકારવ્યાખ્યા HBUINT8 HBUSHORT;
  પ્રકારવ્યાખ્યા ClassTable<HBUINT8> ClassTypeNarrow;
  પ્રકારવ્યાખ્યા ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  સ્થિર પૂર્ણાંક offsetToIndex (int64_t offset,
			    અચલ શૂન્ય *base,
			    અચલ T *array)
  {
    int64_t array_offset = (અચલ અક્ષર *) array - (અચલ અક્ષર *) base;
    પૂર્ણાંક bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    જો (unlikely (offset < array_offset))
      પાછા bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    જો (unlikely (index > bad_index))
      પાછા bad_index;
    પાછા index;
  }
  template <typename T>
  સ્થિર પૂર્ણાંક byteOffsetToIndex (int64_t offset,
				અચલ શૂન્ય *base,
				અચલ T *array)
  {
    પાછા offsetToIndex (offset, base, array);
  }
  template <typename T>
  સ્થિર પૂર્ણાંક wordOffsetToIndex (int64_t offset,
				અચલ શૂન્ય *base,
				અચલ T *array)
  {
    પાછા offsetToIndex (2 * offset, base, array);
  }
};
સંરચના ExtendedTypes
{
  સ્થિર constexpr તાર્કિક extended = સાચું;
  પ્રકારવ્યાખ્યા HBUINT32 HBUINT;
  પ્રકારવ્યાખ્યા HBUINT16 HBUSHORT;
  પ્રકારવ્યાખ્યા Lookup<HBUINT16> ClassTypeNarrow;
  પ્રકારવ્યાખ્યા Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  સ્થિર unsigned પૂર્ણાંક offsetToIndex (unsigned પૂર્ણાંક offset,
				     અચલ શૂન્ય *base HB_UNUSED,
				     અચલ T *array HB_UNUSED)
  {
    પાછા offset;
  }
  template <typename T>
  સ્થિર unsigned પૂર્ણાંક byteOffsetToIndex (unsigned પૂર્ણાંક offset,
					 અચલ શૂન્ય *base HB_UNUSED,
					 અચલ T *array HB_UNUSED)
  {
    પાછા offset / 2;
  }
  template <typename T>
  સ્થિર unsigned પૂર્ણાંક wordOffsetToIndex (unsigned પૂર્ણાંક offset,
					 અચલ શૂન્ય *base HB_UNUSED,
					 અચલ T *array HB_UNUSED)
  {
    પાછા offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
સંરચના StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (અચલ StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      અંક_glyphs (face_->get_અંક_glyphs ()) {}

  template <typename context_t>
  શૂન્ય drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    જો (!c->in_place)
      buffer->clear_output ();

    પૂર્ણાંક state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    અચલ તાર્કિક start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    માટે (buffer->idx = 0; buffer->successful;)
    {
      unsigned પૂર્ણાંક klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, અંક_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_સંદેશ (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      અચલ EntryT &entry = machine.get_entry (state, klass);
      અચલ પૂર્ણાંક next_state = machine.new_state (entry.newState);

      તાર્કિક is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      તાર્કિક is_not_actionable = !c->table->is_actionable (entry);

      જો (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	જો (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  જ્યારે (cluster < range->cluster_first)
	    range--;
	  જ્યારે (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	જો (!(range->flags & ac->subtable_flags))
	{
	  જો (buffer->idx == buffer->len)
	    તોડો;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (શૂન્ય) buffer->next_glyph ();
	  ચાલુ;
	}
      }
      નહીંતર
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	તાર્કિક is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	જો (is_null_transition)
	{
	  unsigned old_klass = klass;
	  કરો
	  {
	    c->transition (buffer, આ, entry);

	    જો (buffer->idx == buffer->len || !buffer->successful)
	      તોડો;

	    (શૂન્ય) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, અંક_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } જ્યારે (klass == old_klass);

	  જો (buffer->idx == buffer->len || !buffer->successful)
	    તોડો;

	  જાઓ resume;
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
      અચલ EntryT *wouldbe_entry;
      તાર્કિક is_safe_to_break =
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

      જો (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, આ, entry);

      state = next_state;
      DEBUG_સંદેશ (APPLY, nullptr, "s%d", state);

      જો (buffer->idx == buffer->len)
	તોડો;

      જો (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(શૂન્ય) buffer->next_glyph ();
    }

    જો (!c->in_place)
      buffer->સમકાલિક ();
  }

  જાહેર:
  અચલ StateTableT &machine;
  unsigned પૂર્ણાંક અંક_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
