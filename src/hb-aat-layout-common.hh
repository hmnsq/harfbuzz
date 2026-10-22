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

#که_نه_تعریف_شوی HB_AAT_LAYOUT_COMMON_HH
#تعریف_کړه HB_AAT_LAYOUT_COMMON_HH

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

کارول نطاق OT;

بنية ankr;

کارول hb_aat_class_cache_t = hb_ot_layout_mapping_cache_t;

/* The state-machine subtables' transition functions are called once per
 * transition from the drive loop, but are big enough that the compiler
 * declines to inline them, leaving a call in the hottest loop of AAT
 * shaping.  Forcing them inline is worth 3-10% instructions on
 * morx/kerx-heavy fonts, for about 16KB of code. */
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
#تعریف_کړه HB_AAT_TRANSITION_INLINE HB_ALWAYS_INLINE
#وإلا
#تعریف_کړه HB_AAT_TRANSITION_INLINE
#د_که_پای

بنية hb_aat_scratch_t
{
  hb_aat_scratch_t () = افتراضي;
  hb_aat_scratch_t (ثابت hb_aat_scratch_t &) = ړنګول;

  hb_aat_scratch_t (hb_aat_scratch_t &&o)
  {
    بفر_glyph_سېټ.سېټ_relaxed (o.بفر_glyph_سېټ.get_relaxed ());
    o.بفر_glyph_سېټ.سېټ_relaxed (تش_اشاره);
  }
  hb_aat_scratch_t & چلوونکی = (hb_aat_scratch_t &&o)
  {
    بفر_glyph_سېټ.سېټ_relaxed (o.بفر_glyph_سېټ.get_relaxed ());
    o.بفر_glyph_سېټ.سېټ_relaxed (تش_اشاره);
    أرجع *هذا;
  }
  ~hb_aat_scratch_t ()
  {
    خپلکاری *s = بفر_glyph_سېټ.get_relaxed ();
    إذا (unlikely (!s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  hb_بټ_سېټ_t *create_بفر_glyph_سېټ () ثابت
  {
    hb_بټ_سېټ_t *s = بفر_glyph_سېټ.get_acquire ();
    إذا (s && بفر_glyph_سېټ.cmpexch (s, تش_اشاره))
    {
      s->clear ();
      أرجع s;
    }

    s = (hb_بټ_سېټ_t *) hb_calloc (1, حجم (hb_بټ_سېټ_t));
    إذا (unlikely (!s))
      أرجع تش_اشاره;
    s->init ();

    أرجع s;
  }
  فراغ destroy_بفر_glyph_سېټ (hb_بټ_سېټ_t *s) ثابت
  {
    إذا (unlikely (!s))
      أرجع;
    إذا (بفر_glyph_سېټ.cmpexch (تش_اشاره, s))
      أرجع;
    s->fini ();
    hb_free (s);
  }

  بدلیدونکی hb_atomic_t<hb_بټ_سېټ_t *> بفر_glyph_سېټ;
};

تعداد { DELETED_GLYPH = 0xFFFF };

#تعریف_کړه HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED HB_بفر_SCRATCH_FLAG_SHAPER0

بنية hb_aat_apply_context_t :
       hb_dispatch_context_t<hb_aat_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { أرجع obj.apply (هذا, std::forward<Ts> (ds)...); }
  ساكن return_t default_return_ارزښت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  ثابت hb_ot_shape_plan_t *plan;
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t *بفر;
  hb_sanitize_context_t sanitizer;
  ثابت ankr *ankr_جدول;
  ثابت OT::GDEF &gdef;
  منطقي has_glyph_classes;
  ثابت hb_sorted_ویکټور_t<hb_aat_map_t::range_flags_t> *range_flags = تش_اشاره;
  hb_ماسک_t subtable_flags = 0;
  منطقي بفر_is_reversed = خطأ;
  // Caches
  منطقي using_بفر_glyph_سېټ = خطأ;
  hb_بټ_سېټ_t *بفر_glyph_سېټ = تش_اشاره;
  ثابت hb_بټ_سېټ_t *لومړی_سېټ = تش_اشاره;
  ثابت hb_بټ_سېټ_t *دوهم_سېټ = تش_اشاره;
  hb_aat_class_cache_t *machine_class_cache = تش_اشاره;

  /* Unused. For debug tracing only. */
  بینښه صحيح lookup_شاخص;

  HB_دننی hb_aat_apply_context_t (ثابت hb_ot_shape_plan_t *plan_,
				      hb_font_t *font_,
				      hb_بفر_t *بفر_2,
				      hb_blob_t *blob = const_cast<hb_blob_t *> (&Null (hb_blob_t)));

  HB_دننی ~hb_aat_apply_context_t ();

  HB_دننی فراغ سېټ_ankr_جدول (ثابت AAT::ankr *ankr_جدول_2);

  فراغ سېټ_lookup_شاخص (بینښه صحيح i) { lookup_شاخص = i; }

  فراغ سرچپه_بفر ()
  {
    بفر->reverse ();
    بفر_is_reversed = !بفر_is_reversed;
  }

  فراغ تیاری_بفر_glyph_سېټ ()
  {
    using_بفر_glyph_سېټ = بفر->len >= 4 && بفر_glyph_سېټ;

    إذا (likely (using_بفر_glyph_سېټ))
      بفر->collect_codepoints (*بفر_glyph_سېټ);
  }
  منطقي بفر_intersects_machine () ثابت
  {
    إذا (likely (using_بفر_glyph_سېټ))
      أرجع بفر_glyph_سېټ->intersects (*لومړی_سېټ);

    // Faster for shorter buffers.
    لكل (بینښه i = 0; i < بفر->len; i++)
      إذا (لومړی_سېټ->has (بفر->info[i].codepoint))
	أرجع ریښتیا;
    أرجع خطأ;
  }

  کينډۍ <ډول_نوم T>
  HB_NODISCARD منطقي output_glyphs (بینښه صحيح شمېر,
				   ثابت T *glyphs)
  {
    إذا (likely (using_بفر_glyph_سېټ))
      بفر_glyph_سېټ->جمع_کړه_array (glyphs, شمېر);
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      منطقي at_پای_ټکی_د_متن = بفر->idx == بفر->len;
      إذا (at_پای_ټکی_د_متن)
	إذا (unlikely (!بفر->output_glyph (glyphs[i]))) أرجع خطأ;

      hb_glyph_info_t &info = at_پای_ټکی_د_متن ? بفر->prev () : بفر->cur ();
      إذا (glyphs[i] == DELETED_GLYPH)
      {
        بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
	hb_glyph_info_سېټ_aat_deleted (&info);
      }
      وإلا
      {
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT
	إذا (has_glyph_classes)
	  hb_glyph_info_سېټ_glyph_props (&info,
					  gdef.get_glyph_props (glyphs[i]));
#د_که_پای
      }
      إذا (!at_پای_ټکی_د_متن)
	إذا (unlikely (!بفر->output_glyph (glyphs[i]))) أرجع خطأ;
    }
    أرجع ریښتیا;
  }

  HB_NODISCARD منطقي replace_glyph (hb_codepoint_t glyph)
  {
    إذا (glyph == DELETED_GLYPH)
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_سېټ_aat_deleted (&بفر->cur());
    }

    إذا (likely (using_بفر_glyph_سېټ))
      بفر_glyph_سېټ->جمع_کړه (glyph);
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      hb_glyph_info_سېټ_glyph_props (&بفر->cur(),
				      gdef.get_glyph_props (glyph));
#د_که_پای
    أرجع بفر->replace_glyph (glyph);
  }

  HB_NODISCARD منطقي delete_glyph ()
  {
    بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
    hb_glyph_info_سېټ_aat_deleted (&بفر->cur());
    إذا (likely (using_بفر_glyph_سېټ))
      بفر_glyph_سېټ->جمع_کړه (DELETED_GLYPH);
    أرجع بفر->replace_glyph (DELETED_GLYPH);
  }

  فراغ replace_glyph_inplace (بینښه i, hb_codepoint_t glyph)
  {
    بفر->info[i].codepoint = glyph;
    إذا (glyph == DELETED_GLYPH)
    {
      بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_AAT_HAS_DELETED;
      hb_glyph_info_سېټ_aat_deleted (&بفر->info[i]);
    }
    إذا (likely (using_بفر_glyph_سېټ))
      بفر_glyph_سېټ->جمع_کړه (glyph);
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT
    إذا (has_glyph_classes)
      hb_glyph_info_سېټ_glyph_props (&بفر->info[i],
				      gdef.get_glyph_props (glyph));
#د_که_پای
  }
};


