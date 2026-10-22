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

#جيڪڏهن_وصف_نه_ٿيل HB_AAT_LAYOUT_COMMON_HH
#وصف_ڪريو HB_AAT_LAYOUT_COMMON_HH

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

استعمال نطاق OT;

بنية ankr;

استعمال hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
#وصف_ڪريو HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#وإلا
#وصف_ڪريو HB_AAT_TRANSITION_INLINE
#جيڪڏهن_پورو

بنية hb_aat_scratch_t
{
  hb_aat_scratch_t () = افتراضي;
  hb_aat_scratch_t (ثابت hb_aat_scratch_t &) = ميٽيو;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    بفر_glyph_سيٽ.سيٽ_relaxed (o.بفر_glyph_سيٽ.get_relaxed ());
    o.بفر_glyph_سيٽ.سيٽ_relaxed (خالي_اشارو);
  }
  hb_aat_scratch_t & عامل = (hb_aat_scratch_t &&o)
  {
    بفر_glyph_سيٽ.سيٽ_relaxed (o.بفر_glyph_سيٽ.get_relaxed ());
    o.بفر_glyph_سيٽ.سيٽ_relaxed (خالي_اشارو);
    أرجع *هذا;
  }
  ~hb_aat_scratch_t ()
  {
    خودڪار *s = بفر_glyph_سيٽ.get_relaxed ();
    إذا (unlikely (!s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  hb_ٻٽو_انگ_سيٽ_t *create_بفر_glyph_سيٽ () ثابت
  {
    hb_ٻٽو_انگ_سيٽ_t *s = بفر_glyph_سيٽ.get_acquire ();
    إذا (s && بفر_glyph_سيٽ.cmpexch (s, خالي_اشارو))
    {
      s->clear ();
      أرجع s;
    }

    s = (hb_ٻٽو_انگ_سيٽ_t *) hb_calloc (1, حجم (hb_ٻٽو_انگ_سيٽ_t));
    إذا (unlikely (!s))
      أرجع خالي_اشارو;
    s->init ();

    أرجع s;
  }
  فراغ destroy_بفر_glyph_سيٽ (hb_ٻٽو_انگ_سيٽ_t *s) ثابت
  {
    إذا (unlikely (!s))
      أرجع;
    إذا (بفر_glyph_سيٽ.cmpexch (خالي_اشارو, s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  بدلجندڙ hb_atomic_t<hb_ٻٽو_انگ_سيٽ_t *> بفر_glyph_سيٽ;
};

تعداد { DELETED_GLYPH = 0xFFFF };

#وصف_ڪريو HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED HB_بفر_SCRATCH_FLAG_SHAPER0

بنية hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  سانچو <قسم_نالو T, قسم_نالو ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { أرجع obj.apply (هذا, std::forward<Ts> (ds)...); }
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  ثابت hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t *بفر;
  hb_sanitize_context_t sanitizer;
  ثابت ankr *ankr_جدول;
  ثابت OT::GDEF &gdef;
  منطقي has_glyph_classes;
  ثابت hb_sorted_ويڪٽر_t<hb_aat_map_t::range_flags_t> *range_flags = خالي_اشارو;
  hb_نقاب_t subtable_flags = 0;
  منطقي بفر_is_reversed = خطأ;
  // Caches
  منطقي using_بفر_glyph_سيٽ = خطأ;
  hb_ٻٽو_انگ_سيٽ_t *بفر_glyph_سيٽ = خالي_اشارو;
  ثابت hb_ٻٽو_انگ_سيٽ_t *پهريون_سيٽ = خالي_اشارو;
  ثابت hb_ٻٽو_انگ_سيٽ_t *ٻيو_سيٽ = خالي_اشارو;
  hb_aat_class_cache_t *machine_class_cache = خالي_اشارو;

  /* Unused. For debug tracing only. */
  بنانشان صحيح lookup_اشاريو;

  HB_اندروني hb_aat_apply_context_t (ثابت hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_بفر_t *بفر_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_اندروني ~hb_aat_apply_context_t ();

  HB_اندروني فراغ سيٽ_ankr_جدول (ثابت AAT::ankr *ankr_جدول_2);

  فراغ سيٽ_lookup_اشاريو (بنانشان صحيح i) { lookup_اشاريو = i; }

  فراغ ابتو_بفر ()
  {
    بفر->reverse ();
    بفر_is_reversed = !بفر_is_reversed;
  }

  فراغ تياري_بفر_glyph_سيٽ ()
  {
    using_بفر_glyph_سيٽ = بفر->len >= 4 && بفر_glyph_سيٽ;

    إذا (likely (using_بفر_glyph_سيٽ))
      بفر->collect_codepoints (*بفر_glyph_سيٽ);
  }
  منطقي بفر_intersects_machine () ثابت
  {
    إذا (likely (using_بفر_glyph_سيٽ))
      أرجع بفر_glyph_سيٽ->intersects (*پهريون_سيٽ);

    // Faster for shorter buffers.
    لكل (بنانشان i = 0; i < بفر->len; i++)
      إذا (پهريون_سيٽ->has (بفر->info[i].codepoint))
	أرجع سچ;
    أرجع خطأ;
  }

  سانچو <قسم_نالو T>
  HB_NODISCARD منطقي output_glyphs (بنانشان صحيح ڳڻپ,
				   ثابت T *glyphs)
  {
    إذا (likely (using_بفر_glyph_سيٽ))
      بفر_glyph_سيٽ->جوڙيو_array (glyphs, ڳڻپ);
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      منطقي at_پڇاڙي_جو_متن = بفر->idx == بفر->len;
      إذا (at_پڇاڙي_جو_متن)
	إذا (unlikely (!بفر->output_glyph (glyphs[i]))) أرجع خطأ;

      hb_glyph_info_t &info = at_پڇاڙي_جو_متن ? بفر->prev () : بفر->cur ();
      إذا (glyphs[i] == DELETED_GLYPH)
      {
        بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
	hb_glyph_info_سيٽ_aat_deleted (&info);
      }
      وإلا
      {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT
	إذا (has_glyph_classes)
	  hb_glyph_info_سيٽ_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#جيڪڏهن_پورو
      }
      إذا (!at_پڇاڙي_جو_متن)
	إذا (unlikely (!بفر->output_glyph (glyphs[i]))) أرجع خطأ;
    }
    أرجع سچ;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph)
  {
    إذا (glyph == DELETED_GLYPH)
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_سيٽ_aat_deleted (&بفر->cur());
    }

    إذا (likely (using_بفر_glyph_سيٽ))
      بفر_glyph_سيٽ->جوڙيو (glyph);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      hb_glyph_info_سيٽ_glyph_props (&بفر->cur(),
				      gdef.get_glyph_props (glyph));
#جيڪڏهن_پورو
    أرجع بفر->replace_glyph (glyph);
  }

  HB_NODISCARD منطقي delete_glyph ()
  {
    بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
    hb_glyph_info_سيٽ_aat_deleted (&بفر->cur());
    إذا (likely (using_بفر_glyph_سيٽ))
      بفر_glyph_سيٽ->جوڙيو (DELETED_GLYPH);
    أرجع بفر->replace_glyph (DELETED_GLYPH);
  }

  فراغ replace_glyph_inplace (بنانشان i, hb_codepoint_t glyph)
  {
    بفر->info[i].codepoint = glyph;
    إذا (glyph == DELETED_GLYPH)
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_سيٽ_aat_deleted (&بفر->info[i]);
    }
    إذا (likely (using_بفر_glyph_سيٽ))
      بفر_glyph_سيٽ->جوڙيو (glyph);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      hb_glyph_info_سيٽ_glyph_props (&بفر->info[i],
				      gdef.get_glyph_props (glyph));
#جيڪڏهن_پورو
  }
};


/*
 * Lookup Table
 */

سانچو <قسم_نالو T> بنية Lookup;

سانچو <قسم_نالو T>
بنية LookupFormat0
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id, بنانشان صحيح انگ_glyphs) ثابت
  {
    إذا (unlikely (glyph_id >= انگ_glyphs)) أرجع خالي_اشارو;
    أرجع &arrayZ[glyph_id];
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs, بنانشان انگ_glyphs) ثابت
  {
    glyphs.جوڙيو_range (0, انگ_glyphs - 1);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, بنانشان انگ_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (بنانشان i = 0; i < انگ_glyphs; i++)
      إذا (filter (arrayZ[i]))
	glyphs.جوڙيو (i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_انگ_glyphs ()));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_انگ_glyphs (), بنياد));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 0 */
  Unsized_Array_جو<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  عام:
  DEFINE_سائيز_UNBOUNDED (2);
};


