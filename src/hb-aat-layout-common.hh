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
  hb_aat_scratch_t () = पूर्वनिर्धारित;
  hb_aat_scratch_t (स्थिरांक hb_aat_scratch_t &) = delete;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & चालक = (hb_aat_scratch_t &&o)
  {
    buffer_glyph_set.set_relaxed (o.buffer_glyph_set.get_relaxed ());
    o.buffer_glyph_set.set_relaxed (nullptr);
    परत *हे;
  }
  ~hb_aat_scratch_t ()
  {
    स्वतः *s = buffer_glyph_set.get_relaxed ();
    जर (unlikely (!s))
      परत;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_buffer_glyph_set () स्थिरांक
  {
    hb_bit_set_t *s = buffer_glyph_set.get_acquire ();
    जर (s && buffer_glyph_set.cmpexch (s, nullptr))
    {
      s->clear ();
      परत s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, आकार (hb_bit_set_t));
    जर (unlikely (!s))
      परत nullptr;
    s->init ();

    परत s;
  }
  शून्य destroy_buffer_glyph_set (hb_bit_set_t *s) स्थिरांक
  {
    जर (unlikely (!s))
      परत;
    जर (buffer_glyph_set.cmpexch (nullptr, s))
      परत;
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
  स्थिरांक अक्षर *get_name () { परत "APPLY"; }
  ढाँचा <typename T, typename ...Ts>
  return_t dispatch (स्थिरांक T &obj, Ts&&... ds)
  { परत obj.apply (हे, std::forward<Ts> (ds)...); }
  स्थिर return_t default_return_मूल्य () { परत असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) स्थिरांक { परत r; }

  स्थिरांक hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  स्थिरांक ankr *ankr_table;
  स्थिरांक OT::GDEF &gdef;
  तार्किक has_glyph_classes;
  स्थिरांक hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  तार्किक buffer_is_reversed = असत्य;
  // Caches
  तार्किक using_buffer_glyph_set = असत्य;
  hb_bit_set_t *buffer_glyph_set = nullptr;
  स्थिरांक hb_bit_set_t *first_set = nullptr;
  स्थिरांक hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  अचिन्हित पूर्णांक lookup_index;

  HB_INTERNAL hb_aat_apply_context_t (स्थिरांक hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_buffer_t *buffer_,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL शून्य set_ankr_table (स्थिरांक AAT::ankr *ankr_table_);

  शून्य set_lookup_index (अचिन्हित पूर्णांक i) { lookup_index = i; }

  शून्य reverse_buffer ()
  {
    buffer->reverse ();
    buffer_is_reversed = !buffer_is_reversed;
  }

  शून्य setup_buffer_glyph_set ()
  {
    using_buffer_glyph_set = buffer->len >= 4 && buffer_glyph_set;

    जर (likely (using_buffer_glyph_set))
      buffer->collect_codepoints (*buffer_glyph_set);
  }
  तार्किक buffer_intersects_machine () स्थिरांक
  {
    जर (likely (using_buffer_glyph_set))
      परत buffer_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    साठी (अचिन्हित i = 0; i < buffer->len; i++)
      जर (first_set->has (buffer->info[i].codepoint))
	परत सत्य;
    परत असत्य;
  }

  ढाँचा <typename T>
  HB_NODISCARD तार्किक output_glyphs (अचिन्हित पूर्णांक मोजणी,
				   स्थिरांक T *glyphs)
  {
    जर (likely (using_buffer_glyph_set))
      buffer_glyph_set->add_array (glyphs, मोजणी);
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      तार्किक at_end_of_text = buffer->idx == buffer->len;
      जर (at_end_of_text)
	जर (unlikely (!buffer->output_glyph (glyphs[i]))) परत असत्य;

      hb_glyph_info_t &info = at_end_of_text ? buffer->prev () : buffer->cur ();
      जर (glyphs[i] == DELETED_GLYPH)
      {
        buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      अन्यथा
      {
#ifndef HB_NO_OT_LAYOUT
	जर (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#endif
      }
      जर (!at_end_of_text)
	जर (unlikely (!buffer->output_glyph (glyphs[i]))) परत असत्य;
    }
    परत सत्य;
  }

  HB_NODISCARD तार्किक replace_glyph (hb_codepoint_t glyph)
  {
    जर (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->cur());
    }

    जर (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    जर (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&buffer->cur(),
				      gdef.get_glyph_props (glyph));
#endif
    परत buffer->replace_glyph (glyph);
  }

  HB_NODISCARD तार्किक delete_glyph ()
  {
    buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&buffer->cur());
    जर (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (DELETED_GLYPH);
    परत buffer->replace_glyph (DELETED_GLYPH);
  }

  शून्य replace_glyph_inplace (अचिन्हित i, hb_codepoint_t glyph)
  {
    buffer->info[i].codepoint = glyph;
    जर (glyph == DELETED_GLYPH)
    {
      buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&buffer->info[i]);
    }
    जर (likely (using_buffer_glyph_set))
      buffer_glyph_set->add (glyph);
#ifndef HB_NO_OT_LAYOUT
    जर (has_glyph_classes)
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

  खाजगी:
  स्थिरांक T* get_मूल्य (hb_codepoint_t glyph_id, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    जर (unlikely (glyph_id >= अंक_glyphs)) परत nullptr;
    परत &arrayZ[glyph_id];
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, अचिन्हित अंक_glyphs) स्थिरांक
  {
    glyphs.add_range (0, अंक_glyphs - 1);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचिन्हित अंक_glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    साठी (अचिन्हित i = 0; i < अंक_glyphs; i++)
      जर (filter (arrayZ[i]))
	glyphs.add (i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (arrayZ.sanitize (c, c->get_अंक_glyphs ()));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (arrayZ.sanitize (c, c->get_अंक_glyphs (), base));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  सार्वजनिक:
  DEFINE_SIZE_UNBOUNDED (2);
};


ढाँचा <typename T>
संरचना LookupSegmentSingle
{
  स्थिर constexpr अचिन्हित Termination_Word_मोजणी = 2u;

  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक
  { परत g < first ? -1 : g <= last ? 0 : +1 ; }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) स्थिरांक
  {
    जर (first == DELETED_GLYPH) परत;
    glyphs.add_range (first, last);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    जर (first == DELETED_GLYPH) परत;
    जर (!filter (value)) परत;
    glyphs.add_range (first, last);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && value.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && value.sanitize (c, base));
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

  खाजगी:
  स्थिरांक T* get_मूल्य (hb_codepoint_t glyph_id) स्थिरांक
  {
    स्थिरांक LookupSegmentSingle<T> *v = segments.द्वा_खोज (glyph_id);
    परत v ? &v->value : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) स्थिरांक
  {
    अचिन्हित मोजणी = segments.get_length ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      segments[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    अचिन्हित मोजणी = segments.get_length ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (segments.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (segments.sanitize (c, base));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_शोध_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, segments);
};

ढाँचा <typename T>
संरचना LookupSegmentArray
{
  स्थिर constexpr अचिन्हित Termination_Word_मोजणी = 2u;

  स्थिरांक T* get_मूल्य (hb_codepoint_t glyph_id, स्थिरांक शून्य *base) स्थिरांक
  {
    परत first <= glyph_id && glyph_id <= last ? &(base+मूल्ये_Z)[glyph_id - first] : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) स्थिरांक
  {
    जर (first == DELETED_GLYPH) परत;
    glyphs.add_range (first, last);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, स्थिरांक शून्य *base, स्थिरांक filter_t &filter) स्थिरांक
  {
    जर (first == DELETED_GLYPH) परत;
    स्थिरांक स्वतः &values = base+मूल्ये_Z;
    साठी (hb_codepoint_t i = first; i <= last; i++)
      जर (filter (values[i - first]))
	glyphs.add (i);
  }

  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक
  { परत g < first ? -1 : g <= last ? 0 : +1; }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  first <= last &&
		  मूल्ये_Z.sanitize (c, base, last - first + 1));
  }
  ढाँचा <typename ...Ts>
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base, Ts&&... ds) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  first <= last &&
		  मूल्ये_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		मूल्ये_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

ढाँचा <typename T>
संरचना LookupFormat4
{
  मित्र संरचना Lookup<T>;

  खाजगी:
  स्थिरांक T* get_मूल्य (hb_codepoint_t glyph_id) स्थिरांक
  {
    स्थिरांक LookupSegmentArray<T> *v = segments.द्वा_खोज (glyph_id);
    परत v ? v->get_मूल्य (glyph_id, हे) : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) स्थिरांक
  {
    अचिन्हित मोजणी = segments.get_length ();
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      segments[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    अचिन्हित मोजणी = segments.get_length ();
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      segments[i].collect_glyphs_filtered (glyphs, हे, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (segments.sanitize (c, हे));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (segments.sanitize (c, हे, base));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_शोध_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, segments);
};

ढाँचा <typename T>
संरचना LookupSingle
{
  स्थिर constexpr अचिन्हित Termination_Word_मोजणी = 1u;

  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक { परत glyph.cmp (g); }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) स्थिरांक
  {
    जर (glyph == DELETED_GLYPH) परत;
    glyphs.add (glyph);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    जर (glyph == DELETED_GLYPH) परत;
    जर (!filter (value)) परत;
    glyphs.add (glyph);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && value.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && value.sanitize (c, base));
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

  खाजगी:
  स्थिरांक T* get_मूल्य (hb_codepoint_t glyph_id) स्थिरांक
  {
    स्थिरांक LookupSingle<T> *v = entries.द्वा_खोज (glyph_id);
    परत v ? &v->value : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) स्थिरांक
  {
    अचिन्हित मोजणी = entries.get_length ();
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      entries[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    अचिन्हित मोजणी = entries.get_length ();
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (entries.sanitize (c));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (entries.sanitize (c, base));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_शोध_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, entries);
};

ढाँचा <typename T>
संरचना LookupFormat8
{
  मित्र संरचना Lookup<T>;

  खाजगी:
  स्थिरांक T* get_मूल्य (hb_codepoint_t glyph_id) स्थिरांक
  {
    परत firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_मोजणी_2 ?
	   &मूल्य_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) स्थिरांक
  {
    जर (unlikely (!glyph_मोजणी_2)) परत;
    जर (firstGlyph == DELETED_GLYPH) परत;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_मोजणी_2 - 1);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    जर (unlikely (!glyph_मोजणी_2)) परत;
    जर (firstGlyph == DELETED_GLYPH) परत;
    स्थिरांक T *p = मूल्य_Array_Z.arrayZ;
    साठी (अचिन्हित i = 0; i < glyph_मोजणी_2; i++)
      जर (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && मूल्य_Array_Z.sanitize (c, glyph_मोजणी_2));
  }
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && मूल्य_Array_Z.sanitize (c, glyph_मोजणी_2, base));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_मोजणी_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		मूल्य_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, मूल्य_Array_Z);
};

ढाँचा <typename T>
संरचना LookupFormat10
{
  मित्र संरचना Lookup<T>;

  खाजगी:
  स्थिरांक typename T::type get_मूल्य_or_null (hb_codepoint_t glyph_id) स्थिरांक
  {
    जर (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_मोजणी_2))
      परत Null (T);

    स्थिरांक HBUINT8 *p = &मूल्य_Array_Z[(glyph_id - firstGlyph) * मूल्य_Size];

    अचिन्हित पूर्णांक v = 0;
    अचिन्हित पूर्णांक मोजणी = मूल्य_Size;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      v = (v << 8) | *p++;

    परत v;
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs) स्थिरांक
  {
    जर (unlikely (!glyph_मोजणी_2)) परत;
    जर (firstGlyph == DELETED_GLYPH) परत;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_मोजणी_2 - 1);
  }

  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    जर (unlikely (!glyph_मोजणी_2)) परत;
    जर (firstGlyph == DELETED_GLYPH) परत;
    स्थिरांक HBUINT8 *p = मूल्य_Array_Z.arrayZ;
    साठी (अचिन्हित i = 0; i < glyph_मोजणी_2; i++)
    {
      अचिन्हित पूर्णांक v = 0;
      अचिन्हित पूर्णांक मोजणी = मूल्य_Size;
      साठी (अचिन्हित पूर्णांक j = 0; j < मोजणी; j++)
	v = (v << 8) | *p++;
      जर (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  मूल्य_Size <= 4 &&
		  मूल्य_Array_Z.sanitize (c, glyph_मोजणी_2 * मूल्य_Size));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	मूल्य_Size;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_मोजणी_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		मूल्य_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, मूल्य_Array_Z);
};

ढाँचा <typename T>
संरचना Lookup
{
  स्थिरांक T* get_मूल्य (hb_codepoint_t glyph_id, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); परत u.format0.get_मूल्य (glyph_id, अंक_glyphs);
    स्थिती 2: hb_barrier (); परत u.format2.get_मूल्य (glyph_id);
    स्थिती 4: hb_barrier (); परत u.format4.get_मूल्य (glyph_id);
    स्थिती 6: hb_barrier (); परत u.format6.get_मूल्य (glyph_id);
    स्थिती 8: hb_barrier (); परत u.format8.get_मूल्य (glyph_id);
    पूर्वनिर्धारित:परत nullptr;
    }
  }

  स्थिरांक typename T::type get_मूल्य_or_null (hb_codepoint_t glyph_id, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    निवड (u.format.v) {
      /* Format 10 cannot return a pointer. */
      स्थिती 10: hb_barrier (); परत u.format10.get_मूल्य_or_null (glyph_id);
      पूर्वनिर्धारित:
      स्थिरांक T *v = get_मूल्य (glyph_id, अंक_glyphs);
      परत v ? *v : Null (T);
    }
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, अचिन्हित पूर्णांक अंक_glyphs) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); u.format0.collect_glyphs (glyphs, अंक_glyphs); परत;
    स्थिती 2: hb_barrier (); u.format2.collect_glyphs (glyphs); परत;
    स्थिती 4: hb_barrier (); u.format4.collect_glyphs (glyphs); परत;
    स्थिती 6: hb_barrier (); u.format6.collect_glyphs (glyphs); परत;
    स्थिती 8: hb_barrier (); u.format8.collect_glyphs (glyphs); परत;
    स्थिती 10: hb_barrier (); u.format10.collect_glyphs (glyphs); परत;
    पूर्वनिर्धारित:परत;
    }
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचिन्हित अंक_glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, अंक_glyphs, filter); परत;
    स्थिती 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); परत;
    स्थिती 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); परत;
    स्थिती 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); परत;
    स्थिती 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); परत;
    स्थिती 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); परत;
    पूर्वनिर्धारित:परत;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      अचिन्हित पूर्णांक अंक_glyphs,
			      अचिन्हित पूर्णांक outOfRange) स्थिरांक
  {
    स्थिरांक T *v = get_मूल्य (glyph_id, अंक_glyphs);
    परत v ? *v : outOfRange;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    स्थिती 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    स्थिती 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    स्थिती 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    स्थिती 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    स्थिती 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }
  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    स्थिती 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    स्थिती 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    स्थिती 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    स्थिती 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    स्थिती 10: return_trace (असत्य); /* We don't support format10 here currently. */
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  संरक्षित:
  संघ {
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
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_size, "");

    return_trace (c->check_struct (हे));
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
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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
  CLASS_END_OF_ओळ = 3,
};