/*
 * Lookup Table
 */

کينډۍ <ډول_نوم T> بنية Lookup;

کينډۍ <ډول_نوم T>
بنية LookupFormat0
{
  ملګری بنية Lookup<T>;

  خاص:
  ثابت T* get_ارزښت (hb_codepoint_t glyph_id, بینښه صحيح عدد_glyphs) ثابت
  {
    إذا (unlikely (glyph_id >= عدد_glyphs)) أرجع تش_اشاره;
    أرجع &arrayZ[glyph_id];
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs, بینښه عدد_glyphs) ثابت
  {
    glyphs.جمع_کړه_range (0, عدد_glyphs - 1);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, بینښه عدد_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (بینښه i = 0; i < عدد_glyphs; i++)
      إذا (filter (arrayZ[i]))
	glyphs.جمع_کړه (i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_عدد_glyphs ()));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (arrayZ.sanitize (c, c->get_عدد_glyphs (), بنسټ));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 0 */
  Unsized_Array_د<T>
		arrayZ;		/* Array of lookup values, indexed by glyph index. */
  عام:
  DEFINE_اندازه_UNBOUNDED (2);
};


کينډۍ <ډول_نوم T>
بنية LookupSegmentSingle
{
  ساكن constexpr بینښه Termination_کلمه_شمېر = 2u;

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < لومړی ? -1 : g <= وروستی ? 0 : +1 ; }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs) ثابت
  {
    إذا (لومړی == DELETED_GLYPH) أرجع;
    glyphs.جمع_کړه_range (لومړی, وروستی);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (لومړی == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
    glyphs.جمع_کړه_range (لومړی, وروستی);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, بنسټ));
  }

  HBGlyphID16	وروستی;		/* Last GlyphID in this segment */
  HBGlyphID16	لومړی;		/* First GlyphID in this segment */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_اندازه_STATIC (4 + T::static_اندازه);
};

