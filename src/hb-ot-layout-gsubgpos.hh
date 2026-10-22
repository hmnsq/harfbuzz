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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_LAYOUT_GSUBGPOS_HH
#وصف_ڪريو HB_OT_LAYOUT_GSUBGPOS_HH

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
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { أرجع obj.intersects (هذا->glyphs); }
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  ثابت hb_سيٽ_t *glyphs;

  hb_intersects_context_t (ثابت hb_سيٽ_t *glyphs_) :
                            glyphs (glyphs_) {}
};

بنية hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, منطقي>
{
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { أرجع obj.may_have_non_1to1 (); }
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
};

بنية hb_گهيرو_context_t :
       hb_dispatch_context_t<hb_گهيرو_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_گهيرو_context_t *c, بنانشان lookup_اشاريو, hb_سيٽ_t *covered_seq_indicies, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو);
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { obj.گهيرو (هذا); أرجع hb_خالي_جايون_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }
  فراغ recurse (بنانشان lookup_اشاريو, hb_سيٽ_t *covered_seq_indicies, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو)
  {
    إذا (unlikely (nesting_level_کاٻو == 0 || !recurse_func))
      أرجع;

    nesting_level_کاٻو--;
    recurse_func (هذا, lookup_اشاريو, covered_seq_indicies, seq_اشاريو, پڇاڙي_اشاريو);
    nesting_level_کاٻو++;
  }

  فراغ reset_lookup_گهمو_ڳڻپ ()
  { lookup_ڳڻپ = 0; }

  منطقي lookup_حد_بندي_exceeded ()
  { أرجع lookup_ڳڻپ > HB_MAX_LOOKUP_گهمو_ڳڻپ; }

  منطقي should_گهمو_lookup (بنانشان صحيح lookup_اشاريو)
  {
    إذا (lookup_ڳڻپ++ > HB_MAX_LOOKUP_گهمو_ڳڻپ)
      أرجع خطأ;

    إذا (is_lookup_پورو (lookup_اشاريو))
      أرجع خطأ;

    أرجع سچ;
  }

  منطقي is_lookup_پورو (بنانشان صحيح lookup_اشاريو)
  {
    إذا (unlikely (پورو_lookups_glyph_ڳڻپ->اندريون_error () ||
		  پورو_lookups_glyph_سيٽ->اندريون_error ()))
      أرجع سچ;

    /* Have we visited this lookup with the current set of glyphs? */
    بنانشان ڪڍو = glyphs->get_population ();
    إذا (unlikely (ڪڍو == HB_MAP_قدر_INVALID)) // Avoid sentinel interference.
      ڪڍو--;
    إذا (پورو_lookups_glyph_ڳڻپ->get (lookup_اشاريو) != ڪڍو)
    {
      پورو_lookups_glyph_ڳڻپ->سيٽ (lookup_اشاريو, ڪڍو);

      إذا (!پورو_lookups_glyph_سيٽ->has (lookup_اشاريو))
      {
	إذا (unlikely (!پورو_lookups_glyph_سيٽ->سيٽ (lookup_اشاريو, hb::unique_ptr<hb_سيٽ_t> {hb_سيٽ_create ()})))
	  أرجع سچ;
      }

      پورو_lookups_glyph_سيٽ->get (lookup_اشاريو)->clear ();
    }

    hb_سيٽ_t *covered_glyph_سيٽ = پورو_lookups_glyph_سيٽ->get (lookup_اشاريو);
    إذا (unlikely (covered_glyph_سيٽ->اندريون_error ()))
      أرجع سچ;
    إذا (والد_سرگرم_glyphs ().is_subset (*covered_glyph_سيٽ))
      أرجع سچ;

    covered_glyph_سيٽ->union_ (والد_سرگرم_glyphs ());
    أرجع خطأ;
  }

  ثابت hb_سيٽ_t& اڳيون_والد_سرگرم_glyphs () {
    إذا (سرگرم_glyphs_ڍير.ڊيگهائي <= 1)
      أرجع *glyphs;

    أرجع سرگرم_glyphs_ڍير[سرگرم_glyphs_ڍير.ڊيگهائي - 2];
  }

  ثابت hb_سيٽ_t& والد_سرگرم_glyphs ()
  {
    إذا (!سرگرم_glyphs_ڍير)
      أرجع *glyphs;

    أرجع سرگرم_glyphs_ڍير.پڇ ();
  }

  hb_سيٽ_t* ڌڪو_cur_سرگرم_glyphs ()
  {
    hb_سيٽ_t *s = سرگرم_glyphs_ڍير.push ();
    إذا (unlikely (سرگرم_glyphs_ڍير.اندريون_error ()))
      أرجع خالي_اشارو;
    أرجع s;
  }

  منطقي ڪڍو_cur_پورو_glyphs ()
  {
    إذا (!سرگرم_glyphs_ڍير)
      أرجع خطأ;

    سرگرم_glyphs_ڍير.ڪڍو ();
    أرجع سچ;
  }

  hb_face_t *face;
  hb_سيٽ_t *glyphs;
  hb_سيٽ_t output[1];
  hb_ويڪٽر_t<hb_سيٽ_t> سرگرم_glyphs_ڍير;
  recurse_func_t recurse_func = خالي_اشارو;
  بنانشان صحيح nesting_level_کاٻو;

  hb_گهيرو_context_t (hb_face_t *face_,
			hb_سيٽ_t *glyphs_,
			hb_map_t *پورو_lookups_glyph_ڳڻپ_2,
			hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> *پورو_lookups_glyph_سيٽ_2,
			بنانشان صحيح nesting_level_کاٻو_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_کاٻو (nesting_level_کاٻو_2),
			  پورو_lookups_glyph_ڳڻپ (پورو_lookups_glyph_ڳڻپ_2),
			  پورو_lookups_glyph_سيٽ (پورو_lookups_glyph_سيٽ_2)
  {}

  ~hb_گهيرو_context_t () { flush (); }

  فراغ سيٽ_recurse_func (recurse_func_t func) { recurse_func = func; }

  فراغ flush ()
  {
    output->del_range (face->get_انگ_glyphs (), HB_سيٽ_قدر_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    سرگرم_glyphs_ڍير.ڪڍو ();
    سرگرم_glyphs_ڍير.reset ();
  }

  خاص:
  hb_map_t *پورو_lookups_glyph_ڳڻپ;
  hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> *پورو_lookups_glyph_سيٽ;
  بنانشان صحيح lookup_ڳڻپ = 0;
};



بنية hb_گهيرو_lookups_context_t :
       hb_dispatch_context_t<hb_گهيرو_lookups_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_گهيرو_lookups_context_t *c, بنانشان lookup_اشاريو);
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { obj.گهيرو_lookups (هذا); أرجع hb_خالي_جايون_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }
  فراغ recurse (بنانشان lookup_اشاريو)
  {
    إذا (unlikely (nesting_level_کاٻو == 0 || !recurse_func))
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (lookup_حد_بندي_exceeded ()
        || گهميل_lookups->اندريون_error ()
        || گهميل_lookups->has (lookup_اشاريو))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      أرجع;

    nesting_level_کاٻو--;
    recurse_func (هذا, lookup_اشاريو);
    nesting_level_کاٻو++;
  }

  فراغ سيٽ_lookup_گهميل (بنانشان lookup_اشاريو)
  { گهميل_lookups->جوڙيو (lookup_اشاريو); }

  فراغ سيٽ_lookup_inactive (بنانشان lookup_اشاريو)
  { inactive_lookups->جوڙيو (lookup_اشاريو); }

  منطقي lookup_حد_بندي_exceeded ()
  {
    منطقي ret = lookup_ڳڻپ > HB_MAX_LOOKUP_گهمو_ڳڻپ;
    إذا (ret)
      DEBUG_نياپو (SUBSET, خالي_اشارو, "lookup visit count limit exceeded in lookup closure!");
    أرجع ret; }

  منطقي is_lookup_گهميل (بنانشان lookup_اشاريو)
  {
    إذا (unlikely (lookup_ڳڻپ++ > HB_MAX_LOOKUP_گهمو_ڳڻپ))
    {
      DEBUG_نياپو (SUBSET, خالي_اشارو, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_ڳڻپ, lookup_اشاريو);
      أرجع سچ;
    }

    إذا (unlikely (گهميل_lookups->اندريون_error ()))
      أرجع سچ;

    أرجع گهميل_lookups->has (lookup_اشاريو);
  }

  hb_face_t *face;
  ثابت hb_سيٽ_t *glyphs;
  recurse_func_t recurse_func;
  بنانشان صحيح nesting_level_کاٻو;

  hb_گهيرو_lookups_context_t (hb_face_t *face_,
				ثابت hb_سيٽ_t *glyphs_,
				hb_سيٽ_t *گهميل_lookups_2,
				hb_سيٽ_t *inactive_lookups_,
				بنانشان nesting_level_کاٻو_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (خالي_اشارو),
				nesting_level_کاٻو (nesting_level_کاٻو_2),
				گهميل_lookups (گهميل_lookups_2),
				inactive_lookups (inactive_lookups_),
				lookup_ڳڻپ (0) {}

  فراغ سيٽ_recurse_func (recurse_func_t func) { recurse_func = func; }

  خاص:
  hb_سيٽ_t *گهميل_lookups;
  hb_سيٽ_t *inactive_lookups;
  بنانشان صحيح lookup_ڳڻپ;
};

بنية hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, منطقي>
{
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { أرجع obj.would_apply (هذا); }
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  hb_face_t *face;
  ثابت hb_codepoint_t *glyphs;
  بنانشان صحيح len;
  منطقي zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ثابت hb_codepoint_t *glyphs_,
			    بنانشان صحيح len_,
			    منطقي zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

بنية hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, بنانشان صحيح lookup_اشاريو);
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { obj.collect_glyphs (هذا); أرجع hb_خالي_جايون_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }
  فراغ recurse (بنانشان صحيح lookup_اشاريو)
  {
    إذا (unlikely (nesting_level_کاٻو == 0 || !recurse_func))
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

    إذا (output == hb_سيٽ_get_خالي_جايون ())
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (recursed_lookups->has (lookup_اشاريو))
      أرجع;

    hb_سيٽ_t *old_before = before;
    hb_سيٽ_t *old_داخلا  = داخلا_2;
    hb_سيٽ_t *old_after  = after;
    before = داخلا_2 = after = hb_سيٽ_get_خالي_جايون ();

    nesting_level_کاٻو--;
    recurse_func (هذا, lookup_اشاريو);
    nesting_level_کاٻو++;

    before = old_before;
    داخلا_2  = old_داخلا;
    after  = old_after;

    recursed_lookups->جوڙيو (lookup_اشاريو);
  }

  hb_face_t *face;
  hb_سيٽ_t *before;
  hb_سيٽ_t *داخلا_2;
  hb_سيٽ_t *after;
  hb_سيٽ_t *output;
  recurse_func_t recurse_func;
  hb_سيٽ_t *recursed_lookups;
  بنانشان صحيح nesting_level_کاٻو;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_سيٽ_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_سيٽ_t  *glyphs_داخلا,  /* OUT.  May be NULL */
			       hb_سيٽ_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_سيٽ_t  *glyphs_output, /* OUT.  May be NULL */
			       بنانشان صحيح nesting_level_کاٻو_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_سيٽ_get_خالي_جايون ()),
			      داخلا_2  (glyphs_داخلا  ? glyphs_داخلا  : hb_سيٽ_get_خالي_جايون ()),
			      after  (glyphs_after  ? glyphs_after  : hb_سيٽ_get_خالي_جايون ()),
			      output (glyphs_output ? glyphs_output : hb_سيٽ_get_خالي_جايون ()),
			      recurse_func (خالي_اشارو),
			      recursed_lookups (hb_سيٽ_create ()),
			      nesting_level_کاٻو (nesting_level_کاٻو_2) {}
  ~hb_collect_glyphs_context_t () { hb_سيٽ_destroy (recursed_lookups); }

  فراغ سيٽ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