ढाँचा <typename Types, typename Extra>
संरचना StateTable
{
  प्रकारव्याख्या typename Types::HBUINT HBUINT;
  प्रकारव्याख्या typename Types::HBUSHORT HBUSHORT;
  प्रकारव्याख्या typename Types::ClassTypeNarrow ClassType;

  गणना State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_ओळ = 1,
  };

  ढाँचा <typename set_t, typename table_t>
  शून्य collect_initial_glyphs (set_t &glyphs, अचिन्हित अंक_glyphs, स्थिरांक table_t &table) स्थिरांक
  {
    अचिन्हित अंक_classes = nClasses;

    जर (unlikely (अंक_classes > hb_bit_page_t::BITS))
    {
      (हे+classTable).collect_glyphs (glyphs, अंक_glyphs);
      परत;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    साठी (अचिन्हित i = 0; i < अंक_classes; i++)
    {
      स्थिरांक स्वतः &entry = get_entry (STATE_START_OF_TEXT, i);
      जर (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	सुरूठेवा;

      filter.add (i);
    }

    // And glyphs in those classes.

    जर (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (हे+classTable).collect_glyphs_filtered (glyphs, अंक_glyphs, filter);
  }

  पूर्णांक new_state (अचिन्हित पूर्णांक newState) स्थिरांक
  { परत Types::extended ? newState : ((पूर्णांक) newState - (पूर्णांक) stateArrayTable) / (पूर्णांक) nClasses; }

  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id,
			  अचिन्हित पूर्णांक अंक_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) स्थिरांक
  {
    अचिन्हित klass;
    जर (cache && cache->get (glyph_id, &klass)) परत klass;
    जर (unlikely (glyph_id == DELETED_GLYPH)) परत CLASS_DELETED_GLYPH;
    klass = (हे+classTable).get_class (glyph_id, अंक_glyphs, CLASS_OUT_OF_BOUNDS);
    जर (cache) cache->set (glyph_id, klass);
    परत klass;
  }

  स्थिरांक Entry<Extra> *get_entries () स्थिरांक
  { परत (हे+entryTable).arrayZ; }

  स्थिरांक Entry<Extra> &get_entry (पूर्णांक state, अचिन्हित पूर्णांक klass) स्थिरांक
  {
    अचिन्हित n_classes = nClasses;
    जर (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    स्थिरांक HBUSHORT *states = (हे+stateArrayTable).arrayZ;
    स्थिरांक Entry<Extra> *entries = (हे+entryTable).arrayZ;

    अचिन्हित पूर्णांक entry = states[state * n_classes + klass];
    DEBUG_संदेश (APPLY, nullptr, "e%u", entry);

    परत entries[entry];
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 अचिन्हित पूर्णांक *अंक_entries_out = nullptr) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!(c->check_struct (हे) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, हे)))) return_trace (असत्य);

    स्थिरांक HBUSHORT *states = (हे+stateArrayTable).arrayZ;
    स्थिरांक Entry<Extra> *entries = (हे+entryTable).arrayZ;

    अचिन्हित पूर्णांक अंक_classes = nClasses;
    जर (unlikely (hb_unsigned_mul_overflows (अंक_classes, states[0].static_size)))
      return_trace (असत्य);
    अचिन्हित पूर्णांक row_stride = अंक_classes * states[0].static_size;

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

    पूर्णांक min_state = 0;
    पूर्णांक max_state = 0;
    अचिन्हित पूर्णांक अंक_entries = 0;

    पूर्णांक state_pos = 0;
    पूर्णांक state_neg = 0;
    अचिन्हित पूर्णांक entry = 0;
    पर्यंत (min_state < state_neg || state_pos <= max_state)
    {
      जर (min_state < state_neg)
      {
	/* Negative states. */
	जर (unlikely (hb_unsigned_mul_overflows (min_state, अंक_classes)))
	  return_trace (असत्य);
	जर (unlikely (!c->check_range (&states[min_state * अंक_classes],
				       -min_state,
				       row_stride)))
	  return_trace (असत्य);
	जर ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (असत्य);
	{ /* Sweep new states. */
	  स्थिरांक HBUSHORT *stop = &states[min_state * अंक_classes];
	  जर (unlikely (stop > states))
	    return_trace (असत्य);
	  साठी (स्थिरांक HBUSHORT *p = states; stop < p; p--)
	    अंक_entries = hb_max (अंक_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      जर (state_pos <= max_state)
      {
	/* Positive states. */
	जर (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (असत्य);
	जर ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (असत्य);
	{ /* Sweep new states. */
	  जर (unlikely (hb_unsigned_mul_overflows ((max_state + 1), अंक_classes)))
	    return_trace (असत्य);
	  स्थिरांक HBUSHORT *stop = &states[(max_state + 1) * अंक_classes];
	  जर (unlikely (stop < states))
	    return_trace (असत्य);
	  साठी (स्थिरांक HBUSHORT *p = &states[state_pos * अंक_classes]; p < stop; p++)
	    अंक_entries = hb_max (अंक_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      जर (unlikely (!c->check_array (entries, अंक_entries)))
	return_trace (असत्य);
      जर ((c->max_ops -= अंक_entries - entry) <= 0)
	return_trace (असत्य);
      { /* Sweep new entries. */
	स्थिरांक Entry<Extra> *stop = &entries[अंक_entries];
	साठी (स्थिरांक Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  पूर्णांक newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = अंक_entries;
      }
    }

    जर (अंक_entries_out)
      *अंक_entries_out = अंक_entries;

    return_trace (सत्य);
  }

  संरक्षित:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffsetTo<ClassType, HBUINT>
		classTable;	/* Offset to the class table. */
  NNOffsetTo<UnsizedArrayOf<HBUSHORT>, HBUINT>
		stateArrayTable;/* Offset to the state array. */
  NNOffsetTo<UnsizedArrayOf<Entry<Extra>>, HBUINT>
		entryTable;	/* Offset to the entry array. */

  सार्वजनिक:
  DEFINE_SIZE_STATIC (4 * आकार (HBUINT));
};

ढाँचा <typename HBUCHAR>
संरचना ClassTable
{
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id, अचिन्हित पूर्णांक outOfRange) स्थिरांक
  {
    अचिन्हित पूर्णांक i = glyph_id - firstGlyph;
    परत i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id,
			  अचिन्हित पूर्णांक अंक_glyphs HB_UNUSED,
			  अचिन्हित पूर्णांक outOfRange) स्थिरांक
  {
    परत get_class (glyph_id, outOfRange);
  }

  ढाँचा <typename set_t>
  शून्य collect_glyphs (set_t &glyphs, अचिन्हित अंक_glyphs) स्थिरांक
  {
    साठी (अचिन्हित i = 0; i < classArray.len; i++)
      जर (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  ढाँचा <typename set_t, typename filter_t>
  शून्य collect_glyphs_filtered (set_t &glyphs, अचिन्हित अंक_glyphs, स्थिरांक filter_t &filter) स्थिरांक
  {
    साठी (अचिन्हित i = 0; i < classArray.len; i++)
      जर (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && classArray.sanitize (c));
  }
  संरक्षित:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, classArray);
};

संरचना SubtableGlyphCoverage
{
  तार्किक sanitize (hb_sanitize_context_t *c, अचिन्हित subtable_मोजणी) स्थिरांक
  {
    TRACE_SANITIZE (हे);

    जर (unlikely (!c->check_array (&subtableOffsets, subtable_मोजणी)))
      return_trace (असत्य);

    अचिन्हित bytes = (c->get_अंक_glyphs () + अक्षर_बिट - 1) / अक्षर_बिट;
    साठी (अचिन्हित i = 0; i < subtable_मोजणी; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      जर (offset == 0 || offset == 0xFFFFFFFF)
        सुरूठेवा;
      जर (unlikely (!subtableOffsets[i].sanitize (c, हे, bytes)))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }
  संरक्षित:
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
  प्रकारव्याख्या HBUINT16 HBUINT;
  प्रकारव्याख्या HBUINT8 HBUSHORT;
  प्रकारव्याख्या ClassTable<HBUINT8> ClassTypeNarrow;
  प्रकारव्याख्या ClassTable<HBUINT16> ClassTypeWide;

  ढाँचा <typename T>
  स्थिर पूर्णांक offsetToIndex (int64_t offset,
			    स्थिरांक शून्य *base,
			    स्थिरांक T *array)
  {
    int64_t array_offset = (स्थिरांक अक्षर *) array - (स्थिरांक अक्षर *) base;
    पूर्णांक bad_index = पूर्णांक_उच्च / T::static_size;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    जर (unlikely (offset < array_offset))
      परत bad_index;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t index = (offset - array_offset) / T::static_size;
    जर (unlikely (index > bad_index))
      परत bad_index;
    परत index;
  }
  ढाँचा <typename T>
  स्थिर पूर्णांक byteOffsetToIndex (int64_t offset,
				स्थिरांक शून्य *base,
				स्थिरांक T *array)
  {
    परत offsetToIndex (offset, base, array);
  }
  ढाँचा <typename T>
  स्थिर पूर्णांक wordOffsetToIndex (int64_t offset,
				स्थिरांक शून्य *base,
				स्थिरांक T *array)
  {
    परत offsetToIndex (2 * offset, base, array);
  }
};
संरचना ExtendedTypes
{
  स्थिर constexpr तार्किक extended = सत्य;
  प्रकारव्याख्या HBUINT32 HBUINT;
  प्रकारव्याख्या HBUINT16 HBUSHORT;
  प्रकारव्याख्या Lookup<HBUINT16> ClassTypeNarrow;
  प्रकारव्याख्या Lookup<HBUINT16> ClassTypeWide;

  ढाँचा <typename T>
  स्थिर अचिन्हित पूर्णांक offsetToIndex (अचिन्हित पूर्णांक offset,
				     स्थिरांक शून्य *base HB_UNUSED,
				     स्थिरांक T *array HB_UNUSED)
  {
    परत offset;
  }
  ढाँचा <typename T>
  स्थिर अचिन्हित पूर्णांक byteOffsetToIndex (अचिन्हित पूर्णांक offset,
					 स्थिरांक शून्य *base HB_UNUSED,
					 स्थिरांक T *array HB_UNUSED)
  {
    परत offset / 2;
  }
  ढाँचा <typename T>
  स्थिर अचिन्हित पूर्णांक wordOffsetToIndex (अचिन्हित पूर्णांक offset,
					 स्थिरांक शून्य *base HB_UNUSED,
					 स्थिरांक T *array HB_UNUSED)
  {
    परत offset;
  }
};

ढाँचा <typename Types, typename EntryData, typename Flags>
संरचना StateTableDriver
{
  using StateTableT = StateTable<Types, EntryData>;
  using EntryT = Entry<EntryData>;

  StateTableDriver (स्थिरांक StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      अंक_glyphs (face_->get_अंक_glyphs ()) {}

  ढाँचा <typename context_t>
  शून्य drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_buffer_t *buffer = ac->buffer;

    जर (!c->in_place)
      buffer->clear_output ();

    पूर्णांक state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    स्वतः *last_range = ac->range_flags && (ac->range_flags->length > 1) ? &(*ac->range_flags)[0] : nullptr;
    स्थिरांक तार्किक start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    साठी (buffer->idx = 0; buffer->successful;)
    {
      अचिन्हित पूर्णांक klass = likely (buffer->idx < buffer->len) ?
			   machine.get_class (buffer->cur().codepoint, अंक_glyphs, ac->machine_class_cache) :
			   (अचिन्हित) CLASS_END_OF_TEXT;
    resume:
      DEBUG_संदेश (APPLY, nullptr, "c%u at %u", klass, buffer->idx);
      स्थिरांक EntryT &entry = machine.get_entry (state, klass);
      स्थिरांक पूर्णांक next_state = machine.new_state (entry.newState);

      तार्किक is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      तार्किक is_not_actionable = !c->table->is_actionable (entry);

      जर (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	स्वतः *range = last_range;
	जर (buffer->idx < buffer->len)
	{
	  अचिन्हित cluster = buffer->cur().cluster;
	  पर्यंत (cluster < range->cluster_first)
	    range--;
	  पर्यंत (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	जर (!(range->flags & ac->subtable_flags))
	{
	  जर (buffer->idx == buffer->len)
	    थांबा;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (शून्य) buffer->next_glyph ();
	  सुरूठेवा;
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

	जर (is_null_transition)
	{
	  अचिन्हित old_klass = klass;
	  करा
	  {
	    c->transition (buffer, हे, entry);

	    जर (buffer->idx == buffer->len || !buffer->successful)
	      थांबा;

	    (शून्य) buffer->next_glyph ();

	    klass = likely (buffer->idx < buffer->len) ?
		     machine.get_class (buffer->cur().codepoint, अंक_glyphs, ac->machine_class_cache) :
		     (अचिन्हित) CLASS_END_OF_TEXT;
	  } पर्यंत (klass == old_klass);

	  जर (buffer->idx == buffer->len || !buffer->successful)
	    थांबा;

	  जा resume;
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
      स्थिरांक EntryT *wouldbe_entry;
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

      जर (!is_safe_to_break && buffer->backtrack_len () && buffer->idx < buffer->len)
	buffer->unsafe_to_break_from_outbuffer (buffer->backtrack_len () - 1, buffer->idx + 1);

      c->transition (buffer, हे, entry);

      state = next_state;
      DEBUG_संदेश (APPLY, nullptr, "s%d", state);

      जर (buffer->idx == buffer->len)
	थांबा;

      जर (is_not_epsilon_transition || buffer->max_ops-- <= 0)
	(शून्य) buffer->next_glyph ();
    }

    जर (!c->in_place)
      buffer->समकालिक ();
  }

  सार्वजनिक:
  स्थिरांक StateTableT &machine;
  अचिन्हित पूर्णांक अंक_glyphs;
};


} /* namespace AAT */


#endif /* HB_AAT_LAYOUT_COMMON_HH */