کينډۍ <ډول_نوم T>
بنية LookupFormat2
{
  ملګری بنية Lookup<T>;

  خاص:
  ثابت T* get_ارزښت (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentSingle<T> *v = segments.bsearch (glyph_id);
    أرجع v ? &v->value : تش_اشاره;
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs) ثابت
  {
    بینښه شمېر = segments.get_اوږدتیا ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      segments[i].collect_glyphs (glyphs);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بینښه شمېر = segments.get_اوږدتیا ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      segments[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, بنسټ));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 */
  Var_Sized_Bin_پلټنه_Array_د<LookupSegmentSingle<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_اندازه_ARRAY (8, segments);
};

کينډۍ <ډول_نوم T>
بنية LookupSegmentArray
{
  ساكن constexpr بینښه Termination_کلمه_شمېر = 2u;

  ثابت T* get_ارزښت (hb_codepoint_t glyph_id, ثابت فراغ *بنسټ) ثابت
  {
    أرجع لومړی <= glyph_id && glyph_id <= وروستی ? &(بنسټ+ارزښتونه_Z)[glyph_id - لومړی] : تش_اشاره;
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs) ثابت
  {
    إذا (لومړی == DELETED_GLYPH) أرجع;
    glyphs.جمع_کړه_range (لومړی, وروستی);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, ثابت فراغ *بنسټ, ثابت filter_t &filter) ثابت
  {
    إذا (لومړی == DELETED_GLYPH) أرجع;
    ثابت خپلکاری &values = بنسټ+ارزښتونه_Z;
    لكل (hb_codepoint_t i = لومړی; i <= وروستی; i++)
      إذا (filter (values[i - لومړی]))
	glyphs.جمع_کړه (i);
  }

  صحيح cmp (hb_codepoint_t g) ثابت
  { أرجع g < لومړی ? -1 : g <= وروستی ? 0 : +1; }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  لومړی <= وروستی &&
		  ارزښتونه_Z.sanitize (c, بنسټ, وروستی - لومړی + 1));
  }
  کينډۍ <ډول_نوم ...Ts>
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ, Ts&&... ds) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  لومړی <= وروستی &&
		  ارزښتونه_Z.sanitize (c, بنسټ, وروستی - لومړی + 1, std::forward<Ts> (ds)...));
  }

  HBGlyphID16	وروستی;		/* Last GlyphID in this segment */
  HBGlyphID16	لومړی;		/* First GlyphID in this segment */
  NNOffset16_منزل<Unsized_Array_د<T>>
		ارزښتونه_Z;	/* A 16-bit offset from the start of
				 * the table to the data. */
  عام:
  DEFINE_اندازه_STATIC (6);
};

کينډۍ <ډول_نوم T>
بنية LookupFormat4
{
  ملګری بنية Lookup<T>;

  خاص:
  ثابت T* get_ارزښت (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSegmentArray<T> *v = segments.bsearch (glyph_id);
    أرجع v ? v->get_ارزښت (glyph_id, هذا) : تش_اشاره;
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs) ثابت
  {
    بینښه شمېر = segments.get_اوږدتیا ();
    لكل (بینښه i = 0; i < شمېر; i++)
      segments[i].collect_glyphs (glyphs);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بینښه شمېر = segments.get_اوږدتیا ();
    لكل (بینښه i = 0; i < شمېر; i++)
      segments[i].collect_glyphs_filtered (glyphs, هذا, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (segments.sanitize (c, هذا, بنسټ));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Var_Sized_Bin_پلټنه_Array_د<LookupSegmentArray<T>>
		segments;	/* The actual segments. These must already be sorted,
				 * according to the first word in each one (the last
				 * glyph in each segment). */
  عام:
  DEFINE_اندازه_ARRAY (8, segments);
};

کينډۍ <ډول_نوم T>
بنية LookupSingle
{
  ساكن constexpr بینښه Termination_کلمه_شمېر = 1u;

  صحيح cmp (hb_codepoint_t g) ثابت { أرجع glyph.cmp (g); }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    glyphs.جمع_کړه (glyph);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (glyph == DELETED_GLYPH) أرجع;
    إذا (!filter (value)) أرجع;
    glyphs.جمع_کړه (glyph);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && value.sanitize (c, بنسټ));
  }

  HBGlyphID16	glyph;		/* Last GlyphID */
  T		value;		/* The lookup value (only one) */
  عام:
  DEFINE_اندازه_STATIC (2 + T::static_اندازه);
};

