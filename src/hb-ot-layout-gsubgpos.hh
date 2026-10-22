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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-buffer.hh"
#অন্তর্ভাব "hb-map.hh"
#অন্তর্ভাব "hb-set.hh"
#অন্তর্ভাব "hb-ot-map.hh"
#অন্তর্ভাব "hb-ot-layout-common.hh"
#অন্তর্ভাব "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাব "hb-depend-data.hh"


নামাকাশ OT {


কাঠামো hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, যৌক্তিক>
{
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { ফেরত obj.intersects (এই->glyphs); }
  স্থির return_t default_return_মান () { ফেরত মিথ্যা; }
  যৌক্তিক stop_sublookup_iteration (return_t r) ধ্রুবক { ফেরত r; }

  ধ্রুবক hb_set_t *glyphs;

  hb_intersects_context_t (ধ্রুবক hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

কাঠামো hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, যৌক্তিক>
{
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { ফেরত obj.may_have_non_1to1 (); }
  স্থির return_t default_return_মান () { ফেরত মিথ্যা; }
  যৌক্তিক stop_sublookup_iteration (return_t r) ধ্রুবক { ফেরত r; }
};

কাঠামো hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  প্রকারলেখ return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_সূচক, hb_set_t *covered_seq_indicies, unsigned seq_সূচক, unsigned end_সূচক);
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { obj.closure (এই); ফেরত hb_empty_t (); }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }
  শূন্য recurse (unsigned lookup_সূচক, hb_set_t *covered_seq_indicies, unsigned seq_সূচক, unsigned end_সূচক)
  {
    যদি (unlikely (nesting_level_বাম == 0 || !recurse_func))
      ফেরত;

    nesting_level_বাম--;
    recurse_func (এই, lookup_সূচক, covered_seq_indicies, seq_সূচক, end_সূচক);
    nesting_level_বাম++;
  }

  শূন্য reset_lookup_visit_গুনতি ()
  { lookup_গুনতি = 0; }

  যৌক্তিক lookup_limit_exceeded ()
  { ফেরত lookup_গুনতি > HB_MAX_LOOKUP_VISIT_গুনতি; }

  যৌক্তিক should_visit_lookup (unsigned পূর্ণসংখ্যা lookup_সূচক)
  {
    যদি (lookup_গুনতি++ > HB_MAX_LOOKUP_VISIT_গুনতি)
      ফেরত মিথ্যা;

    যদি (is_lookup_done (lookup_সূচক))
      ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  যৌক্তিক is_lookup_done (unsigned পূর্ণসংখ্যা lookup_সূচক)
  {
    যদি (unlikely (done_lookups_glyph_গুনতি->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      ফেরত সত্য;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned বের_করো = glyphs->get_population ();
    যদি (unlikely (বের_করো == HB_MAP_মান_INVALID)) // Avoid sentinel interference.
      বের_করো--;
    যদি (done_lookups_glyph_গুনতি->get (lookup_সূচক) != বের_করো)
    {
      done_lookups_glyph_গুনতি->set (lookup_সূচক, বের_করো);

      যদি (!done_lookups_glyph_set->has (lookup_সূচক))
      {
	যদি (unlikely (!done_lookups_glyph_set->set (lookup_সূচক, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  ফেরত সত্য;
      }

      done_lookups_glyph_set->get (lookup_সূচক)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_সূচক);
    যদি (unlikely (covered_glyph_set->in_error ()))
      ফেরত সত্য;
    যদি (parent_active_glyphs ().is_subset (*covered_glyph_set))
      ফেরত সত্য;

    covered_glyph_set->union_ (parent_active_glyphs ());
    ফেরত মিথ্যা;
  }

  ধ্রুবক hb_set_t& previous_parent_active_glyphs () {
    যদি (active_glyphs_স্তূপ.দৈর্ঘ্য <= 1)
      ফেরত *glyphs;

    ফেরত active_glyphs_স্তূপ[active_glyphs_স্তূপ.দৈর্ঘ্য - 2];
  }

  ধ্রুবক hb_set_t& parent_active_glyphs ()
  {
    যদি (!active_glyphs_স্তূপ)
      ফেরত *glyphs;

    ফেরত active_glyphs_স্তূপ.লেজ ();
  }

  hb_set_t* ঠেলো_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_স্তূপ.push ();
    যদি (unlikely (active_glyphs_স্তূপ.in_error ()))
      ফেরত nullptr;
    ফেরত s;
  }

  যৌক্তিক বের_করো_cur_done_glyphs ()
  {
    যদি (!active_glyphs_স্তূপ)
      ফেরত মিথ্যা;

    active_glyphs_স্তূপ.বের_করো ();
    ফেরত সত্য;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_স্তূপ;
  recurse_func_t recurse_func = nullptr;
  unsigned পূর্ণসংখ্যা nesting_level_বাম;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_গুনতি_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned পূর্ণসংখ্যা nesting_level_বাম_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_বাম (nesting_level_বাম_2),
			  done_lookups_glyph_গুনতি (done_lookups_glyph_গুনতি_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  শূন্য set_recurse_func (recurse_func_t func) { recurse_func = func; }

  শূন্য flush ()
  {
    output->del_range (face->get_অঙ্ক_glyphs (), HB_SET_মান_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_স্তূপ.বের_করো ();
    active_glyphs_স্তূপ.reset ();
  }

  ব্যক্তিগত:
  hb_map_t *done_lookups_glyph_গুনতি;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned পূর্ণসংখ্যা lookup_গুনতি = 0;
};



কাঠামো hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  প্রকারলেখ return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_সূচক);
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { obj.closure_lookups (এই); ফেরত hb_empty_t (); }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }
  শূন্য recurse (unsigned lookup_সূচক)
  {
    যদি (unlikely (nesting_level_বাম == 0 || !recurse_func))
      ফেরত;

    /* Return if new lookup was recursed to before. */
    যদি (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_সূচক))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      ফেরত;

    nesting_level_বাম--;
    recurse_func (এই, lookup_সূচক);
    nesting_level_বাম++;
  }

  শূন্য set_lookup_visited (unsigned lookup_সূচক)
  { visited_lookups->add (lookup_সূচক); }

  শূন্য set_lookup_inactive (unsigned lookup_সূচক)
  { inactive_lookups->add (lookup_সূচক); }

  যৌক্তিক lookup_limit_exceeded ()
  {
    যৌক্তিক ret = lookup_গুনতি > HB_MAX_LOOKUP_VISIT_গুনতি;
    যদি (ret)
      DEBUG_বার্তা (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    ফেরত ret; }

  যৌক্তিক is_lookup_visited (unsigned lookup_সূচক)
  {
    যদি (unlikely (lookup_গুনতি++ > HB_MAX_LOOKUP_VISIT_গুনতি))
    {
      DEBUG_বার্তা (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_গুনতি, lookup_সূচক);
      ফেরত সত্য;
    }

    যদি (unlikely (visited_lookups->in_error ()))
      ফেরত সত্য;

    ফেরত visited_lookups->has (lookup_সূচক);
  }

  hb_face_t *face;
  ধ্রুবক hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned পূর্ণসংখ্যা nesting_level_বাম;

  hb_closure_lookups_context_t (hb_face_t *face_,
				ধ্রুবক hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				unsigned nesting_level_বাম_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_বাম (nesting_level_বাম_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_গুনতি (0) {}

  শূন্য set_recurse_func (recurse_func_t func) { recurse_func = func; }

  ব্যক্তিগত:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned পূর্ণসংখ্যা lookup_গুনতি;
};

কাঠামো hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, যৌক্তিক>
{
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { ফেরত obj.would_apply (এই); }
  স্থির return_t default_return_মান () { ফেরত মিথ্যা; }
  যৌক্তিক stop_sublookup_iteration (return_t r) ধ্রুবক { ফেরত r; }

  hb_face_t *face;
  ধ্রুবক hb_codepoint_t *glyphs;
  unsigned পূর্ণসংখ্যা len;
  যৌক্তিক zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ধ্রুবক hb_codepoint_t *glyphs_,
			    unsigned পূর্ণসংখ্যা len_,
			    যৌক্তিক zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

কাঠামো hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  প্রকারলেখ return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned পূর্ণসংখ্যা lookup_সূচক);
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { obj.collect_glyphs (এই); ফেরত hb_empty_t (); }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }
  শূন্য recurse (unsigned পূর্ণসংখ্যা lookup_সূচক)
  {
    যদি (unlikely (nesting_level_বাম == 0 || !recurse_func))
      ফেরত;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    যদি (output == hb_set_get_empty ())
      ফেরত;

    /* Return if new lookup was recursed to before. */
    যদি (recursed_lookups->has (lookup_সূচক))
      ফেরত;

    hb_set_t *old_before = before;
    hb_set_t *old_প্রবেশ  = প্রবেশ;
    hb_set_t *old_after  = after;
    before = প্রবেশ = after = hb_set_get_empty ();

    nesting_level_বাম--;
    recurse_func (এই, lookup_সূচক);
    nesting_level_বাম++;

    before = old_before;
    প্রবেশ  = old_প্রবেশ;
    after  = old_after;

    recursed_lookups->add (lookup_সূচক);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *প্রবেশ;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned পূর্ণসংখ্যা nesting_level_বাম;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_প্রবেশ,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned পূর্ণসংখ্যা nesting_level_বাম_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      প্রবেশ  (glyphs_প্রবেশ  ? glyphs_প্রবেশ  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_বাম (nesting_level_বাম_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  শূন্য set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

কাঠামো hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  গণনা glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  স্থির constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  কাঠামো glyph_set_cache_চাবি_t
  {
    glyph_set_cache_চাবি_t () = পূর্বনির্ধারিত;
    glyph_set_cache_চাবি_t (ধ্রুবক শূন্য *তথ্য_2, unsigned মান_3, glyph_set_type_t type_)
      : তথ্য (তথ্য_2), value (মান_3), type (type_) {}

    যৌক্তিক operator == (ধ্রুবক glyph_set_cache_চাবি_t &o) ধ্রুবক
    { ফেরত তথ্য == o.তথ্য && value == o.value && type == o.type; }

    uint32_t hash () ধ্রুবক
    {
      uint32_t current = hb_hash ((uintptr_t) তথ্য);
      current = current * 31 + hb_hash (value);
      ফেরত current * 31 + hb_hash ((unsigned) type);
    }

    ধ্রুবক শূন্য *তথ্য = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  কাঠামো context_set_cache_চাবি_t
  {
    context_set_cache_চাবি_t () = পূর্বনির্ধারিত;
    context_set_cache_চাবি_t (ধ্রুবক শূন্য *rule_,
			     ধ্রুবক শূন্য *record_,
			     ধ্রুবক শূন্য *তথ্য_2,
			     hb_codepoint_t active_glyphs_,
			     unsigned মান_3,
			     unsigned format_)
      : rule (rule_), record (record_), তথ্য (তথ্য_2), active_glyphs (active_glyphs_),
	value (মান_3), format (format_) {}

    যৌক্তিক operator == (ধ্রুবক context_set_cache_চাবি_t &o) ধ্রুবক
    {
      ফেরত rule == o.rule &&
	     record == o.record &&
	     তথ্য == o.তথ্য &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ধ্রুবক
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) তথ্য);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      ফেরত current * 31 + hb_hash (format);
    }

    ধ্রুবক শূন্য *rule = nullptr;
    ধ্রুবক শূন্য *record = nullptr;
    ধ্রুবক শূন্য *তথ্য = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  কাঠামো recurse_চাবি_t
  {
    recurse_চাবি_t () = পূর্বনির্ধারিত;
    recurse_চাবি_t (unsigned lookup_সূচক_3,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_সূচক (lookup_সূচক_3),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    যৌক্তিক operator == (ধ্রুবক recurse_চাবি_t &o) ধ্রুবক
    {
      ফেরত lookup_সূচক == o.lookup_সূচক &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () ধ্রুবক
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_সূচক)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      ফেরত current;
    }

    unsigned lookup_সূচক = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  প্রকারলেখ return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_সূচক, hb_bit_page_t *covered_seq_indices, unsigned seq_সূচক, unsigned end_সূচক);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { obj.depend (এই); ফেরত hb_empty_t (); }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }

  শূন্য recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_সূচক, unsigned end_সূচক)
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
    যদি (lookups_seen.has (lookup_idx)) ফেরত;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_চাবি = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_সূচক;
    যদি (!recurse_path_to_সূচক.has (path_চাবি, &path))
    {
      path_সূচক = recurse_path_to_সূচক.get_population () + 1;
      যদি (unlikely (!recurse_path_to_সূচক.set (path_চাবি, path_সূচক)))
      {
	depend_তথ্য->fail ();
	lookups_seen.del (lookup_idx);
	ফেরত;
      }
    }
    অন্যথা
      path_সূচক = *path;
    recurse_path = path_সূচক;
    recurse_func (এই, lookup_idx, covered_seq_indices, seq_সূচক, end_সূচক);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  কাঠামো active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    যৌক্তিক use_মূল = মিথ্যা;
  };

  ধ্রুবক hb_set_t& parent_active_glyphs ()
  {
    যদি (!active_glyphs_স্তূপ_depth)
      ফেরত *glyphs;

    ধ্রুবক active_glyphs_frame_t &frame = active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth - 1];
    ফেরত frame.use_মূল ? *glyphs : frame.glyphs;
  }

  hb_set_t* ঠেলো_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    যদি (active_glyphs_স্তূপ_depth == active_glyphs_স্তূপ.দৈর্ঘ্য &&
	unlikely (!active_glyphs_স্তূপ.ঠেলো_or_fail ()))
      ফেরত nullptr;

    active_glyphs_frame_t &frame = active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_মূল = মিথ্যা;
    ফেরত &frame.glyphs;
  }

  যৌক্তিক ঠেলো_মূল_active_glyphs ()
  {
    যদি (active_glyphs_স্তূপ_depth == active_glyphs_স্তূপ.দৈর্ঘ্য &&
	unlikely (!active_glyphs_স্তূপ.ঠেলো_or_fail ()))
      ফেরত মিথ্যা;

    active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth++].use_মূল = সত্য;
    ফেরত সত্য;
  }

  যৌক্তিক বের_করো_cur_done_glyphs ()
  {
    যদি (!active_glyphs_স্তূপ_depth)
      ফেরত মিথ্যা;

    active_glyphs_স্তূপ_depth--;
    ফেরত সত্য;
  }

  যৌক্তিক get_parent_active_glyphs_সূচক (hb_codepoint_t *active_idx)
  {
    যদি (!active_glyphs_স্তূপ_depth ||
	active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth - 1].use_মূল)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      ফেরত সত্য;
    }

    active_glyphs_frame_t &frame = active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth - 1];
    *active_idx = frame.active_idx;
    যদি (*active_idx != HB_CODEPOINT_INVALID)
      ফেরত সত্য;

    hb_codepoint_t glyph;
    যদি (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    অন্যথা যদি (!find_or_create_recurse_set (frame.glyphs, active_idx))
      ফেরত মিথ্যা;

    frame.active_idx = *active_idx;
    ফেরত সত্য;
  }

  যৌক্তিক get_context_set_cache_চাবি (ধ্রুবক শূন্য *rule,
				  ধ্রুবক শূন্য *record,
				  ধ্রুবক শূন্য *তথ্য,
				  unsigned value,
				  unsigned format,
				  context_set_cache_চাবি_t *key)
  {
    hb_codepoint_t active_idx;
    যদি (!get_parent_active_glyphs_সূচক (&active_idx))
      ফেরত মিথ্যা;
    *key = context_set_cache_চাবি_t (rule, record, তথ্য, active_idx, value, format);
    ফেরত সত্য;
  }

  যৌক্তিক get_recurse_চাবি (unsigned lookup_idx, recurse_চাবি_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    যদি (!get_parent_active_glyphs_সূচক (&active_idx))
      ফেরত মিথ্যা;

    *key = recurse_চাবি_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_তথ্য->current_context_set_সূচক);
    ফেরত !completed_recursions.has (*key);
  }

  শূন্য finish_recurse (ধ্রুবক recurse_চাবি_t &key)
  { depend_তথ্য->check_success (completed_recursions.set (key, সত্য)); }

  শূন্য reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_সূচক.clear ();
    recurse_path = 0;
  }

  যৌক্তিক find_or_create_recurse_set (ধ্রুবক hb_set_t &set, hb_codepoint_t *সূচক_2)
  {
    hb_codepoint_t *existing = nullptr;
    যদি (recurse_set_to_সূচক.has (&set, &existing))
    {
      *সূচক_2 = *existing;
      ফেরত সত্য;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    যদি (unlikely (!copy))
      ফেরত depend_তথ্য->fail ();
    copy->set (set);
    যদি (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      ফেরত depend_তথ্য->fail ();
    }

    *সূচক_2 = recurse_sets.দৈর্ঘ্য;
    যদি (unlikely (*সূচক_2 & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      ফেরত depend_তথ্য->fail ();
    }
    যদি (unlikely (!recurse_sets.ঠেলো_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      ফেরত depend_তথ্য->fail ();
    যদি (unlikely (!recurse_set_to_সূচক.set (recurse_sets[*সূচক_2].get (), *সূচক_2)))
      ফেরত depend_তথ্য->fail ();
    ফেরত সত্য;
  }

  ধ্রুবক hb_set_t *find_or_create_glyph_set (ধ্রুবক শূন্য *তথ্য,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (ধ্রুবক hb_set_t *set);

  কাঠামো depend_scratch_t
  {
    শূন্য clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      প্রবেশ_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<ধ্রুবক hb_set_t *> backtrack_sets;
    hb_vector_t<ধ্রুবক hb_set_t *> lookahead_sets;
    hb_vector_t<ধ্রুবক hb_set_t *> প্রবেশ_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<unsigned> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    যদি (scratch)
      depend_scratch_pool = scratch->next;
    অন্যথা
    {
      scratch = (depend_scratch_t *) hb_malloc (আকার (depend_scratch_t));
      যদি (unlikely (!scratch))
      {
        depend_তথ্য->fail ();
        ফেরত nullptr;
      }
      নতুন (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    ফেরত scratch;
  }

  শূন্য release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_তথ্য_builder_t *depend_তথ্য;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_স্তূপ;
  unsigned active_glyphs_স্তূপ_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_সূচক = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<ধ্রুবক hb_set_t *, hb_codepoint_t> recurse_set_to_সূচক;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_সূচক;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_চাবি_t, যৌক্তিক> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_চাবি_t, hb_codepoint_t> glyph_set_to_সূচক;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_চাবি_t, hb_codepoint_t> context_set_cache;
  depend_scratch_t *depend_scratch_pool = nullptr;

  hb_depend_context_t (hb_depend_তথ্য_builder_t *depend_তথ্য_2,
                       hb_face_t *face_,
                       hb_set_t *glyphs_)
                      : depend_তথ্য (depend_তথ্য_2),
                        face(face_),
                        glyphs (glyphs_),
                        recurse_func (nullptr) {}
  ~hb_depend_context_t ()
  {
    যখন (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  শূন্য set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

template <typename set_t>
কাঠামো hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, ধ্রুবক Coverage &>
{
  প্রকারলেখ ধ্রুবক Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { ফেরত obj.get_coverage (); }
  স্থির return_t default_return_মান () { ফেরত Null (Coverage); }
  যৌক্তিক stop_sublookup_iteration (return_t r) ধ্রুবক
  {
    r.collect_coverage (set);
    ফেরত মিথ্যা;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

কাঠামো matcher_t
{
  প্রকারলেখ যৌক্তিক (*match_func_t) (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য);

  template <typename context_t>
  শূন্য init (ধ্রুবক context_t *c, যৌক্তিক context_match = মিথ্যা)
  {
    set_match_func (nullptr, nullptr);
    lookup_props = c->lookup_props;
    /* Ignore ZWNJ if we are matching GPOS, or matching GSUB context and asked to. */
    ignore_zwnj = c->table_সূচক == 1 || (context_match && c->auto_zwnj);
    /* Ignore ZWJ if we are matching context, or asked to. */
    ignore_zwj = context_match || c->auto_zwj;
    /* Ignore hidden glyphs (like CGJ) during GPOS. */
    ignore_hidden = c->table_সূচক == 1;
    mask = context_match ? -1 : c->lookup_mask;
    /* Per syllable matching is only for GSUB. */
    per_syllable = c->table_সূচক == 0 && c->per_syllable;
    syllable = 0;
  }

  শূন্য set_match_func (match_func_t match_func_,
		       ধ্রুবক শূন্য *match_তথ্য)
  { match_func = match_func_; match_তথ্য_2 = match_তথ্য; }

  গণনা may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_তথ্য) ধ্রুবক
  {
    যদি (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      ফেরত MATCH_NO;

    যদি (match_func)
      ফেরত match_func (info, glyph_তথ্য, match_তথ্য_2) ? MATCH_YES : MATCH_NO;

    ফেরত MATCH_MAYBE;
  }

  গণনা may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  template <typename context_t>
#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  may_skip_t may_skip (ধ্রুবক context_t *c,
		       ধ্রুবক hb_glyph_info_t &info) ধ্রুবক
  {
    যদি (!c->check_glyph_property (&info, lookup_props))
      ফেরত SKIP_YES;

    যদি (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      ফেরত SKIP_MAYBE;

    ফেরত SKIP_NO;
  }

  সর্বজনীন:
  unsigned পূর্ণসংখ্যা lookup_props = 0;
  hb_mask_t mask = -1;
  যৌক্তিক ignore_zwnj = মিথ্যা;
  যৌক্তিক ignore_zwj = মিথ্যা;
  যৌক্তিক ignore_hidden = মিথ্যা;
  যৌক্তিক per_syllable = মিথ্যা;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  ধ্রুবক শূন্য *match_তথ্য_2 = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
কাঠামো glyph_তথ্য_t
{
  template <typename T>
  glyph_তথ্য_t (ধ্রুবক T *তথ্য_2) : তথ্য (তথ্য_2)
#ifndef HB_NO_BEYOND_64K
                              , wide (আকার (T) == 3)
#endif
  {
    static_assert (আকার (T) == 2 || আকার (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (wide)
      ফেরত ((ধ্রুবক HBUINT24 *) তথ্য)[i];
#endif
    ফেরত ((ধ্রুবক HBUINT16 *) তথ্য)[i];
  }

  ধ্রুবক শূন্য *তথ্য;
#ifndef HB_NO_BEYOND_64K
  যৌক্তিক wide;
#endif
};

template <typename context_t>
কাঠামো skipping_iterator_t
{
  শূন্য init (context_t *c_, যৌক্তিক context_match = মিথ্যা)
  {
    c = c_;
    end = c->বাফার->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  শূন্য set_lookup_props (unsigned পূর্ণসংখ্যা lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  শূন্য set_match_func (matcher_t::match_func_t match_func_,
		       ধ্রুবক শূন্য *match_তথ্য)
  {
    matcher.set_match_func (match_func_, match_তথ্য);
  }
  শূন্য set_glyph_তথ্য (glyph_তথ্য_t glyph_তথ্য)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_তথ্য.wide ? nullptr : (ধ্রুবক HBUINT16 *) glyph_তথ্য.তথ্য;
    match_glyph_data24 = glyph_তথ্য.wide ? (ধ্রুবক HBUINT24 *) glyph_তথ্য.তথ্য : nullptr;
#অন্যথা
    match_glyph_data16 = (ধ্রুবক HBUINT16 *) glyph_তথ্য.তথ্য;
#endif
  }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  শূন্য reset (unsigned পূর্ণসংখ্যা start_সূচক_2)
  {
    // For GSUB forward iterator
    idx = start_সূচক_2;
    end = c->বাফার->len;
    matcher.syllable = c->বাফার->cur().syllable();
  }
  শূন্য reset_back (unsigned পূর্ণসংখ্যা start_সূচক_2, যৌক্তিক from_out_বাফার = মিথ্যা)
  {
    // For GSUB backward iterator
    idx = start_সূচক_2;
    matcher.syllable = c->বাফার->cur().syllable();
  }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  শূন্য reset_fast (unsigned পূর্ণসংখ্যা start_সূচক_2)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_সূচক_2;
  }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (ধ্রুবক hb_glyph_info_t &info) ধ্রুবক
  { ফেরত matcher.may_skip (c, info); }

  গণনা match_t {
    MATCH,
    NOT_MATCH,
    SKIP
  };

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  match_t match (hb_glyph_info_t &info)
  {
    matcher_t::may_skip_t skip = matcher.may_skip (c, info);
    যদি (unlikely (skip == matcher_t::SKIP_YES))
      ফেরত SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_তথ্য ());
    যদি (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      ফেরত MATCH;

    যদি (skip == matcher_t::SKIP_NO)
      ফেরত NOT_MATCH;

    ফেরত SKIP;
  }

  যৌক্তিক next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->বাফার->info;
    ধ্রুবক signed stop = (signed) end - 1;
    যখন ((signed) idx < stop)
    {
      idx++;
      নির্বাচন (match (info[idx]))
      {
	ক্ষেত্র MATCH:
	{
	  advance_glyph_তথ্য ();
	  ফেরত সত্য;
	}
	ক্ষেত্র NOT_MATCH:
	{
	  যদি (unsafe_to)
	    *unsafe_to = idx + 1;
	  ফেরত মিথ্যা;
	}
	ক্ষেত্র SKIP:
	  চালু;
      }
    }
    যদি (unsafe_to)
      *unsafe_to = end;
    ফেরত মিথ্যা;
  }
  যৌক্তিক prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->বাফার->out_info;
    ধ্রুবক unsigned stop = 0;
    যখন (idx > stop)
    {
      idx--;
      নির্বাচন (match (out_info[idx]))
      {
	ক্ষেত্র MATCH:
	{
	  advance_glyph_তথ্য ();
	  ফেরত সত্য;
	}
	ক্ষেত্র NOT_MATCH:
	{
	  যদি (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  ফেরত মিথ্যা;
	}
	ক্ষেত্র SKIP:
	  চালু;
      }
    }
    যদি (unsafe_from)
      *unsafe_from = 0;
    ফেরত মিথ্যা;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_তথ্য ()
  {
    যদি (match_glyph_data16) ফেরত *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    অন্যথা
    যদি (match_glyph_data24) ফেরত *match_glyph_data24;
#endif
    ফেরত 0;
  }
  HB_ALWAYS_INLINE
  শূন্য
  advance_glyph_তথ্য ()
  {
    যদি (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    অন্যথা
    যদি (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned পূর্ণসংখ্যা idx;
  সুরক্ষিত:
  context_t *c;
  matcher_t matcher;
  ধ্রুবক HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  ধ্রুবক HBUINT24 *match_glyph_data24;
#endif

  unsigned পূর্ণসংখ্যা end;
};

কাঠামো hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, যৌক্তিক, HB_DEBUG_APPLY>
{
  ধ্রুবক অক্ষর *get_name () { ফেরত "APPLY"; }
  প্রকারলেখ return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned পূর্ণসংখ্যা lookup_সূচক);

  template <typename T>
  স্থির অন্তঃরেখ auto apply_ (ধ্রুবক T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  স্থির অন্তঃরেখ auto apply_ (ধ্রুবক T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj) { ফেরত apply_(obj, এই, hb_prioritize); }

  স্থির return_t default_return_মান () { ফেরত মিথ্যা; }
  যৌক্তিক stop_sublookup_iteration (return_t r) ধ্রুবক { ফেরত r; }
  return_t recurse (unsigned পূর্ণসংখ্যা sub_lookup_সূচক)
  {
    প্রতিজ্ঞা (recurse_func);
    যদি (unlikely (nesting_level_বাম == 0))
    {
      বাফার->successful = মিথ্যা;
      ফেরত default_return_মান ();
    }

    বাফার->max_ops--;
    যদি (unlikely (বাফার->max_ops < 0))
    {
      বাফার->successful = মিথ্যা;
      ফেরত default_return_মান ();
    }

    nesting_level_বাম--;
    যৌক্তিক ret = recurse_func (এই, sub_lookup_সূচক);
    nesting_level_বাম++;
    ফেরত ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_প্রবেশ, iter_context;

  unsigned পূর্ণসংখ্যা table_সূচক; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_বাফার_t *বাফার;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  ধ্রুবক GDEF &gdef;
  ধ্রুবক GDEF::accelerator_t &gdef_accel;
  ধ্রুবক ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  unsigned পূর্ণসংখ্যা lookup_সূচক = (unsigned) -1;
  unsigned পূর্ণসংখ্যা lookup_props = 0;
  unsigned পূর্ণসংখ্যা nesting_level_বাম = HB_MAX_NESTING_LEVEL;

  যৌক্তিক has_glyph_classes;
  যৌক্তিক auto_zwnj = সত্য;
  যৌক্তিক auto_zwj = সত্য;
  যৌক্তিক per_syllable = মিথ্যা;
  যৌক্তিক random = মিথ্যা;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t স্তূপ_match_positions[8];
#ifndef HB_NO_বাফার_MESSAGE
  hb_বাফার_t::changed_func_t orig_changed_func = nullptr;
  শূন্য *orig_changed_তথ্য = nullptr;
#endif

  hb_ot_apply_context_t (unsigned পূর্ণসংখ্যা table_সূচক_2,
			 hb_font_t *font_,
			 hb_বাফার_t *বাফার_2,
			 hb_blob_t *table_blob_,
			 hb_scalar_cache_t *var_store_cache_ = nullptr) :
			table_সূচক (table_সূচক_2),
			font (font_), face (font->face), বাফার (বাফার_2),
			sanitizer (table_blob_),
			gdef (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF->table
#অন্যথা
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#অন্যথা
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (বাফার_2->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (স্তূপ_match_positions);

#ifndef HB_NO_বাফার_MESSAGE
    যদি (বাফার->messaging ())
    {
      প্রতিজ্ঞা (বাফার->changed_func == nullptr ||
	      বাফার->changed_func == বাফার_changed_trampoline);
      orig_changed_func = বাফার->changed_func;
      orig_changed_তথ্য = বাফার->changed_তথ্য;
      বাফার->changed_func = বাফার_changed_trampoline;
      বাফার->changed_তথ্য = এই;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_বাফার_MESSAGE
    যদি (বাফার->messaging ())
    {
      প্রতিজ্ঞা (বাফার->changed_func == বাফার_changed_trampoline);
      প্রতিজ্ঞা (বাফার->changed_তথ্য == এই);
      বাফার->changed_func = orig_changed_func;
      বাফার->changed_তথ্য = orig_changed_তথ্য;
    }
#endif
  }

  শূন্য init_iters ()
  {
    iter_প্রবেশ.init (এই, মিথ্যা);
    iter_context.init (এই, সত্য);
  }

  শূন্য set_lookup_mask (hb_mask_t mask, যৌক্তিক init = সত্য) { lookup_mask = mask; last_base = -1; last_base_until = 0; যদি (init) init_iters (); }
  শূন্য set_auto_zwj (যৌক্তিক auto_zwj_, যৌক্তিক init = সত্য) { auto_zwj = auto_zwj_; যদি (init) init_iters (); }
  শূন্য set_auto_zwnj (যৌক্তিক auto_zwnj_, যৌক্তিক init = সত্য) { auto_zwnj = auto_zwnj_; যদি (init) init_iters (); }
  শূন্য set_per_syllable (যৌক্তিক per_syllable_, যৌক্তিক init = সত্য) { per_syllable = per_syllable_; যদি (init) init_iters (); }
  শূন্য set_random (যৌক্তিক random_) { random = random_; }
  শূন্য set_recurse_func (recurse_func_t func) { recurse_func = func; }
  শূন্য set_lookup_সূচক (unsigned পূর্ণসংখ্যা lookup_সূচক_3) { lookup_সূচক = lookup_সূচক_3; }
  শূন্য set_lookup_props (unsigned পূর্ণসংখ্যা lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_সংখ্যা ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    বাফার->random_state = বাফার->random_state * 48271 % 2147483647;
    ফেরত বাফার->random_state;
  }

  স্থির শূন্য বাফার_changed_trampoline (hb_বাফার_t *বাফার HB_UNUSED, শূন্য *user_তথ্য)
  {
    ((hb_ot_apply_context_t *) user_তথ্য)->বাফার_changed ();
  }
  শূন্য বাফার_changed ()
  {
    বাফার->update_digest ();
    যদি (likely (has_glyph_classes))
      জন্য (unsigned i = 0; i < বাফার->len; i++)
	_set_glyph_class_props (বাফার->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  যৌক্তিক match_properties_mark (ধ্রুবক hb_glyph_info_t *info,
			      unsigned পূর্ণসংখ্যা    glyph_props,
			      unsigned পূর্ণসংখ্যা    match_props) ধ্রুবক
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    যদি (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_সূচক = match_props >> 16;
      ফেরত gdef_accel.mark_set_covers (set_সূচক, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    যদি (match_props & LookupFlag::MarkAttachmentType)
      ফেরত (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    ফেরত সত্য;
  }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  যৌক্তিক check_glyph_property (ধ্রুবক hb_glyph_info_t *info,
			     unsigned match_props) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    যদি (glyph_props & match_props & LookupFlag::IgnoreFlags)
      ফেরত মিথ্যা;

    যদি (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      ফেরত match_properties_mark (info, glyph_props, match_props);

    ফেরত সত্য;
  }

  শূন্য _set_glyph_class (hb_codepoint_t glyph_সূচক,
			  unsigned পূর্ণসংখ্যা class_guess = 0,
			  যৌক্তিক ligature = মিথ্যা,
			  যৌক্তিক component = মিথ্যা)
  {
    বাফার->digest.add (glyph_সূচক);

    যদি (new_syllables != (unsigned) -1)
      বাফার->cur().syllable() = new_syllables;

    unsigned পূর্ণসংখ্যা props = _hb_glyph_info_get_glyph_props (&বাফার->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    যদি (ligature)
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
    যদি (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    যদি (likely (has_glyph_classes))
    {
      _set_glyph_class_props (বাফার->cur(), props, glyph_সূচক);
    }
    অন্যথা যদি (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&বাফার->cur(), props | class_guess);
    }
    অন্যথা
      _hb_glyph_info_set_glyph_props (&বাফার->cur(), props);
  }
  শূন্য _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned পূর্ণসংখ্যা props,
			       hb_codepoint_t glyph_সূচক)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_সূচক));
  }
  শূন্য _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  শূন্য replace_glyph (hb_codepoint_t glyph_সূচক)
  {
    _set_glyph_class (glyph_সূচক);
    (শূন্য) বাফার->replace_glyph (glyph_সূচক);
  }
  শূন্য replace_glyph_inplace (hb_codepoint_t glyph_সূচক)
  {
    _set_glyph_class (glyph_সূচক);
    বাফার->cur().codepoint = glyph_সূচক;
  }
  শূন্য replace_glyph_with_ligature (hb_codepoint_t glyph_সূচক,
				    unsigned পূর্ণসংখ্যা class_guess)
  {
    _set_glyph_class (glyph_সূচক, class_guess, সত্য);
    (শূন্য) বাফার->replace_glyph (glyph_সূচক);
  }
  শূন্য output_glyph_for_component (hb_codepoint_t glyph_সূচক,
				   unsigned পূর্ণসংখ্যা class_guess)
  {
    _set_glyph_class (glyph_সূচক, class_guess, মিথ্যা, সত্য);
    (শূন্য) বাফার->output_glyph (glyph_সূচক);
  }
};

গণনা শ্রেণী hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

কাঠামো hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  template <typename T>
  স্থির অন্তঃরেখ auto collect_second_glyphs_ (ধ্রুবক T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (শূন্য, obj.collect_second_glyphs (digest) )
  template <typename T>
  স্থির অন্তঃরেখ শূন্য collect_second_glyphs_ (ধ্রুবক T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  স্থির অন্তঃরেখ auto apply_ (ধ্রুবক T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<1>) HB_RETURN (যৌক্তিক, obj->apply (c, external_cache) )
  template <typename T>
  স্থির অন্তঃরেখ auto apply_ (ধ্রুবক T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<0>) HB_RETURN (যৌক্তিক, obj->apply (c) )
  template <typename T>
  স্থির অন্তঃরেখ যৌক্তিক apply_to (ধ্রুবক শূন্য *obj, hb_ot_apply_context_t *c, শূন্য *external_cache)
  {
    ধ্রুবক T *typed_obj = (ধ্রুবক T *) obj;
    ফেরত apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  স্থির অন্তঃরেখ auto apply_cached_ (ধ্রুবক T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<2>) HB_RETURN (যৌক্তিক, obj->apply_cached (c, external_cache) )
  template <typename T>
  স্থির অন্তঃরেখ auto apply_cached_ (ধ্রুবক T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<1>) HB_RETURN (যৌক্তিক, obj->apply (c, external_cache) )
  template <typename T>
  স্থির অন্তঃরেখ auto apply_cached_ (ধ্রুবক T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<0>) HB_RETURN (যৌক্তিক, obj->apply (c) )
  template <typename T>
  স্থির অন্তঃরেখ যৌক্তিক apply_cached_to (ধ্রুবক শূন্য *obj, hb_ot_apply_context_t *c, শূন্য *external_cache)
  {
    ধ্রুবক T *typed_obj = (ধ্রুবক T *) obj;
    ফেরত apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  স্থির অন্তঃরেখ auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (যৌক্তিক, T::cache_func (c, op) )
  template <typename T=শূন্য>
  স্থির অন্তঃরেখ যৌক্তিক cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { ফেরত মিথ্যা; }
  template <typename Type>
  স্থির অন্তঃরেখ যৌক্তিক cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    ফেরত cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  প্রকারলেখ যৌক্তিক (*hb_apply_func_t) (ধ্রুবক শূন্য *obj, hb_ot_apply_context_t *c, শূন্য *external_cache);
  প্রকারলেখ যৌক্তিক (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  কাঠামো hb_applicable_t
  {
    friend কাঠামো hb_accelerate_subtables_context_t;
    friend কাঠামো hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    শূন্য init (ধ্রুবক T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , শূন্য *external_cache_
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
    যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
    {
      ফেরত digest.may_have (c->বাফার->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    যৌক্তিক apply_no_digest (hb_ot_apply_context_t *c) ধ্রুবক
    {
      ফেরত apply_func (obj, c, nullptr);
    }
#অন্যথা
    যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
    {
      ফেরত digest.may_have (c->বাফার->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    যৌক্তিক apply_cached (hb_ot_apply_context_t *c) ধ্রুবক
    {
      ফেরত digest.may_have (c->বাফার->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    যৌক্তিক apply_no_digest (hb_ot_apply_context_t *c) ধ্রুবক
    {
      ফেরত apply_func (obj, c, external_cache);
    }
    যৌক্তিক apply_cached_no_digest (hb_ot_apply_context_t *c) ধ্রুবক
    {
      ফেরত apply_cached_func (obj, c, external_cache);
    }

    যৌক্তিক cache_enter (hb_ot_apply_context_t *c) ধ্রুবক
    {
      ফেরত cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    শূন্য cache_leave (hb_ot_apply_context_t *c) ধ্রুবক
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    ব্যক্তিগত:
    ধ্রুবক শূন্য *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    শূন্য *external_cache;
#endif
    hb_set_digest_t digest;
  };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  auto cache_cost (ধ্রুবক T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  template <typename T>
  auto cache_cost (ধ্রুবক T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  template <typename T>
  auto external_cache_create (ধ্রুবক T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  template <typename T>
  auto external_cache_create (ধ্রুবক T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ধ্রুবক T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    শূন্য *external_cache = nullptr;
    যদি (i < 8)
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
    যদি (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    ফেরত hb_empty_t ();
  }
  স্থির return_t default_return_মান () { ফেরত hb_empty_t (); }

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


প্রকারলেখ যৌক্তিক (*intersects_func_t) (ধ্রুবক hb_set_t *glyphs, unsigned value, ধ্রুবক শূন্য *তথ্য, শূন্য *cache);
প্রকারলেখ শূন্য (*intersected_glyphs_func_t) (ধ্রুবক hb_set_t *glyphs, ধ্রুবক শূন্য *তথ্য, unsigned value, hb_set_t *intersected_glyphs, শূন্য *cache);
প্রকারলেখ শূন্য (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, ধ্রুবক শূন্য *তথ্য);
প্রকারলেখ যৌক্তিক (*match_func_t) (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য);
প্রকারলেখ unsigned (*get_মান_func_t) (hb_glyph_info_t &info, ধ্রুবক শূন্য *তথ্য);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
কাঠামো hb_ot_layout_ruleset_digest_t
{
  শূন্য init () { mask = 0; }
  শূন্য set_full () { mask = (uint64_t) -1; }
  শূন্য add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  যৌক্তিক may_have (unsigned value) ধ্রুবক { ফেরত mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

কাঠামো ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
কাঠামো ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
কাঠামো ContextApplyFuncs
{
  match_func_t match;
};
কাঠামো ChainContextApplyFuncs
{
  match_func_t match[3];
};


স্থির অন্তঃরেখ যৌক্তিক intersects_glyph (ধ্রুবক hb_set_t *glyphs, unsigned value, ধ্রুবক শূন্য *তথ্য HB_UNUSED, শূন্য *cache HB_UNUSED)
{
  ফেরত glyphs->has (value);
}
স্থির অন্তঃরেখ যৌক্তিক intersects_class (ধ্রুবক hb_set_t *glyphs, unsigned value, ধ্রুবক শূন্য *তথ্য, শূন্য *cache)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *>(তথ্য);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  যদি (map->has (value, &cached_v))
    ফেরত *cached_v;

  যৌক্তিক v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  ফেরত v;
}
স্থির অন্তঃরেখ যৌক্তিক intersects_coverage (ধ্রুবক hb_set_t *glyphs, unsigned value, ধ্রুবক শূন্য *তথ্য, শূন্য *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  ফেরত (তথ্য+coverage).intersects (glyphs);
}


স্থির অন্তঃরেখ শূন্য intersected_glyph (ধ্রুবক hb_set_t *glyphs HB_UNUSED, ধ্রুবক শূন্য *তথ্য, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED শূন্য *cache)
{
  unsigned g = reinterpret_cast<ধ্রুবক HBUINT16 *>(তথ্য)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

স্থির অন্তঃরেখ শূন্য intersected_class_glyphs (ধ্রুবক hb_set_t *glyphs, ধ্রুবক শূন্য *তথ্য, unsigned value, hb_set_t *intersected_glyphs, শূন্য *cache)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *>(তথ্য);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  যদি (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    ফেরত;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

স্থির অন্তঃরেখ শূন্য intersected_coverage_glyphs (ধ্রুবক hb_set_t *glyphs, ধ্রুবক শূন্য *তথ্য, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED শূন্য *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (তথ্য+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


স্থির অন্তঃরেখ যৌক্তিক array_is_subset_of (ধ্রুবক hb_set_t *glyphs,
				       unsigned পূর্ণসংখ্যা গুনতি,
				       glyph_তথ্য_t values,
				       intersects_func_t intersects_func,
				       ধ্রুবক শূন্য *intersects_তথ্য,
				       শূন্য *cache)
{
  জন্য (unsigned i = 0; i < গুনতি; i++)
    যদি (!intersects_func (glyphs, values[i], intersects_তথ্য, cache)) ফেরত মিথ্যা;
  ফেরত সত্য;
}


স্থির অন্তঃরেখ শূন্য collect_glyph (hb_set_t *glyphs, unsigned value, ধ্রুবক শূন্য *তথ্য HB_UNUSED)
{
  glyphs->add (value);
}
স্থির অন্তঃরেখ শূন্য collect_class (hb_set_t *glyphs, unsigned value, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *>(তথ্য);
  class_def.collect_class (glyphs, value);
}
স্থির অন্তঃরেখ শূন্য collect_coverage (hb_set_t *glyphs, unsigned value, ধ্রুবক শূন্য *তথ্য)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (তথ্য+coverage).collect_coverage (glyphs);
}

অন্তঃরেখ ধ্রুবক hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (ধ্রুবক শূন্য *তথ্য,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_চাবি_t key (তথ্য, value, type);
  hb_codepoint_t *existing = nullptr;
  যদি (glyph_set_to_সূচক.has (key, &existing))
    ফেরত glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  যদি (unlikely (!set))
  {
    depend_তথ্য->fail ();
    ফেরত nullptr;
  }

  যদি (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, তথ্য);
  অন্যথা যদি (type == GLYPH_SET_CLASS)
    collect_class (set, value, তথ্য);
  অন্যথা
    set->add (value);
  যদি (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_তথ্য->fail ();
    ফেরত nullptr;
  }

  hb_codepoint_t সূচক_2 = glyph_sets.দৈর্ঘ্য;
  যদি (unlikely (!glyph_sets.ঠেলো_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_তথ্য->fail ();
    ফেরত nullptr;
  }
  যদি (unlikely (!glyph_set_to_সূচক.set (key, সূচক_2)))
  {
    depend_তথ্য->fail ();
    ফেরত nullptr;
  }
  ফেরত glyph_sets[সূচক_2].get ();
}

অন্তঃরেখ hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (ধ্রুবক hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  যদি (cached_context_set_indices.has (key, &existing))
    ফেরত *existing;

  hb_codepoint_t সূচক_2 = depend_তথ্য->find_or_create_context_set (*set);
  যদি (unlikely (সূচক_2 == HB_CODEPOINT_INVALID))
    ফেরত HB_CODEPOINT_INVALID;
  যদি (unlikely (!cached_context_set_indices.set (key, সূচক_2)))
    ফেরত depend_তথ্য->fail_invalid ();
  ফেরত সূচক_2;
}

স্থির অন্তঃরেখ শূন্য collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned পূর্ণসংখ্যা গুনতি,
				  glyph_তথ্য_t values,
				  collect_glyphs_func_t collect_func,
				  ধ্রুবক শূন্য *collect_তথ্য)
{
  জন্য (unsigned i = 0; i < গুনতি; i++)
    collect_func (glyphs, values[i], collect_তথ্য);
}


স্থির অন্তঃরেখ যৌক্তিক match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, ধ্রুবক শূন্য *তথ্য HB_UNUSED)
{
  ফেরত সত্য;
}
স্থির অন্তঃরেখ যৌক্তিক match_glyph (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য HB_UNUSED)
{
  ফেরত info.codepoint == value;
}
স্থির অন্তঃরেখ যৌক্তিক match_class (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *>(তথ্য);
  ফেরত class_def.get_class (info.codepoint) == value;
}
স্থির অন্তঃরেখ unsigned get_class_মান (hb_glyph_info_t &info, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *> (তথ্য);
  ফেরত class_def.get_class (info.codepoint);
}
কাঠামো match_class_cache_তথ্য_t
{
  ধ্রুবক ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
স্থির অন্তঃরেখ যৌক্তিক match_class_cache (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক auto &cache_তথ্য = *reinterpret_cast<ধ্রুবক match_class_cache_তথ্য_t *> (তথ্য);
  ফেরত cache_তথ্য.class_def->get_class (info.codepoint, cache_তথ্য.cache) == value;
}
স্থির অন্তঃরেখ unsigned get_class_cache_মান (hb_glyph_info_t &info, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক auto &cache_তথ্য = *reinterpret_cast<ধ্রুবক match_class_cache_তথ্য_t *> (তথ্য);
  ফেরত cache_তথ্য.class_def->get_class (info.codepoint, cache_তথ্য.cache);
}
স্থির অন্তঃরেখ unsigned get_class_cached (ধ্রুবক ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  যদি (klass < 255)
    ফেরত klass;
  klass = class_def.get_class (info.codepoint);
  যদি (likely (klass < 255))
    info.syllable() = klass;
  ফেরত klass;
}
স্থির অন্তঃরেখ যৌক্তিক match_class_cached (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *>(তথ্য);
  ফেরত get_class_cached (class_def, info) == value;
}
স্থির অন্তঃরেখ unsigned get_class_cached_মান (hb_glyph_info_t &info, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *> (তথ্য);
  ফেরত get_class_cached (class_def, info);
}
স্থির অন্তঃরেখ unsigned get_class_cached1 (ধ্রুবক ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  যদি (klass < 15)
    ফেরত klass;
  klass = class_def.get_class (info.codepoint);
  যদি (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  ফেরত klass;
}
স্থির অন্তঃরেখ যৌক্তিক match_class_cached1 (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *>(তথ্য);
  ফেরত get_class_cached1 (class_def, info) == value;
}
স্থির অন্তঃরেখ unsigned get_class_cached2 (ধ্রুবক ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  যদি (klass < 15)
    ফেরত klass;
  klass = class_def.get_class (info.codepoint);
  যদি (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  ফেরত klass;
}
স্থির অন্তঃরেখ যৌক্তিক match_class_cached2 (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *>(তথ্য);
  ফেরত get_class_cached2 (class_def, info) == value;
}
স্থির অন্তঃরেখ unsigned get_class_cached2_মান (hb_glyph_info_t &info, ধ্রুবক শূন্য *তথ্য)
{
  ধ্রুবক ClassDef &class_def = *reinterpret_cast<ধ্রুবক ClassDef *> (তথ্য);
  ফেরত get_class_cached2 (class_def, info);
}
স্থির অন্তঃরেখ যৌক্তিক match_coverage (hb_glyph_info_t &info, unsigned value, ধ্রুবক শূন্য *তথ্য)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  ফেরত (তথ্য+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

স্থির অন্তঃরেখ যৌক্তিক would_match_প্রবেশ (hb_would_apply_context_t *c,
				      unsigned পূর্ণসংখ্যা গুনতি, /* Including the first glyph (not matched) */
				      glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      ধ্রুবক শূন্য *match_তথ্য_2)
{
  যদি (গুনতি != c->len)
    ফেরত মিথ্যা;

  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    যদি (likely (!match_func (info, প্রবেশ[i - 1], match_তথ্য_2)))
      ফেরত মিথ্যা;
  }

  ফেরত সত্য;
}
#ifndef HB_OPTIMIZE_আকার
HB_ALWAYS_INLINE
#অন্যথা
অন্তঃরেখ
#endif
স্থির যৌক্তিক match_প্রবেশ (hb_ot_apply_context_t *c,
			 unsigned পূর্ণসংখ্যা গুনতি, /* Including the first glyph (not matched) */
			 glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 ধ্রুবক শূন্য *match_তথ্য_2,
			 unsigned পূর্ণসংখ্যা *end_position,
			 unsigned পূর্ণসংখ্যা *p_যোগ_component_গুনতি = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_বাফার_t *বাফার = c->বাফার;

  যদি (গুনতি == 1)
  {
    *end_position = বাফার->idx + 1;
    c->match_positions[0] = বাফার->idx;
    যদি (p_যোগ_component_গুনতি)
      *p_যোগ_component_গুনতি = hb_glyph_info_get_lig_অঙ্ক_comps (&বাফার->cur());
    return_trace (সত্য);
  }

  যদি (unlikely (গুনতি > HB_MAX_CONTEXT_দৈর্ঘ্য)) return_trace (মিথ্যা);

  auto &skippy_iter = c->iter_প্রবেশ;
  skippy_iter.reset (বাফার->idx);
  skippy_iter.set_match_func (match_func, match_তথ্য_2);
  skippy_iter.set_glyph_তথ্য (প্রবেশ);

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

  unsigned পূর্ণসংখ্যা যোগ_component_গুনতি = 0;

  unsigned পূর্ণসংখ্যা first_lig_id = _hb_glyph_info_get_lig_id (&বাফার->cur());
  unsigned পূর্ণসংখ্যা first_lig_comp = _hb_glyph_info_get_lig_comp (&বাফার->cur());

  গণনা {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
  {
    unsigned unsafe_to;
    যদি (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (মিথ্যা);
    }

    যদি (unlikely (i + 1 > c->match_positions.দৈর্ঘ্য &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (মিথ্যা);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned পূর্ণসংখ্যা this_lig_id = _hb_glyph_info_get_lig_id (&বাফার->info[skippy_iter.idx]);
    unsigned পূর্ণসংখ্যা this_lig_comp = _hb_glyph_info_get_lig_comp (&বাফার->info[skippy_iter.idx]);

    যদি (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      যদি (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	যদি (ligbase == LIGBASE_NOT_CHECKED)
	{
	  যৌক্তিক found = মিথ্যা;
	  ধ্রুবক auto *out = বাফার->out_info;
	  unsigned পূর্ণসংখ্যা j = বাফার->out_len;
	  যখন (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    যদি (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = সত্য;
	      বিরতি;
	    }
	    j--;
	  }

	  যদি (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  অন্যথা
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	যদি (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (মিথ্যা);
      }
    }
    অন্যথা
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      যদি (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (মিথ্যা);
    }

    যোগ_component_গুনতি += hb_glyph_info_get_lig_অঙ্ক_comps_in_ligation (&বাফার->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  যদি (p_যোগ_component_গুনতি)
  {
    যোগ_component_গুনতি += hb_glyph_info_get_lig_অঙ্ক_comps (&বাফার->cur());
    *p_যোগ_component_গুনতি = যোগ_component_গুনতি;
  }

  c->match_positions.arrayZ[0] = বাফার->idx;

  return_trace (সত্য);
}
স্থির অন্তঃরেখ যৌক্তিক ligate_প্রবেশ (hb_ot_apply_context_t *c,
				 unsigned পূর্ণসংখ্যা গুনতি, /* Including the first glyph */
				 unsigned পূর্ণসংখ্যা match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned পূর্ণসংখ্যা যোগ_component_গুনতি)
{
  TRACE_APPLY (nullptr);

  hb_বাফার_t *বাফার = c->বাফার;

  বাফার->merge_clusters (বাফার->idx, match_end);

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

  যৌক্তিক is_base_ligature = _hb_glyph_info_is_base_glyph (&বাফার->info[c->match_positions.arrayZ[0]]);
  যৌক্তিক is_mark_ligature = _hb_glyph_info_is_mark (&বাফার->info[c->match_positions.arrayZ[0]]);
  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
    যদি (!_hb_glyph_info_is_mark (&বাফার->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = মিথ্যা;
      is_mark_ligature = মিথ্যা;
      বিরতি;
    }
  যৌক্তিক is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned পূর্ণসংখ্যা klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned পূর্ণসংখ্যা lig_id = is_ligature ? _hb_allocate_lig_id (বাফার) : 0;
  unsigned পূর্ণসংখ্যা last_lig_id = _hb_glyph_info_get_lig_id (&বাফার->cur());
  unsigned পূর্ণসংখ্যা last_অঙ্ক_components = hb_glyph_info_get_lig_অঙ্ক_comps (&বাফার->cur());
  unsigned পূর্ণসংখ্যা components_so_far = last_অঙ্ক_components;

  যদি (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&বাফার->cur(), lig_id, যোগ_component_গুনতি);
    যদি (_hb_glyph_info_get_general_category (&বাফার->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&বাফার->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
  {
    যখন (বাফার->idx < c->match_positions.arrayZ[i] && বাফার->successful)
    {
      যদি (is_ligature)
      {
	unsigned পূর্ণসংখ্যা this_comp = _hb_glyph_info_get_lig_comp (&বাফার->cur());
	যদি (this_comp == 0)
	  this_comp = last_অঙ্ক_components;
	প্রতিজ্ঞা (components_so_far >= last_অঙ্ক_components);
	unsigned পূর্ণসংখ্যা new_lig_comp = components_so_far - last_অঙ্ক_components +
				    hb_min (this_comp, last_অঙ্ক_components);
	  _hb_glyph_info_set_lig_props_for_mark (&বাফার->cur(), lig_id, new_lig_comp);
      }
      (শূন্য) বাফার->পরবর্তী_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&বাফার->cur());
    last_অঙ্ক_components = hb_glyph_info_get_lig_অঙ্ক_comps_in_ligation (&বাফার->cur());
    components_so_far += last_অঙ্ক_components;

    /* Skip the base glyph */
    বাফার->idx++;
  }

  যদি (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    জন্য (unsigned i = বাফার->idx; i < বাফার->len; ++i)
    {
      যদি (last_lig_id != _hb_glyph_info_get_lig_id (&বাফার->info[i])) বিরতি;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&বাফার->info[i]);
      যদি (!this_comp) বিরতি;

      প্রতিজ্ঞা (components_so_far >= last_অঙ্ক_components);
      unsigned new_lig_comp = components_so_far - last_অঙ্ক_components +
			      hb_min (this_comp, last_অঙ্ক_components);
      _hb_glyph_info_set_lig_props_for_mark (&বাফার->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (সত্য);
}

#ifndef HB_OPTIMIZE_আকার
HB_ALWAYS_INLINE
#অন্যথা
অন্তঃরেখ
#endif
স্থির যৌক্তিক match_backtrack (hb_ot_apply_context_t *c,
			     unsigned পূর্ণসংখ্যা গুনতি,
			     glyph_তথ্য_t backtrack,
			     match_func_t match_func,
			     ধ্রুবক শূন্য *match_তথ্য_2,
			     unsigned পূর্ণসংখ্যা *match_start)
{
  TRACE_APPLY (nullptr);

  যদি (!গুনতি)
  {
    *match_start = c->বাফার->backtrack_len ();
    return_trace (সত্য);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->বাফার->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_তথ্য_2);
  skippy_iter.set_glyph_তথ্য (backtrack);

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
  {
    unsigned unsafe_from;
    যদি (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (মিথ্যা);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (সত্য);
}

#ifndef HB_OPTIMIZE_আকার
HB_ALWAYS_INLINE
#অন্যথা
অন্তঃরেখ
#endif
স্থির যৌক্তিক match_lookahead (hb_ot_apply_context_t *c,
			     unsigned পূর্ণসংখ্যা গুনতি,
			     glyph_তথ্য_t lookahead,
			     match_func_t match_func,
			     ধ্রুবক শূন্য *match_তথ্য_2,
			     unsigned পূর্ণসংখ্যা start_সূচক,
			     unsigned পূর্ণসংখ্যা *end_সূচক)
{
  TRACE_APPLY (nullptr);

  যদি (!গুনতি)
  {
    *end_সূচক = start_সূচক;
    return_trace (সত্য);
  }

  auto &skippy_iter = c->iter_context;
  প্রতিজ্ঞা (start_সূচক >= 1);
  skippy_iter.reset (start_সূচক - 1);
  skippy_iter.set_match_func (match_func, match_তথ্য_2);
  skippy_iter.set_glyph_তথ্য (lookahead);

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
  {
    unsigned unsafe_to;
    যদি (!skippy_iter.next (&unsafe_to))
    {
      *end_সূচক = unsafe_to;
      return_trace (মিথ্যা);
    }
  }

  *end_সূচক = skippy_iter.idx + 1;
  return_trace (সত্য);
}



কাঠামো LookupRecord
{
  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক hb_map_t         *lookup_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->embed (*এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    return_trace (c->check_assign (out->lookup_List_সূচক, lookup_map->get (lookup_List_সূচক), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই));
  }

  HBUINT16	sequence_সূচক;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookup_List_সূচক;	/* Lookup to apply to that
					 * position--zero--based */
  সর্বজনীন:
  DEFINE_আকার_STATIC (4);
};

স্থির অন্তঃরেখ unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ধ্রুবক hb_array_t<ধ্রুবক LookupRecord> lookupRecords,
						     ধ্রুবক hb_map_t *lookup_map)
{
  unsigned গুনতি = 0;
  জন্য (ধ্রুবক LookupRecord& r : lookupRecords)
  {
    যদি (!lookup_map->has (r.lookup_List_সূচক))
      চালু;

    যদি (!r.serialize (c, lookup_map))
      ফেরত 0;

    গুনতি++;
  }
  ফেরত গুনতি;
}

গণনা ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

স্থির অন্তঃরেখ ধ্রুবক hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ধ্রুবক শূন্য *তথ্য,
			unsigned value,
			যৌক্তিক filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  নির্বাচন (context_format)
  {
    ক্ষেত্র ContextFormat::SimpleContext:
      ফেরত c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    ক্ষেত্র ContextFormat::ClassBasedContext:
    {
      ধ্রুবক hb_set_t *class_set = c->find_or_create_glyph_set (
        তথ্য, value, hb_depend_context_t::GLYPH_SET_CLASS);
      যদি (unlikely (!class_set))
	ফেরত nullptr;
      যদি (!filter_class_by_parent)
	ফেরত class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      বিরতি;
    }
    ক্ষেত্র ContextFormat::CoverageBasedContext:
      ফেরত c->find_or_create_glyph_set (
        তথ্য, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  যদি (unlikely (scratch->in_error ()))
  {
    c->depend_তথ্য->fail ();
    ফেরত nullptr;
  }
  ফেরত scratch;
}

স্থির অন্তঃরেখ শূন্য context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned প্রবেশ_গুনতি, glyph_তথ্য_t প্রবেশ,
					     unsigned lookup_গুনতি_2,
					     ধ্রুবক LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     ধ্রুবক শূন্য *তথ্য,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     শূন্য *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < lookup_গুনতি_2; i++)
  {
    unsigned seq_সূচক_2 = lookupRecord[i].sequence_সূচক;
    যদি (seq_সূচক_2 >= প্রবেশ_গুনতি) চালু;

    যৌক্তিক has_pos_glyphs = মিথ্যা;

    যদি (!covered_seq_indicies.has (seq_সূচক_2))
    {
      has_pos_glyphs = সত্য;
      pos_glyphs.clear ();
      যদি (seq_সূচক_2 == 0)
      {
        নির্বাচন (context_format) {
        ক্ষেত্র ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          বিরতি;
        ক্ষেত্র ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), তথ্য, value, &pos_glyphs, cache);
          বিরতি;
        ক্ষেত্র ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          বিরতি;
        }
      }
      অন্যথা
      {
        ধ্রুবক শূন্য *প্রবেশ_তথ্য = প্রবেশ.তথ্য;
        unsigned প্রবেশ_মান = seq_সূচক_2 - 1;
        যদি (context_format != ContextFormat::SimpleContext)
        {
          প্রবেশ_তথ্য = তথ্য;
          প্রবেশ_মান = প্রবেশ[seq_সূচক_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, প্রবেশ_তথ্য, প্রবেশ_মান, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seq_সূচক_2);
    hb_set_t *cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs))
      ফেরত;
    যদি (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } অন্যথা {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned end_সূচক_2 = প্রবেশ_গুনতি;
    যদি (context_format == ContextFormat::CoverageBasedContext)
      end_সূচক_2 += 1;

    c->recurse (lookupRecord[i].lookup_List_সূচক, &covered_seq_indicies, seq_সূচক_2, end_সূচক_2);

    c->বের_করো_cur_done_glyphs ();
  }
}

স্থির যৌক্তিক
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned প্রবেশ_গুনতি_2,
				unsigned lookup_গুনতি,
				ধ্রুবক LookupRecord lookup_records[],
				ধ্রুবক শূন্য *rule,
				unsigned value,
				ContextFormat context_format,
				ধ্রুবক শূন্য *তথ্য,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  যৌক্তিক have_active_idx = মিথ্যা;
  জন্য (unsigned i = 0; i < lookup_গুনতি; i++)
  {
    যদি (lookup_records[i].sequence_সূচক >= প্রবেশ_গুনতি_2)
      চালু;

    যদি (!have_active_idx)
    {
      যদি (unlikely (!c->get_parent_active_glyphs_সূচক (&active_idx)))
	ফেরত মিথ্যা;
      have_active_idx = সত্য;
    }

    hb_depend_context_t::context_set_cache_চাবি_t key (
      rule, &lookup_records[i], তথ্য, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    যদি (!c->context_set_cache.has (key, &cached_context_set))
      ফেরত মিথ্যা;
    যদি (unlikely (!cached_context_sets->ঠেলো_or_fail (*cached_context_set)))
      ফেরত c->depend_তথ্য->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  ফেরত have_active_idx;
}

স্থির অন্তঃরেখ শূন্য context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned প্রবেশ_গুনতি, glyph_তথ্য_t প্রবেশ,
					     unsigned lookup_গুনতি_2,
					     ধ্রুবক LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     ধ্রুবক শূন্য *rule,
					     unsigned value,
					     ContextFormat context_format,
					     ধ্রুবক শূন্য *তথ্য,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     শূন্য *cache,
					     ধ্রুবক hb_set_t &preliminary_context,
					     ধ্রুবক hb_vector_t<ধ্রুবক hb_set_t *> *প্রবেশ_position_glyphs,
					     ধ্রুবক hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  যদি (unlikely (প্রবেশ_গুনতি > HB_MAX_CONTEXT_দৈর্ঘ্য))
    ফেরত;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_সূচক = 0;
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < lookup_গুনতি_2; i++)
  {
    unsigned seq_সূচক_2 = lookupRecord[i].sequence_সূচক;
    যদি (seq_সূচক_2 >= প্রবেশ_গুনতি)
      চালু;

    hb_depend_context_t::context_set_cache_চাবি_t context_cache_চাবি;
    যদি (unlikely (!c->get_context_set_cache_চাবি (rule, &lookupRecord[i], তথ্য, value,
						 context_format, &context_cache_চাবি)))
      ফেরত;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    যদি (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_সূচক++];
    অন্যথা যদি (c->context_set_cache.has (context_cache_চাবি, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    অন্যথা
    {
      যদি (unlikely (!প্রবেশ_position_glyphs))
      {
	c->depend_তথ্য->fail ();
	ফেরত;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_মান_INVALID;
      যখন (preliminary_context.next (&elem)) {
	যদি (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} অন্যথা {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      জন্য (unsigned j = 0; j < প্রবেশ_position_glyphs->দৈর্ঘ্য; j++) {
	যদি (j != seq_সূচক_2 && (*প্রবেশ_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*প্রবেশ_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_মান_INVALID;
      যখন (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	ধ্রুবক hb_set_t *original_set = c->depend_তথ্য->get_set_from_সূচক (set_idx);
	যদি (unlikely (!original_set)) চালু;

	/* Dependency context sets are never inverted. */
	যদি (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      জন্য (unsigned j = 0; j < প্রবেশ_position_glyphs->দৈর্ঘ্য; j++) {
	যদি (j != seq_সূচক_2 && (*প্রবেশ_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  যদি (!(*প্রবেশ_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    যৌক্তিক cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*প্রবেশ_position_glyphs)[j])
	      : c->depend_তথ্য->find_or_create_context_set (*(*প্রবেশ_position_glyphs)[j]);
	    যদি (unlikely (idx == HB_CODEPOINT_INVALID))
	      ফেরত;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      যদি (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_তথ্য->fail ();
	ফেরত;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_তথ্য->find_or_create_context_set (position_context);
      যদি (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	ফেরত;

      যদি (unlikely (!c->context_set_cache.set (context_cache_চাবি, context_set_idx)))
      {
	c->depend_তথ্য->fail ();
	ফেরত;
      }
    }

    /* Save outer context to restore after this lookup. When a contextual lookup
     * calls another contextual lookup, we want each lookup in this rule to see
     * the same outer context, not context from a sibling lookup. */
    hb_codepoint_t saved_context_set_সূচক = c->depend_তথ্য->current_context_set_সূচক;
    hb_subset_depend_edge_flags_t saved_edge_flags = c->depend_তথ্য->current_edge_flags;

    /* Detect nested contextual: if we already have a context set, we're inside
     * an outer contextual rule that called this one. The outer context will be
     * lost, so edges created here may over-approximate. */
    যৌক্তিক nested_contextual = (saved_context_set_সূচক != HB_CODEPOINT_INVALID);

    /* Set for this position's edges */
    c->depend_তথ্য->current_context_set_সূচক = context_set_idx;

    /* Mark edges from multi-position rules (inputCount > 1) as potentially
     * over-approximate. Depend extraction records edges based on what glyphs
     * COULD be at each position per the static input coverage/class. But during
     * closure computation, lookups within the rule are applied sequentially:
     * lookups at earlier positions may transform later positions, and multiple
     * lookups at the same position may interact (one produces a glyph, another
     * immediately consumes it as an "intermediate"). So a glyph matching the
     * coverage might not persist at that position when closure traverses. */
    c->depend_তথ্য->current_edge_flags = HB_SUBSET_DEPEND_EDGE_FLAG_NONE;
    যদি (প্রবেশ_গুনতি > 1) {
      c->depend_তথ্য->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    যদি (nested_contextual) {
      c->depend_তথ্য->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    যৌক্তিক has_pos_glyphs = মিথ্যা;

    যদি (!covered_seq_indices.has (seq_সূচক_2))
    {
      has_pos_glyphs = সত্য;
      pos_glyphs.clear ();
      যদি (seq_সূচক_2 == 0)
      {
        নির্বাচন (context_format) {
        ক্ষেত্র ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          বিরতি;
        ক্ষেত্র ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), তথ্য, value, &pos_glyphs, cache);
          বিরতি;
        ক্ষেত্র ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          বিরতি;
        }
      }
      অন্যথা
      {
        ধ্রুবক শূন্য *প্রবেশ_তথ্য = প্রবেশ.তথ্য;
        unsigned প্রবেশ_মান = seq_সূচক_2 - 1;
        যদি (context_format != ContextFormat::SimpleContext)
        {
          প্রবেশ_তথ্য = তথ্য;
          প্রবেশ_মান = প্রবেশ[seq_সূচক_2 - 1];
        }

        intersected_glyphs_func (c->glyphs, প্রবেশ_তথ্য, প্রবেশ_মান, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seq_সূচক_2);
    যদি (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      যদি (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seq_সূচক_2 ? pos_glyphs.get_min () : value;
	যদি (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->ঠেলো_cur_active_glyphs (active_idx);
      যদি (unlikely (!cur_active_glyphs))
      {
	c->depend_তথ্য->current_context_set_সূচক = saved_context_set_সূচক;
	c->depend_তথ্য->current_edge_flags = saved_edge_flags;
	ফেরত;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    অন্যথা যদি (unlikely (!c->ঠেলো_মূল_active_glyphs ()))
    {
      c->depend_তথ্য->current_context_set_সূচক = saved_context_set_সূচক;
      c->depend_তথ্য->current_edge_flags = saved_edge_flags;
      ফেরত;
    }

    unsigned end_সূচক_2 = প্রবেশ_গুনতি;
    যদি (context_format == ContextFormat::CoverageBasedContext)
      end_সূচক_2 += 1;

    c->recurse (lookupRecord[i].lookup_List_সূচক, &covered_seq_indices, seq_সূচক_2, end_সূচক_2);
    c->বের_করো_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_তথ্য->current_context_set_সূচক = saved_context_set_সূচক;
    c->depend_তথ্য->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
স্থির অন্তঃরেখ শূন্য recurse_lookups (context_t *c,
                                    unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
                                    ধ্রুবক LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < lookup_গুনতি_2; i++)
    c->recurse (lookupRecord[i].lookup_List_সূচক);
}

স্থির অন্তঃরেখ শূন্য apply_lookup (hb_ot_apply_context_t *c,
				 unsigned পূর্ণসংখ্যা গুনতি, /* Including the first glyph */
				 unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
				 ধ্রুবক LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned পূর্ণসংখ্যা match_end)
{
  hb_বাফার_t *বাফার = c->বাফার;
  পূর্ণসংখ্যা end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned পূর্ণসংখ্যা bl = বাফার->backtrack_len ();
    end = bl + match_end - বাফার->idx;

    পূর্ণসংখ্যা delta = bl - বাফার->idx;
    /* Convert positions to new indexing. */
    জন্য (unsigned পূর্ণসংখ্যা j = 0; j < গুনতি; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  জন্য (unsigned পূর্ণসংখ্যা i = 0; i < lookup_গুনতি_2 && বাফার->successful; i++)
  {
    unsigned পূর্ণসংখ্যা idx = lookupRecord[i].sequence_সূচক;
    যদি (idx >= গুনতি)
      চালু;

    unsigned পূর্ণসংখ্যা orig_len = বাফার->backtrack_len () + বাফার->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    যদি (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      চালু;

    যদি (unlikely (!বাফার->move_to (c->match_positions.arrayZ[idx])))
      বিরতি;

    যদি (unlikely (বাফার->max_ops <= 0))
      বিরতি;

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      যদি (বাফার->have_output)
        c->বাফার->sync_so_far ();
      c->বাফার->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookup_List_সূচক,
			  বাফার->idx);
    }

    যৌক্তিক ret = c->recurse (lookupRecord[i].lookup_List_সূচক);

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      যদি (বাফার->have_output)
        c->বাফার->sync_so_far ();
      c->বাফার->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookup_List_সূচক);
    }

    যদি (!ret)
      চালু;

    unsigned পূর্ণসংখ্যা new_len = বাফার->backtrack_len () + বাফার->lookahead_len ();
    পূর্ণসংখ্যা delta = new_len - orig_len;

    যদি (!delta)
      চালু;

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
    যদি (end < পূর্ণসংখ্যা (c->match_positions.arrayZ[idx]))
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

    unsigned পূর্ণসংখ্যা next = idx + 1; /* next now is the position after the recursed lookup. */

    যদি (delta > 0)
    {
      যদি (unlikely (delta + গুনতি > HB_MAX_CONTEXT_দৈর্ঘ্য))
	বিরতি;
      যদি (unlikely (গুনতি + delta > c->match_positions.দৈর্ঘ্য &&
		    !c->match_positions.resize_dirty  (গুনতি + delta)))
        ফেরত;
    }
    অন্যথা
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (পূর্ণসংখ্যা) next - (পূর্ণসংখ্যা) গুনতি);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (গুনতি - next) * আকার (c->match_positions.arrayZ[0]));
    next += delta;
    গুনতি += delta;

    /* Fill in new entries. */
    জন্য (unsigned পূর্ণসংখ্যা j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    জন্য (; next < গুনতি; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  প্রতিজ্ঞা (end >= 0);
  (শূন্য) বাফার->move_to (end);
}



/* Contextual lookups */

কাঠামো ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ধ্রুবক শূন্য *intersects_তথ্য;
  শূন্য *intersects_cache;
  শূন্য *intersected_glyphs_cache;
};

প্রকারলেখ ContextClosureLookupContext ContextDependLookupContext;

কাঠামো ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ধ্রুবক শূন্য *collect_তথ্য;
};

কাঠামো ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ধ্রুবক শূন্য *match_তথ্য_2;
  get_মান_func_t get_মান;
};

স্থির অন্তঃরেখ যৌক্তিক context_intersects (ধ্রুবক hb_set_t *glyphs,
				       unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
				       glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  ফেরত array_is_subset_of (glyphs,
			     প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0, প্রবেশ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_তথ্য,
			     lookup_context.intersects_cache);
}

স্থির অন্তঃরেখ শূন্য context_closure_lookup (hb_closure_context_t *c,
					   unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
					   glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
					   unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
					   ধ্রুবক LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  যদি (context_intersects (c->glyphs,
			  প্রবেশ_গুনতি, প্রবেশ,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     প্রবেশ_গুনতি, প্রবেশ,
				     lookup_গুনতি_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_তথ্য,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

স্থির অন্তঃরেখ শূন্য context_depend_lookup (hb_depend_context_t *c,
					  unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
					  glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
					  unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
					  ধ্রুবক LookupRecord lookupRecord[],
					  ধ্রুবক শূন্য *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  যদি (!context_intersects (c->glyphs,
			   প্রবেশ_গুনতি, প্রবেশ,
			   lookup_context))
    ফেরত;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  যদি (unlikely (!scratch))
    ফেরত;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  যৌক্তিক context_sets_cached = context_depend_sets_are_cached (
    c, প্রবেশ_গুনতি, lookup_গুনতি_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_তথ্য,
    &scratch->cached_context_sets);
  যদি (unlikely (!c->depend_তথ্য->successful))
    ফেরত;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ধ্রুবক hb_set_t *> &প্রবেশ_position_glyphs = scratch->প্রবেশ_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  যদি (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ধ্রুবক hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_তথ্য, value,
      সত্য, &position_scratch, &scratch->compact_workspace);
    যদি (unlikely (!pos0_glyphs ||
		  !প্রবেশ_position_glyphs.ঠেলো_or_fail (pos0_glyphs)))
    {
      c->depend_তথ্য->fail ();
      ফেরত;
    }

    /* Positions 1+ (Input array) */
    জন্য (unsigned i = 0; i + 1 < প্রবেশ_গুনতি; i++)
    {
      ধ্রুবক hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_তথ্য,
	প্রবেশ[i], মিথ্যা, &position_scratch);
      যদি (unlikely (!pos_glyphs ||
		    !প্রবেশ_position_glyphs.ঠেলো_or_fail (pos_glyphs)))
      {
	c->depend_তথ্য->fail ();
	ফেরত;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  প্রবেশ_গুনতি, প্রবেশ,
				  lookup_গুনতি_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_তথ্য,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &প্রবেশ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

স্থির অন্তঃরেখ শূন্য context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
						  glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
						  unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
						  ধ্রুবক LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->প্রবেশ,
		 প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0, প্রবেশ,
		 lookup_context.funcs.collect, lookup_context.collect_তথ্য);
  recurse_lookups (c,
		   lookup_গুনতি_2, lookupRecord);
}

স্থির অন্তঃরেখ যৌক্তিক context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
					       glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
					       unsigned পূর্ণসংখ্যা lookup_গুনতি_2 HB_UNUSED,
					       ধ্রুবক LookupRecord lookupRecord[] HB_UNUSED,
					       ধ্রুবক ContextApplyLookupContext &lookup_context)
{
  ফেরত would_match_প্রবেশ (c,
			    প্রবেশ_গুনতি, প্রবেশ,
			    lookup_context.funcs.match, lookup_context.match_তথ্য_2);
}

স্থির অন্তঃরেখ যৌক্তিক context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
					 glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
					 unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
					 ধ্রুবক LookupRecord lookupRecord[],
					 ধ্রুবক ContextApplyLookupContext &lookup_context)
{
  যদি (unlikely (প্রবেশ_গুনতি > HB_MAX_CONTEXT_দৈর্ঘ্য)) ফেরত মিথ্যা;

  unsigned match_end = 0;
  যৌক্তিক ret = মিথ্যা;
  যদি (match_প্রবেশ (c,
		   প্রবেশ_গুনতি, প্রবেশ,
		   lookup_context.funcs.match, lookup_context.match_তথ্য_2,
		   &match_end))
  {
    c->বাফার->unsafe_to_break (c->বাফার->idx, match_end);
    apply_lookup (c,
		  প্রবেশ_গুনতি,
		  lookup_গুনতি_2, lookupRecord,
		  match_end);
    ret = সত্য;
  }
  অন্যথা
  {
    c->বাফার->unsafe_to_concat (c->বাফার->idx, match_end);
    ret = মিথ্যা;
  }

  ফেরত ret;
}

স্থির অন্তঃরেখ যৌক্তিক context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  নির্বাচন (op)
  {
    ক্ষেত্র hb_ot_subtable_cache_op_t::ENTER:
    {
      যদি (!HB_বাফার_TRY_ALLOCATE_VAR (c->বাফার, syllable))
	ফেরত মিথ্যা;
      auto &info = c->বাফার->info;
      unsigned গুনতি = c->বাফার->len;
      জন্য (unsigned i = 0; i < গুনতি; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      ফেরত সত্য;
    }
    ক্ষেত্র hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_বাফার_DEALLOCATE_VAR (c->বাফার, syllable);
      বিরতি;
    }
  }
  ফেরত মিথ্যা;
}

template <typename Types>
কাঠামো Rule
{
  template <typename SetTypes, typename RuleType>
  friend কাঠামো RuleSetOf;

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    ফেরত context_intersects (glyphs,
			       প্রবেশ_গুনতি, প্রবেশ_Z.arrayZ,
			       lookup_context);
  }

  শূন্য depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ধ্রুবক
  {
    ধ্রুবক auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্রবেশ_Z.as_array ((প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0)));
    context_depend_lookup (c,
			   প্রবেশ_গুনতি, প্রবেশ_Z.arrayZ,
			   lookup_গুনতি_2, lookupRecord.arrayZ,
			   এই,
			   value, lookup_context);
  }

  শূন্য closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ফেরত;

    ধ্রুবক auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্রবেশ_Z.as_array ((প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0)));
    context_closure_lookup (c,
			    প্রবেশ_গুনতি, প্রবেশ_Z.arrayZ,
			    lookup_গুনতি_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ফেরত;
    যদি (!intersects (c->glyphs, lookup_context)) ফেরত;

    ধ্রুবক auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্রবেশ_Z.as_array (প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0));
    recurse_lookups (c, lookup_গুনতি_2, lookupRecord.arrayZ);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ধ্রুবক
  {
    ধ্রুবক auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্রবেশ_Z.as_array (প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0));
    context_collect_glyphs_lookup (c,
				   প্রবেশ_গুনতি, প্রবেশ_Z.arrayZ,
				   lookup_গুনতি_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest,
			      ধ্রুবক ClassDef *class_def = nullptr) ধ্রুবক
  {
    যদি (প্রবেশ_গুনতি <= 1)
    {
      *digest = hb_set_digest_t::full ();
      ফেরত;
    }

    unsigned second = প্রবেশ_Z.arrayZ[0];
    যদি (!class_def)
      digest->add (second);
    অন্যথা যদি (!second)
      *digest = hb_set_digest_t::full ();
    অন্যথা
      class_def->collect_class (digest, second);
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c,
		    ধ্রুবক ContextApplyLookupContext &lookup_context) ধ্রুবক
  {
    ধ্রুবক auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্রবেশ_Z.as_array (প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0));
    ফেরত context_would_apply_lookup (c,
				       প্রবেশ_গুনতি, প্রবেশ_Z.arrayZ,
				       lookup_গুনতি_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c,
	      ধ্রুবক ContextApplyLookupContext &lookup_context) ধ্রুবক
  {
    TRACE_APPLY (এই);
    ধ্রুবক auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্রবেশ_Z.as_array (প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0));
    return_trace (context_apply_lookup (c, প্রবেশ_গুনতি, প্রবেশ_Z.arrayZ, lookup_গুনতি_2, lookupRecord.arrayZ, lookup_context));
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক hb_map_t *প্রবেশ_mapping, /* old->new glyphid or class mapping */
		  ধ্রুবক hb_map_t *lookup_map) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->start_embed (এই);
    যদি (unlikely (!c->extend_min (out))) return_trace (মিথ্যা);

    out->প্রবেশ_গুনতি = প্রবেশ_গুনতি;
    ধ্রুবক auto প্রবেশ = প্রবেশ_Z.as_array (প্রবেশ_গুনতি - 1);
    জন্য (ধ্রুবক auto org : প্রবেশ)
    {
      typename Types::HBUINT d;
      d = প্রবেশ_mapping->get (org);
      c->copy (d);
    }

    ধ্রুবক auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্রবেশ_Z.as_array ((প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0)));

    unsigned গুনতি = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_গুনতি_2), lookup_map);
    return_trace (c->check_assign (out->lookup_গুনতি_2, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       ধ্রুবক hb_map_t *lookup_map,
	       ধ্রুবক hb_map_t *klass_map = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (unlikely (!প্রবেশ_গুনতি)) return_trace (মিথ্যা);
    ধ্রুবক auto প্রবেশ = প্রবেশ_Z.as_array (প্রবেশ_গুনতি - 1);

    ধ্রুবক hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    যদি (!hb_all (প্রবেশ, mapping)) return_trace (মিথ্যা);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  সর্বজনীন:
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  c->check_range (প্রবেশ_Z.arrayZ,
				  প্রবেশ_Z.item_আকার * (প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0) +
				  LookupRecord::static_আকার * lookup_গুনতি_2));
  }

  সুরক্ষিত:
  HBUINT16	প্রবেশ_গুনতি;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_গুনতি_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		প্রবেশ_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, প্রবেশ_Z);
};

template <typename Types, typename RuleType>
কাঠামো RuleSetOf
{
  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    ফেরত
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_map ([&] (ধ্রুবক RuleType &_) { ফেরত _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  শূন্য collect_first_প্রবেশ_classes (hb_ot_layout_ruleset_digest_t *digest) ধ্রুবক
  {
    digest->init ();
    জন্য (unsigned i = 0; i < rule.len; i++)
    {
      ধ্রুবক RuleType &r = এই+rule.arrayZ[i];
      যদি (r.প্রবেশ_গুনতি <= 1)
      {
	digest->set_full ();
	ফেরত;
      }
      digest->add (r.প্রবেশ_Z.arrayZ[0]);
    }
  }

  শূন্য depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ধ্রুবক
  {
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  শূন্য closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ফেরত;

    ফেরত
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ফেরত;
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ধ্রুবক
  {
    ফেরত
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest,
			      ধ্রুবক ClassDef *class_def = nullptr) ধ্রুবক
  {
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([=] (ধ্রুবক RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c,
		    ধ্রুবক ContextApplyLookupContext &lookup_context) ধ্রুবক
  {
    ফেরত
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_map ([&] (ধ্রুবক RuleType &_) { ফেরত _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c,
	      ধ্রুবক ContextApplyLookupContext &lookup_context,
	      ধ্রুবক hb_ot_layout_ruleset_digest_t *digest = nullptr) ধ্রুবক
  {
    TRACE_APPLY (এই);

    unsigned অঙ্ক_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    যদি (HB_OPTIMIZE_আকার_VAL || অঙ্ক_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (এই))
      | hb_map ([&] (ধ্রুবক RuleType &_) { ফেরত _.apply (c, lookup_context); })
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
    skippy_iter.reset (c->বাফার->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_তথ্য ((HBUINT16 *) nullptr);
    unsigned unsafe_to = (unsigned) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *first = nullptr, *second = nullptr;
    যৌক্তিক matched = skippy_iter.next (&unsafe_to1);
    যদি (likely (matched))
    {
      যদি (skippy_iter.may_skip (c->বাফার->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        যাও slow;
      }

      first = &c->বাফার->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      যদি (digest && lookup_context.get_মান &&
	  !digest->may_have (lookup_context.get_মান (*first, lookup_context.match_তথ্য_2)))
	{
	  c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to1);
	  return_trace (মিথ্যা);
	}
    }
    অন্যথা
    {
      যৌক্তিক unsafe_to_concat = মিথ্যা;
      যৌক্তিক ret =
      + hb_iter (rule)
      | hb_map (hb_add (এই))
      | hb_filter ([&] (ধ্রুবক RuleType &_)
		   {
		     যদি (_.প্রবেশ_গুনতি <= 1) ফেরত সত্য;
		     unsafe_to_concat = সত্য;
		     ফেরত মিথ্যা;
		   })
      | hb_map ([&] (ধ্রুবক RuleType &_) { ফেরত _.apply (c, lookup_context); })
      | hb_any
      ;
      যদি (unsafe_to_concat)
	c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    যদি (likely (matched))
    {
      যদি (skippy_iter.may_skip (c->বাফার->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        যাও slow;
      }

      second = &c->বাফার->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_প্রবেশ = lookup_context.funcs.match;
    auto *প্রবেশ_তথ্য = lookup_context.match_তথ্য_2;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_rules; i++)
    {
      ধ্রুবক auto &r = এই+rule.arrayZ[i];

      ধ্রুবক auto &প্রবেশ = r.প্রবেশ_Z;

      যদি (r.প্রবেশ_গুনতি <= 1 ||
	  (!match_প্রবেশ ||
	   match_প্রবেশ (*first, প্রবেশ.arrayZ[0], প্রবেশ_তথ্য)))
      {
        যদি (!second ||
	    (r.প্রবেশ_গুনতি <= 2 ||
	     (!match_প্রবেশ ||
	      match_প্রবেশ (*second, প্রবেশ.arrayZ[1], প্রবেশ_তথ্য)))
	   )
	{
	  যদি (r.apply (c, lookup_context))
	  {
	    যদি (unsafe_to != (unsigned) -1)
	      c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to);
	    return_trace (সত্য);
	  }
	}
	অন্যথা
	  unsafe_to = unsafe_to2;
      }
      অন্যথা
      {
	যদি (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	জন্য (; i + 1 < অঙ্ক_rules; i++)
	{
	  ধ্রুবক auto &r2 = এই+rule.arrayZ[i + 1];
	  ধ্রুবক auto &input2 = r2.প্রবেশ_Z;
	  যদি (r2.প্রবেশ_গুনতি <= 1 || input2.arrayZ[0] != প্রবেশ.arrayZ[0])
	    বিরতি;
	}
      }
    }
    যদি (likely (unsafe_to != (unsigned) -1))
      c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to);

    return_trace (মিথ্যা);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       ধ্রুবক hb_map_t *lookup_map,
	       ধ্রুবক hb_map_t *klass_map = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    জন্য (ধ্রুবক auto& _ : rule)
    {
      যদি (!_) চালু;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      যদি (unlikely (!o)) চালু;

      যদি (!o->serialize_subset (c, _, এই, lookup_map, klass_map))
      {
	out->rule.বের_করো ();
	c->serializer->revert (o_snap);
      }
    }

    যৌক্তিক ret = যৌক্তিক (out->rule);
    যদি (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (rule.sanitize (c, এই));
  }

  সুরক্ষিত:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, rule);
};

template <typename Types>
কাঠামো RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
কাঠামো ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
কাঠামো ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    কাঠামো ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    ফেরত
    + hb_zip (এই+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_map ([&] (ধ্রুবক RuleSet &_) { ফেরত _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত সত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (এই+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ফেরত c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ধ্রুবক hb_pair_t<hb_codepoint_t, unsigned> _) { ফেরত hb_pair_t<unsigned, ধ্রুবক RuleSet&> (_.first, এই+ruleSet[_.second]); })
    | hb_apply ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    কাঠামো ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (এই+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ফেরত c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ধ্রুবক hb_pair_t<hb_codepoint_t, unsigned> _) { ফেরত hb_pair_t<unsigned, ধ্রুবক RuleSet&> (_.first, এই+ruleSet[_.second]); })
    | hb_apply ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক
  {
    কাঠামো ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (এই+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    (এই+coverage).collect_coverage (c->প্রবেশ);

    কাঠামো ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_apply ([digest] (ধ্রুবক RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    ধ্রুবক RuleSet &rule_set = এই+ruleSet[(এই+coverage).get_coverage (c->glyphs[0])];
    কাঠামো ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    ফেরত rule_set.would_apply (c, lookup_context);
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (c->বাফার->cur().codepoint);
    যদি (likely (সূচক_2 == NOT_COVERED))
      return_trace (মিথ্যা);

    ধ্রুবক RuleSet &rule_set = এই+ruleSet[সূচক_2];
    কাঠামো ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    ধ্রুবক hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এই+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, এই, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (যৌক্তিক (new_coverage));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) && ruleSet.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + Types::HBUINT::static_আকার, ruleSet);
};


template <typename Types>
কাঠামো ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (glyphs))
      ফেরত মিথ্যা;

    ধ্রুবক ClassDef &class_def = এই+classDef;

    hb_map_t cache;
    কাঠামো ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (এই+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    ফেরত
    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_enumerate
    | hb_map ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক RuleSet &> p)
	      { ফেরত class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত সত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (c->glyphs))
      ফেরত;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ধ্রুবক ClassDef &class_def = এই+classDef;

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
    { ফেরত class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ধ্রুবক RuleSet& rule_set = এই+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (c->glyphs))
      ফেরত;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ধ্রুবক ClassDef &class_def = এই+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    কাঠামো ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ফেরত class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ধ্রুবক RuleSet& rule_set = এই+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (c->glyphs))
      ফেরত;

    ধ্রুবক ClassDef &class_def = এই+classDef;

    hb_map_t cache;
    কাঠামো ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_enumerate
    | hb_filter ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক RuleSet &> p)
    { ফেরত class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (ধ্রুবক RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    (এই+coverage).collect_coverage (c->প্রবেশ);

    ধ্রুবক ClassDef &class_def = এই+classDef;
    কাঠামো ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  {
    ধ্রুবক ClassDef &class_def = এই+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_apply ([=, &class_def] (ধ্রুবক RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    ধ্রুবক ClassDef &class_def = এই+classDef;
    unsigned পূর্ণসংখ্যা সূচক_2 = class_def.get_class (c->glyphs[0]);
    ধ্রুবক RuleSet &rule_set = এই+ruleSet[সূচক_2];
    কাঠামো ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    ফেরত rule_set.would_apply (c, lookup_context);
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  unsigned cache_cost () ধ্রুবক
  {
    ফেরত (এই+classDef).cost ();
  }
  স্থির যৌক্তিক cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ফেরত context_cache_func (c, op);
  }

  কাঠামো external_cache_t
  {
    hb_ot_layout_দ্বিমিক_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  শূন্য *external_cache_create () ধ্রুবক
  {
    unsigned গুনতি = ruleSet.len;
    unsigned আকার_2 = আকার (external_cache_t) -
		    HB_VAR_ARRAY * আকার (hb_ot_layout_ruleset_digest_t) +
		    গুনতি * আকার (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (আকার_2);
    যদি (likely (cache))
    {
      cache->coverage.clear ();
      জন্য (unsigned i = 0; i < গুনতি; i++)
	(এই+ruleSet[i]).collect_first_প্রবেশ_classes (&cache->rule_sets[i]);
    }
    ফেরত cache;
  }
  যৌক্তিক apply_cached (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুবক { ফেরত _apply (c, সত্য, external_cache); }
  যৌক্তিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুবক { ফেরত _apply (c, মিথ্যা, external_cache); }
  যৌক্তিক _apply (hb_ot_apply_context_t *c, যৌক্তিক cached, শূন্য *external_cache) ধ্রুবক
  {
    TRACE_APPLY (এই);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage_দ্বিমিক (c->বাফার->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (c->বাফার->cur().codepoint);
#endif
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    ধ্রুবক ClassDef &class_def = এই+classDef;

    কাঠামো ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_মান : get_class_মান
    };

    সূচক_2 = cached ? get_class_cached (class_def, c->বাফার->cur()) : class_def.get_class (c->বাফার->cur().codepoint);
    ধ্রুবক RuleSet &rule_set = এই+ruleSet[সূচক_2];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ধ্রুবক hb_ot_layout_ruleset_digest_t *digest = cache && সূচক_2 < ruleSet.len ? &cache->rule_sets[সূচক_2] : nullptr;
#অন্যথা
    ধ্রুবক hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;
    যদি (unlikely (!out->coverage.serialize_subset (c, coverage, এই)))
      return_trace (মিথ্যা);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, এই, &klass_map);

    ধ্রুবক hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (এই+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (এই+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ধ্রুবক hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    যৌক্তিক ret = সত্য;
    পূর্ণসংখ্যা non_zero_সূচক = -1, সূচক_2 = 0;
    auto snapshot = c->serializer->snapshot();
    জন্য (ধ্রুবক auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      যদি (unlikely (!o))
      {
	ret = মিথ্যা;
	বিরতি;
      }

      যদি (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, এই, lookup_map, &klass_map)) {
	non_zero_সূচক = সূচক_2;
        snapshot = c->serializer->snapshot();
      }

      সূচক_2++;
    }

    যদি (!ret || non_zero_সূচক == -1) return_trace (মিথ্যা);

    //prune empty trailing ruleSets
    --সূচক_2;
    যখন (সূচক_2 > non_zero_সূচক)
    {
      out->ruleSet.বের_করো ();
      সূচক_2--;
    }
    c->serializer->revert (snapshot);

    return_trace (যৌক্তিক (out->ruleSet));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) && classDef.sanitize (c, এই) && ruleSet.sanitize (c, এই));
  }

  সুরক্ষিত:
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
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + 2 * Types::LOffset::static_আকার + Types::HBUINT::static_আকার, ruleSet);
};


template <typename Types>
কাঠামো ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    যদি (!(এই+coverageZ[0]).intersects (glyphs))
      ফেরত মিথ্যা;

    কাঠামো ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      এই
    };
    ফেরত context_intersects (glyphs,
			       glyph_গুনতি_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত সত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    যদি (!(এই+coverageZ[0]).intersects (c->glyphs))
      ফেরত;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ধ্রুবক LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      এই,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_গুনতি_2, (ধ্রুবক HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_গুনতি_2, lookupRecord,
			   এই,
			   coverageZ[0], lookup_context);

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    যদি (!(এই+coverageZ[0]).intersects (c->glyphs))
      ফেরত;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ধ্রুবক LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    কাঠামো ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      এই
    };
    context_closure_lookup (c,
			    glyph_গুনতি_2, coverageZ.arrayZ + 1,
			    lookup_গুনতি_2, lookupRecord,
			    0, lookup_context);

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক
  {
    যদি (!intersects (c->glyphs))
      ফেরত;
    ধ্রুবক LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    recurse_lookups (c, lookup_গুনতি_2, lookupRecord);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    (এই+coverageZ[0]).collect_coverage (c->প্রবেশ);

    ধ্রুবক LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    কাঠামো ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      এই
    };

    context_collect_glyphs_lookup (c,
				   glyph_গুনতি_2, coverageZ.arrayZ + 1,
				   lookup_গুনতি_2, lookupRecord,
				   lookup_context);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  {
    যদি (glyph_গুনতি_2 <= 1)
      *digest = hb_set_digest_t::full ();
    অন্যথা
      (এই+coverageZ[1]).collect_coverage (digest);
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    ধ্রুবক LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    কাঠামো ContextApplyLookupContext lookup_context = {
      {match_coverage},
      এই
    };
    ফেরত context_would_apply_lookup (c,
				       glyph_গুনতি_2, coverageZ.arrayZ + 1,
				       lookup_গুনতি_2, lookupRecord,
				       lookup_context);
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverageZ[0]; }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverageZ[0]).get_coverage (c->বাফার->cur().codepoint);
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    ধ্রুবক LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    কাঠামো ContextApplyLookupContext lookup_context = {
      {match_coverage},
      এই
    };
    return_trace (context_apply_lookup (c,
					glyph_গুনতি_2,
					coverageZ.arrayZ + 1,
					lookup_গুনতি_2, lookupRecord,
					lookup_context));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    out->format = format;
    out->glyph_গুনতি_2 = glyph_গুনতি_2;

    auto coverages = coverageZ.as_array (glyph_গুনতি_2);

    জন্য (ধ্রুবক auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_আকার<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_আকার);
      যদি (unlikely (!o)) return_trace (মিথ্যা);
      যদি (!o->serialize_subset (c, offset, এই)) return_trace (মিথ্যা);
    }

    ধ্রুবক auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_গুনতি_2));
    ধ্রুবক hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned গুনতি = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_গুনতি_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_গুনতি_2, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!c->check_struct (এই))) return_trace (মিথ্যা);
    hb_barrier ();
    unsigned পূর্ণসংখ্যা গুনতি = glyph_গুনতি_2;
    যদি (unlikely (!গুনতি)) return_trace (মিথ্যা); /* We want to access coverageZ[0] freely. */
    যদি (unlikely (!c->check_array (coverageZ.arrayZ, গুনতি))) return_trace (মিথ্যা);
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++)
      যদি (unlikely (!coverageZ[i].sanitize (c, এই))) return_trace (মিথ্যা);
    ধ্রুবক LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_গুনতি_2)));
  }

  সুরক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_গুনতি_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_গুনতি_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::template OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (6, coverageZ);
};

কাঠামো Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ক্ষেত্র 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ক্ষেত্র 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    ক্ষেত্র 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;		/* Format identifier */
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

কাঠামো ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ধ্রুবক শূন্য *intersects_তথ্য[3];
  শূন্য *intersects_cache[3];
  শূন্য *intersected_glyphs_cache;
};

প্রকারলেখ ChainContextClosureLookupContext ChainContextDependLookupContext;

কাঠামো ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ধ্রুবক শূন্য *collect_তথ্য[3];
};

কাঠামো ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ধ্রুবক শূন্য *match_তথ্য_2[3];
  get_মান_func_t get_প্রবেশ_মান;
};

স্থির অন্তঃরেখ যৌক্তিক chain_context_intersects (ধ্রুবক hb_set_t *glyphs,
					     unsigned পূর্ণসংখ্যা backtrack_গুনতি,
					     glyph_তথ্য_t backtrack,
					     unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
					     glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
					     unsigned পূর্ণসংখ্যা lookahead_গুনতি,
					     glyph_তথ্য_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  যৌক্তিক bt_ok = array_is_subset_of (glyphs,
			     backtrack_গুনতি, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_তথ্য[0],
			     lookup_context.intersects_cache[0]);
  যৌক্তিক in_ok = array_is_subset_of (glyphs,
			     প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0, প্রবেশ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_তথ্য[1],
			     lookup_context.intersects_cache[1]);
  যৌক্তিক la_ok = array_is_subset_of (glyphs,
			     lookahead_গুনতি, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_তথ্য[2],
			     lookup_context.intersects_cache[2]);

  ফেরত bt_ok && in_ok && la_ok;
}

স্থির অন্তঃরেখ শূন্য chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned পূর্ণসংখ্যা backtrack_গুনতি,
						 glyph_তথ্য_t backtrack,
						 unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
						 glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
						 unsigned পূর্ণসংখ্যা lookahead_গুনতি,
						 glyph_তথ্য_t lookahead,
						 unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
						 ধ্রুবক LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  যদি (chain_context_intersects (c->glyphs,
				backtrack_গুনতি, backtrack,
				প্রবেশ_গুনতি, প্রবেশ,
				lookahead_গুনতি, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     প্রবেশ_গুনতি, প্রবেশ,
		     lookup_গুনতি_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_তথ্য[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

স্থির অন্তঃরেখ শূন্য chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned পূর্ণসংখ্যা backtrack_গুনতি,
						glyph_তথ্য_t backtrack,
						unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
						glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
						unsigned পূর্ণসংখ্যা lookahead_গুনতি,
						glyph_তথ্য_t lookahead,
						unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
						ধ্রুবক LookupRecord lookupRecord[],
						ধ্রুবক শূন্য *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  যদি (!chain_context_intersects (c->glyphs,
				 backtrack_গুনতি, backtrack,
				 প্রবেশ_গুনতি, প্রবেশ,
				 lookahead_গুনতি, lookahead,
				 lookup_context))
    ফেরত;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  যদি (unlikely (!scratch))
    ফেরত;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  যৌক্তিক context_sets_cached = context_depend_sets_are_cached (
    c, প্রবেশ_গুনতি, lookup_গুনতি_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_তথ্য[1],
    &scratch->cached_context_sets);
  যদি (unlikely (!c->depend_তথ্য->successful))
    ফেরত;
  যদি (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    প্রবেশ_গুনতি, প্রবেশ,
				    lookup_গুনতি_2, lookupRecord,
				    rule,
				    value,
				    lookup_context.context_format,
				    lookup_context.intersects_তথ্য[1],
				    lookup_context.funcs.intersected_glyphs,
				    lookup_context.intersected_glyphs_cache,
				    scratch->preliminary_context,
				    nullptr,
				    &scratch->cached_context_sets,
				    scratch);
    ফেরত;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ধ্রুবক hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<ধ্রুবক hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  জন্য (unsigned i = 0; i < backtrack_গুনতি; i++)
  {
    ধ্রুবক hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_তথ্য[0],
      backtrack[i], মিথ্যা, &position_scratch);
    যদি (unlikely (!pos_glyphs))
      ফেরত;
    যদি (!pos_glyphs->is_empty ())
    {
      যদি (unlikely (!backtrack_sets.ঠেলো_or_fail (pos_glyphs)))
      {
        c->depend_তথ্য->fail ();
        ফেরত;
      }
    }
  }

  /* Extract lookahead glyphs */
  জন্য (unsigned i = 0; i < lookahead_গুনতি; i++)
  {
    ধ্রুবক hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_তথ্য[2],
      lookahead[i], মিথ্যা, &position_scratch);
    যদি (unlikely (!pos_glyphs))
      ফেরত;
    যদি (!pos_glyphs->is_empty ())
    {
      যদি (unlikely (!lookahead_sets.ঠেলো_or_fail (pos_glyphs)))
      {
        c->depend_তথ্য->fail ();
        ফেরত;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  জন্য (ধ্রুবক hb_set_t *back_set : backtrack_sets)
  {
    যদি (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } অন্যথা যদি (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_তথ্য->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
        ফেরত;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  জন্য (ধ্রুবক hb_set_t *look_set : lookahead_sets)
  {
    যদি (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } অন্যথা যদি (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_তথ্য->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
        ফেরত;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  যদি (unlikely (preliminary_context.in_error ()))
  {
    c->depend_তথ্য->fail ();
    ফেরত;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<ধ্রুবক hb_set_t *> &প্রবেশ_position_glyphs = scratch->প্রবেশ_position_glyphs;

  যদি (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ধ্রুবক hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_তথ্য[1], value,
      সত্য, &position_scratch, &scratch->compact_workspace);
    যদি (unlikely (!pos0_glyphs ||
		  !প্রবেশ_position_glyphs.ঠেলো_or_fail (pos0_glyphs)))
    {
      c->depend_তথ্য->fail ();
      ফেরত;
    }

    /* Positions 1+ (Input array) */
    জন্য (unsigned i = 0; i + 1 < প্রবেশ_গুনতি; i++)
    {
      ধ্রুবক hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_তথ্য[1],
	প্রবেশ[i], মিথ্যা, &position_scratch);
      যদি (unlikely (!pos_glyphs ||
		    !প্রবেশ_position_glyphs.ঠেলো_or_fail (pos_glyphs)))
      {
	c->depend_তথ্য->fail ();
	ফেরত;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  প্রবেশ_গুনতি, প্রবেশ,
				  lookup_গুনতি_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_তথ্য[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &প্রবেশ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

স্থির অন্তঃরেখ শূন্য chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned পূর্ণসংখ্যা backtrack_গুনতি,
							glyph_তথ্য_t backtrack,
							unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
							glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
							unsigned পূর্ণসংখ্যা lookahead_গুনতি,
							glyph_তথ্য_t lookahead,
							unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
							ধ্রুবক LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_গুনতি, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_তথ্য[0]);
  collect_array (c, c->প্রবেশ,
		 প্রবেশ_গুনতি ? প্রবেশ_গুনতি - 1 : 0, প্রবেশ,
		 lookup_context.funcs.collect, lookup_context.collect_তথ্য[1]);
  collect_array (c, c->after,
		 lookahead_গুনতি, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_তথ্য[2]);
  recurse_lookups (c,
		   lookup_গুনতি_2, lookupRecord);
}

স্থির অন্তঃরেখ যৌক্তিক chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned পূর্ণসংখ্যা backtrack_গুনতি,
						     glyph_তথ্য_t backtrack HB_UNUSED,
						     unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
						     glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
						     unsigned পূর্ণসংখ্যা lookahead_গুনতি,
						     glyph_তথ্য_t lookahead HB_UNUSED,
						     unsigned পূর্ণসংখ্যা lookup_গুনতি_2 HB_UNUSED,
						     ধ্রুবক LookupRecord lookupRecord[] HB_UNUSED,
						     ধ্রুবক ChainContextApplyLookupContext &lookup_context)
{
  ফেরত (c->zero_context ? !backtrack_গুনতি && !lookahead_গুনতি : সত্য)
      && would_match_প্রবেশ (c,
			    প্রবেশ_গুনতি, প্রবেশ,
			    lookup_context.funcs.match[1], lookup_context.match_তথ্য_2[1]);
}

স্থির অন্তঃরেখ যৌক্তিক chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned পূর্ণসংখ্যা backtrack_গুনতি,
					       glyph_তথ্য_t backtrack,
					       unsigned পূর্ণসংখ্যা প্রবেশ_গুনতি, /* Including the first glyph (not matched) */
					       glyph_তথ্য_t প্রবেশ, /* Array of input values--start with second glyph */
					       unsigned পূর্ণসংখ্যা lookahead_গুনতি,
					       glyph_তথ্য_t lookahead,
					       unsigned পূর্ণসংখ্যা lookup_গুনতি_2,
					       ধ্রুবক LookupRecord lookupRecord[],
					       ধ্রুবক ChainContextApplyLookupContext &lookup_context)
{
  যদি (unlikely (প্রবেশ_গুনতি > HB_MAX_CONTEXT_দৈর্ঘ্য)) ফেরত মিথ্যা;

  unsigned start_সূচক = c->বাফার->out_len;
  unsigned end_সূচক = c->বাফার->idx;
  unsigned match_end = 0;
  যৌক্তিক ret = সত্য;
  যদি (!(match_প্রবেশ (c,
		     প্রবেশ_গুনতি, প্রবেশ,
		     lookup_context.funcs.match[1], lookup_context.match_তথ্য_2[1],
		     &match_end) && (end_সূচক = match_end)
       && match_lookahead (c,
			   lookahead_গুনতি, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_তথ্য_2[2],
			   match_end, &end_সূচক)))
  {
    c->বাফার->unsafe_to_concat (c->বাফার->idx, end_সূচক);
    ফেরত মিথ্যা;
  }

  যদি (!match_backtrack (c,
			backtrack_গুনতি, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_তথ্য_2[0],
			&start_সূচক))
  {
    c->বাফার->unsafe_to_concat_from_outbuffer (start_সূচক, end_সূচক);
    ফেরত মিথ্যা;
  }

  c->বাফার->unsafe_to_break_from_outbuffer (start_সূচক, end_সূচক);
  apply_lookup (c,
		প্রবেশ_গুনতি,
		lookup_গুনতি_2, lookupRecord,
		match_end);

  ফেরত ret;
}

template <typename Types>
কাঠামো ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend কাঠামো ChainRuleSetOf;

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ফেরত chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     প্রবেশ.lenP1, প্রবেশ.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  শূন্য depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 প্রবেশ.lenP1, প্রবেশ.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 এই,
				 value,
				 lookup_context);
  }

  শূন্য closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ফেরত;

    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  প্রবেশ.lenP1, প্রবেশ.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ফেরত;
    যদি (!intersects (c->glyphs, lookup_context)) ফেরত;

    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 প্রবেশ.lenP1, প্রবেশ.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest,
			      ধ্রুবক ClassDef *প্রবেশ_class_def = nullptr,
			      ধ্রুবক ClassDef *lookahead_class_def = nullptr) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);

    unsigned second;
    ধ্রুবক ClassDef *class_def;
    যদি (প্রবেশ.lenP1 > 1)
    {
      second = প্রবেশ.arrayZ[0];
      class_def = প্রবেশ_class_def;
    }
    অন্যথা যদি (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    অন্যথা
    {
      *digest = hb_set_digest_t::full ();
      ফেরত;
    }

    যদি (!class_def)
      digest->add (second);
    অন্যথা যদি (!second)
      *digest = hb_set_digest_t::full ();
    অন্যথা
      class_def->collect_class (digest, second);
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c,
		    ধ্রুবক ChainContextApplyLookupContext &lookup_context) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ফেরত chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     প্রবেশ.lenP1, প্রবেশ.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c,
	      ধ্রুবক ChainContextApplyLookupContext &lookup_context) ধ্রুবক
  {
    TRACE_APPLY (এই);
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      প্রবেশ.lenP1, প্রবেশ.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ধ্রুবক
  {
    c->copy (len);
    জন্য (ধ্রুবক auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  যৌক্তিক serialize (hb_serialize_context_t *c,
		  ধ্রুবক hb_map_t *lookup_map,
		  ধ্রুবক hb_map_t *backtrack_map,
		  ধ্রুবক hb_map_t *প্রবেশ_map = nullptr,
		  ধ্রুবক hb_map_t *lookahead_map = nullptr) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    ধ্রুবক hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    যদি (প্রবেশ_map) mapping = প্রবেশ_map;
    serialize_array (c, প্রবেশ.lenP1, + প্রবেশ.iter ()
				     | hb_map (mapping));

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    যদি (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_গুনতি_2 = c->embed (&(lookup.len));
    যদি (!lookup_গুনতি_2) return_trace (মিথ্যা);

    unsigned গুনতি = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_গুনতি_2, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       ধ্রুবক hb_map_t *lookup_map,
	       ধ্রুবক hb_map_t *backtrack_map = nullptr,
	       ধ্রুবক hb_map_t *প্রবেশ_map = nullptr,
	       ধ্রুবক hb_map_t *lookahead_map = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);

    যদি (!backtrack_map)
    {
      ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      যদি (!hb_all (backtrack, glyphset) ||
	  !hb_all (প্রবেশ, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (মিথ্যা);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    অন্যথা
    {
      যদি (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (প্রবেশ, প্রবেশ_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (মিথ্যা);

      serialize (c->serializer, lookup_map, backtrack_map, প্রবেশ_map, lookahead_map);
    }

    return_trace (সত্য);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    /* Hyper-optimized sanitized because this is really hot. */
    যদি (unlikely (!backtrack.len.sanitize (c))) return_trace (মিথ্যা);
    hb_barrier ();
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    যদি (unlikely (!প্রবেশ.lenP1.sanitize (c))) return_trace (মিথ্যা);
    hb_barrier ();
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    যদি (unlikely (!lookahead.len.sanitize (c))) return_trace (মিথ্যা);
    hb_barrier ();
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  সুরক্ষিত:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		প্রবেশ_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  সর্বজনীন:
  DEFINE_আকার_MIN (8);
};

template <typename Types, typename ChainRuleType>
কাঠামো ChainRuleSetOf
{
  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    ফেরত
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_map ([&] (ধ্রুবক ChainRuleType &_) { ফেরত _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  শূন্য collect_first_প্রবেশ_classes (hb_ot_layout_ruleset_digest_t *digest) ধ্রুবক
  {
    digest->init ();
    জন্য (unsigned i = 0; i < rule.len; i++)
    {
      ধ্রুবক ChainRuleType &r = এই+rule.arrayZ[i];
      ধ্রুবক auto &প্রবেশ = StructAfter<decltype (r.প্রবেশ_X)> (r.backtrack);
      যদি (প্রবেশ.lenP1 <= 1)
      {
	digest->set_full ();
	ফেরত;
      }
      digest->add (প্রবেশ.arrayZ[0]);
    }
  }
  শূন্য depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ধ্রুবক
  {
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  শূন্য closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ফেরত;

    ফেরত
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ধ্রুবক
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ফেরত;

    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ধ্রুবক
  {
    ফেরত
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest,
			      ধ্রুবক ClassDef *প্রবেশ_class_def = nullptr,
			      ধ্রুবক ClassDef *lookahead_class_def = nullptr) ধ্রুবক
  {
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_apply ([=] (ধ্রুবক ChainRuleType &_)
		{ _.collect_second_glyphs (digest, প্রবেশ_class_def, lookahead_class_def); })
    ;
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c,
		    ধ্রুবক ChainContextApplyLookupContext &lookup_context) ধ্রুবক
  {
    ফেরত
    + hb_iter (rule)
    | hb_map (hb_add (এই))
    | hb_map ([&] (ধ্রুবক ChainRuleType &_) { ফেরত _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c,
	      ধ্রুবক ChainContextApplyLookupContext &lookup_context,
	      ধ্রুবক hb_ot_layout_ruleset_digest_t *digest = nullptr) ধ্রুবক
  {
    TRACE_APPLY (এই);

    unsigned অঙ্ক_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    যদি (HB_OPTIMIZE_আকার_VAL || অঙ্ক_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (এই))
      | hb_map ([&] (ধ্রুবক ChainRuleType &_) { ফেরত _.apply (c, lookup_context); })
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
    skippy_iter.reset (c->বাফার->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_তথ্য ((HBUINT16 *) nullptr);
    unsigned unsafe_to = (unsigned) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *first = nullptr, *second = nullptr;
    যৌক্তিক matched = skippy_iter.next (&unsafe_to1);
    যদি (likely (matched))
    {
      যদি (skippy_iter.may_skip (c->বাফার->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        যাও slow;
      }

      first = &c->বাফার->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      যদি (digest && lookup_context.get_প্রবেশ_মান &&
	  !digest->may_have (lookup_context.get_প্রবেশ_মান (*first, lookup_context.match_তথ্য_2[1])))
	{
	  c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to1);
	  return_trace (মিথ্যা);
	}
    }
    অন্যথা
    {
      যৌক্তিক unsafe_to_concat = মিথ্যা;
      যৌক্তিক ret =
      + hb_iter (rule)
      | hb_map (hb_add (এই))
      | hb_filter ([&] (ধ্রুবক ChainRuleType &_)
		   {
		     ধ্রুবক auto &প্রবেশ = StructAfter<decltype (_.প্রবেশ_X)> (_.backtrack);
		     ধ্রুবক auto &lookahead = StructAfter<decltype (_.lookaheadX)> (প্রবেশ);
		     যদি (প্রবেশ.lenP1 <= 1 && lookahead.len == 0) ফেরত সত্য;
		     unsafe_to_concat = সত্য;
		     ফেরত মিথ্যা;
		   })
      | hb_map ([&] (ধ্রুবক ChainRuleType &_) { ফেরত _.apply (c, lookup_context); })
      | hb_any
      ;
      যদি (unsafe_to_concat)
	c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    যদি (likely (matched))
    {
      যদি (skippy_iter.may_skip (c->বাফার->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        যাও slow;
      }

      second = &c->বাফার->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_প্রবেশ = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *প্রবেশ_তথ্য = lookup_context.match_তথ্য_2[1];
    auto *lookahead_তথ্য = lookup_context.match_তথ্য_2[2];
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < অঙ্ক_rules; i++)
    {
      ধ্রুবক auto &r = এই+rule.arrayZ[i];

      ধ্রুবক auto &প্রবেশ = StructAfter<decltype (r.প্রবেশ_X)> (r.backtrack);
      ধ্রুবক auto &lookahead = StructAfter<decltype (r.lookaheadX)> (প্রবেশ);

      unsigned lenP1 = প্রবেশ.lenP1;
      যদি (lenP1 > 1 ?
	   (!match_প্রবেশ ||
	    match_প্রবেশ (*first, প্রবেশ.arrayZ[0], প্রবেশ_তথ্য))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_তথ্য)))
      {
	lenP1 = hb_max (lenP1, 1u);
        যদি (!second ||
	    (lenP1 > 2 ?
	     (!match_প্রবেশ ||
	      match_প্রবেশ (*second, প্রবেশ.arrayZ[1], প্রবেশ_তথ্য))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_তথ্য))))
	{
	  যদি (r.apply (c, lookup_context))
	  {
	    যদি (unsafe_to != (unsigned) -1)
	      c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to);
	    return_trace (সত্য);
	  }
	}
	অন্যথা
	  unsafe_to = unsafe_to2;
      }
      অন্যথা
      {
	যদি (unsafe_to == (unsigned) -1)
	  unsafe_to = unsafe_to1;

	যদি (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  জন্য (; i + 1 < অঙ্ক_rules; i++)
	  {
	    ধ্রুবক auto &r2 = এই+rule.arrayZ[i + 1];
	    ধ্রুবক auto &input2 = StructAfter<decltype (r2.প্রবেশ_X)> (r2.backtrack);
	    যদি (input2.lenP1 <= 1 || input2.arrayZ[0] != প্রবেশ.arrayZ[0])
	      বিরতি;
	  }
	}
      }
    }
    যদি (likely (unsafe_to != (unsigned) -1))
      c->বাফার->unsafe_to_concat (c->বাফার->idx, unsafe_to);

    return_trace (মিথ্যা);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       ধ্রুবক hb_map_t *lookup_map,
	       ধ্রুবক hb_map_t *backtrack_klass_map = nullptr,
	       ধ্রুবক hb_map_t *প্রবেশ_klass_map = nullptr,
	       ধ্রুবক hb_map_t *lookahead_klass_map = nullptr) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    জন্য (ধ্রুবক auto& _ : rule)
    {
      যদি (!_) চালু;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      যদি (unlikely (!o)) চালু;

      যদি (!o->serialize_subset (c, _, এই,
				lookup_map,
				backtrack_klass_map,
				প্রবেশ_klass_map,
				lookahead_klass_map))
      {
	out->rule.বের_করো ();
	c->serializer->revert (o_snap);
      }
    }

    যৌক্তিক ret = যৌক্তিক (out->rule);
    যদি (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (rule.sanitize (c, এই));
  }

  সুরক্ষিত:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2, rule);
};

template <typename Types>
কাঠামো ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (আকার (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_আকার + Offset24::static_আকার, "");

template <typename Types>
কাঠামো ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
কাঠামো ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    কাঠামো ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    ফেরত
    + hb_zip (এই+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_map ([&] (ধ্রুবক ChainRuleSet &_) { ফেরত _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত সত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (এই+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ফেরত c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ধ্রুবক hb_pair_t<hb_codepoint_t, unsigned> _) { ফেরত hb_pair_t<unsigned, ধ্রুবক ChainRuleSet&> (_.first, এই+ruleSet[_.second]); })
    | hb_apply ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    কাঠামো ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (এই+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ফেরত c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ধ্রুবক hb_pair_t<hb_codepoint_t, unsigned> _) { ফেরত hb_pair_t<unsigned, ধ্রুবক ChainRuleSet&> (_.first, এই+ruleSet[_.second]); })
    | hb_apply ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক
  {
    কাঠামো ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (এই+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    (এই+coverage).collect_coverage (c->প্রবেশ);

    কাঠামো ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_apply ([digest] (ধ্রুবক ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    ধ্রুবক ChainRuleSet &rule_set = এই+ruleSet[(এই+coverage).get_coverage (c->glyphs[0])];
    কাঠামো ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    ফেরত rule_set.would_apply (c, lookup_context);
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (c->বাফার->cur().codepoint);
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    ধ্রুবক ChainRuleSet &rule_set = এই+ruleSet[সূচক_2];
    কাঠামো ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;

    ধ্রুবক hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এই+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, এই, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (যৌক্তিক (new_coverage));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) && ruleSet.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + Types::HBUINT::static_আকার, ruleSet);
};

template <typename Types>
কাঠামো ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (glyphs))
      ফেরত মিথ্যা;

    ধ্রুবক ClassDef &backtrack_class_def = এই+backtrackClassDef;
    ধ্রুবক ClassDef &প্রবেশ_class_def = এই+প্রবেশ_Class_Def;
    ধ্রুবক ClassDef &lookahead_class_def = এই+lookaheadClassDef;

    hb_map_t caches[3] = {};
    কাঠামো ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &প্রবেশ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (এই+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    প্রবেশ_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ফেরত
    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_enumerate
    | hb_map ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক ChainRuleSet &> p)
	      { ফেরত প্রবেশ_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত সত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (c->glyphs))
      ফেরত;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ধ্রুবক ClassDef &backtrack_class_def = এই+backtrackClassDef;
    ধ্রুবক ClassDef &প্রবেশ_class_def = এই+প্রবেশ_Class_Def;
    ধ্রুবক ClassDef &lookahead_class_def = এই+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &প্রবেশ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ফেরত প্রবেশ_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ধ্রুবক ChainRuleSet& chainrule_set = এই+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (c->glyphs))
      ফেরত;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ফেরত;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ধ্রুবক ClassDef &backtrack_class_def = এই+backtrackClassDef;
    ধ্রুবক ClassDef &প্রবেশ_class_def = এই+প্রবেশ_Class_Def;
    ধ্রুবক ClassDef &lookahead_class_def = এই+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    কাঠামো ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &প্রবেশ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ফেরত প্রবেশ_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ধ্রুবক hb_pair_t<unsigned, ধ্রুবক typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ধ্রুবক ChainRuleSet& chainrule_set = এই+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক
  {
    যদি (!(এই+coverage).intersects (c->glyphs))
      ফেরত;

    ধ্রুবক ClassDef &backtrack_class_def = এই+backtrackClassDef;
    ধ্রুবক ClassDef &প্রবেশ_class_def = এই+প্রবেশ_Class_Def;
    ধ্রুবক ClassDef &lookahead_class_def = এই+lookaheadClassDef;

    hb_map_t caches[3] = {};
    কাঠামো ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &প্রবেশ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { ফেরত প্রবেশ_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ধ্রুবক ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    (এই+coverage).collect_coverage (c->প্রবেশ);

    ধ্রুবক ClassDef &backtrack_class_def = এই+backtrackClassDef;
    ধ্রুবক ClassDef &প্রবেশ_class_def = এই+প্রবেশ_Class_Def;
    ধ্রুবক ClassDef &lookahead_class_def = এই+lookaheadClassDef;

    কাঠামো ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &প্রবেশ_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_apply ([&] (ধ্রুবক ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  {
    ধ্রুবক ClassDef &প্রবেশ_class_def = এই+প্রবেশ_Class_Def;
    ধ্রুবক ClassDef &lookahead_class_def = এই+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (এই))
    | hb_apply ([digest, &প্রবেশ_class_def, &lookahead_class_def] (ধ্রুবক ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &প্রবেশ_class_def, &lookahead_class_def); })
    ;
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    ধ্রুবক ClassDef &backtrack_class_def = এই+backtrackClassDef;
    ধ্রুবক ClassDef &প্রবেশ_class_def = এই+প্রবেশ_Class_Def;
    ধ্রুবক ClassDef &lookahead_class_def = এই+lookaheadClassDef;

    unsigned পূর্ণসংখ্যা সূচক_2 = প্রবেশ_class_def.get_class (c->glyphs[0]);
    ধ্রুবক ChainRuleSet &rule_set = এই+ruleSet[সূচক_2];
    কাঠামো ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &প্রবেশ_class_def,
       &lookahead_class_def}
    };
    ফেরত rule_set.would_apply (c, lookup_context);
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  unsigned cache_cost () ধ্রুবক
  {
    ফেরত (এই+প্রবেশ_Class_Def).cost () + (এই+lookaheadClassDef).cost ();
  }
  স্থির যৌক্তিক cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ফেরত context_cache_func (c, op);
  }

  কাঠামো external_cache_t
  {
    hb_ot_layout_দ্বিমিক_cache_t coverage;
    hb_ot_layout_mapping_cache_t প্রবেশ_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  শূন্য *external_cache_create () ধ্রুবক
  {
    unsigned গুনতি = ruleSet.len;
    unsigned আকার_2 = আকার (external_cache_t) -
		    HB_VAR_ARRAY * আকার (hb_ot_layout_ruleset_digest_t) +
		    গুনতি * আকার (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (আকার_2);
    যদি (likely (cache))
    {
      cache->coverage.clear ();
      cache->প্রবেশ_class.clear ();
      cache->lookahead_class.clear ();
      জন্য (unsigned i = 0; i < গুনতি; i++)
	(এই+ruleSet[i]).collect_first_প্রবেশ_classes (&cache->rule_sets[i]);
    }
    ফেরত cache;
  }
  যৌক্তিক apply_cached (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুবক { ফেরত _apply (c, সত্য, external_cache); }
  যৌক্তিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুবক { ফেরত _apply (c, মিথ্যা, external_cache); }
  যৌক্তিক _apply (hb_ot_apply_context_t *c, যৌক্তিক cached, শূন্য *external_cache) ধ্রুবক
  {
    TRACE_APPLY (এই);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage_দ্বিমিক (c->বাফার->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (c->বাফার->cur().codepoint);
#endif
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    ধ্রুবক ClassDef &backtrack_class_def = এই+backtrackClassDef;
    ধ্রুবক ClassDef &প্রবেশ_class_def = এই+প্রবেশ_Class_Def;
    ধ্রুবক ClassDef &lookahead_class_def = এই+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_তথ্য_t প্রবেশ_cache_তথ্য = {
      &প্রবেশ_class_def,
      cache ? &cache->প্রবেশ_class : nullptr
    };
    match_class_cache_তথ্য_t lookahead_cache_তথ্য = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    ধ্রুবক শূন্য *প্রবেশ_match_তথ্য = cached
      ? static_cast<ধ্রুবক শূন্য *> (&প্রবেশ_class_def)
      : static_cast<ধ্রুবক শূন্য *> (&প্রবেশ_cache_তথ্য);
    ধ্রুবক শূন্য *lookahead_match_তথ্য = cached
      ? static_cast<ধ্রুবক শূন্য *> (&lookahead_class_def)
      : static_cast<ধ্রুবক শূন্য *> (&lookahead_cache_তথ্য);
    যৌক্তিক backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ধ্রুবক শূন্য *backtrack_match_তথ্য = backtrack_uses_lookahead_cache
      ? static_cast<ধ্রুবক শূন্য *> (&lookahead_cache_তথ্য)
      : static_cast<ধ্রুবক শূন্য *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    কাঠামো ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_তথ্য,
       প্রবেশ_match_তথ্য,
       lookahead_match_তথ্য},
      cached ? get_class_cached2_মান : get_class_cache_মান
#অন্যথা
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &প্রবেশ_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_মান : get_class_মান
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    সূচক_2 = cached
         ? get_class_cached2 (প্রবেশ_class_def, c->বাফার->cur())
         : প্রবেশ_class_def.get_class (c->বাফার->cur().codepoint, প্রবেশ_cache_তথ্য.cache);
#অন্যথা
    সূচক_2 = cached
         ? get_class_cached2 (প্রবেশ_class_def, c->বাফার->cur())
          : প্রবেশ_class_def.get_class (c->বাফার->cur().codepoint);
#endif
    ধ্রুবক ChainRuleSet &rule_set = এই+ruleSet[সূচক_2];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ধ্রুবক hb_ot_layout_ruleset_digest_t *digest = cache && সূচক_2 < ruleSet.len ? &cache->rule_sets[সূচক_2] : nullptr;
#অন্যথা
    ধ্রুবক hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    auto *out = c->serializer->start_embed (*এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, এই);

    hb_map_t backtrack_klass_map;
    hb_map_t প্রবেশ_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, এই, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->প্রবেশ_Class_Def.serialize_subset (c, প্রবেশ_Class_Def, এই, &প্রবেশ_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, এই, &lookahead_klass_map);

    যদি (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   প্রবেশ_klass_map,
						   lookahead_klass_map)))
      return_trace (মিথ্যা);

    ধ্রুবক hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (এই+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (এই+প্রবেশ_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    পূর্ণসংখ্যা non_zero_সূচক = -1, সূচক_2 = 0;
    যৌক্তিক ret = সত্য;
    ধ্রুবক hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    জন্য (ধ্রুবক auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (প্রবেশ_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      যদি (unlikely (!o))
      {
	ret = মিথ্যা;
	বিরতি;
      }
      যদি (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, এই,
			       lookup_map,
			       &backtrack_klass_map,
			       &প্রবেশ_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_সূচক = সূচক_2;
      }

      সূচক_2++;
    }

    যদি (!ret || non_zero_সূচক == -1) return_trace (মিথ্যা);

    // prune empty trailing ruleSets
    যদি (সূচক_2 > non_zero_সূচক) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_সূচক + 1;
    }

    return_trace (যৌক্তিক (out->ruleSet));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) &&
		  backtrackClassDef.sanitize (c, এই) &&
		  প্রবেশ_Class_Def.sanitize (c, এই) &&
		  lookaheadClassDef.sanitize (c, এই) &&
		  ruleSet.sanitize (c, এই));
  }

  সুরক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		প্রবেশ_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4 + 4 * Types::LOffset::static_আকার, ruleSet);
};

template <typename Types>
কাঠামো ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);

    যদি (!(এই+প্রবেশ[0]).intersects (glyphs))
      ফেরত মিথ্যা;

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    কাঠামো ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {এই, এই, এই}
    };
    ফেরত chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     প্রবেশ.len, প্রবেশ.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত সত্য; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);

    যদি (!(এই+প্রবেশ[0]).intersects (c->glyphs))
      ফেরত;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs))
      ফেরত;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {এই, এই, এই},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ধ্রুবক HBUINT16 *) backtrack.arrayZ,
				 প্রবেশ.len, (ধ্রুবক HBUINT16 *) প্রবেশ.arrayZ + 1,
				 lookahead.len, (ধ্রুবক HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 এই,
				 প্রবেশ[0], lookup_context);

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);

    যদি (!(এই+প্রবেশ[0]).intersects (c->glyphs))
      ফেরত;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs))
      ফেরত;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    কাঠামো ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {এই, এই, এই}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  প্রবেশ.len, প্রবেশ.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->বের_করো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক
  {
    যদি (!intersects (c->glyphs))
      ফেরত;

    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);

    (এই+প্রবেশ[0]).collect_coverage (c->প্রবেশ);

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    কাঠামো ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {এই, এই, এই}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 প্রবেশ.len, প্রবেশ.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    যদি (প্রবেশ.len > 1)
      (এই+প্রবেশ[1]).collect_coverage (digest);
    অন্যথা যদি (lookahead.len)
      (এই+lookahead[0]).collect_coverage (digest);
    অন্যথা
      *digest = hb_set_digest_t::full ();
  }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    কাঠামো ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {এই, এই, এই}
    };
    ফেরত chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     প্রবেশ.len, প্রবেশ.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক
  {
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    ফেরত এই+প্রবেশ[0];
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);

    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+প্রবেশ[0]).get_coverage (c->বাফার->cur().codepoint);
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    কাঠামো ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {এই, এই, এই}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      প্রবেশ.len, প্রবেশ.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ধ্রুবক শূন্য* base) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    যদি (unlikely (!out->serialize (c->serializer, 0u))) return_trace (মিথ্যা);

    জন্য (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      যদি (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (মিথ্যা);
    }

    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    যদি (unlikely (!c->serializer->embed (এই->format))) return_trace (মিথ্যা);

    যদি (!serialize_coverage_offsets (c, backtrack.iter (), এই))
      return_trace (মিথ্যা);

    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    যদি (!serialize_coverage_offsets (c, প্রবেশ.iter (), এই))
      return_trace (মিথ্যা);

    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    যদি (!serialize_coverage_offsets (c, lookahead.iter (), এই))
      return_trace (মিথ্যা);

    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ধ্রুবক hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_গুনতি_2 = c->serializer->copy<HBUINT16> (lookup.len);
    যদি (!lookup_গুনতি_2) return_trace (মিথ্যা);

    unsigned গুনতি = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_গুনতি_2, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!backtrack.sanitize (c, এই))) return_trace (মিথ্যা);
    hb_barrier ();
    ধ্রুবক auto &প্রবেশ = StructAfter<decltype (প্রবেশ_X)> (backtrack);
    যদি (unlikely (!প্রবেশ.sanitize (c, এই))) return_trace (মিথ্যা);
    hb_barrier ();
    যদি (unlikely (!প্রবেশ.len)) return_trace (মিথ্যা); /* To be consistent with Context. */
    ধ্রুবক auto &lookahead = StructAfter<decltype (lookaheadX)> (প্রবেশ);
    যদি (unlikely (!lookahead.sanitize (c, এই))) return_trace (মিথ্যা);
    hb_barrier ();
    ধ্রুবক auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  সুরক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		প্রবেশ_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  সর্বজনীন:
  DEFINE_আকার_MIN (10);
};

কাঠামো ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    ক্ষেত্র 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    ক্ষেত্র 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    ক্ষেত্র 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    ক্ষেত্র 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }		format;	/* Format identifier */
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
কাঠামো ExtensionFormat1
{
  unsigned পূর্ণসংখ্যা get_type () ধ্রুবক { ফেরত extensionLookupType; }

  template <typename X>
  ধ্রুবক X& get_subtable () ধ্রুবক
  { ফেরত এই + reinterpret_cast<ধ্রুবক Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, এই))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    auto *out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    ধ্রুবক auto& src_offset =
        reinterpret_cast<ধ্রুবক Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, এই, get_type ()));
  }

  সুরক্ষিত:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  সর্বজনীন:
  DEFINE_আকার_STATIC (8);
};

template <typename T>
কাঠামো Extension
{
  unsigned পূর্ণসংখ্যা get_type () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.get_type ();
    পূর্বনির্ধারিত:ফেরত 0;
    }
  }
  template <typename X>
  ধ্রুবক X& get_subtable () ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.template get_subtable<typename T::SubTable> ();
    পূর্বনির্ধারিত:ফেরত Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ধ্রুবক
  {
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); ফেরত u.format1.subset (c);
    পূর্বনির্ধারিত: ফেরত c->default_return_মান ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুবক
  {
    যদি (unlikely (!c->may_dispatch (এই, &u.format.v))) ফেরত c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এই, u.format.v);
    নির্বাচন (u.format.v) {
    ক্ষেত্র 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    পূর্বনির্ধারিত:return_trace (c->default_return_মান ());
    }
  }

  সুরক্ষিত:
  সংঘ {
  কাঠামো { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

কাঠামো hb_ot_layout_lookup_accelerator_t
{
  template <typename TLookup>
  স্থির hb_ot_layout_lookup_accelerator_t *create (ধ্রুবক TLookup &lookup)
  {
    unsigned গুনতি = lookup.get_subtable_গুনতি ();

    unsigned আকার_2 = আকার (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * আকার (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    গুনতি * আকার (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, আকার_2);
    যদি (unlikely (!thiz))
      ফেরত nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    যখন (গুনতি && !thiz->subtables[গুনতি - 1].apply_func)
      গুনতি--;

    thiz->digest.init ();
    জন্য (auto& subtable : hb_iter (thiz->subtables, গুনতি))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->গুনতি = গুনতি;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    জন্য (unsigned i = 0; i < গুনতি; i++)
      যদি (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    ফেরত thiz;
  }

  শূন্য fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    জন্য (unsigned i = 0; i < গুনতি; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  যৌক্তিক may_have (hb_codepoint_t g) ধ্রুবক
  { ফেরত digest.may_have (g); }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  যৌক্তিক apply (hb_ot_apply_context_t *c, যৌক্তিক use_cache) ধ্রুবক
  {
    যদি (গুনতি == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      যদি (use_cache)
	ফেরত subtables[0].apply_cached_no_digest (c);
#endif
      ফেরত subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    যদি (use_cache)
    {
      ফেরত
      + hb_iter (hb_iter (subtables, গুনতি))
      | hb_map ([&c] (ধ্রুবক hb_accelerate_subtables_context_t::hb_applicable_t &_) { ফেরত _.apply_cached (c); })
      | hb_any
      ;
    }
    অন্যথা
#endif
    {
      ফেরত
      + hb_iter (hb_iter (subtables, গুনতি))
      | hb_map ([&c] (ধ্রুবক hb_accelerate_subtables_context_t::hb_applicable_t &_) { ফেরত _.apply (c); })
      | hb_any
      ;
    }
    ফেরত মিথ্যা;
  }

  যৌক্তিক cache_enter (hb_ot_apply_context_t *c) ধ্রুবক
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ফেরত subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#অন্যথা
    ফেরত মিথ্যা;
#endif
  }
  শূন্য cache_leave (hb_ot_apply_context_t *c) ধ্রুবক
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  ব্যক্তিগত:
  unsigned গুনতি = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

কাঠামো GSUBGPOS
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

  size_t get_আকার () ধ্রুবক
  {
    যদি (version.major != 1) ফেরত version.static_আকার;

    ফেরত min_আকার +
	   (version.to_int () >= 0x00010001u ? featureVars.static_আকার : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_আকার : 0)
#অন্যথা
	   0
#endif
	   ;
  }

  ধ্রুবক ScriptList &get_script_list () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && scriptList2) ফেরত এই + scriptList2;
#endif
    ফেরত এই + scriptList;
  }
  ধ্রুবক FeatureList &get_feature_list () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && featureList2) ফেরত এই + featureList2;
#endif
    ফেরত এই + featureList;
  }
  template <typename TLookup>
  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (এই, get_আকার ())))
      return_trace (মিথ্যা);
    hb_barrier ();

    যৌক্তিক script_list_ok = scriptList.sanitize (c, এই);
    যৌক্তিক feature_list_ok = featureList.sanitize (c, এই);
    যৌক্তিক lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, এই);

#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u)
    {
      যদি (scriptList2) script_list_ok = scriptList2.sanitize (c, এই);
      যদি (featureList2) feature_list_ok = featureList2.sanitize (c, এই);
      যদি (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, এই);
    }
#endif

    যদি (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (মিথ্যা);

#ifndef HB_NO_VAR
    যদি (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, এই, &get_feature_list ()))))
      return_trace (মিথ্যা);
#endif

    return_trace (সত্য);
  }

  template <typename TLookup>
  যৌক্তিক subset (hb_subset_layout_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    যদি (version.major != 1) return_trace (মিথ্যা);

    auto *out = c->subset_context->serializer->start_embed (এই);
    যদি (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (মিথ্যা);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u)
    {
      যদি (unlikely (!c->subset_context->serializer->extend_আকার (out, get_আকার ())))
        return_trace (মিথ্যা);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      যদি (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 এই, c);
      অন্যথা
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 এই, c);

      যদি (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 এই, c);
      অন্যথা
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 এই, c);

      যদি (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, এই, c);
      অন্যথা
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, এই, c);
    }
    অন্যথা
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     এই,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     এই,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					এই,
					c);
    }

#ifndef HB_NO_VAR
    যদি (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      যদি (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (মিথ্যা);

      // if all axes are pinned all feature vars are dropped.
      যৌক্তিক ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, এই, c);
      যদি (!ret)
      {
        out->featureVars = 0;
	যদি (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (সত্য);
  }

  unsigned পূর্ণসংখ্যা get_lookup_গুনতি () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && lookupList2)
      ফেরত (এই + lookupList2).len;
#endif
    ফেরত (এই + lookupList).len;
  }
  ধ্রুবক Lookup& get_lookup (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && lookupList2)
      ফেরত (এই + lookupList2)[i];
#endif
    ফেরত (এই + lookupList)[i];
  }
  ধ্রুবক FeatureVariations &get_feature_variations () ধ্রুবক
  {
    ফেরত (version.to_int () >= 0x00010001u && hb_barrier ()) ? এই+featureVars : Null (FeatureVariations);
  }

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version.to_int (); }
  unsigned পূর্ণসংখ্যা get_script_গুনতি () ধ্রুবক
  { ফেরত get_script_list ().len; }
  ধ্রুবক Tag& get_script_tag (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত get_script_list ().get_tag (i); }
  unsigned পূর্ণসংখ্যা get_script_tags (unsigned পূর্ণসংখ্যা start_offset,
				unsigned পূর্ণসংখ্যা *script_গুনতি /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ধ্রুবক
  { ফেরত get_script_list ().get_tags (start_offset, script_গুনতি, script_tags); }
  ধ্রুবক Script& get_script (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত get_script_list ()[i]; }
  যৌক্তিক find_script_সূচক (hb_tag_t tag, unsigned পূর্ণসংখ্যা *সূচক_2) ধ্রুবক
  { ফেরত get_script_list ().find_সূচক (tag, সূচক_2); }

  unsigned পূর্ণসংখ্যা get_feature_গুনতি () ধ্রুবক
  { ফেরত get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত i == সূচক::NOT_FOUND_সূচক ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned পূর্ণসংখ্যা get_feature_tags (unsigned পূর্ণসংখ্যা start_offset,
				 unsigned পূর্ণসংখ্যা *feature_গুনতি_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) ধ্রুবক
  { ফেরত get_feature_list ().get_tags (start_offset, feature_গুনতি_2, feature_tags); }
  ধ্রুবক Feature& get_feature (unsigned পূর্ণসংখ্যা i) ধ্রুবক
  { ফেরত get_feature_list ()[i]; }
  যৌক্তিক find_feature_সূচক (hb_tag_t tag, unsigned পূর্ণসংখ্যা *সূচক_2) ধ্রুবক
  { ফেরত get_feature_list ().find_সূচক (tag, সূচক_2); }

  যৌক্তিক find_variations_সূচক (ধ্রুবক পূর্ণসংখ্যা *coords, unsigned পূর্ণসংখ্যা অঙ্ক_coords,
			      unsigned পূর্ণসংখ্যা *সূচক_2,
			      ItemVarStoreInstancer *instancer) ধ্রুবক
  {
#ifdef HB_NO_VAR
    *সূচক_2 = FeatureVariations::NOT_FOUND_সূচক;
    ফেরত মিথ্যা;
#endif
    ফেরত get_feature_variations ().find_সূচক (coords, অঙ্ক_coords, সূচক_2, instancer);
  }
  যৌক্তিক get_feature_variations_state (ধ্রুবক পূর্ণসংখ্যা *coords,
				     unsigned পূর্ণসংখ্যা অঙ্ক_coords,
				     unsigned পূর্ণসংখ্যা *সূচক_2,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) ধ্রুবক
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *সূচক_2 = FeatureVariations::NOT_FOUND_সূচক;
    ফেরত সত্য;
#অন্যথা
    find_variations_সূচক (coords, অঙ্ক_coords, সূচক_2, instancer);
    ফেরত get_feature_variations ().collect_lookup_conditions (
	coords,
	অঙ্ক_coords,
	instancer,
	lookup_conditions);
#endif
  }
  যৌক্তিক get_lookup_variations (unsigned feature_সূচক,
                              unsigned variations_সূচক,
                              hb_sorted_array_t<ধ্রুবক unsigned> conditions,
                              hb_set_t *lookup_indexes) ধ্রুবক
  {
#ifdef HB_NO_VAR
    ফেরত মিথ্যা;
#অন্যথা
    ফেরত get_feature_variations ().get_lookup_variations (
	feature_সূচক, get_feature_variation (feature_সূচক, variations_সূচক),
	conditions, lookup_indexes);
#endif
  }
  ধ্রুবক Feature& get_feature_variation (unsigned পূর্ণসংখ্যা feature_সূচক,
					unsigned পূর্ণসংখ্যা variations_সূচক) ধ্রুবক
  {
#ifndef HB_NO_VAR
    যদি (FeatureVariations::NOT_FOUND_সূচক != variations_সূচক &&
	version.to_int () >= 0x00010001u)
    {
      ধ্রুবক Feature *feature = get_feature_variations ().find_substitute (variations_সূচক,
									  feature_সূচক);
      যদি (feature)
	ফেরত *feature;
    }
#endif
    ফেরত get_feature (feature_সূচক);
  }

  শূন্য feature_variation_collect_lookups (ধ্রুবক hb_set_t *feature_indexes,
					  ধ্রুবক hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) ধ্রুবক
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  শূন্য collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  শূন্য collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ধ্রুবক
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  শূন্য closure_lookups (hb_face_t      *face,
			ধ্রুবক hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) ধ্রুবক
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    জন্য (unsigned lookup_সূচক : *lookup_indexes)
      reinterpret_cast<ধ্রুবক TLookup &> (get_lookup (lookup_সূচক)).closure_lookups (&c, lookup_সূচক);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  শূন্য prune_langsys (ধ্রুবক hb_map_t *duplicate_feature_map,
                      ধ্রুবক hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) ধ্রুবক
  {
    hb_prune_langsys_context_t c (এই, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned গুনতি = get_script_গুনতি ();
    জন্য (unsigned script_সূচক = 0; script_সূচক < গুনতি; script_সূচক++)
    {
      ধ্রুবক Tag& tag = get_script_tag (script_সূচক);
      যদি (!layout_scripts->has (tag)) চালু;
      ধ্রুবক Script& s = get_script (script_সূচক);
      s.prune_langsys (&c, script_সূচক);
    }
  }

  শূন্য prune_features (ধ্রুবক hb_map_t *lookup_indices, /* IN */
		       ধ্রুবক hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       ধ্রুবক hb_hashmap_t<unsigned, ধ্রুবক Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) ধ্রুবক
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    যদি (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      ফেরত;
    }
#endif

    জন্য (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      যদি (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        চালু;


      ধ্রুবক Feature *f = &(get_feature (i));
      ধ্রুবক Feature** p = nullptr;
      যদি (feature_substitutes_map->has (i, &p))
        f = *p;

      যদি (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        চালু;

      যদি (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  শূন্য collect_name_ids (ধ্রুবক hb_map_t *feature_সূচক_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         ধ্রুবক hb_hashmap_t<unsigned, ধ্রুবক Feature*> *feature_substitutes_map,
                         ধ্রুবক hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         ধ্রুবক hb_set_t *catch_all_features,
                         যৌক্তিক all_axes_pinned HB_UNUSED) ধ্রুবক
  {
    unsigned গুনতি = get_feature_গুনতি ();
    জন্য (unsigned i = 0 ; i < গুনতি; i++)
    {
      যদি (!feature_সূচক_map->has (i)) চালু;
      hb_tag_t tag = get_feature_tag (i);
      ধ্রুবক Feature *feature = &get_feature (i);
      ধ্রুবক Feature **substitute;
      যদি (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        যদি (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    যদি (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_সূচক_map, feature_record_cond_idx_map,
                                                  [এই] (unsigned i) { ফেরত get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = সত্য;
      এই->table = sc.reference_table<T> (face);

      যদি (unlikely (এই->table->is_blocklisted (এই->table.get_blob (), face)))
      {
	hb_blob_destroy (এই->table.get_blob ());
	এই->table = hb_blob_get_empty ();
      }

      এই->lookup_গুনতি = table->get_lookup_গুনতি ();

      এই->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (এই->lookup_গুনতি, আকার (*accels));
      যদি (unlikely (!এই->accels))
      {
	এই->lookup_গুনতি = 0;
	এই->table.destroy ();
	এই->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      জন্য (unsigned পূর্ণসংখ্যা i = 0; i < এই->lookup_গুনতি; i++)
      {
	auto *accel = এই->accels[i].get_relaxed ();
	যদি (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (এই->accels);
      এই->table.destroy ();
    }

    hb_blob_t *get_blob () ধ্রুবক { ফেরত table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_সূচক) ধ্রুবক
    {
      যদি (unlikely (lookup_সূচক >= lookup_গুনতি)) ফেরত nullptr;

    retry:
      auto *accel = accels[lookup_সূচক].get_acquire ();
      যদি (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_সূচক));
	যদি (unlikely (!accel))
	  ফেরত nullptr;

	যদি (unlikely (!accels[lookup_সূচক].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  যাও retry;
	}
      }

      ফেরত accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned পূর্ণসংখ্যা lookup_গুনতি;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  সুরক্ষিত:
  template <typename TLookup>
  ধ্রুবক Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ধ্রুবক Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  ধ্রুবক Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { ফেরত reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ধ্রুবক Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  ধ্রুবক Offset16To<RecordListOfFeature>& subset_feature_list_offset () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { ফেরত reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  ধ্রুবক Offset32To<RecordListOfFeature>& subset_feature_list2_offset () ধ্রুবক
  { ফেরত reinterpret_cast<ধ্রুবক Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  ধ্রুবক শূন্য* get_lookup_list_field_offset () ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && lookupList2) ফেরত &lookupList2;
#endif
    ফেরত &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  স্থির যৌক্তিক serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ধ্রুবক OffsetIn& in,
				       ধ্রুবক Base *base,
				       Ts&&... ds)
  {
    out = 0;
    যদি (in.is_null ()) ফেরত মিথ্যা;

    auto *s = c->serializer;
    s->push ();
    যৌক্তিক ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    যদি (ret) s->add_link (out, s->বের_করো_pack ());
    অন্যথা s->বের_করো_discard ();
    ফেরত ret;
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
  সর্বজনীন:
  DEFINE_আকার_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
