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

#اگر_غیر_تعریف_شدہ HB_OT_LAYOUT_GSUBGPOS_HH
#تعریف_کرو HB_OT_LAYOUT_GSUBGPOS_HH

#شامل "hb.hh"
#شامل "hb-buffer.hh"
#شامل "hb-map.hh"
#شامل "hb-set.hh"
#شامل "hb-ot-map.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-depend-data.hh"


نامخلا OT {


ساخت hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, منطقی>
{
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { واپس obj.intersects (یہ->glyphs); }
  ساکن return_t default_return_قدر () { واپس جھوٹ; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس r; }

  ثابت hb_سیٹ_t *glyphs;

  hb_intersects_context_t (ثابت hb_سیٹ_t *glyphs_) :
                            glyphs (glyphs_) {}
};

ساخت hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, منطقی>
{
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { واپس obj.may_have_non_1to1 (); }
  ساکن return_t default_return_قدر () { واپس جھوٹ; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس r; }
};

ساخت hb_احاطہ_context_t :
       hb_dispatch_context_t<hb_احاطہ_context_t>
{
  قسمتعریف return_t (*recurse_func_t) (hb_احاطہ_context_t *c, غیرعلامتی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indicies, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ);
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { obj.احاطہ (یہ); واپس hb_خالی_خانے_t (); }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }
  خالی recurse (غیرعلامتی lookup_اشاریہ, hb_سیٹ_t *covered_seq_indicies, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ)
  {
    اگر (unlikely (nesting_level_بایاں == 0 || !recurse_func))
      واپس;

    nesting_level_بایاں--;
    recurse_func (یہ, lookup_اشاریہ, covered_seq_indicies, seq_اشاریہ, انتہا_اشاریہ);
    nesting_level_بایاں++;
  }

  خالی reset_lookup_دورہ_گنتی ()
  { lookup_گنتی = 0; }

  منطقی lookup_حد_بندی_exceeded ()
  { واپس lookup_گنتی > HB_MAX_LOOKUP_دورہ_گنتی; }

  منطقی should_دورہ_lookup (غیرعلامتی صحیح lookup_اشاریہ)
  {
    اگر (lookup_گنتی++ > HB_MAX_LOOKUP_دورہ_گنتی)
      واپس جھوٹ;

    اگر (is_lookup_مکمل (lookup_اشاریہ))
      واپس جھوٹ;

    واپس سچ;
  }

  منطقی is_lookup_مکمل (غیرعلامتی صحیح lookup_اشاریہ)
  {
    اگر (unlikely (مکمل_lookups_glyph_گنتی->اندر_error () ||
		  مکمل_lookups_glyph_سیٹ->اندر_error ()))
      واپس سچ;

    /* Have we visited this lookup with the current set of glyphs? */
    غیرعلامتی نکالو = glyphs->get_population ();
    اگر (unlikely (نکالو == HB_MAP_قدر_INVALID)) // Avoid sentinel interference.
      نکالو--;
    اگر (مکمل_lookups_glyph_گنتی->get (lookup_اشاریہ) != نکالو)
    {
      مکمل_lookups_glyph_گنتی->سیٹ (lookup_اشاریہ, نکالو);

      اگر (!مکمل_lookups_glyph_سیٹ->has (lookup_اشاریہ))
      {
	اگر (unlikely (!مکمل_lookups_glyph_سیٹ->سیٹ (lookup_اشاریہ, hb::unique_ptr<hb_سیٹ_t> {hb_سیٹ_create ()})))
	  واپس سچ;
      }

      مکمل_lookups_glyph_سیٹ->get (lookup_اشاریہ)->clear ();
    }

    hb_سیٹ_t *covered_glyph_سیٹ = مکمل_lookups_glyph_سیٹ->get (lookup_اشاریہ);
    اگر (unlikely (covered_glyph_سیٹ->اندر_error ()))
      واپس سچ;
    اگر (والد_فعال_glyphs ().is_subset (*covered_glyph_سیٹ))
      واپس سچ;

    covered_glyph_سیٹ->union_ (والد_فعال_glyphs ());
    واپس جھوٹ;
  }

  ثابت hb_سیٹ_t& پچھلا_والد_فعال_glyphs () {
    اگر (فعال_glyphs_ڈھیر.لمبائی <= 1)
      واپس *glyphs;

    واپس فعال_glyphs_ڈھیر[فعال_glyphs_ڈھیر.لمبائی - 2];
  }

  ثابت hb_سیٹ_t& والد_فعال_glyphs ()
  {
    اگر (!فعال_glyphs_ڈھیر)
      واپس *glyphs;

    واپس فعال_glyphs_ڈھیر.دم ();
  }

  hb_سیٹ_t* ڈالو_cur_فعال_glyphs ()
  {
    hb_سیٹ_t *s = فعال_glyphs_ڈھیر.push ();
    اگر (unlikely (فعال_glyphs_ڈھیر.اندر_error ()))
      واپس خالی_اشاریہ;
    واپس s;
  }

  منطقی نکالو_cur_مکمل_glyphs ()
  {
    اگر (!فعال_glyphs_ڈھیر)
      واپس جھوٹ;

    فعال_glyphs_ڈھیر.نکالو ();
    واپس سچ;
  }

  hb_face_t *face;
  hb_سیٹ_t *glyphs;
  hb_سیٹ_t output[1];
  hb_سمتیہ_t<hb_سیٹ_t> فعال_glyphs_ڈھیر;
  recurse_func_t recurse_func = خالی_اشاریہ;
  غیرعلامتی صحیح nesting_level_بایاں;

  hb_احاطہ_context_t (hb_face_t *face_,
			hb_سیٹ_t *glyphs_,
			hb_map_t *مکمل_lookups_glyph_گنتی_2,
			hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> *مکمل_lookups_glyph_سیٹ_2,
			غیرعلامتی صحیح nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_بایاں (nesting_level_بایاں_2),
			  مکمل_lookups_glyph_گنتی (مکمل_lookups_glyph_گنتی_2),
			  مکمل_lookups_glyph_سیٹ (مکمل_lookups_glyph_سیٹ_2)
  {}

  ~hb_احاطہ_context_t () { flush (); }

  خالی سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }

  خالی flush ()
  {
    output->del_range (face->get_عدد_glyphs (), HB_سیٹ_قدر_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    فعال_glyphs_ڈھیر.نکالو ();
    فعال_glyphs_ڈھیر.reset ();
  }

  نجی:
  hb_map_t *مکمل_lookups_glyph_گنتی;
  hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> *مکمل_lookups_glyph_سیٹ;
  غیرعلامتی صحیح lookup_گنتی = 0;
};



ساخت hb_احاطہ_lookups_context_t :
       hb_dispatch_context_t<hb_احاطہ_lookups_context_t>
{
  قسمتعریف return_t (*recurse_func_t) (hb_احاطہ_lookups_context_t *c, غیرعلامتی lookup_اشاریہ);
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { obj.احاطہ_lookups (یہ); واپس hb_خالی_خانے_t (); }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }
  خالی recurse (غیرعلامتی lookup_اشاریہ)
  {
    اگر (unlikely (nesting_level_بایاں == 0 || !recurse_func))
      واپس;

    /* Return if new lookup was recursed to before. */
    اگر (lookup_حد_بندی_exceeded ()
        || دیکھا_lookups->اندر_error ()
        || دیکھا_lookups->has (lookup_اشاریہ))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      واپس;

    nesting_level_بایاں--;
    recurse_func (یہ, lookup_اشاریہ);
    nesting_level_بایاں++;
  }

  خالی سیٹ_lookup_دیکھا (غیرعلامتی lookup_اشاریہ)
  { دیکھا_lookups->جوڑو (lookup_اشاریہ); }

  خالی سیٹ_lookup_inactive (غیرعلامتی lookup_اشاریہ)
  { inactive_lookups->جوڑو (lookup_اشاریہ); }

  منطقی lookup_حد_بندی_exceeded ()
  {
    منطقی ret = lookup_گنتی > HB_MAX_LOOKUP_دورہ_گنتی;
    اگر (ret)
      DEBUG_پیغام (SUBSET, خالی_اشاریہ, "lookup visit count limit exceeded in lookup closure!");
    واپس ret; }

  منطقی is_lookup_دیکھا (غیرعلامتی lookup_اشاریہ)
  {
    اگر (unlikely (lookup_گنتی++ > HB_MAX_LOOKUP_دورہ_گنتی))
    {
      DEBUG_پیغام (SUBSET, خالی_اشاریہ, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_گنتی, lookup_اشاریہ);
      واپس سچ;
    }

    اگر (unlikely (دیکھا_lookups->اندر_error ()))
      واپس سچ;

    واپس دیکھا_lookups->has (lookup_اشاریہ);
  }

  hb_face_t *face;
  ثابت hb_سیٹ_t *glyphs;
  recurse_func_t recurse_func;
  غیرعلامتی صحیح nesting_level_بایاں;

  hb_احاطہ_lookups_context_t (hb_face_t *face_,
				ثابت hb_سیٹ_t *glyphs_,
				hb_سیٹ_t *دیکھا_lookups_2,
				hb_سیٹ_t *inactive_lookups_,
				غیرعلامتی nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (خالی_اشاریہ),
				nesting_level_بایاں (nesting_level_بایاں_2),
				دیکھا_lookups (دیکھا_lookups_2),
				inactive_lookups (inactive_lookups_),
				lookup_گنتی (0) {}

  خالی سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }

  نجی:
  hb_سیٹ_t *دیکھا_lookups;
  hb_سیٹ_t *inactive_lookups;
  غیرعلامتی صحیح lookup_گنتی;
};

