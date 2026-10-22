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

#جے_بنا_تعریف HB_OT_LAYOUT_GSUBGPOS_HH
#تعریف_کرو HB_OT_LAYOUT_GSUBGPOS_HH

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
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { أرجع obj.intersects (هذا->glyphs); }
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  ثابت hb_سیٹ_t *glyphs;

  hb_intersects_context_t (ثابت hb_سیٹ_t *glyphs_) :
                            glyphs (glyphs_) {}
};

بنية hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, منطقي>
{
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { أرجع obj.may_have_non_1to1 (); }
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
};

بنية hb_گھیرا_context_t :
       hb_dispatch_context_t<hb_گھیرا_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_گھیرا_context_t *c, بنانشانی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indicies, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ);
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { obj.گھیرا (هذا); أرجع hb_خالی_تھاواں_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }
  فراغ recurse (بنانشانی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indicies, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ)
  {
    إذا (unlikely (nesting_level_بایاں == 0 || !recurse_func))
      أرجع;

    nesting_level_بایاں--;
    recurse_func (هذا, lookup_اشاریہ, covered_seq_indicies, seq_اشاریہ, سرا_اشاریہ);
    nesting_level_بایاں++;
  }

  فراغ reset_lookup_ویکھو_گنتی ()
  { lookup_گنتی = 0; }

  منطقي lookup_حد_بندی_exceeded ()
  { أرجع lookup_گنتی > HB_MAX_LOOKUP_ویکھو_گنتی; }

  منطقي should_ویکھو_lookup (بنانشانی صحيح lookup_اشاریہ)
  {
    إذا (lookup_گنتی++ > HB_MAX_LOOKUP_ویکھو_گنتی)
      أرجع خطأ;

    إذا (is_lookup_پورا (lookup_اشاریہ))
      أرجع خطأ;

    أرجع سچ;
  }

  منطقي is_lookup_پورا (بنانشانی صحيح lookup_اشاریہ)
  {
    إذا (unlikely (پورا_lookups_glyph_گنتی->وچ_error () ||
		  پورا_lookups_glyph_سیٹ->وچ_error ()))
      أرجع سچ;

    /* Have we visited this lookup with the current set of glyphs? */
    بنانشانی نکالو = glyphs->get_population ();
    إذا (unlikely (نکالو == HB_MAP_قدر_INVALID)) // Avoid sentinel interference.
      نکالو--;
    إذا (پورا_lookups_glyph_گنتی->get (lookup_اشاریہ) != نکالو)
    {
      پورا_lookups_glyph_گنتی->سیٹ (lookup_اشاریہ, نکالو);

      إذا (!پورا_lookups_glyph_سیٹ->has (lookup_اشاریہ))
      {
	إذا (unlikely (!پورا_lookups_glyph_سیٹ->سیٹ (lookup_اشاریہ, hb::unique_ptr<hb_سیٹ_t> {hb_سیٹ_create ()})))
	  أرجع سچ;
      }

      پورا_lookups_glyph_سیٹ->get (lookup_اشاریہ)->clear ();
    }

    hb_سیٹ_t *covered_glyph_سیٹ = پورا_lookups_glyph_سیٹ->get (lookup_اشاریہ);
    إذا (unlikely (covered_glyph_سیٹ->وچ_error ()))
      أرجع سچ;
    إذا (ماپا_سرگرم_glyphs ().is_subset (*covered_glyph_سیٹ))
      أرجع سچ;

    covered_glyph_سیٹ->union_ (ماپا_سرگرم_glyphs ());
    أرجع خطأ;
  }

  ثابت hb_سیٹ_t& پچھلا_ماپا_سرگرم_glyphs () {
    إذا (سرگرم_glyphs_ڈھیر.لمائی <= 1)
      أرجع *glyphs;

    أرجع سرگرم_glyphs_ڈھیر[سرگرم_glyphs_ڈھیر.لمائی - 2];
  }

  ثابت hb_سیٹ_t& ماپا_سرگرم_glyphs ()
  {
    إذا (!سرگرم_glyphs_ڈھیر)
      أرجع *glyphs;

    أرجع سرگرم_glyphs_ڈھیر.پوچھ ();
  }

  hb_سیٹ_t* ڈالو_cur_سرگرم_glyphs ()
  {
    hb_سیٹ_t *s = سرگرم_glyphs_ڈھیر.push ();
    إذا (unlikely (سرگرم_glyphs_ڈھیر.وچ_error ()))
      أرجع خالی_سوچک;
    أرجع s;
  }

  منطقي نکالو_cur_پورا_glyphs ()
  {
    إذا (!سرگرم_glyphs_ڈھیر)
      أرجع خطأ;

    سرگرم_glyphs_ڈھیر.نکالو ();
    أرجع سچ;
  }

  hb_face_t *face;
  hb_سیٹ_t *glyphs;
  hb_سیٹ_t output[1];
  hb_سمتیہ_t<hb_سیٹ_t> سرگرم_glyphs_ڈھیر;
  recurse_func_t recurse_func = خالی_سوچک;
  بنانشانی صحيح nesting_level_بایاں;

  hb_گھیرا_context_t (hb_face_t *face_,
			hb_سیٹ_t *glyphs_,
			hb_map_t *پورا_lookups_glyph_گنتی_2,
			hb_hashmap_t<بنانشانی, hb::unique_ptr<hb_سیٹ_t>> *پورا_lookups_glyph_سیٹ_2,
			بنانشانی صحيح nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_بایاں (nesting_level_بایاں_2),
			  پورا_lookups_glyph_گنتی (پورا_lookups_glyph_گنتی_2),
			  پورا_lookups_glyph_سیٹ (پورا_lookups_glyph_سیٹ_2)
  {}

  ~hb_گھیرا_context_t () { flush (); }

  فراغ سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }

  فراغ flush ()
  {
    output->del_range (face->get_عدد_glyphs (), HB_سیٹ_قدر_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    سرگرم_glyphs_ڈھیر.نکالو ();
    سرگرم_glyphs_ڈھیر.reset ();
  }

  خاص:
  hb_map_t *پورا_lookups_glyph_گنتی;
  hb_hashmap_t<بنانشانی, hb::unique_ptr<hb_سیٹ_t>> *پورا_lookups_glyph_سیٹ;
  بنانشانی صحيح lookup_گنتی = 0;
};



بنية hb_گھیرا_lookups_context_t :
       hb_dispatch_context_t<hb_گھیرا_lookups_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_گھیرا_lookups_context_t *c, بنانشانی lookup_اشاریہ);
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { obj.گھیرا_lookups (هذا); أرجع hb_خالی_تھاواں_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }
  فراغ recurse (بنانشانی lookup_اشاریہ)
  {
    إذا (unlikely (nesting_level_بایاں == 0 || !recurse_func))
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (lookup_حد_بندی_exceeded ()
        || ویکھیا_lookups->وچ_error ()
        || ویکھیا_lookups->has (lookup_اشاریہ))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      أرجع;

    nesting_level_بایاں--;
    recurse_func (هذا, lookup_اشاریہ);
    nesting_level_بایاں++;
  }

  فراغ سیٹ_lookup_ویکھیا (بنانشانی lookup_اشاریہ)
  { ویکھیا_lookups->جوڑو (lookup_اشاریہ); }

  فراغ سیٹ_lookup_inactive (بنانشانی lookup_اشاریہ)
  { inactive_lookups->جوڑو (lookup_اشاریہ); }

  منطقي lookup_حد_بندی_exceeded ()
  {
    منطقي ret = lookup_گنتی > HB_MAX_LOOKUP_ویکھو_گنتی;
    إذا (ret)
      DEBUG_پیغام (SUBSET, خالی_سوچک, "lookup visit count limit exceeded in lookup closure!");
    أرجع ret; }

  منطقي is_lookup_ویکھیا (بنانشانی lookup_اشاریہ)
  {
    إذا (unlikely (lookup_گنتی++ > HB_MAX_LOOKUP_ویکھو_گنتی))
    {
      DEBUG_پیغام (SUBSET, خالی_سوچک, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_گنتی, lookup_اشاریہ);
      أرجع سچ;
    }

    إذا (unlikely (ویکھیا_lookups->وچ_error ()))
      أرجع سچ;

    أرجع ویکھیا_lookups->has (lookup_اشاریہ);
  }

  hb_face_t *face;
  ثابت hb_سیٹ_t *glyphs;
  recurse_func_t recurse_func;
  بنانشانی صحيح nesting_level_بایاں;

  hb_گھیرا_lookups_context_t (hb_face_t *face_,
				ثابت hb_سیٹ_t *glyphs_,
				hb_سیٹ_t *ویکھیا_lookups_2,
				hb_سیٹ_t *inactive_lookups_,
				بنانشانی nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (خالی_سوچک),
				nesting_level_بایاں (nesting_level_بایاں_2),
				ویکھیا_lookups (ویکھیا_lookups_2),
				inactive_lookups (inactive_lookups_),
				lookup_گنتی (0) {}

  فراغ سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }

  خاص:
  hb_سیٹ_t *ویکھیا_lookups;
  hb_سیٹ_t *inactive_lookups;
  بنانشانی صحيح lookup_گنتی;
};

بنية hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, منطقي>
{
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { أرجع obj.would_apply (هذا); }
  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  hb_face_t *face;
  ثابت hb_codepoint_t *glyphs;
  بنانشانی صحيح len;
  منطقي zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ثابت hb_codepoint_t *glyphs_,
			    بنانشانی صحيح len_,
			    منطقي zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

بنية hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, بنانشانی صحيح lookup_اشاریہ);
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { obj.collect_glyphs (هذا); أرجع hb_خالی_تھاواں_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }
  فراغ recurse (بنانشانی صحيح lookup_اشاریہ)
  {
    إذا (unlikely (nesting_level_بایاں == 0 || !recurse_func))
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

    إذا (output == hb_سیٹ_get_خالی_تھاواں ())
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (recursed_lookups->has (lookup_اشاریہ))
      أرجع;

    hb_سیٹ_t *old_before = before;
    hb_سیٹ_t *old_داخلہ  = داخلہ;
    hb_سیٹ_t *old_after  = after;
    before = داخلہ = after = hb_سیٹ_get_خالی_تھاواں ();

    nesting_level_بایاں--;
    recurse_func (هذا, lookup_اشاریہ);
    nesting_level_بایاں++;

    before = old_before;
    داخلہ  = old_داخلہ;
    after  = old_after;

    recursed_lookups->جوڑو (lookup_اشاریہ);
  }

  hb_face_t *face;
  hb_سیٹ_t *before;
  hb_سیٹ_t *داخلہ;
  hb_سیٹ_t *after;
  hb_سیٹ_t *output;
  recurse_func_t recurse_func;
  hb_سیٹ_t *recursed_lookups;
  بنانشانی صحيح nesting_level_بایاں;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_سیٹ_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_سیٹ_t  *glyphs_داخلہ,  /* OUT.  May be NULL */
			       hb_سیٹ_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_سیٹ_t  *glyphs_output, /* OUT.  May be NULL */
			       بنانشانی صحيح nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_سیٹ_get_خالی_تھاواں ()),
			      داخلہ  (glyphs_داخلہ  ? glyphs_داخلہ  : hb_سیٹ_get_خالی_تھاواں ()),
			      after  (glyphs_after  ? glyphs_after  : hb_سیٹ_get_خالی_تھاواں ()),
			      output (glyphs_output ? glyphs_output : hb_سیٹ_get_خالی_تھاواں ()),
			      recurse_func (خالی_سوچک),
			      recursed_lookups (hb_سیٹ_create ()),
			      nesting_level_بایاں (nesting_level_بایاں_2) {}
  ~hb_collect_glyphs_context_t () { hb_سیٹ_destroy (recursed_lookups); }

  فراغ سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

