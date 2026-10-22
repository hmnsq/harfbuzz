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

#अगर_अघोषित HB_AAT_LAYOUT_COMMON_HH
#घोषणा HB_AAT_LAYOUT_COMMON_HH

#समावेश "hb-aat-layout.hh"
#समावेश "hb-aat-map.hh"
#समावेश "hb-ot-layout-common.hh"
#समावेश "hb-ot-layout-gdef-table.hh"
#समावेश "hb-open-type.hh"
#समावेश "hb-cache.hh"
#समावेश "hb-bit-set.hh"
#समावेश "hb-bit-page.hh"


namespace OT {
काष्ठा GDEF;
};

namespace AAT {

using namespace OT;

काष्ठा ankr;

using hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#अगर_अघोषित HB_OPTIMIZE_आकार
#घोषणा HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#अन्यथा
#घोषणा HB_AAT_TRANSITION_INLINE
#पूर्ण_अगर

काष्ठा hb_aat_scratch_t
{
  hb_aat_scratch_t () = शेष;
  hb_aat_scratch_t (स्थिर hb_aat_scratch_t &) = मिटाओ;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    बफर_glyph_set.set_relaxed (o.बफर_glyph_set.get_relaxed ());
    o.बफर_glyph_set.set_relaxed (nullptr);
  }
  hb_aat_scratch_t & चालक = (hb_aat_scratch_t &&o)
  {
    बफर_glyph_set.set_relaxed (o.बफर_glyph_set.get_relaxed ());
    o.बफर_glyph_set.set_relaxed (nullptr);
    वापस *यह;
  }
  ~hb_aat_scratch_t ()
  {
    स्वतः *s = बफर_glyph_set.get_relaxed ();
    अगर (unlikely (!s))
      वापस;
    s->fini ();
    hb_free (s);
  }

  hb_bit_set_t *create_बफर_glyph_set () स्थिर
  {
    hb_bit_set_t *s = बफर_glyph_set.get_acquire ();
    अगर (s && बफर_glyph_set.cmpexch (s, nullptr))
    {
      s->सफा ();
      वापस s;
    }

    s = (hb_bit_set_t *) hb_calloc (1, माप (hb_bit_set_t));
    अगर (unlikely (!s))
      वापस nullptr;
    s->init ();

    वापस s;
  }
  व्योम destroy_बफर_glyph_set (hb_bit_set_t *s) स्थिर
  {
    अगर (unlikely (!s))
      वापस;
    अगर (बफर_glyph_set.cmpexch (nullptr, s))
      वापस;
    s->fini ();
    hb_free (s);
  }

  mutable hb_atomic_t<hb_bit_set_t *> बफर_glyph_set;
};

क्रमागत { DELETED_GLYPH = 0xFFFF };

#घोषणा HB_बफर_SCRATCH_FLAG_AAT_HAS_DELETED HB_बफर_SCRATCH_FLAG_SHAPER0