ساخت hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, منطقی>
{
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { واپس obj.would_apply (یہ); }
  ساکن return_t default_return_قدر () { واپس جھوٹ; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس r; }

  hb_face_t *face;
  ثابت hb_codepoint_t *glyphs;
  غیرعلامتی صحیح len;
  منطقی zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ثابت hb_codepoint_t *glyphs_,
			    غیرعلامتی صحیح len_,
			    منطقی zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

ساخت hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  قسمتعریف return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, غیرعلامتی صحیح lookup_اشاریہ);
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { obj.collect_glyphs (یہ); واپس hb_خالی_خانے_t (); }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }
  خالی recurse (غیرعلامتی صحیح lookup_اشاریہ)
  {
    اگر (unlikely (nesting_level_بایاں == 0 || !recurse_func))
      واپس;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    اگر (output == hb_سیٹ_get_خالی_خانے ())
      واپس;

    /* Return if new lookup was recursed to before. */
    اگر (recursed_lookups->has (lookup_اشاریہ))
      واپس;

    hb_سیٹ_t *old_before = before;
    hb_سیٹ_t *old_داخلہ  = داخلہ;
    hb_سیٹ_t *old_after  = after;
    before = داخلہ = after = hb_سیٹ_get_خالی_خانے ();

    nesting_level_بایاں--;
    recurse_func (یہ, lookup_اشاریہ);
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
  غیرعلامتی صحیح nesting_level_بایاں;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_سیٹ_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_سیٹ_t  *glyphs_داخلہ,  /* OUT.  May be NULL */
			       hb_سیٹ_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_سیٹ_t  *glyphs_output, /* OUT.  May be NULL */
			       غیرعلامتی صحیح nesting_level_بایاں_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_سیٹ_get_خالی_خانے ()),
			      داخلہ  (glyphs_داخلہ  ? glyphs_داخلہ  : hb_سیٹ_get_خالی_خانے ()),
			      after  (glyphs_after  ? glyphs_after  : hb_سیٹ_get_خالی_خانے ()),
			      output (glyphs_output ? glyphs_output : hb_سیٹ_get_خالی_خانے ()),
			      recurse_func (خالی_اشاریہ),
			      recursed_lookups (hb_سیٹ_create ()),
			      nesting_level_بایاں (nesting_level_بایاں_2) {}
  ~hb_collect_glyphs_context_t () { hb_سیٹ_destroy (recursed_lookups); }

  خالی سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ساخت hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  شمار glyph_سیٹ_type_t
  {
    GLYPH_سیٹ_CLASS,
    GLYPH_سیٹ_COVERAGE,
    GLYPH_سیٹ_SINGLE
  };

  ساکن constexpr hb_codepoint_t SINGLE_GLYPH_فعال_FLAG = 0x80000000u;

  ساخت glyph_سیٹ_cache_کلید_t
  {
    glyph_سیٹ_cache_کلید_t () = طےشدہ;
    glyph_سیٹ_cache_کلید_t (ثابت خالی *ڈیٹا_2, غیرعلامتی قدر_3, glyph_سیٹ_type_t type_)
      : ڈیٹا (ڈیٹا_2), value (قدر_3), type (type_) {}

    منطقی عامل == (ثابت glyph_سیٹ_cache_کلید_t &o) ثابت
    { واپس ڈیٹا == o.ڈیٹا && value == o.value && type == o.type; }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) ڈیٹا);
      current = current * 31 + hb_hash (value);
      واپس current * 31 + hb_hash ((غیرعلامتی) type);
    }

    ثابت خالی *ڈیٹا = خالی_اشاریہ;
    غیرعلامتی value = 0;
    glyph_سیٹ_type_t type = GLYPH_سیٹ_CLASS;
  };

  ساخت context_سیٹ_cache_کلید_t
  {
    context_سیٹ_cache_کلید_t () = طےشدہ;
    context_سیٹ_cache_کلید_t (ثابت خالی *قاعدہ,
			     ثابت خالی *ریکارڈ_3,
			     ثابت خالی *ڈیٹا_2,
			     hb_codepoint_t فعال_glyphs,
			     غیرعلامتی قدر_3,
			     غیرعلامتی format_)
      : قاعدہ_2 (قاعدہ), ریکارڈ (ریکارڈ_3), ڈیٹا (ڈیٹا_2), فعال_glyphs_2 (فعال_glyphs),
	value (قدر_3), format (format_) {}

    منطقی عامل == (ثابت context_سیٹ_cache_کلید_t &o) ثابت
    {
      واپس قاعدہ_2 == o.قاعدہ_2 &&
	     ریکارڈ == o.ریکارڈ &&
	     ڈیٹا == o.ڈیٹا &&
	     فعال_glyphs_2 == o.فعال_glyphs_2 &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) قاعدہ_2);
      current = current * 31 + hb_hash ((uintptr_t) ریکارڈ);
      current = current * 31 + hb_hash ((uintptr_t) ڈیٹا);
      current = current * 31 + hb_hash (فعال_glyphs_2);
      current = current * 31 + hb_hash (value);
      واپس current * 31 + hb_hash (format);
    }

    ثابت خالی *قاعدہ_2 = خالی_اشاریہ;
    ثابت خالی *ریکارڈ = خالی_اشاریہ;
    ثابت خالی *ڈیٹا = خالی_اشاریہ;
    hb_codepoint_t فعال_glyphs_2 = HB_CODEPOINT_INVALID;
    غیرعلامتی value = 0;
    غیرعلامتی format = 0;
  };

  ساخت recurse_کلید_t
  {
    recurse_کلید_t () = طےشدہ;
    recurse_کلید_t (غیرعلامتی lookup_اشاریہ_3,
                   hb_codepoint_t فعال_glyphs,
                   hb_codepoint_t recurse_راستہ,
                   hb_codepoint_t context_سیٹ_2)
      : lookup_اشاریہ (lookup_اشاریہ_3),
        فعال_glyphs_2 (فعال_glyphs),
        recurse_راستہ_2 (recurse_راستہ),
        context_سیٹ (context_سیٹ_2) {}

    منطقی عامل == (ثابت recurse_کلید_t &o) ثابت
    {
      واپس lookup_اشاریہ == o.lookup_اشاریہ &&
             فعال_glyphs_2 == o.فعال_glyphs_2 &&
             recurse_راستہ_2 == o.recurse_راستہ_2 &&
             context_سیٹ == o.context_سیٹ;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_اشاریہ)) * 16777619;
      current = (current ^ hb_hash (فعال_glyphs_2)) * 16777619;
      current = (current ^ hb_hash (recurse_راستہ_2)) * 16777619;
      current = (current ^ hb_hash (context_سیٹ)) * 16777619;
      واپس current;
    }

    غیرعلامتی lookup_اشاریہ = 0;
    hb_codepoint_t فعال_glyphs_2 = 0;
    hb_codepoint_t recurse_راستہ_2 = 0;
    hb_codepoint_t context_سیٹ = HB_CODEPOINT_INVALID;
  };

  قسمتعریف return_t (*recurse_func_t) (hb_depend_context_t *c, غیرعلامتی lookup_اشاریہ, hb_ثنائی_ہندسہ_صفحہ_t *covered_seq_indices, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { obj.depend (یہ); واپس hb_خالی_خانے_t (); }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }

  خالی recurse (غیرعلامتی lookup_idx, hb_ثنائی_ہندسہ_صفحہ_t *covered_seq_indices, غیرعلامتی seq_اشاریہ, غیرعلامتی انتہا_اشاریہ)
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
    اگر (lookups_نظر_آیا.has (lookup_idx)) واپس;
    lookups_نظر_آیا.جوڑو (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t والد_راستہ = recurse_راستہ_2;
    uint64_t راستہ_کلید = (uint64_t) والد_راستہ << 32 | lookup_idx;
    hb_codepoint_t *راستہ = خالی_اشاریہ;
    hb_codepoint_t راستہ_اشاریہ;
    اگر (!recurse_راستہ_مقصد_اشاریہ.has (راستہ_کلید, &راستہ))
    {
      راستہ_اشاریہ = recurse_راستہ_مقصد_اشاریہ.get_population () + 1;
      اگر (unlikely (!recurse_راستہ_مقصد_اشاریہ.سیٹ (راستہ_کلید, راستہ_اشاریہ)))
      {
	depend_ڈیٹا->fail ();
	lookups_نظر_آیا.del (lookup_idx);
	واپس;
      }
    }
    ورنہ
      راستہ_اشاریہ = *راستہ;
    recurse_راستہ_2 = راستہ_اشاریہ;
    recurse_func (یہ, lookup_idx, covered_seq_indices, seq_اشاریہ, انتہا_اشاریہ);
    recurse_راستہ_2 = والد_راستہ;
    lookups_نظر_آیا.del (lookup_idx);
  }

  ساخت فعال_glyphs_چوکھٹا_t
  {
    hb_سیٹ_t glyphs;
    hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
    منطقی use_جڑ = جھوٹ;
  };

  ثابت hb_سیٹ_t& والد_فعال_glyphs ()
  {
    اگر (!فعال_glyphs_ڈھیر_گہرائی)
      واپس *glyphs;

    ثابت فعال_glyphs_چوکھٹا_t &چوکھٹا = فعال_glyphs_ڈھیر[فعال_glyphs_ڈھیر_گہرائی - 1];
    واپس چوکھٹا.use_جڑ ? *glyphs : چوکھٹا.glyphs;
  }

  hb_سیٹ_t* ڈالو_cur_فعال_glyphs (hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID)
  {
    اگر (فعال_glyphs_ڈھیر_گہرائی == فعال_glyphs_ڈھیر.لمبائی &&
	unlikely (!فعال_glyphs_ڈھیر.ڈالو_or_fail ()))
      واپس خالی_اشاریہ;

    فعال_glyphs_چوکھٹا_t &چوکھٹا = فعال_glyphs_ڈھیر[فعال_glyphs_ڈھیر_گہرائی++];
    چوکھٹا.glyphs.clear ();
    چوکھٹا.فعال_idx = فعال_idx;
    چوکھٹا.use_جڑ = جھوٹ;
    واپس &چوکھٹا.glyphs;
  }

  منطقی ڈالو_جڑ_فعال_glyphs ()
  {
    اگر (فعال_glyphs_ڈھیر_گہرائی == فعال_glyphs_ڈھیر.لمبائی &&
	unlikely (!فعال_glyphs_ڈھیر.ڈالو_or_fail ()))
      واپس جھوٹ;

    فعال_glyphs_ڈھیر[فعال_glyphs_ڈھیر_گہرائی++].use_جڑ = سچ;
    واپس سچ;
  }

  منطقی نکالو_cur_مکمل_glyphs ()
  {
    اگر (!فعال_glyphs_ڈھیر_گہرائی)
      واپس جھوٹ;

    فعال_glyphs_ڈھیر_گہرائی--;
    واپس سچ;
  }

  منطقی get_والد_فعال_glyphs_اشاریہ (hb_codepoint_t *فعال_idx)
  {
    اگر (!فعال_glyphs_ڈھیر_گہرائی ||
	فعال_glyphs_ڈھیر[فعال_glyphs_ڈھیر_گہرائی - 1].use_جڑ)
    {
      *فعال_idx = HB_CODEPOINT_INVALID;
      واپس سچ;
    }

    فعال_glyphs_چوکھٹا_t &چوکھٹا = فعال_glyphs_ڈھیر[فعال_glyphs_ڈھیر_گہرائی - 1];
    *فعال_idx = چوکھٹا.فعال_idx;
    اگر (*فعال_idx != HB_CODEPOINT_INVALID)
      واپس سچ;

    hb_codepoint_t glyph;
    اگر (چوکھٹا.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_فعال_FLAG))
      *فعال_idx = glyph | SINGLE_GLYPH_فعال_FLAG;
    ورنہ اگر (!ڈھونڈو_or_create_recurse_سیٹ (چوکھٹا.glyphs, فعال_idx))
      واپس جھوٹ;

    چوکھٹا.فعال_idx = *فعال_idx;
    واپس سچ;
  }

  منطقی get_context_سیٹ_cache_کلید (ثابت خالی *قاعدہ_2,
				  ثابت خالی *ریکارڈ,
				  ثابت خالی *ڈیٹا,
				  غیرعلامتی value,
				  غیرعلامتی format,
				  context_سیٹ_cache_کلید_t *key)
  {
    hb_codepoint_t فعال_idx;
    اگر (!get_والد_فعال_glyphs_اشاریہ (&فعال_idx))
      واپس جھوٹ;
    *key = context_سیٹ_cache_کلید_t (قاعدہ_2, ریکارڈ, ڈیٹا, فعال_idx, value, format);
    واپس سچ;
  }

  منطقی get_recurse_کلید (غیرعلامتی lookup_idx, recurse_کلید_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t فعال_idx;
    اگر (!get_والد_فعال_glyphs_اشاریہ (&فعال_idx))
      واپس جھوٹ;

    *key = recurse_کلید_t (lookup_idx,
                          فعال_idx,
                          recurse_راستہ_2,
                          depend_ڈیٹا->current_context_سیٹ_اشاریہ);
    واپس !completed_recursions.has (*key);
  }

  خالی اختتام_recurse (ثابت recurse_کلید_t &key)
  { depend_ڈیٹا->check_success (completed_recursions.سیٹ (key, سچ)); }

  خالی reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_راستہ_مقصد_اشاریہ.clear ();
    recurse_راستہ_2 = 0;
  }

  منطقی ڈھونڈو_or_create_recurse_سیٹ (ثابت hb_سیٹ_t &سیٹ, hb_codepoint_t *اشاریہ_2)
  {
    hb_codepoint_t *existing = خالی_اشاریہ;
    اگر (recurse_سیٹ_مقصد_اشاریہ.has (&سیٹ, &existing))
    {
      *اشاریہ_2 = *existing;
      واپس سچ;
    }

    hb_سیٹ_t *copy = hb_object_create<hb_سیٹ_t> ();
    اگر (unlikely (!copy))
      واپس depend_ڈیٹا->fail ();
    copy->سیٹ (سیٹ);
    اگر (unlikely (copy->اندر_error ()))
    {
      hb_سیٹ_destroy (copy);
      واپس depend_ڈیٹا->fail ();
    }

    *اشاریہ_2 = recurse_sets.لمبائی;
    اگر (unlikely (*اشاریہ_2 & SINGLE_GLYPH_فعال_FLAG))
    {
      hb_سیٹ_destroy (copy);
      واپس depend_ڈیٹا->fail ();
    }
    اگر (unlikely (!recurse_sets.ڈالو_or_fail (hb::unique_ptr<hb_سیٹ_t> {copy})))
      واپس depend_ڈیٹا->fail ();
    اگر (unlikely (!recurse_سیٹ_مقصد_اشاریہ.سیٹ (recurse_sets[*اشاریہ_2].get (), *اشاریہ_2)))
      واپس depend_ڈیٹا->fail ();
    واپس سچ;
  }

  ثابت hb_سیٹ_t *ڈھونڈو_or_create_glyph_سیٹ (ثابت خالی *ڈیٹا,
                                             غیرعلامتی value,
                                             glyph_سیٹ_type_t type);
  hb_codepoint_t ڈھونڈو_or_create_cached_context_سیٹ (ثابت hb_سیٹ_t *سیٹ);

  ساخت depend_scratch_t
  {
    خالی clear ()
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
    hb_سمتیہ_t<غیرعلامتی> compact_workspace;
    depend_scratch_t *next = خالی_اشاریہ;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    اگر (scratch)
      depend_scratch_pool = scratch->next;
    ورنہ
    {
      scratch = (depend_scratch_t *) hb_malloc (حجم (depend_scratch_t));
      اگر (unlikely (!scratch))
      {
        depend_ڈیٹا->fail ();
        واپس خالی_اشاریہ;
      }
      نیا (scratch) depend_scratch_t ();
    }

    scratch->next = خالی_اشاریہ;
    scratch->clear ();
    واپس scratch;
  }

  خالی release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_ڈیٹا_builder_t *depend_ڈیٹا;
  hb_face_t *face;
  hb_سیٹ_t *glyphs;
  hb_سمتیہ_t<فعال_glyphs_چوکھٹا_t> فعال_glyphs_ڈھیر;
  غیرعلامتی فعال_glyphs_ڈھیر_گہرائی = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_اشاریہ = HB_CODEPOINT_INVALID;
  hb_ثنائی_ہندسہ_سیٹ_t lookups_نظر_آیا;
  hb_سمتیہ_t<hb::unique_ptr<hb_سیٹ_t>> recurse_sets;
  hb_hashmap_t<ثابت hb_سیٹ_t *, hb_codepoint_t> recurse_سیٹ_مقصد_اشاریہ;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_راستہ_مقصد_اشاریہ;
  hb_codepoint_t recurse_راستہ_2 = 0;
  hb_hashmap_t<recurse_کلید_t, منطقی> completed_recursions;
  hb_سمتیہ_t<hb::unique_ptr<hb_سیٹ_t>> glyph_sets;
  hb_hashmap_t<glyph_سیٹ_cache_کلید_t, hb_codepoint_t> glyph_سیٹ_مقصد_اشاریہ;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_سیٹ_indices;
  hb_hashmap_t<context_سیٹ_cache_کلید_t, hb_codepoint_t> context_سیٹ_cache;
  depend_scratch_t *depend_scratch_pool = خالی_اشاریہ;

  hb_depend_context_t (hb_depend_ڈیٹا_builder_t *depend_ڈیٹا_2,
                       hb_face_t *face_,
                       hb_سیٹ_t *glyphs_)
                      : depend_ڈیٹا (depend_ڈیٹا_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (خالی_اشاریہ) {}
  ~hb_depend_context_t ()
  {
    جب (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  خالی سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

سانچہ <قسم_نام سیٹ_t>
ساخت hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<سیٹ_t>, ثابت Coverage &>
{
  قسمتعریف ثابت Coverage &return_t; // Stoopid that we have to dupe this here.
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { واپس obj.get_coverage (); }
  ساکن return_t default_return_قدر () { واپس Null (Coverage); }
  منطقی stop_sublookup_iteration (return_t r) ثابت
  {
    r.collect_coverage (سیٹ);
    واپس جھوٹ;
  }

  hb_collect_coverage_context_t (سیٹ_t *سیٹ_2) :
				   سیٹ (سیٹ_2) {}

  سیٹ_t *سیٹ;
};

ساخت matcher_t
{
  قسمتعریف منطقی (*مطابقت_func_t) (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا);

  سانچہ <قسم_نام context_t>
  خالی init (ثابت context_t *c, منطقی context_مطابقت = جھوٹ)
  {
    سیٹ_مطابقت_func (خالی_اشاریہ, خالی_اشاریہ);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->جدول_اشاریہ == 1 || (context_مطابقت && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_مطابقت || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->جدول_اشاریہ == 1;
    نقاب = context_مطابقت ? -1 : c->lookup_نقاب;
    /* Per syllable matching is only for GSUB. */
    فی_syllable = c->جدول_اشاریہ == 0 && c->فی_syllable;
    syllable = 0;
  }

  خالی سیٹ_مطابقت_func (مطابقت_func_t مطابقت_func,
		       ثابت خالی *مطابقت_ڈیٹا)
  { مطابقت_func_2 = مطابقت_func; مطابقت_ڈیٹا_2 = مطابقت_ڈیٹا; }

  شمار may_مطابقت_t {
    مطابقت_NO,
    مطابقت_YES,
    مطابقت_MAYBE
  };

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  may_مطابقت_t may_مطابقت (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_ڈیٹا) ثابت
  {
    اگر (!(info.نقاب & نقاب) ||
	(فی_syllable && syllable && syllable != info.syllable ()))
      واپس مطابقت_NO;

    اگر (مطابقت_func_2)
      واپس مطابقت_func_2 (info, glyph_ڈیٹا, مطابقت_ڈیٹا_2) ? مطابقت_YES : مطابقت_NO;

    واپس مطابقت_MAYBE;
  }

  شمار may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  سانچہ <قسم_نام context_t>
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  may_skip_t may_skip (ثابت context_t *c,
		       ثابت hb_glyph_info_t &info) ثابت
  {
    اگر (!c->check_glyph_property (&info, lookup_props))
      واپس SKIP_YES;

    اگر (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      واپس SKIP_MAYBE;

    واپس SKIP_NO;
  }

  عوامی:
  غیرعلامتی صحیح lookup_props = 0;
  hb_نقاب_t نقاب = -1;
  منطقی ignore_zwnj = جھوٹ;
  منطقی ignore_zwj = جھوٹ;
  منطقی ignore_hidden = جھوٹ;
  منطقی فی_syllable = جھوٹ;
  uint8_t syllable = 0;
  مطابقت_func_t مطابقت_func_2 = خالی_اشاریہ;
  ثابت خالی *مطابقت_ڈیٹا_2 = خالی_اشاریہ;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
ساخت glyph_ڈیٹا_t
{
  سانچہ <قسم_نام T>
  glyph_ڈیٹا_t (ثابت T *ڈیٹا_2) : ڈیٹا (ڈیٹا_2)
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
                              , wide (حجم (T) == 3)
#اگر_ختم
  {
    static_assert (حجم (T) == 2 || حجم (T) == 3, "invalid glyph data size");
  }

  غیرعلامتی عامل [] (غیرعلامتی i) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (wide)
      واپس ((ثابت HBUINT24 *) ڈیٹا)[i];
#اگر_ختم
    واپس ((ثابت HBUINT16 *) ڈیٹا)[i];
  }

  ثابت خالی *ڈیٹا;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  منطقی wide;
#اگر_ختم
};

سانچہ <قسم_نام context_t>
ساخت skipping_iterator_t
{
  خالی init (context_t *c_, منطقی context_مطابقت = جھوٹ)
  {
    c = c_;
    انتہا = c->بفر->len;
    مطابقت_glyph_data16 = خالی_اشاریہ;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    مطابقت_glyph_data24 = خالی_اشاریہ;
#اگر_ختم
    matcher.init (c, context_مطابقت);
  }
  خالی سیٹ_lookup_props (غیرعلامتی صحیح lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  خالی سیٹ_مطابقت_func (matcher_t::مطابقت_func_t مطابقت_func,
		       ثابت خالی *مطابقت_ڈیٹا)
  {
    matcher.سیٹ_مطابقت_func (مطابقت_func, مطابقت_ڈیٹا);
  }
  خالی سیٹ_glyph_ڈیٹا (glyph_ڈیٹا_t glyph_ڈیٹا)
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    مطابقت_glyph_data16 = glyph_ڈیٹا.wide ? خالی_اشاریہ : (ثابت HBUINT16 *) glyph_ڈیٹا.ڈیٹا;
    مطابقت_glyph_data24 = glyph_ڈیٹا.wide ? (ثابت HBUINT24 *) glyph_ڈیٹا.ڈیٹا : خالی_اشاریہ;
#ورنہ
    مطابقت_glyph_data16 = (ثابت HBUINT16 *) glyph_ڈیٹا.ڈیٹا;
#اگر_ختم
  }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  خالی reset (غیرعلامتی صحیح آغاز_اشاریہ_2)
  {
    // For GSUB forward iterator
    idx = آغاز_اشاریہ_2;
    انتہا = c->بفر->len;
    matcher.syllable = c->بفر->cur().syllable();
  }
  خالی reset_عقب (غیرعلامتی صحیح آغاز_اشاریہ_2, منطقی from_برآمد_بفر = جھوٹ)
  {
    // For GSUB backward iterator
    idx = آغاز_اشاریہ_2;
    matcher.syllable = c->بفر->cur().syllable();
  }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  خالی reset_fast (غیرعلامتی صحیح آغاز_اشاریہ_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = آغاز_اشاریہ_2;
  }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  matcher_t::may_skip_t may_skip (ثابت hb_glyph_info_t &info) ثابت
  { واپس matcher.may_skip (c, info); }

  شمار مطابقت_t {
    مطابقت_2,
    NOT_مطابقت,
    SKIP
  };

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  مطابقت_t مطابقت (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    اگر (unlikely (skip == matcher_t::SKIP_YES))
      واپس SKIP;

    matcher_t::may_مطابقت_t مطابقت = matcher.may_مطابقت (info, get_glyph_ڈیٹا ());
    اگر (مطابقت == matcher_t::مطابقت_YES ||
	(مطابقت == matcher_t::مطابقت_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      واپس مطابقت_2;

    اگر (skip == matcher_t::SKIP_NO)
      واپس NOT_مطابقت;

    واپس SKIP;
  }

  منطقی next (غیرعلامتی *unsafe_مقصد = خالی_اشاریہ)
  {
    خودکار *info = c->بفر->info;
    ثابت علامتی stop = (علامتی) انتہا - 1;
    جب ((علامتی) idx < stop)
    {
      idx++;
      انتخاب (مطابقت (info[idx]))
      {
	حالت مطابقت_2:
	{
	  advance_glyph_ڈیٹا ();
	  واپس سچ;
	}
	حالت NOT_مطابقت:
	{
	  اگر (unsafe_مقصد)
	    *unsafe_مقصد = idx + 1;
	  واپس جھوٹ;
	}
	حالت SKIP:
	  جاری;
      }
    }
    اگر (unsafe_مقصد)
      *unsafe_مقصد = انتہا;
    واپس جھوٹ;
  }
  منطقی prev (غیرعلامتی *unsafe_from = خالی_اشاریہ)
  {
    خودکار *برآمد_info = c->بفر->برآمد_info;
    ثابت غیرعلامتی stop = 0;
    جب (idx > stop)
    {
      idx--;
      انتخاب (مطابقت (برآمد_info[idx]))
      {
	حالت مطابقت_2:
	{
	  advance_glyph_ڈیٹا ();
	  واپس سچ;
	}
	حالت NOT_مطابقت:
	{
	  اگر (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  واپس جھوٹ;
	}
	حالت SKIP:
	  جاری;
      }
    }
    اگر (unsafe_from)
      *unsafe_from = 0;
    واپس جھوٹ;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_ڈیٹا ()
  {
    اگر (مطابقت_glyph_data16) واپس *مطابقت_glyph_data16;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    ورنہ
    اگر (مطابقت_glyph_data24) واپس *مطابقت_glyph_data24;
#اگر_ختم
    واپس 0;
  }
  HB_ALWAYS_INLINE
  خالی
  advance_glyph_ڈیٹا ()
  {
    اگر (مطابقت_glyph_data16) مطابقت_glyph_data16++;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    ورنہ
    اگر (مطابقت_glyph_data24) مطابقت_glyph_data24++;
#اگر_ختم
  }

  غیرعلامتی صحیح idx;
  محفوظ:
  context_t *c;
  matcher_t matcher;
  ثابت HBUINT16 *مطابقت_glyph_data16;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  ثابت HBUINT24 *مطابقت_glyph_data24;
#اگر_ختم

  غیرعلامتی صحیح انتہا;
};

ساخت hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, منطقی, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { واپس "APPLY"; }
  قسمتعریف return_t (*recurse_func_t) (hb_ot_apply_context_t *c, غیرعلامتی صحیح lookup_اشاریہ);

  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_ترجیح<1>) HB_RETURN (return_t, obj.apply (c, خالی_اشاریہ) )
  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_ترجیح<0>) HB_RETURN (return_t, obj.apply (c) )
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { واپس apply_(obj, یہ, hb_prioritize); }

  ساکن return_t default_return_قدر () { واپس جھوٹ; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { واپس r; }
  return_t recurse (غیرعلامتی صحیح sub_lookup_اشاریہ)
  {
    أكد (recurse_func);
    اگر (unlikely (nesting_level_بایاں == 0))
    {
      بفر->successful = جھوٹ;
      واپس default_return_قدر ();
    }

    بفر->max_ops--;
    اگر (unlikely (بفر->max_ops < 0))
    {
      بفر->successful = جھوٹ;
      واپس default_return_قدر ();
    }

    nesting_level_بایاں--;
    منطقی ret = recurse_func (یہ, sub_lookup_اشاریہ);
    nesting_level_بایاں++;
    واپس ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_داخلہ, iter_context;

  غیرعلامتی صحیح جدول_اشاریہ; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t *بفر;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = خالی_اشاریہ;
  ثابت GDEF &gdef;
  ثابت GDEF::accelerator_t &gdef_accel;
  ثابت شے_Variation_Store &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_نقاب_t lookup_نقاب = 1;
  غیرعلامتی صحیح lookup_اشاریہ = (غیرعلامتی) -1;
  غیرعلامتی صحیح lookup_props = 0;
  غیرعلامتی صحیح nesting_level_بایاں = HB_MAX_NESTING_LEVEL;

  منطقی has_glyph_classes;
  منطقی auto_zwnj = سچ;
  منطقی auto_zwj = سچ;
  منطقی فی_syllable = جھوٹ;
  منطقی random = جھوٹ;
  غیرعلامتی new_syllables = (غیرعلامتی) -1;

  علامتی آخری_بنیاد = -1; // GPOS uses
  غیرعلامتی آخری_بنیاد_until = 0; // GPOS uses

  hb_سمتیہ_t<uint32_t> مطابقت_positions;
  uint32_t ڈھیر_مطابقت_positions[8];
#اگر_غیر_تعریف_شدہ HB_NO_بفر_پیغام
  hb_بفر_t::بدلا_func_t orig_بدلا_func = خالی_اشاریہ;
  خالی *orig_بدلا_ڈیٹا = خالی_اشاریہ;
#اگر_ختم

  hb_ot_apply_context_t (غیرعلامتی صحیح جدول_اشاریہ_2,
			 hb_font_t *font_,
			 hb_بفر_t *بفر_2,
			 hb_blob_t *جدول_blob_2,
			 hb_scalar_cache_t *var_store_cache_ = خالی_اشاریہ) :
			جدول_اشاریہ (جدول_اشاریہ_2),
			font (font_), face (font->face), بفر (بفر_2),
			sanitizer (جدول_blob_2),
			gdef (
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT
			      *face->جدول.GDEF->جدول
#ورنہ
			      Null (GDEF)
#اگر_ختم
			     ),
			gdef_accel (
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT
			      *face->جدول.GDEF
#ورنہ
			      Null (GDEF::accelerator_t)
#اگر_ختم
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (بفر_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    مطابقت_positions.سیٹ_storage (ڈھیر_مطابقت_positions);

#اگر_غیر_تعریف_شدہ HB_NO_بفر_پیغام
    اگر (بفر->messaging ())
    {
      أكد (بفر->بدلا_func == خالی_اشاریہ ||
	      بفر->بدلا_func == بفر_بدلا_trampoline);
      orig_بدلا_func = بفر->بدلا_func;
      orig_بدلا_ڈیٹا = بفر->بدلا_ڈیٹا;
      بفر->بدلا_func = بفر_بدلا_trampoline;
      بفر->بدلا_ڈیٹا = یہ;
    }
#اگر_ختم
  }
  ~hb_ot_apply_context_t ()
  {
#اگر_غیر_تعریف_شدہ HB_NO_بفر_پیغام
    اگر (بفر->messaging ())
    {
      أكد (بفر->بدلا_func == بفر_بدلا_trampoline);
      أكد (بفر->بدلا_ڈیٹا == یہ);
      بفر->بدلا_func = orig_بدلا_func;
      بفر->بدلا_ڈیٹا = orig_بدلا_ڈیٹا;
    }
#اگر_ختم
  }

  خالی init_iters ()
  {
    iter_داخلہ.init (یہ, جھوٹ);
    iter_context.init (یہ, سچ);
  }

  خالی سیٹ_lookup_نقاب (hb_نقاب_t نقاب, منطقی init = سچ) { lookup_نقاب = نقاب; آخری_بنیاد = -1; آخری_بنیاد_until = 0; اگر (init) init_iters (); }
  خالی سیٹ_auto_zwj (منطقی auto_zwj_, منطقی init = سچ) { auto_zwj = auto_zwj_; اگر (init) init_iters (); }
  خالی سیٹ_auto_zwnj (منطقی auto_zwnj_, منطقی init = سچ) { auto_zwnj = auto_zwnj_; اگر (init) init_iters (); }
  خالی سیٹ_فی_syllable (منطقی فی_syllable_2, منطقی init = سچ) { فی_syllable = فی_syllable_2; اگر (init) init_iters (); }
  خالی سیٹ_random (منطقی random_) { random = random_; }
  خالی سیٹ_recurse_func (recurse_func_t func) { recurse_func = func; }
  خالی سیٹ_lookup_اشاریہ (غیرعلامتی صحیح lookup_اشاریہ_3) { lookup_اشاریہ = lookup_اشاریہ_3; }
  خالی سیٹ_lookup_props (غیرعلامتی صحیح lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_نمبر ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    بفر->random_state = بفر->random_state * 48271 % 2147483647;
    واپس بفر->random_state;
  }

  ساکن خالی بفر_بدلا_trampoline (hb_بفر_t *بفر HB_غیر_مستعمل, خالی *user_ڈیٹا)
  {
    ((hb_ot_apply_context_t *) user_ڈیٹا)->بفر_بدلا ();
  }
  خالی بفر_بدلا ()
  {
    بفر->update_digest ();
    اگر (likely (has_glyph_classes))
      برائے (غیرعلامتی i = 0; i < بفر->len; i++)
	سیٹ_glyph_class_props (بفر->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  منطقی مطابقت_properties_mark (ثابت hb_glyph_info_t *info,
			      غیرعلامتی صحیح    glyph_props,
			      غیرعلامتی صحیح    مطابقت_props) ثابت
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    اگر (مطابقت_props & LookupFlag::Use_Mark_Filtering_سیٹ)
    {
      غیرعلامتی سیٹ_اشاریہ = مطابقت_props >> 16;
      واپس gdef_accel.mark_سیٹ_covers (سیٹ_اشاریہ, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    اگر (مطابقت_props & LookupFlag::MarkAttachmentType)
      واپس (مطابقت_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    واپس سچ;
  }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  منطقی check_glyph_property (ثابت hb_glyph_info_t *info,
			     غیرعلامتی مطابقت_props) ثابت
  {
    غیرعلامتی صحیح glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    اگر (glyph_props & مطابقت_props & LookupFlag::IgnoreFlags)
      واپس جھوٹ;

    اگر (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      واپس مطابقت_properties_mark (info, glyph_props, مطابقت_props);

    واپس سچ;
  }

  خالی سیٹ_glyph_class (hb_codepoint_t glyph_اشاریہ,
			  غیرعلامتی صحیح class_guess = 0,
			  منطقی ligature = جھوٹ,
			  منطقی component = جھوٹ)
  {
    بفر->digest.جوڑو (glyph_اشاریہ);

    اگر (new_syllables != (غیرعلامتی) -1)
      بفر->cur().syllable() = new_syllables;

    غیرعلامتی صحیح props = _hb_glyph_info_get_glyph_props (&بفر->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    اگر (ligature)
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
    اگر (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    اگر (likely (has_glyph_classes))
    {
      سیٹ_glyph_class_props (بفر->cur(), props, glyph_اشاریہ);
    }
    ورنہ اگر (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      hb_glyph_info_سیٹ_glyph_props (&بفر->cur(), props | class_guess);
    }
    ورنہ
      hb_glyph_info_سیٹ_glyph_props (&بفر->cur(), props);
  }
  خالی سیٹ_glyph_class_props (hb_glyph_info_t &info,
			       غیرعلامتی صحیح props,
			       hb_codepoint_t glyph_اشاریہ)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    hb_glyph_info_سیٹ_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_اشاریہ));
  }
  خالی سیٹ_glyph_class_props (hb_glyph_info_t &info)
  {
    سیٹ_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  خالی replace_glyph (hb_codepoint_t glyph_اشاریہ)
  {
    سیٹ_glyph_class (glyph_اشاریہ);
    (خالی) بفر->replace_glyph (glyph_اشاریہ);
  }
  خالی replace_glyph_inplace (hb_codepoint_t glyph_اشاریہ)
  {
    سیٹ_glyph_class (glyph_اشاریہ);
    بفر->cur().codepoint = glyph_اشاریہ;
  }
  خالی replace_glyph_with_ligature (hb_codepoint_t glyph_اشاریہ,
				    غیرعلامتی صحیح class_guess)
  {
    سیٹ_glyph_class (glyph_اشاریہ, class_guess, سچ);
    (خالی) بفر->replace_glyph (glyph_اشاریہ);
  }
  خالی output_glyph_for_component (hb_codepoint_t glyph_اشاریہ,
				   غیرعلامتی صحیح class_guess)
  {
    سیٹ_glyph_class (glyph_اشاریہ, class_guess, جھوٹ, سچ);
    (خالی) بفر->output_glyph (glyph_اشاریہ);
  }
};

شمار جماعت hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

ساخت hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار collect_دوسرا_glyphs_2 (ثابت T &obj,
					     hb_سیٹ_digest_t *digest,
					     hb_ترجیح<1>) HB_RETURN (خالی, obj.collect_دوسرا_glyphs (digest) )
  سانچہ <قسم_نام T>
  ساکن اندرونی خالی collect_دوسرا_glyphs_2 (ثابت T &obj HB_غیر_مستعمل,
					     hb_سیٹ_digest_t *digest,
					     hb_ترجیح<0>)
  { *digest = hb_سیٹ_digest_t::بھرا (); }

  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار apply_ (ثابت T *obj, hb_ot_apply_context_t *c, خالی *external_cache, hb_ترجیح<1>) HB_RETURN (منطقی, obj->apply (c, external_cache) )
  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار apply_ (ثابت T *obj, hb_ot_apply_context_t *c, خالی *external_cache, hb_ترجیح<0>) HB_RETURN (منطقی, obj->apply (c) )
  سانچہ <قسم_نام T>
  ساکن اندرونی منطقی apply_مقصد (ثابت خالی *obj, hb_ot_apply_context_t *c, خالی *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    واپس apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, خالی *external_cache, hb_ترجیح<2>) HB_RETURN (منطقی, obj->apply_cached (c, external_cache) )
  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, خالی *external_cache, hb_ترجیح<1>) HB_RETURN (منطقی, obj->apply (c, external_cache) )
  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, خالی *external_cache, hb_ترجیح<0>) HB_RETURN (منطقی, obj->apply (c) )
  سانچہ <قسم_نام T>
  ساکن اندرونی منطقی apply_cached_مقصد (ثابت خالی *obj, hb_ot_apply_context_t *c, خالی *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    واپس apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  سانچہ <قسم_نام T>
  ساکن اندرونی خودکار cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_ترجیح<1>) HB_RETURN (منطقی, T::cache_func (c, op) )
  سانچہ <قسم_نام T=خالی>
  ساکن اندرونی منطقی cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_غیر_مستعمل,
				  hb_ترجیح<0>) { واپس جھوٹ; }
  سانچہ <قسم_نام Type>
  ساکن اندرونی منطقی cache_func_مقصد (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    واپس cache_func_<Type> (c, op, hb_prioritize);
  }
#اگر_ختم

  قسمتعریف منطقی (*hb_apply_func_t) (ثابت خالی *obj, hb_ot_apply_context_t *c, خالی *external_cache);
  قسمتعریف منطقی (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  ساخت hb_applicable_t
  {
    دوست ساخت hb_accelerate_subtables_context_t;
    دوست ساخت hb_ot_layout_lookup_accelerator_t;

    سانچہ <قسم_نام T>
    خالی init (ثابت T &obj_,
	       hb_apply_func_t apply_func_
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , خالی *external_cache_
#اگر_ختم
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#اگر_ختم
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#اگر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    منطقی apply (hb_ot_apply_context_t *c) ثابت
    {
      واپس digest.may_have (c->بفر->cur().codepoint) && apply_func (obj, c, خالی_اشاریہ);
    }
    منطقی apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      واپس apply_func (obj, c, خالی_اشاریہ);
    }
#ورنہ
    منطقی apply (hb_ot_apply_context_t *c) ثابت
    {
      واپس digest.may_have (c->بفر->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    منطقی apply_cached (hb_ot_apply_context_t *c) ثابت
    {
      واپس digest.may_have (c->بفر->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    منطقی apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      واپس apply_func (obj, c, external_cache);
    }
    منطقی apply_cached_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      واپس apply_cached_func (obj, c, external_cache);
    }

    منطقی cache_enter (hb_ot_apply_context_t *c) ثابت
    {
      واپس cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    خالی cache_leave (hb_ot_apply_context_t *c) ثابت
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#اگر_ختم

    نجی:
    ثابت خالی *obj;
    hb_apply_func_t apply_func;
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    خالی *external_cache;
#اگر_ختم
    hb_سیٹ_digest_t digest;
  };

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
  سانچہ <قسم_نام T>
  خودکار cache_لاگت (ثابت T &obj, hb_ترجیح<1>) HB_AUTO_RETURN ( obj.cache_لاگت () )
  سانچہ <قسم_نام T>
  خودکار cache_لاگت (ثابت T &obj, hb_ترجیح<0>) HB_AUTO_RETURN ( 0u )

  سانچہ <قسم_نام T>
  خودکار external_cache_create (ثابت T &obj, hb_ترجیح<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  سانچہ <قسم_نام T>
  خودکار external_cache_create (ثابت T &obj, hb_ترجیح<0>) HB_AUTO_RETURN ( خالی_اشاریہ )
#اگر_ختم

  /* Dispatch interface. */
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj)
  {
    collect_دوسرا_glyphs_2 (obj, &digest_دوسرا, hb_prioritize);

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    خالی *external_cache = خالی_اشاریہ;
    اگر (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#اگر_ختم

    hb_applicable_t *اندراج_2 = &array[i++];

    اندراج_2->init (obj,
		 apply_مقصد<T>
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_مقصد<T>
		 , cache_func_مقصد<T>
		 , external_cache
#اگر_ختم
		 );

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    غیرعلامتی لاگت = cache_لاگت (obj, hb_prioritize);
    اگر (لاگت > subtable_cache_user_لاگت)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_لاگت = لاگت;
    }
#اگر_ختم

    واپس hb_خالی_خانے_t ();
  }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  غیرعلامتی i = 0;
  hb_سیٹ_digest_t digest_دوسرا;

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
  غیرعلامتی subtable_cache_user_idx = (غیرعلامتی) -1;
  غیرعلامتی subtable_cache_user_لاگت = 0;
#اگر_ختم
};


قسمتعریف منطقی (*intersects_func_t) (ثابت hb_سیٹ_t *glyphs, غیرعلامتی value, ثابت خالی *ڈیٹا, خالی *cache);
قسمتعریف خالی (*intersected_glyphs_func_t) (ثابت hb_سیٹ_t *glyphs, ثابت خالی *ڈیٹا, غیرعلامتی value, hb_سیٹ_t *intersected_glyphs, خالی *cache);
قسمتعریف خالی (*collect_glyphs_func_t) (hb_سیٹ_t *glyphs, غیرعلامتی value, ثابت خالی *ڈیٹا);
قسمتعریف منطقی (*مطابقت_func_t) (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا);
قسمتعریف غیرعلامتی (*get_قدر_func_t) (hb_glyph_info_t &info, ثابت خالی *ڈیٹا);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
ساخت hb_ot_layout_ruleset_digest_t
{
  خالی init () { نقاب = 0; }
  خالی سیٹ_بھرا () { نقاب = (uint64_t) -1; }
  خالی جوڑو (غیرعلامتی value) { نقاب |= (uint64_t) 1 << (value & 63); }
  منطقی may_have (غیرعلامتی value) ثابت { واپس نقاب & ((uint64_t) 1 << (value & 63)); }

  uint64_t نقاب;
};

ساخت Context_احاطہ_Funcs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
ساخت ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
ساخت ContextApplyFuncs
{
  مطابقت_func_t مطابقت;
};
ساخت ChainContextApplyFuncs
{
  مطابقت_func_t مطابقت[3];
};


ساکن اندرونی منطقی intersects_glyph (ثابت hb_سیٹ_t *glyphs, غیرعلامتی value, ثابت خالی *ڈیٹا HB_غیر_مستعمل, خالی *cache HB_غیر_مستعمل)
{
  واپس glyphs->has (value);
}
ساکن اندرونی منطقی intersects_class (ثابت hb_سیٹ_t *glyphs, غیرعلامتی value, ثابت خالی *ڈیٹا, خالی *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(ڈیٹا);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  اگر (map->has (value, &cached_v))
    واپس *cached_v;

  منطقی v = class_def.intersects_class (glyphs, value);
  map->سیٹ (value, v);

  واپس v;
}
ساکن اندرونی منطقی intersects_coverage (ثابت hb_سیٹ_t *glyphs, غیرعلامتی value, ثابت خالی *ڈیٹا, خالی *cache HB_غیر_مستعمل)
{
  Offset32_مقصد<Coverage> coverage;
  coverage = value;
  واپس (ڈیٹا+coverage).intersects (glyphs);
}


ساکن اندرونی خالی intersected_glyph (ثابت hb_سیٹ_t *glyphs HB_غیر_مستعمل, ثابت خالی *ڈیٹا, غیرعلامتی value, hb_سیٹ_t *intersected_glyphs, HB_غیر_مستعمل خالی *cache)
{
  غیرعلامتی g = reinterpret_cast<ثابت HBUINT16 *>(ڈیٹا)[value];
  intersected_glyphs->جوڑو (g);
}

استعمال intersected_class_cache_t = hb_hashmap_t<غیرعلامتی, hb_سیٹ_t>;

ساکن اندرونی خالی intersected_class_glyphs (ثابت hb_سیٹ_t *glyphs, ثابت خالی *ڈیٹا, غیرعلامتی value, hb_سیٹ_t *intersected_glyphs, خالی *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(ڈیٹا);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_سیٹ_t *cached_v;
  اگر (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    واپس;
  }

  hb_سیٹ_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->سیٹ (value, std::move (v));
}

ساکن اندرونی خالی intersected_coverage_glyphs (ثابت hb_سیٹ_t *glyphs, ثابت خالی *ڈیٹا, غیرعلامتی value, hb_سیٹ_t *intersected_glyphs, HB_غیر_مستعمل خالی *cache)
{
  Offset32_مقصد<Coverage> coverage;
  coverage = value;
  (ڈیٹا+coverage).intersect_سیٹ (*glyphs, *intersected_glyphs);
}


ساکن اندرونی منطقی array_is_subset_کا (ثابت hb_سیٹ_t *glyphs,
				       غیرعلامتی صحیح گنتی,
				       glyph_ڈیٹا_t values,
				       intersects_func_t intersects_func,
				       ثابت خالی *intersects_ڈیٹا,
				       خالی *cache)
{
  برائے (غیرعلامتی i = 0; i < گنتی; i++)
    اگر (!intersects_func (glyphs, values[i], intersects_ڈیٹا, cache)) واپس جھوٹ;
  واپس سچ;
}


ساکن اندرونی خالی collect_glyph (hb_سیٹ_t *glyphs, غیرعلامتی value, ثابت خالی *ڈیٹا HB_غیر_مستعمل)
{
  glyphs->جوڑو (value);
}
ساکن اندرونی خالی collect_class (hb_سیٹ_t *glyphs, غیرعلامتی value, ثابت خالی *ڈیٹا)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(ڈیٹا);
  class_def.collect_class (glyphs, value);
}
ساکن اندرونی خالی collect_coverage (hb_سیٹ_t *glyphs, غیرعلامتی value, ثابت خالی *ڈیٹا)
{
  Offset32_مقصد<Coverage> coverage;
  coverage = value;
  (ڈیٹا+coverage).collect_coverage (glyphs);
}

اندرونی ثابت hb_سیٹ_t *
hb_depend_context_t::ڈھونڈو_or_create_glyph_سیٹ (ثابت خالی *ڈیٹا,
                                                غیرعلامتی value,
                                                glyph_سیٹ_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_سیٹ_cache_کلید_t key (ڈیٹا, value, type);
  hb_codepoint_t *existing = خالی_اشاریہ;
  اگر (glyph_سیٹ_مقصد_اشاریہ.has (key, &existing))
    واپس glyph_sets[*existing].get ();

  hb_سیٹ_t *سیٹ = hb_object_create<hb_سیٹ_t> ();
  اگر (unlikely (!سیٹ))
  {
    depend_ڈیٹا->fail ();
    واپس خالی_اشاریہ;
  }

  اگر (type == GLYPH_سیٹ_COVERAGE)
    collect_coverage (سیٹ, value, ڈیٹا);
  ورنہ اگر (type == GLYPH_سیٹ_CLASS)
    collect_class (سیٹ, value, ڈیٹا);
  ورنہ
    سیٹ->جوڑو (value);
  اگر (unlikely (سیٹ->اندر_error ()))
  {
    hb_سیٹ_destroy (سیٹ);
    depend_ڈیٹا->fail ();
    واپس خالی_اشاریہ;
  }

  hb_codepoint_t اشاریہ_2 = glyph_sets.لمبائی;
  اگر (unlikely (!glyph_sets.ڈالو_or_fail (hb::unique_ptr<hb_سیٹ_t> {سیٹ})))
  {
    depend_ڈیٹا->fail ();
    واپس خالی_اشاریہ;
  }
  اگر (unlikely (!glyph_سیٹ_مقصد_اشاریہ.سیٹ (key, اشاریہ_2)))
  {
    depend_ڈیٹا->fail ();
    واپس خالی_اشاریہ;
  }
  واپس glyph_sets[اشاریہ_2].get ();
}

اندرونی hb_codepoint_t
hb_depend_context_t::ڈھونڈو_or_create_cached_context_سیٹ (ثابت hb_سیٹ_t *سیٹ)
{
  uintptr_t key = (uintptr_t) سیٹ;
  hb_codepoint_t *existing = خالی_اشاریہ;
  اگر (cached_context_سیٹ_indices.has (key, &existing))
    واپس *existing;

  hb_codepoint_t اشاریہ_2 = depend_ڈیٹا->ڈھونڈو_or_create_context_سیٹ (*سیٹ);
  اگر (unlikely (اشاریہ_2 == HB_CODEPOINT_INVALID))
    واپس HB_CODEPOINT_INVALID;
  اگر (unlikely (!cached_context_سیٹ_indices.سیٹ (key, اشاریہ_2)))
    واپس depend_ڈیٹا->fail_invalid ();
  واپس اشاریہ_2;
}

ساکن اندرونی خالی collect_array (hb_collect_glyphs_context_t *c HB_غیر_مستعمل,
				  hb_سیٹ_t *glyphs,
				  غیرعلامتی صحیح گنتی,
				  glyph_ڈیٹا_t values,
				  collect_glyphs_func_t collect_func,
				  ثابت خالی *collect_ڈیٹا)
{
  برائے (غیرعلامتی i = 0; i < گنتی; i++)
    collect_func (glyphs, values[i], collect_ڈیٹا);
}


ساکن اندرونی منطقی مطابقت_always (hb_glyph_info_t &info HB_غیر_مستعمل, غیرعلامتی value HB_غیر_مستعمل, ثابت خالی *ڈیٹا HB_غیر_مستعمل)
{
  واپس سچ;
}
ساکن اندرونی منطقی مطابقت_glyph (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا HB_غیر_مستعمل)
{
  واپس info.codepoint == value;
}
ساکن اندرونی منطقی مطابقت_class (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(ڈیٹا);
  واپس class_def.get_class (info.codepoint) == value;
}
ساکن اندرونی غیرعلامتی get_class_قدر (hb_glyph_info_t &info, ثابت خالی *ڈیٹا)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (ڈیٹا);
  واپس class_def.get_class (info.codepoint);
}
ساخت مطابقت_class_cache_ڈیٹا_t
{
  ثابت ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ساکن اندرونی منطقی مطابقت_class_cache (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا)
{
  ثابت خودکار &cache_ڈیٹا = *reinterpret_cast<ثابت مطابقت_class_cache_ڈیٹا_t *> (ڈیٹا);
  واپس cache_ڈیٹا.class_def->get_class (info.codepoint, cache_ڈیٹا.cache) == value;
}
ساکن اندرونی غیرعلامتی get_class_cache_قدر (hb_glyph_info_t &info, ثابت خالی *ڈیٹا)
{
  ثابت خودکار &cache_ڈیٹا = *reinterpret_cast<ثابت مطابقت_class_cache_ڈیٹا_t *> (ڈیٹا);
  واپس cache_ڈیٹا.class_def->get_class (info.codepoint, cache_ڈیٹا.cache);
}
ساکن اندرونی غیرعلامتی get_class_cached (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  غیرعلامتی klass = info.syllable();
  اگر (klass < 255)
    واپس klass;
  klass = class_def.get_class (info.codepoint);
  اگر (likely (klass < 255))
    info.syllable() = klass;
  واپس klass;
}
ساکن اندرونی منطقی مطابقت_class_cached (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(ڈیٹا);
  واپس get_class_cached (class_def, info) == value;
}
ساکن اندرونی غیرعلامتی get_class_cached_قدر (hb_glyph_info_t &info, ثابت خالی *ڈیٹا)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (ڈیٹا);
  واپس get_class_cached (class_def, info);
}
ساکن اندرونی غیرعلامتی get_class_cached1 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  غیرعلامتی klass = info.syllable() & 0x0F;
  اگر (klass < 15)
    واپس klass;
  klass = class_def.get_class (info.codepoint);
  اگر (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  واپس klass;
}
ساکن اندرونی منطقی مطابقت_class_cached1 (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(ڈیٹا);
  واپس get_class_cached1 (class_def, info) == value;
}
ساکن اندرونی غیرعلامتی get_class_cached2 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  غیرعلامتی klass = (info.syllable() & 0xF0) >> 4;
  اگر (klass < 15)
    واپس klass;
  klass = class_def.get_class (info.codepoint);
  اگر (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  واپس klass;
}
ساکن اندرونی منطقی مطابقت_class_cached2 (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(ڈیٹا);
  واپس get_class_cached2 (class_def, info) == value;
}
ساکن اندرونی غیرعلامتی get_class_cached2_قدر (hb_glyph_info_t &info, ثابت خالی *ڈیٹا)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (ڈیٹا);
  واپس get_class_cached2 (class_def, info);
}
ساکن اندرونی منطقی مطابقت_coverage (hb_glyph_info_t &info, غیرعلامتی value, ثابت خالی *ڈیٹا)
{
  Offset32_مقصد<Coverage> coverage;
  coverage = value;
  واپس (ڈیٹا+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ساکن اندرونی منطقی would_مطابقت_داخلہ (hb_would_apply_context_t *c,
				      غیرعلامتی صحیح گنتی, /* Including the first glyph (not matched) */
				      glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
				      مطابقت_func_t مطابقت_func_2,
				      ثابت خالی *مطابقت_ڈیٹا_2)
{
  اگر (گنتی != c->len)
    واپس جھوٹ;

  برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    اگر (likely (!مطابقت_func_2 (info, داخلہ[i - 1], مطابقت_ڈیٹا_2)))
      واپس جھوٹ;
  }

  واپس سچ;
}
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
HB_ALWAYS_INLINE
#ورنہ
اندرونی
#اگر_ختم
ساکن منطقی مطابقت_داخلہ (hb_ot_apply_context_t *c,
			 غیرعلامتی صحیح گنتی, /* Including the first glyph (not matched) */
			 glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
			 مطابقت_func_t مطابقت_func_2,
			 ثابت خالی *مطابقت_ڈیٹا_2,
			 غیرعلامتی صحیح *انتہا_position,
			 غیرعلامتی صحیح *p_کل_component_گنتی = خالی_اشاریہ)
{
  TRACE_APPLY (خالی_اشاریہ);

  hb_بفر_t *بفر = c->بفر;

  اگر (گنتی == 1)
  {
    *انتہا_position = بفر->idx + 1;
    c->مطابقت_positions[0] = بفر->idx;
    اگر (p_کل_component_گنتی)
      *p_کل_component_گنتی = hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
    return_trace (سچ);
  }

  اگر (unlikely (گنتی > HB_MAX_CONTEXT_لمبائی)) return_trace (جھوٹ);

  خودکار &skippy_iter = c->iter_داخلہ;
  skippy_iter.reset (بفر->idx);
  skippy_iter.سیٹ_مطابقت_func (مطابقت_func_2, مطابقت_ڈیٹا_2);
  skippy_iter.سیٹ_glyph_ڈیٹا (داخلہ);

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

  غیرعلامتی صحیح کل_component_گنتی = 0;

  غیرعلامتی صحیح پہلا_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
  غیرعلامتی صحیح پہلا_lig_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());

  شمار {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
  {
    غیرعلامتی unsafe_مقصد;
    اگر (!skippy_iter.next (&unsafe_مقصد))
    {
      *انتہا_position = unsafe_مقصد;
      return_trace (جھوٹ);
    }

    اگر (unlikely (i + 1 > c->مطابقت_positions.لمبائی &&
		  !c->مطابقت_positions.resize_dirty  (i + 1)))
      return_trace (جھوٹ);
    c->مطابقت_positions.arrayZ[i] = skippy_iter.idx;

    غیرعلامتی صحیح this_lig_id = _hb_glyph_info_get_lig_id (&بفر->info[skippy_iter.idx]);
    غیرعلامتی صحیح this_lig_comp = _hb_glyph_info_get_lig_comp (&بفر->info[skippy_iter.idx]);

    اگر (پہلا_lig_id && پہلا_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      اگر (پہلا_lig_id != this_lig_id || پہلا_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	اگر (ligbase == LIGBASE_NOT_CHECKED)
	{
	  منطقی ملا = جھوٹ;
	  ثابت خودکار *برآمد = بفر->برآمد_info;
	  غیرعلامتی صحیح j = بفر->برآمد_len;
	  جب (j && _hb_glyph_info_get_lig_id (&برآمد[j - 1]) == پہلا_lig_id)
	  {
	    اگر (_hb_glyph_info_get_lig_comp (&برآمد[j - 1]) == 0)
	    {
	      j--;
	      ملا = سچ;
	      توڑو;
	    }
	    j--;
	  }

	  اگر (ملا && skippy_iter.may_skip (برآمد[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  ورنہ
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	اگر (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (جھوٹ);
      }
    }
    ورنہ
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      اگر (this_lig_id && this_lig_comp && (this_lig_id != پہلا_lig_id))
	return_trace (جھوٹ);
    }

    کل_component_گنتی += hb_glyph_info_get_lig_عدد_comps_اندر_ligation (&بفر->info[skippy_iter.idx]);
  }

  *انتہا_position = skippy_iter.idx + 1;

  اگر (p_کل_component_گنتی)
  {
    کل_component_گنتی += hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
    *p_کل_component_گنتی = کل_component_گنتی;
  }

  c->مطابقت_positions.arrayZ[0] = بفر->idx;

  return_trace (سچ);
}
ساکن اندرونی منطقی ligate_داخلہ (hb_ot_apply_context_t *c,
				 غیرعلامتی صحیح گنتی, /* Including the first glyph */
				 غیرعلامتی صحیح مطابقت_انتہا,
				 hb_codepoint_t lig_glyph,
				 غیرعلامتی صحیح کل_component_گنتی)
{
  TRACE_APPLY (خالی_اشاریہ);

  hb_بفر_t *بفر = c->بفر;

  بفر->ملاؤ_clusters (بفر->idx, مطابقت_انتہا);

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

  منطقی is_بنیاد_ligature = hb_glyph_info_is_بنیاد_glyph (&بفر->info[c->مطابقت_positions.arrayZ[0]]);
  منطقی is_mark_ligature = _hb_glyph_info_is_mark (&بفر->info[c->مطابقت_positions.arrayZ[0]]);
  برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
    اگر (!_hb_glyph_info_is_mark (&بفر->info[c->مطابقت_positions.arrayZ[i]]))
    {
      is_بنیاد_ligature = جھوٹ;
      is_mark_ligature = جھوٹ;
      توڑو;
    }
  منطقی is_ligature = !is_بنیاد_ligature && !is_mark_ligature;

  غیرعلامتی صحیح klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  غیرعلامتی صحیح lig_id = is_ligature ? _hb_allocate_lig_id (بفر) : 0;
  غیرعلامتی صحیح آخری_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
  غیرعلامتی صحیح آخری_عدد_components = hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
  غیرعلامتی صحیح components_so_far = آخری_عدد_components;

  اگر (is_ligature)
  {
    hb_glyph_info_سیٹ_lig_props_for_ligature (&بفر->cur(), lig_id, کل_component_گنتی);
    اگر (_hb_glyph_info_get_general_category (&بفر->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      hb_glyph_info_سیٹ_general_category (&بفر->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
  {
    جب (بفر->idx < c->مطابقت_positions.arrayZ[i] && بفر->successful)
    {
      اگر (is_ligature)
      {
	غیرعلامتی صحیح this_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());
	اگر (this_comp == 0)
	  this_comp = آخری_عدد_components;
	أكد (components_so_far >= آخری_عدد_components);
	غیرعلامتی صحیح new_lig_comp = components_so_far - آخری_عدد_components +
				    hb_min (this_comp, آخری_عدد_components);
	  hb_glyph_info_سیٹ_lig_props_for_mark (&بفر->cur(), lig_id, new_lig_comp);
      }
      (خالی) بفر->اگلا_glyph ();
    }

    آخری_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
    آخری_عدد_components = hb_glyph_info_get_lig_عدد_comps_اندر_ligation (&بفر->cur());
    components_so_far += آخری_عدد_components;

    /* Skip the base glyph */
    بفر->idx++;
  }

  اگر (!is_mark_ligature && آخری_lig_id)
  {
    /* Re-adjust components for any marks following. */
    برائے (غیرعلامتی i = بفر->idx; i < بفر->len; ++i)
    {
      اگر (آخری_lig_id != _hb_glyph_info_get_lig_id (&بفر->info[i])) توڑو;

      غیرعلامتی this_comp = _hb_glyph_info_get_lig_comp (&بفر->info[i]);
      اگر (!this_comp) توڑو;

      أكد (components_so_far >= آخری_عدد_components);
      غیرعلامتی new_lig_comp = components_so_far - آخری_عدد_components +
			      hb_min (this_comp, آخری_عدد_components);
      hb_glyph_info_سیٹ_lig_props_for_mark (&بفر->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (سچ);
}

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
HB_ALWAYS_INLINE
#ورنہ
اندرونی
#اگر_ختم
ساکن منطقی مطابقت_backtrack (hb_ot_apply_context_t *c,
			     غیرعلامتی صحیح گنتی,
			     glyph_ڈیٹا_t backtrack,
			     مطابقت_func_t مطابقت_func_2,
			     ثابت خالی *مطابقت_ڈیٹا_2,
			     غیرعلامتی صحیح *مطابقت_آغاز)
{
  TRACE_APPLY (خالی_اشاریہ);

  اگر (!گنتی)
  {
    *مطابقت_آغاز = c->بفر->backtrack_len ();
    return_trace (سچ);
  }

  خودکار &skippy_iter = c->iter_context;
  skippy_iter.reset_عقب (c->بفر->backtrack_len ());
  skippy_iter.سیٹ_مطابقت_func (مطابقت_func_2, مطابقت_ڈیٹا_2);
  skippy_iter.سیٹ_glyph_ڈیٹا (backtrack);

  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    غیرعلامتی unsafe_from;
    اگر (!skippy_iter.prev (&unsafe_from))
    {
      *مطابقت_آغاز = unsafe_from;
      return_trace (جھوٹ);
    }
  }

  *مطابقت_آغاز = skippy_iter.idx;
  return_trace (سچ);
}

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
HB_ALWAYS_INLINE
#ورنہ
اندرونی
#اگر_ختم
ساکن منطقی مطابقت_lookahead (hb_ot_apply_context_t *c,
			     غیرعلامتی صحیح گنتی,
			     glyph_ڈیٹا_t lookahead,
			     مطابقت_func_t مطابقت_func_2,
			     ثابت خالی *مطابقت_ڈیٹا_2,
			     غیرعلامتی صحیح آغاز_اشاریہ,
			     غیرعلامتی صحیح *انتہا_اشاریہ)
{
  TRACE_APPLY (خالی_اشاریہ);

  اگر (!گنتی)
  {
    *انتہا_اشاریہ = آغاز_اشاریہ;
    return_trace (سچ);
  }

  خودکار &skippy_iter = c->iter_context;
  أكد (آغاز_اشاریہ >= 1);
  skippy_iter.reset (آغاز_اشاریہ - 1);
  skippy_iter.سیٹ_مطابقت_func (مطابقت_func_2, مطابقت_ڈیٹا_2);
  skippy_iter.سیٹ_glyph_ڈیٹا (lookahead);

  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
  {
    غیرعلامتی unsafe_مقصد;
    اگر (!skippy_iter.next (&unsafe_مقصد))
    {
      *انتہا_اشاریہ = unsafe_مقصد;
      return_trace (جھوٹ);
    }
  }

  *انتہا_اشاریہ = skippy_iter.idx + 1;
  return_trace (سچ);
}



ساخت Lookup_ریکارڈ
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t         *lookup_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->embed (*یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    return_trace (c->check_تفویض (برآمد->lookup_List_اشاریہ, lookup_map->get (lookup_List_اشاریہ), HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  HBUINT16	sequence_اشاریہ;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookup_List_اشاریہ;	/* Lookup to apply to that
					 * position--zero--based */
  عوامی:
  DEFINE_سائز_STATIC (4);
};

ساکن اندرونی غیرعلامتی serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ثابت hb_array_t<ثابت Lookup_ریکارڈ> lookup_اندراجات,
						     ثابت hb_map_t *lookup_map)
{
  غیرعلامتی گنتی = 0;
  برائے (ثابت Lookup_ریکارڈ& r : lookup_اندراجات)
  {
    اگر (!lookup_map->has (r.lookup_List_اشاریہ))
      جاری;

    اگر (!r.serialize (c, lookup_map))
      واپس 0;

    گنتی++;
  }
  واپس گنتی;
}

شمار ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ساکن اندرونی ثابت hb_سیٹ_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ثابت خالی *ڈیٹا,
			غیرعلامتی value,
			منطقی filter_class_by_والد,
			hb_سیٹ_t *scratch,
			hb_سمتیہ_t<غیرعلامتی> *compact_workspace = خالی_اشاریہ)
{
  انتخاب (context_format)
  {
    حالت ContextFormat::SimpleContext:
      واپس c->ڈھونڈو_or_create_glyph_سیٹ (خالی_اشاریہ, value,
                                           hb_depend_context_t::GLYPH_سیٹ_SINGLE);
    حالت ContextFormat::ClassBasedContext:
    {
      ثابت hb_سیٹ_t *class_سیٹ = c->ڈھونڈو_or_create_glyph_سیٹ (
        ڈیٹا, value, hb_depend_context_t::GLYPH_سیٹ_CLASS);
      اگر (unlikely (!class_سیٹ))
	واپس خالی_اشاریہ;
      اگر (!filter_class_by_والد)
	واپس class_سیٹ;
      scratch->سیٹ (*class_سیٹ);
      scratch->intersect (c->والد_فعال_glyphs (), compact_workspace);
      توڑو;
    }
    حالت ContextFormat::CoverageBasedContext:
      واپس c->ڈھونڈو_or_create_glyph_سیٹ (
        ڈیٹا, value, hb_depend_context_t::GLYPH_سیٹ_COVERAGE);
  }

  اگر (unlikely (scratch->اندر_error ()))
  {
    c->depend_ڈیٹا->fail ();
    واپس خالی_اشاریہ;
  }
  واپس scratch;
}

ساکن اندرونی خالی context_احاطہ_recurse_lookups (hb_احاطہ_context_t *c,
					     غیرعلامتی داخلہ_گنتی, glyph_ڈیٹا_t داخلہ,
					     غیرعلامتی lookup_گنتی_2,
					     ثابت Lookup_ریکارڈ lookup_ریکارڈ[] /* Array of LookupRecords--in design order */,
					     غیرعلامتی value,
					     ContextFormat context_format,
					     ثابت خالی *ڈیٹا,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     خالی *cache)
{
  hb_سیٹ_t covered_seq_indicies;
  hb_سیٹ_t pos_glyphs;
  برائے (غیرعلامتی صحیح i = 0; i < lookup_گنتی_2; i++)
  {
    غیرعلامتی seq_اشاریہ_2 = lookup_ریکارڈ[i].sequence_اشاریہ;
    اگر (seq_اشاریہ_2 >= داخلہ_گنتی) جاری;

    منطقی has_pos_glyphs = جھوٹ;

    اگر (!covered_seq_indicies.has (seq_اشاریہ_2))
    {
      has_pos_glyphs = سچ;
      pos_glyphs.clear ();
      اگر (seq_اشاریہ_2 == 0)
      {
        انتخاب (context_format) {
        حالت ContextFormat::SimpleContext:
          pos_glyphs.جوڑو (value);
          توڑو;
        حالت ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->والد_فعال_glyphs (), ڈیٹا, value, &pos_glyphs, cache);
          توڑو;
        حالت ContextFormat::CoverageBasedContext:
          pos_glyphs.سیٹ (c->والد_فعال_glyphs ());
          توڑو;
        }
      }
      ورنہ
      {
        ثابت خالی *داخلہ_ڈیٹا = داخلہ.ڈیٹا;
        غیرعلامتی داخلہ_قدر = seq_اشاریہ_2 - 1;
        اگر (context_format != ContextFormat::SimpleContext)
        {
          داخلہ_ڈیٹا = ڈیٹا;
          داخلہ_قدر = داخلہ[seq_اشاریہ_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, داخلہ_ڈیٹا, داخلہ_قدر, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.جوڑو (seq_اشاریہ_2);
    hb_سیٹ_t *cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs))
      واپس;
    اگر (has_pos_glyphs) {
      *cur_فعال_glyphs = std::move (pos_glyphs);
    } ورنہ {
      *cur_فعال_glyphs = *c->glyphs;
    }

    غیرعلامتی انتہا_اشاریہ_2 = داخلہ_گنتی;
    اگر (context_format == ContextFormat::CoverageBasedContext)
      انتہا_اشاریہ_2 += 1;

    c->recurse (lookup_ریکارڈ[i].lookup_List_اشاریہ, &covered_seq_indicies, seq_اشاریہ_2, انتہا_اشاریہ_2);

    c->نکالو_cur_مکمل_glyphs ();
  }
}

ساکن منطقی
context_depend_sets_are_cached (hb_depend_context_t *c,
				غیرعلامتی داخلہ_گنتی_2,
				غیرعلامتی lookup_گنتی,
				ثابت Lookup_ریکارڈ lookup_اندراجات_2[],
				ثابت خالی *قاعدہ_2,
				غیرعلامتی value,
				ContextFormat context_format,
				ثابت خالی *ڈیٹا,
				hb_سمتیہ_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
  منطقی have_فعال_idx = جھوٹ;
  برائے (غیرعلامتی i = 0; i < lookup_گنتی; i++)
  {
    اگر (lookup_اندراجات_2[i].sequence_اشاریہ >= داخلہ_گنتی_2)
      جاری;

    اگر (!have_فعال_idx)
    {
      اگر (unlikely (!c->get_والد_فعال_glyphs_اشاریہ (&فعال_idx)))
	واپس جھوٹ;
      have_فعال_idx = سچ;
    }

    hb_depend_context_t::context_سیٹ_cache_کلید_t key (
      قاعدہ_2, &lookup_اندراجات_2[i], ڈیٹا, فعال_idx, value, context_format);
    hb_codepoint_t *cached_context_سیٹ = خالی_اشاریہ;
    اگر (!c->context_سیٹ_cache.has (key, &cached_context_سیٹ))
      واپس جھوٹ;
    اگر (unlikely (!cached_context_sets->ڈالو_or_fail (*cached_context_سیٹ)))
      واپس c->depend_ڈیٹا->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  واپس have_فعال_idx;
}

ساکن اندرونی خالی context_depend_recurse_lookups (hb_depend_context_t *c,
					     غیرعلامتی داخلہ_گنتی, glyph_ڈیٹا_t داخلہ,
					     غیرعلامتی lookup_گنتی_2,
					     ثابت Lookup_ریکارڈ lookup_ریکارڈ[] /* Array of LookupRecords--in design order */,
					     ثابت خالی *قاعدہ_2,
					     غیرعلامتی value,
					     ContextFormat context_format,
					     ثابت خالی *ڈیٹا,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     خالی *cache,
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
  اگر (unlikely (داخلہ_گنتی > HB_MAX_CONTEXT_لمبائی))
    واپس;

  hb_ثنائی_ہندسہ_صفحہ_t covered_seq_indices;
  hb_سیٹ_t &pos_glyphs = scratch->pos_glyphs;
  hb_سیٹ_t &position_context = scratch->position_context;
  hb_سیٹ_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_سیٹ_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  غیرعلامتی cached_context_اشاریہ = 0;
  برائے (غیرعلامتی صحیح i = 0; i < lookup_گنتی_2; i++)
  {
    غیرعلامتی seq_اشاریہ_2 = lookup_ریکارڈ[i].sequence_اشاریہ;
    اگر (seq_اشاریہ_2 >= داخلہ_گنتی)
      جاری;

    hb_depend_context_t::context_سیٹ_cache_کلید_t context_cache_کلید;
    اگر (unlikely (!c->get_context_سیٹ_cache_کلید (قاعدہ_2, &lookup_ریکارڈ[i], ڈیٹا, value,
						 context_format, &context_cache_کلید)))
      واپس;

    hb_codepoint_t context_سیٹ_idx;
    hb_codepoint_t *cached_context_سیٹ_idx;
    اگر (cached_context_sets)
      context_سیٹ_idx = (*cached_context_sets)[cached_context_اشاریہ++];
    ورنہ اگر (c->context_سیٹ_cache.has (context_cache_کلید, &cached_context_سیٹ_idx))
      context_سیٹ_idx = *cached_context_سیٹ_idx;
    ورنہ
    {
      اگر (unlikely (!داخلہ_position_glyphs))
      {
	c->depend_ڈیٹا->fail ();
	واپس;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_سیٹ_قدر_INVALID;
      جب (preliminary_context.next (&elem)) {
	اگر (!(elem & HB_DEPEND_CONTEXT_سیٹ_FLAG)) {
	  /* Direct glyph */
	  position_context.جوڑو (elem);
	} ورنہ {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.جوڑو (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      برائے (غیرعلامتی j = 0; j < داخلہ_position_glyphs->لمبائی; j++) {
	اگر (j != seq_اشاریہ_2 && (*داخلہ_position_glyphs)[j]->get_population () == 1) {
	  position_context.جوڑو ((*داخلہ_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_سیٹ_قدر_INVALID;
      جب (disjunctive_indices.next (&elem)) {
	hb_codepoint_t سیٹ_idx = elem & 0x7FFFFFFF;
	ثابت hb_سیٹ_t *original_سیٹ = c->depend_ڈیٹا->get_سیٹ_from_اشاریہ (سیٹ_idx);
	اگر (unlikely (!original_سیٹ)) جاری;

	/* Dependency context sets are never inverted. */
	اگر (!original_سیٹ->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.جوڑو (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      برائے (غیرعلامتی j = 0; j < داخلہ_position_glyphs->لمبائی; j++) {
	اگر (j != seq_اشاریہ_2 && (*داخلہ_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  اگر (!(*داخلہ_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    منطقی cached_سیٹ = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_سیٹ
	      ? c->ڈھونڈو_or_create_cached_context_سیٹ ((*داخلہ_position_glyphs)[j])
	      : c->depend_ڈیٹا->ڈھونڈو_or_create_context_سیٹ (*(*داخلہ_position_glyphs)[j]);
	    اگر (unlikely (idx == HB_CODEPOINT_INVALID))
	      واپس;
	    filtered_disjunctive_indices.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      اگر (unlikely (position_context.اندر_error () ||
		    filtered_disjunctive_indices.اندر_error ()))
      {
	c->depend_ڈیٹا->fail ();
	واپس;
      }

      context_سیٹ_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_ڈیٹا->ڈھونڈو_or_create_context_سیٹ (position_context);
      اگر (unlikely (!position_context.is_empty () &&
		    context_سیٹ_idx == HB_CODEPOINT_INVALID))
	واپس;

      اگر (unlikely (!c->context_سیٹ_cache.سیٹ (context_cache_کلید, context_سیٹ_idx)))
      {
	c->depend_ڈیٹا->fail ();
	واپس;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_سیٹ_اشاریہ = c->depend_ڈیٹا->current_context_سیٹ_اشاریہ;
    hb_subset_depend_کنارہ_flags_t saved_کنارہ_flags = c->depend_ڈیٹا->current_کنارہ_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    منطقی nested_contextual = (saved_context_سیٹ_اشاریہ != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_ڈیٹا->current_context_سیٹ_اشاریہ = context_سیٹ_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_ڈیٹا->current_کنارہ_flags = HB_SUBSET_DEPEND_کنارہ_FLAG_NONE;
    اگر (داخلہ_گنتی > 1) {
      c->depend_ڈیٹا->current_کنارہ_flags |= HB_SUBSET_DEPEND_کنارہ_FLAG_FROM_CONTEXT_POSITION;
    }
    اگر (nested_contextual) {
      c->depend_ڈیٹا->current_کنارہ_flags |= HB_SUBSET_DEPEND_کنارہ_FLAG_FROM_NESTED_CONTEXT;
    }

    منطقی has_pos_glyphs = جھوٹ;

    اگر (!covered_seq_indices.has (seq_اشاریہ_2))
    {
      has_pos_glyphs = سچ;
      pos_glyphs.clear ();
      اگر (seq_اشاریہ_2 == 0)
      {
        انتخاب (context_format) {
        حالت ContextFormat::SimpleContext:
          pos_glyphs.جوڑو (value);
          توڑو;
        حالت ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->والد_فعال_glyphs (), ڈیٹا, value, &pos_glyphs, cache);
          توڑو;
        حالت ContextFormat::CoverageBasedContext:
          pos_glyphs.سیٹ (c->والد_فعال_glyphs ());
          توڑو;
        }
      }
      ورنہ
      {
        ثابت خالی *داخلہ_ڈیٹا = داخلہ.ڈیٹا;
        غیرعلامتی داخلہ_قدر = seq_اشاریہ_2 - 1;
        اگر (context_format != ContextFormat::SimpleContext)
        {
          داخلہ_ڈیٹا = ڈیٹا;
          داخلہ_قدر = داخلہ[seq_اشاریہ_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, داخلہ_ڈیٹا, داخلہ_قدر, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.جوڑو (seq_اشاریہ_2);
    اگر (has_pos_glyphs)
    {
      hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
      اگر (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_اشاریہ_2 ? pos_glyphs.get_min () : value;
	اگر (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_فعال_FLAG))
	  فعال_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_فعال_FLAG;
      }
      hb_سیٹ_t *cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs (فعال_idx);
      اگر (unlikely (!cur_فعال_glyphs))
      {
	c->depend_ڈیٹا->current_context_سیٹ_اشاریہ = saved_context_سیٹ_اشاریہ;
	c->depend_ڈیٹا->current_کنارہ_flags = saved_کنارہ_flags;
	واپس;
      }
      *cur_فعال_glyphs = std::move (pos_glyphs);
    }
    ورنہ اگر (unlikely (!c->ڈالو_جڑ_فعال_glyphs ()))
    {
      c->depend_ڈیٹا->current_context_سیٹ_اشاریہ = saved_context_سیٹ_اشاریہ;
      c->depend_ڈیٹا->current_کنارہ_flags = saved_کنارہ_flags;
      واپس;
    }

    غیرعلامتی انتہا_اشاریہ_2 = داخلہ_گنتی;
    اگر (context_format == ContextFormat::CoverageBasedContext)
      انتہا_اشاریہ_2 += 1;

    c->recurse (lookup_ریکارڈ[i].lookup_List_اشاریہ, &covered_seq_indices, seq_اشاریہ_2, انتہا_اشاریہ_2);
    c->نکالو_cur_مکمل_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_ڈیٹا->current_context_سیٹ_اشاریہ = saved_context_سیٹ_اشاریہ;
    c->depend_ڈیٹا->current_کنارہ_flags = saved_کنارہ_flags;
  }
}

سانچہ <قسم_نام context_t>
ساکن اندرونی خالی recurse_lookups (context_t *c,
                                    غیرعلامتی صحیح lookup_گنتی_2,
                                    ثابت Lookup_ریکارڈ lookup_ریکارڈ[] /* Array of LookupRecords--in design order */)
{
  برائے (غیرعلامتی صحیح i = 0; i < lookup_گنتی_2; i++)
    c->recurse (lookup_ریکارڈ[i].lookup_List_اشاریہ);
}

ساکن اندرونی خالی apply_lookup (hb_ot_apply_context_t *c,
				 غیرعلامتی صحیح گنتی, /* Including the first glyph */
				 غیرعلامتی صحیح lookup_گنتی_2,
				 ثابت Lookup_ریکارڈ lookup_ریکارڈ[], /* Array of LookupRecords--in design order */
				 غیرعلامتی صحیح مطابقت_انتہا)
{
  hb_بفر_t *بفر = c->بفر;
  صحیح انتہا;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    غیرعلامتی صحیح bl = بفر->backtrack_len ();
    انتہا = bl + مطابقت_انتہا - بفر->idx;

    صحیح delta = bl - بفر->idx;
    /* Convert positions to new indexing. */
    برائے (غیرعلامتی صحیح j = 0; j < گنتی; j++)
      c->مطابقت_positions.arrayZ[j] += delta;
  }

  برائے (غیرعلامتی صحیح i = 0; i < lookup_گنتی_2 && بفر->successful; i++)
  {
    غیرعلامتی صحیح idx = lookup_ریکارڈ[i].sequence_اشاریہ;
    اگر (idx >= گنتی)
      جاری;

    غیرعلامتی صحیح orig_len = بفر->backtrack_len () + بفر->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    اگر (unlikely (c->مطابقت_positions.arrayZ[idx] >= orig_len))
      جاری;

    اگر (unlikely (!بفر->move_مقصد (c->مطابقت_positions.arrayZ[idx])))
      توڑو;

    اگر (unlikely (بفر->max_ops <= 0))
      توڑو;

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      اگر (بفر->have_output)
        c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "start recursing to lookup %u at %u",
			  (غیرعلامتی) lookup_ریکارڈ[i].lookup_List_اشاریہ,
			  بفر->idx);
    }

    منطقی ret = c->recurse (lookup_ریکارڈ[i].lookup_List_اشاریہ);

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      اگر (بفر->have_output)
        c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "end recursing to lookup %u",
			  (غیرعلامتی) lookup_ریکارڈ[i].lookup_List_اشاریہ);
    }

    اگر (!ret)
      جاری;

    غیرعلامتی صحیح new_len = بفر->backtrack_len () + بفر->lookahead_len ();
    صحیح delta = new_len - orig_len;

    اگر (!delta)
      جاری;

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

    انتہا += delta;
    اگر (انتہا < صحیح (c->مطابقت_positions.arrayZ[idx]))
    {
      /* End might end up being smaller than match_positions.arrayZ[idx] if the recursed
       * lookup ended up removing many items.
       * Just never rewind end beyond start of current position, since that is
       * not possible in the recursed lookup.  Also adjust delta as such.
       *
       * https://bugs.chromium.org/p/chromium/issues/detail?id=659496
       * https://github.com/harfbuzz/harfbuzz/issues/1611
       */
      delta += c->مطابقت_positions.arrayZ[idx] - انتہا;
      انتہا = c->مطابقت_positions.arrayZ[idx];
    }

    غیرعلامتی صحیح next = idx + 1; /* next now is the position after the recursed lookup. */

    اگر (delta > 0)
    {
      اگر (unlikely (delta + گنتی > HB_MAX_CONTEXT_لمبائی))
	توڑو;
      اگر (unlikely (گنتی + delta > c->مطابقت_positions.لمبائی &&
		    !c->مطابقت_positions.resize_dirty  (گنتی + delta)))
        واپس;
    }
    ورنہ
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (صحیح) next - (صحیح) گنتی);
      next -= delta;
    }

    /* Shift! */
    memmove (c->مطابقت_positions + next + delta, c->مطابقت_positions + next,
	     (گنتی - next) * حجم (c->مطابقت_positions.arrayZ[0]));
    next += delta;
    گنتی += delta;

    /* Fill in new entries. */
    برائے (غیرعلامتی صحیح j = idx + 1; j < next; j++)
      c->مطابقت_positions.arrayZ[j] = c->مطابقت_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    برائے (; next < گنتی; next++)
      c->مطابقت_positions.arrayZ[next] += delta;
  }

  أكد (انتہا >= 0);
  (خالی) بفر->move_مقصد (انتہا);
}



/* Contextual lookups */

ساخت Context_احاطہ_Lookup_Context
{
  Context_احاطہ_Funcs funcs;
  ContextFormat context_format;
  ثابت خالی *intersects_ڈیٹا;
  خالی *intersects_cache;
  خالی *intersected_glyphs_cache;
};

قسمتعریف Context_احاطہ_Lookup_Context ContextDependLookupContext;

ساخت ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت خالی *collect_ڈیٹا;
};

ساخت ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ثابت خالی *مطابقت_ڈیٹا_2;
  get_قدر_func_t get_قدر;
};

ساکن اندرونی منطقی context_intersects (ثابت hb_سیٹ_t *glyphs,
				       غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
				       glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
				       Context_احاطہ_Lookup_Context &lookup_context)
{
  واپس array_is_subset_کا (glyphs,
			     داخلہ_گنتی ? داخلہ_گنتی - 1 : 0, داخلہ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_ڈیٹا,
			     lookup_context.intersects_cache);
}

ساکن اندرونی خالی context_احاطہ_lookup (hb_احاطہ_context_t *c,
					   غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
					   glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
					   غیرعلامتی صحیح lookup_گنتی_2,
					   ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
					   غیرعلامتی value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   Context_احاطہ_Lookup_Context &lookup_context)
{
  اگر (context_intersects (c->glyphs,
			  داخلہ_گنتی, داخلہ,
			  lookup_context))
    context_احاطہ_recurse_lookups (c,
				     داخلہ_گنتی, داخلہ,
				     lookup_گنتی_2, lookup_ریکارڈ,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_ڈیٹا,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ساکن اندرونی خالی context_depend_lookup (hb_depend_context_t *c,
					  غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
					  glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
					  غیرعلامتی صحیح lookup_گنتی_2,
					  ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
					  ثابت خالی *قاعدہ_2,
					  غیرعلامتی value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  اگر (!context_intersects (c->glyphs,
			   داخلہ_گنتی, داخلہ,
			   lookup_context))
    واپس;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  اگر (unlikely (!scratch))
    واپس;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقی context_sets_cached = context_depend_sets_are_cached (
    c, داخلہ_گنتی, lookup_گنتی_2, lookup_ریکارڈ, قاعدہ_2, value,
    lookup_context.context_format, lookup_context.intersects_ڈیٹا,
    &scratch->cached_context_sets);
  اگر (unlikely (!c->depend_ڈیٹا->successful))
    واپس;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_سیٹ_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_سیٹ_t &position_scratch = scratch->position_scratch;
  hb_سمتیہ_t<ثابت hb_سیٹ_t *> &داخلہ_position_glyphs = scratch->داخلہ_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  اگر (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ثابت hb_سیٹ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_ڈیٹا, value,
      سچ, &position_scratch, &scratch->compact_workspace);
    اگر (unlikely (!pos0_glyphs ||
		  !داخلہ_position_glyphs.ڈالو_or_fail (pos0_glyphs)))
    {
      c->depend_ڈیٹا->fail ();
      واپس;
    }

    /* Positions 1+ (Input array) */
    برائے (غیرعلامتی i = 0; i + 1 < داخلہ_گنتی; i++)
    {
      ثابت hb_سیٹ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_ڈیٹا,
	داخلہ[i], جھوٹ, &position_scratch);
      اگر (unlikely (!pos_glyphs ||
		    !داخلہ_position_glyphs.ڈالو_or_fail (pos_glyphs)))
      {
	c->depend_ڈیٹا->fail ();
	واپس;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  داخلہ_گنتی, داخلہ,
				  lookup_گنتی_2, lookup_ریکارڈ,
				  قاعدہ_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_ڈیٹا,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? خالی_اشاریہ : &داخلہ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : خالی_اشاریہ,
				  scratch);
}

ساکن اندرونی خالی context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
						  glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
						  غیرعلامتی صحیح lookup_گنتی_2,
						  ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->داخلہ,
		 داخلہ_گنتی ? داخلہ_گنتی - 1 : 0, داخلہ,
		 lookup_context.funcs.collect, lookup_context.collect_ڈیٹا);
  recurse_lookups (c,
		   lookup_گنتی_2, lookup_ریکارڈ);
}

ساکن اندرونی منطقی context_would_apply_lookup (hb_would_apply_context_t *c,
					       غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
					       glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
					       غیرعلامتی صحیح lookup_گنتی_2 HB_غیر_مستعمل,
					       ثابت Lookup_ریکارڈ lookup_ریکارڈ[] HB_غیر_مستعمل,
					       ثابت ContextApplyLookupContext &lookup_context)
{
  واپس would_مطابقت_داخلہ (c,
			    داخلہ_گنتی, داخلہ,
			    lookup_context.funcs.مطابقت, lookup_context.مطابقت_ڈیٹا_2);
}

ساکن اندرونی منطقی context_apply_lookup (hb_ot_apply_context_t *c,
					 غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
					 glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
					 غیرعلامتی صحیح lookup_گنتی_2,
					 ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
					 ثابت ContextApplyLookupContext &lookup_context)
{
  اگر (unlikely (داخلہ_گنتی > HB_MAX_CONTEXT_لمبائی)) واپس جھوٹ;

  غیرعلامتی مطابقت_انتہا = 0;
  منطقی ret = جھوٹ;
  اگر (مطابقت_داخلہ (c,
		   داخلہ_گنتی, داخلہ,
		   lookup_context.funcs.مطابقت, lookup_context.مطابقت_ڈیٹا_2,
		   &مطابقت_انتہا))
  {
    c->بفر->unsafe_مقصد_break (c->بفر->idx, مطابقت_انتہا);
    apply_lookup (c,
		  داخلہ_گنتی,
		  lookup_گنتی_2, lookup_ریکارڈ,
		  مطابقت_انتہا);
    ret = سچ;
  }
  ورنہ
  {
    c->بفر->unsafe_مقصد_concat (c->بفر->idx, مطابقت_انتہا);
    ret = جھوٹ;
  }

  واپس ret;
}

ساکن اندرونی منطقی context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  انتخاب (op)
  {
    حالت hb_ot_subtable_cache_op_t::ENTER:
    {
      اگر (!HB_بفر_TRY_ALLOCATE_VAR (c->بفر, syllable))
	واپس جھوٹ;
      خودکار &info = c->بفر->info;
      غیرعلامتی گنتی = c->بفر->len;
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      واپس سچ;
    }
    حالت hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (غیرعلامتی) -1;
      HB_بفر_DEALLOCATE_VAR (c->بفر, syllable);
      توڑو;
    }
  }
  واپس جھوٹ;
}

سانچہ <قسم_نام Types>
ساخت قاعدہ_3
{
  سانچہ <قسم_نام سیٹ_Types, قسم_نام قاعدہ_Type>
  دوست ساخت قاعدہ_سیٹ_کا;

  منطقی intersects (ثابت hb_سیٹ_t *glyphs, Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    واپس context_intersects (glyphs,
			       داخلہ_گنتی, داخلہ_Z.arrayZ,
			       lookup_context);
  }

  خالی depend (hb_depend_context_t *c, غیرعلامتی value, ContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_کا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array ((داخلہ_گنتی ? داخلہ_گنتی - 1 : 0)));
    context_depend_lookup (c,
			   داخلہ_گنتی, داخلہ_Z.arrayZ,
			   lookup_گنتی_2, lookup_ریکارڈ.arrayZ,
			   یہ,
			   value, lookup_context);
  }

  خالی احاطہ (hb_احاطہ_context_t *c, غیرعلامتی value, Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_حد_بندی_exceeded ())) واپس;

    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_کا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array ((داخلہ_گنتی ? داخلہ_گنتی - 1 : 0)));
    context_احاطہ_lookup (c,
			    داخلہ_گنتی, داخلہ_Z.arrayZ,
			    lookup_گنتی_2, lookup_ریکارڈ.arrayZ,
			    value, lookup_context);
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c,
                        Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_حد_بندی_exceeded ())) واپس;
    اگر (!intersects (c->glyphs, lookup_context)) واپس;

    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_کا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0));
    recurse_lookups (c, lookup_گنتی_2, lookup_ریکارڈ.arrayZ);
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_کا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0));
    context_collect_glyphs_lookup (c,
				   داخلہ_گنتی, داخلہ_Z.arrayZ,
				   lookup_گنتی_2, lookup_ریکارڈ.arrayZ,
				   lookup_context);
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest,
			      ثابت ClassDef *class_def = خالی_اشاریہ) ثابت
  {
    اگر (داخلہ_گنتی <= 1)
    {
      *digest = hb_سیٹ_digest_t::بھرا ();
      واپس;
    }

    غیرعلامتی second = داخلہ_Z.arrayZ[0];
    اگر (!class_def)
      digest->جوڑو (second);
    ورنہ اگر (!second)
      *digest = hb_سیٹ_digest_t::بھرا ();
    ورنہ
      class_def->collect_class (digest, second);
  }

  منطقی would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_کا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0));
    واپس context_would_apply_lookup (c,
				       داخلہ_گنتی, داخلہ_Z.arrayZ,
				       lookup_گنتی_2, lookup_ریکارڈ.arrayZ,
				       lookup_context);
  }

  منطقی apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (یہ);
    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_کا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0));
    return_trace (context_apply_lookup (c, داخلہ_گنتی, داخلہ_Z.arrayZ, lookup_گنتی_2, lookup_ریکارڈ.arrayZ, lookup_context));
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *داخلہ_mapping, /* old->new glyphid or class mapping */
		  ثابت hb_map_t *lookup_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->آغاز_embed (یہ);
    اگر (unlikely (!c->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->داخلہ_گنتی = داخلہ_گنتی;
    ثابت خودکار داخلہ = داخلہ_Z.as_array (داخلہ_گنتی - 1);
    برائے (ثابت خودکار org : داخلہ)
    {
      قسم_نام Types::HBUINT d;
      d = داخلہ_mapping->get (org);
      c->copy (d);
    }

    ثابت خودکار &lookup_ریکارڈ = StructAfter<Unsized_Array_کا<Lookup_ریکارڈ>>
					   (داخلہ_Z.as_array ((داخلہ_گنتی ? داخلہ_گنتی - 1 : 0)));

    غیرعلامتی گنتی = serialize_lookuprecord_array (c, lookup_ریکارڈ.as_array (lookup_گنتی_2), lookup_map);
    return_trace (c->check_تفویض (برآمد->lookup_گنتی_2, گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (unlikely (!داخلہ_گنتی)) return_trace (جھوٹ);
    ثابت خودکار داخلہ = داخلہ_Z.as_array (داخلہ_گنتی - 1);

    ثابت hb_map_t *mapping = klass_map == خالی_اشاریہ ? c->plan->glyph_map : klass_map;
    اگر (!hb_all (داخلہ, mapping)) return_trace (جھوٹ);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  عوامی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  c->check_range (داخلہ_Z.arrayZ,
				  داخلہ_Z.شے_سائز * (داخلہ_گنتی ? داخلہ_گنتی - 1 : 0) +
				  Lookup_ریکارڈ::static_سائز * lookup_گنتی_2));
  }

  محفوظ:
  HBUINT16	داخلہ_گنتی;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_گنتی_2;		/* Number of LookupRecords */
  Unsized_Array_کا<قسم_نام Types::HBUINT>
		داخلہ_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عوامی:
  DEFINE_سائز_ARRAY (4, داخلہ_Z);
};

سانچہ <قسم_نام Types, قسم_نام قاعدہ_Type>
ساخت قاعدہ_سیٹ_کا
{
  منطقی intersects (ثابت hb_سیٹ_t *glyphs,
		   Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    واپس
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([&] (ثابت قاعدہ_Type &_) { واپس _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  خالی collect_پہلا_داخلہ_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    برائے (غیرعلامتی i = 0; i < قاعدہ_2.len; i++)
    {
      ثابت قاعدہ_Type &r = یہ+قاعدہ_2.arrayZ[i];
      اگر (r.داخلہ_گنتی <= 1)
      {
	digest->سیٹ_بھرا ();
	واپس;
      }
      digest->جوڑو (r.داخلہ_Z.arrayZ[0]);
    }
  }

  خالی depend (hb_depend_context_t *c, غیرعلامتی value, ContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت قاعدہ_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }

  خالی احاطہ (hb_احاطہ_context_t *c, غیرعلامتی value,
		Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_حد_بندی_exceeded ())) واپس;

    واپس
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت قاعدہ_Type &_) { _.احاطہ (c, value, lookup_context); })
    ;
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c,
                        Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_حد_بندی_exceeded ())) واپس;
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت قاعدہ_Type &_) { _.احاطہ_lookups (c, lookup_context); })
    ;
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    واپس
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت قاعدہ_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest,
			      ثابت ClassDef *class_def = خالی_اشاریہ) ثابت
  {
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([=] (ثابت قاعدہ_Type &_) { _.collect_دوسرا_glyphs (digest, class_def); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    واپس
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([&] (ثابت قاعدہ_Type &_) { واپس _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = خالی_اشاریہ) ثابت
  {
    TRACE_APPLY (یہ);

    غیرعلامتی عدد_قواعد = قاعدہ_2.len;

#اگر_غیر_تعریف_شدہ HB_NO_OT_RULESETS_FAST_راستہ
    اگر (HB_OPTIMIZE_سائز_VAL || عدد_قواعد <= 4)
#اگر_ختم
    {
    slow:
      return_trace (
      + hb_iter (قاعدہ_2)
      | hb_map (hb_جوڑو (یہ))
      | hb_map ([&] (ثابت قاعدہ_Type &_) { واپس _.apply (c, lookup_context); })
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
    skippy_iter.سیٹ_مطابقت_func (مطابقت_always, خالی_اشاریہ);
    skippy_iter.سیٹ_glyph_ڈیٹا ((HBUINT16 *) خالی_اشاریہ);
    غیرعلامتی unsafe_مقصد = (غیرعلامتی) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *پہلا = خالی_اشاریہ, *second = خالی_اشاریہ;
    منطقی matched = skippy_iter.next (&unsafe_to1);
    اگر (likely (matched))
    {
      اگر (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        جاؤ slow;
      }

      پہلا = &c->بفر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      اگر (digest && lookup_context.get_قدر &&
	  !digest->may_have (lookup_context.get_قدر (*پہلا, lookup_context.مطابقت_ڈیٹا_2)))
	{
	  c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_to1);
	  return_trace (جھوٹ);
	}
    }
    ورنہ
    {
      منطقی unsafe_مقصد_concat = جھوٹ;
      منطقی ret =
      + hb_iter (قاعدہ_2)
      | hb_map (hb_جوڑو (یہ))
      | hb_filter ([&] (ثابت قاعدہ_Type &_)
		   {
		     اگر (_.داخلہ_گنتی <= 1) واپس سچ;
		     unsafe_مقصد_concat = سچ;
		     واپس جھوٹ;
		   })
      | hb_map ([&] (ثابت قاعدہ_Type &_) { واپس _.apply (c, lookup_context); })
      | hb_any
      ;
      اگر (unsafe_مقصد_concat)
	c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    اگر (likely (matched))
    {
      اگر (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        جاؤ slow;
      }

      second = &c->بفر->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    خودکار مطابقت_داخلہ = lookup_context.funcs.مطابقت;
    خودکار *داخلہ_ڈیٹا = lookup_context.مطابقت_ڈیٹا_2;
    برائے (غیرعلامتی صحیح i = 0; i < عدد_قواعد; i++)
    {
      ثابت خودکار &r = یہ+قاعدہ_2.arrayZ[i];

      ثابت خودکار &داخلہ = r.داخلہ_Z;

      اگر (r.داخلہ_گنتی <= 1 ||
	  (!مطابقت_داخلہ ||
	   مطابقت_داخلہ (*پہلا, داخلہ.arrayZ[0], داخلہ_ڈیٹا)))
      {
        اگر (!second ||
	    (r.داخلہ_گنتی <= 2 ||
	     (!مطابقت_داخلہ ||
	      مطابقت_داخلہ (*second, داخلہ.arrayZ[1], داخلہ_ڈیٹا)))
	   )
	{
	  اگر (r.apply (c, lookup_context))
	  {
	    اگر (unsafe_مقصد != (غیرعلامتی) -1)
	      c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_مقصد);
	    return_trace (سچ);
	  }
	}
	ورنہ
	  unsafe_مقصد = unsafe_to2;
      }
      ورنہ
      {
	اگر (unsafe_مقصد == (غیرعلامتی) -1)
	  unsafe_مقصد = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	برائے (; i + 1 < عدد_قواعد; i++)
	{
	  ثابت خودکار &r2 = یہ+قاعدہ_2.arrayZ[i + 1];
	  ثابت خودکار &input2 = r2.داخلہ_Z;
	  اگر (r2.داخلہ_گنتی <= 1 || input2.arrayZ[0] != داخلہ.arrayZ[0])
	    توڑو;
	}
      }
    }
    اگر (likely (unsafe_مقصد != (غیرعلامتی) -1))
      c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_مقصد);

    return_trace (جھوٹ);
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);

    خودکار snap = c->serializer->snapshot ();
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برائے (ثابت خودکار& _ : قاعدہ_2)
    {
      اگر (!_) جاری;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = برآمد->قاعدہ_2.serialize_append (c->serializer);
      اگر (unlikely (!o)) جاری;

      اگر (!o->serialize_subset (c, _, یہ, lookup_map, klass_map))
      {
	برآمد->قاعدہ_2.نکالو ();
	c->serializer->revert (o_snap);
      }
    }

    منطقی ret = منطقی (برآمد->قاعدہ_2);
    اگر (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (قاعدہ_2.sanitize (c, یہ));
  }

  محفوظ:
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<قاعدہ_Type>>
		قاعدہ_2;			/* Array of Rule tables
					 * ordered by preference */
  عوامی:
  DEFINE_سائز_ARRAY (2, قاعدہ_2);
};

سانچہ <قسم_نام Types>
ساخت قاعدہ_سیٹ : قاعدہ_سیٹ_کا<SmallTypes, قاعدہ_3<Types>> {};

سانچہ <قسم_نام Types>
ساخت Class_قاعدہ_سیٹ : قاعدہ_سیٹ_کا<Types, قاعدہ_3<SmallTypes>> {};


سانچہ <قسم_نام Types>
ساخت ContextFormat1_4
{
  استعمال قاعدہ_سیٹ = OT::قاعدہ_سیٹ<Types>;

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    ساخت Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالی_اشاریہ
    };

    واپس
    + hb_zip (یہ+coverage, قاعدہ_سیٹ_2)
    | hb_filter (*glyphs, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([&] (ثابت قاعدہ_سیٹ &_) { واپس _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { واپس سچ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_فعال_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالی_اشاریہ,
      خالی_اشاریہ,  /* intersects_cache */
      خالی_اشاریہ   /* intersected_glyphs_cache */
    };

    + hb_zip (یہ+coverage, hb_range ((غیرعلامتی) قاعدہ_سیٹ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      واپس c->والد_فعال_glyphs ().has (_);
    }, hb_پہلا)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, غیرعلامتی> _) { واپس hb_pair_t<غیرعلامتی, ثابت قاعدہ_سیٹ&> (_.پہلا, یہ+قاعدہ_سیٹ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت قاعدہ_سیٹ&>& _) { _.second.depend (c, _.پہلا, lookup_context); })
    ;

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (c->پچھلا_والد_فعال_glyphs (), *cur_فعال_glyphs);

    ساخت Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      خالی_اشاریہ
    };

    + hb_zip (یہ+coverage, hb_range ((غیرعلامتی) قاعدہ_سیٹ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      واپس c->پچھلا_والد_فعال_glyphs ().has (_);
    }, hb_پہلا)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, غیرعلامتی> _) { واپس hb_pair_t<غیرعلامتی, ثابت قاعدہ_سیٹ&> (_.پہلا, یہ+قاعدہ_سیٹ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت قاعدہ_سیٹ&>& _) { _.second.احاطہ (c, _.پہلا, lookup_context); })
    ;

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت
  {
    ساخت Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_glyph, خالی_اشاریہ},
      ContextFormat::SimpleContext,
      خالی_اشاریہ
    };

    + hb_zip (یہ+coverage, قاعدہ_سیٹ_2)
    | hb_filter (*c->glyphs, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت قاعدہ_سیٹ &_) { _.احاطہ_lookups (c, lookup_context); })
    ;
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (یہ+coverage).collect_coverage (c->داخلہ);

    ساخت ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      خالی_اشاریہ
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت قاعدہ_سیٹ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([digest] (ثابت قاعدہ_سیٹ &_) { _.collect_دوسرا_glyphs (digest); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت قاعدہ_سیٹ &قاعدہ_سیٹ_3 = یہ+قاعدہ_سیٹ_2[(یہ+coverage).get_coverage (c->glyphs[0])];
    ساخت ContextApplyLookupContext lookup_context = {
      {مطابقت_glyph},
      خالی_اشاریہ
    };
    واپس قاعدہ_سیٹ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->بفر->cur().codepoint);
    اگر (likely (اشاریہ_2 == NOT_COVERED))
      return_trace (جھوٹ);

    ثابت قاعدہ_سیٹ &قاعدہ_سیٹ_3 = یہ+قاعدہ_سیٹ_2[اشاریہ_2];
    ساخت ContextApplyLookupContext lookup_context = {
      {مطابقت_glyph},
      خالی_اشاریہ
    };
    return_trace (قاعدہ_سیٹ_3.apply (c, lookup_context));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_لیبل_2 == HB_OT_لیبل_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (یہ+coverage, قاعدہ_سیٹ_2)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_انحراف_array (c, برآمد->قاعدہ_سیٹ_2, یہ, lookup_map), hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    برآمد->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && قاعدہ_سیٹ_2.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<قاعدہ_سیٹ>>
		قاعدہ_سیٹ_2;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز, قاعدہ_سیٹ_2);
};


سانچہ <قسم_نام Types>
ساخت ContextFormat2_5
{
  استعمال قاعدہ_سیٹ = OT::Class_قاعدہ_سیٹ<Types>;

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    اگر (!(یہ+coverage).intersects (glyphs))
      واپس جھوٹ;

    ثابت ClassDef &class_def = یہ+classDef;

    hb_map_t cache;
    ساخت Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_class, خالی_اشاریہ},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_سیٹ_t retained_coverage_glyphs;
    (یہ+coverage).intersect_سیٹ (*glyphs, retained_coverage_glyphs);

    hb_سیٹ_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    واپس
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت قاعدہ_سیٹ &> p)
	      { واپس class_def.intersects_class (glyphs, p.پہلا) &&
		       coverage_glyph_classes.has (p.پہلا) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { واپس سچ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    اگر (!(یہ+coverage).intersects (c->glyphs))
      واپس;

    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_فعال_glyphs);

    ثابت ClassDef &class_def = یہ+classDef;

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
    | hb_filter ([&] (غیرعلامتی _)
    { واپس class_def.intersects_class (&c->والد_فعال_glyphs (), _); },
		 hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت قسم_نام Types::سانچہ انحراف_مقصد<قاعدہ_سیٹ>&> _)
                {
                  ثابت قاعدہ_سیٹ& قاعدہ_سیٹ_3 = یہ+_.second;
                  قاعدہ_سیٹ_3.depend (c, _.پہلا, lookup_context);
                })
    ;

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    اگر (!(یہ+coverage).intersects (c->glyphs))
      واپس;

    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (c->پچھلا_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت ClassDef &class_def = یہ+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ساخت Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (قاعدہ_سیٹ_2)
    | hb_filter ([&] (غیرعلامتی _)
    { واپس class_def.intersects_class (&c->والد_فعال_glyphs (), _); },
		 hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت قسم_نام Types::سانچہ انحراف_مقصد<قاعدہ_سیٹ>&> _)
                {
                  ثابت قاعدہ_سیٹ& قاعدہ_سیٹ_3 = یہ+_.second;
                  قاعدہ_سیٹ_3.احاطہ (c, _.پہلا, lookup_context);
                })
    ;

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت
  {
    اگر (!(یہ+coverage).intersects (c->glyphs))
      واپس;

    ثابت ClassDef &class_def = یہ+classDef;

    hb_map_t cache;
    ساخت Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_class, خالی_اشاریہ},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_enumerate
    | hb_filter ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت قاعدہ_سیٹ &> p)
    { واپس class_def.intersects_class (c->glyphs, p.پہلا); })
    | hb_map (hb_دوسرا)
    | hb_apply ([&] (ثابت قاعدہ_سیٹ & _)
    { _.احاطہ_lookups (c, lookup_context); });
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (یہ+coverage).collect_coverage (c->داخلہ);

    ثابت ClassDef &class_def = یہ+classDef;
    ساخت ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت قاعدہ_سیٹ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    ثابت ClassDef &class_def = یہ+classDef;
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([=, &class_def] (ثابت قاعدہ_سیٹ &_) { _.collect_دوسرا_glyphs (digest, &class_def); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &class_def = یہ+classDef;
    غیرعلامتی صحیح اشاریہ_2 = class_def.get_class (c->glyphs[0]);
    ثابت قاعدہ_سیٹ &قاعدہ_سیٹ_3 = یہ+قاعدہ_سیٹ_2[اشاریہ_2];
    ساخت ContextApplyLookupContext lookup_context = {
      {مطابقت_class},
      &class_def
    };
    واپس قاعدہ_سیٹ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  غیرعلامتی cache_لاگت () ثابت
  {
    واپس (یہ+classDef).لاگت ();
  }
  ساکن منطقی cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    واپس context_cache_func (c, op);
  }

  ساخت external_cache_t
  {
    hb_ot_layout_ثنائی_cache_t coverage;
    hb_ot_layout_ruleset_digest_t قاعدہ_sets[HB_VAR_ARRAY];
  };
  خالی *external_cache_create () ثابت
  {
    غیرعلامتی گنتی = قاعدہ_سیٹ_2.len;
    غیرعلامتی سائز = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    گنتی * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (سائز);
    اگر (likely (cache))
    {
      cache->coverage.clear ();
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
	(یہ+قاعدہ_سیٹ_2[i]).collect_پہلا_داخلہ_classes (&cache->قاعدہ_sets[i]);
    }
    واپس cache;
  }
  منطقی apply_cached (hb_ot_apply_context_t *c, خالی *external_cache) ثابت { واپس _apply (c, سچ, external_cache); }
  منطقی apply (hb_ot_apply_context_t *c, خالی *external_cache) ثابت { واپس _apply (c, جھوٹ, external_cache); }
  منطقی _apply (hb_ot_apply_context_t *c, منطقی cached, خالی *external_cache) ثابت
  {
    TRACE_APPLY (یہ);
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage_ثنائی (c->بفر->cur().codepoint, cache ? &cache->coverage : خالی_اشاریہ);
#ورنہ
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->بفر->cur().codepoint);
#اگر_ختم
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    ثابت ClassDef &class_def = یہ+classDef;

    ساخت ContextApplyLookupContext lookup_context = {
      {cached ? مطابقت_class_cached : مطابقت_class},
      &class_def,
      cached ? get_class_cached_قدر : get_class_قدر
    };

    اشاریہ_2 = cached ? get_class_cached (class_def, c->بفر->cur()) : class_def.get_class (c->بفر->cur().codepoint);
    ثابت قاعدہ_سیٹ &قاعدہ_سیٹ_3 = یہ+قاعدہ_سیٹ_2[اشاریہ_2];
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && اشاریہ_2 < قاعدہ_سیٹ_2.len ? &cache->قاعدہ_sets[اشاریہ_2] : خالی_اشاریہ;
#ورنہ
    ثابت hb_ot_layout_ruleset_digest_t *digest = خالی_اشاریہ;
#اگر_ختم
    return_trace (قاعدہ_سیٹ_3.apply (c, lookup_context, digest));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;
    اگر (unlikely (!برآمد->coverage.serialize_subset (c, coverage, یہ)))
      return_trace (جھوٹ);

    hb_map_t klass_map;
    برآمد->classDef.serialize_subset (c, classDef, یہ, &klass_map);

    ثابت hb_سیٹ_t* glyphset = c->plan->glyphset_gsub ();
    hb_سیٹ_t retained_coverage_glyphs;
    (یہ+coverage).intersect_سیٹ (*glyphset, retained_coverage_glyphs);

    hb_سیٹ_t coverage_glyph_classes;
    (یہ+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ثابت hb_map_t *lookup_map = c->جدول_لیبل_2 == HB_OT_لیبل_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    منطقی ret = سچ;
    صحیح non_zero_اشاریہ = -1, اشاریہ_2 = 0;
    خودکار snapshot = c->serializer->snapshot();
    برائے (ثابت خودکار& _ : + hb_enumerate (قاعدہ_سیٹ_2)
			 | hb_filter (klass_map, hb_پہلا))
    {
      خودکار *o = برآمد->قاعدہ_سیٹ_2.serialize_append (c->serializer);
      اگر (unlikely (!o))
      {
	ret = جھوٹ;
	توڑو;
      }

      اگر (coverage_glyph_classes.has (_.پہلا) &&
	  o->serialize_subset (c, _.second, یہ, lookup_map, &klass_map)) {
	non_zero_اشاریہ = اشاریہ_2;
        snapshot = c->serializer->snapshot();
      }

      اشاریہ_2++;
    }

    اگر (!ret || non_zero_اشاریہ == -1) return_trace (جھوٹ);

    //prune empty trailing ruleSets
    --اشاریہ_2;
    جب (اشاریہ_2 > non_zero_اشاریہ)
    {
      برآمد->قاعدہ_سیٹ_2.نکالو ();
      اشاریہ_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (منطقی (برآمد->قاعدہ_سیٹ_2));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && classDef.sanitize (c, یہ) && قاعدہ_سیٹ_2.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;			/* Format identifier--format = 2 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_نام Types::سانچہ LOffset_مقصد<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<قاعدہ_سیٹ>>
		قاعدہ_سیٹ_2;		/* Array of RuleSet tables
					 * ordered by class */
  عوامی:
  DEFINE_سائز_ARRAY (2 + 2 * Types::LOffset::static_سائز + Types::HBUINT::static_سائز, قاعدہ_سیٹ_2);
};


سانچہ <قسم_نام Types>
ساخت ContextFormat3_6
{
  استعمال قاعدہ_سیٹ = OT::قاعدہ_سیٹ<SmallTypes>;

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    اگر (!(یہ+coverageZ[0]).intersects (glyphs))
      واپس جھوٹ;

    ساخت Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_coverage, خالی_اشاریہ},
      ContextFormat::CoverageBasedContext,
      یہ
    };
    واپس context_intersects (glyphs,
			       glyph_گنتی_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  منطقی may_have_non_1to1 () ثابت
  { واپس سچ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    اگر (!(یہ+coverageZ[0]).intersects (c->glyphs))
      واپس;

    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_فعال_glyphs);

    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      یہ,
      خالی_اشاریہ,  /* intersects_cache */
      خالی_اشاریہ   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_گنتی_2, (ثابت HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_گنتی_2, lookup_ریکارڈ,
			   یہ,
			   coverageZ[0], lookup_context);

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    اگر (!(یہ+coverageZ[0]).intersects (c->glyphs))
      واپس;

    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (c->پچھلا_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    ساخت Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      یہ
    };
    context_احاطہ_lookup (c,
			    glyph_گنتی_2, coverageZ.arrayZ + 1,
			    lookup_گنتی_2, lookup_ریکارڈ,
			    0, lookup_context);

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت
  {
    اگر (!intersects (c->glyphs))
      واپس;
    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    recurse_lookups (c, lookup_گنتی_2, lookup_ریکارڈ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (یہ+coverageZ[0]).collect_coverage (c->داخلہ);

    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    ساخت ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      یہ
    };

    context_collect_glyphs_lookup (c,
				   glyph_گنتی_2, coverageZ.arrayZ + 1,
				   lookup_گنتی_2, lookup_ریکارڈ,
				   lookup_context);
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    اگر (glyph_گنتی_2 <= 1)
      *digest = hb_سیٹ_digest_t::بھرا ();
    ورنہ
      (یہ+coverageZ[1]).collect_coverage (digest);
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    ساخت ContextApplyLookupContext lookup_context = {
      {مطابقت_coverage},
      یہ
    };
    واپس context_would_apply_lookup (c,
				       glyph_گنتی_2, coverageZ.arrayZ + 1,
				       lookup_گنتی_2, lookup_ریکارڈ,
				       lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverageZ[0]; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverageZ[0]).get_coverage (c->بفر->cur().codepoint);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    ساخت ContextApplyLookupContext lookup_context = {
      {مطابقت_coverage},
      یہ
    };
    return_trace (context_apply_lookup (c,
					glyph_گنتی_2,
					coverageZ.arrayZ + 1,
					lookup_گنتی_2, lookup_ریکارڈ,
					lookup_context));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->format = format;
    برآمد->glyph_گنتی_2 = glyph_گنتی_2;

    خودکار coverages = coverageZ.as_array (glyph_گنتی_2);

    برائے (ثابت خودکار& انحراف : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      خودکار *o = c->serializer->allocate_سائز<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>> (Types::انحراف_2::static_سائز);
      اگر (unlikely (!o)) return_trace (جھوٹ);
      اگر (!o->serialize_subset (c, انحراف, یہ)) return_trace (جھوٹ);
    }

    ثابت خودکار& lookup_ریکارڈ = StructAfter<Unsized_Array_کا<Lookup_ریکارڈ>> (coverageZ.as_array (glyph_گنتی_2));
    ثابت hb_map_t *lookup_map = c->جدول_لیبل_2 == HB_OT_لیبل_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    غیرعلامتی گنتی = serialize_lookuprecord_array (c->serializer, lookup_ریکارڈ.as_array (lookup_گنتی_2), lookup_map);
    return_trace (c->serializer->check_تفویض (برآمد->lookup_گنتی_2, گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ))) return_trace (جھوٹ);
    hb_barrier ();
    غیرعلامتی صحیح گنتی = glyph_گنتی_2;
    اگر (unlikely (!گنتی)) return_trace (جھوٹ); /* We want to access coverageZ[0] freely. */
    اگر (unlikely (!c->check_array (coverageZ.arrayZ, گنتی))) return_trace (جھوٹ);
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (unlikely (!coverageZ[i].sanitize (c, یہ))) return_trace (جھوٹ);
    ثابت Lookup_ریکارڈ *lookup_ریکارڈ = &StructAfter<Lookup_ریکارڈ> (coverageZ.as_array (glyph_گنتی_2));
    return_trace (likely (c->check_array (lookup_ریکارڈ, lookup_گنتی_2)));
  }

  محفوظ:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_گنتی_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_گنتی_2;		/* Number of LookupRecords */
  Unsized_Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عوامی:
  DEFINE_سائز_ARRAY (6, coverageZ);
};

ساخت Context
{
  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالت 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    حالت 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#اگر_ختم
  } u;
};


/* Chaining Contextual lookups */

ساخت Chain_Context_احاطہ_Lookup_Context
{
  Context_احاطہ_Funcs funcs;
  ContextFormat context_format;
  ثابت خالی *intersects_ڈیٹا[3];
  خالی *intersects_cache[3];
  خالی *intersected_glyphs_cache;
};

قسمتعریف Chain_Context_احاطہ_Lookup_Context ChainContextDependLookupContext;

ساخت ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت خالی *collect_ڈیٹا[3];
};

ساخت ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ثابت خالی *مطابقت_ڈیٹا_2[3];
  get_قدر_func_t get_داخلہ_قدر;
};

ساکن اندرونی منطقی chain_context_intersects (ثابت hb_سیٹ_t *glyphs,
					     غیرعلامتی صحیح backtrack_گنتی,
					     glyph_ڈیٹا_t backtrack,
					     غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
					     glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
					     غیرعلامتی صحیح lookahead_گنتی,
					     glyph_ڈیٹا_t lookahead,
					     Chain_Context_احاطہ_Lookup_Context &lookup_context)
{
  منطقی bt_ok = array_is_subset_کا (glyphs,
			     backtrack_گنتی, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_ڈیٹا[0],
			     lookup_context.intersects_cache[0]);
  منطقی اندر_ok = array_is_subset_کا (glyphs,
			     داخلہ_گنتی ? داخلہ_گنتی - 1 : 0, داخلہ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_ڈیٹا[1],
			     lookup_context.intersects_cache[1]);
  منطقی la_ok = array_is_subset_کا (glyphs,
			     lookahead_گنتی, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_ڈیٹا[2],
			     lookup_context.intersects_cache[2]);

  واپس bt_ok && اندر_ok && la_ok;
}

ساکن اندرونی خالی chain_context_احاطہ_lookup (hb_احاطہ_context_t *c,
						 غیرعلامتی صحیح backtrack_گنتی,
						 glyph_ڈیٹا_t backtrack,
						 غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
						 glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
						 غیرعلامتی صحیح lookahead_گنتی,
						 glyph_ڈیٹا_t lookahead,
						 غیرعلامتی صحیح lookup_گنتی_2,
						 ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
						 غیرعلامتی value,
						 Chain_Context_احاطہ_Lookup_Context &lookup_context)
{
  اگر (chain_context_intersects (c->glyphs,
				backtrack_گنتی, backtrack,
				داخلہ_گنتی, داخلہ,
				lookahead_گنتی, lookahead,
				lookup_context))
    context_احاطہ_recurse_lookups (c,
		     داخلہ_گنتی, داخلہ,
		     lookup_گنتی_2, lookup_ریکارڈ,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_ڈیٹا[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ساکن اندرونی خالی chain_context_depend_lookup (hb_depend_context_t *c,
						غیرعلامتی صحیح backtrack_گنتی,
						glyph_ڈیٹا_t backtrack,
						غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
						glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
						غیرعلامتی صحیح lookahead_گنتی,
						glyph_ڈیٹا_t lookahead,
						غیرعلامتی صحیح lookup_گنتی_2,
						ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
						ثابت خالی *قاعدہ_2,
						غیرعلامتی value,
						ChainContextDependLookupContext &lookup_context)
{
  اگر (!chain_context_intersects (c->glyphs,
				 backtrack_گنتی, backtrack,
				 داخلہ_گنتی, داخلہ,
				 lookahead_گنتی, lookahead,
				 lookup_context))
    واپس;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  اگر (unlikely (!scratch))
    واپس;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقی context_sets_cached = context_depend_sets_are_cached (
    c, داخلہ_گنتی, lookup_گنتی_2, lookup_ریکارڈ, قاعدہ_2, value,
    lookup_context.context_format, lookup_context.intersects_ڈیٹا[1],
    &scratch->cached_context_sets);
  اگر (unlikely (!c->depend_ڈیٹا->successful))
    واپس;
  اگر (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    داخلہ_گنتی, داخلہ,
				    lookup_گنتی_2, lookup_ریکارڈ,
				    قاعدہ_2,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_ڈیٹا[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    خالی_اشاریہ,
				    &scratch->cached_context_sets,
				    scratch);
    واپس;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_سیٹ_t &position_scratch = scratch->position_scratch;
  hb_سمتیہ_t<ثابت hb_سیٹ_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_سمتیہ_t<ثابت hb_سیٹ_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  برائے (غیرعلامتی i = 0; i < backtrack_گنتی; i++)
  {
    ثابت hb_سیٹ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_ڈیٹا[0],
      backtrack[i], جھوٹ, &position_scratch);
    اگر (unlikely (!pos_glyphs))
      واپس;
    اگر (!pos_glyphs->is_empty ())
    {
      اگر (unlikely (!backtrack_sets.ڈالو_or_fail (pos_glyphs)))
      {
        c->depend_ڈیٹا->fail ();
        واپس;
      }
    }
  }

  /* Extract lookahead glyphs */
  برائے (غیرعلامتی i = 0; i < lookahead_گنتی; i++)
  {
    ثابت hb_سیٹ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_ڈیٹا[2],
      lookahead[i], جھوٹ, &position_scratch);
    اگر (unlikely (!pos_glyphs))
      واپس;
    اگر (!pos_glyphs->is_empty ())
    {
      اگر (unlikely (!lookahead_sets.ڈالو_or_fail (pos_glyphs)))
      {
        c->depend_ڈیٹا->fail ();
        واپس;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_سیٹ_t &preliminary_context = scratch->preliminary_context;
  برائے (ثابت hb_سیٹ_t *عقب_سیٹ : backtrack_sets)
  {
    اگر (عقب_سیٹ->get_population () == 1) {
      preliminary_context.جوڑو (عقب_سیٹ->get_min ());  // Direct glyph
    } ورنہ اگر (عقب_سیٹ->get_population () > 1) {
      hb_codepoint_t سیٹ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_ڈیٹا->ڈھونڈو_or_create_context_سیٹ (*عقب_سیٹ)
        : c->ڈھونڈو_or_create_cached_context_سیٹ (عقب_سیٹ);
      اگر (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
        واپس;
      preliminary_context.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);  // Encoded set reference
    }
  }
  برائے (ثابت hb_سیٹ_t *look_سیٹ : lookahead_sets)
  {
    اگر (look_سیٹ->get_population () == 1) {
      preliminary_context.جوڑو (look_سیٹ->get_min ());  // Direct glyph
    } ورنہ اگر (look_سیٹ->get_population () > 1) {
      hb_codepoint_t سیٹ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_ڈیٹا->ڈھونڈو_or_create_context_سیٹ (*look_سیٹ)
        : c->ڈھونڈو_or_create_cached_context_سیٹ (look_سیٹ);
      اگر (unlikely (سیٹ_idx == HB_CODEPOINT_INVALID))
        واپس;
      preliminary_context.جوڑو (HB_DEPEND_CONTEXT_سیٹ_FLAG | سیٹ_idx);  // Encoded set reference
    }
  }
  اگر (unlikely (preliminary_context.اندر_error ()))
  {
    c->depend_ڈیٹا->fail ();
    واپس;
  }

  /* Build glyph sets for ALL input positions */
  hb_سمتیہ_t<ثابت hb_سیٹ_t *> &داخلہ_position_glyphs = scratch->داخلہ_position_glyphs;

  اگر (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ثابت hb_سیٹ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_ڈیٹا[1], value,
      سچ, &position_scratch, &scratch->compact_workspace);
    اگر (unlikely (!pos0_glyphs ||
		  !داخلہ_position_glyphs.ڈالو_or_fail (pos0_glyphs)))
    {
      c->depend_ڈیٹا->fail ();
      واپس;
    }

    /* Positions 1+ (Input array) */
    برائے (غیرعلامتی i = 0; i + 1 < داخلہ_گنتی; i++)
    {
      ثابت hb_سیٹ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_ڈیٹا[1],
	داخلہ[i], جھوٹ, &position_scratch);
      اگر (unlikely (!pos_glyphs ||
		    !داخلہ_position_glyphs.ڈالو_or_fail (pos_glyphs)))
      {
	c->depend_ڈیٹا->fail ();
	واپس;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  داخلہ_گنتی, داخلہ,
				  lookup_گنتی_2, lookup_ریکارڈ,
				  قاعدہ_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_ڈیٹا[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? خالی_اشاریہ : &داخلہ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : خالی_اشاریہ,
				  scratch);
}

ساکن اندرونی خالی chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							غیرعلامتی صحیح backtrack_گنتی,
							glyph_ڈیٹا_t backtrack,
							غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
							glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
							غیرعلامتی صحیح lookahead_گنتی,
							glyph_ڈیٹا_t lookahead,
							غیرعلامتی صحیح lookup_گنتی_2,
							ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_گنتی, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_ڈیٹا[0]);
  collect_array (c, c->داخلہ,
		 داخلہ_گنتی ? داخلہ_گنتی - 1 : 0, داخلہ,
		 lookup_context.funcs.collect, lookup_context.collect_ڈیٹا[1]);
  collect_array (c, c->after,
		 lookahead_گنتی, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_ڈیٹا[2]);
  recurse_lookups (c,
		   lookup_گنتی_2, lookup_ریکارڈ);
}

ساکن اندرونی منطقی chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     غیرعلامتی صحیح backtrack_گنتی,
						     glyph_ڈیٹا_t backtrack HB_غیر_مستعمل,
						     غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
						     glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
						     غیرعلامتی صحیح lookahead_گنتی,
						     glyph_ڈیٹا_t lookahead HB_غیر_مستعمل,
						     غیرعلامتی صحیح lookup_گنتی_2 HB_غیر_مستعمل,
						     ثابت Lookup_ریکارڈ lookup_ریکارڈ[] HB_غیر_مستعمل,
						     ثابت ChainContextApplyLookupContext &lookup_context)
{
  واپس (c->zero_context ? !backtrack_گنتی && !lookahead_گنتی : سچ)
      && would_مطابقت_داخلہ (c,
			    داخلہ_گنتی, داخلہ,
			    lookup_context.funcs.مطابقت[1], lookup_context.مطابقت_ڈیٹا_2[1]);
}

ساکن اندرونی منطقی chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       غیرعلامتی صحیح backtrack_گنتی,
					       glyph_ڈیٹا_t backtrack,
					       غیرعلامتی صحیح داخلہ_گنتی, /* Including the first glyph (not matched) */
					       glyph_ڈیٹا_t داخلہ, /* Array of input values--start with second glyph */
					       غیرعلامتی صحیح lookahead_گنتی,
					       glyph_ڈیٹا_t lookahead,
					       غیرعلامتی صحیح lookup_گنتی_2,
					       ثابت Lookup_ریکارڈ lookup_ریکارڈ[],
					       ثابت ChainContextApplyLookupContext &lookup_context)
{
  اگر (unlikely (داخلہ_گنتی > HB_MAX_CONTEXT_لمبائی)) واپس جھوٹ;

  غیرعلامتی آغاز_اشاریہ = c->بفر->برآمد_len;
  غیرعلامتی انتہا_اشاریہ = c->بفر->idx;
  غیرعلامتی مطابقت_انتہا = 0;
  منطقی ret = سچ;
  اگر (!(مطابقت_داخلہ (c,
		     داخلہ_گنتی, داخلہ,
		     lookup_context.funcs.مطابقت[1], lookup_context.مطابقت_ڈیٹا_2[1],
		     &مطابقت_انتہا) && (انتہا_اشاریہ = مطابقت_انتہا)
       && مطابقت_lookahead (c,
			   lookahead_گنتی, lookahead,
			   lookup_context.funcs.مطابقت[2], lookup_context.مطابقت_ڈیٹا_2[2],
			   مطابقت_انتہا, &انتہا_اشاریہ)))
  {
    c->بفر->unsafe_مقصد_concat (c->بفر->idx, انتہا_اشاریہ);
    واپس جھوٹ;
  }

  اگر (!مطابقت_backtrack (c,
			backtrack_گنتی, backtrack,
			lookup_context.funcs.مطابقت[0], lookup_context.مطابقت_ڈیٹا_2[0],
			&آغاز_اشاریہ))
  {
    c->بفر->unsafe_مقصد_concat_from_outbuffer (آغاز_اشاریہ, انتہا_اشاریہ);
    واپس جھوٹ;
  }

  c->بفر->unsafe_مقصد_break_from_outbuffer (آغاز_اشاریہ, انتہا_اشاریہ);
  apply_lookup (c,
		داخلہ_گنتی,
		lookup_گنتی_2, lookup_ریکارڈ,
		مطابقت_انتہا);

  واپس ret;
}

سانچہ <قسم_نام Types>
ساخت Chain_قاعدہ
{
  سانچہ <قسم_نام سیٹ_Types, قسم_نام Chain_قاعدہ_Type>
  دوست ساخت Chain_قاعدہ_سیٹ_کا;

  منطقی intersects (ثابت hb_سیٹ_t *glyphs, Chain_Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    واپس chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     داخلہ.lenP1, داخلہ.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  خالی depend (hb_depend_context_t *c, غیرعلامتی value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 داخلہ.lenP1, داخلہ.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 یہ,
				 value,
				 lookup_context);
  }

  خالی احاطہ (hb_احاطہ_context_t *c, غیرعلامتی value,
		Chain_Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_حد_بندی_exceeded ())) واپس;

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_احاطہ_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  داخلہ.lenP1, داخلہ.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c,
                        Chain_Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_حد_بندی_exceeded ())) واپس;
    اگر (!intersects (c->glyphs, lookup_context)) واپس;

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c,
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

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest,
			      ثابت ClassDef *داخلہ_class_def = خالی_اشاریہ,
			      ثابت ClassDef *lookahead_class_def = خالی_اشاریہ) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);

    غیرعلامتی second;
    ثابت ClassDef *class_def;
    اگر (داخلہ.lenP1 > 1)
    {
      second = داخلہ.arrayZ[0];
      class_def = داخلہ_class_def;
    }
    ورنہ اگر (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    ورنہ
    {
      *digest = hb_سیٹ_digest_t::بھرا ();
      واپس;
    }

    اگر (!class_def)
      digest->جوڑو (second);
    ورنہ اگر (!second)
      *digest = hb_سیٹ_digest_t::بھرا ();
    ورنہ
      class_def->collect_class (digest, second);
  }

  منطقی would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    واپس chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     داخلہ.lenP1, داخلہ.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  منطقی apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (یہ);
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      داخلہ.lenP1, داخلہ.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  خالی serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ثابت
  {
    c->copy (len);
    برائے (ثابت خودکار g : it)
    {
      قسم_نام Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *lookup_map,
		  ثابت hb_map_t *backtrack_map,
		  ثابت hb_map_t *داخلہ_map = خالی_اشاریہ,
		  ثابت hb_map_t *lookahead_map = خالی_اشاریہ) ثابت
  {
    TRACE_SERIALIZE (یہ);

    ثابت hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    اگر (داخلہ_map) mapping = داخلہ_map;
    serialize_array (c, داخلہ.lenP1, + داخلہ.iter ()
				     | hb_map (mapping));

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    اگر (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_گنتی_2 = c->embed (&(lookup.len));
    اگر (!lookup_گنتی_2) return_trace (جھوٹ);

    غیرعلامتی گنتی = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_تفویض (*lookup_گنتی_2, گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_map = خالی_اشاریہ,
	       ثابت hb_map_t *داخلہ_map = خالی_اشاریہ,
	       ثابت hb_map_t *lookahead_map = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);

    اگر (!backtrack_map)
    {
      ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
      اگر (!hb_all (backtrack, glyphset) ||
	  !hb_all (داخلہ, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (جھوٹ);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    ورنہ
    {
      اگر (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (داخلہ, داخلہ_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (جھوٹ);

      serialize (c->serializer, lookup_map, backtrack_map, داخلہ_map, lookahead_map);
    }

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* Hyper-optimized sanitized because this is really hot. */
    اگر (unlikely (!backtrack.len.sanitize (c))) return_trace (جھوٹ);
    hb_barrier ();
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    اگر (unlikely (!داخلہ.lenP1.sanitize (c))) return_trace (جھوٹ);
    hb_barrier ();
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    اگر (unlikely (!lookahead.len.sanitize (c))) return_trace (جھوٹ);
    hb_barrier ();
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محفوظ:
  Array16_کا<قسم_نام Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  Headless_Array16_کا<قسم_نام Types::HBUINT>
		داخلہ_X;			/* Array of input values (start with
					 * second glyph) */
  Array16_کا<قسم_نام Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16_کا<Lookup_ریکارڈ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عوامی:
  DEFINE_سائز_MIN (8);
};

سانچہ <قسم_نام Types, قسم_نام Chain_قاعدہ_Type>
ساخت Chain_قاعدہ_سیٹ_کا
{
  منطقی intersects (ثابت hb_سیٹ_t *glyphs, Chain_Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    واپس
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([&] (ثابت Chain_قاعدہ_Type &_) { واپس _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  خالی collect_پہلا_داخلہ_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    برائے (غیرعلامتی i = 0; i < قاعدہ_2.len; i++)
    {
      ثابت Chain_قاعدہ_Type &r = یہ+قاعدہ_2.arrayZ[i];
      ثابت خودکار &داخلہ = StructAfter<decltype (r.داخلہ_X)> (r.backtrack);
      اگر (داخلہ.lenP1 <= 1)
      {
	digest->سیٹ_بھرا ();
	واپس;
      }
      digest->جوڑو (داخلہ.arrayZ[0]);
    }
  }
  خالی depend (hb_depend_context_t *c, غیرعلامتی value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Chain_قاعدہ_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }
  خالی احاطہ (hb_احاطہ_context_t *c, غیرعلامتی value, Chain_Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_حد_بندی_exceeded ())) واپس;

    واپس
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Chain_قاعدہ_Type &_) { _.احاطہ (c, value, lookup_context); })
    ;
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c,
                        Chain_Context_احاطہ_Lookup_Context &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_حد_بندی_exceeded ())) واپس;

    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Chain_قاعدہ_Type &_) { _.احاطہ_lookups (c, lookup_context); })
    ;
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    واپس
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Chain_قاعدہ_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest,
			      ثابت ClassDef *داخلہ_class_def = خالی_اشاریہ,
			      ثابت ClassDef *lookahead_class_def = خالی_اشاریہ) ثابت
  {
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([=] (ثابت Chain_قاعدہ_Type &_)
		{ _.collect_دوسرا_glyphs (digest, داخلہ_class_def, lookahead_class_def); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    واپس
    + hb_iter (قاعدہ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([&] (ثابت Chain_قاعدہ_Type &_) { واپس _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = خالی_اشاریہ) ثابت
  {
    TRACE_APPLY (یہ);

    غیرعلامتی عدد_قواعد = قاعدہ_2.len;

#اگر_غیر_تعریف_شدہ HB_NO_OT_RULESETS_FAST_راستہ
    اگر (HB_OPTIMIZE_سائز_VAL || عدد_قواعد <= 4)
#اگر_ختم
    {
    slow:
      return_trace (
      + hb_iter (قاعدہ_2)
      | hb_map (hb_جوڑو (یہ))
      | hb_map ([&] (ثابت Chain_قاعدہ_Type &_) { واپس _.apply (c, lookup_context); })
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
    skippy_iter.سیٹ_مطابقت_func (مطابقت_always, خالی_اشاریہ);
    skippy_iter.سیٹ_glyph_ڈیٹا ((HBUINT16 *) خالی_اشاریہ);
    غیرعلامتی unsafe_مقصد = (غیرعلامتی) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *پہلا = خالی_اشاریہ, *second = خالی_اشاریہ;
    منطقی matched = skippy_iter.next (&unsafe_to1);
    اگر (likely (matched))
    {
      اگر (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        جاؤ slow;
      }

      پہلا = &c->بفر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      اگر (digest && lookup_context.get_داخلہ_قدر &&
	  !digest->may_have (lookup_context.get_داخلہ_قدر (*پہلا, lookup_context.مطابقت_ڈیٹا_2[1])))
	{
	  c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_to1);
	  return_trace (جھوٹ);
	}
    }
    ورنہ
    {
      منطقی unsafe_مقصد_concat = جھوٹ;
      منطقی ret =
      + hb_iter (قاعدہ_2)
      | hb_map (hb_جوڑو (یہ))
      | hb_filter ([&] (ثابت Chain_قاعدہ_Type &_)
		   {
		     ثابت خودکار &داخلہ = StructAfter<decltype (_.داخلہ_X)> (_.backtrack);
		     ثابت خودکار &lookahead = StructAfter<decltype (_.lookaheadX)> (داخلہ);
		     اگر (داخلہ.lenP1 <= 1 && lookahead.len == 0) واپس سچ;
		     unsafe_مقصد_concat = سچ;
		     واپس جھوٹ;
		   })
      | hb_map ([&] (ثابت Chain_قاعدہ_Type &_) { واپس _.apply (c, lookup_context); })
      | hb_any
      ;
      اگر (unsafe_مقصد_concat)
	c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    اگر (likely (matched))
    {
      اگر (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        جاؤ slow;
      }

      second = &c->بفر->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    خودکار مطابقت_داخلہ = lookup_context.funcs.مطابقت[1];
    خودکار مطابقت_lookahead = lookup_context.funcs.مطابقت[2];
    خودکار *داخلہ_ڈیٹا = lookup_context.مطابقت_ڈیٹا_2[1];
    خودکار *lookahead_ڈیٹا = lookup_context.مطابقت_ڈیٹا_2[2];
    برائے (غیرعلامتی صحیح i = 0; i < عدد_قواعد; i++)
    {
      ثابت خودکار &r = یہ+قاعدہ_2.arrayZ[i];

      ثابت خودکار &داخلہ = StructAfter<decltype (r.داخلہ_X)> (r.backtrack);
      ثابت خودکار &lookahead = StructAfter<decltype (r.lookaheadX)> (داخلہ);

      غیرعلامتی lenP1 = داخلہ.lenP1;
      اگر (lenP1 > 1 ?
	   (!مطابقت_داخلہ ||
	    مطابقت_داخلہ (*پہلا, داخلہ.arrayZ[0], داخلہ_ڈیٹا))
	  :
	   (!lookahead.len || !مطابقت_lookahead ||
	    مطابقت_lookahead (*پہلا, lookahead.arrayZ[0], lookahead_ڈیٹا)))
      {
	lenP1 = hb_max (lenP1, 1u);
        اگر (!second ||
	    (lenP1 > 2 ?
	     (!مطابقت_داخلہ ||
	      مطابقت_داخلہ (*second, داخلہ.arrayZ[1], داخلہ_ڈیٹا))
	     :
	     (lookahead.len <= 2 - lenP1 || !مطابقت_lookahead ||
	      مطابقت_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_ڈیٹا))))
	{
	  اگر (r.apply (c, lookup_context))
	  {
	    اگر (unsafe_مقصد != (غیرعلامتی) -1)
	      c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_مقصد);
	    return_trace (سچ);
	  }
	}
	ورنہ
	  unsafe_مقصد = unsafe_to2;
      }
      ورنہ
      {
	اگر (unsafe_مقصد == (غیرعلامتی) -1)
	  unsafe_مقصد = unsafe_to1;

	اگر (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  برائے (; i + 1 < عدد_قواعد; i++)
	  {
	    ثابت خودکار &r2 = یہ+قاعدہ_2.arrayZ[i + 1];
	    ثابت خودکار &input2 = StructAfter<decltype (r2.داخلہ_X)> (r2.backtrack);
	    اگر (input2.lenP1 <= 1 || input2.arrayZ[0] != داخلہ.arrayZ[0])
	      توڑو;
	  }
	}
      }
    }
    اگر (likely (unsafe_مقصد != (غیرعلامتی) -1))
      c->بفر->unsafe_مقصد_concat (c->بفر->idx, unsafe_مقصد);

    return_trace (جھوٹ);
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_klass_map = خالی_اشاریہ,
	       ثابت hb_map_t *داخلہ_klass_map = خالی_اشاریہ,
	       ثابت hb_map_t *lookahead_klass_map = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);

    خودکار snap = c->serializer->snapshot ();
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برائے (ثابت خودکار& _ : قاعدہ_2)
    {
      اگر (!_) جاری;
      خودکار o_snap = c->serializer->snapshot ();
      خودکار *o = برآمد->قاعدہ_2.serialize_append (c->serializer);
      اگر (unlikely (!o)) جاری;

      اگر (!o->serialize_subset (c, _, یہ,
				lookup_map,
				backtrack_klass_map,
				داخلہ_klass_map,
				lookahead_klass_map))
      {
	برآمد->قاعدہ_2.نکالو ();
	c->serializer->revert (o_snap);
      }
    }

    منطقی ret = منطقی (برآمد->قاعدہ_2);
    اگر (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (قاعدہ_2.sanitize (c, یہ));
  }

  محفوظ:
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Chain_قاعدہ_Type>>
		قاعدہ_2;			/* Array of ChainRule tables
					 * ordered by preference */
  عوامی:
  DEFINE_سائز_ARRAY (2, قاعدہ_2);
};

سانچہ <قسم_نام Types>
ساخت Chain_قاعدہ_سیٹ : Chain_قاعدہ_سیٹ_کا<Types, Chain_قاعدہ<Types>> {};

static_assert (حجم (Chain_قاعدہ_سیٹ<MediumTypes>) ==
	       HBUINT16::static_سائز + Offset24::static_سائز, "");

سانچہ <قسم_نام Types>
ساخت Chain_Class_قاعدہ_سیٹ : Chain_قاعدہ_سیٹ_کا<Types, Chain_قاعدہ<SmallTypes>> {};

سانچہ <قسم_نام Types>
ساخت ChainContextFormat1_4
{
  استعمال Chain_قاعدہ_سیٹ = OT::Chain_قاعدہ_سیٹ<Types>;

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    ساخت Chain_Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ}
    };

    واپس
    + hb_zip (یہ+coverage, قاعدہ_سیٹ_2)
    | hb_filter (*glyphs, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_map ([&] (ثابت Chain_قاعدہ_سیٹ &_) { واپس _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { واپس سچ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_فعال_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ},
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ},  /* intersects_cache */
      خالی_اشاریہ                        /* intersected_glyphs_cache */
    };

    + hb_zip (یہ+coverage, hb_range ((غیرعلامتی) قاعدہ_سیٹ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      واپس c->والد_فعال_glyphs ().has (_);
    }, hb_پہلا)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, غیرعلامتی> _) { واپس hb_pair_t<غیرعلامتی, ثابت Chain_قاعدہ_سیٹ&> (_.پہلا, یہ+قاعدہ_سیٹ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت Chain_قاعدہ_سیٹ&>& _) { _.second.depend (c, _.پہلا, lookup_context); })
    ;

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (c->پچھلا_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ساخت Chain_Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ}
    };

    + hb_zip (یہ+coverage, hb_range ((غیرعلامتی) قاعدہ_سیٹ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      واپس c->پچھلا_والد_فعال_glyphs ().has (_);
    }, hb_پہلا)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, غیرعلامتی> _) { واپس hb_pair_t<غیرعلامتی, ثابت Chain_قاعدہ_سیٹ&> (_.پہلا, یہ+قاعدہ_سیٹ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت Chain_قاعدہ_سیٹ&>& _) { _.second.احاطہ (c, _.پہلا, lookup_context); })
    ;

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت
  {
    ساخت Chain_Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_glyph, خالی_اشاریہ},
      ContextFormat::SimpleContext,
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ}
    };

    + hb_zip (یہ+coverage, قاعدہ_سیٹ_2)
    | hb_filter (*c->glyphs, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Chain_قاعدہ_سیٹ &_) { _.احاطہ_lookups (c, lookup_context); })
    ;
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (یہ+coverage).collect_coverage (c->داخلہ);

    ساخت ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ}
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Chain_قاعدہ_سیٹ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([digest] (ثابت Chain_قاعدہ_سیٹ &_) { _.collect_دوسرا_glyphs (digest); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Chain_قاعدہ_سیٹ &قاعدہ_سیٹ_3 = یہ+قاعدہ_سیٹ_2[(یہ+coverage).get_coverage (c->glyphs[0])];
    ساخت ChainContextApplyLookupContext lookup_context = {
      {{مطابقت_glyph, مطابقت_glyph, مطابقت_glyph}},
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ}
    };
    واپس قاعدہ_سیٹ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->بفر->cur().codepoint);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    ثابت Chain_قاعدہ_سیٹ &قاعدہ_سیٹ_3 = یہ+قاعدہ_سیٹ_2[اشاریہ_2];
    ساخت ChainContextApplyLookupContext lookup_context = {
      {{مطابقت_glyph, مطابقت_glyph, مطابقت_glyph}},
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ}
    };
    return_trace (قاعدہ_سیٹ_3.apply (c, lookup_context));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_لیبل_2 == HB_OT_لیبل_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + hb_zip (یہ+coverage, قاعدہ_سیٹ_2)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (subset_انحراف_array (c, برآمد->قاعدہ_سیٹ_2, یہ, lookup_map), hb_دوسرا)
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    برآمد->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) && قاعدہ_سیٹ_2.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;			/* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::سانچہ انحراف_مقصد<Chain_قاعدہ_سیٹ>>
		قاعدہ_سیٹ_2;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز, قاعدہ_سیٹ_2);
};

سانچہ <قسم_نام Types>
ساخت ChainContextFormat2_5
{
  استعمال Chain_قاعدہ_سیٹ = OT::Chain_Class_قاعدہ_سیٹ<Types>;

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    اگر (!(یہ+coverage).intersects (glyphs))
      واپس جھوٹ;

    ثابت ClassDef &backtrack_class_def = یہ+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = یہ+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = یہ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ساخت Chain_Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_class, خالی_اشاریہ},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_سیٹ_t retained_coverage_glyphs;
    (یہ+coverage).intersect_سیٹ (*glyphs, retained_coverage_glyphs);

    hb_سیٹ_t coverage_glyph_classes;
    داخلہ_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    واپس
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت Chain_قاعدہ_سیٹ &> p)
	      { واپس داخلہ_class_def.intersects_class (glyphs, p.پہلا) &&
		       coverage_glyph_classes.has (p.پہلا) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { واپس سچ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    اگر (!(یہ+coverage).intersects (c->glyphs))
      واپس;

    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_فعال_glyphs);

    ثابت ClassDef &backtrack_class_def = یہ+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = یہ+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = یہ+lookaheadClassDef;

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
    | hb_filter ([&] (غیرعلامتی _)
    { واپس داخلہ_class_def.intersects_class (&c->والد_فعال_glyphs (), _); },
		 hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت قسم_نام Types::سانچہ انحراف_مقصد<Chain_قاعدہ_سیٹ>&> _)
                {
                  ثابت Chain_قاعدہ_سیٹ& chainrule_سیٹ = یہ+_.second;
                  chainrule_سیٹ.depend (c, _.پہلا, lookup_context);
                })
    ;

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    اگر (!(یہ+coverage).intersects (c->glyphs))
      واپس;

    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs)) واپس;
    get_coverage ().intersect_سیٹ (c->پچھلا_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت ClassDef &backtrack_class_def = یہ+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = یہ+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = یہ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ساخت Chain_Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعدہ_سیٹ_2)
    | hb_filter ([&] (غیرعلامتی _)
    { واپس داخلہ_class_def.intersects_class (&c->والد_فعال_glyphs (), _); },
		 hb_پہلا)
    | hb_apply ([&] (ثابت hb_pair_t<غیرعلامتی, ثابت قسم_نام Types::سانچہ انحراف_مقصد<Chain_قاعدہ_سیٹ>&> _)
                {
                  ثابت Chain_قاعدہ_سیٹ& chainrule_سیٹ = یہ+_.second;
                  chainrule_سیٹ.احاطہ (c, _.پہلا, lookup_context);
                })
    ;

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت
  {
    اگر (!(یہ+coverage).intersects (c->glyphs))
      واپس;

    ثابت ClassDef &backtrack_class_def = یہ+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = یہ+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = یہ+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ساخت Chain_Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_class, خالی_اشاریہ},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_enumerate
    | hb_filter([&] (غیرعلامتی klass)
    { واپس داخلہ_class_def.intersects_class (c->glyphs, klass); }, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_apply ([&] (ثابت Chain_قاعدہ_سیٹ &_)
    { _.احاطہ_lookups (c, lookup_context); })
    ;
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (یہ+coverage).collect_coverage (c->داخلہ);

    ثابت ClassDef &backtrack_class_def = یہ+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = یہ+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = یہ+lookaheadClassDef;

    ساخت ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def}
    };

    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([&] (ثابت Chain_قاعدہ_سیٹ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    ثابت ClassDef &داخلہ_class_def = یہ+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = یہ+lookaheadClassDef;
    + hb_iter (قاعدہ_سیٹ_2)
    | hb_map (hb_جوڑو (یہ))
    | hb_apply ([digest, &داخلہ_class_def, &lookahead_class_def] (ثابت Chain_قاعدہ_سیٹ &_)
		{ _.collect_دوسرا_glyphs (digest, &داخلہ_class_def, &lookahead_class_def); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &backtrack_class_def = یہ+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = یہ+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = یہ+lookaheadClassDef;

    غیرعلامتی صحیح اشاریہ_2 = داخلہ_class_def.get_class (c->glyphs[0]);
    ثابت Chain_قاعدہ_سیٹ &قاعدہ_سیٹ_3 = یہ+قاعدہ_سیٹ_2[اشاریہ_2];
    ساخت ChainContextApplyLookupContext lookup_context = {
      {{مطابقت_class, مطابقت_class, مطابقت_class}},
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def}
    };
    واپس قاعدہ_سیٹ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  غیرعلامتی cache_لاگت () ثابت
  {
    واپس (یہ+داخلہ_Class_Def).لاگت () + (یہ+lookaheadClassDef).لاگت ();
  }
  ساکن منطقی cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    واپس context_cache_func (c, op);
  }

  ساخت external_cache_t
  {
    hb_ot_layout_ثنائی_cache_t coverage;
    hb_ot_layout_mapping_cache_t داخلہ_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t قاعدہ_sets[HB_VAR_ARRAY];
  };
  خالی *external_cache_create () ثابت
  {
    غیرعلامتی گنتی = قاعدہ_سیٹ_2.len;
    غیرعلامتی سائز = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    گنتی * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (سائز);
    اگر (likely (cache))
    {
      cache->coverage.clear ();
      cache->داخلہ_class.clear ();
      cache->lookahead_class.clear ();
      برائے (غیرعلامتی i = 0; i < گنتی; i++)
	(یہ+قاعدہ_سیٹ_2[i]).collect_پہلا_داخلہ_classes (&cache->قاعدہ_sets[i]);
    }
    واپس cache;
  }
  منطقی apply_cached (hb_ot_apply_context_t *c, خالی *external_cache) ثابت { واپس _apply (c, سچ, external_cache); }
  منطقی apply (hb_ot_apply_context_t *c, خالی *external_cache) ثابت { واپس _apply (c, جھوٹ, external_cache); }
  منطقی _apply (hb_ot_apply_context_t *c, منطقی cached, خالی *external_cache) ثابت
  {
    TRACE_APPLY (یہ);
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage_ثنائی (c->بفر->cur().codepoint, cache ? &cache->coverage : خالی_اشاریہ);
#ورنہ
    غیرعلامتی صحیح اشاریہ_2 = (یہ+coverage).get_coverage (c->بفر->cur().codepoint);
#اگر_ختم
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    ثابت ClassDef &backtrack_class_def = یہ+backtrackClassDef;
    ثابت ClassDef &داخلہ_class_def = یہ+داخلہ_Class_Def;
    ثابت ClassDef &lookahead_class_def = یہ+lookaheadClassDef;

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    مطابقت_class_cache_ڈیٹا_t داخلہ_cache_ڈیٹا = {
      &داخلہ_class_def,
      cache ? &cache->داخلہ_class : خالی_اشاریہ
    };
    مطابقت_class_cache_ڈیٹا_t lookahead_cache_ڈیٹا = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : خالی_اشاریہ
    };
    ثابت خالی *داخلہ_مطابقت_ڈیٹا = cached
      ? static_cast<ثابت خالی *> (&داخلہ_class_def)
      : static_cast<ثابت خالی *> (&داخلہ_cache_ڈیٹا);
    ثابت خالی *lookahead_مطابقت_ڈیٹا = cached
      ? static_cast<ثابت خالی *> (&lookahead_class_def)
      : static_cast<ثابت خالی *> (&lookahead_cache_ڈیٹا);
    منطقی backtrack_استعمال_کرتا_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ثابت خالی *backtrack_مطابقت_ڈیٹا = backtrack_استعمال_کرتا_lookahead_cache
      ? static_cast<ثابت خالی *> (&lookahead_cache_ڈیٹا)
      : static_cast<ثابت خالی *> (&backtrack_class_def);
#اگر_ختم

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    ساخت ChainContextApplyLookupContext lookup_context = {
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? مطابقت_class_cached1 :
        backtrack_استعمال_کرتا_lookahead_cache ? مطابقت_class_cache : مطابقت_class,
        cached ? مطابقت_class_cached2 : مطابقت_class_cache,
        cached ? مطابقت_class_cached1 : مطابقت_class_cache}},
      {backtrack_مطابقت_ڈیٹا,
       داخلہ_مطابقت_ڈیٹا,
       lookahead_مطابقت_ڈیٹا},
      cached ? get_class_cached2_قدر : get_class_cache_قدر
#ورنہ
      {{cached && &backtrack_class_def == &lookahead_class_def ? مطابقت_class_cached1 : مطابقت_class,
        cached ? مطابقت_class_cached2 : مطابقت_class,
        cached ? مطابقت_class_cached1 : مطابقت_class}},
      {&backtrack_class_def,
       &داخلہ_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_قدر : get_class_قدر
#اگر_ختم
    };

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    اشاریہ_2 = cached
         ? get_class_cached2 (داخلہ_class_def, c->بفر->cur())
         : داخلہ_class_def.get_class (c->بفر->cur().codepoint, داخلہ_cache_ڈیٹا.cache);
#ورنہ
    اشاریہ_2 = cached
         ? get_class_cached2 (داخلہ_class_def, c->بفر->cur())
          : داخلہ_class_def.get_class (c->بفر->cur().codepoint);
#اگر_ختم
    ثابت Chain_قاعدہ_سیٹ &قاعدہ_سیٹ_3 = یہ+قاعدہ_سیٹ_2[اشاریہ_2];
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && اشاریہ_2 < قاعدہ_سیٹ_2.len ? &cache->قاعدہ_sets[اشاریہ_2] : خالی_اشاریہ;
#ورنہ
    ثابت hb_ot_layout_ruleset_digest_t *digest = خالی_اشاریہ;
#اگر_ختم
    return_trace (قاعدہ_سیٹ_3.apply (c, lookup_context, digest));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;
    برآمد->coverage.serialize_subset (c, coverage, یہ);

    hb_map_t backtrack_klass_map;
    hb_map_t داخلہ_klass_map;
    hb_map_t lookahead_klass_map;

    برآمد->backtrackClassDef.serialize_subset (c, backtrackClassDef, یہ, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    برآمد->داخلہ_Class_Def.serialize_subset (c, داخلہ_Class_Def, یہ, &داخلہ_klass_map);
    برآمد->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, یہ, &lookahead_klass_map);

    اگر (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   داخلہ_klass_map,
						   lookahead_klass_map)))
      return_trace (جھوٹ);

    ثابت hb_سیٹ_t* glyphset = c->plan->glyphset_gsub ();
    hb_سیٹ_t retained_coverage_glyphs;
    (یہ+coverage).intersect_سیٹ (*glyphset, retained_coverage_glyphs);

    hb_سیٹ_t coverage_glyph_classes;
    (یہ+داخلہ_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    صحیح non_zero_اشاریہ = -1, اشاریہ_2 = 0;
    منطقی ret = سچ;
    ثابت hb_map_t *lookup_map = c->جدول_لیبل_2 == HB_OT_لیبل_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    خودکار آخری_non_zero = c->serializer->snapshot ();
    برائے (ثابت خودکار& _ : + hb_enumerate (قاعدہ_سیٹ_2)
			 | hb_filter (داخلہ_klass_map, hb_پہلا))
    {
      خودکار *o = برآمد->قاعدہ_سیٹ_2.serialize_append (c->serializer);
      اگر (unlikely (!o))
      {
	ret = جھوٹ;
	توڑو;
      }
      اگر (coverage_glyph_classes.has (_.پہلا) &&
          o->serialize_subset (c, _.second, یہ,
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

    اگر (!ret || non_zero_اشاریہ == -1) return_trace (جھوٹ);

    // prune empty trailing ruleSets
    اگر (اشاریہ_2 > non_zero_اشاریہ) {
      c->serializer->revert (آخری_non_zero);
      برآمد->قاعدہ_سیٹ_2.len = non_zero_اشاریہ + 1;
    }

    return_trace (منطقی (برآمد->قاعدہ_سیٹ_2));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (coverage.sanitize (c, یہ) &&
		  backtrackClassDef.sanitize (c, یہ) &&
		  داخلہ_Class_Def.sanitize (c, یہ) &&
		  lookaheadClassDef.sanitize (c, یہ) &&
		  قاعدہ_سیٹ_2.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;			/* Format identifier--format = 2 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  قسم_نام Types::سانچہ LOffset_مقصد<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  قسم_نام Types::سانچہ LOffset_مقصد<ClassDef>
		داخلہ_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  قسم_نام Types::سانچہ LOffset_مقصد<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Chain_قاعدہ_سیٹ>>
		قاعدہ_سیٹ_2;		/* Array of ChainRuleSet tables
					 * ordered by class */
  عوامی:
  DEFINE_سائز_ARRAY (4 + 4 * Types::LOffset::static_سائز, قاعدہ_سیٹ_2);
};

سانچہ <قسم_نام Types>
ساخت ChainContextFormat3
{
  استعمال قاعدہ_سیٹ = OT::قاعدہ_سیٹ<SmallTypes>;

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    اگر (!(یہ+داخلہ[0]).intersects (glyphs))
      واپس جھوٹ;

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ساخت Chain_Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_coverage, خالی_اشاریہ},
      ContextFormat::CoverageBasedContext,
      {یہ, یہ, یہ}
    };
    واپس chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     داخلہ.len, داخلہ.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  منطقی may_have_non_1to1 () ثابت
  { واپس سچ; }

  خالی depend (hb_depend_context_t *c) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    اگر (!(یہ+داخلہ[0]).intersects (c->glyphs))
      واپس;

    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs))
      واپس;
    get_coverage ().intersect_سیٹ (*c->glyphs, *cur_فعال_glyphs);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {یہ, یہ, یہ},
      {خالی_اشاریہ, خالی_اشاریہ, خالی_اشاریہ},  /* intersects_cache */
      خالی_اشاریہ                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ثابت HBUINT16 *) backtrack.arrayZ,
				 داخلہ.len, (ثابت HBUINT16 *) داخلہ.arrayZ + 1,
				 lookahead.len, (ثابت HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 یہ,
				 داخلہ[0], lookup_context);

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    اگر (!(یہ+داخلہ[0]).intersects (c->glyphs))
      واپس;

    hb_سیٹ_t* cur_فعال_glyphs = c->ڈالو_cur_فعال_glyphs ();
    اگر (unlikely (!cur_فعال_glyphs))
      واپس;
    get_coverage ().intersect_سیٹ (c->پچھلا_والد_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ساخت Chain_Context_احاطہ_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {یہ, یہ, یہ}
    };
    chain_context_احاطہ_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  داخلہ.len, داخلہ.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->نکالو_cur_مکمل_glyphs ();
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت
  {
    اگر (!intersects (c->glyphs))
      واپس;

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    (یہ+داخلہ[0]).collect_coverage (c->داخلہ);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);

    ساخت ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {یہ, یہ, یہ}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 داخلہ.len, داخلہ.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    اگر (داخلہ.len > 1)
      (یہ+داخلہ[1]).collect_coverage (digest);
    ورنہ اگر (lookahead.len)
      (یہ+lookahead[0]).collect_coverage (digest);
    ورنہ
      *digest = hb_سیٹ_digest_t::بھرا ();
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ساخت ChainContextApplyLookupContext lookup_context = {
      {{مطابقت_coverage, مطابقت_coverage, مطابقت_coverage}},
      {یہ, یہ, یہ}
    };
    واپس chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     داخلہ.len, داخلہ.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت
  {
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    واپس یہ+داخلہ[0];
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);

    غیرعلامتی صحیح اشاریہ_2 = (یہ+داخلہ[0]).get_coverage (c->بفر->cur().codepoint);
    اگر (اشاریہ_2 == NOT_COVERED) return_trace (جھوٹ);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ساخت ChainContextApplyLookupContext lookup_context = {
      {{مطابقت_coverage, مطابقت_coverage, مطابقت_coverage}},
      {یہ, یہ, یہ}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      داخلہ.len, داخلہ.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ثابت خالی* بنیاد) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed<Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>> ();
    اگر (unlikely (!برآمد->serialize (c->serializer, 0u))) return_trace (جھوٹ);

    برائے (خودکار& انحراف : it) {
      خودکار *o = برآمد->serialize_append (c->serializer);
      اگر (unlikely (!o) || !o->serialize_subset (c, انحراف, بنیاد))
        return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);

    اگر (unlikely (!c->serializer->embed (یہ->format))) return_trace (جھوٹ);

    اگر (!serialize_coverage_offsets (c, backtrack.iter (), یہ))
      return_trace (جھوٹ);

    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    اگر (!serialize_coverage_offsets (c, داخلہ.iter (), یہ))
      return_trace (جھوٹ);

    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    اگر (!serialize_coverage_offsets (c, lookahead.iter (), یہ))
      return_trace (جھوٹ);

    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ثابت hb_map_t *lookup_map = c->جدول_لیبل_2 == HB_OT_لیبل_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_گنتی_2 = c->serializer->copy<HBUINT16> (lookup.len);
    اگر (!lookup_گنتی_2) return_trace (جھوٹ);

    غیرعلامتی گنتی = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_تفویض (*lookup_گنتی_2, گنتی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!backtrack.sanitize (c, یہ))) return_trace (جھوٹ);
    hb_barrier ();
    ثابت خودکار &داخلہ = StructAfter<decltype (داخلہ_X)> (backtrack);
    اگر (unlikely (!داخلہ.sanitize (c, یہ))) return_trace (جھوٹ);
    hb_barrier ();
    اگر (unlikely (!داخلہ.len)) return_trace (جھوٹ); /* To be consistent with Context. */
    ثابت خودکار &lookahead = StructAfter<decltype (lookaheadX)> (داخلہ);
    اگر (unlikely (!lookahead.sanitize (c, یہ))) return_trace (جھوٹ);
    hb_barrier ();
    ثابت خودکار &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محفوظ:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>
		داخلہ_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16_کا<قسم_نام Types::سانچہ انحراف_مقصد<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16_کا<Lookup_ریکارڈ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عوامی:
  DEFINE_سائز_MIN (10);
};

