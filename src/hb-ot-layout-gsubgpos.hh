/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2010,2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#اگر_تعریف_نشده HB_OT_LAYOUT_GSUBGPOS_HH
#تعریف_کن HB_OT_LAYOUT_GSUBGPOS_HH

#ضمن "hb.hh"
#ضمن "hb-buffer.hh"
#ضمن "hb-map.hh"
#ضمن "hb-set.hh"
#ضمن "hb-ot-map.hh"
#ضمن "hb-ot-layout-common.hh"
#ضمن "hb-ot-layout-gdef-table.hh"
#ضمن "hb-depend-data.hh"


نطاق OT {


بنية hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, منطقي>
{
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { أرجع obj.intersects (هذا->glyphs); }
  ساكن return_t default_return_مقدار () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  ثابت hb_مجموعه_t *glyphs;

  hb_intersects_context_t (ثابت hb_مجموعه_t *glyphs_) :
                            glyphs (glyphs_) {}
};

بنية hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, منطقي>
{
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { أرجع obj.may_have_non_1to1 (); }
  ساكن return_t default_return_مقدار () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
};

بنية hb_بستگی_context_t :
       hb_dispatch_context_t<hb_بستگی_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_بستگی_context_t *c, بیعلامت lookup_شاخص, hb_مجموعه_t *covered_seq_indicies, بیعلامت seq_شاخص, بیعلامت انتها_شاخص);
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { obj.بستگی (هذا); أرجع hb_خالی_t (); }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }
  فراغ recurse (بیعلامت lookup_شاخص, hb_مجموعه_t *covered_seq_indicies, بیعلامت seq_شاخص, بیعلامت انتها_شاخص)
  {
    إذا (unlikely (nesting_level_چپ == 0 || !recurse_func))
      أرجع;

    nesting_level_چپ--;
    recurse_func (هذا, lookup_شاخص, covered_seq_indicies, seq_شاخص, انتها_شاخص);
    nesting_level_چپ++;
  }

  فراغ reset_lookup_بازدید_شمار ()
  { lookup_شمار = 0; }

  منطقي lookup_حد_exceeded ()
  { أرجع lookup_شمار > HB_MAX_LOOKUP_بازدید_شمار; }

  منطقي should_بازدید_lookup (بیعلامت صحيح lookup_شاخص)
  {
    إذا (lookup_شمار++ > HB_MAX_LOOKUP_بازدید_شمار)
      أرجع خطأ;

    إذا (is_lookup_انجامشده (lookup_شاخص))
      أرجع خطأ;

    أرجع درست;
  }

  منطقي is_lookup_انجامشده (بیعلامت صحيح lookup_شاخص)
  {
    إذا (unlikely (انجامشده_lookups_glyph_شمار->درون_error () ||
		  انجامشده_lookups_glyph_مجموعه->درون_error ()))
      أرجع درست;

    /* Have we visited this lookup with the current set of glyphs? */
    بیعلامت برداشت = glyphs->get_population ();
    إذا (unlikely (برداشت == HB_MAP_مقدار_INVALID)) // Avoid sentinel interference.
      برداشت--;
    إذا (انجامشده_lookups_glyph_شمار->get (lookup_شاخص) != برداشت)
    {
      انجامشده_lookups_glyph_شمار->مجموعه (lookup_شاخص, برداشت);

      إذا (!انجامشده_lookups_glyph_مجموعه->has (lookup_شاخص))
      {
	إذا (unlikely (!انجامشده_lookups_glyph_مجموعه->مجموعه (lookup_شاخص, hb::unique_ptr<hb_مجموعه_t> {hb_مجموعه_create ()})))
	  أرجع درست;
      }

      انجامشده_lookups_glyph_مجموعه->get (lookup_شاخص)->clear ();
    }

    hb_مجموعه_t *covered_glyph_مجموعه = انجامشده_lookups_glyph_مجموعه->get (lookup_شاخص);
    إذا (unlikely (covered_glyph_مجموعه->درون_error ()))
      أرجع درست;
    إذا (والد_فعال_glyphs ().is_subset (*covered_glyph_مجموعه))
      أرجع درست;

    covered_glyph_مجموعه->union_ (والد_فعال_glyphs ());
    أرجع خطأ;
  }

  ثابت hb_مجموعه_t& قبلی_والد_فعال_glyphs () {
    إذا (فعال_glyphs_پشته.درازی <= 1)
      أرجع *glyphs;

    أرجع فعال_glyphs_پشته[فعال_glyphs_پشته.درازی - 2];
  }

  ثابت hb_مجموعه_t& والد_فعال_glyphs ()
  {
    إذا (!فعال_glyphs_پشته)
      أرجع *glyphs;

    أرجع فعال_glyphs_پشته.دم ();
  }

  hb_مجموعه_t* افزودن_cur_فعال_glyphs ()
  {
    hb_مجموعه_t *s = فعال_glyphs_پشته.push ();
    إذا (unlikely (فعال_glyphs_پشته.درون_error ()))
      أرجع اشاره_تهی;
    أرجع s;
  }

  منطقي برداشت_cur_انجامشده_glyphs ()
  {
    إذا (!فعال_glyphs_پشته)
      أرجع خطأ;

    فعال_glyphs_پشته.برداشت ();
    أرجع درست;
  }

  hb_face_t *face;
  hb_مجموعه_t *glyphs;
  hb_مجموعه_t output[1];
  hb_وکتور_t<hb_مجموعه_t> فعال_glyphs_پشته;
  recurse_func_t recurse_func = اشاره_تهی;
  بیعلامت صحيح nesting_level_چپ;

  hb_بستگی_context_t (hb_face_t *face_,
			hb_مجموعه_t *glyphs_,
			hb_map_t *انجامشده_lookups_glyph_شمار_2,
			hb_hashmap_t<بیعلامت, hb::unique_ptr<hb_مجموعه_t>> *انجامشده_lookups_glyph_مجموعه_2,
			بیعلامت صحيح nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_چپ (nesting_level_چپ_2),
			  انجامشده_lookups_glyph_شمار (انجامشده_lookups_glyph_شمار_2),
			  انجامشده_lookups_glyph_مجموعه (انجامشده_lookups_glyph_مجموعه_2)
  {}

  ~hb_بستگی_context_t () { flush (); }

  فراغ مجموعه_recurse_func (recurse_func_t func) { recurse_func = func; }

  فراغ flush ()
  {
    output->del_range (face->get_رقم_glyphs (), HB_مجموعه_مقدار_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    فعال_glyphs_پشته.برداشت ();
    فعال_glyphs_پشته.reset ();
  }

  خاص:
  hb_map_t *انجامشده_lookups_glyph_شمار;
  hb_hashmap_t<بیعلامت, hb::unique_ptr<hb_مجموعه_t>> *انجامشده_lookups_glyph_مجموعه;
  بیعلامت صحيح lookup_شمار = 0;
};



بنية hb_بستگی_lookups_context_t :
       hb_dispatch_context_t<hb_بستگی_lookups_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_بستگی_lookups_context_t *c, بیعلامت lookup_شاخص);
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { obj.بستگی_lookups (هذا); أرجع hb_خالی_t (); }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }
  فراغ recurse (بیعلامت lookup_شاخص)
  {
    إذا (unlikely (nesting_level_چپ == 0 || !recurse_func))
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (lookup_حد_exceeded ()
        || دیدهشده_lookups->درون_error ()
        || دیدهشده_lookups->has (lookup_شاخص))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      أرجع;

    nesting_level_چپ--;
    recurse_func (هذا, lookup_شاخص);
    nesting_level_چپ++;
  }

  فراغ مجموعه_lookup_دیدهشده (بیعلامت lookup_شاخص)
  { دیدهشده_lookups->جمع_کن (lookup_شاخص); }

  فراغ مجموعه_lookup_inactive (بیعلامت lookup_شاخص)
  { inactive_lookups->جمع_کن (lookup_شاخص); }

  منطقي lookup_حد_exceeded ()
  {
    منطقي ret = lookup_شمار > HB_MAX_LOOKUP_بازدید_شمار;
    إذا (ret)
      DEBUG_پیام (SUBSET, اشاره_تهی, "lookup visit count limit exceeded in lookup closure!");
    أرجع ret; }

  منطقي is_lookup_دیدهشده (بیعلامت lookup_شاخص)
  {
    إذا (unlikely (lookup_شمار++ > HB_MAX_LOOKUP_بازدید_شمار))
    {
      DEBUG_پیام (SUBSET, اشاره_تهی, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_شمار, lookup_شاخص);
      أرجع درست;
    }

    إذا (unlikely (دیدهشده_lookups->درون_error ()))
      أرجع درست;

    أرجع دیدهشده_lookups->has (lookup_شاخص);
  }

  hb_face_t *face;
  ثابت hb_مجموعه_t *glyphs;
  recurse_func_t recurse_func;
  بیعلامت صحيح nesting_level_چپ;

  hb_بستگی_lookups_context_t (hb_face_t *face_,
				ثابت hb_مجموعه_t *glyphs_,
				hb_مجموعه_t *دیدهشده_lookups_2,
				hb_مجموعه_t *inactive_lookups_,
				بیعلامت nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (اشاره_تهی),
				nesting_level_چپ (nesting_level_چپ_2),
				دیدهشده_lookups (دیدهشده_lookups_2),
				inactive_lookups (inactive_lookups_),
				lookup_شمار (0) {}

  فراغ مجموعه_recurse_func (recurse_func_t func) { recurse_func = func; }

  خاص:
  hb_مجموعه_t *دیدهشده_lookups;
  hb_مجموعه_t *inactive_lookups;
  بیعلامت صحيح lookup_شمار;
};

بنية hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, منطقي>
{
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { أرجع obj.would_apply (هذا); }
  ساكن return_t default_return_مقدار () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  hb_face_t *face;
  ثابت hb_codepoint_t *glyphs;
  بیعلامت صحيح len;
  منطقي zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ثابت hb_codepoint_t *glyphs_,
			    بیعلامت صحيح len_,
			    منطقي zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

بنية hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, بیعلامت صحيح lookup_شاخص);
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { obj.collect_glyphs (هذا); أرجع hb_خالی_t (); }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }
  فراغ recurse (بیعلامت صحيح lookup_شاخص)
  {
    إذا (unlikely (nesting_level_چپ == 0 || !recurse_func))
      أرجع;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    إذا (output == hb_مجموعه_get_خالی ())
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (recursed_lookups->has (lookup_شاخص))
      أرجع;

    hb_مجموعه_t *old_before = before;
    hb_مجموعه_t *old_درون_داد  = درون_داد;
    hb_مجموعه_t *old_after  = after;
    before = درون_داد = after = hb_مجموعه_get_خالی ();

    nesting_level_چپ--;
    recurse_func (هذا, lookup_شاخص);
    nesting_level_چپ++;

    before = old_before;
    درون_داد  = old_درون_داد;
    after  = old_after;

    recursed_lookups->جمع_کن (lookup_شاخص);
  }

  hb_face_t *face;
  hb_مجموعه_t *before;
  hb_مجموعه_t *درون_داد;
  hb_مجموعه_t *after;
  hb_مجموعه_t *output;
  recurse_func_t recurse_func;
  hb_مجموعه_t *recursed_lookups;
  بیعلامت صحيح nesting_level_چپ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_مجموعه_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_مجموعه_t  *glyphs_درون_داد,  /* OUT.  May be NULL */
			       hb_مجموعه_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_مجموعه_t  *glyphs_output, /* OUT.  May be NULL */
			       بیعلامت صحيح nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_مجموعه_get_خالی ()),
			      درون_داد  (glyphs_درون_داد  ? glyphs_درون_داد  : hb_مجموعه_get_خالی ()),
			      after  (glyphs_after  ? glyphs_after  : hb_مجموعه_get_خالی ()),
			      output (glyphs_output ? glyphs_output : hb_مجموعه_get_خالی ()),
			      recurse_func (اشاره_تهی),
			      recursed_lookups (hb_مجموعه_create ()),
			      nesting_level_چپ (nesting_level_چپ_2) {}
  ~hb_collect_glyphs_context_t () { hb_مجموعه_destroy (recursed_lookups); }

  فراغ مجموعه_recurse_func (recurse_func_t func) { recurse_func = func; }
};

