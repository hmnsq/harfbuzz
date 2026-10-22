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

#اگر_تعریف_نشده HB_AAT_LAYOUT_COMMON_HH
#تعریف_کن HB_AAT_LAYOUT_COMMON_HH

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

استفاده نطاق OT;

بنية ankr;

استفاده hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
#تعریف_کن HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#وإلا
#تعریف_کن HB_AAT_TRANSITION_INLINE
#پایان_اگر

بنية hb_aat_scratch_t
{
  hb_aat_scratch_t () = افتراضي;
  hb_aat_scratch_t (ثابت hb_aat_scratch_t &) = حذف;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    میانگیر_glyph_مجموعه.مجموعه_relaxed (o.میانگیر_glyph_مجموعه.get_relaxed ());
    o.میانگیر_glyph_مجموعه.مجموعه_relaxed (اشاره_تهی);
  }
  hb_aat_scratch_t & عملگر = (hb_aat_scratch_t &&o)
  {
    میانگیر_glyph_مجموعه.مجموعه_relaxed (o.میانگیر_glyph_مجموعه.get_relaxed ());
    o.میانگیر_glyph_مجموعه.مجموعه_relaxed (اشاره_تهی);
    أرجع *هذا;
  }
  ~hb_aat_scratch_t ()
  {
    خودکار *s = میانگیر_glyph_مجموعه.get_relaxed ();
    إذا (unlikely (!s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  hb_بیت_مجموعه_t *create_میانگیر_glyph_مجموعه () ثابت
  {
    hb_بیت_مجموعه_t *s = میانگیر_glyph_مجموعه.get_acquire ();
    إذا (s && میانگیر_glyph_مجموعه.cmpexch (s, اشاره_تهی))
    {
      s->clear ();
      أرجع s;
    }

    s = (hb_بیت_مجموعه_t *) hb_calloc (1, حجم (hb_بیت_مجموعه_t));
    إذا (unlikely (!s))
      أرجع اشاره_تهی;
    s->init ();

    أرجع s;
  }
  فراغ destroy_میانگیر_glyph_مجموعه (hb_بیت_مجموعه_t *s) ثابت
  {
    إذا (unlikely (!s))
      أرجع;
    إذا (میانگیر_glyph_مجموعه.cmpexch (اشاره_تهی, s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  تغییرپذیر hb_atomic_t<hb_بیت_مجموعه_t *> میانگیر_glyph_مجموعه;
};

تعداد { DELETED_GLYPH = 0xFFFF };

#تعریف_کن HB_میانگیر_SCRATCH_FLAG_AAT_HAS_DELETED HB_میانگیر_SCRATCH_FLAG_SHAPER0

بنية hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  قالب <نام_نوع T, نام_نوع ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { أرجع obj.apply (هذا, std::forward<Ts> (ds)...); }
  ساكن return_t default_return_مقدار () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  ثابت hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_میانگیر_t *میانگیر;
  hb_sanitize_context_t sanitizer;
  ثابت ankr *ankr_جدول;
  ثابت OT::GDEF &gdef;
  منطقي has_glyph_classes;
  ثابت hb_sorted_وکتور_t<hb_aat_map_t::range_flags_t> *range_flags = اشاره_تهی;
  hb_ماسک_t subtable_flags = 0;
  منطقي میانگیر_is_reversed = خطأ;
  // Caches
  منطقي using_میانگیر_glyph_مجموعه = خطأ;
  hb_بیت_مجموعه_t *میانگیر_glyph_مجموعه = اشاره_تهی;
  ثابت hb_بیت_مجموعه_t *اولی_مجموعه = اشاره_تهی;
  ثابت hb_بیت_مجموعه_t *دومی_مجموعه = اشاره_تهی;
  hb_aat_class_cache_t *machine_class_cache = اشاره_تهی;

  /* Unused. For debug tracing only. */
  بیعلامت صحيح lookup_شاخص;

  HB_داخلی hb_aat_apply_context_t (ثابت hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_میانگیر_t *میانگیر_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_داخلی ~hb_aat_apply_context_t ();

  HB_داخلی فراغ مجموعه_ankr_جدول (ثابت AAT::ankr *ankr_جدول_2);

  فراغ مجموعه_lookup_شاخص (بیعلامت صحيح i) { lookup_شاخص = i; }

  فراغ معکوس_میانگیر ()
  {
    میانگیر->reverse ();
    میانگیر_is_reversed = !میانگیر_is_reversed;
  }

  فراغ راهاندازی_میانگیر_glyph_مجموعه ()
  {
    using_میانگیر_glyph_مجموعه = میانگیر->len >= 4 && میانگیر_glyph_مجموعه;

    إذا (likely (using_میانگیر_glyph_مجموعه))
      میانگیر->collect_codepoints (*میانگیر_glyph_مجموعه);
  }
  منطقي میانگیر_intersects_machine () ثابت
  {
    إذا (likely (using_میانگیر_glyph_مجموعه))
      أرجع میانگیر_glyph_مجموعه->intersects (*اولی_مجموعه);

    // Faster for shorter buffers.
    لكل (بیعلامت i = 0; i < میانگیر->len; i++)
      إذا (اولی_مجموعه->has (میانگیر->info[i].codepoint))
	أرجع درست;
    أرجع خطأ;
  }

  قالب <نام_نوع T>
  HB_NODISCARD منطقي output_glyphs (بیعلامت صحيح شمار,
				   ثابت T *glyphs)
  {
    إذا (likely (using_میانگیر_glyph_مجموعه))
      میانگیر_glyph_مجموعه->جمع_کن_array (glyphs, شمار);
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      منطقي at_انتها_از_متن = میانگیر->idx == میانگیر->len;
      إذا (at_انتها_از_متن)
	إذا (unlikely (!میانگیر->output_glyph (glyphs[i]))) أرجع خطأ;

      hb_glyph_info_t &info = at_انتها_از_متن ? میانگیر->prev () : میانگیر->cur ();
      إذا (glyphs[i] == DELETED_GLYPH)
      {
        میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_AAT_HAS_DELETED;
	hb_glyph_info_مجموعه_aat_deleted (&info);
      }
      وإلا
      {
#اگر_تعریف_نشده HB_NO_OT_LAYOUT
	إذا (has_glyph_classes)
	  hb_glyph_info_مجموعه_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#پایان_اگر
      }
      إذا (!at_انتها_از_متن)
	إذا (unlikely (!میانگیر->output_glyph (glyphs[i]))) أرجع خطأ;
    }
    أرجع درست;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph)
  {
    إذا (glyph == DELETED_GLYPH)
    {
      میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_مجموعه_aat_deleted (&میانگیر->cur());
    }

    إذا (likely (using_میانگیر_glyph_مجموعه))
      میانگیر_glyph_مجموعه->جمع_کن (glyph);
#اگر_تعریف_نشده HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      hb_glyph_info_مجموعه_glyph_props (&میانگیر->cur(),
				      gdef.get_glyph_props (glyph));
#پایان_اگر
    أرجع میانگیر->replace_glyph (glyph);
  }

  HB_NODISCARD منطقي delete_glyph ()
  {
    میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_AAT_HAS_DELETED;
    hb_glyph_info_مجموعه_aat_deleted (&میانگیر->cur());
    إذا (likely (using_میانگیر_glyph_مجموعه))
      میانگیر_glyph_مجموعه->جمع_کن (DELETED_GLYPH);
    أرجع میانگیر->replace_glyph (DELETED_GLYPH);
  }

  فراغ replace_glyph_inplace (بیعلامت i, hb_codepoint_t glyph)
  {
    میانگیر->info[i].codepoint = glyph;
    إذا (glyph == DELETED_GLYPH)
    {
      میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_مجموعه_aat_deleted (&میانگیر->info[i]);
    }
    إذا (likely (using_میانگیر_glyph_مجموعه))
      میانگیر_glyph_مجموعه->جمع_کن (glyph);
#اگر_تعریف_نشده HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      hb_glyph_info_مجموعه_glyph_props (&میانگیر->info[i],
				      gdef.get_glyph_props (glyph));
#پایان_اگر
  }
};


/*
 * Lookup Table
 */

قالب <نام_نوع T> بنية Lookup;

قالب <نام_نوع T>
بنية LookupFormat0
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id, بیعلامت صحيح رقم_glyphs) ثابت
  {
    إذا (unlikely (glyph_id >= رقم_glyphs)) أرجع اشاره_تهی;
    أرجع &arrayZ[glyph_id];
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs, بیعلامت رقم_glyphs) ثابت
  {
    glyphs.جمع_کن_range (0, رقم_glyphs - 1);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, بیعلامت رقم_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (بیعلامت i = 0; i < رقم_glyphs; i++)
      إذا (filter (arrayZ[i]))
	glyphs.جمع_کن (i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_رقم_glyphs ()));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_رقم_glyphs (), پایه));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 0 */
  Unsized_Array_از<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  عام:
  DEFINE_اندازه_UNBOUNDED (2);
};