سانچو <قسم_نالو T>
بنية LookupSegmentSingle
{
  ساكن constexpr بنانشان Termination_لفظ_ڳڻپ = 2u;

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < پهريون ? -1 : g <= آخري ? 0 : +1 ; }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs) ثابت
  {
    إذا (پهريون == DELETED_GLYPH) أرجع;
    glyphs.جوڙيو_range (پهريون, آخري);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (پهريون == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
    glyphs.جوڙيو_range (پهريون, آخري);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, بنياد));
  }

  HBGlyphID16	آخري;		/* Last GlyphID in this segment */
  HBGlyphID16	پهريون;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_سائيز_STATIC (4 + T::static_سائيز);
};

سانچو <قسم_نالو T>
بنية LookupFormat2
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    أرجع v ? &v->value : خالي_اشارو;
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs) ثابت
  {
    بنانشان ڳڻپ = segments.get_ڊيگهائي ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      segments[i].collect_glyphs (glyphs);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بنانشان ڳڻپ = segments.get_ڊيگهائي ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, بنياد));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_ڳولا_Array_جو<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_سائيز_ARRAY (8, segments);
};

سانچو <قسم_نالو T>
بنية LookupSegmentArray
{
  ساكن constexpr بنانشان Termination_لفظ_ڳڻپ = 2u;

  ثابت T* get_قدر (hb_codepoint_t glyph_id, ثابت فراغ *بنياد) ثابت
  {
    أرجع پهريون <= glyph_id && glyph_id <= آخري ? &(بنياد+قدرون_Z)[glyph_id - پهريون] : خالي_اشارو;
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs) ثابت
  {
    إذا (پهريون == DELETED_GLYPH) أرجع;
    glyphs.جوڙيو_range (پهريون, آخري);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, ثابت فراغ *بنياد, ثابت filter_t &filter) ثابت
  {
    إذا (پهريون == DELETED_GLYPH) أرجع;
    ثابت خودڪار &values = بنياد+قدرون_Z;
    لكل (hb_codepoint_t i = پهريون; i <= آخري; i++)
      إذا (filter (values[i - پهريون]))
	glyphs.جوڙيو (i);
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < پهريون ? -1 : g <= آخري ? 0 : +1; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  پهريون <= آخري &&
		  قدرون_Z.sanitize (c, بنياد, آخري - پهريون + 1));
  }
  سانچو <قسم_نالو ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  پهريون <= آخري &&
		  قدرون_Z.sanitize (c, بنياد, آخري - پهريون + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	آخري;		/* Last GlyphID in this segment */
  HBGlyphID16	پهريون;		/* First GlyphID in this segment */
  NNOffset16_منزل<Unsized_Array_جو<T>>
		قدرون_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  عام:
  DEFINE_سائيز_STATIC (6);
};

