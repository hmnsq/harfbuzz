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

#اگر_غیر_تعریف_شدہ HB_AAT_LAYOUT_COMMON_HH
#تعریف_کرو HB_AAT_LAYOUT_COMMON_HH

#شامل "hb-aat-layout.hh"
#شامل "hb-aat-map.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-open-type.hh"
#شامل "hb-cache.hh"
#شامل "hb-bit-set.hh"
#شامل "hb-bit-page.hh"


نامخلا OT {
ساخت GDEF;
};

نامخلا AAT {

استعمال نامخلا OT;

ساخت ankr;

استعمال hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
#تعریف_کرو HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#ورنہ
#تعریف_کرو HB_AAT_TRANSITION_INLINE
#اگر_ختم

ساخت hb_aat_scratch_t
{
  hb_aat_scratch_t () = طےشدہ;
  hb_aat_scratch_t (ثابت hb_aat_scratch_t &) = حذف;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    بفر_glyph_سیٹ.سیٹ_relaxed (o.بفر_glyph_سیٹ.get_relaxed ());
    o.بفر_glyph_سیٹ.سیٹ_relaxed (خالی_اشاریہ);
  }
  hb_aat_scratch_t & عامل = (hb_aat_scratch_t &&o)
  {
    بفر_glyph_سیٹ.سیٹ_relaxed (o.بفر_glyph_سیٹ.get_relaxed ());
    o.بفر_glyph_سیٹ.سیٹ_relaxed (خالی_اشاریہ);
    واپس *یہ;
  }
  ~hb_aat_scratch_t ()
  {
    خودکار *s = بفر_glyph_سیٹ.get_relaxed ();
    اگر (unlikely (!s))
      واپس;
    s->fini ();
    hb_free (s);
  }

  hb_ثنائی_ہندسہ_سیٹ_t *create_بفر_glyph_سیٹ () ثابت
  {
    hb_ثنائی_ہندسہ_سیٹ_t *s = بفر_glyph_سیٹ.get_acquire ();
    اگر (s && بفر_glyph_سیٹ.cmpexch (s, خالی_اشاریہ))
    {
      s->clear ();
      واپس s;
    }

    s = (hb_ثنائی_ہندسہ_سیٹ_t *) hb_calloc (1, حجم (hb_ثنائی_ہندسہ_سیٹ_t));
    اگر (unlikely (!s))
      واپس خالی_اشاریہ;
    s->init ();

    واپس s;
  }
  خالی destroy_بفر_glyph_سیٹ (hb_ثنائی_ہندسہ_سیٹ_t *s) ثابت
  {
    اگر (unlikely (!s))
      واپس;
    اگر (بفر_glyph_سیٹ.cmpexch (خالی_اشاریہ, s))
      واپس;
    s->fini ();
    hb_free (s);
  }

  تغیرپذیر hb_atomic_t<hb_ثنائی_ہندسہ_سیٹ_t *> بفر_glyph_سیٹ;
};

شمار { DELETED_GLYPH = 0xFFFF };

#تعریف_کرو HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED HB_بفر_SCRATCH_FLAG_SHAPER0

ساخت hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, منطقی, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { واپس "APPLY"; }
  سانچہ <قسم_نام T, قسم_نام ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { واپس obj.apply (یہ, std::forward<Ts> (ds)...); }
  ساکن return_t default_return_قدر () { واپس جھوٹ; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس r; }

  ثابت hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t *بفر;
  hb_sanitize_context_t sanitizer;
  ثابت ankr *ankr_جدول;
  ثابت OT::GDEF &gdef;
  منطقی has_glyph_classes;
  ثابت hb_sorted_سمتیہ_t<hb_aat_map_t::range_flags_t> *range_flags = خالی_اشاریہ;
  hb_نقاب_t subtable_flags = 0;
  منطقی بفر_is_reversed = جھوٹ;
  // Caches
  منطقی using_بفر_glyph_سیٹ = جھوٹ;
  hb_ثنائی_ہندسہ_سیٹ_t *بفر_glyph_سیٹ = خالی_اشاریہ;
  ثابت hb_ثنائی_ہندسہ_سیٹ_t *پہلا_سیٹ = خالی_اشاریہ;
  ثابت hb_ثنائی_ہندسہ_سیٹ_t *دوسرا_سیٹ = خالی_اشاریہ;
  hb_aat_class_cache_t *machine_class_cache = خالی_اشاریہ;

  /* Unused. For debug tracing only. */
  غیرعلامتی صحیح lookup_اشاریہ;

  HB_اندرونی hb_aat_apply_context_t (ثابت hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_بفر_t *بفر_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_اندرونی ~hb_aat_apply_context_t ();

  HB_اندرونی خالی سیٹ_ankr_جدول (ثابت AAT::ankr *ankr_جدول_2);

  خالی سیٹ_lookup_اشاریہ (غیرعلامتی صحیح i) { lookup_اشاریہ = i; }

  خالی الٹ_بفر ()
  {
    بفر->reverse ();
    بفر_is_reversed = !بفر_is_reversed;
  }

  خالی تیاری_بفر_glyph_سیٹ ()
  {
    using_بفر_glyph_سیٹ = بفر->len >= 4 && بفر_glyph_سیٹ;

    اگر (likely (using_بفر_glyph_سیٹ))
      بفر->collect_codepoints (*بفر_glyph_سیٹ);
  }
  منطقی بفر_intersects_machine () ثابت
  {
    اگر (likely (using_بفر_glyph_سیٹ))
      واپس بفر_glyph_سیٹ->intersects (*پہلا_سیٹ);

    // Faster for shorter buffers.
    برائے (غیرعلامتی i = 0; i < بفر->len; i++)
      اگر (پہلا_سیٹ->has (بفر->info[i].codepoint))
	واپس سچ;
    واپس جھوٹ;
  }

  سانچہ <قسم_نام T>
  HB_NODISCARD منطقی output_glyphs (غیرعلامتی صحیح گنتی,
				   ثابت T *glyphs)
  {
    اگر (likely (using_بفر_glyph_سیٹ))
      بفر_glyph_سیٹ->جوڑو_array (glyphs, گنتی);
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      منطقی at_انتہا_کا_متن = بفر->idx == بفر->len;
      اگر (at_انتہا_کا_متن)
	اگر (unlikely (!بفر->output_glyph (glyphs[i]))) واپس جھوٹ;

      hb_glyph_info_t &info = at_انتہا_کا_متن ? بفر->prev () : بفر->cur ();
      اگر (glyphs[i] == DELETED_GLYPH)
      {
        بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
	hb_glyph_info_سیٹ_aat_deleted (&info);
      }
      ورنہ
      {
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT
	اگر (has_glyph_classes)
	  hb_glyph_info_سیٹ_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#اگر_ختم
      }
      اگر (!at_انتہا_کا_متن)
	اگر (unlikely (!بفر->output_glyph (glyphs[i]))) واپس جھوٹ;
    }
    واپس سچ;
  }

  HB_NODISCARD منطقی replace_glyph (hb_codepoint_t glyph)
  {
    اگر (glyph == DELETED_GLYPH)
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_سیٹ_aat_deleted (&بفر->cur());
    }

    اگر (likely (using_بفر_glyph_سیٹ))
      بفر_glyph_سیٹ->جوڑو (glyph);
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT
    اگر (has_glyph_classes)
      hb_glyph_info_سیٹ_glyph_props (&بفر->cur(),
				      gdef.get_glyph_props (glyph));
