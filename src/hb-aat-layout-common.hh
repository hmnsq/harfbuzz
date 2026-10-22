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

#جے_بنا_تعریف HB_AAT_LAYOUT_COMMON_HH
#تعریف_کرو HB_AAT_LAYOUT_COMMON_HH

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

ورتوں نطاق OT;

بنية ankr;

ورتوں hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
#تعریف_کرو HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#وإلا
#تعریف_کرو HB_AAT_TRANSITION_INLINE
#جے_انت

بنية hb_aat_scratch_t
{
  hb_aat_scratch_t () = افتراضي;
  hb_aat_scratch_t (ثابت hb_aat_scratch_t &) = مٹاؤ;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    بفر_glyph_سیٹ.سیٹ_relaxed (o.بفر_glyph_سیٹ.get_relaxed ());
    o.بفر_glyph_سیٹ.سیٹ_relaxed (خالی_سوچک);
  }
  hb_aat_scratch_t & عامل = (hb_aat_scratch_t &&o)
  {
    بفر_glyph_سیٹ.سیٹ_relaxed (o.بفر_glyph_سیٹ.get_relaxed ());
    o.بفر_glyph_سیٹ.سیٹ_relaxed (خالی_سوچک);
    أرجع *هذا;
  }
  ~hb_aat_scratch_t ()
  {
    خودکار *s = بفر_glyph_سیٹ.get_relaxed ();
    إذا (unlikely (!s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  hb_ثنائی_ہندسہ_سیٹ_t *create_بفر_glyph_سیٹ () ثابت
  {
    hb_ثنائی_ہندسہ_سیٹ_t *s = بفر_glyph_سیٹ.get_acquire ();
    إذا (s && بفر_glyph_سیٹ.cmpexch (s, خالی_سوچک))
    {
      s->clear ();
      أرجع s;
    }

    s = (hb_ثنائی_ہندسہ_سیٹ_t *) hb_calloc (1, حجم (hb_ثنائی_ہندسہ_سیٹ_t));
    إذا (unlikely (!s))
      أرجع خالی_سوچک;
    s->init ();

    أرجع s;
  }
  فراغ destroy_بفر_glyph_سیٹ (hb_ثنائی_ہندسہ_سیٹ_t *s) ثابت
  {
    إذا (unlikely (!s))
      أرجع;
    إذا (بفر_glyph_سیٹ.cmpexch (خالی_سوچک, s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  بدلنجوگ hb_atomic_t<hb_ثنائی_ہندسہ_سیٹ_t *> بفر_glyph_سیٹ;
};

تعداد { DELETED_GLYPH = 0xFFFF };

#تعریف_کرو HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED HB_بفر_SCRATCH_FLAG_SHAPER0

بنية hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  سانچا <قسم_ناں T, قسم_ناں ...Ts>
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
  ثابت hb_sorted_سمتیہ_t<hb_aat_map_t::range_flags_t> *range_flags = خالی_سوچک;
  hb_نقاب_t subtable_flags = 0;
  منطقي بفر_is_reversed = خطأ;
  // Caches
  منطقي using_بفر_glyph_سیٹ = خطأ;
  hb_ثنائی_ہندسہ_سیٹ_t *بفر_glyph_سیٹ = خالی_سوچک;
  ثابت hb_ثنائی_ہندسہ_سیٹ_t *پہلا_سیٹ = خالی_سوچک;
  ثابت hb_ثنائی_ہندسہ_سیٹ_t *دوجا_سیٹ = خالی_سوچک;
  hb_aat_class_cache_t *machine_class_cache = خالی_سوچک;

  /* Unused. For debug tracing only. */
  بنانشانی صحيح lookup_اشاریہ;

  HB_اندرونی hb_aat_apply_context_t (ثابت hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_بفر_t *بفر_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_اندرونی ~hb_aat_apply_context_t ();

  HB_اندرونی فراغ سیٹ_ankr_جدول (ثابت AAT::ankr *ankr_جدول_2);

  فراغ سیٹ_lookup_اشاریہ (بنانشانی صحيح i) { lookup_اشاریہ = i; }

  فراغ الٹ_بفر ()
  {
    بفر->reverse ();
    بفر_is_reversed = !بفر_is_reversed;
  }

  فراغ تیاری_بفر_glyph_سیٹ ()
  {
    using_بفر_glyph_سیٹ = بفر->len >= 4 && بفر_glyph_سیٹ;

    إذا (likely (using_بفر_glyph_سیٹ))
      بفر->collect_codepoints (*بفر_glyph_سیٹ);
  }
  منطقي بفر_intersects_machine () ثابت
  {
    إذا (likely (using_بفر_glyph_سیٹ))
      أرجع بفر_glyph_سیٹ->intersects (*پہلا_سیٹ);

    // Faster for shorter buffers.
    لكل (بنانشانی i = 0; i < بفر->len; i++)
      إذا (پہلا_سیٹ->has (بفر->info[i].codepoint))
	أرجع سچ;
    أرجع خطأ;
  }

  سانچا <قسم_ناں T>
  HB_NODISCARD منطقي output_glyphs (بنانشانی صحيح گنتی,
				   ثابت T *glyphs)
  {
    إذا (likely (using_بفر_glyph_سیٹ))
      بفر_glyph_سیٹ->جوڑو_array (glyphs, گنتی);
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      منطقي at_سرا_دا_متن = بفر->idx == بفر->len;
      إذا (at_سرا_دا_متن)
	إذا (unlikely (!بفر->output_glyph (glyphs[i]))) أرجع خطأ;

      hb_glyph_info_t &info = at_سرا_دا_متن ? بفر->prev () : بفر->cur ();
      إذا (glyphs[i] == DELETED_GLYPH)
      {
        بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
	hb_glyph_info_سیٹ_aat_deleted (&info);
      }
      وإلا
      {
#جے_بنا_تعریف HB_NO_OT_LAYOUT
	إذا (has_glyph_classes)
	  hb_glyph_info_سیٹ_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#جے_انت
      }
      إذا (!at_سرا_دا_متن)
	إذا (unlikely (!بفر->output_glyph (glyphs[i]))) أرجع خطأ;
    }
    أرجع سچ;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph)
  {
    إذا (glyph == DELETED_GLYPH)
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_سیٹ_aat_deleted (&بفر->cur());
    }

    إذا (likely (using_بفر_glyph_سیٹ))
      بفر_glyph_سیٹ->جوڑو (glyph);
#جے_بنا_تعریف HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      hb_glyph_info_سیٹ_glyph_props (&بفر->cur(),
				      gdef.get_glyph_props (glyph));
#جے_انت
    أرجع بفر->replace_glyph (glyph);
  }

  HB_NODISCARD منطقي delete_glyph ()
  {
    بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
    hb_glyph_info_سیٹ_aat_deleted (&بفر->cur());
    إذا (likely (using_بفر_glyph_سیٹ))
      بفر_glyph_سیٹ->جوڑو (DELETED_GLYPH);
    أرجع بفر->replace_glyph (DELETED_GLYPH);
  }

  فراغ replace_glyph_inplace (بنانشانی i, hb_codepoint_t glyph)
  {
    بفر->info[i].codepoint = glyph;
    إذا (glyph == DELETED_GLYPH)
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_سیٹ_aat_deleted (&بفر->info[i]);
    }
    إذا (likely (using_بفر_glyph_سیٹ))
      بفر_glyph_سیٹ->جوڑو (glyph);
#جے_بنا_تعریف HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      hb_glyph_info_سیٹ_glyph_props (&بفر->info[i],
				      gdef.get_glyph_props (glyph));
#جے_انت
  }
};