بنية hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  تعداد glyph_سیٹ_type_t
  {
    GLYPH_سیٹ_CLASS,
    GLYPH_سیٹ_COVERAGE,
    GLYPH_سیٹ_SINGLE
  };

  ساكن constexpr hb_codepoint_t SINGLE_GLYPH_سرگرم_FLAG = 0x80000000u;

  بنية glyph_سیٹ_cache_کلید_t
  {
    glyph_سیٹ_cache_کلید_t () = افتراضي;
    glyph_سیٹ_cache_کلید_t (ثابت فراغ *انکڑے_2, بنانشانی قدر_3, glyph_سیٹ_type_t type_)
      : انکڑے (انکڑے_2), value (قدر_3), type (type_) {}

    منطقي عامل == (ثابت glyph_سیٹ_cache_کلید_t &o) ثابت
    { أرجع انکڑے == o.انکڑے && value == o.value && type == o.type; }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) انکڑے);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash ((بنانشانی) type);
    }

    ثابت فراغ *انکڑے = خالی_سوچک;
    بنانشانی value = 0;
    glyph_سیٹ_type_t type = GLYPH_سیٹ_CLASS;
  };

  بنية context_سیٹ_cache_کلید_t
  {
    context_سیٹ_cache_کلید_t () = افتراضي;
    context_سیٹ_cache_کلید_t (ثابت فراغ *قاعدہ,
			     ثابت فراغ *ریکارڈ_3,
			     ثابت فراغ *انکڑے_2,
			     hb_codepoint_t سرگرم_glyphs,
			     بنانشانی قدر_3,
			     بنانشانی format_)
      : قاعدہ_2 (قاعدہ), ریکارڈ (ریکارڈ_3), انکڑے (انکڑے_2), سرگرم_glyphs_2 (سرگرم_glyphs),
	value (قدر_3), format (format_) {}

    منطقي عامل == (ثابت context_سیٹ_cache_کلید_t &o) ثابت
    {
      أرجع قاعدہ_2 == o.قاعدہ_2 &&
	     ریکارڈ == o.ریکارڈ &&
	     انکڑے == o.انکڑے &&
	     سرگرم_glyphs_2 == o.سرگرم_glyphs_2 &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) قاعدہ_2);
      current = current * 31 + hb_hash ((uintptr_t) ریکارڈ);
      current = current * 31 + hb_hash ((uintptr_t) انکڑے);
      current = current * 31 + hb_hash (سرگرم_glyphs_2);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash (format);
    }

    ثابت فراغ *قاعدہ_2 = خالی_سوچک;
    ثابت فراغ *ریکارڈ = خالی_سوچک;
    ثابت فراغ *انکڑے = خالی_سوچک;
    hb_codepoint_t سرگرم_glyphs_2 = HB_CODEPOINT_INVALID;
    بنانشانی value = 0;
    بنانشانی format = 0;
  };

  بنية recurse_کلید_t
  {
    recurse_کلید_t () = افتراضي;
    recurse_کلید_t (بنانشانی lookup_اشاریہ_3,
                   hb_codepoint_t سرگرم_glyphs,
                   hb_codepoint_t recurse_رستہ,
                   hb_codepoint_t context_سیٹ_2)
      : lookup_اشاریہ (lookup_اشاریہ_3),
        سرگرم_glyphs_2 (سرگرم_glyphs),
        recurse_رستہ_2 (recurse_رستہ),
        context_سیٹ (context_سیٹ_2) {}

    منطقي عامل == (ثابت recurse_کلید_t &o) ثابت
    {
      أرجع lookup_اشاریہ == o.lookup_اشاریہ &&
             سرگرم_glyphs_2 == o.سرگرم_glyphs_2 &&
             recurse_رستہ_2 == o.recurse_رستہ_2 &&
             context_سیٹ == o.context_سیٹ;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_اشاریہ)) * 16777619;
      current = (current ^ hb_hash (سرگرم_glyphs_2)) * 16777619;
      current = (current ^ hb_hash (recurse_رستہ_2)) * 16777619;
      current = (current ^ hb_hash (context_سیٹ)) * 16777619;
      أرجع current;
    }

    بنانشانی lookup_اشاریہ = 0;
    hb_codepoint_t سرگرم_glyphs_2 = 0;
    hb_codepoint_t recurse_رستہ_2 = 0;
    hb_codepoint_t context_سیٹ = HB_CODEPOINT_INVALID;
  };

  تعريفنوع return_t (*recurse_func_t) (hb_depend_context_t *c, بنانشانی lookup_اشاریہ, hb_ثنائی_ہندسہ_ورقہ_t *covered_seq_indices, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { obj.depend (هذا); أرجع hb_خالی_تھاواں_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }

  فراغ recurse (بنانشانی lookup_idx, hb_ثنائی_ہندسہ_ورقہ_t *covered_seq_indices, بنانشانی seq_اشاریہ, بنانشانی سرا_اشاریہ)
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
    إذا (lookups_تکیا.has (lookup_idx)) أرجع;
    lookups_تکیا.جوڑو (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t ماپا_رستہ = recurse_رستہ_2;
    uint64_t رستہ_کلید = (uint64_t) ماپا_رستہ << 32 | lookup_idx;
    hb_codepoint_t *رستہ = خالی_سوچک;
    hb_codepoint_t رستہ_اشاریہ;
    إذا (!recurse_رستہ_منزل_اشاریہ.has (رستہ_کلید, &رستہ))
    {
      رستہ_اشاریہ = recurse_رستہ_منزل_اشاریہ.get_population () + 1;
      إذا (unlikely (!recurse_رستہ_منزل_اشاریہ.سیٹ (رستہ_کلید, رستہ_اشاریہ)))
      {
	depend_انکڑے->fail ();
	lookups_تکیا.del (lookup_idx);
	أرجع;
      }
    }
    وإلا
      رستہ_اشاریہ = *رستہ;
    recurse_رستہ_2 = رستہ_اشاریہ;
    recurse_func (هذا, lookup_idx, covered_seq_indices, seq_اشاریہ, سرا_اشاریہ);
    recurse_رستہ_2 = ماپا_رستہ;
    lookups_تکیا.del (lookup_idx);
  }

  بنية سرگرم_glyphs_چوکھٹا_t
  {
    hb_سیٹ_t glyphs;
    hb_codepoint_t سرگرم_idx = HB_CODEPOINT_INVALID;
    منطقي use_جڑ = خطأ;
  };

  ثابت hb_سیٹ_t& ماپا_سرگرم_glyphs ()
  {
    إذا (!سرگرم_glyphs_ڈھیر_ڈونگھائی)
      أرجع *glyphs;

    ثابت سرگرم_glyphs_چوکھٹا_t &چوکھٹا = سرگرم_glyphs_ڈھیر[سرگرم_glyphs_ڈھیر_ڈونگھائی - 1];
    أرجع چوکھٹا.use_جڑ ? *glyphs : چوکھٹا.glyphs;
  }

  hb_سیٹ_t* ڈالو_cur_سرگرم_glyphs (hb_codepoint_t سرگرم_idx = HB_CODEPOINT_INVALID)
  {
    إذا (سرگرم_glyphs_ڈھیر_ڈونگھائی == سرگرم_glyphs_ڈھیر.لمائی &&
	unlikely (!سرگرم_glyphs_ڈھیر.ڈالو_or_fail ()))
      أرجع خالی_سوچک;

    سرگرم_glyphs_چوکھٹا_t &چوکھٹا = سرگرم_glyphs_ڈھیر[سرگرم_glyphs_ڈھیر_ڈونگھائی++];
    چوکھٹا.glyphs.clear ();
    چوکھٹا.سرگرم_idx = سرگرم_idx;
    چوکھٹا.use_جڑ = خطأ;
    أرجع &چوکھٹا.glyphs;
  }

  منطقي ڈالو_جڑ_سرگرم_glyphs ()
  {
    إذا (سرگرم_glyphs_ڈھیر_ڈونگھائی == سرگرم_glyphs_ڈھیر.لمائی &&
	unlikely (!سرگرم_glyphs_ڈھیر.ڈالو_or_fail ()))
      أرجع خطأ;

    سرگرم_glyphs_ڈھیر[سرگرم_glyphs_ڈھیر_ڈونگھائی++].use_جڑ = سچ;
    أرجع سچ;
  }

  منطقي نکالو_cur_پورا_glyphs ()
  {
    إذا (!سرگرم_glyphs_ڈھیر_ڈونگھائی)
      أرجع خطأ;

    سرگرم_glyphs_ڈھیر_ڈونگھائی--;
    أرجع سچ;
  }

  منطقي get_ماپا_سرگرم_glyphs_اشاریہ (hb_codepoint_t *سرگرم_idx)
  {
    إذا (!سرگرم_glyphs_ڈھیر_ڈونگھائی ||
	سرگرم_glyphs_ڈھیر[سرگرم_glyphs_ڈھیر_ڈونگھائی - 1].use_جڑ)
    {
      *سرگرم_idx = HB_CODEPOINT_INVALID;
      أرجع سچ;
    }

    سرگرم_glyphs_چوکھٹا_t &چوکھٹا = سرگرم_glyphs_ڈھیر[سرگرم_glyphs_ڈھیر_ڈونگھائی - 1];
    *سرگرم_idx = چوکھٹا.سرگرم_idx;
    إذا (*سرگرم_idx != HB_CODEPOINT_INVALID)
      أرجع سچ;

    hb_codepoint_t glyph;
    إذا (چوکھٹا.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_سرگرم_FLAG))
      *سرگرم_idx = glyph | SINGLE_GLYPH_سرگرم_FLAG;
    وإلا إذا (!لبھو_or_create_recurse_سیٹ (چوکھٹا.glyphs, سرگرم_idx))
      أرجع خطأ;

    چوکھٹا.سرگرم_idx = *سرگرم_idx;
    أرجع سچ;
  }

  منطقي get_context_سیٹ_cache_کلید (ثابت فراغ *قاعدہ_2,
				  ثابت فراغ *ریکارڈ,
				  ثابت فراغ *انکڑے,
				  بنانشانی value,
				  بنانشانی format,
				  context_سیٹ_cache_کلید_t *key)
  {
    hb_codepoint_t سرگرم_idx;
    إذا (!get_ماپا_سرگرم_glyphs_اشاریہ (&سرگرم_idx))
      أرجع خطأ;
    *key = context_سیٹ_cache_کلید_t (قاعدہ_2, ریکارڈ, انکڑے, سرگرم_idx, value, format);
    أرجع سچ;
  }

  منطقي get_recurse_کلید (بنانشانی lookup_idx, recurse_کلید_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t سرگرم_idx;
    إذا (!get_ماپا_سرگرم_glyphs_اشاریہ (&سرگرم_idx))
      أرجع خطأ;

    *key = recurse_کلید_t (lookup_idx,
                          سرگرم_idx,
                          recurse_رستہ_2,
                          depend_انکڑے->current_context_سیٹ_اشاریہ);
    أرجع !completed_recursions.has (*key);
  }

  فراغ انت_recurse (ثابت recurse_کلید_t &key)
  { depend_انکڑے->check_success (completed_recursions.سیٹ (key, سچ)); }

  فراغ reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_رستہ_منزل_اشاریہ.clear ();
    recurse_رستہ_2 = 0;
  }

  منطقي لبھو_or_create_recurse_سیٹ (ثابت hb_سیٹ_t &سیٹ, hb_codepoint_t *اشاریہ_2)
  {
    hb_codepoint_t *existing = خالی_سوچک;
    إذا (recurse_سیٹ_منزل_اشاریہ.has (&سیٹ, &existing))
    {
      *اشاریہ_2 = *existing;
      أرجع سچ;
    }

    hb_سیٹ_t *copy = hb_object_create<hb_سیٹ_t> ();
    إذا (unlikely (!copy))
      أرجع depend_انکڑے->fail ();
    copy->سیٹ (سیٹ);
    إذا (unlikely (copy->وچ_error ()))
    {
      hb_سیٹ_destroy (copy);
      أرجع depend_انکڑے->fail ();
    }

    *اشاریہ_2 = recurse_sets.لمائی;
    إذا (unlikely (*اشاریہ_2 & SINGLE_GLYPH_سرگرم_FLAG))
    {
      hb_سیٹ_destroy (copy);
      أرجع depend_انکڑے->fail ();
    }
    إذا (unlikely (!recurse_sets.ڈالو_or_fail (hb::unique_ptr<hb_سیٹ_t> {copy})))
      أرجع depend_انکڑے->fail ();
    إذا (unlikely (!recurse_سیٹ_منزل_اشاریہ.سیٹ (recurse_sets[*اشاریہ_2].get (), *اشاریہ_2)))
      أرجع depend_انکڑے->fail ();
    أرجع سچ;
  }

  ثابت hb_سیٹ_t *لبھو_or_create_glyph_سیٹ (ثابت فراغ *انکڑے,
                                             بنانشانی value,
                                             glyph_سیٹ_type_t type);
  hb_codepoint_t لبھو_or_create_cached_context_سیٹ (ثابت hb_سیٹ_t *سیٹ);

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
      داخلہ_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_سیٹ_t preliminary_context;
    hb_سیٹ_t position_scratch;
    hb_سیٹ_t pos_glyphs;
    hb_سیٹ_t position_context;
    hb_سیٹ_t disjunctive_indices;
    hb_سیٹ_t filtered_disjunctive_indices;
    hb_سمتیہ_t<ثابت hb_سیٹ_t *> backtrack_sets;
    hb_سمتیہ_t<ثابت hb_سیٹ_t *> lookahead_sets;
    hb_سمتیہ_t<ثابت hb_سیٹ_t *> داخلہ_position_glyphs;
    hb_سمتیہ_t<hb_codepoint_t> cached_context_sets;
    hb_سمتیہ_t<بنانشانی> compact_workspace;
    depend_scratch_t *next = خالی_سوچک;
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
        depend_انکڑے->fail ();
        أرجع خالی_سوچک;
      }
      جديد (scratch) depend_scratch_t ();
    }

    scratch->next = خالی_سوچک;
    scratch->clear ();
    أرجع scratch;
  }

  فراغ release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_انکڑے_builder_t *depend_انکڑے;
  hb_face_t *face;
  hb_سیٹ_t *glyphs;
  hb_سمتیہ_t<سرگرم_glyphs_چوکھٹا_t> سرگرم_glyphs_ڈھیر;
  بنانشانی سرگرم_glyphs_ڈھیر_ڈونگھائی = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_اشاریہ = HB_CODEPOINT_INVALID;
  hb_ثنائی_ہندسہ_سیٹ_t lookups_تکیا;
  hb_سمتیہ_t<hb::unique_ptr<hb_سیٹ_t>> recurse_sets;
  hb_hashmap_t<ثابت hb_سیٹ_t *, hb_codepoint_t> recurse_سیٹ_منزل_اشاریہ;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_رستہ_منزل_اشاریہ;
  hb_codepoint_t recurse_رستہ_2 = 0;
  hb_hashmap_t<recurse_کلید_t, منطقي> completed_recursions;
  hb_سمتیہ_t<hb::unique_ptr<hb_سیٹ_t>> glyph_sets;
  hb_hashmap_t<glyph_سیٹ_cache_کلید_t, hb_codepoint_t> glyph_سیٹ_منزل_اشاریہ;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_سیٹ_indices;
  hb_hashmap_t<context_سیٹ_cache_کلید_t, hb_codepoint_t> context_سیٹ_cache;
  depend_scratch_t *depend_scratch_pool = خالی_سوچک;

  hb_depend_context_t (hb_depend_انکڑے_builder_t *depend_انکڑے_2,
                       hb_face_t *face_,
                       hb_سیٹ_t *glyphs_)
                      : depend_انکڑے (depend_انکڑے_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (خالی_سوچک) {}
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
  فراغ سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

سانچا <قسم_ناں سیٹ_t>
بنية hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<سیٹ_t>, ثابت Coverage &>
{
  تعريفنوع ثابت Coverage &return_t; // Stoopid that we have to dupe this here.
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { أرجع obj.get_coverage (); }
  ساكن return_t default_return_قدر () { أرجع Null (Coverage); }
  منطقي stop_sublookup_iteration (return_t r) ثابت
  {
    r.collect_coverage (سیٹ);
    أرجع خطأ;
  }

  hb_collect_coverage_context_t (سیٹ_t *سیٹ_2) :
				   سیٹ (سیٹ_2) {}

  سیٹ_t *سیٹ;
};

بنية matcher_t
{
  تعريفنوع منطقي (*میل_func_t) (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے);

  سانچا <قسم_ناں context_t>
  فراغ init (ثابت context_t *c, منطقي context_میل = خطأ)
  {
    سیٹ_میل_func (خالی_سوچک, خالی_سوچک);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->جدول_اشاریہ == 1 || (context_میل && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_میل || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->جدول_اشاریہ == 1;
    نقاب = context_میل ? -1 : c->lookup_نقاب;
    /* Per syllable matching is only for GSUB. */
    فی_syllable = c->جدول_اشاریہ == 0 && c->فی_syllable;
    syllable = 0;
  }

  فراغ سیٹ_میل_func (میل_func_t میل_func,
		       ثابت فراغ *میل_انکڑے)
  { میل_func_2 = میل_func; میل_انکڑے_2 = میل_انکڑے; }

  تعداد may_میل_t {
    میل_NO,
    میل_YES,
    میل_MAYBE
  };

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  may_میل_t may_میل (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_انکڑے) ثابت
  {
    إذا (!(info.نقاب & نقاب) ||
	(فی_syllable && syllable && syllable != info.syllable ()))
      أرجع میل_NO;

    إذا (میل_func_2)
      أرجع میل_func_2 (info, glyph_انکڑے, میل_انکڑے_2) ? میل_YES : میل_NO;

    أرجع میل_MAYBE;
  }

  تعداد may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  سانچا <قسم_ناں context_t>
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
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
  بنانشانی صحيح lookup_props = 0;
  hb_نقاب_t نقاب = -1;
  منطقي ignore_zwnj = خطأ;
  منطقي ignore_zwj = خطأ;
  منطقي ignore_hidden = خطأ;
  منطقي فی_syllable = خطأ;
  uint8_t syllable = 0;
  میل_func_t میل_func_2 = خالی_سوچک;
  ثابت فراغ *میل_انکڑے_2 = خالی_سوچک;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
بنية glyph_انکڑے_t
{
  سانچا <قسم_ناں T>
  glyph_انکڑے_t (ثابت T *انکڑے_2) : انکڑے (انکڑے_2)
#جے_بنا_تعریف HB_NO_BEYOND_64K
                              , wide (حجم (T) == 3)
#جے_انت
  {
    static_assert (حجم (T) == 2 || حجم (T) == 3, "invalid glyph data size");
  }

  بنانشانی عامل [] (بنانشانی i) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (wide)
      أرجع ((ثابت HBUINT24 *) انکڑے)[i];
#جے_انت
    أرجع ((ثابت HBUINT16 *) انکڑے)[i];
  }

  ثابت فراغ *انکڑے;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  منطقي wide;
#جے_انت
};

سانچا <قسم_ناں context_t>
بنية skipping_iterator_t
{
  فراغ init (context_t *c_, منطقي context_میل = خطأ)
  {
    c = c_;
    سرا = c->بفر->len;
    میل_glyph_data16 = خالی_سوچک;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    میل_glyph_data24 = خالی_سوچک;
#جے_انت
    matcher.init (c, context_میل);
  }
  فراغ سیٹ_lookup_props (بنانشانی صحيح lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  فراغ سیٹ_میل_func (matcher_t::میل_func_t میل_func,
		       ثابت فراغ *میل_انکڑے)
  {
    matcher.سیٹ_میل_func (میل_func, میل_انکڑے);
  }
  فراغ سیٹ_glyph_انکڑے (glyph_انکڑے_t glyph_انکڑے)
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    میل_glyph_data16 = glyph_انکڑے.wide ? خالی_سوچک : (ثابت HBUINT16 *) glyph_انکڑے.انکڑے;
    میل_glyph_data24 = glyph_انکڑے.wide ? (ثابت HBUINT24 *) glyph_انکڑے.انکڑے : خالی_سوچک;
#وإلا
    میل_glyph_data16 = (ثابت HBUINT16 *) glyph_انکڑے.انکڑے;
#جے_انت
  }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  فراغ reset (بنانشانی صحيح شروع_اشاریہ_2)
  {
    // For GSUB forward iterator
    idx = شروع_اشاریہ_2;
    سرا = c->بفر->len;
    matcher.syllable = c->بفر->cur().syllable();
  }
  فراغ reset_پٹھ (بنانشانی صحيح شروع_اشاریہ_2, منطقي from_باہرلا_بفر = خطأ)
  {
    // For GSUB backward iterator
    idx = شروع_اشاریہ_2;
    matcher.syllable = c->بفر->cur().syllable();
  }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  فراغ reset_fast (بنانشانی صحيح شروع_اشاریہ_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = شروع_اشاریہ_2;
  }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  matcher_t::may_skip_t may_skip (ثابت hb_glyph_info_t &info) ثابت
  { أرجع matcher.may_skip (c, info); }

  تعداد میل_t {
    میل_2,
    NOT_میل,
    SKIP
  };

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  میل_t میل (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    إذا (unlikely (skip == matcher_t::SKIP_YES))
      أرجع SKIP;

    matcher_t::may_میل_t میل = matcher.may_میل (info, get_glyph_انکڑے ());
    إذا (میل == matcher_t::میل_YES ||
	(میل == matcher_t::میل_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      أرجع میل_2;

    إذا (skip == matcher_t::SKIP_NO)
      أرجع NOT_میل;

    أرجع SKIP;
  }

  منطقي next (بنانشانی *unsafe_منزل = خالی_سوچک)
  {
    خودکار *info = c->بفر->info;
    ثابت نشانیوالا stop = (نشانیوالا) سرا - 1;
    طالما ((نشانیوالا) idx < stop)
    {
      idx++;
      بدل (میل (info[idx]))
      {
	حالة میل_2:
	{
	  advance_glyph_انکڑے ();
	  أرجع سچ;
	}
	حالة NOT_میل:
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
      *unsafe_منزل = سرا;
    أرجع خطأ;
  }
  منطقي prev (بنانشانی *unsafe_from = خالی_سوچک)
  {
    خودکار *باہرلا_info = c->بفر->باہرلا_info;
    ثابت بنانشانی stop = 0;
    طالما (idx > stop)
    {
      idx--;
      بدل (میل (باہرلا_info[idx]))
      {
	حالة میل_2:
	{
	  advance_glyph_انکڑے ();
	  أرجع سچ;
	}
	حالة NOT_میل:
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
  get_glyph_انکڑے ()
  {
    إذا (میل_glyph_data16) أرجع *میل_glyph_data16;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    وإلا
    إذا (میل_glyph_data24) أرجع *میل_glyph_data24;
#جے_انت
    أرجع 0;
  }
  HB_ALWAYS_INLINE
  فراغ
  advance_glyph_انکڑے ()
  {
    إذا (میل_glyph_data16) میل_glyph_data16++;
#جے_بنا_تعریف HB_NO_BEYOND_64K
    وإلا
    إذا (میل_glyph_data24) میل_glyph_data24++;
#جے_انت
  }

  بنانشانی صحيح idx;
  محمي:
  context_t *c;
  matcher_t matcher;
  ثابت HBUINT16 *میل_glyph_data16;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  ثابت HBUINT24 *میل_glyph_data24;
#جے_انت

  بنانشانی صحيح سرا;
};

بنية hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  تعريفنوع return_t (*recurse_func_t) (hb_ot_apply_context_t *c, بنانشانی صحيح lookup_اشاریہ);

  سانچا <قسم_ناں T>
  ساكن مضمن خودکار apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_ترجیح<1>) HB_RETURN (return_t, obj.apply (c, خالی_سوچک) )
  سانچا <قسم_ناں T>
  ساكن مضمن خودکار apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_ترجیح<0>) HB_RETURN (return_t, obj.apply (c) )
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { أرجع apply_(obj, هذا, hb_prioritize); }

  ساكن return_t default_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
  return_t recurse (بنانشانی صحيح sub_lookup_اشاریہ)
  {
    أكد (recurse_func);
    إذا (unlikely (nesting_level_بایاں == 0))
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

    nesting_level_بایاں--;
    منطقي ret = recurse_func (هذا, sub_lookup_اشاریہ);
    nesting_level_بایاں++;
    أرجع ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_داخلہ, iter_context;

  بنانشانی صحيح جدول_اشاریہ; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t *بفر;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = خالی_سوچک;
  ثابت GDEF &gdef;
  ثابت GDEF::accelerator_t &gdef_accel;
  ثابت شے_Variation_Store &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_نقاب_t lookup_نقاب = 1;
  بنانشانی صحيح lookup_اشاریہ = (بنانشانی) -1;
  بنانشانی صحيح lookup_props = 0;
  بنانشانی صحيح nesting_level_بایاں = HB_MAX_NESTING_LEVEL;

  منطقي has_glyph_classes;
  منطقي auto_zwnj = سچ;
  منطقي auto_zwj = سچ;
  منطقي فی_syllable = خطأ;
  منطقي random = خطأ;
  بنانشانی new_syllables = (بنانشانی) -1;

  نشانیوالا آخری_ادھار = -1; // GPOS uses
  بنانشانی آخری_ادھار_until = 0; // GPOS uses

  hb_سمتیہ_t<uint32_t> میل_positions;
  uint32_t ڈھیر_میل_positions[8];
#جے_بنا_تعریف HB_NO_بفر_سنیہا
  hb_بفر_t::بدلیا_func_t orig_بدلیا_func = خالی_سوچک;
  فراغ *orig_بدلیا_انکڑے = خالی_سوچک;
#جے_انت

  hb_ot_apply_context_t (بنانشانی صحيح جدول_اشاریہ_2,
			 hb_font_t *font_,
			 hb_بفر_t *بفر_2,
			 hb_blob_t *جدول_blob_2,
			 hb_scalar_cache_t *var_store_cache_ = خالی_سوچک) :
			جدول_اشاریہ (جدول_اشاریہ_2),
			font (font_), face (font->face), بفر (بفر_2),
			sanitizer (جدول_blob_2),
			gdef (
#جے_بنا_تعریف HB_NO_OT_LAYOUT
			      *face->جدول.GDEF->جدول
#وإلا
			      Null (GDEF)
#جے_انت
			     ),
			gdef_accel (
#جے_بنا_تعریف HB_NO_OT_LAYOUT
			      *face->جدول.GDEF
#وإلا
			      Null (GDEF::accelerator_t)
#جے_انت
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (بفر_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    میل_positions.سیٹ_storage (ڈھیر_میل_positions);

#جے_بنا_تعریف HB_NO_بفر_سنیہا
    إذا (بفر->messaging ())
    {
      أكد (بفر->بدلیا_func == خالی_سوچک ||
	      بفر->بدلیا_func == بفر_بدلیا_trampoline);
      orig_بدلیا_func = بفر->بدلیا_func;
      orig_بدلیا_انکڑے = بفر->بدلیا_انکڑے;
      بفر->بدلیا_func = بفر_بدلیا_trampoline;
      بفر->بدلیا_انکڑے = هذا;
    }
#جے_انت
  }
  ~hb_ot_apply_context_t ()
  {
#جے_بنا_تعریف HB_NO_بفر_سنیہا
    إذا (بفر->messaging ())
    {
      أكد (بفر->بدلیا_func == بفر_بدلیا_trampoline);
      أكد (بفر->بدلیا_انکڑے == هذا);
      بفر->بدلیا_func = orig_بدلیا_func;
      بفر->بدلیا_انکڑے = orig_بدلیا_انکڑے;
    }
#جے_انت
  }

  فراغ init_iters ()
  {
    iter_داخلہ.init (هذا, خطأ);
    iter_context.init (هذا, سچ);
  }

  فراغ سیٹ_lookup_نقاب (hb_نقاب_t نقاب, منطقي init = سچ) { lookup_نقاب = نقاب; آخری_ادھار = -1; آخری_ادھار_until = 0; إذا (init) init_iters (); }
  فراغ سیٹ_auto_zwj (منطقي auto_zwj_, منطقي init = سچ) { auto_zwj = auto_zwj_; إذا (init) init_iters (); }
  فراغ سیٹ_auto_zwnj (منطقي auto_zwnj_, منطقي init = سچ) { auto_zwnj = auto_zwnj_; إذا (init) init_iters (); }
  فراغ سیٹ_فی_syllable (منطقي فی_syllable_2, منطقي init = سچ) { فی_syllable = فی_syllable_2; إذا (init) init_iters (); }
  فراغ سیٹ_random (منطقي random_) { random = random_; }
  فراغ سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }
  فراغ سیٹ_lookup_اشاریہ (بنانشانی صحيح lookup_اشاریہ_3) { lookup_اشاریہ = lookup_اشاریہ_3; }
  فراغ سیٹ_lookup_props (بنانشانی صحيح lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_نمبر ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    بفر->random_state = بفر->random_state * 48271 % 2147483647;
    أرجع بفر->random_state;
  }

  ساكن فراغ بفر_بدلیا_trampoline (hb_بفر_t *بفر HB_بن_ورتیا, فراغ *user_انکڑے)
  {
    ((hb_ot_apply_context_t *) user_انکڑے)->بفر_بدلیا ();
  }
  فراغ بفر_بدلیا ()
  {
    بفر->update_digest ();
    إذا (likely (has_glyph_classes))
      لكل (بنانشانی i = 0; i < بفر->len; i++)
	سیٹ_glyph_class_props (بفر->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  منطقي میل_properties_mark (ثابت hb_glyph_info_t *info,
			      بنانشانی صحيح    glyph_props,
			      بنانشانی صحيح    میل_props) ثابت
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    إذا (میل_props & LookupFlag::Use_Mark_Filtering_سیٹ)
    {
      بنانشانی سیٹ_اشاریہ = میل_props >> 16;
      أرجع gdef_accel.mark_سیٹ_covers (سیٹ_اشاریہ, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    إذا (میل_props & LookupFlag::MarkAttachmentType)
      أرجع (میل_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    أرجع سچ;
  }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  منطقي check_glyph_property (ثابت hb_glyph_info_t *info,
			     بنانشانی میل_props) ثابت
  {
    بنانشانی صحيح glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    إذا (glyph_props & میل_props & LookupFlag::IgnoreFlags)
      أرجع خطأ;

    إذا (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      أرجع میل_properties_mark (info, glyph_props, میل_props);

    أرجع سچ;
  }

  فراغ سیٹ_glyph_class (hb_codepoint_t glyph_اشاریہ,
			  بنانشانی صحيح class_guess = 0,
			  منطقي ligature = خطأ,
			  منطقي component = خطأ)
  {
    بفر->digest.جوڑو (glyph_اشاریہ);

    إذا (new_syllables != (بنانشانی) -1)
      بفر->cur().syllable() = new_syllables;

    بنانشانی صحيح props = _hb_glyph_info_get_glyph_props (&بفر->cur());
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
      سیٹ_glyph_class_props (بفر->cur(), props, glyph_اشاریہ);
    }
    وإلا إذا (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      hb_glyph_info_سیٹ_glyph_props (&بفر->cur(), props | class_guess);
    }
    وإلا
      hb_glyph_info_سیٹ_glyph_props (&بفر->cur(), props);
  }
  فراغ سیٹ_glyph_class_props (hb_glyph_info_t &info,
			       بنانشانی صحيح props,
			       hb_codepoint_t glyph_اشاریہ)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    hb_glyph_info_سیٹ_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_اشاریہ));
  }
  فراغ سیٹ_glyph_class_props (hb_glyph_info_t &info)
  {
    سیٹ_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  فراغ replace_glyph (hb_codepoint_t glyph_اشاریہ)
  {
    سیٹ_glyph_class (glyph_اشاریہ);
    (فراغ) بفر->replace_glyph (glyph_اشاریہ);
  }
  فراغ replace_glyph_inplace (hb_codepoint_t glyph_اشاریہ)
  {
    سیٹ_glyph_class (glyph_اشاریہ);
    بفر->cur().codepoint = glyph_اشاریہ;
  }
  فراغ replace_glyph_with_ligature (hb_codepoint_t glyph_اشاریہ,
				    بنانشانی صحيح class_guess)
  {
    سیٹ_glyph_class (glyph_اشاریہ, class_guess, سچ);
    (فراغ) بفر->replace_glyph (glyph_اشاریہ);
  }
  فراغ output_glyph_for_component (hb_codepoint_t glyph_اشاریہ,
				   بنانشانی صحيح class_guess)
  {
    سیٹ_glyph_class (glyph_اشاریہ, class_guess, خطأ, سچ);
    (فراغ) بفر->output_glyph (glyph_اشاریہ);
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
  سانچا <قسم_ناں T>
  ساكن مضمن خودکار collect_دوجا_glyphs_2 (ثابت T &obj,
					     hb_سیٹ_digest_t *digest,
					     hb_ترجیح<1>) HB_RETURN (فراغ, obj.collect_دوجا_glyphs (digest) )
  سانچا <قسم_ناں T>
  ساكن مضمن فراغ collect_دوجا_glyphs_2 (ثابت T &obj HB_بن_ورتیا,
					     hb_سیٹ_digest_t *digest,
					     hb_ترجیح<0>)
  { *digest = hb_سیٹ_digest_t::بھریا (); }

  سانچا <قسم_ناں T>
  ساكن مضمن خودکار apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجیح<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  سانچا <قسم_ناں T>
  ساكن مضمن خودکار apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجیح<0>) HB_RETURN (منطقي, obj->apply (c) )
  سانچا <قسم_ناں T>
  ساكن مضمن منطقي apply_منزل (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
  سانچا <قسم_ناں T>
  ساكن مضمن خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجیح<2>) HB_RETURN (منطقي, obj->apply_cached (c, external_cache) )
  سانچا <قسم_ناں T>
  ساكن مضمن خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجیح<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  سانچا <قسم_ناں T>
  ساكن مضمن خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_ترجیح<0>) HB_RETURN (منطقي, obj->apply (c) )
  سانچا <قسم_ناں T>
  ساكن مضمن منطقي apply_cached_منزل (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  سانچا <قسم_ناں T>
  ساكن مضمن خودکار cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_ترجیح<1>) HB_RETURN (منطقي, T::cache_func (c, op) )
  سانچا <قسم_ناں T=فراغ>
  ساكن مضمن منطقي cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_بن_ورتیا,
				  hb_ترجیح<0>) { أرجع خطأ; }
  سانچا <قسم_ناں Type>
  ساكن مضمن منطقي cache_func_منزل (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    أرجع cache_func_<Type> (c, op, hb_prioritize);
  }
#جے_انت

  تعريفنوع منطقي (*hb_apply_func_t) (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache);
  تعريفنوع منطقي (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  بنية hb_applicable_t
  {
    یار بنية hb_accelerate_subtables_context_t;
    یار بنية hb_ot_layout_lookup_accelerator_t;

    سانچا <قسم_ناں T>
    فراغ init (ثابت T &obj_,
	       hb_apply_func_t apply_func_
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , فراغ *external_cache_
#جے_انت
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#جے_انت
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#جے_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    منطقي apply (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->بفر->cur().codepoint) && apply_func (obj, c, خالی_سوچک);
    }
    منطقي apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      أرجع apply_func (obj, c, خالی_سوچک);
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
#جے_انت

    خاص:
    ثابت فراغ *obj;
    hb_apply_func_t apply_func;
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    فراغ *external_cache;
#جے_انت
    hb_سیٹ_digest_t digest;
  };

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
  سانچا <قسم_ناں T>
  خودکار cache_لاگت (ثابت T &obj, hb_ترجیح<1>) HB_AUTO_RETURN ( obj.cache_لاگت () )
  سانچا <قسم_ناں T>
  خودکار cache_لاگت (ثابت T &obj, hb_ترجیح<0>) HB_AUTO_RETURN ( 0u )

  سانچا <قسم_ناں T>
  خودکار external_cache_create (ثابت T &obj, hb_ترجیح<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  سانچا <قسم_ناں T>
  خودکار external_cache_create (ثابت T &obj, hb_ترجیح<0>) HB_AUTO_RETURN ( خالی_سوچک )
#جے_انت

  /* Dispatch interface. */
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj)
  {
    collect_دوجا_glyphs_2 (obj, &digest_دوجا, hb_prioritize);

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    فراغ *external_cache = خالی_سوچک;
    إذا (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#جے_انت

    hb_applicable_t *اندراج = &array[i++];

    اندراج->init (obj,
		 apply_منزل<T>
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_منزل<T>
		 , cache_func_منزل<T>
		 , external_cache
#جے_انت
		 );

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    بنانشانی لاگت = cache_لاگت (obj, hb_prioritize);
    إذا (لاگت > subtable_cache_user_لاگت)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_لاگت = لاگت;
    }
#جے_انت

    أرجع hb_خالی_تھاواں_t ();
  }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  بنانشانی i = 0;
  hb_سیٹ_digest_t digest_دوجا;

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
  بنانشانی subtable_cache_user_idx = (بنانشانی) -1;
  بنانشانی subtable_cache_user_لاگت = 0;
#جے_انت
};


تعريفنوع منطقي (*intersects_func_t) (ثابت hb_سیٹ_t *glyphs, بنانشانی value, ثابت فراغ *انکڑے, فراغ *cache);
تعريفنوع فراغ (*intersected_glyphs_func_t) (ثابت hb_سیٹ_t *glyphs, ثابت فراغ *انکڑے, بنانشانی value, hb_سیٹ_t *intersected_glyphs, فراغ *cache);
تعريفنوع فراغ (*collect_glyphs_func_t) (hb_سیٹ_t *glyphs, بنانشانی value, ثابت فراغ *انکڑے);
تعريفنوع منطقي (*میل_func_t) (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے);
تعريفنوع بنانشانی (*get_قدر_func_t) (hb_glyph_info_t &info, ثابت فراغ *انکڑے);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
بنية hb_ot_layout_ruleset_digest_t
{
  فراغ init () { نقاب = 0; }
  فراغ سیٹ_بھریا () { نقاب = (uint64_t) -1; }
  فراغ جوڑو (بنانشانی value) { نقاب |= (uint64_t) 1 << (value & 63); }
  منطقي may_have (بنانشانی value) ثابت { أرجع نقاب & ((uint64_t) 1 << (value & 63)); }

  uint64_t نقاب;
};

بنية Context_گھیرا_Funcs
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
  میل_func_t میل;
};
بنية ChainContextApplyFuncs
{
  میل_func_t میل[3];
};


ساكن مضمن منطقي intersects_glyph (ثابت hb_سیٹ_t *glyphs, بنانشانی value, ثابت فراغ *انکڑے HB_بن_ورتیا, فراغ *cache HB_بن_ورتیا)
{
  أرجع glyphs->has (value);
}
ساكن مضمن منطقي intersects_class (ثابت hb_سیٹ_t *glyphs, بنانشانی value, ثابت فراغ *انکڑے, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(انکڑے);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  إذا (map->has (value, &cached_v))
    أرجع *cached_v;

  منطقي v = class_def.intersects_class (glyphs, value);
  map->سیٹ (value, v);

  أرجع v;
}
ساكن مضمن منطقي intersects_coverage (ثابت hb_سیٹ_t *glyphs, بنانشانی value, ثابت فراغ *انکڑے, فراغ *cache HB_بن_ورتیا)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  أرجع (انکڑے+coverage).intersects (glyphs);
}


ساكن مضمن فراغ intersected_glyph (ثابت hb_سیٹ_t *glyphs HB_بن_ورتیا, ثابت فراغ *انکڑے, بنانشانی value, hb_سیٹ_t *intersected_glyphs, HB_بن_ورتیا فراغ *cache)
{
  بنانشانی g = reinterpret_cast<ثابت HBUINT16 *>(انکڑے)[value];
  intersected_glyphs->جوڑو (g);
}

ورتوں intersected_class_cache_t = hb_hashmap_t<بنانشانی, hb_سیٹ_t>;

ساكن مضمن فراغ intersected_class_glyphs (ثابت hb_سیٹ_t *glyphs, ثابت فراغ *انکڑے, بنانشانی value, hb_سیٹ_t *intersected_glyphs, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(انکڑے);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_سیٹ_t *cached_v;
  إذا (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    أرجع;
  }

  hb_سیٹ_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->سیٹ (value, std::move (v));
}

ساكن مضمن فراغ intersected_coverage_glyphs (ثابت hb_سیٹ_t *glyphs, ثابت فراغ *انکڑے, بنانشانی value, hb_سیٹ_t *intersected_glyphs, HB_بن_ورتیا فراغ *cache)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  (انکڑے+coverage).intersect_سیٹ (*glyphs, *intersected_glyphs);
}


ساكن مضمن منطقي array_is_subset_دا (ثابت hb_سیٹ_t *glyphs,
				       بنانشانی صحيح گنتی,
				       glyph_انکڑے_t values,
				       intersects_func_t intersects_func,
				       ثابت فراغ *intersects_انکڑے,
				       فراغ *cache)
{
  لكل (بنانشانی i = 0; i < گنتی; i++)
    إذا (!intersects_func (glyphs, values[i], intersects_انکڑے, cache)) أرجع خطأ;
  أرجع سچ;
}


ساكن مضمن فراغ collect_glyph (hb_سیٹ_t *glyphs, بنانشانی value, ثابت فراغ *انکڑے HB_بن_ورتیا)
{
  glyphs->جوڑو (value);
}
ساكن مضمن فراغ collect_class (hb_سیٹ_t *glyphs, بنانشانی value, ثابت فراغ *انکڑے)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(انکڑے);
  class_def.collect_class (glyphs, value);
}
ساكن مضمن فراغ collect_coverage (hb_سیٹ_t *glyphs, بنانشانی value, ثابت فراغ *انکڑے)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  (انکڑے+coverage).collect_coverage (glyphs);
}

مضمن ثابت hb_سیٹ_t *
hb_depend_context_t::لبھو_or_create_glyph_سیٹ (ثابت فراغ *انکڑے,
                                                بنانشانی value,
                                                glyph_سیٹ_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_سیٹ_cache_کلید_t key (انکڑے, value, type);
  hb_codepoint_t *existing = خالی_سوچک;
  إذا (glyph_سیٹ_منزل_اشاریہ.has (key, &existing))
    أرجع glyph_sets[*existing].get ();

  hb_سیٹ_t *سیٹ = hb_object_create<hb_سیٹ_t> ();
  إذا (unlikely (!سیٹ))
  {
    depend_انکڑے->fail ();
    أرجع خالی_سوچک;
  }

  إذا (type == GLYPH_سیٹ_COVERAGE)
    collect_coverage (سیٹ, value, انکڑے);
  وإلا إذا (type == GLYPH_سیٹ_CLASS)
    collect_class (سیٹ, value, انکڑے);
  وإلا
    سیٹ->جوڑو (value);
  إذا (unlikely (سیٹ->وچ_error ()))
  {
    hb_سیٹ_destroy (سیٹ);
    depend_انکڑے->fail ();
    أرجع خالی_سوچک;
  }

  hb_codepoint_t اشاریہ_2 = glyph_sets.لمائی;
  إذا (unlikely (!glyph_sets.ڈالو_or_fail (hb::unique_ptr<hb_سیٹ_t> {سیٹ})))
  {
    depend_انکڑے->fail ();
    أرجع خالی_سوچک;
  }
  إذا (unlikely (!glyph_سیٹ_منزل_اشاریہ.سیٹ (key, اشاریہ_2)))
  {
    depend_انکڑے->fail ();
    أرجع خالی_سوچک;
  }
  أرجع glyph_sets[اشاریہ_2].get ();
}

مضمن hb_codepoint_t
hb_depend_context_t::لبھو_or_create_cached_context_سیٹ (ثابت hb_سیٹ_t *سیٹ)
{
  uintptr_t key = (uintptr_t) سیٹ;
  hb_codepoint_t *existing = خالی_سوچک;
  إذا (cached_context_سیٹ_indices.has (key, &existing))
    أرجع *existing;

  hb_codepoint_t اشاریہ_2 = depend_انکڑے->لبھو_or_create_context_سیٹ (*سیٹ);
  إذا (unlikely (اشاریہ_2 == HB_CODEPOINT_INVALID))
    أرجع HB_CODEPOINT_INVALID;
  إذا (unlikely (!cached_context_سیٹ_indices.سیٹ (key, اشاریہ_2)))
    أرجع depend_انکڑے->fail_invalid ();
  أرجع اشاریہ_2;
}

ساكن مضمن فراغ collect_array (hb_collect_glyphs_context_t *c HB_بن_ورتیا,
				  hb_سیٹ_t *glyphs,
				  بنانشانی صحيح گنتی,
				  glyph_انکڑے_t values,
				  collect_glyphs_func_t collect_func,
				  ثابت فراغ *collect_انکڑے)
{
  لكل (بنانشانی i = 0; i < گنتی; i++)
    collect_func (glyphs, values[i], collect_انکڑے);
}


ساكن مضمن منطقي میل_always (hb_glyph_info_t &info HB_بن_ورتیا, بنانشانی value HB_بن_ورتیا, ثابت فراغ *انکڑے HB_بن_ورتیا)
{
  أرجع سچ;
}
ساكن مضمن منطقي میل_glyph (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے HB_بن_ورتیا)
{
  أرجع info.codepoint == value;
}
ساكن مضمن منطقي میل_class (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(انکڑے);
  أرجع class_def.get_class (info.codepoint) == value;
}
ساكن مضمن بنانشانی get_class_قدر (hb_glyph_info_t &info, ثابت فراغ *انکڑے)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (انکڑے);
  أرجع class_def.get_class (info.codepoint);
}
بنية میل_class_cache_انکڑے_t
{
  ثابت ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ساكن مضمن منطقي میل_class_cache (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے)
{
  ثابت خودکار &cache_انکڑے = *reinterpret_cast<ثابت میل_class_cache_انکڑے_t *> (انکڑے);
  أرجع cache_انکڑے.class_def->get_class (info.codepoint, cache_انکڑے.cache) == value;
}
ساكن مضمن بنانشانی get_class_cache_قدر (hb_glyph_info_t &info, ثابت فراغ *انکڑے)
{
  ثابت خودکار &cache_انکڑے = *reinterpret_cast<ثابت میل_class_cache_انکڑے_t *> (انکڑے);
  أرجع cache_انکڑے.class_def->get_class (info.codepoint, cache_انکڑے.cache);
}
ساكن مضمن بنانشانی get_class_cached (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بنانشانی klass = info.syllable();
  إذا (klass < 255)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 255))
    info.syllable() = klass;
  أرجع klass;
}
ساكن مضمن منطقي میل_class_cached (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(انکڑے);
  أرجع get_class_cached (class_def, info) == value;
}
ساكن مضمن بنانشانی get_class_cached_قدر (hb_glyph_info_t &info, ثابت فراغ *انکڑے)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (انکڑے);
  أرجع get_class_cached (class_def, info);
}
ساكن مضمن بنانشانی get_class_cached1 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بنانشانی klass = info.syllable() & 0x0F;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  أرجع klass;
}
ساكن مضمن منطقي میل_class_cached1 (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(انکڑے);
  أرجع get_class_cached1 (class_def, info) == value;
}
ساكن مضمن بنانشانی get_class_cached2 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بنانشانی klass = (info.syllable() & 0xF0) >> 4;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  أرجع klass;
}
ساكن مضمن منطقي میل_class_cached2 (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(انکڑے);
  أرجع get_class_cached2 (class_def, info) == value;
}
ساكن مضمن بنانشانی get_class_cached2_قدر (hb_glyph_info_t &info, ثابت فراغ *انکڑے)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (انکڑے);
  أرجع get_class_cached2 (class_def, info);
}
ساكن مضمن منطقي میل_coverage (hb_glyph_info_t &info, بنانشانی value, ثابت فراغ *انکڑے)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  أرجع (انکڑے+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ساكن مضمن منطقي would_میل_داخلہ (hb_would_apply_context_t *c,
				      بنانشانی صحيح گنتی, /* Including the first glyph (not matched) */
				      glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
				      میل_func_t میل_func_2,
				      ثابت فراغ *میل_انکڑے_2)
{
  إذا (گنتی != c->len)
    أرجع خطأ;

  لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    إذا (likely (!میل_func_2 (info, داخلہ[i - 1], میل_انکڑے_2)))
      أرجع خطأ;
  }

  أرجع سچ;
}
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
HB_ALWAYS_INLINE
#وإلا
مضمن
#جے_انت
ساكن منطقي میل_داخلہ (hb_ot_apply_context_t *c,
			 بنانشانی صحيح گنتی, /* Including the first glyph (not matched) */
			 glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
			 میل_func_t میل_func_2,
			 ثابت فراغ *میل_انکڑے_2,
			 بنانشانی صحيح *سرا_position,
			 بنانشانی صحيح *p_کل_component_گنتی = خالی_سوچک)
{
  TRACE_APPLY (خالی_سوچک);

  hb_بفر_t *بفر = c->بفر;

  إذا (گنتی == 1)
  {
    *سرا_position = بفر->idx + 1;
    c->میل_positions[0] = بفر->idx;
    إذا (p_کل_component_گنتی)
      *p_کل_component_گنتی = hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
    return_trace (سچ);
  }

  إذا (unlikely (گنتی > HB_MAX_CONTEXT_لمائی)) return_trace (خطأ);

  خودکار &skippy_iter = c->iter_داخلہ;
  skippy_iter.reset (بفر->idx);
  skippy_iter.سیٹ_میل_func (میل_func_2, میل_انکڑے_2);
  skippy_iter.سیٹ_glyph_انکڑے (داخلہ);

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

  بنانشانی صحيح کل_component_گنتی = 0;

  بنانشانی صحيح پہلا_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
  بنانشانی صحيح پہلا_lig_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());

  تعداد {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
  {
    بنانشانی unsafe_منزل;
    إذا (!skippy_iter.next (&unsafe_منزل))
    {
      *سرا_position = unsafe_منزل;
      return_trace (خطأ);
    }

    إذا (unlikely (i + 1 > c->میل_positions.لمائی &&
		  !c->میل_positions.resize_dirty  (i + 1)))
      return_trace (خطأ);
    c->میل_positions.arrayZ[i] = skippy_iter.idx;

    بنانشانی صحيح this_lig_id = _hb_glyph_info_get_lig_id (&بفر->info[skippy_iter.idx]);
    بنانشانی صحيح this_lig_comp = _hb_glyph_info_get_lig_comp (&بفر->info[skippy_iter.idx]);

    إذا (پہلا_lig_id && پہلا_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      إذا (پہلا_lig_id != this_lig_id || پہلا_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	إذا (ligbase == LIGBASE_NOT_CHECKED)
	{
	  منطقي لبھیا = خطأ;
	  ثابت خودکار *باہرلا = بفر->باہرلا_info;
	  بنانشانی صحيح j = بفر->باہرلا_len;
	  طالما (j && _hb_glyph_info_get_lig_id (&باہرلا[j - 1]) == پہلا_lig_id)
	  {
	    إذا (_hb_glyph_info_get_lig_comp (&باہرلا[j - 1]) == 0)
	    {
	      j--;
	      لبھیا = سچ;
	      اكسر;
	    }
	    j--;
	  }

	  إذا (لبھیا && skippy_iter.may_skip (باہرلا[j]) == matcher_t::SKIP_YES)
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
      إذا (this_lig_id && this_lig_comp && (this_lig_id != پہلا_lig_id))
	return_trace (خطأ);
    }

    کل_component_گنتی += hb_glyph_info_get_lig_عدد_comps_وچ_ligation (&بفر->info[skippy_iter.idx]);
  }

  *سرا_position = skippy_iter.idx + 1;

  إذا (p_کل_component_گنتی)
  {
    کل_component_گنتی += hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
    *p_کل_component_گنتی = کل_component_گنتی;
  }

  c->میل_positions.arrayZ[0] = بفر->idx;

  return_trace (سچ);
}
ساكن مضمن منطقي ligate_داخلہ (hb_ot_apply_context_t *c,
				 بنانشانی صحيح گنتی, /* Including the first glyph */
				 بنانشانی صحيح میل_سرا,
				 hb_codepoint_t lig_glyph,
				 بنانشانی صحيح کل_component_گنتی)
{
  TRACE_APPLY (خالی_سوچک);

  hb_بفر_t *بفر = c->بفر;

  بفر->ملاؤ_clusters (بفر->idx, میل_سرا);

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

  منطقي is_ادھار_ligature = hb_glyph_info_is_ادھار_glyph (&بفر->info[c->میل_positions.arrayZ[0]]);
  منطقي is_mark_ligature = _hb_glyph_info_is_mark (&بفر->info[c->میل_positions.arrayZ[0]]);
  لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
    إذا (!_hb_glyph_info_is_mark (&بفر->info[c->میل_positions.arrayZ[i]]))
    {
      is_ادھار_ligature = خطأ;
      is_mark_ligature = خطأ;
      اكسر;
    }
  منطقي is_ligature = !is_ادھار_ligature && !is_mark_ligature;

  بنانشانی صحيح klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  بنانشانی صحيح lig_id = is_ligature ? _hb_allocate_lig_id (بفر) : 0;
  بنانشانی صحيح آخری_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
  بنانشانی صحيح آخری_عدد_components = hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
  بنانشانی صحيح components_so_far = آخری_عدد_components;

  إذا (is_ligature)
  {
    hb_glyph_info_سیٹ_lig_props_for_ligature (&بفر->cur(), lig_id, کل_component_گنتی);
    إذا (_hb_glyph_info_get_general_category (&بفر->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      hb_glyph_info_سیٹ_general_category (&بفر->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
  {
    طالما (بفر->idx < c->میل_positions.arrayZ[i] && بفر->successful)
    {
      إذا (is_ligature)
      {
	بنانشانی صحيح this_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());
	إذا (this_comp == 0)
	  this_comp = آخری_عدد_components;
	أكد (components_so_far >= آخری_عدد_components);
	بنانشانی صحيح new_lig_comp = components_so_far - آخری_عدد_components +
				    hb_min (this_comp, آخری_عدد_components);
	  hb_glyph_info_سیٹ_lig_props_for_mark (&بفر->cur(), lig_id, new_lig_comp);
      }
      (فراغ) بفر->اگلا_glyph ();
    }

    آخری_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
    آخری_عدد_components = hb_glyph_info_get_lig_عدد_comps_وچ_ligation (&بفر->cur());
    components_so_far += آخری_عدد_components;

    /* Skip the base glyph */
    بفر->idx++;
  }

  إذا (!is_mark_ligature && آخری_lig_id)
  {
    /* Re-adjust components for any marks following. */
    لكل (بنانشانی i = بفر->idx; i < بفر->len; ++i)
    {
      إذا (آخری_lig_id != _hb_glyph_info_get_lig_id (&بفر->info[i])) اكسر;

      بنانشانی this_comp = _hb_glyph_info_get_lig_comp (&بفر->info[i]);
      إذا (!this_comp) اكسر;

      أكد (components_so_far >= آخری_عدد_components);
      بنانشانی new_lig_comp = components_so_far - آخری_عدد_components +
			      hb_min (this_comp, آخری_عدد_components);
      hb_glyph_info_سیٹ_lig_props_for_mark (&بفر->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (سچ);
}

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
HB_ALWAYS_INLINE
#وإلا
مضمن
#جے_انت
ساكن منطقي میل_backtrack (hb_ot_apply_context_t *c,
			     بنانشانی صحيح گنتی,
			     glyph_انکڑے_t backtrack,
			     میل_func_t میل_func_2,
			     ثابت فراغ *میل_انکڑے_2,
			     بنانشانی صحيح *میل_شروع)
{
  TRACE_APPLY (خالی_سوچک);

  إذا (!گنتی)
  {
    *میل_شروع = c->بفر->backtrack_len ();
    return_trace (سچ);
  }

  خودکار &skippy_iter = c->iter_context;
  skippy_iter.reset_پٹھ (c->بفر->backtrack_len ());
  skippy_iter.سیٹ_میل_func (میل_func_2, میل_انکڑے_2);
  skippy_iter.سیٹ_glyph_انکڑے (backtrack);

  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
  {
    بنانشانی unsafe_from;
    إذا (!skippy_iter.prev (&unsafe_from))
    {
      *میل_شروع = unsafe_from;
      return_trace (خطأ);
    }
  }

  *میل_شروع = skippy_iter.idx;
  return_trace (سچ);
}

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
HB_ALWAYS_INLINE
#وإلا
مضمن
#جے_انت
ساكن منطقي میل_lookahead (hb_ot_apply_context_t *c,
			     بنانشانی صحيح گنتی,
			     glyph_انکڑے_t lookahead,
			     میل_func_t میل_func_2,
			     ثابت فراغ *میل_انکڑے_2,
			     بنانشانی صحيح شروع_اشاریہ,
			     بنانشانی صحيح *سرا_اشاریہ)
{
  TRACE_APPLY (خالی_سوچک);

  إذا (!گنتی)
  {
    *سرا_اشاریہ = شروع_اشاریہ;
    return_trace (سچ);
  }

  خودکار &skippy_iter = c->iter_context;
  أكد (شروع_اشاریہ >= 1);
  skippy_iter.reset (شروع_اشاریہ - 1);
  skippy_iter.سیٹ_میل_func (میل_func_2, میل_انکڑے_2);
  skippy_iter.سیٹ_glyph_انکڑے (lookahead);

  لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
  {
    بنانشانی unsafe_منزل;
    إذا (!skippy_iter.next (&unsafe_منزل))
    {
      *سرا_اشاریہ = unsafe_منزل;
      return_trace (خطأ);
    }
  }

  *سرا_اشاریہ = skippy_iter.idx + 1;
  return_trace (سچ);
}



بنية Lookup_ریکارڈ
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t         *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->embed (*هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    return_trace (c->check_سونپو (باہرلا->lookup_List_اشاریہ, lookup_map->get (lookup_List_اشاریہ), HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	sequence_اشاریہ;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookup_List_اشاریہ;	/* Lookup to apply to that
					 * position--zero--based */
  عام:
  DEFINE_ناپ_STATIC (4);
};

ساكن مضمن بنانشانی serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ثابت hb_array_t<ثابت Lookup_ریکارڈ> lookup_ریکارڈاں,
						     ثابت hb_map_t *lookup_map)
{
  بنانشانی گنتی = 0;
  لكل (ثابت Lookup_ریکارڈ& r : lookup_ریکارڈاں)
  {
    إذا (!lookup_map->has (r.lookup_List_اشاریہ))
      استمر;

    إذا (!r.serialize (c, lookup_map))
      أرجع 0;

    گنتی++;
  }
  أرجع گنتی;
}

تعداد ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ساكن مضمن ثابت hb_سیٹ_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ثابت فراغ *انکڑے,
			بنانشانی value,
			منطقي filter_class_by_ماپا,
			hb_سیٹ_t *scratch,
			hb_سمتیہ_t<بنانشانی> *compact_workspace = خالی_سوچک)
{
  بدل (context_format)
  {
    حالة ContextFormat::SimpleContext:
      أرجع c->لبھو_or_create_glyph_سیٹ (خالی_سوچک, value,
                                           hb_depend_context_t::GLYPH_سیٹ_SINGLE);
    حالة ContextFormat::ClassBasedContext:
    {
      ثابت hb_سیٹ_t *class_سیٹ = c->لبھو_or_create_glyph_سیٹ (
        انکڑے, value, hb_depend_context_t::GLYPH_سیٹ_CLASS);
      إذا (unlikely (!class_سیٹ))
	أرجع خالی_سوچک;
      إذا (!filter_class_by_ماپا)
	أرجع class_سیٹ;
      scratch->سیٹ (*class_سیٹ);
      scratch->intersect (c->ماپا_سرگرم_glyphs (), compact_workspace);
      اكسر;
    }
    حالة ContextFormat::CoverageBasedContext:
      أرجع c->لبھو_or_create_glyph_سیٹ (
        انکڑے, value, hb_depend_context_t::GLYPH_سیٹ_COVERAGE);
  }

  إذا (unlikely (scratch->وچ_error ()))
  {
    c->depend_انکڑے->fail ();
    أرجع خالی_سوچک;
  }
  أرجع scratch;
}

ساكن مضمن فراغ context_گھیرا_recurse_lookups (hb_گھیرا_context_t *c,
					     بنانشانی داخلہ_گنتی, glyph_انکڑے_t داخلہ,
					     بنانشانی lookup_گنتی_2,
					     ثابت Lookup_ریکارڈ lookup_ریکارڈ[] /* Array of LookupRecords--in design order */,
					     بنانشانی value,
					     ContextFormat context_format,
					     ثابت فراغ *انکڑے,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache)
{
  hb_سیٹ_t covered_seq_indicies;
  hb_سیٹ_t pos_glyphs;
  لكل (بنانشانی صحيح i = 0; i < lookup_گنتی_2; i++)
  {
    بنانشانی seq_اشاریہ_2 = lookup_ریکارڈ[i].sequence_اشاریہ;
    إذا (seq_اشاریہ_2 >= داخلہ_گنتی) استمر;

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indicies.has (seq_اشاریہ_2))
    {
      has_pos_glyphs = سچ;
      pos_glyphs.clear ();
      إذا (seq_اشاریہ_2 == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.جوڑو (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->ماپا_سرگرم_glyphs (), انکڑے, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.سیٹ (c->ماپا_سرگرم_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *داخلہ_انکڑے = داخلہ.انکڑے;
        بنانشانی داخلہ_قدر = seq_اشاریہ_2 - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          داخلہ_انکڑے = انکڑے;
          داخلہ_قدر = داخلہ[seq_اشاریہ_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, داخلہ_انکڑے, داخلہ_قدر, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.جوڑو (seq_اشاریہ_2);
    hb_سیٹ_t *cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs))
      أرجع;
    إذا (has_pos_glyphs) {
      *cur_سرگرم_glyphs = std::move (pos_glyphs);
    } وإلا {
      *cur_سرگرم_glyphs = *c->glyphs;
    }

    بنانشانی سرا_اشاریہ_2 = داخلہ_گنتی;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      سرا_اشاریہ_2 += 1;

    c->recurse (lookup_ریکارڈ[i].lookup_List_اشاریہ, &covered_seq_indicies, seq_اشاریہ_2, سرا_اشاریہ_2);

    c->نکالو_cur_پورا_glyphs ();
  }
}

ساكن منطقي
context_depend_sets_are_cached (hb_depend_context_t *c,
				بنانشانی داخلہ_گنتی_2,
				بنانشانی lookup_گنتی,
				ثابت Lookup_ریکارڈ lookup_ریکارڈاں_2[],
				ثابت فراغ *قاعدہ_2,
				بنانشانی value,
				ContextFormat context_format,
				ثابت فراغ *انکڑے,
				hb_سمتیہ_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t سرگرم_idx = HB_CODEPOINT_INVALID;
  منطقي have_سرگرم_idx = خطأ;
  لكل (بنانشانی i = 0; i < lookup_گنتی; i++)
  {
    إذا (lookup_ریکارڈاں_2[i].sequence_اشاریہ >= داخلہ_گنتی_2)
      استمر;

    إذا (!have_سرگرم_idx)
    {
      إذا (unlikely (!c->get_ماپا_سرگرم_glyphs_اشاریہ (&سرگرم_idx)))
	أرجع خطأ;
      have_سرگرم_idx = سچ;
    }

    hb_depend_context_t::context_سیٹ_cache_کلید_t key (
      قاعدہ_2, &lookup_ریکارڈاں_2[i], انکڑے, سرگرم_idx, value, context_format);
    hb_codepoint_t *cached_context_سیٹ = خالی_سوچک;
    إذا (!c->context_سیٹ_cache.has (key, &cached_context_سیٹ))
      أرجع خطأ;
    إذا (unlikely (!cached_context_sets->ڈالو_or_fail (*cached_context_سیٹ)))
      أرجع c->depend_انکڑے->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  أرجع have_سرگرم_idx;
}

ساكن مضمن فراغ context_depend_recurse_lookups (hb_depend_context_t *c,
					     بنانشانی داخلہ_گنتی, glyph_انکڑے_t داخلہ,
					     بنانشانی lookup_گنتی_2,
					     ثابت Lookup_ریکارڈ lookup_ریکارڈ[] /* Array of LookupRecords--in design order */,
					     ثابت فراغ *قاعدہ_2,
					     بنانشانی value,
					     ContextFormat context_format,
					     ثابت فراغ *انکڑے,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache,
					     ثابت hb_سیٹ_t &preliminary_context,
					     ثابت hb_سمتیہ_t<ثابت hb_سیٹ_t *> *داخلہ_position_glyphs,
					     ثابت hb_سمتیہ_t<hb_codepoint_t> *cached_context_sets,
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
  إذا (unlikely (داخلہ_گنتی > HB_MAX_CONTEXT_لمائی))
    أرجع;

  hb_ثنائی_ہندسہ_ورقہ_t covered_seq_indices;
  hb_سیٹ_t &pos_glyphs = scratch->pos_glyphs;
  hb_سیٹ_t &position_context = scratch->position_context;
  hb_سیٹ_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_سیٹ_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  بنانشانی cached_context_اشاریہ = 0;
  لكل (بنانشانی صحيح i = 0; i < lookup_گنتی_2; i++)
  {
    بنانشانی seq_اشاریہ_2 = lookup_ریکارڈ[i].sequence_اشاریہ;
    إذا (seq_اشاریہ_2 >= داخلہ_گنتی)
      استمر;

    hb_depend_context_t::context_سیٹ_cache_کلید_t context_cache_کلید;
    إذا (unlikely (!c->get_context_سیٹ_cache_کلید (قاعدہ_2, &lookup_ریکارڈ[i], انکڑے, value,
						 context_format, &context_cache_کلید)))
      أرجع;

    hb_codepoint_t context_سیٹ_idx;
    hb_codepoint_t *cached_context_سیٹ_idx;
    إذا (cached_context_sets)
      context_سیٹ_idx = (*cached_context_sets)[cached_context_اشاریہ++];
    وإلا إذا (c->context_سیٹ_cache.has (context_cache_کلید, &cached_context_سیٹ_idx))
      context_سیٹ_idx = *cached_context_سیٹ_idx;
    وإلا
    {
      إذا (unlikely (!داخلہ_position_glyphs))
      {
	c->depend_انکڑے->fail ();
	أرجع;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_سیٹ_قدر_INVALID;
      طالما (preliminary_context.next (&elem)) {
	إذا (!(elem & HB_DEPEND_CONTEXT_سیٹ_FLAG)) {
	  /* Direct glyph */
	  position_context.جوڑو (elem);
	} وإلا {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.جوڑو (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      لكل (بنانشانی j = 0; j < داخلہ_position_glyphs->لمائی; j++) {
	إذا (j != seq_اشاریہ_2 && (*داخلہ_position_glyphs)[j]->get_population () == 1) {
	  position_context.جوڑو ((*داخلہ_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_سیٹ_قدر_INVALID;
      طالما (disjunctive_indices.next (&elem)) {
	hb_codepoint_t سیٹ_idx = elem & 0x7FFFFFFF;
	ثابت hb_سیٹ_t *original_سیٹ = c->depend_انکڑے->get_سیٹ_from_اشاریہ (سیٹ_idx);
	إذا (unlikely (!original_سیٹ)) استمر;

	/* Dependency context sets are never inverted. */
	إذا (!original_سیٹ->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.جوڑو (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      لكل (بنانشانی j = 0; j < داخلہ_position_glyphs->لمائی; j++) {
	إذا (j != seq_اشاریہ_2 && (*داخلہ_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  إذا (!(*داخلہ_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    منطقي cached_سیٹ = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_سیٹ
	      ? c->لبھو_or_create_cached_context_سیٹ ((*داخلہ_position_glyphs)[j])
	      : c->depend_انکڑے->لبھو_or_create_context_سیٹ (*(*داخلہ_position_glyphs)[j]);
	    إذا (unlikely (idx == HB_CODEPOINT_INVALID))
	      أرجع;
	    filtered_disjunctive_indices.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      إذا (unlikely (position_context.وچ_error () ||
		    filtered_disjunctive_indices.وچ_error ()))
      {
	c->depend_انکڑے->fail ();
	أرجع;
      }

      context_سیٹ_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_انکڑے->لبھو_or_create_context_سیٹ (position_context);
      إذا (unlikely (!position_context.is_empty () &&
		    context_سیٹ_idx == HB_CODEPOINT_INVALID))
	أرجع;

      إذا (unlikely (!c->context_سیٹ_cache.سیٹ (context_cache_کلید, context_سیٹ_idx)))
      {
	c->depend_انکڑے->fail ();
	أرجع;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_سیٹ_اشاریہ = c->depend_انکڑے->current_context_سیٹ_اشاریہ;
    hb_subset_depend_کنارا_flags_t saved_کنارا_flags = c->depend_انکڑے->current_کنارا_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    منطقي nested_contextual = (saved_context_سیٹ_اشاریہ != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_انکڑے->current_context_سیٹ_اشاریہ = context_سیٹ_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_انکڑے->current_کنارا_flags = HB_SUBSET_DEPEND_کنارا_FLAG_NONE;
    إذا (داخلہ_گنتی > 1) {
      c->depend_انکڑے->current_کنارا_flags |= HB_SUBSET_DEPEND_کنارا_FLAG_FROM_CONTEXT_POSITION;
    }
    إذا (nested_contextual) {
      c->depend_انکڑے->current_کنارا_flags |= HB_SUBSET_DEPEND_کنارا_FLAG_FROM_NESTED_CONTEXT;
    }

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indices.has (seq_اشاریہ_2))
    {
      has_pos_glyphs = سچ;
      pos_glyphs.clear ();
      إذا (seq_اشاریہ_2 == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.جوڑو (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->ماپا_سرگرم_glyphs (), انکڑے, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.سیٹ (c->ماپا_سرگرم_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *داخلہ_انکڑے = داخلہ.انکڑے;
        بنانشانی داخلہ_قدر = seq_اشاریہ_2 - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          داخلہ_انکڑے = انکڑے;
          داخلہ_قدر = داخلہ[seq_اشاریہ_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, داخلہ_انکڑے, داخلہ_قدر, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.جوڑو (seq_اشاریہ_2);
    إذا (has_pos_glyphs)
    {
      hb_codepoint_t سرگرم_idx = HB_CODEPOINT_INVALID;
      إذا (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_اشاریہ_2 ? pos_glyphs.get_min () : value;
	إذا (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_سرگرم_FLAG))
	  سرگرم_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_سرگرم_FLAG;
      }
      hb_سیٹ_t *cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs (سرگرم_idx);
      إذا (unlikely (!cur_سرگرم_glyphs))
      {
	c->depend_انکڑے->current_context_سیٹ_اشاریہ = saved_context_سیٹ_اشاریہ;
	c->depend_انکڑے->current_کنارا_flags = saved_کنارا_flags;
	أرجع;
      }
      *cur_سرگرم_glyphs = std::move (pos_glyphs);
    }
    وإلا إذا (unlikely (!c->ڈالو_جڑ_سرگرم_glyphs ()))
    {
      c->depend_انکڑے->current_context_سیٹ_اشاریہ = saved_context_سیٹ_اشاریہ;
      c->depend_انکڑے->current_کنارا_flags = saved_کنارا_flags;
      أرجع;
    }

    بنانشانی سرا_اشاریہ_2 = داخلہ_گنتی;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      سرا_اشاریہ_2 += 1;

    c->recurse (lookup_ریکارڈ[i].lookup_List_اشاریہ, &covered_seq_indices, seq_اشاریہ_2, سرا_اشاریہ_2);
    c->نکالو_cur_پورا_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_انکڑے->current_context_سیٹ_اشاریہ = saved_context_سیٹ_اشاریہ;
    c->depend_انکڑے->current_کنارا_flags = saved_کنارا_flags;
  }
}

سانچا <قسم_ناں context_t>
ساكن مضمن فراغ recurse_lookups (context_t *c,
                                    بنانشانی صحيح lookup_گنتی_2,
                                    ثابت Lookup_ریکارڈ lookup_ریکارڈ[] /* Array of LookupRecords--in design order */)
{
  لكل (بنانشانی صحيح i = 0; i < lookup_گنتی_2; i++)
    c->recurse (lookup_ریکارڈ[i].lookup_List_اشاریہ);
}

ساكن مضمن فراغ apply_lookup (hb_ot_apply_context_t *c,
				 بنانشانی صحيح گنتی, /* Including the first glyph */
				 بنانشانی صحيح lookup_گنتی_2,
				 ثابت Lookup_ریکارڈ lookup_ریکارڈ[], /* Array of LookupRecords--in design order */
				 بنانشانی صحيح میل_سرا)
{
  hb_بفر_t *بفر = c->بفر;
  صحيح سرا;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    بنانشانی صحيح bl = بفر->backtrack_len ();
    سرا = bl + میل_سرا - بفر->idx;

    صحيح delta = bl - بفر->idx;
    /* Convert positions to new indexing. */
    لكل (بنانشانی صحيح j = 0; j < گنتی; j++)
      c->میل_positions.arrayZ[j] += delta;
  }

  لكل (بنانشانی صحيح i = 0; i < lookup_گنتی_2 && بفر->successful; i++)
  {
    بنانشانی صحيح idx = lookup_ریکارڈ[i].sequence_اشاریہ;
    إذا (idx >= گنتی)
      استمر;

    بنانشانی صحيح orig_len = بفر->backtrack_len () + بفر->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    إذا (unlikely (c->میل_positions.arrayZ[idx] >= orig_len))
      استمر;

    إذا (unlikely (!بفر->move_منزل (c->میل_positions.arrayZ[idx])))
      اكسر;

    إذا (unlikely (بفر->max_ops <= 0))
      اكسر;

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      إذا (بفر->have_output)
        c->بفر->sync_so_far ();
      c->بفر->سنیہا (c->font,
			  "start recursing to lookup %u at %u",
			  (بنانشانی) lookup_ریکارڈ[i].lookup_List_اشاریہ,
			  بفر->idx);
    }

    منطقي ret = c->recurse (lookup_ریکارڈ[i].lookup_List_اشاریہ);

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      إذا (بفر->have_output)
        c->بفر->sync_so_far ();
      c->بفر->سنیہا (c->font,
			  "end recursing to lookup %u",
			  (بنانشانی) lookup_ریکارڈ[i].lookup_List_اشاریہ);
    }

    إذا (!ret)
      استمر;

    بنانشانی صحيح new_len = بفر->backtrack_len () + بفر->lookahead_len ();
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

    سرا += delta;
    إذا (سرا < صحيح (c->میل_positions.arrayZ[idx]))
    {
      /* End might end up being smaller than match_positions.arrayZ[idx] if the recursed
       * lookup ended up removing many items.
       * Just never rewind end beyond start of current position, since that is
       * not possible in the recursed lookup.  Also adjust delta as such.
       *
       * https://bugs.chromium.org/p/chromium/issues/detail?id=659496
       * https://github.com/harfbuzz/harfbuzz/issues/1611
       */
      delta += c->میل_positions.arrayZ[idx] - سرا;
      سرا = c->میل_positions.arrayZ[idx];
    }

    بنانشانی صحيح next = idx + 1; /* next now is the position after the recursed lookup. */

    إذا (delta > 0)
    {
      إذا (unlikely (delta + گنتی > HB_MAX_CONTEXT_لمائی))
	اكسر;
      إذا (unlikely (گنتی + delta > c->میل_positions.لمائی &&
		    !c->میل_positions.resize_dirty  (گنتی + delta)))
        أرجع;
    }
    وإلا
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (صحيح) next - (صحيح) گنتی);
      next -= delta;
    }

    /* Shift! */
    memmove (c->میل_positions + next + delta, c->میل_positions + next,
	     (گنتی - next) * حجم (c->میل_positions.arrayZ[0]));
    next += delta;
    گنتی += delta;

    /* Fill in new entries. */
    لكل (بنانشانی صحيح j = idx + 1; j < next; j++)
      c->میل_positions.arrayZ[j] = c->میل_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    لكل (; next < گنتی; next++)
      c->میل_positions.arrayZ[next] += delta;
  }

  أكد (سرا >= 0);
  (فراغ) بفر->move_منزل (سرا);
}



/* Contextual lookups */

بنية Context_گھیرا_Lookup_Context
{
  Context_گھیرا_Funcs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_انکڑے;
  فراغ *intersects_cache;
  فراغ *intersected_glyphs_cache;
};

تعريفنوع Context_گھیرا_Lookup_Context ContextDependLookupContext;

بنية ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_انکڑے;
};

بنية ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ثابت فراغ *میل_انکڑے_2;
  get_قدر_func_t get_قدر;
};

ساكن مضمن منطقي context_intersects (ثابت hb_سیٹ_t *glyphs,
				       بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
				       glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
				       Context_گھیرا_Lookup_Context &lookup_context)
{
  أرجع array_is_subset_دا (glyphs,
			     داخلہ_گنتی ? داخلہ_گنتی - 1 : 0, داخلہ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_انکڑے,
			     lookup_context.intersects_cache);
}

ساكن مضمن فراغ context_گھیرا_lookup (hb_گھیرا_context_t *c,
					   بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
					   glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
					   بنانشانی صحيح lookup_گنتی_2,
					   ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
					   بنانشانی value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   Context_گھیرا_Lookup_Context &lookup_context)
{
  إذا (context_intersects (c->glyphs,
			  داخلہ_گنتی, داخلہ,
			  lookup_context))
    context_گھیرا_recurse_lookups (c,
				     داخلہ_گنتی, داخلہ,
				     lookup_گنتی_2, lookup_ریکارڈ,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_انکڑے,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ context_depend_lookup (hb_depend_context_t *c,
					  بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
					  glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
					  بنانشانی صحيح lookup_گنتی_2,
					  ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
					  ثابت فراغ *قاعدہ_2,
					  بنانشانی value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  إذا (!context_intersects (c->glyphs,
			   داخلہ_گنتی, داخلہ,
			   lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, داخلہ_گنتی, lookup_گنتی_2, lookup_ریکارڈ, قاعدہ_2, value,
    lookup_context.context_format, lookup_context.intersects_انکڑے,
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_انکڑے->successful))
    أرجع;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_سیٹ_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_سیٹ_t &position_scratch = scratch->position_scratch;
  hb_سمتیہ_t<ثابت hb_سیٹ_t *> &داخلہ_position_glyphs = scratch->داخلہ_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  إذا (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ثابت hb_سیٹ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_انکڑے, value,
      سچ, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !داخلہ_position_glyphs.ڈالو_or_fail (pos0_glyphs)))
    {
      c->depend_انکڑے->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (بنانشانی i = 0; i + 1 < داخلہ_گنتی; i++)
    {
      ثابت hb_سیٹ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_انکڑے,
	داخلہ[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !داخلہ_position_glyphs.ڈالو_or_fail (pos_glyphs)))
      {
	c->depend_انکڑے->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  داخلہ_گنتی, داخلہ,
				  lookup_گنتی_2, lookup_ریکارڈ,
				  قاعدہ_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_انکڑے,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? خالی_سوچک : &داخلہ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : خالی_سوچک,
				  scratch);
}

ساكن مضمن فراغ context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
						  glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
						  بنانشانی صحيح lookup_گنتی_2,
						  ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->داخلہ,
		 داخلہ_گنتی ? داخلہ_گنتی - 1 : 0, داخلہ,
		 lookup_context.funcs.collect, lookup_context.collect_انکڑے);
  recurse_lookups (c,
		   lookup_گنتی_2, lookup_ریکارڈ);
}

ساكن مضمن منطقي context_would_apply_lookup (hb_would_apply_context_t *c,
					       بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
					       glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
					       بنانشانی صحيح lookup_گنتی_2 HB_بن_ورتیا,
					       ثابت Lookup_ریکارڈ lookup_ریکارڈ[] HB_بن_ورتیا,
					       ثابت ContextApplyLookupContext &lookup_context)
{
  أرجع would_میل_داخلہ (c,
			    داخلہ_گنتی, داخلہ,
			    lookup_context.funcs.میل, lookup_context.میل_انکڑے_2);
}

ساكن مضمن منطقي context_apply_lookup (hb_ot_apply_context_t *c,
					 بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
					 glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
					 بنانشانی صحيح lookup_گنتی_2,
					 ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
					 ثابت ContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (داخلہ_گنتی > HB_MAX_CONTEXT_لمائی)) أرجع خطأ;

  بنانشانی میل_سرا = 0;
  منطقي ret = خطأ;
  إذا (میل_داخلہ (c,
		   داخلہ_گنتی, داخلہ,
		   lookup_context.funcs.میل, lookup_context.میل_انکڑے_2,
		   &میل_سرا))
  {
    c->بفر->unsafe_منزل_break (c->بفر->idx, میل_سرا);
    apply_lookup (c,
		  داخلہ_گنتی,
		  lookup_گنتی_2, lookup_ریکارڈ,
		  میل_سرا);
    ret = سچ;
  }
  وإلا
  {
    c->بفر->unsafe_منزل_concat (c->بفر->idx, میل_سرا);
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
      خودکار &info = c->بفر->info;
      بنانشانی گنتی = c->بفر->len;
      لكل (بنانشانی i = 0; i < گنتی; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      أرجع سچ;
    }
    حالة hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (بنانشانی) -1;
      HB_بفر_DEALLOCATE_VAR (c->بفر, syllable);
      اكسر;
    }
  }
  أرجع خطأ;
}

سانچا <قسم_ناں Types>
بنية قاعدہ_3
{
  سانچا <قسم_ناں سیٹ_Types, قسم_ناں قاعدہ_Type>
  یار بنية قاعدہ_سیٹ_دا;

  منطقي intersects (ثابت hb_سیٹ_t *glyphs, Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    أرجع context_intersects (glyphs,
			       داخلہ_گنتی, داخلہ_Z.arrayZ,
			       lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, بنانشانی value, ContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_دا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array ((داخلہ_گنتی ? داخلہ_گنتی - 1 : 0)));
    context_depend_lookup (c,
			   داخلہ_گنتی, داخلہ_Z.arrayZ,
			   lookup_گنتی_2, lookup_ریکارڈ.arrayZ,
			   هذا,
			   value, lookup_context);
  }

  فراغ گھیرا (hb_گھیرا_context_t *c, بنانشانی value, Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندی_exceeded ())) أرجع;

    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_دا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array ((داخلہ_گنتی ? داخلہ_گنتی - 1 : 0)));
    context_گھیرا_lookup (c,
			    داخلہ_گنتی, داخلہ_Z.arrayZ,
			    lookup_گنتی_2, lookup_ریکارڈ.arrayZ,
			    value, lookup_context);
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c,
                        Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندی_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_دا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0));
    recurse_lookups (c, lookup_گنتی_2, lookup_ریکارڈ.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_دا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0));
    context_collect_glyphs_lookup (c,
				   داخلہ_گنتی, داخلہ_Z.arrayZ,
				   lookup_گنتی_2, lookup_ریکارڈ.arrayZ,
				   lookup_context);
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest,
			      ثابت ClassDef *class_def = خالی_سوچک) ثابت
  {
    إذا (داخلہ_گنتی <= 1)
    {
      *digest = hb_سیٹ_digest_t::بھریا ();
      أرجع;
    }

    بنانشانی second = داخلہ_Z.arrayZ[0];
    إذا (!class_def)
      digest->جوڑو (second);
    وإلا إذا (!second)
      *digest = hb_سیٹ_digest_t::بھریا ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_دا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0));
    أرجع context_would_apply_lookup (c,
				       داخلہ_گنتی, داخلہ_Z.arrayZ,
				       lookup_گنتی_2, lookup_ریکارڈ.arrayZ,
				       lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_دا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0));
    return_trace (context_apply_lookup (c, داخلہ_گنتی, داخلہ_Z.arrayZ, lookup_گنتی_2, lookup_ریکارڈ.arrayZ, lookup_context));
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *داخلہ_mapping, /* old->new glyphid or class mapping */
		  ثابت hb_map_t *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->شروع_embed (هذا);
    إذا (unlikely (!c->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->داخلہ_گنتی = داخلہ_گنتی;
    ثابت خودکار داخلہ = داخلہ_Z.as_array (داخلہ_گنتی - 1);
    لكل (ثابت خودکار org : داخلہ)
    {
      قسم_ناں Types::HBUINT d;
      d = داخلہ_mapping->get (org);
      c->copy (d);
    }

    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_دا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array ((داخلہ_گنتی ? داخلہ_گنتی - 1 : 0)));

    بنانشانی گنتی = serialize_lookuprecord_array (c, lookup_ریکارڈ.as_array (lookup_گنتی_2), lookup_map);
    return_trace (c->check_سونپو (باہرلا->lookup_گنتی_2, گنتی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!داخلہ_گنتی)) return_trace (خطأ);
    ثابت خودکار داخلہ = داخلہ_Z.as_array (داخلہ_گنتی - 1);

    ثابت hb_map_t *mapping = klass_map == خالی_سوچک ? c->plan->glyph_map : klass_map;
    إذا (!hb_all (داخلہ, mapping)) return_trace (خطأ);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (داخلہ_Z.arrayZ,
				  داخلہ_Z.شے_ناپ * (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0) +
				  Lookup_ریکارڈ::static_ناپ * lookup_گنتی_2));
  }

  محمي:
  HBUINT16	داخلہ_گنتی;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_گنتی_2;		/* Number of LookupRecords */
  Unsized_Array_دا<قسم_ناں Types::HBUINT>
		داخلہ_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_ناپ_ARRAY (4, داخلہ_Z);
};

سانچا <قسم_ناں Types, قسم_ناں قاعدہ_Type>
بنية قاعدہ_سیٹ_دا
{
  منطقي intersects (ثابت hb_سیٹ_t *glyphs,
		   Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([&] (ثابت قاعدہ_Type &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_پہلا_داخلہ_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (بنانشانی i = 0; i < قاعدہ_2.len; i++)
    {
      ثابت قاعدہ_Type &r = هذا+قاعدہ_2.arrayZ[i];
      إذا (r.داخلہ_گنتی <= 1)
      {
	digest->سیٹ_بھریا ();
	أرجع;
      }
      digest->جوڑو (r.داخلہ_Z.arrayZ[0]);
    }
  }

  فراغ depend (hb_depend_context_t *c, بنانشانی value, ContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت قاعدہ_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }

  فراغ گھیرا (hb_گھیرا_context_t *c, بنانشانی value,
		Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندی_exceeded ())) أرجع;

    أرجع
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت قاعدہ_Type &_) { _.گھیرا (c, value, lookup_context); })
    ;
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c,
                        Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندی_exceeded ())) أرجع;
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت قاعدہ_Type &_) { _.گھیرا_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت قاعدہ_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest,
			      ثابت ClassDef *class_def = خالی_سوچک) ثابت
  {
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([=] (ثابت قاعدہ_Type &_) { _.collect_دوجا_glyphs (digest, class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([&] (ثابت قاعدہ_Type &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = خالی_سوچک) ثابت
  {
    TRACE_APPLY (هذا);

    بنانشانی عدد_قاعدے = قاعدہ_2.len;

#جے_بنا_تعریف HB_NO_OT_RULESETS_FAST_رستہ
    إذا (HB_OPTIMIZE_ناپ_VAL || عدد_قاعدے <= 4)
#جے_انت
    {
    slow:
      return_trace (
      + hb_iter (قاعدہ_2)
      | hb_map (hb_جوڑو (هذا))
      | hb_map ([&] (ثابت قاعدہ_Type &_) { أرجع _.apply (c, lookup_context); })
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
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سیٹ_میل_func (میل_always, خالی_سوچک);
    skippy_iter.سیٹ_glyph_انکڑے ((HBUINT16 *) خالی_سوچک);
    بنانشانی unsafe_منزل = (بنانشانی) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *پہلا = خالی_سوچک, *second = خالی_سوچک;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      پہلا = &c->بفر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_قدر &&
	  !digest->may_have (lookup_context.get_قدر (*پہلا, lookup_context.میل_انکڑے_2)))
	{
	  c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_منزل_concat = خطأ;
      منطقي ret =
      + hb_iter (قاعدہ_2)
      | hb_map (hb_جوڑو (هذا))
      | hb_filter ([&] (ثابت قاعدہ_Type &_)
		   {
		     إذا (_.داخلہ_گنتی <= 1) أرجع سچ;
		     unsafe_منزل_concat = سچ;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت قاعدہ_Type &_) { أرجع _.apply (c, lookup_context); })
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

    خودکار میل_داخلہ = lookup_context.funcs.میل;
    خودکار *داخلہ_انکڑے = lookup_context.میل_انکڑے_2;
    لكل (بنانشانی صحيح i = 0; i < عدد_قاعدے; i++)
    {
      ثابت خودکار &r = هذا+قاعدہ_2.arrayZ[i];

      ثابت خودکار &داخلہ = r.داخلہ_Z;

      إذا (r.داخلہ_گنتی <= 1 ||
	  (!میل_داخلہ ||
	   میل_داخلہ (*پہلا, داخلہ.arrayZ[0], داخلہ_انکڑے)))
      {
        إذا (!second ||
	    (r.داخلہ_گنتی <= 2 ||
	     (!میل_داخلہ ||
	      میل_داخلہ (*second, داخلہ.arrayZ[1], داخلہ_انکڑے)))
	   )
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_منزل != (بنانشانی) -1)
	      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
	    return_trace (سچ);
	  }
	}
	وإلا
	  unsafe_منزل = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_منزل == (بنانشانی) -1)
	  unsafe_منزل = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	لكل (; i + 1 < عدد_قاعدے; i++)
	{
	  ثابت خودکار &r2 = هذا+قاعدہ_2.arrayZ[i + 1];
	  ثابت خودکار &input2 = r2.داخلہ_Z;
	  إذا (r2.داخلہ_گنتی <= 1 || input2.arrayZ[0] != داخلہ.arrayZ[0])
	    اكسر;
	}
      }
    }
    إذا (likely (unsafe_منزل != (بنانشانی) -1))
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار snap = c->serializer->snapshot ();
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    لكل (ثابت خودکار& _ : قاعدہ_2)
    {
      إذا (!_) استمر;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = باہرلا->قاعدہ_2.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا, lookup_map, klass_map))
      {
	باہرلا->قاعدہ_2.نکالو ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (باہرلا->قاعدہ_2);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (قاعدہ_2.sanitize (c, هذا));
  }

  محمي:
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<قاعدہ_Type>>
		قاعدہ_2;			/* Array of Rule tables
					 * ordered by preference */
  عام:
  DEFINE_ناپ_ARRAY (2, قاعدہ_2);
};

سانچا <قسم_ناں Types>
بنية قاعدہ_سیٹ : قاعدہ_سیٹ_دا<SmallTypes, قاعدہ_3<Types>> {};

سانچا <قسم_ناں Types>
بنية Class_قاعدہ_سیٹ : قاعدہ_سیٹ_دا<Types, قاعدہ_3<SmallTypes>> {};


سانچا <قسم_ناں Types>
بنية ContextFormat1_4
{
  ورتوں قاعدہ_سیٹ = OT::قاعدہ_سیٹ<Types>;

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    بنية Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالی_سوچک
    };

    أرجع
    + hb_zip (هذا+coverage, قاعدہ_سیٹ_2)
    | hb_filter (*glyphs, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([&] (ثابت قاعدہ_سیٹ &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_سرگرم_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالی_سوچک,
      خالی_سوچک,  /* intersects_cache */
      خالی_سوچک   /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((بنانشانی) قاعدہ_سیٹ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->ماپا_سرگرم_glyphs ().has (_);
    }, hb_پہلا)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بنانشانی> _) { أرجع hb_pair_t<بنانشانی, ثابت قاعدہ_سیٹ&> (_.پہلا, هذا+قاعدہ_سیٹ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بنانشانی, ثابت قاعدہ_سیٹ&>& _) { _.second.depend (c, _.پہلا, lookup_context); })
    ;

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (c->پچھلا_ماپا_سرگرم_glyphs (), *cur_سرگرم_glyphs);

    بنية Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالی_سوچک
    };

    + hb_zip (هذا+coverage, hb_range ((بنانشانی) قاعدہ_سیٹ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->پچھلا_ماپا_سرگرم_glyphs ().has (_);
    }, hb_پہلا)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بنانشانی> _) { أرجع hb_pair_t<بنانشانی, ثابت قاعدہ_سیٹ&> (_.پہلا, هذا+قاعدہ_سیٹ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بنانشانی, ثابت قاعدہ_سیٹ&>& _) { _.second.گھیرا (c, _.پہلا, lookup_context); })
    ;

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت
  {
    بنية Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_glyph, خالی_سوچک},
      ContextFormat::SimpleContext,
      خالی_سوچک
    };

    + hb_zip (هذا+coverage, قاعدہ_سیٹ_2)
    | hb_filter (*c->glyphs, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت قاعدہ_سیٹ &_) { _.گھیرا_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->داخلہ);

    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      خالی_سوچک
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت قاعدہ_سیٹ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([digest] (ثابت قاعدہ_سیٹ &_) { _.collect_دوجا_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت قاعدہ_سیٹ &قاعدہ_سیٹ_3 = هذا+قاعدہ_سیٹ_2[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ContextApplyLookupContext lookup_context = {
      {میل_glyph},
      خالی_سوچک
    };
    أرجع قاعدہ_سیٹ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (likely (اشاریہ_2 == NOT_COVERED))
      return_trace (خطأ);

    ثابت قاعدہ_سیٹ &قاعدہ_سیٹ_3 = هذا+قاعدہ_سیٹ_2[اشاریہ_2];
    بنية ContextApplyLookupContext lookup_context = {
      {میل_glyph},
      خالی_سوچک
    };
    return_trace (قاعدہ_سیٹ_3.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, قاعدہ_سیٹ_2)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_وتھ_array (c, باہرلا->قاعدہ_سیٹ_2, هذا, lookup_map), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    باہرلا->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && قاعدہ_سیٹ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<قاعدہ_سیٹ>>
		قاعدہ_سیٹ_2;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, قاعدہ_سیٹ_2);
};


سانچا <قسم_ناں Types>
بنية ContextFormat2_5
{
  ورتوں قاعدہ_سیٹ = OT::Class_قاعدہ_سیٹ<Types>;

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_class, خالی_سوچک},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_سیٹ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سیٹ (*glyphs, retained_coverage_glyphs);

    hb_سیٹ_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    أرجع
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<بنانشانی, ثابت قاعدہ_سیٹ &> p)
	      { أرجع class_def.intersects_class (glyphs, p.پہلا) &&
		       coverage_glyph_classes.has (p.پہلا) &&
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

    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_سرگرم_glyphs);

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

    + hb_enumerate (قاعدہ_سیٹ_2)
    | hb_filter ([&] (بنانشانی _)
    { أرجع class_def.intersects_class (&c->ماپا_سرگرم_glyphs (), _); },
		 hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<بنانشانی, ثابت قسم_ناں Types::سانچا وتھ_منزل<قاعدہ_سیٹ>&> _)
                {
                  ثابت قاعدہ_سیٹ& قاعدہ_سیٹ_3 = هذا+_.second;
                  قاعدہ_سیٹ_3.depend (c, _.پہلا, lookup_context);
                })
    ;

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (c->پچھلا_ماپا_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    بنية Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (قاعدہ_سیٹ_2)
    | hb_filter ([&] (بنانشانی _)
    { أرجع class_def.intersects_class (&c->ماپا_سرگرم_glyphs (), _); },
		 hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<بنانشانی, ثابت قسم_ناں Types::سانچا وتھ_منزل<قاعدہ_سیٹ>&> _)
                {
                  ثابت قاعدہ_سیٹ& قاعدہ_سیٹ_3 = هذا+_.second;
                  قاعدہ_سیٹ_3.گھیرا (c, _.پہلا, lookup_context);
                })
    ;

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_class, خالی_سوچک},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_enumerate
    | hb_filter ([&] (ثابت hb_pair_t<بنانشانی, ثابت قاعدہ_سیٹ &> p)
    { أرجع class_def.intersects_class (c->glyphs, p.پہلا); })
    | hb_map (hb_دوجا)
    | hb_apply ([&] (ثابت قاعدہ_سیٹ & _)
    { _.گھیرا_lookups (c, lookup_context); });
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->داخلہ);

    ثابت ClassDef &class_def = هذا+classDef;
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت قاعدہ_سیٹ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([=, &class_def] (ثابت قاعدہ_سیٹ &_) { _.collect_دوجا_glyphs (digest, &class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    بنانشانی صحيح اشاریہ_2 = class_def.get_class (c->glyphs[0]);
    ثابت قاعدہ_سیٹ &قاعدہ_سیٹ_3 = هذا+قاعدہ_سیٹ_2[اشاریہ_2];
    بنية ContextApplyLookupContext lookup_context = {
      {میل_class},
      &class_def
    };
    أرجع قاعدہ_سیٹ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنانشانی cache_لاگت () ثابت
  {
    أرجع (هذا+classDef).لاگت ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_ثنائی_cache_t coverage;
    hb_ot_layout_ruleset_digest_t قاعدہ_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بنانشانی گنتی = قاعدہ_سیٹ_2.len;
    بنانشانی ناپ = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    گنتی * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (ناپ);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (بنانشانی i = 0; i < گنتی; i++)
	(هذا+قاعدہ_سیٹ_2[i]).collect_پہلا_داخلہ_classes (&cache->قاعدہ_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, سچ, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage_ثنائی (c->بفر->cur().codepoint, cache ? &cache->coverage : خالی_سوچک);
#وإلا
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
#جے_انت
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &class_def = هذا+classDef;

    بنية ContextApplyLookupContext lookup_context = {
      {cached ? میل_class_cached : میل_class},
      &class_def,
      cached ? get_class_cached_قدر : get_class_قدر
    };

    اشاریہ_2 = cached ? get_class_cached (class_def, c->بفر->cur()) : class_def.get_class (c->بفر->cur().codepoint);
    ثابت قاعدہ_سیٹ &قاعدہ_سیٹ_3 = هذا+قاعدہ_سیٹ_2[اشاریہ_2];
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && اشاریہ_2 < قاعدہ_سیٹ_2.len ? &cache->قاعدہ_sets[اشاریہ_2] : خالی_سوچک;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = خالی_سوچک;
#جے_انت
    return_trace (قاعدہ_سیٹ_3.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;
    إذا (unlikely (!باہرلا->coverage.serialize_subset (c, coverage, هذا)))
      return_trace (خطأ);

    hb_map_t klass_map;
    باہرلا->classDef.serialize_subset (c, classDef, هذا, &klass_map);

    ثابت hb_سیٹ_t* glyphset = c->plan->glyphset_gsub ();
    hb_سیٹ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سیٹ (*glyphset, retained_coverage_glyphs);

    hb_سیٹ_t coverage_glyph_classes;
    (هذا+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ثابت hb_map_t *lookup_map = c->جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    منطقي ret = سچ;
    صحيح non_zero_اشاریہ = -1, اشاریہ_2 = 0;
    خودکار snapshot = c->serializer->snapshot();
    لكل (ثابت خودکار& _ : + hb_enumerate (قاعدہ_سیٹ_2)
			 | hb_filter (klass_map, hb_پہلا))
    {
      خودکار *o = باہرلا->قاعدہ_سیٹ_2.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      إذا (coverage_glyph_classes.has (_.پہلا) &&
	  o->serialize_subset (c, _.second, هذا, lookup_map, &klass_map)) {
	non_zero_اشاریہ = اشاریہ_2;
        snapshot = c->serializer->snapshot();
      }

      اشاریہ_2++;
    }

    إذا (!ret || non_zero_اشاریہ == -1) return_trace (خطأ);

    //prune empty trailing ruleSets
    --اشاریہ_2;
    طالما (اشاریہ_2 > non_zero_اشاریہ)
    {
      باہرلا->قاعدہ_سیٹ_2.نکالو ();
      اشاریہ_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (منطقي (باہرلا->قاعدہ_سیٹ_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && classDef.sanitize (c, هذا) && قاعدہ_سیٹ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_ناں Types::سانچا LOffset_منزل<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<قاعدہ_سیٹ>>
		قاعدہ_سیٹ_2;		/* Array of RuleSet tables
					 * ordered by class */
  عام:
  DEFINE_ناپ_ARRAY (2 + 2 * Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, قاعدہ_سیٹ_2);
};


سانچا <قسم_ناں Types>
بنية ContextFormat3_6
{
  ورتوں قاعدہ_سیٹ = OT::قاعدہ_سیٹ<SmallTypes>;

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (glyphs))
      أرجع خطأ;

    بنية Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_coverage, خالی_سوچک},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    أرجع context_intersects (glyphs,
			       glyph_گنتی_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_سرگرم_glyphs);

    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا,
      خالی_سوچک,  /* intersects_cache */
      خالی_سوچک   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_گنتی_2, (ثابت HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_گنتی_2, lookup_ریکارڈ,
			   هذا,
			   coverageZ[0], lookup_context);

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (c->پچھلا_ماپا_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    بنية Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    context_گھیرا_lookup (c,
			    glyph_گنتی_2, coverageZ.arrayZ + 1,
			    lookup_گنتی_2, lookup_ریکارڈ,
			    0, lookup_context);

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;
    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    recurse_lookups (c, lookup_گنتی_2, lookup_ریکارڈ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverageZ[0]).collect_coverage (c->داخلہ);

    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      هذا
    };

    context_collect_glyphs_lookup (c,
				   glyph_گنتی_2, coverageZ.arrayZ + 1,
				   lookup_گنتی_2, lookup_ریکارڈ,
				   lookup_context);
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    إذا (glyph_گنتی_2 <= 1)
      *digest = hb_سیٹ_digest_t::بھریا ();
    وإلا
      (هذا+coverageZ[1]).collect_coverage (digest);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    بنية ContextApplyLookupContext lookup_context = {
      {میل_coverage},
      هذا
    };
    أرجع context_would_apply_lookup (c,
				       glyph_گنتی_2, coverageZ.arrayZ + 1,
				       lookup_گنتی_2, lookup_ریکارڈ,
				       lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverageZ[0]; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشانی صحيح اشاریہ_2 = (هذا+coverageZ[0]).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    بنية ContextApplyLookupContext lookup_context = {
      {میل_coverage},
      هذا
    };
    return_trace (context_apply_lookup (c,
					glyph_گنتی_2,
					coverageZ.arrayZ + 1,
					lookup_گنتی_2, lookup_ریکارڈ,
					lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->format = format;
    باہرلا->glyph_گنتی_2 = glyph_گنتی_2;

    خودکار coverages = coverageZ.as_array (glyph_گنتی_2);

    لكل (ثابت خودکار& وتھ : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      خودکار *o = c->serializer->allocate_ناپ<قسم_ناں Types::سانچا وتھ_منزل<Coverage>> (Types::وتھ_2::static_ناپ);
      إذا (unlikely (!o)) return_trace (خطأ);
      إذا (!o->serialize_subset (c, وتھ, هذا)) return_trace (خطأ);
    }

    ثابت خودکار& lookup_ریکارڈ = StructAfter<Unsized_Array_دا<Lookup_ریکارڈ>> (coverageZ.as_array (glyph_گنتی_2));
    ثابت hb_map_t *lookup_map = c->جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    بنانشانی گنتی = serialize_lookuprecord_array (c->serializer, lookup_ریکارڈ.as_array (lookup_گنتی_2), lookup_map);
    return_trace (c->serializer->check_سونپو (باہرلا->lookup_گنتی_2, گنتی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    بنانشانی صحيح گنتی = glyph_گنتی_2;
    إذا (unlikely (!گنتی)) return_trace (خطأ); /* We want to access coverageZ[0] freely. */
    إذا (unlikely (!c->check_array (coverageZ.arrayZ, گنتی))) return_trace (خطأ);
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (unlikely (!coverageZ[i].sanitize (c, هذا))) return_trace (خطأ);
    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    return_trace (likely (c->check_array (lookup_ریکارڈ, lookup_گنتی_2)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_گنتی_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_گنتی_2;		/* Number of LookupRecords */
  Unsized_Array_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_ناپ_ARRAY (6, coverageZ);
};

بنية Context
{
  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    حالة 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#جے_انت
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#جے_انت
  } u;
};


/* Chaining Contextual lookups */

بنية Chain_Context_گھیرا_Lookup_Context
{
  Context_گھیرا_Funcs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_انکڑے[3];
  فراغ *intersects_cache[3];
  فراغ *intersected_glyphs_cache;
};

تعريفنوع Chain_Context_گھیرا_Lookup_Context ChainContextDependLookupContext;

بنية ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_انکڑے[3];
};

بنية ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ثابت فراغ *میل_انکڑے_2[3];
  get_قدر_func_t get_داخلہ_قدر;
};

ساكن مضمن منطقي chain_context_intersects (ثابت hb_سیٹ_t *glyphs,
					     بنانشانی صحيح backtrack_گنتی,
					     glyph_انکڑے_t backtrack,
					     بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
					     glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
					     بنانشانی صحيح lookahead_گنتی,
					     glyph_انکڑے_t lookahead,
					     Chain_Context_گھیرا_Lookup_Context &lookup_context)
{
  منطقي bt_ok = array_is_subset_دا (glyphs,
			     backtrack_گنتی, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_انکڑے[0],
			     lookup_context.intersects_cache[0]);
  منطقي وچ_ok = array_is_subset_دا (glyphs,
			     داخلہ_گنتی ? داخلہ_گنتی - 1 : 0, داخلہ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_انکڑے[1],
			     lookup_context.intersects_cache[1]);
  منطقي la_ok = array_is_subset_دا (glyphs,
			     lookahead_گنتی, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_انکڑے[2],
			     lookup_context.intersects_cache[2]);

  أرجع bt_ok && وچ_ok && la_ok;
}

ساكن مضمن فراغ chain_context_گھیرا_lookup (hb_گھیرا_context_t *c,
						 بنانشانی صحيح backtrack_گنتی,
						 glyph_انکڑے_t backtrack,
						 بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
						 glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
						 بنانشانی صحيح lookahead_گنتی,
						 glyph_انکڑے_t lookahead,
						 بنانشانی صحيح lookup_گنتی_2,
						 ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
						 بنانشانی value,
						 Chain_Context_گھیرا_Lookup_Context &lookup_context)
{
  إذا (chain_context_intersects (c->glyphs,
				backtrack_گنتی, backtrack,
				داخلہ_گنتی, داخلہ,
				lookahead_گنتی, lookahead,
				lookup_context))
    context_گھیرا_recurse_lookups (c,
		     داخلہ_گنتی, داخلہ,
		     lookup_گنتی_2, lookup_ریکارڈ,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_انکڑے[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ chain_context_depend_lookup (hb_depend_context_t *c,
						بنانشانی صحيح backtrack_گنتی,
						glyph_انکڑے_t backtrack,
						بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
						glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
						بنانشانی صحيح lookahead_گنتی,
						glyph_انکڑے_t lookahead,
						بنانشانی صحيح lookup_گنتی_2,
						ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
						ثابت فراغ *قاعدہ_2,
						بنانشانی value,
						ChainContextDependLookupContext &lookup_context)
{
  إذا (!chain_context_intersects (c->glyphs,
				 backtrack_گنتی, backtrack,
				 داخلہ_گنتی, داخلہ,
				 lookahead_گنتی, lookahead,
				 lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, داخلہ_گنتی, lookup_گنتی_2, lookup_ریکارڈ, قاعدہ_2, value,
    lookup_context.context_format, lookup_context.intersects_انکڑے[1],
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_انکڑے->successful))
    أرجع;
  إذا (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    داخلہ_گنتی, داخلہ,
				    lookup_گنتی_2, lookup_ریکارڈ,
				    قاعدہ_2,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_انکڑے[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    خالی_سوچک,
				    &scratch->cached_context_sets,
				    scratch);
    أرجع;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_سیٹ_t &position_scratch = scratch->position_scratch;
  hb_سمتیہ_t<ثابت hb_سیٹ_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_سمتیہ_t<ثابت hb_سیٹ_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  لكل (بنانشانی i = 0; i < backtrack_گنتی; i++)
  {
    ثابت hb_سیٹ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_انکڑے[0],
      backtrack[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!backtrack_sets.ڈالو_or_fail (pos_glyphs)))
      {
        c->depend_انکڑے->fail ();
        أرجع;
      }
    }
  }

  /* Extract lookahead glyphs */
  لكل (بنانشانی i = 0; i < lookahead_گنتی; i++)
  {
    ثابت hb_سیٹ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_انکڑے[2],
      lookahead[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!lookahead_sets.ڈالو_or_fail (pos_glyphs)))
      {
        c->depend_انکڑے->fail ();
        أرجع;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_سیٹ_t &preliminary_context = scratch->preliminary_context;
  لكل (ثابت hb_سیٹ_t *پٹھ_سیٹ : backtrack_sets)
  {
    إذا (پٹھ_سیٹ->get_population () == 1) {
      preliminary_context.جوڑو (پٹھ_سیٹ->get_min ());  // Direct glyph
    } وإلا إذا (پٹھ_سیٹ->get_population () > 1) {
      hb_codepoint_t سیٹ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_انکڑے->لبھو_or_create_context_سیٹ (*پٹھ_سیٹ)
        : c->لبھو_or_create_cached_context_سیٹ (پٹھ_سیٹ);
      إذا (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);  // Encoded set reference
    }
  }
  لكل (ثابت hb_سیٹ_t *look_سیٹ : lookahead_sets)
  {
    إذا (look_سیٹ->get_population () == 1) {
      preliminary_context.جوڑو (look_سیٹ->get_min ());  // Direct glyph
    } وإلا إذا (look_سیٹ->get_population () > 1) {
      hb_codepoint_t سیٹ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_انکڑے->لبھو_or_create_context_سیٹ (*look_سیٹ)
        : c->لبھو_or_create_cached_context_سیٹ (look_سیٹ);
      إذا (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);  // Encoded set reference
    }
  }
  إذا (unlikely (preliminary_context.وچ_error ()))
  {
    c->depend_انکڑے->fail ();
    أرجع;
  }

  /* Build glyph sets for ALL input positions */
  hb_سمتیہ_t<ثابت hb_سیٹ_t *> &داخلہ_position_glyphs = scratch->داخلہ_position_glyphs;

  إذا (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ثابت hb_سیٹ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_انکڑے[1], value,
      سچ, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !داخلہ_position_glyphs.ڈالو_or_fail (pos0_glyphs)))
    {
      c->depend_انکڑے->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (بنانشانی i = 0; i + 1 < داخلہ_گنتی; i++)
    {
      ثابت hb_سیٹ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_انکڑے[1],
	داخلہ[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !داخلہ_position_glyphs.ڈالو_or_fail (pos_glyphs)))
      {
	c->depend_انکڑے->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  داخلہ_گنتی, داخلہ,
				  lookup_گنتی_2, lookup_ریکارڈ,
				  قاعدہ_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_انکڑے[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? خالی_سوچک : &داخلہ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : خالی_سوچک,
				  scratch);
}

ساكن مضمن فراغ chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							بنانشانی صحيح backtrack_گنتی,
							glyph_انکڑے_t backtrack,
							بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
							glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
							بنانشانی صحيح lookahead_گنتی,
							glyph_انکڑے_t lookahead,
							بنانشانی صحيح lookup_گنتی_2,
							ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_گنتی, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_انکڑے[0]);
  collect_array (c, c->داخلہ,
		 داخلہ_گنتی ? داخلہ_گنتی - 1 : 0, داخلہ,
		 lookup_context.funcs.collect, lookup_context.collect_انکڑے[1]);
  collect_array (c, c->after,
		 lookahead_گنتی, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_انکڑے[2]);
  recurse_lookups (c,
		   lookup_گنتی_2, lookup_ریکارڈ);
}

ساكن مضمن منطقي chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     بنانشانی صحيح backtrack_گنتی,
						     glyph_انکڑے_t backtrack HB_بن_ورتیا,
						     بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
						     glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
						     بنانشانی صحيح lookahead_گنتی,
						     glyph_انکڑے_t lookahead HB_بن_ورتیا,
						     بنانشانی صحيح lookup_گنتی_2 HB_بن_ورتیا,
						     ثابت Lookup_ریکارڈ lookup_ریکارڈ[] HB_بن_ورتیا,
						     ثابت ChainContextApplyLookupContext &lookup_context)
{
  أرجع (c->zero_context ? !backtrack_گنتی && !lookahead_گنتی : سچ)
      && would_میل_داخلہ (c,
			    داخلہ_گنتی, داخلہ,
			    lookup_context.funcs.میل[1], lookup_context.میل_انکڑے_2[1]);
}

ساكن مضمن منطقي chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       بنانشانی صحيح backtrack_گنتی,
					       glyph_انکڑے_t backtrack,
					       بنانشانی صحيح داخلہ_گنتی, /* Including the first glyph (not matched) */
					       glyph_انکڑے_t داخلہ, /* Array of input values--start with second glyph */
					       بنانشانی صحيح lookahead_گنتی,
					       glyph_انکڑے_t lookahead,
					       بنانشانی صحيح lookup_گنتی_2,
					       ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
					       ثابت ChainContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (داخلہ_گنتی > HB_MAX_CONTEXT_لمائی)) أرجع خطأ;

  بنانشانی شروع_اشاریہ = c->بفر->باہرلا_len;
  بنانشانی سرا_اشاریہ = c->بفر->idx;
  بنانشانی میل_سرا = 0;
  منطقي ret = سچ;
  إذا (!(میل_داخلہ (c,
		     داخلہ_گنتی, داخلہ,
		     lookup_context.funcs.میل[1], lookup_context.میل_انکڑے_2[1],
		     &میل_سرا) && (سرا_اشاریہ = میل_سرا)
       && میل_lookahead (c,
			   lookahead_گنتی, lookahead,
			   lookup_context.funcs.میل[2], lookup_context.میل_انکڑے_2[2],
			   میل_سرا, &سرا_اشاریہ)))
  {
    c->بفر->unsafe_منزل_concat (c->بفر->idx, سرا_اشاریہ);
    أرجع خطأ;
  }

  إذا (!میل_backtrack (c,
			backtrack_گنتی, backtrack,
			lookup_context.funcs.میل[0], lookup_context.میل_انکڑے_2[0],
			&شروع_اشاریہ))
  {
    c->بفر->unsafe_منزل_concat_from_outbuffer (شروع_اشاریہ, سرا_اشاریہ);
    أرجع خطأ;
  }

  c->بفر->unsafe_منزل_break_from_outbuffer (شروع_اشاریہ, سرا_اشاریہ);
  apply_lookup (c,
		داخلہ_گنتی,
		lookup_گنتی_2, lookup_ریکارڈ,
		میل_سرا);

  أرجع ret;
}

سانچا <قسم_ناں Types>
بنية Chain_قاعدہ
{
  سانچا <قسم_ناں سیٹ_Types, قسم_ناں Chain_قاعدہ_Type>
  یار بنية Chain_قاعدہ_سیٹ_دا;

  منطقي intersects (ثابت hb_سیٹ_t *glyphs, Chain_Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     داخلہ.lenP1, داخلہ.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, بنانشانی value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 داخلہ.lenP1, داخلہ.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 value,
				 lookup_context);
  }

  فراغ گھیرا (hb_گھیرا_context_t *c, بنانشانی value,
		Chain_Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندی_exceeded ())) أرجع;

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_گھیرا_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  داخلہ.lenP1, داخلہ.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c,
                        Chain_Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندی_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 داخلہ.lenP1, داخلہ.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest,
			      ثابت ClassDef *داخلہ_class_def = خالی_سوچک,
			      ثابت ClassDef *lookahead_class_def = خالی_سوچک) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);

    بنانشانی second;
    ثابت ClassDef *class_def;
    إذا (داخلہ.lenP1 > 1)
    {
      second = داخلہ.arrayZ[0];
      class_def = داخلہ_class_def;
    }
    وإلا إذا (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    وإلا
    {
      *digest = hb_سیٹ_digest_t::بھریا ();
      أرجع;
    }

    إذا (!class_def)
      digest->جوڑو (second);
    وإلا إذا (!second)
      *digest = hb_سیٹ_digest_t::بھریا ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     داخلہ.lenP1, داخلہ.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      داخلہ.lenP1, داخلہ.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ثابت
  {
    c->copy (len);
    لكل (ثابت خودکار g : it)
    {
      قسم_ناں Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *lookup_map,
		  ثابت hb_map_t *backtrack_map,
		  ثابت hb_map_t *داخلہ_map = خالی_سوچک,
		  ثابت hb_map_t *lookahead_map = خالی_سوچک) ثابت
  {
    TRACE_SERIALIZE (هذا);

    ثابت hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    إذا (داخلہ_map) mapping = داخلہ_map;
    serialize_array (c, داخلہ.lenP1, + داخلہ.iter ()
				     | hb_map (mapping));

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    إذا (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_گنتی_2 = c->embed (&(lookup.len));
    إذا (!lookup_گنتی_2) return_trace (خطأ);

    بنانشانی گنتی = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_سونپو (*lookup_گنتی_2, گنتی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_map = خالی_سوچک,
	       ثابت hb_map_t *داخلہ_map = خالی_سوچک,
	       ثابت hb_map_t *lookahead_map = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);

    إذا (!backtrack_map)
    {
      ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
      إذا (!hb_all (backtrack, glyphset) ||
	  !hb_all (داخلہ, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    وإلا
    {
      إذا (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (داخلہ, داخلہ_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, backtrack_map, داخلہ_map, lookahead_map);
    }

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Hyper-optimized sanitized because this is really hot. */
    إذا (unlikely (!backtrack.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    إذا (unlikely (!داخلہ.lenP1.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    إذا (unlikely (!lookahead.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  Array16_دا<قسم_ناں Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  Headless_Array16_دا<قسم_ناں Types::HBUINT>
		داخلہ_X;			/* Array of input values (start with
					 * second glyph) */
  Array16_دا<قسم_ناں Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16_دا<Lookup_ریکارڈ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_ناپ_MIN (8);
};

سانچا <قسم_ناں Types, قسم_ناں Chain_قاعدہ_Type>
بنية Chain_قاعدہ_سیٹ_دا
{
  منطقي intersects (ثابت hb_سیٹ_t *glyphs, Chain_Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([&] (ثابت Chain_قاعدہ_Type &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_پہلا_داخلہ_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (بنانشانی i = 0; i < قاعدہ_2.len; i++)
    {
      ثابت Chain_قاعدہ_Type &r = هذا+قاعدہ_2.arrayZ[i];
      ثابت خودکار &داخلہ = StructAfter<decltype (r.داخلہ_X)> (r.backtrack);
      إذا (داخلہ.lenP1 <= 1)
      {
	digest->سیٹ_بھریا ();
	أرجع;
      }
      digest->جوڑو (داخلہ.arrayZ[0]);
    }
  }
  فراغ depend (hb_depend_context_t *c, بنانشانی value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدہ_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }
  فراغ گھیرا (hb_گھیرا_context_t *c, بنانشانی value, Chain_Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندی_exceeded ())) أرجع;

    أرجع
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدہ_Type &_) { _.گھیرا (c, value, lookup_context); })
    ;
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c,
                        Chain_Context_گھیرا_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_بندی_exceeded ())) أرجع;

    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدہ_Type &_) { _.گھیرا_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدہ_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest,
			      ثابت ClassDef *داخلہ_class_def = خالی_سوچک,
			      ثابت ClassDef *lookahead_class_def = خالی_سوچک) ثابت
  {
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([=] (ثابت Chain_قاعدہ_Type &_)
		{ _.collect_دوجا_glyphs (digest, داخلہ_class_def, lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([&] (ثابت Chain_قاعدہ_Type &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = خالی_سوچک) ثابت
  {
    TRACE_APPLY (هذا);

    بنانشانی عدد_قاعدے = قاعدہ_2.len;

#جے_بنا_تعریف HB_NO_OT_RULESETS_FAST_رستہ
    إذا (HB_OPTIMIZE_ناپ_VAL || عدد_قاعدے <= 4)
#جے_انت
    {
    slow:
      return_trace (
      + hb_iter (قاعدہ_2)
      | hb_map (hb_جوڑو (هذا))
      | hb_map ([&] (ثابت Chain_قاعدہ_Type &_) { أرجع _.apply (c, lookup_context); })
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
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سیٹ_میل_func (میل_always, خالی_سوچک);
    skippy_iter.سیٹ_glyph_انکڑے ((HBUINT16 *) خالی_سوچک);
    بنانشانی unsafe_منزل = (بنانشانی) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *پہلا = خالی_سوچک, *second = خالی_سوچک;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      پہلا = &c->بفر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_داخلہ_قدر &&
	  !digest->may_have (lookup_context.get_داخلہ_قدر (*پہلا, lookup_context.میل_انکڑے_2[1])))
	{
	  c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_منزل_concat = خطأ;
      منطقي ret =
      + hb_iter (قاعدہ_2)
      | hb_map (hb_جوڑو (هذا))
      | hb_filter ([&] (ثابت Chain_قاعدہ_Type &_)
		   {
		     ثابت خودکار &داخلہ = StructAfter<decltype (_.داخلہ_X)> (_.backtrack);
		     ثابت خودکار &lookahead = StructAfter<decltype (_.lookaheadX)> (داخلہ);
		     إذا (داخلہ.lenP1 <= 1 && lookahead.len == 0) أرجع سچ;
		     unsafe_منزل_concat = سچ;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت Chain_قاعدہ_Type &_) { أرجع _.apply (c, lookup_context); })
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

    خودکار میل_داخلہ = lookup_context.funcs.میل[1];
    خودکار میل_lookahead = lookup_context.funcs.میل[2];
    خودکار *داخلہ_انکڑے = lookup_context.میل_انکڑے_2[1];
    خودکار *lookahead_انکڑے = lookup_context.میل_انکڑے_2[2];
    لكل (بنانشانی صحيح i = 0; i < عدد_قاعدے; i++)
    {
      ثابت خودکار &r = هذا+قاعدہ_2.arrayZ[i];

      ثابت خودکار &داخلہ = StructAfter<decltype (r.داخلہ_X)> (r.backtrack);
      ثابت خودکار &lookahead = StructAfter<decltype (r.lookaheadX)> (داخلہ);

      بنانشانی lenP1 = داخلہ.lenP1;
      إذا (lenP1 > 1 ?
	   (!میل_داخلہ ||
	    میل_داخلہ (*پہلا, داخلہ.arrayZ[0], داخلہ_انکڑے))
	  :
	   (!lookahead.len || !میل_lookahead ||
	    میل_lookahead (*پہلا, lookahead.arrayZ[0], lookahead_انکڑے)))
      {
	lenP1 = hb_max (lenP1, 1u);
        إذا (!second ||
	    (lenP1 > 2 ?
	     (!میل_داخلہ ||
	      میل_داخلہ (*second, داخلہ.arrayZ[1], داخلہ_انکڑے))
	     :
	     (lookahead.len <= 2 - lenP1 || !میل_lookahead ||
	      میل_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_انکڑے))))
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_منزل != (بنانشانی) -1)
	      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
	    return_trace (سچ);
	  }
	}
	وإلا
	  unsafe_منزل = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_منزل == (بنانشانی) -1)
	  unsafe_منزل = unsafe_to1;

	إذا (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  لكل (; i + 1 < عدد_قاعدے; i++)
	  {
	    ثابت خودکار &r2 = هذا+قاعدہ_2.arrayZ[i + 1];
	    ثابت خودکار &input2 = StructAfter<decltype (r2.داخلہ_X)> (r2.backtrack);
	    إذا (input2.lenP1 <= 1 || input2.arrayZ[0] != داخلہ.arrayZ[0])
	      اكسر;
	  }
	}
      }
    }
    إذا (likely (unsafe_منزل != (بنانشانی) -1))
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_klass_map = خالی_سوچک,
	       ثابت hb_map_t *داخلہ_klass_map = خالی_سوچک,
	       ثابت hb_map_t *lookahead_klass_map = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);

    خودکار snap = c->serializer->snapshot ();
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    لكل (ثابت خودکار& _ : قاعدہ_2)
    {
      إذا (!_) استمر;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = باہرلا->قاعدہ_2.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا,
				lookup_map,
				backtrack_klass_map,
				داخلہ_klass_map,
				lookahead_klass_map))
      {
	باہرلا->قاعدہ_2.نکالو ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (باہرلا->قاعدہ_2);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (قاعدہ_2.sanitize (c, هذا));
  }

  محمي:
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Chain_قاعدہ_Type>>
		قاعدہ_2;			/* Array of ChainRule tables
					 * ordered by preference */
  عام:
  DEFINE_ناپ_ARRAY (2, قاعدہ_2);
};

سانچا <قسم_ناں Types>
بنية Chain_قاعدہ_سیٹ : Chain_قاعدہ_سیٹ_دا<Types, Chain_قاعدہ<Types>> {};

static_assert (حجم (Chain_قاعدہ_سیٹ<MediumTypes>) ==
	       HBUINT16::static_ناپ + Offset24::static_ناپ, "");

سانچا <قسم_ناں Types>
بنية Chain_Class_قاعدہ_سیٹ : Chain_قاعدہ_سیٹ_دا<Types, Chain_قاعدہ<SmallTypes>> {};

سانچا <قسم_ناں Types>
بنية ChainContextFormat1_4
{
  ورتوں Chain_قاعدہ_سیٹ = OT::Chain_قاعدہ_سیٹ<Types>;

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    بنية Chain_Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالی_سوچک, خالی_سوچک, خالی_سوچک}
    };

    أرجع
    + hb_zip (هذا+coverage, قاعدہ_سیٹ_2)
    | hb_filter (*glyphs, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_map ([&] (ثابت Chain_قاعدہ_سیٹ &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_سرگرم_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالی_سوچک, خالی_سوچک, خالی_سوچک},
      {خالی_سوچک, خالی_سوچک, خالی_سوچک},  /* intersects_cache */
      خالی_سوچک                        /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((بنانشانی) قاعدہ_سیٹ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->ماپا_سرگرم_glyphs ().has (_);
    }, hb_پہلا)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بنانشانی> _) { أرجع hb_pair_t<بنانشانی, ثابت Chain_قاعدہ_سیٹ&> (_.پہلا, هذا+قاعدہ_سیٹ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بنانشانی, ثابت Chain_قاعدہ_سیٹ&>& _) { _.second.depend (c, _.پہلا, lookup_context); })
    ;

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (c->پچھلا_ماپا_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    بنية Chain_Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالی_سوچک, خالی_سوچک, خالی_سوچک}
    };

    + hb_zip (هذا+coverage, hb_range ((بنانشانی) قاعدہ_سیٹ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->پچھلا_ماپا_سرگرم_glyphs ().has (_);
    }, hb_پہلا)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بنانشانی> _) { أرجع hb_pair_t<بنانشانی, ثابت Chain_قاعدہ_سیٹ&> (_.پہلا, هذا+قاعدہ_سیٹ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بنانشانی, ثابت Chain_قاعدہ_سیٹ&>& _) { _.second.گھیرا (c, _.پہلا, lookup_context); })
    ;

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت
  {
    بنية Chain_Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_glyph, خالی_سوچک},
      ContextFormat::SimpleContext,
      {خالی_سوچک, خالی_سوچک, خالی_سوچک}
    };

    + hb_zip (هذا+coverage, قاعدہ_سیٹ_2)
    | hb_filter (*c->glyphs, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدہ_سیٹ &_) { _.گھیرا_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->داخلہ);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {خالی_سوچک, خالی_سوچک, خالی_سوچک}
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدہ_سیٹ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([digest] (ثابت Chain_قاعدہ_سیٹ &_) { _.collect_دوجا_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Chain_قاعدہ_سیٹ &قاعدہ_سیٹ_3 = هذا+قاعدہ_سیٹ_2[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{میل_glyph, میل_glyph, میل_glyph}},
      {خالی_سوچک, خالی_سوچک, خالی_سوچک}
    };
    أرجع قاعدہ_سیٹ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    ثابت Chain_قاعدہ_سیٹ &قاعدہ_سیٹ_3 = هذا+قاعدہ_سیٹ_2[اشاریہ_2];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{میل_glyph, میل_glyph, میل_glyph}},
      {خالی_سوچک, خالی_سوچک, خالی_سوچک}
    };
    return_trace (قاعدہ_سیٹ_3.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, قاعدہ_سیٹ_2)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_وتھ_array (c, باہرلا->قاعدہ_سیٹ_2, هذا, lookup_map), hb_دوجا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    باہرلا->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && قاعدہ_سیٹ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::سانچا وتھ_منزل<Chain_قاعدہ_سیٹ>>
		قاعدہ_سیٹ_2;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, قاعدہ_سیٹ_2);
};

سانچا <قسم_ناں Types>
بنية ChainContextFormat2_5
{
  ورتوں Chain_قاعدہ_سیٹ = OT::Chain_Class_قاعدہ_سیٹ<Types>;

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = هذا+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية Chain_Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_class, خالی_سوچک},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_سیٹ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سیٹ (*glyphs, retained_coverage_glyphs);

    hb_سیٹ_t coverage_glyph_classes;
    داخلہ_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    أرجع
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<بنانشانی, ثابت Chain_قاعدہ_سیٹ &> p)
	      { أرجع داخلہ_class_def.intersects_class (glyphs, p.پہلا) &&
		       coverage_glyph_classes.has (p.پہلا) &&
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

    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_سرگرم_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = هذا+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعدہ_سیٹ_2)
    | hb_filter ([&] (بنانشانی _)
    { أرجع داخلہ_class_def.intersects_class (&c->ماپا_سرگرم_glyphs (), _); },
		 hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<بنانشانی, ثابت قسم_ناں Types::سانچا وتھ_منزل<Chain_قاعدہ_سیٹ>&> _)
                {
                  ثابت Chain_قاعدہ_سیٹ& chainrule_سیٹ = هذا+_.second;
                  chainrule_سیٹ.depend (c, _.پہلا, lookup_context);
                })
    ;

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs)) أرجع;
    get_coverage ().intersect_سیٹ (c->پچھلا_ماپا_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = هذا+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    بنية Chain_Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعدہ_سیٹ_2)
    | hb_filter ([&] (بنانشانی _)
    { أرجع داخلہ_class_def.intersects_class (&c->ماپا_سرگرم_glyphs (), _); },
		 hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<بنانشانی, ثابت قسم_ناں Types::سانچا وتھ_منزل<Chain_قاعدہ_سیٹ>&> _)
                {
                  ثابت Chain_قاعدہ_سیٹ& chainrule_سیٹ = هذا+_.second;
                  chainrule_سیٹ.گھیرا (c, _.پہلا, lookup_context);
                })
    ;

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = هذا+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية Chain_Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_class, خالی_سوچک},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_enumerate
    | hb_filter([&] (بنانشانی klass)
    { أرجع داخلہ_class_def.intersects_class (c->glyphs, klass); }, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_apply ([&] (ثابت Chain_قاعدہ_سیٹ &_)
    { _.گھیرا_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->داخلہ);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = هذا+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def}
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([&] (ثابت Chain_قاعدہ_سیٹ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    ثابت ClassDef &داخلہ_class_def = هذا+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (هذا))
    | hb_apply ([digest, &داخلہ_class_def, &lookahead_class_def] (ثابت Chain_قاعدہ_سیٹ &_)
		{ _.collect_دوجا_glyphs (digest, &داخلہ_class_def, &lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = هذا+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بنانشانی صحيح اشاریہ_2 = داخلہ_class_def.get_class (c->glyphs[0]);
    ثابت Chain_قاعدہ_سیٹ &قاعدہ_سیٹ_3 = هذا+قاعدہ_سیٹ_2[اشاریہ_2];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{میل_class, میل_class, میل_class}},
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def}
    };
    أرجع قاعدہ_سیٹ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بنانشانی cache_لاگت () ثابت
  {
    أرجع (هذا+داخلہ_Class_Def).لاگت () + (هذا+lookaheadClassDef).لاگت ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_ثنائی_cache_t coverage;
    hb_ot_layout_mapping_cache_t داخلہ_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t قاعدہ_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بنانشانی گنتی = قاعدہ_سیٹ_2.len;
    بنانشانی ناپ = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    گنتی * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (ناپ);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->داخلہ_class.clear ();
      cache->lookahead_class.clear ();
      لكل (بنانشانی i = 0; i < گنتی; i++)
	(هذا+قاعدہ_سیٹ_2[i]).collect_پہلا_داخلہ_classes (&cache->قاعدہ_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, سچ, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage_ثنائی (c->بفر->cur().codepoint, cache ? &cache->coverage : خالی_سوچک);
#وإلا
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
#جے_انت
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = هذا+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    میل_class_cache_انکڑے_t داخلہ_cache_انکڑے = {
      &داخلہ_class_def,
      cache ? &cache->داخلہ_class : خالی_سوچک
    };
    میل_class_cache_انکڑے_t lookahead_cache_انکڑے = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : خالی_سوچک
    };
    ثابت فراغ *داخلہ_میل_انکڑے = cached
      ? static_cast<ثابت فراغ *> (&داخلہ_class_def)
      : static_cast<ثابت فراغ *> (&داخلہ_cache_انکڑے);
    ثابت فراغ *lookahead_میل_انکڑے = cached
      ? static_cast<ثابت فراغ *> (&lookahead_class_def)
      : static_cast<ثابت فراغ *> (&lookahead_cache_انکڑے);
    منطقي backtrack_ورتدا_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ثابت فراغ *backtrack_میل_انکڑے = backtrack_ورتدا_lookahead_cache
      ? static_cast<ثابت فراغ *> (&lookahead_cache_انکڑے)
      : static_cast<ثابت فراغ *> (&backtrack_class_def);
#جے_انت

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    بنية ChainContextApplyLookupContext lookup_context = {
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? میل_class_cached1 :
        backtrack_ورتدا_lookahead_cache ? میل_class_cache : میل_class,
        cached ? میل_class_cached2 : میل_class_cache,
        cached ? میل_class_cached1 : میل_class_cache}},
      {backtrack_میل_انکڑے,
       داخلہ_میل_انکڑے,
       lookahead_میل_انکڑے},
      cached ? get_class_cached2_قدر : get_class_cache_قدر
#وإلا
      {{cached && &backtrack_class_def == &lookahead_class_def ? میل_class_cached1 : میل_class,
        cached ? میل_class_cached2 : میل_class,
        cached ? میل_class_cached1 : میل_class}},
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_قدر : get_class_قدر
#جے_انت
    };

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    اشاریہ_2 = cached
         ? get_class_cached2 (داخلہ_class_def, c->بفر->cur())
         : داخلہ_class_def.get_class (c->بفر->cur().codepoint, داخلہ_cache_انکڑے.cache);
#وإلا
    اشاریہ_2 = cached
         ? get_class_cached2 (داخلہ_class_def, c->بفر->cur())
          : داخلہ_class_def.get_class (c->بفر->cur().codepoint);
#جے_انت
    ثابت Chain_قاعدہ_سیٹ &قاعدہ_سیٹ_3 = هذا+قاعدہ_سیٹ_2[اشاریہ_2];
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && اشاریہ_2 < قاعدہ_سیٹ_2.len ? &cache->قاعدہ_sets[اشاریہ_2] : خالی_سوچک;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = خالی_سوچک;
#جے_انت
    return_trace (قاعدہ_سیٹ_3.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;
    باہرلا->coverage.serialize_subset (c, coverage, هذا);

    hb_map_t backtrack_klass_map;
    hb_map_t داخلہ_klass_map;
    hb_map_t lookahead_klass_map;

    باہرلا->backtrackClassDef.serialize_subset (c, backtrackClassDef, هذا, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    باہرلا->داخلہ_Class_Def.serialize_subset (c, داخلہ_Class_Def, هذا, &داخلہ_klass_map);
    باہرلا->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, هذا, &lookahead_klass_map);

    إذا (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   داخلہ_klass_map,
						   lookahead_klass_map)))
      return_trace (خطأ);

    ثابت hb_سیٹ_t* glyphset = c->plan->glyphset_gsub ();
    hb_سیٹ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سیٹ (*glyphset, retained_coverage_glyphs);

    hb_سیٹ_t coverage_glyph_classes;
    (هذا+داخلہ_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    صحيح non_zero_اشاریہ = -1, اشاریہ_2 = 0;
    منطقي ret = سچ;
    ثابت hb_map_t *lookup_map = c->جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    خودکار آخری_non_zero = c->serializer->snapshot ();
    لكل (ثابت خودکار& _ : + hb_enumerate (قاعدہ_سیٹ_2)
			 | hb_filter (داخلہ_klass_map, hb_پہلا))
    {
      خودکار *o = باہرلا->قاعدہ_سیٹ_2.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }
      إذا (coverage_glyph_classes.has (_.پہلا) &&
          o->serialize_subset (c, _.second, هذا,
			       lookup_map,
			       &backtrack_klass_map,
			       &داخلہ_klass_map,
			       &lookahead_klass_map))
      {
        آخری_non_zero = c->serializer->snapshot ();
	non_zero_اشاریہ = اشاریہ_2;
      }

      اشاریہ_2++;
    }

    إذا (!ret || non_zero_اشاریہ == -1) return_trace (خطأ);

    // prune empty trailing ruleSets
    إذا (اشاریہ_2 > non_zero_اشاریہ) {
      c->serializer->revert (آخری_non_zero);
      باہرلا->قاعدہ_سیٹ_2.len = non_zero_اشاریہ + 1;
    }

    return_trace (منطقي (باہرلا->قاعدہ_سیٹ_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) &&
		  backtrackClassDef.sanitize (c, هذا) &&
		  داخلہ_Class_Def.sanitize (c, هذا) &&
		  lookaheadClassDef.sanitize (c, هذا) &&
		  قاعدہ_سیٹ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_ناں Types::سانچا LOffset_منزل<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  قسم_ناں Types::سانچا LOffset_منزل<ClassDef>
		داخلہ_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  قسم_ناں Types::سانچا LOffset_منزل<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Chain_قاعدہ_سیٹ>>
		قاعدہ_سیٹ_2;		/* Array of ChainRuleSet tables
					 * ordered by class */
  عام:
  DEFINE_ناپ_ARRAY (4 + 4 * Types::LOffset::static_ناپ, قاعدہ_سیٹ_2);
};

سانچا <قسم_ناں Types>
بنية ChainContextFormat3
{
  ورتوں قاعدہ_سیٹ = OT::قاعدہ_سیٹ<SmallTypes>;

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    إذا (!(هذا+داخلہ[0]).intersects (glyphs))
      أرجع خطأ;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    بنية Chain_Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_coverage, خالی_سوچک},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     داخلہ.len, داخلہ.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع سچ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    إذا (!(هذا+داخلہ[0]).intersects (c->glyphs))
      أرجع;

    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs))
      أرجع;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_سرگرم_glyphs);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا},
      {خالی_سوچک, خالی_سوچک, خالی_سوچک},  /* intersects_cache */
      خالی_سوچک                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ثابت HBUINT16 *) backtrack.arrayZ,
				 داخلہ.len, (ثابت HBUINT16 *) داخلہ.arrayZ + 1,
				 lookahead.len, (ثابت HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 داخلہ[0], lookup_context);

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    إذا (!(هذا+داخلہ[0]).intersects (c->glyphs))
      أرجع;

    hb_سیٹ_t* cur_سرگرم_glyphs = c->ڈالو_cur_سرگرم_glyphs ();
    إذا (unlikely (!cur_سرگرم_glyphs))
      أرجع;
    get_coverage ().intersect_سیٹ (c->پچھلا_ماپا_سرگرم_glyphs (),
				   *cur_سرگرم_glyphs);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية Chain_Context_گھیرا_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    chain_context_گھیرا_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  داخلہ.len, داخلہ.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->نکالو_cur_پورا_glyphs ();
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    (هذا+داخلہ[0]).collect_coverage (c->داخلہ);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {هذا, هذا, هذا}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 داخلہ.len, داخلہ.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    إذا (داخلہ.len > 1)
      (هذا+داخلہ[1]).collect_coverage (digest);
    وإلا إذا (lookahead.len)
      (هذا+lookahead[0]).collect_coverage (digest);
    وإلا
      *digest = hb_سیٹ_digest_t::بھریا ();
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{میل_coverage, میل_coverage, میل_coverage}},
      {هذا, هذا, هذا}
    };
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     داخلہ.len, داخلہ.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    أرجع هذا+داخلہ[0];
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    بنانشانی صحيح اشاریہ_2 = (هذا+داخلہ[0]).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{میل_coverage, میل_coverage, میل_coverage}},
      {هذا, هذا, هذا}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      داخلہ.len, داخلہ.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ثابت فراغ* ادھار) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed<Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>> ();
    إذا (unlikely (!باہرلا->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خودکار& وتھ : it) {
      خودکار *o = باہرلا->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, وتھ, ادھار))
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

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    إذا (!serialize_coverage_offsets (c, داخلہ.iter (), هذا))
      return_trace (خطأ);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    إذا (!serialize_coverage_offsets (c, lookahead.iter (), هذا))
      return_trace (خطأ);

    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ثابت hb_map_t *lookup_map = c->جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_گنتی_2 = c->serializer->copy<HBUINT16> (lookup.len);
    إذا (!lookup_گنتی_2) return_trace (خطأ);

    بنانشانی گنتی = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_سونپو (*lookup_گنتی_2, گنتی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!backtrack.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    إذا (unlikely (!داخلہ.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!داخلہ.len)) return_trace (خطأ); /* To be consistent with Context. */
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    إذا (unlikely (!lookahead.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>
		داخلہ_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16_دا<قسم_ناں Types::سانچا وتھ_منزل<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16_دا<Lookup_ریکارڈ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_ناپ_MIN (10);
};

بنية ChainContext
{
  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#جے_انت
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#جے_انت
  } u;
};


سانچا <قسم_ناں T>
بنية ExtensionFormat1
{
  بنانشانی صحيح get_type () ثابت { أرجع extensionLookupType; }

  سانچا <قسم_ناں X>
  ثابت X& get_subtable () ثابت
  { أرجع هذا + reinterpret_cast<ثابت Offset32_منزل<قسم_ناں T::Sub_جدول> &> (extension_وتھ); }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, هذا))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, format);
    return_trace (get_subtable<قسم_ناں T::Sub_جدول> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
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

    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->format = format;
    باہرلا->extensionLookupType = extensionLookupType;

    ثابت خودکار& src_وتھ =
        reinterpret_cast<ثابت Offset32_منزل<قسم_ناں T::Sub_جدول> &> (extension_وتھ);
    خودکار& dest_وتھ =
        reinterpret_cast<Offset32_منزل<قسم_ناں T::Sub_جدول> &> (باہرلا->extension_وتھ);

    return_trace (dest_وتھ.serialize_subset (c, src_وتھ, هذا, get_type ()));
  }

  محمي:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extension_وتھ;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  عام:
  DEFINE_ناپ_STATIC (8);
};

سانچا <قسم_ناں T>
بنية Extension
{
  بنانشانی صحيح get_type () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_type ();
    افتراضي:أرجع 0;
    }
  }
  سانچا <قسم_ناں X>
  ثابت X& get_subtable () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.سانچا get_subtable<قسم_ناں T::Sub_جدول> ();
    افتراضي:أرجع Null (قسم_ناں T::Sub_جدول);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  سانچا <قسم_ناں ...Ts>
  قسم_ناں hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.subset (c);
    افتراضي: أرجع c->default_return_قدر ();
    }
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
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
  سانچا <قسم_ناں TLookup>
  ساكن hb_ot_layout_lookup_accelerator_t *create (ثابت TLookup &lookup)
  {
    بنانشانی گنتی = lookup.get_subtable_گنتی ();

    بنانشانی ناپ = حجم (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    گنتی * حجم (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خودکار *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, ناپ);
    إذا (unlikely (!thiz))
      أرجع خالی_سوچک;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    طالما (گنتی && !thiz->subtables[گنتی - 1].apply_func)
      گنتی--;

    thiz->digest.init ();
    لكل (خودکار& subtable : hb_iter (thiz->subtables, گنتی))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_دوجا = c_accelerate_subtables.digest_دوجا;

    thiz->گنتی = گنتی;

#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    لكل (بنانشانی i = 0; i < گنتی; i++)
      إذا (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#جے_انت

    أرجع thiz;
  }

  فراغ fini ()
  {
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    لكل (بنانشانی i = 0; i < گنتی; i++)
      hb_free (subtables[i].external_cache);
#جے_انت
  }

  منطقي may_have (hb_codepoint_t g) ثابت
  { أرجع digest.may_have (g); }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  منطقي apply (hb_ot_apply_context_t *c, منطقي use_cache) ثابت
  {
    إذا (گنتی == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
      إذا (use_cache)
	أرجع subtables[0].apply_cached_no_digest (c);
#جے_انت
      أرجع subtables[0].apply_no_digest (c);
    }
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (use_cache)
    {
      أرجع
      + hb_iter (hb_iter (subtables, گنتی))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply_cached (c); })
      | hb_any
      ;
    }
    وإلا
#جے_انت
    {
      أرجع
      + hb_iter (hb_iter (subtables, گنتی))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply (c); })
      | hb_any
      ;
    }
    أرجع خطأ;
  }

  منطقي cache_enter (hb_ot_apply_context_t *c) ثابت
  {
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    أرجع subtable_cache_user_idx != (بنانشانی) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#وإلا
    أرجع خطأ;
#جے_انت
  }
  فراغ cache_leave (hb_ot_apply_context_t *c) ثابت
  {
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#جے_انت
  }


  hb_سیٹ_digest_t digest;
  hb_سیٹ_digest_t digest_دوجا;
  خاص:
  بنانشانی گنتی = 0; /* Number of subtables in the array. */