کينډۍ <ډول_نوم T>
بنية LookupFormat6
{
  ملګری بنية Lookup<T>;

  خاص:
  ثابت T* get_ارزښت (hb_codepoint_t glyph_id) ثابت
  {
    ثابت LookupSingle<T> *v = entries.bsearch (glyph_id);
    أرجع v ? &v->value : تش_اشاره;
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs) ثابت
  {
    بینښه شمېر = entries.get_اوږدتیا ();
    لكل (بینښه i = 0; i < شمېر; i++)
      entries[i].collect_glyphs (glyphs);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    بینښه شمېر = entries.get_اوږدتیا ();
    لكل (بینښه i = 0; i < شمېر; i++)
      entries[i].collect_glyphs_filtered (glyphs, filter);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (entries.sanitize (c, بنسټ));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 6 */
  Var_Sized_Bin_پلټنه_Array_د<LookupSingle<T>>
		entries;	/* The actual entries, sorted by glyph index. */
  عام:
  DEFINE_اندازه_ARRAY (8, entries);
};

کينډۍ <ډول_نوم T>
بنية LookupFormat8
{
  ملګری بنية Lookup<T>;

  خاص:
  ثابت T* get_ارزښت (hb_codepoint_t glyph_id) ثابت
  {
    أرجع لومړی_Glyph <= glyph_id && glyph_id - لومړی_Glyph < glyph_شمېر_2 ?
	   &ارزښت_Array_Z[glyph_id - لومړی_Glyph] : تش_اشاره;
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_شمېر_2)) أرجع;
    إذا (لومړی_Glyph == DELETED_GLYPH) أرجع;
    glyphs.جمع_کړه_range (لومړی_Glyph, لومړی_Glyph + glyph_شمېر_2 - 1);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_شمېر_2)) أرجع;
    إذا (لومړی_Glyph == DELETED_GLYPH) أرجع;
    ثابت T *p = ارزښت_Array_Z.arrayZ;
    لكل (بینښه i = 0; i < glyph_شمېر_2; i++)
      إذا (filter (p[i]))
	glyphs.جمع_کړه (لومړی_Glyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && ارزښت_Array_Z.sanitize (c, glyph_شمېر_2));
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && ارزښت_Array_Z.sanitize (c, glyph_شمېر_2, بنسټ));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBGlyphID16	لومړی_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_شمېر_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_د<T>
		ارزښت_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_اندازه_ARRAY (6, ارزښت_Array_Z);
};

کينډۍ <ډول_نوم T>
بنية LookupFormat10
{
  ملګری بنية Lookup<T>;

  خاص:
  ثابت ډول_نوم T::type get_ارزښت_or_null (hb_codepoint_t glyph_id) ثابت
  {
    إذا (!(لومړی_Glyph <= glyph_id && glyph_id - لومړی_Glyph < glyph_شمېر_2))
      أرجع Null (T);

    ثابت HBUINT8 *p = &ارزښت_Array_Z[(glyph_id - لومړی_Glyph) * ارزښت_اندازه];

    بینښه صحيح v = 0;
    بینښه صحيح شمېر = ارزښت_اندازه;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      v = (v << 8) | *p++;

    أرجع v;
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs) ثابت
  {
    إذا (unlikely (!glyph_شمېر_2)) أرجع;
    إذا (لومړی_Glyph == DELETED_GLYPH) أرجع;
    glyphs.جمع_کړه_range (لومړی_Glyph, لومړی_Glyph + glyph_شمېر_2 - 1);
  }

  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, ثابت filter_t &filter) ثابت
  {
    إذا (unlikely (!glyph_شمېر_2)) أرجع;
    إذا (لومړی_Glyph == DELETED_GLYPH) أرجع;
    ثابت HBUINT8 *p = ارزښت_Array_Z.arrayZ;
    لكل (بینښه i = 0; i < glyph_شمېر_2; i++)
    {
      بینښه صحيح v = 0;
      بینښه صحيح شمېر = ارزښت_اندازه;
      لكل (بینښه صحيح j = 0; j < شمېر; j++)
	v = (v << 8) | *p++;
      إذا (filter (v))
	glyphs.جمع_کړه (لومړی_Glyph + i);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  ارزښت_اندازه <= 4 &&
		  ارزښت_Array_Z.sanitize (c, glyph_شمېر_2 * ارزښت_اندازه));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 8 */
  HBUINT16	ارزښت_اندازه;	/* Byte size of each value. */
  HBGlyphID16	لومړی_Glyph;	/* First glyph index included in the trimmed array. */
  HBUINT16	glyph_شمېر_2;	/* Total number of glyphs (equivalent to the last
				 * glyph minus the value of firstGlyph plus 1). */
  Unsized_Array_د<HBUINT8>
		ارزښت_Array_Z;	/* The lookup values (indexed by the glyph index
				 * minus the value of firstGlyph). */
  عام:
  DEFINE_اندازه_ARRAY (8, ارزښت_Array_Z);
};