بنية hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  تعداد glyph_مجموعه_type_t
  {
    GLYPH_مجموعه_CLASS,
    GLYPH_مجموعه_COVERAGE,
    GLYPH_مجموعه_SINGLE
  };

  ساكن constexpr hb_codepoint_t SINGLE_GLYPH_فعال_FLAG = 0x80000000u;

  بنية glyph_مجموعه_cache_کلید_t
  {
    glyph_مجموعه_cache_کلید_t () = افتراضي;
    glyph_مجموعه_cache_کلید_t (ثابت فراغ *داده_2, بیعلامت مقدار_3, glyph_مجموعه_type_t type_)
      : داده (داده_2), value (مقدار_3), type (type_) {}

    منطقي عملگر == (ثابت glyph_مجموعه_cache_کلید_t &o) ثابت
    { أرجع داده == o.داده && value == o.value && type == o.type; }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) داده);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash ((بیعلامت) type);
    }

    ثابت فراغ *داده = اشاره_تهی;
    بیعلامت value = 0;
    glyph_مجموعه_type_t type = GLYPH_مجموعه_CLASS;
  };

  بنية context_مجموعه_cache_کلید_t
  {
    context_مجموعه_cache_کلید_t () = افتراضي;
    context_مجموعه_cache_کلید_t (ثابت فراغ *قاعده,
			     ثابت فراغ *رکورد_3,
			     ثابت فراغ *داده_2,
			     hb_codepoint_t فعال_glyphs,
			     بیعلامت مقدار_3,
			     بیعلامت format_)
      : قاعده_2 (قاعده), رکورد (رکورد_3), داده (داده_2), فعال_glyphs_2 (فعال_glyphs),
	value (مقدار_3), format (format_) {}

    منطقي عملگر == (ثابت context_مجموعه_cache_کلید_t &o) ثابت
    {
      أرجع قاعده_2 == o.قاعده_2 &&
	     رکورد == o.رکورد &&
	     داده == o.داده &&
	     فعال_glyphs_2 == o.فعال_glyphs_2 &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) قاعده_2);
      current = current * 31 + hb_hash ((uintptr_t) رکورد);
      current = current * 31 + hb_hash ((uintptr_t) داده);
      current = current * 31 + hb_hash (فعال_glyphs_2);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash (format);
    }

    ثابت فراغ *قاعده_2 = اشاره_تهی;
    ثابت فراغ *رکورد = اشاره_تهی;
    ثابت فراغ *داده = اشاره_تهی;
    hb_codepoint_t فعال_glyphs_2 = HB_CODEPOINT_INVALID;
    بیعلامت value = 0;
    بیعلامت format = 0;
  };

  بنية recurse_کلید_t
  {
    recurse_کلید_t () = افتراضي;
    recurse_کلید_t (بیعلامت lookup_شاخص_3,
                   hb_codepoint_t فعال_glyphs,
                   hb_codepoint_t recurse_مسیر,
                   hb_codepoint_t context_مجموعه_2)
      : lookup_شاخص (lookup_شاخص_3),
        فعال_glyphs_2 (فعال_glyphs),
        recurse_مسیر_2 (recurse_مسیر),
        context_مجموعه (context_مجموعه_2) {}

    منطقي عملگر == (ثابت recurse_کلید_t &o) ثابت
    {
      أرجع lookup_شاخص == o.lookup_شاخص &&
             فعال_glyphs_2 == o.فعال_glyphs_2 &&
             recurse_مسیر_2 == o.recurse_مسیر_2 &&
             context_مجموعه == o.context_مجموعه;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_شاخص)) * 16777619;
      current = (current ^ hb_hash (فعال_glyphs_2)) * 16777619;
      current = (current ^ hb_hash (recurse_مسیر_2)) * 16777619;
      current = (current ^ hb_hash (context_مجموعه)) * 16777619;
      أرجع current;
    }

    بیعلامت lookup_شاخص = 0;
    hb_codepoint_t فعال_glyphs_2 = 0;
    hb_codepoint_t recurse_مسیر_2 = 0;
    hb_codepoint_t context_مجموعه = HB_CODEPOINT_INVALID;
  };

  تعريفنوع return_t (*recurse_func_t) (hb_depend_context_t *c, بیعلامت lookup_شاخص, hb_بیت_صفحه_t *covered_seq_indices, بیعلامت seq_شاخص, بیعلامت انتها_شاخص);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { obj.depend (هذا); أرجع hb_خالی_t (); }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }

  فراغ recurse (بیعلامت lookup_idx, hb_بیت_صفحه_t *covered_seq_indices, بیعلامت seq_شاخص, بیعلامت انتها_شاخص)
  {
    /* Cycle detection using call-stack semantics: block re-entry of a
     * lookup that is currently on the execution stack, but allow it again
     * once it has returned.
     *
     * A flat "visited" set would be wrong: the same lookup legitimately
     * appears more than once when a rule iterates over multiple sequence
     * positions — each such call is independent and must be processed.
     *
     * Cutting an in-progress cycle is safe because the active glyph set
     * can only stay the same or shrink as we recurse.  If we re-encounter
     * lookup L while it is still on the stack, L is already executing
     * with a context at least as broad, so any edges a re-entry would add
     * are a subset of what L is already in the process of registering.
     *
     * lookups_seen is pre-seeded with the top-level lookup index in
     * GSUB_accelerator_t::depend before the direct depend() call, so
     * that indirect cycles of the form A→B→A are also caught here. */
    إذا (lookups_دیده.has (lookup_idx)) أرجع;
    lookups_دیده.جمع_کن (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t والد_مسیر = recurse_مسیر_2;
    uint64_t مسیر_کلید = (uint64_t) والد_مسیر << 32 | lookup_idx;
    hb_codepoint_t *مسیر = اشاره_تهی;
    hb_codepoint_t مسیر_شاخص;
    إذا (!recurse_مسیر_مقصد_شاخص.has (مسیر_کلید, &مسیر))
    {
      مسیر_شاخص = recurse_مسیر_مقصد_شاخص.get_population () + 1;
      إذا (unlikely (!recurse_مسیر_مقصد_شاخص.مجموعه (مسیر_کلید, مسیر_شاخص)))
      {
	depend_داده->fail ();
	lookups_دیده.del (lookup_idx);
	أرجع;
      }
    }
    وإلا
      مسیر_شاخص = *مسیر;
    recurse_مسیر_2 = مسیر_شاخص;
    recurse_func (هذا, lookup_idx, covered_seq_indices, seq_شاخص, انتها_شاخص);
    recurse_مسیر_2 = والد_مسیر;
    lookups_دیده.del (lookup_idx);
  }

  بنية فعال_glyphs_قاب_t
  {
    hb_مجموعه_t glyphs;
    hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
    منطقي use_ریشه = خطأ;
  };

  ثابت hb_مجموعه_t& والد_فعال_glyphs ()
  {
    إذا (!فعال_glyphs_پشته_عمق)
      أرجع *glyphs;

    ثابت فعال_glyphs_قاب_t &قاب = فعال_glyphs_پشته[فعال_glyphs_پشته_عمق - 1];
    أرجع قاب.use_ریشه ? *glyphs : قاب.glyphs;
  }

  hb_مجموعه_t* افزودن_cur_فعال_glyphs (hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID)
  {
    إذا (فعال_glyphs_پشته_عمق == فعال_glyphs_پشته.درازی &&
	unlikely (!فعال_glyphs_پشته.افزودن_or_fail ()))
      أرجع اشاره_تهی;

    فعال_glyphs_قاب_t &قاب = فعال_glyphs_پشته[فعال_glyphs_پشته_عمق++];
    قاب.glyphs.clear ();
    قاب.فعال_idx = فعال_idx;
    قاب.use_ریشه = خطأ;
    أرجع &قاب.glyphs;
  }

  منطقي افزودن_ریشه_فعال_glyphs ()
  {
    إذا (فعال_glyphs_پشته_عمق == فعال_glyphs_پشته.درازی &&
	unlikely (!فعال_glyphs_پشته.افزودن_or_fail ()))
      أرجع خطأ;

    فعال_glyphs_پشته[فعال_glyphs_پشته_عمق++].use_ریشه = درست;
    أرجع درست;
  }

  منطقي برداشت_cur_انجامشده_glyphs ()
  {
    إذا (!فعال_glyphs_پشته_عمق)
      أرجع خطأ;

    فعال_glyphs_پشته_عمق--;
    أرجع درست;
  }

  منطقي get_والد_فعال_glyphs_شاخص (hb_codepoint_t *فعال_idx)
  {
    إذا (!فعال_glyphs_پشته_عمق ||
	فعال_glyphs_پشته[فعال_glyphs_پشته_عمق - 1].use_ریشه)
    {
      *فعال_idx = HB_CODEPOINT_INVALID;
      أرجع درست;
    }

    فعال_glyphs_قاب_t &قاب = فعال_glyphs_پشته[فعال_glyphs_پشته_عمق - 1];
    *فعال_idx = قاب.فعال_idx;
    إذا (*فعال_idx != HB_CODEPOINT_INVALID)
      أرجع درست;

    hb_codepoint_t glyph;
    إذا (قاب.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_فعال_FLAG))
      *فعال_idx = glyph | SINGLE_GLYPH_فعال_FLAG;
    وإلا إذا (!پیدا_کن_or_create_recurse_مجموعه (قاب.glyphs, فعال_idx))
      أرجع خطأ;

    قاب.فعال_idx = *فعال_idx;
    أرجع درست;
  }

  منطقي get_context_مجموعه_cache_کلید (ثابت فراغ *قاعده_2,
				  ثابت فراغ *رکورد,
				  ثابت فراغ *داده,
				  بیعلامت value,
				  بیعلامت format,
				  context_مجموعه_cache_کلید_t *key)
  {
    hb_codepoint_t فعال_idx;
    إذا (!get_والد_فعال_glyphs_شاخص (&فعال_idx))
      أرجع خطأ;
    *key = context_مجموعه_cache_کلید_t (قاعده_2, رکورد, داده, فعال_idx, value, format);
    أرجع درست;
  }

  منطقي get_recurse_کلید (بیعلامت lookup_idx, recurse_کلید_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t فعال_idx;
    إذا (!get_والد_فعال_glyphs_شاخص (&فعال_idx))
      أرجع خطأ;

    *key = recurse_کلید_t (lookup_idx,
                          فعال_idx,
                          recurse_مسیر_2,
                          depend_داده->current_context_مجموعه_شاخص);
    أرجع !completed_recursions.has (*key);
  }

  فراغ پایان_recurse (ثابت recurse_کلید_t &key)
  { depend_داده->check_success (completed_recursions.مجموعه (key, درست)); }

  فراغ reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_مسیر_مقصد_شاخص.clear ();
    recurse_مسیر_2 = 0;
  }

  منطقي پیدا_کن_or_create_recurse_مجموعه (ثابت hb_مجموعه_t &مجموعه, hb_codepoint_t *شاخص_2)
  {
    hb_codepoint_t *existing = اشاره_تهی;
    إذا (recurse_مجموعه_مقصد_شاخص.has (&مجموعه, &existing))
    {
      *شاخص_2 = *existing;
      أرجع درست;
    }

    hb_مجموعه_t *copy = hb_object_create<hb_مجموعه_t> ();
    إذا (unlikely (!copy))
      أرجع depend_داده->fail ();
    copy->مجموعه (مجموعه);
    إذا (unlikely (copy->درون_error ()))
    {
      hb_مجموعه_destroy (copy);
      أرجع depend_داده->fail ();
    }

    *شاخص_2 = recurse_sets.درازی;
    إذا (unlikely (*شاخص_2 & SINGLE_GLYPH_فعال_FLAG))
    {
      hb_مجموعه_destroy (copy);
      أرجع depend_داده->fail ();
    }
    إذا (unlikely (!recurse_sets.افزودن_or_fail (hb::unique_ptr<hb_مجموعه_t> {copy})))
      أرجع depend_داده->fail ();
    إذا (unlikely (!recurse_مجموعه_مقصد_شاخص.مجموعه (recurse_sets[*شاخص_2].get (), *شاخص_2)))
      أرجع depend_داده->fail ();
    أرجع درست;
  }

  ثابت hb_مجموعه_t *پیدا_کن_or_create_glyph_مجموعه (ثابت فراغ *داده,
                                             بیعلامت value,
                                             glyph_مجموعه_type_t type);
  hb_codepoint_t پیدا_کن_or_create_cached_context_مجموعه (ثابت hb_مجموعه_t *مجموعه);

  بنية depend_scratch_t
  {
    فراغ clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      درون_داد_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_مجموعه_t preliminary_context;
    hb_مجموعه_t position_scratch;
    hb_مجموعه_t pos_glyphs;
    hb_مجموعه_t position_context;
    hb_مجموعه_t disjunctive_indices;
    hb_مجموعه_t filtered_disjunctive_indices;
    hb_وکتور_t<ثابت hb_مجموعه_t *> backtrack_sets;
    hb_وکتور_t<ثابت hb_مجموعه_t *> lookahead_sets;
    hb_وکتور_t<ثابت hb_مجموعه_t *> درون_داد_position_glyphs;
    hb_وکتور_t<hb_codepoint_t> cached_context_sets;
    hb_وکتور_t<بیعلامت> compact_workspace;
    depend_scratch_t *next = اشاره_تهی;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    إذا (scratch)
      depend_scratch_pool = scratch->next;
    وإلا
    {
      scratch = (depend_scratch_t *) hb_malloc (حجم (depend_scratch_t));
      إذا (unlikely (!scratch))
      {
        depend_داده->fail ();
        أرجع اشاره_تهی;
      }
      جديد (scratch) depend_scratch_t ();
    }

    scratch->next = اشاره_تهی;
    scratch->clear ();
    أرجع scratch;
  }

  فراغ release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_داده_builder_t *depend_داده;
  hb_face_t *face;
  hb_مجموعه_t *glyphs;
  hb_وکتور_t<فعال_glyphs_قاب_t> فعال_glyphs_پشته;
  بیعلامت فعال_glyphs_پشته_عمق = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_شاخص = HB_CODEPOINT_INVALID;
  hb_بیت_مجموعه_t lookups_دیده;
  hb_وکتور_t<hb::unique_ptr<hb_مجموعه_t>> recurse_sets;
  hb_hashmap_t<ثابت hb_مجموعه_t *, hb_codepoint_t> recurse_مجموعه_مقصد_شاخص;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_مسیر_مقصد_شاخص;
  hb_codepoint_t recurse_مسیر_2 = 0;
  hb_hashmap_t<recurse_کلید_t, منطقي> completed_recursions;
  hb_وکتور_t<hb::unique_ptr<hb_مجموعه_t>> glyph_sets;
  hb_hashmap_t<glyph_مجموعه_cache_کلید_t, hb_codepoint_t> glyph_مجموعه_مقصد_شاخص;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_مجموعه_indices;
  hb_hashmap_t<context_مجموعه_cache_کلید_t, hb_codepoint_t> context_مجموعه_cache;
  depend_scratch_t *depend_scratch_pool = اشاره_تهی;

  hb_depend_context_t (hb_depend_داده_builder_t *depend_داده_2,
                       hb_face_t *face_,
                       hb_مجموعه_t *glyphs_)
                      : depend_داده (depend_داده_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (اشاره_تهی) {}
  ~hb_depend_context_t ()
  {
    طالما (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  فراغ مجموعه_recurse_func (recurse_func_t func) { recurse_func = func; }
};

قالب <نام_نوع مجموعه_t>
بنية hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<مجموعه_t>, ثابت Coverage &>
{
  تعريفنوع ثابت Coverage &return_t; // Stoopid that we have to dupe this here.
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { أرجع obj.get_coverage (); }
  ساكن return_t default_return_مقدار () { أرجع Null (Coverage); }
  منطقي stop_sublookup_iteration (return_t r) ثابت
  {
    r.collect_coverage (مجموعه);
    أرجع خطأ;
  }

  hb_collect_coverage_context_t (مجموعه_t *مجموعه_2) :
				   مجموعه (مجموعه_2) {}

  مجموعه_t *مجموعه;
};

بنية matcher_t
{
  تعريفنوع منطقي (*تطابق_func_t) (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده);

  قالب <نام_نوع context_t>
  فراغ init (ثابت context_t *c, منطقي context_تطابق = خطأ)
  {
    مجموعه_تطابق_func (اشاره_تهی, اشاره_تهی);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->جدول_شاخص == 1 || (context_تطابق && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_تطابق || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->جدول_شاخص == 1;
    ماسک = context_تطابق ? -1 : c->lookup_ماسک;
    /* Per syllable matching is only for GSUB. */
    در_هر_syllable = c->جدول_شاخص == 0 && c->در_هر_syllable;
    syllable = 0;
  }

  فراغ مجموعه_تطابق_func (تطابق_func_t تطابق_func,
		       ثابت فراغ *تطابق_داده)
  { تطابق_func_2 = تطابق_func; تطابق_داده_2 = تطابق_داده; }

  تعداد may_تطابق_t {
    تطابق_NO,
    تطابق_YES,
    تطابق_MAYBE
  };

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  may_تطابق_t may_تطابق (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_داده) ثابت
  {
    إذا (!(info.ماسک & ماسک) ||
	(در_هر_syllable && syllable && syllable != info.syllable ()))
      أرجع تطابق_NO;

    إذا (تطابق_func_2)
      أرجع تطابق_func_2 (info, glyph_داده, تطابق_داده_2) ? تطابق_YES : تطابق_NO;

    أرجع تطابق_MAYBE;
  }

  تعداد may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  قالب <نام_نوع context_t>
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  may_skip_t may_skip (ثابت context_t *c,
		       ثابت hb_glyph_info_t &info) ثابت
  {
    إذا (!c->check_glyph_property (&info, lookup_props))
      أرجع SKIP_YES;

    إذا (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      أرجع SKIP_MAYBE;

    أرجع SKIP_NO;
  }

  عام:
  بیعلامت صحيح lookup_props = 0;
  hb_ماسک_t ماسک = -1;
  منطقي ignore_zwnj = خطأ;
  منطقي ignore_zwj = خطأ;
  منطقي ignore_hidden = خطأ;
  منطقي در_هر_syllable = خطأ;
  uint8_t syllable = 0;
  تطابق_func_t تطابق_func_2 = اشاره_تهی;
  ثابت فراغ *تطابق_داده_2 = اشاره_تهی;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
بنية glyph_داده_t
{
  قالب <نام_نوع T>
  glyph_داده_t (ثابت T *داده_2) : داده (داده_2)
#اگر_تعریف_نشده HB_NO_BEYOND_64K
                              , wide (حجم (T) == 3)
#پایان_اگر
  {
    static_assert (حجم (T) == 2 || حجم (T) == 3, "invalid glyph data size");
  }

  بیعلامت عملگر [] (بیعلامت i) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (wide)
      أرجع ((ثابت HBUINT24 *) داده)[i];
#پایان_اگر
    أرجع ((ثابت HBUINT16 *) داده)[i];
  }

  ثابت فراغ *داده;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  منطقي wide;
#پایان_اگر
};

قالب <نام_نوع context_t>
بنية skipping_iterator_t
{
  فراغ init (context_t *c_, منطقي context_تطابق = خطأ)
  {
    c = c_;
    انتها = c->میانگیر->len;
    تطابق_glyph_data16 = اشاره_تهی;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    تطابق_glyph_data24 = اشاره_تهی;
#پایان_اگر
    matcher.init (c, context_تطابق);
  }
  فراغ مجموعه_lookup_props (بیعلامت صحيح lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  فراغ مجموعه_تطابق_func (matcher_t::تطابق_func_t تطابق_func,
		       ثابت فراغ *تطابق_داده)
  {
    matcher.مجموعه_تطابق_func (تطابق_func, تطابق_داده);
  }
  فراغ مجموعه_glyph_داده (glyph_داده_t glyph_داده)
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    تطابق_glyph_data16 = glyph_داده.wide ? اشاره_تهی : (ثابت HBUINT16 *) glyph_داده.داده;
    تطابق_glyph_data24 = glyph_داده.wide ? (ثابت HBUINT24 *) glyph_داده.داده : اشاره_تهی;
#وإلا
    تطابق_glyph_data16 = (ثابت HBUINT16 *) glyph_داده.داده;
#پایان_اگر
  }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  فراغ reset (بیعلامت صحيح شروع_شاخص_2)
  {
    // For GSUB forward iterator
    idx = شروع_شاخص_2;
    انتها = c->میانگیر->len;
    matcher.syllable = c->میانگیر->cur().syllable();
  }
  فراغ reset_پشت (بیعلامت صحيح شروع_شاخص_2, منطقي from_بیرون_میانگیر = خطأ)
  {
    // For GSUB backward iterator
    idx = شروع_شاخص_2;
    matcher.syllable = c->میانگیر->cur().syllable();
  }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  فراغ reset_fast (بیعلامت صحيح شروع_شاخص_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = شروع_شاخص_2;
  }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  matcher_t::may_skip_t may_skip (ثابت hb_glyph_info_t &info) ثابت
  { أرجع matcher.may_skip (c, info); }

  تعداد تطابق_t {
    تطابق_2,
    NOT_تطابق,
    SKIP
  };

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  تطابق_t تطابق (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    إذا (unlikely (skip == matcher_t::SKIP_YES))
      أرجع SKIP;

    matcher_t::may_تطابق_t تطابق = matcher.may_تطابق (info, get_glyph_داده ());
    إذا (تطابق == matcher_t::تطابق_YES ||
	(تطابق == matcher_t::تطابق_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      أرجع تطابق_2;

    إذا (skip == matcher_t::SKIP_NO)
      أرجع NOT_تطابق;

    أرجع SKIP;
  }

  منطقي next (بیعلامت *unsafe_مقصد = اشاره_تهی)
  {
    خودکار *info = c->میانگیر->info;
    ثابت علامتدار stop = (علامتدار) انتها - 1;
    طالما ((علامتدار) idx < stop)
    {
      idx++;
      بدل (تطابق (info[idx]))
      {
	حالة تطابق_2:
	{
	  advance_glyph_داده ();
	  أرجع درست;
	}
	حالة NOT_تطابق:
	{
	  إذا (unsafe_مقصد)
	    *unsafe_مقصد = idx + 1;
	  أرجع خطأ;
	}
	حالة SKIP:
	  استمر;
      }
    }
    إذا (unsafe_مقصد)
      *unsafe_مقصد = انتها;
    أرجع خطأ;
  }
  منطقي prev (بیعلامت *unsafe_from = اشاره_تهی)
  {
    خودکار *بیرون_info = c->میانگیر->بیرون_info;
    ثابت بیعلامت stop = 0;
    طالما (idx > stop)
    {
      idx--;
      بدل (تطابق (بیرون_info[idx]))
      {
	حالة تطابق_2:
	{
	  advance_glyph_داده ();
	  أرجع درست;
	}
	حالة NOT_تطابق:
	{
	  إذا (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  أرجع خطأ;
	}
	حالة SKIP:
	  استمر;
      }
    }
    إذا (unsafe_from)
      *unsafe_from = 0;
    أرجع خطأ;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_داده ()
  {
    إذا (تطابق_glyph_data16) أرجع *تطابق_glyph_data16;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    وإلا
    إذا (تطابق_glyph_data24) أرجع *تطابق_glyph_data24;
#پایان_اگر
    أرجع 0;
  }
  HB_ALWAYS_INLINE
  فراغ
  advance_glyph_داده ()
  {
    إذا (تطابق_glyph_data16) تطابق_glyph_data16++;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    وإلا
    إذا (تطابق_glyph_data24) تطابق_glyph_data24++;
#پایان_اگر
  }

  بیعلامت صحيح idx;
  محمي:
  context_t *c;
  matcher_t matcher;
  ثابت HBUINT16 *تطابق_glyph_data16;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  ثابت HBUINT24 *تطابق_glyph_data24;
#پایان_اگر

  بیعلامت صحيح انتها;
};

بنية hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  تعريفنوع return_t (*recurse_func_t) (hb_ot_apply_context_t *c, بیعلامت صحيح lookup_شاخص);

  قالب <نام_نوع T>
  ساكن مضمن خودکار apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_اولویت<1>) HB_RETURN (return_t, obj.apply (c, اشاره_تهی) )
  قالب <نام_نوع T>
  ساكن مضمن خودکار apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_اولویت<0>) HB_RETURN (return_t, obj.apply (c) )
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { أرجع apply_(obj, هذا, hb_prioritize); }

  ساكن return_t default_return_مقدار () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
  return_t recurse (بیعلامت صحيح sub_lookup_شاخص)
  {
    أكد (recurse_func);
    إذا (unlikely (nesting_level_چپ == 0))
    {
      میانگیر->successful = خطأ;
      أرجع default_return_مقدار ();
    }

    میانگیر->max_ops--;
    إذا (unlikely (میانگیر->max_ops < 0))
    {
      میانگیر->successful = خطأ;
      أرجع default_return_مقدار ();
    }

    nesting_level_چپ--;
    منطقي ret = recurse_func (هذا, sub_lookup_شاخص);
    nesting_level_چپ++;
    أرجع ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_درون_داد, iter_context;

  بیعلامت صحيح جدول_شاخص; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_میانگیر_t *میانگیر;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = اشاره_تهی;
  ثابت GDEF &gdef;
  ثابت GDEF::accelerator_t &gdef_accel;
  ثابت قلم_Variation_Store &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_ماسک_t lookup_ماسک = 1;
  بیعلامت صحيح lookup_شاخص = (بیعلامت) -1;
  بیعلامت صحيح lookup_props = 0;
  بیعلامت صحيح nesting_level_چپ = HB_MAX_NESTING_LEVEL;

  منطقي has_glyph_classes;
  منطقي auto_zwnj = درست;
  منطقي auto_zwj = درست;
  منطقي در_هر_syllable = خطأ;
  منطقي random = خطأ;
  بیعلامت new_syllables = (بیعلامت) -1;

  علامتدار آخرین_پایه = -1; // GPOS uses
  بیعلامت آخرین_پایه_until = 0; // GPOS uses

  hb_وکتور_t<uint32_t> تطابق_positions;
  uint32_t پشته_تطابق_positions[8];
#اگر_تعریف_نشده HB_NO_میانگیر_پیام
  hb_میانگیر_t::تغییریافته_func_t orig_تغییریافته_func = اشاره_تهی;
  فراغ *orig_تغییریافته_داده = اشاره_تهی;
#پایان_اگر

  hb_ot_apply_context_t (بیعلامت صحيح جدول_شاخص_2,
			 hb_font_t *font_,
			 hb_میانگیر_t *میانگیر_2,
			 hb_blob_t *جدول_blob_2,
			 hb_scalar_cache_t *var_store_cache_ = اشاره_تهی) :
			جدول_شاخص (جدول_شاخص_2),
			font (font_), face (font->face), میانگیر (میانگیر_2),
			sanitizer (جدول_blob_2),
			gdef (
#اگر_تعریف_نشده HB_NO_OT_LAYOUT
			      *face->جدول.GDEF->جدول
#وإلا
			      Null (GDEF)
#پایان_اگر
			     ),
			gdef_accel (
#اگر_تعریف_نشده HB_NO_OT_LAYOUT
			      *face->جدول.GDEF
#وإلا
			      Null (GDEF::accelerator_t)
#پایان_اگر
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (میانگیر_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    تطابق_positions.مجموعه_storage (پشته_تطابق_positions);

#اگر_تعریف_نشده HB_NO_میانگیر_پیام
    إذا (میانگیر->messaging ())
    {
      أكد (میانگیر->تغییریافته_func == اشاره_تهی ||
	      میانگیر->تغییریافته_func == میانگیر_تغییریافته_trampoline);
      orig_تغییریافته_func = میانگیر->تغییریافته_func;
      orig_تغییریافته_داده = میانگیر->تغییریافته_داده;
      میانگیر->تغییریافته_func = میانگیر_تغییریافته_trampoline;
      میانگیر->تغییریافته_داده = هذا;
    }
#پایان_اگر
  }
  ~hb_ot_apply_context_t ()
  {
#اگر_تعریف_نشده HB_NO_میانگیر_پیام
    إذا (میانگیر->messaging ())
    {
      أكد (میانگیر->تغییریافته_func == میانگیر_تغییریافته_trampoline);
      أكد (میانگیر->تغییریافته_داده == هذا);
      میانگیر->تغییریافته_func = orig_تغییریافته_func;
      میانگیر->تغییریافته_داده = orig_تغییریافته_داده;
    }
#پایان_اگر
  }

  فراغ init_iters ()
  {
    iter_درون_داد.init (هذا, خطأ);
    iter_context.init (هذا, درست);
  }

  فراغ مجموعه_lookup_ماسک (hb_ماسک_t ماسک, منطقي init = درست) { lookup_ماسک = ماسک; آخرین_پایه = -1; آخرین_پایه_until = 0; إذا (init) init_iters (); }
  فراغ مجموعه_auto_zwj (منطقي auto_zwj_, منطقي init = درست) { auto_zwj = auto_zwj_; إذا (init) init_iters (); }
  فراغ مجموعه_auto_zwnj (منطقي auto_zwnj_, منطقي init = درست) { auto_zwnj = auto_zwnj_; إذا (init) init_iters (); }
  فراغ مجموعه_در_هر_syllable (منطقي در_هر_syllable_2, منطقي init = درست) { در_هر_syllable = در_هر_syllable_2; إذا (init) init_iters (); }
  فراغ مجموعه_random (منطقي random_) { random = random_; }
  فراغ مجموعه_recurse_func (recurse_func_t func) { recurse_func = func; }
  فراغ مجموعه_lookup_شاخص (بیعلامت صحيح lookup_شاخص_3) { lookup_شاخص = lookup_شاخص_3; }
  فراغ مجموعه_lookup_props (بیعلامت صحيح lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_عدد ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    میانگیر->random_state = میانگیر->random_state * 48271 % 2147483647;
    أرجع میانگیر->random_state;
  }

  ساكن فراغ میانگیر_تغییریافته_trampoline (hb_میانگیر_t *میانگیر HB_استفادهنشده, فراغ *user_داده)
  {
    ((hb_ot_apply_context_t *) user_داده)->میانگیر_تغییریافته ();
  }
  فراغ میانگیر_تغییریافته ()
  {
    میانگیر->update_digest ();
    إذا (likely (has_glyph_classes))
      لكل (بیعلامت i = 0; i < میانگیر->len; i++)
	مجموعه_glyph_class_props (میانگیر->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  منطقي تطابق_properties_mark (ثابت hb_glyph_info_t *info,
			      بیعلامت صحيح    glyph_props,
			      بیعلامت صحيح    تطابق_props) ثابت
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    إذا (تطابق_props & LookupFlag::Use_Mark_Filtering_مجموعه)
    {
      بیعلامت مجموعه_شاخص = تطابق_props >> 16;
      أرجع gdef_accel.mark_مجموعه_covers (مجموعه_شاخص, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    إذا (تطابق_props & LookupFlag::MarkAttachmentType)
      أرجع (تطابق_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    أرجع درست;
  }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  منطقي check_glyph_property (ثابت hb_glyph_info_t *info,
			     بیعلامت تطابق_props) ثابت
  {
    بیعلامت صحيح glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    إذا (glyph_props & تطابق_props & LookupFlag::IgnoreFlags)
      أرجع خطأ;

    إذا (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      أرجع تطابق_properties_mark (info, glyph_props, تطابق_props);

    أرجع درست;
  }

  فراغ مجموعه_glyph_class (hb_codepoint_t glyph_شاخص,
			  بیعلامت صحيح class_guess = 0,
			  منطقي ligature = خطأ,
			  منطقي component = خطأ)
  {
    میانگیر->digest.جمع_کن (glyph_شاخص);

    إذا (new_syllables != (بیعلامت) -1)
      میانگیر->cur().syllable() = new_syllables;

    بیعلامت صحيح props = _hb_glyph_info_get_glyph_props (&میانگیر->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    إذا (ligature)
    {
      props |= HB_OT_LAYOUT_GLYPH_PROPS_LIGATED;
      /* In the only place that the MULTIPLIED bit is used, Uniscribe
       * seems to only care about the "last" transformation between
       * Ligature and Multiple substitutions.  Ie. if you ligate, expand,
       * and ligate again, it forgives the multiplication and acts as
       * if only ligation happened.  As such, clear MULTIPLIED bit.
       */
      props &= ~HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    }
    إذا (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    إذا (likely (has_glyph_classes))
    {
      مجموعه_glyph_class_props (میانگیر->cur(), props, glyph_شاخص);
    }
    وإلا إذا (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      hb_glyph_info_مجموعه_glyph_props (&میانگیر->cur(), props | class_guess);
    }
    وإلا
      hb_glyph_info_مجموعه_glyph_props (&میانگیر->cur(), props);
  }
  فراغ مجموعه_glyph_class_props (hb_glyph_info_t &info,
			       بیعلامت صحيح props,
			       hb_codepoint_t glyph_شاخص)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    hb_glyph_info_مجموعه_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_شاخص));
  }
  فراغ مجموعه_glyph_class_props (hb_glyph_info_t &info)
  {
    مجموعه_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  فراغ replace_glyph (hb_codepoint_t glyph_شاخص)
  {
    مجموعه_glyph_class (glyph_شاخص);
    (فراغ) میانگیر->replace_glyph (glyph_شاخص);
  }
  فراغ replace_glyph_inplace (hb_codepoint_t glyph_شاخص)
  {
    مجموعه_glyph_class (glyph_شاخص);
    میانگیر->cur().codepoint = glyph_شاخص;
  }
  فراغ replace_glyph_with_ligature (hb_codepoint_t glyph_شاخص,
				    بیعلامت صحيح class_guess)
  {
    مجموعه_glyph_class (glyph_شاخص, class_guess, درست);
    (فراغ) میانگیر->replace_glyph (glyph_شاخص);
  }
  فراغ output_glyph_for_component (hb_codepoint_t glyph_شاخص,
				   بیعلامت صحيح class_guess)
  {
    مجموعه_glyph_class (glyph_شاخص, class_guess, خطأ, درست);
    (فراغ) میانگیر->output_glyph (glyph_شاخص);
  }
};

تعداد صنف hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

بنية hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  قالب <نام_نوع T>
  ساكن مضمن خودکار collect_دومی_glyphs_2 (ثابت T &obj,
					     hb_مجموعه_digest_t *digest,
					     hb_اولویت<1>) HB_RETURN (فراغ, obj.collect_دومی_glyphs (digest) )
  قالب <نام_نوع T>
  ساكن مضمن فراغ collect_دومی_glyphs_2 (ثابت T &obj HB_استفادهنشده,
					     hb_مجموعه_digest_t *digest,
					     hb_اولویت<0>)
  { *digest = hb_مجموعه_digest_t::پر (); }

  قالب <نام_نوع T>
  ساكن مضمن خودکار apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_اولویت<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  قالب <نام_نوع T>
  ساكن مضمن خودکار apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_اولویت<0>) HB_RETURN (منطقي, obj->apply (c) )
  قالب <نام_نوع T>
  ساكن مضمن منطقي apply_مقصد (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
  قالب <نام_نوع T>
  ساكن مضمن خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_اولویت<2>) HB_RETURN (منطقي, obj->apply_cached (c, external_cache) )
  قالب <نام_نوع T>
  ساكن مضمن خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_اولویت<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  قالب <نام_نوع T>
  ساكن مضمن خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_اولویت<0>) HB_RETURN (منطقي, obj->apply (c) )
  قالب <نام_نوع T>
  ساكن مضمن منطقي apply_cached_مقصد (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  قالب <نام_نوع T>
  ساكن مضمن خودکار cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_اولویت<1>) HB_RETURN (منطقي, T::cache_func (c, op) )
  قالب <نام_نوع T=فراغ>
  ساكن مضمن منطقي cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_استفادهنشده,
				  hb_اولویت<0>) { أرجع خطأ; }
  قالب <نام_نوع Type>
  ساكن مضمن منطقي cache_func_مقصد (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    أرجع cache_func_<Type> (c, op, hb_prioritize);
  }
#پایان_اگر

  تعريفنوع منطقي (*hb_apply_func_t) (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache);
  تعريفنوع منطقي (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  بنية hb_applicable_t
  {
    دوست بنية hb_accelerate_subtables_context_t;
    دوست بنية hb_ot_layout_lookup_accelerator_t;

    قالب <نام_نوع T>
    فراغ init (ثابت T &obj_,
	       hb_apply_func_t apply_func_
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , فراغ *external_cache_
#پایان_اگر
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#پایان_اگر
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#اگر_تعریف_شده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    منطقي apply (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->میانگیر->cur().codepoint) && apply_func (obj, c, اشاره_تهی);
    }
    منطقي apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      أرجع apply_func (obj, c, اشاره_تهی);
    }
#وإلا
    منطقي apply (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->میانگیر->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    منطقي apply_cached (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->میانگیر->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    منطقي apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      أرجع apply_func (obj, c, external_cache);
    }
    منطقي apply_cached_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      أرجع apply_cached_func (obj, c, external_cache);
    }

    منطقي cache_enter (hb_ot_apply_context_t *c) ثابت
    {
      أرجع cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    فراغ cache_leave (hb_ot_apply_context_t *c) ثابت
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#پایان_اگر

    خاص:
    ثابت فراغ *obj;
    hb_apply_func_t apply_func;
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    فراغ *external_cache;
#پایان_اگر
    hb_مجموعه_digest_t digest;
  };

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
  قالب <نام_نوع T>
  خودکار cache_هزینه (ثابت T &obj, hb_اولویت<1>) HB_AUTO_RETURN ( obj.cache_هزینه () )
  قالب <نام_نوع T>
  خودکار cache_هزینه (ثابت T &obj, hb_اولویت<0>) HB_AUTO_RETURN ( 0u )

  قالب <نام_نوع T>
  خودکار external_cache_create (ثابت T &obj, hb_اولویت<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  قالب <نام_نوع T>
  خودکار external_cache_create (ثابت T &obj, hb_اولویت<0>) HB_AUTO_RETURN ( اشاره_تهی )
#پایان_اگر

  /* Dispatch interface. */
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj)
  {
    collect_دومی_glyphs_2 (obj, &digest_دومی, hb_prioritize);

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    فراغ *external_cache = اشاره_تهی;
    إذا (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#پایان_اگر

    hb_applicable_t *مدخل = &array[i++];

    مدخل->init (obj,
		 apply_مقصد<T>
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_مقصد<T>
		 , cache_func_مقصد<T>
		 , external_cache
#پایان_اگر
		 );

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    بیعلامت هزینه = cache_هزینه (obj, hb_prioritize);
    إذا (هزینه > subtable_cache_user_هزینه)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_هزینه = هزینه;
    }
#پایان_اگر

    أرجع hb_خالی_t ();
  }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  بیعلامت i = 0;
  hb_مجموعه_digest_t digest_دومی;

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
  بیعلامت subtable_cache_user_idx = (بیعلامت) -1;
  بیعلامت subtable_cache_user_هزینه = 0;
#پایان_اگر
};


تعريفنوع منطقي (*intersects_func_t) (ثابت hb_مجموعه_t *glyphs, بیعلامت value, ثابت فراغ *داده, فراغ *cache);
تعريفنوع فراغ (*intersected_glyphs_func_t) (ثابت hb_مجموعه_t *glyphs, ثابت فراغ *داده, بیعلامت value, hb_مجموعه_t *intersected_glyphs, فراغ *cache);
تعريفنوع فراغ (*collect_glyphs_func_t) (hb_مجموعه_t *glyphs, بیعلامت value, ثابت فراغ *داده);
تعريفنوع منطقي (*تطابق_func_t) (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده);
تعريفنوع بیعلامت (*get_مقدار_func_t) (hb_glyph_info_t &info, ثابت فراغ *داده);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
بنية hb_ot_layout_ruleset_digest_t
{
  فراغ init () { ماسک = 0; }
  فراغ مجموعه_پر () { ماسک = (uint64_t) -1; }
  فراغ جمع_کن (بیعلامت value) { ماسک |= (uint64_t) 1 << (value & 63); }
  منطقي may_have (بیعلامت value) ثابت { أرجع ماسک & ((uint64_t) 1 << (value & 63)); }

  uint64_t ماسک;
};

بنية Context_بستگی_Funcs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
بنية ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
بنية ContextApplyFuncs
{
  تطابق_func_t تطابق;
};
بنية ChainContextApplyFuncs
{
  تطابق_func_t تطابق[3];
};


ساكن مضمن منطقي intersects_glyph (ثابت hb_مجموعه_t *glyphs, بیعلامت value, ثابت فراغ *داده HB_استفادهنشده, فراغ *cache HB_استفادهنشده)
{
  أرجع glyphs->has (value);
}
ساكن مضمن منطقي intersects_class (ثابت hb_مجموعه_t *glyphs, بیعلامت value, ثابت فراغ *داده, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(داده);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  إذا (map->has (value, &cached_v))
    أرجع *cached_v;

  منطقي v = class_def.intersects_class (glyphs, value);
  map->مجموعه (value, v);

  أرجع v;
}
ساكن مضمن منطقي intersects_coverage (ثابت hb_مجموعه_t *glyphs, بیعلامت value, ثابت فراغ *داده, فراغ *cache HB_استفادهنشده)
{
  Offset32_مقصد<Coverage> coverage;
  coverage = value;
  أرجع (داده+coverage).intersects (glyphs);
}


ساكن مضمن فراغ intersected_glyph (ثابت hb_مجموعه_t *glyphs HB_استفادهنشده, ثابت فراغ *داده, بیعلامت value, hb_مجموعه_t *intersected_glyphs, HB_استفادهنشده فراغ *cache)
{
  بیعلامت g = reinterpret_cast<ثابت HBUINT16 *>(داده)[value];
  intersected_glyphs->جمع_کن (g);
}

استفاده intersected_class_cache_t = hb_hashmap_t<بیعلامت, hb_مجموعه_t>;

ساكن مضمن فراغ intersected_class_glyphs (ثابت hb_مجموعه_t *glyphs, ثابت فراغ *داده, بیعلامت value, hb_مجموعه_t *intersected_glyphs, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(داده);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_مجموعه_t *cached_v;
  إذا (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    أرجع;
  }

  hb_مجموعه_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->مجموعه (value, std::move (v));
}

ساكن مضمن فراغ intersected_coverage_glyphs (ثابت hb_مجموعه_t *glyphs, ثابت فراغ *داده, بیعلامت value, hb_مجموعه_t *intersected_glyphs, HB_استفادهنشده فراغ *cache)
{
  Offset32_مقصد<Coverage> coverage;
  coverage = value;
  (داده+coverage).intersect_مجموعه (*glyphs, *intersected_glyphs);
}


ساكن مضمن منطقي array_is_subset_از (ثابت hb_مجموعه_t *glyphs,
				       بیعلامت صحيح شمار,
				       glyph_داده_t values,
				       intersects_func_t intersects_func,
				       ثابت فراغ *intersects_داده,
				       فراغ *cache)
{
  لكل (بیعلامت i = 0; i < شمار; i++)
    إذا (!intersects_func (glyphs, values[i], intersects_داده, cache)) أرجع خطأ;
  أرجع درست;
}


ساكن مضمن فراغ collect_glyph (hb_مجموعه_t *glyphs, بیعلامت value, ثابت فراغ *داده HB_استفادهنشده)
{
  glyphs->جمع_کن (value);
}
ساكن مضمن فراغ collect_class (hb_مجموعه_t *glyphs, بیعلامت value, ثابت فراغ *داده)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(داده);
  class_def.collect_class (glyphs, value);
}
ساكن مضمن فراغ collect_coverage (hb_مجموعه_t *glyphs, بیعلامت value, ثابت فراغ *داده)
{
  Offset32_مقصد<Coverage> coverage;
  coverage = value;
  (داده+coverage).collect_coverage (glyphs);
}

مضمن ثابت hb_مجموعه_t *
hb_depend_context_t::پیدا_کن_or_create_glyph_مجموعه (ثابت فراغ *داده,
                                                بیعلامت value,
                                                glyph_مجموعه_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_مجموعه_cache_کلید_t key (داده, value, type);
  hb_codepoint_t *existing = اشاره_تهی;
  إذا (glyph_مجموعه_مقصد_شاخص.has (key, &existing))
    أرجع glyph_sets[*existing].get ();

  hb_مجموعه_t *مجموعه = hb_object_create<hb_مجموعه_t> ();
  إذا (unlikely (!مجموعه))
  {
    depend_داده->fail ();
    أرجع اشاره_تهی;
  }

  إذا (type == GLYPH_مجموعه_COVERAGE)
    collect_coverage (مجموعه, value, داده);
  وإلا إذا (type == GLYPH_مجموعه_CLASS)
    collect_class (مجموعه, value, داده);
  وإلا
    مجموعه->جمع_کن (value);
  إذا (unlikely (مجموعه->درون_error ()))
  {
    hb_مجموعه_destroy (مجموعه);
    depend_داده->fail ();
    أرجع اشاره_تهی;
  }

  hb_codepoint_t شاخص_2 = glyph_sets.درازی;
  إذا (unlikely (!glyph_sets.افزودن_or_fail (hb::unique_ptr<hb_مجموعه_t> {مجموعه})))
  {
    depend_داده->fail ();
    أرجع اشاره_تهی;
  }
  إذا (unlikely (!glyph_مجموعه_مقصد_شاخص.مجموعه (key, شاخص_2)))
  {
    depend_داده->fail ();
    أرجع اشاره_تهی;
  }
  أرجع glyph_sets[شاخص_2].get ();
}

مضمن hb_codepoint_t
hb_depend_context_t::پیدا_کن_or_create_cached_context_مجموعه (ثابت hb_مجموعه_t *مجموعه)
{
  uintptr_t key = (uintptr_t) مجموعه;
  hb_codepoint_t *existing = اشاره_تهی;
  إذا (cached_context_مجموعه_indices.has (key, &existing))
    أرجع *existing;

  hb_codepoint_t شاخص_2 = depend_داده->پیدا_کن_or_create_context_مجموعه (*مجموعه);
  إذا (unlikely (شاخص_2 == HB_CODEPOINT_INVALID))
    أرجع HB_CODEPOINT_INVALID;
  إذا (unlikely (!cached_context_مجموعه_indices.مجموعه (key, شاخص_2)))
    أرجع depend_داده->fail_invalid ();
  أرجع شاخص_2;
}

ساكن مضمن فراغ collect_array (hb_collect_glyphs_context_t *c HB_استفادهنشده,
				  hb_مجموعه_t *glyphs,
				  بیعلامت صحيح شمار,
				  glyph_داده_t values,
				  collect_glyphs_func_t collect_func,
				  ثابت فراغ *collect_داده)
{
  لكل (بیعلامت i = 0; i < شمار; i++)
    collect_func (glyphs, values[i], collect_داده);
}


ساكن مضمن منطقي تطابق_always (hb_glyph_info_t &info HB_استفادهنشده, بیعلامت value HB_استفادهنشده, ثابت فراغ *داده HB_استفادهنشده)
{
  أرجع درست;
}
ساكن مضمن منطقي تطابق_glyph (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده HB_استفادهنشده)
{
  أرجع info.codepoint == value;
}
ساكن مضمن منطقي تطابق_class (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(داده);
  أرجع class_def.get_class (info.codepoint) == value;
}
ساكن مضمن بیعلامت get_class_مقدار (hb_glyph_info_t &info, ثابت فراغ *داده)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (داده);
  أرجع class_def.get_class (info.codepoint);
}
بنية تطابق_class_cache_داده_t
{
  ثابت ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ساكن مضمن منطقي تطابق_class_cache (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده)
{
  ثابت خودکار &cache_داده = *reinterpret_cast<ثابت تطابق_class_cache_داده_t *> (داده);
  أرجع cache_داده.class_def->get_class (info.codepoint, cache_داده.cache) == value;
}
ساكن مضمن بیعلامت get_class_cache_مقدار (hb_glyph_info_t &info, ثابت فراغ *داده)
{
  ثابت خودکار &cache_داده = *reinterpret_cast<ثابت تطابق_class_cache_داده_t *> (داده);
  أرجع cache_داده.class_def->get_class (info.codepoint, cache_داده.cache);
}
ساكن مضمن بیعلامت get_class_cached (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بیعلامت klass = info.syllable();
  إذا (klass < 255)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 255))
    info.syllable() = klass;
  أرجع klass;
}
ساكن مضمن منطقي تطابق_class_cached (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(داده);
  أرجع get_class_cached (class_def, info) == value;
}
ساكن مضمن بیعلامت get_class_cached_مقدار (hb_glyph_info_t &info, ثابت فراغ *داده)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (داده);
  أرجع get_class_cached (class_def, info);
}
ساكن مضمن بیعلامت get_class_cached1 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بیعلامت klass = info.syllable() & 0x0F;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  أرجع klass;
}
ساكن مضمن منطقي تطابق_class_cached1 (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(داده);
  أرجع get_class_cached1 (class_def, info) == value;
}
ساكن مضمن بیعلامت get_class_cached2 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بیعلامت klass = (info.syllable() & 0xF0) >> 4;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  أرجع klass;
}
ساكن مضمن منطقي تطابق_class_cached2 (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(داده);
  أرجع get_class_cached2 (class_def, info) == value;
}
ساكن مضمن بیعلامت get_class_cached2_مقدار (hb_glyph_info_t &info, ثابت فراغ *داده)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (داده);
  أرجع get_class_cached2 (class_def, info);
}
ساكن مضمن منطقي تطابق_coverage (hb_glyph_info_t &info, بیعلامت value, ثابت فراغ *داده)
{
  Offset32_مقصد<Coverage> coverage;
  coverage = value;
  أرجع (داده+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ساكن مضمن منطقي would_تطابق_درون_داد (hb_would_apply_context_t *c,
				      بیعلامت صحيح شمار, /* Including the first glyph (not matched) */
				      glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
				      تطابق_func_t تطابق_func_2,
				      ثابت فراغ *تطابق_داده_2)
{
  إذا (شمار != c->len)
    أرجع خطأ;

  لكل (بیعلامت صحيح i = 1; i < شمار; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    إذا (likely (!تطابق_func_2 (info, درون_داد[i - 1], تطابق_داده_2)))
      أرجع خطأ;
  }

  أرجع درست;
}
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
HB_ALWAYS_INLINE
#وإلا
مضمن
#پایان_اگر
ساكن منطقي تطابق_درون_داد (hb_ot_apply_context_t *c,
			 بیعلامت صحيح شمار, /* Including the first glyph (not matched) */
			 glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
			 تطابق_func_t تطابق_func_2,
			 ثابت فراغ *تطابق_داده_2,
			 بیعلامت صحيح *انتها_position,
			 بیعلامت صحيح *p_مجموع_component_شمار = اشاره_تهی)
{
  TRACE_APPLY (اشاره_تهی);

  hb_میانگیر_t *میانگیر = c->میانگیر;

  إذا (شمار == 1)
  {
    *انتها_position = میانگیر->idx + 1;
    c->تطابق_positions[0] = میانگیر->idx;
    إذا (p_مجموع_component_شمار)
      *p_مجموع_component_شمار = hb_glyph_info_get_lig_رقم_comps (&میانگیر->cur());
    return_trace (درست);
  }

  إذا (unlikely (شمار > HB_MAX_CONTEXT_درازی)) return_trace (خطأ);

  خودکار &skippy_iter = c->iter_درون_داد;
  skippy_iter.reset (میانگیر->idx);
  skippy_iter.مجموعه_تطابق_func (تطابق_func_2, تطابق_داده_2);
  skippy_iter.مجموعه_glyph_داده (درون_داد);

  /*
   * This is perhaps the trickiest part of OpenType...  Remarks:
   *
   * - If all components of the ligature were marks, we call this a mark ligature.
   *
   * - If there is no GDEF, and the ligature is NOT a mark ligature, we categorize
   *   it as a ligature glyph.
   *
   * - Ligatures cannot be formed across glyphs attached to different components
   *   of previous ligatures.  Eg. the sequence is LAM,SHADDA,LAM,FATHA,HEH, and
   *   LAM,LAM,HEH form a ligature, leaving SHADDA,FATHA next to eachother.
   *   However, it would be wrong to ligate that SHADDA,FATHA sequence.
   *   There are a couple of exceptions to this:
   *
   *   o If a ligature tries ligating with marks that belong to it itself, go ahead,
   *     assuming that the font designer knows what they are doing (otherwise it can
   *     break Indic stuff when a matra wants to ligate with a conjunct,
   *
   *   o If two marks want to ligate and they belong to different components of the
   *     same ligature glyph, and said ligature glyph is to be ignored according to
   *     mark-filtering rules, then allow.
   *     https://github.com/harfbuzz/harfbuzz/issues/545
   */

  بیعلامت صحيح مجموع_component_شمار = 0;

  بیعلامت صحيح اولی_lig_id = _hb_glyph_info_get_lig_id (&میانگیر->cur());
  بیعلامت صحيح اولی_lig_comp = _hb_glyph_info_get_lig_comp (&میانگیر->cur());

  تعداد {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  لكل (بیعلامت صحيح i = 1; i < شمار; i++)
  {
    بیعلامت unsafe_مقصد;
    إذا (!skippy_iter.next (&unsafe_مقصد))
    {
      *انتها_position = unsafe_مقصد;
      return_trace (خطأ);
    }

    إذا (unlikely (i + 1 > c->تطابق_positions.درازی &&
		  !c->تطابق_positions.resize_dirty  (i + 1)))
      return_trace (خطأ);
    c->تطابق_positions.arrayZ[i] = skippy_iter.idx;

    بیعلامت صحيح this_lig_id = _hb_glyph_info_get_lig_id (&میانگیر->info[skippy_iter.idx]);
    بیعلامت صحيح this_lig_comp = _hb_glyph_info_get_lig_comp (&میانگیر->info[skippy_iter.idx]);

    إذا (اولی_lig_id && اولی_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      إذا (اولی_lig_id != this_lig_id || اولی_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	إذا (ligbase == LIGBASE_NOT_CHECKED)
	{
	  منطقي یافت = خطأ;
	  ثابت خودکار *بیرون = میانگیر->بیرون_info;
	  بیعلامت صحيح j = میانگیر->بیرون_len;
	  طالما (j && _hb_glyph_info_get_lig_id (&بیرون[j - 1]) == اولی_lig_id)
	  {
	    إذا (_hb_glyph_info_get_lig_comp (&بیرون[j - 1]) == 0)
	    {
	      j--;
	      یافت = درست;
	      اكسر;
	    }
	    j--;
	  }

	  إذا (یافت && skippy_iter.may_skip (بیرون[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  وإلا
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	إذا (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (خطأ);
      }
    }
    وإلا
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      إذا (this_lig_id && this_lig_comp && (this_lig_id != اولی_lig_id))
	return_trace (خطأ);
    }

    مجموع_component_شمار += hb_glyph_info_get_lig_رقم_comps_درون_ligation (&میانگیر->info[skippy_iter.idx]);
  }

  *انتها_position = skippy_iter.idx + 1;

  إذا (p_مجموع_component_شمار)
  {
    مجموع_component_شمار += hb_glyph_info_get_lig_رقم_comps (&میانگیر->cur());
    *p_مجموع_component_شمار = مجموع_component_شمار;
  }

  c->تطابق_positions.arrayZ[0] = میانگیر->idx;

  return_trace (درست);
}
ساكن مضمن منطقي ligate_درون_داد (hb_ot_apply_context_t *c,
				 بیعلامت صحيح شمار, /* Including the first glyph */
				 بیعلامت صحيح تطابق_انتها,
				 hb_codepoint_t lig_glyph,
				 بیعلامت صحيح مجموع_component_شمار)
{
  TRACE_APPLY (اشاره_تهی);

  hb_میانگیر_t *میانگیر = c->میانگیر;

  میانگیر->ادغام_clusters (میانگیر->idx, تطابق_انتها);

  /* - If a base and one or more marks ligate, consider that as a base, NOT
   *   ligature, such that all following marks can still attach to it.
   *   https://github.com/harfbuzz/harfbuzz/issues/1109
   *
   * - If all components of the ligature were marks, we call this a mark ligature.
   *   If it *is* a mark ligature, we don't allocate a new ligature id, and leave
   *   the ligature to keep its old ligature id.  This will allow it to attach to
   *   a base ligature in GPOS.  Eg. if the sequence is: LAM,LAM,SHADDA,FATHA,HEH,
   *   and LAM,LAM,HEH for a ligature, they will leave SHADDA and FATHA with a
   *   ligature id and component value of 2.  Then if SHADDA,FATHA form a ligature
   *   later, we don't want them to lose their ligature id/component, otherwise
   *   GPOS will fail to correctly position the mark ligature on top of the
   *   LAM,LAM,HEH ligature.  See:
   *     https://bugzilla.gnome.org/show_bug.cgi?id=676343
   *
   * - If a ligature is formed of components that some of which are also ligatures
   *   themselves, and those ligature components had marks attached to *their*
   *   components, we have to attach the marks to the new ligature component
   *   positions!  Now *that*'s tricky!  And these marks may be following the
   *   last component of the whole sequence, so we should loop forward looking
   *   for them and update them.
   *
   *   Eg. the sequence is LAM,LAM,SHADDA,FATHA,HEH, and the font first forms a
   *   'calt' ligature of LAM,HEH, leaving the SHADDA and FATHA with a ligature
   *   id and component == 1.  Now, during 'liga', the LAM and the LAM-HEH ligature
   *   form a LAM-LAM-HEH ligature.  We need to reassign the SHADDA and FATHA to
   *   the new ligature with a component value of 2.
   *
   *   This in fact happened to a font...  See:
   *   https://bugzilla.gnome.org/show_bug.cgi?id=437633
   */

  منطقي is_پایه_ligature = hb_glyph_info_is_پایه_glyph (&میانگیر->info[c->تطابق_positions.arrayZ[0]]);
  منطقي is_mark_ligature = _hb_glyph_info_is_mark (&میانگیر->info[c->تطابق_positions.arrayZ[0]]);
  لكل (بیعلامت صحيح i = 1; i < شمار; i++)
    إذا (!_hb_glyph_info_is_mark (&میانگیر->info[c->تطابق_positions.arrayZ[i]]))
    {
      is_پایه_ligature = خطأ;
      is_mark_ligature = خطأ;
      اكسر;
    }
  منطقي is_ligature = !is_پایه_ligature && !is_mark_ligature;

  بیعلامت صحيح klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  بیعلامت صحيح lig_id = is_ligature ? _hb_allocate_lig_id (میانگیر) : 0;
  بیعلامت صحيح آخرین_lig_id = _hb_glyph_info_get_lig_id (&میانگیر->cur());
  بیعلامت صحيح آخرین_رقم_components = hb_glyph_info_get_lig_رقم_comps (&میانگیر->cur());
  بیعلامت صحيح components_so_far = آخرین_رقم_components;

  إذا (is_ligature)
  {
    hb_glyph_info_مجموعه_lig_props_for_ligature (&میانگیر->cur(), lig_id, مجموع_component_شمار);
    إذا (_hb_glyph_info_get_general_category (&میانگیر->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      hb_glyph_info_مجموعه_general_category (&میانگیر->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  لكل (بیعلامت صحيح i = 1; i < شمار; i++)
  {
    طالما (میانگیر->idx < c->تطابق_positions.arrayZ[i] && میانگیر->successful)
    {
      إذا (is_ligature)
      {
	بیعلامت صحيح this_comp = _hb_glyph_info_get_lig_comp (&میانگیر->cur());
	إذا (this_comp == 0)
	  this_comp = آخرین_رقم_components;
	أكد (components_so_far >= آخرین_رقم_components);
	بیعلامت صحيح new_lig_comp = components_so_far - آخرین_رقم_components +
				    hb_min (this_comp, آخرین_رقم_components);
	  hb_glyph_info_مجموعه_lig_props_for_mark (&میانگیر->cur(), lig_id, new_lig_comp);
      }
      (فراغ) میانگیر->بعدی_glyph ();
    }

    آخرین_lig_id = _hb_glyph_info_get_lig_id (&میانگیر->cur());
    آخرین_رقم_components = hb_glyph_info_get_lig_رقم_comps_درون_ligation (&میانگیر->cur());
    components_so_far += آخرین_رقم_components;

    /* Skip the base glyph */
    میانگیر->idx++;
  }

  إذا (!is_mark_ligature && آخرین_lig_id)
  {
    /* Re-adjust components for any marks following. */
    لكل (بیعلامت i = میانگیر->idx; i < میانگیر->len; ++i)
    {
      إذا (آخرین_lig_id != _hb_glyph_info_get_lig_id (&میانگیر->info[i])) اكسر;

      بیعلامت this_comp = _hb_glyph_info_get_lig_comp (&میانگیر->info[i]);
      إذا (!this_comp) اكسر;

      أكد (components_so_far >= آخرین_رقم_components);
      بیعلامت new_lig_comp = components_so_far - آخرین_رقم_components +
			      hb_min (this_comp, آخرین_رقم_components);
      hb_glyph_info_مجموعه_lig_props_for_mark (&میانگیر->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (درست);
}

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
HB_ALWAYS_INLINE
#وإلا
مضمن
#پایان_اگر
ساكن منطقي تطابق_backtrack (hb_ot_apply_context_t *c,
			     بیعلامت صحيح شمار,
			     glyph_داده_t backtrack,
			     تطابق_func_t تطابق_func_2,
			     ثابت فراغ *تطابق_داده_2,
			     بیعلامت صحيح *تطابق_شروع)
{
  TRACE_APPLY (اشاره_تهی);

  إذا (!شمار)
  {
    *تطابق_شروع = c->میانگیر->backtrack_len ();
    return_trace (درست);
  }

  خودکار &skippy_iter = c->iter_context;
  skippy_iter.reset_پشت (c->میانگیر->backtrack_len ());
  skippy_iter.مجموعه_تطابق_func (تطابق_func_2, تطابق_داده_2);
  skippy_iter.مجموعه_glyph_داده (backtrack);

  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    بیعلامت unsafe_from;
    إذا (!skippy_iter.prev (&unsafe_from))
    {
      *تطابق_شروع = unsafe_from;
      return_trace (خطأ);
    }
  }

  *تطابق_شروع = skippy_iter.idx;
  return_trace (درست);
}

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
HB_ALWAYS_INLINE
#وإلا
مضمن
#پایان_اگر
ساكن منطقي تطابق_lookahead (hb_ot_apply_context_t *c,
			     بیعلامت صحيح شمار,
			     glyph_داده_t lookahead,
			     تطابق_func_t تطابق_func_2,
			     ثابت فراغ *تطابق_داده_2,
			     بیعلامت صحيح شروع_شاخص,
			     بیعلامت صحيح *انتها_شاخص)
{
  TRACE_APPLY (اشاره_تهی);

  إذا (!شمار)
  {
    *انتها_شاخص = شروع_شاخص;
    return_trace (درست);
  }

  خودکار &skippy_iter = c->iter_context;
  أكد (شروع_شاخص >= 1);
  skippy_iter.reset (شروع_شاخص - 1);
  skippy_iter.مجموعه_تطابق_func (تطابق_func_2, تطابق_داده_2);
  skippy_iter.مجموعه_glyph_داده (lookahead);

  لكل (بیعلامت صحيح i = 0; i < شمار; i++)
  {
    بیعلامت unsafe_مقصد;
    إذا (!skippy_iter.next (&unsafe_مقصد))
    {
      *انتها_شاخص = unsafe_مقصد;
      return_trace (خطأ);
    }
  }

  *انتها_شاخص = skippy_iter.idx + 1;
  return_trace (درست);
}



بنية Lookup_رکورد
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t         *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->embed (*هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    return_trace (c->check_تخصیص_بده (بیرون->lookup_List_شاخص, lookup_map->get (lookup_List_شاخص), HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	sequence_شاخص;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookup_List_شاخص;	/* Lookup to apply to that
					 * position--zero--based */
  عام:
  DEFINE_اندازه_STATIC (4);
};

ساكن مضمن بیعلامت serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ثابت hb_array_t<ثابت Lookup_رکورد> lookup_رکوردها,
						     ثابت hb_map_t *lookup_map)
{
  بیعلامت شمار = 0;
  لكل (ثابت Lookup_رکورد& r : lookup_رکوردها)
  {
    إذا (!lookup_map->has (r.lookup_List_شاخص))
      استمر;

    إذا (!r.serialize (c, lookup_map))
      أرجع 0;

    شمار++;
  }
  أرجع شمار;
}

تعداد ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ساكن مضمن ثابت hb_مجموعه_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ثابت فراغ *داده,
			بیعلامت value,
			منطقي filter_class_by_والد,
			hb_مجموعه_t *scratch,
			hb_وکتور_t<بیعلامت> *compact_workspace = اشاره_تهی)
{
  بدل (context_format)
  {
    حالة ContextFormat::SimpleContext:
      أرجع c->پیدا_کن_or_create_glyph_مجموعه (اشاره_تهی, value,
                                           hb_depend_context_t::GLYPH_مجموعه_SINGLE);
    حالة ContextFormat::ClassBasedContext:
    {
      ثابت hb_مجموعه_t *class_مجموعه = c->پیدا_کن_or_create_glyph_مجموعه (
        داده, value, hb_depend_context_t::GLYPH_مجموعه_CLASS);
      إذا (unlikely (!class_مجموعه))
	أرجع اشاره_تهی;
      إذا (!filter_class_by_والد)
	أرجع class_مجموعه;
      scratch->مجموعه (*class_مجموعه);
      scratch->intersect (c->والد_فعال_glyphs (), compact_workspace);
      اكسر;
    }
    حالة ContextFormat::CoverageBasedContext:
      أرجع c->پیدا_کن_or_create_glyph_مجموعه (
        داده, value, hb_depend_context_t::GLYPH_مجموعه_COVERAGE);
  }

  إذا (unlikely (scratch->درون_error ()))
  {
    c->depend_داده->fail ();
    أرجع اشاره_تهی;
  }
  أرجع scratch;
}

ساكن مضمن فراغ context_بستگی_recurse_lookups (hb_بستگی_context_t *c,
					     بیعلامت درون_داد_شمار, glyph_داده_t درون_داد,
					     بیعلامت lookup_شمار_2,
					     ثابت Lookup_رکورد lookup_رکورد[] /* Array of LookupRecords--in design order */,
					     بیعلامت value,
					     ContextFormat context_format,
					     ثابت فراغ *داده,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache)
{
  hb_مجموعه_t covered_seq_indicies;
  hb_مجموعه_t pos_glyphs;
  لكل (بیعلامت صحيح i = 0; i < lookup_شمار_2; i++)
  {
    بیعلامت seq_شاخص_2 = lookup_رکورد[i].sequence_شاخص;
    إذا (seq_شاخص_2 >= درون_داد_شمار) استمر;

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indicies.has (seq_شاخص_2))
    {
      has_pos_glyphs = درست;
      pos_glyphs.clear ();
      إذا (seq_شاخص_2 == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.جمع_کن (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->والد_فعال_glyphs (), داده, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.مجموعه (c->والد_فعال_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *درون_داد_داده = درون_داد.داده;
        بیعلامت درون_داد_مقدار = seq_شاخص_2 - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          درون_داد_داده = داده;
          درون_داد_مقدار = درون_داد[seq_شاخص_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, درون_داد_داده, درون_داد_مقدار, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.جمع_کن (seq_شاخص_2);
    hb_مجموعه_t *cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs))
      أرجع;
    إذا (has_pos_glyphs) {
      *cur_فعال_glyphs = std::move (pos_glyphs);
    } وإلا {
      *cur_فعال_glyphs = *c->glyphs;
    }

    بیعلامت انتها_شاخص_2 = درون_داد_شمار;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      انتها_شاخص_2 += 1;

    c->recurse (lookup_رکورد[i].lookup_List_شاخص, &covered_seq_indicies, seq_شاخص_2, انتها_شاخص_2);

    c->برداشت_cur_انجامشده_glyphs ();
  }
}

ساكن منطقي
context_depend_sets_are_cached (hb_depend_context_t *c,
				بیعلامت درون_داد_شمار_2,
				بیعلامت lookup_شمار,
				ثابت Lookup_رکورد lookup_رکوردها_2[],
				ثابت فراغ *قاعده_2,
				بیعلامت value,
				ContextFormat context_format,
				ثابت فراغ *داده,
				hb_وکتور_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
  منطقي have_فعال_idx = خطأ;
  لكل (بیعلامت i = 0; i < lookup_شمار; i++)
  {
    إذا (lookup_رکوردها_2[i].sequence_شاخص >= درون_داد_شمار_2)
      استمر;

    إذا (!have_فعال_idx)
    {
      إذا (unlikely (!c->get_والد_فعال_glyphs_شاخص (&فعال_idx)))
	أرجع خطأ;
      have_فعال_idx = درست;
    }

    hb_depend_context_t::context_مجموعه_cache_کلید_t key (
      قاعده_2, &lookup_رکوردها_2[i], داده, فعال_idx, value, context_format);
    hb_codepoint_t *cached_context_مجموعه = اشاره_تهی;
    إذا (!c->context_مجموعه_cache.has (key, &cached_context_مجموعه))
      أرجع خطأ;
    إذا (unlikely (!cached_context_sets->افزودن_or_fail (*cached_context_مجموعه)))
      أرجع c->depend_داده->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  أرجع have_فعال_idx;
}

ساكن مضمن فراغ context_depend_recurse_lookups (hb_depend_context_t *c,
					     بیعلامت درون_داد_شمار, glyph_داده_t درون_داد,
					     بیعلامت lookup_شمار_2,
					     ثابت Lookup_رکورد lookup_رکورد[] /* Array of LookupRecords--in design order */,
					     ثابت فراغ *قاعده_2,
					     بیعلامت value,
					     ContextFormat context_format,
					     ثابت فراغ *داده,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache,
					     ثابت hb_مجموعه_t &preliminary_context,
					     ثابت hb_وکتور_t<ثابت hb_مجموعه_t *> *درون_داد_position_glyphs,
					     ثابت hb_وکتور_t<hb_codepoint_t> *cached_context_sets,
					     hb_depend_context_t::depend_scratch_t *scratch)
{
  /* For depend graph extraction, we filter active glyphs by InputCoverage constraints
   * (same as closure), but we do NOT do sequential accumulation of outputs.
   *
   * - Position filtering (InputCoverage): Required. The contextual rule can only match
   *   glyphs in its InputCoverage. Edges for other glyphs are spurious.
   * - Sequential accumulation: Not done. We want ALL possible edges, so we don't
   *   limit later lookups based on outputs from earlier lookups in the same rule. */

  /* Sequence positions are tracked in one allocation-free bit page below.
   * A rule longer than HB_MAX_CONTEXT_LENGTH never applies during shaping
   * (the apply path rejects it), so skip it here instead of running past
   * the fixed page in add_range(). */
  إذا (unlikely (درون_داد_شمار > HB_MAX_CONTEXT_درازی))
    أرجع;

  hb_بیت_صفحه_t covered_seq_indices;
  hb_مجموعه_t &pos_glyphs = scratch->pos_glyphs;
  hb_مجموعه_t &position_context = scratch->position_context;
  hb_مجموعه_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_مجموعه_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  بیعلامت cached_context_شاخص = 0;
  لكل (بیعلامت صحيح i = 0; i < lookup_شمار_2; i++)
  {
    بیعلامت seq_شاخص_2 = lookup_رکورد[i].sequence_شاخص;
    إذا (seq_شاخص_2 >= درون_داد_شمار)
      استمر;

    hb_depend_context_t::context_مجموعه_cache_کلید_t context_cache_کلید;
    إذا (unlikely (!c->get_context_مجموعه_cache_کلید (قاعده_2, &lookup_رکورد[i], داده, value,
						 context_format, &context_cache_کلید)))
      أرجع;

    hb_codepoint_t context_مجموعه_idx;
    hb_codepoint_t *cached_context_مجموعه_idx;
    إذا (cached_context_sets)
      context_مجموعه_idx = (*cached_context_sets)[cached_context_شاخص++];
    وإلا إذا (c->context_مجموعه_cache.has (context_cache_کلید, &cached_context_مجموعه_idx))
      context_مجموعه_idx = *cached_context_مجموعه_idx;
    وإلا
    {
      إذا (unlikely (!درون_داد_position_glyphs))
      {
	c->depend_داده->fail ();
	أرجع;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_مجموعه_مقدار_INVALID;
      طالما (preliminary_context.next (&elem)) {
	إذا (!(elem & HB_DEPEND_CONTEXT_مجموعه_FLAG)) {
	  /* Direct glyph */
	  position_context.جمع_کن (elem);
	} وإلا {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.جمع_کن (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      لكل (بیعلامت j = 0; j < درون_داد_position_glyphs->درازی; j++) {
	إذا (j != seq_شاخص_2 && (*درون_داد_position_glyphs)[j]->get_population () == 1) {
	  position_context.جمع_کن ((*درون_داد_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_مجموعه_مقدار_INVALID;
      طالما (disjunctive_indices.next (&elem)) {
	hb_codepoint_t مجموعه_idx = elem & 0x7FFFFFFF;
	ثابت hb_مجموعه_t *original_مجموعه = c->depend_داده->get_مجموعه_from_شاخص (مجموعه_idx);
	إذا (unlikely (!original_مجموعه)) استمر;

	/* Dependency context sets are never inverted. */
	إذا (!original_مجموعه->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.جمع_کن (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      لكل (بیعلامت j = 0; j < درون_داد_position_glyphs->درازی; j++) {
	إذا (j != seq_شاخص_2 && (*درون_داد_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  إذا (!(*درون_داد_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    منطقي cached_مجموعه = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_مجموعه
	      ? c->پیدا_کن_or_create_cached_context_مجموعه ((*درون_داد_position_glyphs)[j])
	      : c->depend_داده->پیدا_کن_or_create_context_مجموعه (*(*درون_داد_position_glyphs)[j]);
	    إذا (unlikely (idx == HB_CODEPOINT_INVALID))
	      أرجع;
	    filtered_disjunctive_indices.جمع_کن (HB_DEPEND_CONTEXT_مجموعه_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      إذا (unlikely (position_context.درون_error () ||
		    filtered_disjunctive_indices.درون_error ()))
      {
	c->depend_داده->fail ();
	أرجع;
      }

      context_مجموعه_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_داده->پیدا_کن_or_create_context_مجموعه (position_context);
      إذا (unlikely (!position_context.is_empty () &&
		    context_مجموعه_idx == HB_CODEPOINT_INVALID))
	أرجع;

      إذا (unlikely (!c->context_مجموعه_cache.مجموعه (context_cache_کلید, context_مجموعه_idx)))
      {
	c->depend_داده->fail ();
	أرجع;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_مجموعه_شاخص = c->depend_داده->current_context_مجموعه_شاخص;
    hb_subset_depend_یال_flags_t saved_یال_flags = c->depend_داده->current_یال_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    منطقي nested_contextual = (saved_context_مجموعه_شاخص != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_داده->current_context_مجموعه_شاخص = context_مجموعه_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_داده->current_یال_flags = HB_SUBSET_DEPEND_یال_FLAG_NONE;
    إذا (درون_داد_شمار > 1) {
      c->depend_داده->current_یال_flags |= HB_SUBSET_DEPEND_یال_FLAG_FROM_CONTEXT_POSITION;
    }
    إذا (nested_contextual) {
      c->depend_داده->current_یال_flags |= HB_SUBSET_DEPEND_یال_FLAG_FROM_NESTED_CONTEXT;
    }

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indices.has (seq_شاخص_2))
    {
      has_pos_glyphs = درست;
      pos_glyphs.clear ();
      إذا (seq_شاخص_2 == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.جمع_کن (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->والد_فعال_glyphs (), داده, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.مجموعه (c->والد_فعال_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *درون_داد_داده = درون_داد.داده;
        بیعلامت درون_داد_مقدار = seq_شاخص_2 - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          درون_داد_داده = داده;
          درون_داد_مقدار = درون_داد[seq_شاخص_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, درون_داد_داده, درون_داد_مقدار, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.جمع_کن (seq_شاخص_2);
    إذا (has_pos_glyphs)
    {
      hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
      إذا (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_شاخص_2 ? pos_glyphs.get_min () : value;
	إذا (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_فعال_FLAG))
	  فعال_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_فعال_FLAG;
      }
      hb_مجموعه_t *cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs (فعال_idx);
      إذا (unlikely (!cur_فعال_glyphs))
      {
	c->depend_داده->current_context_مجموعه_شاخص = saved_context_مجموعه_شاخص;
	c->depend_داده->current_یال_flags = saved_یال_flags;
	أرجع;
      }
      *cur_فعال_glyphs = std::move (pos_glyphs);
    }
    وإلا إذا (unlikely (!c->افزودن_ریشه_فعال_glyphs ()))
    {
      c->depend_داده->current_context_مجموعه_شاخص = saved_context_مجموعه_شاخص;
      c->depend_داده->current_یال_flags = saved_یال_flags;
      أرجع;
    }

    بیعلامت انتها_شاخص_2 = درون_داد_شمار;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      انتها_شاخص_2 += 1;

    c->recurse (lookup_رکورد[i].lookup_List_شاخص, &covered_seq_indices, seq_شاخص_2, انتها_شاخص_2);
    c->برداشت_cur_انجامشده_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_داده->current_context_مجموعه_شاخص = saved_context_مجموعه_شاخص;
    c->depend_داده->current_یال_flags = saved_یال_flags;
  }
}

قالب <نام_نوع context_t>
ساكن مضمن فراغ recurse_lookups (context_t *c,
                                    بیعلامت صحيح lookup_شمار_2,
                                    ثابت Lookup_رکورد lookup_رکورد[] /* Array of LookupRecords--in design order */)
{
  لكل (بیعلامت صحيح i = 0; i < lookup_شمار_2; i++)
    c->recurse (lookup_رکورد[i].lookup_List_شاخص);
}

ساكن مضمن فراغ apply_lookup (hb_ot_apply_context_t *c,
				 بیعلامت صحيح شمار, /* Including the first glyph */
				 بیعلامت صحيح lookup_شمار_2,
				 ثابت Lookup_رکورد lookup_رکورد[], /* Array of LookupRecords--in design order */
				 بیعلامت صحيح تطابق_انتها)
{
  hb_میانگیر_t *میانگیر = c->میانگیر;
  صحيح انتها;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    بیعلامت صحيح bl = میانگیر->backtrack_len ();
    انتها = bl + تطابق_انتها - میانگیر->idx;

    صحيح delta = bl - میانگیر->idx;
    /* Convert positions to new indexing. */
    لكل (بیعلامت صحيح j = 0; j < شمار; j++)
      c->تطابق_positions.arrayZ[j] += delta;
  }

  لكل (بیعلامت صحيح i = 0; i < lookup_شمار_2 && میانگیر->successful; i++)
  {
    بیعلامت صحيح idx = lookup_رکورد[i].sequence_شاخص;
    إذا (idx >= شمار)
      استمر;

    بیعلامت صحيح orig_len = میانگیر->backtrack_len () + میانگیر->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    إذا (unlikely (c->تطابق_positions.arrayZ[idx] >= orig_len))
      استمر;

    إذا (unlikely (!میانگیر->move_مقصد (c->تطابق_positions.arrayZ[idx])))
      اكسر;

    إذا (unlikely (میانگیر->max_ops <= 0))
      اكسر;

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      إذا (میانگیر->have_output)
        c->میانگیر->sync_so_far ();
      c->میانگیر->پیام (c->font,
			  "start recursing to lookup %u at %u",
			  (بیعلامت) lookup_رکورد[i].lookup_List_شاخص,
			  میانگیر->idx);
    }

    منطقي ret = c->recurse (lookup_رکورد[i].lookup_List_شاخص);

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      إذا (میانگیر->have_output)
        c->میانگیر->sync_so_far ();
      c->میانگیر->پیام (c->font,
			  "end recursing to lookup %u",
			  (بیعلامت) lookup_رکورد[i].lookup_List_شاخص);
    }

    إذا (!ret)
      استمر;

    بیعلامت صحيح new_len = میانگیر->backtrack_len () + میانگیر->lookahead_len ();
    صحيح delta = new_len - orig_len;

    إذا (!delta)
      استمر;

    /* Recursed lookup changed buffer len.  Adjust.
     *
     * TODO:
     *
     * Right now, if buffer length increased by n, we assume n new glyphs
     * were added right after the current position, and if buffer length
     * was decreased by n, we assume n match positions after the current
     * one where removed.  The former (buffer length increased) case is
     * fine, but the decrease case can be improved in at least two ways,
     * both of which are significant:
     *
     *   - If recursed-to lookup is MultipleSubst and buffer length
     *     decreased, then it's current match position that was deleted,
     *     NOT the one after it.
     *
     *   - If buffer length was decreased by n, it does not necessarily
     *     mean that n match positions where removed, as there recursed-to
     *     lookup might had a different LookupFlag.  Here's a constructed
     *     case of that:
     *     https://github.com/harfbuzz/harfbuzz/discussions/3538
     *
     * It should be possible to construct tests for both of these cases.
     */

    انتها += delta;
    إذا (انتها < صحيح (c->تطابق_positions.arrayZ[idx]))
    {
      /* End might end up being smaller than match_positions.arrayZ[idx] if the recursed
       * lookup ended up removing many items.
       * Just never rewind end beyond start of current position, since that is
       * not possible in the recursed lookup.  Also adjust delta as such.
       *
       * https://bugs.chromium.org/p/chromium/issues/detail?id=659496
       * https://github.com/harfbuzz/harfbuzz/issues/1611
       */
      delta += c->تطابق_positions.arrayZ[idx] - انتها;
      انتها = c->تطابق_positions.arrayZ[idx];
    }

    بیعلامت صحيح next = idx + 1; /* next now is the position after the recursed lookup. */

    إذا (delta > 0)
    {
      إذا (unlikely (delta + شمار > HB_MAX_CONTEXT_درازی))
	اكسر;
      إذا (unlikely (شمار + delta > c->تطابق_positions.درازی &&
		    !c->تطابق_positions.resize_dirty  (شمار + delta)))
        أرجع;
    }
    وإلا
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (صحيح) next - (صحيح) شمار);
      next -= delta;
    }

    /* Shift! */
    memmove (c->تطابق_positions + next + delta, c->تطابق_positions + next,
	     (شمار - next) * حجم (c->تطابق_positions.arrayZ[0]));
    next += delta;
    شمار += delta;

    /* Fill in new entries. */
    لكل (بیعلامت صحيح j = idx + 1; j < next; j++)
      c->تطابق_positions.arrayZ[j] = c->تطابق_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    لكل (; next < شمار; next++)
      c->تطابق_positions.arrayZ[next] += delta;
  }

  أكد (انتها >= 0);
  (فراغ) میانگیر->move_مقصد (انتها);
}



/* Contextual lookups */

بنية Context_بستگی_Lookup_Context
{
  Context_بستگی_Funcs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_داده;
  فراغ *intersects_cache;
  فراغ *intersected_glyphs_cache;
};

تعريفنوع Context_بستگی_Lookup_Context ContextDependLookupContext;

بنية ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_داده;
};

بنية ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ثابت فراغ *تطابق_داده_2;
  get_مقدار_func_t get_مقدار;
};

ساكن مضمن منطقي context_intersects (ثابت hb_مجموعه_t *glyphs,
				       بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
				       glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
				       Context_بستگی_Lookup_Context &lookup_context)
{
  أرجع array_is_subset_از (glyphs,
			     درون_داد_شمار ? درون_داد_شمار - 1 : 0, درون_داد,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_داده,
			     lookup_context.intersects_cache);
}

ساكن مضمن فراغ context_بستگی_lookup (hb_بستگی_context_t *c,
					   بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
					   glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
					   بیعلامت صحيح lookup_شمار_2,
					   ثابت Lookup_رکورد lookup_رکورد[],
					   بیعلامت value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   Context_بستگی_Lookup_Context &lookup_context)
{
  إذا (context_intersects (c->glyphs,
			  درون_داد_شمار, درون_داد,
			  lookup_context))
    context_بستگی_recurse_lookups (c,
				     درون_داد_شمار, درون_داد,
				     lookup_شمار_2, lookup_رکورد,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_داده,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ context_depend_lookup (hb_depend_context_t *c,
					  بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
					  glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
					  بیعلامت صحيح lookup_شمار_2,
					  ثابت Lookup_رکورد lookup_رکورد[],
					  ثابت فراغ *قاعده_2,
					  بیعلامت value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  إذا (!context_intersects (c->glyphs,
			   درون_داد_شمار, درون_داد,
			   lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, درون_داد_شمار, lookup_شمار_2, lookup_رکورد, قاعده_2, value,
    lookup_context.context_format, lookup_context.intersects_داده,
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_داده->successful))
    أرجع;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_مجموعه_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_مجموعه_t &position_scratch = scratch->position_scratch;
  hb_وکتور_t<ثابت hb_مجموعه_t *> &درون_داد_position_glyphs = scratch->درون_داد_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  إذا (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ثابت hb_مجموعه_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_داده, value,
      درست, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !درون_داد_position_glyphs.افزودن_or_fail (pos0_glyphs)))
    {
      c->depend_داده->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (بیعلامت i = 0; i + 1 < درون_داد_شمار; i++)
    {
      ثابت hb_مجموعه_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_داده,
	درون_داد[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !درون_داد_position_glyphs.افزودن_or_fail (pos_glyphs)))
      {
	c->depend_داده->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  درون_داد_شمار, درون_داد,
				  lookup_شمار_2, lookup_رکورد,
				  قاعده_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_داده,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? اشاره_تهی : &درون_داد_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : اشاره_تهی,
				  scratch);
}

ساكن مضمن فراغ context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
						  glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
						  بیعلامت صحيح lookup_شمار_2,
						  ثابت Lookup_رکورد lookup_رکورد[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->درون_داد,
		 درون_داد_شمار ? درون_داد_شمار - 1 : 0, درون_داد,
		 lookup_context.funcs.collect, lookup_context.collect_داده);
  recurse_lookups (c,
		   lookup_شمار_2, lookup_رکورد);
}

ساكن مضمن منطقي context_would_apply_lookup (hb_would_apply_context_t *c,
					       بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
					       glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
					       بیعلامت صحيح lookup_شمار_2 HB_استفادهنشده,
					       ثابت Lookup_رکورد lookup_رکورد[] HB_استفادهنشده,
					       ثابت ContextApplyLookupContext &lookup_context)
{
  أرجع would_تطابق_درون_داد (c,
			    درون_داد_شمار, درون_داد,
			    lookup_context.funcs.تطابق, lookup_context.تطابق_داده_2);
}

ساكن مضمن منطقي context_apply_lookup (hb_ot_apply_context_t *c,
					 بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
					 glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
					 بیعلامت صحيح lookup_شمار_2,
					 ثابت Lookup_رکورد lookup_رکورد[],
					 ثابت ContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (درون_داد_شمار > HB_MAX_CONTEXT_درازی)) أرجع خطأ;

  بیعلامت تطابق_انتها = 0;
  منطقي ret = خطأ;
  إذا (تطابق_درون_داد (c,
		   درون_داد_شمار, درون_داد,
		   lookup_context.funcs.تطابق, lookup_context.تطابق_داده_2,
		   &تطابق_انتها))
  {
    c->میانگیر->unsafe_مقصد_break (c->میانگیر->idx, تطابق_انتها);
    apply_lookup (c,
		  درون_داد_شمار,
		  lookup_شمار_2, lookup_رکورد,
		  تطابق_انتها);
    ret = درست;
  }
  وإلا
  {
    c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, تطابق_انتها);
    ret = خطأ;
  }

  أرجع ret;
}

ساكن مضمن منطقي context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  بدل (op)
  {
    حالة hb_ot_subtable_cache_op_t::ENTER:
    {
      إذا (!HB_میانگیر_TRY_ALLOCATE_VAR (c->میانگیر, syllable))
	أرجع خطأ;
      خودکار &info = c->میانگیر->info;
      بیعلامت شمار = c->میانگیر->len;
      لكل (بیعلامت i = 0; i < شمار; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      أرجع درست;
    }
    حالة hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (بیعلامت) -1;
      HB_میانگیر_DEALLOCATE_VAR (c->میانگیر, syllable);
      اكسر;
    }
  }
  أرجع خطأ;
}

قالب <نام_نوع Types>
بنية قاعده_3
{
  قالب <نام_نوع مجموعه_Types, نام_نوع قاعده_Type>
  دوست بنية قاعده_مجموعه_از;

  منطقي intersects (ثابت hb_مجموعه_t *glyphs, Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    أرجع context_intersects (glyphs,
			       درون_داد_شمار, درون_داد_Z.arrayZ,
			       lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, بیعلامت value, ContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_رکورد = StructAfter<Unsized_Array_از<Lookup_رکورد>>
					   (درون_داد_Z.as_array ((درون_داد_شمار ? درون_داد_شمار - 1 : 0)));
    context_depend_lookup (c,
			   درون_داد_شمار, درون_داد_Z.arrayZ,
			   lookup_شمار_2, lookup_رکورد.arrayZ,
			   هذا,
			   value, lookup_context);
  }

  فراغ بستگی (hb_بستگی_context_t *c, بیعلامت value, Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    ثابت خودکار &lookup_رکورد = StructAfter<Unsized_Array_از<Lookup_رکورد>>
					   (درون_داد_Z.as_array ((درون_داد_شمار ? درون_داد_شمار - 1 : 0)));
    context_بستگی_lookup (c,
			    درون_داد_شمار, درون_داد_Z.arrayZ,
			    lookup_شمار_2, lookup_رکورد.arrayZ,
			    value, lookup_context);
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c,
                        Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت خودکار &lookup_رکورد = StructAfter<Unsized_Array_از<Lookup_رکورد>>
					   (درون_داد_Z.as_array (درون_داد_شمار ? درون_داد_شمار - 1 : 0));
    recurse_lookups (c, lookup_شمار_2, lookup_رکورد.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_رکورد = StructAfter<Unsized_Array_از<Lookup_رکورد>>
					   (درون_داد_Z.as_array (درون_داد_شمار ? درون_داد_شمار - 1 : 0));
    context_collect_glyphs_lookup (c,
				   درون_داد_شمار, درون_داد_Z.arrayZ,
				   lookup_شمار_2, lookup_رکورد.arrayZ,
				   lookup_context);
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest,
			      ثابت ClassDef *class_def = اشاره_تهی) ثابت
  {
    إذا (درون_داد_شمار <= 1)
    {
      *digest = hb_مجموعه_digest_t::پر ();
      أرجع;
    }

    بیعلامت second = درون_داد_Z.arrayZ[0];
    إذا (!class_def)
      digest->جمع_کن (second);
    وإلا إذا (!second)
      *digest = hb_مجموعه_digest_t::پر ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_رکورد = StructAfter<Unsized_Array_از<Lookup_رکورد>>
					   (درون_داد_Z.as_array (درون_داد_شمار ? درون_داد_شمار - 1 : 0));
    أرجع context_would_apply_lookup (c,
				       درون_داد_شمار, درون_داد_Z.arrayZ,
				       lookup_شمار_2, lookup_رکورد.arrayZ,
				       lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودکار &lookup_رکورد = StructAfter<Unsized_Array_از<Lookup_رکورد>>
					   (درون_داد_Z.as_array (درون_داد_شمار ? درون_داد_شمار - 1 : 0));
    return_trace (context_apply_lookup (c, درون_داد_شمار, درون_داد_Z.arrayZ, lookup_شمار_2, lookup_رکورد.arrayZ, lookup_context));
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *درون_داد_mapping, /* old->new glyphid or class mapping */
		  ثابت hb_map_t *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->شروع_embed (هذا);
    إذا (unlikely (!c->extend_min (بیرون))) return_trace (خطأ);

    بیرون->درون_داد_شمار = درون_داد_شمار;
    ثابت خودکار درون_داد = درون_داد_Z.as_array (درون_داد_شمار - 1);
    لكل (ثابت خودکار org : درون_داد)
    {
      نام_نوع Types::HBUINT d;
      d = درون_داد_mapping->get (org);
      c->copy (d);
    }

    ثابت خودکار &lookup_رکورد = StructAfter<Unsized_Array_از<Lookup_رکورد>>
					   (درون_داد_Z.as_array ((درون_داد_شمار ? درون_داد_شمار - 1 : 0)));

    بیعلامت شمار = serialize_lookuprecord_array (c, lookup_رکورد.as_array (lookup_شمار_2), lookup_map);
    return_trace (c->check_تخصیص_بده (بیرون->lookup_شمار_2, شمار, HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!درون_داد_شمار)) return_trace (خطأ);
    ثابت خودکار درون_داد = درون_داد_Z.as_array (درون_داد_شمار - 1);

    ثابت hb_map_t *mapping = klass_map == اشاره_تهی ? c->plan->glyph_map : klass_map;
    إذا (!hb_all (درون_داد, mapping)) return_trace (خطأ);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (درون_داد_Z.arrayZ,
				  درون_داد_Z.قلم_اندازه * (درون_داد_شمار ? درون_داد_شمار - 1 : 0) +
				  Lookup_رکورد::static_اندازه * lookup_شمار_2));
  }

  محمي:
  HBUINT16	درون_داد_شمار;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_شمار_2;		/* Number of LookupRecords */
  Unsized_Array_از<نام_نوع Types::HBUINT>
		درون_داد_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_اندازه_ARRAY (4, درون_داد_Z);
};

قالب <نام_نوع Types, نام_نوع قاعده_Type>
بنية قاعده_مجموعه_از
{
  منطقي intersects (ثابت hb_مجموعه_t *glyphs,
		   Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_map ([&] (ثابت قاعده_Type &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_اولی_درون_داد_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (بیعلامت i = 0; i < قاعده_2.len; i++)
    {
      ثابت قاعده_Type &r = هذا+قاعده_2.arrayZ[i];
      إذا (r.درون_داد_شمار <= 1)
      {
	digest->مجموعه_پر ();
	أرجع;
      }
      digest->جمع_کن (r.درون_داد_Z.arrayZ[0]);
    }
  }

  فراغ depend (hb_depend_context_t *c, بیعلامت value, ContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت قاعده_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }

  فراغ بستگی (hb_بستگی_context_t *c, بیعلامت value,
		Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت قاعده_Type &_) { _.بستگی (c, value, lookup_context); })
    ;
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c,
                        Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت قاعده_Type &_) { _.بستگی_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت قاعده_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest,
			      ثابت ClassDef *class_def = اشاره_تهی) ثابت
  {
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([=] (ثابت قاعده_Type &_) { _.collect_دومی_glyphs (digest, class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_map ([&] (ثابت قاعده_Type &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = اشاره_تهی) ثابت
  {
    TRACE_APPLY (هذا);

    بیعلامت رقم_قواعد = قاعده_2.len;

#اگر_تعریف_نشده HB_NO_OT_RULESETS_FAST_مسیر
    إذا (HB_OPTIMIZE_اندازه_VAL || رقم_قواعد <= 4)
#پایان_اگر
    {
    slow:
      return_trace (
      + hb_iter (قاعده_2)
      | hb_map (hb_جمع_کن (هذا))
      | hb_map ([&] (ثابت قاعده_Type &_) { أرجع _.apply (c, lookup_context); })
      | hb_any
      )
      ;
    }

    /* This version is optimized for speed by matching the first & second
     * components of the rule here, instead of calling into the matching code.
     *
     * Replicated from LigatureSet::apply(). */

    /* We use the iter_context instead of iter_input, to avoid skipping
     * default-ignorables and such.
     *
     * Related: https://github.com/harfbuzz/harfbuzz/issues/4813
     */
    خودکار &skippy_iter = c->iter_context;
    skippy_iter.reset (c->میانگیر->idx);
    skippy_iter.مجموعه_تطابق_func (تطابق_always, اشاره_تهی);
    skippy_iter.مجموعه_glyph_داده ((HBUINT16 *) اشاره_تهی);
    بیعلامت unsafe_مقصد = (بیعلامت) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *اولی = اشاره_تهی, *second = اشاره_تهی;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->میانگیر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      اولی = &c->میانگیر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_مقدار &&
	  !digest->may_have (lookup_context.get_مقدار (*اولی, lookup_context.تطابق_داده_2)))
	{
	  c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_مقصد_concat = خطأ;
      منطقي ret =
      + hb_iter (قاعده_2)
      | hb_map (hb_جمع_کن (هذا))
      | hb_filter ([&] (ثابت قاعده_Type &_)
		   {
		     إذا (_.درون_داد_شمار <= 1) أرجع درست;
		     unsafe_مقصد_concat = درست;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت قاعده_Type &_) { أرجع _.apply (c, lookup_context); })
      | hb_any
      ;
      إذا (unsafe_مقصد_concat)
	c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->میانگیر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      second = &c->میانگیر->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    خودکار تطابق_درون_داد = lookup_context.funcs.تطابق;
    خودکار *درون_داد_داده = lookup_context.تطابق_داده_2;
    لكل (بیعلامت صحيح i = 0; i < رقم_قواعد; i++)
    {
      ثابت خودکار &r = هذا+قاعده_2.arrayZ[i];

      ثابت خودکار &درون_داد = r.درون_داد_Z;

      إذا (r.درون_داد_شمار <= 1 ||
	  (!تطابق_درون_داد ||
	   تطابق_درون_داد (*اولی, درون_داد.arrayZ[0], درون_داد_داده)))
      {
        إذا (!second ||
	    (r.درون_داد_شمار <= 2 ||
	     (!تطابق_درون_داد ||
	      تطابق_درون_داد (*second, درون_داد.arrayZ[1], درون_داد_داده)))
	   )
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_مقصد != (بیعلامت) -1)
	      c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, unsafe_مقصد);
	    return_trace (درست);
	  }
	}
	وإلا
	  unsafe_مقصد = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_مقصد == (بیعلامت) -1)
	  unsafe_مقصد = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	لكل (; i + 1 < رقم_قواعد; i++)
	{
	  ثابت خودکار &r2 = هذا+قاعده_2.arrayZ[i + 1];
	  ثابت خودکار &input2 = r2.درون_داد_Z;
	  إذا (r2.درون_داد_شمار <= 1 || input2.arrayZ[0] != درون_داد.arrayZ[0])
	    اكسر;
	}
      }
    }
    إذا (likely (unsafe_مقصد != (بیعلامت) -1))
      c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, unsafe_مقصد);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار snap = c->serializer->snapshot ();
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    لكل (ثابت خودکار& _ : قاعده_2)
    {
      إذا (!_) استمر;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = بیرون->قاعده_2.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا, lookup_map, klass_map))
      {
	بیرون->قاعده_2.برداشت ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (بیرون->قاعده_2);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (قاعده_2.sanitize (c, هذا));
  }

  محمي:
  Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<قاعده_Type>>
		قاعده_2;			/* Array of Rule tables
					 * ordered by preference */
  عام:
  DEFINE_اندازه_ARRAY (2, قاعده_2);
};

قالب <نام_نوع Types>
بنية قاعده_مجموعه : قاعده_مجموعه_از<SmallTypes, قاعده_3<Types>> {};

قالب <نام_نوع Types>
بنية Class_قاعده_مجموعه : قاعده_مجموعه_از<Types, قاعده_3<SmallTypes>> {};


قالب <نام_نوع Types>
بنية ContextFormat1_4
{
  استفاده قاعده_مجموعه = OT::قاعده_مجموعه<Types>;

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    بنية Context_بستگی_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      اشاره_تهی
    };

    أرجع
    + hb_zip (هذا+coverage, قاعده_مجموعه_2)
    | hb_filter (*glyphs, hb_اولی)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_map ([&] (ثابت قاعده_مجموعه &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع درست; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (*c->glyphs, *cur_فعال_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      اشاره_تهی,
      اشاره_تهی,  /* intersects_cache */
      اشاره_تهی   /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((بیعلامت) قاعده_مجموعه_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->والد_فعال_glyphs ().has (_);
    }, hb_اولی)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بیعلامت> _) { أرجع hb_pair_t<بیعلامت, ثابت قاعده_مجموعه&> (_.اولی, هذا+قاعده_مجموعه_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بیعلامت, ثابت قاعده_مجموعه&>& _) { _.second.depend (c, _.اولی, lookup_context); })
    ;

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (c->قبلی_والد_فعال_glyphs (), *cur_فعال_glyphs);

    بنية Context_بستگی_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      اشاره_تهی
    };

    + hb_zip (هذا+coverage, hb_range ((بیعلامت) قاعده_مجموعه_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->قبلی_والد_فعال_glyphs ().has (_);
    }, hb_اولی)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بیعلامت> _) { أرجع hb_pair_t<بیعلامت, ثابت قاعده_مجموعه&> (_.اولی, هذا+قاعده_مجموعه_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بیعلامت, ثابت قاعده_مجموعه&>& _) { _.second.بستگی (c, _.اولی, lookup_context); })
    ;

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت
  {
    بنية Context_بستگی_Lookup_Context lookup_context = {
      {intersects_glyph, اشاره_تهی},
      ContextFormat::SimpleContext,
      اشاره_تهی
    };

    + hb_zip (هذا+coverage, قاعده_مجموعه_2)
    | hb_filter (*c->glyphs, hb_اولی)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت قاعده_مجموعه &_) { _.بستگی_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->درون_داد);

    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      اشاره_تهی
    };

    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت قاعده_مجموعه &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  {
    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([digest] (ثابت قاعده_مجموعه &_) { _.collect_دومی_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت قاعده_مجموعه &قاعده_مجموعه_3 = هذا+قاعده_مجموعه_2[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ContextApplyLookupContext lookup_context = {
      {تطابق_glyph},
      اشاره_تهی
    };
    أرجع قاعده_مجموعه_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->میانگیر->cur().codepoint);
    إذا (likely (شاخص_2 == NOT_COVERED))
      return_trace (خطأ);

    ثابت قاعده_مجموعه &قاعده_مجموعه_3 = هذا+قاعده_مجموعه_2[شاخص_2];
    بنية ContextApplyLookupContext lookup_context = {
      {تطابق_glyph},
      اشاره_تهی
    };
    return_trace (قاعده_مجموعه_3.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_برچسب_2 == HB_OT_برچسب_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, قاعده_مجموعه_2)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (subset_جابه_جایی_array (c, بیرون->قاعده_مجموعه_2, هذا, lookup_map), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بیرون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && قاعده_مجموعه_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  نام_نوع Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<قاعده_مجموعه>>
		قاعده_مجموعه_2;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, قاعده_مجموعه_2);
};


قالب <نام_نوع Types>
بنية ContextFormat2_5
{
  استفاده قاعده_مجموعه = OT::Class_قاعده_مجموعه<Types>;

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية Context_بستگی_Lookup_Context lookup_context = {
      {intersects_class, اشاره_تهی},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_مجموعه_t retained_coverage_glyphs;
    (هذا+coverage).intersect_مجموعه (*glyphs, retained_coverage_glyphs);

    hb_مجموعه_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    أرجع
    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<بیعلامت, ثابت قاعده_مجموعه &> p)
	      { أرجع class_def.intersects_class (glyphs, p.اولی) &&
		       coverage_glyph_classes.has (p.اولی) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع درست; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (*c->glyphs, *cur_فعال_glyphs);

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (قاعده_مجموعه_2)
    | hb_filter ([&] (بیعلامت _)
    { أرجع class_def.intersects_class (&c->والد_فعال_glyphs (), _); },
		 hb_اولی)
    | hb_apply ([&] (ثابت hb_pair_t<بیعلامت, ثابت نام_نوع Types::قالب جابه_جایی_مقصد<قاعده_مجموعه>&> _)
                {
                  ثابت قاعده_مجموعه& قاعده_مجموعه_3 = هذا+_.second;
                  قاعده_مجموعه_3.depend (c, _.اولی, lookup_context);
                })
    ;

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (c->قبلی_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    بنية Context_بستگی_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (قاعده_مجموعه_2)
    | hb_filter ([&] (بیعلامت _)
    { أرجع class_def.intersects_class (&c->والد_فعال_glyphs (), _); },
		 hb_اولی)
    | hb_apply ([&] (ثابت hb_pair_t<بیعلامت, ثابت نام_نوع Types::قالب جابه_جایی_مقصد<قاعده_مجموعه>&> _)
                {
                  ثابت قاعده_مجموعه& قاعده_مجموعه_3 = هذا+_.second;
                  قاعده_مجموعه_3.بستگی (c, _.اولی, lookup_context);
                })
    ;

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية Context_بستگی_Lookup_Context lookup_context = {
      {intersects_class, اشاره_تهی},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_enumerate
    | hb_filter ([&] (ثابت hb_pair_t<بیعلامت, ثابت قاعده_مجموعه &> p)
    { أرجع class_def.intersects_class (c->glyphs, p.اولی); })
    | hb_map (hb_دومی)
    | hb_apply ([&] (ثابت قاعده_مجموعه & _)
    { _.بستگی_lookups (c, lookup_context); });
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->درون_داد);

    ثابت ClassDef &class_def = هذا+classDef;
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت قاعده_مجموعه &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([=, &class_def] (ثابت قاعده_مجموعه &_) { _.collect_دومی_glyphs (digest, &class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    بیعلامت صحيح شاخص_2 = class_def.get_class (c->glyphs[0]);
    ثابت قاعده_مجموعه &قاعده_مجموعه_3 = هذا+قاعده_مجموعه_2[شاخص_2];
    بنية ContextApplyLookupContext lookup_context = {
      {تطابق_class},
      &class_def
    };
    أرجع قاعده_مجموعه_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بیعلامت cache_هزینه () ثابت
  {
    أرجع (هذا+classDef).هزینه ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_دودویی_cache_t coverage;
    hb_ot_layout_ruleset_digest_t قاعده_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بیعلامت شمار = قاعده_مجموعه_2.len;
    بیعلامت اندازه = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    شمار * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (اندازه);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (بیعلامت i = 0; i < شمار; i++)
	(هذا+قاعده_مجموعه_2[i]).collect_اولی_درون_داد_classes (&cache->قاعده_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, درست, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage_دودویی (c->میانگیر->cur().codepoint, cache ? &cache->coverage : اشاره_تهی);
#وإلا
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->میانگیر->cur().codepoint);
#پایان_اگر
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &class_def = هذا+classDef;

    بنية ContextApplyLookupContext lookup_context = {
      {cached ? تطابق_class_cached : تطابق_class},
      &class_def,
      cached ? get_class_cached_مقدار : get_class_مقدار
    };

    شاخص_2 = cached ? get_class_cached (class_def, c->میانگیر->cur()) : class_def.get_class (c->میانگیر->cur().codepoint);
    ثابت قاعده_مجموعه &قاعده_مجموعه_3 = هذا+قاعده_مجموعه_2[شاخص_2];
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && شاخص_2 < قاعده_مجموعه_2.len ? &cache->قاعده_sets[شاخص_2] : اشاره_تهی;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = اشاره_تهی;
#پایان_اگر
    return_trace (قاعده_مجموعه_3.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;
    إذا (unlikely (!بیرون->coverage.serialize_subset (c, coverage, هذا)))
      return_trace (خطأ);

    hb_map_t klass_map;
    بیرون->classDef.serialize_subset (c, classDef, هذا, &klass_map);

    ثابت hb_مجموعه_t* glyphset = c->plan->glyphset_gsub ();
    hb_مجموعه_t retained_coverage_glyphs;
    (هذا+coverage).intersect_مجموعه (*glyphset, retained_coverage_glyphs);

    hb_مجموعه_t coverage_glyph_classes;
    (هذا+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ثابت hb_map_t *lookup_map = c->جدول_برچسب_2 == HB_OT_برچسب_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    منطقي ret = درست;
    صحيح non_zero_شاخص = -1, شاخص_2 = 0;
    خودکار snapshot = c->serializer->snapshot();
    لكل (ثابت خودکار& _ : + hb_enumerate (قاعده_مجموعه_2)
			 | hb_filter (klass_map, hb_اولی))
    {
      خودکار *o = بیرون->قاعده_مجموعه_2.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      إذا (coverage_glyph_classes.has (_.اولی) &&
	  o->serialize_subset (c, _.second, هذا, lookup_map, &klass_map)) {
	non_zero_شاخص = شاخص_2;
        snapshot = c->serializer->snapshot();
      }

      شاخص_2++;
    }

    إذا (!ret || non_zero_شاخص == -1) return_trace (خطأ);

    //prune empty trailing ruleSets
    --شاخص_2;
    طالما (شاخص_2 > non_zero_شاخص)
    {
      بیرون->قاعده_مجموعه_2.برداشت ();
      شاخص_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (منطقي (بیرون->قاعده_مجموعه_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && classDef.sanitize (c, هذا) && قاعده_مجموعه_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  نام_نوع Types::قالب LOffset_مقصد<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  نام_نوع Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<قاعده_مجموعه>>
		قاعده_مجموعه_2;		/* Array of RuleSet tables
					 * ordered by class */
  عام:
  DEFINE_اندازه_ARRAY (2 + 2 * Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, قاعده_مجموعه_2);
};


قالب <نام_نوع Types>
بنية ContextFormat3_6
{
  استفاده قاعده_مجموعه = OT::قاعده_مجموعه<SmallTypes>;

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (glyphs))
      أرجع خطأ;

    بنية Context_بستگی_Lookup_Context lookup_context = {
      {intersects_coverage, اشاره_تهی},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    أرجع context_intersects (glyphs,
			       glyph_شمار_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع درست; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (*c->glyphs, *cur_فعال_glyphs);

    ثابت Lookup_رکورد *lookup_رکورد = &StructAfter<Lookup_رکورد> (coverageZ.as_array (glyph_شمار_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا,
      اشاره_تهی,  /* intersects_cache */
      اشاره_تهی   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_شمار_2, (ثابت HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_شمار_2, lookup_رکورد,
			   هذا,
			   coverageZ[0], lookup_context);

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (c->قبلی_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت Lookup_رکورد *lookup_رکورد = &StructAfter<Lookup_رکورد> (coverageZ.as_array (glyph_شمار_2));
    بنية Context_بستگی_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    context_بستگی_lookup (c,
			    glyph_شمار_2, coverageZ.arrayZ + 1,
			    lookup_شمار_2, lookup_رکورد,
			    0, lookup_context);

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;
    ثابت Lookup_رکورد *lookup_رکورد = &StructAfter<Lookup_رکورد> (coverageZ.as_array (glyph_شمار_2));
    recurse_lookups (c, lookup_شمار_2, lookup_رکورد);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverageZ[0]).collect_coverage (c->درون_داد);

    ثابت Lookup_رکورد *lookup_رکورد = &StructAfter<Lookup_رکورد> (coverageZ.as_array (glyph_شمار_2));
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      هذا
    };

    context_collect_glyphs_lookup (c,
				   glyph_شمار_2, coverageZ.arrayZ + 1,
				   lookup_شمار_2, lookup_رکورد,
				   lookup_context);
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  {
    إذا (glyph_شمار_2 <= 1)
      *digest = hb_مجموعه_digest_t::پر ();
    وإلا
      (هذا+coverageZ[1]).collect_coverage (digest);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Lookup_رکورد *lookup_رکورد = &StructAfter<Lookup_رکورد> (coverageZ.as_array (glyph_شمار_2));
    بنية ContextApplyLookupContext lookup_context = {
      {تطابق_coverage},
      هذا
    };
    أرجع context_would_apply_lookup (c,
				       glyph_شمار_2, coverageZ.arrayZ + 1,
				       lookup_شمار_2, lookup_رکورد,
				       lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverageZ[0]; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بیعلامت صحيح شاخص_2 = (هذا+coverageZ[0]).get_coverage (c->میانگیر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت Lookup_رکورد *lookup_رکورد = &StructAfter<Lookup_رکورد> (coverageZ.as_array (glyph_شمار_2));
    بنية ContextApplyLookupContext lookup_context = {
      {تطابق_coverage},
      هذا
    };
    return_trace (context_apply_lookup (c,
					glyph_شمار_2,
					coverageZ.arrayZ + 1,
					lookup_شمار_2, lookup_رکورد,
					lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیرون->format = format;
    بیرون->glyph_شمار_2 = glyph_شمار_2;

    خودکار coverages = coverageZ.as_array (glyph_شمار_2);

    لكل (ثابت خودکار& جابه_جایی : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      خودکار *o = c->serializer->allocate_اندازه<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>> (Types::جابه_جایی_2::static_اندازه);
      إذا (unlikely (!o)) return_trace (خطأ);
      إذا (!o->serialize_subset (c, جابه_جایی, هذا)) return_trace (خطأ);
    }

    ثابت خودکار& lookup_رکورد = StructAfter<Unsized_Array_از<Lookup_رکورد>> (coverageZ.as_array (glyph_شمار_2));
    ثابت hb_map_t *lookup_map = c->جدول_برچسب_2 == HB_OT_برچسب_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    بیعلامت شمار = serialize_lookuprecord_array (c->serializer, lookup_رکورد.as_array (lookup_شمار_2), lookup_map);
    return_trace (c->serializer->check_تخصیص_بده (بیرون->lookup_شمار_2, شمار, HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    بیعلامت صحيح شمار = glyph_شمار_2;
    إذا (unlikely (!شمار)) return_trace (خطأ); /* We want to access coverageZ[0] freely. */
    إذا (unlikely (!c->check_array (coverageZ.arrayZ, شمار))) return_trace (خطأ);
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (unlikely (!coverageZ[i].sanitize (c, هذا))) return_trace (خطأ);
    ثابت Lookup_رکورد *lookup_رکورد = &StructAfter<Lookup_رکورد> (coverageZ.as_array (glyph_شمار_2));
    return_trace (likely (c->check_array (lookup_رکورد, lookup_شمار_2)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_شمار_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_شمار_2;		/* Number of LookupRecords */
  Unsized_Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_اندازه_ARRAY (6, coverageZ);
};

بنية Context
{
  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    حالة 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#پایان_اگر
  } u;
};


/* Chaining Contextual lookups */

بنية Chain_Context_بستگی_Lookup_Context
{
  Context_بستگی_Funcs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_داده[3];
  فراغ *intersects_cache[3];
  فراغ *intersected_glyphs_cache;
};

تعريفنوع Chain_Context_بستگی_Lookup_Context ChainContextDependLookupContext;

بنية ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_داده[3];
};

بنية ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ثابت فراغ *تطابق_داده_2[3];
  get_مقدار_func_t get_درون_داد_مقدار;
};

ساكن مضمن منطقي chain_context_intersects (ثابت hb_مجموعه_t *glyphs,
					     بیعلامت صحيح backtrack_شمار,
					     glyph_داده_t backtrack,
					     بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
					     glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
					     بیعلامت صحيح lookahead_شمار,
					     glyph_داده_t lookahead,
					     Chain_Context_بستگی_Lookup_Context &lookup_context)
{
  منطقي bt_ok = array_is_subset_از (glyphs,
			     backtrack_شمار, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_داده[0],
			     lookup_context.intersects_cache[0]);
  منطقي درون_ok = array_is_subset_از (glyphs,
			     درون_داد_شمار ? درون_داد_شمار - 1 : 0, درون_داد,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_داده[1],
			     lookup_context.intersects_cache[1]);
  منطقي la_ok = array_is_subset_از (glyphs,
			     lookahead_شمار, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_داده[2],
			     lookup_context.intersects_cache[2]);

  أرجع bt_ok && درون_ok && la_ok;
}

ساكن مضمن فراغ chain_context_بستگی_lookup (hb_بستگی_context_t *c,
						 بیعلامت صحيح backtrack_شمار,
						 glyph_داده_t backtrack,
						 بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
						 glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
						 بیعلامت صحيح lookahead_شمار,
						 glyph_داده_t lookahead,
						 بیعلامت صحيح lookup_شمار_2,
						 ثابت Lookup_رکورد lookup_رکورد[],
						 بیعلامت value,
						 Chain_Context_بستگی_Lookup_Context &lookup_context)
{
  إذا (chain_context_intersects (c->glyphs,
				backtrack_شمار, backtrack,
				درون_داد_شمار, درون_داد,
				lookahead_شمار, lookahead,
				lookup_context))
    context_بستگی_recurse_lookups (c,
		     درون_داد_شمار, درون_داد,
		     lookup_شمار_2, lookup_رکورد,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_داده[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ chain_context_depend_lookup (hb_depend_context_t *c,
						بیعلامت صحيح backtrack_شمار,
						glyph_داده_t backtrack,
						بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
						glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
						بیعلامت صحيح lookahead_شمار,
						glyph_داده_t lookahead,
						بیعلامت صحيح lookup_شمار_2,
						ثابت Lookup_رکورد lookup_رکورد[],
						ثابت فراغ *قاعده_2,
						بیعلامت value,
						ChainContextDependLookupContext &lookup_context)
{
  إذا (!chain_context_intersects (c->glyphs,
				 backtrack_شمار, backtrack,
				 درون_داد_شمار, درون_داد,
				 lookahead_شمار, lookahead,
				 lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, درون_داد_شمار, lookup_شمار_2, lookup_رکورد, قاعده_2, value,
    lookup_context.context_format, lookup_context.intersects_داده[1],
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_داده->successful))
    أرجع;
  إذا (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    درون_داد_شمار, درون_داد,
				    lookup_شمار_2, lookup_رکورد,
				    قاعده_2,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_داده[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    اشاره_تهی,
				    &scratch->cached_context_sets,
				    scratch);
    أرجع;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_مجموعه_t &position_scratch = scratch->position_scratch;
  hb_وکتور_t<ثابت hb_مجموعه_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_وکتور_t<ثابت hb_مجموعه_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  لكل (بیعلامت i = 0; i < backtrack_شمار; i++)
  {
    ثابت hb_مجموعه_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_داده[0],
      backtrack[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!backtrack_sets.افزودن_or_fail (pos_glyphs)))
      {
        c->depend_داده->fail ();
        أرجع;
      }
    }
  }

  /* Extract lookahead glyphs */
  لكل (بیعلامت i = 0; i < lookahead_شمار; i++)
  {
    ثابت hb_مجموعه_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_داده[2],
      lookahead[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!lookahead_sets.افزودن_or_fail (pos_glyphs)))
      {
        c->depend_داده->fail ();
        أرجع;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_مجموعه_t &preliminary_context = scratch->preliminary_context;
  لكل (ثابت hb_مجموعه_t *پشت_مجموعه : backtrack_sets)
  {
    إذا (پشت_مجموعه->get_population () == 1) {
      preliminary_context.جمع_کن (پشت_مجموعه->get_min ());  // Direct glyph
    } وإلا إذا (پشت_مجموعه->get_population () > 1) {
      hb_codepoint_t مجموعه_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_داده->پیدا_کن_or_create_context_مجموعه (*پشت_مجموعه)
        : c->پیدا_کن_or_create_cached_context_مجموعه (پشت_مجموعه);
      إذا (unlikely (مجموعه_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.جمع_کن (HB_DEPEND_CONTEXT_مجموعه_FLAG | مجموعه_idx);  // Encoded set reference
    }
  }
  لكل (ثابت hb_مجموعه_t *look_مجموعه : lookahead_sets)
  {
    إذا (look_مجموعه->get_population () == 1) {
      preliminary_context.جمع_کن (look_مجموعه->get_min ());  // Direct glyph
    } وإلا إذا (look_مجموعه->get_population () > 1) {
      hb_codepoint_t مجموعه_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_داده->پیدا_کن_or_create_context_مجموعه (*look_مجموعه)
        : c->پیدا_کن_or_create_cached_context_مجموعه (look_مجموعه);
      إذا (unlikely (مجموعه_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.جمع_کن (HB_DEPEND_CONTEXT_مجموعه_FLAG | مجموعه_idx);  // Encoded set reference
    }
  }
  إذا (unlikely (preliminary_context.درون_error ()))
  {
    c->depend_داده->fail ();
    أرجع;
  }

  /* Build glyph sets for ALL input positions */
  hb_وکتور_t<ثابت hb_مجموعه_t *> &درون_داد_position_glyphs = scratch->درون_داد_position_glyphs;

  إذا (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ثابت hb_مجموعه_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_داده[1], value,
      درست, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !درون_داد_position_glyphs.افزودن_or_fail (pos0_glyphs)))
    {
      c->depend_داده->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (بیعلامت i = 0; i + 1 < درون_داد_شمار; i++)
    {
      ثابت hb_مجموعه_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_داده[1],
	درون_داد[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !درون_داد_position_glyphs.افزودن_or_fail (pos_glyphs)))
      {
	c->depend_داده->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  درون_داد_شمار, درون_داد,
				  lookup_شمار_2, lookup_رکورد,
				  قاعده_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_داده[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? اشاره_تهی : &درون_داد_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : اشاره_تهی,
				  scratch);
}

ساكن مضمن فراغ chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							بیعلامت صحيح backtrack_شمار,
							glyph_داده_t backtrack,
							بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
							glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
							بیعلامت صحيح lookahead_شمار,
							glyph_داده_t lookahead,
							بیعلامت صحيح lookup_شمار_2,
							ثابت Lookup_رکورد lookup_رکورد[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_شمار, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_داده[0]);
  collect_array (c, c->درون_داد,
		 درون_داد_شمار ? درون_داد_شمار - 1 : 0, درون_داد,
		 lookup_context.funcs.collect, lookup_context.collect_داده[1]);
  collect_array (c, c->after,
		 lookahead_شمار, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_داده[2]);
  recurse_lookups (c,
		   lookup_شمار_2, lookup_رکورد);
}

ساكن مضمن منطقي chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     بیعلامت صحيح backtrack_شمار,
						     glyph_داده_t backtrack HB_استفادهنشده,
						     بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
						     glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
						     بیعلامت صحيح lookahead_شمار,
						     glyph_داده_t lookahead HB_استفادهنشده,
						     بیعلامت صحيح lookup_شمار_2 HB_استفادهنشده,
						     ثابت Lookup_رکورد lookup_رکورد[] HB_استفادهنشده,
						     ثابت ChainContextApplyLookupContext &lookup_context)
{
  أرجع (c->zero_context ? !backtrack_شمار && !lookahead_شمار : درست)
      && would_تطابق_درون_داد (c,
			    درون_داد_شمار, درون_داد,
			    lookup_context.funcs.تطابق[1], lookup_context.تطابق_داده_2[1]);
}

ساكن مضمن منطقي chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       بیعلامت صحيح backtrack_شمار,
					       glyph_داده_t backtrack,
					       بیعلامت صحيح درون_داد_شمار, /* Including the first glyph (not matched) */
					       glyph_داده_t درون_داد, /* Array of input values--start with second glyph */
					       بیعلامت صحيح lookahead_شمار,
					       glyph_داده_t lookahead,
					       بیعلامت صحيح lookup_شمار_2,
					       ثابت Lookup_رکورد lookup_رکورد[],
					       ثابت ChainContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (درون_داد_شمار > HB_MAX_CONTEXT_درازی)) أرجع خطأ;

  بیعلامت شروع_شاخص = c->میانگیر->بیرون_len;
  بیعلامت انتها_شاخص = c->میانگیر->idx;
  بیعلامت تطابق_انتها = 0;
  منطقي ret = درست;
  إذا (!(تطابق_درون_داد (c,
		     درون_داد_شمار, درون_داد,
		     lookup_context.funcs.تطابق[1], lookup_context.تطابق_داده_2[1],
		     &تطابق_انتها) && (انتها_شاخص = تطابق_انتها)
       && تطابق_lookahead (c,
			   lookahead_شمار, lookahead,
			   lookup_context.funcs.تطابق[2], lookup_context.تطابق_داده_2[2],
			   تطابق_انتها, &انتها_شاخص)))
  {
    c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, انتها_شاخص);
    أرجع خطأ;
  }

  إذا (!تطابق_backtrack (c,
			backtrack_شمار, backtrack,
			lookup_context.funcs.تطابق[0], lookup_context.تطابق_داده_2[0],
			&شروع_شاخص))
  {
    c->میانگیر->unsafe_مقصد_concat_from_outbuffer (شروع_شاخص, انتها_شاخص);
    أرجع خطأ;
  }

  c->میانگیر->unsafe_مقصد_break_from_outbuffer (شروع_شاخص, انتها_شاخص);
  apply_lookup (c,
		درون_داد_شمار,
		lookup_شمار_2, lookup_رکورد,
		تطابق_انتها);

  أرجع ret;
}

قالب <نام_نوع Types>
بنية Chain_قاعده
{
  قالب <نام_نوع مجموعه_Types, نام_نوع Chain_قاعده_Type>
  دوست بنية Chain_قاعده_مجموعه_از;

  منطقي intersects (ثابت hb_مجموعه_t *glyphs, Chain_Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     درون_داد.lenP1, درون_داد.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, بیعلامت value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 درون_داد.lenP1, درون_داد.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 value,
				 lookup_context);
  }

  فراغ بستگی (hb_بستگی_context_t *c, بیعلامت value,
		Chain_Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_بستگی_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  درون_داد.lenP1, درون_داد.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c,
                        Chain_Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 درون_داد.lenP1, درون_داد.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest,
			      ثابت ClassDef *درون_داد_class_def = اشاره_تهی,
			      ثابت ClassDef *lookahead_class_def = اشاره_تهی) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);

    بیعلامت second;
    ثابت ClassDef *class_def;
    إذا (درون_داد.lenP1 > 1)
    {
      second = درون_داد.arrayZ[0];
      class_def = درون_داد_class_def;
    }
    وإلا إذا (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    وإلا
    {
      *digest = hb_مجموعه_digest_t::پر ();
      أرجع;
    }

    إذا (!class_def)
      digest->جمع_کن (second);
    وإلا إذا (!second)
      *digest = hb_مجموعه_digest_t::پر ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     درون_داد.lenP1, درون_داد.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      درون_داد.lenP1, درون_داد.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ثابت
  {
    c->copy (len);
    لكل (ثابت خودکار g : it)
    {
      نام_نوع Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *lookup_map,
		  ثابت hb_map_t *backtrack_map,
		  ثابت hb_map_t *درون_داد_map = اشاره_تهی,
		  ثابت hb_map_t *lookahead_map = اشاره_تهی) ثابت
  {
    TRACE_SERIALIZE (هذا);

    ثابت hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    إذا (درون_داد_map) mapping = درون_داد_map;
    serialize_array (c, درون_داد.lenP1, + درون_داد.iter ()
				     | hb_map (mapping));

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    إذا (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_شمار_2 = c->embed (&(lookup.len));
    إذا (!lookup_شمار_2) return_trace (خطأ);

    بیعلامت شمار = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_تخصیص_بده (*lookup_شمار_2, شمار, HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_map = اشاره_تهی,
	       ثابت hb_map_t *درون_داد_map = اشاره_تهی,
	       ثابت hb_map_t *lookahead_map = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);

    إذا (!backtrack_map)
    {
      ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
      إذا (!hb_all (backtrack, glyphset) ||
	  !hb_all (درون_داد, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    وإلا
    {
      إذا (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (درون_داد, درون_داد_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, backtrack_map, درون_داد_map, lookahead_map);
    }

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Hyper-optimized sanitized because this is really hot. */
    إذا (unlikely (!backtrack.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    إذا (unlikely (!درون_داد.lenP1.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    إذا (unlikely (!lookahead.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  Array16_از<نام_نوع Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  Headless_Array16_از<نام_نوع Types::HBUINT>
		درون_داد_X;			/* Array of input values (start with
					 * second glyph) */
  Array16_از<نام_نوع Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16_از<Lookup_رکورد>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_اندازه_MIN (8);
};

قالب <نام_نوع Types, نام_نوع Chain_قاعده_Type>
بنية Chain_قاعده_مجموعه_از
{
  منطقي intersects (ثابت hb_مجموعه_t *glyphs, Chain_Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_map ([&] (ثابت Chain_قاعده_Type &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_اولی_درون_داد_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (بیعلامت i = 0; i < قاعده_2.len; i++)
    {
      ثابت Chain_قاعده_Type &r = هذا+قاعده_2.arrayZ[i];
      ثابت خودکار &درون_داد = StructAfter<decltype (r.درون_داد_X)> (r.backtrack);
      إذا (درون_داد.lenP1 <= 1)
      {
	digest->مجموعه_پر ();
	أرجع;
      }
      digest->جمع_کن (درون_داد.arrayZ[0]);
    }
  }
  فراغ depend (hb_depend_context_t *c, بیعلامت value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }
  فراغ بستگی (hb_بستگی_context_t *c, بیعلامت value, Chain_Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_Type &_) { _.بستگی (c, value, lookup_context); })
    ;
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c,
                        Chain_Context_بستگی_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_Type &_) { _.بستگی_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest,
			      ثابت ClassDef *درون_داد_class_def = اشاره_تهی,
			      ثابت ClassDef *lookahead_class_def = اشاره_تهی) ثابت
  {
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([=] (ثابت Chain_قاعده_Type &_)
		{ _.collect_دومی_glyphs (digest, درون_داد_class_def, lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_map ([&] (ثابت Chain_قاعده_Type &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = اشاره_تهی) ثابت
  {
    TRACE_APPLY (هذا);

    بیعلامت رقم_قواعد = قاعده_2.len;

#اگر_تعریف_نشده HB_NO_OT_RULESETS_FAST_مسیر
    إذا (HB_OPTIMIZE_اندازه_VAL || رقم_قواعد <= 4)
#پایان_اگر
    {
    slow:
      return_trace (
      + hb_iter (قاعده_2)
      | hb_map (hb_جمع_کن (هذا))
      | hb_map ([&] (ثابت Chain_قاعده_Type &_) { أرجع _.apply (c, lookup_context); })
      | hb_any
      )
      ;
    }

    /* This version is optimized for speed by matching the first & second
     * components of the rule here, instead of calling into the matching code.
     *
     * Replicated from LigatureSet::apply(). */

    /* We use the iter_context instead of iter_input, to avoid skipping
     * default-ignorables and such.
     *
     * Related: https://github.com/harfbuzz/harfbuzz/issues/4813
     */
    خودکار &skippy_iter = c->iter_context;
    skippy_iter.reset (c->میانگیر->idx);
    skippy_iter.مجموعه_تطابق_func (تطابق_always, اشاره_تهی);
    skippy_iter.مجموعه_glyph_داده ((HBUINT16 *) اشاره_تهی);
    بیعلامت unsafe_مقصد = (بیعلامت) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *اولی = اشاره_تهی, *second = اشاره_تهی;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->میانگیر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      اولی = &c->میانگیر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_درون_داد_مقدار &&
	  !digest->may_have (lookup_context.get_درون_داد_مقدار (*اولی, lookup_context.تطابق_داده_2[1])))
	{
	  c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_مقصد_concat = خطأ;
      منطقي ret =
      + hb_iter (قاعده_2)
      | hb_map (hb_جمع_کن (هذا))
      | hb_filter ([&] (ثابت Chain_قاعده_Type &_)
		   {
		     ثابت خودکار &درون_داد = StructAfter<decltype (_.درون_داد_X)> (_.backtrack);
		     ثابت خودکار &lookahead = StructAfter<decltype (_.lookaheadX)> (درون_داد);
		     إذا (درون_داد.lenP1 <= 1 && lookahead.len == 0) أرجع درست;
		     unsafe_مقصد_concat = درست;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت Chain_قاعده_Type &_) { أرجع _.apply (c, lookup_context); })
      | hb_any
      ;
      إذا (unsafe_مقصد_concat)
	c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->میانگیر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      second = &c->میانگیر->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    خودکار تطابق_درون_داد = lookup_context.funcs.تطابق[1];
    خودکار تطابق_lookahead = lookup_context.funcs.تطابق[2];
    خودکار *درون_داد_داده = lookup_context.تطابق_داده_2[1];
    خودکار *lookahead_داده = lookup_context.تطابق_داده_2[2];
    لكل (بیعلامت صحيح i = 0; i < رقم_قواعد; i++)
    {
      ثابت خودکار &r = هذا+قاعده_2.arrayZ[i];

      ثابت خودکار &درون_داد = StructAfter<decltype (r.درون_داد_X)> (r.backtrack);
      ثابت خودکار &lookahead = StructAfter<decltype (r.lookaheadX)> (درون_داد);

      بیعلامت lenP1 = درون_داد.lenP1;
      إذا (lenP1 > 1 ?
	   (!تطابق_درون_داد ||
	    تطابق_درون_داد (*اولی, درون_داد.arrayZ[0], درون_داد_داده))
	  :
	   (!lookahead.len || !تطابق_lookahead ||
	    تطابق_lookahead (*اولی, lookahead.arrayZ[0], lookahead_داده)))
      {
	lenP1 = hb_max (lenP1, 1u);
        إذا (!second ||
	    (lenP1 > 2 ?
	     (!تطابق_درون_داد ||
	      تطابق_درون_داد (*second, درون_داد.arrayZ[1], درون_داد_داده))
	     :
	     (lookahead.len <= 2 - lenP1 || !تطابق_lookahead ||
	      تطابق_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_داده))))
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_مقصد != (بیعلامت) -1)
	      c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, unsafe_مقصد);
	    return_trace (درست);
	  }
	}
	وإلا
	  unsafe_مقصد = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_مقصد == (بیعلامت) -1)
	  unsafe_مقصد = unsafe_to1;

	إذا (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  لكل (; i + 1 < رقم_قواعد; i++)
	  {
	    ثابت خودکار &r2 = هذا+قاعده_2.arrayZ[i + 1];
	    ثابت خودکار &input2 = StructAfter<decltype (r2.درون_داد_X)> (r2.backtrack);
	    إذا (input2.lenP1 <= 1 || input2.arrayZ[0] != درون_داد.arrayZ[0])
	      اكسر;
	  }
	}
      }
    }
    إذا (likely (unsafe_مقصد != (بیعلامت) -1))
      c->میانگیر->unsafe_مقصد_concat (c->میانگیر->idx, unsafe_مقصد);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_klass_map = اشاره_تهی,
	       ثابت hb_map_t *درون_داد_klass_map = اشاره_تهی,
	       ثابت hb_map_t *lookahead_klass_map = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار snap = c->serializer->snapshot ();
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    لكل (ثابت خودکار& _ : قاعده_2)
    {
      إذا (!_) استمر;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = بیرون->قاعده_2.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا,
				lookup_map,
				backtrack_klass_map,
				درون_داد_klass_map,
				lookahead_klass_map))
      {
	بیرون->قاعده_2.برداشت ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (بیرون->قاعده_2);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (قاعده_2.sanitize (c, هذا));
  }

  محمي:
  Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Chain_قاعده_Type>>
		قاعده_2;			/* Array of ChainRule tables
					 * ordered by preference */
  عام:
  DEFINE_اندازه_ARRAY (2, قاعده_2);
};

قالب <نام_نوع Types>
بنية Chain_قاعده_مجموعه : Chain_قاعده_مجموعه_از<Types, Chain_قاعده<Types>> {};

static_assert (حجم (Chain_قاعده_مجموعه<MediumTypes>) ==
	       HBUINT16::static_اندازه + Offset24::static_اندازه, "");

قالب <نام_نوع Types>
بنية Chain_Class_قاعده_مجموعه : Chain_قاعده_مجموعه_از<Types, Chain_قاعده<SmallTypes>> {};

قالب <نام_نوع Types>
بنية ChainContextFormat1_4
{
  استفاده Chain_قاعده_مجموعه = OT::Chain_قاعده_مجموعه<Types>;

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    بنية Chain_Context_بستگی_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {اشاره_تهی, اشاره_تهی, اشاره_تهی}
    };

    أرجع
    + hb_zip (هذا+coverage, قاعده_مجموعه_2)
    | hb_filter (*glyphs, hb_اولی)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_map ([&] (ثابت Chain_قاعده_مجموعه &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع درست; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (*c->glyphs, *cur_فعال_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {اشاره_تهی, اشاره_تهی, اشاره_تهی},
      {اشاره_تهی, اشاره_تهی, اشاره_تهی},  /* intersects_cache */
      اشاره_تهی                        /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((بیعلامت) قاعده_مجموعه_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->والد_فعال_glyphs ().has (_);
    }, hb_اولی)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بیعلامت> _) { أرجع hb_pair_t<بیعلامت, ثابت Chain_قاعده_مجموعه&> (_.اولی, هذا+قاعده_مجموعه_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بیعلامت, ثابت Chain_قاعده_مجموعه&>& _) { _.second.depend (c, _.اولی, lookup_context); })
    ;

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (c->قبلی_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    بنية Chain_Context_بستگی_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {اشاره_تهی, اشاره_تهی, اشاره_تهی}
    };

    + hb_zip (هذا+coverage, hb_range ((بیعلامت) قاعده_مجموعه_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->قبلی_والد_فعال_glyphs ().has (_);
    }, hb_اولی)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بیعلامت> _) { أرجع hb_pair_t<بیعلامت, ثابت Chain_قاعده_مجموعه&> (_.اولی, هذا+قاعده_مجموعه_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بیعلامت, ثابت Chain_قاعده_مجموعه&>& _) { _.second.بستگی (c, _.اولی, lookup_context); })
    ;

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت
  {
    بنية Chain_Context_بستگی_Lookup_Context lookup_context = {
      {intersects_glyph, اشاره_تهی},
      ContextFormat::SimpleContext,
      {اشاره_تهی, اشاره_تهی, اشاره_تهی}
    };

    + hb_zip (هذا+coverage, قاعده_مجموعه_2)
    | hb_filter (*c->glyphs, hb_اولی)
    | hb_map (hb_دومی)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_مجموعه &_) { _.بستگی_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->درون_داد);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {اشاره_تهی, اشاره_تهی, اشاره_تهی}
    };

    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_مجموعه &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  {
    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([digest] (ثابت Chain_قاعده_مجموعه &_) { _.collect_دومی_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Chain_قاعده_مجموعه &قاعده_مجموعه_3 = هذا+قاعده_مجموعه_2[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{تطابق_glyph, تطابق_glyph, تطابق_glyph}},
      {اشاره_تهی, اشاره_تهی, اشاره_تهی}
    };
    أرجع قاعده_مجموعه_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->میانگیر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت Chain_قاعده_مجموعه &قاعده_مجموعه_3 = هذا+قاعده_مجموعه_2[شاخص_2];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{تطابق_glyph, تطابق_glyph, تطابق_glyph}},
      {اشاره_تهی, اشاره_تهی, اشاره_تهی}
    };
    return_trace (قاعده_مجموعه_3.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_برچسب_2 == HB_OT_برچسب_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, قاعده_مجموعه_2)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (subset_جابه_جایی_array (c, بیرون->قاعده_مجموعه_2, هذا, lookup_map), hb_دومی)
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بیرون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && قاعده_مجموعه_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  نام_نوع Types::قالب Array_از<نام_نوع Types::قالب جابه_جایی_مقصد<Chain_قاعده_مجموعه>>
		قاعده_مجموعه_2;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, قاعده_مجموعه_2);
};

قالب <نام_نوع Types>
بنية ChainContextFormat2_5
{
  استفاده Chain_قاعده_مجموعه = OT::Chain_Class_قاعده_مجموعه<Types>;

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = هذا+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية Chain_Context_بستگی_Lookup_Context lookup_context = {
      {intersects_class, اشاره_تهی},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_مجموعه_t retained_coverage_glyphs;
    (هذا+coverage).intersect_مجموعه (*glyphs, retained_coverage_glyphs);

    hb_مجموعه_t coverage_glyph_classes;
    درون_داد_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    أرجع
    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<بیعلامت, ثابت Chain_قاعده_مجموعه &> p)
	      { أرجع درون_داد_class_def.intersects_class (glyphs, p.اولی) &&
		       coverage_glyph_classes.has (p.اولی) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع درست; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (*c->glyphs, *cur_فعال_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = هذا+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعده_مجموعه_2)
    | hb_filter ([&] (بیعلامت _)
    { أرجع درون_داد_class_def.intersects_class (&c->والد_فعال_glyphs (), _); },
		 hb_اولی)
    | hb_apply ([&] (ثابت hb_pair_t<بیعلامت, ثابت نام_نوع Types::قالب جابه_جایی_مقصد<Chain_قاعده_مجموعه>&> _)
                {
                  ثابت Chain_قاعده_مجموعه& chainrule_مجموعه = هذا+_.second;
                  chainrule_مجموعه.depend (c, _.اولی, lookup_context);
                })
    ;

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_مجموعه (c->قبلی_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = هذا+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    بنية Chain_Context_بستگی_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعده_مجموعه_2)
    | hb_filter ([&] (بیعلامت _)
    { أرجع درون_داد_class_def.intersects_class (&c->والد_فعال_glyphs (), _); },
		 hb_اولی)
    | hb_apply ([&] (ثابت hb_pair_t<بیعلامت, ثابت نام_نوع Types::قالب جابه_جایی_مقصد<Chain_قاعده_مجموعه>&> _)
                {
                  ثابت Chain_قاعده_مجموعه& chainrule_مجموعه = هذا+_.second;
                  chainrule_مجموعه.بستگی (c, _.اولی, lookup_context);
                })
    ;

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = هذا+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية Chain_Context_بستگی_Lookup_Context lookup_context = {
      {intersects_class, اشاره_تهی},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_enumerate
    | hb_filter([&] (بیعلامت klass)
    { أرجع درون_داد_class_def.intersects_class (c->glyphs, klass); }, hb_اولی)
    | hb_map (hb_دومی)
    | hb_apply ([&] (ثابت Chain_قاعده_مجموعه &_)
    { _.بستگی_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->درون_داد);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = هذا+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def}
    };

    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_مجموعه &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  {
    ثابت ClassDef &درون_داد_class_def = هذا+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;
    + hb_iter (قاعده_مجموعه_2)
    | hb_map (hb_جمع_کن (هذا))
    | hb_apply ([digest, &درون_داد_class_def, &lookahead_class_def] (ثابت Chain_قاعده_مجموعه &_)
		{ _.collect_دومی_glyphs (digest, &درون_داد_class_def, &lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = هذا+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بیعلامت صحيح شاخص_2 = درون_داد_class_def.get_class (c->glyphs[0]);
    ثابت Chain_قاعده_مجموعه &قاعده_مجموعه_3 = هذا+قاعده_مجموعه_2[شاخص_2];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{تطابق_class, تطابق_class, تطابق_class}},
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def}
    };
    أرجع قاعده_مجموعه_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بیعلامت cache_هزینه () ثابت
  {
    أرجع (هذا+درون_داد_Class_Def).هزینه () + (هذا+lookaheadClassDef).هزینه ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_دودویی_cache_t coverage;
    hb_ot_layout_mapping_cache_t درون_داد_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t قاعده_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بیعلامت شمار = قاعده_مجموعه_2.len;
    بیعلامت اندازه = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    شمار * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (اندازه);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->درون_داد_class.clear ();
      cache->lookahead_class.clear ();
      لكل (بیعلامت i = 0; i < شمار; i++)
	(هذا+قاعده_مجموعه_2[i]).collect_اولی_درون_داد_classes (&cache->قاعده_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, درست, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage_دودویی (c->میانگیر->cur().codepoint, cache ? &cache->coverage : اشاره_تهی);
#وإلا
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->میانگیر->cur().codepoint);
#پایان_اگر
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = هذا+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    تطابق_class_cache_داده_t درون_داد_cache_داده = {
      &درون_داد_class_def,
      cache ? &cache->درون_داد_class : اشاره_تهی
    };
    تطابق_class_cache_داده_t lookahead_cache_داده = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : اشاره_تهی
    };
    ثابت فراغ *درون_داد_تطابق_داده = cached
      ? static_cast<ثابت فراغ *> (&درون_داد_class_def)
      : static_cast<ثابت فراغ *> (&درون_داد_cache_داده);
    ثابت فراغ *lookahead_تطابق_داده = cached
      ? static_cast<ثابت فراغ *> (&lookahead_class_def)
      : static_cast<ثابت فراغ *> (&lookahead_cache_داده);
    منطقي backtrack_استفاده_میکند_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ثابت فراغ *backtrack_تطابق_داده = backtrack_استفاده_میکند_lookahead_cache
      ? static_cast<ثابت فراغ *> (&lookahead_cache_داده)
      : static_cast<ثابت فراغ *> (&backtrack_class_def);
#پایان_اگر

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    بنية ChainContextApplyLookupContext lookup_context = {
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? تطابق_class_cached1 :
        backtrack_استفاده_میکند_lookahead_cache ? تطابق_class_cache : تطابق_class,
        cached ? تطابق_class_cached2 : تطابق_class_cache,
        cached ? تطابق_class_cached1 : تطابق_class_cache}},
      {backtrack_تطابق_داده,
       درون_داد_تطابق_داده,
       lookahead_تطابق_داده},
      cached ? get_class_cached2_مقدار : get_class_cache_مقدار
#وإلا
      {{cached && &backtrack_class_def == &lookahead_class_def ? تطابق_class_cached1 : تطابق_class,
        cached ? تطابق_class_cached2 : تطابق_class,
        cached ? تطابق_class_cached1 : تطابق_class}},
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_مقدار : get_class_مقدار
#پایان_اگر
    };

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    شاخص_2 = cached
         ? get_class_cached2 (درون_داد_class_def, c->میانگیر->cur())
         : درون_داد_class_def.get_class (c->میانگیر->cur().codepoint, درون_داد_cache_داده.cache);
#وإلا
    شاخص_2 = cached
         ? get_class_cached2 (درون_داد_class_def, c->میانگیر->cur())
          : درون_داد_class_def.get_class (c->میانگیر->cur().codepoint);
#پایان_اگر
    ثابت Chain_قاعده_مجموعه &قاعده_مجموعه_3 = هذا+قاعده_مجموعه_2[شاخص_2];
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && شاخص_2 < قاعده_مجموعه_2.len ? &cache->قاعده_sets[شاخص_2] : اشاره_تهی;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = اشاره_تهی;
#پایان_اگر
    return_trace (قاعده_مجموعه_3.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;
    بیرون->coverage.serialize_subset (c, coverage, هذا);

    hb_map_t backtrack_klass_map;
    hb_map_t درون_داد_klass_map;
    hb_map_t lookahead_klass_map;

    بیرون->backtrackClassDef.serialize_subset (c, backtrackClassDef, هذا, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    بیرون->درون_داد_Class_Def.serialize_subset (c, درون_داد_Class_Def, هذا, &درون_داد_klass_map);
    بیرون->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, هذا, &lookahead_klass_map);

    إذا (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   درون_داد_klass_map,
						   lookahead_klass_map)))
      return_trace (خطأ);

    ثابت hb_مجموعه_t* glyphset = c->plan->glyphset_gsub ();
    hb_مجموعه_t retained_coverage_glyphs;
    (هذا+coverage).intersect_مجموعه (*glyphset, retained_coverage_glyphs);

    hb_مجموعه_t coverage_glyph_classes;
    (هذا+درون_داد_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    صحيح non_zero_شاخص = -1, شاخص_2 = 0;
    منطقي ret = درست;
    ثابت hb_map_t *lookup_map = c->جدول_برچسب_2 == HB_OT_برچسب_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    خودکار آخرین_non_zero = c->serializer->snapshot ();
    لكل (ثابت خودکار& _ : + hb_enumerate (قاعده_مجموعه_2)
			 | hb_filter (درون_داد_klass_map, hb_اولی))
    {
      خودکار *o = بیرون->قاعده_مجموعه_2.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }
      إذا (coverage_glyph_classes.has (_.اولی) &&
          o->serialize_subset (c, _.second, هذا,
			       lookup_map,
			       &backtrack_klass_map,
			       &درون_داد_klass_map,
			       &lookahead_klass_map))
      {
        آخرین_non_zero = c->serializer->snapshot ();
	non_zero_شاخص = شاخص_2;
      }

      شاخص_2++;
    }

    إذا (!ret || non_zero_شاخص == -1) return_trace (خطأ);

    // prune empty trailing ruleSets
    إذا (شاخص_2 > non_zero_شاخص) {
      c->serializer->revert (آخرین_non_zero);
      بیرون->قاعده_مجموعه_2.len = non_zero_شاخص + 1;
    }

    return_trace (منطقي (بیرون->قاعده_مجموعه_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) &&
		  backtrackClassDef.sanitize (c, هذا) &&
		  درون_داد_Class_Def.sanitize (c, هذا) &&
		  lookaheadClassDef.sanitize (c, هذا) &&
		  قاعده_مجموعه_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  نام_نوع Types::قالب LOffset_مقصد<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  نام_نوع Types::قالب LOffset_مقصد<ClassDef>
		درون_داد_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  نام_نوع Types::قالب LOffset_مقصد<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Chain_قاعده_مجموعه>>
		قاعده_مجموعه_2;		/* Array of ChainRuleSet tables
					 * ordered by class */
  عام:
  DEFINE_اندازه_ARRAY (4 + 4 * Types::LOffset::static_اندازه, قاعده_مجموعه_2);
};

قالب <نام_نوع Types>
بنية ChainContextFormat3
{
  استفاده قاعده_مجموعه = OT::قاعده_مجموعه<SmallTypes>;

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    إذا (!(هذا+درون_داد[0]).intersects (glyphs))
      أرجع خطأ;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    بنية Chain_Context_بستگی_Lookup_Context lookup_context = {
      {intersects_coverage, اشاره_تهی},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     درون_داد.len, درون_داد.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع درست; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    إذا (!(هذا+درون_داد[0]).intersects (c->glyphs))
      أرجع;

    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs))
      أرجع;
    get_coverage ().intersect_مجموعه (*c->glyphs, *cur_فعال_glyphs);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا},
      {اشاره_تهی, اشاره_تهی, اشاره_تهی},  /* intersects_cache */
      اشاره_تهی                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ثابت HBUINT16 *) backtrack.arrayZ,
				 درون_داد.len, (ثابت HBUINT16 *) درون_داد.arrayZ + 1,
				 lookahead.len, (ثابت HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 درون_داد[0], lookup_context);

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    إذا (!(هذا+درون_داد[0]).intersects (c->glyphs))
      أرجع;

    hb_مجموعه_t* cur_فعال_glyphs = c->افزودن_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs))
      أرجع;
    get_coverage ().intersect_مجموعه (c->قبلی_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية Chain_Context_بستگی_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    chain_context_بستگی_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  درون_داد.len, درون_داد.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->برداشت_cur_انجامشده_glyphs ();
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;

    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    (هذا+درون_داد[0]).collect_coverage (c->درون_داد);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {هذا, هذا, هذا}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 درون_داد.len, درون_داد.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    إذا (درون_داد.len > 1)
      (هذا+درون_داد[1]).collect_coverage (digest);
    وإلا إذا (lookahead.len)
      (هذا+lookahead[0]).collect_coverage (digest);
    وإلا
      *digest = hb_مجموعه_digest_t::پر ();
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{تطابق_coverage, تطابق_coverage, تطابق_coverage}},
      {هذا, هذا, هذا}
    };
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     درون_داد.len, درون_داد.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت
  {
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    أرجع هذا+درون_داد[0];
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    بیعلامت صحيح شاخص_2 = (هذا+درون_داد[0]).get_coverage (c->میانگیر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{تطابق_coverage, تطابق_coverage, تطابق_coverage}},
      {هذا, هذا, هذا}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      درون_داد.len, درون_داد.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ثابت فراغ* پایه) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->serializer->شروع_embed<Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>> ();
    إذا (unlikely (!بیرون->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودکار& جابه_جایی : it) {
      خودکار *o = بیرون->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, جابه_جایی, پایه))
        return_trace (خطأ);
    }

    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);

    إذا (!serialize_coverage_offsets (c, backtrack.iter (), هذا))
      return_trace (خطأ);

    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    إذا (!serialize_coverage_offsets (c, درون_داد.iter (), هذا))
      return_trace (خطأ);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    إذا (!serialize_coverage_offsets (c, lookahead.iter (), هذا))
      return_trace (خطأ);

    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ثابت hb_map_t *lookup_map = c->جدول_برچسب_2 == HB_OT_برچسب_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_شمار_2 = c->serializer->copy<HBUINT16> (lookup.len);
    إذا (!lookup_شمار_2) return_trace (خطأ);

    بیعلامت شمار = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_تخصیص_بده (*lookup_شمار_2, شمار, HB_SERIALIZE_ERROR_INT_سرریز));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!backtrack.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    إذا (unlikely (!درون_داد.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!درون_داد.len)) return_trace (خطأ); /* To be consistent with Context. */
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    إذا (unlikely (!lookahead.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>
		درون_داد_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16_از<نام_نوع Types::قالب جابه_جایی_مقصد<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16_از<Lookup_رکورد>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_اندازه_MIN (10);
};

بنية ChainContext
{
  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#پایان_اگر
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#پایان_اگر
  } u;
};


قالب <نام_نوع T>
بنية ExtensionFormat1
{
  بیعلامت صحيح get_type () ثابت { أرجع extensionLookupType; }

  قالب <نام_نوع X>
  ثابت X& get_subtable () ثابت
  { أرجع هذا + reinterpret_cast<ثابت Offset32_مقصد<نام_نوع T::Sub_جدول> &> (extension_جابه_جایی); }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, هذا))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, format);
    return_trace (get_subtable<نام_نوع T::Sub_جدول> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  extensionLookupType != T::Sub_جدول::Extension);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیرون->format = format;
    بیرون->extensionLookupType = extensionLookupType;

    ثابت خودکار& src_جابه_جایی =
        reinterpret_cast<ثابت Offset32_مقصد<نام_نوع T::Sub_جدول> &> (extension_جابه_جایی);
    خودکار& dest_جابه_جایی =
        reinterpret_cast<Offset32_مقصد<نام_نوع T::Sub_جدول> &> (بیرون->extension_جابه_جایی);

    return_trace (dest_جابه_جایی.serialize_subset (c, src_جابه_جایی, هذا, get_type ()));
  }

  محمي:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extension_جابه_جایی;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

قالب <نام_نوع T>
بنية Extension
{
  بیعلامت صحيح get_type () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_type ();
    افتراضي:أرجع 0;
    }
  }
  قالب <نام_نوع X>
  ثابت X& get_subtable () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.قالب get_subtable<نام_نوع T::Sub_جدول> ();
    افتراضي:أرجع Null (نام_نوع T::Sub_جدول);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  قالب <نام_نوع ...Ts>
  نام_نوع hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.subset (c);
    افتراضي: أرجع c->default_return_مقدار ();
    }
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

بنية hb_ot_layout_lookup_accelerator_t
{
  قالب <نام_نوع TLookup>
  ساكن hb_ot_layout_lookup_accelerator_t *create (ثابت TLookup &lookup)
  {
    بیعلامت شمار = lookup.get_subtable_شمار ();

    بیعلامت اندازه = حجم (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    شمار * حجم (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خودکار *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, اندازه);
    إذا (unlikely (!thiz))
      أرجع اشاره_تهی;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    طالما (شمار && !thiz->subtables[شمار - 1].apply_func)
      شمار--;

    thiz->digest.init ();
    لكل (خودکار& subtable : hb_iter (thiz->subtables, شمار))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_دومی = c_accelerate_subtables.digest_دومی;

    thiz->شمار = شمار;

#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    لكل (بیعلامت i = 0; i < شمار; i++)
      إذا (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#پایان_اگر

    أرجع thiz;
  }

  فراغ fini ()
  {
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    لكل (بیعلامت i = 0; i < شمار; i++)
      hb_free (subtables[i].external_cache);
#پایان_اگر
  }

  منطقي may_have (hb_codepoint_t g) ثابت
  { أرجع digest.may_have (g); }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  منطقي apply (hb_ot_apply_context_t *c, منطقي use_cache) ثابت
  {
    إذا (شمار == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
      إذا (use_cache)
	أرجع subtables[0].apply_cached_no_digest (c);
#پایان_اگر
      أرجع subtables[0].apply_no_digest (c);
    }
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (use_cache)
    {
      أرجع
      + hb_iter (hb_iter (subtables, شمار))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply_cached (c); })
      | hb_any
      ;
    }
    وإلا
#پایان_اگر
    {
      أرجع
      + hb_iter (hb_iter (subtables, شمار))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply (c); })
      | hb_any
      ;
    }
    أرجع خطأ;
  }

  منطقي cache_enter (hb_ot_apply_context_t *c) ثابت
  {
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    أرجع subtable_cache_user_idx != (بیعلامت) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#وإلا
    أرجع خطأ;
#پایان_اگر
  }
  فراغ cache_leave (hb_ot_apply_context_t *c) ثابت
  {
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#پایان_اگر
  }


  hb_مجموعه_digest_t digest;
  hb_مجموعه_digest_t digest_دومی;
  خاص:
  بیعلامت شمار = 0; /* Number of subtables in the array. */
#اگر_تعریف_نشده HB_NO_OT_LAYOUT_LOOKUP_CACHE
  بیعلامت subtable_cache_user_idx = (بیعلامت) -1;
#پایان_اگر
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

بنية GSUBGPOS
{
  استفاده LookupListT = LookupList<SmallTypes>;
  استفاده LookupList2T = LookupList<MediumTypes>;
  قالب <نام_نوع TLookup>
  استفاده TLookupList = List16_از_جابه_جایی_مقصد<TLookup, نام_نوع SmallTypes::HBLUINT>;
  قالب <نام_نوع TLookup>
  استفاده TLookupList2 = List16_از_جابه_جایی_مقصد<TLookup, نام_نوع MediumTypes::HBLUINT>;
  قالب <نام_نوع TLookup>
  استفاده TLookup_جابه_جایی_List = Lookup_جابه_جایی_List<TLookup, نام_نوع SmallTypes::HBLUINT>;
  قالب <نام_نوع TLookup>
  استفاده TLookup_جابه_جایی_List2 = Lookup_جابه_جایی_List<TLookup, نام_نوع MediumTypes::HBLUINT>;

  size_t get_اندازه () ثابت
  {
    إذا (نسخه.major != 1) أرجع نسخه.static_اندازه;

    أرجع min_اندازه +
	   (نسخه.مقصد_int () >= 0x00010001u ? featureVars.static_اندازه : 0) +
#اگر_تعریف_نشده HB_NO_BEYOND_64K
	   (نسخه.مقصد_int () >= 0x00010002u ? 3 * Offset32::static_اندازه : 0)
#وإلا
	   0
#پایان_اگر
	   ;
  }

  ثابت ScriptList &get_script_list () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010002u && scriptList2) أرجع هذا + scriptList2;
#پایان_اگر
    أرجع هذا + scriptList;
  }
  ثابت FeatureList &get_feature_list () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010002u && featureList2) أرجع هذا + featureList2;
#پایان_اگر
    أرجع هذا + featureList;
  }
  قالب <نام_نوع TLookup>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخه.sanitize (c) ||
		  نسخه.major != 1 ||
		  !c->check_range (هذا, get_اندازه ())))
      return_trace (خطأ);
    hb_barrier ();

    منطقي script_list_ok = scriptList.sanitize (c, هذا);
    منطقي feature_list_ok = featureList.sanitize (c, هذا);
    منطقي lookup_list_ok = typed_lookup_list_جابه_جایی<TLookup> ().sanitize (c, هذا);

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010002u)
    {
      إذا (scriptList2) script_list_ok = scriptList2.sanitize (c, هذا);
      إذا (featureList2) feature_list_ok = featureList2.sanitize (c, هذا);
      إذا (lookupList2) lookup_list_ok = typed_lookup_list2_جابه_جایی<TLookup> ().sanitize (c, هذا);
    }
#پایان_اگر

    إذا (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (خطأ);

#اگر_تعریف_نشده HB_NO_VAR
    إذا (unlikely (!(نسخه.مقصد_int () < 0x00010001u ||
		    featureVars.sanitize (c, هذا, &get_feature_list ()))))
      return_trace (خطأ);
#پایان_اگر

    return_trace (درست);
  }

  قالب <نام_نوع TLookup>
  منطقي subset (hb_subset_layout_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (نسخه.major != 1) return_trace (خطأ);

    خودکار *بیرون = c->subset_context->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->subset_context->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیرون->نسخه = نسخه;

#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010002u)
    {
      إذا (unlikely (!c->subset_context->serializer->extend_اندازه (بیرون, get_اندازه ())))
        return_trace (خطأ);

      بیرون->scriptList = 0;
      بیرون->featureList = 0;
      بیرون->lookupList = 0;

      إذا (lookupList2)
	serialize_subset_جابه_جایی (c->subset_context, بیرون->lookupList2,
				 subset_lookup_list2_جابه_جایی<TLookup> (),
				 هذا, c);
      وإلا
	serialize_subset_جابه_جایی (c->subset_context, بیرون->lookupList2,
				 subset_lookup_list_جابه_جایی<TLookup> (),
				 هذا, c);

      إذا (featureList2)
	serialize_subset_جابه_جایی (c->subset_context, بیرون->featureList2,
				 subset_feature_list2_جابه_جایی (),
				 هذا, c);
      وإلا
	serialize_subset_جابه_جایی (c->subset_context, بیرون->featureList2,
				 subset_feature_list_جابه_جایی (),
				 هذا, c);

      إذا (scriptList2)
	serialize_subset_جابه_جایی (c->subset_context, بیرون->scriptList2, scriptList2, هذا, c);
      وإلا
	serialize_subset_جابه_جایی (c->subset_context, بیرون->scriptList2, scriptList, هذا, c);
    }
    وإلا
#پایان_اگر
    {
      بیرون->subset_lookup_list_جابه_جایی<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_جابه_جایی<TLookup> (),
			     هذا,
			     c);

      بیرون->subset_feature_list_جابه_جایی ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_جابه_جایی (),
			     هذا,
			     c);

      بیرون->scriptList.serialize_subset (c->subset_context,
					scriptList,
					هذا,
					c);
    }

#اگر_تعریف_نشده HB_NO_VAR
    إذا (نسخه.مقصد_int () >= 0x00010001u)
    {
      خودکار snapshot = c->subset_context->serializer->snapshot ();
      إذا (نسخه.مقصد_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&بیرون->featureVars))
        return_trace (خطأ);

      // if all axes are pinned all feature vars are dropped.
      منطقي ret = !c->subset_context->plan->all_axes_pinned
                 && بیرون->featureVars.serialize_subset (c->subset_context, featureVars, هذا, c);
      إذا (!ret)
      {
        بیرون->featureVars = 0;
	إذا (نسخه.مقصد_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  بیرون->نسخه.major = 1;
	  بیرون->نسخه.minor = 0;
	}
      }
    }
#پایان_اگر

    return_trace (درست);
  }

  بیعلامت صحيح get_lookup_شمار () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2).len;
#پایان_اگر
    أرجع (هذا + lookupList).len;
  }
  ثابت Lookup& get_lookup (بیعلامت صحيح i) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2)[i];
#پایان_اگر
    أرجع (هذا + lookupList)[i];
  }
  ثابت FeatureVariations &get_feature_variations () ثابت
  {
    أرجع (نسخه.مقصد_int () >= 0x00010001u && hb_barrier ()) ? هذا+featureVars : Null (FeatureVariations);
  }

  منطقي has_داده () ثابت { أرجع نسخه.مقصد_int (); }
  بیعلامت صحيح get_script_شمار () ثابت
  { أرجع get_script_list ().len; }
  ثابت برچسب& get_script_برچسب (بیعلامت صحيح i) ثابت
  { أرجع get_script_list ().get_برچسب (i); }
  بیعلامت صحيح get_script_برچسبها (بیعلامت صحيح شروع_جابه_جایی,
				بیعلامت صحيح *script_شمار /* IN/OUT */,
				hb_برچسب_t     *script_برچسبها /*بیرون*/) ثابت
  { أرجع get_script_list ().get_برچسبها (شروع_جابه_جایی, script_شمار, script_برچسبها); }
  ثابت Script& get_script (بیعلامت صحيح i) ثابت
  { أرجع get_script_list ()[i]; }
  منطقي پیدا_کن_script_شاخص (hb_برچسب_t برچسب_2, بیعلامت صحيح *شاخص_2) ثابت
  { أرجع get_script_list ().پیدا_کن_شاخص (برچسب_2, شاخص_2); }

  بیعلامت صحيح get_feature_شمار () ثابت
  { أرجع get_feature_list ().len; }
  hb_برچسب_t get_feature_برچسب (بیعلامت صحيح i) ثابت
  { أرجع i == شاخص::NOT_یافت_شاخص ? HB_برچسب_NONE : get_feature_list ().get_برچسب (i); }
  بیعلامت صحيح get_feature_برچسبها (بیعلامت صحيح شروع_جابه_جایی,
				 بیعلامت صحيح *feature_شمار_2 /* IN/OUT */,
				 hb_برچسب_t     *feature_برچسبها /*بیرون*/) ثابت
  { أرجع get_feature_list ().get_برچسبها (شروع_جابه_جایی, feature_شمار_2, feature_برچسبها); }
  ثابت Feature& get_feature (بیعلامت صحيح i) ثابت
  { أرجع get_feature_list ()[i]; }
  منطقي پیدا_کن_feature_شاخص (hb_برچسب_t برچسب_2, بیعلامت صحيح *شاخص_2) ثابت
  { أرجع get_feature_list ().پیدا_کن_شاخص (برچسب_2, شاخص_2); }

  منطقي پیدا_کن_variations_شاخص (ثابت صحيح *coords, بیعلامت صحيح رقم_coords,
			      بیعلامت صحيح *شاخص_2,
			      قلم_Var_Store_Instancer *instancer) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    *شاخص_2 = FeatureVariations::NOT_یافت_شاخص;
    أرجع خطأ;
#پایان_اگر
    أرجع get_feature_variations ().پیدا_کن_شاخص (coords, رقم_coords, شاخص_2, instancer);
  }
  منطقي get_feature_variations_state (ثابت صحيح *coords,
				     بیعلامت صحيح رقم_coords,
				     بیعلامت صحيح *شاخص_2,
				     قلم_Var_Store_Instancer *instancer,
				     hb_وکتور_t<بیعلامت> *lookup_conditions) ثابت
  {
    lookup_conditions->reset ();
#اگر_تعریف_شده HB_NO_VAR
    *شاخص_2 = FeatureVariations::NOT_یافت_شاخص;
    أرجع درست;
#وإلا
    پیدا_کن_variations_شاخص (coords, رقم_coords, شاخص_2, instancer);
    أرجع get_feature_variations ().collect_lookup_conditions (
	coords,
	رقم_coords,
	instancer,
	lookup_conditions);
#پایان_اگر
  }
  منطقي get_lookup_variations (بیعلامت feature_شاخص,
                              بیعلامت variations_شاخص,
                              hb_sorted_array_t<ثابت بیعلامت> conditions,
                              hb_مجموعه_t *lookup_indexes) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع خطأ;
#وإلا
    أرجع get_feature_variations ().get_lookup_variations (
	feature_شاخص, get_feature_variation (feature_شاخص, variations_شاخص),
	conditions, lookup_indexes);
#پایان_اگر
  }
  ثابت Feature& get_feature_variation (بیعلامت صحيح feature_شاخص,
					بیعلامت صحيح variations_شاخص) ثابت
  {
#اگر_تعریف_نشده HB_NO_VAR
    إذا (FeatureVariations::NOT_یافت_شاخص != variations_شاخص &&
	نسخه.مقصد_int () >= 0x00010001u)
    {
      ثابت Feature *feature = get_feature_variations ().پیدا_کن_substitute (variations_شاخص,
									  feature_شاخص);
      إذا (feature)
	أرجع *feature;
    }
#پایان_اگر
    أرجع get_feature (feature_شاخص);
  }

  فراغ feature_variation_collect_lookups (ثابت hb_مجموعه_t *feature_indexes,
					  ثابت hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map,
					  hb_مجموعه_t       *lookup_indexes /*بیرون*/) ثابت
  {
#اگر_تعریف_نشده HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_رکورد_cond_idx_map, lookup_indexes);
#پایان_اگر
  }

  فراغ collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#اگر_تعریف_نشده HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#پایان_اگر
  }

#اگر_تعریف_نشده HB_NO_VAR
  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#پایان_اگر

  قالب <نام_نوع TLookup>
  فراغ بستگی_lookups (hb_face_t      *face,
			ثابت hb_مجموعه_t *glyphs,
			hb_مجموعه_t       *lookup_indexes /* IN/OUT */) ثابت
  {
    hb_مجموعه_t دیدهشده_lookups, inactive_lookups;
    hb_بستگی_lookups_context_t c (face, glyphs, &دیدهشده_lookups, &inactive_lookups);

    c.مجموعه_recurse_func (TLookup::قالب dispatch_recurse_func<hb_بستگی_lookups_context_t>);

    لكل (بیعلامت lookup_شاخص : *lookup_indexes)
      reinterpret_cast<ثابت TLookup &> (get_lookup (lookup_شاخص)).بستگی_lookups (&c, lookup_شاخص);

    hb_مجموعه_union (lookup_indexes, &دیدهشده_lookups);
    hb_مجموعه_تفریق (lookup_indexes, &inactive_lookups);
  }

  فراغ prune_langsys (ثابت hb_map_t *duplicate_feature_map,
                      ثابت hb_مجموعه_t *layout_scripts,
                      hb_hashmap_t<بیعلامت, hb::unique_ptr<hb_مجموعه_t>> *script_langsys_map,
                      hb_مجموعه_t       *new_feature_indexes /*بیرون*/) ثابت
  {
    hb_prune_langsys_context_t c (هذا, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    بیعلامت شمار = get_script_شمار ();
    لكل (بیعلامت script_شاخص = 0; script_شاخص < شمار; script_شاخص++)
    {
      ثابت برچسب& برچسب_2 = get_script_برچسب (script_شاخص);
      إذا (!layout_scripts->has (برچسب_2)) استمر;
      ثابت Script& s = get_script (script_شاخص);
      s.prune_langsys (&c, script_شاخص);
    }
  }

  فراغ prune_features (ثابت hb_map_t *lookup_indices, /*درون*/
		       ثابت hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map, /*درون*/
		       ثابت hb_hashmap_t<بیعلامت, ثابت Feature*> *feature_substitutes_map, /*درون*/
		       hb_مجموعه_t       *feature_indices /* IN/OUT */) ثابت
  {
#اگر_تعریف_نشده HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_مجموعه_t alternate_feature_indices;
    get_feature_variations ().بستگی_features (lookup_indices, feature_رکورد_cond_idx_map, &alternate_feature_indices);
    إذا (unlikely (alternate_feature_indices.درون_error()))
    {
      feature_indices->err ();
      أرجع;
    }
#پایان_اگر

    لكل (بیعلامت i : hb_iter (feature_indices))
    {
      hb_برچسب_t برچسب_2 =  get_feature_برچسب (i);
      إذا (برچسب_2 == HB_برچسب ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        استمر;


      ثابت Feature *f = &(get_feature (i));
      ثابت Feature** p = اشاره_تهی;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      إذا (!f->featureParams.is_null () &&
          برچسب_2 == HB_برچسب ('s', 'i', 'z', 'e'))
        استمر;

      إذا (!f->intersects_lookup_indexes (lookup_indices)
#اگر_تعریف_نشده HB_NO_VAR
          && !alternate_feature_indices.has (i)
#پایان_اگر
	  )
	feature_indices->del (i);
    }
  }

  فراغ collect_name_ids (ثابت hb_map_t *feature_شاخص_map,
                         hb_مجموعه_t *nameids_مقصد_retain, /*بیرون*/
                         ثابت hb_hashmap_t<بیعلامت, ثابت Feature*> *feature_substitutes_map,
                         ثابت hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map HB_استفادهنشده,
                         ثابت hb_مجموعه_t *catch_all_features,
                         منطقي all_axes_pinned HB_استفادهنشده) ثابت
  {
    بیعلامت شمار = get_feature_شمار ();
    لكل (بیعلامت i = 0 ; i < شمار; i++)
    {
      إذا (!feature_شاخص_map->has (i)) استمر;
      hb_برچسب_t برچسب_2 = get_feature_برچسب (i);
      ثابت Feature *feature = &get_feature (i);
      ثابت Feature **substitute;
      إذا (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        إذا (catch_all_features->has (i))
          get_feature (i).collect_name_ids (برچسب_2, nameids_مقصد_retain);
      }
      feature->collect_name_ids (برچسب_2, nameids_مقصد_retain);
    }
#اگر_تعریف_نشده HB_NO_VAR
    إذا (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_شاخص_map, feature_رکورد_cond_idx_map,
                                                  [هذا] (بیعلامت i) { أرجع get_feature_برچسب (i); },
                                                  nameids_مقصد_retain);
#پایان_اگر
  }

  قالب <نام_نوع T>
  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = درست;
      هذا->جدول = sc.reference_جدول<T> (face);

      إذا (unlikely (هذا->جدول->is_blocklisted (هذا->جدول.get_blob (), face)))
      {
	hb_blob_destroy (هذا->جدول.get_blob ());
	هذا->جدول = hb_blob_get_خالی ();
      }

      هذا->lookup_شمار = جدول->get_lookup_شمار ();

      هذا->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (هذا->lookup_شمار, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->lookup_شمار = 0;
	هذا->جدول.destroy ();
	هذا->جدول = hb_blob_get_خالی ();
      }
    }
    ~accelerator_t ()
    {
      لكل (بیعلامت صحيح i = 0; i < هذا->lookup_شمار; i++)
      {
	خودکار *accel = هذا->accels[i].get_relaxed ();
	إذا (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (هذا->accels);
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (بیعلامت lookup_شاخص) ثابت
    {
      إذا (unlikely (lookup_شاخص >= lookup_شمار)) أرجع اشاره_تهی;

    retry:
      خودکار *accel = accels[lookup_شاخص].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (جدول->get_lookup (lookup_شاخص));
	إذا (unlikely (!accel))
	  أرجع اشاره_تهی;

	إذا (unlikely (!accels[lookup_شاخص].cmpexch (اشاره_تهی, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> جدول;
    بیعلامت صحيح lookup_شمار;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  محمي:
  قالب <نام_نوع TLookup>
  ثابت Offset16_مقصد<TLookupList<TLookup>>& typed_lookup_list_جابه_جایی () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_مقصد<TLookupList<TLookup>>&> (lookupList); }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
  قالب <نام_نوع TLookup>
  ثابت Offset32_مقصد<TLookupList2<TLookup>>& typed_lookup_list2_جابه_جایی () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_مقصد<TLookupList2<TLookup>>&> (lookupList2); }
#پایان_اگر

  قالب <نام_نوع TLookup>
  ثابت Offset16_مقصد<TLookup_جابه_جایی_List<TLookup>>& subset_lookup_list_جابه_جایی () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_مقصد<TLookup_جابه_جایی_List<TLookup>>&> (lookupList); }
  قالب <نام_نوع TLookup>
  Offset16_مقصد<TLookup_جابه_جایی_List<TLookup>>& subset_lookup_list_جابه_جایی ()
  { أرجع reinterpret_cast<Offset16_مقصد<TLookup_جابه_جایی_List<TLookup>>&> (lookupList); }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
  قالب <نام_نوع TLookup>
  ثابت Offset32_مقصد<TLookup_جابه_جایی_List2<TLookup>>& subset_lookup_list2_جابه_جایی () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_مقصد<TLookup_جابه_جایی_List2<TLookup>>&> (lookupList2); }
#پایان_اگر

  ثابت Offset16_مقصد<رکورد_List_از_Feature>& subset_feature_list_جابه_جایی () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_مقصد<رکورد_List_از_Feature>&> (featureList); }
  Offset16_مقصد<رکورد_List_از_Feature>& subset_feature_list_جابه_جایی ()
  { أرجع reinterpret_cast<Offset16_مقصد<رکورد_List_از_Feature>&> (featureList); }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
  ثابت Offset32_مقصد<رکورد_List_از_Feature>& subset_feature_list2_جابه_جایی () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_مقصد<رکورد_List_از_Feature>&> (featureList2); }
#پایان_اگر

  ثابت فراغ* get_lookup_list_فیلد_جابه_جایی () ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (نسخه.مقصد_int () >= 0x00010002u && lookupList2) أرجع &lookupList2;
#پایان_اگر
    أرجع &lookupList;
  }

  قالب <نام_نوع جابه_جایی_بیرون, نام_نوع جابه_جایی_درون, نام_نوع پایه_3, نام_نوع ...Ts>
  ساكن منطقي serialize_subset_جابه_جایی (hb_subset_context_t *c,
				       جابه_جایی_بیرون& بیرون,
				       ثابت جابه_جایی_درون& درون,
				       ثابت پایه_3 *پایه,
				       Ts&&... ds)
  {
    بیرون = 0;
    إذا (درون.is_null ()) أرجع خطأ;

    خودکار *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (پایه+درون, std::forward<Ts> (ds)...);
    إذا (ret) s->جمع_کن_link (بیرون, s->برداشت_pack ());
    وإلا s->برداشت_discard ();
    أرجع ret;
  }

  Fixed_نسخه<>نسخه;	/* Version of the GSUB/GPOS table--initially set
				 * to 0x00010000u */
  Offset16_مقصد<ScriptList>
		scriptList;	/* Offset to ScriptList table */
  Offset16_مقصد<FeatureList>
		featureList;	/* Offset to FeatureList table */
  Offset16_مقصد<LookupListT>
		lookupList;	/* Offset to LookupList table */
  Offset32_مقصد<FeatureVariations>
		featureVars;	/* Offset to Feature Variations table--from
				 * beginning of table (may be NULL).
				 * Introduced in version 0x00010001. */
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  Offset32_مقصد<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32_مقصد<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32_مقصد<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#پایان_اگر
  عام:
  DEFINE_اندازه_MIN (10);
};


} /* namespace OT */


#پایان_اگر /* HB_OT_LAYOUT_GSUBGPOS_HH */
