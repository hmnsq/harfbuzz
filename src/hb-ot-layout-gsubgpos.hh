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

#شامل "hb.hh"
#شامل "hb-buffer.hh"
#شامل "hb-map.hh"
#شامل "hb-set.hh"
#شامل "hb-ot-map.hh"
#شامل "hb-ot-layout-common.hh"
#شامل "hb-ot-layout-gdef-table.hh"
#شامل "hb-depend-data.hh"


فضاینام OT {


ساختار hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, منطقی>
{
  template <typename T>
  return_t dispatch (ثابت T &obj) { بازگشت obj.intersects (این->glyphs); }
  ایستا return_t default_return_مقدار () { بازگشت نادرست; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { بازگشت r; }

  ثابت hb_set_t *glyphs;

  hb_intersects_context_t (ثابت hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

ساختار hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, منطقی>
{
  template <typename T>
  return_t dispatch (ثابت T &obj) { بازگشت obj.may_have_non_1to1 (); }
  ایستا return_t default_return_مقدار () { بازگشت نادرست; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { بازگشت r; }
};

ساختار hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  تعریفنوع return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index);
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.closure (این); بازگشت hb_empty_t (); }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }
  تهی recurse (unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index)
  {
    اگر (unlikely (nesting_level_چپ == 0 || !recurse_func))
      بازگشت;

    nesting_level_چپ--;
    recurse_func (این, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_چپ++;
  }

  تهی reset_lookup_visit_شمار ()
  { lookup_شمار = 0; }

  منطقی lookup_limit_exceeded ()
  { بازگشت lookup_شمار > HB_MAX_LOOKUP_VISIT_شمار; }

  منطقی should_visit_lookup (unsigned صحیح lookup_index)
  {
    اگر (lookup_شمار++ > HB_MAX_LOOKUP_VISIT_شمار)
      بازگشت نادرست;

    اگر (is_lookup_done (lookup_index))
      بازگشت نادرست;

    بازگشت درست;
  }

  منطقی is_lookup_done (unsigned صحیح lookup_index)
  {
    اگر (unlikely (done_lookups_glyph_شمار->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      بازگشت درست;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned برداشت = glyphs->get_population ();
    اگر (unlikely (برداشت == HB_MAP_مقدار_INVALID)) // Avoid sentinel interference.
      برداشت--;
    اگر (done_lookups_glyph_شمار->get (lookup_index) != برداشت)
    {
      done_lookups_glyph_شمار->set (lookup_index, برداشت);

      اگر (!done_lookups_glyph_set->has (lookup_index))
      {
	اگر (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  بازگشت درست;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    اگر (unlikely (covered_glyph_set->in_error ()))
      بازگشت درست;
    اگر (parent_active_glyphs ().is_subset (*covered_glyph_set))
      بازگشت درست;

    covered_glyph_set->union_ (parent_active_glyphs ());
    بازگشت نادرست;
  }

  ثابت hb_set_t& previous_parent_active_glyphs () {
    اگر (active_glyphs_پشته.length <= 1)
      بازگشت *glyphs;

    بازگشت active_glyphs_پشته[active_glyphs_پشته.length - 2];
  }

  ثابت hb_set_t& parent_active_glyphs ()
  {
    اگر (!active_glyphs_پشته)
      بازگشت *glyphs;

    بازگشت active_glyphs_پشته.tail ();
  }

  hb_set_t* افزودن_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_پشته.push ();
    اگر (unlikely (active_glyphs_پشته.in_error ()))
      بازگشت nullptr;
    بازگشت s;
  }

  منطقی برداشت_cur_done_glyphs ()
  {
    اگر (!active_glyphs_پشته)
      بازگشت نادرست;

    active_glyphs_پشته.برداشت ();
    بازگشت درست;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_پشته;
  recurse_func_t recurse_func = nullptr;
  unsigned صحیح nesting_level_چپ;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_شمار_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned صحیح nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_چپ (nesting_level_چپ_2),
			  done_lookups_glyph_شمار (done_lookups_glyph_شمار_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  تهی set_recurse_func (recurse_func_t func) { recurse_func = func; }

  تهی flush ()
  {
    output->del_range (face->get_رقم_glyphs (), HB_SET_مقدار_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_پشته.برداشت ();
    active_glyphs_پشته.reset ();
  }

  خصوصی:
  hb_map_t *done_lookups_glyph_شمار;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned صحیح lookup_شمار = 0;
};



ساختار hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  تعریفنوع return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_index);
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.closure_lookups (این); بازگشت hb_empty_t (); }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }
  تهی recurse (unsigned lookup_index)
  {
    اگر (unlikely (nesting_level_چپ == 0 || !recurse_func))
      بازگشت;

    /* Return if new lookup was recursed to before. */
    اگر (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      بازگشت;

    nesting_level_چپ--;
    recurse_func (این, lookup_index);
    nesting_level_چپ++;
  }

  تهی set_lookup_visited (unsigned lookup_index)
  { visited_lookups->add (lookup_index); }

  تهی set_lookup_inactive (unsigned lookup_index)
  { inactive_lookups->add (lookup_index); }

  منطقی lookup_limit_exceeded ()
  {
    منطقی ret = lookup_شمار > HB_MAX_LOOKUP_VISIT_شمار;
    اگر (ret)
      DEBUG_پیام (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    بازگشت ret; }

  منطقی is_lookup_visited (unsigned lookup_index)
  {
    اگر (unlikely (lookup_شمار++ > HB_MAX_LOOKUP_VISIT_شمار))
    {
      DEBUG_پیام (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_شمار, lookup_index);
      بازگشت درست;
    }

    اگر (unlikely (visited_lookups->in_error ()))
      بازگشت درست;

    بازگشت visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  ثابت hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned صحیح nesting_level_چپ;

  hb_closure_lookups_context_t (hb_face_t *face_,
				ثابت hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				unsigned nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_چپ (nesting_level_چپ_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_شمار (0) {}

  تهی set_recurse_func (recurse_func_t func) { recurse_func = func; }

  خصوصی:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned صحیح lookup_شمار;
};

ساختار hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, منطقی>
{
  template <typename T>
  return_t dispatch (ثابت T &obj) { بازگشت obj.would_apply (این); }
  ایستا return_t default_return_مقدار () { بازگشت نادرست; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { بازگشت r; }

  hb_face_t *face;
  ثابت hb_codepoint_t *glyphs;
  unsigned صحیح len;
  منطقی zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ثابت hb_codepoint_t *glyphs_,
			    unsigned صحیح len_,
			    منطقی zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

ساختار hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  تعریفنوع return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned صحیح lookup_index);
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.collect_glyphs (این); بازگشت hb_empty_t (); }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }
  تهی recurse (unsigned صحیح lookup_index)
  {
    اگر (unlikely (nesting_level_چپ == 0 || !recurse_func))
      بازگشت;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    اگر (output == hb_set_get_empty ())
      بازگشت;

    /* Return if new lookup was recursed to before. */
    اگر (recursed_lookups->has (lookup_index))
      بازگشت;

    hb_set_t *old_before = before;
    hb_set_t *old_درون_داد  = درون_داد;
    hb_set_t *old_after  = after;
    before = درون_داد = after = hb_set_get_empty ();

    nesting_level_چپ--;
    recurse_func (این, lookup_index);
    nesting_level_چپ++;

    before = old_before;
    درون_داد  = old_درون_داد;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *درون_داد;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned صحیح nesting_level_چپ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_درون_داد,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned صحیح nesting_level_چپ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      درون_داد  (glyphs_درون_داد  ? glyphs_درون_داد  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_چپ (nesting_level_چپ_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  تهی set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ساختار hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  شمارش glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  ایستا constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  ساختار glyph_set_cache_کلید_t
  {
    glyph_set_cache_کلید_t () = پیشفرض;
    glyph_set_cache_کلید_t (ثابت تهی *data_, unsigned مقدار_3, glyph_set_type_t type_)
      : data (data_), value (مقدار_3), type (type_) {}

    منطقی operator == (ثابت glyph_set_cache_کلید_t &o) ثابت
    { بازگشت data == o.data && value == o.value && type == o.type; }

    uint32_t hash () ثابت
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      بازگشت current * 31 + hb_hash ((unsigned) type);
    }

    ثابت تهی *data = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  ساختار context_set_cache_کلید_t
  {
    context_set_cache_کلید_t () = پیشفرض;
    context_set_cache_کلید_t (ثابت تهی *rule_,
			     ثابت تهی *record_,
			     ثابت تهی *data_,
			     hb_codepoint_t active_glyphs_,
			     unsigned مقدار_3,
			     unsigned format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (مقدار_3), format (format_) {}

    منطقی operator == (ثابت context_set_cache_کلید_t &o) ثابت
    {
      بازگشت rule == o.rule &&
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
      بازگشت current * 31 + hb_hash (format);
    }

    ثابت تهی *rule = nullptr;
    ثابت تهی *record = nullptr;
    ثابت تهی *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  ساختار recurse_کلید_t
  {
    recurse_کلید_t () = پیشفرض;
    recurse_کلید_t (unsigned lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    منطقی operator == (ثابت recurse_کلید_t &o) ثابت
    {
      بازگشت lookup_index == o.lookup_index &&
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
      بازگشت current;
    }

    unsigned lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  تعریفنوع return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.depend (این); بازگشت hb_empty_t (); }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }

  تهی recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
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
    اگر (lookups_seen.has (lookup_idx)) بازگشت;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_کلید = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    اگر (!recurse_path_to_index.has (path_کلید, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      اگر (unlikely (!recurse_path_to_index.set (path_کلید, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	بازگشت;
      }
    }
    وگرنه
      path_index = *path;
    recurse_path = path_index;
    recurse_func (این, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  ساختار active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    منطقی use_ریشه = نادرست;
  };

  ثابت hb_set_t& parent_active_glyphs ()
  {
    اگر (!active_glyphs_پشته_depth)
      بازگشت *glyphs;

    ثابت active_glyphs_frame_t &frame = active_glyphs_پشته[active_glyphs_پشته_depth - 1];
    بازگشت frame.use_ریشه ? *glyphs : frame.glyphs;
  }

  hb_set_t* افزودن_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    اگر (active_glyphs_پشته_depth == active_glyphs_پشته.length &&
	unlikely (!active_glyphs_پشته.افزودن_or_fail ()))
      بازگشت nullptr;

    active_glyphs_frame_t &frame = active_glyphs_پشته[active_glyphs_پشته_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_ریشه = نادرست;
    بازگشت &frame.glyphs;
  }

  منطقی افزودن_ریشه_active_glyphs ()
  {
    اگر (active_glyphs_پشته_depth == active_glyphs_پشته.length &&
	unlikely (!active_glyphs_پشته.افزودن_or_fail ()))
      بازگشت نادرست;

    active_glyphs_پشته[active_glyphs_پشته_depth++].use_ریشه = درست;
    بازگشت درست;
  }

  منطقی برداشت_cur_done_glyphs ()
  {
    اگر (!active_glyphs_پشته_depth)
      بازگشت نادرست;

    active_glyphs_پشته_depth--;
    بازگشت درست;
  }

  منطقی get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    اگر (!active_glyphs_پشته_depth ||
	active_glyphs_پشته[active_glyphs_پشته_depth - 1].use_ریشه)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      بازگشت درست;
    }

    active_glyphs_frame_t &frame = active_glyphs_پشته[active_glyphs_پشته_depth - 1];
    *active_idx = frame.active_idx;
    اگر (*active_idx != HB_CODEPOINT_INVALID)
      بازگشت درست;

    hb_codepoint_t glyph;
    اگر (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    وگرنه اگر (!find_or_create_recurse_set (frame.glyphs, active_idx))
      بازگشت نادرست;

    frame.active_idx = *active_idx;
    بازگشت درست;
  }

  منطقی get_context_set_cache_کلید (ثابت تهی *rule,
				  ثابت تهی *record,
				  ثابت تهی *data,
				  unsigned value,
				  unsigned format,
				  context_set_cache_کلید_t *key)
  {
    hb_codepoint_t active_idx;
    اگر (!get_parent_active_glyphs_index (&active_idx))
      بازگشت نادرست;
    *key = context_set_cache_کلید_t (rule, record, data, active_idx, value, format);
    بازگشت درست;
  }

  منطقی get_recurse_کلید (unsigned lookup_idx, recurse_کلید_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    اگر (!get_parent_active_glyphs_index (&active_idx))
      بازگشت نادرست;

    *key = recurse_کلید_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    بازگشت !completed_recursions.has (*key);
  }

  تهی finish_recurse (ثابت recurse_کلید_t &key)
  { depend_data->check_success (completed_recursions.set (key, درست)); }

  تهی reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  منطقی find_or_create_recurse_set (ثابت hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    اگر (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      بازگشت درست;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    اگر (unlikely (!copy))
      بازگشت depend_data->fail ();
    copy->set (set);
    اگر (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      بازگشت depend_data->fail ();
    }

    *index = recurse_sets.length;
    اگر (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      بازگشت depend_data->fail ();
    }
    اگر (unlikely (!recurse_sets.افزودن_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      بازگشت depend_data->fail ();
    اگر (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      بازگشت depend_data->fail ();
    بازگشت درست;
  }

  ثابت hb_set_t *find_or_create_glyph_set (ثابت تهی *data,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (ثابت hb_set_t *set);

  ساختار depend_scratch_t
  {
    تهی clear ()
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

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<ثابت hb_set_t *> backtrack_sets;
    hb_vector_t<ثابت hb_set_t *> lookahead_sets;
    hb_vector_t<ثابت hb_set_t *> درون_داد_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<unsigned> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    اگر (scratch)
      depend_scratch_pool = scratch->next;
    وگرنه
    {
      scratch = (depend_scratch_t *) hb_malloc (اندازه (depend_scratch_t));
      اگر (unlikely (!scratch))
      {
        depend_data->fail ();
        بازگشت nullptr;
      }
      جدید (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    بازگشت scratch;
  }

  تهی release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_پشته;
  unsigned active_glyphs_پشته_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<ثابت hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_کلید_t, منطقی> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_کلید_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_کلید_t, hb_codepoint_t> context_set_cache;
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
    وقتی (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  تهی set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

template <typename set_t>
ساختار hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, ثابت Coverage &>
{
  تعریفنوع ثابت Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (ثابت T &obj) { بازگشت obj.get_coverage (); }
  ایستا return_t default_return_مقدار () { بازگشت Null (Coverage); }
  منطقی stop_sublookup_iteration (return_t r) ثابت
  {
    r.collect_coverage (set);
    بازگشت نادرست;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

ساختار matcher_t
{
  تعریفنوع منطقی (*match_func_t) (hb_glyph_info_t &info, unsigned value, ثابت تهی *data);

  template <typename context_t>
  تهی init (ثابت context_t *c, منطقی context_match = نادرست)
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

  تهی set_match_func (match_func_t match_func_,
		       ثابت تهی *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  شمارش may_match_t {
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
    اگر (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      بازگشت MATCH_NO;

    اگر (match_func)
      بازگشت match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    بازگشت MATCH_MAYBE;
  }

  شمارش may_skip_t {
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
    اگر (!c->check_glyph_property (&info, lookup_props))
      بازگشت SKIP_YES;

    اگر (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      بازگشت SKIP_MAYBE;

    بازگشت SKIP_NO;
  }

  عمومی:
  unsigned صحیح lookup_props = 0;
  hb_mask_t mask = -1;
  منطقی ignore_zwnj = نادرست;
  منطقی ignore_zwj = نادرست;
  منطقی ignore_hidden = نادرست;
  منطقی per_syllable = نادرست;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  ثابت تهی *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
ساختار glyph_data_t
{
  template <typename T>
  glyph_data_t (ثابت T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (اندازه (T) == 3)
#endif
  {
    static_assert (اندازه (T) == 2 || اندازه (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (wide)
      بازگشت ((ثابت HBUINT24 *) data)[i];
#endif
    بازگشت ((ثابت HBUINT16 *) data)[i];
  }

  ثابت تهی *data;
#ifndef HB_NO_BEYOND_64K
  منطقی wide;
#endif
};

template <typename context_t>
ساختار skipping_iterator_t
{
  تهی init (context_t *c_, منطقی context_match = نادرست)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  تهی set_lookup_props (unsigned صحیح lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  تهی set_match_func (matcher_t::match_func_t match_func_,
		       ثابت تهی *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  تهی set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (ثابت HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (ثابت HBUINT24 *) glyph_data.data : nullptr;
#وگرنه
    match_glyph_data16 = (ثابت HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  تهی reset (unsigned صحیح start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  تهی reset_back (unsigned صحیح start_index_, منطقی from_out_buffer = نادرست)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  تهی reset_fast (unsigned صحیح start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (ثابت hb_glyph_info_t &info) ثابت
  { بازگشت matcher.may_skip (c, info); }

  شمارش match_t {
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
    اگر (unlikely (skip == matcher_t::SKIP_YES))
      بازگشت SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    اگر (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      بازگشت MATCH;

    اگر (skip == matcher_t::SKIP_NO)
      بازگشت NOT_MATCH;

    بازگشت SKIP;
  }

  منطقی next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->buffer->info;
    ثابت signed stop = (signed) end - 1;
    وقتی ((signed) idx < stop)
    {
      idx++;
      انتخاب (match (info[idx]))
      {
	حالت MATCH:
	{
	  advance_glyph_data ();
	  بازگشت درست;
	}
	حالت NOT_MATCH:
	{
	  اگر (unsafe_to)
	    *unsafe_to = idx + 1;
	  بازگشت نادرست;
	}
	حالت SKIP:
	  ادامه;
      }
    }
    اگر (unsafe_to)
      *unsafe_to = end;
    بازگشت نادرست;
  }
  منطقی prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->buffer->out_info;
    ثابت unsigned stop = 0;
    وقتی (idx > stop)
    {
      idx--;
      انتخاب (match (out_info[idx]))
      {
	حالت MATCH:
	{
	  advance_glyph_data ();
	  بازگشت درست;
	}
	حالت NOT_MATCH:
	{
	  اگر (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  بازگشت نادرست;
	}
	حالت SKIP:
	  ادامه;
      }
    }
    اگر (unsafe_from)
      *unsafe_from = 0;
    بازگشت نادرست;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    اگر (match_glyph_data16) بازگشت *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    وگرنه
    اگر (match_glyph_data24) بازگشت *match_glyph_data24;
#endif
    بازگشت 0;
  }
  HB_ALWAYS_INLINE
  تهی
  advance_glyph_data ()
  {
    اگر (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    وگرنه
    اگر (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned صحیح idx;
  محافظتشده:
  context_t *c;
  matcher_t matcher;
  ثابت HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  ثابت HBUINT24 *match_glyph_data24;
#endif

  unsigned صحیح end;
};

ساختار hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, منطقی, HB_DEBUG_APPLY>
{
  ثابت نویسه *get_name () { بازگشت "APPLY"; }
  تعریفنوع return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned صحیح lookup_index);

  template <typename T>
  ایستا درونخطی auto apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  ایستا درونخطی auto apply_ (ثابت T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (ثابت T &obj) { بازگشت apply_(obj, این, hb_prioritize); }

  ایستا return_t default_return_مقدار () { بازگشت نادرست; }
  منطقی stop_sublookup_iteration (return_t r) ثابت { بازگشت r; }
  return_t recurse (unsigned صحیح sub_lookup_index)
  {
    أكد (recurse_func);
    اگر (unlikely (nesting_level_چپ == 0))
    {
      buffer->successful = نادرست;
      بازگشت default_return_مقدار ();
    }

    buffer->max_ops--;
    اگر (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = نادرست;
      بازگشت default_return_مقدار ();
    }

    nesting_level_چپ--;
    منطقی ret = recurse_func (این, sub_lookup_index);
    nesting_level_چپ++;
    بازگشت ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_درون_داد, iter_context;

  unsigned صحیح table_index; /* GSUB/GPOS */
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
  unsigned صحیح lookup_index = (unsigned) -1;
  unsigned صحیح lookup_props = 0;
  unsigned صحیح nesting_level_چپ = HB_MAX_NESTING_LEVEL;

  منطقی has_glyph_classes;
  منطقی auto_zwnj = درست;
  منطقی auto_zwj = درست;
  منطقی per_syllable = نادرست;
  منطقی random = نادرست;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t پشته_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  تهی *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (unsigned صحیح table_index_,
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
#وگرنه
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#وگرنه
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (پشته_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    اگر (buffer->messaging ())
    {
      أكد (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = این;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    اگر (buffer->messaging ())
    {
      أكد (buffer->changed_func == buffer_changed_trampoline);
      أكد (buffer->changed_data == این);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  تهی init_iters ()
  {
    iter_درون_داد.init (این, نادرست);
    iter_context.init (این, درست);
  }

  تهی set_lookup_mask (hb_mask_t mask, منطقی init = درست) { lookup_mask = mask; last_base = -1; last_base_until = 0; اگر (init) init_iters (); }
  تهی set_auto_zwj (منطقی auto_zwj_, منطقی init = درست) { auto_zwj = auto_zwj_; اگر (init) init_iters (); }
  تهی set_auto_zwnj (منطقی auto_zwnj_, منطقی init = درست) { auto_zwnj = auto_zwnj_; اگر (init) init_iters (); }
  تهی set_per_syllable (منطقی per_syllable_, منطقی init = درست) { per_syllable = per_syllable_; اگر (init) init_iters (); }
  تهی set_random (منطقی random_) { random = random_; }
  تهی set_recurse_func (recurse_func_t func) { recurse_func = func; }
  تهی set_lookup_index (unsigned صحیح lookup_index_) { lookup_index = lookup_index_; }
  تهی set_lookup_props (unsigned صحیح lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_عدد ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    بازگشت buffer->random_state;
  }

  ایستا تهی buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, تهی *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  تهی buffer_changed ()
  {
    buffer->update_digest ();
    اگر (likely (has_glyph_classes))
      برای (unsigned i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  منطقی match_properties_mark (ثابت hb_glyph_info_t *info,
			      unsigned صحیح    glyph_props,
			      unsigned صحیح    match_props) ثابت
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    اگر (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_index = match_props >> 16;
      بازگشت gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    اگر (match_props & LookupFlag::MarkAttachmentType)
      بازگشت (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    بازگشت درست;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقی check_glyph_property (ثابت hb_glyph_info_t *info,
			     unsigned match_props) ثابت
  {
    unsigned صحیح glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    اگر (glyph_props & match_props & LookupFlag::IgnoreFlags)
      بازگشت نادرست;

    اگر (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      بازگشت match_properties_mark (info, glyph_props, match_props);

    بازگشت درست;
  }

  تهی _set_glyph_class (hb_codepoint_t glyph_index,
			  unsigned صحیح class_guess = 0,
			  منطقی ligature = نادرست,
			  منطقی component = نادرست)
  {
    buffer->digest.add (glyph_index);

    اگر (new_syllables != (unsigned) -1)
      buffer->cur().syllable() = new_syllables;

    unsigned صحیح props = _hb_glyph_info_get_glyph_props (&buffer->cur());
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
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    وگرنه اگر (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    وگرنه
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  تهی _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned صحیح props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  تهی _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  تهی replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (تهی) buffer->replace_glyph (glyph_index);
  }
  تهی replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  تهی replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    unsigned صحیح class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, درست);
    (تهی) buffer->replace_glyph (glyph_index);
  }
  تهی output_glyph_for_component (hb_codepoint_t glyph_index,
				   unsigned صحیح class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, نادرست, درست);
    (تهی) buffer->output_glyph (glyph_index);
  }
};

شمارش رده hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

ساختار hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  template <typename T>
  ایستا درونخطی auto collect_second_glyphs_ (ثابت T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (تهی, obj.collect_second_glyphs (digest) )
  template <typename T>
  ایستا درونخطی تهی collect_second_glyphs_ (ثابت T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  ایستا درونخطی auto apply_ (ثابت T *obj, hb_ot_apply_context_t *c, تهی *external_cache, hb_priority<1>) HB_RETURN (منطقی, obj->apply (c, external_cache) )
  template <typename T>
  ایستا درونخطی auto apply_ (ثابت T *obj, hb_ot_apply_context_t *c, تهی *external_cache, hb_priority<0>) HB_RETURN (منطقی, obj->apply (c) )
  template <typename T>
  ایستا درونخطی منطقی apply_to (ثابت تهی *obj, hb_ot_apply_context_t *c, تهی *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    بازگشت apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  ایستا درونخطی auto apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, تهی *external_cache, hb_priority<2>) HB_RETURN (منطقی, obj->apply_cached (c, external_cache) )
  template <typename T>
  ایستا درونخطی auto apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, تهی *external_cache, hb_priority<1>) HB_RETURN (منطقی, obj->apply (c, external_cache) )
  template <typename T>
  ایستا درونخطی auto apply_cached_ (ثابت T *obj, hb_ot_apply_context_t *c, تهی *external_cache, hb_priority<0>) HB_RETURN (منطقی, obj->apply (c) )
  template <typename T>
  ایستا درونخطی منطقی apply_cached_to (ثابت تهی *obj, hb_ot_apply_context_t *c, تهی *external_cache)
  {
    ثابت T *typed_obj = (ثابت T *) obj;
    بازگشت apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  ایستا درونخطی auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (منطقی, T::cache_func (c, op) )
  template <typename T=تهی>
  ایستا درونخطی منطقی cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { بازگشت نادرست; }
  template <typename Type>
  ایستا درونخطی منطقی cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    بازگشت cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  تعریفنوع منطقی (*hb_apply_func_t) (ثابت تهی *obj, hb_ot_apply_context_t *c, تهی *external_cache);
  تعریفنوع منطقی (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  ساختار hb_applicable_t
  {
    friend ساختار hb_accelerate_subtables_context_t;
    friend ساختار hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    تهی init (ثابت T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , تهی *external_cache_
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
    منطقی apply (hb_ot_apply_context_t *c) ثابت
    {
      بازگشت digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    منطقی apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      بازگشت apply_func (obj, c, nullptr);
    }
#وگرنه
    منطقی apply (hb_ot_apply_context_t *c) ثابت
    {
      بازگشت digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    منطقی apply_cached (hb_ot_apply_context_t *c) ثابت
    {
      بازگشت digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    منطقی apply_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      بازگشت apply_func (obj, c, external_cache);
    }
    منطقی apply_cached_no_digest (hb_ot_apply_context_t *c) ثابت
    {
      بازگشت apply_cached_func (obj, c, external_cache);
    }

    منطقی cache_enter (hb_ot_apply_context_t *c) ثابت
    {
      بازگشت cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    تهی cache_leave (hb_ot_apply_context_t *c) ثابت
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    خصوصی:
    ثابت تهی *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    تهی *external_cache;
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
    تهی *external_cache = nullptr;
    اگر (i < 8)
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
    اگر (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    بازگشت hb_empty_t ();
  }
  ایستا return_t default_return_مقدار () { بازگشت hb_empty_t (); }

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


تعریفنوع منطقی (*intersects_func_t) (ثابت hb_set_t *glyphs, unsigned value, ثابت تهی *data, تهی *cache);
تعریفنوع تهی (*intersected_glyphs_func_t) (ثابت hb_set_t *glyphs, ثابت تهی *data, unsigned value, hb_set_t *intersected_glyphs, تهی *cache);
تعریفنوع تهی (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, ثابت تهی *data);
تعریفنوع منطقی (*match_func_t) (hb_glyph_info_t &info, unsigned value, ثابت تهی *data);
تعریفنوع unsigned (*get_مقدار_func_t) (hb_glyph_info_t &info, ثابت تهی *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
ساختار hb_ot_layout_ruleset_digest_t
{
  تهی init () { mask = 0; }
  تهی set_full () { mask = (uint64_t) -1; }
  تهی add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  منطقی may_have (unsigned value) ثابت { بازگشت mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

ساختار ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
ساختار ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
ساختار ContextApplyFuncs
{
  match_func_t match;
};
ساختار ChainContextApplyFuncs
{
  match_func_t match[3];
};


ایستا درونخطی منطقی intersects_glyph (ثابت hb_set_t *glyphs, unsigned value, ثابت تهی *data HB_UNUSED, تهی *cache HB_UNUSED)
{
  بازگشت glyphs->has (value);
}
ایستا درونخطی منطقی intersects_class (ثابت hb_set_t *glyphs, unsigned value, ثابت تهی *data, تهی *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  اگر (map->has (value, &cached_v))
    بازگشت *cached_v;

  منطقی v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  بازگشت v;
}
ایستا درونخطی منطقی intersects_coverage (ثابت hb_set_t *glyphs, unsigned value, ثابت تهی *data, تهی *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  بازگشت (data+coverage).intersects (glyphs);
}


ایستا درونخطی تهی intersected_glyph (ثابت hb_set_t *glyphs HB_UNUSED, ثابت تهی *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED تهی *cache)
{
  unsigned g = reinterpret_cast<ثابت HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

ایستا درونخطی تهی intersected_class_glyphs (ثابت hb_set_t *glyphs, ثابت تهی *data, unsigned value, hb_set_t *intersected_glyphs, تهی *cache)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  اگر (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    بازگشت;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

ایستا درونخطی تهی intersected_coverage_glyphs (ثابت hb_set_t *glyphs, ثابت تهی *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED تهی *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


ایستا درونخطی منطقی array_is_subset_of (ثابت hb_set_t *glyphs,
				       unsigned صحیح شمار,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       ثابت تهی *intersects_data,
				       تهی *cache)
{
  برای (unsigned i = 0; i < شمار; i++)
    اگر (!intersects_func (glyphs, values[i], intersects_data, cache)) بازگشت نادرست;
  بازگشت درست;
}


ایستا درونخطی تهی collect_glyph (hb_set_t *glyphs, unsigned value, ثابت تهی *data HB_UNUSED)
{
  glyphs->add (value);
}
ایستا درونخطی تهی collect_class (hb_set_t *glyphs, unsigned value, ثابت تهی *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
ایستا درونخطی تهی collect_coverage (hb_set_t *glyphs, unsigned value, ثابت تهی *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

درونخطی ثابت hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (ثابت تهی *data,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_کلید_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  اگر (glyph_set_to_index.has (key, &existing))
    بازگشت glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  اگر (unlikely (!set))
  {
    depend_data->fail ();
    بازگشت nullptr;
  }

  اگر (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  وگرنه اگر (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  وگرنه
    set->add (value);
  اگر (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    بازگشت nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  اگر (unlikely (!glyph_sets.افزودن_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    بازگشت nullptr;
  }
  اگر (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    بازگشت nullptr;
  }
  بازگشت glyph_sets[index].get ();
}

درونخطی hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (ثابت hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  اگر (cached_context_set_indices.has (key, &existing))
    بازگشت *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  اگر (unlikely (index == HB_CODEPOINT_INVALID))
    بازگشت HB_CODEPOINT_INVALID;
  اگر (unlikely (!cached_context_set_indices.set (key, index)))
    بازگشت depend_data->fail_invalid ();
  بازگشت index;
}

ایستا درونخطی تهی collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned صحیح شمار,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  ثابت تهی *collect_data)
{
  برای (unsigned i = 0; i < شمار; i++)
    collect_func (glyphs, values[i], collect_data);
}


ایستا درونخطی منطقی match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, ثابت تهی *data HB_UNUSED)
{
  بازگشت درست;
}
ایستا درونخطی منطقی match_glyph (hb_glyph_info_t &info, unsigned value, ثابت تهی *data HB_UNUSED)
{
  بازگشت info.codepoint == value;
}
ایستا درونخطی منطقی match_class (hb_glyph_info_t &info, unsigned value, ثابت تهی *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  بازگشت class_def.get_class (info.codepoint) == value;
}
ایستا درونخطی unsigned get_class_مقدار (hb_glyph_info_t &info, ثابت تهی *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (data);
  بازگشت class_def.get_class (info.codepoint);
}
ساختار match_class_cache_data_t
{
  ثابت ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
ایستا درونخطی منطقی match_class_cache (hb_glyph_info_t &info, unsigned value, ثابت تهی *data)
{
  ثابت auto &cache_data = *reinterpret_cast<ثابت match_class_cache_data_t *> (data);
  بازگشت cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
ایستا درونخطی unsigned get_class_cache_مقدار (hb_glyph_info_t &info, ثابت تهی *data)
{
  ثابت auto &cache_data = *reinterpret_cast<ثابت match_class_cache_data_t *> (data);
  بازگشت cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
ایستا درونخطی unsigned get_class_cached (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  اگر (klass < 255)
    بازگشت klass;
  klass = class_def.get_class (info.codepoint);
  اگر (likely (klass < 255))
    info.syllable() = klass;
  بازگشت klass;
}
ایستا درونخطی منطقی match_class_cached (hb_glyph_info_t &info, unsigned value, ثابت تهی *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  بازگشت get_class_cached (class_def, info) == value;
}
ایستا درونخطی unsigned get_class_cached_مقدار (hb_glyph_info_t &info, ثابت تهی *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (data);
  بازگشت get_class_cached (class_def, info);
}
ایستا درونخطی unsigned get_class_cached1 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  اگر (klass < 15)
    بازگشت klass;
  klass = class_def.get_class (info.codepoint);
  اگر (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  بازگشت klass;
}
ایستا درونخطی منطقی match_class_cached1 (hb_glyph_info_t &info, unsigned value, ثابت تهی *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  بازگشت get_class_cached1 (class_def, info) == value;
}
ایستا درونخطی unsigned get_class_cached2 (ثابت ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  اگر (klass < 15)
    بازگشت klass;
  klass = class_def.get_class (info.codepoint);
  اگر (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  بازگشت klass;
}
ایستا درونخطی منطقی match_class_cached2 (hb_glyph_info_t &info, unsigned value, ثابت تهی *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *>(data);
  بازگشت get_class_cached2 (class_def, info) == value;
}
ایستا درونخطی unsigned get_class_cached2_مقدار (hb_glyph_info_t &info, ثابت تهی *data)
{
  ثابت ClassDef &class_def = *reinterpret_cast<ثابت ClassDef *> (data);
  بازگشت get_class_cached2 (class_def, info);
}
ایستا درونخطی منطقی match_coverage (hb_glyph_info_t &info, unsigned value, ثابت تهی *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  بازگشت (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

ایستا درونخطی منطقی would_match_درون_داد (hb_would_apply_context_t *c,
				      unsigned صحیح شمار, /* Including the first glyph (not matched) */
				      glyph_data_t درون_داد, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      ثابت تهی *match_data)
{
  اگر (شمار != c->len)
    بازگشت نادرست;

  برای (unsigned صحیح i = 1; i < شمار; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    اگر (likely (!match_func (info, درون_داد[i - 1], match_data)))
      بازگشت نادرست;
  }

  بازگشت درست;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#وگرنه
درونخطی
#endif
ایستا منطقی match_درون_داد (hb_ot_apply_context_t *c,
			 unsigned صحیح شمار, /* Including the first glyph (not matched) */
			 glyph_data_t درون_داد, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 ثابت تهی *match_data,
			 unsigned صحیح *end_position,
			 unsigned صحیح *p_total_component_شمار = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  اگر (شمار == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    اگر (p_total_component_شمار)
      *p_total_component_شمار = hb_glyph_info_get_lig_رقم_comps (&buffer->cur());
    return_trace (درست);
  }

  اگر (unlikely (شمار > HB_MAX_CONTEXT_LENGTH)) return_trace (نادرست);

  auto &skippy_iter = c->iter_درون_داد;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (درون_داد);

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

  unsigned صحیح total_component_شمار = 0;

  unsigned صحیح first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned صحیح first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  شمارش {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  برای (unsigned صحیح i = 1; i < شمار; i++)
  {
    unsigned unsafe_to;
    اگر (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (نادرست);
    }

    اگر (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (نادرست);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned صحیح this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    unsigned صحیح this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    اگر (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      اگر (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	اگر (ligbase == LIGBASE_NOT_CHECKED)
	{
	  منطقی found = نادرست;
	  ثابت auto *out = buffer->out_info;
	  unsigned صحیح j = buffer->out_len;
	  وقتی (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    اگر (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = درست;
	      بشکن;
	    }
	    j--;
	  }

	  اگر (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  وگرنه
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	اگر (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (نادرست);
      }
    }
    وگرنه
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      اگر (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (نادرست);
    }

    total_component_شمار += hb_glyph_info_get_lig_رقم_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  اگر (p_total_component_شمار)
  {
    total_component_شمار += hb_glyph_info_get_lig_رقم_comps (&buffer->cur());
    *p_total_component_شمار = total_component_شمار;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (درست);
}
ایستا درونخطی منطقی ligate_درون_داد (hb_ot_apply_context_t *c,
				 unsigned صحیح شمار, /* Including the first glyph */
				 unsigned صحیح match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned صحیح total_component_شمار)
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

  منطقی is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  منطقی is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  برای (unsigned صحیح i = 1; i < شمار; i++)
    اگر (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = نادرست;
      is_mark_ligature = نادرست;
      بشکن;
    }
  منطقی is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned صحیح klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned صحیح lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  unsigned صحیح last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned صحیح last_رقم_components = hb_glyph_info_get_lig_رقم_comps (&buffer->cur());
  unsigned صحیح components_so_far = last_رقم_components;

  اگر (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_شمار);
    اگر (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  برای (unsigned صحیح i = 1; i < شمار; i++)
  {
    وقتی (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      اگر (is_ligature)
      {
	unsigned صحیح this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	اگر (this_comp == 0)
	  this_comp = last_رقم_components;
	أكد (components_so_far >= last_رقم_components);
	unsigned صحیح new_lig_comp = components_so_far - last_رقم_components +
				    hb_min (this_comp, last_رقم_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (تهی) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_رقم_components = hb_glyph_info_get_lig_رقم_comps_in_ligation (&buffer->cur());
    components_so_far += last_رقم_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  اگر (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    برای (unsigned i = buffer->idx; i < buffer->len; ++i)
    {
      اگر (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) بشکن;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      اگر (!this_comp) بشکن;

      أكد (components_so_far >= last_رقم_components);
      unsigned new_lig_comp = components_so_far - last_رقم_components +
			      hb_min (this_comp, last_رقم_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (درست);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#وگرنه
درونخطی
#endif
ایستا منطقی match_backtrack (hb_ot_apply_context_t *c,
			     unsigned صحیح شمار,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     ثابت تهی *match_data,
			     unsigned صحیح *match_start)
{
  TRACE_APPLY (nullptr);

  اگر (!شمار)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (درست);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  برای (unsigned صحیح i = 0; i < شمار; i++)
  {
    unsigned unsafe_from;
    اگر (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (نادرست);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (درست);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#وگرنه
درونخطی
#endif
ایستا منطقی match_lookahead (hb_ot_apply_context_t *c,
			     unsigned صحیح شمار,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     ثابت تهی *match_data,
			     unsigned صحیح start_index,
			     unsigned صحیح *end_index)
{
  TRACE_APPLY (nullptr);

  اگر (!شمار)
  {
    *end_index = start_index;
    return_trace (درست);
  }

  auto &skippy_iter = c->iter_context;
  أكد (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  برای (unsigned صحیح i = 0; i < شمار; i++)
  {
    unsigned unsafe_to;
    اگر (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (نادرست);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (درست);
}



ساختار LookupRecord
{
  منطقی serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t         *lookup_map) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->embed (*این);
    اگر (unlikely (!out)) return_trace (نادرست);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  عمومی:
  DEFINE_SIZE_STATIC (4);
};

ایستا درونخطی unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ثابت hb_array_t<ثابت LookupRecord> lookupRecords,
						     ثابت hb_map_t *lookup_map)
{
  unsigned شمار = 0;
  برای (ثابت LookupRecord& r : lookupRecords)
  {
    اگر (!lookup_map->has (r.lookupListIndex))
      ادامه;

    اگر (!r.serialize (c, lookup_map))
      بازگشت 0;

    شمار++;
  }
  بازگشت شمار;
}

شمارش ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

ایستا درونخطی ثابت hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ثابت تهی *data,
			unsigned value,
			منطقی filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  انتخاب (context_format)
  {
    حالت ContextFormat::SimpleContext:
      بازگشت c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    حالت ContextFormat::ClassBasedContext:
    {
      ثابت hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      اگر (unlikely (!class_set))
	بازگشت nullptr;
      اگر (!filter_class_by_parent)
	بازگشت class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      بشکن;
    }
    حالت ContextFormat::CoverageBasedContext:
      بازگشت c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  اگر (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    بازگشت nullptr;
  }
  بازگشت scratch;
}

ایستا درونخطی تهی context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned درون_داد_شمار, glyph_data_t درون_داد,
					     unsigned lookup_شمار_2,
					     ثابت LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     ثابت تهی *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     تهی *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  برای (unsigned صحیح i = 0; i < lookup_شمار_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    اگر (seqIndex >= درون_داد_شمار) ادامه;

    منطقی has_pos_glyphs = نادرست;

    اگر (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = درست;
      pos_glyphs.clear ();
      اگر (seqIndex == 0)
      {
        انتخاب (context_format) {
        حالت ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          بشکن;
        حالت ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          بشکن;
        حالت ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          بشکن;
        }
      }
      وگرنه
      {
        ثابت تهی *درون_داد_data = درون_داد.data;
        unsigned درون_داد_مقدار = seqIndex - 1;
        اگر (context_format != ContextFormat::SimpleContext)
        {
          درون_داد_data = data;
          درون_داد_مقدار = درون_داد[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, درون_داد_data, درون_داد_مقدار, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs))
      بازگشت;
    اگر (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } وگرنه {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned endIndex = درون_داد_شمار;
    اگر (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->برداشت_cur_done_glyphs ();
  }
}

ایستا منطقی
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned درون_داد_شمار_2,
				unsigned lookup_شمار,
				ثابت LookupRecord lookup_records[],
				ثابت تهی *rule,
				unsigned value,
				ContextFormat context_format,
				ثابت تهی *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  منطقی have_active_idx = نادرست;
  برای (unsigned i = 0; i < lookup_شمار; i++)
  {
    اگر (lookup_records[i].sequenceIndex >= درون_داد_شمار_2)
      ادامه;

    اگر (!have_active_idx)
    {
      اگر (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	بازگشت نادرست;
      have_active_idx = درست;
    }

    hb_depend_context_t::context_set_cache_کلید_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    اگر (!c->context_set_cache.has (key, &cached_context_set))
      بازگشت نادرست;
    اگر (unlikely (!cached_context_sets->افزودن_or_fail (*cached_context_set)))
      بازگشت c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  بازگشت have_active_idx;
}

ایستا درونخطی تهی context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned درون_داد_شمار, glyph_data_t درون_داد,
					     unsigned lookup_شمار_2,
					     ثابت LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     ثابت تهی *rule,
					     unsigned value,
					     ContextFormat context_format,
					     ثابت تهی *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     تهی *cache,
					     ثابت hb_set_t &preliminary_context,
					     ثابت hb_vector_t<ثابت hb_set_t *> *درون_داد_position_glyphs,
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
  اگر (unlikely (درون_داد_شمار > HB_MAX_CONTEXT_LENGTH))
    بازگشت;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_index = 0;
  برای (unsigned صحیح i = 0; i < lookup_شمار_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    اگر (seqIndex >= درون_داد_شمار)
      ادامه;

    hb_depend_context_t::context_set_cache_کلید_t context_cache_کلید;
    اگر (unlikely (!c->get_context_set_cache_کلید (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_کلید)))
      بازگشت;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    اگر (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    وگرنه اگر (c->context_set_cache.has (context_cache_کلید, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    وگرنه
    {
      اگر (unlikely (!درون_داد_position_glyphs))
      {
	c->depend_data->fail ();
	بازگشت;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_مقدار_INVALID;
      وقتی (preliminary_context.next (&elem)) {
	اگر (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} وگرنه {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      برای (unsigned j = 0; j < درون_داد_position_glyphs->length; j++) {
	اگر (j != seqIndex && (*درون_داد_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*درون_داد_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_مقدار_INVALID;
      وقتی (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	ثابت hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	اگر (unlikely (!original_set)) ادامه;

	/* Dependency context sets are never inverted. */
	اگر (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      برای (unsigned j = 0; j < درون_داد_position_glyphs->length; j++) {
	اگر (j != seqIndex && (*درون_داد_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  اگر (!(*درون_داد_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    منطقی cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*درون_داد_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*درون_داد_position_glyphs)[j]);
	    اگر (unlikely (idx == HB_CODEPOINT_INVALID))
	      بازگشت;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      اگر (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	بازگشت;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      اگر (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	بازگشت;

      اگر (unlikely (!c->context_set_cache.set (context_cache_کلید, context_set_idx)))
      {
	c->depend_data->fail ();
	بازگشت;
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
    منطقی nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

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
    اگر (درون_داد_شمار > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    اگر (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    منطقی has_pos_glyphs = نادرست;

    اگر (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = درست;
      pos_glyphs.clear ();
      اگر (seqIndex == 0)
      {
        انتخاب (context_format) {
        حالت ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          بشکن;
        حالت ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          بشکن;
        حالت ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          بشکن;
        }
      }
      وگرنه
      {
        ثابت تهی *درون_داد_data = درون_داد.data;
        unsigned درون_داد_مقدار = seqIndex - 1;
        اگر (context_format != ContextFormat::SimpleContext)
        {
          درون_داد_data = data;
          درون_داد_مقدار = درون_داد[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, درون_داد_data, درون_داد_مقدار, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    اگر (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      اگر (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	اگر (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->افزودن_cur_active_glyphs (active_idx);
      اگر (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	بازگشت;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    وگرنه اگر (unlikely (!c->افزودن_ریشه_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      بازگشت;
    }

    unsigned endIndex = درون_داد_شمار;
    اگر (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->برداشت_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
ایستا درونخطی تهی recurse_lookups (context_t *c,
                                    unsigned صحیح lookup_شمار_2,
                                    ثابت LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  برای (unsigned صحیح i = 0; i < lookup_شمار_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

ایستا درونخطی تهی apply_lookup (hb_ot_apply_context_t *c,
				 unsigned صحیح شمار, /* Including the first glyph */
				 unsigned صحیح lookup_شمار_2,
				 ثابت LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned صحیح match_end)
{
  hb_buffer_t *buffer = c->buffer;
  صحیح end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned صحیح bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    صحیح delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    برای (unsigned صحیح j = 0; j < شمار; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  برای (unsigned صحیح i = 0; i < lookup_شمار_2 && buffer->successful; i++)
  {
    unsigned صحیح idx = lookupRecord[i].sequenceIndex;
    اگر (idx >= شمار)
      ادامه;

    unsigned صحیح orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    اگر (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      ادامه;

    اگر (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      بشکن;

    اگر (unlikely (buffer->max_ops <= 0))
      بشکن;

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      اگر (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    منطقی ret = c->recurse (lookupRecord[i].lookupListIndex);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      اگر (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookupListIndex);
    }

    اگر (!ret)
      ادامه;

    unsigned صحیح new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    صحیح delta = new_len - orig_len;

    اگر (!delta)
      ادامه;

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
    اگر (end < صحیح (c->match_positions.arrayZ[idx]))
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

    unsigned صحیح next = idx + 1; /* next now is the position after the recursed lookup. */

    اگر (delta > 0)
    {
      اگر (unlikely (delta + شمار > HB_MAX_CONTEXT_LENGTH))
	بشکن;
      اگر (unlikely (شمار + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (شمار + delta)))
        بازگشت;
    }
    وگرنه
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (صحیح) next - (صحیح) شمار);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (شمار - next) * اندازه (c->match_positions.arrayZ[0]));
    next += delta;
    شمار += delta;

    /* Fill in new entries. */
    برای (unsigned صحیح j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    برای (; next < شمار; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  أكد (end >= 0);
  (تهی) buffer->move_to (end);
}



/* Contextual lookups */

ساختار ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ثابت تهی *intersects_data;
  تهی *intersects_cache;
  تهی *intersected_glyphs_cache;
};

تعریفنوع ContextClosureLookupContext ContextDependLookupContext;

ساختار ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت تهی *collect_data;
};

ساختار ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ثابت تهی *match_data;
  get_مقدار_func_t get_مقدار;
};

ایستا درونخطی منطقی context_intersects (ثابت hb_set_t *glyphs,
				       unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
				       glyph_data_t درون_داد, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  بازگشت array_is_subset_of (glyphs,
			     درون_داد_شمار ? درون_داد_شمار - 1 : 0, درون_داد,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

ایستا درونخطی تهی context_closure_lookup (hb_closure_context_t *c,
					   unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
					   glyph_data_t درون_داد, /* Array of input values--start with second glyph */
					   unsigned صحیح lookup_شمار_2,
					   ثابت LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  اگر (context_intersects (c->glyphs,
			  درون_داد_شمار, درون_داد,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     درون_داد_شمار, درون_داد,
				     lookup_شمار_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

ایستا درونخطی تهی context_depend_lookup (hb_depend_context_t *c,
					  unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
					  glyph_data_t درون_داد, /* Array of input values--start with second glyph */
					  unsigned صحیح lookup_شمار_2,
					  ثابت LookupRecord lookupRecord[],
					  ثابت تهی *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  اگر (!context_intersects (c->glyphs,
			   درون_داد_شمار, درون_داد,
			   lookup_context))
    بازگشت;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  اگر (unlikely (!scratch))
    بازگشت;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقی context_sets_cached = context_depend_sets_are_cached (
    c, درون_داد_شمار, lookup_شمار_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  اگر (unlikely (!c->depend_data->successful))
    بازگشت;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ثابت hb_set_t *> &درون_داد_position_glyphs = scratch->درون_داد_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  اگر (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ثابت hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      درست, &position_scratch, &scratch->compact_workspace);
    اگر (unlikely (!pos0_glyphs ||
		  !درون_داد_position_glyphs.افزودن_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      بازگشت;
    }

    /* Positions 1+ (Input array) */
    برای (unsigned i = 0; i + 1 < درون_داد_شمار; i++)
    {
      ثابت hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	درون_داد[i], نادرست, &position_scratch);
      اگر (unlikely (!pos_glyphs ||
		    !درون_داد_position_glyphs.افزودن_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	بازگشت;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  درون_داد_شمار, درون_داد,
				  lookup_شمار_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &درون_داد_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

ایستا درونخطی تهی context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
						  glyph_data_t درون_داد, /* Array of input values--start with second glyph */
						  unsigned صحیح lookup_شمار_2,
						  ثابت LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->درون_داد,
		 درون_داد_شمار ? درون_داد_شمار - 1 : 0, درون_داد,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_شمار_2, lookupRecord);
}

ایستا درونخطی منطقی context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
					       glyph_data_t درون_داد, /* Array of input values--start with second glyph */
					       unsigned صحیح lookup_شمار_2 HB_UNUSED,
					       ثابت LookupRecord lookupRecord[] HB_UNUSED,
					       ثابت ContextApplyLookupContext &lookup_context)
{
  بازگشت would_match_درون_داد (c,
			    درون_داد_شمار, درون_داد,
			    lookup_context.funcs.match, lookup_context.match_data);
}

ایستا درونخطی منطقی context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
					 glyph_data_t درون_داد, /* Array of input values--start with second glyph */
					 unsigned صحیح lookup_شمار_2,
					 ثابت LookupRecord lookupRecord[],
					 ثابت ContextApplyLookupContext &lookup_context)
{
  اگر (unlikely (درون_داد_شمار > HB_MAX_CONTEXT_LENGTH)) بازگشت نادرست;

  unsigned match_end = 0;
  منطقی ret = نادرست;
  اگر (match_درون_داد (c,
		   درون_داد_شمار, درون_داد,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  درون_داد_شمار,
		  lookup_شمار_2, lookupRecord,
		  match_end);
    ret = درست;
  }
  وگرنه
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = نادرست;
  }

  بازگشت ret;
}

ایستا درونخطی منطقی context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  انتخاب (op)
  {
    حالت hb_ot_subtable_cache_op_t::ENTER:
    {
      اگر (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	بازگشت نادرست;
      auto &info = c->buffer->info;
      unsigned شمار = c->buffer->len;
      برای (unsigned i = 0; i < شمار; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      بازگشت درست;
    }
    حالت hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      بشکن;
    }
  }
  بازگشت نادرست;
}

template <typename Types>
ساختار Rule
{
  template <typename SetTypes, typename RuleType>
  friend ساختار RuleSetOf;

  منطقی intersects (ثابت hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) ثابت
  {
    بازگشت context_intersects (glyphs,
			       درون_داد_شمار, درون_داد_Z.arrayZ,
			       lookup_context);
  }

  تهی depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ثابت
  {
    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (درون_داد_Z.as_array ((درون_داد_شمار ? درون_داد_شمار - 1 : 0)));
    context_depend_lookup (c,
			   درون_داد_شمار, درون_داد_Z.arrayZ,
			   lookup_شمار_2, lookupRecord.arrayZ,
			   این,
			   value, lookup_context);
  }

  تهی closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_limit_exceeded ())) بازگشت;

    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (درون_داد_Z.as_array ((درون_داد_شمار ? درون_داد_شمار - 1 : 0)));
    context_closure_lookup (c,
			    درون_داد_شمار, درون_داد_Z.arrayZ,
			    lookup_شمار_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_limit_exceeded ())) بازگشت;
    اگر (!intersects (c->glyphs, lookup_context)) بازگشت;

    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (درون_داد_Z.as_array (درون_داد_شمار ? درون_داد_شمار - 1 : 0));
    recurse_lookups (c, lookup_شمار_2, lookupRecord.arrayZ);
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (درون_داد_Z.as_array (درون_داد_شمار ? درون_داد_شمار - 1 : 0));
    context_collect_glyphs_lookup (c,
				   درون_داد_شمار, درون_داد_Z.arrayZ,
				   lookup_شمار_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest,
			      ثابت ClassDef *class_def = nullptr) ثابت
  {
    اگر (درون_داد_شمار <= 1)
    {
      *digest = hb_set_digest_t::full ();
      بازگشت;
    }

    unsigned second = درون_داد_Z.arrayZ[0];
    اگر (!class_def)
      digest->add (second);
    وگرنه اگر (!second)
      *digest = hb_set_digest_t::full ();
    وگرنه
      class_def->collect_class (digest, second);
  }

  منطقی would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (درون_داد_Z.as_array (درون_داد_شمار ? درون_داد_شمار - 1 : 0));
    بازگشت context_would_apply_lookup (c,
				       درون_داد_شمار, درون_داد_Z.arrayZ,
				       lookup_شمار_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  منطقی apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (این);
    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (درون_داد_Z.as_array (درون_داد_شمار ? درون_داد_شمار - 1 : 0));
    return_trace (context_apply_lookup (c, درون_داد_شمار, درون_داد_Z.arrayZ, lookup_شمار_2, lookupRecord.arrayZ, lookup_context));
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *درون_داد_mapping, /* old->new glyphid or class mapping */
		  ثابت hb_map_t *lookup_map) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->start_embed (این);
    اگر (unlikely (!c->extend_min (out))) return_trace (نادرست);

    out->درون_داد_شمار = درون_داد_شمار;
    ثابت auto درون_داد = درون_داد_Z.as_array (درون_داد_شمار - 1);
    برای (ثابت auto org : درون_داد)
    {
      typename Types::HBUINT d;
      d = درون_داد_mapping->get (org);
      c->copy (d);
    }

    ثابت auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (درون_داد_Z.as_array ((درون_داد_شمار ? درون_داد_شمار - 1 : 0)));

    unsigned شمار = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_شمار_2), lookup_map);
    return_trace (c->check_assign (out->lookup_شمار_2, شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = nullptr) ثابت
  {
    TRACE_SUBSET (این);
    اگر (unlikely (!درون_داد_شمار)) return_trace (نادرست);
    ثابت auto درون_داد = درون_داد_Z.as_array (درون_داد_شمار - 1);

    ثابت hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    اگر (!hb_all (درون_داد, mapping)) return_trace (نادرست);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  عمومی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  c->check_range (درون_داد_Z.arrayZ,
				  درون_داد_Z.item_size * (درون_داد_شمار ? درون_داد_شمار - 1 : 0) +
				  LookupRecord::static_size * lookup_شمار_2));
  }

  محافظتشده:
  HBUINT16	درون_داد_شمار;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_شمار_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		درون_داد_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عمومی:
  DEFINE_SIZE_ARRAY (4, درون_داد_Z);
};

template <typename Types, typename RuleType>
ساختار RuleSetOf
{
  منطقی intersects (ثابت hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) ثابت
  {
    بازگشت
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_map ([&] (ثابت RuleType &_) { بازگشت _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  تهی collect_first_درون_داد_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    برای (unsigned i = 0; i < rule.len; i++)
    {
      ثابت RuleType &r = این+rule.arrayZ[i];
      اگر (r.درون_داد_شمار <= 1)
      {
	digest->set_full ();
	بازگشت;
      }
      digest->add (r.درون_داد_Z.arrayZ[0]);
    }
  }

  تهی depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  تهی closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_limit_exceeded ())) بازگشت;

    بازگشت
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_limit_exceeded ())) بازگشت;
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    بازگشت
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest,
			      ثابت ClassDef *class_def = nullptr) ثابت
  {
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([=] (ثابت RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c,
		    ثابت ContextApplyLookupContext &lookup_context) ثابت
  {
    بازگشت
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_map ([&] (ثابت RuleType &_) { بازگشت _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c,
	      ثابت ContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = nullptr) ثابت
  {
    TRACE_APPLY (این);

    unsigned رقم_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    اگر (HB_OPTIMIZE_SIZE_VAL || رقم_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (این))
      | hb_map ([&] (ثابت RuleType &_) { بازگشت _.apply (c, lookup_context); })
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
    منطقی matched = skippy_iter.next (&unsafe_to1);
    اگر (likely (matched))
    {
      اگر (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        برو slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      اگر (digest && lookup_context.get_مقدار &&
	  !digest->may_have (lookup_context.get_مقدار (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (نادرست);
	}
    }
    وگرنه
    {
      منطقی unsafe_to_concat = نادرست;
      منطقی ret =
      + hb_iter (rule)
      | hb_map (hb_add (این))
      | hb_filter ([&] (ثابت RuleType &_)
		   {
		     اگر (_.درون_داد_شمار <= 1) بازگشت درست;
		     unsafe_to_concat = درست;
		     بازگشت نادرست;
		   })
      | hb_map ([&] (ثابت RuleType &_) { بازگشت _.apply (c, lookup_context); })
      | hb_any
      ;
      اگر (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    اگر (likely (matched))
    {
      اگر (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        برو slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_درون_داد = lookup_context.funcs.match;
    auto *درون_داد_data = lookup_context.match_data;
    برای (unsigned صحیح i = 0; i < رقم_rules; i++)
    {
      ثابت auto &r = این+rule.arrayZ[i];

      ثابت auto &درون_داد = r.درون_داد_Z;

      اگر (r.درون_داد_شمار <= 1 ||
	  (!match_درون_داد ||
	   match_درون_داد (*first, درون_داد.arrayZ[0], درون_داد_data)))
      {
        اگر (!second ||
	    (r.درون_داد_شمار <= 2 ||
	     (!match_درون_داد ||
	      match_درون_داد (*second, درون_داد.arrayZ[1], درون_داد_data)))
	   )
	{
	  اگر (r.apply (c, lookup_context))
	  {
	    اگر (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (درست);
	  }
	}
	وگرنه
	  unsafe_to = unsafe_to2;
      }
      وگرنه
      {
	اگر (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	برای (; i + 1 < رقم_rules; i++)
	{
	  ثابت auto &r2 = این+rule.arrayZ[i + 1];
	  ثابت auto &input2 = r2.درون_داد_Z;
	  اگر (r2.درون_داد_شمار <= 1 || input2.arrayZ[0] != درون_داد.arrayZ[0])
	    بشکن;
	}
      }
    }
    اگر (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (نادرست);
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *klass_map = nullptr) ثابت
  {
    TRACE_SUBSET (این);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    برای (ثابت auto& _ : rule)
    {
      اگر (!_) ادامه;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      اگر (unlikely (!o)) ادامه;

      اگر (!o->serialize_subset (c, _, این, lookup_map, klass_map))
      {
	out->rule.برداشت ();
	c->serializer->revert (o_snap);
      }
    }

    منطقی ret = منطقی (out->rule);
    اگر (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (rule.sanitize (c, این));
  }

  محافظتشده:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  عمومی:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
ساختار RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
ساختار ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
ساختار ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    ساختار ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    بازگشت
    + hb_zip (این+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_map ([&] (ثابت RuleSet &_) { بازگشت _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت درست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (این+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      بازگشت c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, unsigned> _) { بازگشت hb_pair_t<unsigned, ثابت RuleSet&> (_.first, این+ruleSet[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    ساختار ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (این+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      بازگشت c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, unsigned> _) { بازگشت hb_pair_t<unsigned, ثابت RuleSet&> (_.first, این+ruleSet[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    ساختار ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (این+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (این+coverage).collect_coverage (c->درون_داد);

    ساختار ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_apply ([digest] (ثابت RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت RuleSet &rule_set = این+ruleSet[(این+coverage).get_coverage (c->glyphs[0])];
    ساختار ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    بازگشت rule_set.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    unsigned صحیح index = (این+coverage).get_coverage (c->buffer->cur().codepoint);
    اگر (likely (index == NOT_COVERED))
      return_trace (نادرست);

    ثابت RuleSet &rule_set = این+ruleSet[index];
    ساختار ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (این+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, این, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) && ruleSet.sanitize (c, این));
  }

  محافظتشده:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
ساختار ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    اگر (!(این+coverage).intersects (glyphs))
      بازگشت نادرست;

    ثابت ClassDef &class_def = این+classDef;

    hb_map_t cache;
    ساختار ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (این+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    بازگشت
    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<unsigned, ثابت RuleSet &> p)
	      { بازگشت class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت درست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    اگر (!(این+coverage).intersects (c->glyphs))
      بازگشت;

    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ثابت ClassDef &class_def = این+classDef;

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
    { بازگشت class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ثابت RuleSet& rule_set = این+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    اگر (!(این+coverage).intersects (c->glyphs))
      بازگشت;

    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ثابت ClassDef &class_def = این+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    ساختار ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { بازگشت class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ثابت RuleSet& rule_set = این+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    اگر (!(این+coverage).intersects (c->glyphs))
      بازگشت;

    ثابت ClassDef &class_def = این+classDef;

    hb_map_t cache;
    ساختار ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_enumerate
    | hb_filter ([&] (ثابت hb_pair_t<unsigned, ثابت RuleSet &> p)
    { بازگشت class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (ثابت RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (این+coverage).collect_coverage (c->درون_داد);

    ثابت ClassDef &class_def = این+classDef;
    ساختار ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    ثابت ClassDef &class_def = این+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_apply ([=, &class_def] (ثابت RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &class_def = این+classDef;
    unsigned صحیح index = class_def.get_class (c->glyphs[0]);
    ثابت RuleSet &rule_set = این+ruleSet[index];
    ساختار ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    بازگشت rule_set.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  unsigned cache_cost () ثابت
  {
    بازگشت (این+classDef).cost ();
  }
  ایستا منطقی cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    بازگشت context_cache_func (c, op);
  }

  ساختار external_cache_t
  {
    hb_ot_layout_دودویی_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  تهی *external_cache_create () ثابت
  {
    unsigned شمار = ruleSet.len;
    unsigned size = اندازه (external_cache_t) -
		    HB_VAR_ARRAY * اندازه (hb_ot_layout_ruleset_digest_t) +
		    شمار * اندازه (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    اگر (likely (cache))
    {
      cache->coverage.clear ();
      برای (unsigned i = 0; i < شمار; i++)
	(این+ruleSet[i]).collect_first_درون_داد_classes (&cache->rule_sets[i]);
    }
    بازگشت cache;
  }
  منطقی apply_cached (hb_ot_apply_context_t *c, تهی *external_cache) ثابت { بازگشت _apply (c, درست, external_cache); }
  منطقی apply (hb_ot_apply_context_t *c, تهی *external_cache) ثابت { بازگشت _apply (c, نادرست, external_cache); }
  منطقی _apply (hb_ot_apply_context_t *c, منطقی cached, تهی *external_cache) ثابت
  {
    TRACE_APPLY (این);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned صحیح index = (این+coverage).get_coverage_دودویی (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#وگرنه
    unsigned صحیح index = (این+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    اگر (index == NOT_COVERED) return_trace (نادرست);

    ثابت ClassDef &class_def = این+classDef;

    ساختار ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_مقدار : get_class_مقدار
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    ثابت RuleSet &rule_set = این+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#وگرنه
    ثابت hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;
    اگر (unlikely (!out->coverage.serialize_subset (c, coverage, این)))
      return_trace (نادرست);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, این, &klass_map);

    ثابت hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (این+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (این+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    منطقی ret = درست;
    صحیح non_zero_index = -1, index = 0;
    auto snapshot = c->serializer->snapshot();
    برای (ثابت auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      اگر (unlikely (!o))
      {
	ret = نادرست;
	بشکن;
      }

      اگر (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, این, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    اگر (!ret || non_zero_index == -1) return_trace (نادرست);

    //prune empty trailing ruleSets
    --index;
    وقتی (index > non_zero_index)
    {
      out->ruleSet.برداشت ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (منطقی (out->ruleSet));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) && classDef.sanitize (c, این) && ruleSet.sanitize (c, این));
  }

  محافظتشده:
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
  عمومی:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
ساختار ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    اگر (!(این+coverageZ[0]).intersects (glyphs))
      بازگشت نادرست;

    ساختار ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      این
    };
    بازگشت context_intersects (glyphs,
			       glyph_شمار_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت درست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    اگر (!(این+coverageZ[0]).intersects (c->glyphs))
      بازگشت;

    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_شمار_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      این,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_شمار_2, (ثابت HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_شمار_2, lookupRecord,
			   این,
			   coverageZ[0], lookup_context);

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    اگر (!(این+coverageZ[0]).intersects (c->glyphs))
      بازگشت;

    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_شمار_2));
    ساختار ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      این
    };
    context_closure_lookup (c,
			    glyph_شمار_2, coverageZ.arrayZ + 1,
			    lookup_شمار_2, lookupRecord,
			    0, lookup_context);

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    اگر (!intersects (c->glyphs))
      بازگشت;
    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_شمار_2));
    recurse_lookups (c, lookup_شمار_2, lookupRecord);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (این+coverageZ[0]).collect_coverage (c->درون_داد);

    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_شمار_2));
    ساختار ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      این
    };

    context_collect_glyphs_lookup (c,
				   glyph_شمار_2, coverageZ.arrayZ + 1,
				   lookup_شمار_2, lookupRecord,
				   lookup_context);
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    اگر (glyph_شمار_2 <= 1)
      *digest = hb_set_digest_t::full ();
    وگرنه
      (این+coverageZ[1]).collect_coverage (digest);
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_شمار_2));
    ساختار ContextApplyLookupContext lookup_context = {
      {match_coverage},
      این
    };
    بازگشت context_would_apply_lookup (c,
				       glyph_شمار_2, coverageZ.arrayZ + 1,
				       lookup_شمار_2, lookupRecord,
				       lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverageZ[0]; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    unsigned صحیح index = (این+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_شمار_2));
    ساختار ContextApplyLookupContext lookup_context = {
      {match_coverage},
      این
    };
    return_trace (context_apply_lookup (c,
					glyph_شمار_2,
					coverageZ.arrayZ + 1,
					lookup_شمار_2, lookupRecord,
					lookup_context));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    out->format = format;
    out->glyph_شمار_2 = glyph_شمار_2;

    auto coverages = coverageZ.as_array (glyph_شمار_2);

    برای (ثابت auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_size<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_size);
      اگر (unlikely (!o)) return_trace (نادرست);
      اگر (!o->serialize_subset (c, offset, این)) return_trace (نادرست);
    }

    ثابت auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_شمار_2));
    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned شمار = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_شمار_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_شمار_2, شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!c->check_struct (این))) return_trace (نادرست);
    hb_barrier ();
    unsigned صحیح شمار = glyph_شمار_2;
    اگر (unlikely (!شمار)) return_trace (نادرست); /* We want to access coverageZ[0] freely. */
    اگر (unlikely (!c->check_array (coverageZ.arrayZ, شمار))) return_trace (نادرست);
    برای (unsigned صحیح i = 0; i < شمار; i++)
      اگر (unlikely (!coverageZ[i].sanitize (c, این))) return_trace (نادرست);
    ثابت LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_شمار_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_شمار_2)));
  }

  محافظتشده:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_شمار_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_شمار_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::template OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  عمومی:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

ساختار Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالت 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    حالت 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;		/* Format identifier */
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

ساختار ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ثابت تهی *intersects_data[3];
  تهی *intersects_cache[3];
  تهی *intersected_glyphs_cache;
};

تعریفنوع ChainContextClosureLookupContext ChainContextDependLookupContext;

ساختار ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ثابت تهی *collect_data[3];
};

ساختار ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ثابت تهی *match_data[3];
  get_مقدار_func_t get_درون_داد_مقدار;
};

ایستا درونخطی منطقی chain_context_intersects (ثابت hb_set_t *glyphs,
					     unsigned صحیح backtrack_شمار,
					     glyph_data_t backtrack,
					     unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
					     glyph_data_t درون_داد, /* Array of input values--start with second glyph */
					     unsigned صحیح lookahead_شمار,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  منطقی bt_ok = array_is_subset_of (glyphs,
			     backtrack_شمار, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  منطقی in_ok = array_is_subset_of (glyphs,
			     درون_داد_شمار ? درون_داد_شمار - 1 : 0, درون_داد,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  منطقی la_ok = array_is_subset_of (glyphs,
			     lookahead_شمار, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  بازگشت bt_ok && in_ok && la_ok;
}

ایستا درونخطی تهی chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned صحیح backtrack_شمار,
						 glyph_data_t backtrack,
						 unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
						 glyph_data_t درون_داد, /* Array of input values--start with second glyph */
						 unsigned صحیح lookahead_شمار,
						 glyph_data_t lookahead,
						 unsigned صحیح lookup_شمار_2,
						 ثابت LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  اگر (chain_context_intersects (c->glyphs,
				backtrack_شمار, backtrack,
				درون_داد_شمار, درون_داد,
				lookahead_شمار, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     درون_داد_شمار, درون_داد,
		     lookup_شمار_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

ایستا درونخطی تهی chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned صحیح backtrack_شمار,
						glyph_data_t backtrack,
						unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
						glyph_data_t درون_داد, /* Array of input values--start with second glyph */
						unsigned صحیح lookahead_شمار,
						glyph_data_t lookahead,
						unsigned صحیح lookup_شمار_2,
						ثابت LookupRecord lookupRecord[],
						ثابت تهی *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  اگر (!chain_context_intersects (c->glyphs,
				 backtrack_شمار, backtrack,
				 درون_داد_شمار, درون_داد,
				 lookahead_شمار, lookahead,
				 lookup_context))
    بازگشت;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  اگر (unlikely (!scratch))
    بازگشت;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  منطقی context_sets_cached = context_depend_sets_are_cached (
    c, درون_داد_شمار, lookup_شمار_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  اگر (unlikely (!c->depend_data->successful))
    بازگشت;
  اگر (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    درون_داد_شمار, درون_داد,
				    lookup_شمار_2, lookupRecord,
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
    بازگشت;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ثابت hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<ثابت hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  برای (unsigned i = 0; i < backtrack_شمار; i++)
  {
    ثابت hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], نادرست, &position_scratch);
    اگر (unlikely (!pos_glyphs))
      بازگشت;
    اگر (!pos_glyphs->is_empty ())
    {
      اگر (unlikely (!backtrack_sets.افزودن_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        بازگشت;
      }
    }
  }

  /* Extract lookahead glyphs */
  برای (unsigned i = 0; i < lookahead_شمار; i++)
  {
    ثابت hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], نادرست, &position_scratch);
    اگر (unlikely (!pos_glyphs))
      بازگشت;
    اگر (!pos_glyphs->is_empty ())
    {
      اگر (unlikely (!lookahead_sets.افزودن_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        بازگشت;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  برای (ثابت hb_set_t *back_set : backtrack_sets)
  {
    اگر (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } وگرنه اگر (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      اگر (unlikely (set_idx == HB_CODEPOINT_INVALID))
        بازگشت;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  برای (ثابت hb_set_t *look_set : lookahead_sets)
  {
    اگر (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } وگرنه اگر (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      اگر (unlikely (set_idx == HB_CODEPOINT_INVALID))
        بازگشت;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  اگر (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    بازگشت;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<ثابت hb_set_t *> &درون_داد_position_glyphs = scratch->درون_داد_position_glyphs;

  اگر (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ثابت hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      درست, &position_scratch, &scratch->compact_workspace);
    اگر (unlikely (!pos0_glyphs ||
		  !درون_داد_position_glyphs.افزودن_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      بازگشت;
    }

    /* Positions 1+ (Input array) */
    برای (unsigned i = 0; i + 1 < درون_داد_شمار; i++)
    {
      ثابت hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	درون_داد[i], نادرست, &position_scratch);
      اگر (unlikely (!pos_glyphs ||
		    !درون_داد_position_glyphs.افزودن_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	بازگشت;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  درون_داد_شمار, درون_داد,
				  lookup_شمار_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &درون_داد_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

ایستا درونخطی تهی chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned صحیح backtrack_شمار,
							glyph_data_t backtrack,
							unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
							glyph_data_t درون_داد, /* Array of input values--start with second glyph */
							unsigned صحیح lookahead_شمار,
							glyph_data_t lookahead,
							unsigned صحیح lookup_شمار_2,
							ثابت LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_شمار, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->درون_داد,
		 درون_داد_شمار ? درون_داد_شمار - 1 : 0, درون_داد,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_شمار, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_شمار_2, lookupRecord);
}

ایستا درونخطی منطقی chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned صحیح backtrack_شمار,
						     glyph_data_t backtrack HB_UNUSED,
						     unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
						     glyph_data_t درون_داد, /* Array of input values--start with second glyph */
						     unsigned صحیح lookahead_شمار,
						     glyph_data_t lookahead HB_UNUSED,
						     unsigned صحیح lookup_شمار_2 HB_UNUSED,
						     ثابت LookupRecord lookupRecord[] HB_UNUSED,
						     ثابت ChainContextApplyLookupContext &lookup_context)
{
  بازگشت (c->zero_context ? !backtrack_شمار && !lookahead_شمار : درست)
      && would_match_درون_داد (c,
			    درون_داد_شمار, درون_داد,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

ایستا درونخطی منطقی chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned صحیح backtrack_شمار,
					       glyph_data_t backtrack,
					       unsigned صحیح درون_داد_شمار, /* Including the first glyph (not matched) */
					       glyph_data_t درون_داد, /* Array of input values--start with second glyph */
					       unsigned صحیح lookahead_شمار,
					       glyph_data_t lookahead,
					       unsigned صحیح lookup_شمار_2,
					       ثابت LookupRecord lookupRecord[],
					       ثابت ChainContextApplyLookupContext &lookup_context)
{
  اگر (unlikely (درون_داد_شمار > HB_MAX_CONTEXT_LENGTH)) بازگشت نادرست;

  unsigned start_index = c->buffer->out_len;
  unsigned end_index = c->buffer->idx;
  unsigned match_end = 0;
  منطقی ret = درست;
  اگر (!(match_درون_داد (c,
		     درون_داد_شمار, درون_داد,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_شمار, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    بازگشت نادرست;
  }

  اگر (!match_backtrack (c,
			backtrack_شمار, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    بازگشت نادرست;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		درون_داد_شمار,
		lookup_شمار_2, lookupRecord,
		match_end);

  بازگشت ret;
}

template <typename Types>
ساختار ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend ساختار ChainRuleSetOf;

  منطقی intersects (ثابت hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    بازگشت chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     درون_داد.lenP1, درون_داد.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  تهی depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 درون_داد.lenP1, درون_داد.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 این,
				 value,
				 lookup_context);
  }

  تهی closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_limit_exceeded ())) بازگشت;

    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  درون_داد.lenP1, درون_داد.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_limit_exceeded ())) بازگشت;
    اگر (!intersects (c->glyphs, lookup_context)) بازگشت;

    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 درون_داد.lenP1, درون_داد.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest,
			      ثابت ClassDef *درون_داد_class_def = nullptr,
			      ثابت ClassDef *lookahead_class_def = nullptr) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);

    unsigned second;
    ثابت ClassDef *class_def;
    اگر (درون_داد.lenP1 > 1)
    {
      second = درون_داد.arrayZ[0];
      class_def = درون_داد_class_def;
    }
    وگرنه اگر (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    وگرنه
    {
      *digest = hb_set_digest_t::full ();
      بازگشت;
    }

    اگر (!class_def)
      digest->add (second);
    وگرنه اگر (!second)
      *digest = hb_set_digest_t::full ();
    وگرنه
      class_def->collect_class (digest, second);
  }

  منطقی would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    بازگشت chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     درون_داد.lenP1, درون_داد.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  منطقی apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    TRACE_APPLY (این);
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      درون_داد.lenP1, درون_داد.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  تهی serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ثابت
  {
    c->copy (len);
    برای (ثابت auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت hb_map_t *lookup_map,
		  ثابت hb_map_t *backtrack_map,
		  ثابت hb_map_t *درون_داد_map = nullptr,
		  ثابت hb_map_t *lookahead_map = nullptr) ثابت
  {
    TRACE_SERIALIZE (این);

    ثابت hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    اگر (درون_داد_map) mapping = درون_داد_map;
    serialize_array (c, درون_داد.lenP1, + درون_داد.iter ()
				     | hb_map (mapping));

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    اگر (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_شمار_2 = c->embed (&(lookup.len));
    اگر (!lookup_شمار_2) return_trace (نادرست);

    unsigned شمار = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_شمار_2, شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_map = nullptr,
	       ثابت hb_map_t *درون_داد_map = nullptr,
	       ثابت hb_map_t *lookahead_map = nullptr) ثابت
  {
    TRACE_SUBSET (این);

    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);

    اگر (!backtrack_map)
    {
      ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      اگر (!hb_all (backtrack, glyphset) ||
	  !hb_all (درون_داد, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (نادرست);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    وگرنه
    {
      اگر (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (درون_داد, درون_داد_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (نادرست);

      serialize (c->serializer, lookup_map, backtrack_map, درون_داد_map, lookahead_map);
    }

    return_trace (درست);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    /* Hyper-optimized sanitized because this is really hot. */
    اگر (unlikely (!backtrack.len.sanitize (c))) return_trace (نادرست);
    hb_barrier ();
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    اگر (unlikely (!درون_داد.lenP1.sanitize (c))) return_trace (نادرست);
    hb_barrier ();
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    اگر (unlikely (!lookahead.len.sanitize (c))) return_trace (نادرست);
    hb_barrier ();
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محافظتشده:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		درون_داد_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عمومی:
  DEFINE_SIZE_MIN (8);
};

template <typename Types, typename ChainRuleType>
ساختار ChainRuleSetOf
{
  منطقی intersects (ثابت hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ثابت
  {
    بازگشت
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_map ([&] (ثابت ChainRuleType &_) { بازگشت _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  تهی collect_first_درون_داد_classes (hb_ot_layout_ruleset_digest_t *digest) ثابت
  {
    digest->init ();
    برای (unsigned i = 0; i < rule.len; i++)
    {
      ثابت ChainRuleType &r = این+rule.arrayZ[i];
      ثابت auto &درون_داد = StructAfter<decltype (r.درون_داد_X)> (r.backtrack);
      اگر (درون_داد.lenP1 <= 1)
      {
	digest->set_full ();
	بازگشت;
      }
      digest->add (درون_داد.arrayZ[0]);
    }
  }
  تهی depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ثابت
  {
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  تهی closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_limit_exceeded ())) بازگشت;

    بازگشت
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ثابت
  {
    اگر (unlikely (c->lookup_limit_exceeded ())) بازگشت;

    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ثابت
  {
    بازگشت
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest,
			      ثابت ClassDef *درون_داد_class_def = nullptr,
			      ثابت ClassDef *lookahead_class_def = nullptr) ثابت
  {
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_apply ([=] (ثابت ChainRuleType &_)
		{ _.collect_second_glyphs (digest, درون_داد_class_def, lookahead_class_def); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c,
		    ثابت ChainContextApplyLookupContext &lookup_context) ثابت
  {
    بازگشت
    + hb_iter (rule)
    | hb_map (hb_add (این))
    | hb_map ([&] (ثابت ChainRuleType &_) { بازگشت _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c,
	      ثابت ChainContextApplyLookupContext &lookup_context,
	      ثابت hb_ot_layout_ruleset_digest_t *digest = nullptr) ثابت
  {
    TRACE_APPLY (این);

    unsigned رقم_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    اگر (HB_OPTIMIZE_SIZE_VAL || رقم_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (این))
      | hb_map ([&] (ثابت ChainRuleType &_) { بازگشت _.apply (c, lookup_context); })
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
    منطقی matched = skippy_iter.next (&unsafe_to1);
    اگر (likely (matched))
    {
      اگر (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        برو slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      اگر (digest && lookup_context.get_درون_داد_مقدار &&
	  !digest->may_have (lookup_context.get_درون_داد_مقدار (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (نادرست);
	}
    }
    وگرنه
    {
      منطقی unsafe_to_concat = نادرست;
      منطقی ret =
      + hb_iter (rule)
      | hb_map (hb_add (این))
      | hb_filter ([&] (ثابت ChainRuleType &_)
		   {
		     ثابت auto &درون_داد = StructAfter<decltype (_.درون_داد_X)> (_.backtrack);
		     ثابت auto &lookahead = StructAfter<decltype (_.lookaheadX)> (درون_داد);
		     اگر (درون_داد.lenP1 <= 1 && lookahead.len == 0) بازگشت درست;
		     unsafe_to_concat = درست;
		     بازگشت نادرست;
		   })
      | hb_map ([&] (ثابت ChainRuleType &_) { بازگشت _.apply (c, lookup_context); })
      | hb_any
      ;
      اگر (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    اگر (likely (matched))
    {
      اگر (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        برو slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_درون_داد = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *درون_داد_data = lookup_context.match_data[1];
    auto *lookahead_data = lookup_context.match_data[2];
    برای (unsigned صحیح i = 0; i < رقم_rules; i++)
    {
      ثابت auto &r = این+rule.arrayZ[i];

      ثابت auto &درون_داد = StructAfter<decltype (r.درون_داد_X)> (r.backtrack);
      ثابت auto &lookahead = StructAfter<decltype (r.lookaheadX)> (درون_داد);

      unsigned lenP1 = درون_داد.lenP1;
      اگر (lenP1 > 1 ?
	   (!match_درون_داد ||
	    match_درون_داد (*first, درون_داد.arrayZ[0], درون_داد_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        اگر (!second ||
	    (lenP1 > 2 ?
	     (!match_درون_داد ||
	      match_درون_داد (*second, درون_داد.arrayZ[1], درون_داد_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  اگر (r.apply (c, lookup_context))
	  {
	    اگر (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (درست);
	  }
	}
	وگرنه
	  unsafe_to = unsafe_to2;
      }
      وگرنه
      {
	اگر (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	اگر (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  برای (; i + 1 < رقم_rules; i++)
	  {
	    ثابت auto &r2 = این+rule.arrayZ[i + 1];
	    ثابت auto &input2 = StructAfter<decltype (r2.درون_داد_X)> (r2.backtrack);
	    اگر (input2.lenP1 <= 1 || input2.arrayZ[0] != درون_داد.arrayZ[0])
	      بشکن;
	  }
	}
      }
    }
    اگر (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (نادرست);
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت hb_map_t *lookup_map,
	       ثابت hb_map_t *backtrack_klass_map = nullptr,
	       ثابت hb_map_t *درون_داد_klass_map = nullptr,
	       ثابت hb_map_t *lookahead_klass_map = nullptr) ثابت
  {
    TRACE_SUBSET (این);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    برای (ثابت auto& _ : rule)
    {
      اگر (!_) ادامه;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      اگر (unlikely (!o)) ادامه;

      اگر (!o->serialize_subset (c, _, این,
				lookup_map,
				backtrack_klass_map,
				درون_داد_klass_map,
				lookahead_klass_map))
      {
	out->rule.برداشت ();
	c->serializer->revert (o_snap);
      }
    }

    منطقی ret = منطقی (out->rule);
    اگر (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (rule.sanitize (c, این));
  }

  محافظتشده:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  عمومی:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
ساختار ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (اندازه (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

template <typename Types>
ساختار ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
ساختار ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    ساختار ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    بازگشت
    + hb_zip (این+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_map ([&] (ثابت ChainRuleSet &_) { بازگشت _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت درست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (این+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      بازگشت c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, unsigned> _) { بازگشت hb_pair_t<unsigned, ثابت ChainRuleSet&> (_.first, این+ruleSet[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ساختار ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (این+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      بازگشت c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ثابت hb_pair_t<hb_codepoint_t, unsigned> _) { بازگشت hb_pair_t<unsigned, ثابت ChainRuleSet&> (_.first, این+ruleSet[_.second]); })
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    ساختار ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (این+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (این+coverage).collect_coverage (c->درون_داد);

    ساختار ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_apply ([digest] (ثابت ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ChainRuleSet &rule_set = این+ruleSet[(این+coverage).get_coverage (c->glyphs[0])];
    ساختار ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    بازگشت rule_set.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    unsigned صحیح index = (این+coverage).get_coverage (c->buffer->cur().codepoint);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    ثابت ChainRuleSet &rule_set = این+ruleSet[index];
    ساختار ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;

    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (این+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, این, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (منطقی (new_coverage));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) && ruleSet.sanitize (c, این));
  }

  محافظتشده:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

template <typename Types>
ساختار ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    اگر (!(این+coverage).intersects (glyphs))
      بازگشت نادرست;

    ثابت ClassDef &backtrack_class_def = این+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = این+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = این+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ساختار ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (این+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    درون_داد_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    بازگشت
    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_enumerate
    | hb_map ([&] (ثابت hb_pair_t<unsigned, ثابت ChainRuleSet &> p)
	      { بازگشت درون_داد_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت درست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    اگر (!(این+coverage).intersects (c->glyphs))
      بازگشت;

    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ثابت ClassDef &backtrack_class_def = این+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = این+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = این+lookaheadClassDef;

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

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { بازگشت درون_داد_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ثابت ChainRuleSet& chainrule_set = این+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    اگر (!(این+coverage).intersects (c->glyphs))
      بازگشت;

    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs)) بازگشت;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ثابت ClassDef &backtrack_class_def = این+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = این+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = این+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ساختار ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { بازگشت درون_داد_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ثابت hb_pair_t<unsigned, ثابت typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ثابت ChainRuleSet& chainrule_set = این+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    اگر (!(این+coverage).intersects (c->glyphs))
      بازگشت;

    ثابت ClassDef &backtrack_class_def = این+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = این+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = این+lookaheadClassDef;

    hb_map_t caches[3] = {};
    ساختار ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { بازگشت درون_داد_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ثابت ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    (این+coverage).collect_coverage (c->درون_داد);

    ثابت ClassDef &backtrack_class_def = این+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = این+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = این+lookaheadClassDef;

    ساختار ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_apply ([&] (ثابت ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    ثابت ClassDef &درون_داد_class_def = این+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = این+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (این))
    | hb_apply ([digest, &درون_داد_class_def, &lookahead_class_def] (ثابت ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &درون_داد_class_def, &lookahead_class_def); })
    ;
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت ClassDef &backtrack_class_def = این+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = این+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = این+lookaheadClassDef;

    unsigned صحیح index = درون_داد_class_def.get_class (c->glyphs[0]);
    ثابت ChainRuleSet &rule_set = این+ruleSet[index];
    ساختار ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def}
    };
    بازگشت rule_set.would_apply (c, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  unsigned cache_cost () ثابت
  {
    بازگشت (این+درون_داد_Class_Def).cost () + (این+lookaheadClassDef).cost ();
  }
  ایستا منطقی cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    بازگشت context_cache_func (c, op);
  }

  ساختار external_cache_t
  {
    hb_ot_layout_دودویی_cache_t coverage;
    hb_ot_layout_mapping_cache_t درون_داد_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  تهی *external_cache_create () ثابت
  {
    unsigned شمار = ruleSet.len;
    unsigned size = اندازه (external_cache_t) -
		    HB_VAR_ARRAY * اندازه (hb_ot_layout_ruleset_digest_t) +
		    شمار * اندازه (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    اگر (likely (cache))
    {
      cache->coverage.clear ();
      cache->درون_داد_class.clear ();
      cache->lookahead_class.clear ();
      برای (unsigned i = 0; i < شمار; i++)
	(این+ruleSet[i]).collect_first_درون_داد_classes (&cache->rule_sets[i]);
    }
    بازگشت cache;
  }
  منطقی apply_cached (hb_ot_apply_context_t *c, تهی *external_cache) ثابت { بازگشت _apply (c, درست, external_cache); }
  منطقی apply (hb_ot_apply_context_t *c, تهی *external_cache) ثابت { بازگشت _apply (c, نادرست, external_cache); }
  منطقی _apply (hb_ot_apply_context_t *c, منطقی cached, تهی *external_cache) ثابت
  {
    TRACE_APPLY (این);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned صحیح index = (این+coverage).get_coverage_دودویی (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#وگرنه
    unsigned صحیح index = (این+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    اگر (index == NOT_COVERED) return_trace (نادرست);

    ثابت ClassDef &backtrack_class_def = این+backtrackClassDef;
    ثابت ClassDef &درون_داد_class_def = این+درون_داد_Class_Def;
    ثابت ClassDef &lookahead_class_def = این+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t درون_داد_cache_data = {
      &درون_داد_class_def,
      cache ? &cache->درون_داد_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    ثابت تهی *درون_داد_match_data = cached
      ? static_cast<ثابت تهی *> (&درون_داد_class_def)
      : static_cast<ثابت تهی *> (&درون_داد_cache_data);
    ثابت تهی *lookahead_match_data = cached
      ? static_cast<ثابت تهی *> (&lookahead_class_def)
      : static_cast<ثابت تهی *> (&lookahead_cache_data);
    منطقی backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ثابت تهی *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<ثابت تهی *> (&lookahead_cache_data)
      : static_cast<ثابت تهی *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    ساختار ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       درون_داد_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_مقدار : get_class_cache_مقدار
#وگرنه
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &درون_داد_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_مقدار : get_class_مقدار
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (درون_داد_class_def, c->buffer->cur())
         : درون_داد_class_def.get_class (c->buffer->cur().codepoint, درون_داد_cache_data.cache);
#وگرنه
    index = cached
         ? get_class_cached2 (درون_داد_class_def, c->buffer->cur())
          : درون_داد_class_def.get_class (c->buffer->cur().codepoint);
#endif
    ثابت ChainRuleSet &rule_set = این+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ثابت hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#وگرنه
    ثابت hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    auto *out = c->serializer->start_embed (*این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, این);

    hb_map_t backtrack_klass_map;
    hb_map_t درون_داد_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, این, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->درون_داد_Class_Def.serialize_subset (c, درون_داد_Class_Def, این, &درون_داد_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, این, &lookahead_klass_map);

    اگر (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   درون_داد_klass_map,
						   lookahead_klass_map)))
      return_trace (نادرست);

    ثابت hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (این+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (این+درون_داد_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    صحیح non_zero_index = -1, index = 0;
    منطقی ret = درست;
    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    برای (ثابت auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (درون_داد_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      اگر (unlikely (!o))
      {
	ret = نادرست;
	بشکن;
      }
      اگر (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, این,
			       lookup_map,
			       &backtrack_klass_map,
			       &درون_داد_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    اگر (!ret || non_zero_index == -1) return_trace (نادرست);

    // prune empty trailing ruleSets
    اگر (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (منطقی (out->ruleSet));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) &&
		  backtrackClassDef.sanitize (c, این) &&
		  درون_داد_Class_Def.sanitize (c, این) &&
		  lookaheadClassDef.sanitize (c, این) &&
		  ruleSet.sanitize (c, این));
  }

  محافظتشده:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		درون_داد_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  عمومی:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

template <typename Types>
ساختار ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    اگر (!(این+درون_داد[0]).intersects (glyphs))
      بازگشت نادرست;

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ساختار ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {این, این, این}
    };
    بازگشت chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     درون_داد.len, درون_داد.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت درست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    اگر (!(این+درون_داد[0]).intersects (c->glyphs))
      بازگشت;

    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs))
      بازگشت;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {این, این, این},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ثابت HBUINT16 *) backtrack.arrayZ,
				 درون_داد.len, (ثابت HBUINT16 *) درون_داد.arrayZ + 1,
				 lookahead.len, (ثابت HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 این,
				 درون_داد[0], lookup_context);

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    اگر (!(این+درون_داد[0]).intersects (c->glyphs))
      بازگشت;

    hb_set_t* cur_active_glyphs = c->افزودن_cur_active_glyphs ();
    اگر (unlikely (!cur_active_glyphs))
      بازگشت;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ساختار ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {این, این, این}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  درون_داد.len, درون_داد.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->برداشت_cur_done_glyphs ();
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت
  {
    اگر (!intersects (c->glyphs))
      بازگشت;

    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    (این+درون_داد[0]).collect_coverage (c->درون_داد);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    ساختار ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {این, این, این}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 درون_داد.len, درون_داد.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    اگر (درون_داد.len > 1)
      (این+درون_داد[1]).collect_coverage (digest);
    وگرنه اگر (lookahead.len)
      (این+lookahead[0]).collect_coverage (digest);
    وگرنه
      *digest = hb_set_digest_t::full ();
  }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ساختار ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {این, این, این}
    };
    بازگشت chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     درون_داد.len, درون_داد.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ثابت Coverage &get_coverage () ثابت
  {
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    بازگشت این+درون_داد[0];
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);

    unsigned صحیح index = (این+درون_داد[0]).get_coverage (c->buffer->cur().codepoint);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ساختار ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {این, این, این}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      درون_داد.len, درون_داد.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  منطقی serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ثابت تهی* base) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    اگر (unlikely (!out->serialize (c->serializer, 0u))) return_trace (نادرست);

    برای (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      اگر (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (نادرست);
    }

    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);

    اگر (unlikely (!c->serializer->embed (این->format))) return_trace (نادرست);

    اگر (!serialize_coverage_offsets (c, backtrack.iter (), این))
      return_trace (نادرست);

    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    اگر (!serialize_coverage_offsets (c, درون_داد.iter (), این))
      return_trace (نادرست);

    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    اگر (!serialize_coverage_offsets (c, lookahead.iter (), این))
      return_trace (نادرست);

    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ثابت hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_شمار_2 = c->serializer->copy<HBUINT16> (lookup.len);
    اگر (!lookup_شمار_2) return_trace (نادرست);

    unsigned شمار = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_شمار_2, شمار, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!backtrack.sanitize (c, این))) return_trace (نادرست);
    hb_barrier ();
    ثابت auto &درون_داد = StructAfter<decltype (درون_داد_X)> (backtrack);
    اگر (unlikely (!درون_داد.sanitize (c, این))) return_trace (نادرست);
    hb_barrier ();
    اگر (unlikely (!درون_داد.len)) return_trace (نادرست); /* To be consistent with Context. */
    ثابت auto &lookahead = StructAfter<decltype (lookaheadX)> (درون_داد);
    اگر (unlikely (!lookahead.sanitize (c, این))) return_trace (نادرست);
    hb_barrier ();
    ثابت auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  محافظتشده:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		درون_داد_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  عمومی:
  DEFINE_SIZE_MIN (10);
};

ساختار ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالت 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }		format;	/* Format identifier */
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
ساختار ExtensionFormat1
{
  unsigned صحیح get_type () ثابت { بازگشت extensionLookupType; }

  template <typename X>
  ثابت X& get_subtable () ثابت
  { بازگشت این + reinterpret_cast<ثابت Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, این))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);

    auto *out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    ثابت auto& src_offset =
        reinterpret_cast<ثابت Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, این, get_type ()));
  }

  محافظتشده:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  عمومی:
  DEFINE_SIZE_STATIC (8);
};

template <typename T>
ساختار Extension
{
  unsigned صحیح get_type () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.get_type ();
    پیشفرض:بازگشت 0;
    }
  }
  template <typename X>
  ثابت X& get_subtable () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.template get_subtable<typename T::SubTable> ();
    پیشفرض:بازگشت Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); بازگشت u.format1.subset (c);
    پیشفرض: بازگشت c->default_return_مقدار ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (این, &u.format.v))) بازگشت c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (این, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    پیشفرض:return_trace (c->default_return_مقدار ());
    }
  }

  محافظتشده:
  اجتماع {
  ساختار { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

ساختار hb_ot_layout_lookup_accelerator_t
{
  template <typename TLookup>
  ایستا hb_ot_layout_lookup_accelerator_t *create (ثابت TLookup &lookup)
  {
    unsigned شمار = lookup.get_subtable_شمار ();

    unsigned size = اندازه (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * اندازه (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    شمار * اندازه (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    اگر (unlikely (!thiz))
      بازگشت nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    وقتی (شمار && !thiz->subtables[شمار - 1].apply_func)
      شمار--;

    thiz->digest.init ();
    برای (auto& subtable : hb_iter (thiz->subtables, شمار))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->شمار = شمار;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    برای (unsigned i = 0; i < شمار; i++)
      اگر (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    بازگشت thiz;
  }

  تهی fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    برای (unsigned i = 0; i < شمار; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  منطقی may_have (hb_codepoint_t g) ثابت
  { بازگشت digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  منطقی apply (hb_ot_apply_context_t *c, منطقی use_cache) ثابت
  {
    اگر (شمار == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      اگر (use_cache)
	بازگشت subtables[0].apply_cached_no_digest (c);
#endif
      بازگشت subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    اگر (use_cache)
    {
      بازگشت
      + hb_iter (hb_iter (subtables, شمار))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { بازگشت _.apply_cached (c); })
      | hb_any
      ;
    }
    وگرنه
#endif
    {
      بازگشت
      + hb_iter (hb_iter (subtables, شمار))
      | hb_map ([&c] (ثابت hb_accelerate_subtables_context_t::hb_applicable_t &_) { بازگشت _.apply (c); })
      | hb_any
      ;
    }
    بازگشت نادرست;
  }

  منطقی cache_enter (hb_ot_apply_context_t *c) ثابت
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    بازگشت subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#وگرنه
    بازگشت نادرست;
#endif
  }
  تهی cache_leave (hb_ot_apply_context_t *c) ثابت
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  خصوصی:
  unsigned شمار = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

ساختار GSUBGPOS
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
    اگر (version.major != 1) بازگشت version.static_size;

    بازگشت min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#وگرنه
	   0
#endif
	   ;
  }

  ثابت ScriptList &get_script_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010002u && scriptList2) بازگشت این + scriptList2;
#endif
    بازگشت این + scriptList;
  }
  ثابت FeatureList &get_feature_list () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010002u && featureList2) بازگشت این + featureList2;
#endif
    بازگشت این + featureList;
  }
  template <typename TLookup>
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (این, get_size ())))
      return_trace (نادرست);
    hb_barrier ();

    منطقی script_list_ok = scriptList.sanitize (c, این);
    منطقی feature_list_ok = featureList.sanitize (c, این);
    منطقی lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, این);

#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010002u)
    {
      اگر (scriptList2) script_list_ok = scriptList2.sanitize (c, این);
      اگر (featureList2) feature_list_ok = featureList2.sanitize (c, این);
      اگر (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, این);
    }
#endif

    اگر (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (نادرست);

#ifndef HB_NO_VAR
    اگر (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, این, &get_feature_list ()))))
      return_trace (نادرست);
#endif

    return_trace (درست);
  }

  template <typename TLookup>
  منطقی subset (hb_subset_layout_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    اگر (version.major != 1) return_trace (نادرست);

    auto *out = c->subset_context->serializer->start_embed (این);
    اگر (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (نادرست);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010002u)
    {
      اگر (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (نادرست);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      اگر (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 این, c);
      وگرنه
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 این, c);

      اگر (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 این, c);
      وگرنه
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 این, c);

      اگر (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, این, c);
      وگرنه
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, این, c);
    }
    وگرنه
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     این,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     این,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					این,
					c);
    }

#ifndef HB_NO_VAR
    اگر (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      اگر (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (نادرست);

      // if all axes are pinned all feature vars are dropped.
      منطقی ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, این, c);
      اگر (!ret)
      {
        out->featureVars = 0;
	اگر (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (درست);
  }

  unsigned صحیح get_lookup_شمار () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010002u && lookupList2)
      بازگشت (این + lookupList2).len;
#endif
    بازگشت (این + lookupList).len;
  }
  ثابت Lookup& get_lookup (unsigned صحیح i) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010002u && lookupList2)
      بازگشت (این + lookupList2)[i];
#endif
    بازگشت (این + lookupList)[i];
  }
  ثابت FeatureVariations &get_feature_variations () ثابت
  {
    بازگشت (version.to_int () >= 0x00010001u && hb_barrier ()) ? این+featureVars : Null (FeatureVariations);
  }

  منطقی has_data () ثابت { بازگشت version.to_int (); }
  unsigned صحیح get_script_شمار () ثابت
  { بازگشت get_script_list ().len; }
  ثابت Tag& get_script_tag (unsigned صحیح i) ثابت
  { بازگشت get_script_list ().get_tag (i); }
  unsigned صحیح get_script_tags (unsigned صحیح start_offset,
				unsigned صحیح *script_شمار /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ثابت
  { بازگشت get_script_list ().get_tags (start_offset, script_شمار, script_tags); }
  ثابت Script& get_script (unsigned صحیح i) ثابت
  { بازگشت get_script_list ()[i]; }
  منطقی find_script_index (hb_tag_t tag, unsigned صحیح *index) ثابت
  { بازگشت get_script_list ().find_index (tag, index); }

  unsigned صحیح get_feature_شمار () ثابت
  { بازگشت get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned صحیح i) ثابت
  { بازگشت i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned صحیح get_feature_tags (unsigned صحیح start_offset,
				 unsigned صحیح *feature_شمار_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) ثابت
  { بازگشت get_feature_list ().get_tags (start_offset, feature_شمار_2, feature_tags); }
  ثابت Feature& get_feature (unsigned صحیح i) ثابت
  { بازگشت get_feature_list ()[i]; }
  منطقی find_feature_index (hb_tag_t tag, unsigned صحیح *index) ثابت
  { بازگشت get_feature_list ().find_index (tag, index); }

  منطقی find_variations_index (ثابت صحیح *coords, unsigned صحیح رقم_coords,
			      unsigned صحیح *index,
			      ItemVarStoreInstancer *instancer) ثابت
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    بازگشت نادرست;
#endif
    بازگشت get_feature_variations ().find_index (coords, رقم_coords, index, instancer);
  }
  منطقی get_feature_variations_state (ثابت صحیح *coords,
				     unsigned صحیح رقم_coords,
				     unsigned صحیح *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) ثابت
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    بازگشت درست;
#وگرنه
    find_variations_index (coords, رقم_coords, index, instancer);
    بازگشت get_feature_variations ().collect_lookup_conditions (
	coords,
	رقم_coords,
	instancer,
	lookup_conditions);
#endif
  }
  منطقی get_lookup_variations (unsigned feature_index,
                              unsigned variations_index,
                              hb_sorted_array_t<ثابت unsigned> conditions,
                              hb_set_t *lookup_indexes) ثابت
  {
#ifdef HB_NO_VAR
    بازگشت نادرست;
#وگرنه
    بازگشت get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  ثابت Feature& get_feature_variation (unsigned صحیح feature_index,
					unsigned صحیح variations_index) ثابت
  {
#ifndef HB_NO_VAR
    اگر (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      ثابت Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      اگر (feature)
	بازگشت *feature;
    }
#endif
    بازگشت get_feature (feature_index);
  }

  تهی feature_variation_collect_lookups (ثابت hb_set_t *feature_indexes,
					  ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) ثابت
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  تهی collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  تهی collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  تهی closure_lookups (hb_face_t      *face,
			ثابت hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) ثابت
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    برای (unsigned lookup_index : *lookup_indexes)
      reinterpret_cast<ثابت TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  تهی prune_langsys (ثابت hb_map_t *duplicate_feature_map,
                      ثابت hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) ثابت
  {
    hb_prune_langsys_context_t c (این, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned شمار = get_script_شمار ();
    برای (unsigned script_index = 0; script_index < شمار; script_index++)
    {
      ثابت Tag& tag = get_script_tag (script_index);
      اگر (!layout_scripts->has (tag)) ادامه;
      ثابت Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  تهی prune_features (ثابت hb_map_t *lookup_indices, /* IN */
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
    اگر (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      بازگشت;
    }
#endif

    برای (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      اگر (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        ادامه;


      ثابت Feature *f = &(get_feature (i));
      ثابت Feature** p = nullptr;
      اگر (feature_substitutes_map->has (i, &p))
        f = *p;

      اگر (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        ادامه;

      اگر (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  تهی collect_name_ids (ثابت hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         ثابت hb_hashmap_t<unsigned, ثابت Feature*> *feature_substitutes_map,
                         ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         ثابت hb_set_t *catch_all_features,
                         منطقی all_axes_pinned HB_UNUSED) ثابت
  {
    unsigned شمار = get_feature_شمار ();
    برای (unsigned i = 0 ; i < شمار; i++)
    {
      اگر (!feature_index_map->has (i)) ادامه;
      hb_tag_t tag = get_feature_tag (i);
      ثابت Feature *feature = &get_feature (i);
      ثابت Feature **substitute;
      اگر (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        اگر (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    اگر (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [این] (unsigned i) { بازگشت get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = درست;
      این->table = sc.reference_table<T> (face);

      اگر (unlikely (این->table->is_blocklisted (این->table.get_blob (), face)))
      {
	hb_blob_destroy (این->table.get_blob ());
	این->table = hb_blob_get_empty ();
      }

      این->lookup_شمار = table->get_lookup_شمار ();

      این->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (این->lookup_شمار, اندازه (*accels));
      اگر (unlikely (!این->accels))
      {
	این->lookup_شمار = 0;
	این->table.destroy ();
	این->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      برای (unsigned صحیح i = 0; i < این->lookup_شمار; i++)
      {
	auto *accel = این->accels[i].get_relaxed ();
	اگر (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (این->accels);
      این->table.destroy ();
    }

    hb_blob_t *get_blob () ثابت { بازگشت table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_index) ثابت
    {
      اگر (unlikely (lookup_index >= lookup_شمار)) بازگشت nullptr;

    retry:
      auto *accel = accels[lookup_index].get_acquire ();
      اگر (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	اگر (unlikely (!accel))
	  بازگشت nullptr;

	اگر (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  برو retry;
	}
      }

      بازگشت accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned صحیح lookup_شمار;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  محافظتشده:
  template <typename TLookup>
  ثابت Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () ثابت
  { بازگشت reinterpret_cast<ثابت Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ثابت Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () ثابت
  { بازگشت reinterpret_cast<ثابت Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  ثابت Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () ثابت
  { بازگشت reinterpret_cast<ثابت Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { بازگشت reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ثابت Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () ثابت
  { بازگشت reinterpret_cast<ثابت Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  ثابت Offset16To<RecordListOfFeature>& subset_feature_list_offset () ثابت
  { بازگشت reinterpret_cast<ثابت Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { بازگشت reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  ثابت Offset32To<RecordListOfFeature>& subset_feature_list2_offset () ثابت
  { بازگشت reinterpret_cast<ثابت Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  ثابت تهی* get_lookup_list_field_offset () ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (version.to_int () >= 0x00010002u && lookupList2) بازگشت &lookupList2;
#endif
    بازگشت &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  ایستا منطقی serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ثابت OffsetIn& in,
				       ثابت Base *base,
				       Ts&&... ds)
  {
    out = 0;
    اگر (in.is_null ()) بازگشت نادرست;

    auto *s = c->serializer;
    s->push ();
    منطقی ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    اگر (ret) s->add_link (out, s->برداشت_pack ());
    وگرنه s->برداشت_discard ();
    بازگشت ret;
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
  عمومی:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