کينډۍ <ډول_نوم T>
بنية Lookup
{
  ثابت T* get_ارزښت (hb_codepoint_t glyph_id, بینښه صحيح عدد_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_ارزښت (glyph_id, عدد_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.get_ارزښت (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_ارزښت (glyph_id);
    حالة 6: hb_barrier (); أرجع u.format6.get_ارزښت (glyph_id);
    حالة 8: hb_barrier (); أرجع u.format8.get_ارزښت (glyph_id);
    افتراضي:أرجع تش_اشاره;
    }
  }

  ثابت ډول_نوم T::type get_ارزښت_or_null (hb_codepoint_t glyph_id, بینښه صحيح عدد_glyphs) ثابت
  {
    بدل (u.format.v) {
      /* Format 10 cannot return a pointer. */
      حالة 10: hb_barrier (); أرجع u.format10.get_ارزښت_or_null (glyph_id);
      افتراضي:
      ثابت T *v = get_ارزښت (glyph_id, عدد_glyphs);
      أرجع v ? *v : Null (T);
    }
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs, بینښه صحيح عدد_glyphs) ثابت
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
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, بینښه عدد_glyphs, ثابت filter_t &filter) ثابت
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

  ډول_نوم T::type get_class (hb_codepoint_t glyph_id,
			      بینښه صحيح عدد_glyphs,
			      بینښه صحيح بهر_د_Range) ثابت
  {
    ثابت T *v = get_ارزښت (glyph_id, عدد_glyphs);
    أرجع v ? *v : بهر_د_Range;
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
    افتراضي:return_trace (ریښتیا);
    }
  }
  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c, بنسټ));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c, بنسټ));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c, بنسټ));
    حالة 6: hb_barrier (); return_trace (u.format6.sanitize (c, بنسټ));
    حالة 8: hb_barrier (); return_trace (u.format8.sanitize (c, بنسټ));
    حالة 10: return_trace (خطأ); /* We don't support format10 here currently. */
    افتراضي:return_trace (ریښتیا);
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
DECLARE_NULL_NAMESPACE_بایټونه_TEMPLATE1 (AAT, Lookup, 13);

/*
 * (Extended) State Table
 */

کينډۍ <ډول_نوم T>
بنية ثبت_2
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
  T		معلومات;		/* Optional offsets to per-glyph tables. */
  عام:
  DEFINE_اندازه_STATIC (4 + T::static_اندازه);
};

کينډۍ <>
بنية ثبت_2<فراغ>
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
  CLASS_پای_ټکی_د_متن = 0,
  CLASS_بهر_د_BOUNDS = 1,
  CLASS_DELETED_GLYPH = 2,
  CLASS_پای_ټکی_د_کرښه = 3,
};

