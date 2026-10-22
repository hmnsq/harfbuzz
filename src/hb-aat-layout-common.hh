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
#وإلا
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
    أرجع *هذا;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    إذا (unlikely (!s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () ثابت
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    إذا (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      أرجع s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, حجم (hb_bit_set_t));
    إذا (unlikely (!s))
      أرجع nullptr;
    s->init ();

    أرجع s;
  }
  فراغ destroy_buffer_glyph_set (hb_bit_set_t *s) ثابت
  {
    إذا (unlikely (!s))
      أرجع;
    إذا (buffer_glyph_set.cmpexch (nullptr, s))
      أرجع;
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
  ثابت حرف *get_name () { أرجع "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { أرجع obj.apply (هذا, std::forward<Ts> (ds)...); }
  ساكن return_t default_return_قيمة () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

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
  منطقي buffer_is_reversed = خطأ;
  // Caches
  منطقي using_buffer_glyph_set = خطأ;
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

    إذا (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  منطقي buffer_intersects_machine () ثابت
  {
    إذا (likely (using_buffer_glyph_set))
      أرجع buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    لكل (unsigned i = 0; i < buffer->len; i++)
      إذا (first_set->has (buffer->info[i].codepoint))
	أرجع true;
    أرجع خطأ;
  }

  template <typename T>
  HB_NODISCARD منطقي output_glyphs (unsigned صحيح عدد,
				   ثابت T *glyphs)
  {
    إذا (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, عدد);
    لكل (unsigned صحيح i = 0; i < عدد; i++)
    {
      منطقي at_end_of_text = buffer->idx == buffer->len;
      إذا (at_end_of_text)
	إذا (unlikely (!buffer->output_glyph (glyphs[i]))) أرجع خطأ;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      إذا (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      وإلا
      {
#ifndef HB_NO_OT_LAYOUT
	إذا (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      إذا (!at_end_of_text)
	إذا (unlikely (!buffer->output_glyph (glyphs[i]))) أرجع خطأ;
    }
    أرجع true;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph)
  {
    إذا (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    إذا (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    أرجع buffer->replace_glyph (glyph);
  }

  HB_NODISCARD منطقي delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    إذا (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    أرجع buffer->replace_glyph (DELETED_GLYPH);
  }

  فراغ replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    إذا (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    إذا (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
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
  ثابت T* get_قيمة (hb_codepoint_t glyph_id, unsigned صحيح رقم_glyphs) ثابت
  {
    إذا (unlikely (glyph_id >= رقم_glyphs)) أرجع nullptr;
    أرجع &arrayZ[glyph_id];
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs, unsigned رقم_glyphs) ثابت
  {
    glyphs.add_range (0, رقم_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, unsigned رقم_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (unsigned i = 0; i < رقم_glyphs; i++)
      إذا (filter (arrayZ[i]))
	glyphs.add (i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_رقم_glyphs ()));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_رقم_glyphs (), base));
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
  ساكن constexpr unsigned Termination_Word_عدد = 2u;

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    إذا (first == DELETED_GLYPH) أرجع;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (first == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
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
  ثابت T* get_قيمة (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    أرجع v ? &v->value : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned عدد = segments.get_length ();
    لكل (unsigned صحيح i = 0; i < عدد; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned عدد = segments.get_length ();
    لكل (unsigned صحيح i = 0; i < عدد; i++)
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
  Var_Sized_Bin_بحث_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
بنية LookupSegmentArray
{
  ساكن constexpr unsigned Termination_Word_عدد = 2u;

  ثابت T* get_قيمة (hb_codepoint_t glyph_id, ثابت فراغ *base) ثابت
  {
    أرجع first <= glyph_id && glyph_id <= last ? &(base+قيم_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    إذا (first == DELETED_GLYPH) أرجع;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت فراغ *base, ثابت filter_t &filter) ثابت
  {
    إذا (first == DELETED_GLYPH) أرجع;
    ثابت auto &values = base+قيم_Z;
    لكل (hb_codepoint_t i = first; i <= last; i++)
      إذا (filter (values[i - first]))
	glyphs.add (i);
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < first ? -1 : g <= last ? 0 : +1; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  first <= last &&
		  قيم_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  first <= last &&
		  قيم_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		قيم_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  عام:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
بنية LookupFormat4
{
  friend بنية Lookup<T>;

  خاص:
  ثابت T* get_قيمة (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    أرجع v ? v->get_قيمة (glyph_id, هذا) : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned عدد = segments.get_length ();
    لكل (unsigned i = 0; i < عدد; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned عدد = segments.get_length ();
    لكل (unsigned i = 0; i < عدد; i++)
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
  Var_Sized_Bin_بحث_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
بنية LookupSingle
{
  ساكن constexpr unsigned Termination_Word_عدد = 1u;

  صحيح cmp (hb_codepoint_t g) ثابت { أرجع glyph.cmp (g); }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
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
  ثابت T* get_قيمة (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSingle<T> *v = entries.bsearch (glyph_id);
    أرجع v ? &v->value : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    unsigned عدد = entries.get_length ();
    لكل (unsigned i = 0; i < عدد; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    unsigned عدد = entries.get_length ();
    لكل (unsigned i = 0; i < عدد; i++)
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
  Var_Sized_Bin_بحث_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  عام:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
بنية LookupFormat8
{
  friend بنية Lookup<T>;

  خاص:
  ثابت T* get_قيمة (hb_codepoint_t glyph_id) ثابت
  {
    أرجع firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_عدد_2 ?
	   &قيمة_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_عدد_2)) أرجع;
    إذا (firstGlyph == DELETED_GLYPH) أرجع;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_عدد_2 - 1);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_عدد_2)) أرجع;
    إذا (firstGlyph == DELETED_GLYPH) أرجع;
    ثابت T *p = قيمة_Array_Z.arrayZ;
    لكل (unsigned i = 0; i < glyph_عدد_2; i++)
      إذا (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && قيمة_Array_Z.sanitize (c, glyph_عدد_2));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && قيمة_Array_Z.sanitize (c, glyph_عدد_2, base));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_عدد_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		قيمة_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_SIZE_ARRAY (6, قيمة_Array_Z);
};

template <typename T>
بنية LookupFormat10
{
  friend بنية Lookup<T>;

  خاص:
  ثابت typename T::type get_قيمة_or_null (hb_codepoint_t glyph_id) ثابت
  {
    إذا (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_عدد_2))
      أرجع Null (T);

    ثابت HBUINT8 *p = &قيمة_Array_Z[(glyph_id - firstGlyph) * قيمة_Size];

    unsigned صحيح v = 0;
    unsigned صحيح عدد = قيمة_Size;
    لكل (unsigned صحيح i = 0; i < عدد; i++)
      v = (v << 8) | *p++;

    أرجع v;
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_عدد_2)) أرجع;
    إذا (firstGlyph == DELETED_GLYPH) أرجع;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_عدد_2 - 1);
  }

  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_عدد_2)) أرجع;
    إذا (firstGlyph == DELETED_GLYPH) أرجع;
    ثابت HBUINT8 *p = قيمة_Array_Z.arrayZ;
    لكل (unsigned i = 0; i < glyph_عدد_2; i++)
    {
      unsigned صحيح v = 0;
      unsigned صحيح عدد = قيمة_Size;
      لكل (unsigned صحيح j = 0; j < عدد; j++)
	v = (v << 8) | *p++;
      إذا (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  قيمة_Size <= 4 &&
		  قيمة_Array_Z.sanitize (c, glyph_عدد_2 * قيمة_Size));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	قيمة_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_عدد_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		قيمة_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_SIZE_ARRAY (8, قيمة_Array_Z);
};

template <typename T>
بنية Lookup
{
  ثابت T* get_قيمة (hb_codepoint_t glyph_id, unsigned صحيح رقم_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_قيمة (glyph_id, رقم_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_قيمة (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_قيمة (glyph_id);
    حالة 6: hb_barrier (); أرجع u.format6.get_قيمة (glyph_id);
    حالة 8: hb_barrier (); أرجع u.format8.get_قيمة (glyph_id);
    افتراضي:أرجع nullptr;
    }
  }

  ثابت typename T::type get_قيمة_or_null (hb_codepoint_t glyph_id, unsigned صحيح رقم_glyphs) ثابت
  {
    بدل (u.format.v) {
      /* Format 10 cannot return a pointer. */
      حالة 10: hb_barrier (); أرجع u.format10.get_قيمة_or_null (glyph_id);
      افتراضي:
      ثابت T *v = get_قيمة (glyph_id, رقم_glyphs);
      أرجع v ? *v : Null (T);
    }
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs, unsigned صحيح رقم_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); u.format0.collect_glyphs (glyphs, رقم_glyphs); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyphs (glyphs); أرجع;
    حالة 4: hb_barrier (); u.format4.collect_glyphs (glyphs); أرجع;
    حالة 6: hb_barrier (); u.format6.collect_glyphs (glyphs); أرجع;
    حالة 8: hb_barrier (); u.format8.collect_glyphs (glyphs); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_glyphs (glyphs); أرجع;
    افتراضي:أرجع;
    }
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, unsigned رقم_glyphs, ثابت filter_t &filter) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, رقم_glyphs, filter); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); أرجع;
    افتراضي:أرجع;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned صحيح رقم_glyphs,
			      unsigned صحيح outOfRange) ثابت
  {
    ثابت T *v = get_قيمة (glyph_id, رقم_glyphs);
    أرجع v ? *v : outOfRange;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    حالة 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    حالة 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    حالة 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    افتراضي:return_trace (true);
    }
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    حالة 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    حالة 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    حالة 10: return_trace (خطأ); /* We don't support format10 here currently. */
    افتراضي:return_trace (true);
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
  CLASS_END_OF_سطر = 3,
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
    STATE_START_OF_سطر = 1,
  };

  template <typename set_t, typename table_t>
  فراغ collect_initial_glyphs (set_t &glyphs, unsigned رقم_glyphs, ثابت table_t &table) ثابت
  {
    unsigned رقم_classes = nClasses;

    إذا (unlikely (رقم_classes > hb_bit_page_t::BITS))
    {
      (هذا+classTable).collect_glyphs (glyphs, رقم_glyphs);
      أرجع;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    لكل (unsigned i = 0; i < رقم_classes; i++)
    {
      ثابت auto &entry = get_entry (STATE_START_OF_TEXT, i);
      إذا (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	استمر;

      filter.add (i);
    }

    // And glyphs in those classes.

    إذا (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (هذا+classTable).collect_glyphs_filtered (glyphs, رقم_glyphs, filter);
  }

  صحيح new_state (unsigned صحيح newState) ثابت
  { أرجع Types::extended ? newState : ((صحيح) newState - (صحيح) stateArrayTable) / (صحيح) nClasses; }

  unsigned صحيح get_class (hb_codepoint_t glyph_id,
			  unsigned صحيح رقم_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) ثابت
  {
    unsigned klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    إذا (unlikely (glyph_id == DELETED_GLYPH)) أرجع CLASS_DELETED_GLYPH;
    klass = (هذا+classTable).get_class (glyph_id, رقم_glyphs, CLASS_OUT_OF_BOUNDS);
    إذا (cache) cache->set (glyph_id, klass);
    أرجع klass;
  }

  ثابت Entry<Extra> *get_entries () ثابت
  { أرجع (هذا+entryTable).arrayZ; }

  ثابت Entry<Extra> &get_entry (صحيح state, unsigned صحيح klass) ثابت
  {
    unsigned n_classes = nClasses;
    إذا (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    ثابت HBUSHORT *states = (هذا+stateArrayTable).arrayZ;
    ثابت Entry<Extra> *entries = (هذا+entryTable).arrayZ;

    unsigned صحيح entry = states[state * n_classes + klass];
    DEBUG_رسالة (APPLY, nullptr, "e%u", entry);

    أرجع entries[entry];
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 unsigned صحيح *رقم_entries_out = nullptr) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, هذا)))) return_trace (خطأ);

    ثابت HBUSHORT *states = (هذا+stateArrayTable).arrayZ;
    ثابت Entry<Extra> *entries = (هذا+entryTable).arrayZ;

    unsigned صحيح رقم_classes = nClasses;
    إذا (unlikely (hb_unsigned_mul_overflows (رقم_classes, states[0].static_size)))
      return_trace (خطأ);
    unsigned صحيح row_stride = رقم_classes * states[0].static_size;

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
    unsigned صحيح رقم_entries = 0;

    صحيح state_pos = 0;
    صحيح state_neg = 0;
    unsigned صحيح entry = 0;
    طالما (min_state < state_neg || state_pos <= max_state)
    {
      إذا (min_state < state_neg)
      {
	/* Negative states. */
	إذا (unlikely (hb_unsigned_mul_overflows (min_state, رقم_classes)))
	  return_trace (خطأ);
	إذا (unlikely (!c->check_range (&states[min_state * رقم_classes],
				       -min_state,
				       row_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  ثابت HBUSHORT *stop = &states[min_state * رقم_classes];
	  إذا (unlikely (stop > states))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = states; stop < p; p--)
	    رقم_entries = hb_max (رقم_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      إذا (state_pos <= max_state)
      {
	/* Positive states. */
	إذا (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  إذا (unlikely (hb_unsigned_mul_overflows ((max_state + 1), رقم_classes)))
	    return_trace (خطأ);
	  ثابت HBUSHORT *stop = &states[(max_state + 1) * رقم_classes];
	  إذا (unlikely (stop < states))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = &states[state_pos * رقم_classes]; p < stop; p++)
	    رقم_entries = hb_max (رقم_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      إذا (unlikely (!c->check_array (entries, رقم_entries)))
	return_trace (خطأ);
      إذا ((c->max_ops -= رقم_entries - entry) <= 0)
	return_trace (خطأ);
      { /* Sweep new entries. */
	ثابت Entry<Extra> *stop = &entries[رقم_entries];
	لكل (ثابت Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  صحيح newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = رقم_entries;
      }
    }

    إذا (رقم_entries_out)
      *رقم_entries_out = رقم_entries;

    return_trace (true);
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
    أرجع i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned صحيح get_class (hb_codepoint_t glyph_id,
			  unsigned صحيح رقم_glyphs HB_UNUSED,
			  unsigned صحيح outOfRange) ثابت
  {
    أرجع get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  فراغ collect_glyphs (set_t &glyphs, unsigned رقم_glyphs) ثابت
  {
    لكل (unsigned i = 0; i < classArray.len; i++)
      إذا (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  فراغ collect_glyphs_filtered (set_t &glyphs, unsigned رقم_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (unsigned i = 0; i < classArray.len; i++)
      إذا (filter (classArray.arrayZ[i]))
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
  منطقي sanitize (hb_sanitize_context_t *c, unsigned subtable_عدد) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_array (&subtableOffsets, subtable_عدد)))
      return_trace (خطأ);

    unsigned bytes = (c->get_رقم_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    لكل (unsigned i = 0; i < subtable_عدد; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      إذا (offset == 0 || offset == 0xFFFFFFFF)
        استمر;
      إذا (unlikely (!subtableOffsets[i].sanitize (c, هذا, bytes)))
        return_trace (خطأ);
    }

    return_trace (true);
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
  ساكن constexpr منطقي extended = خطأ;
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
    إذا (unlikely (offset < array_offset))
      أرجع bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    إذا (unlikely (index > bad_index))
      أرجع bad_index;
    أرجع index;
  }
  template <typename T>
  ساكن صحيح byteOffsetToIndex (int64_t offset,
				ثابت فراغ *base,
				ثابت T *array)
  {
    أرجع offsetToIndex (offset, base, array);
  }
  template <typename T>
  ساكن صحيح wordOffsetToIndex (int64_t offset,
				ثابت فراغ *base,
				ثابت T *array)
  {
    أرجع offsetToIndex (2 * offset, base, array);
  }
};
بنية ExtendedTypes
{
  ساكن constexpr منطقي extended = true;
  تعريفنوع HBUINT32 HBUINT;
  تعريفنوع HBUINT16 HBUSHORT;
  تعريفنوع Lookup<HBUINT16> ClassTypeNarrow;
  تعريفنوع Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  ساكن unsigned صحيح offsetToIndex (unsigned صحيح offset,
				     ثابت فراغ *base HB_UNUSED,
				     ثابت T *array HB_UNUSED)
  {
    أرجع offset;
  }
  template <typename T>
  ساكن unsigned صحيح byteOffsetToIndex (unsigned صحيح offset,
					 ثابت فراغ *base HB_UNUSED,
					 ثابت T *array HB_UNUSED)
  {
    أرجع offset / 2;
  }
  template <typename T>
  ساكن unsigned صحيح wordOffsetToIndex (unsigned صحيح offset,
					 ثابت فراغ *base HB_UNUSED,
					 ثابت T *array HB_UNUSED)
  {
    أرجع offset;
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
	      رقم_glyphs (face_->get_رقم_glyphs ()) {}

  template <typename context_t>
  فراغ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    إذا (!c->in_place)
      buffer->clear_output ();

    صحيح state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    ثابت منطقي start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    لكل (buffer->idx = 0; buffer->successful;)
    {
      unsigned صحيح klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, رقم_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_رسالة (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      ثابت EntryT &entry = machine.get_entry (state, klass);
      ثابت صحيح next_state = machine.new_state (entry.newState);

      منطقي is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      منطقي is_not_actionable = !c->table->is_actionable (entry);

      إذا (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	إذا (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  طالما (cluster < range->cluster_first)
	    range--;
	  طالما (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	إذا (!(range->flags & ac->subtable_flags))
	{
	  إذا (buffer->idx == buffer->len)
	    اكسر;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (فراغ) buffer->next_glyph ();
	  استمر;
	}
      }
      وإلا
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	منطقي is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	إذا (is_null_transition)
	{
	  unsigned old_klass = klass;
	  افعل
	  {
	    c->transition (buffer, هذا, entry);

	    إذا (buffer->idx == buffer->len || !buffer->successful)
	      اكسر;

	    (فراغ) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, رقم_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } طالما (klass == old_klass);

	  إذا (buffer->idx == buffer->len || !buffer->successful)
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

      إذا (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, هذا, entry);

      state = next_state;
      DEBUG_رسالة (APPLY, nullptr, "s%d", state);

      إذا (buffer->idx == buffer->len)
	اكسر;

      إذا (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(فراغ) buffer->next_glyph ();
    }

    إذا (!c->in_place)
      buffer->متزامن ();
  }

  عام:
  ثابت StateTableT &machine;
  unsigned صحيح رقم_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
