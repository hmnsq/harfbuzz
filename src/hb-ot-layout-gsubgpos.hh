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

#ifndef HB_OT_LAYOUT_GSUBGPOS_HH
#define HB_OT_LAYOUT_GSUBGPOS_HH

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
  template <typename T>
  return_t dispatch (ثابت T &obj) { أرجع obj.intersects (هذا->glyphs); }
  ساكن return_t default_return_قىممەت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  ثابت hb_set_t *glyphs;

  hb_intersects_context_t (ثابت hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

بنية hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, منطقي>
{
  template <typename T>
  return_t dispatch (ثابت T &obj) { أرجع obj.may_have_non_1to1 (); }
  ساكن return_t default_return_قىممەت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
};

بنية hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index);
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.closure (هذا); أرجع hb_empty_t (); }
  ساكن return_t default_return_قىممەت () { أرجع hb_empty_t (); }
  فراغ recurse (unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index)
  {
    إذا (unlikely (nesting_level_سول == 0 || !recurse_func))
      أرجع;

    nesting_level_سول--;
    recurse_func (هذا, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_سول++;
  }

  فراغ reset_lookup_visit_سانى ()
  { lookup_سانى = 0; }

  منطقي lookup_limit_exceeded ()
  { أرجع lookup_سانى > HB_MAX_LOOKUP_VISIT_سانى; }

  منطقي should_visit_lookup (unsigned صحيح lookup_index)
  {
    إذا (lookup_سانى++ > HB_MAX_LOOKUP_VISIT_سانى)
      أرجع خطأ;

    إذا (is_lookup_done (lookup_index))
      أرجع خطأ;

    أرجع true;
  }

  منطقي is_lookup_done (unsigned صحيح lookup_index)
  {
    إذا (unlikely (done_lookups_glyph_سانى->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      أرجع true;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned چىقىرىش = glyphs->get_population ();
    إذا (unlikely (چىقىرىش == HB_MAP_قىممەت_INVALID)) // Avoid sentinel interference.
      چىقىرىش--;
    إذا (done_lookups_glyph_سانى->get (lookup_index) != چىقىرىش)
    {
      done_lookups_glyph_سانى->set (lookup_index, چىقىرىش);

      إذا (!done_lookups_glyph_set->has (lookup_index))
      {
	إذا (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  أرجع true;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    إذا (unlikely (covered_glyph_set->in_error ()))
      أرجع true;
    إذا (parent_active_glyphs ().is_subset (*covered_glyph_set))
      أرجع true;

    covered_glyph_set->union_ (parent_active_glyphs ());
    أرجع خطأ;
  }

  ثابت hb_set_t& previous_parent_active_glyphs () {
    إذا (active_glyphs_دۆۋە.length <= 1)
      أرجع *glyphs;

    أرجع active_glyphs_دۆۋە[active_glyphs_دۆۋە.length - 2];
  }

  ثابت hb_set_t& parent_active_glyphs ()
  {
    إذا (!active_glyphs_دۆۋە)
      أرجع *glyphs;

    أرجع active_glyphs_دۆۋە.tail ();
  }

  hb_set_t* ئىتتىرىش_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_دۆۋە.push ();
    إذا (unlikely (active_glyphs_دۆۋە.in_error ()))
      أرجع nullptr;
    أرجع s;
  }

  منطقي چىقىرىش_cur_done_glyphs ()
  {
    إذا (!active_glyphs_دۆۋە)
      أرجع خطأ;

    active_glyphs_دۆۋە.چىقىرىش ();
    أرجع true;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_دۆۋە;
  recurse_func_t recurse_func = nullptr;
  unsigned صحيح nesting_level_سول;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_سانى_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned صحيح nesting_level_سول_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_سول (nesting_level_سول_2),
			  done_lookups_glyph_سانى (done_lookups_glyph_سانى_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  فراغ set_recurse_func (recurse_func_t func) { recurse_func = func; }

  فراغ flush ()
  {
    output->del_range (face->get_رەقەم_glyphs (), HB_SET_قىممەت_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_دۆۋە.چىقىرىش ();
    active_glyphs_دۆۋە.reset ();
  }

  خاص:
  hb_map_t *done_lookups_glyph_سانى;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned صحيح lookup_سانى = 0;
};



بنية hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_index);
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.closure_lookups (هذا); أرجع hb_empty_t (); }
  ساكن return_t default_return_قىممەت () { أرجع hb_empty_t (); }
  فراغ recurse (unsigned lookup_index)
  {
    إذا (unlikely (nesting_level_سول == 0 || !recurse_func))
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      أرجع;

    nesting_level_سول--;
    recurse_func (هذا, lookup_index);
    nesting_level_سول++;
  }

  فراغ set_lookup_visited (unsigned lookup_index)
  { visited_lookups->add (lookup_index); }

  فراغ set_lookup_inactive (unsigned lookup_index)
  { inactive_lookups->add (lookup_index); }

  منطقي lookup_limit_exceeded ()
  {
    منطقي ret = lookup_سانى > HB_MAX_LOOKUP_VISIT_سانى;
    إذا (ret)
      DEBUG_ئۇچۇر (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    أرجع ret; }

  منطقي is_lookup_visited (unsigned lookup_index)
  {
    إذا (unlikely (lookup_سانى++ > HB_MAX_LOOKUP_VISIT_سانى))
    {
      DEBUG_ئۇچۇر (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_سانى, lookup_index);
      أرجع true;
    }

    إذا (unlikely (visited_lookups->in_error ()))
      أرجع true;

    أرجع visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  ثابت hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned صحيح nesting_level_سول;

  hb_closure_lookups_context_t (hb_face_t *face_,
				ثابت hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				unsigned nesting_level_سول_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_سول (nesting_level_سول_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_سانى (0) {}

  فراغ set_recurse_func (recurse_func_t func) { recurse_func = func; }

  خاص:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned صحيح lookup_سانى;
};

بنية hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, منطقي>
{
  template <typename T>
  return_t dispatch (ثابت T &obj) { أرجع obj.would_apply (هذا); }
  ساكن return_t default_return_قىممەت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }

  hb_face_t *face;
  ثابت hb_codepoint_t *glyphs;
  unsigned صحيح len;
  منطقي zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ثابت hb_codepoint_t *glyphs_,
			    unsigned صحيح len_,
			    منطقي zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

بنية hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  تعريفنوع return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned صحيح lookup_index);
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.collect_glyphs (هذا); أرجع hb_empty_t (); }
  ساكن return_t default_return_قىممەت () { أرجع hb_empty_t (); }
  فراغ recurse (unsigned صحيح lookup_index)
  {
    إذا (unlikely (nesting_level_سول == 0 || !recurse_func))
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

    إذا (output == hb_set_get_empty ())
      أرجع;

    /* Return if new lookup was recursed to before. */
    إذا (recursed_lookups->has (lookup_index))
      أرجع;

    hb_set_t *old_before = before;
    hb_set_t *old_كىرگۈزۈش  = كىرگۈزۈش;
    hb_set_t *old_after  = after;
    before = كىرگۈزۈش = after = hb_set_get_empty ();

    nesting_level_سول--;
    recurse_func (هذا, lookup_index);
    nesting_level_سول++;

    before = old_before;
    كىرگۈزۈش  = old_كىرگۈزۈش;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *كىرگۈزۈش;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned صحيح nesting_level_سول;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_كىرگۈزۈش,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned صحيح nesting_level_سول_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      كىرگۈزۈش  (glyphs_كىرگۈزۈش  ? glyphs_كىرگۈزۈش  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_سول (nesting_level_سول_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  فراغ set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

بنية hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  تعداد glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  ساكن constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  بنية glyph_set_cache_ئاچقۇچ_t
  {
    glyph_set_cache_ئاچقۇچ_t () = افتراضي;
    glyph_set_cache_ئاچقۇچ_t (ثابت فراغ *data_, unsigned قىممەت_3, glyph_set_type_t type_)
      : data (data_), value (قىممەت_3), type (type_) {}

    منطقي operator == (ثابت glyph_set_cache_ئاچقۇچ_t &o) ثابت
    { أرجع data == o.data && value == o.value && type == o.type; }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash ((unsigned) type);
    }

    ثابت فراغ *data = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  بنية context_set_cache_ئاچقۇچ_t
  {
    context_set_cache_ئاچقۇچ_t () = افتراضي;
    context_set_cache_ئاچقۇچ_t (ثابت فراغ *rule_,
			     ثابت فراغ *record_,
			     ثابت فراغ *data_,
			     hb_codepoint_t active_glyphs_,
			     unsigned قىممەت_3,
			     unsigned format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (قىممەت_3), format (format_) {}

    منطقي operator == (ثابت context_set_cache_ئاچقۇچ_t &o) ثابت
    {
      أرجع rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      أرجع current * 31 + hb_hash (format);
    }

    ثابت فراغ *rule = nullptr;
    ثابت فراغ *record = nullptr;
    ثابت فراغ *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  بنية recurse_ئاچقۇچ_t
  {
    recurse_ئاچقۇچ_t () = افتراضي;
    recurse_ئاچقۇچ_t (unsigned lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    منطقي operator == (ثابت recurse_ئاچقۇچ_t &o) ثابت
    {
      أرجع lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () ثابت
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      أرجع current;
    }

    unsigned lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  تعريفنوع return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.depend (هذا); أرجع hb_empty_t (); }
  ساكن return_t default_return_قىممەت () { أرجع hb_empty_t (); }

  فراغ recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
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
    إذا (lookups_seen.has (lookup_idx)) أرجع;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_ئاچقۇچ = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    إذا (!recurse_path_to_index.has (path_ئاچقۇچ, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      إذا (unlikely (!recurse_path_to_index.set (path_ئاچقۇچ, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	أرجع;
      }
    }
    وإلا
      path_index = *path;
    recurse_path = path_index;
    recurse_func (هذا, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  بنية active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    منطقي use_يىلتىز = خطأ;
  };

  ثابت hb_set_t& parent_active_glyphs ()
  {
    إذا (!active_glyphs_دۆۋە_depth)
      أرجع *glyphs;

    ثابت active_glyphs_frame_t &frame = active_glyphs_دۆۋە[active_glyphs_دۆۋە_depth - 1];
    أرجع frame.use_يىلتىز ? *glyphs : frame.glyphs;
  }

  hb_set_t* ئىتتىرىش_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    إذا (active_glyphs_دۆۋە_depth == active_glyphs_دۆۋە.length &&
	unlikely (!active_glyphs_دۆۋە.ئىتتىرىش_or_fail ()))
      أرجع nullptr;

    active_glyphs_frame_t &frame = active_glyphs_دۆۋە[active_glyphs_دۆۋە_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_يىلتىز = خطأ;
    أرجع &frame.glyphs;
  }

  منطقي ئىتتىرىش_يىلتىز_active_glyphs ()
  {
    إذا (active_glyphs_دۆۋە_depth == active_glyphs_دۆۋە.length &&
	unlikely (!active_glyphs_دۆۋە.ئىتتىرىش_or_fail ()))
      أرجع خطأ;

    active_glyphs_دۆۋە[active_glyphs_دۆۋە_depth++].use_يىلتىز = true;
    أرجع true;
  }

  منطقي چىقىرىش_cur_done_glyphs ()
  {
    إذا (!active_glyphs_دۆۋە_depth)
      أرجع خطأ;

    active_glyphs_دۆۋە_depth--;
    أرجع true;
  }

  منطقي get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    إذا (!active_glyphs_دۆۋە_depth ||
	active_glyphs_دۆۋە[active_glyphs_دۆۋە_depth - 1].use_يىلتىز)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      أرجع true;
    }

    active_glyphs_frame_t &frame = active_glyphs_دۆۋە[active_glyphs_دۆۋە_depth - 1];
    *active_idx = frame.active_idx;
    إذا (*active_idx != HB_CODEPOINT_INVALID)
      أرجع true;

    hb_codepoint_t glyph;
    إذا (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    وإلا إذا (!find_or_create_recurse_set (frame.glyphs, active_idx))
      أرجع خطأ;

    frame.active_idx = *active_idx;
    أرجع true;
  }

  منطقي get_context_set_cache_ئاچقۇچ (ثابت فراغ *rule,
				  ثابت فراغ *record,
				  ثابت فراغ *data,
				  unsigned value,
				  unsigned format,
				  context_set_cache_ئاچقۇچ_t *key)
  {
    hb_codepoint_t active_idx;
    إذا (!get_parent_active_glyphs_index (&active_idx))
      أرجع خطأ;
    *key = context_set_cache_ئاچقۇچ_t (rule, record, data, active_idx, value, format);
    أرجع true;
  }

  منطقي get_recurse_ئاچقۇچ (unsigned lookup_idx, recurse_ئاچقۇچ_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    إذا (!get_parent_active_glyphs_index (&active_idx))
      أرجع خطأ;

    *key = recurse_ئاچقۇچ_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    أرجع !completed_recursions.has (*key);
  }

  فراغ finish_recurse (ثابت recurse_ئاچقۇچ_t &key)
  { depend_data->check_success (completed_recursions.set (key, true)); }

  فراغ reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  منطقي find_or_create_recurse_set (ثابت hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    إذا (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      أرجع true;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    إذا (unlikely (!copy))
      أرجع depend_data->fail ();
    copy->set (set);
    إذا (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      أرجع depend_data->fail ();
    }

    *index = recurse_sets.length;
    إذا (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      أرجع depend_data->fail ();
    }
    إذا (unlikely (!recurse_sets.ئىتتىرىش_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      أرجع depend_data->fail ();
    إذا (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      أرجع depend_data->fail ();
    أرجع true;
  }

  ثابت hb_set_t *find_or_create_glyph_set (ثابت فراغ *data,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (ثابت hb_set_t *set);

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
      كىرگۈزۈش_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<ثابت hb_set_t *> backtrack_sets;
    hb_vector_t<ثابت hb_set_t *> lookahead_sets;
    hb_vector_t<ثابت hb_set_t *> كىرگۈزۈش_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<unsigned> compact_workspace;
    depend_scratch_t *next = nullptr;
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
        depend_data->fail ();
        أرجع nullptr;
      }
      جديد (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    أرجع scratch;
  }

  فراغ release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_دۆۋە;
  unsigned active_glyphs_دۆۋە_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<ثابت hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_ئاچقۇچ_t, منطقي> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_ئاچقۇچ_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_ئاچقۇچ_t, hb_codepoint_t> context_set_cache;
  depend_scratch_t *depend_scratch_pool = nullptr;

  hb_depend_context_t (hb_depend_data_builder_t *depend_data_,
                       hb_face_t *face_,
                       hb_set_t *glyphs_)
                      : depend_data (depend_data_),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (nullptr) {}
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
  فراغ set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

template <typename set_t>
بنية hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, ثابت Coverage &>
{
  تعريفنوع ثابت Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (ثابت T &obj) { أرجع obj.get_coverage (); }
  ساكن return_t default_return_قىممەت () { أرجع Null (Coverage); }
  منطقي stop_sublookup_iteration (return_t r) ثابت
  {
    r.collect_coverage (set);
    أرجع خطأ;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

بنية matcher_t
{
  تعريفنوع منطقي (*match_func_t) (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data);

  template <typename context_t>
  فراغ init (ثابت context_t *c, منطقي context_match = خطأ)
  {
    set_match_func (nullptr, nullptr);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->table_index == 1 || (context_match && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_match || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->table_index == 1;
    mask = context_match ? -1 : c->lookup_mask;
    /* Per syllable matching is only for GSUB. */
    per_syllable = c->table_index == 0 && c->per_syllable;
    syllable = 0;
  }

  فراغ set_match_func (match_func_t match_func_,
		       ثابت فراغ *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  تعداد may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_data) ثابت
  {
    إذا (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      أرجع MATCH_NO;

    إذا (match_func)
      أرجع match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    أرجع MATCH_MAYBE;
  }

  تعداد may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  template <typename context_t>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
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
  unsigned صحيح lookup_props = 0;
  hb_mask_t mask = -1;
  منطقي ignore_zwnj = خطأ;
  منطقي ignore_zwj = خطأ;
  منطقي ignore_hidden = خطأ;
  منطقي per_syllable = خطأ;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  ثابت فراغ *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
بنية glyph_data_t
{
  template <typename T>
  glyph_data_t (ثابت T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (حجم (T) == 3)
#endif
  {
    static_assert (حجم (T) == 2 || حجم (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (wide)
      أرجع ((ثابت HBUINT24 *) data)[i];
#endif
    أرجع ((ثابت HBUINT16 *) data)[i];
  }

  ثابت فراغ *data;
#ifndef HB_NO_BEYOND_64K
  منطقي wide;
#endif
};

template <typename context_t>
بنية skipping_iterator_t
{
  فراغ init (context_t *c_, منطقي context_match = خطأ)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  فراغ set_lookup_props (unsigned صحيح lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  فراغ set_match_func (matcher_t::match_func_t match_func_,
		       ثابت فراغ *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  فراغ set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (ثابت HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (ثابت HBUINT24 *) glyph_data.data : nullptr;
#وإلا
    match_glyph_data16 = (ثابت HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  فراغ reset (unsigned صحيح start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  فراغ reset_back (unsigned صحيح start_index_, منطقي from_out_buffer = خطأ)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  فراغ reset_fast (unsigned صحيح start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (ثابت hb_glyph_info_t &info) ثابت
  { أرجع matcher.may_skip (c, info); }

  تعداد match_t {
    MATCH,
    NOT_MATCH,
    SKIP
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  match_t match (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    إذا (unlikely (skip == matcher_t::SKIP_YES))
      أرجع SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    إذا (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      أرجع MATCH;

    إذا (skip == matcher_t::SKIP_NO)
      أرجع NOT_MATCH;

    أرجع SKIP;
  }

  منطقي next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->buffer->info;
    ثابت signed stop = (signed) end - 1;
    طالما ((signed) idx < stop)
    {
      idx++;
      بدل (match (info[idx]))
      {
	حالة MATCH:
	{
	  advance_glyph_data ();
	  أرجع true;
	}
	حالة NOT_MATCH:
	{
	  إذا (unsafe_to)
	    *unsafe_to = idx + 1;
	  أرجع خطأ;
	}
	حالة SKIP:
	  استمر;
      }
    }
    إذا (unsafe_to)
      *unsafe_to = end;
    أرجع خطأ;
  }
  منطقي prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->buffer->out_info;
    ثابت unsigned stop = 0;
    طالما (idx > stop)
    {
      idx--;
      بدل (match (out_info[idx]))
      {
	حالة MATCH:
	{
	  advance_glyph_data ();
	  أرجع true;
	}
	حالة NOT_MATCH:
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
  get_glyph_data ()
  {
    إذا (match_glyph_data16) أرجع *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    وإلا
    إذا (match_glyph_data24) أرجع *match_glyph_data24;
#endif
    أرجع 0;
  }
  HB_ALWAYS_INLINE
  فراغ
  advance_glyph_data ()
  {
    إذا (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    وإلا
    إذا (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned صحيح idx;
  محمي:
  context_t *c;
  matcher_t matcher;
  ثابت HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  ثابت HBUINT24 *match_glyph_data24;
#endif

  unsigned صحيح end;
};

بنية hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, منطقي, HB_DEBUG_APPLY>
{
  ثابت حرف *get_name () { أرجع "APPLY"; }
  تعريفنوع return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned صحيح lookup_index);

  template <typename T>
  ساكن مضمن auto apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  ساكن مضمن auto apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (ثابت T &obj) { أرجع apply_(obj, هذا, hb_prioritize); }

  ساكن return_t default_return_قىممەت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (return_t r) ثابت { أرجع r; }
  return_t recurse (unsigned صحيح sub_lookup_index)
  {
    أكد (recurse_func);
    إذا (unlikely (nesting_level_سول == 0))
    {
      buffer->successful = خطأ;
      أرجع default_return_قىممەت ();
    }

    buffer->max_ops--;
    إذا (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = خطأ;
      أرجع default_return_قىممەت ();
    }

    nesting_level_سول--;
    منطقي ret = recurse_func (هذا, sub_lookup_index);
    nesting_level_سول++;
    أرجع ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_كىرگۈزۈش, iter_context;

  unsigned صحيح table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  ثابت GDEF &gdef;
  ثابت GDEF::accelerator_t &gdef_accel;
  ثابت ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  unsigned صحيح lookup_index = (unsigned) -1;
  unsigned صحيح lookup_props = 0;
  unsigned صحيح nesting_level_سول = HB_MAX_NESTING_LEVEL;

  منطقي has_glyph_classes;
  منطقي auto_zwnj = true;
  منطقي auto_zwj = true;
  منطقي per_syllable = خطأ;
  منطقي random = خطأ;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t دۆۋە_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  فراغ *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (unsigned صحيح table_index_,
			 hb_font_t *font_,
			 hb_buffer_t *buffer_,
			 hb_blob_t *table_blob_,
			 hb_scalar_cache_t *var_store_cache_ = nullptr) :
			table_index (table_index_),
			font (font_), face (font->face), buffer (buffer_),
			sanitizer (table_blob_),
			gdef (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF->table
#وإلا
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#وإلا
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (دۆۋە_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    إذا (buffer->messaging ())
    {
      أكد (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = هذا;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    إذا (buffer->messaging ())
    {
      أكد (buffer->changed_func == buffer_changed_trampoline);
      أكد (buffer->changed_data == هذا);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  فراغ init_iters ()
  {
    iter_كىرگۈزۈش.init (هذا, خطأ);
    iter_context.init (هذا, true);
  }

  فراغ set_lookup_mask (hb_mask_t mask, منطقي init = true) { lookup_mask = mask; last_base = -1; last_base_until = 0; إذا (init) init_iters (); }
  فراغ set_auto_zwj (منطقي auto_zwj_, منطقي init = true) { auto_zwj = auto_zwj_; إذا (init) init_iters (); }
  فراغ set_auto_zwnj (منطقي auto_zwnj_, منطقي init = true) { auto_zwnj = auto_zwnj_; إذا (init) init_iters (); }
  فراغ set_per_syllable (منطقي per_syllable_, منطقي init = true) { per_syllable = per_syllable_; إذا (init) init_iters (); }
  فراغ set_random (منطقي random_) { random = random_; }
  فراغ set_recurse_func (recurse_func_t func) { recurse_func = func; }
  فراغ set_lookup_index (unsigned صحيح lookup_index_) { lookup_index = lookup_index_; }
  فراغ set_lookup_props (unsigned صحيح lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_سان ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    أرجع buffer->random_state;
  }

  ساكن فراغ buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, فراغ *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  فراغ buffer_changed ()
  {
    buffer->update_digest ();
    إذا (likely (has_glyph_classes))
      لكل (unsigned i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  منطقي match_properties_mark (ثابت hb_glyph_info_t *info,
			      unsigned صحيح    glyph_props,
			      unsigned صحيح    match_props) ثابت
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    إذا (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_index = match_props >> 16;
      أرجع gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    إذا (match_props & LookupFlag::MarkAttachmentType)
      أرجع (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    أرجع true;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقي check_glyph_property (ثابت hb_glyph_info_t *info,
			     unsigned match_props) ثابت
  {
    unsigned صحيح glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    إذا (glyph_props & match_props & LookupFlag::IgnoreFlags)
      أرجع خطأ;

    إذا (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      أرجع match_properties_mark (info, glyph_props, match_props);

    أرجع true;
  }

  فراغ _set_glyph_class (hb_codepoint_t glyph_index,
			  unsigned صحيح class_guess = 0,
			  منطقي ligature = خطأ,
			  منطقي component = خطأ)
  {
    buffer->digest.add (glyph_index);

    إذا (new_syllables != (unsigned) -1)
      buffer->cur().syllable() = new_syllables;

    unsigned صحيح props = _hb_glyph_info_get_glyph_props (&buffer->cur());
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
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    وإلا إذا (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    وإلا
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  فراغ _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned صحيح props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  فراغ _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  فراغ replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (فراغ) buffer->replace_glyph (glyph_index);
  }
  فراغ replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  فراغ replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    unsigned صحيح class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, true);
    (فراغ) buffer->replace_glyph (glyph_index);
  }
  فراغ output_glyph_for_component (hb_codepoint_t glyph_index,
				   unsigned صحيح class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, خطأ, true);
    (فراغ) buffer->output_glyph (glyph_index);
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
  template <typename T>
  ساكن مضمن auto collect_second_glyphs_ (ثابت T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (فراغ, obj.collect_second_glyphs (digest) )
  template <typename T>
  ساكن مضمن فراغ collect_second_glyphs_ (ثابت T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  ساكن مضمن auto apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_priority<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  template <typename T>
  ساكن مضمن auto apply_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_priority<0>) HB_RETURN (منطقي, obj->apply (c) )
  template <typename T>
  ساكن مضمن منطقي apply_to (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  ساكن مضمن auto apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_priority<2>) HB_RETURN (منطقي, obj->apply_cached (c, external_cache) )
  template <typename T>
  ساكن مضمن auto apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_priority<1>) HB_RETURN (منطقي, obj->apply (c, external_cache) )
  template <typename T>
  ساكن مضمن auto apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, فراغ *external_cache, hb_priority<0>) HB_RETURN (منطقي, obj->apply (c) )
  template <typename T>
  ساكن مضمن منطقي apply_cached_to (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    أرجع apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  ساكن مضمن auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (منطقي, T::cache_func (c, op) )
  template <typename T=فراغ>
  ساكن مضمن منطقي cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { أرجع خطأ; }
  template <typename Type>
  ساكن مضمن منطقي cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    أرجع cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  تعريفنوع منطقي (*hb_apply_func_t) (ثابت فراغ *obj, hb_ot_apply_context_t *c, فراغ *external_cache);
  تعريفنوع منطقي (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  بنية hb_applicable_t
  {
    friend بنية hb_accelerate_subtables_context_t;
    friend بنية hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    فراغ init (ثابت T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , فراغ *external_cache_
#endif
		)
    {
      obj = &obj_;
      apply_func = apply_func_;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      apply_cached_func = apply_cached_func_;
      cache_func = cache_func_;
      external_cache = external_cache_;
#endif
      digest.init ();
      obj_.get_coverage ().collect_coverage (&digest);
    }

#ifdef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    منطقي apply (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    منطقي apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      أرجع apply_func (obj, c, nullptr);
    }
#وإلا
    منطقي apply (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    منطقي apply_cached (hb_ot_apply_context_t *c) ثابت
    {
      أرجع digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
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
#endif

    خاص:
    ثابت فراغ *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    فراغ *external_cache;
#endif
    hb_set_digest_t digest;
  };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  auto cache_cost (ثابت T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  template <typename T>
  auto cache_cost (ثابت T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  template <typename T>
  auto external_cache_create (ثابت T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  template <typename T>
  auto external_cache_create (ثابت T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ثابت T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    فراغ *external_cache = nullptr;
    إذا (i < 8)
      external_cache = external_cache_create (obj, hb_prioritize);
#endif

    hb_applicable_t *entry = &array[i++];

    entry->init (obj,
		 apply_to<T>
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
		 , apply_cached_to<T>
		 , cache_func_to<T>
		 , external_cache
#endif
		 );

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    /* Cache handling
     *
     * We allow one subtable from each lookup to use a cache. The assumption
     * being that multiple subtables of the same lookup cannot use a cache
     * because the resources they would use will collide.  As such, we ask
     * each subtable to tell us how much it costs (which a cache would avoid),
     * and we allocate the cache opportunity to the costliest subtable.
     */
    unsigned cost = cache_cost (obj, hb_prioritize);
    إذا (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    أرجع hb_empty_t ();
  }
  ساكن return_t default_return_قىممەت () { أرجع hb_empty_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  unsigned i = 0;
  hb_set_digest_t digest_second;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
  unsigned subtable_cache_user_cost = 0;
#endif
};


تعريفنوع منطقي (*intersects_func_t) (ثابت hb_set_t *glyphs, unsigned value, ثابت فراغ *data, فراغ *cache);
تعريفنوع فراغ (*intersected_glyphs_func_t) (ثابت hb_set_t *glyphs, ثابت فراغ *data, unsigned value, hb_set_t *intersected_glyphs, فراغ *cache);
تعريفنوع فراغ (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, ثابت فراغ *data);
تعريفنوع منطقي (*match_func_t) (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data);
تعريفنوع unsigned (*get_قىممەت_func_t) (hb_glyph_info_t &info, ثابت فراغ *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
بنية hb_ot_layout_ruleset_digest_t
{
  فراغ init () { mask = 0; }
  فراغ set_full () { mask = (uint64_t) -1; }
  فراغ add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  منطقي may_have (unsigned value) ثابت { أرجع mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

بنية ContextClosureFuncs
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
  match_func_t match;
};
بنية ChainContextApplyFuncs
{
  match_func_t match[3];
};


ساكن مضمن منطقي intersects_glyph (ثابت hb_set_t *glyphs, unsigned value, ثابت فراغ *data HB_UNUSED, فراغ *cache HB_UNUSED)
{
  أرجع glyphs->has (value);
}
ساكن مضمن منطقي intersects_class (ثابت hb_set_t *glyphs, unsigned value, ثابت فراغ *data, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  إذا (map->has (value, &cached_v))
    أرجع *cached_v;

  منطقي v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  أرجع v;
}
ساكن مضمن منطقي intersects_coverage (ثابت hb_set_t *glyphs, unsigned value, ثابت فراغ *data, فراغ *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  أرجع (data+coverage).intersects (glyphs);
}


ساكن مضمن فراغ intersected_glyph (ثابت hb_set_t *glyphs HB_UNUSED, ثابت فراغ *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED فراغ *cache)
{
  unsigned g = reinterpret_cast<ثابت HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

ساكن مضمن فراغ intersected_class_glyphs (ثابت hb_set_t *glyphs, ثابت فراغ *data, unsigned value, hb_set_t *intersected_glyphs, فراغ *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  إذا (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    أرجع;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

ساكن مضمن فراغ intersected_coverage_glyphs (ثابت hb_set_t *glyphs, ثابت فراغ *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED فراغ *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


ساكن مضمن منطقي array_is_subset_of (ثابت hb_set_t *glyphs,
				       unsigned صحيح سانى,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       ثابت فراغ *intersects_data,
				       فراغ *cache)
{
  لكل (unsigned i = 0; i < سانى; i++)
    إذا (!intersects_func (glyphs, values[i], intersects_data, cache)) أرجع خطأ;
  أرجع true;
}


ساكن مضمن فراغ collect_glyph (hb_set_t *glyphs, unsigned value, ثابت فراغ *data HB_UNUSED)
{
  glyphs->add (value);
}
ساكن مضمن فراغ collect_class (hb_set_t *glyphs, unsigned value, ثابت فراغ *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
ساكن مضمن فراغ collect_coverage (hb_set_t *glyphs, unsigned value, ثابت فراغ *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

مضمن ثابت hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (ثابت فراغ *data,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_ئاچقۇچ_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  إذا (glyph_set_to_index.has (key, &existing))
    أرجع glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  إذا (unlikely (!set))
  {
    depend_data->fail ();
    أرجع nullptr;
  }

  إذا (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  وإلا إذا (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  وإلا
    set->add (value);
  إذا (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    أرجع nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  إذا (unlikely (!glyph_sets.ئىتتىرىش_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    أرجع nullptr;
  }
  إذا (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    أرجع nullptr;
  }
  أرجع glyph_sets[index].get ();
}

مضمن hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (ثابت hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  إذا (cached_context_set_indices.has (key, &existing))
    أرجع *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  إذا (unlikely (index == HB_CODEPOINT_INVALID))
    أرجع HB_CODEPOINT_INVALID;
  إذا (unlikely (!cached_context_set_indices.set (key, index)))
    أرجع depend_data->fail_invalid ();
  أرجع index;
}

ساكن مضمن فراغ collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned صحيح سانى,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  ثابت فراغ *collect_data)
{
  لكل (unsigned i = 0; i < سانى; i++)
    collect_func (glyphs, values[i], collect_data);
}


ساكن مضمن منطقي match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, ثابت فراغ *data HB_UNUSED)
{
  أرجع true;
}
ساكن مضمن منطقي match_glyph (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data HB_UNUSED)
{
  أرجع info.codepoint == value;
}
ساكن مضمن منطقي match_class (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  أرجع class_def.get_class (info.codepoint) == value;
}
ساكن مضمن unsigned get_class_قىممەت (hb_glyph_info_t &info, ثابت فراغ *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (data);
  أرجع class_def.get_class (info.codepoint);
}
بنية match_class_cache_data_t
{
  ثابت ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ساكن مضمن منطقي match_class_cache (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data)
{
  ثابت auto &cache_data = *reinterpret_cast<ثابت match_class_cache_data_t *> (data);
  أرجع cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
ساكن مضمن unsigned get_class_cache_قىممەت (hb_glyph_info_t &info, ثابت فراغ *data)
{
  ثابت auto &cache_data = *reinterpret_cast<ثابت match_class_cache_data_t *> (data);
  أرجع cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
ساكن مضمن unsigned get_class_cached (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  إذا (klass < 255)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 255))
    info.syllable() = klass;
  أرجع klass;
}
ساكن مضمن منطقي match_class_cached (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  أرجع get_class_cached (class_def, info) == value;
}
ساكن مضمن unsigned get_class_cached_قىممەت (hb_glyph_info_t &info, ثابت فراغ *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (data);
  أرجع get_class_cached (class_def, info);
}
ساكن مضمن unsigned get_class_cached1 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  أرجع klass;
}
ساكن مضمن منطقي match_class_cached1 (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  أرجع get_class_cached1 (class_def, info) == value;
}
ساكن مضمن unsigned get_class_cached2 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  إذا (klass < 15)
    أرجع klass;
  klass = class_def.get_class (info.codepoint);
  إذا (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  أرجع klass;
}
ساكن مضمن منطقي match_class_cached2 (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  أرجع get_class_cached2 (class_def, info) == value;
}
ساكن مضمن unsigned get_class_cached2_قىممەت (hb_glyph_info_t &info, ثابت فراغ *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (data);
  أرجع get_class_cached2 (class_def, info);
}
ساكن مضمن منطقي match_coverage (hb_glyph_info_t &info, unsigned value, ثابت فراغ *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  أرجع (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ساكن مضمن منطقي would_match_كىرگۈزۈش (hb_would_apply_context_t *c,
				      unsigned صحيح سانى, /* Including the first glyph (not matched) */
				      glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      ثابت فراغ *match_data)
{
  إذا (سانى != c->len)
    أرجع خطأ;

  لكل (unsigned صحيح i = 1; i < سانى; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    إذا (likely (!match_func (info, كىرگۈزۈش[i - 1], match_data)))
      أرجع خطأ;
  }

  أرجع true;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#وإلا
مضمن
#endif
ساكن منطقي match_كىرگۈزۈش (hb_ot_apply_context_t *c,
			 unsigned صحيح سانى, /* Including the first glyph (not matched) */
			 glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 ثابت فراغ *match_data,
			 unsigned صحيح *end_position,
			 unsigned صحيح *p_total_component_سانى = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  إذا (سانى == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    إذا (p_total_component_سانى)
      *p_total_component_سانى = hb_glyph_info_get_lig_رەقەم_comps (&buffer->cur());
    return_trace (true);
  }

  إذا (unlikely (سانى > HB_MAX_CONTEXT_LENGTH)) return_trace (خطأ);

  auto &skippy_iter = c->iter_كىرگۈزۈش;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (كىرگۈزۈش);

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

  unsigned صحيح total_component_سانى = 0;

  unsigned صحيح first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned صحيح first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  تعداد {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  لكل (unsigned صحيح i = 1; i < سانى; i++)
  {
    unsigned unsafe_to;
    إذا (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (خطأ);
    }

    إذا (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (خطأ);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned صحيح this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    unsigned صحيح this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    إذا (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      إذا (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	إذا (ligbase == LIGBASE_NOT_CHECKED)
	{
	  منطقي found = خطأ;
	  ثابت auto *out = buffer->out_info;
	  unsigned صحيح j = buffer->out_len;
	  طالما (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    إذا (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = true;
	      اكسر;
	    }
	    j--;
	  }

	  إذا (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
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
      إذا (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (خطأ);
    }

    total_component_سانى += hb_glyph_info_get_lig_رەقەم_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  إذا (p_total_component_سانى)
  {
    total_component_سانى += hb_glyph_info_get_lig_رەقەم_comps (&buffer->cur());
    *p_total_component_سانى = total_component_سانى;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (true);
}
ساكن مضمن منطقي ligate_كىرگۈزۈش (hb_ot_apply_context_t *c,
				 unsigned صحيح سانى, /* Including the first glyph */
				 unsigned صحيح match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned صحيح total_component_سانى)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  buffer->merge_clusters (buffer->idx, match_end);

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

  منطقي is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  منطقي is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  لكل (unsigned صحيح i = 1; i < سانى; i++)
    إذا (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = خطأ;
      is_mark_ligature = خطأ;
      اكسر;
    }
  منطقي is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned صحيح klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned صحيح lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  unsigned صحيح last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned صحيح last_رەقەم_components = hb_glyph_info_get_lig_رەقەم_comps (&buffer->cur());
  unsigned صحيح components_so_far = last_رەقەم_components;

  إذا (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_سانى);
    إذا (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  لكل (unsigned صحيح i = 1; i < سانى; i++)
  {
    طالما (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      إذا (is_ligature)
      {
	unsigned صحيح this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	إذا (this_comp == 0)
	  this_comp = last_رەقەم_components;
	أكد (components_so_far >= last_رەقەم_components);
	unsigned صحيح new_lig_comp = components_so_far - last_رەقەم_components +
				    hb_min (this_comp, last_رەقەم_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (فراغ) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_رەقەم_components = hb_glyph_info_get_lig_رەقەم_comps_in_ligation (&buffer->cur());
    components_so_far += last_رەقەم_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  إذا (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    لكل (unsigned i = buffer->idx; i < buffer->len; ++i)
    {
      إذا (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) اكسر;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      إذا (!this_comp) اكسر;

      أكد (components_so_far >= last_رەقەم_components);
      unsigned new_lig_comp = components_so_far - last_رەقەم_components +
			      hb_min (this_comp, last_رەقەم_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (true);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#وإلا
مضمن
#endif
ساكن منطقي match_backtrack (hb_ot_apply_context_t *c,
			     unsigned صحيح سانى,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     ثابت فراغ *match_data,
			     unsigned صحيح *match_start)
{
  TRACE_APPLY (nullptr);

  إذا (!سانى)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (true);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  لكل (unsigned صحيح i = 0; i < سانى; i++)
  {
    unsigned unsafe_from;
    إذا (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (خطأ);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (true);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#وإلا
مضمن
#endif
ساكن منطقي match_lookahead (hb_ot_apply_context_t *c,
			     unsigned صحيح سانى,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     ثابت فراغ *match_data,
			     unsigned صحيح start_index,
			     unsigned صحيح *end_index)
{
  TRACE_APPLY (nullptr);

  إذا (!سانى)
  {
    *end_index = start_index;
    return_trace (true);
  }

  auto &skippy_iter = c->iter_context;
  أكد (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  لكل (unsigned صحيح i = 0; i < سانى; i++)
  {
    unsigned unsafe_to;
    إذا (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (خطأ);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (true);
}



بنية LookupRecord
{
  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t         *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->embed (*هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  عام:
  DEFINE_SIZE_STATIC (4);
};

ساكن مضمن unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ثابت hb_array_t<ثابت LookupRecord> lookupRecords,
						     ثابت hb_map_t *lookup_map)
{
  unsigned سانى = 0;
  لكل (ثابت LookupRecord& r : lookupRecords)
  {
    إذا (!lookup_map->has (r.lookupListIndex))
      استمر;

    إذا (!r.serialize (c, lookup_map))
      أرجع 0;

    سانى++;
  }
  أرجع سانى;
}

تعداد ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ساكن مضمن ثابت hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ثابت فراغ *data,
			unsigned value,
			منطقي filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  بدل (context_format)
  {
    حالة ContextFormat::SimpleContext:
      أرجع c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    حالة ContextFormat::ClassBasedContext:
    {
      ثابت hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      إذا (unlikely (!class_set))
	أرجع nullptr;
      إذا (!filter_class_by_parent)
	أرجع class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      اكسر;
    }
    حالة ContextFormat::CoverageBasedContext:
      أرجع c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  إذا (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    أرجع nullptr;
  }
  أرجع scratch;
}

ساكن مضمن فراغ context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned كىرگۈزۈش_سانى, glyph_data_t كىرگۈزۈش,
					     unsigned lookup_سانى_2,
					     ثابت LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     ثابت فراغ *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  لكل (unsigned صحيح i = 0; i < lookup_سانى_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    إذا (seqIndex >= كىرگۈزۈش_سانى) استمر;

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = true;
      pos_glyphs.clear ();
      إذا (seqIndex == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *كىرگۈزۈش_data = كىرگۈزۈش.data;
        unsigned كىرگۈزۈش_قىممەت = seqIndex - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          كىرگۈزۈش_data = data;
          كىرگۈزۈش_قىممەت = كىرگۈزۈش[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, كىرگۈزۈش_data, كىرگۈزۈش_قىممەت, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs))
      أرجع;
    إذا (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } وإلا {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned endIndex = كىرگۈزۈش_سانى;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->چىقىرىش_cur_done_glyphs ();
  }
}

ساكن منطقي
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned كىرگۈزۈش_سانى_2,
				unsigned lookup_سانى,
				ثابت LookupRecord lookup_records[],
				ثابت فراغ *rule,
				unsigned value,
				ContextFormat context_format,
				ثابت فراغ *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  منطقي have_active_idx = خطأ;
  لكل (unsigned i = 0; i < lookup_سانى; i++)
  {
    إذا (lookup_records[i].sequenceIndex >= كىرگۈزۈش_سانى_2)
      استمر;

    إذا (!have_active_idx)
    {
      إذا (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	أرجع خطأ;
      have_active_idx = true;
    }

    hb_depend_context_t::context_set_cache_ئاچقۇچ_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    إذا (!c->context_set_cache.has (key, &cached_context_set))
      أرجع خطأ;
    إذا (unlikely (!cached_context_sets->ئىتتىرىش_or_fail (*cached_context_set)))
      أرجع c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  أرجع have_active_idx;
}

ساكن مضمن فراغ context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned كىرگۈزۈش_سانى, glyph_data_t كىرگۈزۈش,
					     unsigned lookup_سانى_2,
					     ثابت LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     ثابت فراغ *rule,
					     unsigned value,
					     ContextFormat context_format,
					     ثابت فراغ *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     فراغ *cache,
					     ثابت hb_set_t &preliminary_context,
					     ثابت hb_vector_t<ثابت hb_set_t *> *كىرگۈزۈش_position_glyphs,
					     ثابت hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  إذا (unlikely (كىرگۈزۈش_سانى > HB_MAX_CONTEXT_LENGTH))
    أرجع;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_index = 0;
  لكل (unsigned صحيح i = 0; i < lookup_سانى_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    إذا (seqIndex >= كىرگۈزۈش_سانى)
      استمر;

    hb_depend_context_t::context_set_cache_ئاچقۇچ_t context_cache_ئاچقۇچ;
    إذا (unlikely (!c->get_context_set_cache_ئاچقۇچ (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_ئاچقۇچ)))
      أرجع;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    إذا (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    وإلا إذا (c->context_set_cache.has (context_cache_ئاچقۇچ, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    وإلا
    {
      إذا (unlikely (!كىرگۈزۈش_position_glyphs))
      {
	c->depend_data->fail ();
	أرجع;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_قىممەت_INVALID;
      طالما (preliminary_context.next (&elem)) {
	إذا (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} وإلا {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      لكل (unsigned j = 0; j < كىرگۈزۈش_position_glyphs->length; j++) {
	إذا (j != seqIndex && (*كىرگۈزۈش_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*كىرگۈزۈش_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_قىممەت_INVALID;
      طالما (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	ثابت hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	إذا (unlikely (!original_set)) استمر;

	/* Dependency context sets are never inverted. */
	إذا (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      لكل (unsigned j = 0; j < كىرگۈزۈش_position_glyphs->length; j++) {
	إذا (j != seqIndex && (*كىرگۈزۈش_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  إذا (!(*كىرگۈزۈش_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    منطقي cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*كىرگۈزۈش_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*كىرگۈزۈش_position_glyphs)[j]);
	    إذا (unlikely (idx == HB_CODEPOINT_INVALID))
	      أرجع;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      إذا (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	أرجع;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      إذا (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	أرجع;

      إذا (unlikely (!c->context_set_cache.set (context_cache_ئاچقۇچ, context_set_idx)))
      {
	c->depend_data->fail ();
	أرجع;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_set_index = c->depend_data->current_context_set_index;
    hb_subset_depend_edge_flags_t saved_edge_flags = c->depend_data->current_edge_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    منطقي nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_data->current_context_set_index = context_set_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_data->current_edge_flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE;
    إذا (كىرگۈزۈش_سانى > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    إذا (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    منطقي has_pos_glyphs = خطأ;

    إذا (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = true;
      pos_glyphs.clear ();
      إذا (seqIndex == 0)
      {
        بدل (context_format) {
        حالة ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          اكسر;
        حالة ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          اكسر;
        حالة ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          اكسر;
        }
      }
      وإلا
      {
        ثابت فراغ *كىرگۈزۈش_data = كىرگۈزۈش.data;
        unsigned كىرگۈزۈش_قىممەت = seqIndex - 1;
        إذا (context_format != ContextFormat::SimpleContext)
        {
          كىرگۈزۈش_data = data;
          كىرگۈزۈش_قىممەت = كىرگۈزۈش[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, كىرگۈزۈش_data, كىرگۈزۈش_قىممەت, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    إذا (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      إذا (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	إذا (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs (active_idx);
      إذا (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	أرجع;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    وإلا إذا (unlikely (!c->ئىتتىرىش_يىلتىز_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      أرجع;
    }

    unsigned endIndex = كىرگۈزۈش_سانى;
    إذا (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->چىقىرىش_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
ساكن مضمن فراغ recurse_lookups (context_t *c,
                                    unsigned صحيح lookup_سانى_2,
                                    ثابت LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  لكل (unsigned صحيح i = 0; i < lookup_سانى_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

ساكن مضمن فراغ apply_lookup (hb_ot_apply_context_t *c,
				 unsigned صحيح سانى, /* Including the first glyph */
				 unsigned صحيح lookup_سانى_2,
				 ثابت LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned صحيح match_end)
{
  hb_buffer_t *buffer = c->buffer;
  صحيح end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned صحيح bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    صحيح delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    لكل (unsigned صحيح j = 0; j < سانى; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  لكل (unsigned صحيح i = 0; i < lookup_سانى_2 && buffer->successful; i++)
  {
    unsigned صحيح idx = lookupRecord[i].sequenceIndex;
    إذا (idx >= سانى)
      استمر;

    unsigned صحيح orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    إذا (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      استمر;

    إذا (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      اكسر;

    إذا (unlikely (buffer->max_ops <= 0))
      اكسر;

    إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      إذا (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    منطقي ret = c->recurse (lookupRecord[i].lookupListIndex);

    إذا (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      إذا (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookupListIndex);
    }

    إذا (!ret)
      استمر;

    unsigned صحيح new_len = buffer->backtrack_len () + buffer->lookahead_len ();
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

    end += delta;
    إذا (end < صحيح (c->match_positions.arrayZ[idx]))
    {
      /* End might end up being smaller than match_positions.arrayZ[idx] if the recursed
       * lookup ended up removing many items.
       * Just never rewind end beyond start of current position, since that is
       * not possible in the recursed lookup.  Also adjust delta as such.
       *
       * https://bugs.chromium.org/p/chromium/issues/detail?id=659496
       * https://github.com/harfbuzz/harfbuzz/issues/1611
       */
      delta += c->match_positions.arrayZ[idx] - end;
      end = c->match_positions.arrayZ[idx];
    }

    unsigned صحيح next = idx + 1; /* next now is the position after the recursed lookup. */

    إذا (delta > 0)
    {
      إذا (unlikely (delta + سانى > HB_MAX_CONTEXT_LENGTH))
	اكسر;
      إذا (unlikely (سانى + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (سانى + delta)))
        أرجع;
    }
    وإلا
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (صحيح) next - (صحيح) سانى);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (سانى - next) * حجم (c->match_positions.arrayZ[0]));
    next += delta;
    سانى += delta;

    /* Fill in new entries. */
    لكل (unsigned صحيح j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    لكل (; next < سانى; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  أكد (end >= 0);
  (فراغ) buffer->move_to (end);
}



/* Contextual lookups */

بنية ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_data;
  فراغ *intersects_cache;
  فراغ *intersected_glyphs_cache;
};

تعريفنوع ContextClosureLookupContext ContextDependLookupContext;

بنية ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_data;
};

بنية ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ثابت فراغ *match_data;
  get_قىممەت_func_t get_قىممەت;
};

ساكن مضمن منطقي context_intersects (ثابت hb_set_t *glyphs,
				       unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
				       glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  أرجع array_is_subset_of (glyphs,
			     كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0, كىرگۈزۈش,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

ساكن مضمن فراغ context_closure_lookup (hb_closure_context_t *c,
					   unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
					   glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
					   unsigned صحيح lookup_سانى_2,
					   ثابت LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  إذا (context_intersects (c->glyphs,
			  كىرگۈزۈش_سانى, كىرگۈزۈش,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     كىرگۈزۈش_سانى, كىرگۈزۈش,
				     lookup_سانى_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ context_depend_lookup (hb_depend_context_t *c,
					  unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
					  glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
					  unsigned صحيح lookup_سانى_2,
					  ثابت LookupRecord lookupRecord[],
					  ثابت فراغ *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  إذا (!context_intersects (c->glyphs,
			   كىرگۈزۈش_سانى, كىرگۈزۈش,
			   lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, كىرگۈزۈش_سانى, lookup_سانى_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_data->successful))
    أرجع;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ثابت hb_set_t *> &كىرگۈزۈش_position_glyphs = scratch->كىرگۈزۈش_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  إذا (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ثابت hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      true, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !كىرگۈزۈش_position_glyphs.ئىتتىرىش_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (unsigned i = 0; i + 1 < كىرگۈزۈش_سانى; i++)
    {
      ثابت hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	كىرگۈزۈش[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !كىرگۈزۈش_position_glyphs.ئىتتىرىش_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  كىرگۈزۈش_سانى, كىرگۈزۈش,
				  lookup_سانى_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &كىرگۈزۈش_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

ساكن مضمن فراغ context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
						  glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
						  unsigned صحيح lookup_سانى_2,
						  ثابت LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->كىرگۈزۈش,
		 كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0, كىرگۈزۈش,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_سانى_2, lookupRecord);
}

ساكن مضمن منطقي context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
					       glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
					       unsigned صحيح lookup_سانى_2 HB_UNUSED,
					       ثابت LookupRecord lookupRecord[] HB_UNUSED,
					       ثابت ContextApplyLookupContext &lookup_context)
{
  أرجع would_match_كىرگۈزۈش (c,
			    كىرگۈزۈش_سانى, كىرگۈزۈش,
			    lookup_context.funcs.match, lookup_context.match_data);
}

ساكن مضمن منطقي context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
					 glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
					 unsigned صحيح lookup_سانى_2,
					 ثابت LookupRecord lookupRecord[],
					 ثابت ContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (كىرگۈزۈش_سانى > HB_MAX_CONTEXT_LENGTH)) أرجع خطأ;

  unsigned match_end = 0;
  منطقي ret = خطأ;
  إذا (match_كىرگۈزۈش (c,
		   كىرگۈزۈش_سانى, كىرگۈزۈش,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  كىرگۈزۈش_سانى,
		  lookup_سانى_2, lookupRecord,
		  match_end);
    ret = true;
  }
  وإلا
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
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
      إذا (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	أرجع خطأ;
      auto &info = c->buffer->info;
      unsigned سانى = c->buffer->len;
      لكل (unsigned i = 0; i < سانى; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      أرجع true;
    }
    حالة hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      اكسر;
    }
  }
  أرجع خطأ;
}

template <typename Types>
بنية Rule
{
  template <typename SetTypes, typename RuleType>
  friend بنية RuleSetOf;

  منطقي intersects (ثابت hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) ثابت
  {
    أرجع context_intersects (glyphs,
			       كىرگۈزۈش_سانى, كىرگۈزۈش_Z.arrayZ,
			       lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ثابت
  {
    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (كىرگۈزۈش_Z.as_array ((كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0)));
    context_depend_lookup (c,
			   كىرگۈزۈش_سانى, كىرگۈزۈش_Z.arrayZ,
			   lookup_سانى_2, lookupRecord.arrayZ,
			   هذا,
			   value, lookup_context);
  }

  فراغ closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_limit_exceeded ())) أرجع;

    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (كىرگۈزۈش_Z.as_array ((كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0)));
    context_closure_lookup (c,
			    كىرگۈزۈش_سانى, كىرگۈزۈش_Z.arrayZ,
			    lookup_سانى_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_limit_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (كىرگۈزۈش_Z.as_array (كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0));
    recurse_lookups (c, lookup_سانى_2, lookupRecord.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (كىرگۈزۈش_Z.as_array (كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0));
    context_collect_glyphs_lookup (c,
				   كىرگۈزۈش_سانى, كىرگۈزۈش_Z.arrayZ,
				   lookup_سانى_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest,
			      ثابت ClassDef *class_def = nullptr) ثابت
  {
    إذا (كىرگۈزۈش_سانى <= 1)
    {
      *digest = hb_set_digest_t::full ();
      أرجع;
    }

    unsigned second = كىرگۈزۈش_Z.arrayZ[0];
    إذا (!class_def)
      digest->add (second);
    وإلا إذا (!second)
      *digest = hb_set_digest_t::full ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (كىرگۈزۈش_Z.as_array (كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0));
    أرجع context_would_apply_lookup (c,
				       كىرگۈزۈش_سانى, كىرگۈزۈش_Z.arrayZ,
				       lookup_سانى_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (كىرگۈزۈش_Z.as_array (كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0));
    return_trace (context_apply_lookup (c, كىرگۈزۈش_سانى, كىرگۈزۈش_Z.arrayZ, lookup_سانى_2, lookupRecord.arrayZ, lookup_context));
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *كىرگۈزۈش_mapping, /* old->new glyphid or class mapping */
		  ثابت hb_map_t *lookup_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->start_embed (هذا);
    إذا (unlikely (!c->extend_min (out))) return_trace (خطأ);

    out->كىرگۈزۈش_سانى = كىرگۈزۈش_سانى;
    ثابت auto كىرگۈزۈش = كىرگۈزۈش_Z.as_array (كىرگۈزۈش_سانى - 1);
    لكل (ثابت auto org : كىرگۈزۈش)
    {
      typename Types::HBUINT d;
      d = كىرگۈزۈش_mapping->get (org);
      c->copy (d);
    }

    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (كىرگۈزۈش_Z.as_array ((كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0)));

    unsigned سانى = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_سانى_2), lookup_map);
    return_trace (c->check_assign (out->lookup_سانى_2, سانى, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (unlikely (!كىرگۈزۈش_سانى)) return_trace (خطأ);
    ثابت auto كىرگۈزۈش = كىرگۈزۈش_Z.as_array (كىرگۈزۈش_سانى - 1);

    ثابت hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    إذا (!hb_all (كىرگۈزۈش, mapping)) return_trace (خطأ);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  c->check_range (كىرگۈزۈش_Z.arrayZ,
				  كىرگۈزۈش_Z.item_size * (كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0) +
				  LookupRecord::static_size * lookup_سانى_2));
  }

  محمي:
  HBUINT16	كىرگۈزۈش_سانى;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_سانى_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		كىرگۈزۈش_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_SIZE_ARRAY (4, كىرگۈزۈش_Z);
};

template <typename Types, typename RuleType>
بنية RuleSetOf
{
  منطقي intersects (ثابت hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_map ([&] (ثابت RuleType &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_first_كىرگۈزۈش_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (unsigned i = 0; i < rule.len; i++)
    {
      ثابت RuleType &r = هذا+rule.arrayZ[i];
      إذا (r.كىرگۈزۈش_سانى <= 1)
      {
	digest->set_full ();
	أرجع;
      }
      digest->add (r.كىرگۈزۈش_Z.arrayZ[0]);
    }
  }

  فراغ depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  فراغ closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_limit_exceeded ())) أرجع;

    أرجع
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_limit_exceeded ())) أرجع;
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest,
			      ثابت ClassDef *class_def = nullptr) ثابت
  {
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([=] (ثابت RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_map ([&] (ثابت RuleType &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = nullptr) ثابت
  {
    TRACE_APPLY (هذا);

    unsigned رەقەم_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    إذا (HB_OPTIMIZE_SIZE_VAL || رەقەم_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (هذا))
      | hb_map ([&] (ثابت RuleType &_) { أرجع _.apply (c, lookup_context); })
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
    auto &skippy_iter = c->iter_context;
    skippy_iter.reset (c->buffer->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_data ((HBUINT16 *) nullptr);
    unsigned unsafe_to = (unsigned) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *first = nullptr, *second = nullptr;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_قىممەت &&
	  !digest->may_have (lookup_context.get_قىممەت (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_to_concat = خطأ;
      منطقي ret =
      + hb_iter (rule)
      | hb_map (hb_add (هذا))
      | hb_filter ([&] (ثابت RuleType &_)
		   {
		     إذا (_.كىرگۈزۈش_سانى <= 1) أرجع true;
		     unsafe_to_concat = true;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت RuleType &_) { أرجع _.apply (c, lookup_context); })
      | hb_any
      ;
      إذا (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_كىرگۈزۈش = lookup_context.funcs.match;
    auto *كىرگۈزۈش_data = lookup_context.match_data;
    لكل (unsigned صحيح i = 0; i < رەقەم_rules; i++)
    {
      ثابت auto &r = هذا+rule.arrayZ[i];

      ثابت auto &كىرگۈزۈش = r.كىرگۈزۈش_Z;

      إذا (r.كىرگۈزۈش_سانى <= 1 ||
	  (!match_كىرگۈزۈش ||
	   match_كىرگۈزۈش (*first, كىرگۈزۈش.arrayZ[0], كىرگۈزۈش_data)))
      {
        إذا (!second ||
	    (r.كىرگۈزۈش_سانى <= 2 ||
	     (!match_كىرگۈزۈش ||
	      match_كىرگۈزۈش (*second, كىرگۈزۈش.arrayZ[1], كىرگۈزۈش_data)))
	   )
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (true);
	  }
	}
	وإلا
	  unsafe_to = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	لكل (; i + 1 < رەقەم_rules; i++)
	{
	  ثابت auto &r2 = هذا+rule.arrayZ[i + 1];
	  ثابت auto &input2 = r2.كىرگۈزۈش_Z;
	  إذا (r2.كىرگۈزۈش_سانى <= 1 || input2.arrayZ[0] != كىرگۈزۈش.arrayZ[0])
	    اكسر;
	}
      }
    }
    إذا (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    لكل (ثابت auto& _ : rule)
    {
      إذا (!_) استمر;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا, lookup_map, klass_map))
      {
	out->rule.چىقىرىش ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (out->rule);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (rule.sanitize (c, هذا));
  }

  محمي:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  عام:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
بنية RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
بنية ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
بنية ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بنية ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    أرجع
    + hb_zip (هذا+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_map ([&] (ثابت RuleSet &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع true; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, unsigned> _) { أرجع hb_pair_t<unsigned, ثابت RuleSet&> (_.first, هذا+ruleSet[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    بنية ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (هذا+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, unsigned> _) { أرجع hb_pair_t<unsigned, ثابت RuleSet&> (_.first, هذا+ruleSet[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    بنية ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (هذا+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->كىرگۈزۈش);

    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([digest] (ثابت RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت RuleSet &rule_set = هذا+ruleSet[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    أرجع rule_set.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    unsigned صحيح index = (هذا+coverage).get_coverage (c->buffer->cur().codepoint);
    إذا (likely (index == NOT_COVERED))
      return_trace (خطأ);

    ثابت RuleSet &rule_set = هذا+ruleSet[index];
    بنية ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;

    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, هذا, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ruleSet.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
بنية ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (هذا+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    أرجع
    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<unsigned, ثابت RuleSet &> p)
	      { أرجع class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع true; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

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

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { أرجع class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ثابت RuleSet& rule_set = هذا+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    بنية ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { أرجع class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ثابت RuleSet& rule_set = هذا+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &class_def = هذا+classDef;

    hb_map_t cache;
    بنية ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_enumerate
    | hb_filter ([&] (ثابت hb_pair_t<unsigned, ثابت RuleSet &> p)
    { أرجع class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (ثابت RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->كىرگۈزۈش);

    ثابت ClassDef &class_def = هذا+classDef;
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([=, &class_def] (ثابت RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &class_def = هذا+classDef;
    unsigned صحيح index = class_def.get_class (c->glyphs[0]);
    ثابت RuleSet &rule_set = هذا+ruleSet[index];
    بنية ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    أرجع rule_set.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  unsigned cache_cost () ثابت
  {
    أرجع (هذا+classDef).cost ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_ئىككىلىك_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    unsigned سانى = ruleSet.len;
    unsigned size = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    سانى * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      لكل (unsigned i = 0; i < سانى; i++)
	(هذا+ruleSet[i]).collect_first_كىرگۈزۈش_classes (&cache->rule_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, true, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned صحيح index = (هذا+coverage).get_coverage_ئىككىلىك (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#وإلا
    unsigned صحيح index = (هذا+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    إذا (index == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &class_def = هذا+classDef;

    بنية ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_قىممەت : get_class_قىممەت
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    ثابت RuleSet &rule_set = هذا+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;
    إذا (unlikely (!out->coverage.serialize_subset (c, coverage, هذا)))
      return_trace (خطأ);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, هذا, &klass_map);

    ثابت hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (هذا+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (هذا+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    منطقي ret = true;
    صحيح non_zero_index = -1, index = 0;
    auto snapshot = c->serializer->snapshot();
    لكل (ثابت auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }

      إذا (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, هذا, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    إذا (!ret || non_zero_index == -1) return_trace (خطأ);

    //prune empty trailing ruleSets
    --index;
    طالما (index > non_zero_index)
    {
      out->ruleSet.چىقىرىش ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (منطقي (out->ruleSet));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && classDef.sanitize (c, هذا) && ruleSet.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by class */
  عام:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
بنية ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (glyphs))
      أرجع خطأ;

    بنية ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    أرجع context_intersects (glyphs,
			       glyph_سانى_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع true; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_سانى_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_سانى_2, (ثابت HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_سانى_2, lookupRecord,
			   هذا,
			   coverageZ[0], lookup_context);

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    إذا (!(هذا+coverageZ[0]).intersects (c->glyphs))
      أرجع;

    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_سانى_2));
    بنية ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      هذا
    };
    context_closure_lookup (c,
			    glyph_سانى_2, coverageZ.arrayZ + 1,
			    lookup_سانى_2, lookupRecord,
			    0, lookup_context);

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;
    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_سانى_2));
    recurse_lookups (c, lookup_سانى_2, lookupRecord);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverageZ[0]).collect_coverage (c->كىرگۈزۈش);

    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_سانى_2));
    بنية ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      هذا
    };

    context_collect_glyphs_lookup (c,
				   glyph_سانى_2, coverageZ.arrayZ + 1,
				   lookup_سانى_2, lookupRecord,
				   lookup_context);
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    إذا (glyph_سانى_2 <= 1)
      *digest = hb_set_digest_t::full ();
    وإلا
      (هذا+coverageZ[1]).collect_coverage (digest);
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_سانى_2));
    بنية ContextApplyLookupContext lookup_context = {
      {match_coverage},
      هذا
    };
    أرجع context_would_apply_lookup (c,
				       glyph_سانى_2, coverageZ.arrayZ + 1,
				       lookup_سانى_2, lookupRecord,
				       lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverageZ[0]; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    unsigned صحيح index = (هذا+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    إذا (index == NOT_COVERED) return_trace (خطأ);

    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_سانى_2));
    بنية ContextApplyLookupContext lookup_context = {
      {match_coverage},
      هذا
    };
    return_trace (context_apply_lookup (c,
					glyph_سانى_2,
					coverageZ.arrayZ + 1,
					lookup_سانى_2, lookupRecord,
					lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    out->format = format;
    out->glyph_سانى_2 = glyph_سانى_2;

    auto coverages = coverageZ.as_array (glyph_سانى_2);

    لكل (ثابت auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_size<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_size);
      إذا (unlikely (!o)) return_trace (خطأ);
      إذا (!o->serialize_subset (c, offset, هذا)) return_trace (خطأ);
    }

    ثابت auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_سانى_2));
    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned سانى = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_سانى_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_سانى_2, سانى, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    unsigned صحيح سانى = glyph_سانى_2;
    إذا (unlikely (!سانى)) return_trace (خطأ); /* We want to access coverageZ[0] freely. */
    إذا (unlikely (!c->check_array (coverageZ.arrayZ, سانى))) return_trace (خطأ);
    لكل (unsigned صحيح i = 0; i < سانى; i++)
      إذا (unlikely (!coverageZ[i].sanitize (c, هذا))) return_trace (خطأ);
    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_سانى_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_سانى_2)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_سانى_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_سانى_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::template OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عام:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

بنية Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قىممەت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    حالة 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    افتراضي:return_trace (c->default_return_قىممەت ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ContextFormat1_4<SmallTypes>	format1;
  ContextFormat2_5<SmallTypes>	format2;
  ContextFormat3_6<SmallTypes>	format3;
#ifndef HB_NO_BEYOND_64K
  ContextFormat1_4<MediumTypes>	format4;
  ContextFormat2_5<MediumTypes>	format5;
  ContextFormat3_6<MediumTypes>	format6;
#endif
  } u;
};


/* Chaining Contextual lookups */

بنية ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ثابت فراغ *intersects_data[3];
  فراغ *intersects_cache[3];
  فراغ *intersected_glyphs_cache;
};

تعريفنوع ChainContextClosureLookupContext ChainContextDependLookupContext;

بنية ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت فراغ *collect_data[3];
};

بنية ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ثابت فراغ *match_data[3];
  get_قىممەت_func_t get_كىرگۈزۈش_قىممەت;
};

ساكن مضمن منطقي chain_context_intersects (ثابت hb_set_t *glyphs,
					     unsigned صحيح backtrack_سانى,
					     glyph_data_t backtrack,
					     unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
					     glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
					     unsigned صحيح lookahead_سانى,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  منطقي bt_ok = array_is_subset_of (glyphs,
			     backtrack_سانى, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  منطقي in_ok = array_is_subset_of (glyphs,
			     كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0, كىرگۈزۈش,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  منطقي la_ok = array_is_subset_of (glyphs,
			     lookahead_سانى, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  أرجع bt_ok && in_ok && la_ok;
}

ساكن مضمن فراغ chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned صحيح backtrack_سانى,
						 glyph_data_t backtrack,
						 unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
						 glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
						 unsigned صحيح lookahead_سانى,
						 glyph_data_t lookahead,
						 unsigned صحيح lookup_سانى_2,
						 ثابت LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  إذا (chain_context_intersects (c->glyphs,
				backtrack_سانى, backtrack,
				كىرگۈزۈش_سانى, كىرگۈزۈش,
				lookahead_سانى, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     كىرگۈزۈش_سانى, كىرگۈزۈش,
		     lookup_سانى_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ساكن مضمن فراغ chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned صحيح backtrack_سانى,
						glyph_data_t backtrack,
						unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
						glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
						unsigned صحيح lookahead_سانى,
						glyph_data_t lookahead,
						unsigned صحيح lookup_سانى_2,
						ثابت LookupRecord lookupRecord[],
						ثابت فراغ *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  إذا (!chain_context_intersects (c->glyphs,
				 backtrack_سانى, backtrack,
				 كىرگۈزۈش_سانى, كىرگۈزۈش,
				 lookahead_سانى, lookahead,
				 lookup_context))
    أرجع;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  إذا (unlikely (!scratch))
    أرجع;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقي context_sets_cached = context_depend_sets_are_cached (
    c, كىرگۈزۈش_سانى, lookup_سانى_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  إذا (unlikely (!c->depend_data->successful))
    أرجع;
  إذا (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    كىرگۈزۈش_سانى, كىرگۈزۈش,
				    lookup_سانى_2, lookupRecord,
				    rule,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_data[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    nullptr,
				    &scratch->cached_context_sets,
				    scratch);
    أرجع;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ثابت hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<ثابت hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  لكل (unsigned i = 0; i < backtrack_سانى; i++)
  {
    ثابت hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!backtrack_sets.ئىتتىرىش_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        أرجع;
      }
    }
  }

  /* Extract lookahead glyphs */
  لكل (unsigned i = 0; i < lookahead_سانى; i++)
  {
    ثابت hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], خطأ, &position_scratch);
    إذا (unlikely (!pos_glyphs))
      أرجع;
    إذا (!pos_glyphs->is_empty ())
    {
      إذا (unlikely (!lookahead_sets.ئىتتىرىش_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        أرجع;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  لكل (ثابت hb_set_t *back_set : backtrack_sets)
  {
    إذا (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } وإلا إذا (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      إذا (unlikely (set_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  لكل (ثابت hb_set_t *look_set : lookahead_sets)
  {
    إذا (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } وإلا إذا (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      إذا (unlikely (set_idx == HB_CODEPOINT_INVALID))
        أرجع;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  إذا (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    أرجع;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<ثابت hb_set_t *> &كىرگۈزۈش_position_glyphs = scratch->كىرگۈزۈش_position_glyphs;

  إذا (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ثابت hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      true, &position_scratch, &scratch->compact_workspace);
    إذا (unlikely (!pos0_glyphs ||
		  !كىرگۈزۈش_position_glyphs.ئىتتىرىش_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      أرجع;
    }

    /* Positions 1+ (Input array) */
    لكل (unsigned i = 0; i + 1 < كىرگۈزۈش_سانى; i++)
    {
      ثابت hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	كىرگۈزۈش[i], خطأ, &position_scratch);
      إذا (unlikely (!pos_glyphs ||
		    !كىرگۈزۈش_position_glyphs.ئىتتىرىش_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	أرجع;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  كىرگۈزۈش_سانى, كىرگۈزۈش,
				  lookup_سانى_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &كىرگۈزۈش_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

ساكن مضمن فراغ chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned صحيح backtrack_سانى,
							glyph_data_t backtrack,
							unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
							glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
							unsigned صحيح lookahead_سانى,
							glyph_data_t lookahead,
							unsigned صحيح lookup_سانى_2,
							ثابت LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_سانى, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->كىرگۈزۈش,
		 كىرگۈزۈش_سانى ? كىرگۈزۈش_سانى - 1 : 0, كىرگۈزۈش,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_سانى, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_سانى_2, lookupRecord);
}

ساكن مضمن منطقي chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned صحيح backtrack_سانى,
						     glyph_data_t backtrack HB_UNUSED,
						     unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
						     glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
						     unsigned صحيح lookahead_سانى,
						     glyph_data_t lookahead HB_UNUSED,
						     unsigned صحيح lookup_سانى_2 HB_UNUSED,
						     ثابت LookupRecord lookupRecord[] HB_UNUSED,
						     ثابت ChainContextApplyLookupContext &lookup_context)
{
  أرجع (c->zero_context ? !backtrack_سانى && !lookahead_سانى : true)
      && would_match_كىرگۈزۈش (c,
			    كىرگۈزۈش_سانى, كىرگۈزۈش,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

ساكن مضمن منطقي chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned صحيح backtrack_سانى,
					       glyph_data_t backtrack,
					       unsigned صحيح كىرگۈزۈش_سانى, /* Including the first glyph (not matched) */
					       glyph_data_t كىرگۈزۈش, /* Array of input values--start with second glyph */
					       unsigned صحيح lookahead_سانى,
					       glyph_data_t lookahead,
					       unsigned صحيح lookup_سانى_2,
					       ثابت LookupRecord lookupRecord[],
					       ثابت ChainContextApplyLookupContext &lookup_context)
{
  إذا (unlikely (كىرگۈزۈش_سانى > HB_MAX_CONTEXT_LENGTH)) أرجع خطأ;

  unsigned start_index = c->buffer->out_len;
  unsigned end_index = c->buffer->idx;
  unsigned match_end = 0;
  منطقي ret = true;
  إذا (!(match_كىرگۈزۈش (c,
		     كىرگۈزۈش_سانى, كىرگۈزۈش,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_سانى, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    أرجع خطأ;
  }

  إذا (!match_backtrack (c,
			backtrack_سانى, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    أرجع خطأ;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		كىرگۈزۈش_سانى,
		lookup_سانى_2, lookupRecord,
		match_end);

  أرجع ret;
}

template <typename Types>
بنية ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend بنية ChainRuleSetOf;

  منطقي intersects (ثابت hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     كىرگۈزۈش.lenP1, كىرگۈزۈش.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  فراغ depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 كىرگۈزۈش.lenP1, كىرگۈزۈش.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 value,
				 lookup_context);
  }

  فراغ closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_limit_exceeded ())) أرجع;

    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  كىرگۈزۈش.lenP1, كىرگۈزۈش.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_limit_exceeded ())) أرجع;
    إذا (!intersects (c->glyphs, lookup_context)) أرجع;

    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 كىرگۈزۈش.lenP1, كىرگۈزۈش.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest,
			      ثابت ClassDef *كىرگۈزۈش_class_def = nullptr,
			      ثابت ClassDef *lookahead_class_def = nullptr) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);

    unsigned second;
    ثابت ClassDef *class_def;
    إذا (كىرگۈزۈش.lenP1 > 1)
    {
      second = كىرگۈزۈش.arrayZ[0];
      class_def = كىرگۈزۈش_class_def;
    }
    وإلا إذا (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    وإلا
    {
      *digest = hb_set_digest_t::full ();
      أرجع;
    }

    إذا (!class_def)
      digest->add (second);
    وإلا إذا (!second)
      *digest = hb_set_digest_t::full ();
    وإلا
      class_def->collect_class (digest, second);
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     كىرگۈزۈش.lenP1, كىرگۈزۈش.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      كىرگۈزۈش.lenP1, كىرگۈزۈش.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ثابت
  {
    c->copy (len);
    لكل (ثابت auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *lookup_map,
		  ثابت hb_map_t *backtrack_map,
		  ثابت hb_map_t *كىرگۈزۈش_map = nullptr,
		  ثابت hb_map_t *lookahead_map = nullptr) ثابت
  {
    TRACE_SERIALIZE (هذا);

    ثابت hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    إذا (كىرگۈزۈش_map) mapping = كىرگۈزۈش_map;
    serialize_array (c, كىرگۈزۈش.lenP1, + كىرگۈزۈش.iter ()
				     | hb_map (mapping));

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    إذا (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_سانى_2 = c->embed (&(lookup.len));
    إذا (!lookup_سانى_2) return_trace (خطأ);

    unsigned سانى = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_سانى_2, سانى, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_map = nullptr,
	       ثابت hb_map_t *كىرگۈزۈش_map = nullptr,
	       ثابت hb_map_t *lookahead_map = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);

    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);

    إذا (!backtrack_map)
    {
      ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      إذا (!hb_all (backtrack, glyphset) ||
	  !hb_all (كىرگۈزۈش, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    وإلا
    {
      إذا (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (كىرگۈزۈش, كىرگۈزۈش_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (خطأ);

      serialize (c->serializer, lookup_map, backtrack_map, كىرگۈزۈش_map, lookahead_map);
    }

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Hyper-optimized sanitized because this is really hot. */
    إذا (unlikely (!backtrack.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    إذا (unlikely (!كىرگۈزۈش.lenP1.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    إذا (unlikely (!lookahead.len.sanitize (c))) return_trace (خطأ);
    hb_barrier ();
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		كىرگۈزۈش_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_SIZE_MIN (8);
};

template <typename Types, typename ChainRuleType>
بنية ChainRuleSetOf
{
  منطقي intersects (ثابت hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_map ([&] (ثابت ChainRuleType &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  فراغ collect_first_كىرگۈزۈش_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    لكل (unsigned i = 0; i < rule.len; i++)
    {
      ثابت ChainRuleType &r = هذا+rule.arrayZ[i];
      ثابت auto &كىرگۈزۈش = StructAfter<decltype (r.كىرگۈزۈش_X)> (r.backtrack);
      إذا (كىرگۈزۈش.lenP1 <= 1)
      {
	digest->set_full ();
	أرجع;
      }
      digest->add (كىرگۈزۈش.arrayZ[0]);
    }
  }
  فراغ depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  فراغ closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_limit_exceeded ())) أرجع;

    أرجع
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ثابت
  {
    إذا (unlikely (c->lookup_limit_exceeded ())) أرجع;

    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest,
			      ثابت ClassDef *كىرگۈزۈش_class_def = nullptr,
			      ثابت ClassDef *lookahead_class_def = nullptr) ثابت
  {
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_apply ([=] (ثابت ChainRuleType &_)
		{ _.collect_second_glyphs (digest, كىرگۈزۈش_class_def, lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    أرجع
    + hb_iter (rule)
    | hb_map (hb_add (هذا))
    | hb_map ([&] (ثابت ChainRuleType &_) { أرجع _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = nullptr) ثابت
  {
    TRACE_APPLY (هذا);

    unsigned رەقەم_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    إذا (HB_OPTIMIZE_SIZE_VAL || رەقەم_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (هذا))
      | hb_map ([&] (ثابت ChainRuleType &_) { أرجع _.apply (c, lookup_context); })
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
    auto &skippy_iter = c->iter_context;
    skippy_iter.reset (c->buffer->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_data ((HBUINT16 *) nullptr);
    unsigned unsafe_to = (unsigned) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *first = nullptr, *second = nullptr;
    منطقي matched = skippy_iter.next (&unsafe_to1);
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      إذا (digest && lookup_context.get_كىرگۈزۈش_قىممەت &&
	  !digest->may_have (lookup_context.get_كىرگۈزۈش_قىممەت (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (خطأ);
	}
    }
    وإلا
    {
      منطقي unsafe_to_concat = خطأ;
      منطقي ret =
      + hb_iter (rule)
      | hb_map (hb_add (هذا))
      | hb_filter ([&] (ثابت ChainRuleType &_)
		   {
		     ثابت auto &كىرگۈزۈش = StructAfter<decltype (_.كىرگۈزۈش_X)> (_.backtrack);
		     ثابت auto &lookahead = StructAfter<decltype (_.lookaheadX)> (كىرگۈزۈش);
		     إذا (كىرگۈزۈش.lenP1 <= 1 && lookahead.len == 0) أرجع true;
		     unsafe_to_concat = true;
		     أرجع خطأ;
		   })
      | hb_map ([&] (ثابت ChainRuleType &_) { أرجع _.apply (c, lookup_context); })
      | hb_any
      ;
      إذا (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    إذا (likely (matched))
    {
      إذا (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        اذهب slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_كىرگۈزۈش = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *كىرگۈزۈش_data = lookup_context.match_data[1];
    auto *lookahead_data = lookup_context.match_data[2];
    لكل (unsigned صحيح i = 0; i < رەقەم_rules; i++)
    {
      ثابت auto &r = هذا+rule.arrayZ[i];

      ثابت auto &كىرگۈزۈش = StructAfter<decltype (r.كىرگۈزۈش_X)> (r.backtrack);
      ثابت auto &lookahead = StructAfter<decltype (r.lookaheadX)> (كىرگۈزۈش);

      unsigned lenP1 = كىرگۈزۈش.lenP1;
      إذا (lenP1 > 1 ?
	   (!match_كىرگۈزۈش ||
	    match_كىرگۈزۈش (*first, كىرگۈزۈش.arrayZ[0], كىرگۈزۈش_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        إذا (!second ||
	    (lenP1 > 2 ?
	     (!match_كىرگۈزۈش ||
	      match_كىرگۈزۈش (*second, كىرگۈزۈش.arrayZ[1], كىرگۈزۈش_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  إذا (r.apply (c, lookup_context))
	  {
	    إذا (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (true);
	  }
	}
	وإلا
	  unsafe_to = unsafe_to2;
      }
      وإلا
      {
	إذا (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	إذا (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  لكل (; i + 1 < رەقەم_rules; i++)
	  {
	    ثابت auto &r2 = هذا+rule.arrayZ[i + 1];
	    ثابت auto &input2 = StructAfter<decltype (r2.كىرگۈزۈش_X)> (r2.backtrack);
	    إذا (input2.lenP1 <= 1 || input2.arrayZ[0] != كىرگۈزۈش.arrayZ[0])
	      اكسر;
	  }
	}
      }
    }
    إذا (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (خطأ);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_klass_map = nullptr,
	       ثابت hb_map_t *كىرگۈزۈش_klass_map = nullptr,
	       ثابت hb_map_t *lookahead_klass_map = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    لكل (ثابت auto& _ : rule)
    {
      إذا (!_) استمر;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      إذا (unlikely (!o)) استمر;

      إذا (!o->serialize_subset (c, _, هذا,
				lookup_map,
				backtrack_klass_map,
				كىرگۈزۈش_klass_map,
				lookahead_klass_map))
      {
	out->rule.چىقىرىش ();
	c->serializer->revert (o_snap);
      }
    }

    منطقي ret = منطقي (out->rule);
    إذا (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (rule.sanitize (c, هذا));
  }

  محمي:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  عام:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
بنية ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (حجم (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

template <typename Types>
بنية ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
بنية ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بنية ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    أرجع
    + hb_zip (هذا+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_map ([&] (ثابت ChainRuleSet &_) { أرجع _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع true; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (هذا+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, unsigned> _) { أرجع hb_pair_t<unsigned, ثابت ChainRuleSet&> (_.first, هذا+ruleSet[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    بنية ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (هذا+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      أرجع c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, unsigned> _) { أرجع hb_pair_t<unsigned, ثابت ChainRuleSet&> (_.first, هذا+ruleSet[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    بنية ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (هذا+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->كىرگۈزۈش);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([digest] (ثابت ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ChainRuleSet &rule_set = هذا+ruleSet[(هذا+coverage).get_coverage (c->glyphs[0])];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    أرجع rule_set.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    unsigned صحيح index = (هذا+coverage).get_coverage (c->buffer->cur().codepoint);
    إذا (index == NOT_COVERED) return_trace (خطأ);

    ثابت ChainRuleSet &rule_set = هذا+ruleSet[index];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;

    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (هذا+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, هذا, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقي (new_coverage));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && ruleSet.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

template <typename Types>
بنية ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    إذا (!(هذا+coverage).intersects (glyphs))
      أرجع خطأ;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &كىرگۈزۈش_class_def = هذا+كىرگۈزۈش_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &كىرگۈزۈش_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (هذا+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    كىرگۈزۈش_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    أرجع
    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<unsigned, ثابت ChainRuleSet &> p)
	      { أرجع كىرگۈزۈش_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع true; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &كىرگۈزۈش_class_def = هذا+كىرگۈزۈش_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &كىرگۈزۈش_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { أرجع كىرگۈزۈش_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ثابت ChainRuleSet& chainrule_set = هذا+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs)) أرجع;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &كىرگۈزۈش_class_def = هذا+كىرگۈزۈش_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    بنية ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &كىرگۈزۈش_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { أرجع كىرگۈزۈش_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ثابت ChainRuleSet& chainrule_set = هذا+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    إذا (!(هذا+coverage).intersects (c->glyphs))
      أرجع;

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &كىرگۈزۈش_class_def = هذا+كىرگۈزۈش_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    hb_map_t caches[3] = {};
    بنية ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &كىرگۈزۈش_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { أرجع كىرگۈزۈش_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ثابت ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (هذا+coverage).collect_coverage (c->كىرگۈزۈش);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &كىرگۈزۈش_class_def = هذا+كىرگۈزۈش_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &كىرگۈزۈش_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([&] (ثابت ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    ثابت ClassDef &كىرگۈزۈش_class_def = هذا+كىرگۈزۈش_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (هذا))
    | hb_apply ([digest, &كىرگۈزۈش_class_def, &lookahead_class_def] (ثابت ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &كىرگۈزۈش_class_def, &lookahead_class_def); })
    ;
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &كىرگۈزۈش_class_def = هذا+كىرگۈزۈش_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

    unsigned صحيح index = كىرگۈزۈش_class_def.get_class (c->glyphs[0]);
    ثابت ChainRuleSet &rule_set = هذا+ruleSet[index];
    بنية ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &كىرگۈزۈش_class_def,
       &lookahead_class_def}
    };
    أرجع rule_set.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  unsigned cache_cost () ثابت
  {
    أرجع (هذا+كىرگۈزۈش_Class_Def).cost () + (هذا+lookaheadClassDef).cost ();
  }
  ساكن منطقي cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    أرجع context_cache_func (c, op);
  }

  بنية external_cache_t
  {
    hb_ot_layout_ئىككىلىك_cache_t coverage;
    hb_ot_layout_mapping_cache_t كىرگۈزۈش_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  فراغ *external_cache_create () ثابت
  {
    unsigned سانى = ruleSet.len;
    unsigned size = حجم (external_cache_t) -
		    HB_VAR_ARRAY * حجم (hb_ot_layout_ruleset_digest_t) +
		    سانى * حجم (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    إذا (likely (cache))
    {
      cache->coverage.clear ();
      cache->كىرگۈزۈش_class.clear ();
      cache->lookahead_class.clear ();
      لكل (unsigned i = 0; i < سانى; i++)
	(هذا+ruleSet[i]).collect_first_كىرگۈزۈش_classes (&cache->rule_sets[i]);
    }
    أرجع cache;
  }
  منطقي apply_cached (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, true, external_cache); }
  منطقي apply (hb_ot_apply_context_t *c, فراغ *external_cache) ثابت { أرجع _apply (c, خطأ, external_cache); }
  منطقي _apply (hb_ot_apply_context_t *c, منطقي cached, فراغ *external_cache) ثابت
  {
    TRACE_APPLY (هذا);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned صحيح index = (هذا+coverage).get_coverage_ئىككىلىك (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#وإلا
    unsigned صحيح index = (هذا+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    إذا (index == NOT_COVERED) return_trace (خطأ);

    ثابت ClassDef &backtrack_class_def = هذا+backtrackClassDef;
    ثابت ClassDef &كىرگۈزۈش_class_def = هذا+كىرگۈزۈش_Class_Def;
    ثابت ClassDef &lookahead_class_def = هذا+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t كىرگۈزۈش_cache_data = {
      &كىرگۈزۈش_class_def,
      cache ? &cache->كىرگۈزۈش_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    ثابت فراغ *كىرگۈزۈش_match_data = cached
      ? static_cast<ثابت فراغ *> (&كىرگۈزۈش_class_def)
      : static_cast<ثابت فراغ *> (&كىرگۈزۈش_cache_data);
    ثابت فراغ *lookahead_match_data = cached
      ? static_cast<ثابت فراغ *> (&lookahead_class_def)
      : static_cast<ثابت فراغ *> (&lookahead_cache_data);
    منطقي backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ثابت فراغ *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<ثابت فراغ *> (&lookahead_cache_data)
      : static_cast<ثابت فراغ *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    بنية ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       كىرگۈزۈش_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_قىممەت : get_class_cache_قىممەت
#وإلا
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &كىرگۈزۈش_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_قىممەت : get_class_قىممەت
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (كىرگۈزۈش_class_def, c->buffer->cur())
         : كىرگۈزۈش_class_def.get_class (c->buffer->cur().codepoint, كىرگۈزۈش_cache_data.cache);
#وإلا
    index = cached
         ? get_class_cached2 (كىرگۈزۈش_class_def, c->buffer->cur())
          : كىرگۈزۈش_class_def.get_class (c->buffer->cur().codepoint);
#endif
    ثابت ChainRuleSet &rule_set = هذا+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#وإلا
    ثابت hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, هذا);

    hb_map_t backtrack_klass_map;
    hb_map_t كىرگۈزۈش_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, هذا, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->كىرگۈزۈش_Class_Def.serialize_subset (c, كىرگۈزۈش_Class_Def, هذا, &كىرگۈزۈش_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, هذا, &lookahead_klass_map);

    إذا (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   كىرگۈزۈش_klass_map,
						   lookahead_klass_map)))
      return_trace (خطأ);

    ثابت hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (هذا+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (هذا+كىرگۈزۈش_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    صحيح non_zero_index = -1, index = 0;
    منطقي ret = true;
    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    لكل (ثابت auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (كىرگۈزۈش_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      إذا (unlikely (!o))
      {
	ret = خطأ;
	اكسر;
      }
      إذا (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, هذا,
			       lookup_map,
			       &backtrack_klass_map,
			       &كىرگۈزۈش_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    إذا (!ret || non_zero_index == -1) return_trace (خطأ);

    // prune empty trailing ruleSets
    إذا (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (منطقي (out->ruleSet));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) &&
		  backtrackClassDef.sanitize (c, هذا) &&
		  كىرگۈزۈش_Class_Def.sanitize (c, هذا) &&
		  lookaheadClassDef.sanitize (c, هذا) &&
		  ruleSet.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		كىرگۈزۈش_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  عام:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

template <typename Types>
بنية ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);

    إذا (!(هذا+كىرگۈزۈش[0]).intersects (glyphs))
      أرجع خطأ;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    بنية ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    أرجع chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     كىرگۈزۈش.len, كىرگۈزۈش.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  منطقي may_have_non_1to1 () ثابت
  { أرجع true; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);

    إذا (!(هذا+كىرگۈزۈش[0]).intersects (c->glyphs))
      أرجع;

    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs))
      أرجع;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ثابت HBUINT16 *) backtrack.arrayZ,
				 كىرگۈزۈش.len, (ثابت HBUINT16 *) كىرگۈزۈش.arrayZ + 1,
				 lookahead.len, (ثابت HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 هذا,
				 كىرگۈزۈش[0], lookup_context);

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure (hb_closure_context_t *c) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);

    إذا (!(هذا+كىرگۈزۈش[0]).intersects (c->glyphs))
      أرجع;

    hb_set_t* cur_active_glyphs = c->ئىتتىرىش_cur_active_glyphs ();
    إذا (unlikely (!cur_active_glyphs))
      أرجع;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {هذا, هذا, هذا}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  كىرگۈزۈش.len, كىرگۈزۈش.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->چىقىرىش_cur_done_glyphs ();
  }

  فراغ closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    إذا (!intersects (c->glyphs))
      أرجع;

    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);

    (هذا+كىرگۈزۈش[0]).collect_coverage (c->كىرگۈزۈش);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    بنية ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {هذا, هذا, هذا}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 كىرگۈزۈش.len, كىرگۈزۈش.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  فراغ collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    إذا (كىرگۈزۈش.len > 1)
      (هذا+كىرگۈزۈش[1]).collect_coverage (digest);
    وإلا إذا (lookahead.len)
      (هذا+lookahead[0]).collect_coverage (digest);
    وإلا
      *digest = hb_set_digest_t::full ();
  }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {هذا, هذا, هذا}
    };
    أرجع chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     كىرگۈزۈش.len, كىرگۈزۈش.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت
  {
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    أرجع هذا+كىرگۈزۈش[0];
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);

    unsigned صحيح index = (هذا+كىرگۈزۈش[0]).get_coverage (c->buffer->cur().codepoint);
    إذا (index == NOT_COVERED) return_trace (خطأ);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بنية ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {هذا, هذا, هذا}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      كىرگۈزۈش.len, كىرگۈزۈش.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقي serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ثابت فراغ* base) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    إذا (unlikely (!out->serialize (c->serializer, 0u))) return_trace (خطأ);

    لكل (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      إذا (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (خطأ);
    }

    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (unlikely (!c->serializer->embed (هذا->format))) return_trace (خطأ);

    إذا (!serialize_coverage_offsets (c, backtrack.iter (), هذا))
      return_trace (خطأ);

    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    إذا (!serialize_coverage_offsets (c, كىرگۈزۈش.iter (), هذا))
      return_trace (خطأ);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    إذا (!serialize_coverage_offsets (c, lookahead.iter (), هذا))
      return_trace (خطأ);

    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_سانى_2 = c->serializer->copy<HBUINT16> (lookup.len);
    إذا (!lookup_سانى_2) return_trace (خطأ);

    unsigned سانى = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_سانى_2, سانى, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!backtrack.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت auto &كىرگۈزۈش = StructAfter<decltype (كىرگۈزۈش_X)> (backtrack);
    إذا (unlikely (!كىرگۈزۈش.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    إذا (unlikely (!كىرگۈزۈش.len)) return_trace (خطأ); /* To be consistent with Context. */
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (كىرگۈزۈش);
    إذا (unlikely (!lookahead.sanitize (c, هذا))) return_trace (خطأ);
    hb_barrier ();
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محمي:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		كىرگۈزۈش_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عام:
  DEFINE_SIZE_MIN (10);
};

بنية ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قىممەت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    افتراضي:return_trace (c->default_return_قىممەت ());
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }		format;	/* Format identifier */
  ChainContextFormat1_4<SmallTypes>	format1;
  ChainContextFormat2_5<SmallTypes>	format2;
  ChainContextFormat3<SmallTypes>	format3;
#ifndef HB_NO_BEYOND_64K
  ChainContextFormat1_4<MediumTypes>	format4;
  ChainContextFormat2_5<MediumTypes>	format5;
  /* The beyond-64k spec adds sequence-context format 6, but no
   * corresponding chained-context coverage format. */
#endif
  } u;
};


template <typename T>
بنية ExtensionFormat1
{
  unsigned صحيح get_type () ثابت { أرجع extensionLookupType; }

  template <typename X>
  ثابت X& get_subtable () ثابت
  { أرجع هذا + reinterpret_cast<ثابت Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, هذا))) أرجع c->no_dispatch_return_قىممەت ();
    TRACE_DISPATCH (هذا, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);

    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    ثابت auto& src_offset =
        reinterpret_cast<ثابت Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, هذا, get_type ()));
  }

  محمي:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  عام:
  DEFINE_SIZE_STATIC (8);
};

template <typename T>
بنية Extension
{
  unsigned صحيح get_type () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_type ();
    افتراضي:أرجع 0;
    }
  }
  template <typename X>
  ثابت X& get_subtable () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.template get_subtable<typename T::SubTable> ();
    افتراضي:أرجع Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.subset (c);
    افتراضي: أرجع c->default_return_قىممەت ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قىممەت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_قىممەت ());
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
  template <typename TLookup>
  ساكن hb_ot_layout_lookup_accelerator_t *create (ثابت TLookup &lookup)
  {
    unsigned سانى = lookup.get_subtable_سانى ();

    unsigned size = حجم (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * حجم (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    سانى * حجم (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    إذا (unlikely (!thiz))
      أرجع nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    طالما (سانى && !thiz->subtables[سانى - 1].apply_func)
      سانى--;

    thiz->digest.init ();
    لكل (auto& subtable : hb_iter (thiz->subtables, سانى))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->سانى = سانى;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    لكل (unsigned i = 0; i < سانى; i++)
      إذا (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    أرجع thiz;
  }

  فراغ fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    لكل (unsigned i = 0; i < سانى; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  منطقي may_have (hb_codepoint_t g) ثابت
  { أرجع digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقي apply (hb_ot_apply_context_t *c, منطقي use_cache) ثابت
  {
    إذا (سانى == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      إذا (use_cache)
	أرجع subtables[0].apply_cached_no_digest (c);
#endif
      أرجع subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    إذا (use_cache)
    {
      أرجع
      + hb_iter (hb_iter (subtables, سانى))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply_cached (c); })
      | hb_any
      ;
    }
    وإلا
#endif
    {
      أرجع
      + hb_iter (hb_iter (subtables, سانى))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { أرجع _.apply (c); })
      | hb_any
      ;
    }
    أرجع خطأ;
  }

  منطقي cache_enter (hb_ot_apply_context_t *c) ثابت
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    أرجع subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#وإلا
    أرجع خطأ;
#endif
  }
  فراغ cache_leave (hb_ot_apply_context_t *c) ثابت
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  خاص:
  unsigned سانى = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

بنية GSUBGPOS
{
  using LookupListT = LookupList<SmallTypes>;
  using LookupList2T = LookupList<MediumTypes>;
  template <typename TLookup>
  using TLookupList = List16OfOffsetTo<TLookup, typename SmallTypes::HBLUINT>;
  template <typename TLookup>
  using TLookupList2 = List16OfOffsetTo<TLookup, typename MediumTypes::HBLUINT>;
  template <typename TLookup>
  using TLookupOffsetList = LookupOffsetList<TLookup, typename SmallTypes::HBLUINT>;
  template <typename TLookup>
  using TLookupOffsetList2 = LookupOffsetList<TLookup, typename MediumTypes::HBLUINT>;

  size_t get_size () ثابت
  {
    إذا (version.major != 1) أرجع version.static_size;

    أرجع min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#وإلا
	   0
#endif
	   ;
  }

  ثابت ScriptList &get_script_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010002u && scriptList2) أرجع هذا + scriptList2;
#endif
    أرجع هذا + scriptList;
  }
  ثابت FeatureList &get_feature_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010002u && featureList2) أرجع هذا + featureList2;
#endif
    أرجع هذا + featureList;
  }
  template <typename TLookup>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (هذا, get_size ())))
      return_trace (خطأ);
    hb_barrier ();

    منطقي script_list_ok = scriptList.sanitize (c, هذا);
    منطقي feature_list_ok = featureList.sanitize (c, هذا);
    منطقي lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, هذا);

#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010002u)
    {
      إذا (scriptList2) script_list_ok = scriptList2.sanitize (c, هذا);
      إذا (featureList2) feature_list_ok = featureList2.sanitize (c, هذا);
      إذا (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, هذا);
    }
#endif

    إذا (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (خطأ);

#ifndef HB_NO_VAR
    إذا (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, هذا, &get_feature_list ()))))
      return_trace (خطأ);
#endif

    return_trace (true);
  }

  template <typename TLookup>
  منطقي subset (hb_subset_layout_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (version.major != 1) return_trace (خطأ);

    auto *out = c->subset_context->serializer->start_embed (هذا);
    إذا (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (خطأ);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010002u)
    {
      إذا (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (خطأ);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      إذا (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 هذا, c);
      وإلا
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 هذا, c);

      إذا (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 هذا, c);
      وإلا
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 هذا, c);

      إذا (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, هذا, c);
      وإلا
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, هذا, c);
    }
    وإلا
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     هذا,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     هذا,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					هذا,
					c);
    }

#ifndef HB_NO_VAR
    إذا (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      إذا (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (خطأ);

      // if all axes are pinned all feature vars are dropped.
      منطقي ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, هذا, c);
      إذا (!ret)
      {
        out->featureVars = 0;
	إذا (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (true);
  }

  unsigned صحيح get_lookup_سانى () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2).len;
#endif
    أرجع (هذا + lookupList).len;
  }
  ثابت Lookup& get_lookup (unsigned صحيح i) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010002u && lookupList2)
      أرجع (هذا + lookupList2)[i];
#endif
    أرجع (هذا + lookupList)[i];
  }
  ثابت FeatureVariations &get_feature_variations () ثابت
  {
    أرجع (version.to_int () >= 0x00010001u && hb_barrier ()) ? هذا+featureVars : Null (FeatureVariations);
  }

  منطقي has_data () ثابت { أرجع version.to_int (); }
  unsigned صحيح get_script_سانى () ثابت
  { أرجع get_script_list ().len; }
  ثابت Tag& get_script_tag (unsigned صحيح i) ثابت
  { أرجع get_script_list ().get_tag (i); }
  unsigned صحيح get_script_tags (unsigned صحيح start_offset,
				unsigned صحيح *script_سانى /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ثابت
  { أرجع get_script_list ().get_tags (start_offset, script_سانى, script_tags); }
  ثابت Script& get_script (unsigned صحيح i) ثابت
  { أرجع get_script_list ()[i]; }
  منطقي find_script_index (hb_tag_t tag, unsigned صحيح *index) ثابت
  { أرجع get_script_list ().find_index (tag, index); }

  unsigned صحيح get_feature_سانى () ثابت
  { أرجع get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned صحيح i) ثابت
  { أرجع i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned صحيح get_feature_tags (unsigned صحيح start_offset,
				 unsigned صحيح *feature_سانى_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) ثابت
  { أرجع get_feature_list ().get_tags (start_offset, feature_سانى_2, feature_tags); }
  ثابت Feature& get_feature (unsigned صحيح i) ثابت
  { أرجع get_feature_list ()[i]; }
  منطقي find_feature_index (hb_tag_t tag, unsigned صحيح *index) ثابت
  { أرجع get_feature_list ().find_index (tag, index); }

  منطقي find_variations_index (ثابت صحيح *coords, unsigned صحيح رەقەم_coords,
			      unsigned صحيح *index,
			      ItemVarStoreInstancer *instancer) ثابت
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    أرجع خطأ;
#endif
    أرجع get_feature_variations ().find_index (coords, رەقەم_coords, index, instancer);
  }
  منطقي get_feature_variations_state (ثابت صحيح *coords,
				     unsigned صحيح رەقەم_coords,
				     unsigned صحيح *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) ثابت
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    أرجع true;
#وإلا
    find_variations_index (coords, رەقەم_coords, index, instancer);
    أرجع get_feature_variations ().collect_lookup_conditions (
	coords,
	رەقەم_coords,
	instancer,
	lookup_conditions);
#endif
  }
  منطقي get_lookup_variations (unsigned feature_index,
                              unsigned variations_index,
                              hb_sorted_array_t<ثابت unsigned> conditions,
                              hb_set_t *lookup_indexes) ثابت
  {
#ifdef HB_NO_VAR
    أرجع خطأ;
#وإلا
    أرجع get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  ثابت Feature& get_feature_variation (unsigned صحيح feature_index,
					unsigned صحيح variations_index) ثابت
  {
#ifndef HB_NO_VAR
    إذا (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      ثابت Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      إذا (feature)
	أرجع *feature;
    }
#endif
    أرجع get_feature (feature_index);
  }

  فراغ feature_variation_collect_lookups (ثابت hb_set_t *feature_indexes,
					  ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) ثابت
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  فراغ collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  فراغ closure_lookups (hb_face_t      *face,
			ثابت hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) ثابت
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    لكل (unsigned lookup_index : *lookup_indexes)
      reinterpret_cast<ثابت TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  فراغ prune_langsys (ثابت hb_map_t *duplicate_feature_map,
                      ثابت hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) ثابت
  {
    hb_prune_langsys_context_t c (هذا, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned سانى = get_script_سانى ();
    لكل (unsigned script_index = 0; script_index < سانى; script_index++)
    {
      ثابت Tag& tag = get_script_tag (script_index);
      إذا (!layout_scripts->has (tag)) استمر;
      ثابت Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  فراغ prune_features (ثابت hb_map_t *lookup_indices, /* IN */
		       ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       ثابت hb_hashmap_t<unsigned, ثابت Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) ثابت
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    إذا (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      أرجع;
    }
#endif

    لكل (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      إذا (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        استمر;


      ثابت Feature *f = &(get_feature (i));
      ثابت Feature** p = nullptr;
      إذا (feature_substitutes_map->has (i, &p))
        f = *p;

      إذا (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        استمر;

      إذا (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  فراغ collect_name_ids (ثابت hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         ثابت hb_hashmap_t<unsigned, ثابت Feature*> *feature_substitutes_map,
                         ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         ثابت hb_set_t *catch_all_features,
                         منطقي all_axes_pinned HB_UNUSED) ثابت
  {
    unsigned سانى = get_feature_سانى ();
    لكل (unsigned i = 0 ; i < سانى; i++)
    {
      إذا (!feature_index_map->has (i)) استمر;
      hb_tag_t tag = get_feature_tag (i);
      ثابت Feature *feature = &get_feature (i);
      ثابت Feature **substitute;
      إذا (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        إذا (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    إذا (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [هذا] (unsigned i) { أرجع get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = true;
      هذا->table = sc.reference_table<T> (face);

      إذا (unlikely (هذا->table->is_blocklisted (هذا->table.get_blob (), face)))
      {
	hb_blob_destroy (هذا->table.get_blob ());
	هذا->table = hb_blob_get_empty ();
      }

      هذا->lookup_سانى = table->get_lookup_سانى ();

      هذا->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (هذا->lookup_سانى, حجم (*accels));
      إذا (unlikely (!هذا->accels))
      {
	هذا->lookup_سانى = 0;
	هذا->table.destroy ();
	هذا->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      لكل (unsigned صحيح i = 0; i < هذا->lookup_سانى; i++)
      {
	auto *accel = هذا->accels[i].get_relaxed ();
	إذا (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (هذا->accels);
      هذا->table.destroy ();
    }

    hb_blob_t *get_blob () ثابت { أرجع table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_index) ثابت
    {
      إذا (unlikely (lookup_index >= lookup_سانى)) أرجع nullptr;

    retry:
      auto *accel = accels[lookup_index].get_acquire ();
      إذا (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	إذا (unlikely (!accel))
	  أرجع nullptr;

	إذا (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  اذهب retry;
	}
      }

      أرجع accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned صحيح lookup_سانى;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  محمي:
  template <typename TLookup>
  ثابت Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () ثابت
  { أرجع reinterpret_cast<ثابت Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ثابت Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () ثابت
  { أرجع reinterpret_cast<ثابت Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  ثابت Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () ثابت
  { أرجع reinterpret_cast<ثابت Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { أرجع reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ثابت Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () ثابت
  { أرجع reinterpret_cast<ثابت Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  ثابت Offset16To<RecordListOfFeature>& subset_feature_list_offset () ثابت
  { أرجع reinterpret_cast<ثابت Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { أرجع reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  ثابت Offset32To<RecordListOfFeature>& subset_feature_list2_offset () ثابت
  { أرجع reinterpret_cast<ثابت Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  ثابت فراغ* get_lookup_list_field_offset () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    إذا (version.to_int () >= 0x00010002u && lookupList2) أرجع &lookupList2;
#endif
    أرجع &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  ساكن منطقي serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ثابت OffsetIn& in,
				       ثابت Base *base,
				       Ts&&... ds)
  {
    out = 0;
    إذا (in.is_null ()) أرجع خطأ;

    auto *s = c->serializer;
    s->push ();
    منطقي ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    إذا (ret) s->add_link (out, s->چىقىرىش_pack ());
    وإلا s->چىقىرىش_discard ();
    أرجع ret;
  }

  FixedVersion<>version;	/* Version of the GSUB/GPOS table--initially set
				 * to 0x00010000u */
  Offset16To<ScriptList>
		scriptList;	/* Offset to ScriptList table */
  Offset16To<FeatureList>
		featureList;	/* Offset to FeatureList table */
  Offset16To<LookupListT>
		lookupList;	/* Offset to LookupList table */
  Offset32To<FeatureVariations>
		featureVars;	/* Offset to Feature Variations table--from
				 * beginning of table (may be NULL).
				 * Introduced in version 0x00010001. */
#ifndef HB_NO_BEYOND_64K
  Offset32To<ScriptList>
		scriptList2;	/* 32-bit offset to ScriptList table.
				 * Introduced in version 0x00010002. */
  Offset32To<FeatureList>
		featureList2;	/* 32-bit offset to FeatureList table.
				 * Introduced in version 0x00010002. */
  Offset32To<LookupList2T>
		lookupList2;	/* 32-bit offset to LookupList2 table.
				 * Introduced in version 0x00010002. */
#endif
  عام:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