سانچو <قسم_نالو T>
بنية LookupFormat4
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    أرجع v ? v->get_قدر (glyph_id, هذا) : خالي_اشارو;
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs) ثابت
  {
    بنانشان ڳڻپ = segments.get_ڊيگهائي ();
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      segments[i].collect_glyphs (glyphs);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بنانشان ڳڻپ = segments.get_ڊيگهائي ();
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      segments[i].collect_glyphs_filtered (glyphs, هذا, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا, بنياد));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_ڳولا_Array_جو<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_سائيز_ARRAY (8, segments);
};

سانچو <قسم_نالو T>
بنية LookupSingle
{
  ساكن constexpr بنانشان Termination_لفظ_ڳڻپ = 1u;

  صحيح cmp (hb_codepoint_t g) ثابت { أرجع glyph.cmp (g); }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    glyphs.جوڙيو (glyph);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
    glyphs.جوڙيو (glyph);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, بنياد));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_سائيز_STATIC (2 + T::static_سائيز);
};

سانچو <قسم_نالو T>
بنية LookupFormat6
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSingle<T> *v = entries.bsearch (glyph_id);
    أرجع v ? &v->value : خالي_اشارو;
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs) ثابت
  {
    بنانشان ڳڻپ = entries.get_ڊيگهائي ();
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      entries[i].collect_glyphs (glyphs);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بنانشان ڳڻپ = entries.get_ڊيگهائي ();
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c, بنياد));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_ڳولا_Array_جو<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  عام:
  DEFINE_سائيز_ARRAY (8, entries);
};

سانچو <قسم_نالو T>
بنية LookupFormat8
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    أرجع پهريون_Glyph <= glyph_id && glyph_id - پهريون_Glyph < glyph_ڳڻپ_2 ?
	   &قدر_Array_Z[glyph_id - پهريون_Glyph] : خالي_اشارو;
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_ڳڻپ_2)) أرجع;
    إذا (پهريون_Glyph == DELETED_GLYPH) أرجع;
    glyphs.جوڙيو_range (پهريون_Glyph, پهريون_Glyph + glyph_ڳڻپ_2 - 1);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_ڳڻپ_2)) أرجع;
    إذا (پهريون_Glyph == DELETED_GLYPH) أرجع;
    ثابت T *p = قدر_Array_Z.arrayZ;
    لكل (بنانشان i = 0; i < glyph_ڳڻپ_2; i++)
      إذا (filter (p[i]))
	glyphs.جوڙيو (پهريون_Glyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && قدر_Array_Z.sanitize (c, glyph_ڳڻپ_2));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && قدر_Array_Z.sanitize (c, glyph_ڳڻپ_2, بنياد));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	پهريون_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ڳڻپ_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_جو<T>
		قدر_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_سائيز_ARRAY (6, قدر_Array_Z);
};