قالب <نام_نوع T>
بنية LookupSegmentSingle
{
  ساكن constexpr بیعلامت Termination_کلمه_شمار = 2u;

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < اولی ? -1 : g <= آخرین ? 0 : +1 ; }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs) ثابت
  {
    إذا (اولی == DELETED_GLYPH) أرجع;
    glyphs.جمع_کن_range (اولی, آخرین);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (اولی == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
    glyphs.جمع_کن_range (اولی, آخرین);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, پایه));
  }

  HBGlyphID16	آخرین;		/* Last GlyphID in this segment */
  HBGlyphID16	اولی;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_اندازه_STATIC (4 + T::static_اندازه);
};

قالب <نام_نوع T>
بنية LookupFormat2
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    أرجع v ? &v->value : اشاره_تهی;
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs) ثابت
  {
    بیعلامت شمار = segments.get_درازی ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      segments[i].collect_glyphs (glyphs);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بیعلامت شمار = segments.get_درازی ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, پایه));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_جستجو_Array_از<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_اندازه_ARRAY (8, segments);
};

قالب <نام_نوع T>
بنية LookupSegmentArray
{
  ساكن constexpr بیعلامت Termination_کلمه_شمار = 2u;

  ثابت T* get_مقدار (hb_codepoint_t glyph_id, ثابت فراغ *پایه) ثابت
  {
    أرجع اولی <= glyph_id && glyph_id <= آخرین ? &(پایه+مقادیر_Z)[glyph_id - اولی] : اشاره_تهی;
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs) ثابت
  {
    إذا (اولی == DELETED_GLYPH) أرجع;
    glyphs.جمع_کن_range (اولی, آخرین);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, ثابت فراغ *پایه, ثابت filter_t &filter) ثابت
  {
    إذا (اولی == DELETED_GLYPH) أرجع;
    ثابت خودکار &values = پایه+مقادیر_Z;
    لكل (hb_codepoint_t i = اولی; i <= آخرین; i++)
      إذا (filter (values[i - اولی]))
	glyphs.جمع_کن (i);
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < اولی ? -1 : g <= آخرین ? 0 : +1; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  اولی <= آخرین &&
		  مقادیر_Z.sanitize (c, پایه, آخرین - اولی + 1));
  }
  قالب <نام_نوع ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  اولی <= آخرین &&
		  مقادیر_Z.sanitize (c, پایه, آخرین - اولی + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	آخرین;		/* Last GlyphID in this segment */
  HBGlyphID16	اولی;		/* First GlyphID in this segment */
  NNOffset16_مقصد<Unsized_Array_از<T>>
		مقادیر_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  عام:
  DEFINE_اندازه_STATIC (6);
};