#جے_بنا_تعریف HB_NO_OT_LAYOUT_LOOKUP_CACHE
  بنانشانی subtable_cache_user_idx = (بنانشانی) -1;
#جے_انت
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

بنية GSUBGPOS
{
  ورتوں LookupListT = LookupList<SmallTypes>;
  ورتوں LookupList2T = LookupList<MediumTypes>;
  سانچا <قسم_ناں TLookup>
  ورتوں TLookupList = List16_دا_وتھ_منزل<TLookup, قسم_ناں SmallTypes::HBLUINT>;
  سانچا <قسم_ناں TLookup>
  ورتوں TLookupList2 = List16_دا_وتھ_منزل<TLookup, قسم_ناں MediumTypes::HBLUINT>;
  سانچا <قسم_ناں TLookup>
  ورتوں TLookup_وتھ_List = Lookup_وتھ_List<TLookup, قسم_ناں SmallTypes::HBLUINT>;
  سانچا <قسم_ناں TLookup>
  ورتوں TLookup_وتھ_List2 = Lookup_وتھ_List<TLookup, قسم_ناں MediumTypes::HBLUINT>;

  size_t get_ناپ () ثابت
  {
    إذا (نسخہ.major != 1) أرجع نسخہ.static_ناپ;

    أرجع min_ناپ +
	   (نسخہ.منزل_int () >= 0x00010001u ? featureVars.static_ناپ : 0) +
#جے_بنا_تعریف HB_NO_BEYOND_64K
	   (نسخہ.منزل_int () >= 0x00010002u ? 3 * Offset32::static_ناپ : 0)
#وإلا
	   0
#جے_انت
	   ;
  }

  ثابت ScriptList &get_script_list () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010002u && scriptList2) أرجع هذا + scriptList2;