سانچو <قسم_نالو T>
بنية LookupFormat10
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت قسم_نالو T::type get_قدر_or_null (hb_codepoint_t glyph_id) ثابت
  {
    إذا (!(پهريون_Glyph <= glyph_id && glyph_id - پهريون_Glyph < glyph_ڳڻپ_2))
      أرجع Null (T);

    ثابت HBUINT8 *p = &قدر_Array_Z[(glyph_id - پهريون_Glyph) * قدر_سائيز];

    بنانشان صحيح v = 0;
    بنانشان صحيح ڳڻپ = قدر_سائيز;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      v = (v << 8) | *p++;

    أرجع v;
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_ڳڻپ_2)) أرجع;
    إذا (پهريون_Glyph == DELETED_GLYPH) أرجع;
    glyphs.جوڙيو_range (پهريون_Glyph, پهريون_Glyph + glyph_ڳڻپ_2 - 1);
  }

  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_ڳڻپ_2)) أرجع;
    إذا (پهريون_Glyph == DELETED_GLYPH) أرجع;
    ثابت HBUINT8 *p = قدر_Array_Z.arrayZ;
    لكل (بنانشان i = 0; i < glyph_ڳڻپ_2; i++)
    {
      بنانشان صحيح v = 0;
      بنانشان صحيح ڳڻپ = قدر_سائيز;
      لكل (بنانشان صحيح j = 0; j < ڳڻپ; j++)
	v = (v << 8) | *p++;
      إذا (filter (v))
	glyphs.جوڙيو (پهريون_Glyph + i);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  قدر_سائيز <= 4 &&
		  قدر_Array_Z.sanitize (c, glyph_ڳڻپ_2 * قدر_سائيز));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	قدر_سائيز;	/* Byte size of each value. */
  HBGlyphID16	پهريون_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_ڳڻپ_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_جو<HBUINT8>
		قدر_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_سائيز_ARRAY (8, قدر_Array_Z);
};

سانچو <قسم_نالو T>
بنية Lookup
{
  ثابت T* get_قدر (hb_codepoint_t glyph_id, بنانشان صحيح انگ_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_قدر (glyph_id, انگ_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_قدر (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_قدر (glyph_id);
    حالة 6: hb_barrier (); أرجع u.format6.get_قدر (glyph_id);
    حالة 8: hb_barrier (); أرجع u.format8.get_قدر (glyph_id);
    افتراضي:أرجع خالي_اشارو;
    }
  }

  ثابت قسم_نالو T::type get_قدر_or_null (hb_codepoint_t glyph_id, بنانشان صحيح انگ_glyphs) ثابت
  {
    بدل (u.format.v) {
      /* Format 10 cannot return a pointer. */
      حالة 10: hb_barrier (); أرجع u.format10.get_قدر_or_null (glyph_id);
      افتراضي:
      ثابت T *v = get_قدر (glyph_id, انگ_glyphs);
      أرجع v ? *v : Null (T);
    }
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs, بنانشان صحيح انگ_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); u.format0.collect_glyphs (glyphs, انگ_glyphs); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyphs (glyphs); أرجع;
    حالة 4: hb_barrier (); u.format4.collect_glyphs (glyphs); أرجع;
    حالة 6: hb_barrier (); u.format6.collect_glyphs (glyphs); أرجع;
    حالة 8: hb_barrier (); u.format8.collect_glyphs (glyphs); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_glyphs (glyphs); أرجع;
    افتراضي:أرجع;
    }
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, بنانشان انگ_glyphs, ثابت filter_t &filter) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, انگ_glyphs, filter); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); أرجع;
    افتراضي:أرجع;
    }
  }

  قسم_نالو T::type get_class (hb_codepoint_t glyph_id,
			      بنانشان صحيح انگ_glyphs,
			      بنانشان صحيح ٻاهريون_جو_Range) ثابت
  {
    ثابت T *v = get_قدر (glyph_id, انگ_glyphs);
    أرجع v ? *v : ٻاهريون_جو_Range;
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
    افتراضي:return_trace (سچ);
    }
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, بنياد));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, بنياد));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, بنياد));
    حالة 6: hb_barrier (); return_trace (u.format6.sanitize (c, بنياد));
    حالة 8: hb_barrier (); return_trace (u.format8.sanitize (c, بنياد));
    حالة 10: return_trace (خطأ); /* We don't support format10 here currently. */
    افتراضي:return_trace (سچ);
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
  DEFINE_سائيز_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_بائيٽون_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