ساخت ChainContext
{
  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالت 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#اگر_ختم
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#اگر_ختم
  } u;
};


سانچہ <قسم_نام T>
ساخت ExtensionFormat1
{
  غیرعلامتی صحیح get_type () ثابت { واپس extensionLookupType; }

  سانچہ <قسم_نام X>
  ثابت X& get_subtable () ثابت
  { واپس یہ + reinterpret_cast<ثابت Offset32_مقصد<قسم_نام T::Sub_جدول> &> (extension_انحراف); }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, یہ))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, format);
    return_trace (get_subtable<قسم_نام T::Sub_جدول> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  extensionLookupType != T::Sub_جدول::Extension);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);

    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->format = format;
    برآمد->extensionLookupType = extensionLookupType;

    ثابت خودکار& src_انحراف =
        reinterpret_cast<ثابت Offset32_مقصد<قسم_نام T::Sub_جدول> &> (extension_انحراف);
    خودکار& dest_انحراف =
        reinterpret_cast<Offset32_مقصد<قسم_نام T::Sub_جدول> &> (برآمد->extension_انحراف);

    return_trace (dest_انحراف.serialize_subset (c, src_انحراف, یہ, get_type ()));
  }

  محفوظ:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extension_انحراف;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  عوامی:
  DEFINE_سائز_STATIC (8);
};

