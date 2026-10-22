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

#ضمن "hb-aat-layout.hh"
#ضمن "hb-aat-map.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-cache.hh"
#ضمن "hb-bit-set.hh"
#ضمن "hb-bit-page.hh"


نطاق OT {
بنية GDEF;
};

نطاق AAT {

using نطاق OT;

بنية ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#אחרת
#define HB_AAT_TRANSITION_INLINE
#endif

بنية hb_aat_scratch_t
{
  hb_aat_scratch_t () = افتراضي;
  hb_aat_scratch_t (ثابت hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    החזר *هذا;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    אם (unlikely (!s))
      החזר;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () ثابت
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    אם (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      החזר s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, حجم (hb_bit_set_t));
    אם (unlikely (!s))
      החזר nullptr;
    s->init ();

    החזר s;
  }
  فراغ destroy_buffer_glyph_set (hb_bit_set_t *s) ثابت
  {
    אם (unlikely (!s))
      החזר;
    אם (buffer_glyph_set.cmpexch (nullptr, s))
      החזר;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

تعداد { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

بنية hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { החזר "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { החזר obj.apply (هذا, std::forward<Ts> (ds)...); }
  ساكن return_t default_return_value () { החזר שקר; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { החזר r; }

  ثابت hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  ثابت ankr *ankr_table;
  ثابت OT::GDEF &gdef;
  منطقي has_glyph_classes;
  ثابت hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  منطقي buffer_is_reversed = שקר;
  // Caches
  منطقي using_buffer_glyph_set = שקר;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  ثابت hb_bit_set_t *first_set = nullptr;
  ثابت hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned صحيح lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (ثابت hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL فراغ set_ankr_table (ثابت AAT::ankr *ankr_table_);

  فراغ set_lookup_index (unsigned صحيح i) { lookup_index = i; }

  فراغ reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  فراغ setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    אם (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  منطقي buffer_intersects_machine () ثابت
  {
    אם (likely (using_buffer_glyph_set))
      החזר buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    עבור (unsigned i = 0; i < buffer->len; i++)
      אם (first_set->has (buffer->info[i].codepoint))
	החזר אמת;
    החזר שקר;
  }

  template <typename T>
  HB_NODISCARD منطقي output_glyphs (unsigned صحيح count,
				   ثابت T *glyphs)
  {
    אם (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, count);
    עבור (unsigned صحيح i = 0; i < count; i++)
    {
      منطقي at_end_of_text = buffer->idx == buffer->len;
      אם (at_end_of_text)
	אם (unlikely (!buffer->output_glyph (glyphs[i]))) החזר שקר;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      אם (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      אחרת
      {
#ifndef HB_NO_OT_LAYOUT
	אם (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      אם (!at_end_of_text)
	אם (unlikely (!buffer->output_glyph (glyphs[i]))) החזר שקר;
    }
    החזר אמת;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph)
  {
    אם (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    אם (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    אם (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    החזר buffer->replace_glyph (glyph);
  }

  HB_NODISCARD منطقي delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    אם (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    החזר buffer->replace_glyph (DELETED_GLYPH);
  }

  فراغ replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    אם (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    אם (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    אם (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> بنية Lookup;

template <typename T>
بنية LookupFormat0
{
  friend بنية Lookup<T>;

  خاص:
  ثابت T* get_value (hb_codepoint_t glyph_id, unsigned صحيح num_glyphs) ثابت
  {
    אם (unlikely (glyph_id >= num_glyphs)) החזר nullptr;
    החזר &arrayZ[glyph_id];
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs, unsigned num_glyphs) ثابت
  {
    glyphs.add_range (0, num_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, unsigned num_glyphs, ثابت filter_t &filter) ثابت
  {
    עבור (unsigned i = 0; i < num_glyphs; i++)
      אם (filter (arrayZ[i]))
	glyphs.add (i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_num_glyphs ()));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_num_glyphs (), base));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  عام:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
بنية LookupSegmentSingle
{
  ساكن constexpr unsigned TerminationWordCount = 2u;

  صحيح cmp (hb_codepoint_t g) ثابت
  { החזר g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    אם (first == DELETED_GLYPH) החזר;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    אם (first == DELETED_GLYPH) החזר;
    אם (!filter (value)) החזר;
    glyphs.add_range (first, last);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
بنية LookupFormat2
{
  friend بنية Lookup<T>;

  خاص:
  ثابت T* get_value (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    החזר v ? &v->value : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned count = segments.get_length ();
    עבור (unsigned صحيح i = 0; i < count; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned count = segments.get_length ();
    עבור (unsigned صحيح i = 0; i < count; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, base));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 */
  VarSizedBinSearchArrayOf<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
بنية LookupSegmentArray
{
  ساكن constexpr unsigned TerminationWordCount = 2u;

  ثابت T* get_value (hb_codepoint_t glyph_id, ثابت فراغ *base) ثابت
  {
    החזר first <= glyph_id && glyph_id <= last ? &(base+valuesZ)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    אם (first == DELETED_GLYPH) החזר;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت فراغ *base, ثابت filter_t &filter) ثابت
  {
    אם (first == DELETED_GLYPH) החזר;
    ثابت auto &values = base+valuesZ;
    עבור (hb_codepoint_t i = first; i <= last; i++)
      אם (filter (values[i - first]))
	glyphs.add (i);
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { החזר g < first ? -1 : g <= last ? 0 : +1; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  first <= last &&
		  valuesZ.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  first <= last &&
		  valuesZ.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		valuesZ;	/* A 16-bit offset from the start of
				 * the table to the data. */
  عام:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
بنية LookupFormat4
{
  friend بنية Lookup<T>;

  خاص:
  ثابت T* get_value (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    החזר v ? v->get_value (glyph_id, هذا) : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned count = segments.get_length ();
    עבור (unsigned i = 0; i < count; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned count = segments.get_length ();
    עבור (unsigned i = 0; i < count; i++)
      segments[i].collect_glyphs_filtered (glyphs, هذا, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا, base));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 4 */
  VarSizedBinSearchArrayOf<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
بنية LookupSingle
{
  ساكن constexpr unsigned TerminationWordCount = 1u;

  صحيح cmp (hb_codepoint_t g) ثابت { החזר glyph.cmp (g); }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    אם (glyph == DELETED_GLYPH) החזר;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    אם (glyph == DELETED_GLYPH) החזר;
    אם (!filter (value)) החזר;
    glyphs.add (glyph);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
بنية LookupFormat6
{
  friend بنية Lookup<T>;

  خاص:
  ثابت T* get_value (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSingle<T> *v = entries.bsearch (glyph_id);
    החזר v ? &v->value : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned count = entries.get_length ();
    עבור (unsigned i = 0; i < count; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned count = entries.get_length ();
    עבור (unsigned i = 0; i < count; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c, base));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 6 */
  VarSizedBinSearchArrayOf<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  عام:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
بنية LookupFormat8
{
  friend بنية Lookup<T>;

  خاص:
  ثابت T* get_value (hb_codepoint_t glyph_id) ثابت
  {
    החזר firstGlyph <= glyph_id && glyph_id - firstGlyph < glyphCount ?
	   &valueArrayZ[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    אם (unlikely (!glyphCount)) החזר;
    אם (firstGlyph == DELETED_GLYPH) החזר;
    glyphs.add_range (firstGlyph, firstGlyph + glyphCount - 1);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    אם (unlikely (!glyphCount)) החזר;
    אם (firstGlyph == DELETED_GLYPH) החזר;
    ثابت T *p = valueArrayZ.arrayZ;
    עבור (unsigned i = 0; i < glyphCount; i++)
      אם (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && valueArrayZ.sanitize (c, glyphCount));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && valueArrayZ.sanitize (c, glyphCount, base));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyphCount;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		valueArrayZ;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_SIZE_ARRAY (6, valueArrayZ);
};

template <typename T>
بنية LookupFormat10
{
  friend بنية Lookup<T>;

  خاص:
  ثابت typename T::type get_value_or_null (hb_codepoint_t glyph_id) ثابت
  {
    אם (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyphCount))
      החזר Null (T);

    ثابت HBUINT8 *p = &valueArrayZ[(glyph_id - firstGlyph) * valueSize];

    unsigned صحيح v = 0;
    unsigned صحيح count = valueSize;
    עבור (unsigned صحيح i = 0; i < count; i++)
      v = (v << 8) | *p++;

    החזר v;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    אם (unlikely (!glyphCount)) החזר;
    אם (firstGlyph == DELETED_GLYPH) החזר;
    glyphs.add_range (firstGlyph, firstGlyph + glyphCount - 1);
  }

  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    אם (unlikely (!glyphCount)) החזר;
    אם (firstGlyph == DELETED_GLYPH) החזר;
    ثابت HBUINT8 *p = valueArrayZ.arrayZ;
    עבור (unsigned i = 0; i < glyphCount; i++)
    {
      unsigned صحيح v = 0;
      unsigned صحيح count = valueSize;
      עבור (unsigned صحيح j = 0; j < count; j++)
	v = (v << 8) | *p++;
      אם (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  valueSize <= 4 &&
		  valueArrayZ.sanitize (c, glyphCount * valueSize));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	valueSize;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyphCount;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		valueArrayZ;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_SIZE_ARRAY (8, valueArrayZ);
};

template <typename T>
بنية Lookup
{
  ثابت T* get_value (hb_codepoint_t glyph_id, unsigned صحيح num_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); החזר u.format0.get_value (glyph_id, num_glyphs);
    حالة 2: hb_barrier (); החזר u.format2.get_value (glyph_id);
    حالة 4: hb_barrier (); החזר u.format4.get_value (glyph_id);
    حالة 6: hb_barrier (); החזר u.format6.get_value (glyph_id);
    حالة 8: hb_barrier (); החזר u.format8.get_value (glyph_id);
    افتراضي:החזר nullptr;
    }
  }

  ثابت typename T::type get_value_or_null (hb_codepoint_t glyph_id, unsigned صحيح num_glyphs) ثابت
  {
    بدل (u.format.v) {
      /* Format 10 cannot return a pointer. */
      حالة 10: hb_barrier (); החזר u.format10.get_value_or_null (glyph_id);
      افتراضي:
      ثابت T *v = get_value (glyph_id, num_glyphs);
      החזר v ? *v : Null (T);
    }
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs, unsigned صحيح num_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); u.format0.collect_glyphs (glyphs, num_glyphs); החזר;
    حالة 2: hb_barrier (); u.format2.collect_glyphs (glyphs); החזר;
    حالة 4: hb_barrier (); u.format4.collect_glyphs (glyphs); החזר;
    حالة 6: hb_barrier (); u.format6.collect_glyphs (glyphs); החזר;
    حالة 8: hb_barrier (); u.format8.collect_glyphs (glyphs); החזר;
    حالة 10: hb_barrier (); u.format10.collect_glyphs (glyphs); החזר;
    افتراضي:החזר;
    }
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, unsigned num_glyphs, ثابت filter_t &filter) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, num_glyphs, filter); החזר;
    حالة 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); החזר;
    حالة 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); החזר;
    حالة 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); החזר;
    حالة 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); החזר;
    حالة 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); החזר;
    افتراضي:החזר;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned صحيح num_glyphs,
			      unsigned صحيح outOfRange) ثابت
  {
    ثابت T *v = get_value (glyph_id, num_glyphs);
    החזר v ? *v : outOfRange;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    אם (!u.format.v.sanitize (c)) return_trace (שקר);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    حالة 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    حالة 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    حالة 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    افتراضي:return_trace (אמת);
    }
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    אם (!u.format.v.sanitize (c)) return_trace (שקר);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    حالة 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    حالة 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    حالة 10: return_trace (שקר); /* We don't support format10 here currently. */
    افتراضي:return_trace (אמת);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
بنية Entry
{
  // This doesn't seem like it's ever called.
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  عام:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
بنية Entry<فراغ>
{
  // This does seem like it's ever called.
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  عام:
  DEFINE_SIZE_STATIC (4);
};

تعداد Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_LINE = 3,
};

template <typename Types, typename Extra>
بنية StateTable
{
  تعريفنوع typename Types::HBUINT HBUINT;
  تعريفنوع typename Types::HBUSHORT HBUSHORT;
  تعريفنوع typename Types::ClassTypeNarrow ClassType;

  تعداد State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_LINE = 1,
  };

  template <typename set_t, typename table_t>
  فراغ collect_initial_glyphs (set_t &glyphs, unsigned num_glyphs, ثابت table_t &table) ثابت
  {
    unsigned num_classes = nClasses;

    אם (unlikely (num_classes > hb_bit_page_t::BITS))
    {
      (هذا+classTable).collect_glyphs (glyphs, num_glyphs);
      החזר;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    עבור (unsigned i = 0; i < num_classes; i++)
    {
      ثابت auto &entry = get_entry (STATE_START_OF_TEXT, i);
      אם (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	استمر;

      filter.add (i);
    }

    // And glyphs in those classes.

    אם (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (هذا+classTable).collect_glyphs_filtered (glyphs, num_glyphs, filter);
  }

  صحيح new_state (unsigned صحيح newState) ثابت
  { החזר Types::extended ? newState : ((صحيح) newState - (صحيح) stateArrayTable) / (صحيح) nClasses; }

  unsigned صحيح get_class (hb_codepoint_t glyph_id,
			  unsigned صحيح num_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) ثابت
  {
    unsigned klass;
    אם (cache && cache->get (glyph_id, &klass)) החזר klass;
    אם (unlikely (glyph_id == DELETED_GLYPH)) החזר CLASS_DELETED_GLYPH;
    klass = (هذا+classTable).get_class (glyph_id, num_glyphs, CLASS_OUT_OF_BOUNDS);
    אם (cache) cache->set (glyph_id, klass);
    החזר klass;
  }

  ثابت Entry<Extra> *get_entries () ثابت
  { החזר (هذا+entryTable).arrayZ; }

  ثابت Entry<Extra> &get_entry (صحيح state, unsigned صحيح klass) ثابت
  {
    unsigned n_classes = nClasses;
    אם (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    ثابت HBUSHORT *states = (هذا+stateArrayTable).arrayZ;
    ثابت Entry<Extra> *entries = (هذا+entryTable).arrayZ;

    unsigned صحيح entry = states[state * n_classes + klass];
    DEBUG_MSG (APPLY, nullptr, "e%u", entry);

    החזר entries[entry];
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 unsigned صحيح *num_entries_out = nullptr) ثابت
  {
    TRACE_SANITIZE (هذا);
    אם (unlikely (!(c->check_struct (هذا) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, هذا)))) return_trace (שקר);

    ثابت HBUSHORT *states = (هذا+stateArrayTable).arrayZ;
    ثابت Entry<Extra> *entries = (هذا+entryTable).arrayZ;

    unsigned صحيح num_classes = nClasses;
    אם (unlikely (hb_unsigned_mul_overflows (num_classes, states[0].static_size)))
      return_trace (שקר);
    unsigned صحيح row_stride = num_classes * states[0].static_size;

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

    صحيح min_state = 0;
    صحيح max_state = 0;
    unsigned صحيح num_entries = 0;

    صحيح state_pos = 0;
    صحيح state_neg = 0;
    unsigned صحيح entry = 0;
    כלעוד (min_state < state_neg || state_pos <= max_state)
    {
      אם (min_state < state_neg)
      {
	/* Negative states. */
	אם (unlikely (hb_unsigned_mul_overflows (min_state, num_classes)))
	  return_trace (שקר);
	אם (unlikely (!c->check_range (&states[min_state * num_classes],
				       -min_state,
				       row_stride)))
	  return_trace (שקר);
	אם ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (שקר);
	{ /* Sweep new states. */
	  ثابت HBUSHORT *stop = &states[min_state * num_classes];
	  אם (unlikely (stop > states))
	    return_trace (שקר);
	  עבור (ثابت HBUSHORT *p = states; stop < p; p--)
	    num_entries = hb_max (num_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      אם (state_pos <= max_state)
      {
	/* Positive states. */
	אם (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (שקר);
	אם ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (שקר);
	{ /* Sweep new states. */
	  אם (unlikely (hb_unsigned_mul_overflows ((max_state + 1), num_classes)))
	    return_trace (שקר);
	  ثابت HBUSHORT *stop = &states[(max_state + 1) * num_classes];
	  אם (unlikely (stop < states))
	    return_trace (שקר);
	  עבור (ثابت HBUSHORT *p = &states[state_pos * num_classes]; p < stop; p++)
	    num_entries = hb_max (num_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      אם (unlikely (!c->check_array (entries, num_entries)))
	return_trace (שקר);
      אם ((c->max_ops -= num_entries - entry) <= 0)
	return_trace (שקר);
      { /* Sweep new entries. */
	ثابت Entry<Extra> *stop = &entries[num_entries];
	עבור (ثابت Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  صحيح newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = num_entries;
      }
    }

    אם (num_entries_out)
      *num_entries_out = num_entries;

    return_trace (אמת);
  }

  محمي:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  عام:
  DEFINE_SIZE_STATIC (4 * حجم (HBUINT));
};

template <typename HBUCHAR>
بنية ClassTable
{
  unsigned صحيح get_class (hb_codepoint_t glyph_id, unsigned صحيح outOfRange) ثابت
  {
    unsigned صحيح i = glyph_id - firstGlyph;
    החזר i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned صحيح get_class (hb_codepoint_t glyph_id,
			  unsigned صحيح num_glyphs HB_UNUSED,
			  unsigned صحيح outOfRange) ثابت
  {
    החזר get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs, unsigned num_glyphs) ثابت
  {
    עבור (unsigned i = 0; i < classArray.len; i++)
      אם (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, unsigned num_glyphs, ثابت filter_t &filter) ثابت
  {
    עבור (unsigned i = 0; i < classArray.len; i++)
      אם (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && classArray.sanitize (c));
  }
  محمي:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  عام:
  DEFINE_SIZE_ARRAY (4, classArray);
};

بنية SubtableGlyphCoverage
{
  منطقي sanitize (hb_sanitize_context_t *c, unsigned subtable_count) ثابت
  {
    TRACE_SANITIZE (هذا);

    אם (unlikely (!c->check_array (&subtableOffsets, subtable_count)))
      return_trace (שקר);

    unsigned bytes = (c->get_num_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    עבור (unsigned i = 0; i < subtable_count; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      אם (offset == 0 || offset == 0xFFFFFFFF)
        استمر;
      אם (unlikely (!subtableOffsets[i].sanitize (c, هذا, bytes)))
        return_trace (שקר);
    }

    return_trace (אמת);
  }
  محمي:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  عام:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

بنية ObsoleteTypes
{
  ساكن constexpr منطقي extended = שקר;
  تعريفنوع HBUINT16 HBUINT;
  تعريفنوع HBUINT8 HBUSHORT;
  تعريفنوع ClassTable<HBUINT8> ClassTypeNarrow;
  تعريفنوع ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  ساكن صحيح offsetToIndex (int64_t offset,
			    ثابت فراغ *base,
			    ثابت T *array)
  {
    int64_t array_offset = (ثابت حرف *) array - (ثابت حرف *) base;
    صحيح bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    אם (unlikely (offset < array_offset))
      החזר bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    אם (unlikely (index > bad_index))
      החזר bad_index;
    החזר index;
  }
  template <typename T>
  ساكن صحيح byteOffsetToIndex (int64_t offset,
				ثابت فراغ *base,
				ثابت T *array)
  {
    החזר offsetToIndex (offset, base, array);
  }
  template <typename T>
  ساكن صحيح wordOffsetToIndex (int64_t offset,
				ثابت فراغ *base,
				ثابت T *array)
  {
    החזר offsetToIndex (2 * offset, base, array);
  }
};
بنية ExtendedTypes
{
  ساكن constexpr منطقي extended = אמת;
  تعريفنوع HBUINT32 HBUINT;
  تعريفنوع HBUINT16 HBUSHORT;
  تعريفنوع Lookup<HBUINT16> ClassTypeNarrow;
  تعريفنوع Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  ساكن unsigned صحيح offsetToIndex (unsigned صحيح offset,
				     ثابت فراغ *base HB_UNUSED,
				     ثابت T *array HB_UNUSED)
  {
    החזר offset;
  }
  template <typename T>
  ساكن unsigned صحيح byteOffsetToIndex (unsigned صحيح offset,
					 ثابت فراغ *base HB_UNUSED,
					 ثابت T *array HB_UNUSED)
  {
    החזר offset / 2;
  }
  template <typename T>
  ساكن unsigned صحيح wordOffsetToIndex (unsigned صحيح offset,
					 ثابت فراغ *base HB_UNUSED,
					 ثابت T *array HB_UNUSED)
  {
    החזר offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
بنية StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (ثابت StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      num_glyphs (face_->get_num_glyphs ()) {}

  template <typename context_t>
  فراغ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    אם (!c->in_place)
      buffer->clear_output ();

    صحيح state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    ثابت منطقي start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    עבור (buffer->idx = 0; buffer->successful;)
    {
      unsigned صحيح klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, num_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_MSG (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      ثابت EntryT &entry = machine.get_entry (state, klass);
      ثابت صحيح next_state = machine.new_state (entry.newState);

      منطقي is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      منطقي is_not_actionable = !c->table->is_actionable (entry);

      אם (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	אם (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  כלעוד (cluster < range->cluster_first)
	    range--;
	  כלעוד (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	אם (!(range->flags & ac->subtable_flags))
	{
	  אם (buffer->idx == buffer->len)
	    اكسر;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (فراغ) buffer->next_glyph ();
	  استمر;
	}
      }
      אחרת
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	منطقي is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	אם (is_null_transition)
	{
	  unsigned old_klass = klass;
	  עשה
	  {
	    c->transition (buffer, هذا, entry);

	    אם (buffer->idx == buffer->len || !buffer->successful)
	      اكسر;

	    (فراغ) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, num_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } כלעוד (klass == old_klass);

	  אם (buffer->idx == buffer->len || !buffer->successful)
	    اكسر;

	  اذهب resume;
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
      ثابت EntryT *wouldbe_entry;
      منطقي is_safe_to_break =
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

      אם (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, هذا, entry);

      state = next_state;
      DEBUG_MSG (APPLY, nullptr, "s%d", state);

      אם (buffer->idx == buffer->len)
	اكسر;

      אם (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(فراغ) buffer->next_glyph ();
    }

    אם (!c->in_place)
      buffer->متزامن ();
  }

  عام:
  ثابت StateTableT &machine;
  unsigned صحيح num_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