کينډۍ <ډول_نوم Types, ډول_نوم Extra>
بنية State_جدول
{
  تعريفنوع ډول_نوم Types::HBUINT HBUINT;
  تعريفنوع ډول_نوم Types::HBUSHORT HBUSHORT;
  تعريفنوع ډول_نوم Types::ClassTypeNarrow ClassType;

  تعداد State
  {
    STATE_پیل_د_متن = 0,
    STATE_پیل_د_کرښه = 1,
  };

  کينډۍ <ډول_نوم سېټ_t, ډول_نوم جدول_t>
  فراغ collect_initial_glyphs (سېټ_t &glyphs, بینښه عدد_glyphs, ثابت جدول_t &جدول) ثابت
  {
    بینښه عدد_classes = nClasses;

    إذا (unlikely (عدد_classes > hb_بټ_پاڼه_t::بټونه_2))
    {
      (هذا+class_جدول).collect_glyphs (glyphs, عدد_glyphs);
      أرجع;
    }

    // Collect all classes going out from the start state.
    hb_بټ_پاڼه_t filter;

    لكل (بینښه i = 0; i < عدد_classes; i++)
    {
      ثابت خپلکاری &ثبت = get_ثبت (STATE_پیل_د_متن, i);
      إذا (new_state (ثبت.newState) == STATE_پیل_د_متن &&
	  !جدول.is_action_initiable (ثبت) && !جدول.is_actionable (ثبت))
	استمر;

      filter.جمع_کړه (i);
    }

    // And glyphs in those classes.

    إذا (filter (CLASS_DELETED_GLYPH))
      glyphs.جمع_کړه (DELETED_GLYPH);

    (هذا+class_جدول).collect_glyphs_filtered (glyphs, عدد_glyphs, filter);
  }

  صحيح new_state (بینښه صحيح newState) ثابت
  { أرجع Types::extended ? newState : ((صحيح) newState - (صحيح) state_Array_جدول) / (صحيح) nClasses; }

  بینښه صحيح get_class (hb_codepoint_t glyph_id,
			  بینښه صحيح عدد_glyphs,
			  hb_aat_class_cache_t *cache = تش_اشاره) ثابت
  {
    بینښه klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    إذا (unlikely (glyph_id == DELETED_GLYPH)) أرجع CLASS_DELETED_GLYPH;
    klass = (هذا+class_جدول).get_class (glyph_id, عدد_glyphs, CLASS_بهر_د_BOUNDS);
    إذا (cache) cache->سېټ (glyph_id, klass);
    أرجع klass;
  }

  ثابت ثبت_2<Extra> *get_entries () ثابت
  { أرجع (هذا+ثبت_جدول).arrayZ; }

  ثابت ثبت_2<Extra> &get_ثبت (صحيح state, بینښه صحيح klass) ثابت
  {
    بینښه n_classes = nClasses;
    إذا (unlikely (klass >= n_classes))
      klass = CLASS_بهر_د_BOUNDS;

    ثابت HBUSHORT *حالتونه = (هذا+state_Array_جدول).arrayZ;
    ثابت ثبت_2<Extra> *entries = (هذا+ثبت_جدول).arrayZ;

    بینښه صحيح ثبت = حالتونه[state * n_classes + klass];
    DEBUG_پیغام (APPLY, تش_اشاره, "e%u", ثبت);

    أرجع entries[ثبت];
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 بینښه صحيح *عدد_entries_بهر = تش_اشاره) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!(c->check_struct (هذا) &&
		    hb_barrier () &&
		    nClasses >= 4 /* Ensure pre-defined classes fit.  */ &&
		    class_جدول.sanitize (c, هذا)))) return_trace (خطأ);

    ثابت HBUSHORT *حالتونه = (هذا+state_Array_جدول).arrayZ;
    ثابت ثبت_2<Extra> *entries = (هذا+ثبت_جدول).arrayZ;

    بینښه صحيح عدد_classes = nClasses;
    إذا (unlikely (hb_unsigned_mul_overflows (عدد_classes, حالتونه[0].static_اندازه)))
      return_trace (خطأ);
    بینښه صحيح لیکه_stride = عدد_classes * حالتونه[0].static_اندازه;

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
    بینښه صحيح عدد_entries = 0;

    صحيح state_pos = 0;
    صحيح state_neg = 0;
    بینښه صحيح ثبت = 0;
    طالما (min_state < state_neg || state_pos <= max_state)
    {
      إذا (min_state < state_neg)
      {
	/* Negative states. */
	إذا (unlikely (hb_unsigned_mul_overflows (min_state, عدد_classes)))
	  return_trace (خطأ);
	إذا (unlikely (!c->check_range (&حالتونه[min_state * عدد_classes],
				       -min_state,
				       لیکه_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= state_neg - min_state) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  ثابت HBUSHORT *stop = &حالتونه[min_state * عدد_classes];
	  إذا (unlikely (stop > حالتونه))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = حالتونه; stop < p; p--)
	    عدد_entries = hb_max (عدد_entries, *(p - 1) + 1u);
	  state_neg = min_state;
	}
      }

      إذا (state_pos <= max_state)
      {
	/* Positive states. */
	إذا (unlikely (!c->check_range (حالتونه,
				       max_state + 1,
				       لیکه_stride)))
	  return_trace (خطأ);
	إذا ((c->max_ops -= max_state - state_pos + 1) <= 0)
	  return_trace (خطأ);
	{ /* Sweep new states. */
	  إذا (unlikely (hb_unsigned_mul_overflows ((max_state + 1), عدد_classes)))
	    return_trace (خطأ);
	  ثابت HBUSHORT *stop = &حالتونه[(max_state + 1) * عدد_classes];
	  إذا (unlikely (stop < حالتونه))
	    return_trace (خطأ);
	  لكل (ثابت HBUSHORT *p = &حالتونه[state_pos * عدد_classes]; p < stop; p++)
	    عدد_entries = hb_max (عدد_entries, *p + 1u);
	  state_pos = max_state + 1;
	}
      }

      إذا (unlikely (!c->check_array (entries, عدد_entries)))
	return_trace (خطأ);
      إذا ((c->max_ops -= عدد_entries - ثبت) <= 0)
	return_trace (خطأ);
      { /* Sweep new entries. */
	ثابت ثبت_2<Extra> *stop = &entries[عدد_entries];
	لكل (ثابت ثبت_2<Extra> *p = &entries[ثبت]; p < stop; p++)
	{
	  صحيح newState = new_state (p->newState);
	  min_state = hb_min (min_state, newState);
	  max_state = hb_max (max_state, newState);
	}
	ثبت = عدد_entries;
      }
    }

    إذا (عدد_entries_بهر)
      *عدد_entries_بهر = عدد_entries;

    return_trace (ریښتیا);
  }

  محمي:
  HBUINT	nClasses;	/* Number of classes, which is the number of indices
				 * in a single line in the state array. */
  NNOffset_منزل<ClassType, HBUINT>
		class_جدول;	/* Offset to the class table. */
  NNOffset_منزل<Unsized_Array_د<HBUSHORT>, HBUINT>
		state_Array_جدول;/* Offset to the state array. */
  NNOffset_منزل<Unsized_Array_د<ثبت_2<Extra>>, HBUINT>
		ثبت_جدول;	/* Offset to the entry array. */

  عام:
  DEFINE_اندازه_STATIC (4 * حجم (HBUINT));
};

کينډۍ <ډول_نوم HBUCHAR>
بنية Class_جدول
{
  بینښه صحيح get_class (hb_codepoint_t glyph_id, بینښه صحيح بهر_د_Range) ثابت
  {
    بینښه صحيح i = glyph_id - لومړی_Glyph;
    أرجع i >= classArray.len ? بهر_د_Range : classArray.arrayZ[i];
  }
  بینښه صحيح get_class (hb_codepoint_t glyph_id,
			  بینښه صحيح عدد_glyphs HB_ناکارول_شوی,
			  بینښه صحيح بهر_د_Range) ثابت
  {
    أرجع get_class (glyph_id, بهر_د_Range);
  }

  کينډۍ <ډول_نوم سېټ_t>
  فراغ collect_glyphs (سېټ_t &glyphs, بینښه عدد_glyphs) ثابت
  {
    لكل (بینښه i = 0; i < classArray.len; i++)
      إذا (classArray.arrayZ[i] != CLASS_بهر_د_BOUNDS)
	glyphs.جمع_کړه (لومړی_Glyph + i);
  }
  کينډۍ <ډول_نوم سېټ_t, ډول_نوم filter_t>
  فراغ collect_glyphs_filtered (سېټ_t &glyphs, بینښه عدد_glyphs, ثابت filter_t &filter) ثابت
  {
    لكل (بینښه i = 0; i < classArray.len; i++)
      إذا (filter (classArray.arrayZ[i]))
	glyphs.جمع_کړه (لومړی_Glyph + i);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && classArray.sanitize (c));
  }
  محمي:
  HBGlyphID16		لومړی_Glyph;	/* First glyph index included in the trimmed array. */
  Array16_د<HBUCHAR>	classArray;	/* The class codes (indexed by glyph index minus
					 * firstGlyph). */
  عام:
  DEFINE_اندازه_ARRAY (4, classArray);
};