سانچہ <قسم_نام T>
ساخت Extension
{
  غیرعلامتی صحیح get_type () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.get_type ();
    طےشدہ:واپس 0;
    }
  }
  سانچہ <قسم_نام X>
  ثابت X& get_subtable () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.سانچہ get_subtable<قسم_نام T::Sub_جدول> ();
    طےشدہ:واپس Null (قسم_نام T::Sub_جدول);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  سانچہ <قسم_نام ...Ts>
  قسم_نام hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.subset (c);
    طےشدہ: واپس c->default_return_قدر ();
    }
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

ساخت hb_ot_layout_lookup_accelerator_t
{
  سانچہ <قسم_نام TLookup>
  ساکن hb_ot_layout_lookup_accelerator_t *create (ثابت TLookup &lookup)
  {
    غیرعلامتی گنتی = lookup.get_subtable_گنتی ();

    غیرعلامتی سائز = حجم (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    گنتی * حجم (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خودکار *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, سائز);
    اگر (unlikely (!thiz))
      واپس خالی_اشاریہ;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    جب (گنتی && !thiz->subtables[گنتی - 1].apply_func)
      گنتی--;

    thiz->digest.init ();
    برائے (خودکار& subtable : hb_iter (thiz->subtables, گنتی))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_دوسرا = c_accelerate_subtables.digest_دوسرا;

    thiz->گنتی = گنتی;

#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      اگر (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#اگر_ختم

    واپس thiz;
  }

  خالی fini ()
  {
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      hb_free (subtables[i].external_cache);
#اگر_ختم
  }

  منطقی may_have (hb_codepoint_t g) ثابت
  { واپس digest.may_have (g); }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  منطقی apply (hb_ot_apply_context_t *c, منطقی use_cache) ثابت
  {
    اگر (گنتی == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
      اگر (use_cache)
	واپس subtables[0].apply_cached_no_digest (c);
#اگر_ختم
      واپس subtables[0].apply_no_digest (c);
    }
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    اگر (use_cache)
    {
      واپس
      + hb_iter (hb_iter (subtables, گنتی))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { واپس _.apply_cached (c); })
      | hb_any
      ;
    }
    ورنہ
#اگر_ختم
    {
      واپس
      + hb_iter (hb_iter (subtables, گنتی))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { واپس _.apply (c); })
      | hb_any
      ;
    }
    واپس جھوٹ;
  }

  منطقی cache_enter (hb_ot_apply_context_t *c) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    واپس subtable_cache_user_idx != (غیرعلامتی) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#ورنہ
    واپس جھوٹ;
