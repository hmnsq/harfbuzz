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

#شامل "hb-aat-layout.hh"
#شامل "hb-aat-map.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-open-type.hh"
#شامل "hb-cache.hh"
#شامل "hb-bit-set.hh"
#شامل "hb-bit-page.hh"


فضاینام OT {
ساختار GDEF;
};

فضاینام AAT {

using فضاینام OT;

ساختار ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#وگرنه
#define HB_AAT_TRANSITION_INLINE
#endif

ساختار hb_aat_scratch_t
{
  hb_aat_scratch_t () = پیشفرض;
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
    بازگشت *این;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    اگر (unlikely (!s))
      بازگشت;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () ثابت
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    اگر (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      بازگشت s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, اندازه (hb_bit_set_t));
    اگر (unlikely (!s))
      بازگشت nullptr;
    s->init ();

    بازگشت s;
  }
  تهی destroy_buffer_glyph_set (hb_bit_set_t *s) ثابت
  {
    اگر (unlikely (!s))
      بازگشت;
    اگر (buffer_glyph_set.cmpexch (nullptr, s))
      بازگشت;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

شمارش { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

ساختار hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, منطقی, HB_DEBUG_APPLY>
{
  ثابت نویسه *get_name () { بازگشت "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { بازگشت obj.apply (این, std::forward<Ts> (ds)...); }
  ایستا return_t default_return_مقدار () { بازگشت نادرست; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { بازگشت r; }

  ثابت hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  ثابت ankr *ankr_table;
  ثابت OT::GDEF &gdef;
  منطقی has_glyph_classes;
  ثابت hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  منطقی buffer_is_reversed = نادرست;
  // Caches
  منطقی using_buffer_glyph_set = نادرست;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  ثابت hb_bit_set_t *first_set = nullptr;
  ثابت hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned صحیح lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (ثابت hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL تهی set_ankr_table (ثابت AAT::ankr *ankr_table_);

  تهی set_lookup_index (unsigned صحیح i) { lookup_index = i; }

  تهی reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  تهی setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    اگر (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  منطقی buffer_intersects_machine () ثابت
  {
    اگر (likely (using_buffer_glyph_set))
      بازگشت buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    برای (unsigned i = 0; i < buffer->len; i++)
      اگر (first_set->has (buffer->info[i].codepoint))
	بازگشت درست;
    بازگشت نادرست;
  }

  template <typename T>
  HB_NODISCARD منطقی output_glyphs (unsigned صحیح شمار,
				   ثابت T *glyphs)
  {
    اگر (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, شمار);
    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      منطقی at_end_of_text = buffer->idx == buffer->len;
      اگر (at_end_of_text)
	اگر (unlikely (!buffer->output_glyph (glyphs[i]))) بازگشت نادرست;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      اگر (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      وگرنه
      {
#ifndef HB_NO_OT_LAYOUT
	اگر (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      اگر (!at_end_of_text)
	اگر (unlikely (!buffer->output_glyph (glyphs[i]))) بازگشت نادرست;
    }
    بازگشت درست;
  }

  HB_NODISCARD منطقی replace_glyph (hb_codepoint_t glyph)
  {
    اگر (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    اگر (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    اگر (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    بازگشت buffer->replace_glyph (glyph);
  }

  HB_NODISCARD منطقی delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    اگر (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    بازگشت buffer->replace_glyph (DELETED_GLYPH);
  }

  تهی replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    اگر (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    اگر (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    اگر (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> ساختار Lookup;

template <typename T>
ساختار LookupFormat0
{
  friend ساختار Lookup<T>;

  خصوصی:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id, unsigned صحیح رقم_glyphs) ثابت
  {
    اگر (unlikely (glyph_id >= رقم_glyphs)) بازگشت nullptr;
    بازگشت &arrayZ[glyph_id];
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs, unsigned رقم_glyphs) ثابت
  {
    glyphs.add_range (0, رقم_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, unsigned رقم_glyphs, ثابت filter_t &filter) ثابت
  {
    برای (unsigned i = 0; i < رقم_glyphs; i++)
      اگر (filter (arrayZ[i]))
	glyphs.add (i);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (arrayZ.sanitize (c, c->get_رقم_glyphs ()));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (arrayZ.sanitize (c, c->get_رقم_glyphs (), base));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  عمومی:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
ساختار LookupSegmentSingle
{
  ایستا constexpr unsigned Termination_Word_شمار = 2u;

  صحیح cmp (hb_codepoint_t g) ثابت
  { بازگشت g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs) ثابت
  {
    اگر (first == DELETED_GLYPH) بازگشت;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    اگر (first == DELETED_GLYPH) بازگشت;
    اگر (!filter (value)) بازگشت;
    glyphs.add_range (first, last);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && value.sanitize (c));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  عمومی:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
ساختار LookupFormat2
{
  friend ساختار Lookup<T>;

  خصوصی:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    بازگشت v ? &v->value : nullptr;
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned شمار = segments.get_length ();
    برای (unsigned صحیح i = 0; i < شمار; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned شمار = segments.get_length ();
    برای (unsigned صحیح i = 0; i < شمار; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (segments.sanitize (c));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (segments.sanitize (c, base));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_جستجو_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عمومی:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
ساختار LookupSegmentArray
{
  ایستا constexpr unsigned Termination_Word_شمار = 2u;

  ثابت T* get_مقدار (hb_codepoint_t glyph_id, ثابت تهی *base) ثابت
  {
    بازگشت first <= glyph_id && glyph_id <= last ? &(base+مقادیر_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs) ثابت
  {
    اگر (first == DELETED_GLYPH) بازگشت;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, ثابت تهی *base, ثابت filter_t &filter) ثابت
  {
    اگر (first == DELETED_GLYPH) بازگشت;
    ثابت auto &values = base+مقادیر_Z;
    برای (hb_codepoint_t i = first; i <= last; i++)
      اگر (filter (values[i - first]))
	glyphs.add (i);
  }

  صحیح cmp (hb_codepoint_t g) ثابت
  { بازگشت g < first ? -1 : g <= last ? 0 : +1; }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  first <= last &&
		  مقادیر_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  first <= last &&
		  مقادیر_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		مقادیر_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  عمومی:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
ساختار LookupFormat4
{
  friend ساختار Lookup<T>;

  خصوصی:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    بازگشت v ? v->get_مقدار (glyph_id, این) : nullptr;
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned شمار = segments.get_length ();
    برای (unsigned i = 0; i < شمار; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned شمار = segments.get_length ();
    برای (unsigned i = 0; i < شمار; i++)
      segments[i].collect_glyphs_filtered (glyphs, این, filter);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (segments.sanitize (c, این));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (segments.sanitize (c, این, base));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_جستجو_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عمومی:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
ساختار LookupSingle
{
  ایستا constexpr unsigned Termination_Word_شمار = 1u;

  صحیح cmp (hb_codepoint_t g) ثابت { بازگشت glyph.cmp (g); }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs) ثابت
  {
    اگر (glyph == DELETED_GLYPH) بازگشت;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    اگر (glyph == DELETED_GLYPH) بازگشت;
    اگر (!filter (value)) بازگشت;
    glyphs.add (glyph);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && value.sanitize (c));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  عمومی:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
ساختار LookupFormat6
{
  friend ساختار Lookup<T>;

  خصوصی:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSingle<T> *v = entries.bsearch (glyph_id);
    بازگشت v ? &v->value : nullptr;
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned شمار = entries.get_length ();
    برای (unsigned i = 0; i < شمار; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned شمار = entries.get_length ();
    برای (unsigned i = 0; i < شمار; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (entries.sanitize (c));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (entries.sanitize (c, base));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_جستجو_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  عمومی:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
ساختار LookupFormat8
{
  friend ساختار Lookup<T>;

  خصوصی:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id) ثابت
  {
    بازگشت firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_شمار_2 ?
	   &مقدار_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs) ثابت
  {
    اگر (unlikely (!glyph_شمار_2)) بازگشت;
    اگر (firstGlyph == DELETED_GLYPH) بازگشت;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_شمار_2 - 1);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    اگر (unlikely (!glyph_شمار_2)) بازگشت;
    اگر (firstGlyph == DELETED_GLYPH) بازگشت;
    ثابت T *p = مقدار_Array_Z.arrayZ;
    برای (unsigned i = 0; i < glyph_شمار_2; i++)
      اگر (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && مقدار_Array_Z.sanitize (c, glyph_شمار_2));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && مقدار_Array_Z.sanitize (c, glyph_شمار_2, base));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_شمار_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		مقدار_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عمومی:
  DEFINE_SIZE_ARRAY (6, مقدار_Array_Z);
};

template <typename T>
ساختار LookupFormat10
{
  friend ساختار Lookup<T>;

  خصوصی:
  ثابت typename T::type get_مقدار_or_null (hb_codepoint_t glyph_id) ثابت
  {
    اگر (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_شمار_2))
      بازگشت Null (T);

    ثابت HBUINT8 *p = &مقدار_Array_Z[(glyph_id - firstGlyph) * مقدار_Size];

    unsigned صحیح v = 0;
    unsigned صحیح شمار = مقدار_Size;
    برای (unsigned صحیح i = 0; i < شمار; i++)
      v = (v << 8) | *p++;

    بازگشت v;
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs) ثابت
  {
    اگر (unlikely (!glyph_شمار_2)) بازگشت;
    اگر (firstGlyph == DELETED_GLYPH) بازگشت;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_شمار_2 - 1);
  }

  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    اگر (unlikely (!glyph_شمار_2)) بازگشت;
    اگر (firstGlyph == DELETED_GLYPH) بازگشت;
    ثابت HBUINT8 *p = مقدار_Array_Z.arrayZ;
    برای (unsigned i = 0; i < glyph_شمار_2; i++)
    {
      unsigned صحیح v = 0;
      unsigned صحیح شمار = مقدار_Size;
      برای (unsigned صحیح j = 0; j < شمار; j++)
	v = (v << 8) | *p++;
      اگر (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  مقدار_Size <= 4 &&
		  مقدار_Array_Z.sanitize (c, glyph_شمار_2 * مقدار_Size));
  }

  محافظتشده:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	مقدار_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_شمار_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		مقدار_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عمومی:
  DEFINE_SIZE_ARRAY (8, مقدار_Array_Z);
};

template <typename T>
ساختار Lookup
{
  ثابت T* get_مقدار (hb_codepoint_t glyph_id, unsigned صحیح رقم_glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); بازگشت u.format0.get_مقدار (glyph_id, رقم_glyphs);
    حالت 2: hb_barrier (); بازگشت u.format2.get_مقدار (glyph_id);
    حالت 4: hb_barrier (); بازگشت u.format4.get_مقدار (glyph_id);
    حالت 6: hb_barrier (); بازگشت u.format6.get_مقدار (glyph_id);
    حالت 8: hb_barrier (); بازگشت u.format8.get_مقدار (glyph_id);
    پیشفرض:بازگشت nullptr;
    }
  }

  ثابت typename T::type get_مقدار_or_null (hb_codepoint_t glyph_id, unsigned صحیح رقم_glyphs) ثابت
  {
    انتخاب (u.format.v) {
      /* Format 10 cannot return a pointer. */
      حالت 10: hb_barrier (); بازگشت u.format10.get_مقدار_or_null (glyph_id);
      پیشفرض:
      ثابت T *v = get_مقدار (glyph_id, رقم_glyphs);
      بازگشت v ? *v : Null (T);
    }
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs, unsigned صحیح رقم_glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); u.format0.collect_glyphs (glyphs, رقم_glyphs); بازگشت;
    حالت 2: hb_barrier (); u.format2.collect_glyphs (glyphs); بازگشت;
    حالت 4: hb_barrier (); u.format4.collect_glyphs (glyphs); بازگشت;
    حالت 6: hb_barrier (); u.format6.collect_glyphs (glyphs); بازگشت;
    حالت 8: hb_barrier (); u.format8.collect_glyphs (glyphs); بازگشت;
    حالت 10: hb_barrier (); u.format10.collect_glyphs (glyphs); بازگشت;
    پیشفرض:بازگشت;
    }
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, unsigned رقم_glyphs, ثابت filter_t &filter) ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, رقم_glyphs, filter); بازگشت;
    حالت 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); بازگشت;
    حالت 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); بازگشت;
    حالت 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); بازگشت;
    حالت 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); بازگشت;
    حالت 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); بازگشت;
    پیشفرض:بازگشت;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned صحیح رقم_glyphs,
			      unsigned صحیح outOfRange) ثابت
  {
    ثابت T *v = get_مقدار (glyph_id, رقم_glyphs);
    بازگشت v ? *v : outOfRange;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.format.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    حالت 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    حالت 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    حالت 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    پیشفرض:return_trace (درست);
    }
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت تهی *base) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (!u.format.v.sanitize (c)) return_trace (نادرست);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    حالت 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    حالت 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    حالت 10: return_trace (نادرست); /* We don't support format10 here currently. */
    پیشفرض:return_trace (درست);
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  عمومی:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
ساختار Entry
{
  // This doesn't seem like it's ever called.
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (این));
  }

  عمومی:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  عمومی:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
ساختار Entry<تهی>
{
  // This does seem like it's ever called.
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  عمومی:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  عمومی:
  DEFINE_SIZE_STATIC (4);
};

شمارش Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_سطر = 3,
};

template <typename Types, typename Extra>
ساختار StateTable
{
  تعریفنوع typename Types::HBUINT HBUINT;
  تعریفنوع typename Types::HBUSHORT HBUSHORT;
  تعریفنوع typename Types::ClassTypeNarrow ClassType;

  شمارش State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_سطر = 1,
  };

  template <typename set_t, typename table_t>
  تهی collect_initial_glyphs (set_t &glyphs, unsigned رقم_glyphs, ثابت table_t &table) ثابت
  {
    unsigned رقم_classes = nClasses;

    اگر (unlikely (رقم_classes > hb_bit_page_t::BITS))
    {
      (این+classTable).collect_glyphs (glyphs, رقم_glyphs);
      بازگشت;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    برای (unsigned i = 0; i < رقم_classes; i++)
    {
      ثابت auto &entry = get_entry (STATE_START_OF_TEXT, i);
      اگر (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	ادامه;

      filter.add (i);
    }

    // And glyphs in those classes.

    اگر (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (این+classTable).collect_glyphs_filtered (glyphs, رقم_glyphs, filter);
  }

  صحیح new_state (unsigned صحیح newState) ثابت
  { بازگشت Types::extended ? newState : ((صحیح) newState - (صحیح) stateArrayTable) / (صحیح) nClasses; }

  unsigned صحیح get_class (hb_codepoint_t glyph_id,
			  unsigned صحیح رقم_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) ثابت
  {
    unsigned klass;
    اگر (cache && cache->get (glyph_id, &klass)) بازگشت klass;
    اگر (unlikely (glyph_id == DELETED_GLYPH)) بازگشت CLASS_DELETED_GLYPH;
    klass = (این+classTable).get_class (glyph_id, رقم_glyphs, CLASS_OUT_OF_BOUNDS);
    اگر (cache) cache->set (glyph_id, klass);
    بازگشت klass;
  }

  ثابت Entry<Extra> *get_entries () ثابت
  { بازگشت (این+entryTable).arrayZ; }

  ثابت Entry<Extra> &get_entry (صحیح state, unsigned صحیح klass) ثابت
  {
    unsigned n_classes = nClasses;
    اگر (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    ثابت HBUSHORT *states = (این+stateArrayTable).arrayZ;
    ثابت Entry<Extra> *entries = (این+entryTable).arrayZ;

    unsigned صحیح entry = states[state * n_classes + klass];
    DEBUG_پیام (APPLY, nullptr, "e%u", entry);

    بازگشت entries[entry];
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 unsigned صحیح *رقم_entries_out = nullptr) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!(c->check_struct (این) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, این)))) return_trace (نادرست);

    ثابت HBUSHORT *states = (این+stateArrayTable).arrayZ;
    ثابت Entry<Extra> *entries = (این+entryTable).arrayZ;

    unsigned صحیح رقم_classes = nClasses;
    اگر (unlikely (hb_unsigned_mul_overflows (رقم_classes, states[0].static_size)))
      return_trace (نادرست);
    unsigned صحیح row_stride = رقم_classes * states[0].static_size;

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

    صحیح min_state = 0;
    صحیح max_state = 0;
    unsigned صحیح رقم_entries = 0;

    صحیح state_pos = 0;
    صحیح state_neg = 0;
    unsigned صحیح entry = 0;
    وقتی (min_state < state_neg || state_pos <= max_state)
    {
      اگر (min_state < state_neg)
      {
	/* Negative states. */
	اگر (unlikely (hb_unsigned_mul_overflows (min_state, رقم_classes)))
	  return_trace (نادرست);
	اگر (unlikely (!c->check_range (&states[min_state * رقم_classes],
				       -min_state,
				       row_stride)))
	  return_trace (نادرست);
	اگر ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (نادرست);
	{ /* Sweep new states. */
	  ثابت HBUSHORT *stop = &states[min_state * رقم_classes];
	  اگر (unlikely (stop > states))
	    return_trace (نادرست);
	  برای (ثابت HBUSHORT *p = states; stop < p; p--)
	    رقم_entries = hb_max (رقم_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      اگر (state_pos <= max_state)
      {
	/* Positive states. */
	اگر (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (نادرست);
	اگر ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (نادرست);
	{ /* Sweep new states. */
	  اگر (unlikely (hb_unsigned_mul_overflows ((max_state + 1), رقم_classes)))
	    return_trace (نادرست);
	  ثابت HBUSHORT *stop = &states[(max_state + 1) * رقم_classes];
	  اگر (unlikely (stop < states))
	    return_trace (نادرست);
	  برای (ثابت HBUSHORT *p = &states[state_pos * رقم_classes]; p < stop; p++)
	    رقم_entries = hb_max (رقم_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      اگر (unlikely (!c->check_array (entries, رقم_entries)))
	return_trace (نادرست);
      اگر ((c->max_ops -= رقم_entries - entry) <= 0)
	return_trace (نادرست);
      { /* Sweep new entries. */
	ثابت Entry<Extra> *stop = &entries[رقم_entries];
	برای (ثابت Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  صحیح newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = رقم_entries;
      }
    }

    اگر (رقم_entries_out)
      *رقم_entries_out = رقم_entries;

    return_trace (درست);
  }

  محافظتشده:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  عمومی:
  DEFINE_SIZE_STATIC (4 * اندازه (HBUINT));
};

template <typename HBUCHAR>
ساختار ClassTable
{
  unsigned صحیح get_class (hb_codepoint_t glyph_id, unsigned صحیح outOfRange) ثابت
  {
    unsigned صحیح i = glyph_id - firstGlyph;
    بازگشت i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned صحیح get_class (hb_codepoint_t glyph_id,
			  unsigned صحیح رقم_glyphs HB_UNUSED,
			  unsigned صحیح outOfRange) ثابت
  {
    بازگشت get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  تهی collect_glyphs (set_t &glyphs, unsigned رقم_glyphs) ثابت
  {
    برای (unsigned i = 0; i < classArray.len; i++)
      اگر (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  تهی collect_glyphs_filtered (set_t &glyphs, unsigned رقم_glyphs, ثابت filter_t &filter) ثابت
  {
    برای (unsigned i = 0; i < classArray.len; i++)
      اگر (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) && classArray.sanitize (c));
  }
  محافظتشده:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  عمومی:
  DEFINE_SIZE_ARRAY (4, classArray);
};

ساختار SubtableGlyphCoverage
{
  منطقی sanitize (hb_sanitize_context_t *c, unsigned subtable_شمار) ثابت
  {
    TRACE_SANITIZE (این);

    اگر (unlikely (!c->check_array (&subtableOffsets, subtable_شمار)))
      return_trace (نادرست);

    unsigned bytes = (c->get_رقم_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    برای (unsigned i = 0; i < subtable_شمار; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      اگر (offset == 0 || offset == 0xFFFFFFFF)
        ادامه;
      اگر (unlikely (!subtableOffsets[i].sanitize (c, این, bytes)))
        return_trace (نادرست);
    }

    return_trace (درست);
  }
  محافظتشده:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  عمومی:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

ساختار ObsoleteTypes
{
  ایستا constexpr منطقی extended = نادرست;
  تعریفنوع HBUINT16 HBUINT;
  تعریفنوع HBUINT8 HBUSHORT;
  تعریفنوع ClassTable<HBUINT8> ClassTypeNarrow;
  تعریفنوع ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  ایستا صحیح offsetToIndex (int64_t offset,
			    ثابت تهی *base,
			    ثابت T *array)
  {
    int64_t array_offset = (ثابت نویسه *) array - (ثابت نویسه *) base;
    صحیح bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    اگر (unlikely (offset < array_offset))
      بازگشت bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    اگر (unlikely (index > bad_index))
      بازگشت bad_index;
    بازگشت index;
  }
  template <typename T>
  ایستا صحیح byteOffsetToIndex (int64_t offset,
				ثابت تهی *base,
				ثابت T *array)
  {
    بازگشت offsetToIndex (offset, base, array);
  }
  template <typename T>
  ایستا صحیح wordOffsetToIndex (int64_t offset,
				ثابت تهی *base,
				ثابت T *array)
  {
    بازگشت offsetToIndex (2 * offset, base, array);
  }
};
ساختار ExtendedTypes
{
  ایستا constexpr منطقی extended = درست;
  تعریفنوع HBUINT32 HBUINT;
  تعریفنوع HBUINT16 HBUSHORT;
  تعریفنوع Lookup<HBUINT16> ClassTypeNarrow;
  تعریفنوع Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  ایستا unsigned صحیح offsetToIndex (unsigned صحیح offset,
				     ثابت تهی *base HB_UNUSED,
				     ثابت T *array HB_UNUSED)
  {
    بازگشت offset;
  }
  template <typename T>
  ایستا unsigned صحیح byteOffsetToIndex (unsigned صحیح offset,
					 ثابت تهی *base HB_UNUSED,
					 ثابت T *array HB_UNUSED)
  {
    بازگشت offset / 2;
  }
  template <typename T>
  ایستا unsigned صحیح wordOffsetToIndex (unsigned صحیح offset,
					 ثابت تهی *base HB_UNUSED,
					 ثابت T *array HB_UNUSED)
  {
    بازگشت offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
ساختار StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (ثابت StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      رقم_glyphs (face_->get_رقم_glyphs ()) {}

  template <typename context_t>
  تهی drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    اگر (!c->in_place)
      buffer->clear_output ();

    صحیح state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    ثابت منطقی start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    برای (buffer->idx = 0; buffer->successful;)
    {
      unsigned صحیح klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, رقم_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_پیام (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      ثابت EntryT &entry = machine.get_entry (state, klass);
      ثابت صحیح next_state = machine.new_state (entry.newState);

      منطقی is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      منطقی is_not_actionable = !c->table->is_actionable (entry);

      اگر (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	اگر (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  وقتی (cluster < range->cluster_first)
	    range--;
	  وقتی (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	اگر (!(range->flags & ac->subtable_flags))
	{
	  اگر (buffer->idx == buffer->len)
	    بشکن;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (تهی) buffer->next_glyph ();
	  ادامه;
	}
      }
      وگرنه
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	منطقی is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	اگر (is_null_transition)
	{
	  unsigned old_klass = klass;
	  انجامبده
	  {
	    c->transition (buffer, این, entry);

	    اگر (buffer->idx == buffer->len || !buffer->successful)
	      بشکن;

	    (تهی) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, رقم_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } وقتی (klass == old_klass);

	  اگر (buffer->idx == buffer->len || !buffer->successful)
	    بشکن;

	  برو resume;
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
      منطقی is_safe_to_break =
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

      اگر (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, این, entry);

      state = next_state;
      DEBUG_پیام (APPLY, nullptr, "s%d", state);

      اگر (buffer->idx == buffer->len)
	بشکن;

      اگر (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(تهی) buffer->next_glyph ();
    }

    اگر (!c->in_place)
      buffer->همگام ();
  }

  عمومی:
  ثابت StateTableT &machine;
  unsigned صحیح رقم_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
