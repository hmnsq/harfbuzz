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
#अन्यथा
#define HB_AAT_TRANSITION_INLINE
#endif

संरचना hb_aat_scratch_t
{
  hb_aat_scratch_t () = स्वभाव;
  hb_aat_scratch_t (अचर hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & चालक = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    फर्काऊ *एतत्;
  }
  ~hb_aat_scratch_t ()
  {
    स्वतः *s = buffer_glyph_set.get_relaxed ();
    यदि (unlikely (!s))
      फर्काऊ;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () अचर
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    यदि (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      फर्काऊ s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, परिमाण (hb_bit_set_t));
    यदि (unlikely (!s))
      फर्काऊ nullptr;
    s->init ();

    फर्काऊ s;
  }
  शून्य destroy_buffer_glyph_set (hb_bit_set_t *s) अचर
  {
    यदि (unlikely (!s))
      फर्काऊ;
    यदि (buffer_glyph_set.cmpexch (nullptr, s))
      फर्काऊ;
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
  अचर वर्ण *get_name () { फर्काऊ "APPLY"; }
  ढाँचा <typename T, typename ...Ts>
  return_t dispatch (अचर T &obj, Ts&&... ds)
  { फर्काऊ obj.apply (एतत्, std::forward<Ts> (ds)...); }
  स्थिर return_t default_return_मान () { फर्काऊ असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) अचर { फर्काऊ r; }

  अचर hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  अचर ankr *ankr_table;
  अचर OT::GDEF &gdef;
  तार्किक has_glyph_classes;
  अचर hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  तार्किक buffer_is_reversed = असत्य;
  // Caches
  तार्किक using_buffer_glyph_set = असत्य;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  अचर hb_bit_set_t *first_set = nullptr;
  अचर hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  अचिन्हित पूर्णाङ्क lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (अचर hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL शून्य set_ankr_table (अचर AAT::ankr *ankr_table_);

  शून्य set_lookup_index (अचिन्हित पूर्णाङ्क i) { lookup_index = i; }

  शून्य reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  शून्य setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    यदि (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  तार्किक buffer_intersects_machine () अचर
  {
    यदि (likely (using_buffer_glyph_set))
      फर्काऊ buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    लागि (अचिन्हित i = 0; i < buffer->len; i++)
      यदि (first_set->has (buffer->info[i].codepoint))
	फर्काऊ सत्य;
    फर्काऊ असत्य;
  }

  ढाँचा <typename T>
  HB_NODISCARD तार्किक output_glyphs (अचिन्हित पूर्णाङ्क गन्ती,
				   अचर T *glyphs)
  {
    यदि (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, गन्ती);
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      तार्किक at_end_of_text = buffer->idx == buffer->len;
      यदि (at_end_of_text)
	यदि (unlikely (!buffer->output_glyph (glyphs[i]))) फर्काऊ असत्य;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      यदि (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      अन्यथा
      {
#ifndef HB_NO_OT_LAYOUT
	यदि (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      यदि (!at_end_of_text)
	यदि (unlikely (!buffer->output_glyph (glyphs[i]))) फर्काऊ असत्य;
    }
    फर्काऊ सत्य;
  }

  HB_NODISCARD तार्किक replace_glyph (hb_codepoint_t glyph)
  {
    यदि (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    यदि (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    यदि (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    फर्काऊ buffer->replace_glyph (glyph);
  }

  HB_NODISCARD तार्किक delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    यदि (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    फर्काऊ buffer->replace_glyph (DELETED_GLYPH);
  }

  शून्य replace_glyph_inplace (अचिन्हित i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    यदि (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    यदि (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    यदि (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->info[i],
				      gdef.get_glyph_props (glyph));
#endif
  }
};


/*
 * Lookup Table
 */

ढाँचा <typename T> संरचना Lookup;

ढाँचा <typename T>
संरचना LookupFormat0
{
  मित्र संरचना Lookup<T>;

  गुप्त:
  अचर T* get_मान (hb_codepoint_t glyph_id, अचिन्हित पूर्णाङ्क अङ्क_glyphs) अचर
  {
    यदि (unlikely (glyph_id >= अङ्क_glyphs)) फर्काऊ nullptr;
    फर्काऊ &arrayZ[glyph_id];
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, अचिन्हित अङ्क_glyphs) अचर
  {
    glyphs.add_range (0, अङ्क_glyphs - 1);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचिन्हित अङ्क_glyphs, अचर filter_t &filter) अचर
  {
    लागि (अचिन्हित i = 0; i < अङ्क_glyphs; i++)
      यदि (filter (arrayZ[i]))
	glyphs.add (i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (arrayZ.sanitize (c, c->get_अङ्क_glyphs ()));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (arrayZ.sanitize (c, c->get_अङ्क_glyphs (), base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  सार्वजनिक:
  DEFINE_SIZE_UNBOUNDED (2);
};


ढाँचा <typename T>
संरचना LookupSegmentSingle
{
  स्थिर constexpr अचिन्हित Termination_Word_गन्ती = 2u;

  पूर्णाङ्क cmp (hb_codepoint_t g) अचर
  { फर्काऊ g < first ? -1 : g <= last ? 0 : +1 ; }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) अचर
  {
    यदि (first == DELETED_GLYPH) फर्काऊ;
    glyphs.add_range (first, last);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचर filter_t &filter) अचर
  {
    यदि (first == DELETED_GLYPH) फर्काऊ;
    यदि (!filter (value)) फर्काऊ;
    glyphs.add_range (first, last);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
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

ढाँचा <typename T>
संरचना LookupFormat2
{
  मित्र संरचना Lookup<T>;

  गुप्त:
  अचर T* get_मान (hb_codepoint_t glyph_id) अचर
  {
    अचर LookupSegmentSingle<T> *v = segments.द्वा_खोज (glyph_id);
    फर्काऊ v ? &v->value : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) अचर
  {
    अचिन्हित गन्ती = segments.get_length ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      segments[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचर filter_t &filter) अचर
  {
    अचिन्हित गन्ती = segments.get_length ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (segments.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (segments.sanitize (c, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_खोज_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, segments);
};

ढाँचा <typename T>
संरचना LookupSegmentArray
{
  स्थिर constexpr अचिन्हित Termination_Word_गन्ती = 2u;

  अचर T* get_मान (hb_codepoint_t glyph_id, अचर शून्य *base) अचर
  {
    फर्काऊ first <= glyph_id && glyph_id <= last ? &(base+मानहरू_Z)[glyph_id - first] : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) अचर
  {
    यदि (first == DELETED_GLYPH) फर्काऊ;
    glyphs.add_range (first, last);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचर शून्य *base, अचर filter_t &filter) अचर
  {
    यदि (first == DELETED_GLYPH) फर्काऊ;
    अचर स्वतः &values = base+मानहरू_Z;
    लागि (hb_codepoint_t i = first; i <= last; i++)
      यदि (filter (values[i - first]))
	glyphs.add (i);
  }

  पूर्णाङ्क cmp (hb_codepoint_t g) अचर
  { फर्काऊ g < first ? -1 : g <= last ? 0 : +1; }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  first <= last &&
		  मानहरू_Z.sanitize (c, base, last - first + 1));
  }
  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base, Ts&&... ds) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  first <= last &&
		  मानहरू_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		मानहरू_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

ढाँचा <typename T>
संरचना LookupFormat4
{
  मित्र संरचना Lookup<T>;

  गुप्त:
  अचर T* get_मान (hb_codepoint_t glyph_id) अचर
  {
    अचर LookupSegmentArray<T> *v = segments.द्वा_खोज (glyph_id);
    फर्काऊ v ? v->get_मान (glyph_id, एतत्) : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) अचर
  {
    अचिन्हित गन्ती = segments.get_length ();
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      segments[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचर filter_t &filter) अचर
  {
    अचिन्हित गन्ती = segments.get_length ();
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      segments[i].collect_glyphs_filtered (glyphs, एतत्, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (segments.sanitize (c, एतत्));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (segments.sanitize (c, एतत्, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_खोज_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, segments);
};

ढाँचा <typename T>
संरचना LookupSingle
{
  स्थिर constexpr अचिन्हित Termination_Word_गन्ती = 1u;

  पूर्णाङ्क cmp (hb_codepoint_t g) अचर { फर्काऊ glyph.cmp (g); }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) अचर
  {
    यदि (glyph == DELETED_GLYPH) फर्काऊ;
    glyphs.add (glyph);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचर filter_t &filter) अचर
  {
    यदि (glyph == DELETED_GLYPH) फर्काऊ;
    यदि (!filter (value)) फर्काऊ;
    glyphs.add (glyph);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 + T::static_size);
};

ढाँचा <typename T>
संरचना LookupFormat6
{
  मित्र संरचना Lookup<T>;

  गुप्त:
  अचर T* get_मान (hb_codepoint_t glyph_id) अचर
  {
    अचर LookupSingle<T> *v = entries.द्वा_खोज (glyph_id);
    फर्काऊ v ? &v->value : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) अचर
  {
    अचिन्हित गन्ती = entries.get_length ();
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      entries[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचर filter_t &filter) अचर
  {
    अचिन्हित गन्ती = entries.get_length ();
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (entries.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (entries.sanitize (c, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_खोज_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, entries);
};

ढाँचा <typename T>
संरचना LookupFormat8
{
  मित्र संरचना Lookup<T>;

  गुप्त:
  अचर T* get_मान (hb_codepoint_t glyph_id) अचर
  {
    फर्काऊ firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_गन्ती_2 ?
	   &मान_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) अचर
  {
    यदि (unlikely (!glyph_गन्ती_2)) फर्काऊ;
    यदि (firstGlyph == DELETED_GLYPH) फर्काऊ;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_गन्ती_2 - 1);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचर filter_t &filter) अचर
  {
    यदि (unlikely (!glyph_गन्ती_2)) फर्काऊ;
    यदि (firstGlyph == DELETED_GLYPH) फर्काऊ;
    अचर T *p = मान_Array_Z.arrayZ;
    लागि (अचिन्हित i = 0; i < glyph_गन्ती_2; i++)
      यदि (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && मान_Array_Z.sanitize (c, glyph_गन्ती_2));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && मान_Array_Z.sanitize (c, glyph_गन्ती_2, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_गन्ती_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		मान_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, मान_Array_Z);
};

ढाँचा <typename T>
संरचना LookupFormat10
{
  मित्र संरचना Lookup<T>;

  गुप्त:
  अचर typename T::type get_मान_or_null (hb_codepoint_t glyph_id) अचर
  {
    यदि (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_गन्ती_2))
      फर्काऊ Null (T);

    अचर HBUINT8 *p = &मान_Array_Z[(glyph_id - firstGlyph) * मान_Size];

    अचिन्हित पूर्णाङ्क v = 0;
    अचिन्हित पूर्णाङ्क गन्ती = मान_Size;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      v = (v << 8) | *p++;

    फर्काऊ v;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) अचर
  {
    यदि (unlikely (!glyph_गन्ती_2)) फर्काऊ;
    यदि (firstGlyph == DELETED_GLYPH) फर्काऊ;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_गन्ती_2 - 1);
  }

  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचर filter_t &filter) अचर
  {
    यदि (unlikely (!glyph_गन्ती_2)) फर्काऊ;
    यदि (firstGlyph == DELETED_GLYPH) फर्काऊ;
    अचर HBUINT8 *p = मान_Array_Z.arrayZ;
    लागि (अचिन्हित i = 0; i < glyph_गन्ती_2; i++)
    {
      अचिन्हित पूर्णाङ्क v = 0;
      अचिन्हित पूर्णाङ्क गन्ती = मान_Size;
      लागि (अचिन्हित पूर्णाङ्क j = 0; j < गन्ती; j++)
	v = (v << 8) | *p++;
      यदि (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  मान_Size <= 4 &&
		  मान_Array_Z.sanitize (c, glyph_गन्ती_2 * मान_Size));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	मान_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_गन्ती_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		मान_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, मान_Array_Z);
};

ढाँचा <typename T>
संरचना Lookup
{
  अचर T* get_मान (hb_codepoint_t glyph_id, अचिन्हित पूर्णाङ्क अङ्क_glyphs) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); फर्काऊ u.format0.get_मान (glyph_id, अङ्क_glyphs);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.get_मान (glyph_id);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.get_मान (glyph_id);
    पक्ष 6: hb_barrier (); फर्काऊ u.format6.get_मान (glyph_id);
    पक्ष 8: hb_barrier (); फर्काऊ u.format8.get_मान (glyph_id);
    स्वभाव:फर्काऊ nullptr;
    }
  }

  अचर typename T::type get_मान_or_null (hb_codepoint_t glyph_id, अचिन्हित पूर्णाङ्क अङ्क_glyphs) अचर
  {
    विकल्प (u.format.v) {
      /* Format 10 cannot return a pointer. */
      पक्ष 10: hb_barrier (); फर्काऊ u.format10.get_मान_or_null (glyph_id);
      स्वभाव:
      अचर T *v = get_मान (glyph_id, अङ्क_glyphs);
      फर्काऊ v ? *v : Null (T);
    }
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, अचिन्हित पूर्णाङ्क अङ्क_glyphs) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); u.format0.collect_glyphs (glyphs, अङ्क_glyphs); फर्काऊ;
    पक्ष 2: hb_barrier (); u.format2.collect_glyphs (glyphs); फर्काऊ;
    पक्ष 4: hb_barrier (); u.format4.collect_glyphs (glyphs); फर्काऊ;
    पक्ष 6: hb_barrier (); u.format6.collect_glyphs (glyphs); फर्काऊ;
    पक्ष 8: hb_barrier (); u.format8.collect_glyphs (glyphs); फर्काऊ;
    पक्ष 10: hb_barrier (); u.format10.collect_glyphs (glyphs); फर्काऊ;
    स्वभाव:फर्काऊ;
    }
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचिन्हित अङ्क_glyphs, अचर filter_t &filter) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, अङ्क_glyphs, filter); फर्काऊ;
    पक्ष 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); फर्काऊ;
    पक्ष 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); फर्काऊ;
    पक्ष 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); फर्काऊ;
    पक्ष 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); फर्काऊ;
    पक्ष 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); फर्काऊ;
    स्वभाव:फर्काऊ;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      अचिन्हित पूर्णाङ्क अङ्क_glyphs,
			      अचिन्हित पूर्णाङ्क outOfRange) अचर
  {
    अचर T *v = get_मान (glyph_id, अङ्क_glyphs);
    फर्काऊ v ? *v : outOfRange;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    पक्ष 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    पक्ष 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    पक्ष 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    स्वभाव:return_trace (सत्य);
    }
  }
  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    पक्ष 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    पक्ष 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    पक्ष 10: return_trace (असत्य); /* We don't support format10 here currently. */
    स्वभाव:return_trace (सत्य);
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

ढाँचा <typename T>
संरचना Entry
{
  // This doesn't seem like it's ever called.
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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

ढाँचा <>
संरचना Entry<शून्य>
{
  // This does seem like it's ever called.
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  CLASS_END_OF_पङ्क्ति = 3,
};

ढाँचा <typename Types, typename Extra>
संरचना StateTable
{
  प्रकारनिर्देश typename Types::HBUINT HBUINT;
  प्रकारनिर्देश typename Types::HBUSHORT HBUSHORT;
  प्रकारनिर्देश typename Types::ClassTypeNarrow ClassType;

  गणना State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_पङ्क्ति = 1,
  };

  ढाँचा <typename set_t, typename table_t>
  शून्य collect_initial_glyphs (set_t &glyphs, अचिन्हित अङ्क_glyphs, अचर table_t &table) अचर
  {
    अचिन्हित अङ्क_classes = nClasses;

    यदि (unlikely (अङ्क_classes > hb_bit_page_t::BITS))
    {
      (एतत्+classTable).collect_glyphs (glyphs, अङ्क_glyphs);
      फर्काऊ;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    लागि (अचिन्हित i = 0; i < अङ्क_classes; i++)
    {
      अचर स्वतः &entry = get_entry (STATE_START_OF_TEXT, i);
      यदि (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	जारी;

      filter.add (i);
    }

    // And glyphs in those classes.

    यदि (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (एतत्+classTable).collect_glyphs_filtered (glyphs, अङ्क_glyphs, filter);
  }

  पूर्णाङ्क new_state (अचिन्हित पूर्णाङ्क newState) अचर
  { फर्काऊ Types::extended ? newState : ((पूर्णाङ्क) newState - (पूर्णाङ्क) stateArrayTable) / (पूर्णाङ्क) nClasses; }

  अचिन्हित पूर्णाङ्क get_class (hb_codepoint_t glyph_id,
			  अचिन्हित पूर्णाङ्क अङ्क_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) अचर
  {
    अचिन्हित klass;
    यदि (cache && cache->get (glyph_id, &klass)) फर्काऊ klass;
    यदि (unlikely (glyph_id == DELETED_GLYPH)) फर्काऊ CLASS_DELETED_GLYPH;
    klass = (एतत्+classTable).get_class (glyph_id, अङ्क_glyphs, CLASS_OUT_OF_BOUNDS);
    यदि (cache) cache->set (glyph_id, klass);
    फर्काऊ klass;
  }

  अचर Entry<Extra> *get_entries () अचर
  { फर्काऊ (एतत्+entryTable).arrayZ; }

  अचर Entry<Extra> &get_entry (पूर्णाङ्क state, अचिन्हित पूर्णाङ्क klass) अचर
  {
    अचिन्हित n_classes = nClasses;
    यदि (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    अचर HBUSHORT *states = (एतत्+stateArrayTable).arrayZ;
    अचर Entry<Extra> *entries = (एतत्+entryTable).arrayZ;

    अचिन्हित पूर्णाङ्क entry = states[state * n_classes + klass];
    DEBUG_सन्देश (APPLY, nullptr, "e%u", entry);

    फर्काऊ entries[entry];
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 अचिन्हित पूर्णाङ्क *अङ्क_entries_out = nullptr) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!(c->check_struct (एतत्) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, एतत्)))) return_trace (असत्य);

    अचर HBUSHORT *states = (एतत्+stateArrayTable).arrayZ;
    अचर Entry<Extra> *entries = (एतत्+entryTable).arrayZ;

    अचिन्हित पूर्णाङ्क अङ्क_classes = nClasses;
    यदि (unlikely (hb_unsigned_mul_overflows (अङ्क_classes, states[0].static_size)))
      return_trace (असत्य);
    अचिन्हित पूर्णाङ्क row_stride = अङ्क_classes * states[0].static_size;

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
    अचिन्हित पूर्णाङ्क अङ्क_entries = 0;

    पूर्णाङ्क state_pos = 0;
    पूर्णाङ्क state_neg = 0;
    अचिन्हित पूर्णाङ्क entry = 0;
    जबसम्म (min_state < state_neg || state_pos <= max_state)
    {
      यदि (min_state < state_neg)
      {
	/* Negative states. */
	यदि (unlikely (hb_unsigned_mul_overflows (min_state, अङ्क_classes)))
	  return_trace (असत्य);
	यदि (unlikely (!c->check_range (&states[min_state * अङ्क_classes],
				       -min_state,
				       row_stride)))
	  return_trace (असत्य);
	यदि ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (असत्य);
	{ /* Sweep new states. */
	  अचर HBUSHORT *stop = &states[min_state * अङ्क_classes];
	  यदि (unlikely (stop > states))
	    return_trace (असत्य);
	  लागि (अचर HBUSHORT *p = states; stop < p; p--)
	    अङ्क_entries = hb_max (अङ्क_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      यदि (state_pos <= max_state)
      {
	/* Positive states. */
	यदि (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (असत्य);
	यदि ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (असत्य);
	{ /* Sweep new states. */
	  यदि (unlikely (hb_unsigned_mul_overflows ((max_state + 1), अङ्क_classes)))
	    return_trace (असत्य);
	  अचर HBUSHORT *stop = &states[(max_state + 1) * अङ्क_classes];
	  यदि (unlikely (stop < states))
	    return_trace (असत्य);
	  लागि (अचर HBUSHORT *p = &states[state_pos * अङ्क_classes]; p < stop; p++)
	    अङ्क_entries = hb_max (अङ्क_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      यदि (unlikely (!c->check_array (entries, अङ्क_entries)))
	return_trace (असत्य);
      यदि ((c->max_ops -= अङ्क_entries - entry) <= 0)
	return_trace (असत्य);
      { /* Sweep new entries. */
	अचर Entry<Extra> *stop = &entries[अङ्क_entries];
	लागि (अचर Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  पूर्णाङ्क newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = अङ्क_entries;
      }
    }

    यदि (अङ्क_entries_out)
      *अङ्क_entries_out = अङ्क_entries;

    return_trace (सत्य);
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

ढाँचा <typename HBUCHAR>
संरचना ClassTable
{
  अचिन्हित पूर्णाङ्क get_class (hb_codepoint_t glyph_id, अचिन्हित पूर्णाङ्क outOfRange) अचर
  {
    अचिन्हित पूर्णाङ्क i = glyph_id - firstGlyph;
    फर्काऊ i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  अचिन्हित पूर्णाङ्क get_class (hb_codepoint_t glyph_id,
			  अचिन्हित पूर्णाङ्क अङ्क_glyphs HB_UNUSED,
			  अचिन्हित पूर्णाङ्क outOfRange) अचर
  {
    फर्काऊ get_class (glyph_id, outOfRange);
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, अचिन्हित अङ्क_glyphs) अचर
  {
    लागि (अचिन्हित i = 0; i < classArray.len; i++)
      यदि (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचिन्हित अङ्क_glyphs, अचर filter_t &filter) अचर
  {
    लागि (अचिन्हित i = 0; i < classArray.len; i++)
      यदि (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित subtable_गन्ती) अचर
  {
    TRACE_SANITIZE (एतत्);

    यदि (unlikely (!c->check_array (&subtableOffsets, subtable_गन्ती)))
      return_trace (असत्य);

    अचिन्हित bytes = (c->get_अङ्क_glyphs () + अक्षर_बिट - 1) / अक्षर_बिट;
    लागि (अचिन्हित i = 0; i < subtable_गन्ती; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      यदि (offset == 0 || offset == 0xFFFFFFFF)
        जारी;
      यदि (unlikely (!subtableOffsets[i].sanitize (c, एतत्, bytes)))
        return_trace (असत्य);
    }

    return_trace (सत्य);
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
  स्थिर constexpr तार्किक extended = असत्य;
  प्रकारनिर्देश HBUINT16 HBUINT;
  प्रकारनिर्देश HBUINT8 HBUSHORT;
  प्रकारनिर्देश ClassTable<HBUINT8> ClassTypeNarrow;
  प्रकारनिर्देश ClassTable<HBUINT16> ClassTypeWide;

  ढाँचा <typename T>
  स्थिर पूर्णाङ्क offsetToIndex (int64_t offset,
			    अचर शून्य *base,
			    अचर T *array)
  {
    int64_t array_offset = (अचर वर्ण *) array - (अचर वर्ण *) base;
    पूर्णाङ्क bad_index = पूर्णांक_उच्च / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    यदि (unlikely (offset < array_offset))
      फर्काऊ bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    यदि (unlikely (index > bad_index))
      फर्काऊ bad_index;
    फर्काऊ index;
  }
  ढाँचा <typename T>
  स्थिर पूर्णाङ्क byteOffsetToIndex (int64_t offset,
				अचर शून्य *base,
				अचर T *array)
  {
    फर्काऊ offsetToIndex (offset, base, array);
  }
  ढाँचा <typename T>
  स्थिर पूर्णाङ्क wordOffsetToIndex (int64_t offset,
				अचर शून्य *base,
				अचर T *array)
  {
    फर्काऊ offsetToIndex (2 * offset, base, array);
  }
};
संरचना ExtendedTypes
{
  स्थिर constexpr तार्किक extended = सत्य;
  प्रकारनिर्देश HBUINT32 HBUINT;
  प्रकारनिर्देश HBUINT16 HBUSHORT;
  प्रकारनिर्देश Lookup<HBUINT16> ClassTypeNarrow;
  प्रकारनिर्देश Lookup<HBUINT16> ClassTypeWide;

  ढाँचा <typename T>
  स्थिर अचिन्हित पूर्णाङ्क offsetToIndex (अचिन्हित पूर्णाङ्क offset,
				     अचर शून्य *base HB_UNUSED,
				     अचर T *array HB_UNUSED)
  {
    फर्काऊ offset;
  }
  ढाँचा <typename T>
  स्थिर अचिन्हित पूर्णाङ्क byteOffsetToIndex (अचिन्हित पूर्णाङ्क offset,
					 अचर शून्य *base HB_UNUSED,
					 अचर T *array HB_UNUSED)
  {
    फर्काऊ offset / 2;
  }
  ढाँचा <typename T>
  स्थिर अचिन्हित पूर्णाङ्क wordOffsetToIndex (अचिन्हित पूर्णाङ्क offset,
					 अचर शून्य *base HB_UNUSED,
					 अचर T *array HB_UNUSED)
  {
    फर्काऊ offset;
  }
};

ढाँचा <typename Types, typename EntryData, typename Flags>
संरचना StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (अचर StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      अङ्क_glyphs (face_->get_अङ्क_glyphs ()) {}

  ढाँचा <typename context_t>
  शून्य drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    यदि (!c->in_place)
      buffer->clear_output ();

    पूर्णाङ्क state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    स्वतः *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    अचर तार्किक start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    लागि (buffer->idx = 0; buffer->successful;)
    {
      अचिन्हित पूर्णाङ्क klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, अङ्क_glyphs, ac->machine_class_cache) :
			   (अचिन्हित) CLASS_END_OF_TEXT;
    resume:
      DEBUG_सन्देश (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      अचर EntryT &entry = machine.get_entry (state, klass);
      अचर पूर्णाङ्क next_state = machine.new_state (entry.newState);

      तार्किक is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      तार्किक is_not_actionable = !c->table->is_actionable (entry);

      यदि (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	स्वतः *range = last_range;
	यदि (buffer->idx < buffer->len)
	{
	  अचिन्हित cluster = buffer->cur().cluster;
	  जबसम्म (cluster < range->cluster_first)
	    range--;
	  जबसम्म (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	यदि (!(range->flags & ac->subtable_flags))
	{
	  यदि (buffer->idx == buffer->len)
	    रोक;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (शून्य) buffer->next_glyph ();
	  जारी;
	}
      }
      अन्यथा
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	तार्किक is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  next_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	यदि (is_null_transition)
	{
	  अचिन्हित old_klass = klass;
	  गर
	  {
	    c->transition (buffer, एतत्, entry);

	    यदि (buffer->idx == buffer->len || !buffer->successful)
	      रोक;

	    (शून्य) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, अङ्क_glyphs, ac->machine_class_cache) :
		     (अचिन्हित) CLASS_END_OF_TEXT;
	  } जबसम्म (klass == old_klass);

	  यदि (buffer->idx == buffer->len || !buffer->successful)
	    रोक;

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
      अचर EntryT *wouldbe_entry;
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

      यदि (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, एतत्, entry);

      state = next_state;
      DEBUG_सन्देश (APPLY, nullptr, "s%d", state);

      यदि (buffer->idx == buffer->len)
	रोक;

      यदि (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(शून्य) buffer->next_glyph ();
    }

    यदि (!c->in_place)
      buffer->समकाल ();
  }

  सार्वजनिक:
  अचर StateTableT &machine;
  अचिन्हित पूर्णाङ्क अङ्क_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