/*
 * Lookup Table
 */

سانچا <قسم_ناں T> بنية Lookup;

سانچا <قسم_ناں T>
بنية LookupFormat0
{
  یار بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id, بنانشانی صحيح عدد_glyphs) ثابت
  {
    إذا (unlikely (glyph_id >= عدد_glyphs)) أرجع خالی_سوچک;
    أرجع &arrayZ[glyph_id];
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs, بنانشانی عدد_glyphs) ثابت
  {
    glyphs.جوڑو_range (0, عدد_glyphs - 1);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, بنانشانی عدد_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (بنانشانی i = 0; i < عدد_glyphs; i++)
      إذا (filter (arrayZ[i]))
	glyphs.جوڑو (i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_عدد_glyphs ()));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_عدد_glyphs (), ادھار));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 0 */
  Unsized_Array_دا<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  عام:
  DEFINE_ناپ_UNBOUNDED (2);
};


سانچا <قسم_ناں T>
بنية LookupSegmentSingle
{
  ساكن constexpr بنانشانی Termination_لفظ_گنتی = 2u;

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < پہلا ? -1 : g <= آخری ? 0 : +1 ; }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    إذا (پہلا == DELETED_GLYPH) أرجع;
    glyphs.جوڑو_range (پہلا, آخری);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (پہلا == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
    glyphs.جوڑو_range (پہلا, آخری);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, ادھار));
  }

  HBGlyphID16	آخری;		/* Last GlyphID in this segment */
  HBGlyphID16	پہلا;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_ناپ_STATIC (4 + T::static_ناپ);
};