#اگر_ختم
    واپس بفر->replace_glyph (glyph);
  }

  HB_NODISCARD منطقی delete_glyph ()
  {
    بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
    hb_glyph_info_سیٹ_aat_deleted (&بفر->cur());
    اگر (likely (using_بفر_glyph_سیٹ))
      بفر_glyph_سیٹ->جوڑو (DELETED_GLYPH);
    واپس بفر->replace_glyph (DELETED_GLYPH);
  }

  خالی replace_glyph_inplace (غیرعلامتی i, hb_codepoint_t glyph)
  {
    بفر->info[i].codepoint = glyph;
    اگر (glyph == DELETED_GLYPH)
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_سیٹ_aat_deleted (&بفر->info[i]);
    }
    اگر (likely (using_بفر_glyph_سیٹ))
      بفر_glyph_سیٹ->جوڑو (glyph);
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT
    اگر (has_glyph_classes)
      hb_glyph_info_سیٹ_glyph_props (&بفر->info[i],
				      gdef.get_glyph_props (glyph));
#اگر_ختم
  }
};


/*
 * Lookup Table
 */

سانچہ <قسم_نام T> ساخت Lookup;

سانچہ <قسم_نام T>
ساخت LookupFormat0
{
  دوست ساخت Lookup<T>;

  نجی:
  ثابت T* get_قدر (hb_codepoint_t glyph_id, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    اگر (unlikely (glyph_id >= عدد_glyphs)) واپس خالی_اشاریہ;
    واپس &arrayZ[glyph_id];
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs, غیرعلامتی عدد_glyphs) ثابت
  {
    glyphs.جوڑو_range (0, عدد_glyphs - 1);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, غیرعلامتی عدد_glyphs, ثابت filter_t &filter) ثابت
  {
    برائے (غیرعلامتی i = 0; i < عدد_glyphs; i++)
      اگر (filter (arrayZ[i]))
	glyphs.جوڑو (i);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (arrayZ.sanitize (c, c->get_عدد_glyphs ()));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (arrayZ.sanitize (c, c->get_عدد_glyphs (), بنیاد));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 0 */
  Unsized_Array_کا<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  عوامی:
  DEFINE_سائز_UNBOUNDED (2);
};


سانچہ <قسم_نام T>
ساخت LookupSegmentSingle
{
  ساکن constexpr غیرعلامتی Termination_لفظ_گنتی = 2u;

  صحیح cmp (hb_codepoint_t g) ثابت
  { واپس g < پہلا ? -1 : g <= آخری ? 0 : +1 ; }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    اگر (پہلا == DELETED_GLYPH) واپس;
    glyphs.جوڑو_range (پہلا, آخری);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    اگر (پہلا == DELETED_GLYPH) واپس;
    اگر (!filter (value)) واپس;
    glyphs.جوڑو_range (پہلا, آخری);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && value.sanitize (c));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && value.sanitize (c, بنیاد));
  }

  HBGlyphID16	آخری;		/* Last GlyphID in this segment */
  HBGlyphID16	پہلا;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  عوامی:
  DEFINE_سائز_STATIC (4 + T::static_سائز);
};