काष्ठा hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, bool, HB_DEBUG_APPLY>
{
  स्थिर अक्षर *get_name () { वापस "APPLY"; }
  ढाँचा <typename T, typename ...Ts>
  return_t dispatch (स्थिर T &obj, Ts&&... ds)
  { वापस obj.apply (यह, std::forward<Ts> (ds)...); }
  अटल return_t default_return_मूल्य () { वापस false; }
  bool stop_sublookup_iteration (return_t r) स्थिर { वापस r; }

  स्थिर hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_बफर_t *बफर;
  hb_sanitize_context_t sanitizer;
  स्थिर ankr *ankr_table;
  स्थिर OT::GDEF &gdef;
  bool has_glyph_classes;
  स्थिर hb_sorted_vector_t<hb_aat_map_t::range_flags_t> *range_flags = nullptr;
  hb_mask_t subtable_flags = 0;
  bool बफर_is_reversed = false;
  // Caches
  bool using_बफर_glyph_set = false;
  hb_bit_set_t *बफर_glyph_set = nullptr;
  स्थिर hb_bit_set_t *first_set = nullptr;
  स्थिर hb_bit_set_t *second_set = nullptr;
  hb_aat_class_cache_t *machine_class_cache = nullptr;

  /* Unused. For debug tracing only. */
  अचिन्हित पूर्णांक lookup_सूचकांक;

  HB_INTERNAL hb_aat_apply_context_t (स्थिर hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_बफर_t *बफर_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_INTERNAL ~hb_aat_apply_context_t ();

  HB_INTERNAL व्योम set_ankr_table (स्थिर AAT::ankr *ankr_table_);

  व्योम set_lookup_सूचकांक (अचिन्हित पूर्णांक i) { lookup_सूचकांक = i; }

  व्योम reverse_बफर ()
  {
    बफर->reverse ();
    बफर_is_reversed = !बफर_is_reversed;
  }

  व्योम setup_बफर_glyph_set ()
  {
    using_बफर_glyph_set = बफर->len >= 4 && बफर_glyph_set;

    अगर (likely (using_बफर_glyph_set))
      बफर->collect_codepoints (*बफर_glyph_set);
  }
  bool बफर_intersects_machine () स्थिर
  {
    अगर (likely (using_बफर_glyph_set))
      वापस बफर_glyph_set->intersects (*first_set);

    // Faster for shorter buffers.
    क्रम (अचिन्हित i = 0; i < बफर->len; i++)
      अगर (first_set->has (बफर->info[i].codepoint))
	वापस true;
    वापस false;
  }

  ढाँचा <typename T>
  HB_NODISCARD bool output_glyphs (अचिन्हित पूर्णांक गिनती,
				   स्थिर T *glyphs)
  {
    अगर (likely (using_बफर_glyph_set))
      बफर_glyph_set->add_array (glyphs, गिनती);
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      bool at_end_of_text = बफर->idx == बफर->len;
      अगर (at_end_of_text)
	अगर (unlikely (!बफर->output_glyph (glyphs[i]))) वापस false;

      hb_glyph_info_t &info = at_end_of_text ? बफर->prev () : बफर->cur ();
      अगर (glyphs[i] == DELETED_GLYPH)
      {
        बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_AAT_HAS_DELETED;
	_hb_glyph_info_set_aat_deleted (&info);
      }
      अन्यथा
      {
#अगर_अघोषित HB_NO_OT_LAYOUT
	अगर (has_glyph_classes)
	  _hb_glyph_info_set_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#पूर्ण_अगर
      }
      अगर (!at_end_of_text)
	अगर (unlikely (!बफर->output_glyph (glyphs[i]))) वापस false;
    }
    वापस true;
  }

  HB_NODISCARD bool replace_glyph (hb_codepoint_t glyph)
  {
    अगर (glyph == DELETED_GLYPH)
    {
      बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&बफर->cur());
    }

    अगर (likely (using_बफर_glyph_set))
      बफर_glyph_set->add (glyph);
#अगर_अघोषित HB_NO_OT_LAYOUT
    अगर (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&बफर->cur(),
				      gdef.get_glyph_props (glyph));
#पूर्ण_अगर
    वापस बफर->replace_glyph (glyph);
  }

  HB_NODISCARD bool delete_glyph ()
  {
    बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_AAT_HAS_DELETED;
    _hb_glyph_info_set_aat_deleted (&बफर->cur());
    अगर (likely (using_बफर_glyph_set))
      बफर_glyph_set->add (DELETED_GLYPH);
    वापस बफर->replace_glyph (DELETED_GLYPH);
  }

  व्योम replace_glyph_inplace (अचिन्हित i, hb_codepoint_t glyph)
  {
    बफर->info[i].codepoint = glyph;
    अगर (glyph == DELETED_GLYPH)
    {
      बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_AAT_HAS_DELETED;
      _hb_glyph_info_set_aat_deleted (&बफर->info[i]);
    }
    अगर (likely (using_बफर_glyph_set))
      बफर_glyph_set->add (glyph);
#अगर_अघोषित HB_NO_OT_LAYOUT
    अगर (has_glyph_classes)
      _hb_glyph_info_set_glyph_props (&बफर->info[i],
				      gdef.get_glyph_props (glyph));
#पूर्ण_अगर
  }
};


/*
 * Lookup Table
 */

ढाँचा <typename T> काष्ठा Lookup;

ढाँचा <typename T>
काष्ठा LookupFormat0
{
  मित्र काष्ठा Lookup<T>;

  निजी:
  स्थिर T* get_मूल्य (hb_codepoint_t glyph_id, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    अगर (unlikely (glyph_id >= अंक_glyphs)) वापस nullptr;
    वापस &arrayZ[glyph_id];
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs, अचिन्हित अंक_glyphs) स्थिर
  {
    glyphs.add_range (0, अंक_glyphs - 1);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, अचिन्हित अंक_glyphs, स्थिर filter_t &filter) स्थिर
  {
    क्रम (अचिन्हित i = 0; i < अंक_glyphs; i++)
      अगर (filter (arrayZ[i]))
	glyphs.add (i);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (arrayZ.sanitize (c, c->get_अंक_glyphs ()));
  }
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (arrayZ.sanitize (c, c->get_अंक_glyphs (), base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 0 */
  UnsizedArrayOf<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  खुला:
  DEFINE_आकार_UNBOUNDED (2);
};


ढाँचा <typename T>
काष्ठा LookupSegmentSingle
{
  अटल constexpr अचिन्हित Termination_Word_गिनती = 2u;

  पूर्णांक cmp (hb_codepoint_t g) स्थिर
  { वापस g < first ? -1 : g <= last ? 0 : +1 ; }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs) स्थिर
  {
    अगर (first == DELETED_GLYPH) वापस;
    glyphs.add_range (first, last);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, स्थिर filter_t &filter) स्थिर
  {
    अगर (first == DELETED_GLYPH) वापस;
    अगर (!filter (value)) वापस;
    glyphs.add_range (first, last);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && value.sanitize (c));
  }
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && value.sanitize (c, base));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  खुला:
  DEFINE_आकार_STATIC (4 + T::static_आकार);
};