سانچا <قسم_ناں T>
بنية LookupFormat2
{
  یار بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    أرجع v ? &v->value : خالی_سوچک;
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    بنانشانی گنتی = segments.get_لمائی ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      segments[i].collect_glyphs (glyphs);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بنانشانی گنتی = segments.get_لمائی ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, ادھار));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_تلاش_Array_دا<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_ناپ_ARRAY (8, segments);
};

سانچا <قسم_ناں T>
بنية LookupSegmentArray
{
  ساكن constexpr بنانشانی Termination_لفظ_گنتی = 2u;

  ثابت T* get_قدر (hb_codepoint_t glyph_id, ثابت فراغ *ادھار) ثابت
  {
    أرجع پہلا <= glyph_id && glyph_id <= آخری ? &(ادھار+اقدار_Z)[glyph_id - پہلا] : خالی_سوچک;
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    إذا (پہلا == DELETED_GLYPH) أرجع;
    glyphs.جوڑو_range (پہلا, آخری);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, ثابت فراغ *ادھار, ثابت filter_t &filter) ثابت
  {
    إذا (پہلا == DELETED_GLYPH) أرجع;
    ثابت خودکار &values = ادھار+اقدار_Z;
    لكل (hb_codepoint_t i = پہلا; i <= آخری; i++)
      إذا (filter (values[i - پہلا]))
	glyphs.جوڑو (i);
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < پہلا ? -1 : g <= آخری ? 0 : +1; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  پہلا <= آخری &&
		  اقدار_Z.sanitize (c, ادھار, آخری - پہلا + 1));
  }
  سانچا <قسم_ناں ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  پہلا <= آخری &&
		  اقدار_Z.sanitize (c, ادھار, آخری - پہلا + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	آخری;		/* Last GlyphID in this segment */
  HBGlyphID16	پہلا;		/* First GlyphID in this segment */
  NNOffset16_منزل<Unsized_Array_دا<T>>
		اقدار_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  عام:
  DEFINE_ناپ_STATIC (6);
};

سانچا <قسم_ناں T>
بنية LookupFormat4
{
  یار بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    أرجع v ? v->get_قدر (glyph_id, هذا) : خالی_سوچک;
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    بنانشانی گنتی = segments.get_لمائی ();
    لكل (بنانشانی i = 0; i < گنتی; i++)
      segments[i].collect_glyphs (glyphs);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بنانشانی گنتی = segments.get_لمائی ();
    لكل (بنانشانی i = 0; i < گنتی; i++)
      segments[i].collect_glyphs_filtered (glyphs, هذا, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا, ادھار));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_تلاش_Array_دا<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_ناپ_ARRAY (8, segments);
};

سانچا <قسم_ناں T>
بنية LookupSingle
{
  ساكن constexpr بنانشانی Termination_لفظ_گنتی = 1u;

  صحيح cmp (hb_codepoint_t g) ثابت { أرجع glyph.cmp (g); }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    glyphs.جوڑو (glyph);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
    glyphs.جوڑو (glyph);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, ادھار));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_ناپ_STATIC (2 + T::static_ناپ);
};

سانچا <قسم_ناں T>
بنية LookupFormat6
{
  یار بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSingle<T> *v = entries.bsearch (glyph_id);
    أرجع v ? &v->value : خالی_سوچک;
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    بنانشانی گنتی = entries.get_لمائی ();
    لكل (بنانشانی i = 0; i < گنتی; i++)
      entries[i].collect_glyphs (glyphs);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بنانشانی گنتی = entries.get_لمائی ();
    لكل (بنانشانی i = 0; i < گنتی; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c, ادھار));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_تلاش_Array_دا<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  عام:
  DEFINE_ناپ_ARRAY (8, entries);
};

سانچا <قسم_ناں T>
بنية LookupFormat8
{
  یار بنية Lookup<T>;

  خاص:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    أرجع پہلا_Glyph <= glyph_id && glyph_id - پہلا_Glyph < glyph_گنتی_2 ?
	   &قدر_Array_Z[glyph_id - پہلا_Glyph] : خالی_سوچک;
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_گنتی_2)) أرجع;
    إذا (پہلا_Glyph == DELETED_GLYPH) أرجع;
    glyphs.جوڑو_range (پہلا_Glyph, پہلا_Glyph + glyph_گنتی_2 - 1);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_گنتی_2)) أرجع;
    إذا (پہلا_Glyph == DELETED_GLYPH) أرجع;
    ثابت T *p = قدر_Array_Z.arrayZ;
    لكل (بنانشانی i = 0; i < glyph_گنتی_2; i++)
      إذا (filter (p[i]))
	glyphs.جوڑو (پہلا_Glyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && قدر_Array_Z.sanitize (c, glyph_گنتی_2));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && قدر_Array_Z.sanitize (c, glyph_گنتی_2, ادھار));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	پہلا_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_گنتی_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_دا<T>
		قدر_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_ناپ_ARRAY (6, قدر_Array_Z);
};