#اگر_ختم
  }
  خالی cache_leave (hb_ot_apply_context_t *c) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#اگر_ختم
  }


  hb_سیٹ_digest_t digest;
  hb_سیٹ_digest_t digest_دوسرا;
  نجی:
  غیرعلامتی گنتی = 0; /* Number of subtables in the array. */
#اگر_غیر_تعریف_شدہ HB_NO_OT_LAYOUT_LOOKUP_CACHE
  غیرعلامتی subtable_cache_user_idx = (غیرعلامتی) -1;
#اگر_ختم
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ساخت GSUBGPOS
{
  استعمال LookupListT = LookupList<SmallTypes>;
  استعمال LookupList2T = LookupList<MediumTypes>;
  سانچہ <قسم_نام TLookup>
  استعمال TLookupList = List16_کا_انحراف_مقصد<TLookup, قسم_نام SmallTypes::HBLUINT>;
  سانچہ <قسم_نام TLookup>
  استعمال TLookupList2 = List16_کا_انحراف_مقصد<TLookup, قسم_نام MediumTypes::HBLUINT>;
  سانچہ <قسم_نام TLookup>
  استعمال TLookup_انحراف_List = Lookup_انحراف_List<TLookup, قسم_نام SmallTypes::HBLUINT>;
  سانچہ <قسم_نام TLookup>
  استعمال TLookup_انحراف_List2 = Lookup_انحراف_List<TLookup, قسم_نام MediumTypes::HBLUINT>;

  size_t get_سائز () ثابت
  {
    اگر (نسخہ.major != 1) واپس نسخہ.static_سائز;

    واپس min_سائز +
	   (نسخہ.مقصد_int () >= 0x00010001u ? featureVars.static_سائز : 0) +
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
	   (نسخہ.مقصد_int () >= 0x00010002u ? 3 * Offset32::static_سائز : 0)
#ورنہ
	   0
#اگر_ختم
	   ;
  }

  ثابت ScriptList &get_script_list () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010002u && scriptList2) واپس یہ + scriptList2;
