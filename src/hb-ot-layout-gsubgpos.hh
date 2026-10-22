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

#که_نه_تعریف_شوی HB_OT_LAYOUT_GSUBGPOS_HH
#تعریف_کړه HB_OT_LAYOUT_GSUBGPOS_HH

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
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { أرجع obj.intersects (هذا->glyphs); }
  ساكن return_t default_return_ارزښت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  ثابت hb_سېټ_t *glyphs;

  hb_intersects_context_t (ثابت hb_سېټ_t *glyphs_) :
                            glyphs (glyphs_) {}
};

بنية hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, منطقي>
{
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { أرجع obj.may_have_non_1to1 (); }
  ساكن return_t default_return_ارزښت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
};

بنية hb_تړون_context_t :
       hb_dispatch_context_t<hb_تړون_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_تړون_context_t *c, بینښه lookup_شاخص, hb_سېټ_t *covered_seq_indicies, بینښه seq_شاخص, بینښه پای_ټکی_شاخص);
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { obj.تړون (هذا); أرجع hb_تشه_t (); }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }
  فراغ recurse (بینښه lookup_شاخص, hb_سېټ_t *covered_seq_indicies, بینښه seq_شاخص, بینښه پای_ټکی_شاخص)
  {
    إذا (unlikely (nesting_level_کیڼ == 0 || !recurse_func))
      أرجع;

    nesting_level_کیڼ--;
    recurse_func (هذا, lookup_شاخص, covered_seq_indicies, seq_شاخص, پای_ټکی_شاخص);
    nesting_level_کیڼ++;
  }

  فراغ reset_lookup_لیدنه_شمېر ()
  { lookup_شمېر = 0; }

  منطقي lookup_حد_exceeded ()
  { أرجع lookup_شمېر > HB_MAX_LOOKUP_لیدنه_شمېر; }

  منطقي should_لیدنه_lookup (بینښه صحيح lookup_شاخص)
  {
    إذا (lookup_شمېر++ > HB_MAX_LOOKUP_لیدنه_شمېر)
      أرجع خطأ;

    إذا (is_lookup_بشپړ (lookup_شاخص))
      أرجع خطأ;

    أرجع ریښتیا;
  }

  منطقي is_lookup_بشپړ (بینښه صحيح lookup_شاخص)
  {
    إذا (unlikely (بشپړ_lookups_glyph_شمېر->دننه_error () ||
		  بشپړ_lookups_glyph_سېټ->دننه_error ()))
      أرجع ریښتیا;

    /* Have we visited this lookup with the current set of glyphs? */
    بینښه راوباسه = glyphs->get_population ();
    إذا (unlikely (راوباسه == HB_MAP_ارزښت_INVALID)) // Avoid sentinel interference.
      راوباسه--;
    إذا (بشپړ_lookups_glyph_شمېر->get (lookup_شاخص) != راوباسه)
    {
      بشپړ_lookups_glyph_شمېر->سېټ (lookup_شاخص, راوباسه);

      إذا (!بشپړ_lookups_glyph_سېټ->has (lookup_شاخص))
      {
	إذا (unlikely (!بشپړ_lookups_glyph_سېټ->سېټ (lookup_شاخص, hb::unique_ptr<hb_سېټ_t> {hb_سېټ_create ()})))
	  أرجع ریښتیا;
      }

      بشپړ_lookups_glyph_سېټ->get (lookup_شاخص)->clear ();
    }

    hb_سېټ_t *covered_glyph_سېټ = بشپړ_lookups_glyph_سېټ->get (lookup_شاخص);
    إذا (unlikely (covered_glyph_سېټ->دننه_error ()))
      أرجع ریښتیا;
    إذا (پلار_فعال_glyphs ().is_subset (*covered_glyph_سېټ))
      أرجع ریښتیا;

    covered_glyph_سېټ->union_ (پلار_فعال_glyphs ());
    أرجع خطأ;
  }

  ثابت hb_سېټ_t& پخوانی_پلار_فعال_glyphs () {
    إذا (فعال_glyphs_ډېری.اوږدتیا <= 1)
      أرجع *glyphs;

    أرجع فعال_glyphs_ډېری[فعال_glyphs_ډېری.اوږدتیا - 2];
  }

  ثابت hb_سېټ_t& پلار_فعال_glyphs ()
  {
    إذا (!فعال_glyphs_ډېری)
      أرجع *glyphs;

    أرجع فعال_glyphs_ډېری.لکۍ ();
  }

  hb_سېټ_t* ټېل_cur_فعال_glyphs ()
  {
    hb_سېټ_t *s = فعال_glyphs_ډېری.push ();
    إذا (unlikely (فعال_glyphs_ډېری.دننه_error ()))
      أرجع تش_اشاره;
    أرجع s;
  }

  منطقي راوباسه_cur_بشپړ_glyphs ()
  {
    إذا (!فعال_glyphs_ډېری)
      أرجع خطأ;

    فعال_glyphs_ډېری.راوباسه ();
    أرجع ریښتیا;
  }

  hb_face_t *face;
  hb_سېټ_t *glyphs;
  hb_سېټ_t output[1];
  hb_ویکټور_t<hb_سېټ_t> فعال_glyphs_ډېری;
  recurse_func_t recurse_func = تش_اشاره;
  بینښه صحيح nesting_level_کیڼ;

  hb_تړون_context_t (hb_face_t *face_,
			hb_سېټ_t *glyphs_,
			hb_map_t *بشپړ_lookups_glyph_شمېر_2,
			hb_hashmap_t<بینښه, hb::unique_ptr<hb_سېټ_t>> *بشپړ_lookups_glyph_سېټ_2,
			بینښه صحيح nesting_level_کیڼ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_کیڼ (nesting_level_کیڼ_2),
			  بشپړ_lookups_glyph_شمېر (بشپړ_lookups_glyph_شمېر_2),
			  بشپړ_lookups_glyph_سېټ (بشپړ_lookups_glyph_سېټ_2)
  {}

  ~hb_تړون_context_t () { flush (); }

  فراغ سېټ_recurse_func (recurse_func_t func) { recurse_func = func; }

  فراغ flush ()
  {
    output->del_range (face->get_عدد_glyphs (), HB_سېټ_ارزښت_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    فعال_glyphs_ډېری.راوباسه ();
    فعال_glyphs_ډېری.reset ();
  }

  خاص:
  hb_map_t *بشپړ_lookups_glyph_شمېر;
  hb_hashmap_t<بینښه, hb::unique_ptr<hb_سېټ_t>> *بشپړ_lookups_glyph_سېټ;
  بینښه صحيح lookup_شمېر = 0;
};



بنية hb_تړون_lookups_context_t :
       hb_dispatch_context_t<hb_تړون_lookups_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_تړون_lookups_context_t *c, بینښه lookup_شاخص);
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { obj.تړون_lookups (هذا); أرجع hb_تشه_t (); }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }
  فراغ recurse (بینښه lookup_شاخص)
  {
    إذا (unlikely (nesting_level_کیڼ == 0 || !recurse_func))
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (lookup_حد_exceeded ()
        || لیدل_شوی_lookups->دننه_error ()
        || لیدل_شوی_lookups->has (lookup_شاخص))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      أرجع;

    nesting_level_کیڼ--;
    recurse_func (هذا, lookup_شاخص);
    nesting_level_کیڼ++;
  }

  فراغ سېټ_lookup_لیدل_شوی (بینښه lookup_شاخص)
  { لیدل_شوی_lookups->جمع_کړه (lookup_شاخص); }

  فراغ سېټ_lookup_inactive (بینښه lookup_شاخص)
  { inactive_lookups->جمع_کړه (lookup_شاخص); }

  منطقي lookup_حد_exceeded ()
  {
    منطقي ret = lookup_شمېر > HB_MAX_LOOKUP_لیدنه_شمېر;
    إذا (ret)
      DEBUG_پیغام (SUBSET, تش_اشاره, "lookup visit count limit exceeded in lookup closure!");
    أرجع ret; }

  منطقي is_lookup_لیدل_شوی (بینښه lookup_شاخص)
  {
    إذا (unlikely (lookup_شمېر++ > HB_MAX_LOOKUP_لیدنه_شمېر))
    {
      DEBUG_پیغام (SUBSET, تش_اشاره, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_شمېر, lookup_شاخص);
      أرجع ریښتیا;
    }

    إذا (unlikely (لیدل_شوی_lookups->دننه_error ()))
      أرجع ریښتیا;

    أرجع لیدل_شوی_lookups->has (lookup_شاخص);
  }

  hb_face_t *face;
  ثابت hb_سېټ_t *glyphs;
  recurse_func_t recurse_func;
  بینښه صحيح nesting_level_کیڼ;

  hb_تړون_lookups_context_t (hb_face_t *face_,
				ثابت hb_سېټ_t *glyphs_,
				hb_سېټ_t *لیدل_شوی_lookups_2,
				hb_سېټ_t *inactive_lookups_,
				بینښه nesting_level_کیڼ_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (تش_اشاره),
				nesting_level_کیڼ (nesting_level_کیڼ_2),
				لیدل_شوی_lookups (لیدل_شوی_lookups_2),
				inactive_lookups (inactive_lookups_),
				lookup_شمېر (0) {}

  فراغ سېټ_recurse_func (recurse_func_t func) { recurse_func = func; }

  خاص:
  hb_سېټ_t *لیدل_شوی_lookups;
  hb_سېټ_t *inactive_lookups;
  بینښه صحيح lookup_شمېر;
};

بنية hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, منطقي>
{
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { أرجع obj.would_apply (هذا); }
  ساكن return_t default_return_ارزښت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  hb_face_t *face;
  ثابت hb_codepoint_t *glyphs;
  بینښه صحيح len;
  منطقي zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ثابت hb_codepoint_t *glyphs_,
			    بینښه صحيح len_,
			    منطقي zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

بنية hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, بینښه صحيح lookup_شاخص);
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { obj.collect_glyphs (هذا); أرجع hb_تشه_t (); }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }
  فراغ recurse (بینښه صحيح lookup_شاخص)
  {
    إذا (unlikely (nesting_level_کیڼ == 0 || !recurse_func))
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

    إذا (output == hb_سېټ_get_تشه ())
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (recursed_lookups->has (lookup_شاخص))
      أرجع;

    hb_سېټ_t *old_before = before;
    hb_سېټ_t *old_ننوت  = ننوت;
    hb_سېټ_t *old_after  = after;
    before = ننوت = after = hb_سېټ_get_تشه ();

    nesting_level_کیڼ--;
    recurse_func (هذا, lookup_شاخص);
    nesting_level_کیڼ++;

    before = old_before;
    ننوت  = old_ننوت;
    after  = old_after;

    recursed_lookups->جمع_کړه (lookup_شاخص);
  }

  hb_face_t *face;
  hb_سېټ_t *before;
  hb_سېټ_t *ننوت;
  hb_سېټ_t *after;
  hb_سېټ_t *output;
  recurse_func_t recurse_func;
  hb_سېټ_t *recursed_lookups;
  بینښه صحيح nesting_level_کیڼ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_سېټ_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_سېټ_t  *glyphs_ننوت,  /* OUT.  May be NULL */
			       hb_سېټ_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_سېټ_t  *glyphs_output, /* OUT.  May be NULL */
			       بینښه صحيح nesting_level_کیڼ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_سېټ_get_تشه ()),
			      ننوت  (glyphs_ننوت  ? glyphs_ننوت  : hb_سېټ_get_تشه ()),
			      after  (glyphs_after  ? glyphs_after  : hb_سېټ_get_تشه ()),
			      output (glyphs_output ? glyphs_output : hb_سېټ_get_تشه ()),
			      recurse_func (تش_اشاره),
			      recursed_lookups (hb_سېټ_create ()),
			      nesting_level_کیڼ (nesting_level_کیڼ_2) {}
  ~hb_collect_glyphs_context_t () { hb_سېټ_destroy (recursed_lookups); }

  فراغ سېټ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

بنية hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  تعداد glyph_سېټ_type_t
  {
    GLYPH_سېټ_CLASS,
    GLYPH_سېټ_COVERAGE,
    GLYPH_سېټ_SINGLE
  };

  ساكن constexpr hb_codepoint_t SINGLE_GLYPH_فعال_FLAG = 0x80000000u;

  بنية glyph_سېټ_cache_کلی_t
  {
    glyph_سېټ_cache_کلی_t () = افتراضي;
    glyph_سېټ_cache_کلی_t (ثابت فراغ *معلومات_2, بینښه ارزښت_3, glyph_سېټ_type_t type_)
      : معلومات (معلومات_2), value (ارزښت_3), type (type_) {}

    منطقي چلوونکی == (ثابت glyph_سېټ_cache_کلی_t &o) ثابت
    { أرجع معلومات == o.معلومات && value == o.value && type == o.type; }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) معلومات);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash ((بینښه) type);
    }

    ثابت فراغ *معلومات = تش_اشاره;
    بینښه value = 0;
    glyph_سېټ_type_t type = GLYPH_سېټ_CLASS;
  };

  بنية context_سېټ_cache_کلی_t
  {
    context_سېټ_cache_کلی_t () = افتراضي;
    context_سېټ_cache_کلی_t (ثابت فراغ *قاعده,
			     ثابت فراغ *ریکارډ_3,
			     ثابت فراغ *معلومات_2,
			     hb_codepoint_t فعال_glyphs,
			     بینښه ارزښت_3,
			     بینښه format_)
      : قاعده_2 (قاعده), ریکارډ (ریکارډ_3), معلومات (معلومات_2), فعال_glyphs_2 (فعال_glyphs),
	value (ارزښت_3), format (format_) {}

    منطقي چلوونکی == (ثابت context_سېټ_cache_کلی_t &o) ثابت
    {
      أرجع قاعده_2 == o.قاعده_2 &&
	     ریکارډ == o.ریکارډ &&
	     معلومات == o.معلومات &&
	     فعال_glyphs_2 == o.فعال_glyphs_2 &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) قاعده_2);
      current = current * 31 + hb_hash ((uintptr_t) ریکارډ);
      current = current * 31 + hb_hash ((uintptr_t) معلومات);
      current = current * 31 + hb_hash (فعال_glyphs_2);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash (format);
    }

    ثابت فراغ *قاعده_2 = تش_اشاره;
    ثابت فراغ *ریکارډ = تش_اشاره;
    ثابت فراغ *معلومات = تش_اشاره;
    hb_codepoint_t فعال_glyphs_2 = HB_CODEPOINT_INVALID;
    بینښه value = 0;
    بینښه format = 0;
  };

  بنية recurse_کلی_t
  {
    recurse_کلی_t () = افتراضي;
    recurse_کلی_t (بینښه lookup_شاخص_3,
                   hb_codepoint_t فعال_glyphs,
                   hb_codepoint_t recurse_لار,
                   hb_codepoint_t context_سېټ_2)
      : lookup_شاخص (lookup_شاخص_3),
        فعال_glyphs_2 (فعال_glyphs),
        recurse_لار_2 (recurse_لار),
        context_سېټ (context_سېټ_2) {}

    منطقي چلوونکی == (ثابت recurse_کلی_t &o) ثابت
    {
      أرجع lookup_شاخص == o.lookup_شاخص &&
             فعال_glyphs_2 == o.فعال_glyphs_2 &&
             recurse_لار_2 == o.recurse_لار_2 &&
             context_سېټ == o.context_سېټ;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_شاخص)) * 16777619;
      current = (current ^ hb_hash (فعال_glyphs_2)) * 16777619;
      current = (current ^ hb_hash (recurse_لار_2)) * 16777619;
      current = (current ^ hb_hash (context_سېټ)) * 16777619;
      أرجع current;
    }

    بینښه lookup_شاخص = 0;
    hb_codepoint_t فعال_glyphs_2 = 0;
    hb_codepoint_t recurse_لار_2 = 0;
    hb_codepoint_t context_سېټ = HB_CODEPOINT_INVALID;
  };

  تعريفنوع return_t (*recurse_func_t) (hb_depend_context_t *c, بینښه lookup_شاخص, hb_بټ_پاڼه_t *covered_seq_indices, بینښه seq_شاخص, بینښه پای_ټکی_شاخص);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { obj.depend (هذا); أرجع hb_تشه_t (); }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }

  فراغ recurse (بینښه lookup_idx, hb_بټ_پاڼه_t *covered_seq_indices, بینښه seq_شاخص, بینښه پای_ټکی_شاخص)
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
    إذا (lookups_کتل_شوی.has (lookup_idx)) أرجع;
    lookups_کتل_شوی.جمع_کړه (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t پلار_لار = recurse_لار_2;
    uint64_t لار_کلی = (uint64_t) پلار_لار << 32 | lookup_idx;
    hb_codepoint_t *لار = تش_اشاره;
    hb_codepoint_t لار_شاخص;
    إذا (!recurse_لار_منزل_شاخص.has (لار_کلی, &لار))
    {
      لار_شاخص = recurse_لار_منزل_شاخص.get_population () + 1;
      إذا (unlikely (!recurse_لار_منزل_شاخص.سېټ (لار_کلی, لار_شاخص)))
      {
	depend_معلومات->fail ();
	lookups_کتل_شوی.del (lookup_idx);
	أرجع;
      }
    }
    وإلا
      لار_شاخص = *لار;
    recurse_لار_2 = لار_شاخص;
    recurse_func (هذا, lookup_idx, covered_seq_indices, seq_شاخص, پای_ټکی_شاخص);
    recurse_لار_2 = پلار_لار;
    lookups_کتل_شوی.del (lookup_idx);
  }

  بنية فعال_glyphs_چوکاټ_t
  {
    hb_سېټ_t glyphs;
    hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
    منطقي use_ریښه = خطأ;
  };

  ثابت hb_سېټ_t& پلار_فعال_glyphs ()
  {
    إذا (!فعال_glyphs_ډېری_ژوروالی)
      أرجع *glyphs;

    ثابت فعال_glyphs_چوکاټ_t &چوکاټ = فعال_glyphs_ډېری[فعال_glyphs_ډېری_ژوروالی - 1];
    أرجع چوکاټ.use_ریښه ? *glyphs : چوکاټ.glyphs;
  }

  hb_سېټ_t* ټېل_cur_فعال_glyphs (hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID)
  {
    إذا (فعال_glyphs_ډېری_ژوروالی == فعال_glyphs_ډېری.اوږدتیا &&
	unlikely (!فعال_glyphs_ډېری.ټېل_or_fail ()))
      أرجع تش_اشاره;

    فعال_glyphs_چوکاټ_t &چوکاټ = فعال_glyphs_ډېری[فعال_glyphs_ډېری_ژوروالی++];
    چوکاټ.glyphs.clear ();
    چوکاټ.فعال_idx = فعال_idx;
    چوکاټ.use_ریښه = خطأ;
    أرجع &چوکاټ.glyphs;
  }

  منطقي ټېل_ریښه_فعال_glyphs ()
  {
    إذا (فعال_glyphs_ډېری_ژوروالی == فعال_glyphs_ډېری.اوږدتیا &&
	unlikely (!فعال_glyphs_ډېری.ټېل_or_fail ()))
      أرجع خطأ;

    فعال_glyphs_ډېری[فعال_glyphs_ډېری_ژوروالی++].use_ریښه = ریښتیا;
    أرجع ریښتیا;
  }

  منطقي راوباسه_cur_بشپړ_glyphs ()
  {
    إذا (!فعال_glyphs_ډېری_ژوروالی)
      أرجع خطأ;

    فعال_glyphs_ډېری_ژوروالی--;
    أرجع ریښتیا;
  }

  منطقي get_پلار_فعال_glyphs_شاخص (hb_codepoint_t *فعال_idx)
  {
    إذا (!فعال_glyphs_ډېری_ژوروالی ||
	فعال_glyphs_ډېری[فعال_glyphs_ډېری_ژوروالی - 1].use_ریښه)
    {
      *فعال_idx = HB_CODEPOINT_INVALID;
      أرجع ریښتیا;
    }

    فعال_glyphs_چوکاټ_t &چوکاټ = فعال_glyphs_ډېری[فعال_glyphs_ډېری_ژوروالی - 1];
    *فعال_idx = چوکاټ.فعال_idx;
    إذا (*فعال_idx != HB_CODEPOINT_INVALID)
      أرجع ریښتیا;

    hb_codepoint_t glyph;
    إذا (چوکاټ.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_فعال_FLAG))
      *فعال_idx = glyph | SINGLE_GLYPH_فعال_FLAG;
    وإلا إذا (!پیدا_کړه_or_create_recurse_سېټ (چوکاټ.glyphs, فعال_idx))
      أرجع خطأ;

    چوکاټ.فعال_idx = *فعال_idx;
    أرجع ریښتیا;
  }

  منطقي get_context_سېټ_cache_کلی (ثابت فراغ *قاعده_2,
				  ثابت فراغ *ریکارډ,
				  ثابت فراغ *معلومات,
				  بینښه value,
				  بینښه format,
				  context_سېټ_cache_کلی_t *key)
  {
    hb_codepoint_t فعال_idx;
    إذا (!get_پلار_فعال_glyphs_شاخص (&فعال_idx))
      أرجع خطأ;
    *key = context_سېټ_cache_کلی_t (قاعده_2, ریکارډ, معلومات, فعال_idx, value, format);
    أرجع ریښتیا;
  }

  منطقي get_recurse_کلی (بینښه lookup_idx, recurse_کلی_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t فعال_idx;
    إذا (!get_پلار_فعال_glyphs_شاخص (&فعال_idx))
      أرجع خطأ;

    *key = recurse_کلی_t (lookup_idx,
                          فعال_idx,
                          recurse_لار_2,
                          depend_معلومات->current_context_سېټ_شاخص);
    أرجع !completed_recursions.has (*key);
  }

  فراغ پای_recurse (ثابت recurse_کلی_t &key)
  { depend_معلومات->check_success (completed_recursions.سېټ (key, ریښتیا)); }

  فراغ reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_لار_منزل_شاخص.clear ();
    recurse_لار_2 = 0;
  }

  منطقي پیدا_کړه_or_create_recurse_سېټ (ثابت hb_سېټ_t &سېټ, hb_codepoint_t *شاخص_2)
  {
    hb_codepoint_t *existing = تش_اشاره;
    إذا (recurse_سېټ_منزل_شاخص.has (&سېټ, &existing))
    {
      *شاخص_2 = *existing;
      أرجع ریښتیا;
    }

    hb_سېټ_t *copy = hb_object_create<hb_سېټ_t> ();
    إذا (unlikely (!copy))
      أرجع depend_معلومات->fail ();
    copy->سېټ (سېټ);
    إذا (unlikely (copy->دننه_error ()))
    {
      hb_سېټ_destroy (copy);
      أرجع depend_معلومات->fail ();
    }

    *شاخص_2 = recurse_sets.اوږدتیا;
    إذا (unlikely (*شاخص_2 & SINGLE_GLYPH_فعال_FLAG))
    {
      hb_سېټ_destroy (copy);
      أرجع depend_معلومات->fail ();
    }
    إذا (unlikely (!recurse_sets.ټېل_or_fail (hb::unique_ptr<hb_سېټ_t> {copy})))
      أرجع depend_معلومات->fail ();
    إذا (unlikely (!recurse_سېټ_منزل_شاخص.سېټ (recurse_sets[*شاخص_2].get (), *شاخص_2)))
      أرجع depend_معلومات->fail ();
    أرجع ریښتیا;
  }

  ثابت hb_سېټ_t *پیدا_کړه_or_create_glyph_سېټ (ثابت فراغ *معلومات,
                                             بینښه value,
                                             glyph_سېټ_type_t type);
  hb_codepoint_t پیدا_کړه_or_create_cached_context_سېټ (ثابت hb_سېټ_t *سېټ);

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
      ننوت_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_سېټ_t preliminary_context;
    hb_سېټ_t position_scratch;
    hb_سېټ_t pos_glyphs;
    hb_سېټ_t position_context;
    hb_سېټ_t disjunctive_indices;
    hb_سېټ_t filtered_disjunctive_indices;
    hb_ویکټور_t<ثابت hb_سېټ_t *> backtrack_sets;
    hb_ویکټور_t<ثابت hb_سېټ_t *> lookahead_sets;
    hb_ویکټور_t<ثابت hb_سېټ_t *> ننوت_position_glyphs;
    hb_ویکټور_t<hb_codepoint_t> cached_context_sets;
    hb_ویکټور_t<بینښه> compact_workspace;
    depend_scratch_t *next = تش_اشاره;
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
        أرجع تش_اشاره;
      }
      جديد (scratch) depend_scratch_t ();
    }

    scratch->next = تش_اشاره;
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
  hb_سېټ_t *glyphs;
  hb_ویکټور_t<فعال_glyphs_چوکاټ_t> فعال_glyphs_ډېری;
  بینښه فعال_glyphs_ډېری_ژوروالی = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_شاخص = HB_CODEPOINT_INVALID;
  hb_بټ_سېټ_t lookups_کتل_شوی;
  hb_ویکټور_t<hb::unique_ptr<hb_سېټ_t>> recurse_sets;
  hb_hashmap_t<ثابت hb_سېټ_t *, hb_codepoint_t> recurse_سېټ_منزل_شاخص;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_لار_منزل_شاخص;
  hb_codepoint_t recurse_لار_2 = 0;
  hb_hashmap_t<recurse_کلی_t, منطقي> completed_recursions;
  hb_ویکټور_t<hb::unique_ptr<hb_سېټ_t>> glyph_sets;
  hb_hashmap_t<glyph_سېټ_cache_کلی_t, hb_codepoint_t> glyph_سېټ_منزل_شاخص;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_سېټ_indices;
  hb_hashmap_t<context_سېټ_cache_کلی_t, hb_codepoint_t> context_سېټ_cache;
  depend_scratch_t *depend_scratch_pool = تش_اشاره;

  hb_depend_context_t (hb_depend_معلومات_builder_t *depend_معلومات_2,
                       hb_face_t *face_,
                       hb_سېټ_t *glyphs_)
                      : depend_معلومات (depend_معلومات_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (تش_اشاره) {}
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
  فراغ سېټ_recurse_func (recurse_func_t func) { recurse_func = func; }
};