سانچا <قسم_ناں T>
بنية LookupFormat10
{
  یار بنية Lookup<T>;

  خاص:
  ثابت قسم_ناں T::type get_قدر_or_null (hb_codepoint_t glyph_id) ثابت
  {
    إذا (!(پہلا_Glyph <= glyph_id && glyph_id - پہلا_Glyph < glyph_گنتی_2))
      أرجع Null (T);

    ثابت HBUINT8 *p = &قدر_Array_Z[(glyph_id - پہلا_Glyph) * قدر_ناپ];

    بنانشانی صحيح v = 0;
    بنانشانی صحيح گنتی = قدر_ناپ;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      v = (v << 8) | *p++;

    أرجع v;
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_گنتی_2)) أرجع;
    إذا (پہلا_Glyph == DELETED_GLYPH) أرجع;
    glyphs.جوڑو_range (پہلا_Glyph, پہلا_Glyph + glyph_گنتی_2 - 1);
  }

  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_گنتی_2)) أرجع;
    إذا (پہلا_Glyph == DELETED_GLYPH) أرجع;
    ثابت HBUINT8 *p = قدر_Array_Z.arrayZ;
    لكل (بنانشانی i = 0; i < glyph_گنتی_2; i++)
    {
      بنانشانی صحيح v = 0;
      بنانشانی صحيح گنتی = قدر_ناپ;
      لكل (بنانشانی صحيح j = 0; j < گنتی; j++)
	v = (v << 8) | *p++;
      إذا (filter (v))
	glyphs.جوڑو (پہلا_Glyph + i);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  قدر_ناپ <= 4 &&
		  قدر_Array_Z.sanitize (c, glyph_گنتی_2 * قدر_ناپ));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	قدر_ناپ;	/* Byte size of each value. */
  HBGlyphID16	پہلا_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_گنتی_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_دا<HBUINT8>
		قدر_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_ناپ_ARRAY (8, قدر_Array_Z);
};

سانچا <قسم_ناں T>
بنية Lookup
{
  ثابت T* get_قدر (hb_codepoint_t glyph_id, بنانشانی صحيح عدد_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_قدر (glyph_id, عدد_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_قدر (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_قدر (glyph_id);
    حالة 6: hb_barrier (); أرجع u.format6.get_قدر (glyph_id);
    حالة 8: hb_barrier (); أرجع u.format8.get_قدر (glyph_id);
    افتراضي:أرجع خالی_سوچک;
    }
  }

  ثابت قسم_ناں T::type get_قدر_or_null (hb_codepoint_t glyph_id, بنانشانی صحيح عدد_glyphs) ثابت
  {
    بدل (u.format.v) {
      /* Format 10 cannot return a pointer. */
      حالة 10: hb_barrier (); أرجع u.format10.get_قدر_or_null (glyph_id);
      افتراضي:
      ثابت T *v = get_قدر (glyph_id, عدد_glyphs);
      أرجع v ? *v : Null (T);
    }
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs, بنانشانی صحيح عدد_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); u.format0.collect_glyphs (glyphs, عدد_glyphs); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyphs (glyphs); أرجع;
    حالة 4: hb_barrier (); u.format4.collect_glyphs (glyphs); أرجع;
    حالة 6: hb_barrier (); u.format6.collect_glyphs (glyphs); أرجع;
    حالة 8: hb_barrier (); u.format8.collect_glyphs (glyphs); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_glyphs (glyphs); أرجع;
    افتراضي:أرجع;
    }
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, بنانشانی عدد_glyphs, ثابت filter_t &filter) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, عدد_glyphs, filter); أرجع;
    حالة 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); أرجع;
    حالة 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); أرجع;
    افتراضي:أرجع;
    }
  }

  قسم_ناں T::type get_class (hb_codepoint_t glyph_id,
			      بنانشانی صحيح عدد_glyphs,
			      بنانشانی صحيح باہرلا_دا_Range) ثابت
  {
    ثابت T *v = get_قدر (glyph_id, عدد_glyphs);
    أرجع v ? *v : باہرلا_دا_Range;
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
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, ادھار));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, ادھار));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, ادھار));
    حالة 6: hb_barrier (); return_trace (u.format6.sanitize (c, ادھار));
    حالة 8: hb_barrier (); return_trace (u.format8.sanitize (c, ادھار));
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
  DEFINE_ناپ_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_بائٹاں_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