#اگر_ختم
    واپس یہ + scriptList;
  }
  ثابت FeatureList &get_feature_list () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010002u && featureList2) واپس یہ + featureList2;
#اگر_ختم
    واپس یہ + featureList;
  }
  سانچہ <قسم_نام TLookup>
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!نسخہ.sanitize (c) ||
		  نسخہ.major != 1 ||
		  !c->check_range (یہ, get_سائز ())))
      return_trace (جھوٹ);
    hb_barrier ();

    منطقی script_list_ok = scriptList.sanitize (c, یہ);
    منطقی feature_list_ok = featureList.sanitize (c, یہ);
    منطقی lookup_list_ok = typed_lookup_list_انحراف<TLookup> ().sanitize (c, یہ);

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010002u)
    {
      اگر (scriptList2) script_list_ok = scriptList2.sanitize (c, یہ);
      اگر (featureList2) feature_list_ok = featureList2.sanitize (c, یہ);
      اگر (lookupList2) lookup_list_ok = typed_lookup_list2_انحراف<TLookup> ().sanitize (c, یہ);
    }
#اگر_ختم

    اگر (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (جھوٹ);

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (unlikely (!(نسخہ.مقصد_int () < 0x00010001u ||
		    featureVars.sanitize (c, یہ, &get_feature_list ()))))
      return_trace (جھوٹ);
#اگر_ختم

    return_trace (سچ);
  }

  سانچہ <قسم_نام TLookup>
  منطقی subset (hb_subset_layout_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (نسخہ.major != 1) return_trace (جھوٹ);

    خودکار *برآمد = c->subset_context->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->subset_context->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->نسخہ = نسخہ;

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010002u)
    {
      اگر (unlikely (!c->subset_context->serializer->extend_سائز (برآمد, get_سائز ())))
        return_trace (جھوٹ);

      برآمد->scriptList = 0;
      برآمد->featureList = 0;
      برآمد->lookupList = 0;

      اگر (lookupList2)
	serialize_subset_انحراف (c->subset_context, برآمد->lookupList2,
				 subset_lookup_list2_انحراف<TLookup> (),
				 یہ, c);
      ورنہ
	serialize_subset_انحراف (c->subset_context, برآمد->lookupList2,
				 subset_lookup_list_انحراف<TLookup> (),
				 یہ, c);

      اگر (featureList2)
	serialize_subset_انحراف (c->subset_context, برآمد->featureList2,
				 subset_feature_list2_انحراف (),
				 یہ, c);
      ورنہ
	serialize_subset_انحراف (c->subset_context, برآمد->featureList2,
				 subset_feature_list_انحراف (),
				 یہ, c);

      اگر (scriptList2)
	serialize_subset_انحراف (c->subset_context, برآمد->scriptList2, scriptList2, یہ, c);
      ورنہ
	serialize_subset_انحراف (c->subset_context, برآمد->scriptList2, scriptList, یہ, c);
    }
    ورنہ