قالب <نام_نوع T>
بنية LookupFormat4
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    أرجع v ? v->get_مقدار (glyph_id, هذا) : اشاره_تهی;
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs) ثابت
  {
    بیعلامت شمار = segments.get_درازی ();
    لكل (بیعلامت i = 0; i < شمار; i++)
      segments[i].collect_glyphs (glyphs);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بیعلامت شمار = segments.get_درازی ();
    لكل (بیعلامت i = 0; i < شمار; i++)
      segments[i].collect_glyphs_filtered (glyphs, هذا, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا, پایه));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_جستجو_Array_از<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_اندازه_ARRAY (8, segments);
};

قالب <نام_نوع T>
بنية LookupSingle
{
  ساكن constexpr بیعلامت Termination_کلمه_شمار = 1u;

  صحيح cmp (hb_codepoint_t g) ثابت { أرجع glyph.cmp (g); }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    glyphs.جمع_کن (glyph);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
    glyphs.جمع_کن (glyph);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, پایه));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_اندازه_STATIC (2 + T::static_اندازه);
};

قالب <نام_نوع T>
بنية LookupFormat6
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSingle<T> *v = entries.bsearch (glyph_id);
    أرجع v ? &v->value : اشاره_تهی;
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs) ثابت
  {
    بیعلامت شمار = entries.get_درازی ();
    لكل (بیعلامت i = 0; i < شمار; i++)
      entries[i].collect_glyphs (glyphs);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بیعلامت شمار = entries.get_درازی ();
    لكل (بیعلامت i = 0; i < شمار; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c, پایه));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_جستجو_Array_از<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  عام:
  DEFINE_اندازه_ARRAY (8, entries);
};

قالب <نام_نوع T>
بنية LookupFormat8
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت T* get_مقدار (hb_codepoint_t glyph_id) ثابت
  {
    أرجع اولی_Glyph <= glyph_id && glyph_id - اولی_Glyph < glyph_شمار_2 ?
	   &مقدار_Array_Z[glyph_id - اولی_Glyph] : اشاره_تهی;
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_شمار_2)) أرجع;
    إذا (اولی_Glyph == DELETED_GLYPH) أرجع;
    glyphs.جمع_کن_range (اولی_Glyph, اولی_Glyph + glyph_شمار_2 - 1);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_شمار_2)) أرجع;
    إذا (اولی_Glyph == DELETED_GLYPH) أرجع;
    ثابت T *p = مقدار_Array_Z.arrayZ;
    لكل (بیعلامت i = 0; i < glyph_شمار_2; i++)
      إذا (filter (p[i]))
	glyphs.جمع_کن (اولی_Glyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && مقدار_Array_Z.sanitize (c, glyph_شمار_2));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && مقدار_Array_Z.sanitize (c, glyph_شمار_2, پایه));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	اولی_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_شمار_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_از<T>
		مقدار_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_اندازه_ARRAY (6, مقدار_Array_Z);
};