سانچو <قسم_نالو T>
بنية داخلا_4
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
    static_assert (T::static_سائيز, "");

    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		معلومات;		/* Optional offsets to per-glyph tables. */
  عام:
  DEFINE_سائيز_STATIC (4 + T::static_سائيز);
};

سانچو <>
بنية داخلا_4<فراغ>
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
  DEFINE_سائيز_STATIC (4);
};

تعداد Class
{
  CLASS_پڇاڙي_جو_متن = 0,
  CLASS_ٻاهريون_جو_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_پڇاڙي_جو_سٽ = 3,
};

سانچو <قسم_نالو Types, قسم_نالو Extra>
بنية State_جدول
{
  تعريفنوع قسم_نالو Types::HBUINT HBUINT;
  تعريفنوع قسم_نالو Types::HBUSHORT HBUSHORT;
  تعريفنوع قسم_نالو Types::ClassTypeNarrow ClassType;

  تعداد State
  {
    STATE_شروعات_جو_متن = 0,
    STATE_شروعات_جو_سٽ = 1,
  };

  سانچو <قسم_نالو سيٽ_t, قسم_نالو جدول_t>
  فراغ collect_initial_glyphs (سيٽ_t &glyphs, بنانشان انگ_glyphs, ثابت جدول_t &جدول) ثابت
  {
    بنانشان انگ_classes = nClasses;

    إذا (unlikely (انگ_classes > hb_ٻٽو_انگ_صفحو_t::بٽ_2))
    {
      (هذا+class_جدول).collect_glyphs (glyphs, انگ_glyphs);
      أرجع;
    }

    // Collect all classes going out from the start state.
    hb_ٻٽو_انگ_صفحو_t filter;

    لكل (بنانشان i = 0; i < انگ_classes; i++)
    {
      ثابت خودڪار &داخلا = get_داخلا (STATE_شروعات_جو_متن, i);
      إذا (new_state (داخلا.newState) == STATE_شروعات_جو_متن &&
	  !جدول.is_action_initiable (داخلا) && !جدول.is_actionable (داخلا))
	استمر;

      filter.جوڙيو (i);
    }

    // And glyphs in those classes.

    إذا (filter (CLASS_DELETED_GLYPH))
      glyphs.جوڙيو (DELETED_GLYPH);

    (هذا+class_جدول).collect_glyphs_filtered (glyphs, انگ_glyphs, filter);
  }

  صحيح new_state (بنانشان صحيح newState) ثابت
  { أرجع Types::extended ? newState : ((صحيح) newState - (صحيح) state_Array_جدول) / (صحيح) nClasses; }

  بنانشان صحيح get_class (hb_codepoint_t glyph_id,
			  بنانشان صحيح انگ_glyphs,
			  hb_aat_class_cache_t *cache = خالي_اشارو) ثابت
  {
    بنانشان klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    إذا (unlikely (glyph_id == DELETED_GLYPH)) أرجع CLASS_DELETED_GLYPH;
    klass = (هذا+class_جدول).get_class (glyph_id, انگ_glyphs, CLASS_ٻاهريون_جو_BOUNDS);
    إذا (cache) cache->سيٽ (glyph_id, klass);
    أرجع klass;
  }

  ثابت داخلا_4<Extra> *get_entries () ثابت
  { أرجع (هذا+داخلا_جدول).arrayZ; }

  ثابت داخلا_4<Extra> &get_داخلا (صحيح state, بنانشان صحيح klass) ثابت
  {
    بنانشان n_classes = nClasses;
    إذا (unlikely (klass >= n_classes))
      klass = CLASS_ٻاهريون_جو_BOUNDS;

    ثابت HBUSHORT *حالتون = (هذا+state_Array_جدول).arrayZ;
    ثابت داخلا_4<Extra> *entries = (هذا+داخلا_جدول).arrayZ;

    بنانشان صحيح داخلا = حالتون[state * n_classes + klass];
    DEBUG_نياپو (APPLY, خالي_اشارو, "e%u", داخلا);

    أرجع entries[داخلا];
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 بنانشان صحيح *انگ_entries_ٻاهريون = خالي_اشارو) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    class_جدول.sanitize (c, هذا)))) return_trace (خطأ);

    ثابت HBUSHORT *حالتون = (هذا+state_Array_جدول).arrayZ;
    ثابت داخلا_4<Extra> *entries = (هذا+داخلا_جدول).arrayZ;

    بنانشان صحيح انگ_classes = nClasses;
    إذا (unlikely (hb_unsigned_mul_overflows (انگ_classes, حالتون[0].static_سائيز)))
      return_trace (خطأ);
    بنانشان صحيح صف_stride = انگ_classes * حالتون[0].static_سائيز;

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
    بنانشان صحيح انگ_entries = 0;

    صحيح state_pos = 0;
    صحيح state_neg = 0;
    بنانشان صحيح داخلا = 0;
    طالما (min_state < state_neg || state_pos <= max_state)
    {
      إذا (min_state < state_neg)
      {
	/* Negative states. */
	إذا (unlikely (hb_unsigned_mul_overflows (min_state, انگ_classes)))
	  return_trace (خطأ);
	إذا (unlikely (!c->check_range (&حالتون[min_state * انگ_classes],
				       -min_state,
				       صف_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  ثابت HBUSHORT *stop = &حالتون[min_state * انگ_classes];
	  إذا (unlikely (stop > حالتون))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = حالتون; stop < p; p--)
	    انگ_entries = hb_max (انگ_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      إذا (state_pos <= max_state)
      {
	/* Positive states. */
	إذا (unlikely (!c->check_range (حالتون,
				       max_state + 1,
				       صف_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  إذا (unlikely (hb_unsigned_mul_overflows ((max_state + 1), انگ_classes)))
	    return_trace (خطأ);
	  ثابت HBUSHORT *stop = &حالتون[(max_state + 1) * انگ_classes];
	  إذا (unlikely (stop < حالتون))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = &حالتون[state_pos * انگ_classes]; p < stop; p++)
	    انگ_entries = hb_max (انگ_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      إذا (unlikely (!c->check_array (entries, انگ_entries)))
	return_trace (خطأ);
      إذا ((c->max_ops -= انگ_entries - داخلا) <= 0)
	return_trace (خطأ);
      { /* Sweep new entries. */
	ثابت داخلا_4<Extra> *stop = &entries[انگ_entries];
	لكل (ثابت داخلا_4<Extra> *p = &entries[داخلا]; p < stop; p++)
	{
	  صحيح newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	داخلا = انگ_entries;
      }
    }

    إذا (انگ_entries_ٻاهريون)
      *انگ_entries_ٻاهريون = انگ_entries;

    return_trace (سچ);
  }

  محمي:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffset_منزل<ClassType, HBUINT>
		class_جدول;	/* Offset to the class table. */
  NNOffset_منزل<Unsized_Array_جو<HBUSHORT>, HBUINT>
		state_Array_جدول;/* Offset to the state array. */
  NNOffset_منزل<Unsized_Array_جو<داخلا_4<Extra>>, HBUINT>
		داخلا_جدول;	/* Offset to the entry array. */

  عام:
  DEFINE_سائيز_STATIC (4 * حجم (HBUINT));
};

سانچو <قسم_نالو HBUCHAR>
بنية Class_جدول
{
  بنانشان صحيح get_class (hb_codepoint_t glyph_id, بنانشان صحيح ٻاهريون_جو_Range) ثابت
  {
    بنانشان صحيح i = glyph_id - پهريون_Glyph;
    أرجع i >= classArray.len ? ٻاهريون_جو_Range : classArray.arrayZ[i];
  }
  بنانشان صحيح get_class (hb_codepoint_t glyph_id,
			  بنانشان صحيح انگ_glyphs HB_اڻ_استعمال_ٿيل,
			  بنانشان صحيح ٻاهريون_جو_Range) ثابت
  {
    أرجع get_class (glyph_id, ٻاهريون_جو_Range);
  }

  سانچو <قسم_نالو سيٽ_t>
  فراغ collect_glyphs (سيٽ_t &glyphs, بنانشان انگ_glyphs) ثابت
  {
    لكل (بنانشان i = 0; i < classArray.len; i++)
      إذا (classArray.arrayZ[i] != CLASS_ٻاهريون_جو_BOUNDS)
	glyphs.جوڙيو (پهريون_Glyph + i);
  }
  سانچو <قسم_نالو سيٽ_t, قسم_نالو filter_t>
  فراغ collect_glyphs_filtered (سيٽ_t &glyphs, بنانشان انگ_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (بنانشان i = 0; i < classArray.len; i++)
      إذا (filter (classArray.arrayZ[i]))
	glyphs.جوڙيو (پهريون_Glyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && classArray.sanitize (c));
  }
  محمي:
  HBGlyphID16		پهريون_Glyph;	/* First glyph index included in the trimmed array. */
  Array16_جو<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  عام:
  DEFINE_سائيز_ARRAY (4, classArray);
};

بنية SubtableGlyphCoverage
{
  منطقي sanitize (hb_sanitize_context_t *c, بنانشان subtable_ڳڻپ) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_array (&subtableOffsets, subtable_ڳڻپ)))
      return_trace (خطأ);

    بنانشان بائيٽون = (c->get_انگ_glyphs () + CHAR_ٻٽو_انگ - 1) / CHAR_ٻٽو_انگ;
    لكل (بنانشان i = 0; i < subtable_ڳڻپ; i++)
    {
      uint32_t وٿي = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      إذا (وٿي == 0 || وٿي == 0xFFFFFFFF)
        استمر;
      إذا (unlikely (!subtableOffsets[i].sanitize (c, هذا, بائيٽون)))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }
  محمي:
  Unsized_Array_جو<NNOffset32_منزل<Unsized_Array_جو<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  عام:
  DEFINE_سائيز_ARRAY (0, subtableOffsets);
};

بنية ObsoleteTypes
{
  ساكن constexpr منطقي extended = خطأ;
  تعريفنوع HBUINT16 HBUINT;
  تعريفنوع HBUINT8 HBUSHORT;
  تعريفنوع Class_جدول<HBUINT8> ClassTypeNarrow;
  تعريفنوع Class_جدول<HBUINT16> ClassTypeWide;

  سانچو <قسم_نالو T>
  ساكن صحيح وٿي_منزل_اشاريو (int64_t وٿي,
			    ثابت فراغ *بنياد,
			    ثابت T *array)
  {
    int64_t array_وٿي = (ثابت حرف *) array - (ثابت حرف *) بنياد;
    صحيح bad_اشاريو = وڌ_کان_وڌ_عدد / T::static_سائيز;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    إذا (unlikely (وٿي < array_وٿي))
      أرجع bad_اشاريو;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t اشاريو_2 = (وٿي - array_وٿي) / T::static_سائيز;
    إذا (unlikely (اشاريو_2 > bad_اشاريو))
      أرجع bad_اشاريو;
    أرجع اشاريو_2;
  }
  سانچو <قسم_نالو T>
  ساكن صحيح byte_وٿي_منزل_اشاريو (int64_t وٿي,
				ثابت فراغ *بنياد,
				ثابت T *array)
  {
    أرجع وٿي_منزل_اشاريو (وٿي, بنياد, array);
  }
  سانچو <قسم_نالو T>
  ساكن صحيح لفظ_وٿي_منزل_اشاريو (int64_t وٿي,
				ثابت فراغ *بنياد,
				ثابت T *array)
  {
    أرجع وٿي_منزل_اشاريو (2 * وٿي, بنياد, array);
  }
};
بنية ExtendedTypes
{
  ساكن constexpr منطقي extended = سچ;
  تعريفنوع HBUINT32 HBUINT;
  تعريفنوع HBUINT16 HBUSHORT;
  تعريفنوع Lookup<HBUINT16> ClassTypeNarrow;
  تعريفنوع Lookup<HBUINT16> ClassTypeWide;

  سانچو <قسم_نالو T>
  ساكن بنانشان صحيح وٿي_منزل_اشاريو (بنانشان صحيح وٿي,
				     ثابت فراغ *بنياد HB_اڻ_استعمال_ٿيل,
				     ثابت T *array HB_اڻ_استعمال_ٿيل)
  {
    أرجع وٿي;
  }
  سانچو <قسم_نالو T>
  ساكن بنانشان صحيح byte_وٿي_منزل_اشاريو (بنانشان صحيح وٿي,
					 ثابت فراغ *بنياد HB_اڻ_استعمال_ٿيل,
					 ثابت T *array HB_اڻ_استعمال_ٿيل)
  {
    أرجع وٿي / 2;
  }
  سانچو <قسم_نالو T>
  ساكن بنانشان صحيح لفظ_وٿي_منزل_اشاريو (بنانشان صحيح وٿي,
					 ثابت فراغ *بنياد HB_اڻ_استعمال_ٿيل,
					 ثابت T *array HB_اڻ_استعمال_ٿيل)
  {
    أرجع وٿي;
  }
};

سانچو <قسم_نالو Types, قسم_نالو داخلا_معلومات, قسم_نالو Flags>
بنية State_جدول_Driver
{
  استعمال State_جدول_T = State_جدول<Types, داخلا_معلومات>;
  استعمال داخلا_T = داخلا_4<داخلا_معلومات>;

  State_جدول_Driver (ثابت State_جدول_T &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      انگ_glyphs (face_->get_انگ_glyphs ()) {}

  سانچو <قسم_نالو context_t>
  فراغ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_بفر_t *بفر = ac->بفر;

    إذا (!c->in_place)
      بفر->clear_output ();

    صحيح state = State_جدول_T::STATE_شروعات_جو_متن;
    // If there's only one range, we already checked the flag.
    خودڪار *آخري_range = ac->range_flags && (ac->range_flags->ڊيگهائي > 1) ? &(*ac->range_flags)[0] : خالي_اشارو;
    ثابت منطقي شروعات_state_محفوظ_منزل_break_eot =
      !c->جدول->is_actionable (machine.get_داخلا (State_جدول_T::STATE_شروعات_جو_متن, CLASS_پڇاڙي_جو_متن));
    لكل (بفر->idx = 0; بفر->successful;)
    {
      بنانشان صحيح klass = likely (بفر->idx < بفر->len) ?
			   machine.get_class (بفر->cur().codepoint, انگ_glyphs, ac->machine_class_cache) :
			   (بنانشان) CLASS_پڇاڙي_جو_متن;
    resume:
      DEBUG_نياپو (APPLY, خالي_اشارو, "c%u at %u", klass, بفر->idx);
      ثابت داخلا_T &داخلا = machine.get_داخلا (state, klass);
      ثابت صحيح ايندڙ_state = machine.new_state (داخلا.newState);

      منطقي is_not_epsilon_transition = !(داخلا.flags & Flags::DontAdvance);
      منطقي is_not_actionable = !c->جدول->is_actionable (داخلا);

      إذا (unlikely (آخري_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	خودڪار *range = آخري_range;
	إذا (بفر->idx < بفر->len)
	{
	  بنانشان cluster = بفر->cur().cluster;
	  طالما (cluster < range->cluster_پهريون)
	    range--;
	  طالما (cluster > range->cluster_آخري)
	    range++;


	  آخري_range = range;
	}
	إذا (!(range->flags & ac->subtable_flags))
	{
	  إذا (بفر->idx == بفر->len)
	    اكسر;

	  state = State_جدول_T::STATE_شروعات_جو_متن;
	  (فراغ) بفر->ايندڙ_glyph ();
	  استمر;
	}
      }
      وإلا
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	منطقي is_null_transition = state == State_جدول_T::STATE_شروعات_جو_متن &&
				  ايندڙ_state == State_جدول_T::STATE_شروعات_جو_متن &&
				  شروعات_state_محفوظ_منزل_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	إذا (is_null_transition)
	{
	  بنانشان old_klass = klass;
	  افعل
	  {
	    c->transition (بفر, هذا, داخلا);

	    إذا (بفر->idx == بفر->len || !بفر->successful)
	      اكسر;

	    (فراغ) بفر->ايندڙ_glyph ();

	    klass = likely (بفر->idx < بفر->len) ?
		     machine.get_class (بفر->cur().codepoint, انگ_glyphs, ac->machine_class_cache) :
		     (بنانشان) CLASS_پڇاڙي_جو_متن;
	  } طالما (klass == old_klass);

	  إذا (بفر->idx == بفر->len || !بفر->successful)
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
      ثابت داخلا_T *wouldbe_داخلا;
      منطقي is_محفوظ_منزل_break =
      (
          /* 1. */
          !c->جدول->is_actionable (داخلا) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == State_جدول_T::STATE_شروعات_جو_متن
              || ((داخلا.flags & Flags::DontAdvance) && ايندڙ_state == State_جدول_T::STATE_شروعات_جو_متن)
              || (
		    /* 2c. */
		    wouldbe_داخلا = &machine.get_داخلا(State_جدول_T::STATE_شروعات_جو_متن, klass)
		    ,
		    /* 2c'. */
		    !c->جدول->is_actionable (*wouldbe_داخلا) &&
		    /* 2c". */
		    (
		      ايندڙ_state == machine.new_state(wouldbe_داخلا->newState) &&
		      (داخلا.flags & Flags::DontAdvance) == (wouldbe_داخلا->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->جدول->is_actionable (machine.get_داخلا (state, CLASS_پڇاڙي_جو_متن))
      );

      إذا (!is_محفوظ_منزل_break && بفر->backtrack_len () && بفر->idx < بفر->len)
	بفر->unsafe_منزل_break_from_outbuffer (بفر->backtrack_len () - 1, بفر->idx + 1);

      c->transition (بفر, هذا, داخلا);

      state = ايندڙ_state;
      DEBUG_نياپو (APPLY, خالي_اشارو, "s%d", state);

      إذا (بفر->idx == بفر->len)
	اكسر;

      إذا (is_not_epsilon_transition || بفر->max_ops-- <= 0)
	(فراغ) بفر->ايندڙ_glyph ();
    }

    إذا (!c->in_place)
      بفر->متزامن ();
  }

  عام:
  ثابت State_جدول_T &machine;
  بنانشان صحيح انگ_glyphs;
};


} /* namespace AAT */


#جيڪڏهن_پورو /* HB_AAT_LAYOUT_COMMON_HH */