سانچہ <قسم_نام T>
ساخت LookupFormat2
{
  دوست ساخت Lookup<T>;

  نجی:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    واپس v ? &v->value : خالی_اشاریہ;
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    غیرعلامتی گنتی = segments.get_لمبائی ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      segments[i].collect_glyphs (glyphs);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    غیرعلامتی گنتی = segments.get_لمبائی ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (segments.sanitize (c));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (segments.sanitize (c, بنیاد));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_تلاش_Array_کا<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عوامی:
  DEFINE_سائز_ARRAY (8, segments);
};

سانچہ <قسم_نام T>
ساخت LookupSegmentArray
{
  ساکن constexpr غیرعلامتی Termination_لفظ_گنتی = 2u;

  ثابت T* get_قدر (hb_codepoint_t glyph_id, ثابت خالی *بنیاد) ثابت
  {
    واپس پہلا <= glyph_id && glyph_id <= آخری ? &(بنیاد+اقدار_Z)[glyph_id - پہلا] : خالی_اشاریہ;
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    اگر (پہلا == DELETED_GLYPH) واپس;
    glyphs.جوڑو_range (پہلا, آخری);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, ثابت خالی *بنیاد, ثابت filter_t &filter) ثابت
  {
    اگر (پہلا == DELETED_GLYPH) واپس;
    ثابت خودکار &values = بنیاد+اقدار_Z;
    برائے (hb_codepoint_t i = پہلا; i <= آخری; i++)
      اگر (filter (values[i - پہلا]))
	glyphs.جوڑو (i);
  }

  صحیح cmp (hb_codepoint_t g) ثابت
  { واپس g < پہلا ? -1 : g <= آخری ? 0 : +1; }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  پہلا <= آخری &&
		  اقدار_Z.sanitize (c, بنیاد, آخری - پہلا + 1));
  }
  سانچہ <قسم_نام ...Ts>
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  پہلا <= آخری &&
		  اقدار_Z.sanitize (c, بنیاد, آخری - پہلا + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	آخری;		/* Last GlyphID in this segment */
  HBGlyphID16	پہلا;		/* First GlyphID in this segment */
  NNOffset16_مقصد<Unsized_Array_کا<T>>
		اقدار_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

سانچہ <قسم_نام T>
ساخت LookupFormat4
{
  دوست ساخت Lookup<T>;

  نجی:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    واپس v ? v->get_قدر (glyph_id, یہ) : خالی_اشاریہ;
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    غیرعلامتی گنتی = segments.get_لمبائی ();
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      segments[i].collect_glyphs (glyphs);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    غیرعلامتی گنتی = segments.get_لمبائی ();
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      segments[i].collect_glyphs_filtered (glyphs, یہ, filter);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (segments.sanitize (c, یہ));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (segments.sanitize (c, یہ, بنیاد));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_تلاش_Array_کا<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عوامی:
  DEFINE_سائز_ARRAY (8, segments);
};

سانچہ <قسم_نام T>
ساخت LookupSingle
{
  ساکن constexpr غیرعلامتی Termination_لفظ_گنتی = 1u;

  صحیح cmp (hb_codepoint_t g) ثابت { واپس glyph.cmp (g); }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    اگر (glyph == DELETED_GLYPH) واپس;
    glyphs.جوڑو (glyph);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    اگر (glyph == DELETED_GLYPH) واپس;
    اگر (!filter (value)) واپس;
    glyphs.جوڑو (glyph);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && value.sanitize (c));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && value.sanitize (c, بنیاد));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  عوامی:
  DEFINE_سائز_STATIC (2 + T::static_سائز);
};

سانچہ <قسم_نام T>
ساخت LookupFormat6
{
  دوست ساخت Lookup<T>;

  نجی:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSingle<T> *v = entries.bsearch (glyph_id);
    واپس v ? &v->value : خالی_اشاریہ;
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    غیرعلامتی گنتی = entries.get_لمبائی ();
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      entries[i].collect_glyphs (glyphs);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    غیرعلامتی گنتی = entries.get_لمبائی ();
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (entries.sanitize (c));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (entries.sanitize (c, بنیاد));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_تلاش_Array_کا<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  عوامی:
  DEFINE_سائز_ARRAY (8, entries);
};

سانچہ <قسم_نام T>
ساخت LookupFormat8
{
  دوست ساخت Lookup<T>;

  نجی:
  ثابت T* get_قدر (hb_codepoint_t glyph_id) ثابت
  {
    واپس پہلا_Glyph <= glyph_id && glyph_id - پہلا_Glyph < glyph_گنتی_2 ?
	   &قدر_Array_Z[glyph_id - پہلا_Glyph] : خالی_اشاریہ;
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    اگر (unlikely (!glyph_گنتی_2)) واپس;
    اگر (پہلا_Glyph == DELETED_GLYPH) واپس;
    glyphs.جوڑو_range (پہلا_Glyph, پہلا_Glyph + glyph_گنتی_2 - 1);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    اگر (unlikely (!glyph_گنتی_2)) واپس;
    اگر (پہلا_Glyph == DELETED_GLYPH) واپس;
    ثابت T *p = قدر_Array_Z.arrayZ;
    برائے (غیرعلامتی i = 0; i < glyph_گنتی_2; i++)
      اگر (filter (p[i]))
	glyphs.جوڑو (پہلا_Glyph + i);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && قدر_Array_Z.sanitize (c, glyph_گنتی_2));
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && قدر_Array_Z.sanitize (c, glyph_گنتی_2, بنیاد));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	پہلا_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_گنتی_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_کا<T>
		قدر_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عوامی:
  DEFINE_سائز_ARRAY (6, قدر_Array_Z);
};