ढाँचा <typename T>
काष्ठा LookupFormat2
{
  मित्र काष्ठा Lookup<T>;

  निजी:
  स्थिर T* get_मूल्य (hb_codepoint_t glyph_id) स्थिर
  {
    स्थिर LookupSegmentSingle<T> *v = segments.द्वा_खोज (glyph_id);
    वापस v ? &v->value : nullptr;
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs) स्थिर
  {
    अचिन्हित गिनती = segments.get_लंबाई ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      segments[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, स्थिर filter_t &filter) स्थिर
  {
    अचिन्हित गिनती = segments.get_लंबाई ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (segments.sanitize (c));
  }
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (segments.sanitize (c, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_खोज_Array_Of<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  खुला:
  DEFINE_आकार_ARRAY (8, segments);
};

ढाँचा <typename T>
काष्ठा LookupSegmentArray
{
  अटल constexpr अचिन्हित Termination_Word_गिनती = 2u;

  स्थिर T* get_मूल्य (hb_codepoint_t glyph_id, स्थिर व्योम *base) स्थिर
  {
    वापस first <= glyph_id && glyph_id <= last ? &(base+मूल्यावली_Z)[glyph_id - first] : nullptr;
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs) स्थिर
  {
    अगर (first == DELETED_GLYPH) वापस;
    glyphs.add_range (first, last);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, स्थिर व्योम *base, स्थिर filter_t &filter) स्थिर
  {
    अगर (first == DELETED_GLYPH) वापस;
    स्थिर स्वतः &values = base+मूल्यावली_Z;
    क्रम (hb_codepoint_t i = first; i <= last; i++)
      अगर (filter (values[i - first]))
	glyphs.add (i);
  }

  पूर्णांक cmp (hb_codepoint_t g) स्थिर
  { वापस g < first ? -1 : g <= last ? 0 : +1; }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  first <= last &&
		  मूल्यावली_Z.sanitize (c, base, last - first + 1));
  }
  ढाँचा <typename ...Ts>
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base, Ts&&... ds) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  first <= last &&
		  मूल्यावली_Z.sanitize (c, base, last - first + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	last;		/* Last GlyphID in this segment */
  HBGlyphID16	first;		/* First GlyphID in this segment */
  NNOffset16To<UnsizedArrayOf<T>>
		मूल्यावली_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  खुला:
  DEFINE_आकार_STATIC (6);
};

ढाँचा <typename T>
काष्ठा LookupFormat4
{
  मित्र काष्ठा Lookup<T>;

  निजी:
  स्थिर T* get_मूल्य (hb_codepoint_t glyph_id) स्थिर
  {
    स्थिर LookupSegmentArray<T> *v = segments.द्वा_खोज (glyph_id);
    वापस v ? v->get_मूल्य (glyph_id, यह) : nullptr;
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs) स्थिर
  {
    अचिन्हित गिनती = segments.get_लंबाई ();
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      segments[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, स्थिर filter_t &filter) स्थिर
  {
    अचिन्हित गिनती = segments.get_लंबाई ();
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      segments[i].collect_glyphs_filtered (glyphs, यह, filter);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (segments.sanitize (c, यह));
  }
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (segments.sanitize (c, यह, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_खोज_Array_Of<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  खुला:
  DEFINE_आकार_ARRAY (8, segments);
};

ढाँचा <typename T>
काष्ठा LookupSingle
{
  अटल constexpr अचिन्हित Termination_Word_गिनती = 1u;

  पूर्णांक cmp (hb_codepoint_t g) स्थिर { वापस glyph.cmp (g); }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs) स्थिर
  {
    अगर (glyph == DELETED_GLYPH) वापस;
    glyphs.add (glyph);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, स्थिर filter_t &filter) स्थिर
  {
    अगर (glyph == DELETED_GLYPH) वापस;
    अगर (!filter (value)) वापस;
    glyphs.add (glyph);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && value.sanitize (c));
  }
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && value.sanitize (c, base));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  खुला:
  DEFINE_आकार_STATIC (2 + T::static_आकार);
};

ढाँचा <typename T>
काष्ठा LookupFormat6
{
  मित्र काष्ठा Lookup<T>;

  निजी:
  स्थिर T* get_मूल्य (hb_codepoint_t glyph_id) स्थिर
  {
    स्थिर LookupSingle<T> *v = entries.द्वा_खोज (glyph_id);
    वापस v ? &v->value : nullptr;
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs) स्थिर
  {
    अचिन्हित गिनती = entries.get_लंबाई ();
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      entries[i].collect_glyphs (glyphs);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, स्थिर filter_t &filter) स्थिर
  {
    अचिन्हित गिनती = entries.get_लंबाई ();
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (entries.sanitize (c));
  }
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (entries.sanitize (c, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_खोज_Array_Of<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  खुला:
  DEFINE_आकार_ARRAY (8, entries);
};

ढाँचा <typename T>
काष्ठा LookupFormat8
{
  मित्र काष्ठा Lookup<T>;

  निजी:
  स्थिर T* get_मूल्य (hb_codepoint_t glyph_id) स्थिर
  {
    वापस firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_गिनती_2 ?
	   &मूल्य_Array_Z[glyph_id - firstGlyph] : nullptr;
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs) स्थिर
  {
    अगर (unlikely (!glyph_गिनती_2)) वापस;
    अगर (firstGlyph == DELETED_GLYPH) वापस;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_गिनती_2 - 1);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, स्थिर filter_t &filter) स्थिर
  {
    अगर (unlikely (!glyph_गिनती_2)) वापस;
    अगर (firstGlyph == DELETED_GLYPH) वापस;
    स्थिर T *p = मूल्य_Array_Z.arrayZ;
    क्रम (अचिन्हित i = 0; i < glyph_गिनती_2; i++)
      अगर (filter (p[i]))
	glyphs.add (firstGlyph + i);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && मूल्य_Array_Z.sanitize (c, glyph_गिनती_2));
  }
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && मूल्य_Array_Z.sanitize (c, glyph_गिनती_2, base));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_गिनती_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<T>
		मूल्य_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  खुला:
  DEFINE_आकार_ARRAY (6, मूल्य_Array_Z);
};