قالب <نام_نوع T>
بنية LookupFormat10
{
  دوست بنية Lookup<T>;

  خاص:
  ثابت نام_نوع T::type get_مقدار_or_null (hb_codepoint_t glyph_id) ثابت
  {
    إذا (!(اولی_Glyph <= glyph_id && glyph_id - اولی_Glyph < glyph_شمار_2))
      أرجع Null (T);

    ثابت HBUINT8 *p = &مقدار_Array_Z[(glyph_id - اولی_Glyph) * مقدار_اندازه];

    بیعلامت صحيح v = 0;
    بیعلامت صحيح شمار = مقدار_اندازه;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      v = (v << 8) | *p++;

    أرجع v;
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_شمار_2)) أرجع;
    إذا (اولی_Glyph == DELETED_GLYPH) أرجع;
    glyphs.جمع_کن_range (اولی_Glyph, اولی_Glyph + glyph_شمار_2 - 1);
  }

  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_شمار_2)) أرجع;
    إذا (اولی_Glyph == DELETED_GLYPH) أرجع;
    ثابت HBUINT8 *p = مقدار_Array_Z.arrayZ;
    لكل (بیعلامت i = 0; i < glyph_شمار_2; i++)
    {
      بیعلامت صحيح v = 0;
      بیعلامت صحيح شمار = مقدار_اندازه;
      لكل (بیعلامت صحيح j = 0; j < شمار; j++)
	v = (v << 8) | *p++;
      إذا (filter (v))
	glyphs.جمع_کن (اولی_Glyph + i);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  مقدار_اندازه <= 4 &&
		  مقدار_Array_Z.sanitize (c, glyph_شمار_2 * مقدار_اندازه));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	مقدار_اندازه;	/* Byte size of each value. */
  HBGlyphID16	اولی_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_شمار_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_از<HBUINT8>
		مقدار_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_اندازه_ARRAY (8, مقدار_Array_Z);
};

قالب <نام_نوع T>
بنية Lookup
{
  ثابت T* get_مقدار (hb_codepoint_t glyph_id, بیعلامت صحيح رقم_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_مقدار (glyph_id, رقم_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_مقدار (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_مقدار (glyph_id);
    حالة 6: hb_barrier (); أرجع u.format6.get_مقدار (glyph_id);
    حالة 8: hb_barrier (); أرجع u.format8.get_مقدار (glyph_id);
    افتراضي:أرجع اشاره_تهی;
    }
  }

  ثابت نام_نوع T::type get_مقدار_or_null (hb_codepoint_t glyph_id, بیعلامت صحيح رقم_glyphs) ثابت
  {
    بدل (u.format.v) {
      /* Format 10 cannot return a pointer. */
      حالة 10: hb_barrier (); أرجع u.format10.get_مقدار_or_null (glyph_id);
      افتراضي:
      ثابت T *v = get_مقدار (glyph_id, رقم_glyphs);
      أرجع v ? *v : Null (T);
    }
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs, بیعلامت صحيح رقم_glyphs) ثابت
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
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, بیعلامت رقم_glyphs, ثابت filter_t &filter) ثابت
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

  نام_نوع T::type get_class (hb_codepoint_t glyph_id,
			      بیعلامت صحيح رقم_glyphs,
			      بیعلامت صحيح بیرون_از_Range) ثابت
  {
    ثابت T *v = get_مقدار (glyph_id, رقم_glyphs);
    أرجع v ? *v : بیرون_از_Range;
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
    افتراضي:return_trace (درست);
    }
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, پایه));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, پایه));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, پایه));
    حالة 6: hb_barrier (); return_trace (u.format6.sanitize (c, پایه));
    حالة 8: hb_barrier (); return_trace (u.format8.sanitize (c, پایه));
    حالة 10: return_trace (خطأ); /* We don't support format10 here currently. */
    افتراضي:return_trace (درست);
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
  DEFINE_اندازه_UNION (2, format.v);
};
DECLARE_NULL_NAMESPACE_بایتها_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