سانچہ <قسم_نام T>
ساخت LookupFormat10
{
  دوست ساخت Lookup<T>;

  نجی:
  ثابت قسم_نام T::type get_قدر_or_null (hb_codepoint_t glyph_id) ثابت
  {
    اگر (!(پہلا_Glyph <= glyph_id && glyph_id - پہلا_Glyph < glyph_گنتی_2))
      واپس Null (T);

    ثابت HBUINT8 *p = &قدر_Array_Z[(glyph_id - پہلا_Glyph) * قدر_سائز];

    غیرعلامتی صحیح v = 0;
    غیرعلامتی صحیح گنتی = قدر_سائز;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      v = (v << 8) | *p++;

    واپس v;
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs) ثابت
  {
    اگر (unlikely (!glyph_گنتی_2)) واپس;
    اگر (پہلا_Glyph == DELETED_GLYPH) واپس;
    glyphs.جوڑو_range (پہلا_Glyph, پہلا_Glyph + glyph_گنتی_2 - 1);
  }

  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    اگر (unlikely (!glyph_گنتی_2)) واپس;
    اگر (پہلا_Glyph == DELETED_GLYPH) واپس;
    ثابت HBUINT8 *p = قدر_Array_Z.arrayZ;
    برائے (غیرعلامتی i = 0; i < glyph_گنتی_2; i++)
    {
      غیرعلامتی صحیح v = 0;
      غیرعلامتی صحیح گنتی = قدر_سائز;
      برائے (غیرعلامتی صحیح j = 0; j < گنتی; j++)
	v = (v << 8) | *p++;
      اگر (filter (v))
	glyphs.جوڑو (پہلا_Glyph + i);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  قدر_سائز <= 4 &&
		  قدر_Array_Z.sanitize (c, glyph_گنتی_2 * قدر_سائز));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	قدر_سائز;	/* Byte size of each value. */
  HBGlyphID16	پہلا_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_گنتی_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_کا<HBUINT8>
		قدر_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عوامی:
  DEFINE_سائز_ARRAY (8, قدر_Array_Z);
};

سانچہ <قسم_نام T>
ساخت Lookup
{
  ثابت T* get_قدر (hb_codepoint_t glyph_id, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); واپس u.format0.get_قدر (glyph_id, عدد_glyphs);
    حالت 2: hb_barrier (); واپس u.format2.get_قدر (glyph_id);
    حالت 4: hb_barrier (); واپس u.format4.get_قدر (glyph_id);
    حالت 6: hb_barrier (); واپس u.format6.get_قدر (glyph_id);
    حالت 8: hb_barrier (); واپس u.format8.get_قدر (glyph_id);
    طےشدہ:واپس خالی_اشاریہ;
    }
  }

  ثابت قسم_نام T::type get_قدر_or_null (hb_codepoint_t glyph_id, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    انتخاب (u.format.v) {
      /* Format 10 cannot return a pointer. */
      حالت 10: hb_barrier (); واپس u.format10.get_قدر_or_null (glyph_id);
      طےشدہ:
      ثابت T *v = get_قدر (glyph_id, عدد_glyphs);
      واپس v ? *v : Null (T);
    }
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs, غیرعلامتی صحیح عدد_glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); u.format0.collect_glyphs (glyphs, عدد_glyphs); واپس;
    حالت 2: hb_barrier (); u.format2.collect_glyphs (glyphs); واپس;
    حالت 4: hb_barrier (); u.format4.collect_glyphs (glyphs); واپس;
    حالت 6: hb_barrier (); u.format6.collect_glyphs (glyphs); واپس;
    حالت 8: hb_barrier (); u.format8.collect_glyphs (glyphs); واپس;
    حالت 10: hb_barrier (); u.format10.collect_glyphs (glyphs); واپس;
    طےشدہ:واپس;
    }
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, غیرعلامتی عدد_glyphs, ثابت filter_t &filter) ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); u.format0.collect_glyphs_filtered (glyphs, عدد_glyphs, filter); واپس;
    حالت 2: hb_barrier (); u.format2.collect_glyphs_filtered (glyphs, filter); واپس;
    حالت 4: hb_barrier (); u.format4.collect_glyphs_filtered (glyphs, filter); واپس;
    حالت 6: hb_barrier (); u.format6.collect_glyphs_filtered (glyphs, filter); واپس;
    حالت 8: hb_barrier (); u.format8.collect_glyphs_filtered (glyphs, filter); واپس;
    حالت 10: hb_barrier (); u.format10.collect_glyphs_filtered (glyphs, filter); واپس;
    طےشدہ:واپس;
    }
  }

  قسم_نام T::type get_class (hb_codepoint_t glyph_id,
			      غیرعلامتی صحیح عدد_glyphs,
			      غیرعلامتی صحیح برآمد_کا_Range) ثابت
  {
    ثابت T *v = get_قدر (glyph_id, عدد_glyphs);
    واپس v ? *v : برآمد_کا_Range;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
    حالت 6: hb_barrier (); return_trace (u.format6.sanitize (c));
    حالت 8: hb_barrier (); return_trace (u.format8.sanitize (c));
    حالت 10: hb_barrier (); return_trace (u.format10.sanitize (c));
    طےشدہ:return_trace (سچ);
    }
  }
  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c, بنیاد));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c, بنیاد));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c, بنیاد));
    حالت 6: hb_barrier (); return_trace (u.format6.sanitize (c, بنیاد));
    حالت 8: hb_barrier (); return_trace (u.format8.sanitize (c, بنیاد));
    حالت 10: return_trace (جھوٹ); /* We don't support format10 here currently. */
    طےشدہ:return_trace (سچ);
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;		/* Format identifier */
  LookupFormat0<T>	format0;
  LookupFormat2<T>	format2;
  LookupFormat4<T>	format4;
  LookupFormat6<T>	format6;
  LookupFormat8<T>	format8;
  LookupFormat10<T>	format10;
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_بائٹیں_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

