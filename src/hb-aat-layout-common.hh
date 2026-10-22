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

#ಅನ್ತರ್ಭಾವ "hb-aat-layout.hh"
#ಅನ್ತರ್ಭಾವ "hb-aat-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-layout-gdef-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-cache.hh"
#ಅನ್ತರ್ಭಾವ "hb-bit-set.hh"
#ಅನ್ತರ್ಭಾವ "hb-bit-page.hh"


ನಾಮಾಕಾಶ OT {
ಸಂರಚನಾ GDEF;
};

ನಾಮಾಕಾಶ AAT {

using ನಾಮಾಕಾಶ OT;

ಸಂರಚನಾ ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#ifndef HB_OPTIMIZE_SIZE
#define HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#ಅನ್ಯಥಾ
#define HB_AAT_TRANSITION_INLINE
#endif

ಸಂರಚನಾ hb_aat_scratch_t
{
  hb_aat_scratch_t () = ಸ್ವಭಾವ;
  hb_aat_scratch_t (ಧ್ರುವ hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & operator = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }
  ~hb_aat_scratch_t ()
  {
    auto *s = buffer_glyph_set.get_relaxed ();
    ಯದಿ (unlikely (!s))
      ಪ್ರತ್ಯಾಗಮ;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () ಧ್ರುವ
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    ಯದಿ (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      ಪ್ರತ್ಯಾಗಮ s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, ಪರಿಮಾಣ (hb_bit_set_t));
    ಯದಿ (unlikely (!s))
      ಪ್ರತ್ಯಾಗಮ nullptr;
    s->init ();

    ಪ್ರತ್ಯಾಗಮ s;
  }
  ಶೂನ್ಯ destroy_buffer_glyph_set (hb_bit_set_t *s) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!s))
      ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (buffer_glyph_set.cmpexch (nullptr, s))
      ಪ್ರತ್ಯಾಗಮ;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> buffer_glyph_set;
};

ಗಣನಾ { DELETED_GLYPH = 0xFFFF };

#define HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED HB_BUFFER_SCRATCH_FLAG_SHAPER0