قالب <نام_نوع T>
بنية مدخل_2
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
    static_assert (T::static_اندازه, "");

    return_trace (c->check_struct (هذا));
  }

  عام:
  HBUINT16	newState;	/* Byte offset from beginning of state table
				 * to the new state. Really?!?! Or just state
				 * number?  The latter in morx for sure. */
  HBUINT16	flags;		/* Table specific. */
  T		داده;		/* Optional offsets to per-glyph tables. */
  عام:
  DEFINE_اندازه_STATIC (4 + T::static_اندازه);
};

قالب <>
بنية مدخل_2<فراغ>
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
  DEFINE_اندازه_STATIC (4);
};

تعداد Class
{
  CLASS_انتها_از_متن = 0,
  CLASS_بیرون_از_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_انتها_از_سطر = 3,
};

قالب <نام_نوع Types, نام_نوع Extra>
بنية State_جدول
{
  تعريفنوع نام_نوع Types::HBUINT HBUINT;
  تعريفنوع نام_نوع Types::HBUSHORT HBUSHORT;
  تعريفنوع نام_نوع Types::ClassTypeNarrow ClassType;

  تعداد State
  {
    STATE_شروع_از_متن = 0,
    STATE_شروع_از_سطر = 1,
  };

  قالب <نام_نوع مجموعه_t, نام_نوع جدول_t>
  فراغ collect_initial_glyphs (مجموعه_t &glyphs, بیعلامت رقم_glyphs, ثابت جدول_t &جدول) ثابت
  {
    بیعلامت رقم_classes = nClasses;

    إذا (unlikely (رقم_classes > hb_بیت_صفحه_t::بیتها_2))
    {
      (هذا+class_جدول).collect_glyphs (glyphs, رقم_glyphs);
      أرجع;
    }

    // Collect all classes going out from the start state.
    hb_بیت_صفحه_t filter;

    لكل (بیعلامت i = 0; i < رقم_classes; i++)
    {
      ثابت خودکار &مدخل = get_مدخل (STATE_شروع_از_متن, i);
      إذا (new_state (مدخل.newState) == STATE_شروع_از_متن &&
	  !جدول.is_action_initiable (مدخل) && !جدول.is_actionable (مدخل))
	استمر;

      filter.جمع_کن (i);
    }

    // And glyphs in those classes.

    إذا (filter (CLASS_DELETED_GLYPH))
      glyphs.جمع_کن (DELETED_GLYPH);

    (هذا+class_جدول).collect_glyphs_filtered (glyphs, رقم_glyphs, filter);
  }

  صحيح new_state (بیعلامت صحيح newState) ثابت
  { أرجع Types::extended ? newState : ((صحيح) newState - (صحيح) state_Array_جدول) / (صحيح) nClasses; }

  بیعلامت صحيح get_class (hb_codepoint_t glyph_id,
			  بیعلامت صحيح رقم_glyphs,
			  hb_aat_class_cache_t *cache = اشاره_تهی) ثابت
  {
    بیعلامت klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    إذا (unlikely (glyph_id == DELETED_GLYPH)) أرجع CLASS_DELETED_GLYPH;
    klass = (هذا+class_جدول).get_class (glyph_id, رقم_glyphs, CLASS_بیرون_از_BOUNDS);
    إذا (cache) cache->مجموعه (glyph_id, klass);
    أرجع klass;
  }

  ثابت مدخل_2<Extra> *get_entries () ثابت
  { أرجع (هذا+مدخل_جدول).arrayZ; }

  ثابت مدخل_2<Extra> &get_مدخل (صحيح state, بیعلامت صحيح klass) ثابت
  {
    بیعلامت n_classes = nClasses;
    إذا (unlikely (klass >= n_classes))
      klass = CLASS_بیرون_از_BOUNDS;

    ثابت HBUSHORT *حالتها = (هذا+state_Array_جدول).arrayZ;
    ثابت مدخل_2<Extra> *entries = (هذا+مدخل_جدول).arrayZ;

    بیعلامت صحيح مدخل = حالتها[state * n_classes + klass];
    DEBUG_پیام (APPLY, اشاره_تهی, "e%u", مدخل);

    أرجع entries[مدخل];
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 بیعلامت صحيح *رقم_entries_بیرون = اشاره_تهی) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    class_جدول.sanitize (c, هذا)))) return_trace (خطأ);

    ثابت HBUSHORT *حالتها = (هذا+state_Array_جدول).arrayZ;
    ثابت مدخل_2<Extra> *entries = (هذا+مدخل_جدول).arrayZ;

    بیعلامت صحيح رقم_classes = nClasses;
    إذا (unlikely (hb_unsigned_mul_overflows (رقم_classes, حالتها[0].static_اندازه)))
      return_trace (خطأ);
    بیعلامت صحيح ردیف_stride = رقم_classes * حالتها[0].static_اندازه;

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
    بیعلامت صحيح رقم_entries = 0;

    صحيح state_pos = 0;
    صحيح state_neg = 0;
    بیعلامت صحيح مدخل = 0;
    طالما (min_state < state_neg || state_pos <= max_state)
    {
      إذا (min_state < state_neg)
      {
	/* Negative states. */
	إذا (unlikely (hb_unsigned_mul_overflows (min_state, رقم_classes)))
	  return_trace (خطأ);
	إذا (unlikely (!c->check_range (&حالتها[min_state * رقم_classes],
				       -min_state,
				       ردیف_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  ثابت HBUSHORT *stop = &حالتها[min_state * رقم_classes];
	  إذا (unlikely (stop > حالتها))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = حالتها; stop < p; p--)
	    رقم_entries = hb_max (رقم_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      إذا (state_pos <= max_state)
      {
	/* Positive states. */
	إذا (unlikely (!c->check_range (حالتها,
				       max_state + 1,
				       ردیف_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  إذا (unlikely (hb_unsigned_mul_overflows ((max_state + 1), رقم_classes)))
	    return_trace (خطأ);
	  ثابت HBUSHORT *stop = &حالتها[(max_state + 1) * رقم_classes];
	  إذا (unlikely (stop < حالتها))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = &حالتها[state_pos * رقم_classes]; p < stop; p++)
	    رقم_entries = hb_max (رقم_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      إذا (unlikely (!c->check_array (entries, رقم_entries)))
	return_trace (خطأ);
      إذا ((c->max_ops -= رقم_entries - مدخل) <= 0)
	return_trace (خطأ);
      { /* Sweep new entries. */
	ثابت مدخل_2<Extra> *stop = &entries[رقم_entries];
	لكل (ثابت مدخل_2<Extra> *p = &entries[مدخل]; p < stop; p++)
	{
	  صحيح newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	مدخل = رقم_entries;
      }
    }

    إذا (رقم_entries_بیرون)
      *رقم_entries_بیرون = رقم_entries;

    return_trace (درست);
  }

  محمي:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffset_مقصد<ClassType, HBUINT>
		class_جدول;	/* Offset to the class table. */
  NNOffset_مقصد<Unsized_Array_از<HBUSHORT>, HBUINT>
		state_Array_جدول;/* Offset to the state array. */
  NNOffset_مقصد<Unsized_Array_از<مدخل_2<Extra>>, HBUINT>
		مدخل_جدول;	/* Offset to the entry array. */

  عام:
  DEFINE_اندازه_STATIC (4 * حجم (HBUINT));
};

قالب <نام_نوع HBUCHAR>
بنية Class_جدول
{
  بیعلامت صحيح get_class (hb_codepoint_t glyph_id, بیعلامت صحيح بیرون_از_Range) ثابت
  {
    بیعلامت صحيح i = glyph_id - اولی_Glyph;
    أرجع i >= classArray.len ? بیرون_از_Range : classArray.arrayZ[i];
  }
  بیعلامت صحيح get_class (hb_codepoint_t glyph_id,
			  بیعلامت صحيح رقم_glyphs HB_استفادهنشده,
			  بیعلامت صحيح بیرون_از_Range) ثابت
  {
    أرجع get_class (glyph_id, بیرون_از_Range);
  }

  قالب <نام_نوع مجموعه_t>
  فراغ collect_glyphs (مجموعه_t &glyphs, بیعلامت رقم_glyphs) ثابت
  {
    لكل (بیعلامت i = 0; i < classArray.len; i++)
      إذا (classArray.arrayZ[i] != CLASS_بیرون_از_BOUNDS)
	glyphs.جمع_کن (اولی_Glyph + i);
  }
  قالب <نام_نوع مجموعه_t, نام_نوع filter_t>
  فراغ collect_glyphs_filtered (مجموعه_t &glyphs, بیعلامت رقم_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (بیعلامت i = 0; i < classArray.len; i++)
      إذا (filter (classArray.arrayZ[i]))
	glyphs.جمع_کن (اولی_Glyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && classArray.sanitize (c));
  }
  محمي:
  HBGlyphID16		اولی_Glyph;	/* First glyph index included in the trimmed array. */
  Array16_از<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  عام:
  DEFINE_اندازه_ARRAY (4, classArray);
};

بنية SubtableGlyphCoverage
{
  منطقي sanitize (hb_sanitize_context_t *c, بیعلامت subtable_شمار) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_array (&subtableOffsets, subtable_شمار)))
      return_trace (خطأ);

    بیعلامت بایتها = (c->get_رقم_glyphs () + CHAR_بیت - 1) / CHAR_بیت;
    لكل (بیعلامت i = 0; i < subtable_شمار; i++)
    {
      uint32_t جابه_جایی = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      إذا (جابه_جایی == 0 || جابه_جایی == 0xFFFFFFFF)
        استمر;
      إذا (unlikely (!subtableOffsets[i].sanitize (c, هذا, بایتها)))
        return_trace (خطأ);
    }

    return_trace (درست);
  }
  محمي:
  Unsized_Array_از<NNOffset32_مقصد<Unsized_Array_از<HBUINT8>>> subtableOffsets;
					    /* Array of offsets from the beginning of the
					     * subtable glyph coverage table to the glyph
					     * coverage bitfield for a given subtable; there
					     * is one offset for each subtable in the chain */
  /* UnsizedArrayOf<HBUINT8> coverageBitfields; *//* The individual coverage bitfields. */
  عام:
  DEFINE_اندازه_ARRAY (0, subtableOffsets);
};

بنية ObsoleteTypes
{
  ساكن constexpr منطقي extended = خطأ;
  تعريفنوع HBUINT16 HBUINT;
  تعريفنوع HBUINT8 HBUSHORT;
  تعريفنوع Class_جدول<HBUINT8> ClassTypeNarrow;
  تعريفنوع Class_جدول<HBUINT16> ClassTypeWide;

  قالب <نام_نوع T>
  ساكن صحيح جابه_جایی_مقصد_شاخص (int64_t جابه_جایی,
			    ثابت فراغ *پایه,
			    ثابت T *array)
  {
    int64_t array_جابه_جایی = (ثابت حرف *) array - (ثابت حرف *) پایه;
    صحيح bad_شاخص = بیشترین_صحیح / T::static_اندازه;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    إذا (unlikely (جابه_جایی < array_جابه_جایی))
      أرجع bad_شاخص;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t شاخص_2 = (جابه_جایی - array_جابه_جایی) / T::static_اندازه;
    إذا (unlikely (شاخص_2 > bad_شاخص))
      أرجع bad_شاخص;
    أرجع شاخص_2;
  }
  قالب <نام_نوع T>
  ساكن صحيح byte_جابه_جایی_مقصد_شاخص (int64_t جابه_جایی,
				ثابت فراغ *پایه,
				ثابت T *array)
  {
    أرجع جابه_جایی_مقصد_شاخص (جابه_جایی, پایه, array);
  }
  قالب <نام_نوع T>
  ساكن صحيح کلمه_جابه_جایی_مقصد_شاخص (int64_t جابه_جایی,
				ثابت فراغ *پایه,
				ثابت T *array)
  {
    أرجع جابه_جایی_مقصد_شاخص (2 * جابه_جایی, پایه, array);
  }
};
بنية ExtendedTypes
{
  ساكن constexpr منطقي extended = درست;
  تعريفنوع HBUINT32 HBUINT;
  تعريفنوع HBUINT16 HBUSHORT;
  تعريفنوع Lookup<HBUINT16> ClassTypeNarrow;
  تعريفنوع Lookup<HBUINT16> ClassTypeWide;

  قالب <نام_نوع T>
  ساكن بیعلامت صحيح جابه_جایی_مقصد_شاخص (بیعلامت صحيح جابه_جایی,
				     ثابت فراغ *پایه HB_استفادهنشده,
				     ثابت T *array HB_استفادهنشده)
  {
    أرجع جابه_جایی;
  }
  قالب <نام_نوع T>
  ساكن بیعلامت صحيح byte_جابه_جایی_مقصد_شاخص (بیعلامت صحيح جابه_جایی,
					 ثابت فراغ *پایه HB_استفادهنشده,
					 ثابت T *array HB_استفادهنشده)
  {
    أرجع جابه_جایی / 2;
  }
  قالب <نام_نوع T>
  ساكن بیعلامت صحيح کلمه_جابه_جایی_مقصد_شاخص (بیعلامت صحيح جابه_جایی,
					 ثابت فراغ *پایه HB_استفادهنشده,
					 ثابت T *array HB_استفادهنشده)
  {
    أرجع جابه_جایی;
  }
};

قالب <نام_نوع Types, نام_نوع مدخل_داده, نام_نوع Flags>
بنية State_جدول_Driver
{
  استفاده State_جدول_T = State_جدول<Types, مدخل_داده>;
  استفاده مدخل_T = مدخل_2<مدخل_داده>;

  State_جدول_Driver (ثابت State_جدول_T &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      رقم_glyphs (face_->get_رقم_glyphs ()) {}

  قالب <نام_نوع context_t>
  فراغ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_میانگیر_t *میانگیر = ac->میانگیر;

    إذا (!c->in_place)
      میانگیر->clear_output ();

    صحيح state = State_جدول_T::STATE_شروع_از_متن;
    // If there's only one range, we already checked the flag.
    خودکار *آخرین_range = ac->range_flags && (ac->range_flags->درازی > 1) ? &(*ac->range_flags)[0] : اشاره_تهی;
    ثابت منطقي شروع_state_امن_مقصد_break_eot =
      !c->جدول->is_actionable (machine.get_مدخل (State_جدول_T::STATE_شروع_از_متن, CLASS_انتها_از_متن));
    لكل (میانگیر->idx = 0; میانگیر->successful;)
    {
      بیعلامت صحيح klass = likely (میانگیر->idx < میانگیر->len) ?
			   machine.get_class (میانگیر->cur().codepoint, رقم_glyphs, ac->machine_class_cache) :
			   (بیعلامت) CLASS_انتها_از_متن;
    resume:
      DEBUG_پیام (APPLY, اشاره_تهی, "c%u at %u", klass, میانگیر->idx);
      ثابت مدخل_T &مدخل = machine.get_مدخل (state, klass);
      ثابت صحيح بعدی_state = machine.new_state (مدخل.newState);

      منطقي is_not_epsilon_transition = !(مدخل.flags & Flags::DontAdvance);
      منطقي is_not_actionable = !c->جدول->is_actionable (مدخل);

      إذا (unlikely (آخرین_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	خودکار *range = آخرین_range;
	إذا (میانگیر->idx < میانگیر->len)
	{
	  بیعلامت cluster = میانگیر->cur().cluster;
	  طالما (cluster < range->cluster_اولی)
	    range--;
	  طالما (cluster > range->cluster_آخرین)
	    range++;


	  آخرین_range = range;
	}
	إذا (!(range->flags & ac->subtable_flags))
	{
	  إذا (میانگیر->idx == میانگیر->len)
	    اكسر;

	  state = State_جدول_T::STATE_شروع_از_متن;
	  (فراغ) میانگیر->بعدی_glyph ();
	  استمر;
	}
      }
      وإلا
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	منطقي is_null_transition = state == State_جدول_T::STATE_شروع_از_متن &&
				  بعدی_state == State_جدول_T::STATE_شروع_از_متن &&
				  شروع_state_امن_مقصد_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	إذا (is_null_transition)
	{
	  بیعلامت old_klass = klass;
	  افعل
	  {
	    c->transition (میانگیر, هذا, مدخل);

	    إذا (میانگیر->idx == میانگیر->len || !میانگیر->successful)
	      اكسر;

	    (فراغ) میانگیر->بعدی_glyph ();

	    klass = likely (میانگیر->idx < میانگیر->len) ?
		     machine.get_class (میانگیر->cur().codepoint, رقم_glyphs, ac->machine_class_cache) :
		     (بیعلامت) CLASS_انتها_از_متن;
	  } طالما (klass == old_klass);

	  إذا (میانگیر->idx == میانگیر->len || !میانگیر->successful)
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
      ثابت مدخل_T *wouldbe_مدخل;
      منطقي is_امن_مقصد_break =
      (
          /* 1. */
          !c->جدول->is_actionable (مدخل) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == State_جدول_T::STATE_شروع_از_متن
              || ((مدخل.flags & Flags::DontAdvance) && بعدی_state == State_جدول_T::STATE_شروع_از_متن)
              || (
		    /* 2c. */
		    wouldbe_مدخل = &machine.get_مدخل(State_جدول_T::STATE_شروع_از_متن, klass)
		    ,
		    /* 2c'. */
		    !c->جدول->is_actionable (*wouldbe_مدخل) &&
		    /* 2c". */
		    (
		      بعدی_state == machine.new_state(wouldbe_مدخل->newState) &&
		      (مدخل.flags & Flags::DontAdvance) == (wouldbe_مدخل->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->جدول->is_actionable (machine.get_مدخل (state, CLASS_انتها_از_متن))
      );

      إذا (!is_امن_مقصد_break && میانگیر->backtrack_len () && میانگیر->idx < میانگیر->len)
	میانگیر->unsafe_مقصد_break_from_outbuffer (میانگیر->backtrack_len () - 1, میانگیر->idx + 1);

      c->transition (میانگیر, هذا, مدخل);

      state = بعدی_state;
      DEBUG_پیام (APPLY, اشاره_تهی, "s%d", state);

      إذا (میانگیر->idx == میانگیر->len)
	اكسر;

      إذا (is_not_epsilon_transition || میانگیر->max_ops-- <= 0)
	(فراغ) میانگیر->بعدی_glyph ();
    }

    إذا (!c->in_place)
      میانگیر->متزامن ();
  }

  عام:
  ثابت State_جدول_T &machine;
  بیعلامت صحيح رقم_glyphs;
};


} /* namespace AAT */


#پایان_اگر /* HB_AAT_LAYOUT_COMMON_HH */