سانچا <قسم_ناں T>
بنية اندراج_2
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
    static_assert (T::static_ناپ, "");

    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		انکڑے;		/* Optional offsets to per-glyph tables. */
  عام:
  DEFINE_ناپ_STATIC (4 + T::static_ناپ);
};

سانچا <>
بنية اندراج_2<فراغ>
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
  DEFINE_ناپ_STATIC (4);
};

تعداد Class
{
  CLASS_سرا_دا_متن = 0,
  CLASS_باہرلا_دا_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_سرا_دا_سطر = 3,
};

سانچا <قسم_ناں Types, قسم_ناں Extra>
بنية State_جدول
{
  تعريفنوع قسم_ناں Types::HBUINT HBUINT;
  تعريفنوع قسم_ناں Types::HBUSHORT HBUSHORT;
  تعريفنوع قسم_ناں Types::ClassTypeNarrow ClassType;

  تعداد State
  {
    STATE_شروع_دا_متن = 0,
    STATE_شروع_دا_سطر = 1,
  };

  سانچا <قسم_ناں سیٹ_t, قسم_ناں جدول_t>
  فراغ collect_initial_glyphs (سیٹ_t &glyphs, بنانشانی عدد_glyphs, ثابت جدول_t &جدول) ثابت
  {
    بنانشانی عدد_classes = nClasses;

    إذا (unlikely (عدد_classes > hb_ثنائی_ہندسہ_ورقہ_t::بٹ_2))
    {
      (هذا+class_جدول).collect_glyphs (glyphs, عدد_glyphs);
      أرجع;
    }

    // Collect all classes going out from the start state.
    hb_ثنائی_ہندسہ_ورقہ_t filter;

    لكل (بنانشانی i = 0; i < عدد_classes; i++)
    {
      ثابت خودکار &اندراج = get_اندراج (STATE_شروع_دا_متن, i);
      إذا (new_state (اندراج.newState) == STATE_شروع_دا_متن &&
	  !جدول.is_action_initiable (اندراج) && !جدول.is_actionable (اندراج))
	استمر;

      filter.جوڑو (i);
    }

    // And glyphs in those classes.

    إذا (filter (CLASS_DELETED_GLYPH))
      glyphs.جوڑو (DELETED_GLYPH);

    (هذا+class_جدول).collect_glyphs_filtered (glyphs, عدد_glyphs, filter);
  }

  صحيح new_state (بنانشانی صحيح newState) ثابت
  { أرجع Types::extended ? newState : ((صحيح) newState - (صحيح) state_Array_جدول) / (صحيح) nClasses; }

  بنانشانی صحيح get_class (hb_codepoint_t glyph_id,
			  بنانشانی صحيح عدد_glyphs,
			  hb_aat_class_cache_t *cache = خالی_سوچک) ثابت
  {
    بنانشانی klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    إذا (unlikely (glyph_id == DELETED_GLYPH)) أرجع CLASS_DELETED_GLYPH;
    klass = (هذا+class_جدول).get_class (glyph_id, عدد_glyphs, CLASS_باہرلا_دا_BOUNDS);
    إذا (cache) cache->سیٹ (glyph_id, klass);
    أرجع klass;
  }

  ثابت اندراج_2<Extra> *get_entries () ثابت
  { أرجع (هذا+اندراج_جدول).arrayZ; }

  ثابت اندراج_2<Extra> &get_اندراج (صحيح state, بنانشانی صحيح klass) ثابت
  {
    بنانشانی n_classes = nClasses;
    إذا (unlikely (klass >= n_classes))
      klass = CLASS_باہرلا_دا_BOUNDS;

    ثابت HBUSHORT *حالتاں = (هذا+state_Array_جدول).arrayZ;
    ثابت اندراج_2<Extra> *entries = (هذا+اندراج_جدول).arrayZ;

    بنانشانی صحيح اندراج = حالتاں[state * n_classes + klass];
    DEBUG_پیغام (APPLY, خالی_سوچک, "e%u", اندراج);

    أرجع entries[اندراج];
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 بنانشانی صحيح *عدد_entries_باہرلا = خالی_سوچک) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    class_جدول.sanitize (c, هذا)))) return_trace (خطأ);

    ثابت HBUSHORT *حالتاں = (هذا+state_Array_جدول).arrayZ;
    ثابت اندراج_2<Extra> *entries = (هذا+اندراج_جدول).arrayZ;

    بنانشانی صحيح عدد_classes = nClasses;
    إذا (unlikely (hb_unsigned_mul_overflows (عدد_classes, حالتاں[0].static_ناپ)))
      return_trace (خطأ);
    بنانشانی صحيح صف_stride = عدد_classes * حالتاں[0].static_ناپ;

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
    بنانشانی صحيح عدد_entries = 0;

    صحيح state_pos = 0;
    صحيح state_neg = 0;
    بنانشانی صحيح اندراج = 0;
    طالما (min_state < state_neg || state_pos <= max_state)
    {
      إذا (min_state < state_neg)
      {
	/* Negative states. */
	إذا (unlikely (hb_unsigned_mul_overflows (min_state, عدد_classes)))
	  return_trace (خطأ);
	إذا (unlikely (!c->check_range (&حالتاں[min_state * عدد_classes],
				       -min_state,
				       صف_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  ثابت HBUSHORT *stop = &حالتاں[min_state * عدد_classes];
	  إذا (unlikely (stop > حالتاں))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = حالتاں; stop < p; p--)
	    عدد_entries = hb_max (عدد_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      إذا (state_pos <= max_state)
      {
	/* Positive states. */
	إذا (unlikely (!c->check_range (حالتاں,
				       max_state + 1,
				       صف_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  إذا (unlikely (hb_unsigned_mul_overflows ((max_state + 1), عدد_classes)))
	    return_trace (خطأ);
	  ثابت HBUSHORT *stop = &حالتاں[(max_state + 1) * عدد_classes];
	  إذا (unlikely (stop < حالتاں))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = &حالتاں[state_pos * عدد_classes]; p < stop; p++)
	    عدد_entries = hb_max (عدد_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      إذا (unlikely (!c->check_array (entries, عدد_entries)))
	return_trace (خطأ);
      إذا ((c->max_ops -= عدد_entries - اندراج) <= 0)
	return_trace (خطأ);
      { /* Sweep new entries. */
	ثابت اندراج_2<Extra> *stop = &entries[عدد_entries];
	لكل (ثابت اندراج_2<Extra> *p = &entries[اندراج]; p < stop; p++)
	{
	  صحيح newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	اندراج = عدد_entries;
      }
    }

    إذا (عدد_entries_باہرلا)
      *عدد_entries_باہرلا = عدد_entries;

    return_trace (سچ);
  }

  محمي:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffset_منزل<ClassType, HBUINT>
		class_جدول;	/* Offset to the class table. */
  NNOffset_منزل<Unsized_Array_دا<HBUSHORT>, HBUINT>
		state_Array_جدول;/* Offset to the state array. */
  NNOffset_منزل<Unsized_Array_دا<اندراج_2<Extra>>, HBUINT>
		اندراج_جدول;	/* Offset to the entry array. */

  عام:
  DEFINE_ناپ_STATIC (4 * حجم (HBUINT));
};

سانچا <قسم_ناں HBUCHAR>
بنية Class_جدول
{
  بنانشانی صحيح get_class (hb_codepoint_t glyph_id, بنانشانی صحيح باہرلا_دا_Range) ثابت
  {
    بنانشانی صحيح i = glyph_id - پہلا_Glyph;
    أرجع i >= classArray.len ? باہرلا_دا_Range : classArray.arrayZ[i];
  }
  بنانشانی صحيح get_class (hb_codepoint_t glyph_id,
			  بنانشانی صحيح عدد_glyphs HB_بن_ورتیا,
			  بنانشانی صحيح باہرلا_دا_Range) ثابت
  {
    أرجع get_class (glyph_id, باہرلا_دا_Range);
  }

  سانچا <قسم_ناں سیٹ_t>
  فراغ collect_glyphs (سیٹ_t &glyphs, بنانشانی عدد_glyphs) ثابت
  {
    لكل (بنانشانی i = 0; i < classArray.len; i++)
      إذا (classArray.arrayZ[i] != CLASS_باہرلا_دا_BOUNDS)
	glyphs.جوڑو (پہلا_Glyph + i);
  }
  سانچا <قسم_ناں سیٹ_t, قسم_ناں filter_t>
  فراغ collect_glyphs_filtered (سیٹ_t &glyphs, بنانشانی عدد_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (بنانشانی i = 0; i < classArray.len; i++)
      إذا (filter (classArray.arrayZ[i]))
	glyphs.جوڑو (پہلا_Glyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && classArray.sanitize (c));
  }
  محمي:
  HBGlyphID16		پہلا_Glyph;	/* First glyph index included in the trimmed array. */
  Array16_دا<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  عام:
  DEFINE_ناپ_ARRAY (4, classArray);
};

بنية SubtableGlyphCoverage
{
  منطقي sanitize (hb_sanitize_context_t *c, بنانشانی subtable_گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_array (&subtableOffsets, subtable_گنتی)))
      return_trace (خطأ);

    بنانشانی بائٹاں = (c->get_عدد_glyphs () + CHAR_ثنائی_ہندسہ - 1) / CHAR_ثنائی_ہندسہ;
    لكل (بنانشانی i = 0; i < subtable_گنتی; i++)
    {
      uint32_t وتھ = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      إذا (وتھ == 0 || وتھ == 0xFFFFFFFF)
        استمر;
      إذا (unlikely (!subtableOffsets[i].sanitize (c, هذا, بائٹاں)))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }
  محمي:
  Unsized_Array_دا<NNOffset32_منزل<Unsized_Array_دا<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  عام:
  DEFINE_ناپ_ARRAY (0, subtableOffsets);
};

بنية ObsoleteTypes
{
  ساكن constexpr منطقي extended = خطأ;
  تعريفنوع HBUINT16 HBUINT;
  تعريفنوع HBUINT8 HBUSHORT;
  تعريفنوع Class_جدول<HBUINT8> ClassTypeNarrow;
  تعريفنوع Class_جدول<HBUINT16> ClassTypeWide;

  سانچا <قسم_ناں T>
  ساكن صحيح وتھ_منزل_اشاریہ (int64_t وتھ,
			    ثابت فراغ *ادھار,
			    ثابت T *array)
  {
    int64_t array_وتھ = (ثابت حرف *) array - (ثابت حرف *) ادھار;
    صحيح bad_اشاریہ = ودھ_توں_ودھ_عدد / T::static_ناپ;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    إذا (unlikely (وتھ < array_وتھ))
      أرجع bad_اشاریہ;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t اشاریہ_2 = (وتھ - array_وتھ) / T::static_ناپ;
    إذا (unlikely (اشاریہ_2 > bad_اشاریہ))
      أرجع bad_اشاریہ;
    أرجع اشاریہ_2;
  }
  سانچا <قسم_ناں T>
  ساكن صحيح byte_وتھ_منزل_اشاریہ (int64_t وتھ,
				ثابت فراغ *ادھار,
				ثابت T *array)
  {
    أرجع وتھ_منزل_اشاریہ (وتھ, ادھار, array);
  }
  سانچا <قسم_ناں T>
  ساكن صحيح لفظ_وتھ_منزل_اشاریہ (int64_t وتھ,
				ثابت فراغ *ادھار,
				ثابت T *array)
  {
    أرجع وتھ_منزل_اشاریہ (2 * وتھ, ادھار, array);
  }
};
بنية ExtendedTypes
{
  ساكن constexpr منطقي extended = سچ;
  تعريفنوع HBUINT32 HBUINT;
  تعريفنوع HBUINT16 HBUSHORT;
  تعريفنوع Lookup<HBUINT16> ClassTypeNarrow;
  تعريفنوع Lookup<HBUINT16> ClassTypeWide;

  سانچا <قسم_ناں T>
  ساكن بنانشانی صحيح وتھ_منزل_اشاریہ (بنانشانی صحيح وتھ,
				     ثابت فراغ *ادھار HB_بن_ورتیا,
				     ثابت T *array HB_بن_ورتیا)
  {
    أرجع وتھ;
  }
  سانچا <قسم_ناں T>
  ساكن بنانشانی صحيح byte_وتھ_منزل_اشاریہ (بنانشانی صحيح وتھ,
					 ثابت فراغ *ادھار HB_بن_ورتیا,
					 ثابت T *array HB_بن_ورتیا)
  {
    أرجع وتھ / 2;
  }
  سانچا <قسم_ناں T>
  ساكن بنانشانی صحيح لفظ_وتھ_منزل_اشاریہ (بنانشانی صحيح وتھ,
					 ثابت فراغ *ادھار HB_بن_ورتیا,
					 ثابت T *array HB_بن_ورتیا)
  {
    أرجع وتھ;
  }
};

سانچا <قسم_ناں Types, قسم_ناں اندراج_انکڑے, قسم_ناں Flags>
بنية State_جدول_Driver
{
  ورتوں State_جدول_T = State_جدول<Types, اندراج_انکڑے>;
  ورتوں اندراج_T = اندراج_2<اندراج_انکڑے>;

  State_جدول_Driver (ثابت State_جدول_T &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      عدد_glyphs (face_->get_عدد_glyphs ()) {}

  سانچا <قسم_ناں context_t>
  فراغ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_بفر_t *بفر = ac->بفر;

    إذا (!c->in_place)
      بفر->clear_output ();

    صحيح state = State_جدول_T::STATE_شروع_دا_متن;
    // If there's only one range, we already checked the flag.
    خودکار *آخری_range = ac->range_flags && (ac->range_flags->لمائی > 1) ? &(*ac->range_flags)[0] : خالی_سوچک;
    ثابت منطقي شروع_state_محفوظ_منزل_break_eot =
      !c->جدول->is_actionable (machine.get_اندراج (State_جدول_T::STATE_شروع_دا_متن, CLASS_سرا_دا_متن));
    لكل (بفر->idx = 0; بفر->successful;)
    {
      بنانشانی صحيح klass = likely (بفر->idx < بفر->len) ?
			   machine.get_class (بفر->cur().codepoint, عدد_glyphs, ac->machine_class_cache) :
			   (بنانشانی) CLASS_سرا_دا_متن;
    resume:
      DEBUG_پیغام (APPLY, خالی_سوچک, "c%u at %u", klass, بفر->idx);
      ثابت اندراج_T &اندراج = machine.get_اندراج (state, klass);
      ثابت صحيح اگلا_state = machine.new_state (اندراج.newState);

      منطقي is_not_epsilon_transition = !(اندراج.flags & Flags::DontAdvance);
      منطقي is_not_actionable = !c->جدول->is_actionable (اندراج);

      إذا (unlikely (آخری_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	خودکار *range = آخری_range;
	إذا (بفر->idx < بفر->len)
	{
	  بنانشانی cluster = بفر->cur().cluster;
	  طالما (cluster < range->cluster_پہلا)
	    range--;
	  طالما (cluster > range->cluster_آخری)
	    range++;


	  آخری_range = range;
	}
	إذا (!(range->flags & ac->subtable_flags))
	{
	  إذا (بفر->idx == بفر->len)
	    اكسر;

	  state = State_جدول_T::STATE_شروع_دا_متن;
	  (فراغ) بفر->اگلا_glyph ();
	  استمر;
	}
      }
      وإلا
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	منطقي is_null_transition = state == State_جدول_T::STATE_شروع_دا_متن &&
				  اگلا_state == State_جدول_T::STATE_شروع_دا_متن &&
				  شروع_state_محفوظ_منزل_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	إذا (is_null_transition)
	{
	  بنانشانی old_klass = klass;
	  افعل
	  {
	    c->transition (بفر, هذا, اندراج);

	    إذا (بفر->idx == بفر->len || !بفر->successful)
	      اكسر;

	    (فراغ) بفر->اگلا_glyph ();

	    klass = likely (بفر->idx < بفر->len) ?
		     machine.get_class (بفر->cur().codepoint, عدد_glyphs, ac->machine_class_cache) :
		     (بنانشانی) CLASS_سرا_دا_متن;
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
      ثابت اندراج_T *wouldbe_اندراج;
      منطقي is_محفوظ_منزل_break =
      (
          /* 1. */
          !c->جدول->is_actionable (اندراج) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == State_جدول_T::STATE_شروع_دا_متن
              || ((اندراج.flags & Flags::DontAdvance) && اگلا_state == State_جدول_T::STATE_شروع_دا_متن)
              || (
		    /* 2c. */
		    wouldbe_اندراج = &machine.get_اندراج(State_جدول_T::STATE_شروع_دا_متن, klass)
		    ,
		    /* 2c'. */
		    !c->جدول->is_actionable (*wouldbe_اندراج) &&
		    /* 2c". */
		    (
		      اگلا_state == machine.new_state(wouldbe_اندراج->newState) &&
		      (اندراج.flags & Flags::DontAdvance) == (wouldbe_اندراج->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->جدول->is_actionable (machine.get_اندراج (state, CLASS_سرا_دا_متن))
      );

      إذا (!is_محفوظ_منزل_break && بفر->backtrack_len () && بفر->idx < بفر->len)
	بفر->unsafe_منزل_break_from_outbuffer (بفر->backtrack_len () - 1, بفر->idx + 1);

      c->transition (بفر, هذا, اندراج);

      state = اگلا_state;
      DEBUG_پیغام (APPLY, خالی_سوچک, "s%d", state);

      إذا (بفر->idx == بفر->len)
	اكسر;

      إذا (is_not_epsilon_transition || بفر->max_ops-- <= 0)
	(فراغ) بفر->اگلا_glyph ();
    }

    إذا (!c->in_place)
      بفر->متزامن ();
  }

  عام:
  ثابت State_جدول_T &machine;
  بنانشانی صحيح عدد_glyphs;
};


} /* namespace AAT */


#جے_انت /* HB_AAT_LAYOUT_COMMON_HH */