ಸಂರಚನಾ hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, ತಾರ್ಕಿಕ, HB_DEBUG_APPLY>
{
  ಧ್ರುವ ವರ್ಣ *get_name () { ಪ್ರತ್ಯಾಗಮ "APPLY"; }
  template <typename T, typename ...Ts>
  return_t dispatch (ಧ್ರುವ T &obj, Ts&&... ds)
  { ಪ್ರತ್ಯಾಗಮ obj.apply (ಏತತ್, std::forward<Ts> (ds)...); }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  ತಾರ್ಕಿಕ stop_sublookup_iteration (return_t r) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ r; }

  ಧ್ರುವ hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  ಧ್ರುವ ankr *ankr_table;
  ಧ್ರುವ OT::GDEF &gdef;
  ತಾರ್ಕಿಕ has_glyph_classes;
  ಧ್ರುವ hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  ತಾರ್ಕಿಕ buffer_is_reversed = ಅಸತ್ಯ;
  // Caches
  ತಾರ್ಕಿಕ using_buffer_glyph_set = ಅಸತ್ಯ;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  ಧ್ರುವ hb_bit_set_t *first_set = nullptr;
  ಧ್ರುವ hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  unsigned ಪೂರ್ಣಾಙ್ಕ lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (ಧ್ರುವ hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL ಶೂನ್ಯ set_ankr_table (ಧ್ರುವ AAT::ankr *ankr_table_);

  ಶೂನ್ಯ set_lookup_index (unsigned ಪೂರ್ಣಾಙ್ಕ i) { lookup_index = i; }

  ಶೂನ್ಯ reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  ಶೂನ್ಯ setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    ಯದಿ (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  ತಾರ್ಕಿಕ buffer_intersects_machine () ಧ್ರುವ
  {
    ಯದಿ (likely (using_buffer_glyph_set))
      ಪ್ರತ್ಯಾಗಮ buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    ಪ್ರತಿ (unsigned i = 0; i < buffer->len; i++)
      ಯದಿ (first_set->has (buffer->info[i].codepoint))
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  template <typename T>
  HB_NODISCARD ತಾರ್ಕಿಕ output_glyphs (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
				   ಧ್ರುವ T *glyphs)
  {
    ಯದಿ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, ಎಣಿಕೆ);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      ತಾರ್ಕಿಕ at_end_of_text = buffer->idx == buffer->len;
      ಯದಿ (at_end_of_text)
	ಯದಿ (unlikely (!buffer->output_glyph (glyphs[i]))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      ಯದಿ (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      ಅನ್ಯಥಾ
      {
#ifndef HB_NO_OT_LAYOUT
	ಯದಿ (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      ಯದಿ (!at_end_of_text)
	ಯದಿ (unlikely (!buffer->output_glyph (glyphs[i]))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  HB_NODISCARD ತಾರ್ಕಿಕ replace_glyph (hb_codepoint_t glyph)
  {
    ಯದಿ (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    ಯದಿ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    ಯದಿ (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    ಪ್ರತ್ಯಾಗಮ buffer->replace_glyph (glyph);
  }

  HB_NODISCARD ತಾರ್ಕಿಕ delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    ಯದಿ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    ಪ್ರತ್ಯಾಗಮ buffer->replace_glyph (DELETED_GLYPH);
  }

  ಶೂನ್ಯ replace_glyph_inplace (unsigned i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    ಯದಿ (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    ಯದಿ (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    ಯದಿ (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

template <typename T> ಸಂರಚನಾ Lookup;

template <typename T>
ಸಂರಚನಾ LookupFormat0
{
  friend ಸಂರಚನಾ Lookup<T>;

  ಗುಪ್ತ:
  ಧ್ರುವ T* get_ಮೌಲ್ಯ (hb_codepoint_t glyph_id, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಯದಿ (unlikely (glyph_id >= ಅಂಕೆ_glyphs)) ಪ್ರತ್ಯಾಗಮ nullptr;
    ಪ್ರತ್ಯಾಗಮ &arrayZ[glyph_id];
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    glyphs.add_range (0, ಅಂಕೆ_glyphs - 1);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, unsigned ಅಂಕೆ_glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_glyphs; i++)
      ಯದಿ (filter (arrayZ[i]))
	glyphs.add (i);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (arrayZ.sanitize (c, c->get_ಅಂಕೆ_glyphs ()));
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (arrayZ.sanitize (c, c->get_ಅಂಕೆ_glyphs (), base));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNBOUNDED (2);
};


template <typename T>
ಸಂರಚನಾ LookupSegmentSingle
{
  ಸ್ಥಿರ constexpr unsigned Termination_Word_ಎಣಿಕೆ = 2u;

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ g < first ? -1 : g <= last ? 0 : +1 ; }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs) ಧ್ರುವ
  {
    ಯದಿ (first == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    ಯದಿ (first == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!filter (value)) ಪ್ರತ್ಯಾಗಮ;
    glyphs.add_range (first, last);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && value.sanitize (c));
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <typename T>
ಸಂರಚನಾ LookupFormat2
{
  friend ಸಂರಚನಾ Lookup<T>;

  ಗುಪ್ತ:
  ಧ್ರುವ T* get_ಮೌಲ್ಯ (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ಧ್ರುವ LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    ಪ್ರತ್ಯಾಗಮ v ? &v->value : nullptr;
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = segments.get_length ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = segments.get_length ();
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (segments.sanitize (c));
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (segments.sanitize (c, base));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_ಹುಡುಕು_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
ಸಂರಚನಾ LookupSegmentArray
{
  ಸ್ಥಿರ constexpr unsigned Termination_Word_ಎಣಿಕೆ = 2u;

  ಧ್ರುವ T* get_ಮೌಲ್ಯ (hb_codepoint_t glyph_id, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ first <= glyph_id && glyph_id <= last ? &(base+ಮೌಲ್ಯಗಳು_Z)[glyph_id - first] : nullptr;
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs) ಧ್ರುವ
  {
    ಯದಿ (first == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    glyphs.add_range (first, last);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, ಧ್ರುವ ಶೂನ್ಯ *base, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    ಯದಿ (first == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    ಧ್ರುವ auto &values = base+ಮೌಲ್ಯಗಳು_Z;
    ಪ್ರತಿ (hb_codepoint_t i = first; i <= last; i++)
      ಯದಿ (filter (values[i - first]))
	glyphs.add (i);
  }

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ g < first ? -1 : g <= last ? 0 : +1; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  first <= last &&
		  ಮೌಲ್ಯಗಳು_Z.sanitize (c, base, last - first + 1));
  }
  template <typename ...Ts>
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base, Ts&&... ds) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  first <= last &&
		  ಮೌಲ್ಯಗಳು_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		ಮೌಲ್ಯಗಳು_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (6);
};

template <typename T>
ಸಂರಚನಾ LookupFormat4
{
  friend ಸಂರಚನಾ Lookup<T>;

  ಗುಪ್ತ:
  ಧ್ರುವ T* get_ಮೌಲ್ಯ (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ಧ್ರುವ LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    ಪ್ರತ್ಯಾಗಮ v ? v->get_ಮೌಲ್ಯ (glyph_id, ಏತತ್) : nullptr;
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = segments.get_length ();
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      segments[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = segments.get_length ();
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      segments[i].collect_glyphs_filtered (glyphs, ಏತತ್, filter);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (segments.sanitize (c, ಏತತ್));
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (segments.sanitize (c, ಏತತ್, base));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_ಹುಡುಕು_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, segments);
};

template <typename T>
ಸಂರಚನಾ LookupSingle
{
  ಸ್ಥಿರ constexpr unsigned Termination_Word_ಎಣಿಕೆ = 1u;

  ಪೂರ್ಣಾಙ್ಕ cmp (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ glyph.cmp (g); }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs) ಧ್ರುವ
  {
    ಯದಿ (glyph == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    glyphs.add (glyph);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    ಯದಿ (glyph == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!filter (value)) ಪ್ರತ್ಯಾಗಮ;
    glyphs.add (glyph);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && value.sanitize (c));
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

template <typename T>
ಸಂರಚನಾ LookupFormat6
{
  friend ಸಂರಚನಾ Lookup<T>;

  ಗುಪ್ತ:
  ಧ್ರುವ T* get_ಮೌಲ್ಯ (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ಧ್ರುವ LookupSingle<T> *v = entries.bsearch (glyph_id);
    ಪ್ರತ್ಯಾಗಮ v ? &v->value : nullptr;
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = entries.get_length ();
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      entries[i].collect_glyphs (glyphs);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = entries.get_length ();
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (entries.sanitize (c));
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (entries.sanitize (c, base));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_ಹುಡುಕು_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, entries);
};

template <typename T>
ಸಂರಚನಾ LookupFormat8
{
  friend ಸಂರಚನಾ Lookup<T>;

  ಗುಪ್ತ:
  ಧ್ರುವ T* get_ಮೌಲ್ಯ (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_ಎಣಿಕೆ_2 ?
	   &ಮೌಲ್ಯ_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!glyph_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (firstGlyph == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_ಎಣಿಕೆ_2 - 1);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!glyph_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (firstGlyph == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    ಧ್ರುವ T *p = ಮೌಲ್ಯ_Array_Z.arrayZ;
    ಪ್ರತಿ (unsigned i = 0; i < glyph_ಎಣಿಕೆ_2; i++)
      ಯದಿ (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && ಮೌಲ್ಯ_Array_Z.sanitize (c, glyph_ಎಣಿಕೆ_2));
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && ಮೌಲ್ಯ_Array_Z.sanitize (c, glyph_ಎಣಿಕೆ_2, base));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ಎಣಿಕೆ_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		ಮೌಲ್ಯ_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (6, ಮೌಲ್ಯ_Array_Z);
};

template <typename T>
ಸಂರಚನಾ LookupFormat10
{
  friend ಸಂರಚನಾ Lookup<T>;

  ಗುಪ್ತ:
  ಧ್ರುವ typename T::type get_ಮೌಲ್ಯ_or_null (hb_codepoint_t glyph_id) ಧ್ರುವ
  {
    ಯದಿ (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_ಎಣಿಕೆ_2))
      ಪ್ರತ್ಯಾಗಮ Null (T);

    ಧ್ರುವ HBUINT8 *p = &ಮೌಲ್ಯ_Array_Z[(glyph_id - firstGlyph) * ಮೌಲ್ಯ_Size];

    unsigned ಪೂರ್ಣಾಙ್ಕ v = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = ಮೌಲ್ಯ_Size;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      v = (v << 8) | *p++;

    ಪ್ರತ್ಯಾಗಮ v;
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!glyph_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (firstGlyph == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_ಎಣಿಕೆ_2 - 1);
  }

  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!glyph_ಎಣಿಕೆ_2)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (firstGlyph == DELETED_GLYPH) ಪ್ರತ್ಯಾಗಮ;
    ಧ್ರುವ HBUINT8 *p = ಮೌಲ್ಯ_Array_Z.arrayZ;
    ಪ್ರತಿ (unsigned i = 0; i < glyph_ಎಣಿಕೆ_2; i++)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ v = 0;
      unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = ಮೌಲ್ಯ_Size;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; j < ಎಣಿಕೆ; j++)
	v = (v << 8) | *p++;
      ಯದಿ (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  hb_barrier () &&
		  ಮೌಲ್ಯ_Size <= 4 &&
		  ಮೌಲ್ಯ_Array_Z.sanitize (c, glyph_ಎಣಿಕೆ_2 * ಮೌಲ್ಯ_Size));
  }

  ರಕ್ಷಿತ:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	ಮೌಲ್ಯ_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ಎಣಿಕೆ_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		ಮೌಲ್ಯ_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, ಮೌಲ್ಯ_Array_Z);
};

template <typename T>
ಸಂರಚನಾ Lookup
{
  ಧ್ರುವ T* get_ಮೌಲ್ಯ (hb_codepoint_t glyph_id, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format0.get_ಮೌಲ್ಯ (glyph_id, ಅಂಕೆ_glyphs);
    ಪಕ್ಷ 2: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format2.get_ಮೌಲ್ಯ (glyph_id);
    ಪಕ್ಷ 4: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format4.get_ಮೌಲ್ಯ (glyph_id);
    ಪಕ್ಷ 6: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format6.get_ಮೌಲ್ಯ (glyph_id);
    ಪಕ್ಷ 8: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format8.get_ಮೌಲ್ಯ (glyph_id);
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ nullptr;
    }
  }

  ಧ್ರುವ typename T::type get_ಮೌಲ್ಯ_or_null (hb_codepoint_t glyph_id, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
      /* Format 10 cannot return a pointer. */
      ಪಕ್ಷ 10: hb_barrier (); ಪ್ರತ್ಯಾಗಮ u.format10.get_ಮೌಲ್ಯ_or_null (glyph_id);
      ಸ್ವಭಾವ:
      ಧ್ರುವ T *v = get_ಮೌಲ್ಯ (glyph_id, ಅಂಕೆ_glyphs);
      ಪ್ರತ್ಯಾಗಮ v ? *v : Null (T);
    }
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); u.format0.collect_glyphs (glyphs, ಅಂಕೆ_glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 2: hb_barrier (); u.format2.collect_glyphs (glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 4: hb_barrier (); u.format4.collect_glyphs (glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 6: hb_barrier (); u.format6.collect_glyphs (glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 8: hb_barrier (); u.format8.collect_glyphs (glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 10: hb_barrier (); u.format10.collect_glyphs (glyphs); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, unsigned ಅಂಕೆ_glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, ಅಂಕೆ_glyphs, filter); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); ಪ್ರತ್ಯಾಗಮ;
    ಪಕ್ಷ 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); ಪ್ರತ್ಯಾಗಮ;
    ಸ್ವಭಾವ:ಪ್ರತ್ಯಾಗಮ;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
			      unsigned ಪೂರ್ಣಾಙ್ಕ outOfRange) ಧ್ರುವ
  {
    ಧ್ರುವ T *v = get_ಮೌಲ್ಯ (glyph_id, ಅಂಕೆ_glyphs);
    ಪ್ರತ್ಯಾಗಮ v ? *v : outOfRange;
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    ಪಕ್ಷ 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    ಪಕ್ಷ 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    ಪಕ್ಷ 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (!u.format.v.sanitize (c)) return_trace (ಅಸತ್ಯ);
    hb_barrier ();
    ವಿಕಲ್ಪ (u.format.v) {
    ಪಕ್ಷ 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    ಪಕ್ಷ 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    ಪಕ್ಷ 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    ಪಕ್ಷ 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    ಪಕ್ಷ 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    ಪಕ್ಷ 10: return_trace (ಅಸತ್ಯ); /* We don't support format10 here currently. */
    ಸ್ವಭಾವ:return_trace (ಸತ್ಯ);
    }
  }

  ರಕ್ಷಿತ:
  ಸಙ್ಘ {
  ಸಂರಚನಾ { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

template <typename T>
ಸಂರಚನಾ Entry
{
  // This doesn't seem like it's ever called.
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		data;		/* Optional offsets to per-glyph tables. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 + T::static_size);
};

template <>
ಸಂರಚನಾ Entry<ಶೂನ್ಯ>
{
  // This does seem like it's ever called.
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್));
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4);
};

ಗಣನಾ Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_ಸಾಲು = 3,
};

template <typename Types, typename Extra>
ಸಂರಚನಾ StateTable
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUINT HBUINT;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::HBUSHORT HBUSHORT;
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::ClassTypeNarrow ClassType;

  ಗಣನಾ State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_ಸಾಲು = 1,
  };

  template <typename set_t, typename table_t>
  ಶೂನ್ಯ collect_initial_glyphs (set_t &glyphs, unsigned ಅಂಕೆ_glyphs, ಧ್ರುವ table_t &table) ಧ್ರುವ
  {
    unsigned ಅಂಕೆ_classes = nClasses;

    ಯದಿ (unlikely (ಅಂಕೆ_classes > hb_bit_page_t::BITS))
    {
      (ಏತತ್+classTable).collect_glyphs (glyphs, ಅಂಕೆ_glyphs);
      ಪ್ರತ್ಯಾಗಮ;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_classes; i++)
    {
      ಧ್ರುವ auto &entry = get_entry (STATE_START_OF_TEXT, i);
      ಯದಿ (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	ಅನುವರ್ತ;

      filter.add (i);
    }

    // And glyphs in those classes.

    ಯದಿ (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (ಏತತ್+classTable).collect_glyphs_filtered (glyphs, ಅಂಕೆ_glyphs, filter);
  }

  ಪೂರ್ಣಾಙ್ಕ new_state (unsigned ಪೂರ್ಣಾಙ್ಕ newState) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ Types::extended ? newState : ((ಪೂರ್ಣಾಙ್ಕ) newState - (ಪೂರ್ಣಾಙ್ಕ) stateArrayTable) / (ಪೂರ್ಣಾಙ್ಕ) nClasses; }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_class (hb_codepoint_t glyph_id,
			  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) ಧ್ರುವ
  {
    unsigned klass;
    ಯದಿ (cache && cache->get (glyph_id, &klass)) ಪ್ರತ್ಯಾಗಮ klass;
    ಯದಿ (unlikely (glyph_id == DELETED_GLYPH)) ಪ್ರತ್ಯಾಗಮ CLASS_DELETED_GLYPH;
    klass = (ಏತತ್+classTable).get_class (glyph_id, ಅಂಕೆ_glyphs, CLASS_OUT_OF_BOUNDS);
    ಯದಿ (cache) cache->set (glyph_id, klass);
    ಪ್ರತ್ಯಾಗಮ klass;
  }

  ಧ್ರುವ Entry<Extra> *get_entries () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (ಏತತ್+entryTable).arrayZ; }

  ಧ್ರುವ Entry<Extra> &get_entry (ಪೂರ್ಣಾಙ್ಕ state, unsigned ಪೂರ್ಣಾಙ್ಕ klass) ಧ್ರುವ
  {
    unsigned n_classes = nClasses;
    ಯದಿ (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    ಧ್ರುವ HBUSHORT *states = (ಏತತ್+stateArrayTable).arrayZ;
    ಧ್ರುವ Entry<Extra> *entries = (ಏತತ್+entryTable).arrayZ;

    unsigned ಪೂರ್ಣಾಙ್ಕ entry = states[state * n_classes + klass];
    DEBUG_ಸಂದೇಶ (APPLY, nullptr, "e%u", entry);

    ಪ್ರತ್ಯಾಗಮ entries[entry];
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c,
		 unsigned ಪೂರ್ಣಾಙ್ಕ *ಅಂಕೆ_entries_out = nullptr) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    ಯದಿ (unlikely (!(c->check_struct (ಏತತ್) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, ಏತತ್)))) return_trace (ಅಸತ್ಯ);

    ಧ್ರುವ HBUSHORT *states = (ಏತತ್+stateArrayTable).arrayZ;
    ಧ್ರುವ Entry<Extra> *entries = (ಏತತ್+entryTable).arrayZ;

    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_classes = nClasses;
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (ಅಂಕೆ_classes, states[0].static_size)))
      return_trace (ಅಸತ್ಯ);
    unsigned ಪೂರ್ಣಾಙ್ಕ row_stride = ಅಂಕೆ_classes * states[0].static_size;

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

    ಪೂರ್ಣಾಙ್ಕ min_state = 0;
    ಪೂರ್ಣಾಙ್ಕ max_state = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_entries = 0;

    ಪೂರ್ಣಾಙ್ಕ state_pos = 0;
    ಪೂರ್ಣಾಙ್ಕ state_neg = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ entry = 0;
    ಯಾವತ್ (min_state < state_neg || state_pos <= max_state)
    {
      ಯದಿ (min_state < state_neg)
      {
	/* Negative states. */
	ಯದಿ (unlikely (hb_unsigned_mul_overflows (min_state, ಅಂಕೆ_classes)))
	  return_trace (ಅಸತ್ಯ);
	ಯದಿ (unlikely (!c->check_range (&states[min_state * ಅಂಕೆ_classes],
				       -min_state,
				       row_stride)))
	  return_trace (ಅಸತ್ಯ);
	ಯದಿ ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (ಅಸತ್ಯ);
	{ /* Sweep new states. */
	  ಧ್ರುವ HBUSHORT *stop = &states[min_state * ಅಂಕೆ_classes];
	  ಯದಿ (unlikely (stop > states))
	    return_trace (ಅಸತ್ಯ);
	  ಪ್ರತಿ (ಧ್ರುವ HBUSHORT *p = states; stop < p; p--)
	    ಅಂಕೆ_entries = hb_max (ಅಂಕೆ_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      ಯದಿ (state_pos <= max_state)
      {
	/* Positive states. */
	ಯದಿ (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (ಅಸತ್ಯ);
	ಯದಿ ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (ಅಸತ್ಯ);
	{ /* Sweep new states. */
	  ಯದಿ (unlikely (hb_unsigned_mul_overflows ((max_state + 1), ಅಂಕೆ_classes)))
	    return_trace (ಅಸತ್ಯ);
	  ಧ್ರುವ HBUSHORT *stop = &states[(max_state + 1) * ಅಂಕೆ_classes];
	  ಯದಿ (unlikely (stop < states))
	    return_trace (ಅಸತ್ಯ);
	  ಪ್ರತಿ (ಧ್ರುವ HBUSHORT *p = &states[state_pos * ಅಂಕೆ_classes]; p < stop; p++)
	    ಅಂಕೆ_entries = hb_max (ಅಂಕೆ_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      ಯದಿ (unlikely (!c->check_array (entries, ಅಂಕೆ_entries)))
	return_trace (ಅಸತ್ಯ);
      ಯದಿ ((c->max_ops -= ಅಂಕೆ_entries - entry) <= 0)
	return_trace (ಅಸತ್ಯ);
      { /* Sweep new entries. */
	ಧ್ರುವ Entry<Extra> *stop = &entries[ಅಂಕೆ_entries];
	ಪ್ರತಿ (ಧ್ರುವ Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  ಪೂರ್ಣಾಙ್ಕ newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = ಅಂಕೆ_entries;
      }
    }

    ಯದಿ (ಅಂಕೆ_entries_out)
      *ಅಂಕೆ_entries_out = ಅಂಕೆ_entries;

    return_trace (ಸತ್ಯ);
  }

  ರಕ್ಷಿತ:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_STATIC (4 * ಪರಿಮಾಣ (HBUINT));
};

template <typename HBUCHAR>
ಸಂರಚನಾ ClassTable
{
  unsigned ಪೂರ್ಣಾಙ್ಕ get_class (hb_codepoint_t glyph_id, unsigned ಪೂರ್ಣಾಙ್ಕ outOfRange) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ i = glyph_id - firstGlyph;
    ಪ್ರತ್ಯಾಗಮ i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_class (hb_codepoint_t glyph_id,
			  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs HB_UNUSED,
			  unsigned ಪೂರ್ಣಾಙ್ಕ outOfRange) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ get_class (glyph_id, outOfRange);
  }

  template <typename set_t>
  ಶೂನ್ಯ collect_glyphs (set_t &glyphs, unsigned ಅಂಕೆ_glyphs) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < classArray.len; i++)
      ಯದಿ (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  template <typename set_t, typename filter_t>
  ಶೂನ್ಯ collect_glyphs_filtered (set_t &glyphs, unsigned ಅಂಕೆ_glyphs, ಧ್ರುವ filter_t &filter) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < classArray.len; i++)
      ಯದಿ (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) && classArray.sanitize (c));
  }
  ರಕ್ಷಿತ:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, classArray);
};

ಸಂರಚನಾ SubtableGlyphCoverage
{
  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c, unsigned subtable_ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    ಯದಿ (unlikely (!c->check_array (&subtableOffsets, subtable_ಎಣಿಕೆ)))
      return_trace (ಅಸತ್ಯ);

    unsigned bytes = (c->get_ಅಂಕೆ_glyphs () + CHAR_BIT - 1) / CHAR_BIT;
    ಪ್ರತಿ (unsigned i = 0; i < subtable_ಎಣಿಕೆ; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      ಯದಿ (offset == 0 || offset == 0xFFFFFFFF)
        ಅನುವರ್ತ;
      ಯದಿ (unlikely (!subtableOffsets[i].sanitize (c, ಏತತ್, bytes)))
        return_trace (ಅಸತ್ಯ);
    }

    return_trace (ಸತ್ಯ);
  }
  ರಕ್ಷಿತ:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (0, subtableOffsets);
};

ಸಂರಚನಾ ObsoleteTypes
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ extended = ಅಸತ್ಯ;
  ಪ್ರಕಾರನಿರ್ದೇಶ HBUINT16 HBUINT;
  ಪ್ರಕಾರನಿರ್ದೇಶ HBUINT8 HBUSHORT;
  ಪ್ರಕಾರನಿರ್ದೇಶ ClassTable<HBUINT8> ClassTypeNarrow;
  ಪ್ರಕಾರನಿರ್ದೇಶ ClassTable<HBUINT16> ClassTypeWide;

  template <typename T>
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ offsetToIndex (int64_t offset,
			    ಧ್ರುವ ಶೂನ್ಯ *base,
			    ಧ್ರುವ T *array)
  {
    int64_t array_offset = (ಧ್ರುವ ವರ್ಣ *) array - (ಧ್ರುವ ವರ್ಣ *) base;
    ಪೂರ್ಣಾಙ್ಕ bad_index = INT_MAX / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    ಯದಿ (unlikely (offset < array_offset))
      ಪ್ರತ್ಯಾಗಮ bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    ಯದಿ (unlikely (index > bad_index))
      ಪ್ರತ್ಯಾಗಮ bad_index;
    ಪ್ರತ್ಯಾಗಮ index;
  }
  template <typename T>
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ byteOffsetToIndex (int64_t offset,
				ಧ್ರುವ ಶೂನ್ಯ *base,
				ಧ್ರುವ T *array)
  {
    ಪ್ರತ್ಯಾಗಮ offsetToIndex (offset, base, array);
  }
  template <typename T>
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ wordOffsetToIndex (int64_t offset,
				ಧ್ರುವ ಶೂನ್ಯ *base,
				ಧ್ರುವ T *array)
  {
    ಪ್ರತ್ಯಾಗಮ offsetToIndex (2 * offset, base, array);
  }
};
ಸಂರಚನಾ ExtendedTypes
{
  ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ extended = ಸತ್ಯ;
  ಪ್ರಕಾರನಿರ್ದೇಶ HBUINT32 HBUINT;
  ಪ್ರಕಾರನಿರ್ದೇಶ HBUINT16 HBUSHORT;
  ಪ್ರಕಾರನಿರ್ದೇಶ Lookup<HBUINT16> ClassTypeNarrow;
  ಪ್ರಕಾರನಿರ್ದೇಶ Lookup<HBUINT16> ClassTypeWide;

  template <typename T>
  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ offsetToIndex (unsigned ಪೂರ್ಣಾಙ್ಕ offset,
				     ಧ್ರುವ ಶೂನ್ಯ *base HB_UNUSED,
				     ಧ್ರುವ T *array HB_UNUSED)
  {
    ಪ್ರತ್ಯಾಗಮ offset;
  }
  template <typename T>
  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ byteOffsetToIndex (unsigned ಪೂರ್ಣಾಙ್ಕ offset,
					 ಧ್ರುವ ಶೂನ್ಯ *base HB_UNUSED,
					 ಧ್ರುವ T *array HB_UNUSED)
  {
    ಪ್ರತ್ಯಾಗಮ offset / 2;
  }
  template <typename T>
  ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ wordOffsetToIndex (unsigned ಪೂರ್ಣಾಙ್ಕ offset,
					 ಧ್ರುವ ಶೂನ್ಯ *base HB_UNUSED,
					 ಧ್ರುವ T *array HB_UNUSED)
  {
    ಪ್ರತ್ಯಾಗಮ offset;
  }
};

template <typename Types, typename EntryData, typename Flags>
ಸಂರಚನಾ StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (ಧ್ರುವ StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      ಅಂಕೆ_glyphs (face_->get_ಅಂಕೆ_glyphs ()) {}

  template <typename context_t>
  ಶೂನ್ಯ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    ಯದಿ (!c->in_place)
      buffer->clear_output ();

    ಪೂರ್ಣಾಙ್ಕ state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    auto *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    ಧ್ರುವ ತಾರ್ಕಿಕ start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    ಪ್ರತಿ (buffer->idx = 0; buffer->successful;)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, ಅಂಕೆ_glyphs, ac->machine_class_cache) :
			   (unsigned) CLASS_END_OF_TEXT;
    resume:
      DEBUG_ಸಂದೇಶ (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      ಧ್ರುವ EntryT &entry = machine.get_entry (state, klass);
      ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ next_state = machine.new_state (entry.newState);

      ತಾರ್ಕಿಕ is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      ತಾರ್ಕಿಕ is_not_actionable = !c->table->is_actionable (entry);

      ಯದಿ (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	auto *range = last_range;
	ಯದಿ (buffer->idx < buffer->len)
	{
	  unsigned cluster = buffer->cur().cluster;
	  ಯಾವತ್ (cluster < range->cluster_first)
	    range--;
	  ಯಾವತ್ (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	ಯದಿ (!(range->flags & ac->subtable_flags))
	{
	  ಯದಿ (buffer->idx == buffer->len)
	    ಭಙ್ಗ;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (ಶೂನ್ಯ) buffer->next_glyph ();
	  ಅನುವರ್ತ;
	}
      }
      ಅನ್ಯಥಾ
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	ತಾರ್ಕಿಕ is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	ಯದಿ (is_null_transition)
	{
	  unsigned old_klass = klass;
	  ಕುರು
	  {
	    c->transition (buffer, ಏತತ್, entry);

	    ಯದಿ (buffer->idx == buffer->len || !buffer->successful)
	      ಭಙ್ಗ;

	    (ಶೂನ್ಯ) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, ಅಂಕೆ_glyphs, ac->machine_class_cache) :
		     (unsigned) CLASS_END_OF_TEXT;
	  } ಯಾವತ್ (klass == old_klass);

	  ಯದಿ (buffer->idx == buffer->len || !buffer->successful)
	    ಭಙ್ಗ;

	  ಗಚ್ಛ resume;
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
      ಧ್ರುವ EntryT *wouldbe_entry;
      ತಾರ್ಕಿಕ is_safe_to_break =
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

      ಯದಿ (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, ಏತತ್, entry);

      state = next_state;
      DEBUG_ಸಂದೇಶ (APPLY, nullptr, "s%d", state);

      ಯದಿ (buffer->idx == buffer->len)
	ಭಙ್ಗ;

      ಯದಿ (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(ಶೂನ್ಯ) buffer->next_glyph ();
    }

    ಯದಿ (!c->in_place)
      buffer->ಸಮಕಾಲ ();
  }

  ಸಾರ್ವಜನಿಕ:
  ಧ್ರುವ StateTableT &machine;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