#اگر_ختم
    {
      برآمد->subset_lookup_list_انحراف<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_انحراف<TLookup> (),
			     یہ,
			     c);

      برآمد->subset_feature_list_انحراف ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_انحراف (),
			     یہ,
			     c);

      برآمد->scriptList.serialize_subset (c->subset_context,
					scriptList,
					یہ,
					c);
    }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (نسخہ.مقصد_int () >= 0x00010001u)
    {
      خودکار snapshot = c->subset_context->serializer->snapshot ();
      اگر (نسخہ.مقصد_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&برآمد->featureVars))
        return_trace (جھوٹ);

      // if all axes are pinned all feature vars are dropped.
      منطقی ret = !c->subset_context->plan->all_axes_pinned
                 && برآمد->featureVars.serialize_subset (c->subset_context, featureVars, یہ, c);
      اگر (!ret)
      {
        برآمد->featureVars = 0;
	اگر (نسخہ.مقصد_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  برآمد->نسخہ.major = 1;
	  برآمد->نسخہ.minor = 0;
	}
      }
    }
#اگر_ختم

    return_trace (سچ);
  }

  غیرعلامتی صحیح get_lookup_گنتی () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010002u && lookupList2)
      واپس (یہ + lookupList2).len;
#اگر_ختم
    واپس (یہ + lookupList).len;
  }
  ثابت Lookup& get_lookup (غیرعلامتی صحیح i) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010002u && lookupList2)
      واپس (یہ + lookupList2)[i];