کينډۍ <ډول_نوم سېټ_t>
بنية hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<سېټ_t>, ثابت Coverage &>
{
  تعريفنوع ثابت Coverage &return_t; // Stoopid that we have to dupe this here.
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { أرجع obj.get_coverage (); }
  ساكن return_t default_return_ارزښت () { أرجع Null (Coverage); }
  منطقي stop_sublookup_iteration (return_t r) ثابت
  {
    r.collect_coverage (سېټ);
    أرجع خطأ;
  }

  hb_collect_coverage_context_t (سېټ_t *سېټ_2) :
				   سېټ (سېټ_2) {}

  سېټ_t *سېټ;
};

بنية matcher_t
{
  تعريفنوع منطقي (*سمون_func_t) (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات);

  کينډۍ <ډول_نوم context_t>
  فراغ init (ثابت context_t *c, منطقي context_سمون = خطأ)
  {
    سېټ_سمون_func (تش_اشاره, تش_اشاره);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->جدول_شاخص == 1 || (context_سمون && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_سمون || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->جدول_شاخص == 1;
    ماسک = context_سمون ? -1 : c->lookup_ماسک;
    /* Per syllable matching is only for GSUB. */
    فی_syllable = c->جدول_شاخص == 0 && c->فی_syllable;
    syllable = 0;
  }

  فراغ سېټ_سمون_func (سمون_func_t سمون_func,
		       ثابت فراغ *سمون_معلومات)
  { سمون_func_2 = سمون_func; سمون_معلومات_2 = سمون_معلومات; }

  تعداد may_سمون_t {
    سمون_NO,
    سمون_YES,
    سمون_MAYBE
  };

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  may_سمون_t may_سمون (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_معلومات) ثابت
  {
    إذا (!(info.ماسک & ماسک) ||
	(فی_syllable && syllable && syllable != info.syllable ()))
      أرجع سمون_NO;

    إذا (سمون_func_2)
      أرجع سمون_func_2 (info, glyph_معلومات, سمون_معلومات_2) ? سمون_YES : سمون_NO;

    أرجع سمون_MAYBE;
  }

  تعداد may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  کينډۍ <ډول_نوم context_t>
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
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
  بینښه صحيح lookup_props = 0;
  hb_ماسک_t ماسک = -1;
  منطقي ignore_zwnj = خطأ;
  منطقي ignore_zwj = خطأ;
  منطقي ignore_hidden = خطأ;
  منطقي فی_syllable = خطأ;
  uint8_t syllable = 0;
  سمون_func_t سمون_func_2 = تش_اشاره;
  ثابت فراغ *سمون_معلومات_2 = تش_اشاره;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
بنية glyph_معلومات_t
{
  کينډۍ <ډول_نوم T>
  glyph_معلومات_t (ثابت T *معلومات_2) : معلومات (معلومات_2)
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
                              , wide (حجم (T) == 3)
#د_که_پای
  {
    static_assert (حجم (T) == 2 || حجم (T) == 3, "invalid glyph data size");
  }

  بینښه چلوونکی [] (بینښه i) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (wide)
      أرجع ((ثابت HBUINT24 *) معلومات)[i];
#د_که_پای
    أرجع ((ثابت HBUINT16 *) معلومات)[i];
  }

  ثابت فراغ *معلومات;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  منطقي wide;
#د_که_پای
};

کينډۍ <ډول_نوم context_t>
بنية skipping_iterator_t
{
  فراغ init (context_t *c_, منطقي context_سمون = خطأ)
  {
    c = c_;
    پای_ټکی = c->بفر->len;
    سمون_glyph_data16 = تش_اشاره;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    سمون_glyph_data24 = تش_اشاره;
#د_که_پای
    matcher.init (c, context_سمون);
  }
  فراغ سېټ_lookup_props (بینښه صحيح lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  فراغ سېټ_سمون_func (matcher_t::سمون_func_t سمون_func,
		       ثابت فراغ *سمون_معلومات)
  {
    matcher.سېټ_سمون_func (سمون_func, سمون_معلومات);
  }
  فراغ سېټ_glyph_معلومات (glyph_معلومات_t glyph_معلومات)
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    سمون_glyph_data16 = glyph_معلومات.wide ? تش_اشاره : (ثابت HBUINT16 *) glyph_معلومات.معلومات;
    سمون_glyph_data24 = glyph_معلومات.wide ? (ثابت HBUINT24 *) glyph_معلومات.معلومات : تش_اشاره;
#وإلا
    سمون_glyph_data16 = (ثابت HBUINT16 *) glyph_معلومات.معلومات;
#د_که_پای
  }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  فراغ reset (بینښه صحيح پیل_شاخص_2)
  {
    // For GSUB forward iterator
    idx = پیل_شاخص_2;
    پای_ټکی = c->بفر->len;
    matcher.syllable = c->بفر->cur().syllable();
  }
  فراغ reset_شا (بینښه صحيح پیل_شاخص_2, منطقي from_بهر_بفر = خطأ)
  {
    // For GSUB backward iterator
    idx = پیل_شاخص_2;
    matcher.syllable = c->بفر->cur().syllable();
  }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  فراغ reset_fast (بینښه صحيح پیل_شاخص_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = پیل_شاخص_2;
  }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  matcher_t::may_skip_t may_skip (ثابت hb_glyph_info_t &info) ثابت
  { أرجع matcher.may_skip (c, info); }

  تعداد سمون_t {
    سمون_2,
    NOT_سمون,
    SKIP
  };

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  سمون_t سمون (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    إذا (unlikely (skip == matcher_t::SKIP_YES))
      أرجع SKIP;

    matcher_t::may_سمون_t سمون = matcher.may_سمون (info, get_glyph_معلومات ());
    إذا (سمون == matcher_t::سمون_YES ||
	(سمون == matcher_t::سمون_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      أرجع سمون_2;

    إذا (skip == matcher_t::SKIP_NO)
      أرجع NOT_سمون;

    أرجع SKIP;
  }

  منطقي next (بینښه *unsafe_منزل = تش_اشاره)
  {
    خپلکاری *info = c->بفر->info;
    ثابت نښهدار stop = (نښهدار) پای_ټکی - 1;
    طالما ((نښهدار) idx < stop)
    {
      idx++;
      بدل (سمون (info[idx]))
      {
	حالة سمون_2:
	{
	  advance_glyph_معلومات ();
	  أرجع ریښتیا;
	}
	حالة NOT_سمون:
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
      *unsafe_منزل = پای_ټکی;
    أرجع خطأ;
  }
  منطقي prev (بینښه *unsafe_from = تش_اشاره)
  {
    خپلکاری *بهر_info = c->بفر->بهر_info;
    ثابت بینښه stop = 0;
    طالما (idx > stop)
    {
      idx--;
      بدل (سمون (بهر_info[idx]))
      {
	حالة سمون_2:
	{
	  advance_glyph_معلومات ();
	  أرجع ریښتیا;
	}
	حالة NOT_سمون:
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
    إذا (سمون_glyph_data16) أرجع *سمون_glyph_data16;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    وإلا
    إذا (سمون_glyph_data24) أرجع *سمون_glyph_data24;
#د_که_پای
    أرجع 0;
  }
  HB_ALWAYS_INLINE
  فراغ
  advance_glyph_معلومات ()
  {
    إذا (سمون_glyph_data16) سمون_glyph_data16++;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    وإلا
    إذا (سمون_glyph_data24) سمون_glyph_data24++;
#د_که_پای
  }

  بینښه صحيح idx;
  محمي:
  context_t *c;
  matcher_t matcher;
  ثابت HBUINT16 *سمون_glyph_data16;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  ثابت HBUINT24 *سمون_glyph_data24;
#د_که_پای

  بینښه صحيح پای_ټکی;
};

بنية hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  تعريفنوع return_t (*recurse_func_t) (hb_ot_apply_context_t *c, بینښه صحيح lookup_شاخص);

  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_لومړیتوب<1>) HB_RETURN (return_t, obj.apply (c, تش_اشاره) )
  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_لومړیتوب<0>) HB_RETURN (return_t, obj.apply (c) )
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { أرجع apply_(obj, هذا, hb_prioritize); }

  ساكن return_t default_return_ارزښت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
  return_t recurse (بینښه صحيح sub_lookup_شاخص)
  {
    أكد (recurse_func);
    إذا (unlikely (nesting_level_کیڼ == 0))
    {
      بفر->successful = خطأ;
      أرجع default_return_ارزښت ();
    }

    بفر->max_ops--;
    إذا (unlikely (بفر->max_ops < 0))
    {
      بفر->successful = خطأ;
      أرجع default_return_ارزښت ();
    }

    nesting_level_کیڼ--;
    منطقي ret = recurse_func (هذا, sub_lookup_شاخص);
    nesting_level_کیڼ++;
    أرجع ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_ننوت, iter_context;

  بینښه صحيح جدول_شاخص; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_بفر_t *بفر;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = تش_اشاره;
  ثابت GDEF &gdef;
  ثابت GDEF::accelerator_t &gdef_accel;
  ثابت توکی_Variation_Store &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_ماسک_t lookup_ماسک = 1;
  بینښه صحيح lookup_شاخص = (بینښه) -1;
  بینښه صحيح lookup_props = 0;
  بینښه صحيح nesting_level_کیڼ = HB_MAX_NESTING_LEVEL;

  منطقي has_glyph_classes;
  منطقي auto_zwnj = ریښتیا;
  منطقي auto_zwj = ریښتیا;
  منطقي فی_syllable = خطأ;
  منطقي random = خطأ;
  بینښه new_syllables = (بینښه) -1;

  نښهدار وروستی_بنسټ = -1; // GPOS uses
  بینښه وروستی_بنسټ_until = 0; // GPOS uses

  hb_ویکټور_t<uint32_t> سمون_positions;
  uint32_t ډېری_سمون_positions[8];
#که_نه_تعریف_شوی HB_NO_بفر_پیغام
  hb_بفر_t::بدل_شوی_func_t orig_بدل_شوی_func = تش_اشاره;
  فراغ *orig_بدل_شوی_معلومات = تش_اشاره;
#د_که_پای

  hb_ot_apply_context_t (بینښه صحيح جدول_شاخص_2,
			 hb_font_t *font_,
			 hb_بفر_t *بفر_2,
			 hb_blob_t *جدول_blob_2,
			 hb_scalar_cache_t *var_store_cache_ = تش_اشاره) :
			جدول_شاخص (جدول_شاخص_2),
			font (font_), face (font->face), بفر (بفر_2),
			sanitizer (جدول_blob_2),
			gdef (
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT
			      *face->جدول.GDEF->جدول
#وإلا
			      Null (GDEF)
#د_که_پای
			     ),
			gdef_accel (
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT
			      *face->جدول.GDEF
#وإلا
			      Null (GDEF::accelerator_t)
#د_که_پای
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (بفر_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    سمون_positions.سېټ_storage (ډېری_سمون_positions);

#که_نه_تعریف_شوی HB_NO_بفر_پیغام
    إذا (بفر->messaging ())
    {
      أكد (بفر->بدل_شوی_func == تش_اشاره ||
	      بفر->بدل_شوی_func == بفر_بدل_شوی_trampoline);
      orig_بدل_شوی_func = بفر->بدل_شوی_func;
      orig_بدل_شوی_معلومات = بفر->بدل_شوی_معلومات;
      بفر->بدل_شوی_func = بفر_بدل_شوی_trampoline;
      بفر->بدل_شوی_معلومات = هذا;
    }
#د_که_پای
  }
  ~hb_ot_apply_context_t ()
  {
#که_نه_تعریف_شوی HB_NO_بفر_پیغام
    إذا (بفر->messaging ())
    {
      أكد (بفر->بدل_شوی_func == بفر_بدل_شوی_trampoline);
      أكد (بفر->بدل_شوی_معلومات == هذا);
      بفر->بدل_شوی_func = orig_بدل_شوی_func;
      بفر->بدل_شوی_معلومات = orig_بدل_شوی_معلومات;
    }
#د_که_پای
  }

  فراغ init_iters ()
  {
    iter_ننوت.init (هذا, خطأ);
    iter_context.init (هذا, ریښتیا);
  }

  فراغ سېټ_lookup_ماسک (hb_ماسک_t ماسک, منطقي init = ریښتیا) { lookup_ماسک = ماسک; وروستی_بنسټ = -1; وروستی_بنسټ_until = 0; إذا (init) init_iters (); }
  فراغ سېټ_auto_zwj (منطقي auto_zwj_, منطقي init = ریښتیا) { auto_zwj = auto_zwj_; إذا (init) init_iters (); }
  فراغ سېټ_auto_zwnj (منطقي auto_zwnj_, منطقي init = ریښتیا) { auto_zwnj = auto_zwnj_; إذا (init) init_iters (); }
  فراغ سېټ_فی_syllable (منطقي فی_syllable_2, منطقي init = ریښتیا) { فی_syllable = فی_syllable_2; إذا (init) init_iters (); }
  فراغ سېټ_random (منطقي random_) { random = random_; }
  فراغ سېټ_recurse_func (recurse_func_t func) { recurse_func = func; }
  فراغ سېټ_lookup_شاخص (بینښه صحيح lookup_شاخص_3) { lookup_شاخص = lookup_شاخص_3; }
  فراغ سېټ_lookup_props (بینښه صحيح lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_شمېره ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    بفر->random_state = بفر->random_state * 48271 % 2147483647;
    أرجع بفر->random_state;
  }

  ساكن فراغ بفر_بدل_شوی_trampoline (hb_بفر_t *بفر HB_ناکارول_شوی, فراغ *user_معلومات)
  {
    ((hb_ot_apply_context_t *) user_معلومات)->بفر_بدل_شوی ();
  }
  فراغ بفر_بدل_شوی ()
  {
    بفر->update_digest ();
    إذا (likely (has_glyph_classes))
      لكل (بینښه i = 0; i < بفر->len; i++)
	سېټ_glyph_class_props (بفر->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  منطقي سمون_properties_mark (ثابت hb_glyph_info_t *info,
			      بینښه صحيح    glyph_props,
			      بینښه صحيح    سمون_props) ثابت
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    إذا (سمون_props & LookupFlag::Use_Mark_Filtering_سېټ)
    {
      بینښه سېټ_شاخص = سمون_props >> 16;
      أرجع gdef_accel.mark_سېټ_covers (سېټ_شاخص, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    إذا (سمون_props & LookupFlag::MarkAttachmentType)
      أرجع (سمون_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    أرجع ریښتیا;
  }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  منطقي check_glyph_property (ثابت hb_glyph_info_t *info,
			     بینښه سمون_props) ثابت
  {
    بینښه صحيح glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    إذا (glyph_props & سمون_props & LookupFlag::IgnoreFlags)
      أرجع خطأ;

    إذا (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      أرجع سمون_properties_mark (info, glyph_props, سمون_props);

    أرجع ریښتیا;
  }

  فراغ سېټ_glyph_class (hb_codepoint_t glyph_شاخص,
			  بینښه صحيح class_guess = 0,
			  منطقي ligature = خطأ,
			  منطقي component = خطأ)
  {
    بفر->digest.جمع_کړه (glyph_شاخص);

    إذا (new_syllables != (بینښه) -1)
      بفر->cur().syllable() = new_syllables;

    بینښه صحيح props = _hb_glyph_info_get_glyph_props (&بفر->cur());
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
      سېټ_glyph_class_props (بفر->cur(), props, glyph_شاخص);
    }
    وإلا إذا (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      hb_glyph_info_سېټ_glyph_props (&بفر->cur(), props | class_guess);
    }
    وإلا
      hb_glyph_info_سېټ_glyph_props (&بفر->cur(), props);
  }
  فراغ سېټ_glyph_class_props (hb_glyph_info_t &info,
			       بینښه صحيح props,
			       hb_codepoint_t glyph_شاخص)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    hb_glyph_info_سېټ_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_شاخص));
  }
  فراغ سېټ_glyph_class_props (hb_glyph_info_t &info)
  {
    سېټ_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  فراغ replace_glyph (hb_codepoint_t glyph_شاخص)
  {
    سېټ_glyph_class (glyph_شاخص);
    (فراغ) بفر->replace_glyph (glyph_شاخص);
  }
  فراغ replace_glyph_inplace (hb_codepoint_t glyph_شاخص)
  {
    سېټ_glyph_class (glyph_شاخص);
    بفر->cur().codepoint = glyph_شاخص;
  }
  فراغ replace_glyph_with_ligature (hb_codepoint_t glyph_شاخص,
				    بینښه صحيح class_guess)
  {
    سېټ_glyph_class (glyph_شاخص, class_guess, ریښتیا);
    (فراغ) بفر->replace_glyph (glyph_شاخص);
  }
  فراغ output_glyph_for_component (hb_codepoint_t glyph_شاخص,
				   بینښه صحيح class_guess)
  {
    سېټ_glyph_class (glyph_شاخص, class_guess, خطأ, ریښتیا);
    (فراغ) بفر->output_glyph (glyph_شاخص);
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
  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری collect_دوهم_glyphs_2 (ثابت T &obj,
					     hb_سېټ_digest_t *digest,
					     hb_لومړیتوب<1>) HB_RETURN (فراغ, obj.collect_دوهم_glyphs (digest) )
  کينډۍ <ډول_نوم T>
  ساكن مضمن فراغ collect_دوهم_glyphs_2 (ثابت T &obj HB_ناکارول_شوی,
					     hb_سېټ_digest_t *digest,
					     hb_لومړیتوب<0>)
  { *digest = hb_سېټ_digest_t::ډک (); }

  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_لومړیتوب<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_لومړیتوب<0>) HB_RETURN (منطقي, obj->apply (c) )
  کينډۍ <ډول_نوم T>
  ساكن مضمن منطقي apply_منزل (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_لومړیتوب<2>) HB_RETURN (منطقي, obj->apply_cached (c, external_cache) )
  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_لومړیتوب<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_لومړیتوب<0>) HB_RETURN (منطقي, obj->apply (c) )
  کينډۍ <ډول_نوم T>
  ساكن مضمن منطقي apply_cached_منزل (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  کينډۍ <ډول_نوم T>
  ساكن مضمن خپلکاری cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_لومړیتوب<1>) HB_RETURN (منطقي, T::cache_func (c, op) )
  کينډۍ <ډول_نوم T=فراغ>
  ساكن مضمن منطقي cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_ناکارول_شوی,
				  hb_لومړیتوب<0>) { أرجع خطأ; }
  کينډۍ <ډول_نوم Type>
  ساكن مضمن منطقي cache_func_منزل (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    أرجع cache_func_<Type> (c, op, hb_prioritize);
  }
#د_که_پای

  تعريفنوع منطقي (*hb_apply_func_t) (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache);
  تعريفنوع منطقي (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  بنية hb_applicable_t
  {
    ملګری بنية hb_accelerate_subtables_context_t;
    ملګری بنية hb_ot_layout_lookup_accelerator_t;

    کينډۍ <ډول_نوم T>
    فراغ init (ثابت T &obj_,
	       hb_apply_func_t apply_func_
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , فراغ *external_cache_
#د_که_پای
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#د_که_پای
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#که_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    منطقي apply (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->بفر->cur().codepoint) && apply_func (obj, c, تش_اشاره);
    }
    منطقي apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      أرجع apply_func (obj, c, تش_اشاره);
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
#د_که_پای

    خاص:
    ثابت فراغ *obj;
    hb_apply_func_t apply_func;
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    فراغ *external_cache;
#د_که_پای
    hb_سېټ_digest_t digest;
  };

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
  کينډۍ <ډول_نوم T>
  خپلکاری cache_لګښت (ثابت T &obj, hb_لومړیتوب<1>) HB_AUTO_RETURN ( obj.cache_لګښت () )
  کينډۍ <ډول_نوم T>
  خپلکاری cache_لګښت (ثابت T &obj, hb_لومړیتوب<0>) HB_AUTO_RETURN ( 0u )

  کينډۍ <ډول_نوم T>
  خپلکاری external_cache_create (ثابت T &obj, hb_لومړیتوب<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  کينډۍ <ډول_نوم T>
  خپلکاری external_cache_create (ثابت T &obj, hb_لومړیتوب<0>) HB_AUTO_RETURN ( تش_اشاره )
#د_که_پای

  /* Dispatch interface. */
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj)
  {
    collect_دوهم_glyphs_2 (obj, &digest_دوهم, hb_prioritize);

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    فراغ *external_cache = تش_اشاره;
    إذا (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#د_که_پای

    hb_applicable_t *ثبت = &array[i++];

    ثبت->init (obj,
		 apply_منزل<T>
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_منزل<T>
		 , cache_func_منزل<T>
		 , external_cache
#د_که_پای
		 );

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    بینښه لګښت = cache_لګښت (obj, hb_prioritize);
    إذا (لګښت > subtable_cache_user_لګښت)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_لګښت = لګښت;
    }
#د_که_پای

    أرجع hb_تشه_t ();
  }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  بینښه i = 0;
  hb_سېټ_digest_t digest_دوهم;

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
  بینښه subtable_cache_user_idx = (بینښه) -1;
  بینښه subtable_cache_user_لګښت = 0;
#د_که_پای
};


تعريفنوع منطقي (*intersects_func_t) (ثابت hb_سېټ_t *glyphs, بینښه value, ثابت فراغ *معلومات, فراغ *cache);
تعريفنوع فراغ (*intersected_glyphs_func_t) (ثابت hb_سېټ_t *glyphs, ثابت فراغ *معلومات, بینښه value, hb_سېټ_t *intersected_glyphs, فراغ *cache);
تعريفنوع فراغ (*collect_glyphs_func_t) (hb_سېټ_t *glyphs, بینښه value, ثابت فراغ *معلومات);
تعريفنوع منطقي (*سمون_func_t) (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات);
تعريفنوع بینښه (*get_ارزښت_func_t) (hb_glyph_info_t &info, ثابت فراغ *معلومات);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
بنية hb_ot_layout_ruleset_digest_t
{
  فراغ init () { ماسک = 0; }
  فراغ سېټ_ډک () { ماسک = (uint64_t) -1; }
  فراغ جمع_کړه (بینښه value) { ماسک |= (uint64_t) 1 << (value & 63); }
  منطقي may_have (بینښه value) ثابت { أرجع ماسک & ((uint64_t) 1 << (value & 63)); }

  uint64_t ماسک;
};

بنية Context_تړون_Funcs
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
  سمون_func_t سمون;
};
بنية ChainContextApplyFuncs
{
  سمون_func_t سمون[3];
};


ساكن مضمن منطقي intersects_glyph (ثابت hb_سېټ_t *glyphs, بینښه value, ثابت فراغ *معلومات HB_ناکارول_شوی, فراغ *cache HB_ناکارول_شوی)
{
  أرجع glyphs->has (value);
}
ساكن مضمن منطقي intersects_class (ثابت hb_سېټ_t *glyphs, بینښه value, ثابت فراغ *معلومات, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  إذا (map->has (value, &cached_v))
    أرجع *cached_v;

  منطقي v = class_def.intersects_class (glyphs, value);
  map->سېټ (value, v);

  أرجع v;
}
ساكن مضمن منطقي intersects_coverage (ثابت hb_سېټ_t *glyphs, بینښه value, ثابت فراغ *معلومات, فراغ *cache HB_ناکارول_شوی)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  أرجع (معلومات+coverage).intersects (glyphs);
}


ساكن مضمن فراغ intersected_glyph (ثابت hb_سېټ_t *glyphs HB_ناکارول_شوی, ثابت فراغ *معلومات, بینښه value, hb_سېټ_t *intersected_glyphs, HB_ناکارول_شوی فراغ *cache)
{
  بینښه g = reinterpret_cast<ثابت HBUINT16 *>(معلومات)[value];
  intersected_glyphs->جمع_کړه (g);
}

کارول intersected_class_cache_t = hb_hashmap_t<بینښه, hb_سېټ_t>;

ساكن مضمن فراغ intersected_class_glyphs (ثابت hb_سېټ_t *glyphs, ثابت فراغ *معلومات, بینښه value, hb_سېټ_t *intersected_glyphs, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_سېټ_t *cached_v;
  إذا (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    أرجع;
  }

  hb_سېټ_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->سېټ (value, std::move (v));
}

ساكن مضمن فراغ intersected_coverage_glyphs (ثابت hb_سېټ_t *glyphs, ثابت فراغ *معلومات, بینښه value, hb_سېټ_t *intersected_glyphs, HB_ناکارول_شوی فراغ *cache)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  (معلومات+coverage).intersect_سېټ (*glyphs, *intersected_glyphs);
}


ساكن مضمن منطقي array_is_subset_د (ثابت hb_سېټ_t *glyphs,
				       بینښه صحيح شمېر,
				       glyph_معلومات_t values,
				       intersects_func_t intersects_func,
				       ثابت فراغ *intersects_معلومات,
				       فراغ *cache)
{
  لكل (بینښه i = 0; i < شمېر; i++)
    إذا (!intersects_func (glyphs, values[i], intersects_معلومات, cache)) أرجع خطأ;
  أرجع ریښتیا;
}


ساكن مضمن فراغ collect_glyph (hb_سېټ_t *glyphs, بینښه value, ثابت فراغ *معلومات HB_ناکارول_شوی)
{
  glyphs->جمع_کړه (value);
}
ساكن مضمن فراغ collect_class (hb_سېټ_t *glyphs, بینښه value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  class_def.collect_class (glyphs, value);
}
ساكن مضمن فراغ collect_coverage (hb_سېټ_t *glyphs, بینښه value, ثابت فراغ *معلومات)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  (معلومات+coverage).collect_coverage (glyphs);
}

مضمن ثابت hb_سېټ_t *
hb_depend_context_t::پیدا_کړه_or_create_glyph_سېټ (ثابت فراغ *معلومات,
                                                بینښه value,
                                                glyph_سېټ_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_سېټ_cache_کلی_t key (معلومات, value, type);
  hb_codepoint_t *existing = تش_اشاره;
  إذا (glyph_سېټ_منزل_شاخص.has (key, &existing))
    أرجع glyph_sets[*existing].get ();

  hb_سېټ_t *سېټ = hb_object_create<hb_سېټ_t> ();
  إذا (unlikely (!سېټ))
  {
    depend_معلومات->fail ();
    أرجع تش_اشاره;
  }

  إذا (type == GLYPH_سېټ_COVERAGE)
    collect_coverage (سېټ, value, معلومات);
  وإلا إذا (type == GLYPH_سېټ_CLASS)
    collect_class (سېټ, value, معلومات);
  وإلا
    سېټ->جمع_کړه (value);
  إذا (unlikely (سېټ->دننه_error ()))
  {
    hb_سېټ_destroy (سېټ);
    depend_معلومات->fail ();
    أرجع تش_اشاره;
  }

  hb_codepoint_t شاخص_2 = glyph_sets.اوږدتیا;
  إذا (unlikely (!glyph_sets.ټېل_or_fail (hb::unique_ptr<hb_سېټ_t> {سېټ})))
  {
    depend_معلومات->fail ();
    أرجع تش_اشاره;
  }
  إذا (unlikely (!glyph_سېټ_منزل_شاخص.سېټ (key, شاخص_2)))
  {
    depend_معلومات->fail ();
    أرجع تش_اشاره;
  }
  أرجع glyph_sets[شاخص_2].get ();
}

مضمن hb_codepoint_t
hb_depend_context_t::پیدا_کړه_or_create_cached_context_سېټ (ثابت hb_سېټ_t *سېټ)
{
  uintptr_t key = (uintptr_t) سېټ;
  hb_codepoint_t *existing = تش_اشاره;
  إذا (cached_context_سېټ_indices.has (key, &existing))
    أرجع *existing;

  hb_codepoint_t شاخص_2 = depend_معلومات->پیدا_کړه_or_create_context_سېټ (*سېټ);
  إذا (unlikely (شاخص_2 == HB_CODEPOINT_INVALID))
    أرجع HB_CODEPOINT_INVALID;
  إذا (unlikely (!cached_context_سېټ_indices.سېټ (key, شاخص_2)))
    أرجع depend_معلومات->fail_invalid ();
  أرجع شاخص_2;
}

ساكن مضمن فراغ collect_array (hb_collect_glyphs_context_t *c HB_ناکارول_شوی,
				  hb_سېټ_t *glyphs,
				  بینښه صحيح شمېر,
				  glyph_معلومات_t values,
				  collect_glyphs_func_t collect_func,
				  ثابت فراغ *collect_معلومات)
{
  لكل (بینښه i = 0; i < شمېر; i++)
    collect_func (glyphs, values[i], collect_معلومات);
}


ساكن مضمن منطقي سمون_always (hb_glyph_info_t &info HB_ناکارول_شوی, بینښه value HB_ناکارول_شوی, ثابت فراغ *معلومات HB_ناکارول_شوی)
{
  أرجع ریښتیا;
}
ساكن مضمن منطقي سمون_glyph (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات HB_ناکارول_شوی)
{
  أرجع info.codepoint == value;
}
ساكن مضمن منطقي سمون_class (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  أرجع class_def.get_class (info.codepoint) == value;
}
ساكن مضمن بینښه get_class_ارزښت (hb_glyph_info_t &info, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (معلومات);
  أرجع class_def.get_class (info.codepoint);
}
بنية سمون_class_cache_معلومات_t
{
  ثابت ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ساكن مضمن منطقي سمون_class_cache (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات)
{
  ثابت خپلکاری &cache_معلومات = *reinterpret_cast<ثابت سمون_class_cache_معلومات_t *> (معلومات);
  أرجع cache_معلومات.class_def->get_class (info.codepoint, cache_معلومات.cache) == value;
}
ساكن مضمن بینښه get_class_cache_ارزښت (hb_glyph_info_t &info, ثابت فراغ *معلومات)
{
  ثابت خپلکاری &cache_معلومات = *reinterpret_cast<ثابت سمون_class_cache_معلومات_t *> (معلومات);
  أرجع cache_معلومات.class_def->get_class (info.codepoint, cache_معلومات.cache);
}
ساكن مضمن بینښه get_class_cached (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بینښه klass = info.syllable();
  إذا (klass < 255)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 255))
    info.syllable() = klass;
  أرجع klass;
}
ساكن مضمن منطقي سمون_class_cached (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  أرجع get_class_cached (class_def, info) == value;
}
ساكن مضمن بینښه get_class_cached_ارزښت (hb_glyph_info_t &info, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (معلومات);
  أرجع get_class_cached (class_def, info);
}
ساكن مضمن بینښه get_class_cached1 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بینښه klass = info.syllable() & 0x0F;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  أرجع klass;
}
ساكن مضمن منطقي سمون_class_cached1 (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  أرجع get_class_cached1 (class_def, info) == value;
}
ساكن مضمن بینښه get_class_cached2 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  بینښه klass = (info.syllable() & 0xF0) >> 4;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  أرجع klass;
}
ساكن مضمن منطقي سمون_class_cached2 (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(معلومات);
  أرجع get_class_cached2 (class_def, info) == value;
}
ساكن مضمن بینښه get_class_cached2_ارزښت (hb_glyph_info_t &info, ثابت فراغ *معلومات)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (معلومات);
  أرجع get_class_cached2 (class_def, info);
}
ساكن مضمن منطقي سمون_coverage (hb_glyph_info_t &info, بینښه value, ثابت فراغ *معلومات)
{
  Offset32_منزل<Coverage> coverage;
  coverage = value;
  أرجع (معلومات+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ساكن مضمن منطقي would_سمون_ننوت (hb_would_apply_context_t *c,
				      بینښه صحيح شمېر, /* Including the first glyph (not matched) */
				      glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
				      سمون_func_t سمون_func_2,
				      ثابت فراغ *سمون_معلومات_2)
{
  إذا (شمېر != c->len)
    أرجع خطأ;

  لكل (بینښه صحيح i = 1; i < شمېر; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    إذا (likely (!سمون_func_2 (info, ننوت[i - 1], سمون_معلومات_2)))
      أرجع خطأ;
  }

  أرجع ریښتیا;
}
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
HB_ALWAYS_INLINE
#وإلا
مضمن
#د_که_پای
ساكن منطقي سمون_ننوت (hb_ot_apply_context_t *c,
			 بینښه صحيح شمېر, /* Including the first glyph (not matched) */
			 glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
			 سمون_func_t سمون_func_2,
			 ثابت فراغ *سمون_معلومات_2,
			 بینښه صحيح *پای_ټکی_position,
			 بینښه صحيح *p_ټول_component_شمېر = تش_اشاره)
{
  TRACE_APPLY (تش_اشاره);

  hb_بفر_t *بفر = c->بفر;

  إذا (شمېر == 1)
  {
    *پای_ټکی_position = بفر->idx + 1;
    c->سمون_positions[0] = بفر->idx;
    إذا (p_ټول_component_شمېر)
      *p_ټول_component_شمېر = hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
    return_trace (ریښتیا);
  }

  إذا (unlikely (شمېر > HB_MAX_CONTEXT_اوږدتیا)) return_trace (خطأ);

  خپلکاری &skippy_iter = c->iter_ننوت;
  skippy_iter.reset (بفر->idx);
  skippy_iter.سېټ_سمون_func (سمون_func_2, سمون_معلومات_2);
  skippy_iter.سېټ_glyph_معلومات (ننوت);

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

  بینښه صحيح ټول_component_شمېر = 0;

  بینښه صحيح لومړی_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
  بینښه صحيح لومړی_lig_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());

  تعداد {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  لكل (بینښه صحيح i = 1; i < شمېر; i++)
  {
    بینښه unsafe_منزل;
    إذا (!skippy_iter.next (&unsafe_منزل))
    {
      *پای_ټکی_position = unsafe_منزل;
      return_trace (خطأ);
    }

    إذا (unlikely (i + 1 > c->سمون_positions.اوږدتیا &&
		  !c->سمون_positions.resize_dirty  (i + 1)))
      return_trace (خطأ);
    c->سمون_positions.arrayZ[i] = skippy_iter.idx;

    بینښه صحيح this_lig_id = _hb_glyph_info_get_lig_id (&بفر->info[skippy_iter.idx]);
    بینښه صحيح this_lig_comp = _hb_glyph_info_get_lig_comp (&بفر->info[skippy_iter.idx]);

    إذا (لومړی_lig_id && لومړی_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      إذا (لومړی_lig_id != this_lig_id || لومړی_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	إذا (ligbase == LIGBASE_NOT_CHECKED)
	{
	  منطقي وموندل = خطأ;
	  ثابت خپلکاری *بهر = بفر->بهر_info;
	  بینښه صحيح j = بفر->بهر_len;
	  طالما (j && _hb_glyph_info_get_lig_id (&بهر[j - 1]) == لومړی_lig_id)
	  {
	    إذا (_hb_glyph_info_get_lig_comp (&بهر[j - 1]) == 0)
	    {
	      j--;
	      وموندل = ریښتیا;
	      اكسر;
	    }
	    j--;
	  }

	  إذا (وموندل && skippy_iter.may_skip (بهر[j]) == matcher_t::SKIP_YES)
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
      إذا (this_lig_id && this_lig_comp && (this_lig_id != لومړی_lig_id))
	return_trace (خطأ);
    }

    ټول_component_شمېر += hb_glyph_info_get_lig_عدد_comps_دننه_ligation (&بفر->info[skippy_iter.idx]);
  }

  *پای_ټکی_position = skippy_iter.idx + 1;

  إذا (p_ټول_component_شمېر)
  {
    ټول_component_شمېر += hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
    *p_ټول_component_شمېر = ټول_component_شمېر;
  }

  c->سمون_positions.arrayZ[0] = بفر->idx;

  return_trace (ریښتیا);
}
ساكن مضمن منطقي ligate_ننوت (hb_ot_apply_context_t *c,
				 بینښه صحيح شمېر, /* Including the first glyph */
				 بینښه صحيح سمون_پای_ټکی,
				 hb_codepoint_t lig_glyph,
				 بینښه صحيح ټول_component_شمېر)
{
  TRACE_APPLY (تش_اشاره);

  hb_بفر_t *بفر = c->بفر;

  بفر->یوځای_clusters (بفر->idx, سمون_پای_ټکی);

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

  منطقي is_بنسټ_ligature = hb_glyph_info_is_بنسټ_glyph (&بفر->info[c->سمون_positions.arrayZ[0]]);
  منطقي is_mark_ligature = _hb_glyph_info_is_mark (&بفر->info[c->سمون_positions.arrayZ[0]]);
  لكل (بینښه صحيح i = 1; i < شمېر; i++)
    إذا (!_hb_glyph_info_is_mark (&بفر->info[c->سمون_positions.arrayZ[i]]))
    {
      is_بنسټ_ligature = خطأ;
      is_mark_ligature = خطأ;
      اكسر;
    }
  منطقي is_ligature = !is_بنسټ_ligature && !is_mark_ligature;

  بینښه صحيح klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  بینښه صحيح lig_id = is_ligature ? _hb_allocate_lig_id (بفر) : 0;
  بینښه صحيح وروستی_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
  بینښه صحيح وروستی_عدد_components = hb_glyph_info_get_lig_عدد_comps (&بفر->cur());
  بینښه صحيح components_so_far = وروستی_عدد_components;

  إذا (is_ligature)
  {
    hb_glyph_info_سېټ_lig_props_for_ligature (&بفر->cur(), lig_id, ټول_component_شمېر);
    إذا (_hb_glyph_info_get_general_category (&بفر->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      hb_glyph_info_سېټ_general_category (&بفر->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  لكل (بینښه صحيح i = 1; i < شمېر; i++)
  {
    طالما (بفر->idx < c->سمون_positions.arrayZ[i] && بفر->successful)
    {
      إذا (is_ligature)
      {
	بینښه صحيح this_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());
	إذا (this_comp == 0)
	  this_comp = وروستی_عدد_components;
	أكد (components_so_far >= وروستی_عدد_components);
	بینښه صحيح new_lig_comp = components_so_far - وروستی_عدد_components +
				    hb_min (this_comp, وروستی_عدد_components);
	  hb_glyph_info_سېټ_lig_props_for_mark (&بفر->cur(), lig_id, new_lig_comp);
      }
      (فراغ) بفر->بل_glyph ();
    }

    وروستی_lig_id = _hb_glyph_info_get_lig_id (&بفر->cur());
    وروستی_عدد_components = hb_glyph_info_get_lig_عدد_comps_دننه_ligation (&بفر->cur());
    components_so_far += وروستی_عدد_components;

    /* Skip the base glyph */
    بفر->idx++;
  }

  إذا (!is_mark_ligature && وروستی_lig_id)
  {
    /* Re-adjust components for any marks following. */
    لكل (بینښه i = بفر->idx; i < بفر->len; ++i)
    {
      إذا (وروستی_lig_id != _hb_glyph_info_get_lig_id (&بفر->info[i])) اكسر;

      بینښه this_comp = _hb_glyph_info_get_lig_comp (&بفر->info[i]);
      إذا (!this_comp) اكسر;

      أكد (components_so_far >= وروستی_عدد_components);
      بینښه new_lig_comp = components_so_far - وروستی_عدد_components +
			      hb_min (this_comp, وروستی_عدد_components);
      hb_glyph_info_سېټ_lig_props_for_mark (&بفر->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (ریښتیا);
}

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
HB_ALWAYS_INLINE
#وإلا
مضمن
#د_که_پای
ساكن منطقي سمون_backtrack (hb_ot_apply_context_t *c,
			     بینښه صحيح شمېر,
			     glyph_معلومات_t backtrack,
			     سمون_func_t سمون_func_2,
			     ثابت فراغ *سمون_معلومات_2,
			     بینښه صحيح *سمون_پیل)
{
  TRACE_APPLY (تش_اشاره);

  إذا (!شمېر)
  {
    *سمون_پیل = c->بفر->backtrack_len ();
    return_trace (ریښتیا);
  }

  خپلکاری &skippy_iter = c->iter_context;
  skippy_iter.reset_شا (c->بفر->backtrack_len ());
  skippy_iter.سېټ_سمون_func (سمون_func_2, سمون_معلومات_2);
  skippy_iter.سېټ_glyph_معلومات (backtrack);

  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    بینښه unsafe_from;
    إذا (!skippy_iter.prev (&unsafe_from))
    {
      *سمون_پیل = unsafe_from;
      return_trace (خطأ);
    }
  }

  *سمون_پیل = skippy_iter.idx;
  return_trace (ریښتیا);
}

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
HB_ALWAYS_INLINE
#وإلا
مضمن
#د_که_پای
ساكن منطقي سمون_lookahead (hb_ot_apply_context_t *c,
			     بینښه صحيح شمېر,
			     glyph_معلومات_t lookahead,
			     سمون_func_t سمون_func_2,
			     ثابت فراغ *سمون_معلومات_2,
			     بینښه صحيح پیل_شاخص,
			     بینښه صحيح *پای_ټکی_شاخص)
{
  TRACE_APPLY (تش_اشاره);

  إذا (!شمېر)
  {
    *پای_ټکی_شاخص = پیل_شاخص;
    return_trace (ریښتیا);
  }

  خپلکاری &skippy_iter = c->iter_context;
  أكد (پیل_شاخص >= 1);
  skippy_iter.reset (پیل_شاخص - 1);
  skippy_iter.سېټ_سمون_func (سمون_func_2, سمون_معلومات_2);
  skippy_iter.سېټ_glyph_معلومات (lookahead);

  لكل (بینښه صحيح i = 0; i < شمېر; i++)
  {
    بینښه unsafe_منزل;
    إذا (!skippy_iter.next (&unsafe_منزل))
    {
      *پای_ټکی_شاخص = unsafe_منزل;
      return_trace (خطأ);
    }
  }

  *پای_ټکی_شاخص = skippy_iter.idx + 1;
  return_trace (ریښتیا);
}



بنية Lookup_ریکارډ
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t         *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->embed (*هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    return_trace (c->check_ټاکل (بهر->lookup_List_شاخص, lookup_map->get (lookup_List_شاخص), HB_SERIALIZE_ERROR_INT_ډکېدنه));
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

ساكن مضمن بینښه serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ثابت hb_array_t<ثابت Lookup_ریکارډ> lookup_ریکارډونه,
						     ثابت hb_map_t *lookup_map)
{
  بینښه شمېر = 0;
  لكل (ثابت Lookup_ریکارډ& r : lookup_ریکارډونه)
  {
    إذا (!lookup_map->has (r.lookup_List_شاخص))
      استمر;

    إذا (!r.serialize (c, lookup_map))
      أرجع 0;

    شمېر++;
  }
  أرجع شمېر;
}

تعداد ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ساكن مضمن ثابت hb_سېټ_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ثابت فراغ *معلومات,
			بینښه value,
			منطقي filter_class_by_پلار,
			hb_سېټ_t *scratch,
			hb_ویکټور_t<بینښه> *compact_workspace = تش_اشاره)
{
  بدل (context_format)
  {
    حالة ContextFormat::SimpleContext:
      أرجع c->پیدا_کړه_or_create_glyph_سېټ (تش_اشاره, value,
                                           hb_depend_context_t::GLYPH_سېټ_SINGLE);
    حالة ContextFormat::ClassBasedContext:
    {
      ثابت hb_سېټ_t *class_سېټ = c->پیدا_کړه_or_create_glyph_سېټ (
        معلومات, value, hb_depend_context_t::GLYPH_سېټ_CLASS);
      إذا (unlikely (!class_سېټ))
	أرجع تش_اشاره;
      إذا (!filter_class_by_پلار)
	أرجع class_سېټ;
      scratch->سېټ (*class_سېټ);
      scratch->intersect (c->پلار_فعال_glyphs (), compact_workspace);
      اكسر;
    }
    حالة ContextFormat::CoverageBasedContext:
      أرجع c->پیدا_کړه_or_create_glyph_سېټ (
        معلومات, value, hb_depend_context_t::GLYPH_سېټ_COVERAGE);
  }

  إذا (unlikely (scratch->دننه_error ()))
  {
    c->depend_معلومات->fail ();
    أرجع تش_اشاره;
  }
  أرجع scratch;
}

ساكن مضمن فراغ context_تړون_recurse_lookups (hb_تړون_context_t *c,
					     بینښه ننوت_شمېر, glyph_معلومات_t ننوت,
					     بینښه lookup_شمېر_2,
					     ثابت Lookup_ریکارډ lookup_ریکارډ[] /* Array of LookupRecords--in design order */,
					     بینښه value,
					     ContextFormat context_format,
					     ثابت فراغ *معلومات,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache)
{
  hb_سېټ_t covered_seq_indicies;
  hb_سېټ_t pos_glyphs;
  لكل (بینښه صحيح i = 0; i < lookup_شمېر_2; i++)
  {
    بینښه seq_شاخص_2 = lookup_ریکارډ[i].sequence_شاخص;
    إذا (seq_شاخص_2 >= ننوت_شمېر) استمر;

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indicies.has (seq_شاخص_2))
    {
      has_pos_glyphs = ریښتیا;
      pos_glyphs.clear ();
      إذا (seq_شاخص_2 == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.جمع_کړه (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->پلار_فعال_glyphs (), معلومات, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.سېټ (c->پلار_فعال_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *ننوت_معلومات = ننوت.معلومات;
        بینښه ننوت_ارزښت = seq_شاخص_2 - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          ننوت_معلومات = معلومات;
          ننوت_ارزښت = ننوت[seq_شاخص_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, ننوت_معلومات, ننوت_ارزښت, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.جمع_کړه (seq_شاخص_2);
    hb_سېټ_t *cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs))
      أرجع;
    إذا (has_pos_glyphs) {
      *cur_فعال_glyphs = std::move (pos_glyphs);
    } وإلا {
      *cur_فعال_glyphs = *c->glyphs;
    }

    بینښه پای_ټکی_شاخص_2 = ننوت_شمېر;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      پای_ټکی_شاخص_2 += 1;

    c->recurse (lookup_ریکارډ[i].lookup_List_شاخص, &covered_seq_indicies, seq_شاخص_2, پای_ټکی_شاخص_2);

    c->راوباسه_cur_بشپړ_glyphs ();
  }
}

ساكن منطقي
context_depend_sets_are_cached (hb_depend_context_t *c,
				بینښه ننوت_شمېر_2,
				بینښه lookup_شمېر,
				ثابت Lookup_ریکارډ lookup_ریکارډونه_2[],
				ثابت فراغ *قاعده_2,
				بینښه value,
				ContextFormat context_format,
				ثابت فراغ *معلومات,
				hb_ویکټور_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
  منطقي have_فعال_idx = خطأ;
  لكل (بینښه i = 0; i < lookup_شمېر; i++)
  {
    إذا (lookup_ریکارډونه_2[i].sequence_شاخص >= ننوت_شمېر_2)
      استمر;

    إذا (!have_فعال_idx)
    {
      إذا (unlikely (!c->get_پلار_فعال_glyphs_شاخص (&فعال_idx)))
	أرجع خطأ;
      have_فعال_idx = ریښتیا;
    }

    hb_depend_context_t::context_سېټ_cache_کلی_t key (
      قاعده_2, &lookup_ریکارډونه_2[i], معلومات, فعال_idx, value, context_format);
    hb_codepoint_t *cached_context_سېټ = تش_اشاره;
    إذا (!c->context_سېټ_cache.has (key, &cached_context_سېټ))
      أرجع خطأ;
    إذا (unlikely (!cached_context_sets->ټېل_or_fail (*cached_context_سېټ)))
      أرجع c->depend_معلومات->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  أرجع have_فعال_idx;
}

ساكن مضمن فراغ context_depend_recurse_lookups (hb_depend_context_t *c,
					     بینښه ننوت_شمېر, glyph_معلومات_t ننوت,
					     بینښه lookup_شمېر_2,
					     ثابت Lookup_ریکارډ lookup_ریکارډ[] /* Array of LookupRecords--in design order */,
					     ثابت فراغ *قاعده_2,
					     بینښه value,
					     ContextFormat context_format,
					     ثابت فراغ *معلومات,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache,
					     ثابت hb_سېټ_t &preliminary_context,
					     ثابت hb_ویکټور_t<ثابت hb_سېټ_t *> *ننوت_position_glyphs,
					     ثابت hb_ویکټور_t<hb_codepoint_t> *cached_context_sets,
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
  إذا (unlikely (ننوت_شمېر > HB_MAX_CONTEXT_اوږدتیا))
    أرجع;

  hb_بټ_پاڼه_t covered_seq_indices;
  hb_سېټ_t &pos_glyphs = scratch->pos_glyphs;
  hb_سېټ_t &position_context = scratch->position_context;
  hb_سېټ_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_سېټ_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  بینښه cached_context_شاخص = 0;
  لكل (بینښه صحيح i = 0; i < lookup_شمېر_2; i++)
  {
    بینښه seq_شاخص_2 = lookup_ریکارډ[i].sequence_شاخص;
    إذا (seq_شاخص_2 >= ننوت_شمېر)
      استمر;

    hb_depend_context_t::context_سېټ_cache_کلی_t context_cache_کلی;
    إذا (unlikely (!c->get_context_سېټ_cache_کلی (قاعده_2, &lookup_ریکارډ[i], معلومات, value,
						 context_format, &context_cache_کلی)))
      أرجع;

    hb_codepoint_t context_سېټ_idx;
    hb_codepoint_t *cached_context_سېټ_idx;
    إذا (cached_context_sets)
      context_سېټ_idx = (*cached_context_sets)[cached_context_شاخص++];
    وإلا إذا (c->context_سېټ_cache.has (context_cache_کلی, &cached_context_سېټ_idx))
      context_سېټ_idx = *cached_context_سېټ_idx;
    وإلا
    {
      إذا (unlikely (!ننوت_position_glyphs))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_سېټ_ارزښت_INVALID;
      طالما (preliminary_context.next (&elem)) {
	إذا (!(elem & HB_DEPEND_CONTEXT_سېټ_FLAG)) {
	  /* Direct glyph */
	  position_context.جمع_کړه (elem);
	} وإلا {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.جمع_کړه (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      لكل (بینښه j = 0; j < ننوت_position_glyphs->اوږدتیا; j++) {
	إذا (j != seq_شاخص_2 && (*ننوت_position_glyphs)[j]->get_population () == 1) {
	  position_context.جمع_کړه ((*ننوت_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_سېټ_ارزښت_INVALID;
      طالما (disjunctive_indices.next (&elem)) {
	hb_codepoint_t سېټ_idx = elem & 0x7FFFFFFF;
	ثابت hb_سېټ_t *original_سېټ = c->depend_معلومات->get_سېټ_from_شاخص (سېټ_idx);
	إذا (unlikely (!original_سېټ)) استمر;

	/* Dependency context sets are never inverted. */
	إذا (!original_سېټ->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.جمع_کړه (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      لكل (بینښه j = 0; j < ننوت_position_glyphs->اوږدتیا; j++) {
	إذا (j != seq_شاخص_2 && (*ننوت_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  إذا (!(*ننوت_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    منطقي cached_سېټ = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_سېټ
	      ? c->پیدا_کړه_or_create_cached_context_سېټ ((*ننوت_position_glyphs)[j])
	      : c->depend_معلومات->پیدا_کړه_or_create_context_سېټ (*(*ننوت_position_glyphs)[j]);
	    إذا (unlikely (idx == HB_CODEPOINT_INVALID))
	      أرجع;
	    filtered_disjunctive_indices.جمع_کړه (HB_DEPEND_CONTEXT_سېټ_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      إذا (unlikely (position_context.دننه_error () ||
		    filtered_disjunctive_indices.دننه_error ()))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }

      context_سېټ_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_معلومات->پیدا_کړه_or_create_context_سېټ (position_context);
      إذا (unlikely (!position_context.is_empty () &&
		    context_سېټ_idx == HB_CODEPOINT_INVALID))
	أرجع;

      إذا (unlikely (!c->context_سېټ_cache.سېټ (context_cache_کلی, context_سېټ_idx)))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_سېټ_شاخص = c->depend_معلومات->current_context_سېټ_شاخص;
    hb_subset_depend_څنډه_flags_t saved_څنډه_flags = c->depend_معلومات->current_څنډه_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    منطقي nested_contextual = (saved_context_سېټ_شاخص != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_معلومات->current_context_سېټ_شاخص = context_سېټ_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_معلومات->current_څنډه_flags = HB_SUBSET_DEPEND_څنډه_FLAG_NONE;
    إذا (ننوت_شمېر > 1) {
      c->depend_معلومات->current_څنډه_flags |= HB_SUBSET_DEPEND_څنډه_FLAG_FROM_CONTEXT_POSITION;
    }
    إذا (nested_contextual) {
      c->depend_معلومات->current_څنډه_flags |= HB_SUBSET_DEPEND_څنډه_FLAG_FROM_NESTED_CONTEXT;
    }

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indices.has (seq_شاخص_2))
    {
      has_pos_glyphs = ریښتیا;
      pos_glyphs.clear ();
      إذا (seq_شاخص_2 == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.جمع_کړه (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->پلار_فعال_glyphs (), معلومات, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.سېټ (c->پلار_فعال_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *ننوت_معلومات = ننوت.معلومات;
        بینښه ننوت_ارزښت = seq_شاخص_2 - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          ننوت_معلومات = معلومات;
          ننوت_ارزښت = ننوت[seq_شاخص_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, ننوت_معلومات, ننوت_ارزښت, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.جمع_کړه (seq_شاخص_2);
    إذا (has_pos_glyphs)
    {
      hb_codepoint_t فعال_idx = HB_CODEPOINT_INVALID;
      إذا (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_شاخص_2 ? pos_glyphs.get_min () : value;
	إذا (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_فعال_FLAG))
	  فعال_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_فعال_FLAG;
      }
      hb_سېټ_t *cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs (فعال_idx);
      إذا (unlikely (!cur_فعال_glyphs))
      {
	c->depend_معلومات->current_context_سېټ_شاخص = saved_context_سېټ_شاخص;
	c->depend_معلومات->current_څنډه_flags = saved_څنډه_flags;
	أرجع;
      }
      *cur_فعال_glyphs = std::move (pos_glyphs);
    }
    وإلا إذا (unlikely (!c->ټېل_ریښه_فعال_glyphs ()))
    {
      c->depend_معلومات->current_context_سېټ_شاخص = saved_context_سېټ_شاخص;
      c->depend_معلومات->current_څنډه_flags = saved_څنډه_flags;
      أرجع;
    }

    بینښه پای_ټکی_شاخص_2 = ننوت_شمېر;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      پای_ټکی_شاخص_2 += 1;

    c->recurse (lookup_ریکارډ[i].lookup_List_شاخص, &covered_seq_indices, seq_شاخص_2, پای_ټکی_شاخص_2);
    c->راوباسه_cur_بشپړ_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_معلومات->current_context_سېټ_شاخص = saved_context_سېټ_شاخص;
    c->depend_معلومات->current_څنډه_flags = saved_څنډه_flags;
  }
}

کينډۍ <ډول_نوم context_t>
ساكن مضمن فراغ recurse_lookups (context_t *c,
                                    بینښه صحيح lookup_شمېر_2,
                                    ثابت Lookup_ریکارډ lookup_ریکارډ[] /* Array of LookupRecords--in design order */)
{
  لكل (بینښه صحيح i = 0; i < lookup_شمېر_2; i++)
    c->recurse (lookup_ریکارډ[i].lookup_List_شاخص);
}

ساكن مضمن فراغ apply_lookup (hb_ot_apply_context_t *c,
				 بینښه صحيح شمېر, /* Including the first glyph */
				 بینښه صحيح lookup_شمېر_2,
				 ثابت Lookup_ریکارډ lookup_ریکارډ[], /* Array of LookupRecords--in design order */
				 بینښه صحيح سمون_پای_ټکی)
{
  hb_بفر_t *بفر = c->بفر;
  صحيح پای_ټکی;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    بینښه صحيح bl = بفر->backtrack_len ();
    پای_ټکی = bl + سمون_پای_ټکی - بفر->idx;

    صحيح delta = bl - بفر->idx;
    /* Convert positions to new indexing. */
    لكل (بینښه صحيح j = 0; j < شمېر; j++)
      c->سمون_positions.arrayZ[j] += delta;
  }

  لكل (بینښه صحيح i = 0; i < lookup_شمېر_2 && بفر->successful; i++)
  {
    بینښه صحيح idx = lookup_ریکارډ[i].sequence_شاخص;
    إذا (idx >= شمېر)
      استمر;

    بینښه صحيح orig_len = بفر->backtrack_len () + بفر->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    إذا (unlikely (c->سمون_positions.arrayZ[idx] >= orig_len))
      استمر;

    إذا (unlikely (!بفر->move_منزل (c->سمون_positions.arrayZ[idx])))
      اكسر;

    إذا (unlikely (بفر->max_ops <= 0))
      اكسر;

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      إذا (بفر->have_output)
        c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "start recursing to lookup %u at %u",
			  (بینښه) lookup_ریکارډ[i].lookup_List_شاخص,
			  بفر->idx);
    }

    منطقي ret = c->recurse (lookup_ریکارډ[i].lookup_List_شاخص);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      إذا (بفر->have_output)
        c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "end recursing to lookup %u",
			  (بینښه) lookup_ریکارډ[i].lookup_List_شاخص);
    }

    إذا (!ret)
      استمر;

    بینښه صحيح new_len = بفر->backtrack_len () + بفر->lookahead_len ();
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

    پای_ټکی += delta;
    إذا (پای_ټکی < صحيح (c->سمون_positions.arrayZ[idx]))
    {
      /* End might end up being smaller than match_positions.arrayZ[idx] if the recursed
       * lookup ended up removing many items.
       * Just never rewind end beyond start of current position, since that is
       * not possible in the recursed lookup.  Also adjust delta as such.
       *
       * https://bugs.chromium.org/p/chromium/issues/detail?id=659496
       * https://github.com/harfbuzz/harfbuzz/issues/1611
       */
      delta += c->سمون_positions.arrayZ[idx] - پای_ټکی;
      پای_ټکی = c->سمون_positions.arrayZ[idx];
    }

    بینښه صحيح next = idx + 1; /* next now is the position after the recursed lookup. */

    إذا (delta > 0)
    {
      إذا (unlikely (delta + شمېر > HB_MAX_CONTEXT_اوږدتیا))
	اكسر;
      إذا (unlikely (شمېر + delta > c->سمون_positions.اوږدتیا &&
		    !c->سمون_positions.resize_dirty  (شمېر + delta)))
        أرجع;
    }
    وإلا
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (صحيح) next - (صحيح) شمېر);
      next -= delta;
    }

    /* Shift! */
    memmove (c->سمون_positions + next + delta, c->سمون_positions + next,
	     (شمېر - next) * حجم (c->سمون_positions.arrayZ[0]));
    next += delta;
    شمېر += delta;

    /* Fill in new entries. */
    لكل (بینښه صحيح j = idx + 1; j < next; j++)
      c->سمون_positions.arrayZ[j] = c->سمون_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    لكل (; next < شمېر; next++)
      c->سمون_positions.arrayZ[next] += delta;
  }

  أكد (پای_ټکی >= 0);
  (فراغ) بفر->move_منزل (پای_ټکی);
}



/* Contextual lookups */

بنية Context_تړون_Lookup_Context
{
  Context_تړون_Funcs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_معلومات;
  فراغ *intersects_cache;
  فراغ *intersected_glyphs_cache;
};

تعريفنوع Context_تړون_Lookup_Context ContextDependLookupContext;

بنية ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_معلومات;
};

بنية ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ثابت فراغ *سمون_معلومات_2;
  get_ارزښت_func_t get_ارزښت;
};

ساكن مضمن منطقي context_intersects (ثابت hb_سېټ_t *glyphs,
				       بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
				       glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
				       Context_تړون_Lookup_Context &lookup_context)
{
  أرجع array_is_subset_د (glyphs,
			     ننوت_شمېر ? ننوت_شمېر - 1 : 0, ننوت,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_معلومات,
			     lookup_context.intersects_cache);
}

ساكن مضمن فراغ context_تړون_lookup (hb_تړون_context_t *c,
					   بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
					   glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
					   بینښه صحيح lookup_شمېر_2,
					   ثابت Lookup_ریکارډ lookup_ریکارډ[],
					   بینښه value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   Context_تړون_Lookup_Context &lookup_context)
{
  إذا (context_intersects (c->glyphs,
			  ننوت_شمېر, ننوت,
			  lookup_context))
    context_تړون_recurse_lookups (c,
				     ننوت_شمېر, ننوت,
				     lookup_شمېر_2, lookup_ریکارډ,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_معلومات,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ context_depend_lookup (hb_depend_context_t *c,
					  بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
					  glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
					  بینښه صحيح lookup_شمېر_2,
					  ثابت Lookup_ریکارډ lookup_ریکارډ[],
					  ثابت فراغ *قاعده_2,
					  بینښه value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  إذا (!context_intersects (c->glyphs,
			   ننوت_شمېر, ننوت,
			   lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, ننوت_شمېر, lookup_شمېر_2, lookup_ریکارډ, قاعده_2, value,
    lookup_context.context_format, lookup_context.intersects_معلومات,
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_معلومات->successful))
    أرجع;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_سېټ_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_سېټ_t &position_scratch = scratch->position_scratch;
  hb_ویکټور_t<ثابت hb_سېټ_t *> &ننوت_position_glyphs = scratch->ننوت_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  إذا (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ثابت hb_سېټ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_معلومات, value,
      ریښتیا, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !ننوت_position_glyphs.ټېل_or_fail (pos0_glyphs)))
    {
      c->depend_معلومات->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (بینښه i = 0; i + 1 < ننوت_شمېر; i++)
    {
      ثابت hb_سېټ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_معلومات,
	ننوت[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !ننوت_position_glyphs.ټېل_or_fail (pos_glyphs)))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ننوت_شمېر, ننوت,
				  lookup_شمېر_2, lookup_ریکارډ,
				  قاعده_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_معلومات,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? تش_اشاره : &ننوت_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : تش_اشاره,
				  scratch);
}

ساكن مضمن فراغ context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
						  glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
						  بینښه صحيح lookup_شمېر_2,
						  ثابت Lookup_ریکارډ lookup_ریکارډ[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->ننوت,
		 ننوت_شمېر ? ننوت_شمېر - 1 : 0, ننوت,
		 lookup_context.funcs.collect, lookup_context.collect_معلومات);
  recurse_lookups (c,
		   lookup_شمېر_2, lookup_ریکارډ);
}

ساكن مضمن منطقي context_would_apply_lookup (hb_would_apply_context_t *c,
					       بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
					       glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
					       بینښه صحيح lookup_شمېر_2 HB_ناکارول_شوی,
					       ثابت Lookup_ریکارډ lookup_ریکارډ[] HB_ناکارول_شوی,
					       ثابت ContextApplyLookupContext &lookup_context)
{
  أرجع would_سمون_ننوت (c,
			    ننوت_شمېر, ننوت,
			    lookup_context.funcs.سمون, lookup_context.سمون_معلومات_2);
}

ساكن مضمن منطقي context_apply_lookup (hb_ot_apply_context_t *c,
					 بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
					 glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
					 بینښه صحيح lookup_شمېر_2,
					 ثابت Lookup_ریکارډ lookup_ریکارډ[],
					 ثابت ContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (ننوت_شمېر > HB_MAX_CONTEXT_اوږدتیا)) أرجع خطأ;

  بینښه سمون_پای_ټکی = 0;
  منطقي ret = خطأ;
  إذا (سمون_ننوت (c,
		   ننوت_شمېر, ننوت,
		   lookup_context.funcs.سمون, lookup_context.سمون_معلومات_2,
		   &سمون_پای_ټکی))
  {
    c->بفر->unsafe_منزل_break (c->بفر->idx, سمون_پای_ټکی);
    apply_lookup (c,
		  ننوت_شمېر,
		  lookup_شمېر_2, lookup_ریکارډ,
		  سمون_پای_ټکی);
    ret = ریښتیا;
  }
  وإلا
  {
    c->بفر->unsafe_منزل_concat (c->بفر->idx, سمون_پای_ټکی);
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
      خپلکاری &info = c->بفر->info;
      بینښه شمېر = c->بفر->len;
      لكل (بینښه i = 0; i < شمېر; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      أرجع ریښتیا;
    }
    حالة hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (بینښه) -1;
      HB_بفر_DEALLOCATE_VAR (c->بفر, syllable);
      اكسر;
    }
  }
  أرجع خطأ;
}

کينډۍ <ډول_نوم Types>
بنية قاعده_3
{
  کينډۍ <ډول_نوم سېټ_Types, ډول_نوم قاعده_Type>
  ملګری بنية قاعده_سېټ_د;

  منطقي intersects (ثابت hb_سېټ_t *glyphs, Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    أرجع context_intersects (glyphs,
			       ننوت_شمېر, ننوت_Z.arrayZ,
			       lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, بینښه value, ContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خپلکاری &lookup_ریکارډ = StructAfter<Unsized_Array_د<Lookup_ریکارډ>>
					   (ننوت_Z.as_array ((ننوت_شمېر ? ننوت_شمېر - 1 : 0)));
    context_depend_lookup (c,
			   ننوت_شمېر, ننوت_Z.arrayZ,
			   lookup_شمېر_2, lookup_ریکارډ.arrayZ,
			   هذا,
			   value, lookup_context);
  }

  فراغ تړون (hb_تړون_context_t *c, بینښه value, Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    ثابت خپلکاری &lookup_ریکارډ = StructAfter<Unsized_Array_د<Lookup_ریکارډ>>
					   (ننوت_Z.as_array ((ننوت_شمېر ? ننوت_شمېر - 1 : 0)));
    context_تړون_lookup (c,
			    ننوت_شمېر, ننوت_Z.arrayZ,
			    lookup_شمېر_2, lookup_ریکارډ.arrayZ,
			    value, lookup_context);
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c,
                        Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت خپلکاری &lookup_ریکارډ = StructAfter<Unsized_Array_د<Lookup_ریکارډ>>
					   (ننوت_Z.as_array (ننوت_شمېر ? ننوت_شمېر - 1 : 0));
    recurse_lookups (c, lookup_شمېر_2, lookup_ریکارډ.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خپلکاری &lookup_ریکارډ = StructAfter<Unsized_Array_د<Lookup_ریکارډ>>
					   (ننوت_Z.as_array (ننوت_شمېر ? ننوت_شمېر - 1 : 0));
    context_collect_glyphs_lookup (c,
				   ننوت_شمېر, ننوت_Z.arrayZ,
				   lookup_شمېر_2, lookup_ریکارډ.arrayZ,
				   lookup_context);
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest,
			      ثابت ClassDef *class_def = تش_اشاره) ثابت
  {
    إذا (ننوت_شمېر <= 1)
    {
      *digest = hb_سېټ_digest_t::ډک ();
      أرجع;
    }

    بینښه second = ننوت_Z.arrayZ[0];
    إذا (!class_def)
      digest->جمع_کړه (second);
    وإلا إذا (!second)
      *digest = hb_سېټ_digest_t::ډک ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خپلکاری &lookup_ریکارډ = StructAfter<Unsized_Array_د<Lookup_ریکارډ>>
					   (ننوت_Z.as_array (ننوت_شمېر ? ننوت_شمېر - 1 : 0));
    أرجع context_would_apply_lookup (c,
				       ننوت_شمېر, ننوت_Z.arrayZ,
				       lookup_شمېر_2, lookup_ریکارډ.arrayZ,
				       lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خپلکاری &lookup_ریکارډ = StructAfter<Unsized_Array_د<Lookup_ریکارډ>>
					   (ننوت_Z.as_array (ننوت_شمېر ? ننوت_شمېر - 1 : 0));
    return_trace (context_apply_lookup (c, ننوت_شمېر, ننوت_Z.arrayZ, lookup_شمېر_2, lookup_ریکارډ.arrayZ, lookup_context));
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *ننوت_mapping, /* old->new glyphid or class mapping */
		  ثابت hb_map_t *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->پیل_embed (هذا);
    إذا (unlikely (!c->extend_min (بهر))) return_trace (خطأ);

    بهر->ننوت_شمېر = ننوت_شمېر;
    ثابت خپلکاری ننوت = ننوت_Z.as_array (ننوت_شمېر - 1);
    لكل (ثابت خپلکاری org : ننوت)
    {
      ډول_نوم Types::HBUINT d;
      d = ننوت_mapping->get (org);
      c->copy (d);
    }

    ثابت خپلکاری &lookup_ریکارډ = StructAfter<Unsized_Array_د<Lookup_ریکارډ>>
					   (ننوت_Z.as_array ((ننوت_شمېر ? ننوت_شمېر - 1 : 0)));

    بینښه شمېر = serialize_lookuprecord_array (c, lookup_ریکارډ.as_array (lookup_شمېر_2), lookup_map);
    return_trace (c->check_ټاکل (بهر->lookup_شمېر_2, شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!ننوت_شمېر)) return_trace (خطأ);
    ثابت خپلکاری ننوت = ننوت_Z.as_array (ننوت_شمېر - 1);

    ثابت hb_map_t *mapping = klass_map == تش_اشاره ? c->plan->glyph_map : klass_map;
    إذا (!hb_all (ننوت, mapping)) return_trace (خطأ);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (ننوت_Z.arrayZ,
				  ننوت_Z.توکی_اندازه * (ننوت_شمېر ? ننوت_شمېر - 1 : 0) +
				  Lookup_ریکارډ::static_اندازه * lookup_شمېر_2));
  }

  محمي:
  HBUINT16	ننوت_شمېر;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_شمېر_2;		/* Number of LookupRecords */
  Unsized_Array_د<ډول_نوم Types::HBUINT>
		ننوت_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_اندازه_ARRAY (4, ننوت_Z);
};

کينډۍ <ډول_نوم Types, ډول_نوم قاعده_Type>
بنية قاعده_سېټ_د
{
  منطقي intersects (ثابت hb_سېټ_t *glyphs,
		   Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([&] (ثابت قاعده_Type &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_لومړی_ننوت_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (بینښه i = 0; i < قاعده_2.len; i++)
    {
      ثابت قاعده_Type &r = هذا+قاعده_2.arrayZ[i];
      إذا (r.ننوت_شمېر <= 1)
      {
	digest->سېټ_ډک ();
	أرجع;
      }
      digest->جمع_کړه (r.ننوت_Z.arrayZ[0]);
    }
  }

  فراغ depend (hb_depend_context_t *c, بینښه value, ContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت قاعده_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }

  فراغ تړون (hb_تړون_context_t *c, بینښه value,
		Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت قاعده_Type &_) { _.تړون (c, value, lookup_context); })
    ;
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c,
                        Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت قاعده_Type &_) { _.تړون_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت قاعده_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest,
			      ثابت ClassDef *class_def = تش_اشاره) ثابت
  {
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([=] (ثابت قاعده_Type &_) { _.collect_دوهم_glyphs (digest, class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([&] (ثابت قاعده_Type &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = تش_اشاره) ثابت
  {
    TRACE_APPLY (هذا);

    بینښه عدد_قواعد = قاعده_2.len;

#که_نه_تعریف_شوی HB_NO_OT_RULESETS_FAST_لار
    إذا (HB_OPTIMIZE_اندازه_VAL || عدد_قواعد <= 4)
#د_که_پای
    {
    slow:
      return_trace (
      + hb_iter (قاعده_2)
      | hb_map (hb_جمع_کړه (هذا))
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
    خپلکاری &skippy_iter = c->iter_context;
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سېټ_سمون_func (سمون_always, تش_اشاره);
    skippy_iter.سېټ_glyph_معلومات ((HBUINT16 *) تش_اشاره);
    بینښه unsafe_منزل = (بینښه) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *لومړی = تش_اشاره, *second = تش_اشاره;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      لومړی = &c->بفر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_ارزښت &&
	  !digest->may_have (lookup_context.get_ارزښت (*لومړی, lookup_context.سمون_معلومات_2)))
	{
	  c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_منزل_concat = خطأ;
      منطقي ret =
      + hb_iter (قاعده_2)
      | hb_map (hb_جمع_کړه (هذا))
      | hb_filter ([&] (ثابت قاعده_Type &_)
		   {
		     إذا (_.ننوت_شمېر <= 1) أرجع ریښتیا;
		     unsafe_منزل_concat = ریښتیا;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت قاعده_Type &_) { أرجع _.apply (c, lookup_context); })
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

    خپلکاری سمون_ننوت = lookup_context.funcs.سمون;
    خپلکاری *ننوت_معلومات = lookup_context.سمون_معلومات_2;
    لكل (بینښه صحيح i = 0; i < عدد_قواعد; i++)
    {
      ثابت خپلکاری &r = هذا+قاعده_2.arrayZ[i];

      ثابت خپلکاری &ننوت = r.ننوت_Z;

      إذا (r.ننوت_شمېر <= 1 ||
	  (!سمون_ننوت ||
	   سمون_ننوت (*لومړی, ننوت.arrayZ[0], ننوت_معلومات)))
      {
        إذا (!second ||
	    (r.ننوت_شمېر <= 2 ||
	     (!سمون_ننوت ||
	      سمون_ننوت (*second, ننوت.arrayZ[1], ننوت_معلومات)))
	   )
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_منزل != (بینښه) -1)
	      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
	    return_trace (ریښتیا);
	  }
	}
	وإلا
	  unsafe_منزل = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_منزل == (بینښه) -1)
	  unsafe_منزل = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	لكل (; i + 1 < عدد_قواعد; i++)
	{
	  ثابت خپلکاری &r2 = هذا+قاعده_2.arrayZ[i + 1];
	  ثابت خپلکاری &input2 = r2.ننوت_Z;
	  إذا (r2.ننوت_شمېر <= 1 || input2.arrayZ[0] != ننوت.arrayZ[0])
	    اكسر;
	}
      }
    }
    إذا (likely (unsafe_منزل != (بینښه) -1))
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);

    خپلکاری snap = c->serializer->snapshot ();
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    لكل (ثابت خپلکاری& _ : قاعده_2)
    {
      إذا (!_) استمر;
      خپلکاری o_snap = c->serializer->snapshot ();
      خپلکاری *o = بهر->قاعده_2.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا, lookup_map, klass_map))
      {
	بهر->قاعده_2.راوباسه ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (بهر->قاعده_2);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (قاعده_2.sanitize (c, هذا));
  }

  محمي:
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<قاعده_Type>>
		قاعده_2;			/* Array of Rule tables
					 * ordered by preference */
  عام:
  DEFINE_اندازه_ARRAY (2, قاعده_2);
};

کينډۍ <ډول_نوم Types>
بنية قاعده_سېټ : قاعده_سېټ_د<SmallTypes, قاعده_3<Types>> {};

کينډۍ <ډول_نوم Types>
بنية Class_قاعده_سېټ : قاعده_سېټ_د<Types, قاعده_3<SmallTypes>> {};


کينډۍ <ډول_نوم Types>
بنية ContextFormat1_4
{
  کارول قاعده_سېټ = OT::قاعده_سېټ<Types>;

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    بنية Context_تړون_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      تش_اشاره
    };

    أرجع
    + hb_zip (هذا+coverage, قاعده_سېټ_2)
    | hb_filter (*glyphs, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([&] (ثابت قاعده_سېټ &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع ریښتیا; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (*c->glyphs, *cur_فعال_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      تش_اشاره,
      تش_اشاره,  /* intersects_cache */
      تش_اشاره   /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((بینښه) قاعده_سېټ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->پلار_فعال_glyphs ().has (_);
    }, hb_لومړی)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بینښه> _) { أرجع hb_pair_t<بینښه, ثابت قاعده_سېټ&> (_.لومړی, هذا+قاعده_سېټ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بینښه, ثابت قاعده_سېټ&>& _) { _.second.depend (c, _.لومړی, lookup_context); })
    ;

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (c->پخوانی_پلار_فعال_glyphs (), *cur_فعال_glyphs);

    بنية Context_تړون_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      تش_اشاره
    };

    + hb_zip (هذا+coverage, hb_range ((بینښه) قاعده_سېټ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->پخوانی_پلار_فعال_glyphs ().has (_);
    }, hb_لومړی)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بینښه> _) { أرجع hb_pair_t<بینښه, ثابت قاعده_سېټ&> (_.لومړی, هذا+قاعده_سېټ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بینښه, ثابت قاعده_سېټ&>& _) { _.second.تړون (c, _.لومړی, lookup_context); })
    ;

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت
  {
    بنية Context_تړون_Lookup_Context lookup_context = {
      {intersects_glyph, تش_اشاره},
      ContextFormat::SimpleContext,
      تش_اشاره
    };

    + hb_zip (هذا+coverage, قاعده_سېټ_2)
    | hb_filter (*c->glyphs, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت قاعده_سېټ &_) { _.تړون_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->ننوت);

    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      تش_اشاره
    };

    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت قاعده_سېټ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  {
    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([digest] (ثابت قاعده_سېټ &_) { _.collect_دوهم_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت قاعده_سېټ &قاعده_سېټ_3 = هذا+قاعده_سېټ_2[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ContextApplyLookupContext lookup_context = {
      {سمون_glyph},
      تش_اشاره
    };
    أرجع قاعده_سېټ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (likely (شاخص_2 == NOT_COVERED))
      return_trace (خطأ);

    ثابت قاعده_سېټ &قاعده_سېټ_3 = هذا+قاعده_سېټ_2[شاخص_2];
    بنية ContextApplyLookupContext lookup_context = {
      {سمون_glyph},
      تش_اشاره
    };
    return_trace (قاعده_سېټ_3.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_نښان_2 == HB_OT_نښان_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, قاعده_سېټ_2)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (subset_دننه_واټن_array (c, بهر->قاعده_سېټ_2, هذا, lookup_map), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بهر->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && قاعده_سېټ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<قاعده_سېټ>>
		قاعده_سېټ_2;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, قاعده_سېټ_2);
};


کينډۍ <ډول_نوم Types>
بنية ContextFormat2_5
{
  کارول قاعده_سېټ = OT::Class_قاعده_سېټ<Types>;

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية Context_تړون_Lookup_Context lookup_context = {
      {intersects_class, تش_اشاره},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_سېټ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سېټ (*glyphs, retained_coverage_glyphs);

    hb_سېټ_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    أرجع
    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<بینښه, ثابت قاعده_سېټ &> p)
	      { أرجع class_def.intersects_class (glyphs, p.لومړی) &&
		       coverage_glyph_classes.has (p.لومړی) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع ریښتیا; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (*c->glyphs, *cur_فعال_glyphs);

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

    + hb_enumerate (قاعده_سېټ_2)
    | hb_filter ([&] (بینښه _)
    { أرجع class_def.intersects_class (&c->پلار_فعال_glyphs (), _); },
		 hb_لومړی)
    | hb_apply ([&] (ثابت hb_pair_t<بینښه, ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<قاعده_سېټ>&> _)
                {
                  ثابت قاعده_سېټ& قاعده_سېټ_3 = هذا+_.second;
                  قاعده_سېټ_3.depend (c, _.لومړی, lookup_context);
                })
    ;

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (c->پخوانی_پلار_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    بنية Context_تړون_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (قاعده_سېټ_2)
    | hb_filter ([&] (بینښه _)
    { أرجع class_def.intersects_class (&c->پلار_فعال_glyphs (), _); },
		 hb_لومړی)
    | hb_apply ([&] (ثابت hb_pair_t<بینښه, ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<قاعده_سېټ>&> _)
                {
                  ثابت قاعده_سېټ& قاعده_سېټ_3 = هذا+_.second;
                  قاعده_سېټ_3.تړون (c, _.لومړی, lookup_context);
                })
    ;

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية Context_تړون_Lookup_Context lookup_context = {
      {intersects_class, تش_اشاره},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_enumerate
    | hb_filter ([&] (ثابت hb_pair_t<بینښه, ثابت قاعده_سېټ &> p)
    { أرجع class_def.intersects_class (c->glyphs, p.لومړی); })
    | hb_map (hb_دوهم)
    | hb_apply ([&] (ثابت قاعده_سېټ & _)
    { _.تړون_lookups (c, lookup_context); });
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->ننوت);

    ثابت ClassDef &class_def = هذا+classDef;
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت قاعده_سېټ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([=, &class_def] (ثابت قاعده_سېټ &_) { _.collect_دوهم_glyphs (digest, &class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    بینښه صحيح شاخص_2 = class_def.get_class (c->glyphs[0]);
    ثابت قاعده_سېټ &قاعده_سېټ_3 = هذا+قاعده_سېټ_2[شاخص_2];
    بنية ContextApplyLookupContext lookup_context = {
      {سمون_class},
      &class_def
    };
    أرجع قاعده_سېټ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بینښه cache_لګښت () ثابت
  {
    أرجع (هذا+classDef).لګښت ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_دوګونی_cache_t coverage;
    hb_ot_layout_ruleset_digest_t قاعده_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بینښه شمېر = قاعده_سېټ_2.len;
    بینښه اندازه = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    شمېر * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (اندازه);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (بینښه i = 0; i < شمېر; i++)
	(هذا+قاعده_سېټ_2[i]).collect_لومړی_ننوت_classes (&cache->قاعده_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, ریښتیا, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage_دوګونی (c->بفر->cur().codepoint, cache ? &cache->coverage : تش_اشاره);
#وإلا
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
#د_که_پای
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &class_def = هذا+classDef;

    بنية ContextApplyLookupContext lookup_context = {
      {cached ? سمون_class_cached : سمون_class},
      &class_def,
      cached ? get_class_cached_ارزښت : get_class_ارزښت
    };

    شاخص_2 = cached ? get_class_cached (class_def, c->بفر->cur()) : class_def.get_class (c->بفر->cur().codepoint);
    ثابت قاعده_سېټ &قاعده_سېټ_3 = هذا+قاعده_سېټ_2[شاخص_2];
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && شاخص_2 < قاعده_سېټ_2.len ? &cache->قاعده_sets[شاخص_2] : تش_اشاره;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = تش_اشاره;
#د_که_پای
    return_trace (قاعده_سېټ_3.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;
    إذا (unlikely (!بهر->coverage.serialize_subset (c, coverage, هذا)))
      return_trace (خطأ);

    hb_map_t klass_map;
    بهر->classDef.serialize_subset (c, classDef, هذا, &klass_map);

    ثابت hb_سېټ_t* glyphset = c->plan->glyphset_gsub ();
    hb_سېټ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سېټ (*glyphset, retained_coverage_glyphs);

    hb_سېټ_t coverage_glyph_classes;
    (هذا+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ثابت hb_map_t *lookup_map = c->جدول_نښان_2 == HB_OT_نښان_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    منطقي ret = ریښتیا;
    صحيح non_zero_شاخص = -1, شاخص_2 = 0;
    خپلکاری snapshot = c->serializer->snapshot();
    لكل (ثابت خپلکاری& _ : + hb_enumerate (قاعده_سېټ_2)
			 | hb_filter (klass_map, hb_لومړی))
    {
      خپلکاری *o = بهر->قاعده_سېټ_2.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      إذا (coverage_glyph_classes.has (_.لومړی) &&
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
      بهر->قاعده_سېټ_2.راوباسه ();
      شاخص_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (منطقي (بهر->قاعده_سېټ_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && classDef.sanitize (c, هذا) && قاعده_سېټ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  ډول_نوم Types::کينډۍ LOffset_منزل<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<قاعده_سېټ>>
		قاعده_سېټ_2;		/* Array of RuleSet tables
					 * ordered by class */
  عام:
  DEFINE_اندازه_ARRAY (2 + 2 * Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, قاعده_سېټ_2);
};


کينډۍ <ډول_نوم Types>
بنية ContextFormat3_6
{
  کارول قاعده_سېټ = OT::قاعده_سېټ<SmallTypes>;

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (glyphs))
      أرجع خطأ;

    بنية Context_تړون_Lookup_Context lookup_context = {
      {intersects_coverage, تش_اشاره},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    أرجع context_intersects (glyphs,
			       glyph_شمېر_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع ریښتیا; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (*c->glyphs, *cur_فعال_glyphs);

    ثابت Lookup_ریکارډ *lookup_ریکارډ = &StructAfter<Lookup_ریکارډ> (coverageZ.as_array (glyph_شمېر_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا,
      تش_اشاره,  /* intersects_cache */
      تش_اشاره   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_شمېر_2, (ثابت HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_شمېر_2, lookup_ریکارډ,
			   هذا,
			   coverageZ[0], lookup_context);

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (c->پخوانی_پلار_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت Lookup_ریکارډ *lookup_ریکارډ = &StructAfter<Lookup_ریکارډ> (coverageZ.as_array (glyph_شمېر_2));
    بنية Context_تړون_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    context_تړون_lookup (c,
			    glyph_شمېر_2, coverageZ.arrayZ + 1,
			    lookup_شمېر_2, lookup_ریکارډ,
			    0, lookup_context);

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;
    ثابت Lookup_ریکارډ *lookup_ریکارډ = &StructAfter<Lookup_ریکارډ> (coverageZ.as_array (glyph_شمېر_2));
    recurse_lookups (c, lookup_شمېر_2, lookup_ریکارډ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverageZ[0]).collect_coverage (c->ننوت);

    ثابت Lookup_ریکارډ *lookup_ریکارډ = &StructAfter<Lookup_ریکارډ> (coverageZ.as_array (glyph_شمېر_2));
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      هذا
    };

    context_collect_glyphs_lookup (c,
				   glyph_شمېر_2, coverageZ.arrayZ + 1,
				   lookup_شمېر_2, lookup_ریکارډ,
				   lookup_context);
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  {
    إذا (glyph_شمېر_2 <= 1)
      *digest = hb_سېټ_digest_t::ډک ();
    وإلا
      (هذا+coverageZ[1]).collect_coverage (digest);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Lookup_ریکارډ *lookup_ریکارډ = &StructAfter<Lookup_ریکارډ> (coverageZ.as_array (glyph_شمېر_2));
    بنية ContextApplyLookupContext lookup_context = {
      {سمون_coverage},
      هذا
    };
    أرجع context_would_apply_lookup (c,
				       glyph_شمېر_2, coverageZ.arrayZ + 1,
				       lookup_شمېر_2, lookup_ریکارډ,
				       lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverageZ[0]; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بینښه صحيح شاخص_2 = (هذا+coverageZ[0]).get_coverage (c->بفر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت Lookup_ریکارډ *lookup_ریکارډ = &StructAfter<Lookup_ریکارډ> (coverageZ.as_array (glyph_شمېر_2));
    بنية ContextApplyLookupContext lookup_context = {
      {سمون_coverage},
      هذا
    };
    return_trace (context_apply_lookup (c,
					glyph_شمېر_2,
					coverageZ.arrayZ + 1,
					lookup_شمېر_2, lookup_ریکارډ,
					lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    بهر->format = format;
    بهر->glyph_شمېر_2 = glyph_شمېر_2;

    خپلکاری coverages = coverageZ.as_array (glyph_شمېر_2);

    لكل (ثابت خپلکاری& دننه_واټن : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      خپلکاری *o = c->serializer->allocate_اندازه<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>> (Types::دننه_واټن_2::static_اندازه);
      إذا (unlikely (!o)) return_trace (خطأ);
      إذا (!o->serialize_subset (c, دننه_واټن, هذا)) return_trace (خطأ);
    }

    ثابت خپلکاری& lookup_ریکارډ = StructAfter<Unsized_Array_د<Lookup_ریکارډ>> (coverageZ.as_array (glyph_شمېر_2));
    ثابت hb_map_t *lookup_map = c->جدول_نښان_2 == HB_OT_نښان_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    بینښه شمېر = serialize_lookuprecord_array (c->serializer, lookup_ریکارډ.as_array (lookup_شمېر_2), lookup_map);
    return_trace (c->serializer->check_ټاکل (بهر->lookup_شمېر_2, شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    بینښه صحيح شمېر = glyph_شمېر_2;
    إذا (unlikely (!شمېر)) return_trace (خطأ); /* We want to access coverageZ[0] freely. */
    إذا (unlikely (!c->check_array (coverageZ.arrayZ, شمېر))) return_trace (خطأ);
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (unlikely (!coverageZ[i].sanitize (c, هذا))) return_trace (خطأ);
    ثابت Lookup_ریکارډ *lookup_ریکارډ = &StructAfter<Lookup_ریکارډ> (coverageZ.as_array (glyph_شمېر_2));
    return_trace (likely (c->check_array (lookup_ریکارډ, lookup_شمېر_2)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_شمېر_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_شمېر_2;		/* Number of LookupRecords */
  Unsized_Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>
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
  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    حالة 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#د_که_پای
  } u;
};


/* Chaining Contextual lookups */

بنية Chain_Context_تړون_Lookup_Context
{
  Context_تړون_Funcs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_معلومات[3];
  فراغ *intersects_cache[3];
  فراغ *intersected_glyphs_cache;
};

تعريفنوع Chain_Context_تړون_Lookup_Context ChainContextDependLookupContext;

بنية ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_معلومات[3];
};

بنية ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ثابت فراغ *سمون_معلومات_2[3];
  get_ارزښت_func_t get_ننوت_ارزښت;
};

ساكن مضمن منطقي chain_context_intersects (ثابت hb_سېټ_t *glyphs,
					     بینښه صحيح backtrack_شمېر,
					     glyph_معلومات_t backtrack,
					     بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
					     glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
					     بینښه صحيح lookahead_شمېر,
					     glyph_معلومات_t lookahead,
					     Chain_Context_تړون_Lookup_Context &lookup_context)
{
  منطقي bt_ok = array_is_subset_د (glyphs,
			     backtrack_شمېر, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_معلومات[0],
			     lookup_context.intersects_cache[0]);
  منطقي دننه_ok = array_is_subset_د (glyphs,
			     ننوت_شمېر ? ننوت_شمېر - 1 : 0, ننوت,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_معلومات[1],
			     lookup_context.intersects_cache[1]);
  منطقي la_ok = array_is_subset_د (glyphs,
			     lookahead_شمېر, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_معلومات[2],
			     lookup_context.intersects_cache[2]);

  أرجع bt_ok && دننه_ok && la_ok;
}

ساكن مضمن فراغ chain_context_تړون_lookup (hb_تړون_context_t *c,
						 بینښه صحيح backtrack_شمېر,
						 glyph_معلومات_t backtrack,
						 بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
						 glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
						 بینښه صحيح lookahead_شمېر,
						 glyph_معلومات_t lookahead,
						 بینښه صحيح lookup_شمېر_2,
						 ثابت Lookup_ریکارډ lookup_ریکارډ[],
						 بینښه value,
						 Chain_Context_تړون_Lookup_Context &lookup_context)
{
  إذا (chain_context_intersects (c->glyphs,
				backtrack_شمېر, backtrack,
				ننوت_شمېر, ننوت,
				lookahead_شمېر, lookahead,
				lookup_context))
    context_تړون_recurse_lookups (c,
		     ننوت_شمېر, ننوت,
		     lookup_شمېر_2, lookup_ریکارډ,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_معلومات[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ chain_context_depend_lookup (hb_depend_context_t *c,
						بینښه صحيح backtrack_شمېر,
						glyph_معلومات_t backtrack,
						بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
						glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
						بینښه صحيح lookahead_شمېر,
						glyph_معلومات_t lookahead,
						بینښه صحيح lookup_شمېر_2,
						ثابت Lookup_ریکارډ lookup_ریکارډ[],
						ثابت فراغ *قاعده_2,
						بینښه value,
						ChainContextDependLookupContext &lookup_context)
{
  إذا (!chain_context_intersects (c->glyphs,
				 backtrack_شمېر, backtrack,
				 ننوت_شمېر, ننوت,
				 lookahead_شمېر, lookahead,
				 lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, ننوت_شمېر, lookup_شمېر_2, lookup_ریکارډ, قاعده_2, value,
    lookup_context.context_format, lookup_context.intersects_معلومات[1],
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_معلومات->successful))
    أرجع;
  إذا (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    ننوت_شمېر, ننوت,
				    lookup_شمېر_2, lookup_ریکارډ,
				    قاعده_2,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_معلومات[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    تش_اشاره,
				    &scratch->cached_context_sets,
				    scratch);
    أرجع;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_سېټ_t &position_scratch = scratch->position_scratch;
  hb_ویکټور_t<ثابت hb_سېټ_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_ویکټور_t<ثابت hb_سېټ_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  لكل (بینښه i = 0; i < backtrack_شمېر; i++)
  {
    ثابت hb_سېټ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_معلومات[0],
      backtrack[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!backtrack_sets.ټېل_or_fail (pos_glyphs)))
      {
        c->depend_معلومات->fail ();
        أرجع;
      }
    }
  }

  /* Extract lookahead glyphs */
  لكل (بینښه i = 0; i < lookahead_شمېر; i++)
  {
    ثابت hb_سېټ_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_معلومات[2],
      lookahead[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!lookahead_sets.ټېل_or_fail (pos_glyphs)))
      {
        c->depend_معلومات->fail ();
        أرجع;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_سېټ_t &preliminary_context = scratch->preliminary_context;
  لكل (ثابت hb_سېټ_t *شا_سېټ : backtrack_sets)
  {
    إذا (شا_سېټ->get_population () == 1) {
      preliminary_context.جمع_کړه (شا_سېټ->get_min ());  // Direct glyph
    } وإلا إذا (شا_سېټ->get_population () > 1) {
      hb_codepoint_t سېټ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_معلومات->پیدا_کړه_or_create_context_سېټ (*شا_سېټ)
        : c->پیدا_کړه_or_create_cached_context_سېټ (شا_سېټ);
      إذا (unlikely (سېټ_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.جمع_کړه (HB_DEPEND_CONTEXT_سېټ_FLAG | سېټ_idx);  // Encoded set reference
    }
  }
  لكل (ثابت hb_سېټ_t *look_سېټ : lookahead_sets)
  {
    إذا (look_سېټ->get_population () == 1) {
      preliminary_context.جمع_کړه (look_سېټ->get_min ());  // Direct glyph
    } وإلا إذا (look_سېټ->get_population () > 1) {
      hb_codepoint_t سېټ_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_معلومات->پیدا_کړه_or_create_context_سېټ (*look_سېټ)
        : c->پیدا_کړه_or_create_cached_context_سېټ (look_سېټ);
      إذا (unlikely (سېټ_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.جمع_کړه (HB_DEPEND_CONTEXT_سېټ_FLAG | سېټ_idx);  // Encoded set reference
    }
  }
  إذا (unlikely (preliminary_context.دننه_error ()))
  {
    c->depend_معلومات->fail ();
    أرجع;
  }

  /* Build glyph sets for ALL input positions */
  hb_ویکټور_t<ثابت hb_سېټ_t *> &ننوت_position_glyphs = scratch->ننوت_position_glyphs;

  إذا (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ثابت hb_سېټ_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_معلومات[1], value,
      ریښتیا, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !ننوت_position_glyphs.ټېل_or_fail (pos0_glyphs)))
    {
      c->depend_معلومات->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (بینښه i = 0; i + 1 < ننوت_شمېر; i++)
    {
      ثابت hb_سېټ_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_معلومات[1],
	ننوت[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !ننوت_position_glyphs.ټېل_or_fail (pos_glyphs)))
      {
	c->depend_معلومات->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  ننوت_شمېر, ننوت,
				  lookup_شمېر_2, lookup_ریکارډ,
				  قاعده_2,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_معلومات[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? تش_اشاره : &ننوت_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : تش_اشاره,
				  scratch);
}

ساكن مضمن فراغ chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							بینښه صحيح backtrack_شمېر,
							glyph_معلومات_t backtrack,
							بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
							glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
							بینښه صحيح lookahead_شمېر,
							glyph_معلومات_t lookahead,
							بینښه صحيح lookup_شمېر_2,
							ثابت Lookup_ریکارډ lookup_ریکارډ[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_شمېر, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_معلومات[0]);
  collect_array (c, c->ننوت,
		 ننوت_شمېر ? ننوت_شمېر - 1 : 0, ننوت,
		 lookup_context.funcs.collect, lookup_context.collect_معلومات[1]);
  collect_array (c, c->after,
		 lookahead_شمېر, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_معلومات[2]);
  recurse_lookups (c,
		   lookup_شمېر_2, lookup_ریکارډ);
}

ساكن مضمن منطقي chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     بینښه صحيح backtrack_شمېر,
						     glyph_معلومات_t backtrack HB_ناکارول_شوی,
						     بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
						     glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
						     بینښه صحيح lookahead_شمېر,
						     glyph_معلومات_t lookahead HB_ناکارول_شوی,
						     بینښه صحيح lookup_شمېر_2 HB_ناکارول_شوی,
						     ثابت Lookup_ریکارډ lookup_ریکارډ[] HB_ناکارول_شوی,
						     ثابت ChainContextApplyLookupContext &lookup_context)
{
  أرجع (c->zero_context ? !backtrack_شمېر && !lookahead_شمېر : ریښتیا)
      && would_سمون_ننوت (c,
			    ننوت_شمېر, ننوت,
			    lookup_context.funcs.سمون[1], lookup_context.سمون_معلومات_2[1]);
}

ساكن مضمن منطقي chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       بینښه صحيح backtrack_شمېر,
					       glyph_معلومات_t backtrack,
					       بینښه صحيح ننوت_شمېر, /* Including the first glyph (not matched) */
					       glyph_معلومات_t ننوت, /* Array of input values--start with second glyph */
					       بینښه صحيح lookahead_شمېر,
					       glyph_معلومات_t lookahead,
					       بینښه صحيح lookup_شمېر_2,
					       ثابت Lookup_ریکارډ lookup_ریکارډ[],
					       ثابت ChainContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (ننوت_شمېر > HB_MAX_CONTEXT_اوږدتیا)) أرجع خطأ;

  بینښه پیل_شاخص = c->بفر->بهر_len;
  بینښه پای_ټکی_شاخص = c->بفر->idx;
  بینښه سمون_پای_ټکی = 0;
  منطقي ret = ریښتیا;
  إذا (!(سمون_ننوت (c,
		     ننوت_شمېر, ننوت,
		     lookup_context.funcs.سمون[1], lookup_context.سمون_معلومات_2[1],
		     &سمون_پای_ټکی) && (پای_ټکی_شاخص = سمون_پای_ټکی)
       && سمون_lookahead (c,
			   lookahead_شمېر, lookahead,
			   lookup_context.funcs.سمون[2], lookup_context.سمون_معلومات_2[2],
			   سمون_پای_ټکی, &پای_ټکی_شاخص)))
  {
    c->بفر->unsafe_منزل_concat (c->بفر->idx, پای_ټکی_شاخص);
    أرجع خطأ;
  }

  إذا (!سمون_backtrack (c,
			backtrack_شمېر, backtrack,
			lookup_context.funcs.سمون[0], lookup_context.سمون_معلومات_2[0],
			&پیل_شاخص))
  {
    c->بفر->unsafe_منزل_concat_from_outbuffer (پیل_شاخص, پای_ټکی_شاخص);
    أرجع خطأ;
  }

  c->بفر->unsafe_منزل_break_from_outbuffer (پیل_شاخص, پای_ټکی_شاخص);
  apply_lookup (c,
		ننوت_شمېر,
		lookup_شمېر_2, lookup_ریکارډ,
		سمون_پای_ټکی);

  أرجع ret;
}

کينډۍ <ډول_نوم Types>
بنية Chain_قاعده
{
  کينډۍ <ډول_نوم سېټ_Types, ډول_نوم Chain_قاعده_Type>
  ملګری بنية Chain_قاعده_سېټ_د;

  منطقي intersects (ثابت hb_سېټ_t *glyphs, Chain_Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ننوت.lenP1, ننوت.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, بینښه value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 ننوت.lenP1, ننوت.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 value,
				 lookup_context);
  }

  فراغ تړون (hb_تړون_context_t *c, بینښه value,
		Chain_Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_تړون_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ننوت.lenP1, ننوت.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c,
                        Chain_Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ننوت.lenP1, ننوت.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest,
			      ثابت ClassDef *ننوت_class_def = تش_اشاره,
			      ثابت ClassDef *lookahead_class_def = تش_اشاره) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);

    بینښه second;
    ثابت ClassDef *class_def;
    إذا (ننوت.lenP1 > 1)
    {
      second = ننوت.arrayZ[0];
      class_def = ننوت_class_def;
    }
    وإلا إذا (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    وإلا
    {
      *digest = hb_سېټ_digest_t::ډک ();
      أرجع;
    }

    إذا (!class_def)
      digest->جمع_کړه (second);
    وإلا إذا (!second)
      *digest = hb_سېټ_digest_t::ډک ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ننوت.lenP1, ننوت.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ننوت.lenP1, ننوت.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ثابت
  {
    c->copy (len);
    لكل (ثابت خپلکاری g : it)
    {
      ډول_نوم Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *lookup_map,
		  ثابت hb_map_t *backtrack_map,
		  ثابت hb_map_t *ننوت_map = تش_اشاره,
		  ثابت hb_map_t *lookahead_map = تش_اشاره) ثابت
  {
    TRACE_SERIALIZE (هذا);

    ثابت hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    إذا (ننوت_map) mapping = ننوت_map;
    serialize_array (c, ننوت.lenP1, + ننوت.iter ()
				     | hb_map (mapping));

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    إذا (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_شمېر_2 = c->embed (&(lookup.len));
    إذا (!lookup_شمېر_2) return_trace (خطأ);

    بینښه شمېر = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_ټاکل (*lookup_شمېر_2, شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_map = تش_اشاره,
	       ثابت hb_map_t *ننوت_map = تش_اشاره,
	       ثابت hb_map_t *lookahead_map = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);

    إذا (!backtrack_map)
    {
      ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
      إذا (!hb_all (backtrack, glyphset) ||
	  !hb_all (ننوت, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    وإلا
    {
      إذا (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (ننوت, ننوت_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, backtrack_map, ننوت_map, lookahead_map);
    }

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Hyper-optimized sanitized because this is really hot. */
    إذا (unlikely (!backtrack.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    إذا (unlikely (!ننوت.lenP1.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    إذا (unlikely (!lookahead.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  Array16_د<ډول_نوم Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  Headless_Array16_د<ډول_نوم Types::HBUINT>
		ننوت_X;			/* Array of input values (start with
					 * second glyph) */
  Array16_د<ډول_نوم Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16_د<Lookup_ریکارډ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_اندازه_MIN (8);
};

کينډۍ <ډول_نوم Types, ډول_نوم Chain_قاعده_Type>
بنية Chain_قاعده_سېټ_د
{
  منطقي intersects (ثابت hb_سېټ_t *glyphs, Chain_Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([&] (ثابت Chain_قاعده_Type &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_لومړی_ننوت_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (بینښه i = 0; i < قاعده_2.len; i++)
    {
      ثابت Chain_قاعده_Type &r = هذا+قاعده_2.arrayZ[i];
      ثابت خپلکاری &ننوت = StructAfter<decltype (r.ننوت_X)> (r.backtrack);
      إذا (ننوت.lenP1 <= 1)
      {
	digest->سېټ_ډک ();
	أرجع;
      }
      digest->جمع_کړه (ننوت.arrayZ[0]);
    }
  }
  فراغ depend (hb_depend_context_t *c, بینښه value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_Type &_) { _.depend (c, value, lookup_context); })
    ;
  }
  فراغ تړون (hb_تړون_context_t *c, بینښه value, Chain_Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_Type &_) { _.تړون (c, value, lookup_context); })
    ;
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c,
                        Chain_Context_تړون_Lookup_Context &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_حد_exceeded ())) أرجع;

    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_Type &_) { _.تړون_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_Type &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest,
			      ثابت ClassDef *ننوت_class_def = تش_اشاره,
			      ثابت ClassDef *lookahead_class_def = تش_اشاره) ثابت
  {
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([=] (ثابت Chain_قاعده_Type &_)
		{ _.collect_دوهم_glyphs (digest, ننوت_class_def, lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (قاعده_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([&] (ثابت Chain_قاعده_Type &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = تش_اشاره) ثابت
  {
    TRACE_APPLY (هذا);

    بینښه عدد_قواعد = قاعده_2.len;

#که_نه_تعریف_شوی HB_NO_OT_RULESETS_FAST_لار
    إذا (HB_OPTIMIZE_اندازه_VAL || عدد_قواعد <= 4)
#د_که_پای
    {
    slow:
      return_trace (
      + hb_iter (قاعده_2)
      | hb_map (hb_جمع_کړه (هذا))
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
    خپلکاری &skippy_iter = c->iter_context;
    skippy_iter.reset (c->بفر->idx);
    skippy_iter.سېټ_سمون_func (سمون_always, تش_اشاره);
    skippy_iter.سېټ_glyph_معلومات ((HBUINT16 *) تش_اشاره);
    بینښه unsafe_منزل = (بینښه) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *لومړی = تش_اشاره, *second = تش_اشاره;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->بفر->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      لومړی = &c->بفر->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_ننوت_ارزښت &&
	  !digest->may_have (lookup_context.get_ننوت_ارزښت (*لومړی, lookup_context.سمون_معلومات_2[1])))
	{
	  c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_منزل_concat = خطأ;
      منطقي ret =
      + hb_iter (قاعده_2)
      | hb_map (hb_جمع_کړه (هذا))
      | hb_filter ([&] (ثابت Chain_قاعده_Type &_)
		   {
		     ثابت خپلکاری &ننوت = StructAfter<decltype (_.ننوت_X)> (_.backtrack);
		     ثابت خپلکاری &lookahead = StructAfter<decltype (_.lookaheadX)> (ننوت);
		     إذا (ننوت.lenP1 <= 1 && lookahead.len == 0) أرجع ریښتیا;
		     unsafe_منزل_concat = ریښتیا;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت Chain_قاعده_Type &_) { أرجع _.apply (c, lookup_context); })
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

    خپلکاری سمون_ننوت = lookup_context.funcs.سمون[1];
    خپلکاری سمون_lookahead = lookup_context.funcs.سمون[2];
    خپلکاری *ننوت_معلومات = lookup_context.سمون_معلومات_2[1];
    خپلکاری *lookahead_معلومات = lookup_context.سمون_معلومات_2[2];
    لكل (بینښه صحيح i = 0; i < عدد_قواعد; i++)
    {
      ثابت خپلکاری &r = هذا+قاعده_2.arrayZ[i];

      ثابت خپلکاری &ننوت = StructAfter<decltype (r.ننوت_X)> (r.backtrack);
      ثابت خپلکاری &lookahead = StructAfter<decltype (r.lookaheadX)> (ننوت);

      بینښه lenP1 = ننوت.lenP1;
      إذا (lenP1 > 1 ?
	   (!سمون_ننوت ||
	    سمون_ننوت (*لومړی, ننوت.arrayZ[0], ننوت_معلومات))
	  :
	   (!lookahead.len || !سمون_lookahead ||
	    سمون_lookahead (*لومړی, lookahead.arrayZ[0], lookahead_معلومات)))
      {
	lenP1 = hb_max (lenP1, 1u);
        إذا (!second ||
	    (lenP1 > 2 ?
	     (!سمون_ننوت ||
	      سمون_ننوت (*second, ننوت.arrayZ[1], ننوت_معلومات))
	     :
	     (lookahead.len <= 2 - lenP1 || !سمون_lookahead ||
	      سمون_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_معلومات))))
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_منزل != (بینښه) -1)
	      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);
	    return_trace (ریښتیا);
	  }
	}
	وإلا
	  unsafe_منزل = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_منزل == (بینښه) -1)
	  unsafe_منزل = unsafe_to1;

	إذا (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  لكل (; i + 1 < عدد_قواعد; i++)
	  {
	    ثابت خپلکاری &r2 = هذا+قاعده_2.arrayZ[i + 1];
	    ثابت خپلکاری &input2 = StructAfter<decltype (r2.ننوت_X)> (r2.backtrack);
	    إذا (input2.lenP1 <= 1 || input2.arrayZ[0] != ننوت.arrayZ[0])
	      اكسر;
	  }
	}
      }
    }
    إذا (likely (unsafe_منزل != (بینښه) -1))
      c->بفر->unsafe_منزل_concat (c->بفر->idx, unsafe_منزل);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_klass_map = تش_اشاره,
	       ثابت hb_map_t *ننوت_klass_map = تش_اشاره,
	       ثابت hb_map_t *lookahead_klass_map = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);

    خپلکاری snap = c->serializer->snapshot ();
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    لكل (ثابت خپلکاری& _ : قاعده_2)
    {
      إذا (!_) استمر;
      خپلکاری o_snap = c->serializer->snapshot ();
      خپلکاری *o = بهر->قاعده_2.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا,
				lookup_map,
				backtrack_klass_map,
				ننوت_klass_map,
				lookahead_klass_map))
      {
	بهر->قاعده_2.راوباسه ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (بهر->قاعده_2);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (قاعده_2.sanitize (c, هذا));
  }

  محمي:
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Chain_قاعده_Type>>
		قاعده_2;			/* Array of ChainRule tables
					 * ordered by preference */
  عام:
  DEFINE_اندازه_ARRAY (2, قاعده_2);
};

کينډۍ <ډول_نوم Types>
بنية Chain_قاعده_سېټ : Chain_قاعده_سېټ_د<Types, Chain_قاعده<Types>> {};

static_assert (حجم (Chain_قاعده_سېټ<MediumTypes>) ==
	       HBUINT16::static_اندازه + Offset24::static_اندازه, "");

کينډۍ <ډول_نوم Types>
بنية Chain_Class_قاعده_سېټ : Chain_قاعده_سېټ_د<Types, Chain_قاعده<SmallTypes>> {};

کينډۍ <ډول_نوم Types>
بنية ChainContextFormat1_4
{
  کارول Chain_قاعده_سېټ = OT::Chain_قاعده_سېټ<Types>;

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    بنية Chain_Context_تړون_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {تش_اشاره, تش_اشاره, تش_اشاره}
    };

    أرجع
    + hb_zip (هذا+coverage, قاعده_سېټ_2)
    | hb_filter (*glyphs, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_map ([&] (ثابت Chain_قاعده_سېټ &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع ریښتیا; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (*c->glyphs, *cur_فعال_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {تش_اشاره, تش_اشاره, تش_اشاره},
      {تش_اشاره, تش_اشاره, تش_اشاره},  /* intersects_cache */
      تش_اشاره                        /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((بینښه) قاعده_سېټ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->پلار_فعال_glyphs ().has (_);
    }, hb_لومړی)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بینښه> _) { أرجع hb_pair_t<بینښه, ثابت Chain_قاعده_سېټ&> (_.لومړی, هذا+قاعده_سېټ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بینښه, ثابت Chain_قاعده_سېټ&>& _) { _.second.depend (c, _.لومړی, lookup_context); })
    ;

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (c->پخوانی_پلار_فعال_glyphs (),
				   *cur_فعال_glyphs);

    بنية Chain_Context_تړون_Lookup_Context lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {تش_اشاره, تش_اشاره, تش_اشاره}
    };

    + hb_zip (هذا+coverage, hb_range ((بینښه) قاعده_سېټ_2.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->پخوانی_پلار_فعال_glyphs ().has (_);
    }, hb_لومړی)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, بینښه> _) { أرجع hb_pair_t<بینښه, ثابت Chain_قاعده_سېټ&> (_.لومړی, هذا+قاعده_سېټ_2[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<بینښه, ثابت Chain_قاعده_سېټ&>& _) { _.second.تړون (c, _.لومړی, lookup_context); })
    ;

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت
  {
    بنية Chain_Context_تړون_Lookup_Context lookup_context = {
      {intersects_glyph, تش_اشاره},
      ContextFormat::SimpleContext,
      {تش_اشاره, تش_اشاره, تش_اشاره}
    };

    + hb_zip (هذا+coverage, قاعده_سېټ_2)
    | hb_filter (*c->glyphs, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_سېټ &_) { _.تړون_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->ننوت);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {تش_اشاره, تش_اشاره, تش_اشاره}
    };

    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_سېټ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  {
    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([digest] (ثابت Chain_قاعده_سېټ &_) { _.collect_دوهم_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت Chain_قاعده_سېټ &قاعده_سېټ_3 = هذا+قاعده_سېټ_2[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{سمون_glyph, سمون_glyph, سمون_glyph}},
      {تش_اشاره, تش_اشاره, تش_اشاره}
    };
    أرجع قاعده_سېټ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت Chain_قاعده_سېټ &قاعده_سېټ_3 = هذا+قاعده_سېټ_2[شاخص_2];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{سمون_glyph, سمون_glyph, سمون_glyph}},
      {تش_اشاره, تش_اشاره, تش_اشاره}
    };
    return_trace (قاعده_سېټ_3.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    ثابت hb_map_t *lookup_map = c->جدول_نښان_2 == HB_OT_نښان_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, قاعده_سېټ_2)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (subset_دننه_واټن_array (c, بهر->قاعده_سېټ_2, هذا, lookup_map), hb_دوهم)
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    بهر->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && قاعده_سېټ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Chain_قاعده_سېټ>>
		قاعده_سېټ_2;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, قاعده_سېټ_2);
};

کينډۍ <ډول_نوم Types>
بنية ChainContextFormat2_5
{
  کارول Chain_قاعده_سېټ = OT::Chain_Class_قاعده_سېټ<Types>;

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &ننوت_class_def = هذا+ننوت_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية Chain_Context_تړون_Lookup_Context lookup_context = {
      {intersects_class, تش_اشاره},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ننوت_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_سېټ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سېټ (*glyphs, retained_coverage_glyphs);

    hb_سېټ_t coverage_glyph_classes;
    ننوت_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    أرجع
    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<بینښه, ثابت Chain_قاعده_سېټ &> p)
	      { أرجع ننوت_class_def.intersects_class (glyphs, p.لومړی) &&
		       coverage_glyph_classes.has (p.لومړی) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع ریښتیا; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (*c->glyphs, *cur_فعال_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &ننوت_class_def = هذا+ننوت_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ننوت_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعده_سېټ_2)
    | hb_filter ([&] (بینښه _)
    { أرجع ننوت_class_def.intersects_class (&c->پلار_فعال_glyphs (), _); },
		 hb_لومړی)
    | hb_apply ([&] (ثابت hb_pair_t<بینښه, ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<Chain_قاعده_سېټ>&> _)
                {
                  ثابت Chain_قاعده_سېټ& chainrule_سېټ = هذا+_.second;
                  chainrule_سېټ.depend (c, _.لومړی, lookup_context);
                })
    ;

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs)) أرجع;
    get_coverage ().intersect_سېټ (c->پخوانی_پلار_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &ننوت_class_def = هذا+ننوت_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    بنية Chain_Context_تړون_Lookup_Context lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ننوت_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (قاعده_سېټ_2)
    | hb_filter ([&] (بینښه _)
    { أرجع ننوت_class_def.intersects_class (&c->پلار_فعال_glyphs (), _); },
		 hb_لومړی)
    | hb_apply ([&] (ثابت hb_pair_t<بینښه, ثابت ډول_نوم Types::کينډۍ دننه_واټن_منزل<Chain_قاعده_سېټ>&> _)
                {
                  ثابت Chain_قاعده_سېټ& chainrule_سېټ = هذا+_.second;
                  chainrule_سېټ.تړون (c, _.لومړی, lookup_context);
                })
    ;

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &ننوت_class_def = هذا+ننوت_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية Chain_Context_تړون_Lookup_Context lookup_context = {
      {intersects_class, تش_اشاره},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &ننوت_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_enumerate
    | hb_filter([&] (بینښه klass)
    { أرجع ننوت_class_def.intersects_class (c->glyphs, klass); }, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_apply ([&] (ثابت Chain_قاعده_سېټ &_)
    { _.تړون_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->ننوت);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &ننوت_class_def = هذا+ننوت_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &ننوت_class_def,
       &lookahead_class_def}
    };

    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([&] (ثابت Chain_قاعده_سېټ &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  {
    ثابت ClassDef &ننوت_class_def = هذا+ننوت_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;
    + hb_iter (قاعده_سېټ_2)
    | hb_map (hb_جمع_کړه (هذا))
    | hb_apply ([digest, &ننوت_class_def, &lookahead_class_def] (ثابت Chain_قاعده_سېټ &_)
		{ _.collect_دوهم_glyphs (digest, &ننوت_class_def, &lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &ننوت_class_def = هذا+ننوت_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بینښه صحيح شاخص_2 = ننوت_class_def.get_class (c->glyphs[0]);
    ثابت Chain_قاعده_سېټ &قاعده_سېټ_3 = هذا+قاعده_سېټ_2[شاخص_2];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{سمون_class, سمون_class, سمون_class}},
      {&backtrack_class_def,
       &ننوت_class_def,
       &lookahead_class_def}
    };
    أرجع قاعده_سېټ_3.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  بینښه cache_لګښت () ثابت
  {
    أرجع (هذا+ننوت_Class_Def).لګښت () + (هذا+lookaheadClassDef).لګښت ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_دوګونی_cache_t coverage;
    hb_ot_layout_mapping_cache_t ننوت_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t قاعده_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    بینښه شمېر = قاعده_سېټ_2.len;
    بینښه اندازه = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    شمېر * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (اندازه);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->ننوت_class.clear ();
      cache->lookahead_class.clear ();
      لكل (بینښه i = 0; i < شمېر; i++)
	(هذا+قاعده_سېټ_2[i]).collect_لومړی_ننوت_classes (&cache->قاعده_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, ریښتیا, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage_دوګونی (c->بفر->cur().codepoint, cache ? &cache->coverage : تش_اشاره);
#وإلا
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
#د_که_پای
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &ننوت_class_def = هذا+ننوت_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    سمون_class_cache_معلومات_t ننوت_cache_معلومات = {
      &ننوت_class_def,
      cache ? &cache->ننوت_class : تش_اشاره
    };
    سمون_class_cache_معلومات_t lookahead_cache_معلومات = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : تش_اشاره
    };
    ثابت فراغ *ننوت_سمون_معلومات = cached
      ? static_cast<ثابت فراغ *> (&ننوت_class_def)
      : static_cast<ثابت فراغ *> (&ننوت_cache_معلومات);
    ثابت فراغ *lookahead_سمون_معلومات = cached
      ? static_cast<ثابت فراغ *> (&lookahead_class_def)
      : static_cast<ثابت فراغ *> (&lookahead_cache_معلومات);
    منطقي backtrack_کاروي_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ثابت فراغ *backtrack_سمون_معلومات = backtrack_کاروي_lookahead_cache
      ? static_cast<ثابت فراغ *> (&lookahead_cache_معلومات)
      : static_cast<ثابت فراغ *> (&backtrack_class_def);
#د_که_پای

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    بنية ChainContextApplyLookupContext lookup_context = {
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? سمون_class_cached1 :
        backtrack_کاروي_lookahead_cache ? سمون_class_cache : سمون_class,
        cached ? سمون_class_cached2 : سمون_class_cache,
        cached ? سمون_class_cached1 : سمون_class_cache}},
      {backtrack_سمون_معلومات,
       ننوت_سمون_معلومات,
       lookahead_سمون_معلومات},
      cached ? get_class_cached2_ارزښت : get_class_cache_ارزښت
#وإلا
      {{cached && &backtrack_class_def == &lookahead_class_def ? سمون_class_cached1 : سمون_class,
        cached ? سمون_class_cached2 : سمون_class,
        cached ? سمون_class_cached1 : سمون_class}},
      {&backtrack_class_def,
       &ننوت_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_ارزښت : get_class_ارزښت
#د_که_پای
    };

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    شاخص_2 = cached
         ? get_class_cached2 (ننوت_class_def, c->بفر->cur())
         : ننوت_class_def.get_class (c->بفر->cur().codepoint, ننوت_cache_معلومات.cache);
#وإلا
    شاخص_2 = cached
         ? get_class_cached2 (ننوت_class_def, c->بفر->cur())
          : ننوت_class_def.get_class (c->بفر->cur().codepoint);
#د_که_پای
    ثابت Chain_قاعده_سېټ &قاعده_سېټ_3 = هذا+قاعده_سېټ_2[شاخص_2];
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && شاخص_2 < قاعده_سېټ_2.len ? &cache->قاعده_sets[شاخص_2] : تش_اشاره;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = تش_اشاره;
#د_که_پای
    return_trace (قاعده_سېټ_3.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;
    بهر->coverage.serialize_subset (c, coverage, هذا);

    hb_map_t backtrack_klass_map;
    hb_map_t ننوت_klass_map;
    hb_map_t lookahead_klass_map;

    بهر->backtrackClassDef.serialize_subset (c, backtrackClassDef, هذا, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    بهر->ننوت_Class_Def.serialize_subset (c, ننوت_Class_Def, هذا, &ننوت_klass_map);
    بهر->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, هذا, &lookahead_klass_map);

    إذا (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   ننوت_klass_map,
						   lookahead_klass_map)))
      return_trace (خطأ);

    ثابت hb_سېټ_t* glyphset = c->plan->glyphset_gsub ();
    hb_سېټ_t retained_coverage_glyphs;
    (هذا+coverage).intersect_سېټ (*glyphset, retained_coverage_glyphs);

    hb_سېټ_t coverage_glyph_classes;
    (هذا+ننوت_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    صحيح non_zero_شاخص = -1, شاخص_2 = 0;
    منطقي ret = ریښتیا;
    ثابت hb_map_t *lookup_map = c->جدول_نښان_2 == HB_OT_نښان_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    خپلکاری وروستی_non_zero = c->serializer->snapshot ();
    لكل (ثابت خپلکاری& _ : + hb_enumerate (قاعده_سېټ_2)
			 | hb_filter (ننوت_klass_map, hb_لومړی))
    {
      خپلکاری *o = بهر->قاعده_سېټ_2.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }
      إذا (coverage_glyph_classes.has (_.لومړی) &&
          o->serialize_subset (c, _.second, هذا,
			       lookup_map,
			       &backtrack_klass_map,
			       &ننوت_klass_map,
			       &lookahead_klass_map))
      {
        وروستی_non_zero = c->serializer->snapshot ();
	non_zero_شاخص = شاخص_2;
      }

      شاخص_2++;
    }

    إذا (!ret || non_zero_شاخص == -1) return_trace (خطأ);

    // prune empty trailing ruleSets
    إذا (شاخص_2 > non_zero_شاخص) {
      c->serializer->revert (وروستی_non_zero);
      بهر->قاعده_سېټ_2.len = non_zero_شاخص + 1;
    }

    return_trace (منطقي (بهر->قاعده_سېټ_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) &&
		  backtrackClassDef.sanitize (c, هذا) &&
		  ننوت_Class_Def.sanitize (c, هذا) &&
		  lookaheadClassDef.sanitize (c, هذا) &&
		  قاعده_سېټ_2.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  ډول_نوم Types::کينډۍ LOffset_منزل<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  ډول_نوم Types::کينډۍ LOffset_منزل<ClassDef>
		ننوت_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  ډول_نوم Types::کينډۍ LOffset_منزل<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Chain_قاعده_سېټ>>
		قاعده_سېټ_2;		/* Array of ChainRuleSet tables
					 * ordered by class */
  عام:
  DEFINE_اندازه_ARRAY (4 + 4 * Types::LOffset::static_اندازه, قاعده_سېټ_2);
};

کينډۍ <ډول_نوم Types>
بنية ChainContextFormat3
{
  کارول قاعده_سېټ = OT::قاعده_سېټ<SmallTypes>;

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);

    إذا (!(هذا+ننوت[0]).intersects (glyphs))
      أرجع خطأ;

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    بنية Chain_Context_تړون_Lookup_Context lookup_context = {
      {intersects_coverage, تش_اشاره},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     ننوت.len, ننوت.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع ریښتیا; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);

    إذا (!(هذا+ننوت[0]).intersects (c->glyphs))
      أرجع;

    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs))
      أرجع;
    get_coverage ().intersect_سېټ (*c->glyphs, *cur_فعال_glyphs);

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا},
      {تش_اشاره, تش_اشاره, تش_اشاره},  /* intersects_cache */
      تش_اشاره                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ثابت HBUINT16 *) backtrack.arrayZ,
				 ننوت.len, (ثابت HBUINT16 *) ننوت.arrayZ + 1,
				 lookahead.len, (ثابت HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 ننوت[0], lookup_context);

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);

    إذا (!(هذا+ننوت[0]).intersects (c->glyphs))
      أرجع;

    hb_سېټ_t* cur_فعال_glyphs = c->ټېل_cur_فعال_glyphs ();
    إذا (unlikely (!cur_فعال_glyphs))
      أرجع;
    get_coverage ().intersect_سېټ (c->پخوانی_پلار_فعال_glyphs (),
				   *cur_فعال_glyphs);

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية Chain_Context_تړون_Lookup_Context lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    chain_context_تړون_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  ننوت.len, ننوت.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->راوباسه_cur_بشپړ_glyphs ();
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;

    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);

    (هذا+ننوت[0]).collect_coverage (c->ننوت);

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {هذا, هذا, هذا}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 ننوت.len, ننوت.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    إذا (ننوت.len > 1)
      (هذا+ننوت[1]).collect_coverage (digest);
    وإلا إذا (lookahead.len)
      (هذا+lookahead[0]).collect_coverage (digest);
    وإلا
      *digest = hb_سېټ_digest_t::ډک ();
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{سمون_coverage, سمون_coverage, سمون_coverage}},
      {هذا, هذا, هذا}
    };
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     ننوت.len, ننوت.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت
  {
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    أرجع هذا+ننوت[0];
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);

    بینښه صحيح شاخص_2 = (هذا+ننوت[0]).get_coverage (c->بفر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{سمون_coverage, سمون_coverage, سمون_coverage}},
      {هذا, هذا, هذا}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      ننوت.len, ننوت.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ثابت فراغ* بنسټ) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed<Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>> ();
    إذا (unlikely (!بهر->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (خپلکاری& دننه_واټن : it) {
      خپلکاری *o = بهر->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, دننه_واټن, بنسټ))
        return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);

    إذا (!serialize_coverage_offsets (c, backtrack.iter (), هذا))
      return_trace (خطأ);

    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    إذا (!serialize_coverage_offsets (c, ننوت.iter (), هذا))
      return_trace (خطأ);

    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    إذا (!serialize_coverage_offsets (c, lookahead.iter (), هذا))
      return_trace (خطأ);

    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ثابت hb_map_t *lookup_map = c->جدول_نښان_2 == HB_OT_نښان_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_شمېر_2 = c->serializer->copy<HBUINT16> (lookup.len);
    إذا (!lookup_شمېر_2) return_trace (خطأ);

    بینښه شمېر = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_ټاکل (*lookup_شمېر_2, شمېر, HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!backtrack.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت خپلکاری &ننوت = StructAfter<decltype (ننوت_X)> (backtrack);
    إذا (unlikely (!ننوت.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!ننوت.len)) return_trace (خطأ); /* To be consistent with Context. */
    ثابت خپلکاری &lookahead = StructAfter<decltype (lookaheadX)> (ننوت);
    إذا (unlikely (!lookahead.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت خپلکاری &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>
		ننوت_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16_د<ډول_نوم Types::کينډۍ دننه_واټن_منزل<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16_د<Lookup_ریکارډ>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_اندازه_MIN (10);
};

بنية ChainContext
{
  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#د_که_پای
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#د_که_پای
  } u;
};


کينډۍ <ډول_نوم T>
بنية ExtensionFormat1
{
  بینښه صحيح get_type () ثابت { أرجع extensionLookupType; }

  کينډۍ <ډول_نوم X>
  ثابت X& get_subtable () ثابت
  { أرجع هذا + reinterpret_cast<ثابت Offset32_منزل<ډول_نوم T::Sub_جدول> &> (extension_دننه_واټن); }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, هذا))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, format);
    return_trace (get_subtable<ډول_نوم T::Sub_جدول> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
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

    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    بهر->format = format;
    بهر->extensionLookupType = extensionLookupType;

    ثابت خپلکاری& src_دننه_واټن =
        reinterpret_cast<ثابت Offset32_منزل<ډول_نوم T::Sub_جدول> &> (extension_دننه_واټن);
    خپلکاری& dest_دننه_واټن =
        reinterpret_cast<Offset32_منزل<ډول_نوم T::Sub_جدول> &> (بهر->extension_دننه_واټن);

    return_trace (dest_دننه_واټن.serialize_subset (c, src_دننه_واټن, هذا, get_type ()));
  }

  محمي:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extension_دننه_واټن;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  عام:
  DEFINE_اندازه_STATIC (8);
};

کينډۍ <ډول_نوم T>
بنية Extension
{
  بینښه صحيح get_type () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_type ();
    افتراضي:أرجع 0;
    }
  }
  کينډۍ <ډول_نوم X>
  ثابت X& get_subtable () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.کينډۍ get_subtable<ډول_نوم T::Sub_جدول> ();
    افتراضي:أرجع Null (ډول_نوم T::Sub_جدول);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  کينډۍ <ډول_نوم ...Ts>
  ډول_نوم hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.subset (c);
    افتراضي: أرجع c->default_return_ارزښت ();
    }
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_ارزښت ());
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
  کينډۍ <ډول_نوم TLookup>
  ساكن hb_ot_layout_lookup_accelerator_t *create (ثابت TLookup &lookup)
  {
    بینښه شمېر = lookup.get_subtable_شمېر ();

    بینښه اندازه = حجم (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    شمېر * حجم (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    خپلکاری *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, اندازه);
    إذا (unlikely (!thiz))
      أرجع تش_اشاره;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    طالما (شمېر && !thiz->subtables[شمېر - 1].apply_func)
      شمېر--;

    thiz->digest.init ();
    لكل (خپلکاری& subtable : hb_iter (thiz->subtables, شمېر))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_دوهم = c_accelerate_subtables.digest_دوهم;

    thiz->شمېر = شمېر;

#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    لكل (بینښه i = 0; i < شمېر; i++)
      إذا (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#د_که_پای

    أرجع thiz;
  }

  فراغ fini ()
  {
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    لكل (بینښه i = 0; i < شمېر; i++)
      hb_free (subtables[i].external_cache);
#د_که_پای
  }

  منطقي may_have (hb_codepoint_t g) ثابت
  { أرجع digest.may_have (g); }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  منطقي apply (hb_ot_apply_context_t *c, منطقي use_cache) ثابت
  {
    إذا (شمېر == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
      إذا (use_cache)
	أرجع subtables[0].apply_cached_no_digest (c);
#د_که_پای
      أرجع subtables[0].apply_no_digest (c);
    }
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (use_cache)
    {
      أرجع
      + hb_iter (hb_iter (subtables, شمېر))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply_cached (c); })
      | hb_any
      ;
    }
    وإلا
#د_که_پای
    {
      أرجع
      + hb_iter (hb_iter (subtables, شمېر))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply (c); })
      | hb_any
      ;
    }
    أرجع خطأ;
  }

  منطقي cache_enter (hb_ot_apply_context_t *c) ثابت
  {
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    أرجع subtable_cache_user_idx != (بینښه) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#وإلا
    أرجع خطأ;
#د_که_پای
  }
  فراغ cache_leave (hb_ot_apply_context_t *c) ثابت
  {
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#د_که_پای
  }


  hb_سېټ_digest_t digest;
  hb_سېټ_digest_t digest_دوهم;
  خاص:
  بینښه شمېر = 0; /* Number of subtables in the array. */
#که_نه_تعریف_شوی HB_NO_OT_LAYOUT_LOOKUP_CACHE
  بینښه subtable_cache_user_idx = (بینښه) -1;
#د_که_پای
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

بنية GSUBGPOS
{
  کارول LookupListT = LookupList<SmallTypes>;
  کارول LookupList2T = LookupList<MediumTypes>;
  کينډۍ <ډول_نوم TLookup>
  کارول TLookupList = List16_د_دننه_واټن_منزل<TLookup, ډول_نوم SmallTypes::HBLUINT>;
  کينډۍ <ډول_نوم TLookup>
  کارول TLookupList2 = List16_د_دننه_واټن_منزل<TLookup, ډول_نوم MediumTypes::HBLUINT>;
  کينډۍ <ډول_نوم TLookup>
  کارول TLookup_دننه_واټن_List = Lookup_دننه_واټن_List<TLookup, ډول_نوم SmallTypes::HBLUINT>;
  کينډۍ <ډول_نوم TLookup>
  کارول TLookup_دننه_واټن_List2 = Lookup_دننه_واټن_List<TLookup, ډول_نوم MediumTypes::HBLUINT>;

  size_t get_اندازه () ثابت
  {
    إذا (نسخه.major != 1) أرجع نسخه.static_اندازه;

    أرجع min_اندازه +
	   (نسخه.منزل_int () >= 0x00010001u ? featureVars.static_اندازه : 0) +
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
	   (نسخه.منزل_int () >= 0x00010002u ? 3 * Offset32::static_اندازه : 0)
#وإلا
	   0
#د_که_پای
	   ;
  }

  ثابت ScriptList &get_script_list () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010002u && scriptList2) أرجع هذا + scriptList2;
#د_که_پای
    أرجع هذا + scriptList;
  }
  ثابت FeatureList &get_feature_list () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010002u && featureList2) أرجع هذا + featureList2;
#د_که_پای
    أرجع هذا + featureList;
  }
  کينډۍ <ډول_نوم TLookup>
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
    منطقي lookup_list_ok = typed_lookup_list_دننه_واټن<TLookup> ().sanitize (c, هذا);

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010002u)
    {
      إذا (scriptList2) script_list_ok = scriptList2.sanitize (c, هذا);
      إذا (featureList2) feature_list_ok = featureList2.sanitize (c, هذا);
      إذا (lookupList2) lookup_list_ok = typed_lookup_list2_دننه_واټن<TLookup> ().sanitize (c, هذا);
    }
#د_که_پای

    إذا (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (خطأ);

#که_نه_تعریف_شوی HB_NO_VAR
    إذا (unlikely (!(نسخه.منزل_int () < 0x00010001u ||
		    featureVars.sanitize (c, هذا, &get_feature_list ()))))
      return_trace (خطأ);
#د_که_پای

    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم TLookup>
  منطقي subset (hb_subset_layout_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (نسخه.major != 1) return_trace (خطأ);

    خپلکاری *بهر = c->subset_context->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->subset_context->serializer->extend_min (بهر))) return_trace (خطأ);

    بهر->نسخه = نسخه;

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010002u)
    {
      إذا (unlikely (!c->subset_context->serializer->extend_اندازه (بهر, get_اندازه ())))
        return_trace (خطأ);

      بهر->scriptList = 0;
      بهر->featureList = 0;
      بهر->lookupList = 0;

      إذا (lookupList2)
	serialize_subset_دننه_واټن (c->subset_context, بهر->lookupList2,
				 subset_lookup_list2_دننه_واټن<TLookup> (),
				 هذا, c);
      وإلا
	serialize_subset_دننه_واټن (c->subset_context, بهر->lookupList2,
				 subset_lookup_list_دننه_واټن<TLookup> (),
				 هذا, c);

      إذا (featureList2)
	serialize_subset_دننه_واټن (c->subset_context, بهر->featureList2,
				 subset_feature_list2_دننه_واټن (),
				 هذا, c);
      وإلا
	serialize_subset_دننه_واټن (c->subset_context, بهر->featureList2,
				 subset_feature_list_دننه_واټن (),
				 هذا, c);

      إذا (scriptList2)
	serialize_subset_دننه_واټن (c->subset_context, بهر->scriptList2, scriptList2, هذا, c);
      وإلا
	serialize_subset_دننه_واټن (c->subset_context, بهر->scriptList2, scriptList, هذا, c);
    }
    وإلا
#د_که_پای
    {
      بهر->subset_lookup_list_دننه_واټن<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_دننه_واټن<TLookup> (),
			     هذا,
			     c);

      بهر->subset_feature_list_دننه_واټن ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_دننه_واټن (),
			     هذا,
			     c);

      بهر->scriptList.serialize_subset (c->subset_context,
					scriptList,
					هذا,
					c);
    }

#که_نه_تعریف_شوی HB_NO_VAR
    إذا (نسخه.منزل_int () >= 0x00010001u)
    {
      خپلکاری snapshot = c->subset_context->serializer->snapshot ();
      إذا (نسخه.منزل_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&بهر->featureVars))
        return_trace (خطأ);

      // if all axes are pinned all feature vars are dropped.
      منطقي ret = !c->subset_context->plan->all_axes_pinned
                 && بهر->featureVars.serialize_subset (c->subset_context, featureVars, هذا, c);
      إذا (!ret)
      {
        بهر->featureVars = 0;
	إذا (نسخه.منزل_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  بهر->نسخه.major = 1;
	  بهر->نسخه.minor = 0;
	}
      }
    }
#د_که_پای

    return_trace (ریښتیا);
  }

  بینښه صحيح get_lookup_شمېر () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2).len;
#د_که_پای
    أرجع (هذا + lookupList).len;
  }
  ثابت Lookup& get_lookup (بینښه صحيح i) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2)[i];
#د_که_پای
    أرجع (هذا + lookupList)[i];
  }
  ثابت FeatureVariations &get_feature_variations () ثابت
  {
    أرجع (نسخه.منزل_int () >= 0x00010001u && hb_barrier ()) ? هذا+featureVars : Null (FeatureVariations);
  }

  منطقي has_معلومات () ثابت { أرجع نسخه.منزل_int (); }
  بینښه صحيح get_script_شمېر () ثابت
  { أرجع get_script_list ().len; }
  ثابت نښان& get_script_نښان (بینښه صحيح i) ثابت
  { أرجع get_script_list ().get_نښان (i); }
  بینښه صحيح get_script_نښانونه (بینښه صحيح پیل_دننه_واټن,
				بینښه صحيح *script_شمېر /* IN/OUT */,
				hb_نښان_t     *script_نښانونه /*بهر*/) ثابت
  { أرجع get_script_list ().get_نښانونه (پیل_دننه_واټن, script_شمېر, script_نښانونه); }
  ثابت Script& get_script (بینښه صحيح i) ثابت
  { أرجع get_script_list ()[i]; }
  منطقي پیدا_کړه_script_شاخص (hb_نښان_t نښان_2, بینښه صحيح *شاخص_2) ثابت
  { أرجع get_script_list ().پیدا_کړه_شاخص (نښان_2, شاخص_2); }

  بینښه صحيح get_feature_شمېر () ثابت
  { أرجع get_feature_list ().len; }
  hb_نښان_t get_feature_نښان (بینښه صحيح i) ثابت
  { أرجع i == شاخص::NOT_وموندل_شاخص ? HB_نښان_NONE : get_feature_list ().get_نښان (i); }
  بینښه صحيح get_feature_نښانونه (بینښه صحيح پیل_دننه_واټن,
				 بینښه صحيح *feature_شمېر_2 /* IN/OUT */,
				 hb_نښان_t     *feature_نښانونه /*بهر*/) ثابت
  { أرجع get_feature_list ().get_نښانونه (پیل_دننه_واټن, feature_شمېر_2, feature_نښانونه); }
  ثابت Feature& get_feature (بینښه صحيح i) ثابت
  { أرجع get_feature_list ()[i]; }
  منطقي پیدا_کړه_feature_شاخص (hb_نښان_t نښان_2, بینښه صحيح *شاخص_2) ثابت
  { أرجع get_feature_list ().پیدا_کړه_شاخص (نښان_2, شاخص_2); }

  منطقي پیدا_کړه_variations_شاخص (ثابت صحيح *coords, بینښه صحيح عدد_coords,
			      بینښه صحيح *شاخص_2,
			      توکی_Var_Store_Instancer *instancer) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    *شاخص_2 = FeatureVariations::NOT_وموندل_شاخص;
    أرجع خطأ;
#د_که_پای
    أرجع get_feature_variations ().پیدا_کړه_شاخص (coords, عدد_coords, شاخص_2, instancer);
  }
  منطقي get_feature_variations_state (ثابت صحيح *coords,
				     بینښه صحيح عدد_coords,
				     بینښه صحيح *شاخص_2,
				     توکی_Var_Store_Instancer *instancer,
				     hb_ویکټور_t<بینښه> *lookup_conditions) ثابت
  {
    lookup_conditions->reset ();
#که_تعریف_شوی HB_NO_VAR
    *شاخص_2 = FeatureVariations::NOT_وموندل_شاخص;
    أرجع ریښتیا;
#وإلا
    پیدا_کړه_variations_شاخص (coords, عدد_coords, شاخص_2, instancer);
    أرجع get_feature_variations ().collect_lookup_conditions (
	coords,
	عدد_coords,
	instancer,
	lookup_conditions);
#د_که_پای
  }
  منطقي get_lookup_variations (بینښه feature_شاخص,
                              بینښه variations_شاخص,
                              hb_sorted_array_t<ثابت بینښه> conditions,
                              hb_سېټ_t *lookup_indexes) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع خطأ;
#وإلا
    أرجع get_feature_variations ().get_lookup_variations (
	feature_شاخص, get_feature_variation (feature_شاخص, variations_شاخص),
	conditions, lookup_indexes);
#د_که_پای
  }
  ثابت Feature& get_feature_variation (بینښه صحيح feature_شاخص,
					بینښه صحيح variations_شاخص) ثابت
  {
#که_نه_تعریف_شوی HB_NO_VAR
    إذا (FeatureVariations::NOT_وموندل_شاخص != variations_شاخص &&
	نسخه.منزل_int () >= 0x00010001u)
    {
      ثابت Feature *feature = get_feature_variations ().پیدا_کړه_substitute (variations_شاخص,
									  feature_شاخص);
      إذا (feature)
	أرجع *feature;
    }
#د_که_پای
    أرجع get_feature (feature_شاخص);
  }

  فراغ feature_variation_collect_lookups (ثابت hb_سېټ_t *feature_indexes,
					  ثابت hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map,
					  hb_سېټ_t       *lookup_indexes /*بهر*/) ثابت
  {
#که_نه_تعریف_شوی HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_ریکارډ_cond_idx_map, lookup_indexes);
#د_که_پای
  }

  فراغ collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#که_نه_تعریف_شوی HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#د_که_پای
  }

#که_نه_تعریف_شوی HB_NO_VAR
  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#د_که_پای

  کينډۍ <ډول_نوم TLookup>
  فراغ تړون_lookups (hb_face_t      *face,
			ثابت hb_سېټ_t *glyphs,
			hb_سېټ_t       *lookup_indexes /* IN/OUT */) ثابت
  {
    hb_سېټ_t لیدل_شوی_lookups, inactive_lookups;
    hb_تړون_lookups_context_t c (face, glyphs, &لیدل_شوی_lookups, &inactive_lookups);

    c.سېټ_recurse_func (TLookup::کينډۍ dispatch_recurse_func<hb_تړون_lookups_context_t>);

    لكل (بینښه lookup_شاخص : *lookup_indexes)
      reinterpret_cast<ثابت TLookup &> (get_lookup (lookup_شاخص)).تړون_lookups (&c, lookup_شاخص);

    hb_سېټ_union (lookup_indexes, &لیدل_شوی_lookups);
    hb_سېټ_کمول (lookup_indexes, &inactive_lookups);
  }

  فراغ prune_langsys (ثابت hb_map_t *duplicate_feature_map,
                      ثابت hb_سېټ_t *layout_scripts,
                      hb_hashmap_t<بینښه, hb::unique_ptr<hb_سېټ_t>> *script_langsys_map,
                      hb_سېټ_t       *new_feature_indexes /*بهر*/) ثابت
  {
    hb_prune_langsys_context_t c (هذا, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    بینښه شمېر = get_script_شمېر ();
    لكل (بینښه script_شاخص = 0; script_شاخص < شمېر; script_شاخص++)
    {
      ثابت نښان& نښان_2 = get_script_نښان (script_شاخص);
      إذا (!layout_scripts->has (نښان_2)) استمر;
      ثابت Script& s = get_script (script_شاخص);
      s.prune_langsys (&c, script_شاخص);
    }
  }

  فراغ prune_features (ثابت hb_map_t *lookup_indices, /*دننه*/
		       ثابت hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map, /*دننه*/
		       ثابت hb_hashmap_t<بینښه, ثابت Feature*> *feature_substitutes_map, /*دننه*/
		       hb_سېټ_t       *feature_indices /* IN/OUT */) ثابت
  {
#که_نه_تعریف_شوی HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_سېټ_t alternate_feature_indices;
    get_feature_variations ().تړون_features (lookup_indices, feature_ریکارډ_cond_idx_map, &alternate_feature_indices);
    إذا (unlikely (alternate_feature_indices.دننه_error()))
    {
      feature_indices->err ();
      أرجع;
    }
#د_که_پای

    لكل (بینښه i : hb_iter (feature_indices))
    {
      hb_نښان_t نښان_2 =  get_feature_نښان (i);
      إذا (نښان_2 == HB_نښان ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        استمر;


      ثابت Feature *f = &(get_feature (i));
      ثابت Feature** p = تش_اشاره;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      إذا (!f->featureParams.is_null () &&
          نښان_2 == HB_نښان ('s', 'i', 'z', 'e'))
        استمر;

      إذا (!f->intersects_lookup_indexes (lookup_indices)
#که_نه_تعریف_شوی HB_NO_VAR
          && !alternate_feature_indices.has (i)
#د_که_پای
	  )
	feature_indices->del (i);
    }
  }

  فراغ collect_name_ids (ثابت hb_map_t *feature_شاخص_map,
                         hb_سېټ_t *nameids_منزل_retain, /*بهر*/
                         ثابت hb_hashmap_t<بینښه, ثابت Feature*> *feature_substitutes_map,
                         ثابت hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map HB_ناکارول_شوی,
                         ثابت hb_سېټ_t *catch_all_features,
                         منطقي all_axes_pinned HB_ناکارول_شوی) ثابت
  {
    بینښه شمېر = get_feature_شمېر ();
    لكل (بینښه i = 0 ; i < شمېر; i++)
    {
      إذا (!feature_شاخص_map->has (i)) استمر;
      hb_نښان_t نښان_2 = get_feature_نښان (i);
      ثابت Feature *feature = &get_feature (i);
      ثابت Feature **substitute;
      إذا (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        إذا (catch_all_features->has (i))
          get_feature (i).collect_name_ids (نښان_2, nameids_منزل_retain);
      }
      feature->collect_name_ids (نښان_2, nameids_منزل_retain);
    }
#که_نه_تعریف_شوی HB_NO_VAR
    إذا (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_شاخص_map, feature_ریکارډ_cond_idx_map,
                                                  [هذا] (بینښه i) { أرجع get_feature_نښان (i); },
                                                  nameids_منزل_retain);
#د_که_پای
  }

  کينډۍ <ډول_نوم T>
  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = ریښتیا;
      هذا->جدول = sc.reference_جدول<T> (face);

      إذا (unlikely (هذا->جدول->is_blocklisted (هذا->جدول.get_blob (), face)))
      {
	hb_blob_destroy (هذا->جدول.get_blob ());
	هذا->جدول = hb_blob_get_تشه ();
      }

      هذا->lookup_شمېر = جدول->get_lookup_شمېر ();

      هذا->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (هذا->lookup_شمېر, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->lookup_شمېر = 0;
	هذا->جدول.destroy ();
	هذا->جدول = hb_blob_get_تشه ();
      }
    }
    ~accelerator_t ()
    {
      لكل (بینښه صحيح i = 0; i < هذا->lookup_شمېر; i++)
      {
	خپلکاری *accel = هذا->accels[i].get_relaxed ();
	إذا (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (هذا->accels);
      هذا->جدول.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع جدول.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (بینښه lookup_شاخص) ثابت
    {
      إذا (unlikely (lookup_شاخص >= lookup_شمېر)) أرجع تش_اشاره;

    retry:
      خپلکاری *accel = accels[lookup_شاخص].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (جدول->get_lookup (lookup_شاخص));
	إذا (unlikely (!accel))
	  أرجع تش_اشاره;

	إذا (unlikely (!accels[lookup_شاخص].cmpexch (تش_اشاره, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> جدول;
    بینښه صحيح lookup_شمېر;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  محمي:
  کينډۍ <ډول_نوم TLookup>
  ثابت Offset16_منزل<TLookupList<TLookup>>& typed_lookup_list_دننه_واټن () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<TLookupList<TLookup>>&> (lookupList); }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  کينډۍ <ډول_نوم TLookup>
  ثابت Offset32_منزل<TLookupList2<TLookup>>& typed_lookup_list2_دننه_واټن () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<TLookupList2<TLookup>>&> (lookupList2); }
#د_که_پای

  کينډۍ <ډول_نوم TLookup>
  ثابت Offset16_منزل<TLookup_دننه_واټن_List<TLookup>>& subset_lookup_list_دننه_واټن () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<TLookup_دننه_واټن_List<TLookup>>&> (lookupList); }
  کينډۍ <ډول_نوم TLookup>
  Offset16_منزل<TLookup_دننه_واټن_List<TLookup>>& subset_lookup_list_دننه_واټن ()
  { أرجع reinterpret_cast<Offset16_منزل<TLookup_دننه_واټن_List<TLookup>>&> (lookupList); }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  کينډۍ <ډول_نوم TLookup>
  ثابت Offset32_منزل<TLookup_دننه_واټن_List2<TLookup>>& subset_lookup_list2_دننه_واټن () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<TLookup_دننه_واټن_List2<TLookup>>&> (lookupList2); }
#د_که_پای

  ثابت Offset16_منزل<ریکارډ_List_د_Feature>& subset_feature_list_دننه_واټن () ثابت
  { أرجع reinterpret_cast<ثابت Offset16_منزل<ریکارډ_List_د_Feature>&> (featureList); }
  Offset16_منزل<ریکارډ_List_د_Feature>& subset_feature_list_دننه_واټن ()
  { أرجع reinterpret_cast<Offset16_منزل<ریکارډ_List_د_Feature>&> (featureList); }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  ثابت Offset32_منزل<ریکارډ_List_د_Feature>& subset_feature_list2_دننه_واټن () ثابت
  { أرجع reinterpret_cast<ثابت Offset32_منزل<ریکارډ_List_د_Feature>&> (featureList2); }
#د_که_پای

  ثابت فراغ* get_lookup_list_ډګر_دننه_واټن () ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (نسخه.منزل_int () >= 0x00010002u && lookupList2) أرجع &lookupList2;
#د_که_پای
    أرجع &lookupList;
  }

  کينډۍ <ډول_نوم دننه_واټن_بهر, ډول_نوم دننه_واټن_دننه, ډول_نوم بنسټ_3, ډول_نوم ...Ts>
  ساكن منطقي serialize_subset_دننه_واټن (hb_subset_context_t *c,
				       دننه_واټن_بهر& بهر,
				       ثابت دننه_واټن_دننه& دننه,
				       ثابت بنسټ_3 *بنسټ,
				       Ts&&... ds)
  {
    بهر = 0;
    إذا (دننه.is_null ()) أرجع خطأ;

    خپلکاری *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (بنسټ+دننه, std::forward<Ts> (ds)...);
    إذا (ret) s->جمع_کړه_link (بهر, s->راوباسه_pack ());
    وإلا s->راوباسه_discard ();
    أرجع ret;
  }

  Fixed_نسخه<>نسخه;	/* Version of the GSUB/GPOS table--initially set
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  Offset32_منزل<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32_منزل<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32_منزل<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#د_که_پای
  عام:
  DEFINE_اندازه_MIN (10);
};


} /* namespace OT */


#د_که_پای /* HB_OT_LAYOUT_GSUBGPOS_HH */