#جے_انت
    أرجع هذا + scriptList;
  }
  ثابت FeatureList &get_feature_list () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010002u && featureList2) أرجع هذا + featureList2;
#جے_انت
    أرجع هذا + featureList;
  }
  سانچا <قسم_ناں TLookup>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!نسخہ.sanitize (c) ||
		  نسخہ.major != 1 ||
		  !c->check_range (هذا, get_ناپ ())))
      return_trace (خطأ);
    hb_barrier ();

    منطقي script_list_ok = scriptList.sanitize (c, هذا);
    منطقي feature_list_ok = featureList.sanitize (c, هذا);
    منطقي lookup_list_ok = typed_lookup_list_وتھ<TLookup> ().sanitize (c, هذا);

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010002u)
    {
      إذا (scriptList2) script_list_ok = scriptList2.sanitize (c, هذا);
      إذا (featureList2) feature_list_ok = featureList2.sanitize (c, هذا);
      إذا (lookupList2) lookup_list_ok = typed_lookup_list2_وتھ<TLookup> ().sanitize (c, هذا);
    }
#جے_انت

    إذا (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (خطأ);

#جے_بنا_تعریف HB_NO_VAR
    إذا (unlikely (!(نسخہ.منزل_int () < 0x00010001u ||
		    featureVars.sanitize (c, هذا, &get_feature_list ()))))
      return_trace (خطأ);
#جے_انت

    return_trace (سچ);
  }

  سانچا <قسم_ناں TLookup>
  منطقي subset (hb_subset_layout_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (نسخہ.major != 1) return_trace (خطأ);

    خودکار *باہرلا = c->subset_context->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->subset_context->serializer->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->نسخہ = نسخہ;

#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010002u)
    {
      إذا (unlikely (!c->subset_context->serializer->extend_ناپ (باہرلا, get_ناپ ())))
        return_trace (خطأ);

      باہرلا->scriptList = 0;
      باہرلا->featureList = 0;
      باہرلا->lookupList = 0;

      إذا (lookupList2)
	serialize_subset_وتھ (c->subset_context, باہرلا->lookupList2,
				 subset_lookup_list2_وتھ<TLookup> (),
				 هذا, c);
      وإلا
	serialize_subset_وتھ (c->subset_context, باہرلا->lookupList2,
				 subset_lookup_list_وتھ<TLookup> (),
				 هذا, c);

      إذا (featureList2)
	serialize_subset_وتھ (c->subset_context, باہرلا->featureList2,
				 subset_feature_list2_وتھ (),
				 هذا, c);
      وإلا
	serialize_subset_وتھ (c->subset_context, باہرلا->featureList2,
				 subset_feature_list_وتھ (),
				 هذا, c);

      إذا (scriptList2)
	serialize_subset_وتھ (c->subset_context, باہرلا->scriptList2, scriptList2, هذا, c);
      وإلا
	serialize_subset_وتھ (c->subset_context, باہرلا->scriptList2, scriptList, هذا, c);
    }
    وإلا
#جے_انت
    {
      باہرلا->subset_lookup_list_وتھ<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_وتھ<TLookup> (),
			     هذا,
			     c);

      باہرلا->subset_feature_list_وتھ ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_وتھ (),
			     هذا,
			     c);

      باہرلا->scriptList.serialize_subset (c->subset_context,
					scriptList,
					هذا,
					c);
    }