ढाँचा <typename T>
काष्ठा LookupFormat10
{
  मित्र काष्ठा Lookup<T>;

  निजी:
  स्थिर typename T::type get_मूल्य_or_null (hb_codepoint_t glyph_id) स्थिर
  {
    अगर (!(firstGlyph <= glyph_id && glyph_id - firstGlyph < glyph_गिनती_2))
      वापस Null (T);

    स्थिर HBUINT8 *p = &मूल्य_Array_Z[(glyph_id - firstGlyph) * मूल्य_आकार];

    अचिन्हित पूर्णांक v = 0;
    अचिन्हित पूर्णांक गिनती = मूल्य_आकार;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      v = (v << 8) | *p++;

    वापस v;
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs) स्थिर
  {
    अगर (unlikely (!glyph_गिनती_2)) वापस;
    अगर (firstGlyph == DELETED_GLYPH) वापस;
    glyphs.add_range (firstGlyph, firstGlyph + glyph_गिनती_2 - 1);
  }

  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, स्थिर filter_t &filter) स्थिर
  {
    अगर (unlikely (!glyph_गिनती_2)) वापस;
    अगर (firstGlyph == DELETED_GLYPH) वापस;
    स्थिर HBUINT8 *p = मूल्य_Array_Z.arrayZ;
    क्रम (अचिन्हित i = 0; i < glyph_गिनती_2; i++)
    {
      अचिन्हित पूर्णांक v = 0;
      अचिन्हित पूर्णांक गिनती = मूल्य_आकार;
      क्रम (अचिन्हित पूर्णांक j = 0; j < गिनती; j++)
	v = (v << 8) | *p++;
      अगर (filter (v))
	glyphs.add (firstGlyph + i);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  मूल्य_आकार <= 4 &&
		  मूल्य_Array_Z.sanitize (c, glyph_गिनती_2 * मूल्य_आकार));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	मूल्य_आकार;	/* Byte size of each value. */
  HBGlyphID16	firstGlyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_गिनती_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  UnsizedArrayOf<HBUINT8>
		मूल्य_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  खुला:
  DEFINE_आकार_ARRAY (8, मूल्य_Array_Z);
};