#اگر_ختم
    واپس (یہ + lookupList)[i];
  }
  ثابت FeatureVariations &get_feature_variations () ثابت
  {
    واپس (نسخہ.مقصد_int () >= 0x00010001u && hb_barrier ()) ? یہ+featureVars : Null (FeatureVariations);
  }

  منطقی has_ڈیٹا () ثابت { واپس نسخہ.مقصد_int (); }
  غیرعلامتی صحیح get_script_گنتی () ثابت
  { واپس get_script_list ().len; }
  ثابت لیبل& get_script_لیبل (غیرعلامتی صحیح i) ثابت
  { واپس get_script_list ().get_لیبل (i); }
  غیرعلامتی صحیح get_script_لیبل_فہرست (غیرعلامتی صحیح آغاز_انحراف,
				غیرعلامتی صحیح *script_گنتی /* IN/OUT */,
				hb_لیبل_t     *script_لیبل_فہرست /*برآمد*/) ثابت
  { واپس get_script_list ().get_لیبل_فہرست (آغاز_انحراف, script_گنتی, script_لیبل_فہرست); }
  ثابت Script& get_script (غیرعلامتی صحیح i) ثابت
  { واپس get_script_list ()[i]; }
  منطقی ڈھونڈو_script_اشاریہ (hb_لیبل_t لیبل_2, غیرعلامتی صحیح *اشاریہ_2) ثابت
  { واپس get_script_list ().ڈھونڈو_اشاریہ (لیبل_2, اشاریہ_2); }

  غیرعلامتی صحیح get_feature_گنتی () ثابت
  { واپس get_feature_list ().len; }
  hb_لیبل_t get_feature_لیبل (غیرعلامتی صحیح i) ثابت
  { واپس i == اشاریہ::NOT_ملا_اشاریہ ? HB_لیبل_NONE : get_feature_list ().get_لیبل (i); }
  غیرعلامتی صحیح get_feature_لیبل_فہرست (غیرعلامتی صحیح آغاز_انحراف,
				 غیرعلامتی صحیح *feature_گنتی_2 /* IN/OUT */,
				 hb_لیبل_t     *feature_لیبل_فہرست /*برآمد*/) ثابت
  { واپس get_feature_list ().get_لیبل_فہرست (آغاز_انحراف, feature_گنتی_2, feature_لیبل_فہرست); }
  ثابت Feature& get_feature (غیرعلامتی صحیح i) ثابت
  { واپس get_feature_list ()[i]; }
  منطقی ڈھونڈو_feature_اشاریہ (hb_لیبل_t لیبل_2, غیرعلامتی صحیح *اشاریہ_2) ثابت
  { واپس get_feature_list ().ڈھونڈو_اشاریہ (لیبل_2, اشاریہ_2); }

  منطقی ڈھونڈو_variations_اشاریہ (ثابت صحیح *coords, غیرعلامتی صحیح عدد_coords,
			      غیرعلامتی صحیح *اشاریہ_2,
			      شے_Var_Store_Instancer *instancer) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    *اشاریہ_2 = FeatureVariations::NOT_ملا_اشاریہ;
    واپس جھوٹ;
#اگر_ختم
    واپس get_feature_variations ().ڈھونڈو_اشاریہ (coords, عدد_coords, اشاریہ_2, instancer);
  }
  منطقی get_feature_variations_state (ثابت صحیح *coords,
				     غیرعلامتی صحیح عدد_coords,
				     غیرعلامتی صحیح *اشاریہ_2,
				     شے_Var_Store_Instancer *instancer,
				     hb_سمتیہ_t<غیرعلامتی> *lookup_conditions) ثابت
  {
    lookup_conditions->reset ();
#اگر_تعریف_شدہ HB_NO_VAR
    *اشاریہ_2 = FeatureVariations::NOT_ملا_اشاریہ;
    واپس سچ;
#ورنہ
    ڈھونڈو_variations_اشاریہ (coords, عدد_coords, اشاریہ_2, instancer);
    واپس get_feature_variations ().collect_lookup_conditions (
	coords,
	عدد_coords,
	instancer,
	lookup_conditions);
#اگر_ختم
  }
  منطقی get_lookup_variations (غیرعلامتی feature_اشاریہ,
                              غیرعلامتی variations_اشاریہ,
                              hb_sorted_array_t<ثابت غیرعلامتی> conditions,
                              hb_سیٹ_t *lookup_indexes) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس جھوٹ;
#ورنہ
    واپس get_feature_variations ().get_lookup_variations (
	feature_اشاریہ, get_feature_variation (feature_اشاریہ, variations_اشاریہ),
	conditions, lookup_indexes);
#اگر_ختم
  }
  ثابت Feature& get_feature_variation (غیرعلامتی صحیح feature_اشاریہ,
					غیرعلامتی صحیح variations_اشاریہ) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (FeatureVariations::NOT_ملا_اشاریہ != variations_اشاریہ &&
	نسخہ.مقصد_int () >= 0x00010001u)
    {
      ثابت Feature *feature = get_feature_variations ().ڈھونڈو_substitute (variations_اشاریہ,
									  feature_اشاریہ);
      اگر (feature)
	واپس *feature;
    }
#اگر_ختم
    واپس get_feature (feature_اشاریہ);
  }

  خالی feature_variation_collect_lookups (ثابت hb_سیٹ_t *feature_indexes,
					  ثابت hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
					  hb_سیٹ_t       *lookup_indexes /*برآمد*/) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_ریکارڈ_cond_idx_map, lookup_indexes);
#اگر_ختم
  }

  خالی collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#اگر_ختم
  }

#اگر_غیر_تعریف_شدہ HB_NO_VAR
  خالی collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#اگر_ختم

  سانچہ <قسم_نام TLookup>
  خالی احاطہ_lookups (hb_face_t      *face,
			ثابت hb_سیٹ_t *glyphs,
			hb_سیٹ_t       *lookup_indexes /* IN/OUT */) ثابت
  {
    hb_سیٹ_t دیکھا_lookups, inactive_lookups;
    hb_احاطہ_lookups_context_t c (face, glyphs, &دیکھا_lookups, &inactive_lookups);

    c.سیٹ_recurse_func (TLookup::سانچہ dispatch_recurse_func<hb_احاطہ_lookups_context_t>);

    برائے (غیرعلامتی lookup_اشاریہ : *lookup_indexes)
      reinterpret_cast<ثابت TLookup &> (get_lookup (lookup_اشاریہ)).احاطہ_lookups (&c, lookup_اشاریہ);

    hb_سیٹ_union (lookup_indexes, &دیکھا_lookups);
    hb_سیٹ_تفریق (lookup_indexes, &inactive_lookups);
  }

  خالی prune_langsys (ثابت hb_map_t *duplicate_feature_map,
                      ثابت hb_سیٹ_t *layout_scripts,
                      hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> *script_langsys_map,
                      hb_سیٹ_t       *new_feature_indexes /*برآمد*/) ثابت
  {
    hb_prune_langsys_context_t c (یہ, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    غیرعلامتی گنتی = get_script_گنتی ();
    برائے (غیرعلامتی script_اشاریہ = 0; script_اشاریہ < گنتی; script_اشاریہ++)
    {
      ثابت لیبل& لیبل_2 = get_script_لیبل (script_اشاریہ);
      اگر (!layout_scripts->has (لیبل_2)) جاری;
      ثابت Script& s = get_script (script_اشاریہ);
      s.prune_langsys (&c, script_اشاریہ);
    }
  }

  خالی prune_features (ثابت hb_map_t *lookup_indices, /*اندر*/
		       ثابت hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map, /*اندر*/
		       ثابت hb_hashmap_t<غیرعلامتی, ثابت Feature*> *feature_substitutes_map, /*اندر*/
		       hb_سیٹ_t       *feature_indices /* IN/OUT */) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_سیٹ_t alternate_feature_indices;
    get_feature_variations ().احاطہ_features (lookup_indices, feature_ریکارڈ_cond_idx_map, &alternate_feature_indices);
    اگر (unlikely (alternate_feature_indices.اندر_error()))
    {
      feature_indices->err ();
      واپس;
    }
#اگر_ختم

    برائے (غیرعلامتی i : hb_iter (feature_indices))
    {
      hb_لیبل_t لیبل_2 =  get_feature_لیبل (i);
      اگر (لیبل_2 == HB_لیبل ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        جاری;


      ثابت Feature *f = &(get_feature (i));
      ثابت Feature** p = خالی_اشاریہ;
      اگر (feature_substitutes_map->has (i, &p))
        f = *p;

      اگر (!f->featureParams.is_null () &&
          لیبل_2 == HB_لیبل ('s', 'i', 'z', 'e'))
        جاری;

      اگر (!f->intersects_lookup_indexes (lookup_indices)
#اگر_غیر_تعریف_شدہ HB_NO_VAR
          && !alternate_feature_indices.has (i)
#اگر_ختم
	  )
	feature_indices->del (i);
    }
  }

  خالی collect_name_ids (ثابت hb_map_t *feature_اشاریہ_map,
                         hb_سیٹ_t *nameids_مقصد_retain, /*برآمد*/
                         ثابت hb_hashmap_t<غیرعلامتی, ثابت Feature*> *feature_substitutes_map,
                         ثابت hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map HB_غیر_مستعمل,
                         ثابت hb_سیٹ_t *catch_all_features,
                         منطقی all_axes_pinned HB_غیر_مستعمل) ثابت
  {
    غیرعلامتی گنتی = get_feature_گنتی ();
    برائے (غیرعلامتی i = 0 ; i < گنتی; i++)
    {
      اگر (!feature_اشاریہ_map->has (i)) جاری;
      hb_لیبل_t لیبل_2 = get_feature_لیبل (i);
      ثابت Feature *feature = &get_feature (i);
      ثابت Feature **substitute;
      اگر (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        اگر (catch_all_features->has (i))
          get_feature (i).collect_name_ids (لیبل_2, nameids_مقصد_retain);
      }
      feature->collect_name_ids (لیبل_2, nameids_مقصد_retain);
    }
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_اشاریہ_map, feature_ریکارڈ_cond_idx_map,
                                                  [یہ] (غیرعلامتی i) { واپس get_feature_لیبل (i); },
                                                  nameids_مقصد_retain);
#اگر_ختم
  }

  سانچہ <قسم_نام T>
  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = سچ;
      یہ->جدول = sc.reference_جدول<T> (face);

      اگر (unlikely (یہ->جدول->is_blocklisted (یہ->جدول.get_blob (), face)))
      {
	hb_blob_destroy (یہ->جدول.get_blob ());
	یہ->جدول = hb_blob_get_خالی_خانے ();
      }

      یہ->lookup_گنتی = جدول->get_lookup_گنتی ();

      یہ->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (یہ->lookup_گنتی, حجم (*accels));
      اگر (unlikely (!یہ->accels))
      {
	یہ->lookup_گنتی = 0;
	یہ->جدول.destroy ();
	یہ->جدول = hb_blob_get_خالی_خانے ();
      }
    }
    ~accelerator_t ()
    {
      برائے (غیرعلامتی صحیح i = 0; i < یہ->lookup_گنتی; i++)
      {
	خودکار *accel = یہ->accels[i].get_relaxed ();
	اگر (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (یہ->accels);
      یہ->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { واپس جدول.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (غیرعلامتی lookup_اشاریہ) ثابت
    {
      اگر (unlikely (lookup_اشاریہ >= lookup_گنتی)) واپس خالی_اشاریہ;

    retry:
      خودکار *accel = accels[lookup_اشاریہ].get_acquire ();
      اگر (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (جدول->get_lookup (lookup_اشاریہ));
	اگر (unlikely (!accel))
	  واپس خالی_اشاریہ;

	اگر (unlikely (!accels[lookup_اشاریہ].cmpexch (خالی_اشاریہ, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  جاؤ retry;
	}
      }

      واپس accel;
    }

    hb_blob_ptr_t<T> جدول;
    غیرعلامتی صحیح lookup_گنتی;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  محفوظ:
  سانچہ <قسم_نام TLookup>
  ثابت Offset16_مقصد<TLookupList<TLookup>>& typed_lookup_list_انحراف () ثابت
  { واپس reinterpret_cast<ثابت Offset16_مقصد<TLookupList<TLookup>>&> (lookupList); }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  سانچہ <قسم_نام TLookup>
  ثابت Offset32_مقصد<TLookupList2<TLookup>>& typed_lookup_list2_انحراف () ثابت
  { واپس reinterpret_cast<ثابت Offset32_مقصد<TLookupList2<TLookup>>&> (lookupList2); }
#اگر_ختم

  سانچہ <قسم_نام TLookup>
  ثابت Offset16_مقصد<TLookup_انحراف_List<TLookup>>& subset_lookup_list_انحراف () ثابت
  { واپس reinterpret_cast<ثابت Offset16_مقصد<TLookup_انحراف_List<TLookup>>&> (lookupList); }
  سانچہ <قسم_نام TLookup>
  Offset16_مقصد<TLookup_انحراف_List<TLookup>>& subset_lookup_list_انحراف ()
  { واپس reinterpret_cast<Offset16_مقصد<TLookup_انحراف_List<TLookup>>&> (lookupList); }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  سانچہ <قسم_نام TLookup>
  ثابت Offset32_مقصد<TLookup_انحراف_List2<TLookup>>& subset_lookup_list2_انحراف () ثابت
  { واپس reinterpret_cast<ثابت Offset32_مقصد<TLookup_انحراف_List2<TLookup>>&> (lookupList2); }
#اگر_ختم

  ثابت Offset16_مقصد<ریکارڈ_List_کا_Feature>& subset_feature_list_انحراف () ثابت
  { واپس reinterpret_cast<ثابت Offset16_مقصد<ریکارڈ_List_کا_Feature>&> (featureList); }
  Offset16_مقصد<ریکارڈ_List_کا_Feature>& subset_feature_list_انحراف ()
  { واپس reinterpret_cast<Offset16_مقصد<ریکارڈ_List_کا_Feature>&> (featureList); }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  ثابت Offset32_مقصد<ریکارڈ_List_کا_Feature>& subset_feature_list2_انحراف () ثابت
  { واپس reinterpret_cast<ثابت Offset32_مقصد<ریکارڈ_List_کا_Feature>&> (featureList2); }
#اگر_ختم

  ثابت خالی* get_lookup_list_میدان_انحراف () ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (نسخہ.مقصد_int () >= 0x00010002u && lookupList2) واپس &lookupList2;
#اگر_ختم
    واپس &lookupList;
  }

  سانچہ <قسم_نام انحراف_برآمد, قسم_نام انحراف_اندر, قسم_نام بنیاد_3, قسم_نام ...Ts>
  ساکن منطقی serialize_subset_انحراف (hb_subset_context_t *c,
				       انحراف_برآمد& برآمد,
				       ثابت انحراف_اندر& اندر,
				       ثابت بنیاد_3 *بنیاد,
				       Ts&&... ds)
  {
    برآمد = 0;
    اگر (اندر.is_null ()) واپس جھوٹ;

    خودکار *s = c->serializer;
    s->push ();
    منطقی ret = c->dispatch (بنیاد+اندر, std::forward<Ts> (ds)...);
    اگر (ret) s->جوڑو_link (برآمد, s->نکالو_pack ());
    ورنہ s->نکالو_discard ();
    واپس ret;
  }

  Fixed_نسخہ<>نسخہ;	/* Version of the GSUB/GPOS table--initially set
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
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  Offset32_مقصد<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32_مقصد<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32_مقصد<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#اگر_ختم
  عوامی:
  DEFINE_سائز_MIN (10);
};


} /* namespace OT */


#اگر_ختم /* HB_OT_LAYOUT_GSUBGPOS_HH */