#جے_بنا_تعریف HB_NO_VAR
    إذا (نسخہ.منزل_int () >= 0x00010001u)
    {
      خودکار snapshot = c->subset_context->serializer->snapshot ();
      إذا (نسخہ.منزل_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&باہرلا->featureVars))
        return_trace (خطأ);

      // if all axes are pinned all feature vars are dropped.
      منطقي ret = !c->subset_context->plan->all_axes_pinned
                 && باہرلا->featureVars.serialize_subset (c->subset_context, featureVars, هذا, c);
      إذا (!ret)
      {
        باہرلا->featureVars = 0;
	إذا (نسخہ.منزل_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  باہرلا->نسخہ.major = 1;
	  باہرلا->نسخہ.minor = 0;
	}
      }
    }
#جے_انت

    return_trace (سچ);
  }

  بنانشانی صحيح get_lookup_گنتی () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2).len;
#جے_انت
    أرجع (هذا + lookupList).len;
  }
  ثابت Lookup& get_lookup (بنانشانی صحيح i) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2)[i];
#جے_انت
    أرجع (هذا + lookupList)[i];
  }
  ثابت FeatureVariations &get_feature_variations () ثابت
  {
    أرجع (نسخہ.منزل_int () >= 0x00010001u && hb_barrier ()) ? هذا+featureVars : Null (FeatureVariations);
  }

  منطقي has_انکڑے () ثابت { أرجع نسخہ.منزل_int (); }
  بنانشانی صحيح get_script_گنتی () ثابت
  { أرجع get_script_list ().len; }
  ثابت ٹیگ& get_script_ٹیگ (بنانشانی صحيح i) ثابت
  { أرجع get_script_list ().get_ٹیگ (i); }
  بنانشانی صحيح get_script_ٹیگ_سوچی (بنانشانی صحيح شروع_وتھ,
				بنانشانی صحيح *script_گنتی /* IN/OUT */,
				hb_ٹیگ_t     *script_ٹیگ_سوچی /*باہرلا*/) ثابت
  { أرجع get_script_list ().get_ٹیگ_سوچی (شروع_وتھ, script_گنتی, script_ٹیگ_سوچی); }
  ثابت Script& get_script (بنانشانی صحيح i) ثابت
  { أرجع get_script_list ()[i]; }
  منطقي لبھو_script_اشاریہ (hb_ٹیگ_t ٹیگ_2, بنانشانی صحيح *اشاریہ_2) ثابت
  { أرجع get_script_list ().لبھو_اشاریہ (ٹیگ_2, اشاریہ_2); }

  بنانشانی صحيح get_feature_گنتی () ثابت
  { أرجع get_feature_list ().len; }
  hb_ٹیگ_t get_feature_ٹیگ (بنانشانی صحيح i) ثابت
  { أرجع i == اشاریہ::NOT_لبھیا_اشاریہ ? HB_ٹیگ_NONE : get_feature_list ().get_ٹیگ (i); }
  بنانشانی صحيح get_feature_ٹیگ_سوچی (بنانشانی صحيح شروع_وتھ,
				 بنانشانی صحيح *feature_گنتی_2 /* IN/OUT */,
				 hb_ٹیگ_t     *feature_ٹیگ_سوچی /*باہرلا*/) ثابت
  { أرجع get_feature_list ().get_ٹیگ_سوچی (شروع_وتھ, feature_گنتی_2, feature_ٹیگ_سوچی); }
  ثابت Feature& get_feature (بنانشانی صحيح i) ثابت
  { أرجع get_feature_list ()[i]; }
  منطقي لبھو_feature_اشاریہ (hb_ٹیگ_t ٹیگ_2, بنانشانی صحيح *اشاریہ_2) ثابت
  { أرجع get_feature_list ().لبھو_اشاریہ (ٹیگ_2, اشاریہ_2); }

  منطقي لبھو_variations_اشاریہ (ثابت صحيح *coords, بنانشانی صحيح عدد_coords,
			      بنانشانی صحيح *اشاریہ_2,
			      شے_Var_Store_Instancer *instancer) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    *اشاریہ_2 = FeatureVariations::NOT_لبھیا_اشاریہ;
    أرجع خطأ;