ढाँचा <typename T>
काष्ठा Lookup
{
  स्थिर T* get_मूल्य (hb_codepoint_t glyph_id, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    चयन (u.format.v) {
    हाल 0: hb_barrier (); वापस u.format0.get_मूल्य (glyph_id, अंक_glyphs);
    हाल 2: hb_barrier (); वापस u.format2.get_मूल्य (glyph_id);
    हाल 4: hb_barrier (); वापस u.format4.get_मूल्य (glyph_id);
    हाल 6: hb_barrier (); वापस u.format6.get_मूल्य (glyph_id);
    हाल 8: hb_barrier (); वापस u.format8.get_मूल्य (glyph_id);
    शेष:वापस nullptr;
    }
  }

  स्थिर typename T::type get_मूल्य_or_null (hb_codepoint_t glyph_id, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    चयन (u.format.v) {
      /* Format 10 cannot return a pointer. */
      हाल 10: hb_barrier (); वापस u.format10.get_मूल्य_or_null (glyph_id);
      शेष:
      स्थिर T *v = get_मूल्य (glyph_id, अंक_glyphs);
      वापस v ? *v : Null (T);
    }
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs, अचिन्हित पूर्णांक अंक_glyphs) स्थिर
  {
    चयन (u.format.v) {
    हाल 0: hb_barrier (); u.format0.collect_glyphs (glyphs, अंक_glyphs); वापस;
    हाल 2: hb_barrier (); u.format2.collect_glyphs (glyphs); वापस;
    हाल 4: hb_barrier (); u.format4.collect_glyphs (glyphs); वापस;
    हाल 6: hb_barrier (); u.format6.collect_glyphs (glyphs); वापस;
    हाल 8: hb_barrier (); u.format8.collect_glyphs (glyphs); वापस;
    हाल 10: hb_barrier (); u.format10.collect_glyphs (glyphs); वापस;
    शेष:वापस;
    }
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, अचिन्हित अंक_glyphs, स्थिर filter_t &filter) स्थिर
  {
    चयन (u.format.v) {
    हाल 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, अंक_glyphs, filter); वापस;
    हाल 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); वापस;
    हाल 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); वापस;
    हाल 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); वापस;
    हाल 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); वापस;
    हाल 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); वापस;
    शेष:वापस;
    }
  }

  typename T::type get_class (hb_codepoint_t glyph_id,
			      अचिन्हित पूर्णांक अंक_glyphs,
			      अचिन्हित पूर्णांक outOfRange) स्थिर
  {
    स्थिर T *v = get_मूल्य (glyph_id, अंक_glyphs);
    वापस v ? *v : outOfRange;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    हाल 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    हाल 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    हाल 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    हाल 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    शेष:return_trace (true);
    }
  }
  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल 0: hb_barrier (); return_trace (u.format0.sanitize (c, base));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c, base));
    हाल 4: hb_barrier (); return_trace (u.format4.sanitize (c, base));
    हाल 6: hb_barrier (); return_trace (u.format6.sanitize (c, base));
    हाल 8: hb_barrier (); return_trace (u.format8.sanitize (c, base));
    हाल 10: return_trace (false); /* We don't support format10 here currently. */
    शेष:return_trace (true);
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_BYTES_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

ढाँचा <typename T>
काष्ठा Entry
{
  // This doesn't seem like it's ever called.
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_आकार, "");

    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		तथ्य;		/* Optional offsets to per-glyph tables. */
  खुला:
  DEFINE_आकार_STATIC (4 + T::static_आकार);
};

ढाँचा <>
काष्ठा Entry<व्योम>
{
  // This does seem like it's ever called.
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  खुला:
  DEFINE_आकार_STATIC (4);
};

क्रमागत Class
{
  CLASS_END_OF_TEXT = 0,
  CLASS_OUT_OF_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_END_OF_पंक्ति = 3,
};