بنية hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  تعداد glyph_سيٽ_type_t
  {
    GLYPH_سيٽ_CLASS,
    GLYPH_سيٽ_COVERAGE,
    GLYPH_سيٽ_SINGLE
  };

  ساكن constexpr hb_codepoint_t SINGLE_GLYPH_سرگرم_FLAG = 0x80000000u;

  بنية glyph_سيٽ_cache_ڪنجي_t
  {
    glyph_سيٽ_cache_ڪنجي_t () = افتراضي;
    glyph_سيٽ_cache_ڪنجي_t (ثابت فراغ *معلومات_2, بنانشان قدر_3, glyph_سيٽ_type_t type_)
      : معلومات (معلومات_2), value (قدر_3), type (type_) {}

    منطقي عامل == (ثابت glyph_سيٽ_cache_ڪنجي_t &o) ثابت
    { أرجع معلومات == o.معلومات && value == o.value && type == o.type; }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) معلومات);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash ((بنانشان) type);
    }

    ثابت فراغ *معلومات = خالي_اشارو;
    بنانشان value = 0;
    glyph_سيٽ_type_t type = GLYPH_سيٽ_CLASS;
  };

  بنية context_سيٽ_cache_ڪنجي_t
  {
    context_سيٽ_cache_ڪنجي_t () = افتراضي;
    context_سيٽ_cache_ڪنجي_t (ثابت فراغ *قاعدو,
			     ثابت فراغ *رڪارڊ_3,
			     ثابت فراغ *معلومات_2,
			     hb_codepoint_t سرگرم_glyphs,
			     بنانشان قدر_3,
			     بنانشان format_)
      : قاعدو_2 (قاعدو), رڪارڊ (رڪارڊ_3), معلومات (معلومات_2), سرگرم_glyphs_2 (سرگرم_glyphs),
	value (قدر_3), format (format_) {}

    منطقي عامل == (ثابت context_سيٽ_cache_ڪنجي_t &o) ثابت
    {
      أرجع قاعدو_2 == o.قاعدو_2 &&
	     رڪارڊ == o.رڪارڊ &&
	     معلومات == o.معلومات &&
	     سرگرم_glyphs_2 == o.سرگرم_glyphs_2 &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) قاعدو_2);
      current = current * 31 + hb_hash ((uintptr_t) رڪارڊ);
      current = current * 31 + hb_hash ((uintptr_t) معلومات);
      current = current * 31 + hb_hash (سرگرم_glyphs_2);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash (format);
    }

    ثابت فراغ *قاعدو_2 = خالي_اشارو;
    ثابت فراغ *رڪارڊ = خالي_اشارو;
    ثابت فراغ *معلومات = خالي_اشارو;
    hb_codepoint_t سرگرم_glyphs_2 = HB_CODEPOINT_INVALID;
    بنانشان value = 0;
    بنانشان format = 0;
  };

  بنية recurse_ڪنجي_t
  {
    recurse_ڪنجي_t () = افتراضي;
    recurse_ڪنجي_t (بنانشان lookup_اشاريو_3,
                   hb_codepoint_t سرگرم_glyphs,
                   hb_codepoint_t recurse_رستو,
                   hb_codepoint_t context_سيٽ_2)
      : lookup_اشاريو (lookup_اشاريو_3),
        سرگرم_glyphs_2 (سرگرم_glyphs),
        recurse_رستو_2 (recurse_رستو),
        context_سيٽ (context_سيٽ_2) {}

    منطقي عامل == (ثابت recurse_ڪنجي_t &o) ثابت
    {
      أرجع lookup_اشاريو == o.lookup_اشاريو &&
             سرگرم_glyphs_2 == o.سرگرم_glyphs_2 &&
             recurse_رستو_2 == o.recurse_رستو_2 &&
             context_سيٽ == o.context_سيٽ;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_اشاريو)) * 16777619;
      current = (current ^ hb_hash (سرگرم_glyphs_2)) * 16777619;
      current = (current ^ hb_hash (recurse_رستو_2)) * 16777619;
      current = (current ^ hb_hash (context_سيٽ)) * 16777619;
      أرجع current;
    }

    بنانشان lookup_اشاريو = 0;
    hb_codepoint_t سرگرم_glyphs_2 = 0;
    hb_codepoint_t recurse_رستو_2 = 0;
    hb_codepoint_t context_سيٽ = HB_CODEPOINT_INVALID;
  };

  تعريفنوع return_t (*recurse_func_t) (hb_depend_context_t *c, بنانشان lookup_اشاريو, hb_ٻٽو_انگ_صفحو_t *covered_seq_indices, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { obj.depend (هذا); أرجع hb_خالي_جايون_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }

  فراغ recurse (بنانشان lookup_idx, hb_ٻٽو_انگ_صفحو_t *covered_seq_indices, بنانشان seq_اشاريو, بنانشان پڇاڙي_اشاريو)
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
    إذا (lookups_ڏٺل.has (lookup_idx)) أرجع;
    lookups_ڏٺل.جوڙيو (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t والد_رستو = recurse_رستو_2;
    uint64_t رستو_ڪنجي = (uint64_t) والد_رستو << 32 | lookup_idx;
    hb_codepoint_t *رستو = خالي_اشارو;
    hb_codepoint_t رستو_اشاريو;
    إذا (!recurse_رستو_منزل_اشاريو.has (رستو_ڪنجي, &رستو))
    {
      رستو_اشاريو = recurse_رستو_منزل_اشاريو.get_population () + 1;
      إذا (unlikely (!recurse_رستو_منزل_اشاريو.سيٽ (رستو_ڪنجي, رستو_اشاريو)))
      {
	depend_معلومات->fail ();
	lookups_ڏٺل.del (lookup_idx);
	أرجع;
      }
    }
    وإلا
      رستو_اشاريو = *رستو;
    recurse_رستو_2 = رستو_اشاريو;
    recurse_func (هذا, lookup_idx, covered_seq_indices, seq_اشاريو, پڇاڙي_اشاريو);
    recurse_رستو_2 = والد_رستو;
    lookups_ڏٺل.del (lookup_idx);
  }

  بنية سرگرم_glyphs_چوکٺو_t
  {
    hb_سيٽ_t glyphs;
    hb_codepoint_t سرگرم_idx = HB_CODEPOINT_INVALID;
    منطقي use_پاڙ = خطأ;
  };

  ثابت hb_سيٽ_t& والد_سرگرم_glyphs ()
  {
    إذا (!سرگرم_glyphs_ڍير_اونهائي)
      أرجع *glyphs;

    ثابت سرگرم_glyphs_چوکٺو_t &چوکٺو = سرگرم_glyphs_ڍير[سرگرم_glyphs_ڍير_اونهائي - 1];
    أرجع چوکٺو.use_پاڙ ? *glyphs : چوکٺو.glyphs;
  }

  hb_سيٽ_t* ڌڪو_cur_سرگرم_glyphs (hb_codepoint_t سرگرم_idx = HB_CODEPOINT_INVALID)
  {
    إذا (سرگرم_glyphs_ڍير_اونهائي == سرگرم_glyphs_ڍير.ڊيگهائي &&
	unlikely (!سرگرم_glyphs_ڍير.ڌڪو_or_fail ()))
      أرجع خالي_اشارو;

    سرگرم_glyphs_چوکٺو_t &چوکٺو = سرگرم_glyphs_ڍير[سرگرم_glyphs_ڍير_اونهائي++];
    چوکٺو.glyphs.clear ();
    چوکٺو.سرگرم_idx = سرگرم_idx;
    چوکٺو.use_پاڙ = خطأ;
    أرجع &چوکٺو.glyphs;
  }

  منطقي ڌڪو_پاڙ_سرگرم_glyphs ()
  {
    إذا (سرگرم_glyphs_ڍير_اونهائي == سرگرم_glyphs_ڍير.ڊيگهائي &&
	unlikely (!سرگرم_glyphs_ڍير.ڌڪو_or_fail ()))
      أرجع خطأ;

    سرگرم_glyphs_ڍير[سرگرم_glyphs_ڍير_اونهائي++].use_پاڙ = سچ;
    أرجع سچ;
  }

  منطقي ڪڍو_cur_پورو_glyphs ()
  {
    إذا (!سرگرم_glyphs_ڍير_اونهائي)
      أرجع خطأ;

    سرگرم_glyphs_ڍير_اونهائي--;
    أرجع سچ;
  }

  منطقي get_والد_سرگرم_glyphs_اشاريو (hb_codepoint_t *سرگرم_idx)
  {
    إذا (!سرگرم_glyphs_ڍير_اونهائي ||
	سرگرم_glyphs_ڍير[سرگرم_glyphs_ڍير_اونهائي - 1].use_پاڙ)
    {
      *سرگرم_idx = HB_CODEPOINT_INVALID;
      أرجع سچ;
    }

    سرگرم_glyphs_چوکٺو_t &چوکٺو = سرگرم_glyphs_ڍير[سرگرم_glyphs_ڍير_اونهائي - 1];
    *سرگرم_idx = چوکٺو.سرگرم_idx;
    إذا (*سرگرم_idx != HB_CODEPOINT_INVALID)
      أرجع سچ;

    hb_codepoint_t glyph;
    إذا (چوکٺو.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_سرگرم_FLAG))
      *سرگرم_idx = glyph | SINGLE_GLYPH_سرگرم_FLAG;
    وإلا إذا (!ڳوليو_or_create_recurse_سيٽ (چوکٺو.glyphs, سرگرم_idx))
      أرجع خطأ;

    چوکٺو.سرگرم_idx = *سرگرم_idx;
    أرجع سچ;
  }

  منطقي get_context_سيٽ_cache_ڪنجي (ثابت فراغ *قاعدو_2,
				  ثابت فراغ *رڪارڊ,
				  ثابت فراغ *معلومات,
				  بنانشان value,
				  بنانشان format,
				  context_سيٽ_cache_ڪنجي_t *key)
  {
    hb_codepoint_t سرگرم_idx;
    إذا (!get_والد_سرگرم_glyphs_اشاريو (&سرگرم_idx))
      أرجع خطأ;
    *key = context_سيٽ_cache_ڪنجي_t (قاعدو_2, رڪارڊ, معلومات, سرگرم_idx, value, format);
    أرجع سچ;
  }

  منطقي get_recurse_ڪنجي (بنانشان lookup_idx, recurse_ڪنجي_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t سرگرم_idx;
    إذا (!get_والد_سرگرم_glyphs_اشاريو (&سرگرم_idx))
      أرجع خطأ;

    *key = recurse_ڪنجي_t (lookup_idx,
                          سرگرم_idx,
                          recurse_رستو_2,
                          depend_معلومات->current_context_سيٽ_اشاريو);
    أرجع !completed_recursions.has (*key);
  }

  فراغ پڄاڻي_recurse (ثابت recurse_ڪنجي_t &key)
  { depend_معلومات->check_success (completed_recursions.سيٽ (key, سچ)); }

  فراغ reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_رستو_منزل_اشاريو.clear ();
    recurse_رستو_2 = 0;
  }

  منطقي ڳوليو_or_create_recurse_سيٽ (ثابت hb_سيٽ_t &سيٽ, hb_codepoint_t *اشاريو_2)
  {
    hb_codepoint_t *existing = خالي_اشارو;
    إذا (recurse_سيٽ_منزل_اشاريو.has (&سيٽ, &existing))
    {
      *اشاريو_2 = *existing;
      أرجع سچ;
    }

    hb_سيٽ_t *copy = hb_object_create<hb_سيٽ_t> ();
    إذا (unlikely (!copy))
      أرجع depend_معلومات->fail ();
    copy->سيٽ (سيٽ);
    إذا (unlikely (copy->اندريون_error ()))
    {
      hb_سيٽ_destroy (copy);
      أرجع depend_معلومات->fail ();
    }

    *اشاريو_2 = recurse_sets.ڊيگهائي;
    إذا (unlikely (*اشاريو_2 & SINGLE_GLYPH_سرگرم_FLAG))
    {
      hb_سيٽ_destroy (copy);
      أرجع depend_معلومات->fail ();
    }
    إذا (unlikely (!recurse_sets.ڌڪو_or_fail (hb::unique_ptr<hb_سيٽ_t> {copy})))
      أرجع depend_معلومات->fail ();
    إذا (unlikely (!recurse_سيٽ_منزل_اشاريو.سيٽ (recurse_sets[*اشاريو_2].get (), *اشاريو_2)))
      أرجع depend_معلومات->fail ();
    أرجع سچ;
  }

  ثابت hb_سيٽ_t *ڳوليو_or_create_glyph_سيٽ (ثابت فراغ *معلومات,
                                             بنانشان value,
                                             glyph_سيٽ_type_t type);
  hb_codepoint_t ڳوليو_or_create_cached_context_سيٽ (ثابت hb_سيٽ_t *سيٽ);

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
      داخلا_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_سيٽ_t preliminary_context;
    hb_سيٽ_t position_scratch;
    hb_سيٽ_t pos_glyphs;
    hb_سيٽ_t position_context;
    hb_سيٽ_t disjunctive_indices;
    hb_سيٽ_t filtered_disjunctive_indices;
    hb_ويڪٽر_t<ثابت hb_سيٽ_t *> backtrack_sets;
    hb_ويڪٽر_t<ثابت hb_سيٽ_t *> lookahead_sets;
    hb_ويڪٽر_t<ثابت hb_سيٽ_t *> داخلا_position_glyphs;
    hb_ويڪٽر_t<hb_codepoint_t> cached_context_sets;
    hb_ويڪٽر_t<بنانشان> compact_workspace;
    depend_scratch_t *next = خالي_اشارو;
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
        depend_معلومات->fail ();
        أرجع خالي_اشارو;
      }
      جديد (scratch) depend_scratch_t ();
    }

    scratch->next = خالي_اشارو;
    scratch->clear ();
    أرجع scratch;
  }

  فراغ release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_معلومات_builder_t *depend_معلومات;
  hb_face_t *face;
  hb_سيٽ_t *glyphs;
  hb_ويڪٽر_t<سرگرم_glyphs_چوکٺو_t> سرگرم_glyphs_ڍير;
  بنانشان سرگرم_glyphs_ڍير_اونهائي = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_اشاريو = HB_CODEPOINT_INVALID;
  hb_ٻٽو_انگ_سيٽ_t lookups_ڏٺل;
  hb_ويڪٽر_t<hb::unique_ptr<hb_سيٽ_t>> recurse_sets;
  hb_hashmap_t<ثابت hb_سيٽ_t *, hb_codepoint_t> recurse_سيٽ_منزل_اشاريو;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_رستو_منزل_اشاريو;
  hb_codepoint_t recurse_رستو_2 = 0;
  hb_hashmap_t<recurse_ڪنجي_t, منطقي> completed_recursions;
  hb_ويڪٽر_t<hb::unique_ptr<hb_سيٽ_t>> glyph_sets;
  hb_hashmap_t<glyph_سيٽ_cache_ڪنجي_t, hb_codepoint_t> glyph_سيٽ_منزل_اشاريو;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_سيٽ_indices;
  hb_hashmap_t<context_سيٽ_cache_ڪنجي_t, hb_codepoint_t> context_سيٽ_cache;
  depend_scratch_t *depend_scratch_pool = خالي_اشارو;

  hb_depend_context_t (hb_depend_معلومات_builder_t *depend_معلومات_2,
                       hb_face_t *face_,
                       hb_سيٽ_t *glyphs_)
                      : depend_معلومات (depend_معلومات_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (خالي_اشارو) {}
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
  فراغ سيٽ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

سانچو <قسم_نالو سيٽ_t>
بنية hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<سيٽ_t>, ثابت Coverage &>
{
  تعريفنوع ثابت Coverage &return_t; // Stoopid that we have to dupe this here.
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { أرجع obj.get_coverage (); }
  ساكن return_t default_return_قدر () { أرجع Null (Coverage); }
  منطقي stop_sublookup_iteration (return_t r) ثابت
  {
    r.collect_coverage (سيٽ);
    أرجع خطأ;
  }

  hb_collect_coverage_context_t (سيٽ_t *سيٽ_2) :
				   سيٽ (سيٽ_2) {}

  سيٽ_t *سيٽ;
};

بنية matcher_t
{
  تعريفنوع منطقي (*ميلاپ_func_t) (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات);

  سانچو <قسم_نالو context_t>
  فراغ init (ثابت context_t *c, منطقي context_ميلاپ = خطأ)
  {
    سيٽ_ميلاپ_func (خالي_اشارو, خالي_اشارو);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->جدول_اشاريو == 1 || (context_ميلاپ && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_ميلاپ || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->جدول_اشاريو == 1;
    نقاب = context_ميلاپ ? -1 : c->lookup_نقاب;
    /* Per syllable matching is only for GSUB. */
    في_syllable = c->جدول_اشاريو == 0 && c->في_syllable;
    syllable = 0;
  }

  فراغ سيٽ_ميلاپ_func (ميلاپ_func_t ميلاپ_func,
		       ثابت فراغ *ميلاپ_معلومات)
  { ميلاپ_func_2 = ميلاپ_func; ميلاپ_معلومات_2 = ميلاپ_معلومات; }

  تعداد may_ميلاپ_t {
    ميلاپ_NO,
    ميلاپ_YES,
    ميلاپ_MAYBE
  };

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  may_ميلاپ_t may_ميلاپ (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_معلومات) ثابت
  {
    إذا (!(info.نقاب & نقاب) ||
	(في_syllable && syllable && syllable != info.syllable ()))
      أرجع ميلاپ_NO;

    إذا (ميلاپ_func_2)
      أرجع ميلاپ_func_2 (info, glyph_معلومات, ميلاپ_معلومات_2) ? ميلاپ_YES : ميلاپ_NO;

    أرجع ميلاپ_MAYBE;
  }

  تعداد may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  سانچو <قسم_نالو context_t>
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
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
  بنانشان صحيح lookup_props = 0;
  hb_نقاب_t نقاب = -1;
  منطقي ignore_zwnj = خطأ;
  منطقي ignore_zwj = خطأ;
  منطقي ignore_hidden = خطأ;
  منطقي في_syllable = خطأ;
  uint8_t syllable = 0;
  ميلاپ_func_t ميلاپ_func_2 = خالي_اشارو;
  ثابت فراغ *ميلاپ_معلومات_2 = خالي_اشارو;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
بنية glyph_معلومات_t
{
  سانچو <قسم_نالو T>
  glyph_معلومات_t (ثابت T *معلومات_2) : معلومات (معلومات_2)
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
                              , wide (حجم (T) == 3)
#جيڪڏهن_پورو
  {
    static_assert (حجم (T) == 2 || حجم (T) == 3, "invalid glyph data size");
  }

  بنانشان عامل [] (بنانشان i) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (wide)
      أرجع ((ثابت HBUINT24 *) معلومات)[i];
#جيڪڏهن_پورو
    أرجع ((ثابت HBUINT16 *) معلومات)[i];
  }

  ثابت فراغ *معلومات;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  منطقي wide;
#جيڪڏهن_پورو
};

سانچو <قسم_نالو context_t>
بنية skipping_iterator_t
{
  فراغ init (context_t *c_, منطقي context_ميلاپ = خطأ)
  {
    c = c_;
    پڇاڙي = c->بفر->len;
    ميلاپ_glyph_data16 = خالي_اشارو;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    ميلاپ_glyph_data24 = خالي_اشارو;
#جيڪڏهن_پورو
    matcher.init (c, context_ميلاپ);
  }
  فراغ سيٽ_lookup_props (بنانشان صحيح lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  فراغ سيٽ_ميلاپ_func (matcher_t::ميلاپ_func_t ميلاپ_func,
		       ثابت فراغ *ميلاپ_معلومات)
  {
    matcher.سيٽ_ميلاپ_func (ميلاپ_func, ميلاپ_معلومات);
  }
  فراغ سيٽ_glyph_معلومات (glyph_معلومات_t glyph_معلومات)
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    ميلاپ_glyph_data16 = glyph_معلومات.wide ? خالي_اشارو : (ثابت HBUINT16 *) glyph_معلومات.معلومات;
    ميلاپ_glyph_data24 = glyph_معلومات.wide ? (ثابت HBUINT24 *) glyph_معلومات.معلومات : خالي_اشارو;
#وإلا
    ميلاپ_glyph_data16 = (ثابت HBUINT16 *) glyph_معلومات.معلومات;
#جيڪڏهن_پورو
  }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  فراغ reset (بنانشان صحيح شروعات_اشاريو_2)
  {
    // For GSUB forward iterator
    idx = شروعات_اشاريو_2;
    پڇاڙي = c->بفر->len;
    matcher.syllable = c->بفر->cur().syllable();
  }
  فراغ reset_پٺ (بنانشان صحيح شروعات_اشاريو_2, منطقي from_ٻاهريون_بفر = خطأ)
  {
    // For GSUB backward iterator
    idx = شروعات_اشاريو_2;
    matcher.syllable = c->بفر->cur().syllable();
  }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  فراغ reset_fast (بنانشان صحيح شروعات_اشاريو_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = شروعات_اشاريو_2;
  }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  matcher_t::may_skip_t may_skip (ثابت hb_glyph_info_t &info) ثابت
  { أرجع matcher.may_skip (c, info); }

  تعداد ميلاپ_t {
    ميلاپ_2,
    NOT_ميلاپ,
    SKIP
  };

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  ميلاپ_t ميلاپ (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    إذا (unlikely (skip == matcher_t::SKIP_YES))
      أرجع SKIP;

    matcher_t::may_ميلاپ_t ميلاپ = matcher.may_ميلاپ (info, get_glyph_معلومات ());
    إذا (ميلاپ == matcher_t::ميلاپ_YES ||
	(ميلاپ == matcher_t::ميلاپ_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      أرجع ميلاپ_2;

    إذا (skip == matcher_t::SKIP_NO)
      أرجع NOT_ميلاپ;

    أرجع SKIP;
  }

  منطقي next (بنانشان *unsafe_منزل = خالي_اشارو)
  {
    خودڪار *info = c->بفر->info;
    ثابت نشانوارو stop = (نشانوارو) پڇاڙي - 1;
    طالما ((نشانوارو) idx < stop)
    {
      idx++;
      بدل (ميلاپ (info[idx]))
      {
	حالة ميلاپ_2:
	{
	  advance_glyph_معلومات ();
	  أرجع سچ;
	}
	حالة NOT_ميلاپ:
	{
	  إذا (unsafe_منزل)
	    *unsafe_منزل = idx + 1;
	  أرجع خطأ;
	}
	حالة SKIP:
	  استمر;
      }
    }
    إذا (unsafe_منزل)
      *unsafe_منزل = پڇاڙي;
    أرجع خطأ;
  }
  منطقي prev (بنانشان *unsafe_from = خالي_اشارو)
  {
    خودڪار *ٻاهريون_info = c->بفر->ٻاهريون_info;
    ثابت بنانشان stop = 0;
    طالما (idx > stop)
    {
      idx--;
      بدل (ميلاپ (ٻاهريون_info[idx]))
      {
	حالة ميلاپ_2:
	{
	  advance_glyph_معلومات ();
	  أرجع سچ;
	}
	حالة NOT_ميلاپ:
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
  get_glyph_معلومات ()
  {
    إذا (ميلاپ_glyph_data16) أرجع *ميلاپ_glyph_data16;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    وإلا
    إذا (ميلاپ_glyph_data24) أرجع *ميلاپ_glyph_data24;
#جيڪڏهن_پورو
    أرجع 0;
  }
  HB_ALWAYS_INLINE
  فراغ
  advance_glyph_معلومات ()
  {
    إذا (ميلاپ_glyph_data16) ميلاپ_glyph_data16++;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    وإلا
    إذا (ميلاپ_glyph_data24) ميلاپ_glyph_data24++;
#جيڪڏهن_پورو
  }

  بنانشان صحيح idx;
  محمي:
  context_t *c;
  matcher_t matcher;
  ثابت HBUINT16 *ميلاپ_glyph_data16;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  ثابت HBUINT24 *ميلاپ_glyph_data24;
#جيڪڏهن_پورو

  بنانشان صحيح پڇاڙي;
};

بنية hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  تعريفنوع return_t (*recurse_func_t) (hb_ot_apply_context_t *c, بنانشان صحيح lookup_اشاريو);

  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_ترجيح<1>) HB_RETURN (return_t, obj.apply (c, خالي_اشارو) )
  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_ترجيح<0>) HB_RETURN (return_t, obj.apply (c) )
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { أرجع apply_(obj, هذا, hb_prioritize); }

  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
  return_t recurse (بنانشان صحيح sub_lookup_اشاريو)
  {
    أكد (recurse_func);
    إذا (unlikely (nesting_level_کاٻو == 0))
    {
      بفر->successful = خطأ;
      أرجع default_return_قدر ();
    }

    بفر->max_ops--;
    إذا (unlikely (بفر->max_ops < 0))
    {
      بفر->successful = خطأ;
      أرجع default_return_قدر ();
    }

    nesting_level_کاٻو--;
    منطقي ret = recurse_func (هذا, sub_lookup_اشاريو);
    nesting_level_کاٻو++;
    أرجع ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_داخلا, iter_context;

  بنانشان صحيح جدول_اشاريو; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t *بفر;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = خالي_اشارو;
  ثابت GDEF &gdef;
  ثابت GDEF::accelerator_t &gdef_accel;
  ثابت شي_Variation_Store &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_نقاب_t lookup_نقاب = 1;
  بنانشان صحيح lookup_اشاريو = (بنانشان) -1;
  بنانشان صحيح lookup_props = 0;
  بنانشان صحيح nesting_level_کاٻو = HB_MAX_NESTING_LEVEL;

  منطقي has_glyph_classes;
  منطقي auto_zwnj = سچ;
  منطقي auto_zwj = سچ;
  منطقي في_syllable = خطأ;
  منطقي random = خطأ;
  بنانشان new_syllables = (بنانشان) -1;

  نشانوارو آخري_بنياد = -1; // GPOS uses
  بنانشان آخري_بنياد_until = 0; // GPOS uses

  hb_ويڪٽر_t<uint32_t> ميلاپ_positions;
  uint32_t ڍير_ميلاپ_positions[8];
#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_پيغام
  hb_بفر_t::بدليو_func_t orig_بدليو_func = خالي_اشارو;
  فراغ *orig_بدليو_معلومات = خالي_اشارو;
#جيڪڏهن_پورو

  hb_ot_apply_context_t (بنانشان صحيح جدول_اشاريو_2,
			 hb_font_t *font_,
			 hb_بفر_t *بفر_2,
			 hb_blob_t *جدول_blob_2,
			 hb_scalar_cache_t *var_store_cache_ = خالي_اشارو) :
			جدول_اشاريو (جدول_اشاريو_2),
			font (font_), face (font->face), بفر (بفر_2),
			sanitizer (جدول_blob_2),
			gdef (
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT
			      *face->جدول.GDEF->جدول
#وإلا
			      Null (GDEF)
#جيڪڏهن_پورو
			     ),
			gdef_accel (
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT
			      *face->جدول.GDEF
#وإلا
			      Null (GDEF::accelerator_t)
#جيڪڏهن_پورو
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (بفر_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    ميلاپ_positions.سيٽ_storage (ڍير_ميلاپ_positions);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_پيغام
    إذا (بفر->messaging ())
    {
      أكد (بفر->بدليو_func == خالي_اشارو ||
	      بفر->بدليو_func == بفر_بدليو_trampoline);
      orig_بدليو_func = بفر->بدليو_func;
      orig_بدليو_معلومات = بفر->بدليو_معلومات;
      بفر->بدليو_func = بفر_بدليو_trampoline;
      بفر->بدليو_معلومات = هذا;
    }
#جيڪڏهن_پورو
  }
  ~hb_ot_apply_context_t ()
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_بفر_پيغام
    إذا (بفر->messaging ())
    {
      أكد (بفر->بدليو_func == بفر_بدليو_trampoline);
      أكد (بفر->بدليو_معلومات == هذا);
      بفر->بدليو_func = orig_بدليو_func;
      بفر->بدليو_معلومات = orig_بدليو_معلومات;
    }
#جيڪڏهن_پورو
  }

  فراغ init_iters ()
  {
    iter_داخلا.init (هذا, خطأ);
    iter_context.init (هذا, سچ);
  }

  فراغ سيٽ_lookup_نقاب (hb_نقاب_t نقاب, منطقي init = سچ) { lookup_نقاب = نقاب; آخري_بنياد = -1; آخري_بنياد_until = 0; إذا (init) init_iters (); }
  فراغ سيٽ_auto_zwj (منطقي auto_zwj_, منطقي init = سچ) { auto_zwj = auto_zwj_; إذا (init) init_iters (); }
  فراغ سيٽ_auto_zwnj (منطقي auto_zwnj_, منطقي init = سچ) { auto_zwnj = auto_zwnj_; إذا (init) init_iters (); }
  فراغ سيٽ_في_syllable (منطقي في_syllable_2, منطقي init = سچ) { في_syllable = في_syllable_2; إذا (init) init_iters (); }
  فراغ سيٽ_random (منطقي random_) { random = random_; }
  فراغ سيٽ_recurse_func (recurse_func_t func) { recurse_func = func; }
  فراغ سيٽ_lookup_اشاريو (بنانشان صحيح lookup_اشاريو_3) { lookup_اشاريو = lookup_اشاريو_3; }
  فراغ سيٽ_lookup_props (بنانشان صحيح lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_عدد ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    بفر->random_state = بفر->random_state * 48271 % 2147483647;
    أرجع بفر->random_state;
  }

  ساكن فراغ بفر_بدليو_trampoline (hb_بفر_t *بفر HB_اڻ_استعمال_ٿيل, فراغ *user_معلومات)
  {
    ((hb_ot_apply_context_t *) user_معلومات)->بفر_بدليو ();
  }
  فراغ بفر_بدليو ()
  {
    بفر->update_digest ();
    إذا (likely (has_glyph_classes))
      لكل (بنانشان i = 0; i < بفر->len; i++)
	سيٽ_glyph_class_props (بفر->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  منطقي ميلاپ_properties_mark (ثابت hb_glyph_info_t *info,
			      بنانشان صحيح    glyph_props,
			      بنانشان صحيح    ميلاپ_props) ثابت
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    إذا (ميلاپ_props & LookupFlag::Use_Mark_Filtering_سيٽ)
    {
      بنانشان سيٽ_اشاريو = ميلاپ_props >> 16;
      أرجع gdef_accel.mark_سيٽ_covers (سيٽ_اشاريو, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    إذا (ميلاپ_props & LookupFlag::MarkAttachmentType)
      أرجع (ميلاپ_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    أرجع سچ;
  }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  منطقي check_glyph_property (ثابت hb_glyph_info_t *info,
			     بنانشان ميلاپ_props) ثابت
  {
    بنانشان صحيح glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    إذا (glyph_props & ميلاپ_props & LookupFlag::IgnoreFlags)
      أرجع خطأ;

    إذا (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      أرجع ميلاپ_properties_mark (info, glyph_props, ميلاپ_props);

    أرجع سچ;
  }

  فراغ سيٽ_glyph_class (hb_codepoint_t glyph_اشاريو,
			  بنانشان صحيح class_guess = 0,
			  منطقي ligature = خطأ,
			  منطقي component = خطأ)
  {
    بفر->digest.جوڙيو (glyph_اشاريو);

    إذا (new_syllables != (بنانشان) -1)
      بفر->cur().syllable() = new_syllables;

    بنانشان صحيح props = _hb_glyph_info_get_glyph_props (&بفر->cur());
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
      سيٽ_glyph_class_props (بفر->cur(), props, glyph_اشاريو);
    }
    وإلا إذا (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      hb_glyph_info_سيٽ_glyph_props (&بفر->cur(), props | class_guess);
    }
    وإلا
      hb_glyph_info_سيٽ_glyph_props (&بفر->cur(), props);
  }
  فراغ سيٽ_glyph_class_props (hb_glyph_info_t &info,
			       بنانشان صحيح props,
			       hb_codepoint_t glyph_اشاريو)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    hb_glyph_info_سيٽ_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_اشاريو));
  }
  فراغ سيٽ_glyph_class_props (hb_glyph_info_t &info)
  {
    سيٽ_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  فراغ replace_glyph (hb_codepoint_t glyph_اشاريو)
  {
    سيٽ_glyph_class (glyph_اشاريو);
    (فراغ) بفر->replace_glyph (glyph_اشاريو);
  }
  فراغ replace_glyph_inplace (hb_codepoint_t glyph_اشاريو)
  {
    سيٽ_glyph_class (glyph_اشاريو);
    بفر->cur().codepoint = glyph_اشاريو;
  }
  فراغ replace_glyph_with_ligature (hb_codepoint_t glyph_اشاريو,
				    بنانشان صحيح class_guess)
  {
    سيٽ_glyph_class (glyph_اشاريو, class_guess, سچ);
    (فراغ) بفر->replace_glyph (glyph_اشاريو);
  }
  فراغ output_glyph_for_component (hb_codepoint_t glyph_اشاريو,
				   بنانشان صحيح class_guess)
  {
    سيٽ_glyph_class (glyph_اشاريو, class_guess, خطأ, سچ);
    (فراغ) بفر->output_glyph (glyph_اشاريو);
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
  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار collect_ٻيو_glyphs_2 (ثابت T &obj,
					     hb_سيٽ_digest_t *digest,
					     hb_ترجيح<1>) HB_RETURN (فراغ, obj.collect_ٻيو_glyphs (digest) )
  سانچو <قسم_نالو T>
  ساكن مضمن فراغ collect_ٻيو_glyphs_2 (ثابت T &obj HB_اڻ_استعمال_ٿيل,
					     hb_سيٽ_digest_t *digest,
					     hb_ترجيح<0>)
  { *digest = hb_سيٽ_digest_t::ڀريل (); }

  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجيح<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجيح<0>) HB_RETURN (منطقي, obj->apply (c) )
  سانچو <قسم_نالو T>
  ساكن مضمن منطقي apply_منزل (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجيح<2>) HB_RETURN (منطقي, obj->apply_cached (c, external_cache) )
  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجيح<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجيح<0>) HB_RETURN (منطقي, obj->apply (c) )
  سانچو <قسم_نالو T>
  ساكن مضمن منطقي apply_cached_منزل (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  سانچو <قسم_نالو T>
  ساكن مضمن خودڪار cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_ترجيح<1>) HB_RETURN (منطقي, T::cache_func (c, op) )
  سانچو <قسم_نالو T=فراغ>
  ساكن مضمن منطقي cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_اڻ_استعمال_ٿيل,
				  hb_ترجيح<0>) { أرجع خطأ; }
  سانچو <قسم_نالو Type>
  ساكن مضمن منطقي cache_func_منزل (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    أرجع cache_func_<Type> (c, op, hb_prioritize);
  }
#جيڪڏهن_پورو

  تعريفنوع منطقي (*hb_apply_func_t) (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache);
  تعريفنوع منطقي (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  بنية hb_applicable_t
  {
    دوست بنية hb_accelerate_subtables_context_t;
    دوست بنية hb_ot_layout_lookup_accelerator_t;

    سانچو <قسم_نالو T>
    فراغ init (ثابت T &obj_,
	       hb_apply_func_t apply_func_
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , فراغ *external_cache_
#جيڪڏهن_پورو
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#جيڪڏهن_پورو
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#جيڪڏهن_وصف_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    منطقي apply (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->بفر->cur().codepoint) && apply_func (obj, c, خالي_اشارو);
    }
    منطقي apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      أرجع apply_func (obj, c, خالي_اشارو);
    }
#وإلا
    منطقي apply (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->بفر->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    منطقي apply_cached (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->بفر->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
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
#جيڪڏهن_پورو

    خاص:
    ثابت فراغ *obj;
    hb_apply_func_t apply_func;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    فراغ *external_cache;
#جيڪڏهن_پورو
    hb_سيٽ_digest_t digest;
  };

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
  سانچو <قسم_نالو T>
  خودڪار cache_لاڳت (ثابت T &obj, hb_ترجيح<1>) HB_AUTO_RETURN ( obj.cache_لاڳت () )
  سانچو <قسم_نالو T>
  خودڪار cache_لاڳت (ثابت T &obj, hb_ترجيح<0>) HB_AUTO_RETURN ( 0u )

  سانچو <قسم_نالو T>
  خودڪار external_cache_create (ثابت T &obj, hb_ترجيح<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  سانچو <قسم_نالو T>
  خودڪار external_cache_create (ثابت T &obj, hb_ترجيح<0>) HB_AUTO_RETURN ( خالي_اشارو )
#جيڪڏهن_پورو

  /* Dispatch interface. */
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj)
  {
    collect_ٻيو_glyphs_2 (obj, &digest_ٻيو, hb_prioritize);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    فراغ *external_cache = خالي_اشارو;
    إذا (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#جيڪڏهن_پورو

    hb_applicable_t *داخلا = &array[i++];

    داخلا->init (obj,
		 apply_منزل<T>
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_منزل<T>
		 , cache_func_منزل<T>
		 , external_cache
#جيڪڏهن_پورو
		 );

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    بنانشان لاڳت = cache_لاڳت (obj, hb_prioritize);
    إذا (لاڳت > subtable_cache_user_لاڳت)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_لاڳت = لاڳت;
    }
#جيڪڏهن_پورو

    أرجع hb_خالي_جايون_t ();
  }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  بنانشان i = 0;
  hb_سيٽ_digest_t digest_ٻيو;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
  بنانشان subtable_cache_user_idx = (بنانشان) -1;
  بنانشان subtable_cache_user_لاڳت = 0;
#جيڪڏهن_پورو
};


تعريفنوع منطقي (*intersects_func_t) (ثابت hb_سيٽ_t *glyphs, بنانشان value, ثابت فراغ *معلومات, فراغ *cache);
تعريفنوع فراغ (*intersected_glyphs_func_t) (ثابت hb_سيٽ_t *glyphs, ثابت فراغ *معلومات, بنانشان value, hb_سيٽ_t *intersected_glyphs, فراغ *cache);
تعريفنوع فراغ (*collect_glyphs_func_t) (hb_سيٽ_t *glyphs, بنانشان value, ثابت فراغ *معلومات);
تعريفنوع منطقي (*ميلاپ_func_t) (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات);
تعريفنوع بنانشان (*get_قدر_func_t) (hb_glyph_info_t &info, ثابت فراغ *معلومات);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
بنية hb_ot_layout_ruleset_digest_t
{
  فراغ init () { نقاب = 0; }
  فراغ سيٽ_ڀريل () { نقاب = (uint64_t) -1; }
  فراغ جوڙيو (بنانشان value) { نقاب |= (uint64_t) 1 << (value & 63); }
  منطقي may_have (بنانشان value) ثابت { أرجع نقاب & ((uint64_t) 1 << (value & 63)); }

  uint64_t نقاب;
};

بنية Context_گهيرو_Funcs
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
  ميلاپ_func_t ميلاپ;
};
بنية ChainContextApplyFuncs
{
  ميلاپ_func_t ميلاپ[3];
};


ساكن مضمن منطقي intersects_glyph (ثابت hb_سيٽ_t *glyphs, بنانشان value, ثابت فراغ *معلومات HB_اڻ_استعمال_ٿيل, فراغ *cache HB_اڻ_استعمال_ٿيل)
{
  أرجع glyphs->has (value);
}
ساكن مضمن منطقي intersects_class (ثابت hb_سيٽ_t *glyphs, بنانشان value, ثابت فراغ *معلومات, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  إذا (map->has (value, &cached_v))
    أرجع *cached_v;

  منطقي v = class_def.intersects_class (glyphs, value);
  map->سيٽ (value, v);

  أرجع v;
}
ساكن مضمن منطقي intersects_coverage (ثابت hb_سيٽ_t *glyphs, بنانشان value, ثابت فراغ *معلومات, فراغ *cache HB_اڻ_استعمال_ٿيل)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  أرجع (معلومات+coverage).intersects (glyphs);
}


ساكن مضمن فراغ intersected_glyph (ثابت hb_سيٽ_t *glyphs HB_اڻ_استعمال_ٿيل, ثابت فراغ *معلومات, بنانشان value, hb_سيٽ_t *intersected_glyphs, HB_اڻ_استعمال_ٿيل فراغ *cache)
{
  بنانشان g = reinterpret_cast<ثابت HBUINT16 *>(معلومات)[value];
  intersected_glyphs->جوڙيو (g);
}

استعمال intersected_class_cache_t = hb_hashmap_t<بنانشان, hb_سيٽ_t>;

ساكن مضمن فراغ intersected_class_glyphs (ثابت hb_سيٽ_t *glyphs, ثابت فراغ *معلومات, بنانشان value, hb_سيٽ_t *intersected_glyphs, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_سيٽ_t *cached_v;
  إذا (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    أرجع;
  }

  hb_سيٽ_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->سيٽ (value, std::move (v));
}

ساكن مضمن فراغ intersected_coverage_glyphs (ثابت hb_سيٽ_t *glyphs, ثابت فراغ *معلومات, بنانشان value, hb_سيٽ_t *intersected_glyphs, HB_اڻ_استعمال_ٿيل فراغ *cache)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  (معلومات+coverage).intersect_سيٽ (*glyphs, *intersected_glyphs);
}


ساكن مضمن منطقي array_is_subset_جو (ثابت hb_سيٽ_t *glyphs,
				       بنانشان صحيح ڳڻپ,
				       glyph_معلومات_t values,
				       intersects_func_t intersects_func,
				       ثابت فراغ *intersects_معلومات,
				       فراغ *cache)
{
  لكل (بنانشان i = 0; i < ڳڻپ; i++)
    إذا (!intersects_func (glyphs, values[i], intersects_معلومات, cache)) أرجع خطأ;
  أرجع سچ;
}


ساكن مضمن فراغ collect_glyph (hb_سيٽ_t *glyphs, بنانشان value, ثابت فراغ *معلومات HB_اڻ_استعمال_ٿيل)
{
  glyphs->جوڙيو (value);
}
ساكن مضمن فراغ collect_class (hb_سيٽ_t *glyphs, بنانشان value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  class_def.collect_class (glyphs, value);
}
ساكن مضمن فراغ collect_coverage (hb_سيٽ_t *glyphs, بنانشان value, ثابت فراغ *معلومات)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  (معلومات+coverage).collect_coverage (glyphs);
}

مضمن ثابت hb_سيٽ_t *
hb_depend_context_t::ڳوليو_or_create_glyph_سيٽ (ثابت فراغ *معلومات,
                                                بنانشان value,
                                                glyph_سيٽ_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_سيٽ_cache_ڪنجي_t key (معلومات, value, type);
  hb_codepoint_t *existing = خالي_اشارو;
  إذا (glyph_سيٽ_منزل_اشاريو.has (key, &existing))
    أرجع glyph_sets[*existing].get ();

  hb_سيٽ_t *سيٽ = hb_object_create<hb_سيٽ_t> ();
  إذا (unlikely (!سيٽ))
  {
    depend_معلومات->fail ();
    أرجع خالي_اشارو;
  }

  إذا (type == GLYPH_سيٽ_COVERAGE)
    collect_coverage (سيٽ, value, معلومات);
  وإلا إذا (type == GLYPH_سيٽ_CLASS)
    collect_class (سيٽ, value, معلومات);
  وإلا
    سيٽ->جوڙيو (value);
  إذا (unlikely (سيٽ->اندريون_error ()))
  {
    hb_سيٽ_destroy (سيٽ);
    depend_معلومات->fail ();
    أرجع خالي_اشارو;
  }

  hb_codepoint_t اشاريو_2 = glyph_sets.ڊيگهائي;
  إذا (unlikely (!glyph_sets.ڌڪو_or_fail (hb::unique_ptr<hb_سيٽ_t> {سيٽ})))
  {
    depend_معلومات->fail ();
    أرجع خالي_اشارو;
  }
  إذا (unlikely (!glyph_سيٽ_منزل_اشاريو.سيٽ (key, اشاريو_2)))
  {
    depend_معلومات->fail ();
    أرجع خالي_اشارو;
  }
  أرجع glyph_sets[اشاريو_2].get ();
}

مضمن hb_codepoint_t
hb_depend_context_t::ڳوليو_or_create_cached_context_سيٽ (ثابت hb_سيٽ_t *سيٽ)
{
  uintptr_t key = (uintptr_t) سيٽ;
  hb_codepoint_t *existing = خالي_اشارو;
  إذا (cached_context_سيٽ_indices.has (key, &existing))
    أرجع *existing;

  hb_codepoint_t اشاريو_2 = depend_معلومات->ڳوليو_or_create_context_سيٽ (*سيٽ);
  إذا (unlikely (اشاريو_2 == HB_CODEPOINT_INVALID))
    أرجع HB_CODEPOINT_INVALID;
  إذا (unlikely (!cached_context_سيٽ_indices.سيٽ (key, اشاريو_2)))
    أرجع depend_معلومات->fail_invalid ();
  أرجع اشاريو_2;
}

ساكن مضمن فراغ collect_array (hb_collect_glyphs_context_t *c HB_اڻ_استعمال_ٿيل,
				  hb_سيٽ_t *glyphs,
				  بنانشان صحيح ڳڻپ,
				  glyph_معلومات_t values,
				  collect_glyphs_func_t collect_func,
				  ثابت فراغ *collect_معلومات)
{
  لكل (بنانشان i = 0; i < ڳڻپ; i++)
    collect_func (glyphs, values[i], collect_معلومات);
}


ساكن مضمن منطقي ميلاپ_always (hb_glyph_info_t &info HB_اڻ_استعمال_ٿيل, بنانشان value HB_اڻ_استعمال_ٿيل, ثابت فراغ *معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع سچ;
}
ساكن مضمن منطقي ميلاپ_glyph (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات HB_اڻ_استعمال_ٿيل)
{
  أرجع info.codepoint == value;
}
ساكن مضمن منطقي ميلاپ_class (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  أرجع class_def.get_class (info.codepoint) == value;
}
ساكن مضمن بنانشان get_class_قدر (hb_glyph_info_t &info, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (معلومات);
  أرجع class_def.get_class (info.codepoint);
}
بنية ميلاپ_class_cache_معلومات_t
{
  ثابت ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ساكن مضمن منطقي ميلاپ_class_cache (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات)
{
  ثابت خودڪار &cache_معلومات = *reinterpret_cast<ثابت ميلاپ_class_cache_معلومات_t *> (معلومات);
  أرجع cache_معلومات.class_def->get_class (info.codepoint, cache_معلومات.cache) == value;
}
ساكن مضمن بنانشان get_class_cache_قدر (hb_glyph_info_t &info, ثابت فراغ *معلومات)
{
  ثابت خودڪار &cache_معلومات = *reinterpret_cast<ثابت ميلاپ_class_cache_معلومات_t *> (معلومات);
  أرجع cache_معلومات.class_def->get_class (info.codepoint, cache_معلومات.cache);
}
ساكن مضمن بنانشان get_class_cached (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بنانشان klass = info.syllable();
  إذا (klass < 255)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 255))
    info.syllable() = klass;
  أرجع klass;
}
ساكن مضمن منطقي ميلاپ_class_cached (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  أرجع get_class_cached (class_def, info) == value;
}
ساكن مضمن بنانشان get_class_cached_قدر (hb_glyph_info_t &info, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (معلومات);
  أرجع get_class_cached (class_def, info);
}
ساكن مضمن بنانشان get_class_cached1 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بنانشان klass = info.syllable() & 0x0F;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  أرجع klass;
}
ساكن مضمن منطقي ميلاپ_class_cached1 (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  أرجع get_class_cached1 (class_def, info) == value;
}
ساكن مضمن بنانشان get_class_cached2 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بنانشان klass = (info.syllable() & 0xF0) >> 4;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  أرجع klass;
}
ساكن مضمن منطقي ميلاپ_class_cached2 (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  أرجع get_class_cached2 (class_def, info) == value;
}
ساكن مضمن بنانشان get_class_cached2_قدر (hb_glyph_info_t &info, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (معلومات);
  أرجع get_class_cached2 (class_def, info);
}
ساكن مضمن منطقي ميلاپ_coverage (hb_glyph_info_t &info, بنانشان value, ثابت فراغ *معلومات)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  أرجع (معلومات+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ساكن مضمن منطقي would_ميلاپ_داخلا (hb_would_apply_context_t *c,
				      بنانشان صحيح ڳڻپ, /* Including the first glyph (not matched) */
				      glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
				      ميلاپ_func_t ميلاپ_func_2,
				      ثابت فراغ *ميلاپ_معلومات_2)
{
  إذا (ڳڻپ != c->len)
    أرجع خطأ;

  لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    إذا (likely (!ميلاپ_func_2 (info, داخلا_2[i - 1], ميلاپ_معلومات_2)))
      أرجع خطأ;
  }

  أرجع سچ;
}
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
HB_ALWAYS_INLINE
#وإلا
مضمن
#جيڪڏهن_پورو
ساكن منطقي ميلاپ_داخلا (hb_ot_apply_context_t *c,
			 بنانشان صحيح ڳڻپ, /* Including the first glyph (not matched) */
			 glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
			 ميلاپ_func_t ميلاپ_func_2,
			 ثابت فراغ *ميلاپ_معلومات_2,
			 بنانشان صحيح *پڇاڙي_position,
			 بنانشان صحيح *p_ڪل_component_ڳڻپ = خالي_اشارو)
{
  TRACE_APPLY (خالي_اشارو);

  hb_بفر_t *بفر = c->بفر;

  إذا (ڳڻپ == 1)
  {
    *پڇاڙي_position = بفر->idx + 1;
    c->ميلاپ_positions[0] = بفر->idx;
    إذا (p_ڪل_component_ڳڻپ)
      *p_ڪل_component_ڳڻپ = hb_glyph_info_get_lig_انگ_comps (&بفر->cur());
    return_trace (سچ);
  }

  إذا (unlikely (ڳڻپ > HB_MAX_CONTEXT_ڊيگهائي)) return_trace (خطأ);

  خودڪار &skippy_iter = c->iter_داخلا;
  skippy_iter.reset (بفر->idx);
  skippy_iter.سيٽ_ميلاپ_func (ميلاپ_func_2, ميلاپ_معلومات_2);
  skippy_iter.سيٽ_glyph_معلومات (داخلا_2);

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

  بنانشان صحيح ڪل_component_ڳڻپ = 0;

  بنانشان صحيح پهريون_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
  بنانشان صحيح پهريون_lig_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());

  تعداد {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
  {
    بنانشان unsafe_منزل;
    إذا (!skippy_iter.next (&unsafe_منزل))
    {
      *پڇاڙي_position = unsafe_منزل;
      return_trace (خطأ);
    }

    إذا (unlikely (i + 1 > c->ميلاپ_positions.ڊيگهائي &&
		  !c->ميلاپ_positions.resize_dirty  (i + 1)))
      return_trace (خطأ);
    c->ميلاپ_positions.arrayZ[i] = skippy_iter.idx;

    بنانشان صحيح this_lig_id = _hb_glyph_info_get_lig_id (&بفر->info[skippy_iter.idx]);
    بنانشان صحيح this_lig_comp = _hb_glyph_info_get_lig_comp (&بفر->info[skippy_iter.idx]);

    إذا (پهريون_lig_id && پهريون_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      إذا (پهريون_lig_id != this_lig_id || پهريون_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	إذا (ligbase == LIGBASE_NOT_CHECKED)
	{
	  منطقي مليو = خطأ;
	  ثابت خودڪار *ٻاهريون = بفر->ٻاهريون_info;
	  بنانشان صحيح j = بفر->ٻاهريون_len;
	  طالما (j && _hb_glyph_info_get_lig_id (&ٻاهريون[j - 1]) == پهريون_lig_id)
	  {
	    إذا (_hb_glyph_info_get_lig_comp (&ٻاهريون[j - 1]) == 0)
	    {
	      j--;
	      مليو = سچ;
	      اكسر;
	    }
	    j--;
	  }

	  إذا (مليو && skippy_iter.may_skip (ٻاهريون[j]) == matcher_t::SKIP_YES)
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
      إذا (this_lig_id && this_lig_comp && (this_lig_id != پهريون_lig_id))
	return_trace (خطأ);
    }

    ڪل_component_ڳڻپ += hb_glyph_info_get_lig_انگ_comps_اندريون_ligation (&بفر->info[skippy_iter.idx]);
  }

  *پڇاڙي_position = skippy_iter.idx + 1;

  إذا (p_ڪل_component_ڳڻپ)
  {
    ڪل_component_ڳڻپ += hb_glyph_info_get_lig_انگ_comps (&بفر->cur());
    *p_ڪل_component_ڳڻپ = ڪل_component_ڳڻپ;
  }

  c->ميلاپ_positions.arrayZ[0] = بفر->idx;

  return_trace (سچ);
}
ساكن مضمن منطقي ligate_داخلا (hb_ot_apply_context_t *c,
				 بنانشان صحيح ڳڻپ, /* Including the first glyph */
				 بنانشان صحيح ميلاپ_پڇاڙي,
				 hb_codepoint_t lig_glyph,
				 بنانشان صحيح ڪل_component_ڳڻپ)
{
  TRACE_APPLY (خالي_اشارو);

  hb_بفر_t *بفر = c->بفر;

  بفر->ملايو_clusters (بفر->idx, ميلاپ_پڇاڙي);

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

  منطقي is_بنياد_ligature = hb_glyph_info_is_بنياد_glyph (&بفر->info[c->ميلاپ_positions.arrayZ[0]]);
  منطقي is_mark_ligature = _hb_glyph_info_is_mark (&بفر->info[c->ميلاپ_positions.arrayZ[0]]);
  لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
    إذا (!_hb_glyph_info_is_mark (&بفر->info[c->ميلاپ_positions.arrayZ[i]]))
    {
      is_بنياد_ligature = خطأ;
      is_mark_ligature = خطأ;
      اكسر;
    }
  منطقي is_ligature = !is_بنياد_ligature && !is_mark_ligature;

  بنانشان صحيح klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  بنانشان صحيح lig_id = is_ligature ? _hb_allocate_lig_id (بفر) : 0;
  بنانشان صحيح آخري_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
  بنانشان صحيح آخري_انگ_components = hb_glyph_info_get_lig_انگ_comps (&بفر->cur());
  بنانشان صحيح components_so_far = آخري_انگ_components;

  إذا (is_ligature)
  {
    hb_glyph_info_سيٽ_lig_props_for_ligature (&بفر->cur(), lig_id, ڪل_component_ڳڻپ);
    إذا (_hb_glyph_info_get_general_category (&بفر->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      hb_glyph_info_سيٽ_general_category (&بفر->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
  {
    طالما (بفر->idx < c->ميلاپ_positions.arrayZ[i] && بفر->successful)
    {
      إذا (is_ligature)
      {
	بنانشان صحيح this_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());
	إذا (this_comp == 0)
	  this_comp = آخري_انگ_components;
	أكد (components_so_far >= آخري_انگ_components);
	بنانشان صحيح new_lig_comp = components_so_far - آخري_انگ_components +
				    hb_min (this_comp, آخري_انگ_components);
	  hb_glyph_info_سيٽ_lig_props_for_mark (&بفر->cur(), lig_id, new_lig_comp);
      }
      (فراغ) بفر->ايندڙ_glyph ();
    }

    آخري_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
    آخري_انگ_components = hb_glyph_info_get_lig_انگ_comps_اندريون_ligation (&بفر->cur());
    components_so_far += آخري_انگ_components;

    /* Skip the base glyph */
    بفر->idx++;
  }

  إذا (!is_mark_ligature && آخري_lig_id)
  {
    /* Re-adjust components for any marks following. */
    لكل (بنانشان i = بفر->idx; i < بفر->len; ++i)
    {
      إذا (آخري_lig_id != _hb_glyph_info_get_lig_id (&بفر->info[i])) اكسر;

      بنانشان this_comp = _hb_glyph_info_get_lig_comp (&بفر->info[i]);
      إذا (!this_comp) اكسر;

      أكد (components_so_far >= آخري_انگ_components);
      بنانشان new_lig_comp = components_so_far - آخري_انگ_components +
			      hb_min (this_comp, آخري_انگ_components);
      hb_glyph_info_سيٽ_lig_props_for_mark (&بفر->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (سچ);
}

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
HB_ALWAYS_INLINE
#وإلا
مضمن
#جيڪڏهن_پورو
ساكن منطقي ميلاپ_backtrack (hb_ot_apply_context_t *c,
			     بنانشان صحيح ڳڻپ,
			     glyph_معلومات_t backtrack,
			     ميلاپ_func_t ميلاپ_func_2,
			     ثابت فراغ *ميلاپ_معلومات_2,
			     بنانشان صحيح *ميلاپ_شروعات)
{
  TRACE_APPLY (خالي_اشارو);

  إذا (!ڳڻپ)
  {
    *ميلاپ_شروعات = c->بفر->backtrack_len ();
    return_trace (سچ);
  }

  خودڪار &skippy_iter = c->iter_context;
  skippy_iter.reset_پٺ (c->بفر->backtrack_len ());
  skippy_iter.سيٽ_ميلاپ_func (ميلاپ_func_2, ميلاپ_معلومات_2);
  skippy_iter.سيٽ_glyph_معلومات (backtrack);

  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    بنانشان unsafe_from;
    إذا (!skippy_iter.prev (&unsafe_from))
    {
      *ميلاپ_شروعات = unsafe_from;
      return_trace (خطأ);
    }
  }

  *ميلاپ_شروعات = skippy_iter.idx;
  return_trace (سچ);
}

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
HB_ALWAYS_INLINE
#وإلا
مضمن
#جيڪڏهن_پورو
ساكن منطقي ميلاپ_lookahead (hb_ot_apply_context_t *c,
			     بنانشان صحيح ڳڻپ,
			     glyph_معلومات_t lookahead,
			     ميلاپ_func_t ميلاپ_func_2,
			     ثابت فراغ *ميلاپ_معلومات_2,
			     بنانشان صحيح شروعات_اشاريو,
			     بنانشان صحيح *پڇاڙي_اشاريو)
{
  TRACE_APPLY (خالي_اشارو);

  إذا (!ڳڻپ)
  {
    *پڇاڙي_اشاريو = شروعات_اشاريو;
    return_trace (سچ);
  }

  خودڪار &skippy_iter = c->iter_context;
  أكد (شروعات_اشاريو >= 1);
  skippy_iter.reset (شروعات_اشاريو - 1);
  skippy_iter.سيٽ_ميلاپ_func (ميلاپ_func_2, ميلاپ_معلومات_2);
  skippy_iter.سيٽ_glyph_معلومات (lookahead);

  لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
  {
    بنانشان unsafe_منزل;
    إذا (!skippy_iter.next (&unsafe_منزل))
    {
      *پڇاڙي_اشاريو = unsafe_منزل;
      return_trace (خطأ);
    }
  }

  *پڇاڙي_اشاريو = skippy_iter.idx + 1;
  return_trace (سچ);
}



بنية Lookup_رڪارڊ
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t         *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->embed (*هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    return_trace (c->check_سونپيو (ٻاهريون->lookup_List_اشاريو, lookup_map->get (lookup_List_اشاريو), HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	sequence_اشاريو;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookup_List_اشاريو;	/* Lookup to apply to that
					 * position--zero--based */
  عام:
  DEFINE_سائيز_STATIC (4);
};

ساكن مضمن بنانشان serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ثابت hb_array_t<ثابت Lookup_رڪارڊ> lookup_رڪارڊون,
						     ثابت hb_map_t *lookup_map)
{
  بنانشان ڳڻپ = 0;
  لكل (ثابت Lookup_رڪارڊ& r : lookup_رڪارڊون)
  {
    إذا (!lookup_map->has (r.lookup_List_اشاريو))
      استمر;

    إذا (!r.serialize (c, lookup_map))
      أرجع 0;

    ڳڻپ++;
  }
  أرجع ڳڻپ;
}

تعداد ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ساكن مضمن ثابت hb_سيٽ_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ثابت فراغ *معلومات,
			بنانشان value,
			منطقي filter_class_by_والد,
			hb_سيٽ_t *scratch,
			hb_ويڪٽر_t<بنانشان> *compact_workspace = خالي_اشارو)
{
  بدل (context_format)
  {
    حالة ContextFormat::SimpleContext:
      أرجع c->ڳوليو_or_create_glyph_سيٽ (خالي_اشارو, value,
                                           hb_depend_context_t::GLYPH_سيٽ_SINGLE);
    حالة ContextFormat::ClassBasedContext:
    {
      ثابت hb_سيٽ_t *class_سيٽ = c->ڳوليو_or_create_glyph_سيٽ (
        معلومات, value, hb_depend_context_t::GLYPH_سيٽ_CLASS);
      إذا (unlikely (!class_سيٽ))
	أرجع خالي_اشارو;
      إذا (!filter_class_by_والد)
	أرجع class_سيٽ;
      scratch->سيٽ (*class_سيٽ);
      scratch->intersect (c->والد_سرگرم_glyphs (), compact_workspace);
      اكسر;
    }
    حالة ContextFormat::CoverageBasedContext:
      أرجع c->ڳوليو_or_create_glyph_سيٽ (
        معلومات, value, hb_depend_context_t::GLYPH_سيٽ_COVERAGE);
  }

  إذا (unlikely (scratch->اندريون_error ()))
  {
    c->depend_معلومات->fail ();
    أرجع خالي_اشارو;
  }
  أرجع scratch;
}

ساكن مضمن فراغ context_گهيرو_recurse_lookups (hb_گهيرو_context_t *c,
					     بنانشان داخلا_ڳڻپ_2, glyph_معلومات_t داخلا_2,
					     بنانشان lookup_ڳڻپ_2,
					     ثابت Lookup_رڪارڊ lookup_رڪارڊ[] /* Array of LookupRecords--in design order */,
					     بنانشان value,
					     ContextFormat context_format,
					     ثابت فراغ *معلومات,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache)
{
  hb_سيٽ_t covered_seq_indicies;
  hb_سيٽ_t pos_glyphs;
  لكل (بنانشان صحيح i = 0; i < lookup_ڳڻپ_2; i++)
  {
    بنانشان seq_اشاريو_2 = lookup_رڪارڊ[i].sequence_اشاريو;
    إذا (seq_اشاريو_2 >= داخلا_ڳڻپ_2) استمر;

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indicies.has (seq_اشاريو_2))
    {
      has_pos_glyphs = سچ;
      pos_glyphs.clear ();
      إذا (seq_اشاريو_2 == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.جوڙيو (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->والد_سرگرم_glyphs (), معلومات, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.سيٽ (c->والد_سرگرم_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *داخلا_معلومات_2 = داخلا_2.معلومات;
        بنانشان داخلا_قدر = seq_اشاريو_2 - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          داخلا_معلومات_2 = معلومات;
          داخلا_قدر = داخلا_2[seq_اشاريو_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, داخلا_معلومات_2, داخلا_قدر, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.جوڙيو (seq_اشاريو_2);
    hb_سيٽ_t *cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs))
      أرجع;
    إذا (has_pos_glyphs) {
      *cur_سرگرم_glyphs = std::move (pos_glyphs);
    } وإلا {
      *cur_سرگرم_glyphs = *c->glyphs;
    }

    بنانشان پڇاڙي_اشاريو_2 = داخلا_ڳڻپ_2;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      پڇاڙي_اشاريو_2 += 1;

    c->recurse (lookup_رڪارڊ[i].lookup_List_اشاريو, &covered_seq_indicies, seq_اشاريو_2, پڇاڙي_اشاريو_2);

    c->ڪڍو_cur_پورو_glyphs ();
  }
}

ساكن منطقي
context_depend_sets_are_cached (hb_depend_context_t *c,
				بنانشان داخلا_ڳڻپ_3,
				بنانشان lookup_ڳڻپ,
				ثابت Lookup_رڪارڊ lookup_رڪارڊون_2[],
				ثابت فراغ *قاعدو_2,
				بنانشان value,
				ContextFormat context_format,
				ثابت فراغ *معلومات,
				hb_ويڪٽر_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t سرگرم_idx = HB_CODEPOINT_INVALID;
  منطقي have_سرگرم_idx = خطأ;
  لكل (بنانشان i = 0; i < lookup_ڳڻپ; i++)
  {
    إذا (lookup_رڪارڊون_2[i].sequence_اشاريو >= داخلا_ڳڻپ_3)
      استمر;

    إذا (!have_سرگرم_idx)
    {
      إذا (unlikely (!c->get_والد_سرگرم_glyphs_اشاريو (&سرگرم_idx)))
	أرجع خطأ;
      have_سرگرم_idx = سچ;
    }

    hb_depend_context_t::context_سيٽ_cache_ڪنجي_t key (
      قاعدو_2, &lookup_رڪارڊون_2[i], معلومات, سرگرم_idx, value, context_format);
    hb_codepoint_t *cached_context_سيٽ = خالي_اشارو;
    إذا (!c->context_سيٽ_cache.has (key, &cached_context_سيٽ))
      أرجع خطأ;
    إذا (unlikely (!cached_context_sets->ڌڪو_or_fail (*cached_context_سيٽ)))
      أرجع c->depend_معلومات->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  أرجع have_سرگرم_idx;
}

ساكن مضمن فراغ context_depend_recurse_lookups (hb_depend_context_t *c,
					     بنانشان داخلا_ڳڻپ_2, glyph_معلومات_t داخلا_2,
					     بنانشان lookup_ڳڻپ_2,
					     ثابت Lookup_رڪارڊ lookup_رڪارڊ[] /* Array of LookupRecords--in design order */,
					     ثابت فراغ *قاعدو_2,
					     بنانشان value,
					     ContextFormat context_format,
					     ثابت فراغ *معلومات,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache,
					     ثابت hb_سيٽ_t &preliminary_context,
					     ثابت hb_ويڪٽر_t<ثابت hb_سيٽ_t *> *داخلا_position_glyphs,
					     ثابت hb_ويڪٽر_t<hb_codepoint_t> *cached_context_sets,
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
  إذا (unlikely (داخلا_ڳڻپ_2 > HB_MAX_CONTEXT_ڊيگهائي))
    أرجع;

  hb_ٻٽو_انگ_صفحو_t covered_seq_indices;
  hb_سيٽ_t &pos_glyphs = scratch->pos_glyphs;
  hb_سيٽ_t &position_context = scratch->position_context;
  hb_سيٽ_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_سيٽ_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  بنانشان cached_context_اشاريو = 0;
  لكل (بنانشان صحيح i = 0; i < lookup_ڳڻپ_2; i++)
  {
    بنانشان seq_اشاريو_2 = lookup_رڪارڊ[i].sequence_اشاريو;
    إذا (seq_اشاريو_2 >= داخلا_ڳڻپ_2)
      استمر;

    hb_depend_context_t::context_سيٽ_cache_ڪنجي_t context_cache_ڪنجي;
    إذا (unlikely (!c->get_context_سيٽ_cache_ڪنجي (قاعدو_2, &lookup_رڪارڊ[i], معلومات, value,
						 context_format, &context_cache_ڪنجي)))
      أرجع;

    hb_codepoint_t context_سيٽ_idx;
    hb_codepoint_t *cached_context_سيٽ_idx;
    إذا (cached_context_sets)
      context_سيٽ_idx = (*cached_context_sets)[cached_context_اشاريو++];
    وإلا إذا (c->context_سيٽ_cache.has (context_cache_ڪنجي, &cached_context_سيٽ_idx))
      context_سيٽ_idx = *cached_context_سيٽ_idx;
    وإلا
    {
      إذا (unlikely (!داخلا_position_glyphs))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_سيٽ_قدر_INVALID;
      طالما (preliminary_context.next (&elem)) {
	إذا (!(elem & HB_DEPEND_CONTEXT_سيٽ_FLAG)) {
	  /* Direct glyph */
	  position_context.جوڙيو (elem);
	} وإلا {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.جوڙيو (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      لكل (بنانشان j = 0; j < داخلا_position_glyphs->ڊيگهائي; j++) {
	إذا (j != seq_اشاريو_2 && (*داخلا_position_glyphs)[j]->get_population () == 1) {
	  position_context.جوڙيو ((*داخلا_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_سيٽ_قدر_INVALID;
      طالما (disjunctive_indices.next (&elem)) {
	hb_codepoint_t سيٽ_idx = elem & 0x7FFFFFFF;
	ثابت hb_سيٽ_t *original_سيٽ = c->depend_معلومات->get_سيٽ_from_اشاريو (سيٽ_idx);
	إذا (unlikely (!original_سيٽ)) استمر;

	/* Dependency context sets are never inverted. */
	إذا (!original_سيٽ->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.جوڙيو (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      لكل (بنانشان j = 0; j < داخلا_position_glyphs->ڊيگهائي; j++) {
	إذا (j != seq_اشاريو_2 && (*داخلا_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  إذا (!(*داخلا_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    منطقي cached_سيٽ = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_سيٽ
	      ? c->ڳوليو_or_create_cached_context_سيٽ ((*داخلا_position_glyphs)[j])
	      : c->depend_معلومات->ڳوليو_or_create_context_سيٽ (*(*داخلا_position_glyphs)[j]);
	    إذا (unlikely (idx == HB_CODEPOINT_INVALID))
	      أرجع;
	    filtered_disjunctive_indices.جوڙيو (HB_DEPEND_CONTEXT_سيٽ_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      إذا (unlikely (position_context.اندريون_error () ||
		    filtered_disjunctive_indices.اندريون_error ()))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }

      context_سيٽ_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_معلومات->ڳوليو_or_create_context_سيٽ (position_context);
      إذا (unlikely (!position_context.is_empty () &&
		    context_سيٽ_idx == HB_CODEPOINT_INVALID))
	أرجع;

      إذا (unlikely (!c->context_سيٽ_cache.سيٽ (context_cache_ڪنجي, context_سيٽ_idx)))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_سيٽ_اشاريو = c->depend_معلومات->current_context_سيٽ_اشاريو;
    hb_subset_depend_ڪنارو_flags_t saved_ڪنارو_flags = c->depend_معلومات->current_ڪنارو_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    منطقي nested_contextual = (saved_context_سيٽ_اشاريو != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_معلومات->current_context_سيٽ_اشاريو = context_سيٽ_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_معلومات->current_ڪنارو_flags = HB_SUBSET_DEPEND_ڪنارو_FLAG_NONE;
    إذا (داخلا_ڳڻپ_2 > 1) {
      c->depend_معلومات->current_ڪنارو_flags |= HB_SUBSET_DEPEND_ڪنارو_FLAG_FROM_CONTEXT_POSITION;
    }
    إذا (nested_contextual) {
      c->depend_معلومات->current_ڪنارو_flags |= HB_SUBSET_DEPEND_ڪنارو_FLAG_FROM_NESTED_CONTEXT;
    }

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indices.has (seq_اشاريو_2))
    {
      has_pos_glyphs = سچ;
      pos_glyphs.clear ();
      إذا (seq_اشاريو_2 == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.جوڙيو (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->والد_سرگرم_glyphs (), معلومات, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.سيٽ (c->والد_سرگرم_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *داخلا_معلومات_2 = داخلا_2.معلومات;
        بنانشان داخلا_قدر = seq_اشاريو_2 - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          داخلا_معلومات_2 = معلومات;
          داخلا_قدر = داخلا_2[seq_اشاريو_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, داخلا_معلومات_2, داخلا_قدر, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.جوڙيو (seq_اشاريو_2);
    إذا (has_pos_glyphs)
    {
      hb_codepoint_t سرگرم_idx = HB_CODEPOINT_INVALID;
      إذا (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_اشاريو_2 ? pos_glyphs.get_min () : value;
	إذا (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_سرگرم_FLAG))
	  سرگرم_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_سرگرم_FLAG;
      }
      hb_سيٽ_t *cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs (سرگرم_idx);
      إذا (unlikely (!cur_سرگرم_glyphs))
      {
	c->depend_معلومات->current_context_سيٽ_اشاريو = saved_context_سيٽ_اشاريو;
	c->depend_معلومات->current_ڪنارو_flags = saved_ڪنارو_flags;
	أرجع;
      }
      *cur_سرگرم_glyphs = std::move (pos_glyphs);
    }
    وإلا إذا (unlikely (!c->ڌڪو_پاڙ_سرگرم_glyphs ()))
    {
      c->depend_معلومات->current_context_سيٽ_اشاريو = saved_context_سيٽ_اشاريو;
      c->depend_معلومات->current_ڪنارو_flags = saved_ڪنارو_flags;
      أرجع;
    }

    بنانشان پڇاڙي_اشاريو_2 = داخلا_ڳڻپ_2;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      پڇاڙي_اشاريو_2 += 1;

    c->recurse (lookup_رڪارڊ[i].lookup_List_اشاريو, &covered_seq_indices, seq_اشاريو_2, پڇاڙي_اشاريو_2);
    c->ڪڍو_cur_پورو_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_معلومات->current_context_سيٽ_اشاريو = saved_context_سيٽ_اشاريو;
    c->depend_معلومات->current_ڪنارو_flags = saved_ڪنارو_flags;
  }
}

سانچو <قسم_نالو context_t>
ساكن مضمن فراغ recurse_lookups (context_t *c,
                                    بنانشان صحيح lookup_ڳڻپ_2,
                                    ثابت Lookup_رڪارڊ lookup_رڪارڊ[] /* Array of LookupRecords--in design order */)
{
  لكل (بنانشان صحيح i = 0; i < lookup_ڳڻپ_2; i++)
    c->recurse (lookup_رڪارڊ[i].lookup_List_اشاريو);
}

ساكن مضمن فراغ apply_lookup (hb_ot_apply_context_t *c,
				 بنانشان صحيح ڳڻپ, /* Including the first glyph */
				 بنانشان صحيح lookup_ڳڻپ_2,
				 ثابت Lookup_رڪارڊ lookup_رڪارڊ[], /* Array of LookupRecords--in design order */
				 بنانشان صحيح ميلاپ_پڇاڙي)
{
  hb_بفر_t *بفر = c->بفر;
  صحيح پڇاڙي;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    بنانشان صحيح bl = بفر->backtrack_len ();
    پڇاڙي = bl + ميلاپ_پڇاڙي - بفر->idx;

    صحيح delta = bl - بفر->idx;
    /* Convert positions to new indexing. */
    لكل (بنانشان صحيح j = 0; j < ڳڻپ; j++)
      c->ميلاپ_positions.arrayZ[j] += delta;
  }

  لكل (بنانشان صحيح i = 0; i < lookup_ڳڻپ_2 && بفر->successful; i++)
  {
    بنانشان صحيح idx = lookup_رڪارڊ[i].sequence_اشاريو;
    إذا (idx >= ڳڻپ)
      استمر;

    بنانشان صحيح orig_len = بفر->backtrack_len () + بفر->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    إذا (unlikely (c->ميلاپ_positions.arrayZ[idx] >= orig_len))
      استمر;

    إذا (unlikely (!بفر->move_منزل (c->ميلاپ_positions.arrayZ[idx])))
      اكسر;

    إذا (unlikely (بفر->max_ops <= 0))
      اكسر;

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      إذا (بفر->have_output)
        c->بفر->sync_so_far ();
      c->بفر->پيغام (c->font,
			  "start recursing to lookup %u at %u",
			  (بنانشان) lookup_رڪارڊ[i].lookup_List_اشاريو,
			  بفر->idx);
    }

    منطقي ret = c->recurse (lookup_رڪارڊ[i].lookup_List_اشاريو);

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      إذا (بفر->have_output)
        c->بفر->sync_so_far ();
      c->بفر->پيغام (c->font,
			  "end recursing to lookup %u",
			  (بنانشان) lookup_رڪارڊ[i].lookup_List_اشاريو);
    }

    إذا (!ret)
      استمر;

    بنانشان صحيح new_len = بفر->backtrack_len () + بفر->lookahead_len ();
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

    پڇاڙي += delta;
    إذا (پڇاڙي < صحيح (c->ميلاپ_positions.arrayZ[idx]))
    {
      /* End might end up being smaller than match_positions.arrayZ[idx] if the recursed
       * lookup ended up removing many items.
       * Just never rewind end beyond start of current position, since that is
       * not possible in the recursed lookup.  Also adjust delta as such.
       *
       * https://bugs.chromium.org/p/chromium/issues/detail?id=659496
       * https://github.com/harfbuzz/harfbuzz/issues/1611
       */
      delta += c->ميلاپ_positions.arrayZ[idx] - پڇاڙي;
      پڇاڙي = c->ميلاپ_positions.arrayZ[idx];
    }

    بنانشان صحيح next = idx + 1; /* next now is the position after the recursed lookup. */

    إذا (delta > 0)
    {
      إذا (unlikely (delta + ڳڻپ > HB_MAX_CONTEXT_ڊيگهائي))
	اكسر;
      إذا (unlikely (ڳڻپ + delta > c->ميلاپ_positions.ڊيگهائي &&
		    !c->ميلاپ_positions.resize_dirty  (ڳڻپ + delta)))
        أرجع;
    }
    وإلا
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (صحيح) next - (صحيح) ڳڻپ);
      next -= delta;
    }

    /* Shift! */
    memmove (c->ميلاپ_positions + next + delta, c->ميلاپ_positions + next,
	     (ڳڻپ - next) * حجم (c->ميلاپ_positions.arrayZ[0]));
    next += delta;
    ڳڻپ += delta;

    /* Fill in new entries. */
    لكل (بنانشان صحيح j = idx + 1; j < next; j++)
      c->ميلاپ_positions.arrayZ[j] = c->ميلاپ_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    لكل (; next < ڳڻپ; next++)
      c->ميلاپ_positions.arrayZ[next] += delta;
  }

  أكد (پڇاڙي >= 0);
  (فراغ) بفر->move_منزل (پڇاڙي);
}



/* Contextual lookups */

بنية Context_گهيرو_Lookup_Context
{
  Context_گهيرو_Funcs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_معلومات;
  فراغ *intersects_cache;
  فراغ *intersected_glyphs_cache;
};

تعريفنوع Context_گهيرو_Lookup_Context ContextDependLookupContext;

بنية ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_معلومات;
};

بنية ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ثابت فراغ *ميلاپ_معلومات_2;
  get_قدر_func_t get_قدر;
};

ساكن مضمن منطقي context_intersects (ثابت hb_سيٽ_t *glyphs,
				       بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
				       glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
				       Context_گهيرو_Lookup_Context &lookup_context)
{
  أرجع array_is_subset_جو (glyphs,
			     داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0, داخلا_2,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_معلومات,
			     lookup_context.intersects_cache);
}

ساكن مضمن فراغ context_گهيرو_lookup (hb_گهيرو_context_t *c,
					   بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
					   glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
					   بنانشان صحيح lookup_ڳڻپ_2,
					   ثابت Lookup_رڪارڊ lookup_رڪارڊ[],
					   بنانشان value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   Context_گهيرو_Lookup_Context &lookup_context)
{
  إذا (context_intersects (c->glyphs,
			  داخلا_ڳڻپ_2, داخلا_2,
			  lookup_context))
    context_گهيرو_recurse_lookups (c,
				     داخلا_ڳڻپ_2, داخلا_2,
				     lookup_ڳڻپ_2, lookup_رڪارڊ,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_معلومات,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ context_depend_lookup (hb_depend_context_t *c,
					  بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
					  glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
					  بنانشان صحيح lookup_ڳڻپ_2,
					  ثابت Lookup_رڪارڊ lookup_رڪارڊ[],
					  ثابت فراغ *قاعدو_2,
					  بنانشان value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  إذا (!context_intersects (c->glyphs,
			   داخلا_ڳڻپ_2, داخلا_2,
			   lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, داخلا_ڳڻپ_2, lookup_ڳڻپ_2, lookup_رڪارڊ, قاعدو_2, value,
    lookup_context.context_format, lookup_context.intersects_معلومات,
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_معلومات->successful))
    أرجع;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_سيٽ_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_سيٽ_t &position_scratch = scratch->position_scratch;
  hb_ويڪٽر_t<ثابت hb_سيٽ_t *> &داخلا_position_glyphs = scratch->داخلا_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  إذا (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ثابت hb_سيٽ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_معلومات, value,
      سچ, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !داخلا_position_glyphs.ڌڪو_or_fail (pos0_glyphs)))
    {
      c->depend_معلومات->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (بنانشان i = 0; i + 1 < داخلا_ڳڻپ_2; i++)
    {
      ثابت hb_سيٽ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_معلومات,
	داخلا_2[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !داخلا_position_glyphs.ڌڪو_or_fail (pos_glyphs)))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  داخلا_ڳڻپ_2, داخلا_2,
				  lookup_ڳڻپ_2, lookup_رڪارڊ,
				  قاعدو_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_معلومات,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? خالي_اشارو : &داخلا_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : خالي_اشارو,
				  scratch);
}

ساكن مضمن فراغ context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
						  glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
						  بنانشان صحيح lookup_ڳڻپ_2,
						  ثابت Lookup_رڪارڊ lookup_رڪارڊ[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->داخلا_2,
		 داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0, داخلا_2,
		 lookup_context.funcs.collect, lookup_context.collect_معلومات);
  recurse_lookups (c,
		   lookup_ڳڻپ_2, lookup_رڪارڊ);
}

ساكن مضمن منطقي context_would_apply_lookup (hb_would_apply_context_t *c,
					       بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
					       glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
					       بنانشان صحيح lookup_ڳڻپ_2 HB_اڻ_استعمال_ٿيل,
					       ثابت Lookup_رڪارڊ lookup_رڪارڊ[] HB_اڻ_استعمال_ٿيل,
					       ثابت ContextApplyLookupContext &lookup_context)
{
  أرجع would_ميلاپ_داخلا (c,
			    داخلا_ڳڻپ_2, داخلا_2,
			    lookup_context.funcs.ميلاپ, lookup_context.ميلاپ_معلومات_2);
}

ساكن مضمن منطقي context_apply_lookup (hb_ot_apply_context_t *c,
					 بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
					 glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
					 بنانشان صحيح lookup_ڳڻپ_2,
					 ثابت Lookup_رڪارڊ lookup_رڪارڊ[],
					 ثابت ContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (داخلا_ڳڻپ_2 > HB_MAX_CONTEXT_ڊيگهائي)) أرجع خطأ;

  بنانشان ميلاپ_پڇاڙي = 0;
  منطقي ret = خطأ;
  إذا (ميلاپ_داخلا (c,
		   داخلا_ڳڻپ_2, داخلا_2,
		   lookup_context.funcs.ميلاپ, lookup_context.ميلاپ_معلومات_2,
		   &ميلاپ_پڇاڙي))
  {
    c->بفر->unsafe_منزل_break (c->بفر->idx, ميلاپ_پڇاڙي);
    apply_lookup (c,
		  داخلا_ڳڻپ_2,
		  lookup_ڳڻپ_2, lookup_رڪارڊ,
		  ميلاپ_پڇاڙي);
    ret = سچ;
  }
  وإلا
  {
    c->بفر->unsafe_منزل_concat (c->بفر->idx, ميلاپ_پڇاڙي);
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
      إذا (!HB_بفر_TRY_ALLOCATE_VAR (c->بفر, syllable))
	أرجع خطأ;
      خودڪار &info = c->بفر->info;
      بنانشان ڳڻپ = c->بفر->len;
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      أرجع سچ;
    }
    حالة hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (بنانشان) -1;
      HB_بفر_DEALLOCATE_VAR (c->بفر, syllable);
      اكسر;
    }
  }
  أرجع خطأ;
}

سانچو <قسم_نالو Types>
بنية قاعدو_3
{
  سانچو <قسم_نالو سيٽ_Types, قسم_نالو قاعدو_Type>
  دوست بنية قاعدو_سيٽ_جو;

  منطقي intersects (ثابت hb_سيٽ_t *glyphs, Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    أرجع context_intersects (glyphs,
			       داخلا_ڳڻپ_2, داخلا_Z.arrayZ,
			       lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, بنانشان value, ContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خودڪار &lookup_رڪارڊ = StructAfter<Unsized_Array_جو<Lookup_رڪارڊ>>
					   (داخلا_Z.as_array ((داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0)));
    context_depend_lookup (c,
			   داخلا_ڳڻپ_2, داخلا_Z.arrayZ,
			   lookup_ڳڻپ_2, lookup_رڪارڊ.arrayZ,
			   هذا,
			   value, lookup_context);
  }

  فراغ گهيرو (hb_گهيرو_context_t *c, بنانشان value, Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندي_exceeded ())) أرجع;

    ثابت خودڪار &lookup_رڪارڊ = StructAfter<Unsized_Array_جو<Lookup_رڪارڊ>>
					   (داخلا_Z.as_array ((داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0)));
    context_گهيرو_lookup (c,
			    داخلا_ڳڻپ_2, داخلا_Z.arrayZ,
			    lookup_ڳڻپ_2, lookup_رڪارڊ.arrayZ,
			    value, lookup_context);
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c,
                        Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندي_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت خودڪار &lookup_رڪارڊ = StructAfter<Unsized_Array_جو<Lookup_رڪارڊ>>
					   (داخلا_Z.as_array (داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0));
    recurse_lookups (c, lookup_ڳڻپ_2, lookup_رڪارڊ.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خودڪار &lookup_رڪارڊ = StructAfter<Unsized_Array_جو<Lookup_رڪارڊ>>
					   (داخلا_Z.as_array (داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0));
    context_collect_glyphs_lookup (c,
				   داخلا_ڳڻپ_2, داخلا_Z.arrayZ,
				   lookup_ڳڻپ_2, lookup_رڪارڊ.arrayZ,
				   lookup_context);
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest,
			      ثابت ClassDef *class_def = خالي_اشارو) ثابت
  {
    إذا (داخلا_ڳڻپ_2 <= 1)
    {
      *digest = hb_سيٽ_digest_t::ڀريل ();
      أرجع;
    }

    بنانشان second = داخلا_Z.arrayZ[0];
    إذا (!class_def)
      digest->جوڙيو (second);
    وإلا إذا (!second)
      *digest = hb_سيٽ_digest_t::ڀريل ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خودڪار &lookup_رڪارڊ = StructAfter<Unsized_Array_جو<Lookup_رڪارڊ>>
					   (داخلا_Z.as_array (داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0));
    أرجع context_would_apply_lookup (c,
				       داخلا_ڳڻپ_2, داخلا_Z.arrayZ,
				       lookup_ڳڻپ_2, lookup_رڪارڊ.arrayZ,
				       lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودڪار &lookup_رڪارڊ = StructAfter<Unsized_Array_جو<Lookup_رڪارڊ>>
					   (داخلا_Z.as_array (داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0));
    return_trace (context_apply_lookup (c, داخلا_ڳڻپ_2, داخلا_Z.arrayZ, lookup_ڳڻپ_2, lookup_رڪارڊ.arrayZ, lookup_context));
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *داخلا_mapping, /* old->new glyphid or class mapping */
		  ثابت hb_map_t *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->شروعات_embed (هذا);
    إذا (unlikely (!c->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->داخلا_ڳڻپ_2 = داخلا_ڳڻپ_2;
    ثابت خودڪار داخلا_2 = داخلا_Z.as_array (داخلا_ڳڻپ_2 - 1);
    لكل (ثابت خودڪار org : داخلا_2)
    {
      قسم_نالو Types::HBUINT d;
      d = داخلا_mapping->get (org);
      c->copy (d);
    }

    ثابت خودڪار &lookup_رڪارڊ = StructAfter<Unsized_Array_جو<Lookup_رڪارڊ>>
					   (داخلا_Z.as_array ((داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0)));

    بنانشان ڳڻپ = serialize_lookuprecord_array (c, lookup_رڪارڊ.as_array (lookup_ڳڻپ_2), lookup_map);
    return_trace (c->check_سونپيو (ٻاهريون->lookup_ڳڻپ_2, ڳڻپ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!داخلا_ڳڻپ_2)) return_trace (خطأ);
    ثابت خودڪار داخلا_2 = داخلا_Z.as_array (داخلا_ڳڻپ_2 - 1);

    ثابت hb_map_t *mapping = klass_map == خالي_اشارو ? c->plan->glyph_map : klass_map;
    إذا (!hb_all (داخلا_2, mapping)) return_trace (خطأ);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (داخلا_Z.arrayZ,
				  داخلا_Z.شي_سائيز * (داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0) +
				  Lookup_رڪارڊ::static_سائيز * lookup_ڳڻپ_2));
  }

  محمي:
  HBUINT16	داخلا_ڳڻپ_2;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_ڳڻپ_2;		/* Number of LookupRecords */
  Unsized_Array_جو<قسم_نالو Types::HBUINT>
		داخلا_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_سائيز_ARRAY (4, داخلا_Z);
};

سانچو <قسم_نالو Types, قسم_نالو قاعدو_Type>
بنية قاعدو_سيٽ_جو
{
  منطقي intersects (ثابت hb_سيٽ_t *glyphs,
		   Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_map ([&] (ثابت قاعدو_Type &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_پهريون_داخلا_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (بنانشان i = 0; i < قاعدو_2.len; i++)
    {
      ثابت قاعدو_Type &r = هذا+قاعدو_2.arrayZ[i];
      إذا (r.داخلا_ڳڻپ_2 <= 1)
      {
	digest->سيٽ_ڀريل ();
	أرجع;
      }
      digest->جوڙيو (r.داخلا_Z.arrayZ[0]);
    }
  }

  فراغ depend (hb_depend_context_t *c, بنانشان value, ContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت قاعدو_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }

  فراغ گهيرو (hb_گهيرو_context_t *c, بنانشان value,
		Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندي_exceeded ())) أرجع;

    أرجع
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت قاعدو_Type &_) { _.گهيرو (c, value, lookup_context); })
    ;
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c,
                        Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندي_exceeded ())) أرجع;
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت قاعدو_Type &_) { _.گهيرو_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت قاعدو_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest,
			      ثابت ClassDef *class_def = خالي_اشارو) ثابت
  {
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([=] (ثابت قاعدو_Type &_) { _.collect_ٻيو_glyphs (digest, class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_map ([&] (ثابت قاعدو_Type &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = خالي_اشارو) ثابت
  {
    TRACE_APPLY (هذا);

    بنانشان انگ_قاعدا = قاعدو_2.len;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_RULESETS_FAST_رستو
    إذا (HB_OPTIMIZE_سائيز_VAL || انگ_قاعدا <= 4)
#جيڪڏهن_پورو
    {
    slow:
      return_trace (
      + hb_iter (قاعدو_2)
      | hb_map (hb_جوڙيو (هذا))
      | hb_map ([&] (ثابت قاعدو_Type &_) { أرجع _.apply (c, lookup_context); })
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
    خودڪار &skippy_iter = c->iter_context;
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سيٽ_ميلاپ_func (ميلاپ_always, خالي_اشارو);
    skippy_iter.سيٽ_glyph_معلومات ((HBUINT16 *) خالي_اشارو);
    بنانشان unsafe_منزل = (بنانشان) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *پهريون = خالي_اشارو, *second = خالي_اشارو;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      پهريون = &c->بفر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_قدر &&
	  !digest->may_have (lookup_context.get_قدر (*پهريون, lookup_context.ميلاپ_معلومات_2)))
	{
	  c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_منزل_concat = خطأ;
      منطقي ret =
      + hb_iter (قاعدو_2)
      | hb_map (hb_جوڙيو (هذا))
      | hb_filter ([&] (ثابت قاعدو_Type &_)
		   {
		     إذا (_.داخلا_ڳڻپ_2 <= 1) أرجع سچ;
		     unsafe_منزل_concat = سچ;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت قاعدو_Type &_) { أرجع _.apply (c, lookup_context); })
      | hb_any
      ;
      إذا (unsafe_منزل_concat)
	c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      second = &c->بفر->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    خودڪار ميلاپ_داخلا = lookup_context.funcs.ميلاپ;
    خودڪار *داخلا_معلومات_2 = lookup_context.ميلاپ_معلومات_2;
    لكل (بنانشان صحيح i = 0; i < انگ_قاعدا; i++)
    {
      ثابت خودڪار &r = هذا+قاعدو_2.arrayZ[i];

      ثابت خودڪار &داخلا_2 = r.داخلا_Z;

      إذا (r.داخلا_ڳڻپ_2 <= 1 ||
	  (!ميلاپ_داخلا ||
	   ميلاپ_داخلا (*پهريون, داخلا_2.arrayZ[0], داخلا_معلومات_2)))
      {
        إذا (!second ||
	    (r.داخلا_ڳڻپ_2 <= 2 ||
	     (!ميلاپ_داخلا ||
	      ميلاپ_داخلا (*second, داخلا_2.arrayZ[1], داخلا_معلومات_2)))
	   )
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_منزل != (بنانشان) -1)
	      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
	    return_trace (سچ);
	  }
	}
	وإلا
	  unsafe_منزل = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_منزل == (بنانشان) -1)
	  unsafe_منزل = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	لكل (; i + 1 < انگ_قاعدا; i++)
	{
	  ثابت خودڪار &r2 = هذا+قاعدو_2.arrayZ[i + 1];
	  ثابت خودڪار &input2 = r2.داخلا_Z;
	  إذا (r2.داخلا_ڳڻپ_2 <= 1 || input2.arrayZ[0] != داخلا_2.arrayZ[0])
	    اكسر;
	}
      }
    }
    إذا (likely (unsafe_منزل != (بنانشان) -1))
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);

    خودڪار snap = c->serializer->snapshot ();
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    لكل (ثابت خودڪار& _ : قاعدو_2)
    {
      إذا (!_) استمر;
      خودڪار o_snap = c->serializer->snapshot ();
      خودڪار *o = ٻاهريون->قاعدو_2.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا, lookup_map, klass_map))
      {
	ٻاهريون->قاعدو_2.ڪڍو ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (ٻاهريون->قاعدو_2);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (قاعدو_2.sanitize (c, هذا));
  }

  محمي:
  Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<قاعدو_Type>>
		قاعدو_2;			/* Array of Rule tables
					 * ordered by preference */
  عام:
  DEFINE_سائيز_ARRAY (2, قاعدو_2);
};

سانچو <قسم_نالو Types>
بنية قاعدو_سيٽ : قاعدو_سيٽ_جو<SmallTypes, قاعدو_3<Types>> {};

سانچو <قسم_نالو Types>
بنية Class_قاعدو_سيٽ : قاعدو_سيٽ_جو<Types, قاعدو_3<SmallTypes>> {};


سانچو <قسم_نالو Types>
بنية ContextFormat1_4
{
  استعمال قاعدو_سيٽ = OT::قاعدو_سيٽ<Types>;

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    بنية Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالي_اشارو
    };

    أرجع
    + hb_zip (هذا+coverage, قاعدو_سيٽ_2)
    | hb_filter (*glyphs, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_map ([&] (ثابت قاعدو_سيٽ &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (*c->glyphs, *cur_سرگرم_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالي_اشارو,
      خالي_اشارو,  /* intersects_cache */
      خالي_اشارو   /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((بنانشان) قاعدو_سيٽ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->والد_سرگرم_glyphs ().has (_);
    }, hb_پهريون)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بنانشان> _) { أرجع hb_pair_t<بنانشان, ثابت قاعدو_سيٽ&> (_.پهريون, هذا+قاعدو_سيٽ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بنانشان, ثابت قاعدو_سيٽ&>& _) { _.second.depend (c, _.پهريون, lookup_context); })
    ;

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (c->اڳيون_والد_سرگرم_glyphs (), *cur_سرگرم_glyphs);

    بنية Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالي_اشارو
    };

    + hb_zip (هذا+coverage, hb_range ((بنانشان) قاعدو_سيٽ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->اڳيون_والد_سرگرم_glyphs ().has (_);
    }, hb_پهريون)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بنانشان> _) { أرجع hb_pair_t<بنانشان, ثابت قاعدو_سيٽ&> (_.پهريون, هذا+قاعدو_سيٽ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بنانشان, ثابت قاعدو_سيٽ&>& _) { _.second.گهيرو (c, _.پهريون, lookup_context); })
    ;

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت
  {
    بنية Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_glyph, خالي_اشارو},
      ContextFormat::SimpleContext,
      خالي_اشارو
    };

    + hb_zip (هذا+coverage, قاعدو_سيٽ_2)
    | hb_filter (*c->glyphs, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت قاعدو_سيٽ &_) { _.گهيرو_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->داخلا_2);

    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      خالي_اشارو
    };

    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت قاعدو_سيٽ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  {
    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([digest] (ثابت قاعدو_سيٽ &_) { _.collect_ٻيو_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت قاعدو_سيٽ &قاعدو_سيٽ_3 = هذا+قاعدو_سيٽ_2[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ContextApplyLookupContext lookup_context = {
      {ميلاپ_glyph},
      خالي_اشارو
    };
    أرجع قاعدو_سيٽ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (likely (اشاريو_2 == NOT_COVERED))
      return_trace (خطأ);

    ثابت قاعدو_سيٽ &قاعدو_سيٽ_3 = هذا+قاعدو_سيٽ_2[اشاريو_2];
    بنية ContextApplyLookupContext lookup_context = {
      {ميلاپ_glyph},
      خالي_اشارو
    };
    return_trace (قاعدو_سيٽ_3.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, قاعدو_سيٽ_2)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (subset_وٿي_array (c, ٻاهريون->قاعدو_سيٽ_2, هذا, lookup_map), hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ٻاهريون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && قاعدو_سيٽ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::سانچو وٿي_منزل<قاعدو_سيٽ>>
		قاعدو_سيٽ_2;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز, قاعدو_سيٽ_2);
};


سانچو <قسم_نالو Types>
بنية ContextFormat2_5
{
  استعمال قاعدو_سيٽ = OT::Class_قاعدو_سيٽ<Types>;

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_class, خالي_اشارو},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_سيٽ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سيٽ (*glyphs, retained_coverage_glyphs);

    hb_سيٽ_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    أرجع
    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<بنانشان, ثابت قاعدو_سيٽ &> p)
	      { أرجع class_def.intersects_class (glyphs, p.پهريون) &&
		       coverage_glyph_classes.has (p.پهريون) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (*c->glyphs, *cur_سرگرم_glyphs);

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

    + hb_enumerate (قاعدو_سيٽ_2)
    | hb_filter ([&] (بنانشان _)
    { أرجع class_def.intersects_class (&c->والد_سرگرم_glyphs (), _); },
		 hb_پهريون)
    | hb_apply ([&] (ثابت hb_pair_t<بنانشان, ثابت قسم_نالو Types::سانچو وٿي_منزل<قاعدو_سيٽ>&> _)
                {
                  ثابت قاعدو_سيٽ& قاعدو_سيٽ_3 = هذا+_.second;
                  قاعدو_سيٽ_3.depend (c, _.پهريون, lookup_context);
                })
    ;

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (c->اڳيون_والد_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    بنية Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (قاعدو_سيٽ_2)
    | hb_filter ([&] (بنانشان _)
    { أرجع class_def.intersects_class (&c->والد_سرگرم_glyphs (), _); },
		 hb_پهريون)
    | hb_apply ([&] (ثابت hb_pair_t<بنانشان, ثابت قسم_نالو Types::سانچو وٿي_منزل<قاعدو_سيٽ>&> _)
                {
                  ثابت قاعدو_سيٽ& قاعدو_سيٽ_3 = هذا+_.second;
                  قاعدو_سيٽ_3.گهيرو (c, _.پهريون, lookup_context);
                })
    ;

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_class, خالي_اشارو},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_enumerate
    | hb_filter ([&] (ثابت hb_pair_t<بنانشان, ثابت قاعدو_سيٽ &> p)
    { أرجع class_def.intersects_class (c->glyphs, p.پهريون); })
    | hb_map (hb_ٻيو)
    | hb_apply ([&] (ثابت قاعدو_سيٽ & _)
    { _.گهيرو_lookups (c, lookup_context); });
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->داخلا_2);

    ثابت ClassDef &class_def = هذا+classDef;
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت قاعدو_سيٽ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([=, &class_def] (ثابت قاعدو_سيٽ &_) { _.collect_ٻيو_glyphs (digest, &class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    بنانشان صحيح اشاريو_2 = class_def.get_class (c->glyphs[0]);
    ثابت قاعدو_سيٽ &قاعدو_سيٽ_3 = هذا+قاعدو_سيٽ_2[اشاريو_2];
    بنية ContextApplyLookupContext lookup_context = {
      {ميلاپ_class},
      &class_def
    };
    أرجع قاعدو_سيٽ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنانشان cache_لاڳت () ثابت
  {
    أرجع (هذا+classDef).لاڳت ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_ٻٽو_cache_t coverage;
    hb_ot_layout_ruleset_digest_t قاعدو_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بنانشان ڳڻپ = قاعدو_سيٽ_2.len;
    بنانشان سائيز = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    ڳڻپ * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (سائيز);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
	(هذا+قاعدو_سيٽ_2[i]).collect_پهريون_داخلا_classes (&cache->قاعدو_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, سچ, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage_ٻٽو (c->بفر->cur().codepoint, cache ? &cache->coverage : خالي_اشارو);
#وإلا
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
#جيڪڏهن_پورو
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &class_def = هذا+classDef;

    بنية ContextApplyLookupContext lookup_context = {
      {cached ? ميلاپ_class_cached : ميلاپ_class},
      &class_def,
      cached ? get_class_cached_قدر : get_class_قدر
    };

    اشاريو_2 = cached ? get_class_cached (class_def, c->بفر->cur()) : class_def.get_class (c->بفر->cur().codepoint);
    ثابت قاعدو_سيٽ &قاعدو_سيٽ_3 = هذا+قاعدو_سيٽ_2[اشاريو_2];
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && اشاريو_2 < قاعدو_سيٽ_2.len ? &cache->قاعدو_sets[اشاريو_2] : خالي_اشارو;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = خالي_اشارو;
#جيڪڏهن_پورو
    return_trace (قاعدو_سيٽ_3.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;
    إذا (unlikely (!ٻاهريون->coverage.serialize_subset (c, coverage, هذا)))
      return_trace (خطأ);

    hb_map_t klass_map;
    ٻاهريون->classDef.serialize_subset (c, classDef, هذا, &klass_map);

    ثابت hb_سيٽ_t* glyphset = c->plan->glyphset_gsub ();
    hb_سيٽ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سيٽ (*glyphset, retained_coverage_glyphs);

    hb_سيٽ_t coverage_glyph_classes;
    (هذا+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ثابت hb_map_t *lookup_map = c->جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    منطقي ret = سچ;
    صحيح non_zero_اشاريو = -1, اشاريو_2 = 0;
    خودڪار snapshot = c->serializer->snapshot();
    لكل (ثابت خودڪار& _ : + hb_enumerate (قاعدو_سيٽ_2)
			 | hb_filter (klass_map, hb_پهريون))
    {
      خودڪار *o = ٻاهريون->قاعدو_سيٽ_2.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      إذا (coverage_glyph_classes.has (_.پهريون) &&
	  o->serialize_subset (c, _.second, هذا, lookup_map, &klass_map)) {
	non_zero_اشاريو = اشاريو_2;
        snapshot = c->serializer->snapshot();
      }

      اشاريو_2++;
    }

    إذا (!ret || non_zero_اشاريو == -1) return_trace (خطأ);

    //prune empty trailing ruleSets
    --اشاريو_2;
    طالما (اشاريو_2 > non_zero_اشاريو)
    {
      ٻاهريون->قاعدو_سيٽ_2.ڪڍو ();
      اشاريو_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (منطقي (ٻاهريون->قاعدو_سيٽ_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && classDef.sanitize (c, هذا) && قاعدو_سيٽ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_نالو Types::سانچو LOffset_منزل<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::سانچو وٿي_منزل<قاعدو_سيٽ>>
		قاعدو_سيٽ_2;		/* Array of RuleSet tables
					 * ordered by class */
  عام:
  DEFINE_سائيز_ARRAY (2 + 2 * Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز, قاعدو_سيٽ_2);
};


سانچو <قسم_نالو Types>
بنية ContextFormat3_6
{
  استعمال قاعدو_سيٽ = OT::قاعدو_سيٽ<SmallTypes>;

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (glyphs))
      أرجع خطأ;

    بنية Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_coverage, خالي_اشارو},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    أرجع context_intersects (glyphs,
			       glyph_ڳڻپ_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (*c->glyphs, *cur_سرگرم_glyphs);

    ثابت Lookup_رڪارڊ *lookup_رڪارڊ = &StructAfter<Lookup_رڪارڊ> (coverageZ.as_array (glyph_ڳڻپ_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا,
      خالي_اشارو,  /* intersects_cache */
      خالي_اشارو   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_ڳڻپ_2, (ثابت HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_ڳڻپ_2, lookup_رڪارڊ,
			   هذا,
			   coverageZ[0], lookup_context);

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (c->اڳيون_والد_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    ثابت Lookup_رڪارڊ *lookup_رڪارڊ = &StructAfter<Lookup_رڪارڊ> (coverageZ.as_array (glyph_ڳڻپ_2));
    بنية Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    context_گهيرو_lookup (c,
			    glyph_ڳڻپ_2, coverageZ.arrayZ + 1,
			    lookup_ڳڻپ_2, lookup_رڪارڊ,
			    0, lookup_context);

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;
    ثابت Lookup_رڪارڊ *lookup_رڪارڊ = &StructAfter<Lookup_رڪارڊ> (coverageZ.as_array (glyph_ڳڻپ_2));
    recurse_lookups (c, lookup_ڳڻپ_2, lookup_رڪارڊ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverageZ[0]).collect_coverage (c->داخلا_2);

    ثابت Lookup_رڪارڊ *lookup_رڪارڊ = &StructAfter<Lookup_رڪارڊ> (coverageZ.as_array (glyph_ڳڻپ_2));
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      هذا
    };

    context_collect_glyphs_lookup (c,
				   glyph_ڳڻپ_2, coverageZ.arrayZ + 1,
				   lookup_ڳڻپ_2, lookup_رڪارڊ,
				   lookup_context);
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  {
    إذا (glyph_ڳڻپ_2 <= 1)
      *digest = hb_سيٽ_digest_t::ڀريل ();
    وإلا
      (هذا+coverageZ[1]).collect_coverage (digest);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Lookup_رڪارڊ *lookup_رڪارڊ = &StructAfter<Lookup_رڪارڊ> (coverageZ.as_array (glyph_ڳڻپ_2));
    بنية ContextApplyLookupContext lookup_context = {
      {ميلاپ_coverage},
      هذا
    };
    أرجع context_would_apply_lookup (c,
				       glyph_ڳڻپ_2, coverageZ.arrayZ + 1,
				       lookup_ڳڻپ_2, lookup_رڪارڊ,
				       lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverageZ[0]; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشان صحيح اشاريو_2 = (هذا+coverageZ[0]).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    ثابت Lookup_رڪارڊ *lookup_رڪارڊ = &StructAfter<Lookup_رڪارڊ> (coverageZ.as_array (glyph_ڳڻپ_2));
    بنية ContextApplyLookupContext lookup_context = {
      {ميلاپ_coverage},
      هذا
    };
    return_trace (context_apply_lookup (c,
					glyph_ڳڻپ_2,
					coverageZ.arrayZ + 1,
					lookup_ڳڻپ_2, lookup_رڪارڊ,
					lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->format = format;
    ٻاهريون->glyph_ڳڻپ_2 = glyph_ڳڻپ_2;

    خودڪار coverages = coverageZ.as_array (glyph_ڳڻپ_2);

    لكل (ثابت خودڪار& وٿي : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      خودڪار *o = c->serializer->allocate_سائيز<قسم_نالو Types::سانچو وٿي_منزل<Coverage>> (Types::وٿي_2::static_سائيز);
      إذا (unlikely (!o)) return_trace (خطأ);
      إذا (!o->serialize_subset (c, وٿي, هذا)) return_trace (خطأ);
    }

    ثابت خودڪار& lookup_رڪارڊ = StructAfter<Unsized_Array_جو<Lookup_رڪارڊ>> (coverageZ.as_array (glyph_ڳڻپ_2));
    ثابت hb_map_t *lookup_map = c->جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    بنانشان ڳڻپ = serialize_lookuprecord_array (c->serializer, lookup_رڪارڊ.as_array (lookup_ڳڻپ_2), lookup_map);
    return_trace (c->serializer->check_سونپيو (ٻاهريون->lookup_ڳڻپ_2, ڳڻپ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    بنانشان صحيح ڳڻپ = glyph_ڳڻپ_2;
    إذا (unlikely (!ڳڻپ)) return_trace (خطأ); /* We want to access coverageZ[0] freely. */
    إذا (unlikely (!c->check_array (coverageZ.arrayZ, ڳڻپ))) return_trace (خطأ);
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (unlikely (!coverageZ[i].sanitize (c, هذا))) return_trace (خطأ);
    ثابت Lookup_رڪارڊ *lookup_رڪارڊ = &StructAfter<Lookup_رڪارڊ> (coverageZ.as_array (glyph_ڳڻپ_2));
    return_trace (likely (c->check_array (lookup_رڪارڊ, lookup_ڳڻپ_2)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_ڳڻپ_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_ڳڻپ_2;		/* Number of LookupRecords */
  Unsized_Array_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_سائيز_ARRAY (6, coverageZ);
};

بنية Context
{
  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    حالة 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#جيڪڏهن_پورو
  } u;
};


/* Chaining Contextual lookups */

بنية Chain_Context_گهيرو_Lookup_Context
{
  Context_گهيرو_Funcs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_معلومات[3];
  فراغ *intersects_cache[3];
  فراغ *intersected_glyphs_cache;
};

تعريفنوع Chain_Context_گهيرو_Lookup_Context ChainContextDependLookupContext;

بنية ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_معلومات[3];
};

بنية ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ثابت فراغ *ميلاپ_معلومات_2[3];
  get_قدر_func_t get_داخلا_قدر;
};

ساكن مضمن منطقي chain_context_intersects (ثابت hb_سيٽ_t *glyphs,
					     بنانشان صحيح backtrack_ڳڻپ,
					     glyph_معلومات_t backtrack,
					     بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
					     glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
					     بنانشان صحيح lookahead_ڳڻپ,
					     glyph_معلومات_t lookahead,
					     Chain_Context_گهيرو_Lookup_Context &lookup_context)
{
  منطقي bt_ok = array_is_subset_جو (glyphs,
			     backtrack_ڳڻپ, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_معلومات[0],
			     lookup_context.intersects_cache[0]);
  منطقي اندريون_ok = array_is_subset_جو (glyphs,
			     داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0, داخلا_2,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_معلومات[1],
			     lookup_context.intersects_cache[1]);
  منطقي la_ok = array_is_subset_جو (glyphs,
			     lookahead_ڳڻپ, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_معلومات[2],
			     lookup_context.intersects_cache[2]);

  أرجع bt_ok && اندريون_ok && la_ok;
}

ساكن مضمن فراغ chain_context_گهيرو_lookup (hb_گهيرو_context_t *c,
						 بنانشان صحيح backtrack_ڳڻپ,
						 glyph_معلومات_t backtrack,
						 بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
						 glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
						 بنانشان صحيح lookahead_ڳڻپ,
						 glyph_معلومات_t lookahead,
						 بنانشان صحيح lookup_ڳڻپ_2,
						 ثابت Lookup_رڪارڊ lookup_رڪارڊ[],
						 بنانشان value,
						 Chain_Context_گهيرو_Lookup_Context &lookup_context)
{
  إذا (chain_context_intersects (c->glyphs,
				backtrack_ڳڻپ, backtrack,
				داخلا_ڳڻپ_2, داخلا_2,
				lookahead_ڳڻپ, lookahead,
				lookup_context))
    context_گهيرو_recurse_lookups (c,
		     داخلا_ڳڻپ_2, داخلا_2,
		     lookup_ڳڻپ_2, lookup_رڪارڊ,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_معلومات[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ chain_context_depend_lookup (hb_depend_context_t *c,
						بنانشان صحيح backtrack_ڳڻپ,
						glyph_معلومات_t backtrack,
						بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
						glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
						بنانشان صحيح lookahead_ڳڻپ,
						glyph_معلومات_t lookahead,
						بنانشان صحيح lookup_ڳڻپ_2,
						ثابت Lookup_رڪارڊ lookup_رڪارڊ[],
						ثابت فراغ *قاعدو_2,
						بنانشان value,
						ChainContextDependLookupContext &lookup_context)
{
  إذا (!chain_context_intersects (c->glyphs,
				 backtrack_ڳڻپ, backtrack,
				 داخلا_ڳڻپ_2, داخلا_2,
				 lookahead_ڳڻپ, lookahead,
				 lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, داخلا_ڳڻپ_2, lookup_ڳڻپ_2, lookup_رڪارڊ, قاعدو_2, value,
    lookup_context.context_format, lookup_context.intersects_معلومات[1],
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_معلومات->successful))
    أرجع;
  إذا (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    داخلا_ڳڻپ_2, داخلا_2,
				    lookup_ڳڻپ_2, lookup_رڪارڊ,
				    قاعدو_2,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_معلومات[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    خالي_اشارو,
				    &scratch->cached_context_sets,
				    scratch);
    أرجع;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_سيٽ_t &position_scratch = scratch->position_scratch;
  hb_ويڪٽر_t<ثابت hb_سيٽ_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_ويڪٽر_t<ثابت hb_سيٽ_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  لكل (بنانشان i = 0; i < backtrack_ڳڻپ; i++)
  {
    ثابت hb_سيٽ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_معلومات[0],
      backtrack[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!backtrack_sets.ڌڪو_or_fail (pos_glyphs)))
      {
        c->depend_معلومات->fail ();
        أرجع;
      }
    }
  }

  /* Extract lookahead glyphs */
  لكل (بنانشان i = 0; i < lookahead_ڳڻپ; i++)
  {
    ثابت hb_سيٽ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_معلومات[2],
      lookahead[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!lookahead_sets.ڌڪو_or_fail (pos_glyphs)))
      {
        c->depend_معلومات->fail ();
        أرجع;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_سيٽ_t &preliminary_context = scratch->preliminary_context;
  لكل (ثابت hb_سيٽ_t *پٺ_سيٽ : backtrack_sets)
  {
    إذا (پٺ_سيٽ->get_population () == 1) {
      preliminary_context.جوڙيو (پٺ_سيٽ->get_min ());  // Direct glyph
    } وإلا إذا (پٺ_سيٽ->get_population () > 1) {
      hb_codepoint_t سيٽ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_معلومات->ڳوليو_or_create_context_سيٽ (*پٺ_سيٽ)
        : c->ڳوليو_or_create_cached_context_سيٽ (پٺ_سيٽ);
      إذا (unlikely (سيٽ_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.جوڙيو (HB_DEPEND_CONTEXT_سيٽ_FLAG | سيٽ_idx);  // Encoded set reference
    }
  }
  لكل (ثابت hb_سيٽ_t *look_سيٽ : lookahead_sets)
  {
    إذا (look_سيٽ->get_population () == 1) {
      preliminary_context.جوڙيو (look_سيٽ->get_min ());  // Direct glyph
    } وإلا إذا (look_سيٽ->get_population () > 1) {
      hb_codepoint_t سيٽ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_معلومات->ڳوليو_or_create_context_سيٽ (*look_سيٽ)
        : c->ڳوليو_or_create_cached_context_سيٽ (look_سيٽ);
      إذا (unlikely (سيٽ_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.جوڙيو (HB_DEPEND_CONTEXT_سيٽ_FLAG | سيٽ_idx);  // Encoded set reference
    }
  }
  إذا (unlikely (preliminary_context.اندريون_error ()))
  {
    c->depend_معلومات->fail ();
    أرجع;
  }

  /* Build glyph sets for ALL input positions */
  hb_ويڪٽر_t<ثابت hb_سيٽ_t *> &داخلا_position_glyphs = scratch->داخلا_position_glyphs;

  إذا (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ثابت hb_سيٽ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_معلومات[1], value,
      سچ, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !داخلا_position_glyphs.ڌڪو_or_fail (pos0_glyphs)))
    {
      c->depend_معلومات->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (بنانشان i = 0; i + 1 < داخلا_ڳڻپ_2; i++)
    {
      ثابت hb_سيٽ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_معلومات[1],
	داخلا_2[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !داخلا_position_glyphs.ڌڪو_or_fail (pos_glyphs)))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  داخلا_ڳڻپ_2, داخلا_2,
				  lookup_ڳڻپ_2, lookup_رڪارڊ,
				  قاعدو_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_معلومات[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? خالي_اشارو : &داخلا_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : خالي_اشارو,
				  scratch);
}

ساكن مضمن فراغ chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							بنانشان صحيح backtrack_ڳڻپ,
							glyph_معلومات_t backtrack,
							بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
							glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
							بنانشان صحيح lookahead_ڳڻپ,
							glyph_معلومات_t lookahead,
							بنانشان صحيح lookup_ڳڻپ_2,
							ثابت Lookup_رڪارڊ lookup_رڪارڊ[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_ڳڻپ, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_معلومات[0]);
  collect_array (c, c->داخلا_2,
		 داخلا_ڳڻپ_2 ? داخلا_ڳڻپ_2 - 1 : 0, داخلا_2,
		 lookup_context.funcs.collect, lookup_context.collect_معلومات[1]);
  collect_array (c, c->after,
		 lookahead_ڳڻپ, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_معلومات[2]);
  recurse_lookups (c,
		   lookup_ڳڻپ_2, lookup_رڪارڊ);
}

ساكن مضمن منطقي chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     بنانشان صحيح backtrack_ڳڻپ,
						     glyph_معلومات_t backtrack HB_اڻ_استعمال_ٿيل,
						     بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
						     glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
						     بنانشان صحيح lookahead_ڳڻپ,
						     glyph_معلومات_t lookahead HB_اڻ_استعمال_ٿيل,
						     بنانشان صحيح lookup_ڳڻپ_2 HB_اڻ_استعمال_ٿيل,
						     ثابت Lookup_رڪارڊ lookup_رڪارڊ[] HB_اڻ_استعمال_ٿيل,
						     ثابت ChainContextApplyLookupContext &lookup_context)
{
  أرجع (c->zero_context ? !backtrack_ڳڻپ && !lookahead_ڳڻپ : سچ)
      && would_ميلاپ_داخلا (c,
			    داخلا_ڳڻپ_2, داخلا_2,
			    lookup_context.funcs.ميلاپ[1], lookup_context.ميلاپ_معلومات_2[1]);
}

ساكن مضمن منطقي chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       بنانشان صحيح backtrack_ڳڻپ,
					       glyph_معلومات_t backtrack,
					       بنانشان صحيح داخلا_ڳڻپ_2, /* Including the first glyph (not matched) */
					       glyph_معلومات_t داخلا_2, /* Array of input values--start with second glyph */
					       بنانشان صحيح lookahead_ڳڻپ,
					       glyph_معلومات_t lookahead,
					       بنانشان صحيح lookup_ڳڻپ_2,
					       ثابت Lookup_رڪارڊ lookup_رڪارڊ[],
					       ثابت ChainContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (داخلا_ڳڻپ_2 > HB_MAX_CONTEXT_ڊيگهائي)) أرجع خطأ;

  بنانشان شروعات_اشاريو = c->بفر->ٻاهريون_len;
  بنانشان پڇاڙي_اشاريو = c->بفر->idx;
  بنانشان ميلاپ_پڇاڙي = 0;
  منطقي ret = سچ;
  إذا (!(ميلاپ_داخلا (c,
		     داخلا_ڳڻپ_2, داخلا_2,
		     lookup_context.funcs.ميلاپ[1], lookup_context.ميلاپ_معلومات_2[1],
		     &ميلاپ_پڇاڙي) && (پڇاڙي_اشاريو = ميلاپ_پڇاڙي)
       && ميلاپ_lookahead (c,
			   lookahead_ڳڻپ, lookahead,
			   lookup_context.funcs.ميلاپ[2], lookup_context.ميلاپ_معلومات_2[2],
			   ميلاپ_پڇاڙي, &پڇاڙي_اشاريو)))
  {
    c->بفر->unsafe_منزل_concat (c->بفر->idx, پڇاڙي_اشاريو);
    أرجع خطأ;
  }

  إذا (!ميلاپ_backtrack (c,
			backtrack_ڳڻپ, backtrack,
			lookup_context.funcs.ميلاپ[0], lookup_context.ميلاپ_معلومات_2[0],
			&شروعات_اشاريو))
  {
    c->بفر->unsafe_منزل_concat_from_outbuffer (شروعات_اشاريو, پڇاڙي_اشاريو);
    أرجع خطأ;
  }

  c->بفر->unsafe_منزل_break_from_outbuffer (شروعات_اشاريو, پڇاڙي_اشاريو);
  apply_lookup (c,
		داخلا_ڳڻپ_2,
		lookup_ڳڻپ_2, lookup_رڪارڊ,
		ميلاپ_پڇاڙي);

  أرجع ret;
}

سانچو <قسم_نالو Types>
بنية Chain_قاعدو
{
  سانچو <قسم_نالو سيٽ_Types, قسم_نالو Chain_قاعدو_Type>
  دوست بنية Chain_قاعدو_سيٽ_جو;

  منطقي intersects (ثابت hb_سيٽ_t *glyphs, Chain_Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     داخلا_2.lenP1, داخلا_2.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, بنانشان value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 داخلا_2.lenP1, داخلا_2.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 value,
				 lookup_context);
  }

  فراغ گهيرو (hb_گهيرو_context_t *c, بنانشان value,
		Chain_Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندي_exceeded ())) أرجع;

    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_گهيرو_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  داخلا_2.lenP1, داخلا_2.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c,
                        Chain_Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندي_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 داخلا_2.lenP1, داخلا_2.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest,
			      ثابت ClassDef *داخلا_class_def = خالي_اشارو,
			      ثابت ClassDef *lookahead_class_def = خالي_اشارو) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);

    بنانشان second;
    ثابت ClassDef *class_def;
    إذا (داخلا_2.lenP1 > 1)
    {
      second = داخلا_2.arrayZ[0];
      class_def = داخلا_class_def;
    }
    وإلا إذا (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    وإلا
    {
      *digest = hb_سيٽ_digest_t::ڀريل ();
      أرجع;
    }

    إذا (!class_def)
      digest->جوڙيو (second);
    وإلا إذا (!second)
      *digest = hb_سيٽ_digest_t::ڀريل ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     داخلا_2.lenP1, داخلا_2.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      داخلا_2.lenP1, داخلا_2.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ثابت
  {
    c->copy (len);
    لكل (ثابت خودڪار g : it)
    {
      قسم_نالو Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *lookup_map,
		  ثابت hb_map_t *backtrack_map,
		  ثابت hb_map_t *داخلا_map = خالي_اشارو,
		  ثابت hb_map_t *lookahead_map = خالي_اشارو) ثابت
  {
    TRACE_SERIALIZE (هذا);

    ثابت hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    إذا (داخلا_map) mapping = داخلا_map;
    serialize_array (c, داخلا_2.lenP1, + داخلا_2.iter ()
				     | hb_map (mapping));

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    إذا (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_ڳڻپ_2 = c->embed (&(lookup.len));
    إذا (!lookup_ڳڻپ_2) return_trace (خطأ);

    بنانشان ڳڻپ = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_سونپيو (*lookup_ڳڻپ_2, ڳڻپ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_map = خالي_اشارو,
	       ثابت hb_map_t *داخلا_map = خالي_اشارو,
	       ثابت hb_map_t *lookahead_map = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);

    إذا (!backtrack_map)
    {
      ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
      إذا (!hb_all (backtrack, glyphset) ||
	  !hb_all (داخلا_2, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    وإلا
    {
      إذا (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (داخلا_2, داخلا_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, backtrack_map, داخلا_map, lookahead_map);
    }

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Hyper-optimized sanitized because this is really hot. */
    إذا (unlikely (!backtrack.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    إذا (unlikely (!داخلا_2.lenP1.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    إذا (unlikely (!lookahead.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  Array16_جو<قسم_نالو Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  Headless_Array16_جو<قسم_نالو Types::HBUINT>
		داخلا_X;			/* Array of input values (start with
					 * second glyph) */
  Array16_جو<قسم_نالو Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16_جو<Lookup_رڪارڊ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_سائيز_MIN (8);
};

سانچو <قسم_نالو Types, قسم_نالو Chain_قاعدو_Type>
بنية Chain_قاعدو_سيٽ_جو
{
  منطقي intersects (ثابت hb_سيٽ_t *glyphs, Chain_Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_map ([&] (ثابت Chain_قاعدو_Type &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_پهريون_داخلا_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (بنانشان i = 0; i < قاعدو_2.len; i++)
    {
      ثابت Chain_قاعدو_Type &r = هذا+قاعدو_2.arrayZ[i];
      ثابت خودڪار &داخلا_2 = StructAfter<decltype (r.داخلا_X)> (r.backtrack);
      إذا (داخلا_2.lenP1 <= 1)
      {
	digest->سيٽ_ڀريل ();
	أرجع;
      }
      digest->جوڙيو (داخلا_2.arrayZ[0]);
    }
  }
  فراغ depend (hb_depend_context_t *c, بنانشان value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدو_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }
  فراغ گهيرو (hb_گهيرو_context_t *c, بنانشان value, Chain_Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندي_exceeded ())) أرجع;

    أرجع
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدو_Type &_) { _.گهيرو (c, value, lookup_context); })
    ;
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c,
                        Chain_Context_گهيرو_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندي_exceeded ())) أرجع;

    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدو_Type &_) { _.گهيرو_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدو_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest,
			      ثابت ClassDef *داخلا_class_def = خالي_اشارو,
			      ثابت ClassDef *lookahead_class_def = خالي_اشارو) ثابت
  {
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([=] (ثابت Chain_قاعدو_Type &_)
		{ _.collect_ٻيو_glyphs (digest, داخلا_class_def, lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدو_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_map ([&] (ثابت Chain_قاعدو_Type &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = خالي_اشارو) ثابت
  {
    TRACE_APPLY (هذا);

    بنانشان انگ_قاعدا = قاعدو_2.len;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_RULESETS_FAST_رستو
    إذا (HB_OPTIMIZE_سائيز_VAL || انگ_قاعدا <= 4)
#جيڪڏهن_پورو
    {
    slow:
      return_trace (
      + hb_iter (قاعدو_2)
      | hb_map (hb_جوڙيو (هذا))
      | hb_map ([&] (ثابت Chain_قاعدو_Type &_) { أرجع _.apply (c, lookup_context); })
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
    خودڪار &skippy_iter = c->iter_context;
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سيٽ_ميلاپ_func (ميلاپ_always, خالي_اشارو);
    skippy_iter.سيٽ_glyph_معلومات ((HBUINT16 *) خالي_اشارو);
    بنانشان unsafe_منزل = (بنانشان) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *پهريون = خالي_اشارو, *second = خالي_اشارو;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      پهريون = &c->بفر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_داخلا_قدر &&
	  !digest->may_have (lookup_context.get_داخلا_قدر (*پهريون, lookup_context.ميلاپ_معلومات_2[1])))
	{
	  c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_منزل_concat = خطأ;
      منطقي ret =
      + hb_iter (قاعدو_2)
      | hb_map (hb_جوڙيو (هذا))
      | hb_filter ([&] (ثابت Chain_قاعدو_Type &_)
		   {
		     ثابت خودڪار &داخلا_2 = StructAfter<decltype (_.داخلا_X)> (_.backtrack);
		     ثابت خودڪار &lookahead = StructAfter<decltype (_.lookaheadX)> (داخلا_2);
		     إذا (داخلا_2.lenP1 <= 1 && lookahead.len == 0) أرجع سچ;
		     unsafe_منزل_concat = سچ;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت Chain_قاعدو_Type &_) { أرجع _.apply (c, lookup_context); })
      | hb_any
      ;
      إذا (unsafe_منزل_concat)
	c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      second = &c->بفر->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    خودڪار ميلاپ_داخلا = lookup_context.funcs.ميلاپ[1];
    خودڪار ميلاپ_lookahead = lookup_context.funcs.ميلاپ[2];
    خودڪار *داخلا_معلومات_2 = lookup_context.ميلاپ_معلومات_2[1];
    خودڪار *lookahead_معلومات = lookup_context.ميلاپ_معلومات_2[2];
    لكل (بنانشان صحيح i = 0; i < انگ_قاعدا; i++)
    {
      ثابت خودڪار &r = هذا+قاعدو_2.arrayZ[i];

      ثابت خودڪار &داخلا_2 = StructAfter<decltype (r.داخلا_X)> (r.backtrack);
      ثابت خودڪار &lookahead = StructAfter<decltype (r.lookaheadX)> (داخلا_2);

      بنانشان lenP1 = داخلا_2.lenP1;
      إذا (lenP1 > 1 ?
	   (!ميلاپ_داخلا ||
	    ميلاپ_داخلا (*پهريون, داخلا_2.arrayZ[0], داخلا_معلومات_2))
	  :
	   (!lookahead.len || !ميلاپ_lookahead ||
	    ميلاپ_lookahead (*پهريون, lookahead.arrayZ[0], lookahead_معلومات)))
      {
	lenP1 = hb_max (lenP1, 1u);
        إذا (!second ||
	    (lenP1 > 2 ?
	     (!ميلاپ_داخلا ||
	      ميلاپ_داخلا (*second, داخلا_2.arrayZ[1], داخلا_معلومات_2))
	     :
	     (lookahead.len <= 2 - lenP1 || !ميلاپ_lookahead ||
	      ميلاپ_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_معلومات))))
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_منزل != (بنانشان) -1)
	      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
	    return_trace (سچ);
	  }
	}
	وإلا
	  unsafe_منزل = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_منزل == (بنانشان) -1)
	  unsafe_منزل = unsafe_to1;

	إذا (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  لكل (; i + 1 < انگ_قاعدا; i++)
	  {
	    ثابت خودڪار &r2 = هذا+قاعدو_2.arrayZ[i + 1];
	    ثابت خودڪار &input2 = StructAfter<decltype (r2.داخلا_X)> (r2.backtrack);
	    إذا (input2.lenP1 <= 1 || input2.arrayZ[0] != داخلا_2.arrayZ[0])
	      اكسر;
	  }
	}
      }
    }
    إذا (likely (unsafe_منزل != (بنانشان) -1))
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_klass_map = خالي_اشارو,
	       ثابت hb_map_t *داخلا_klass_map = خالي_اشارو,
	       ثابت hb_map_t *lookahead_klass_map = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);

    خودڪار snap = c->serializer->snapshot ();
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    لكل (ثابت خودڪار& _ : قاعدو_2)
    {
      إذا (!_) استمر;
      خودڪار o_snap = c->serializer->snapshot ();
      خودڪار *o = ٻاهريون->قاعدو_2.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا,
				lookup_map,
				backtrack_klass_map,
				داخلا_klass_map,
				lookahead_klass_map))
      {
	ٻاهريون->قاعدو_2.ڪڍو ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (ٻاهريون->قاعدو_2);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (قاعدو_2.sanitize (c, هذا));
  }

  محمي:
  Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Chain_قاعدو_Type>>
		قاعدو_2;			/* Array of ChainRule tables
					 * ordered by preference */
  عام:
  DEFINE_سائيز_ARRAY (2, قاعدو_2);
};

سانچو <قسم_نالو Types>
بنية Chain_قاعدو_سيٽ : Chain_قاعدو_سيٽ_جو<Types, Chain_قاعدو<Types>> {};

static_assert (حجم (Chain_قاعدو_سيٽ<MediumTypes>) ==
	       HBUINT16::static_سائيز + Offset24::static_سائيز, "");

سانچو <قسم_نالو Types>
بنية Chain_Class_قاعدو_سيٽ : Chain_قاعدو_سيٽ_جو<Types, Chain_قاعدو<SmallTypes>> {};

سانچو <قسم_نالو Types>
بنية ChainContextFormat1_4
{
  استعمال Chain_قاعدو_سيٽ = OT::Chain_قاعدو_سيٽ<Types>;

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    بنية Chain_Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالي_اشارو, خالي_اشارو, خالي_اشارو}
    };

    أرجع
    + hb_zip (هذا+coverage, قاعدو_سيٽ_2)
    | hb_filter (*glyphs, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_map ([&] (ثابت Chain_قاعدو_سيٽ &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (*c->glyphs, *cur_سرگرم_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالي_اشارو, خالي_اشارو, خالي_اشارو},
      {خالي_اشارو, خالي_اشارو, خالي_اشارو},  /* intersects_cache */
      خالي_اشارو                        /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((بنانشان) قاعدو_سيٽ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->والد_سرگرم_glyphs ().has (_);
    }, hb_پهريون)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بنانشان> _) { أرجع hb_pair_t<بنانشان, ثابت Chain_قاعدو_سيٽ&> (_.پهريون, هذا+قاعدو_سيٽ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بنانشان, ثابت Chain_قاعدو_سيٽ&>& _) { _.second.depend (c, _.پهريون, lookup_context); })
    ;

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (c->اڳيون_والد_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    بنية Chain_Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالي_اشارو, خالي_اشارو, خالي_اشارو}
    };

    + hb_zip (هذا+coverage, hb_range ((بنانشان) قاعدو_سيٽ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->اڳيون_والد_سرگرم_glyphs ().has (_);
    }, hb_پهريون)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بنانشان> _) { أرجع hb_pair_t<بنانشان, ثابت Chain_قاعدو_سيٽ&> (_.پهريون, هذا+قاعدو_سيٽ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بنانشان, ثابت Chain_قاعدو_سيٽ&>& _) { _.second.گهيرو (c, _.پهريون, lookup_context); })
    ;

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت
  {
    بنية Chain_Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_glyph, خالي_اشارو},
      ContextFormat::SimpleContext,
      {خالي_اشارو, خالي_اشارو, خالي_اشارو}
    };

    + hb_zip (هذا+coverage, قاعدو_سيٽ_2)
    | hb_filter (*c->glyphs, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدو_سيٽ &_) { _.گهيرو_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->داخلا_2);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {خالي_اشارو, خالي_اشارو, خالي_اشارو}
    };

    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدو_سيٽ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  {
    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([digest] (ثابت Chain_قاعدو_سيٽ &_) { _.collect_ٻيو_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Chain_قاعدو_سيٽ &قاعدو_سيٽ_3 = هذا+قاعدو_سيٽ_2[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{ميلاپ_glyph, ميلاپ_glyph, ميلاپ_glyph}},
      {خالي_اشارو, خالي_اشارو, خالي_اشارو}
    };
    أرجع قاعدو_سيٽ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    ثابت Chain_قاعدو_سيٽ &قاعدو_سيٽ_3 = هذا+قاعدو_سيٽ_2[اشاريو_2];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{ميلاپ_glyph, ميلاپ_glyph, ميلاپ_glyph}},
      {خالي_اشارو, خالي_اشارو, خالي_اشارو}
    };
    return_trace (قاعدو_سيٽ_3.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, قاعدو_سيٽ_2)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (subset_وٿي_array (c, ٻاهريون->قاعدو_سيٽ_2, هذا, lookup_map), hb_ٻيو)
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ٻاهريون->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && قاعدو_سيٽ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::سانچو وٿي_منزل<Chain_قاعدو_سيٽ>>
		قاعدو_سيٽ_2;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز, قاعدو_سيٽ_2);
};

سانچو <قسم_نالو Types>
بنية ChainContextFormat2_5
{
  استعمال Chain_قاعدو_سيٽ = OT::Chain_Class_قاعدو_سيٽ<Types>;

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلا_class_def = هذا+داخلا_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية Chain_Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_class, خالي_اشارو},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلا_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_سيٽ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سيٽ (*glyphs, retained_coverage_glyphs);

    hb_سيٽ_t coverage_glyph_classes;
    داخلا_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    أرجع
    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<بنانشان, ثابت Chain_قاعدو_سيٽ &> p)
	      { أرجع داخلا_class_def.intersects_class (glyphs, p.پهريون) &&
		       coverage_glyph_classes.has (p.پهريون) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (*c->glyphs, *cur_سرگرم_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلا_class_def = هذا+داخلا_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلا_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعدو_سيٽ_2)
    | hb_filter ([&] (بنانشان _)
    { أرجع داخلا_class_def.intersects_class (&c->والد_سرگرم_glyphs (), _); },
		 hb_پهريون)
    | hb_apply ([&] (ثابت hb_pair_t<بنانشان, ثابت قسم_نالو Types::سانچو وٿي_منزل<Chain_قاعدو_سيٽ>&> _)
                {
                  ثابت Chain_قاعدو_سيٽ& chainrule_سيٽ = هذا+_.second;
                  chainrule_سيٽ.depend (c, _.پهريون, lookup_context);
                })
    ;

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سيٽ (c->اڳيون_والد_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلا_class_def = هذا+داخلا_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    بنية Chain_Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلا_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعدو_سيٽ_2)
    | hb_filter ([&] (بنانشان _)
    { أرجع داخلا_class_def.intersects_class (&c->والد_سرگرم_glyphs (), _); },
		 hb_پهريون)
    | hb_apply ([&] (ثابت hb_pair_t<بنانشان, ثابت قسم_نالو Types::سانچو وٿي_منزل<Chain_قاعدو_سيٽ>&> _)
                {
                  ثابت Chain_قاعدو_سيٽ& chainrule_سيٽ = هذا+_.second;
                  chainrule_سيٽ.گهيرو (c, _.پهريون, lookup_context);
                })
    ;

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلا_class_def = هذا+داخلا_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية Chain_Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_class, خالي_اشارو},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلا_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_enumerate
    | hb_filter([&] (بنانشان klass)
    { أرجع داخلا_class_def.intersects_class (c->glyphs, klass); }, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_apply ([&] (ثابت Chain_قاعدو_سيٽ &_)
    { _.گهيرو_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->داخلا_2);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلا_class_def = هذا+داخلا_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &داخلا_class_def,
       &lookahead_class_def}
    };

    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدو_سيٽ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  {
    ثابت ClassDef &داخلا_class_def = هذا+داخلا_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;
    + hb_iter (قاعدو_سيٽ_2)
    | hb_map (hb_جوڙيو (هذا))
    | hb_apply ([digest, &داخلا_class_def, &lookahead_class_def] (ثابت Chain_قاعدو_سيٽ &_)
		{ _.collect_ٻيو_glyphs (digest, &داخلا_class_def, &lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلا_class_def = هذا+داخلا_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بنانشان صحيح اشاريو_2 = داخلا_class_def.get_class (c->glyphs[0]);
    ثابت Chain_قاعدو_سيٽ &قاعدو_سيٽ_3 = هذا+قاعدو_سيٽ_2[اشاريو_2];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{ميلاپ_class, ميلاپ_class, ميلاپ_class}},
      {&backtrack_class_def,
       &داخلا_class_def,
       &lookahead_class_def}
    };
    أرجع قاعدو_سيٽ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنانشان cache_لاڳت () ثابت
  {
    أرجع (هذا+داخلا_Class_Def).لاڳت () + (هذا+lookaheadClassDef).لاڳت ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_ٻٽو_cache_t coverage;
    hb_ot_layout_mapping_cache_t داخلا_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t قاعدو_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بنانشان ڳڻپ = قاعدو_سيٽ_2.len;
    بنانشان سائيز = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    ڳڻپ * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (سائيز);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->داخلا_class.clear ();
      cache->lookahead_class.clear ();
      لكل (بنانشان i = 0; i < ڳڻپ; i++)
	(هذا+قاعدو_سيٽ_2[i]).collect_پهريون_داخلا_classes (&cache->قاعدو_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, سچ, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage_ٻٽو (c->بفر->cur().codepoint, cache ? &cache->coverage : خالي_اشارو);
#وإلا
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
#جيڪڏهن_پورو
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلا_class_def = هذا+داخلا_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ميلاپ_class_cache_معلومات_t داخلا_cache_معلومات = {
      &داخلا_class_def,
      cache ? &cache->داخلا_class : خالي_اشارو
    };
    ميلاپ_class_cache_معلومات_t lookahead_cache_معلومات = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : خالي_اشارو
    };
    ثابت فراغ *داخلا_ميلاپ_معلومات = cached
      ? static_cast<ثابت فراغ *> (&داخلا_class_def)
      : static_cast<ثابت فراغ *> (&داخلا_cache_معلومات);
    ثابت فراغ *lookahead_ميلاپ_معلومات = cached
      ? static_cast<ثابت فراغ *> (&lookahead_class_def)
      : static_cast<ثابت فراغ *> (&lookahead_cache_معلومات);
    منطقي backtrack_استعمال_ڪري_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ثابت فراغ *backtrack_ميلاپ_معلومات = backtrack_استعمال_ڪري_lookahead_cache
      ? static_cast<ثابت فراغ *> (&lookahead_cache_معلومات)
      : static_cast<ثابت فراغ *> (&backtrack_class_def);
#جيڪڏهن_پورو

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    بنية ChainContextApplyLookupContext lookup_context = {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? ميلاپ_class_cached1 :
        backtrack_استعمال_ڪري_lookahead_cache ? ميلاپ_class_cache : ميلاپ_class,
        cached ? ميلاپ_class_cached2 : ميلاپ_class_cache,
        cached ? ميلاپ_class_cached1 : ميلاپ_class_cache}},
      {backtrack_ميلاپ_معلومات,
       داخلا_ميلاپ_معلومات,
       lookahead_ميلاپ_معلومات},
      cached ? get_class_cached2_قدر : get_class_cache_قدر
#وإلا
      {{cached && &backtrack_class_def == &lookahead_class_def ? ميلاپ_class_cached1 : ميلاپ_class,
        cached ? ميلاپ_class_cached2 : ميلاپ_class,
        cached ? ميلاپ_class_cached1 : ميلاپ_class}},
      {&backtrack_class_def,
       &داخلا_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_قدر : get_class_قدر
#جيڪڏهن_پورو
    };

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    اشاريو_2 = cached
         ? get_class_cached2 (داخلا_class_def, c->بفر->cur())
         : داخلا_class_def.get_class (c->بفر->cur().codepoint, داخلا_cache_معلومات.cache);
#وإلا
    اشاريو_2 = cached
         ? get_class_cached2 (داخلا_class_def, c->بفر->cur())
          : داخلا_class_def.get_class (c->بفر->cur().codepoint);
#جيڪڏهن_پورو
    ثابت Chain_قاعدو_سيٽ &قاعدو_سيٽ_3 = هذا+قاعدو_سيٽ_2[اشاريو_2];
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && اشاريو_2 < قاعدو_سيٽ_2.len ? &cache->قاعدو_sets[اشاريو_2] : خالي_اشارو;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = خالي_اشارو;
#جيڪڏهن_پورو
    return_trace (قاعدو_سيٽ_3.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;
    ٻاهريون->coverage.serialize_subset (c, coverage, هذا);

    hb_map_t backtrack_klass_map;
    hb_map_t داخلا_klass_map;
    hb_map_t lookahead_klass_map;

    ٻاهريون->backtrackClassDef.serialize_subset (c, backtrackClassDef, هذا, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    ٻاهريون->داخلا_Class_Def.serialize_subset (c, داخلا_Class_Def, هذا, &داخلا_klass_map);
    ٻاهريون->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, هذا, &lookahead_klass_map);

    إذا (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   داخلا_klass_map,
						   lookahead_klass_map)))
      return_trace (خطأ);

    ثابت hb_سيٽ_t* glyphset = c->plan->glyphset_gsub ();
    hb_سيٽ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سيٽ (*glyphset, retained_coverage_glyphs);

    hb_سيٽ_t coverage_glyph_classes;
    (هذا+داخلا_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    صحيح non_zero_اشاريو = -1, اشاريو_2 = 0;
    منطقي ret = سچ;
    ثابت hb_map_t *lookup_map = c->جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    خودڪار آخري_non_zero = c->serializer->snapshot ();
    لكل (ثابت خودڪار& _ : + hb_enumerate (قاعدو_سيٽ_2)
			 | hb_filter (داخلا_klass_map, hb_پهريون))
    {
      خودڪار *o = ٻاهريون->قاعدو_سيٽ_2.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }
      إذا (coverage_glyph_classes.has (_.پهريون) &&
          o->serialize_subset (c, _.second, هذا,
			       lookup_map,
			       &backtrack_klass_map,
			       &داخلا_klass_map,
			       &lookahead_klass_map))
      {
        آخري_non_zero = c->serializer->snapshot ();
	non_zero_اشاريو = اشاريو_2;
      }

      اشاريو_2++;
    }

    إذا (!ret || non_zero_اشاريو == -1) return_trace (خطأ);

    // prune empty trailing ruleSets
    إذا (اشاريو_2 > non_zero_اشاريو) {
      c->serializer->revert (آخري_non_zero);
      ٻاهريون->قاعدو_سيٽ_2.len = non_zero_اشاريو + 1;
    }

    return_trace (منطقي (ٻاهريون->قاعدو_سيٽ_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) &&
		  backtrackClassDef.sanitize (c, هذا) &&
		  داخلا_Class_Def.sanitize (c, هذا) &&
		  lookaheadClassDef.sanitize (c, هذا) &&
		  قاعدو_سيٽ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_نالو Types::سانچو LOffset_منزل<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  قسم_نالو Types::سانچو LOffset_منزل<ClassDef>
		داخلا_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  قسم_نالو Types::سانچو LOffset_منزل<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Chain_قاعدو_سيٽ>>
		قاعدو_سيٽ_2;		/* Array of ChainRuleSet tables
					 * ordered by class */
  عام:
  DEFINE_سائيز_ARRAY (4 + 4 * Types::LOffset::static_سائيز, قاعدو_سيٽ_2);
};

سانچو <قسم_نالو Types>
بنية ChainContextFormat3
{
  استعمال قاعدو_سيٽ = OT::قاعدو_سيٽ<SmallTypes>;

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);

    إذا (!(هذا+داخلا_2[0]).intersects (glyphs))
      أرجع خطأ;

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    بنية Chain_Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_coverage, خالي_اشارو},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     داخلا_2.len, داخلا_2.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);

    إذا (!(هذا+داخلا_2[0]).intersects (c->glyphs))
      أرجع;

    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs))
      أرجع;
    get_coverage ().intersect_سيٽ (*c->glyphs, *cur_سرگرم_glyphs);

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا},
      {خالي_اشارو, خالي_اشارو, خالي_اشارو},  /* intersects_cache */
      خالي_اشارو                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ثابت HBUINT16 *) backtrack.arrayZ,
				 داخلا_2.len, (ثابت HBUINT16 *) داخلا_2.arrayZ + 1,
				 lookahead.len, (ثابت HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 داخلا_2[0], lookup_context);

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);

    إذا (!(هذا+داخلا_2[0]).intersects (c->glyphs))
      أرجع;

    hb_سيٽ_t* cur_سرگرم_glyphs = c->ڌڪو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs))
      أرجع;
    get_coverage ().intersect_سيٽ (c->اڳيون_والد_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية Chain_Context_گهيرو_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    chain_context_گهيرو_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  داخلا_2.len, داخلا_2.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->ڪڍو_cur_پورو_glyphs ();
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;

    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);

    (هذا+داخلا_2[0]).collect_coverage (c->داخلا_2);

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {هذا, هذا, هذا}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 داخلا_2.len, داخلا_2.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    إذا (داخلا_2.len > 1)
      (هذا+داخلا_2[1]).collect_coverage (digest);
    وإلا إذا (lookahead.len)
      (هذا+lookahead[0]).collect_coverage (digest);
    وإلا
      *digest = hb_سيٽ_digest_t::ڀريل ();
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{ميلاپ_coverage, ميلاپ_coverage, ميلاپ_coverage}},
      {هذا, هذا, هذا}
    };
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     داخلا_2.len, داخلا_2.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت
  {
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    أرجع هذا+داخلا_2[0];
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);

    بنانشان صحيح اشاريو_2 = (هذا+داخلا_2[0]).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{ميلاپ_coverage, ميلاپ_coverage, ميلاپ_coverage}},
      {هذا, هذا, هذا}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      داخلا_2.len, داخلا_2.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ثابت فراغ* بنياد) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed<Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>> ();
    إذا (unlikely (!ٻاهريون->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودڪار& وٿي : it) {
      خودڪار *o = ٻاهريون->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, وٿي, بنياد))
        return_trace (خطأ);
    }

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);

    إذا (!serialize_coverage_offsets (c, backtrack.iter (), هذا))
      return_trace (خطأ);

    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    إذا (!serialize_coverage_offsets (c, داخلا_2.iter (), هذا))
      return_trace (خطأ);

    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    إذا (!serialize_coverage_offsets (c, lookahead.iter (), هذا))
      return_trace (خطأ);

    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ثابت hb_map_t *lookup_map = c->جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_ڳڻپ_2 = c->serializer->copy<HBUINT16> (lookup.len);
    إذا (!lookup_ڳڻپ_2) return_trace (خطأ);

    بنانشان ڳڻپ = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_سونپيو (*lookup_ڳڻپ_2, ڳڻپ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!backtrack.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودڪار &داخلا_2 = StructAfter<decltype (داخلا_X)> (backtrack);
    إذا (unlikely (!داخلا_2.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!داخلا_2.len)) return_trace (خطأ); /* To be consistent with Context. */
    ثابت خودڪار &lookahead = StructAfter<decltype (lookaheadX)> (داخلا_2);
    إذا (unlikely (!lookahead.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودڪار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>
		داخلا_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16_جو<قسم_نالو Types::سانچو وٿي_منزل<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16_جو<Lookup_رڪارڊ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_سائيز_MIN (10);
};

بنية ChainContext
{
  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#جيڪڏهن_پورو
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#جيڪڏهن_پورو
  } u;
};


سانچو <قسم_نالو T>
بنية ExtensionFormat1
{
  بنانشان صحيح get_type () ثابت { أرجع extensionLookupType; }

  سانچو <قسم_نالو X>
  ثابت X& get_subtable () ثابت
  { أرجع هذا + reinterpret_cast<ثابت Offset32_منزل<قسم_نالو T::Sub_جدول> &> (extension_وٿي); }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, هذا))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, format);
    return_trace (get_subtable<قسم_نالو T::Sub_جدول> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
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

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->format = format;
    ٻاهريون->extensionLookupType = extensionLookupType;

    ثابت خودڪار& src_وٿي =
        reinterpret_cast<ثابت Offset32_منزل<قسم_نالو T::Sub_جدول> &> (extension_وٿي);
    خودڪار& dest_وٿي =
        reinterpret_cast<Offset32_منزل<قسم_نالو T::Sub_جدول> &> (ٻاهريون->extension_وٿي);

    return_trace (dest_وٿي.serialize_subset (c, src_وٿي, هذا, get_type ()));
  }

  محمي:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extension_وٿي;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  عام:
  DEFINE_سائيز_STATIC (8);
};

سانچو <قسم_نالو T>
بنية Extension
{
  بنانشان صحيح get_type () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_type ();
    افتراضي:أرجع 0;
    }
  }
  سانچو <قسم_نالو X>
  ثابت X& get_subtable () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.سانچو get_subtable<قسم_نالو T::Sub_جدول> ();
    افتراضي:أرجع Null (قسم_نالو T::Sub_جدول);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  سانچو <قسم_نالو ...Ts>
  قسم_نالو hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.subset (c);
    افتراضي: أرجع c->default_return_قدر ();
    }
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_قدر ());
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
  سانچو <قسم_نالو TLookup>
  ساكن hb_ot_layout_lookup_accelerator_t *create (ثابت TLookup &lookup)
  {
    بنانشان ڳڻپ = lookup.get_subtable_ڳڻپ ();

    بنانشان سائيز = حجم (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    ڳڻپ * حجم (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خودڪار *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, سائيز);
    إذا (unlikely (!thiz))
      أرجع خالي_اشارو;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    طالما (ڳڻپ && !thiz->subtables[ڳڻپ - 1].apply_func)
      ڳڻپ--;

    thiz->digest.init ();
    لكل (خودڪار& subtable : hb_iter (thiz->subtables, ڳڻپ))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_ٻيو = c_accelerate_subtables.digest_ٻيو;

    thiz->ڳڻپ = ڳڻپ;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      إذا (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#جيڪڏهن_پورو

    أرجع thiz;
  }

  فراغ fini ()
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      hb_free (subtables[i].external_cache);
#جيڪڏهن_پورو
  }

  منطقي may_have (hb_codepoint_t g) ثابت
  { أرجع digest.may_have (g); }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  منطقي apply (hb_ot_apply_context_t *c, منطقي use_cache) ثابت
  {
    إذا (ڳڻپ == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
      إذا (use_cache)
	أرجع subtables[0].apply_cached_no_digest (c);
#جيڪڏهن_پورو
      أرجع subtables[0].apply_no_digest (c);
    }
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (use_cache)
    {
      أرجع
      + hb_iter (hb_iter (subtables, ڳڻپ))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply_cached (c); })
      | hb_any
      ;
    }
    وإلا
#جيڪڏهن_پورو
    {
      أرجع
      + hb_iter (hb_iter (subtables, ڳڻپ))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply (c); })
      | hb_any
      ;
    }
    أرجع خطأ;
  }

  منطقي cache_enter (hb_ot_apply_context_t *c) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    أرجع subtable_cache_user_idx != (بنانشان) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#وإلا
    أرجع خطأ;
#جيڪڏهن_پورو
  }
  فراغ cache_leave (hb_ot_apply_context_t *c) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#جيڪڏهن_پورو
  }


  hb_سيٽ_digest_t digest;
  hb_سيٽ_digest_t digest_ٻيو;
  خاص:
  بنانشان ڳڻپ = 0; /* Number of subtables in the array. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_OT_LAYOUT_LOOKUP_CACHE
  بنانشان subtable_cache_user_idx = (بنانشان) -1;
#جيڪڏهن_پورو
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

بنية GSUBGPOS
{
  استعمال LookupListT = LookupList<SmallTypes>;
  استعمال LookupList2T = LookupList<MediumTypes>;
  سانچو <قسم_نالو TLookup>
  استعمال TLookupList = List16_جو_وٿي_منزل<TLookup, قسم_نالو SmallTypes::HBLUINT>;
  سانچو <قسم_نالو TLookup>
  استعمال TLookupList2 = List16_جو_وٿي_منزل<TLookup, قسم_نالو MediumTypes::HBLUINT>;
  سانچو <قسم_نالو TLookup>
  استعمال TLookup_وٿي_List = Lookup_وٿي_List<TLookup, قسم_نالو SmallTypes::HBLUINT>;
  سانچو <قسم_نالو TLookup>
  استعمال TLookup_وٿي_List2 = Lookup_وٿي_List<TLookup, قسم_نالو MediumTypes::HBLUINT>;

  size_t get_سائيز () ثابت
  {
    إذا (نسخو.major != 1) أرجع نسخو.static_سائيز;

    أرجع min_سائيز +
	   (نسخو.منزل_int () >= 0x00010001u ? featureVars.static_سائيز : 0) +
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
	   (نسخو.منزل_int () >= 0x00010002u ? 3 * Offset32::static_سائيز : 0)
#وإلا
	   0
#جيڪڏهن_پورو
	   ;
  }

  ثابت ScriptList &get_script_list () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010002u && scriptList2) أرجع هذا + scriptList2;
#جيڪڏهن_پورو
    أرجع هذا + scriptList;
  }
  ثابت FeatureList &get_feature_list () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010002u && featureList2) أرجع هذا + featureList2;
#جيڪڏهن_پورو
    أرجع هذا + featureList;
  }
  سانچو <قسم_نالو TLookup>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخو.sanitize (c) ||
		  نسخو.major != 1 ||
		  !c->check_range (هذا, get_سائيز ())))
      return_trace (خطأ);
    hb_barrier ();

    منطقي script_list_ok = scriptList.sanitize (c, هذا);
    منطقي feature_list_ok = featureList.sanitize (c, هذا);
    منطقي lookup_list_ok = typed_lookup_list_وٿي<TLookup> ().sanitize (c, هذا);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010002u)
    {
      إذا (scriptList2) script_list_ok = scriptList2.sanitize (c, هذا);
      إذا (featureList2) feature_list_ok = featureList2.sanitize (c, هذا);
      إذا (lookupList2) lookup_list_ok = typed_lookup_list2_وٿي<TLookup> ().sanitize (c, هذا);
    }
#جيڪڏهن_پورو

    إذا (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (خطأ);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (unlikely (!(نسخو.منزل_int () < 0x00010001u ||
		    featureVars.sanitize (c, هذا, &get_feature_list ()))))
      return_trace (خطأ);
#جيڪڏهن_پورو

    return_trace (سچ);
  }

  سانچو <قسم_نالو TLookup>
  منطقي subset (hb_subset_layout_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (نسخو.major != 1) return_trace (خطأ);

    خودڪار *ٻاهريون = c->subset_context->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->subset_context->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->نسخو = نسخو;

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010002u)
    {
      إذا (unlikely (!c->subset_context->serializer->extend_سائيز (ٻاهريون, get_سائيز ())))
        return_trace (خطأ);

      ٻاهريون->scriptList = 0;
      ٻاهريون->featureList = 0;
      ٻاهريون->lookupList = 0;

      إذا (lookupList2)
	serialize_subset_وٿي (c->subset_context, ٻاهريون->lookupList2,
				 subset_lookup_list2_وٿي<TLookup> (),
				 هذا, c);
      وإلا
	serialize_subset_وٿي (c->subset_context, ٻاهريون->lookupList2,
				 subset_lookup_list_وٿي<TLookup> (),
				 هذا, c);

      إذا (featureList2)
	serialize_subset_وٿي (c->subset_context, ٻاهريون->featureList2,
				 subset_feature_list2_وٿي (),
				 هذا, c);
      وإلا
	serialize_subset_وٿي (c->subset_context, ٻاهريون->featureList2,
				 subset_feature_list_وٿي (),
				 هذا, c);

      إذا (scriptList2)
	serialize_subset_وٿي (c->subset_context, ٻاهريون->scriptList2, scriptList2, هذا, c);
      وإلا
	serialize_subset_وٿي (c->subset_context, ٻاهريون->scriptList2, scriptList, هذا, c);
    }
    وإلا
#جيڪڏهن_پورو
    {
      ٻاهريون->subset_lookup_list_وٿي<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_وٿي<TLookup> (),
			     هذا,
			     c);

      ٻاهريون->subset_feature_list_وٿي ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_وٿي (),
			     هذا,
			     c);

      ٻاهريون->scriptList.serialize_subset (c->subset_context,
					scriptList,
					هذا,
					c);
    }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (نسخو.منزل_int () >= 0x00010001u)
    {
      خودڪار snapshot = c->subset_context->serializer->snapshot ();
      إذا (نسخو.منزل_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&ٻاهريون->featureVars))
        return_trace (خطأ);

      // if all axes are pinned all feature vars are dropped.
      منطقي ret = !c->subset_context->plan->all_axes_pinned
                 && ٻاهريون->featureVars.serialize_subset (c->subset_context, featureVars, هذا, c);
      إذا (!ret)
      {
        ٻاهريون->featureVars = 0;
	إذا (نسخو.منزل_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  ٻاهريون->نسخو.major = 1;
	  ٻاهريون->نسخو.minor = 0;
	}
      }
    }
#جيڪڏهن_پورو

    return_trace (سچ);
  }

  بنانشان صحيح get_lookup_ڳڻپ () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2).len;
#جيڪڏهن_پورو
    أرجع (هذا + lookupList).len;
  }
  ثابت Lookup& get_lookup (بنانشان صحيح i) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2)[i];
#جيڪڏهن_پورو
    أرجع (هذا + lookupList)[i];
  }
  ثابت FeatureVariations &get_feature_variations () ثابت
  {
    أرجع (نسخو.منزل_int () >= 0x00010001u && hb_barrier ()) ? هذا+featureVars : Null (FeatureVariations);
  }

  منطقي has_معلومات () ثابت { أرجع نسخو.منزل_int (); }
  بنانشان صحيح get_script_ڳڻپ () ثابت
  { أرجع get_script_list ().len; }
  ثابت ٽيگ& get_script_ٽيگ (بنانشان صحيح i) ثابت
  { أرجع get_script_list ().get_ٽيگ (i); }
  بنانشان صحيح get_script_ٽيگ_فهرست (بنانشان صحيح شروعات_وٿي,
				بنانشان صحيح *script_ڳڻپ /* IN/OUT */,
				hb_ٽيگ_t     *script_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  { أرجع get_script_list ().get_ٽيگ_فهرست (شروعات_وٿي, script_ڳڻپ, script_ٽيگ_فهرست); }
  ثابت Script& get_script (بنانشان صحيح i) ثابت
  { أرجع get_script_list ()[i]; }
  منطقي ڳوليو_script_اشاريو (hb_ٽيگ_t ٽيگ_2, بنانشان صحيح *اشاريو_2) ثابت
  { أرجع get_script_list ().ڳوليو_اشاريو (ٽيگ_2, اشاريو_2); }

  بنانشان صحيح get_feature_ڳڻپ () ثابت
  { أرجع get_feature_list ().len; }
  hb_ٽيگ_t get_feature_ٽيگ (بنانشان صحيح i) ثابت
  { أرجع i == اشاريو::NOT_مليو_اشاريو ? HB_ٽيگ_NONE : get_feature_list ().get_ٽيگ (i); }
  بنانشان صحيح get_feature_ٽيگ_فهرست (بنانشان صحيح شروعات_وٿي,
				 بنانشان صحيح *feature_ڳڻپ_2 /* IN/OUT */,
				 hb_ٽيگ_t     *feature_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  { أرجع get_feature_list ().get_ٽيگ_فهرست (شروعات_وٿي, feature_ڳڻپ_2, feature_ٽيگ_فهرست); }
  ثابت Feature& get_feature (بنانشان صحيح i) ثابت
  { أرجع get_feature_list ()[i]; }
  منطقي ڳوليو_feature_اشاريو (hb_ٽيگ_t ٽيگ_2, بنانشان صحيح *اشاريو_2) ثابت
  { أرجع get_feature_list ().ڳوليو_اشاريو (ٽيگ_2, اشاريو_2); }

  منطقي ڳوليو_variations_اشاريو (ثابت صحيح *coords, بنانشان صحيح انگ_coords,
			      بنانشان صحيح *اشاريو_2,
			      شي_Var_Store_Instancer *instancer) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    *اشاريو_2 = FeatureVariations::NOT_مليو_اشاريو;
    أرجع خطأ;
#جيڪڏهن_پورو
    أرجع get_feature_variations ().ڳوليو_اشاريو (coords, انگ_coords, اشاريو_2, instancer);
  }
  منطقي get_feature_variations_state (ثابت صحيح *coords,
				     بنانشان صحيح انگ_coords,
				     بنانشان صحيح *اشاريو_2,
				     شي_Var_Store_Instancer *instancer,
				     hb_ويڪٽر_t<بنانشان> *lookup_conditions) ثابت
  {
    lookup_conditions->reset ();
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    *اشاريو_2 = FeatureVariations::NOT_مليو_اشاريو;
    أرجع سچ;
#وإلا
    ڳوليو_variations_اشاريو (coords, انگ_coords, اشاريو_2, instancer);
    أرجع get_feature_variations ().collect_lookup_conditions (
	coords,
	انگ_coords,
	instancer,
	lookup_conditions);
#جيڪڏهن_پورو
  }
  منطقي get_lookup_variations (بنانشان feature_اشاريو,
                              بنانشان variations_اشاريو,
                              hb_sorted_array_t<ثابت بنانشان> conditions,
                              hb_سيٽ_t *lookup_indexes) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع خطأ;
#وإلا
    أرجع get_feature_variations ().get_lookup_variations (
	feature_اشاريو, get_feature_variation (feature_اشاريو, variations_اشاريو),
	conditions, lookup_indexes);
#جيڪڏهن_پورو
  }
  ثابت Feature& get_feature_variation (بنانشان صحيح feature_اشاريو,
					بنانشان صحيح variations_اشاريو) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (FeatureVariations::NOT_مليو_اشاريو != variations_اشاريو &&
	نسخو.منزل_int () >= 0x00010001u)
    {
      ثابت Feature *feature = get_feature_variations ().ڳوليو_substitute (variations_اشاريو,
									  feature_اشاريو);
      إذا (feature)
	أرجع *feature;
    }
#جيڪڏهن_پورو
    أرجع get_feature (feature_اشاريو);
  }

  فراغ feature_variation_collect_lookups (ثابت hb_سيٽ_t *feature_indexes,
					  ثابت hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map,
					  hb_سيٽ_t       *lookup_indexes /*ٻاهريون*/) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_رڪارڊ_cond_idx_map, lookup_indexes);
#جيڪڏهن_پورو
  }

  فراغ collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#جيڪڏهن_پورو
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#جيڪڏهن_پورو

  سانچو <قسم_نالو TLookup>
  فراغ گهيرو_lookups (hb_face_t      *face,
			ثابت hb_سيٽ_t *glyphs,
			hb_سيٽ_t       *lookup_indexes /* IN/OUT */) ثابت
  {
    hb_سيٽ_t گهميل_lookups, inactive_lookups;
    hb_گهيرو_lookups_context_t c (face, glyphs, &گهميل_lookups, &inactive_lookups);

    c.سيٽ_recurse_func (TLookup::سانچو dispatch_recurse_func<hb_گهيرو_lookups_context_t>);

    لكل (بنانشان lookup_اشاريو : *lookup_indexes)
      reinterpret_cast<ثابت TLookup &> (get_lookup (lookup_اشاريو)).گهيرو_lookups (&c, lookup_اشاريو);

    hb_سيٽ_union (lookup_indexes, &گهميل_lookups);
    hb_سيٽ_ڪٽيو (lookup_indexes, &inactive_lookups);
  }

  فراغ prune_langsys (ثابت hb_map_t *duplicate_feature_map,
                      ثابت hb_سيٽ_t *layout_scripts,
                      hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> *script_langsys_map,
                      hb_سيٽ_t       *new_feature_indexes /*ٻاهريون*/) ثابت
  {
    hb_prune_langsys_context_t c (هذا, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    بنانشان ڳڻپ = get_script_ڳڻپ ();
    لكل (بنانشان script_اشاريو = 0; script_اشاريو < ڳڻپ; script_اشاريو++)
    {
      ثابت ٽيگ& ٽيگ_2 = get_script_ٽيگ (script_اشاريو);
      إذا (!layout_scripts->has (ٽيگ_2)) استمر;
      ثابت Script& s = get_script (script_اشاريو);
      s.prune_langsys (&c, script_اشاريو);
    }
  }

  فراغ prune_features (ثابت hb_map_t *lookup_indices, /*اندريون*/
		       ثابت hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map, /*اندريون*/
		       ثابت hb_hashmap_t<بنانشان, ثابت Feature*> *feature_substitutes_map, /*اندريون*/
		       hb_سيٽ_t       *feature_indices /* IN/OUT */) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_سيٽ_t alternate_feature_indices;
    get_feature_variations ().گهيرو_features (lookup_indices, feature_رڪارڊ_cond_idx_map, &alternate_feature_indices);
    إذا (unlikely (alternate_feature_indices.اندريون_error()))
    {
      feature_indices->err ();
      أرجع;
    }
#جيڪڏهن_پورو

    لكل (بنانشان i : hb_iter (feature_indices))
    {
      hb_ٽيگ_t ٽيگ_2 =  get_feature_ٽيگ (i);
      إذا (ٽيگ_2 == HB_ٽيگ ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        استمر;


      ثابت Feature *f = &(get_feature (i));
      ثابت Feature** p = خالي_اشارو;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      إذا (!f->featureParams.is_null () &&
          ٽيگ_2 == HB_ٽيگ ('s', 'i', 'z', 'e'))
        استمر;

      إذا (!f->intersects_lookup_indexes (lookup_indices)
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
          && !alternate_feature_indices.has (i)
#جيڪڏهن_پورو
	  )
	feature_indices->del (i);
    }
  }

  فراغ collect_name_ids (ثابت hb_map_t *feature_اشاريو_map,
                         hb_سيٽ_t *nameids_منزل_retain, /*ٻاهريون*/
                         ثابت hb_hashmap_t<بنانشان, ثابت Feature*> *feature_substitutes_map,
                         ثابت hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map HB_اڻ_استعمال_ٿيل,
                         ثابت hb_سيٽ_t *catch_all_features,
                         منطقي all_axes_pinned HB_اڻ_استعمال_ٿيل) ثابت
  {
    بنانشان ڳڻپ = get_feature_ڳڻپ ();
    لكل (بنانشان i = 0 ; i < ڳڻپ; i++)
    {
      إذا (!feature_اشاريو_map->has (i)) استمر;
      hb_ٽيگ_t ٽيگ_2 = get_feature_ٽيگ (i);
      ثابت Feature *feature = &get_feature (i);
      ثابت Feature **substitute;
      إذا (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        إذا (catch_all_features->has (i))
          get_feature (i).collect_name_ids (ٽيگ_2, nameids_منزل_retain);
      }
      feature->collect_name_ids (ٽيگ_2, nameids_منزل_retain);
    }
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_اشاريو_map, feature_رڪارڊ_cond_idx_map,
                                                  [هذا] (بنانشان i) { أرجع get_feature_ٽيگ (i); },
                                                  nameids_منزل_retain);
#جيڪڏهن_پورو
  }

  سانچو <قسم_نالو T>
  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = سچ;
      هذا->جدول = sc.reference_جدول<T> (face);

      إذا (unlikely (هذا->جدول->is_blocklisted (هذا->جدول.get_blob (), face)))
      {
	hb_blob_destroy (هذا->جدول.get_blob ());
	هذا->جدول = hb_blob_get_خالي_جايون ();
      }

      هذا->lookup_ڳڻپ = جدول->get_lookup_ڳڻپ ();

      هذا->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (هذا->lookup_ڳڻپ, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->lookup_ڳڻپ = 0;
	هذا->جدول.destroy ();
	هذا->جدول = hb_blob_get_خالي_جايون ();
      }
    }
    ~accelerator_t ()
    {
      لكل (بنانشان صحيح i = 0; i < هذا->lookup_ڳڻپ; i++)
      {
	خودڪار *accel = هذا->accels[i].get_relaxed ();
	إذا (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (هذا->accels);
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (بنانشان lookup_اشاريو) ثابت
    {
      إذا (unlikely (lookup_اشاريو >= lookup_ڳڻپ)) أرجع خالي_اشارو;

    retry:
      خودڪار *accel = accels[lookup_اشاريو].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (جدول->get_lookup (lookup_اشاريو));
	إذا (unlikely (!accel))
	  أرجع خالي_اشارو;

	إذا (unlikely (!accels[lookup_اشاريو].cmpexch (خالي_اشارو, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> جدول;
    بنانشان صحيح lookup_ڳڻپ;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  محمي:
  سانچو <قسم_نالو TLookup>
  ثابت Offset16_منزل<TLookupList<TLookup>>& typed_lookup_list_وٿي () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<TLookupList<TLookup>>&> (lookupList); }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  سانچو <قسم_نالو TLookup>
  ثابت Offset32_منزل<TLookupList2<TLookup>>& typed_lookup_list2_وٿي () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<TLookupList2<TLookup>>&> (lookupList2); }
#جيڪڏهن_پورو

  سانچو <قسم_نالو TLookup>
  ثابت Offset16_منزل<TLookup_وٿي_List<TLookup>>& subset_lookup_list_وٿي () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<TLookup_وٿي_List<TLookup>>&> (lookupList); }
  سانچو <قسم_نالو TLookup>
  Offset16_منزل<TLookup_وٿي_List<TLookup>>& subset_lookup_list_وٿي ()
  { أرجع reinterpret_cast<Offset16_منزل<TLookup_وٿي_List<TLookup>>&> (lookupList); }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  سانچو <قسم_نالو TLookup>
  ثابت Offset32_منزل<TLookup_وٿي_List2<TLookup>>& subset_lookup_list2_وٿي () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<TLookup_وٿي_List2<TLookup>>&> (lookupList2); }
#جيڪڏهن_پورو

  ثابت Offset16_منزل<رڪارڊ_List_جو_Feature>& subset_feature_list_وٿي () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<رڪارڊ_List_جو_Feature>&> (featureList); }
  Offset16_منزل<رڪارڊ_List_جو_Feature>& subset_feature_list_وٿي ()
  { أرجع reinterpret_cast<Offset16_منزل<رڪارڊ_List_جو_Feature>&> (featureList); }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  ثابت Offset32_منزل<رڪارڊ_List_جو_Feature>& subset_feature_list2_وٿي () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<رڪارڊ_List_جو_Feature>&> (featureList2); }
#جيڪڏهن_پورو

  ثابت فراغ* get_lookup_list_ميدان_وٿي () ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (نسخو.منزل_int () >= 0x00010002u && lookupList2) أرجع &lookupList2;
#جيڪڏهن_پورو
    أرجع &lookupList;
  }

  سانچو <قسم_نالو وٿي_ٻاهريون, قسم_نالو وٿي_اندريون, قسم_نالو بنياد_3, قسم_نالو ...Ts>
  ساكن منطقي serialize_subset_وٿي (hb_subset_context_t *c,
				       وٿي_ٻاهريون& ٻاهريون,
				       ثابت وٿي_اندريون& اندريون,
				       ثابت بنياد_3 *بنياد,
				       Ts&&... ds)
  {
    ٻاهريون = 0;
    إذا (اندريون.is_null ()) أرجع خطأ;

    خودڪار *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (بنياد+اندريون, std::forward<Ts> (ds)...);
    إذا (ret) s->جوڙيو_link (ٻاهريون, s->ڪڍو_pack ());
    وإلا s->ڪڍو_discard ();
    أرجع ret;
  }

  Fixed_نسخو<>نسخو;	/* Version of the GSUB/GPOS table--initially set
				 * to 0x00010000u */
  Offset16_منزل<ScriptList>
		scriptList;	/* Offset to ScriptList table */
  Offset16_منزل<FeatureList>
		featureList;	/* Offset to FeatureList table */
  Offset16_منزل<LookupListT>
		lookupList;	/* Offset to LookupList table */
  Offset32_منزل<FeatureVariations>
		featureVars;	/* Offset to Feature Variations table--from
				 * beginning of table (may be NULL).
				 * Introduced in version 0x00010001. */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  Offset32_منزل<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32_منزل<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32_منزل<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#جيڪڏهن_پورو
  عام:
  DEFINE_سائيز_MIN (10);
};


} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_LAYOUT_GSUBGPOS_HH */