بنية SubtableGlyphCoverage
{
  منطقي sanitize (hb_sanitize_context_t *c, بینښه subtable_شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_array (&subtableOffsets, subtable_شمېر)))
      return_trace (خطأ);

    بینښه بایټونه = (c->get_عدد_glyphs () + CHAR_بټ - 1) / CHAR_بټ;
    لكل (بینښه i = 0; i < subtable_شمېر; i++)
    {
      uint32_t دننه_واټن = (uint32_t) subtableOffsets[i];
      // A font file called SFNSDisplay.ttf has value 0xFFFFFFFF in the offsets.
      // Just ignore it.
      إذا (دننه_واټن == 0 || دننه_واټن == 0xFFFFFFFF)
        استمر;
      إذا (unlikely (!subtableOffsets[i].sanitize (c, هذا, بایټونه)))
        return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }
  محمي:
  Unsized_Array_د<NNOffset32_منزل<Unsized_Array_د<HBUINT8>>> subtableOffsets;
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

  کينډۍ <ډول_نوم T>
  ساكن صحيح دننه_واټن_منزل_شاخص (int64_t دننه_واټن,
			    ثابت فراغ *بنسټ,
			    ثابت T *array)
  {
    int64_t array_دننه_واټن = (ثابت حرف *) array - (ثابت حرف *) بنسټ;
    صحيح bad_شاخص = لوی_عدد / T::static_اندازه;

    /* https://github.com/harfbuzz/harfbuzz/issues/3483 */
    /* If offset is less than base, return an offset that would
     * result in an address half a 32bit address-space away,
     * to make sure sanitize fails even on 32bit builds. */
    إذا (unlikely (دننه_واټن < array_دننه_واټن))
      أرجع bad_شاخص;

    /* https://github.com/harfbuzz/harfbuzz/issues/2816 */
    int64_t شاخص_2 = (دننه_واټن - array_دننه_واټن) / T::static_اندازه;
    إذا (unlikely (شاخص_2 > bad_شاخص))
      أرجع bad_شاخص;
    أرجع شاخص_2;
  }
  کينډۍ <ډول_نوم T>
  ساكن صحيح byte_دننه_واټن_منزل_شاخص (int64_t دننه_واټن,
				ثابت فراغ *بنسټ,
				ثابت T *array)
  {
    أرجع دننه_واټن_منزل_شاخص (دننه_واټن, بنسټ, array);
  }
  کينډۍ <ډول_نوم T>
  ساكن صحيح کلمه_دننه_واټن_منزل_شاخص (int64_t دننه_واټن,
				ثابت فراغ *بنسټ,
				ثابت T *array)
  {
    أرجع دننه_واټن_منزل_شاخص (2 * دننه_واټن, بنسټ, array);
  }
};
بنية ExtendedTypes
{
  ساكن constexpr منطقي extended = ریښتیا;
  تعريفنوع HBUINT32 HBUINT;
  تعريفنوع HBUINT16 HBUSHORT;
  تعريفنوع Lookup<HBUINT16> ClassTypeNarrow;
  تعريفنوع Lookup<HBUINT16> ClassTypeWide;

  کينډۍ <ډول_نوم T>
  ساكن بینښه صحيح دننه_واټن_منزل_شاخص (بینښه صحيح دننه_واټن,
				     ثابت فراغ *بنسټ HB_ناکارول_شوی,
				     ثابت T *array HB_ناکارول_شوی)
  {
    أرجع دننه_واټن;
  }
  کينډۍ <ډول_نوم T>
  ساكن بینښه صحيح byte_دننه_واټن_منزل_شاخص (بینښه صحيح دننه_واټن,
					 ثابت فراغ *بنسټ HB_ناکارول_شوی,
					 ثابت T *array HB_ناکارول_شوی)
  {
    أرجع دننه_واټن / 2;
  }
  کينډۍ <ډول_نوم T>
  ساكن بینښه صحيح کلمه_دننه_واټن_منزل_شاخص (بینښه صحيح دننه_واټن,
					 ثابت فراغ *بنسټ HB_ناکارول_شوی,
					 ثابت T *array HB_ناکارول_شوی)
  {
    أرجع دننه_واټن;
  }
};