ढाँचा <typename Types, typename Extra>
काष्ठा StateTable
{
  प्रकार typename Types::HBUINT HBUINT;
  प्रकार typename Types::HBUSHORT HBUSHORT;
  प्रकार typename Types::ClassTypeNarrow ClassType;

  क्रमागत State
  {
    STATE_START_OF_TEXT = 0,
    STATE_START_OF_पंक्ति = 1,
  };

  ढाँचा <typename set_t, typename table_t>
  व्योम collect_initial_glyphs (set_t &glyphs, अचिन्हित अंक_glyphs, स्थिर table_t &table) स्थिर
  {
    अचिन्हित अंक_classes = nClasses;

    अगर (unlikely (अंक_classes > hb_bit_page_t::BITS))
    {
      (यह+classTable).collect_glyphs (glyphs, अंक_glyphs);
      वापस;
    }

    // Collect all classes going out from the start state.
    hb_bit_page_t filter;

    क्रम (अचिन्हित i = 0; i < अंक_classes; i++)
    {
      स्थिर स्वतः &entry = get_entry (STATE_START_OF_TEXT, i);
      अगर (new_state (entry.newState) == STATE_START_OF_TEXT &&
	  !table.is_action_initiable (entry) && !table.is_actionable (entry))
	जारी;

      filter.add (i);
    }

    // And glyphs in those classes.

    अगर (filter (CLASS_DELETED_GLYPH))
      glyphs.add (DELETED_GLYPH);

    (यह+classTable).collect_glyphs_filtered (glyphs, अंक_glyphs, filter);
  }

  पूर्णांक new_state (अचिन्हित पूर्णांक newState) स्थिर
  { वापस Types::extended ? newState : ((पूर्णांक) newState - (पूर्णांक) stateArrayTable) / (पूर्णांक) nClasses; }

  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id,
			  अचिन्हित पूर्णांक अंक_glyphs,
			  hb_aat_class_cache_t *cache = nullptr) स्थिर
  {
    अचिन्हित klass;
    अगर (cache && cache->लो (glyph_id, &klass)) वापस klass;
    अगर (unlikely (glyph_id == DELETED_GLYPH)) वापस CLASS_DELETED_GLYPH;
    klass = (यह+classTable).get_class (glyph_id, अंक_glyphs, CLASS_OUT_OF_BOUNDS);
    अगर (cache) cache->set (glyph_id, klass);
    वापस klass;
  }

  स्थिर Entry<Extra> *get_entries () स्थिर
  { वापस (यह+entryTable).arrayZ; }

  स्थिर Entry<Extra> &get_entry (पूर्णांक state, अचिन्हित पूर्णांक klass) स्थिर
  {
    अचिन्हित n_classes = nClasses;
    अगर (unlikely (klass >= n_classes))
      klass = CLASS_OUT_OF_BOUNDS;

    स्थिर HBUSHORT *states = (यह+stateArrayTable).arrayZ;
    स्थिर Entry<Extra> *entries = (यह+entryTable).arrayZ;

    अचिन्हित पूर्णांक entry = states[state * n_classes + klass];
    DEBUG_संदेश (APPLY, nullptr, "e%u", entry);

    वापस entries[entry];
  }

  bool sanitize (hb_sanitize_context_t *c,
		 अचिन्हित पूर्णांक *अंक_entries_out = nullptr) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!(c->check_struct (यह) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    classTable.sanitize (c, यह)))) return_trace (false);

    स्थिर HBUSHORT *states = (यह+stateArrayTable).arrayZ;
    स्थिर Entry<Extra> *entries = (यह+entryTable).arrayZ;

    अचिन्हित पूर्णांक अंक_classes = nClasses;
    अगर (unlikely (hb_unsigned_mul_overflows (अंक_classes, states[0].static_आकार)))
      return_trace (false);
    अचिन्हित पूर्णांक row_stride = अंक_classes * states[0].static_आकार;

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
    जबतक (min_state < state_neg || state_pos <= max_state)
    {
      अगर (min_state < state_neg)
      {
	/* Negative states. */
	अगर (unlikely (hb_unsigned_mul_overflows (min_state, अंक_classes)))
	  return_trace (false);
	अगर (unlikely (!c->check_range (&states[min_state * अंक_classes],
				       -min_state,
				       row_stride)))
	  return_trace (false);
	अगर ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (false);
	{ /* Sweep new states. */
	  स्थिर HBUSHORT *stop = &states[min_state * अंक_classes];
	  अगर (unlikely (stop > states))
	    return_trace (false);
	  क्रम (स्थिर HBUSHORT *p = states; stop < p; p--)
	    अंक_entries = hb_max (अंक_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      अगर (state_pos <= max_state)
      {
	/* Positive states. */
	अगर (unlikely (!c->check_range (states,
				       max_state + 1,
				       row_stride)))
	  return_trace (false);
	अगर ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (false);
	{ /* Sweep new states. */
	  अगर (unlikely (hb_unsigned_mul_overflows ((max_state + 1), अंक_classes)))
	    return_trace (false);
	  स्थिर HBUSHORT *stop = &states[(max_state + 1) * अंक_classes];
	  अगर (unlikely (stop < states))
	    return_trace (false);
	  क्रम (स्थिर HBUSHORT *p = &states[state_pos * अंक_classes]; p < stop; p++)
	    अंक_entries = hb_max (अंक_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      अगर (unlikely (!c->check_array (entries, अंक_entries)))
	return_trace (false);
      अगर ((c->max_ops -= अंक_entries - entry) <= 0)
	return_trace (false);
      { /* Sweep new entries. */
	स्थिर Entry<Extra> *stop = &entries[अंक_entries];
	क्रम (स्थिर Entry<Extra> *p = &entries[entry]; p < stop; p++)
	{
	  पूर्णांक newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	entry = अंक_entries;
      }
    }

    अगर (अंक_entries_out)
      *अंक_entries_out = अंक_entries;

    return_trace (true);
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

  खुला:
  DEFINE_आकार_STATIC (4 * माप (HBUINT));
};

ढाँचा <typename HBUCHAR>
काष्ठा ClassTable
{
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id, अचिन्हित पूर्णांक outOfRange) स्थिर
  {
    अचिन्हित पूर्णांक i = glyph_id - firstGlyph;
    वापस i >= classArray.len ? outOfRange : classArray.arrayZ[i];
  }
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id,
			  अचिन्हित पूर्णांक अंक_glyphs HB_UNUSED,
			  अचिन्हित पूर्णांक outOfRange) स्थिर
  {
    वापस get_class (glyph_id, outOfRange);
  }

  ढाँचा <typename set_t>
  व्योम collect_glyphs (set_t &glyphs, अचिन्हित अंक_glyphs) स्थिर
  {
    क्रम (अचिन्हित i = 0; i < classArray.len; i++)
      अगर (classArray.arrayZ[i] != CLASS_OUT_OF_BOUNDS)
	glyphs.add (firstGlyph + i);
  }
  ढाँचा <typename set_t, typename filter_t>
  व्योम collect_glyphs_filtered (set_t &glyphs, अचिन्हित अंक_glyphs, स्थिर filter_t &filter) स्थिर
  {
    क्रम (अचिन्हित i = 0; i < classArray.len; i++)
      अगर (filter (classArray.arrayZ[i]))
	glyphs.add (firstGlyph + i);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && classArray.sanitize (c));
  }
  रक्षित:
  HBGlyphID16		firstGlyph;	/* First glyph index included in the trimmed array. */
  Array16Of<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  खुला:
  DEFINE_आकार_ARRAY (4, classArray);
};

काष्ठा SubtableGlyphCoverage
{
  bool sanitize (hb_sanitize_context_t *c, अचिन्हित subtable_गिनती) स्थिर
  {
    TRACE_SANITIZE (यह);

    अगर (unlikely (!c->check_array (&subtableOffsets, subtable_गिनती)))
      return_trace (false);

    अचिन्हित bytes = (c->get_अंक_glyphs () + अक्षर_बिट - 1) / अक्षर_बिट;
    क्रम (अचिन्हित i = 0; i < subtable_गिनती; i++)
    {
      uint32_t offset = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      अगर (offset == 0 || offset == 0xFFFFFFFF)
        जारी;
      अगर (unlikely (!subtableOffsets[i].sanitize (c, यह, bytes)))
        return_trace (false);
    }

    return_trace (true);
  }
  रक्षित:
  UnsizedArrayOf<NNOffset32To<UnsizedArrayOf<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  खुला:
  DEFINE_आकार_ARRAY (0, subtableOffsets);
};

काष्ठा ObsoleteTypes
{
  अटल constexpr bool extended = false;
  प्रकार HBUINT16 HBUINT;
  प्रकार HBUINT8 HBUSHORT;
  प्रकार ClassTable<HBUINT8> ClassTypeNarrow;
  प्रकार ClassTable<HBUINT16> ClassTypeWide;

  ढाँचा <typename T>
  अटल पूर्णांक offset_To_सूचकांक (int64_t offset,
			    स्थिर व्योम *base,
			    स्थिर T *array)
  {
    int64_t array_offset = (स्थिर अक्षर *) array - (स्थिर अक्षर *) base;
    पूर्णांक bad_सूचकांक = पूर्णांक_उच्च / T::static_आकार;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    अगर (unlikely (offset < array_offset))
      वापस bad_सूचकांक;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t सूचकांक_2 = (offset - array_offset) / T::static_आकार;
    अगर (unlikely (सूचकांक_2 > bad_सूचकांक))
      वापस bad_सूचकांक;
    वापस सूचकांक_2;
  }
  ढाँचा <typename T>
  अटल पूर्णांक byte_Offset_To_सूचकांक (int64_t offset,
				स्थिर व्योम *base,
				स्थिर T *array)
  {
    वापस offset_To_सूचकांक (offset, base, array);
  }
  ढाँचा <typename T>
  अटल पूर्णांक word_Offset_To_सूचकांक (int64_t offset,
				स्थिर व्योम *base,
				स्थिर T *array)
  {
    वापस offset_To_सूचकांक (2 * offset, base, array);
  }
};
काष्ठा ExtendedTypes
{
  अटल constexpr bool extended = true;
  प्रकार HBUINT32 HBUINT;
  प्रकार HBUINT16 HBUSHORT;
  प्रकार Lookup<HBUINT16> ClassTypeNarrow;
  प्रकार Lookup<HBUINT16> ClassTypeWide;

  ढाँचा <typename T>
  अटल अचिन्हित पूर्णांक offset_To_सूचकांक (अचिन्हित पूर्णांक offset,
				     स्थिर व्योम *base HB_UNUSED,
				     स्थिर T *array HB_UNUSED)
  {
    वापस offset;
  }
  ढाँचा <typename T>
  अटल अचिन्हित पूर्णांक byte_Offset_To_सूचकांक (अचिन्हित पूर्णांक offset,
					 स्थिर व्योम *base HB_UNUSED,
					 स्थिर T *array HB_UNUSED)
  {
    वापस offset / 2;
  }
  ढाँचा <typename T>
  अटल अचिन्हित पूर्णांक word_Offset_To_सूचकांक (अचिन्हित पूर्णांक offset,
					 स्थिर व्योम *base HB_UNUSED,
					 स्थिर T *array HB_UNUSED)
  {
    वापस offset;
  }
};

ढाँचा <typename Types, typename Entry_तथ्य, typename Flags>
काष्ठा StateTableDriver
{
  using StateTableT = StateTable<Types, Entry_तथ्य>;
  using EntryT = Entry<Entry_तथ्य>;

  StateTableDriver (स्थिर StateTableT &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      अंक_glyphs (face_->get_अंक_glyphs ()) {}

  ढाँचा <typename context_t>
  व्योम drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_बफर_t *बफर = ac->बफर;

    अगर (!c->in_place)
      बफर->clear_output ();

    पूर्णांक state = StateTableT::STATE_START_OF_TEXT;
    // If there's only one range, we already checked the flag.
    स्वतः *last_range = ac->range_flags && (ac->range_flags->लंबाई > 1) ? &(*ac->range_flags)[0] : nullptr;
    स्थिर bool start_state_safe_to_break_eot =
      !c->table->is_actionable (machine.get_entry (StateTableT::STATE_START_OF_TEXT, CLASS_END_OF_TEXT));
    क्रम (बफर->idx = 0; बफर->successful;)
    {
      अचिन्हित पूर्णांक klass = likely (बफर->idx < बफर->len) ?
			   machine.get_class (बफर->cur().codepoint, अंक_glyphs, ac->machine_class_cache) :
			   (अचिन्हित) CLASS_END_OF_TEXT;
    resume:
      DEBUG_संदेश (APPLY, nullptr, "c%u at %u", klass, बफर->idx);
      स्थिर EntryT &entry = machine.get_entry (state, klass);
      स्थिर पूर्णांक अग्र_state = machine.new_state (entry.newState);

      bool is_not_epsilon_transition = !(entry.flags & Flags::DontAdvance);
      bool is_not_actionable = !c->table->is_actionable (entry);

      अगर (unlikely (last_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	स्वतः *range = last_range;
	अगर (बफर->idx < बफर->len)
	{
	  अचिन्हित cluster = बफर->cur().cluster;
	  जबतक (cluster < range->cluster_first)
	    range--;
	  जबतक (cluster > range->cluster_last)
	    range++;


	  last_range = range;
	}
	अगर (!(range->flags & ac->subtable_flags))
	{
	  अगर (बफर->idx == बफर->len)
	    अवरोध;

	  state = StateTableT::STATE_START_OF_TEXT;
	  (व्योम) बफर->अग्र_glyph ();
	  जारी;
	}
      }
      अन्यथा
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	bool is_null_transition = state == StateTableT::STATE_START_OF_TEXT &&
				  अग्र_state == StateTableT::STATE_START_OF_TEXT &&
				  start_state_safe_to_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	अगर (is_null_transition)
	{
	  अचिन्हित old_klass = klass;
	  करो
	  {
	    c->transition (बफर, यह, entry);

	    अगर (बफर->idx == बफर->len || !बफर->successful)
	      अवरोध;

	    (व्योम) बफर->अग्र_glyph ();

	    klass = likely (बफर->idx < बफर->len) ?
		     machine.get_class (बफर->cur().codepoint, अंक_glyphs, ac->machine_class_cache) :
		     (अचिन्हित) CLASS_END_OF_TEXT;
	  } जबतक (klass == old_klass);

	  अगर (बफर->idx == बफर->len || !बफर->successful)
	    अवरोध;

	  जाओ resume;
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
      स्थिर EntryT *wouldbe_entry;
      bool is_safe_to_break =
      (
          /* 1. */
          !c->table->is_actionable (entry) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == StateTableT::STATE_START_OF_TEXT
              || ((entry.flags & Flags::DontAdvance) && अग्र_state == StateTableT::STATE_START_OF_TEXT)
              || (
		    /* 2c. */
		    wouldbe_entry = &machine.get_entry(StateTableT::STATE_START_OF_TEXT, klass)
		    ,
		    /* 2c'. */
		    !c->table->is_actionable (*wouldbe_entry) &&
		    /* 2c". */
		    (
		      अग्र_state == machine.new_state(wouldbe_entry->newState) &&
		      (entry.flags & Flags::DontAdvance) == (wouldbe_entry->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->table->is_actionable (machine.get_entry (state, CLASS_END_OF_TEXT))
      );

      अगर (!is_safe_to_break && बफर->backtrack_len () && बफर->idx < बफर->len)
	बफर->unsafe_to_break_from_outbuffer (बफर->backtrack_len () - 1, बफर->idx + 1);

      c->transition (बफर, यह, entry);

      state = अग्र_state;
      DEBUG_संदेश (APPLY, nullptr, "s%d", state);

      अगर (बफर->idx == बफर->len)
	अवरोध;

      अगर (is_not_epsilon_transition || बफर->max_ops-- <= 0)
	(व्योम) बफर->अग्र_glyph ();
    }

    अगर (!c->in_place)
      बफर->sync ();
  }

  खुला:
  स्थिर StateTableT &machine;
  अचिन्हित पूर्णांक अंक_glyphs;
};


} /* namespace AAT */


#पूर्ण_अगर /* HB_AAT_LAYOUT_COMMON_HH */