#جے_انت
    أرجع get_feature_variations ().لبھو_اشاریہ (coords, عدد_coords, اشاریہ_2, instancer);
  }
  منطقي get_feature_variations_state (ثابت صحيح *coords,
				     بنانشانی صحيح عدد_coords,
				     بنانشانی صحيح *اشاریہ_2,
				     شے_Var_Store_Instancer *instancer,
				     hb_سمتیہ_t<بنانشانی> *lookup_conditions) ثابت
  {
    lookup_conditions->reset ();
#جے_تعریف_شدہ HB_NO_VAR
    *اشاریہ_2 = FeatureVariations::NOT_لبھیا_اشاریہ;
    أرجع سچ;
#وإلا
    لبھو_variations_اشاریہ (coords, عدد_coords, اشاریہ_2, instancer);
    أرجع get_feature_variations ().collect_lookup_conditions (
	coords,
	عدد_coords,
	instancer,
	lookup_conditions);
#جے_انت
  }
  منطقي get_lookup_variations (بنانشانی feature_اشاریہ,
                              بنانشانی variations_اشاریہ,
                              hb_sorted_array_t<ثابت بنانشانی> conditions,
                              hb_سیٹ_t *lookup_indexes) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع خطأ;
#وإلا
    أرجع get_feature_variations ().get_lookup_variations (
	feature_اشاریہ, get_feature_variation (feature_اشاریہ, variations_اشاریہ),
	conditions, lookup_indexes);