کينډۍ <ډول_نوم Types, ډول_نوم ثبت_معلومات, ډول_نوم Flags>
بنية State_جدول_Driver
{
  کارول State_جدول_T = State_جدول<Types, ثبت_معلومات>;
  کارول ثبت_T = ثبت_2<ثبت_معلومات>;

  State_جدول_Driver (ثابت State_جدول_T &machine_,
		    hb_face_t *face_) :
	      machine (machine_),
	      عدد_glyphs (face_->get_عدد_glyphs ()) {}

  کينډۍ <ډول_نوم context_t>
  فراغ drive (context_t *c, hb_aat_apply_context_t *ac)
  {
    hb_بفر_t *بفر = ac->بفر;

    إذا (!c->in_place)
      بفر->clear_output ();

    صحيح state = State_جدول_T::STATE_پیل_د_متن;
    // If there's only one range, we already checked the flag.
    خپلکاری *وروستی_range = ac->range_flags && (ac->range_flags->اوږدتیا > 1) ? &(*ac->range_flags)[0] : تش_اشاره;
    ثابت منطقي پیل_state_خوندي_منزل_break_eot =
      !c->جدول->is_actionable (machine.get_ثبت (State_جدول_T::STATE_پیل_د_متن, CLASS_پای_ټکی_د_متن));
    لكل (بفر->idx = 0; بفر->successful;)
    {
      بینښه صحيح klass = likely (بفر->idx < بفر->len) ?
			   machine.get_class (بفر->cur().codepoint, عدد_glyphs, ac->machine_class_cache) :
			   (بینښه) CLASS_پای_ټکی_د_متن;
    resume:
      DEBUG_پیغام (APPLY, تش_اشاره, "c%u at %u", klass, بفر->idx);
      ثابت ثبت_T &ثبت = machine.get_ثبت (state, klass);
      ثابت صحيح بل_state = machine.new_state (ثبت.newState);

      منطقي is_not_epsilon_transition = !(ثبت.flags & Flags::DontAdvance);
      منطقي is_not_actionable = !c->جدول->is_actionable (ثبت);

      إذا (unlikely (وروستی_range))
      {
	/* This block is copied in NoncontextualSubtable::apply. Keep in sync. */
	خپلکاری *range = وروستی_range;
	إذا (بفر->idx < بفر->len)
	{
	  بینښه cluster = بفر->cur().cluster;
	  طالما (cluster < range->cluster_لومړی)
	    range--;
	  طالما (cluster > range->cluster_وروستی)
	    range++;


	  وروستی_range = range;
	}
	إذا (!(range->flags & ac->subtable_flags))
	{
	  إذا (بفر->idx == بفر->len)
	    اكسر;

	  state = State_جدول_T::STATE_پیل_د_متن;
	  (فراغ) بفر->بل_glyph ();
	  استمر;
	}
      }
      وإلا
      {
	// Fast path for when transitioning from start-state to start-state with
	// no action and advancing. Do so as long as the class remains the same.
	// This is common with runs of non-actionable glyphs.

	منطقي is_null_transition = state == State_جدول_T::STATE_پیل_د_متن &&
				  بل_state == State_جدول_T::STATE_پیل_د_متن &&
				  پیل_state_خوندي_منزل_break_eot &&
				  is_not_actionable &&
				  is_not_epsilon_transition;

	إذا (is_null_transition)
	{
	  بینښه old_klass = klass;
	  افعل
	  {
	    c->transition (بفر, هذا, ثبت);

	    إذا (بفر->idx == بفر->len || !بفر->successful)
	      اكسر;

	    (فراغ) بفر->بل_glyph ();

	    klass = likely (بفر->idx < بفر->len) ?
		     machine.get_class (بفر->cur().codepoint, عدد_glyphs, ac->machine_class_cache) :
		     (بینښه) CLASS_پای_ټکی_د_متن;
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
      ثابت ثبت_T *wouldbe_ثبت;
      منطقي is_خوندي_منزل_break =
      (
          /* 1. */
          !c->جدول->is_actionable (ثبت) &&

          /* 2. */
          // This one is meh, I know...
	  (
                 state == State_جدول_T::STATE_پیل_د_متن
              || ((ثبت.flags & Flags::DontAdvance) && بل_state == State_جدول_T::STATE_پیل_د_متن)
              || (
		    /* 2c. */
		    wouldbe_ثبت = &machine.get_ثبت(State_جدول_T::STATE_پیل_د_متن, klass)
		    ,
		    /* 2c'. */
		    !c->جدول->is_actionable (*wouldbe_ثبت) &&
		    /* 2c". */
		    (
		      بل_state == machine.new_state(wouldbe_ثبت->newState) &&
		      (ثبت.flags & Flags::DontAdvance) == (wouldbe_ثبت->flags & Flags::DontAdvance)
		    )
		 )
	  ) &&

          /* 3. */
          !c->جدول->is_actionable (machine.get_ثبت (state, CLASS_پای_ټکی_د_متن))
      );

      إذا (!is_خوندي_منزل_break && بفر->backtrack_len () && بفر->idx < بفر->len)
	بفر->unsafe_منزل_break_from_outbuffer (بفر->backtrack_len () - 1, بفر->idx + 1);

      c->transition (بفر, هذا, ثبت);

      state = بل_state;
      DEBUG_پیغام (APPLY, تش_اشاره, "s%d", state);

      إذا (بفر->idx == بفر->len)
	اكسر;

      إذا (is_not_epsilon_transition || بفر->max_ops-- <= 0)
	(فراغ) بفر->بل_glyph ();
    }

    إذا (!c->in_place)
      بفر->متزامن ();
  }

  عام:
  ثابت State_جدول_T &machine;
  بینښه صحيح عدد_glyphs;
};


} /* namespace AAT */


#د_که_پای /* HB_AAT_LAYOUT_COMMON_HH */