سانچہ <قسم_نام T>
ساخت اندراج_3
{
  // This doesn't seem like it's ever called.
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* Note, we don't recurse-sanitize data because we don't access it.
     * That said, in our DEFINE_SIZE_STATIC we access T::static_size,
     * which ensures that data has a simple sanitize(). To be determined
     * if I need to remove that as well.
     *
     * HOWEVER! Because we are a template, our DEFINE_SIZE_STATIC
     * assertion wouldn't be checked, hence the line below. */
    static_assert (T::static_سائز, "");

    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		ڈیٹا;		/* Optional offsets to per-glyph tables. */
  عوامی:
  DEFINE_سائز_STATIC (4 + T::static_سائز);
};

سانچہ <>
ساخت اندراج_3<خالی>
{
  // This does seem like it's ever called.
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  عوامی:
  HBUINT16	newState;	/* Byte offset from beginning of state table to the new state. */
  HBUINT16	flags;		/* Table specific. */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

شمار Class
{
  CLASS_انتہا_کا_متن = 0,
  CLASS_برآمد_کا_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_انتہا_کا_سطر = 3,
};

سانچہ <قسم_نام Types, قسم_نام Extra>
ساخت State_جدول
{
  قسمتعریف قسم_نام Types::HBUINT HBUINT;
  قسمتعریف قسم_نام Types::HBUSHORT HBUSHORT;
  قسمتعریف قسم_نام Types::ClassTypeNarrow ClassType;

  شمار State
  {
    STATE_آغاز_کا_متن = 0,
    STATE_آغاز_کا_سطر = 1,
  };

  سانچہ <قسم_نام سیٹ_t, قسم_نام جدول_t>
  خالی collect_initial_glyphs (سیٹ_t &glyphs, غیرعلامتی عدد_glyphs, ثابت جدول_t &جدول) ثابت
  {
    غیرعلامتی عدد_classes = nClasses;

    اگر (unlikely (عدد_classes > hb_ثنائی_ہندسہ_صفحہ_t::بٹ_2))
    {
      (یہ+class_جدول).collect_glyphs (glyphs, عدد_glyphs);
      واپس;
    }

    // Collect all classes going out from the start state.
    hb_ثنائی_ہندسہ_صفحہ_t filter;

    برائے (غیرعلامتی i = 0; i < عدد_classes; i++)
    {
      ثابت خودکار &اندراج_2 = get_اندراج (STATE_آغاز_کا_متن, i);
      اگر (new_state (اندراج_2.newState) == STATE_آغاز_کا_متن &&
	  !جدول.is_action_initiable (اندراج_2) && !جدول.is_actionable (اندراج_2))
	جاری;

      filter.جوڑو (i);
    }

    // And glyphs in those classes.

    اگر (filter (CLASS_DELETED_GLYPH))
      glyphs.جوڑو (DELETED_GLYPH);

    (یہ+class_جدول).collect_glyphs_filtered (glyphs, عدد_glyphs, filter);
  }

  صحیح new_state (غیرعلامتی صحیح newState) ثابت
  { واپس Types::extended ? newState : ((صحیح) newState - (صحیح) state_Array_جدول) / (صحیح) nClasses; }

  غیرعلامتی صحیح get_class (hb_codepoint_t glyph_id,
			  غیرعلامتی صحیح عدد_glyphs,
			  hb_aat_class_cache_t *cache = خالی_اشاریہ) ثابت
  {
    غیرعلامتی klass;
    اگر (cache && cache->get (glyph_id, &klass)) واپس klass;
    اگر (unlikely (glyph_id == DELETED_GLYPH)) واپس CLASS_DELETED_GLYPH;
    klass = (یہ+class_جدول).get_class (glyph_id, عدد_glyphs, CLASS_برآمد_کا_BOUNDS);
    اگر (cache) cache->سیٹ (glyph_id, klass);
    واپس klass;
  }

  ثابت اندراج_3<Extra> *get_entries () ثابت
  { واپس (یہ+اندراج_جدول).arrayZ; }

  ثابت اندراج_3<Extra> &get_اندراج (صحیح state, غیرعلامتی صحیح klass) ثابت
  {
    غیرعلامتی n_classes = nClasses;
    اگر (unlikely (klass >= n_classes))
      klass = CLASS_برآمد_کا_BOUNDS;

    ثابت HBUSHORT *حالتیں = (یہ+state_Array_جدول).arrayZ;
    ثابت اندراج_3<Extra> *entries = (یہ+اندراج_جدول).arrayZ;

    غیرعلامتی صحیح اندراج_2 = حالتیں[state * n_classes + klass];
    DEBUG_پیغام (APPLY, خالی_اشاریہ, "e%u", اندراج_2);

    واپس entries[اندراج_2];
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 غیرعلامتی صحیح *عدد_entries_برآمد = خالی_اشاریہ) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!(c->check_struct (یہ) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    class_جدول.sanitize (c, یہ)))) return_trace (جھوٹ);

    ثابت HBUSHORT *حالتیں = (یہ+state_Array_جدول).arrayZ;
    ثابت اندراج_3<Extra> *entries = (یہ+اندراج_جدول).arrayZ;

    غیرعلامتی صحیح عدد_classes = nClasses;
    اگر (unlikely (hb_unsigned_mul_overflows (عدد_classes, حالتیں[0].static_سائز)))
      return_trace (جھوٹ);
    غیرعلامتی صحیح صف_stride = عدد_classes * حالتیں[0].static_سائز;

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
    غیرعلامتی صحیح عدد_entries = 0;

    صحیح state_pos = 0;
    صحیح state_neg = 0;
    غیرعلامتی صحیح اندراج_2 = 0;
    جب (min_state < state_neg || state_pos <= max_state)
    {
      اگر (min_state < state_neg)
      {
	/* Negative states. */
	اگر (unlikely (hb_unsigned_mul_overflows (min_state, عدد_classes)))
	  return_trace (جھوٹ);
	اگر (unlikely (!c->check_range (&حالتیں[min_state * عدد_classes],
				       -min_state,
				       صف_stride)))
	  return_trace (جھوٹ);
	اگر ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (جھوٹ);
	{ /* Sweep new states. */
	  ثابت HBUSHORT *stop = &حالتیں[min_state * عدد_classes];
	  اگر (unlikely (stop > حالتیں))
	    return_trace (جھوٹ);
	  برائے (ثابت HBUSHORT *p = حالتیں; stop < p; p--)
	    عدد_entries = hb_max (عدد_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      اگر (state_pos <= max_state)
      {
	/* Positive states. */
	اگر (unlikely (!c->check_range (حالتیں,
				       max_state + 1,
				       صف_stride)))
	  return_trace (جھوٹ);
	اگر ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (جھوٹ);
	{ /* Sweep new states. */
	  اگر (unlikely (hb_unsigned_mul_overflows ((max_state + 1), عدد_classes)))
	    return_trace (جھوٹ);
	  ثابت HBUSHORT *stop = &حالتیں[(max_state + 1) * عدد_classes];
	  اگر (unlikely (stop < حالتیں))
	    return_trace (جھوٹ);
	  برائے (ثابت HBUSHORT *p = &حالتیں[state_pos * عدد_classes]; p < stop; p++)
	    عدد_entries = hb_max (عدد_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      اگر (unlikely (!c->check_array (entries, عدد_entries)))
	return_trace (جھوٹ);
      اگر ((c->max_ops -= عدد_entries - اندراج_2) <= 0)
	return_trace (جھوٹ);
      { /* Sweep new entries. */
	ثابت اندراج_3<Extra> *stop = &entries[عدد_entries];
	برائے (ثابت اندراج_3<Extra> *p = &entries[اندراج_2]; p < stop; p++)
	{
	  صحیح newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	اندراج_2 = عدد_entries;
      }
    }

    اگر (عدد_entries_برآمد)
      *عدد_entries_برآمد = عدد_entries;

    return_trace (سچ);
  }

  محفوظ:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffset_مقصد<ClassType, HBUINT>
		class_جدول;	/* Offset to the class table. */
  NNOffset_مقصد<Unsized_Array_کا<HBUSHORT>, HBUINT>
		state_Array_جدول;/* Offset to the state array. */
  NNOffset_مقصد<Unsized_Array_کا<اندراج_3<Extra>>, HBUINT>
		اندراج_جدول;	/* Offset to the entry array. */

  عوامی:
  DEFINE_سائز_STATIC (4 * حجم (HBUINT));
};

سانچہ <قسم_نام HBUCHAR>
ساخت Class_جدول
{
  غیرعلامتی صحیح get_class (hb_codepoint_t glyph_id, غیرعلامتی صحیح برآمد_کا_Range) ثابت
  {
    غیرعلامتی صحیح i = glyph_id - پہلا_Glyph;
    واپس i >= classArray.len ? برآمد_کا_Range : classArray.arrayZ[i];
  }
  غیرعلامتی صحیح get_class (hb_codepoint_t glyph_id,
			  غیرعلامتی صحیح عدد_glyphs HB_غیر_مستعمل,
			  غیرعلامتی صحیح برآمد_کا_Range) ثابت
  {
    واپس get_class (glyph_id, برآمد_کا_Range);
  }

  سانچہ <قسم_نام سیٹ_t>
  خالی collect_glyphs (سیٹ_t &glyphs, غیرعلامتی عدد_glyphs) ثابت
  {
    برائے (غیرعلامتی i = 0; i < classArray.len; i++)
      اگر (classArray.arrayZ[i] != CLASS_برآمد_کا_BOUNDS)
	glyphs.جوڑو (پہلا_Glyph + i);
  }
  سانچہ <قسم_نام سیٹ_t, قسم_نام filter_t>
  خالی collect_glyphs_filtered (سیٹ_t &glyphs, غیرعلامتی عدد_glyphs, ثابت filter_t &filter) ثابت
  {
    برائے (غیرعلامتی i = 0; i < classArray.len; i++)
      اگر (filter (classArray.arrayZ[i]))
	glyphs.جوڑو (پہلا_Glyph + i);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && classArray.sanitize (c));
  }
  محفوظ:
  HBGlyphID16		پہلا_Glyph;	/* First glyph index included in the trimmed array. */
  Array16_کا<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  عوامی:
  DEFINE_سائز_ARRAY (4, classArray);
};

ساخت SubtableGlyphCoverage
{
  منطقی sanitize (hb_sanitize_context_t *c, غیرعلامتی subtable_گنتی) ثابت
  {
    TRACE_SANITIZE (یہ);

    اگر (unlikely (!c->check_array (&subtableOffsets, subtable_گنتی)))
      return_trace (جھوٹ);

    غیرعلامتی بائٹیں = (c->get_عدد_glyphs () + CHAR_ثنائی_ہندسہ - 1) / CHAR_ثنائی_ہندسہ;
    برائے (غیرعلامتی i = 0; i < subtable_گنتی; i++)
    {
      uint32_t انحراف = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      اگر (انحراف == 0 || انحراف == 0xFFFFFFFF)
        جاری;
      اگر (unlikely (!subtableOffsets[i].sanitize (c, یہ, بائٹیں)))
        return_trace (جھوٹ);
    }

    return_trace (سچ);
  }
  محفوظ:
  Unsized_Array_کا<NNOffset32_مقصد<Unsized_Array_کا<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  عوامی:
  DEFINE_سائز_ARRAY (0, subtableOffsets);
};

ساخت ObsoleteTypes
{
  ساکن constexpr منطقی extended = جھوٹ;
  قسمتعریف HBUINT16 HBUINT;
  قسمتعریف HBUINT8 HBUSHORT;
  قسمتعریف Class_جدول<HBUINT8> ClassTypeNarrow;
  قسمتعریف Class_جدول<HBUINT16> ClassTypeWide;

  سانچہ <قسم_نام T>
  ساکن صحیح انحراف_مقصد_اشاریہ (int64_t انحراف,
			    ثابت خالی *بنیاد,
			    ثابت T *array)
  {
    int64_t array_انحراف = (ثابت حرف *) array - (ثابت حرف *) بنیاد;
    صحیح bad_اشاریہ = زیادہ_سے_زیادہ_عدد / T::static_سائز;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    اگر (unlikely (انحراف < array_انحراف))
      واپس bad_اشاریہ;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t اشاریہ_2 = (انحراف - array_انحراف) / T::static_سائز;
    اگر (unlikely (اشاریہ_2 > bad_اشاریہ))
      واپس bad_اشاریہ;
    واپس اشاریہ_2;
  }
  سانچہ <قسم_نام T>
  ساکن صحیح byte_انحراف_مقصد_اشاریہ (int64_t انحراف,
				ثابت خالی *بنیاد,
				ثابت T *array)
  {
    واپس انحراف_مقصد_اشاریہ (انحراف, بنیاد, array);
  }
  سانچہ <قسم_نام T>
  ساکن صحیح لفظ_انحراف_مقصد_اشاریہ (int64_t انحراف,
				ثابت خالی *بنیاد,
				ثابت T *array)
  {
    واپس انحراف_مقصد_اشاریہ (2 * انحراف, بنیاد, array);
  }
};
ساخت ExtendedTypes
{
  ساکن constexpr منطقی extended = سچ;
  قسمتعریف HBUINT32 HBUINT;
  قسمتعریف HBUINT16 HBUSHORT;
  قسمتعریف Lookup<HBUINT16> ClassTypeNarrow;
  قسمتعریف Lookup<HBUINT16> ClassTypeWide;

  سانچہ <قسم_نام T>
  ساکن غیرعلامتی صحیح انحراف_مقصد_اشاریہ (غیرعلامتی صحیح انحراف,
				     ثابت خالی *بنیاد HB_غیر_مستعمل,
				     ثابت T *array HB_غیر_مستعمل)
  {
    واپس انحراف;
  }
  سانچہ <قسم_نام T>
  ساکن غیرعلامتی صحیح byte_انحراف_مقصد_اشاریہ (غیرعلامتی صحیح انحراف,
					 ثابت خالی *بنیاد HB_غیر_مستعمل,
					 ثابت T *array HB_غیر_مستعمل)
  {
    واپس انحراف / 2;
  }
  سانچہ <قسم_نام T>
  ساکن غیرعلامتی صحیح لفظ_انحراف_مقصد_اشاریہ (غیرعلامتی صحیح انحراف,
					 ثابت خالی *بنیاد HB_غیر_مستعمل,
					 ثابت T *array HB_غیر_مستعمل)
  {
    واپس انحراف;
  }
};

سانچہ <قسم_نام Types, قسم_نام اندراج_ڈیٹا, قسم_نام Flags>
ساخت State_جدول_Driver
{
  استعمال State_جدول_T = State_جدول<Types, اندراج_ڈیٹا>;
  استعمال اندراج_T = اندراج_3<اندراج_ڈیٹا>;

  State_جدول_Driver (ثابت State_جدول_T &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      عدد_glyphs (face_->get_عدد_glyphs ()) {}

  سانچہ <قسم_نام context_t>
  خالی drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_بفر_t *بفر = ac->بفر;

    اگر (!c->in_place)
      بفر->clear_output ();

    صحیح state = State_جدول_T::STATE_آغاز_کا_متن;
    // If there's only one range, we already checked the flag.
    خودکار *آخری_range = ac->range_flags && (ac->range_flags->لمبائی > 1) ? &(*ac->range_flags)[0] : خالی_اشاریہ;
    ثابت منطقی آغاز_state_محفوظ_مقصد_break_eot =
      !c->جدول->is_actionable (machine.get_اندراج (State_جدول_T::STATE_آغاز_کا_متن, CLASS_انتہا_کا_متن));
    برائے (بفر->idx = 0; بفر->successful;)
    {
      غیرعلامتی صحیح klass = likely (بفر->idx < بفر->len) ?
			   machine.get_class (بفر->cur().codepoint, عدد_glyphs, ac->machine_class_cache) :
			   (غیرعلامتی) CLASS_انتہا_کا_متن;
    resume:
      DEBUG_پیغام (APPLY, خالی_اشاریہ, "c%u at %u", klass, بفر->idx);
      ثابت اندراج_T &اندراج_2 = machine.get_اندراج (state, klass);
      ثابت صحیح اگلا_state = machine.new_state (اندراج_2.newState);

      منطقی is_not_epsilon_transition = !(اندراج_2.flags & Flags::DontAdvance);
      منطقی is_not_actionable = !c->جدول->is_actionable (اندراج_2);

      اگر (unlikely (آخری_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	خودکار *range = آخری_range;
	اگر (بفر->idx < بفر->len)
	{
	  غیرعلامتی cluster = بفر->cur().cluster;
	  جب (cluster < range->cluster_پہلا)
	    range--;
	  جب (cluster > range->cluster_آخری)
	    range++;


	  آخری_range = range;
	}
	اگر (!(range->flags & ac->subtable_flags))
	{
	  اگر (بفر->idx == بفر->len)
	    توڑو;

	  state = State_جدول_T::STATE_آغاز_کا_متن;
	  (خالی) بفر->اگلا_glyph ();
	  جاری;
	}
      }
      ورنہ
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	منطقی is_null_transition = state == State_جدول_T::STATE_آغاز_کا_متن &&
				  اگلا_state == State_جدول_T::STATE_آغاز_کا_متن &&
				  آغاز_state_محفوظ_مقصد_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	اگر (is_null_transition)
	{
	  غیرعلامتی old_klass = klass;
	  کرو
	  {
	    c->transition (بفر, یہ, اندراج_2);

	    اگر (بفر->idx == بفر->len || !بفر->successful)
	      توڑو;

	    (خالی) بفر->اگلا_glyph ();

	    klass = likely (بفر->idx < بفر->len) ?
		     machine.get_class (بفر->cur().codepoint, عدد_glyphs, ac->machine_class_cache) :
		     (غیرعلامتی) CLASS_انتہا_کا_متن;
	  } جب (klass == old_klass);

	  اگر (بفر->idx == بفر->len || !بفر->successful)
	    توڑو;

	  جاؤ resume;
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
      منطقی is_محفوظ_مقصد_break =
      (
          /* 1. */
          !c->جدول->is_actionable (اندراج_2) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == State_جدول_T::STATE_آغاز_کا_متن
              || ((اندراج_2.flags & Flags::DontAdvance) && اگلا_state == State_جدول_T::STATE_آغاز_کا_متن)
              || (
		    /* 2c. */
		    wouldbe_اندراج = &machine.get_اندراج(State_جدول_T::STATE_آغاز_کا_متن, klass)
		    ,
		    /* 2c'. */
		    !c->جدول->is_actionable (*wouldbe_اندراج) &&
		    /* 2c". */
		    (
		      اگلا_state == machine.new_state(wouldbe_اندراج->newState) &&
		      (اندراج_2.flags & Flags::DontAdvance) == (wouldbe_اندراج->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->جدول->is_actionable (machine.get_اندراج (state, CLASS_انتہا_کا_متن))
      );

      اگر (!is_محفوظ_مقصد_break && بفر->backtrack_len () && بفر->idx < بفر->len)
	بفر->unsafe_مقصد_break_from_outbuffer (بفر->backtrack_len () - 1, بفر->idx + 1);

      c->transition (بفر, یہ, اندراج_2);

      state = اگلا_state;
      DEBUG_پیغام (APPLY, خالی_اشاریہ, "s%d", state);

      اگر (بفر->idx == بفر->len)
	توڑو;

      اگر (is_not_epsilon_transition || بفر->max_ops-- <= 0)
	(خالی) بفر->اگلا_glyph ();
    }

    اگر (!c->in_place)
      بفر->ہمآہنگ ();
  }

  عوامی:
  ثابت State_جدول_T &machine;
  غیرعلامتی صحیح عدد_glyphs;
};


} /* namespace AAT */


#اگر_ختم /* HB_AAT_LAYOUT_COMMON_HH */