#جے_انت
  }
  ثابت Feature& get_feature_variation (بنانشانی صحيح feature_اشاریہ,
					بنانشانی صحيح variations_اشاریہ) ثابت
  {
#جے_بنا_تعریف HB_NO_VAR
    إذا (FeatureVariations::NOT_لبھیا_اشاریہ != variations_اشاریہ &&
	نسخہ.منزل_int () >= 0x00010001u)
    {
      ثابت Feature *feature = get_feature_variations ().لبھو_substitute (variations_اشاریہ,
									  feature_اشاریہ);
      إذا (feature)
	أرجع *feature;
    }
#جے_انت
    أرجع get_feature (feature_اشاریہ);
  }

  فراغ feature_variation_collect_lookups (ثابت hb_سیٹ_t *feature_indexes,
					  ثابت hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
					  hb_سیٹ_t       *lookup_indexes /*باہرلا*/) ثابت
  {
#جے_بنا_تعریف HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_ریکارڈ_cond_idx_map, lookup_indexes);
#جے_انت
  }

  فراغ collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#جے_بنا_تعریف HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#جے_انت
  }

#جے_بنا_تعریف HB_NO_VAR
  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#جے_انت

  سانچا <قسم_ناں TLookup>
  فراغ گھیرا_lookups (hb_face_t      *face,
			ثابت hb_سیٹ_t *glyphs,
			hb_سیٹ_t       *lookup_indexes /* IN/OUT */) ثابت
  {
    hb_سیٹ_t ویکھیا_lookups, inactive_lookups;
    hb_گھیرا_lookups_context_t c (face, glyphs, &ویکھیا_lookups, &inactive_lookups);

    c.سیٹ_recurse_func (TLookup::سانچا dispatch_recurse_func<hb_گھیرا_lookups_context_t>);

    لكل (بنانشانی lookup_اشاریہ : *lookup_indexes)
      reinterpret_cast<ثابت TLookup &> (get_lookup (lookup_اشاریہ)).گھیرا_lookups (&c, lookup_اشاریہ);

    hb_سیٹ_union (lookup_indexes, &ویکھیا_lookups);
    hb_سیٹ_گھٹاؤ (lookup_indexes, &inactive_lookups);
  }

  فراغ prune_langsys (ثابت hb_map_t *duplicate_feature_map,
                      ثابت hb_سیٹ_t *layout_scripts,
                      hb_hashmap_t<بنانشانی, hb::unique_ptr<hb_سیٹ_t>> *script_langsys_map,
                      hb_سیٹ_t       *new_feature_indexes /*باہرلا*/) ثابت
  {
    hb_prune_langsys_context_t c (هذا, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    بنانشانی گنتی = get_script_گنتی ();
    لكل (بنانشانی script_اشاریہ = 0; script_اشاریہ < گنتی; script_اشاریہ++)
    {
      ثابت ٹیگ& ٹیگ_2 = get_script_ٹیگ (script_اشاریہ);
      إذا (!layout_scripts->has (ٹیگ_2)) استمر;
      ثابت Script& s = get_script (script_اشاریہ);
      s.prune_langsys (&c, script_اشاریہ);
    }
  }

  فراغ prune_features (ثابت hb_map_t *lookup_indices, /*وچ*/
		       ثابت hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map, /*وچ*/
		       ثابت hb_hashmap_t<بنانشانی, ثابت Feature*> *feature_substitutes_map, /*وچ*/
		       hb_سیٹ_t       *feature_indices /* IN/OUT */) ثابت
  {
#جے_بنا_تعریف HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_سیٹ_t alternate_feature_indices;
    get_feature_variations ().گھیرا_features (lookup_indices, feature_ریکارڈ_cond_idx_map, &alternate_feature_indices);
    إذا (unlikely (alternate_feature_indices.وچ_error()))
    {
      feature_indices->err ();
      أرجع;
    }
#جے_انت

    لكل (بنانشانی i : hb_iter (feature_indices))
    {
      hb_ٹیگ_t ٹیگ_2 =  get_feature_ٹیگ (i);
      إذا (ٹیگ_2 == HB_ٹیگ ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        استمر;


      ثابت Feature *f = &(get_feature (i));
      ثابت Feature** p = خالی_سوچک;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      إذا (!f->featureParams.is_null () &&
          ٹیگ_2 == HB_ٹیگ ('s', 'i', 'z', 'e'))
        استمر;

      إذا (!f->intersects_lookup_indexes (lookup_indices)
#جے_بنا_تعریف HB_NO_VAR
          && !alternate_feature_indices.has (i)
#جے_انت
	  )
	feature_indices->del (i);
    }
  }

  فراغ collect_name_ids (ثابت hb_map_t *feature_اشاریہ_map,
                         hb_سیٹ_t *nameids_منزل_retain, /*باہرلا*/
                         ثابت hb_hashmap_t<بنانشانی, ثابت Feature*> *feature_substitutes_map,
                         ثابت hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map HB_بن_ورتیا,
                         ثابت hb_سیٹ_t *catch_all_features,
                         منطقي all_axes_pinned HB_بن_ورتیا) ثابت
  {
    بنانشانی گنتی = get_feature_گنتی ();
    لكل (بنانشانی i = 0 ; i < گنتی; i++)
    {
      إذا (!feature_اشاریہ_map->has (i)) استمر;
      hb_ٹیگ_t ٹیگ_2 = get_feature_ٹیگ (i);
      ثابت Feature *feature = &get_feature (i);
      ثابت Feature **substitute;
      إذا (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        إذا (catch_all_features->has (i))
          get_feature (i).collect_name_ids (ٹیگ_2, nameids_منزل_retain);
      }
      feature->collect_name_ids (ٹیگ_2, nameids_منزل_retain);
    }
#جے_بنا_تعریف HB_NO_VAR
    إذا (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_اشاریہ_map, feature_ریکارڈ_cond_idx_map,
                                                  [هذا] (بنانشانی i) { أرجع get_feature_ٹیگ (i); },
                                                  nameids_منزل_retain);
#جے_انت
  }

  سانچا <قسم_ناں T>
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
	هذا->جدول = hb_blob_get_خالی_تھاواں ();
      }

      هذا->lookup_گنتی = جدول->get_lookup_گنتی ();

      هذا->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (هذا->lookup_گنتی, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->lookup_گنتی = 0;
	هذا->جدول.destroy ();
	هذا->جدول = hb_blob_get_خالی_تھاواں ();
      }
    }
    ~accelerator_t ()
    {
      لكل (بنانشانی صحيح i = 0; i < هذا->lookup_گنتی; i++)
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

    hb_ot_layout_lookup_accelerator_t *get_accel (بنانشانی lookup_اشاریہ) ثابت
    {
      إذا (unlikely (lookup_اشاریہ >= lookup_گنتی)) أرجع خالی_سوچک;

    retry:
      خودکار *accel = accels[lookup_اشاریہ].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (جدول->get_lookup (lookup_اشاریہ));
	إذا (unlikely (!accel))
	  أرجع خالی_سوچک;

	إذا (unlikely (!accels[lookup_اشاریہ].cmpexch (خالی_سوچک, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> جدول;
    بنانشانی صحيح lookup_گنتی;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  محمي:
  سانچا <قسم_ناں TLookup>
  ثابت Offset16_منزل<TLookupList<TLookup>>& typed_lookup_list_وتھ () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<TLookupList<TLookup>>&> (lookupList); }

#جے_بنا_تعریف HB_NO_BEYOND_64K
  سانچا <قسم_ناں TLookup>
  ثابت Offset32_منزل<TLookupList2<TLookup>>& typed_lookup_list2_وتھ () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<TLookupList2<TLookup>>&> (lookupList2); }
#جے_انت

  سانچا <قسم_ناں TLookup>
  ثابت Offset16_منزل<TLookup_وتھ_List<TLookup>>& subset_lookup_list_وتھ () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<TLookup_وتھ_List<TLookup>>&> (lookupList); }
  سانچا <قسم_ناں TLookup>
  Offset16_منزل<TLookup_وتھ_List<TLookup>>& subset_lookup_list_وتھ ()
  { أرجع reinterpret_cast<Offset16_منزل<TLookup_وتھ_List<TLookup>>&> (lookupList); }

#جے_بنا_تعریف HB_NO_BEYOND_64K
  سانچا <قسم_ناں TLookup>
  ثابت Offset32_منزل<TLookup_وتھ_List2<TLookup>>& subset_lookup_list2_وتھ () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<TLookup_وتھ_List2<TLookup>>&> (lookupList2); }
#جے_انت

  ثابت Offset16_منزل<ریکارڈ_List_دا_Feature>& subset_feature_list_وتھ () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<ریکارڈ_List_دا_Feature>&> (featureList); }
  Offset16_منزل<ریکارڈ_List_دا_Feature>& subset_feature_list_وتھ ()
  { أرجع reinterpret_cast<Offset16_منزل<ریکارڈ_List_دا_Feature>&> (featureList); }

#جے_بنا_تعریف HB_NO_BEYOND_64K
  ثابت Offset32_منزل<ریکارڈ_List_دا_Feature>& subset_feature_list2_وتھ () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<ریکارڈ_List_دا_Feature>&> (featureList2); }
#جے_انت

  ثابت فراغ* get_lookup_list_میدان_وتھ () ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (نسخہ.منزل_int () >= 0x00010002u && lookupList2) أرجع &lookupList2;
#جے_انت
    أرجع &lookupList;
  }

  سانچا <قسم_ناں وتھ_باہرلا, قسم_ناں وتھ_وچ, قسم_ناں ادھار_3, قسم_ناں ...Ts>
  ساكن منطقي serialize_subset_وتھ (hb_subset_context_t *c,
				       وتھ_باہرلا& باہرلا,
				       ثابت وتھ_وچ& وچ,
				       ثابت ادھار_3 *ادھار,
				       Ts&&... ds)
  {
    باہرلا = 0;
    إذا (وچ.is_null ()) أرجع خطأ;

    خودکار *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (ادھار+وچ, std::forward<Ts> (ds)...);
    إذا (ret) s->جوڑو_link (باہرلا, s->نکالو_pack ());
    وإلا s->نکالو_discard ();
    أرجع ret;
  }

  Fixed_نسخہ<>نسخہ;	/* Version of the GSUB/GPOS table--initially set
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
  Offset32_منزل<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32_منزل<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32_منزل<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#جے_انت
  عام:
  DEFINE_ناپ_MIN (10);
};


} /* namespace OT */


#جے_انت /* HB_OT_LAYOUT_GSUBGPOS_HH */
