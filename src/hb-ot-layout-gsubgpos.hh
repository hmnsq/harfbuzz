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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-buffer.hh"
#অন্তর্ভাৱ "hb-map.hh"
#অন্তর্ভাৱ "hb-set.hh"
#অন্তর্ভাৱ "hb-ot-map.hh"
#অন্তর্ভাৱ "hb-ot-layout-common.hh"
#অন্তর্ভাৱ "hb-ot-layout-gdef-table.hh"
#অন্তর্ভাৱ "hb-depend-data.hh"


নামাকাশ OT {


সংরচনা hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, তার্কিক>
{
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { ঘূৰাই obj.intersects (এতত্->glyphs); }
  স্থির return_t default_return_মান () { ঘূৰাই মিছা; }
  তার্কিক stop_sublookup_iteration (return_t r) ধ্রুৱ { ঘূৰাই r; }

  ধ্রুৱ hb_set_t *glyphs;

  hb_intersects_context_t (ধ্রুৱ hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

সংরচনা hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, তার্কিক>
{
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { ঘূৰাই obj.may_have_non_1to1 (); }
  স্থির return_t default_return_মান () { ঘূৰাই মিছা; }
  তার্কিক stop_sublookup_iteration (return_t r) ধ্রুৱ { ঘূৰাই r; }
};

সংরচনা hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  প্রকারনির্দেশ return_t (*recurse_func_t) (hb_closure_context_t *c, unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index);
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { obj.closure (এতত্); ঘূৰাই hb_empty_t (); }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }
  শূন্য recurse (unsigned lookup_index, hb_set_t *covered_seq_indicies, unsigned seq_index, unsigned end_index)
  {
    যদি (unlikely (nesting_level_বাম == 0 || !recurse_func))
      ঘূৰাই;

    nesting_level_বাম--;
    recurse_func (এতত্, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_বাম++;
  }

  শূন্য reset_lookup_visit_গুনতি ()
  { lookup_গুনতি = 0; }

  তার্কিক lookup_limit_exceeded ()
  { ঘূৰাই lookup_গুনতি > HB_MAX_LOOKUP_VISIT_গুনতি; }

  তার্কিক should_visit_lookup (unsigned পূর্ণাঙ্ক lookup_index)
  {
    যদি (lookup_গুনতি++ > HB_MAX_LOOKUP_VISIT_গুনতি)
      ঘূৰাই মিছা;

    যদি (is_lookup_done (lookup_index))
      ঘূৰাই মিছা;

    ঘূৰাই সঁচা;
  }

  তার্কিক is_lookup_done (unsigned পূর্ণাঙ্ক lookup_index)
  {
    যদি (unlikely (done_lookups_glyph_গুনতি->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      ঘূৰাই সঁচা;

    /* Have we visited this lookup with the current set of glyphs? */
    unsigned বেৰ_কৰো = glyphs->get_population ();
    যদি (unlikely (বেৰ_কৰো == HB_MAP_মান_INVALID)) // Avoid sentinel interference.
      বেৰ_কৰো--;
    যদি (done_lookups_glyph_গুনতি->get (lookup_index) != বেৰ_কৰো)
    {
      done_lookups_glyph_গুনতি->set (lookup_index, বেৰ_কৰো);

      যদি (!done_lookups_glyph_set->has (lookup_index))
      {
	যদি (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  ঘূৰাই সঁচা;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    যদি (unlikely (covered_glyph_set->in_error ()))
      ঘূৰাই সঁচা;
    যদি (parent_active_glyphs ().is_subset (*covered_glyph_set))
      ঘূৰাই সঁচা;

    covered_glyph_set->union_ (parent_active_glyphs ());
    ঘূৰাই মিছা;
  }

  ধ্রুৱ hb_set_t& previous_parent_active_glyphs () {
    যদি (active_glyphs_স্তূপ.length <= 1)
      ঘূৰাই *glyphs;

    ঘূৰাই active_glyphs_স্তূপ[active_glyphs_স্তূপ.length - 2];
  }

  ধ্রুৱ hb_set_t& parent_active_glyphs ()
  {
    যদি (!active_glyphs_স্তূপ)
      ঘূৰাই *glyphs;

    ঘূৰাই active_glyphs_স্তূপ.tail ();
  }

  hb_set_t* ঠেলো_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_স্তূপ.push ();
    যদি (unlikely (active_glyphs_স্তূপ.in_error ()))
      ঘূৰাই nullptr;
    ঘূৰাই s;
  }

  তার্কিক বেৰ_কৰো_cur_done_glyphs ()
  {
    যদি (!active_glyphs_স্তূপ)
      ঘূৰাই মিছা;

    active_glyphs_স্তূপ.বেৰ_কৰো ();
    ঘূৰাই সঁচা;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_স্তূপ;
  recurse_func_t recurse_func = nullptr;
  unsigned পূর্ণাঙ্ক nesting_level_বাম;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_গুনতি_2,
			hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			unsigned পূর্ণাঙ্ক nesting_level_বাম_2 = HB_MAX_NESTING_LEVEL) :
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
    active_glyphs_স্তূপ.বেৰ_কৰো ();
    active_glyphs_স্তূপ.reset ();
  }

  গুপ্ত:
  hb_map_t *done_lookups_glyph_গুনতি;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  unsigned পূর্ণাঙ্ক lookup_গুনতি = 0;
};



সংরচনা hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  প্রকারনির্দেশ return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, unsigned lookup_index);
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { obj.closure_lookups (এতত্); ঘূৰাই hb_empty_t (); }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }
  শূন্য recurse (unsigned lookup_index)
  {
    যদি (unlikely (nesting_level_বাম == 0 || !recurse_func))
      ঘূৰাই;

    /* Return if new lookup was recursed to before. */
    যদি (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      ঘূৰাই;

    nesting_level_বাম--;
    recurse_func (এতত্, lookup_index);
    nesting_level_বাম++;
  }

  শূন্য set_lookup_visited (unsigned lookup_index)
  { visited_lookups->add (lookup_index); }

  শূন্য set_lookup_inactive (unsigned lookup_index)
  { inactive_lookups->add (lookup_index); }

  তার্কিক lookup_limit_exceeded ()
  {
    তার্কিক ret = lookup_গুনতি > HB_MAX_LOOKUP_VISIT_গুনতি;
    যদি (ret)
      DEBUG_বাৰ্তা (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    ঘূৰাই ret; }

  তার্কিক is_lookup_visited (unsigned lookup_index)
  {
    যদি (unlikely (lookup_গুনতি++ > HB_MAX_LOOKUP_VISIT_গুনতি))
    {
      DEBUG_বাৰ্তা (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_গুনতি, lookup_index);
      ঘূৰাই সঁচা;
    }

    যদি (unlikely (visited_lookups->in_error ()))
      ঘূৰাই সঁচা;

    ঘূৰাই visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  ধ্রুৱ hb_set_t *glyphs;
  recurse_func_t recurse_func;
  unsigned পূর্ণাঙ্ক nesting_level_বাম;

  hb_closure_lookups_context_t (hb_face_t *face_,
				ধ্রুৱ hb_set_t *glyphs_,
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

  গুপ্ত:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  unsigned পূর্ণাঙ্ক lookup_গুনতি;
};

সংরচনা hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, তার্কিক>
{
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { ঘূৰাই obj.would_apply (এতত্); }
  স্থির return_t default_return_মান () { ঘূৰাই মিছা; }
  তার্কিক stop_sublookup_iteration (return_t r) ধ্রুৱ { ঘূৰাই r; }

  hb_face_t *face;
  ধ্রুৱ hb_codepoint_t *glyphs;
  unsigned পূর্ণাঙ্ক len;
  তার্কিক zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ধ্রুৱ hb_codepoint_t *glyphs_,
			    unsigned পূর্ণাঙ্ক len_,
			    তার্কিক zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

সংরচনা hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  প্রকারনির্দেশ return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, unsigned পূর্ণাঙ্ক lookup_index);
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { obj.collect_glyphs (এতত্); ঘূৰাই hb_empty_t (); }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }
  শূন্য recurse (unsigned পূর্ণাঙ্ক lookup_index)
  {
    যদি (unlikely (nesting_level_বাম == 0 || !recurse_func))
      ঘূৰাই;

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
      ঘূৰাই;

    /* Return if new lookup was recursed to before. */
    যদি (recursed_lookups->has (lookup_index))
      ঘূৰাই;

    hb_set_t *old_before = before;
    hb_set_t *old_প্ৰবেশ  = প্ৰবেশ;
    hb_set_t *old_after  = after;
    before = প্ৰবেশ = after = hb_set_get_empty ();

    nesting_level_বাম--;
    recurse_func (এতত্, lookup_index);
    nesting_level_বাম++;

    before = old_before;
    প্ৰবেশ  = old_প্ৰবেশ;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *প্ৰবেশ;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  unsigned পূর্ণাঙ্ক nesting_level_বাম;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_প্ৰবেশ,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       unsigned পূর্ণাঙ্ক nesting_level_বাম_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      প্ৰবেশ  (glyphs_প্ৰবেশ  ? glyphs_প্ৰবেশ  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_বাম (nesting_level_বাম_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  শূন্য set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

সংরচনা hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  গণনা glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  স্থির constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  সংরচনা glyph_set_cache_চাবি_t
  {
    glyph_set_cache_চাবি_t () = স্ৱভাৱ;
    glyph_set_cache_চাবি_t (ধ্রুৱ শূন্য *data_, unsigned মান_3, glyph_set_type_t type_)
      : data (data_), value (মান_3), type (type_) {}

    তার্কিক operator == (ধ্রুৱ glyph_set_cache_চাবি_t &o) ধ্রুৱ
    { ঘূৰাই data == o.data && value == o.value && type == o.type; }

    uint32_t hash () ধ্রুৱ
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      ঘূৰাই current * 31 + hb_hash ((unsigned) type);
    }

    ধ্রুৱ শূন্য *data = nullptr;
    unsigned value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  সংরচনা context_set_cache_চাবি_t
  {
    context_set_cache_চাবি_t () = স্ৱভাৱ;
    context_set_cache_চাবি_t (ধ্রুৱ শূন্য *rule_,
			     ধ্রুৱ শূন্য *record_,
			     ধ্রুৱ শূন্য *data_,
			     hb_codepoint_t active_glyphs_,
			     unsigned মান_3,
			     unsigned format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (মান_3), format (format_) {}

    তার্কিক operator == (ধ্রুৱ context_set_cache_চাবি_t &o) ধ্রুৱ
    {
      ঘূৰাই rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ধ্রুৱ
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      ঘূৰাই current * 31 + hb_hash (format);
    }

    ধ্রুৱ শূন্য *rule = nullptr;
    ধ্রুৱ শূন্য *record = nullptr;
    ধ্রুৱ শূন্য *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    unsigned value = 0;
    unsigned format = 0;
  };

  সংরচনা recurse_চাবি_t
  {
    recurse_চাবি_t () = স্ৱভাৱ;
    recurse_চাবি_t (unsigned lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    তার্কিক operator == (ধ্রুৱ recurse_চাবি_t &o) ধ্রুৱ
    {
      ঘূৰাই lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () ধ্রুৱ
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      ঘূৰাই current;
    }

    unsigned lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  প্রকারনির্দেশ return_t (*recurse_func_t) (hb_depend_context_t *c, unsigned lookup_index, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { obj.depend (এতত্); ঘূৰাই hb_empty_t (); }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }

  শূন্য recurse (unsigned lookup_idx, hb_bit_page_t *covered_seq_indices, unsigned seq_index, unsigned end_index)
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
    যদি (lookups_seen.has (lookup_idx)) ঘূৰাই;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_চাবি = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    যদি (!recurse_path_to_index.has (path_চাবি, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      যদি (unlikely (!recurse_path_to_index.set (path_চাবি, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	ঘূৰাই;
      }
    }
    অন্যথা
      path_index = *path;
    recurse_path = path_index;
    recurse_func (এতত্, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  সংরচনা active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    তার্কিক use_মূল = মিছা;
  };

  ধ্রুৱ hb_set_t& parent_active_glyphs ()
  {
    যদি (!active_glyphs_স্তূপ_depth)
      ঘূৰাই *glyphs;

    ধ্রুৱ active_glyphs_frame_t &frame = active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth - 1];
    ঘূৰাই frame.use_মূল ? *glyphs : frame.glyphs;
  }

  hb_set_t* ঠেলো_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    যদি (active_glyphs_স্তূপ_depth == active_glyphs_স্তূপ.length &&
	unlikely (!active_glyphs_স্তূপ.ঠেলো_or_fail ()))
      ঘূৰাই nullptr;

    active_glyphs_frame_t &frame = active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_মূল = মিছা;
    ঘূৰাই &frame.glyphs;
  }

  তার্কিক ঠেলো_মূল_active_glyphs ()
  {
    যদি (active_glyphs_স্তূপ_depth == active_glyphs_স্তূপ.length &&
	unlikely (!active_glyphs_স্তূপ.ঠেলো_or_fail ()))
      ঘূৰাই মিছা;

    active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth++].use_মূল = সঁচা;
    ঘূৰাই সঁচা;
  }

  তার্কিক বেৰ_কৰো_cur_done_glyphs ()
  {
    যদি (!active_glyphs_স্তূপ_depth)
      ঘূৰাই মিছা;

    active_glyphs_স্তূপ_depth--;
    ঘূৰাই সঁচা;
  }

  তার্কিক get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    যদি (!active_glyphs_স্তূপ_depth ||
	active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth - 1].use_মূল)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      ঘূৰাই সঁচা;
    }

    active_glyphs_frame_t &frame = active_glyphs_স্তূপ[active_glyphs_স্তূপ_depth - 1];
    *active_idx = frame.active_idx;
    যদি (*active_idx != HB_CODEPOINT_INVALID)
      ঘূৰাই সঁচা;

    hb_codepoint_t glyph;
    যদি (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    অন্যথা যদি (!find_or_create_recurse_set (frame.glyphs, active_idx))
      ঘূৰাই মিছা;

    frame.active_idx = *active_idx;
    ঘূৰাই সঁচা;
  }

  তার্কিক get_context_set_cache_চাবি (ধ্রুৱ শূন্য *rule,
				  ধ্রুৱ শূন্য *record,
				  ধ্রুৱ শূন্য *data,
				  unsigned value,
				  unsigned format,
				  context_set_cache_চাবি_t *key)
  {
    hb_codepoint_t active_idx;
    যদি (!get_parent_active_glyphs_index (&active_idx))
      ঘূৰাই মিছা;
    *key = context_set_cache_চাবি_t (rule, record, data, active_idx, value, format);
    ঘূৰাই সঁচা;
  }

  তার্কিক get_recurse_চাবি (unsigned lookup_idx, recurse_চাবি_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    যদি (!get_parent_active_glyphs_index (&active_idx))
      ঘূৰাই মিছা;

    *key = recurse_চাবি_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    ঘূৰাই !completed_recursions.has (*key);
  }

  শূন্য finish_recurse (ধ্রুৱ recurse_চাবি_t &key)
  { depend_data->check_success (completed_recursions.set (key, সঁচা)); }

  শূন্য reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  তার্কিক find_or_create_recurse_set (ধ্রুৱ hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    যদি (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      ঘূৰাই সঁচা;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    যদি (unlikely (!copy))
      ঘূৰাই depend_data->fail ();
    copy->set (set);
    যদি (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      ঘূৰাই depend_data->fail ();
    }

    *index = recurse_sets.length;
    যদি (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      ঘূৰাই depend_data->fail ();
    }
    যদি (unlikely (!recurse_sets.ঠেলো_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      ঘূৰাই depend_data->fail ();
    যদি (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      ঘূৰাই depend_data->fail ();
    ঘূৰাই সঁচা;
  }

  ধ্রুৱ hb_set_t *find_or_create_glyph_set (ধ্রুৱ শূন্য *data,
                                             unsigned value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (ধ্রুৱ hb_set_t *set);

  সংরচনা depend_scratch_t
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
      প্ৰবেশ_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<ধ্রুৱ hb_set_t *> backtrack_sets;
    hb_vector_t<ধ্রুৱ hb_set_t *> lookahead_sets;
    hb_vector_t<ধ্রুৱ hb_set_t *> প্ৰবেশ_position_glyphs;
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
      scratch = (depend_scratch_t *) hb_malloc (পরিমাণ (depend_scratch_t));
      যদি (unlikely (!scratch))
      {
        depend_data->fail ();
        ঘূৰাই nullptr;
      }
      নূতন (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    ঘূৰাই scratch;
  }

  শূন্য release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_স্তূপ;
  unsigned active_glyphs_স্তূপ_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<ধ্রুৱ hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_চাবি_t, তার্কিক> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_চাবি_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_চাবি_t, hb_codepoint_t> context_set_cache;
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
    যেতিয়ালৈ (depend_scratch_pool)
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
সংরচনা hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, ধ্রুৱ Coverage &>
{
  প্রকারনির্দেশ ধ্রুৱ Coverage &return_t; // Stoopid that we have to dupe this here.
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { ঘূৰাই obj.get_coverage (); }
  স্থির return_t default_return_মান () { ঘূৰাই Null (Coverage); }
  তার্কিক stop_sublookup_iteration (return_t r) ধ্রুৱ
  {
    r.collect_coverage (set);
    ঘূৰাই মিছা;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

সংরচনা matcher_t
{
  প্রকারনির্দেশ তার্কিক (*match_func_t) (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data);

  template <typename context_t>
  শূন্য init (ধ্রুৱ context_t *c, তার্কিক context_match = মিছা)
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

  শূন্য set_match_func (match_func_t match_func_,
		       ধ্রুৱ শূন্য *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  গণনা may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_data) ধ্রুৱ
  {
    যদি (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      ঘূৰাই MATCH_NO;

    যদি (match_func)
      ঘূৰাই match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    ঘূৰাই MATCH_MAYBE;
  }

  গণনা may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  template <typename context_t>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_skip_t may_skip (ধ্রুৱ context_t *c,
		       ধ্রুৱ hb_glyph_info_t &info) ধ্রুৱ
  {
    যদি (!c->check_glyph_property (&info, lookup_props))
      ঘূৰাই SKIP_YES;

    যদি (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      ঘূৰাই SKIP_MAYBE;

    ঘূৰাই SKIP_NO;
  }

  সার্ৱজনিক:
  unsigned পূর্ণাঙ্ক lookup_props = 0;
  hb_mask_t mask = -1;
  তার্কিক ignore_zwnj = মিছা;
  তার্কিক ignore_zwj = মিছা;
  তার্কিক ignore_hidden = মিছা;
  তার্কিক per_syllable = মিছা;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  ধ্রুৱ শূন্য *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
সংরচনা glyph_data_t
{
  template <typename T>
  glyph_data_t (ধ্রুৱ T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (পরিমাণ (T) == 3)
#endif
  {
    static_assert (পরিমাণ (T) == 2 || পরিমাণ (T) == 3, "invalid glyph data size");
  }

  unsigned operator [] (unsigned i) ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (wide)
      ঘূৰাই ((ধ্রুৱ HBUINT24 *) data)[i];
#endif
    ঘূৰাই ((ধ্রুৱ HBUINT16 *) data)[i];
  }

  ধ্রুৱ শূন্য *data;
#ifndef HB_NO_BEYOND_64K
  তার্কিক wide;
#endif
};

template <typename context_t>
সংরচনা skipping_iterator_t
{
  শূন্য init (context_t *c_, তার্কিক context_match = মিছা)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  শূন্য set_lookup_props (unsigned পূর্ণাঙ্ক lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  শূন্য set_match_func (matcher_t::match_func_t match_func_,
		       ধ্রুৱ শূন্য *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  শূন্য set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (ধ্রুৱ HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (ধ্রুৱ HBUINT24 *) glyph_data.data : nullptr;
#অন্যথা
    match_glyph_data16 = (ধ্রুৱ HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  শূন্য reset (unsigned পূর্ণাঙ্ক start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  শূন্য reset_back (unsigned পূর্ণাঙ্ক start_index_, তার্কিক from_out_buffer = মিছা)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  শূন্য reset_fast (unsigned পূর্ণাঙ্ক start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (ধ্রুৱ hb_glyph_info_t &info) ধ্রুৱ
  { ঘূৰাই matcher.may_skip (c, info); }

  গণনা match_t {
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
    যদি (unlikely (skip == matcher_t::SKIP_YES))
      ঘূৰাই SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    যদি (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      ঘূৰাই MATCH;

    যদি (skip == matcher_t::SKIP_NO)
      ঘূৰাই NOT_MATCH;

    ঘূৰাই SKIP;
  }

  তার্কিক next (unsigned *unsafe_to = nullptr)
  {
    auto *info = c->buffer->info;
    ধ্রুৱ signed stop = (signed) end - 1;
    যেতিয়ালৈ ((signed) idx < stop)
    {
      idx++;
      ৱিকল্প (match (info[idx]))
      {
	পক্ষ MATCH:
	{
	  advance_glyph_data ();
	  ঘূৰাই সঁচা;
	}
	পক্ষ NOT_MATCH:
	{
	  যদি (unsafe_to)
	    *unsafe_to = idx + 1;
	  ঘূৰাই মিছা;
	}
	পক্ষ SKIP:
	  অনুৱর্ত;
      }
    }
    যদি (unsafe_to)
      *unsafe_to = end;
    ঘূৰাই মিছা;
  }
  তার্কিক prev (unsigned *unsafe_from = nullptr)
  {
    auto *out_info = c->buffer->out_info;
    ধ্রুৱ unsigned stop = 0;
    যেতিয়ালৈ (idx > stop)
    {
      idx--;
      ৱিকল্প (match (out_info[idx]))
      {
	পক্ষ MATCH:
	{
	  advance_glyph_data ();
	  ঘূৰাই সঁচা;
	}
	পক্ষ NOT_MATCH:
	{
	  যদি (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  ঘূৰাই মিছা;
	}
	পক্ষ SKIP:
	  অনুৱর্ত;
      }
    }
    যদি (unsafe_from)
      *unsafe_from = 0;
    ঘূৰাই মিছা;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    যদি (match_glyph_data16) ঘূৰাই *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    অন্যথা
    যদি (match_glyph_data24) ঘূৰাই *match_glyph_data24;
#endif
    ঘূৰাই 0;
  }
  HB_ALWAYS_INLINE
  শূন্য
  advance_glyph_data ()
  {
    যদি (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    অন্যথা
    যদি (match_glyph_data24) match_glyph_data24++;
#endif
  }

  unsigned পূর্ণাঙ্ক idx;
  রক্ষিত:
  context_t *c;
  matcher_t matcher;
  ধ্রুৱ HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  ধ্রুৱ HBUINT24 *match_glyph_data24;
#endif

  unsigned পূর্ণাঙ্ক end;
};

সংরচনা hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, তার্কিক, HB_DEBUG_APPLY>
{
  ধ্রুৱ ৱর্ণ *get_name () { ঘূৰাই "APPLY"; }
  প্রকারনির্দেশ return_t (*recurse_func_t) (hb_ot_apply_context_t *c, unsigned পূর্ণাঙ্ক lookup_index);

  template <typename T>
  স্থির অন্তঃস্থ auto apply_ (ধ্রুৱ T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  template <typename T>
  স্থির অন্তঃস্থ auto apply_ (ধ্রুৱ T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj) { ঘূৰাই apply_(obj, এতত্, hb_prioritize); }

  স্থির return_t default_return_মান () { ঘূৰাই মিছা; }
  তার্কিক stop_sublookup_iteration (return_t r) ধ্রুৱ { ঘূৰাই r; }
  return_t recurse (unsigned পূর্ণাঙ্ক sub_lookup_index)
  {
    প্রতিজ্ঞা (recurse_func);
    যদি (unlikely (nesting_level_বাম == 0))
    {
      buffer->successful = মিছা;
      ঘূৰাই default_return_মান ();
    }

    buffer->max_ops--;
    যদি (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = মিছা;
      ঘূৰাই default_return_মান ();
    }

    nesting_level_বাম--;
    তার্কিক ret = recurse_func (এতত্, sub_lookup_index);
    nesting_level_বাম++;
    ঘূৰাই ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_প্ৰবেশ, iter_context;

  unsigned পূর্ণাঙ্ক table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  ধ্রুৱ GDEF &gdef;
  ধ্রুৱ GDEF::accelerator_t &gdef_accel;
  ধ্রুৱ ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  unsigned পূর্ণাঙ্ক lookup_index = (unsigned) -1;
  unsigned পূর্ণাঙ্ক lookup_props = 0;
  unsigned পূর্ণাঙ্ক nesting_level_বাম = HB_MAX_NESTING_LEVEL;

  তার্কিক has_glyph_classes;
  তার্কিক auto_zwnj = সঁচা;
  তার্কিক auto_zwj = সঁচা;
  তার্কিক per_syllable = মিছা;
  তার্কিক random = মিছা;
  unsigned new_syllables = (unsigned) -1;

  signed last_base = -1; // GPOS uses
  unsigned last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t স্তূপ_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  শূন্য *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (unsigned পূর্ণাঙ্ক table_index_,
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
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (স্তূপ_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    যদি (buffer->messaging ())
    {
      প্রতিজ্ঞা (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = এতত্;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    যদি (buffer->messaging ())
    {
      প্রতিজ্ঞা (buffer->changed_func == buffer_changed_trampoline);
      প্রতিজ্ঞা (buffer->changed_data == এতত্);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  শূন্য init_iters ()
  {
    iter_প্ৰবেশ.init (এতত্, মিছা);
    iter_context.init (এতত্, সঁচা);
  }

  শূন্য set_lookup_mask (hb_mask_t mask, তার্কিক init = সঁচা) { lookup_mask = mask; last_base = -1; last_base_until = 0; যদি (init) init_iters (); }
  শূন্য set_auto_zwj (তার্কিক auto_zwj_, তার্কিক init = সঁচা) { auto_zwj = auto_zwj_; যদি (init) init_iters (); }
  শূন্য set_auto_zwnj (তার্কিক auto_zwnj_, তার্কিক init = সঁচা) { auto_zwnj = auto_zwnj_; যদি (init) init_iters (); }
  শূন্য set_per_syllable (তার্কিক per_syllable_, তার্কিক init = সঁচা) { per_syllable = per_syllable_; যদি (init) init_iters (); }
  শূন্য set_random (তার্কিক random_) { random = random_; }
  শূন্য set_recurse_func (recurse_func_t func) { recurse_func = func; }
  শূন্য set_lookup_index (unsigned পূর্ণাঙ্ক lookup_index_) { lookup_index = lookup_index_; }
  শূন্য set_lookup_props (unsigned পূর্ণাঙ্ক lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_সংখ্যা ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    ঘূৰাই buffer->random_state;
  }

  স্থির শূন্য buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, শূন্য *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  শূন্য buffer_changed ()
  {
    buffer->update_digest ();
    যদি (likely (has_glyph_classes))
      বাবে (unsigned i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  তার্কিক match_properties_mark (ধ্রুৱ hb_glyph_info_t *info,
			      unsigned পূর্ণাঙ্ক    glyph_props,
			      unsigned পূর্ণাঙ্ক    match_props) ধ্রুৱ
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    যদি (match_props & LookupFlag::UseMarkFilteringSet)
    {
      unsigned set_index = match_props >> 16;
      ঘূৰাই gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    যদি (match_props & LookupFlag::MarkAttachmentType)
      ঘূৰাই (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    ঘূৰাই সঁচা;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  তার্কিক check_glyph_property (ধ্রুৱ hb_glyph_info_t *info,
			     unsigned match_props) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    যদি (glyph_props & match_props & LookupFlag::IgnoreFlags)
      ঘূৰাই মিছা;

    যদি (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      ঘূৰাই match_properties_mark (info, glyph_props, match_props);

    ঘূৰাই সঁচা;
  }

  শূন্য _set_glyph_class (hb_codepoint_t glyph_index,
			  unsigned পূর্ণাঙ্ক class_guess = 0,
			  তার্কিক ligature = মিছা,
			  তার্কিক component = মিছা)
  {
    buffer->digest.add (glyph_index);

    যদি (new_syllables != (unsigned) -1)
      buffer->cur().syllable() = new_syllables;

    unsigned পূর্ণাঙ্ক props = _hb_glyph_info_get_glyph_props (&buffer->cur());
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
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    অন্যথা যদি (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    অন্যথা
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  শূন্য _set_glyph_class_props (hb_glyph_info_t &info,
			       unsigned পূর্ণাঙ্ক props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  শূন্য _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  শূন্য replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (শূন্য) buffer->replace_glyph (glyph_index);
  }
  শূন্য replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  শূন্য replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    unsigned পূর্ণাঙ্ক class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, সঁচা);
    (শূন্য) buffer->replace_glyph (glyph_index);
  }
  শূন্য output_glyph_for_component (hb_codepoint_t glyph_index,
				   unsigned পূর্ণাঙ্ক class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, মিছা, সঁচা);
    (শূন্য) buffer->output_glyph (glyph_index);
  }
};

গণনা শ্ৰেণী hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

সংরচনা hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  template <typename T>
  স্থির অন্তঃস্থ auto collect_second_glyphs_ (ধ্রুৱ T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (শূন্য, obj.collect_second_glyphs (digest) )
  template <typename T>
  স্থির অন্তঃস্থ শূন্য collect_second_glyphs_ (ধ্রুৱ T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  template <typename T>
  স্থির অন্তঃস্থ auto apply_ (ধ্রুৱ T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<1>) HB_RETURN (তার্কিক, obj->apply (c, external_cache) )
  template <typename T>
  স্থির অন্তঃস্থ auto apply_ (ধ্রুৱ T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<0>) HB_RETURN (তার্কিক, obj->apply (c) )
  template <typename T>
  স্থির অন্তঃস্থ তার্কিক apply_to (ধ্রুৱ শূন্য *obj, hb_ot_apply_context_t *c, শূন্য *external_cache)
  {
    ধ্রুৱ T *typed_obj = (ধ্রুৱ T *) obj;
    ঘূৰাই apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  template <typename T>
  স্থির অন্তঃস্থ auto apply_cached_ (ধ্রুৱ T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<2>) HB_RETURN (তার্কিক, obj->apply_cached (c, external_cache) )
  template <typename T>
  স্থির অন্তঃস্থ auto apply_cached_ (ধ্রুৱ T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<1>) HB_RETURN (তার্কিক, obj->apply (c, external_cache) )
  template <typename T>
  স্থির অন্তঃস্থ auto apply_cached_ (ধ্রুৱ T *obj, hb_ot_apply_context_t *c, শূন্য *external_cache, hb_priority<0>) HB_RETURN (তার্কিক, obj->apply (c) )
  template <typename T>
  স্থির অন্তঃস্থ তার্কিক apply_cached_to (ধ্রুৱ শূন্য *obj, hb_ot_apply_context_t *c, শূন্য *external_cache)
  {
    ধ্রুৱ T *typed_obj = (ধ্রুৱ T *) obj;
    ঘূৰাই apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  template <typename T>
  স্থির অন্তঃস্থ auto cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (তার্কিক, T::cache_func (c, op) )
  template <typename T=শূন্য>
  স্থির অন্তঃস্থ তার্কিক cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { ঘূৰাই মিছা; }
  template <typename Type>
  স্থির অন্তঃস্থ তার্কিক cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    ঘূৰাই cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  প্রকারনির্দেশ তার্কিক (*hb_apply_func_t) (ধ্রুৱ শূন্য *obj, hb_ot_apply_context_t *c, শূন্য *external_cache);
  প্রকারনির্দেশ তার্কিক (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  সংরচনা hb_applicable_t
  {
    friend সংরচনা hb_accelerate_subtables_context_t;
    friend সংরচনা hb_ot_layout_lookup_accelerator_t;

    template <typename T>
    শূন্য init (ধ্রুৱ T &obj_,
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
    তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
    {
      ঘূৰাই digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    তার্কিক apply_no_digest (hb_ot_apply_context_t *c) ধ্রুৱ
    {
      ঘূৰাই apply_func (obj, c, nullptr);
    }
#অন্যথা
    তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
    {
      ঘূৰাই digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    তার্কিক apply_cached (hb_ot_apply_context_t *c) ধ্রুৱ
    {
      ঘূৰাই digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    তার্কিক apply_no_digest (hb_ot_apply_context_t *c) ধ্রুৱ
    {
      ঘূৰাই apply_func (obj, c, external_cache);
    }
    তার্কিক apply_cached_no_digest (hb_ot_apply_context_t *c) ধ্রুৱ
    {
      ঘূৰাই apply_cached_func (obj, c, external_cache);
    }

    তার্কিক cache_enter (hb_ot_apply_context_t *c) ধ্রুৱ
    {
      ঘূৰাই cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    শূন্য cache_leave (hb_ot_apply_context_t *c) ধ্রুৱ
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    গুপ্ত:
    ধ্রুৱ শূন্য *obj;
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
  auto cache_cost (ধ্রুৱ T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  template <typename T>
  auto cache_cost (ধ্রুৱ T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  template <typename T>
  auto external_cache_create (ধ্রুৱ T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  template <typename T>
  auto external_cache_create (ধ্রুৱ T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  template <typename T>
  return_t dispatch (ধ্রুৱ T &obj)
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

    ঘূৰাই hb_empty_t ();
  }
  স্থির return_t default_return_মান () { ঘূৰাই hb_empty_t (); }

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


প্রকারনির্দেশ তার্কিক (*intersects_func_t) (ধ্রুৱ hb_set_t *glyphs, unsigned value, ধ্রুৱ শূন্য *data, শূন্য *cache);
প্রকারনির্দেশ শূন্য (*intersected_glyphs_func_t) (ধ্রুৱ hb_set_t *glyphs, ধ্রুৱ শূন্য *data, unsigned value, hb_set_t *intersected_glyphs, শূন্য *cache);
প্রকারনির্দেশ শূন্য (*collect_glyphs_func_t) (hb_set_t *glyphs, unsigned value, ধ্রুৱ শূন্য *data);
প্রকারনির্দেশ তার্কিক (*match_func_t) (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data);
প্রকারনির্দেশ unsigned (*get_মান_func_t) (hb_glyph_info_t &info, ধ্রুৱ শূন্য *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
সংরচনা hb_ot_layout_ruleset_digest_t
{
  শূন্য init () { mask = 0; }
  শূন্য set_full () { mask = (uint64_t) -1; }
  শূন্য add (unsigned value) { mask |= (uint64_t) 1 << (value & 63); }
  তার্কিক may_have (unsigned value) ধ্রুৱ { ঘূৰাই mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

সংরচনা ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
সংরচনা ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
সংরচনা ContextApplyFuncs
{
  match_func_t match;
};
সংরচনা ChainContextApplyFuncs
{
  match_func_t match[3];
};


স্থির অন্তঃস্থ তার্কিক intersects_glyph (ধ্রুৱ hb_set_t *glyphs, unsigned value, ধ্রুৱ শূন্য *data HB_UNUSED, শূন্য *cache HB_UNUSED)
{
  ঘূৰাই glyphs->has (value);
}
স্থির অন্তঃস্থ তার্কিক intersects_class (ধ্রুৱ hb_set_t *glyphs, unsigned value, ধ্রুৱ শূন্য *data, শূন্য *cache)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  যদি (map->has (value, &cached_v))
    ঘূৰাই *cached_v;

  তার্কিক v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  ঘূৰাই v;
}
স্থির অন্তঃস্থ তার্কিক intersects_coverage (ধ্রুৱ hb_set_t *glyphs, unsigned value, ধ্রুৱ শূন্য *data, শূন্য *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  ঘূৰাই (data+coverage).intersects (glyphs);
}


স্থির অন্তঃস্থ শূন্য intersected_glyph (ধ্রুৱ hb_set_t *glyphs HB_UNUSED, ধ্রুৱ শূন্য *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED শূন্য *cache)
{
  unsigned g = reinterpret_cast<ধ্রুৱ HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<unsigned, hb_set_t>;

স্থির অন্তঃস্থ শূন্য intersected_class_glyphs (ধ্রুৱ hb_set_t *glyphs, ধ্রুৱ শূন্য *data, unsigned value, hb_set_t *intersected_glyphs, শূন্য *cache)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  যদি (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    ঘূৰাই;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

স্থির অন্তঃস্থ শূন্য intersected_coverage_glyphs (ধ্রুৱ hb_set_t *glyphs, ধ্রুৱ শূন্য *data, unsigned value, hb_set_t *intersected_glyphs, HB_UNUSED শূন্য *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


স্থির অন্তঃস্থ তার্কিক array_is_subset_of (ধ্রুৱ hb_set_t *glyphs,
				       unsigned পূর্ণাঙ্ক গুনতি,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       ধ্রুৱ শূন্য *intersects_data,
				       শূন্য *cache)
{
  বাবে (unsigned i = 0; i < গুনতি; i++)
    যদি (!intersects_func (glyphs, values[i], intersects_data, cache)) ঘূৰাই মিছা;
  ঘূৰাই সঁচা;
}


স্থির অন্তঃস্থ শূন্য collect_glyph (hb_set_t *glyphs, unsigned value, ধ্রুৱ শূন্য *data HB_UNUSED)
{
  glyphs->add (value);
}
স্থির অন্তঃস্থ শূন্য collect_class (hb_set_t *glyphs, unsigned value, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
স্থির অন্তঃস্থ শূন্য collect_coverage (hb_set_t *glyphs, unsigned value, ধ্রুৱ শূন্য *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

অন্তঃস্থ ধ্রুৱ hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (ধ্রুৱ শূন্য *data,
                                                unsigned value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_চাবি_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  যদি (glyph_set_to_index.has (key, &existing))
    ঘূৰাই glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  যদি (unlikely (!set))
  {
    depend_data->fail ();
    ঘূৰাই nullptr;
  }

  যদি (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  অন্যথা যদি (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  অন্যথা
    set->add (value);
  যদি (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    ঘূৰাই nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  যদি (unlikely (!glyph_sets.ঠেলো_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    ঘূৰাই nullptr;
  }
  যদি (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    ঘূৰাই nullptr;
  }
  ঘূৰাই glyph_sets[index].get ();
}

অন্তঃস্থ hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (ধ্রুৱ hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  যদি (cached_context_set_indices.has (key, &existing))
    ঘূৰাই *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  যদি (unlikely (index == HB_CODEPOINT_INVALID))
    ঘূৰাই HB_CODEPOINT_INVALID;
  যদি (unlikely (!cached_context_set_indices.set (key, index)))
    ঘূৰাই depend_data->fail_invalid ();
  ঘূৰাই index;
}

স্থির অন্তঃস্থ শূন্য collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  unsigned পূর্ণাঙ্ক গুনতি,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  ধ্রুৱ শূন্য *collect_data)
{
  বাবে (unsigned i = 0; i < গুনতি; i++)
    collect_func (glyphs, values[i], collect_data);
}


স্থির অন্তঃস্থ তার্কিক match_always (hb_glyph_info_t &info HB_UNUSED, unsigned value HB_UNUSED, ধ্রুৱ শূন্য *data HB_UNUSED)
{
  ঘূৰাই সঁচা;
}
স্থির অন্তঃস্থ তার্কিক match_glyph (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data HB_UNUSED)
{
  ঘূৰাই info.codepoint == value;
}
স্থির অন্তঃস্থ তার্কিক match_class (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *>(data);
  ঘূৰাই class_def.get_class (info.codepoint) == value;
}
স্থির অন্তঃস্থ unsigned get_class_মান (hb_glyph_info_t &info, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *> (data);
  ঘূৰাই class_def.get_class (info.codepoint);
}
সংরচনা match_class_cache_data_t
{
  ধ্রুৱ ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
স্থির অন্তঃস্থ তার্কিক match_class_cache (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ auto &cache_data = *reinterpret_cast<ধ্রুৱ match_class_cache_data_t *> (data);
  ঘূৰাই cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
স্থির অন্তঃস্থ unsigned get_class_cache_মান (hb_glyph_info_t &info, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ auto &cache_data = *reinterpret_cast<ধ্রুৱ match_class_cache_data_t *> (data);
  ঘূৰাই cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
স্থির অন্তঃস্থ unsigned get_class_cached (ধ্রুৱ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable();
  যদি (klass < 255)
    ঘূৰাই klass;
  klass = class_def.get_class (info.codepoint);
  যদি (likely (klass < 255))
    info.syllable() = klass;
  ঘূৰাই klass;
}
স্থির অন্তঃস্থ তার্কিক match_class_cached (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *>(data);
  ঘূৰাই get_class_cached (class_def, info) == value;
}
স্থির অন্তঃস্থ unsigned get_class_cached_মান (hb_glyph_info_t &info, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *> (data);
  ঘূৰাই get_class_cached (class_def, info);
}
স্থির অন্তঃস্থ unsigned get_class_cached1 (ধ্রুৱ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = info.syllable() & 0x0F;
  যদি (klass < 15)
    ঘূৰাই klass;
  klass = class_def.get_class (info.codepoint);
  যদি (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  ঘূৰাই klass;
}
স্থির অন্তঃস্থ তার্কিক match_class_cached1 (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *>(data);
  ঘূৰাই get_class_cached1 (class_def, info) == value;
}
স্থির অন্তঃস্থ unsigned get_class_cached2 (ধ্রুৱ ClassDef &class_def, hb_glyph_info_t &info)
{
  unsigned klass = (info.syllable() & 0xF0) >> 4;
  যদি (klass < 15)
    ঘূৰাই klass;
  klass = class_def.get_class (info.codepoint);
  যদি (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  ঘূৰাই klass;
}
স্থির অন্তঃস্থ তার্কিক match_class_cached2 (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *>(data);
  ঘূৰাই get_class_cached2 (class_def, info) == value;
}
স্থির অন্তঃস্থ unsigned get_class_cached2_মান (hb_glyph_info_t &info, ধ্রুৱ শূন্য *data)
{
  ধ্রুৱ ClassDef &class_def = *reinterpret_cast<ধ্রুৱ ClassDef *> (data);
  ঘূৰাই get_class_cached2 (class_def, info);
}
স্থির অন্তঃস্থ তার্কিক match_coverage (hb_glyph_info_t &info, unsigned value, ধ্রুৱ শূন্য *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  ঘূৰাই (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

স্থির অন্তঃস্থ তার্কিক would_match_প্ৰবেশ (hb_would_apply_context_t *c,
				      unsigned পূর্ণাঙ্ক গুনতি, /* Including the first glyph (not matched) */
				      glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      ধ্রুৱ শূন্য *match_data)
{
  যদি (গুনতি != c->len)
    ঘূৰাই মিছা;

  বাবে (unsigned পূর্ণাঙ্ক i = 1; i < গুনতি; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    যদি (likely (!match_func (info, প্ৰবেশ[i - 1], match_data)))
      ঘূৰাই মিছা;
  }

  ঘূৰাই সঁচা;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#অন্যথা
অন্তঃস্থ
#endif
স্থির তার্কিক match_প্ৰবেশ (hb_ot_apply_context_t *c,
			 unsigned পূর্ণাঙ্ক গুনতি, /* Including the first glyph (not matched) */
			 glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 ধ্রুৱ শূন্য *match_data,
			 unsigned পূর্ণাঙ্ক *end_position,
			 unsigned পূর্ণাঙ্ক *p_total_component_গুনতি = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  যদি (গুনতি == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    যদি (p_total_component_গুনতি)
      *p_total_component_গুনতি = hb_glyph_info_get_lig_অঙ্ক_comps (&buffer->cur());
    return_trace (সঁচা);
  }

  যদি (unlikely (গুনতি > HB_MAX_CONTEXT_LENGTH)) return_trace (মিছা);

  auto &skippy_iter = c->iter_প্ৰবেশ;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (প্ৰবেশ);

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

  unsigned পূর্ণাঙ্ক total_component_গুনতি = 0;

  unsigned পূর্ণাঙ্ক first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned পূর্ণাঙ্ক first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  গণনা {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  বাবে (unsigned পূর্ণাঙ্ক i = 1; i < গুনতি; i++)
  {
    unsigned unsafe_to;
    যদি (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (মিছা);
    }

    যদি (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (মিছা);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    unsigned পূর্ণাঙ্ক this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    unsigned পূর্ণাঙ্ক this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

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
	  তার্কিক found = মিছা;
	  ধ্রুৱ auto *out = buffer->out_info;
	  unsigned পূর্ণাঙ্ক j = buffer->out_len;
	  যেতিয়ালৈ (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    যদি (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = সঁচা;
	      ভঙ্গ;
	    }
	    j--;
	  }

	  যদি (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  অন্যথা
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	যদি (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (মিছা);
      }
    }
    অন্যথা
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      যদি (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (মিছা);
    }

    total_component_গুনতি += hb_glyph_info_get_lig_অঙ্ক_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  যদি (p_total_component_গুনতি)
  {
    total_component_গুনতি += hb_glyph_info_get_lig_অঙ্ক_comps (&buffer->cur());
    *p_total_component_গুনতি = total_component_গুনতি;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (সঁচা);
}
স্থির অন্তঃস্থ তার্কিক ligate_প্ৰবেশ (hb_ot_apply_context_t *c,
				 unsigned পূর্ণাঙ্ক গুনতি, /* Including the first glyph */
				 unsigned পূর্ণাঙ্ক match_end,
				 hb_codepoint_t lig_glyph,
				 unsigned পূর্ণাঙ্ক total_component_গুনতি)
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

  তার্কিক is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  তার্কিক is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  বাবে (unsigned পূর্ণাঙ্ক i = 1; i < গুনতি; i++)
    যদি (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = মিছা;
      is_mark_ligature = মিছা;
      ভঙ্গ;
    }
  তার্কিক is_ligature = !is_base_ligature && !is_mark_ligature;

  unsigned পূর্ণাঙ্ক klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  unsigned পূর্ণাঙ্ক lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  unsigned পূর্ণাঙ্ক last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  unsigned পূর্ণাঙ্ক last_অঙ্ক_components = hb_glyph_info_get_lig_অঙ্ক_comps (&buffer->cur());
  unsigned পূর্ণাঙ্ক components_so_far = last_অঙ্ক_components;

  যদি (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_গুনতি);
    যদি (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  বাবে (unsigned পূর্ণাঙ্ক i = 1; i < গুনতি; i++)
  {
    যেতিয়ালৈ (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      যদি (is_ligature)
      {
	unsigned পূর্ণাঙ্ক this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	যদি (this_comp == 0)
	  this_comp = last_অঙ্ক_components;
	প্রতিজ্ঞা (components_so_far >= last_অঙ্ক_components);
	unsigned পূর্ণাঙ্ক new_lig_comp = components_so_far - last_অঙ্ক_components +
				    hb_min (this_comp, last_অঙ্ক_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (শূন্য) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_অঙ্ক_components = hb_glyph_info_get_lig_অঙ্ক_comps_in_ligation (&buffer->cur());
    components_so_far += last_অঙ্ক_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  যদি (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    বাবে (unsigned i = buffer->idx; i < buffer->len; ++i)
    {
      যদি (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) ভঙ্গ;

      unsigned this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      যদি (!this_comp) ভঙ্গ;

      প্রতিজ্ঞা (components_so_far >= last_অঙ্ক_components);
      unsigned new_lig_comp = components_so_far - last_অঙ্ক_components +
			      hb_min (this_comp, last_অঙ্ক_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (সঁচা);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#অন্যথা
অন্তঃস্থ
#endif
স্থির তার্কিক match_backtrack (hb_ot_apply_context_t *c,
			     unsigned পূর্ণাঙ্ক গুনতি,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     ধ্রুৱ শূন্য *match_data,
			     unsigned পূর্ণাঙ্ক *match_start)
{
  TRACE_APPLY (nullptr);

  যদি (!গুনতি)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (সঁচা);
  }

  auto &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
  {
    unsigned unsafe_from;
    যদি (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (মিছা);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (সঁচা);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#অন্যথা
অন্তঃস্থ
#endif
স্থির তার্কিক match_lookahead (hb_ot_apply_context_t *c,
			     unsigned পূর্ণাঙ্ক গুনতি,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     ধ্রুৱ শূন্য *match_data,
			     unsigned পূর্ণাঙ্ক start_index,
			     unsigned পূর্ণাঙ্ক *end_index)
{
  TRACE_APPLY (nullptr);

  যদি (!গুনতি)
  {
    *end_index = start_index;
    return_trace (সঁচা);
  }

  auto &skippy_iter = c->iter_context;
  প্রতিজ্ঞা (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
  {
    unsigned unsafe_to;
    যদি (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (মিছা);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (সঁচা);
}



সংরচনা LookupRecord
{
  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ hb_map_t         *lookup_map) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->embed (*এতত্);
    যদি (unlikely (!out)) return_trace (মিছা);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (4);
};

স্থির অন্তঃস্থ unsigned serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ধ্রুৱ hb_array_t<ধ্রুৱ LookupRecord> lookupRecords,
						     ধ্রুৱ hb_map_t *lookup_map)
{
  unsigned গুনতি = 0;
  বাবে (ধ্রুৱ LookupRecord& r : lookupRecords)
  {
    যদি (!lookup_map->has (r.lookupListIndex))
      অনুৱর্ত;

    যদি (!r.serialize (c, lookup_map))
      ঘূৰাই 0;

    গুনতি++;
  }
  ঘূৰাই গুনতি;
}

গণনা ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

স্থির অন্তঃস্থ ধ্রুৱ hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ধ্রুৱ শূন্য *data,
			unsigned value,
			তার্কিক filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<unsigned> *compact_workspace = nullptr)
{
  ৱিকল্প (context_format)
  {
    পক্ষ ContextFormat::SimpleContext:
      ঘূৰাই c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    পক্ষ ContextFormat::ClassBasedContext:
    {
      ধ্রুৱ hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      যদি (unlikely (!class_set))
	ঘূৰাই nullptr;
      যদি (!filter_class_by_parent)
	ঘূৰাই class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      ভঙ্গ;
    }
    পক্ষ ContextFormat::CoverageBasedContext:
      ঘূৰাই c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  যদি (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    ঘূৰাই nullptr;
  }
  ঘূৰাই scratch;
}

স্থির অন্তঃস্থ শূন্য context_closure_recurse_lookups (hb_closure_context_t *c,
					     unsigned প্ৰবেশ_গুনতি, glyph_data_t প্ৰবেশ,
					     unsigned lookup_গুনতি_2,
					     ধ্রুৱ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     unsigned value,
					     ContextFormat context_format,
					     ধ্রুৱ শূন্য *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     শূন্য *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < lookup_গুনতি_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    যদি (seqIndex >= প্ৰবেশ_গুনতি) অনুৱর্ত;

    তার্কিক has_pos_glyphs = মিছা;

    যদি (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = সঁচা;
      pos_glyphs.clear ();
      যদি (seqIndex == 0)
      {
        ৱিকল্প (context_format) {
        পক্ষ ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          ভঙ্গ;
        পক্ষ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          ভঙ্গ;
        পক্ষ ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          ভঙ্গ;
        }
      }
      অন্যথা
      {
        ধ্রুৱ শূন্য *প্ৰবেশ_data = প্ৰবেশ.data;
        unsigned প্ৰবেশ_মান = seqIndex - 1;
        যদি (context_format != ContextFormat::SimpleContext)
        {
          প্ৰবেশ_data = data;
          প্ৰবেশ_মান = প্ৰবেশ[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, প্ৰবেশ_data, প্ৰবেশ_মান, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs))
      ঘূৰাই;
    যদি (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } অন্যথা {
      *cur_active_glyphs = *c->glyphs;
    }

    unsigned endIndex = প্ৰবেশ_গুনতি;
    যদি (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->বেৰ_কৰো_cur_done_glyphs ();
  }
}

স্থির তার্কিক
context_depend_sets_are_cached (hb_depend_context_t *c,
				unsigned প্ৰবেশ_গুনতি_2,
				unsigned lookup_গুনতি,
				ধ্রুৱ LookupRecord lookup_records[],
				ধ্রুৱ শূন্য *rule,
				unsigned value,
				ContextFormat context_format,
				ধ্রুৱ শূন্য *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  তার্কিক have_active_idx = মিছা;
  বাবে (unsigned i = 0; i < lookup_গুনতি; i++)
  {
    যদি (lookup_records[i].sequenceIndex >= প্ৰবেশ_গুনতি_2)
      অনুৱর্ত;

    যদি (!have_active_idx)
    {
      যদি (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	ঘূৰাই মিছা;
      have_active_idx = সঁচা;
    }

    hb_depend_context_t::context_set_cache_চাবি_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    যদি (!c->context_set_cache.has (key, &cached_context_set))
      ঘূৰাই মিছা;
    যদি (unlikely (!cached_context_sets->ঠেলো_or_fail (*cached_context_set)))
      ঘূৰাই c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  ঘূৰাই have_active_idx;
}

স্থির অন্তঃস্থ শূন্য context_depend_recurse_lookups (hb_depend_context_t *c,
					     unsigned প্ৰবেশ_গুনতি, glyph_data_t প্ৰবেশ,
					     unsigned lookup_গুনতি_2,
					     ধ্রুৱ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     ধ্রুৱ শূন্য *rule,
					     unsigned value,
					     ContextFormat context_format,
					     ধ্রুৱ শূন্য *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     শূন্য *cache,
					     ধ্রুৱ hb_set_t &preliminary_context,
					     ধ্রুৱ hb_vector_t<ধ্রুৱ hb_set_t *> *প্ৰবেশ_position_glyphs,
					     ধ্রুৱ hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  যদি (unlikely (প্ৰবেশ_গুনতি > HB_MAX_CONTEXT_LENGTH))
    ঘূৰাই;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  unsigned cached_context_index = 0;
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < lookup_গুনতি_2; i++)
  {
    unsigned seqIndex = lookupRecord[i].sequenceIndex;
    যদি (seqIndex >= প্ৰবেশ_গুনতি)
      অনুৱর্ত;

    hb_depend_context_t::context_set_cache_চাবি_t context_cache_চাবি;
    যদি (unlikely (!c->get_context_set_cache_চাবি (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_চাবি)))
      ঘূৰাই;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    যদি (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    অন্যথা যদি (c->context_set_cache.has (context_cache_চাবি, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    অন্যথা
    {
      যদি (unlikely (!প্ৰবেশ_position_glyphs))
      {
	c->depend_data->fail ();
	ঘূৰাই;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_মান_INVALID;
      যেতিয়ালৈ (preliminary_context.next (&elem)) {
	যদি (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} অন্যথা {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      বাবে (unsigned j = 0; j < প্ৰবেশ_position_glyphs->length; j++) {
	যদি (j != seqIndex && (*প্ৰবেশ_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*প্ৰবেশ_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_মান_INVALID;
      যেতিয়ালৈ (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	ধ্রুৱ hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	যদি (unlikely (!original_set)) অনুৱর্ত;

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
      বাবে (unsigned j = 0; j < প্ৰবেশ_position_glyphs->length; j++) {
	যদি (j != seqIndex && (*প্ৰবেশ_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  যদি (!(*প্ৰবেশ_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    তার্কিক cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*প্ৰবেশ_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*প্ৰবেশ_position_glyphs)[j]);
	    যদি (unlikely (idx == HB_CODEPOINT_INVALID))
	      ঘূৰাই;
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
	c->depend_data->fail ();
	ঘূৰাই;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      যদি (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	ঘূৰাই;

      যদি (unlikely (!c->context_set_cache.set (context_cache_চাবি, context_set_idx)))
      {
	c->depend_data->fail ();
	ঘূৰাই;
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
    তার্কিক nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

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
    যদি (প্ৰবেশ_গুনতি > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    যদি (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    তার্কিক has_pos_glyphs = মিছা;

    যদি (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = সঁচা;
      pos_glyphs.clear ();
      যদি (seqIndex == 0)
      {
        ৱিকল্প (context_format) {
        পক্ষ ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          ভঙ্গ;
        পক্ষ ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          ভঙ্গ;
        পক্ষ ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          ভঙ্গ;
        }
      }
      অন্যথা
      {
        ধ্রুৱ শূন্য *প্ৰবেশ_data = প্ৰবেশ.data;
        unsigned প্ৰবেশ_মান = seqIndex - 1;
        যদি (context_format != ContextFormat::SimpleContext)
        {
          প্ৰবেশ_data = data;
          প্ৰবেশ_মান = প্ৰবেশ[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, প্ৰবেশ_data, প্ৰবেশ_মান, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    যদি (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      যদি (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	যদি (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->ঠেলো_cur_active_glyphs (active_idx);
      যদি (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	ঘূৰাই;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    অন্যথা যদি (unlikely (!c->ঠেলো_মূল_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      ঘূৰাই;
    }

    unsigned endIndex = প্ৰবেশ_গুনতি;
    যদি (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->বেৰ_কৰো_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

template <typename context_t>
স্থির অন্তঃস্থ শূন্য recurse_lookups (context_t *c,
                                    unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
                                    ধ্রুৱ LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < lookup_গুনতি_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

স্থির অন্তঃস্থ শূন্য apply_lookup (hb_ot_apply_context_t *c,
				 unsigned পূর্ণাঙ্ক গুনতি, /* Including the first glyph */
				 unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
				 ধ্রুৱ LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 unsigned পূর্ণাঙ্ক match_end)
{
  hb_buffer_t *buffer = c->buffer;
  পূর্ণাঙ্ক end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    unsigned পূর্ণাঙ্ক bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    পূর্ণাঙ্ক delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    বাবে (unsigned পূর্ণাঙ্ক j = 0; j < গুনতি; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  বাবে (unsigned পূর্ণাঙ্ক i = 0; i < lookup_গুনতি_2 && buffer->successful; i++)
  {
    unsigned পূর্ণাঙ্ক idx = lookupRecord[i].sequenceIndex;
    যদি (idx >= গুনতি)
      অনুৱর্ত;

    unsigned পূর্ণাঙ্ক orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    যদি (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      অনুৱর্ত;

    যদি (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      ভঙ্গ;

    যদি (unlikely (buffer->max_ops <= 0))
      ভঙ্গ;

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      যদি (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (unsigned) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    তার্কিক ret = c->recurse (lookupRecord[i].lookupListIndex);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      যদি (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (unsigned) lookupRecord[i].lookupListIndex);
    }

    যদি (!ret)
      অনুৱর্ত;

    unsigned পূর্ণাঙ্ক new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    পূর্ণাঙ্ক delta = new_len - orig_len;

    যদি (!delta)
      অনুৱর্ত;

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
    যদি (end < পূর্ণাঙ্ক (c->match_positions.arrayZ[idx]))
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

    unsigned পূর্ণাঙ্ক next = idx + 1; /* next now is the position after the recursed lookup. */

    যদি (delta > 0)
    {
      যদি (unlikely (delta + গুনতি > HB_MAX_CONTEXT_LENGTH))
	ভঙ্গ;
      যদি (unlikely (গুনতি + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (গুনতি + delta)))
        ঘূৰাই;
    }
    অন্যথা
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (পূর্ণাঙ্ক) next - (পূর্ণাঙ্ক) গুনতি);
      next -= delta;
    }

    /* Shift! */
    memmove (c->match_positions + next + delta, c->match_positions + next,
	     (গুনতি - next) * পরিমাণ (c->match_positions.arrayZ[0]));
    next += delta;
    গুনতি += delta;

    /* Fill in new entries. */
    বাবে (unsigned পূর্ণাঙ্ক j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    বাবে (; next < গুনতি; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  প্রতিজ্ঞা (end >= 0);
  (শূন্য) buffer->move_to (end);
}



/* Contextual lookups */

সংরচনা ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ধ্রুৱ শূন্য *intersects_data;
  শূন্য *intersects_cache;
  শূন্য *intersected_glyphs_cache;
};

প্রকারনির্দেশ ContextClosureLookupContext ContextDependLookupContext;

সংরচনা ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ধ্রুৱ শূন্য *collect_data;
};

সংরচনা ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ধ্রুৱ শূন্য *match_data;
  get_মান_func_t get_মান;
};

স্থির অন্তঃস্থ তার্কিক context_intersects (ধ্রুৱ hb_set_t *glyphs,
				       unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
				       glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  ঘূৰাই array_is_subset_of (glyphs,
			     প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0, প্ৰবেশ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

স্থির অন্তঃস্থ শূন্য context_closure_lookup (hb_closure_context_t *c,
					   unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
					   glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
					   unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
					   ধ্রুৱ LookupRecord lookupRecord[],
					   unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  যদি (context_intersects (c->glyphs,
			  প্ৰবেশ_গুনতি, প্ৰবেশ,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     প্ৰবেশ_গুনতি, প্ৰবেশ,
				     lookup_গুনতি_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

স্থির অন্তঃস্থ শূন্য context_depend_lookup (hb_depend_context_t *c,
					  unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
					  glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
					  unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
					  ধ্রুৱ LookupRecord lookupRecord[],
					  ধ্রুৱ শূন্য *rule,
					  unsigned value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  যদি (!context_intersects (c->glyphs,
			   প্ৰবেশ_গুনতি, প্ৰবেশ,
			   lookup_context))
    ঘূৰাই;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  যদি (unlikely (!scratch))
    ঘূৰাই;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  তার্কিক context_sets_cached = context_depend_sets_are_cached (
    c, প্ৰবেশ_গুনতি, lookup_গুনতি_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  যদি (unlikely (!c->depend_data->successful))
    ঘূৰাই;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ধ্রুৱ hb_set_t *> &প্ৰবেশ_position_glyphs = scratch->প্ৰবেশ_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  যদি (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ধ্রুৱ hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      সঁচা, &position_scratch, &scratch->compact_workspace);
    যদি (unlikely (!pos0_glyphs ||
		  !প্ৰবেশ_position_glyphs.ঠেলো_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      ঘূৰাই;
    }

    /* Positions 1+ (Input array) */
    বাবে (unsigned i = 0; i + 1 < প্ৰবেশ_গুনতি; i++)
    {
      ধ্রুৱ hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	প্ৰবেশ[i], মিছা, &position_scratch);
      যদি (unlikely (!pos_glyphs ||
		    !প্ৰবেশ_position_glyphs.ঠেলো_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	ঘূৰাই;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  প্ৰবেশ_গুনতি, প্ৰবেশ,
				  lookup_গুনতি_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &প্ৰবেশ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

স্থির অন্তঃস্থ শূন্য context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
						  glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
						  unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
						  ধ্রুৱ LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->প্ৰবেশ,
		 প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0, প্ৰবেশ,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_গুনতি_2, lookupRecord);
}

স্থির অন্তঃস্থ তার্কিক context_would_apply_lookup (hb_would_apply_context_t *c,
					       unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
					       glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
					       unsigned পূর্ণাঙ্ক lookup_গুনতি_2 HB_UNUSED,
					       ধ্রুৱ LookupRecord lookupRecord[] HB_UNUSED,
					       ধ্রুৱ ContextApplyLookupContext &lookup_context)
{
  ঘূৰাই would_match_প্ৰবেশ (c,
			    প্ৰবেশ_গুনতি, প্ৰবেশ,
			    lookup_context.funcs.match, lookup_context.match_data);
}

স্থির অন্তঃস্থ তার্কিক context_apply_lookup (hb_ot_apply_context_t *c,
					 unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
					 glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
					 unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
					 ধ্রুৱ LookupRecord lookupRecord[],
					 ধ্রুৱ ContextApplyLookupContext &lookup_context)
{
  যদি (unlikely (প্ৰবেশ_গুনতি > HB_MAX_CONTEXT_LENGTH)) ঘূৰাই মিছা;

  unsigned match_end = 0;
  তার্কিক ret = মিছা;
  যদি (match_প্ৰবেশ (c,
		   প্ৰবেশ_গুনতি, প্ৰবেশ,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  প্ৰবেশ_গুনতি,
		  lookup_গুনতি_2, lookupRecord,
		  match_end);
    ret = সঁচা;
  }
  অন্যথা
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = মিছা;
  }

  ঘূৰাই ret;
}

স্থির অন্তঃস্থ তার্কিক context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  ৱিকল্প (op)
  {
    পক্ষ hb_ot_subtable_cache_op_t::ENTER:
    {
      যদি (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	ঘূৰাই মিছা;
      auto &info = c->buffer->info;
      unsigned গুনতি = c->buffer->len;
      বাবে (unsigned i = 0; i < গুনতি; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      ঘূৰাই সঁচা;
    }
    পক্ষ hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (unsigned) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      ভঙ্গ;
    }
  }
  ঘূৰাই মিছা;
}

template <typename Types>
সংরচনা Rule
{
  template <typename SetTypes, typename RuleType>
  friend সংরচনা RuleSetOf;

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    ঘূৰাই context_intersects (glyphs,
			       প্ৰবেশ_গুনতি, প্ৰবেশ_Z.arrayZ,
			       lookup_context);
  }

  শূন্য depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ধ্রুৱ
  {
    ধ্রুৱ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্ৰবেশ_Z.as_array ((প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0)));
    context_depend_lookup (c,
			   প্ৰবেশ_গুনতি, প্ৰবেশ_Z.arrayZ,
			   lookup_গুনতি_2, lookupRecord.arrayZ,
			   এতত্,
			   value, lookup_context);
  }

  শূন্য closure (hb_closure_context_t *c, unsigned value, ContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ঘূৰাই;

    ধ্রুৱ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্ৰবেশ_Z.as_array ((প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0)));
    context_closure_lookup (c,
			    প্ৰবেশ_গুনতি, প্ৰবেশ_Z.arrayZ,
			    lookup_গুনতি_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ঘূৰাই;
    যদি (!intersects (c->glyphs, lookup_context)) ঘূৰাই;

    ধ্রুৱ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্ৰবেশ_Z.as_array (প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0));
    recurse_lookups (c, lookup_গুনতি_2, lookupRecord.arrayZ);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ধ্রুৱ
  {
    ধ্রুৱ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্ৰবেশ_Z.as_array (প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0));
    context_collect_glyphs_lookup (c,
				   প্ৰবেশ_গুনতি, প্ৰবেশ_Z.arrayZ,
				   lookup_গুনতি_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest,
			      ধ্রুৱ ClassDef *class_def = nullptr) ধ্রুৱ
  {
    যদি (প্ৰবেশ_গুনতি <= 1)
    {
      *digest = hb_set_digest_t::full ();
      ঘূৰাই;
    }

    unsigned second = প্ৰবেশ_Z.arrayZ[0];
    যদি (!class_def)
      digest->add (second);
    অন্যথা যদি (!second)
      *digest = hb_set_digest_t::full ();
    অন্যথা
      class_def->collect_class (digest, second);
  }

  তার্কিক would_apply (hb_would_apply_context_t *c,
		    ধ্রুৱ ContextApplyLookupContext &lookup_context) ধ্রুৱ
  {
    ধ্রুৱ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্ৰবেশ_Z.as_array (প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0));
    ঘূৰাই context_would_apply_lookup (c,
				       প্ৰবেশ_গুনতি, প্ৰবেশ_Z.arrayZ,
				       lookup_গুনতি_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  তার্কিক apply (hb_ot_apply_context_t *c,
	      ধ্রুৱ ContextApplyLookupContext &lookup_context) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    ধ্রুৱ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্ৰবেশ_Z.as_array (প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0));
    return_trace (context_apply_lookup (c, প্ৰবেশ_গুনতি, প্ৰবেশ_Z.arrayZ, lookup_গুনতি_2, lookupRecord.arrayZ, lookup_context));
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ hb_map_t *প্ৰবেশ_mapping, /* old->new glyphid or class mapping */
		  ধ্রুৱ hb_map_t *lookup_map) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->start_embed (এতত্);
    যদি (unlikely (!c->extend_min (out))) return_trace (মিছা);

    out->প্ৰবেশ_গুনতি = প্ৰবেশ_গুনতি;
    ধ্রুৱ auto প্ৰবেশ = প্ৰবেশ_Z.as_array (প্ৰবেশ_গুনতি - 1);
    বাবে (ধ্রুৱ auto org : প্ৰবেশ)
    {
      typename Types::HBUINT d;
      d = প্ৰবেশ_mapping->get (org);
      c->copy (d);
    }

    ধ্রুৱ auto &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (প্ৰবেশ_Z.as_array ((প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0)));

    unsigned গুনতি = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_গুনতি_2), lookup_map);
    return_trace (c->check_assign (out->lookup_গুনতি_2, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক subset (hb_subset_context_t *c,
	       ধ্রুৱ hb_map_t *lookup_map,
	       ধ্রুৱ hb_map_t *klass_map = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (unlikely (!প্ৰবেশ_গুনতি)) return_trace (মিছা);
    ধ্রুৱ auto প্ৰবেশ = প্ৰবেশ_Z.as_array (প্ৰবেশ_গুনতি - 1);

    ধ্রুৱ hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    যদি (!hb_all (প্ৰবেশ, mapping)) return_trace (মিছা);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  সার্ৱজনিক:
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  c->check_range (প্ৰবেশ_Z.arrayZ,
				  প্ৰবেশ_Z.item_size * (প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0) +
				  LookupRecord::static_size * lookup_গুনতি_2));
  }

  রক্ষিত:
  HBUINT16	প্ৰবেশ_গুনতি;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_গুনতি_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		প্ৰবেশ_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4, প্ৰবেশ_Z);
};

template <typename Types, typename RuleType>
সংরচনা RuleSetOf
{
  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_map ([&] (ধ্রুৱ RuleType &_) { ঘূৰাই _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  শূন্য collect_first_প্ৰবেশ_classes (hb_ot_layout_ruleset_digest_t *digest) ধ্রুৱ
  {
    digest->init ();
    বাবে (unsigned i = 0; i < rule.len; i++)
    {
      ধ্রুৱ RuleType &r = এতত্+rule.arrayZ[i];
      যদি (r.প্ৰবেশ_গুনতি <= 1)
      {
	digest->set_full ();
	ঘূৰাই;
      }
      digest->add (r.প্ৰবেশ_Z.arrayZ[0]);
    }
  }

  শূন্য depend (hb_depend_context_t *c, unsigned value, ContextDependLookupContext &lookup_context) ধ্রুৱ
  {
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  শূন্য closure (hb_closure_context_t *c, unsigned value,
		ContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ঘূৰাই;

    ঘূৰাই
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ঘূৰাই;
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest,
			      ধ্রুৱ ClassDef *class_def = nullptr) ধ্রুৱ
  {
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([=] (ধ্রুৱ RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  তার্কিক would_apply (hb_would_apply_context_t *c,
		    ধ্রুৱ ContextApplyLookupContext &lookup_context) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_map ([&] (ধ্রুৱ RuleType &_) { ঘূৰাই _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  তার্কিক apply (hb_ot_apply_context_t *c,
	      ধ্রুৱ ContextApplyLookupContext &lookup_context,
	      ধ্রুৱ hb_ot_layout_ruleset_digest_t *digest = nullptr) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);

    unsigned অঙ্ক_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    যদি (HB_OPTIMIZE_SIZE_VAL || অঙ্ক_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (এতত্))
      | hb_map ([&] (ধ্রুৱ RuleType &_) { ঘূৰাই _.apply (c, lookup_context); })
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
    তার্কিক matched = skippy_iter.next (&unsafe_to1);
    যদি (likely (matched))
    {
      যদি (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        গচ্ছ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      যদি (digest && lookup_context.get_মান &&
	  !digest->may_have (lookup_context.get_মান (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (মিছা);
	}
    }
    অন্যথা
    {
      তার্কিক unsafe_to_concat = মিছা;
      তার্কিক ret =
      + hb_iter (rule)
      | hb_map (hb_add (এতত্))
      | hb_filter ([&] (ধ্রুৱ RuleType &_)
		   {
		     যদি (_.প্ৰবেশ_গুনতি <= 1) ঘূৰাই সঁচা;
		     unsafe_to_concat = সঁচা;
		     ঘূৰাই মিছা;
		   })
      | hb_map ([&] (ধ্রুৱ RuleType &_) { ঘূৰাই _.apply (c, lookup_context); })
      | hb_any
      ;
      যদি (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    যদি (likely (matched))
    {
      যদি (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        গচ্ছ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_প্ৰবেশ = lookup_context.funcs.match;
    auto *প্ৰবেশ_data = lookup_context.match_data;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_rules; i++)
    {
      ধ্রুৱ auto &r = এতত্+rule.arrayZ[i];

      ধ্রুৱ auto &প্ৰবেশ = r.প্ৰবেশ_Z;

      যদি (r.প্ৰবেশ_গুনতি <= 1 ||
	  (!match_প্ৰবেশ ||
	   match_প্ৰবেশ (*first, প্ৰবেশ.arrayZ[0], প্ৰবেশ_data)))
      {
        যদি (!second ||
	    (r.প্ৰবেশ_গুনতি <= 2 ||
	     (!match_প্ৰবেশ ||
	      match_প্ৰবেশ (*second, প্ৰবেশ.arrayZ[1], প্ৰবেশ_data)))
	   )
	{
	  যদি (r.apply (c, lookup_context))
	  {
	    যদি (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (সঁচা);
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
	বাবে (; i + 1 < অঙ্ক_rules; i++)
	{
	  ধ্রুৱ auto &r2 = এতত্+rule.arrayZ[i + 1];
	  ধ্রুৱ auto &input2 = r2.প্ৰবেশ_Z;
	  যদি (r2.প্ৰবেশ_গুনতি <= 1 || input2.arrayZ[0] != প্ৰবেশ.arrayZ[0])
	    ভঙ্গ;
	}
      }
    }
    যদি (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (মিছা);
  }

  তার্কিক subset (hb_subset_context_t *c,
	       ধ্রুৱ hb_map_t *lookup_map,
	       ধ্রুৱ hb_map_t *klass_map = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    বাবে (ধ্রুৱ auto& _ : rule)
    {
      যদি (!_) অনুৱর্ত;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      যদি (unlikely (!o)) অনুৱর্ত;

      যদি (!o->serialize_subset (c, _, এতত্, lookup_map, klass_map))
      {
	out->rule.বেৰ_কৰো ();
	c->serializer->revert (o_snap);
      }
    }

    তার্কিক ret = তার্কিক (out->rule);
    যদি (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (rule.sanitize (c, এতত্));
  }

  রক্ষিত:
  Array16Of<typename Types::template OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
সংরচনা RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

template <typename Types>
সংরচনা ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


template <typename Types>
সংরচনা ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    সংরচনা ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    ঘূৰাই
    + hb_zip (এতত্+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_map ([&] (ধ্রুৱ RuleSet &_) { ঘূৰাই _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই সঁচা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (এতত্+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ঘূৰাই c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ধ্রুৱ hb_pair_t<hb_codepoint_t, unsigned> _) { ঘূৰাই hb_pair_t<unsigned, ধ্রুৱ RuleSet&> (_.first, এতত্+ruleSet[_.second]); })
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    সংরচনা ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (এতত্+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ঘূৰাই c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ধ্রুৱ hb_pair_t<hb_codepoint_t, unsigned> _) { ঘূৰাই hb_pair_t<unsigned, ধ্রুৱ RuleSet&> (_.first, এতত্+ruleSet[_.second]); })
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ
  {
    সংরচনা ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (এতত্+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    (এতত্+coverage).collect_coverage (c->প্ৰবেশ);

    সংরচনা ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুৱ
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([digest] (ধ্রুৱ RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ RuleSet &rule_set = এতত্+ruleSet[(এতত্+coverage).get_coverage (c->glyphs[0])];
    সংরচনা ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    ঘূৰাই rule_set.would_apply (c, lookup_context);
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->buffer->cur().codepoint);
    যদি (likely (index == NOT_COVERED))
      return_trace (মিছা);

    ধ্রুৱ RuleSet &rule_set = এতত্+ruleSet[index];
    সংরচনা ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->format = format;

    ধ্রুৱ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, এতত্, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (তার্কিক (new_coverage));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && ruleSet.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
সংরচনা ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (glyphs))
      ঘূৰাই মিছা;

    ধ্রুৱ ClassDef &class_def = এতত্+classDef;

    hb_map_t cache;
    সংরচনা ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (এতত্+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    ঘূৰাই
    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_enumerate
    | hb_map ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ RuleSet &> p)
	      { ঘূৰাই class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই সঁচা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (c->glyphs))
      ঘূৰাই;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ধ্রুৱ ClassDef &class_def = এতত্+classDef;

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
    { ঘূৰাই class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ধ্রুৱ RuleSet& rule_set = এতত্+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (c->glyphs))
      ঘূৰাই;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ধ্রুৱ ClassDef &class_def = এতত্+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    সংরচনা ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ঘূৰাই class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ typename Types::template OffsetTo<RuleSet>&> _)
                {
                  ধ্রুৱ RuleSet& rule_set = এতত্+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (c->glyphs))
      ঘূৰাই;

    ধ্রুৱ ClassDef &class_def = এতত্+classDef;

    hb_map_t cache;
    সংরচনা ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_enumerate
    | hb_filter ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ RuleSet &> p)
    { ঘূৰাই class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (ধ্রুৱ RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    (এতত্+coverage).collect_coverage (c->প্ৰবেশ);

    ধ্রুৱ ClassDef &class_def = এতত্+classDef;
    সংরচনা ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুৱ
  {
    ধ্রুৱ ClassDef &class_def = এতত্+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([=, &class_def] (ধ্রুৱ RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ ClassDef &class_def = এতত্+classDef;
    unsigned পূর্ণাঙ্ক index = class_def.get_class (c->glyphs[0]);
    ধ্রুৱ RuleSet &rule_set = এতত্+ruleSet[index];
    সংরচনা ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    ঘূৰাই rule_set.would_apply (c, lookup_context);
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  unsigned cache_cost () ধ্রুৱ
  {
    ঘূৰাই (এতত্+classDef).cost ();
  }
  স্থির তার্কিক cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ঘূৰাই context_cache_func (c, op);
  }

  সংরচনা external_cache_t
  {
    hb_ot_layout_দ্বিমিক_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  শূন্য *external_cache_create () ধ্রুৱ
  {
    unsigned গুনতি = ruleSet.len;
    unsigned size = পরিমাণ (external_cache_t) -
		    HB_VAR_ARRAY * পরিমাণ (hb_ot_layout_ruleset_digest_t) +
		    গুনতি * পরিমাণ (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    যদি (likely (cache))
    {
      cache->coverage.clear ();
      বাবে (unsigned i = 0; i < গুনতি; i++)
	(এতত্+ruleSet[i]).collect_first_প্ৰবেশ_classes (&cache->rule_sets[i]);
    }
    ঘূৰাই cache;
  }
  তার্কিক apply_cached (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুৱ { ঘূৰাই _apply (c, সঁচা, external_cache); }
  তার্কিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুৱ { ঘূৰাই _apply (c, মিছা, external_cache); }
  তার্কিক _apply (hb_ot_apply_context_t *c, তার্কিক cached, শূন্য *external_cache) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage_দ্বিমিক (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    যদি (index == NOT_COVERED) return_trace (মিছা);

    ধ্রুৱ ClassDef &class_def = এতত্+classDef;

    সংরচনা ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_মান : get_class_মান
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    ধ্রুৱ RuleSet &rule_set = এতত্+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ধ্রুৱ hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#অন্যথা
    ধ্রুৱ hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->format = format;
    যদি (unlikely (!out->coverage.serialize_subset (c, coverage, এতত্)))
      return_trace (মিছা);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, এতত্, &klass_map);

    ধ্রুৱ hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (এতত্+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (এতত্+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ধ্রুৱ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    তার্কিক ret = সঁচা;
    পূর্ণাঙ্ক non_zero_index = -1, index = 0;
    auto snapshot = c->serializer->snapshot();
    বাবে (ধ্রুৱ auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      যদি (unlikely (!o))
      {
	ret = মিছা;
	ভঙ্গ;
      }

      যদি (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, এতত্, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    যদি (!ret || non_zero_index == -1) return_trace (মিছা);

    //prune empty trailing ruleSets
    --index;
    যেতিয়ালৈ (index > non_zero_index)
    {
      out->ruleSet.বেৰ_কৰো ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (তার্কিক (out->ruleSet));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && classDef.sanitize (c, এতত্) && ruleSet.sanitize (c, এতত্));
  }

  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


template <typename Types>
সংরচনা ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    যদি (!(এতত্+coverageZ[0]).intersects (glyphs))
      ঘূৰাই মিছা;

    সংরচনা ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      এতত্
    };
    ঘূৰাই context_intersects (glyphs,
			       glyph_গুনতি_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই সঁচা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    যদি (!(এতত্+coverageZ[0]).intersects (c->glyphs))
      ঘূৰাই;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ধ্রুৱ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      এতত্,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_গুনতি_2, (ধ্রুৱ HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_গুনতি_2, lookupRecord,
			   এতত্,
			   coverageZ[0], lookup_context);

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    যদি (!(এতত্+coverageZ[0]).intersects (c->glyphs))
      ঘূৰাই;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ধ্রুৱ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    সংরচনা ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      এতত্
    };
    context_closure_lookup (c,
			    glyph_গুনতি_2, coverageZ.arrayZ + 1,
			    lookup_গুনতি_2, lookupRecord,
			    0, lookup_context);

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ
  {
    যদি (!intersects (c->glyphs))
      ঘূৰাই;
    ধ্রুৱ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    recurse_lookups (c, lookup_গুনতি_2, lookupRecord);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    (এতত্+coverageZ[0]).collect_coverage (c->প্ৰবেশ);

    ধ্রুৱ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    সংরচনা ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      এতত্
    };

    context_collect_glyphs_lookup (c,
				   glyph_গুনতি_2, coverageZ.arrayZ + 1,
				   lookup_গুনতি_2, lookupRecord,
				   lookup_context);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুৱ
  {
    যদি (glyph_গুনতি_2 <= 1)
      *digest = hb_set_digest_t::full ();
    অন্যথা
      (এতত্+coverageZ[1]).collect_coverage (digest);
  }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    সংরচনা ContextApplyLookupContext lookup_context = {
      {match_coverage},
      এতত্
    };
    ঘূৰাই context_would_apply_lookup (c,
				       glyph_গুনতি_2, coverageZ.arrayZ + 1,
				       lookup_গুনতি_2, lookupRecord,
				       lookup_context);
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverageZ[0]; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    unsigned পূর্ণাঙ্ক index = (এতত্+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    যদি (index == NOT_COVERED) return_trace (মিছা);

    ধ্রুৱ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    সংরচনা ContextApplyLookupContext lookup_context = {
      {match_coverage},
      এতত্
    };
    return_trace (context_apply_lookup (c,
					glyph_গুনতি_2,
					coverageZ.arrayZ + 1,
					lookup_গুনতি_2, lookupRecord,
					lookup_context));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    out->format = format;
    out->glyph_গুনতি_2 = glyph_গুনতি_2;

    auto coverages = coverageZ.as_array (glyph_গুনতি_2);

    বাবে (ধ্রুৱ auto& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      auto *o = c->serializer->allocate_size<typename Types::template OffsetTo<Coverage>> (Types::Offset::static_size);
      যদি (unlikely (!o)) return_trace (মিছা);
      যদি (!o->serialize_subset (c, offset, এতত্)) return_trace (মিছা);
    }

    ধ্রুৱ auto& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_গুনতি_2));
    ধ্রুৱ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    unsigned গুনতি = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_গুনতি_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_গুনতি_2, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!c->check_struct (এতত্))) return_trace (মিছা);
    hb_barrier ();
    unsigned পূর্ণাঙ্ক গুনতি = glyph_গুনতি_2;
    যদি (unlikely (!গুনতি)) return_trace (মিছা); /* We want to access coverageZ[0] freely. */
    যদি (unlikely (!c->check_array (coverageZ.arrayZ, গুনতি))) return_trace (মিছা);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (unlikely (!coverageZ[i].sanitize (c, এতত্))) return_trace (মিছা);
    ধ্রুৱ LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_গুনতি_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_গুনতি_2)));
  }

  রক্ষিত:
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
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

সংরচনা Context
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    পক্ষ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    পক্ষ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    পক্ষ 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;		/* Format identifier */
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

সংরচনা ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  ধ্রুৱ শূন্য *intersects_data[3];
  শূন্য *intersects_cache[3];
  শূন্য *intersected_glyphs_cache;
};

প্রকারনির্দেশ ChainContextClosureLookupContext ChainContextDependLookupContext;

সংরচনা ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ধ্রুৱ শূন্য *collect_data[3];
};

সংরচনা ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ধ্রুৱ শূন্য *match_data[3];
  get_মান_func_t get_প্ৰবেশ_মান;
};

স্থির অন্তঃস্থ তার্কিক chain_context_intersects (ধ্রুৱ hb_set_t *glyphs,
					     unsigned পূর্ণাঙ্ক backtrack_গুনতি,
					     glyph_data_t backtrack,
					     unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
					     glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
					     unsigned পূর্ণাঙ্ক lookahead_গুনতি,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  তার্কিক bt_ok = array_is_subset_of (glyphs,
			     backtrack_গুনতি, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  তার্কিক in_ok = array_is_subset_of (glyphs,
			     প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0, প্ৰবেশ,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  তার্কিক la_ok = array_is_subset_of (glyphs,
			     lookahead_গুনতি, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  ঘূৰাই bt_ok && in_ok && la_ok;
}

স্থির অন্তঃস্থ শূন্য chain_context_closure_lookup (hb_closure_context_t *c,
						 unsigned পূর্ণাঙ্ক backtrack_গুনতি,
						 glyph_data_t backtrack,
						 unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
						 glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
						 unsigned পূর্ণাঙ্ক lookahead_গুনতি,
						 glyph_data_t lookahead,
						 unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
						 ধ্রুৱ LookupRecord lookupRecord[],
						 unsigned value,
						 ChainContextClosureLookupContext &lookup_context)
{
  যদি (chain_context_intersects (c->glyphs,
				backtrack_গুনতি, backtrack,
				প্ৰবেশ_গুনতি, প্ৰবেশ,
				lookahead_গুনতি, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     প্ৰবেশ_গুনতি, প্ৰবেশ,
		     lookup_গুনতি_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

স্থির অন্তঃস্থ শূন্য chain_context_depend_lookup (hb_depend_context_t *c,
						unsigned পূর্ণাঙ্ক backtrack_গুনতি,
						glyph_data_t backtrack,
						unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
						glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
						unsigned পূর্ণাঙ্ক lookahead_গুনতি,
						glyph_data_t lookahead,
						unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
						ধ্রুৱ LookupRecord lookupRecord[],
						ধ্রুৱ শূন্য *rule,
						unsigned value,
						ChainContextDependLookupContext &lookup_context)
{
  যদি (!chain_context_intersects (c->glyphs,
				 backtrack_গুনতি, backtrack,
				 প্ৰবেশ_গুনতি, প্ৰবেশ,
				 lookahead_গুনতি, lookahead,
				 lookup_context))
    ঘূৰাই;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  যদি (unlikely (!scratch))
    ঘূৰাই;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  তার্কিক context_sets_cached = context_depend_sets_are_cached (
    c, প্ৰবেশ_গুনতি, lookup_গুনতি_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  যদি (unlikely (!c->depend_data->successful))
    ঘূৰাই;
  যদি (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    প্ৰবেশ_গুনতি, প্ৰবেশ,
				    lookup_গুনতি_2, lookupRecord,
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
    ঘূৰাই;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ধ্রুৱ hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<ধ্রুৱ hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  বাবে (unsigned i = 0; i < backtrack_গুনতি; i++)
  {
    ধ্রুৱ hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], মিছা, &position_scratch);
    যদি (unlikely (!pos_glyphs))
      ঘূৰাই;
    যদি (!pos_glyphs->is_empty ())
    {
      যদি (unlikely (!backtrack_sets.ঠেলো_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        ঘূৰাই;
      }
    }
  }

  /* Extract lookahead glyphs */
  বাবে (unsigned i = 0; i < lookahead_গুনতি; i++)
  {
    ধ্রুৱ hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], মিছা, &position_scratch);
    যদি (unlikely (!pos_glyphs))
      ঘূৰাই;
    যদি (!pos_glyphs->is_empty ())
    {
      যদি (unlikely (!lookahead_sets.ঠেলো_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        ঘূৰাই;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  বাবে (ধ্রুৱ hb_set_t *back_set : backtrack_sets)
  {
    যদি (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } অন্যথা যদি (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
        ঘূৰাই;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  বাবে (ধ্রুৱ hb_set_t *look_set : lookahead_sets)
  {
    যদি (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } অন্যথা যদি (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      যদি (unlikely (set_idx == HB_CODEPOINT_INVALID))
        ঘূৰাই;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  যদি (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    ঘূৰাই;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<ধ্রুৱ hb_set_t *> &প্ৰবেশ_position_glyphs = scratch->প্ৰবেশ_position_glyphs;

  যদি (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ধ্রুৱ hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      সঁচা, &position_scratch, &scratch->compact_workspace);
    যদি (unlikely (!pos0_glyphs ||
		  !প্ৰবেশ_position_glyphs.ঠেলো_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      ঘূৰাই;
    }

    /* Positions 1+ (Input array) */
    বাবে (unsigned i = 0; i + 1 < প্ৰবেশ_গুনতি; i++)
    {
      ধ্রুৱ hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	প্ৰবেশ[i], মিছা, &position_scratch);
      যদি (unlikely (!pos_glyphs ||
		    !প্ৰবেশ_position_glyphs.ঠেলো_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	ঘূৰাই;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  প্ৰবেশ_গুনতি, প্ৰবেশ,
				  lookup_গুনতি_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &প্ৰবেশ_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

স্থির অন্তঃস্থ শূন্য chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							unsigned পূর্ণাঙ্ক backtrack_গুনতি,
							glyph_data_t backtrack,
							unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
							glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
							unsigned পূর্ণাঙ্ক lookahead_গুনতি,
							glyph_data_t lookahead,
							unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
							ধ্রুৱ LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_গুনতি, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->প্ৰবেশ,
		 প্ৰবেশ_গুনতি ? প্ৰবেশ_গুনতি - 1 : 0, প্ৰবেশ,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_গুনতি, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_গুনতি_2, lookupRecord);
}

স্থির অন্তঃস্থ তার্কিক chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     unsigned পূর্ণাঙ্ক backtrack_গুনতি,
						     glyph_data_t backtrack HB_UNUSED,
						     unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
						     glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
						     unsigned পূর্ণাঙ্ক lookahead_গুনতি,
						     glyph_data_t lookahead HB_UNUSED,
						     unsigned পূর্ণাঙ্ক lookup_গুনতি_2 HB_UNUSED,
						     ধ্রুৱ LookupRecord lookupRecord[] HB_UNUSED,
						     ধ্রুৱ ChainContextApplyLookupContext &lookup_context)
{
  ঘূৰাই (c->zero_context ? !backtrack_গুনতি && !lookahead_গুনতি : সঁচা)
      && would_match_প্ৰবেশ (c,
			    প্ৰবেশ_গুনতি, প্ৰবেশ,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

স্থির অন্তঃস্থ তার্কিক chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       unsigned পূর্ণাঙ্ক backtrack_গুনতি,
					       glyph_data_t backtrack,
					       unsigned পূর্ণাঙ্ক প্ৰবেশ_গুনতি, /* Including the first glyph (not matched) */
					       glyph_data_t প্ৰবেশ, /* Array of input values--start with second glyph */
					       unsigned পূর্ণাঙ্ক lookahead_গুনতি,
					       glyph_data_t lookahead,
					       unsigned পূর্ণাঙ্ক lookup_গুনতি_2,
					       ধ্রুৱ LookupRecord lookupRecord[],
					       ধ্রুৱ ChainContextApplyLookupContext &lookup_context)
{
  যদি (unlikely (প্ৰবেশ_গুনতি > HB_MAX_CONTEXT_LENGTH)) ঘূৰাই মিছা;

  unsigned start_index = c->buffer->out_len;
  unsigned end_index = c->buffer->idx;
  unsigned match_end = 0;
  তার্কিক ret = সঁচা;
  যদি (!(match_প্ৰবেশ (c,
		     প্ৰবেশ_গুনতি, প্ৰবেশ,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_গুনতি, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    ঘূৰাই মিছা;
  }

  যদি (!match_backtrack (c,
			backtrack_গুনতি, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    ঘূৰাই মিছা;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		প্ৰবেশ_গুনতি,
		lookup_গুনতি_2, lookupRecord,
		match_end);

  ঘূৰাই ret;
}

template <typename Types>
সংরচনা ChainRule
{
  template <typename SetTypes, typename ChainRuleType>
  friend সংরচনা ChainRuleSetOf;

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ঘূৰাই chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     প্ৰবেশ.lenP1, প্ৰবেশ.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  শূন্য depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 প্ৰবেশ.lenP1, প্ৰবেশ.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 এতত্,
				 value,
				 lookup_context);
  }

  শূন্য closure (hb_closure_context_t *c, unsigned value,
		ChainContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ঘূৰাই;

    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  প্ৰবেশ.lenP1, প্ৰবেশ.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ঘূৰাই;
    যদি (!intersects (c->glyphs, lookup_context)) ঘূৰাই;

    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 প্ৰবেশ.lenP1, প্ৰবেশ.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest,
			      ধ্রুৱ ClassDef *প্ৰবেশ_class_def = nullptr,
			      ধ্রুৱ ClassDef *lookahead_class_def = nullptr) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);

    unsigned second;
    ধ্রুৱ ClassDef *class_def;
    যদি (প্ৰবেশ.lenP1 > 1)
    {
      second = প্ৰবেশ.arrayZ[0];
      class_def = প্ৰবেশ_class_def;
    }
    অন্যথা যদি (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    অন্যথা
    {
      *digest = hb_set_digest_t::full ();
      ঘূৰাই;
    }

    যদি (!class_def)
      digest->add (second);
    অন্যথা যদি (!second)
      *digest = hb_set_digest_t::full ();
    অন্যথা
      class_def->collect_class (digest, second);
  }

  তার্কিক would_apply (hb_would_apply_context_t *c,
		    ধ্রুৱ ChainContextApplyLookupContext &lookup_context) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ঘূৰাই chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     প্ৰবেশ.lenP1, প্ৰবেশ.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  তার্কিক apply (hb_ot_apply_context_t *c,
	      ধ্রুৱ ChainContextApplyLookupContext &lookup_context) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      প্ৰবেশ.lenP1, প্ৰবেশ.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ধ্রুৱ
  {
    c->copy (len);
    বাবে (ধ্রুৱ auto g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  তার্কিক serialize (hb_serialize_context_t *c,
		  ধ্রুৱ hb_map_t *lookup_map,
		  ধ্রুৱ hb_map_t *backtrack_map,
		  ধ্রুৱ hb_map_t *প্ৰবেশ_map = nullptr,
		  ধ্রুৱ hb_map_t *lookahead_map = nullptr) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);

    ধ্রুৱ hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    যদি (প্ৰবেশ_map) mapping = প্ৰবেশ_map;
    serialize_array (c, প্ৰবেশ.lenP1, + প্ৰবেশ.iter ()
				     | hb_map (mapping));

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    যদি (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_গুনতি_2 = c->embed (&(lookup.len));
    যদি (!lookup_গুনতি_2) return_trace (মিছা);

    unsigned গুনতি = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_গুনতি_2, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক subset (hb_subset_context_t *c,
	       ধ্রুৱ hb_map_t *lookup_map,
	       ধ্রুৱ hb_map_t *backtrack_map = nullptr,
	       ধ্রুৱ hb_map_t *প্ৰবেশ_map = nullptr,
	       ধ্রুৱ hb_map_t *lookahead_map = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);

    যদি (!backtrack_map)
    {
      ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      যদি (!hb_all (backtrack, glyphset) ||
	  !hb_all (প্ৰবেশ, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (মিছা);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    অন্যথা
    {
      যদি (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (প্ৰবেশ, প্ৰবেশ_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (মিছা);

      serialize (c->serializer, lookup_map, backtrack_map, প্ৰবেশ_map, lookahead_map);
    }

    return_trace (সঁচা);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    /* Hyper-optimized sanitized because this is really hot. */
    যদি (unlikely (!backtrack.len.sanitize (c))) return_trace (মিছা);
    hb_barrier ();
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    যদি (unlikely (!প্ৰবেশ.lenP1.sanitize (c))) return_trace (মিছা);
    hb_barrier ();
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    যদি (unlikely (!lookahead.len.sanitize (c))) return_trace (মিছা);
    hb_barrier ();
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  রক্ষিত:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		প্ৰবেশ_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (8);
};

template <typename Types, typename ChainRuleType>
সংরচনা ChainRuleSetOf
{
  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_map ([&] (ধ্রুৱ ChainRuleType &_) { ঘূৰাই _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  শূন্য collect_first_প্ৰবেশ_classes (hb_ot_layout_ruleset_digest_t *digest) ধ্রুৱ
  {
    digest->init ();
    বাবে (unsigned i = 0; i < rule.len; i++)
    {
      ধ্রুৱ ChainRuleType &r = এতত্+rule.arrayZ[i];
      ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (r.প্ৰবেশ_X)> (r.backtrack);
      যদি (প্ৰবেশ.lenP1 <= 1)
      {
	digest->set_full ();
	ঘূৰাই;
      }
      digest->add (প্ৰবেশ.arrayZ[0]);
    }
  }
  শূন্য depend (hb_depend_context_t *c, unsigned value, ChainContextDependLookupContext &lookup_context) ধ্রুৱ
  {
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  শূন্য closure (hb_closure_context_t *c, unsigned value, ChainContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ঘূৰাই;

    ঘূৰাই
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ধ্রুৱ
  {
    যদি (unlikely (c->lookup_limit_exceeded ())) ঘূৰাই;

    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest,
			      ধ্রুৱ ClassDef *প্ৰবেশ_class_def = nullptr,
			      ধ্রুৱ ClassDef *lookahead_class_def = nullptr) ধ্রুৱ
  {
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_apply ([=] (ধ্রুৱ ChainRuleType &_)
		{ _.collect_second_glyphs (digest, প্ৰবেশ_class_def, lookahead_class_def); })
    ;
  }

  তার্কিক would_apply (hb_would_apply_context_t *c,
		    ধ্রুৱ ChainContextApplyLookupContext &lookup_context) ধ্রুৱ
  {
    ঘূৰাই
    + hb_iter (rule)
    | hb_map (hb_add (এতত্))
    | hb_map ([&] (ধ্রুৱ ChainRuleType &_) { ঘূৰাই _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  তার্কিক apply (hb_ot_apply_context_t *c,
	      ধ্রুৱ ChainContextApplyLookupContext &lookup_context,
	      ধ্রুৱ hb_ot_layout_ruleset_digest_t *digest = nullptr) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);

    unsigned অঙ্ক_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    যদি (HB_OPTIMIZE_SIZE_VAL || অঙ্ক_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (এতত্))
      | hb_map ([&] (ধ্রুৱ ChainRuleType &_) { ঘূৰাই _.apply (c, lookup_context); })
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
    তার্কিক matched = skippy_iter.next (&unsafe_to1);
    যদি (likely (matched))
    {
      যদি (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        গচ্ছ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      যদি (digest && lookup_context.get_প্ৰবেশ_মান &&
	  !digest->may_have (lookup_context.get_প্ৰবেশ_মান (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (মিছা);
	}
    }
    অন্যথা
    {
      তার্কিক unsafe_to_concat = মিছা;
      তার্কিক ret =
      + hb_iter (rule)
      | hb_map (hb_add (এতত্))
      | hb_filter ([&] (ধ্রুৱ ChainRuleType &_)
		   {
		     ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (_.প্ৰবেশ_X)> (_.backtrack);
		     ধ্রুৱ auto &lookahead = StructAfter<decltype (_.lookaheadX)> (প্ৰবেশ);
		     যদি (প্ৰবেশ.lenP1 <= 1 && lookahead.len == 0) ঘূৰাই সঁচা;
		     unsafe_to_concat = সঁচা;
		     ঘূৰাই মিছা;
		   })
      | hb_map ([&] (ধ্রুৱ ChainRuleType &_) { ঘূৰাই _.apply (c, lookup_context); })
      | hb_any
      ;
      যদি (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    যদি (likely (matched))
    {
      যদি (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        গচ্ছ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    auto match_প্ৰবেশ = lookup_context.funcs.match[1];
    auto match_lookahead = lookup_context.funcs.match[2];
    auto *প্ৰবেশ_data = lookup_context.match_data[1];
    auto *lookahead_data = lookup_context.match_data[2];
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < অঙ্ক_rules; i++)
    {
      ধ্রুৱ auto &r = এতত্+rule.arrayZ[i];

      ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (r.প্ৰবেশ_X)> (r.backtrack);
      ধ্রুৱ auto &lookahead = StructAfter<decltype (r.lookaheadX)> (প্ৰবেশ);

      unsigned lenP1 = প্ৰবেশ.lenP1;
      যদি (lenP1 > 1 ?
	   (!match_প্ৰবেশ ||
	    match_প্ৰবেশ (*first, প্ৰবেশ.arrayZ[0], প্ৰবেশ_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        যদি (!second ||
	    (lenP1 > 2 ?
	     (!match_প্ৰবেশ ||
	      match_প্ৰবেশ (*second, প্ৰবেশ.arrayZ[1], প্ৰবেশ_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  যদি (r.apply (c, lookup_context))
	  {
	    যদি (unsafe_to != (unsigned) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (সঁচা);
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
	  বাবে (; i + 1 < অঙ্ক_rules; i++)
	  {
	    ধ্রুৱ auto &r2 = এতত্+rule.arrayZ[i + 1];
	    ধ্রুৱ auto &input2 = StructAfter<decltype (r2.প্ৰবেশ_X)> (r2.backtrack);
	    যদি (input2.lenP1 <= 1 || input2.arrayZ[0] != প্ৰবেশ.arrayZ[0])
	      ভঙ্গ;
	  }
	}
      }
    }
    যদি (likely (unsafe_to != (unsigned) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (মিছা);
  }

  তার্কিক subset (hb_subset_context_t *c,
	       ধ্রুৱ hb_map_t *lookup_map,
	       ধ্রুৱ hb_map_t *backtrack_klass_map = nullptr,
	       ধ্রুৱ hb_map_t *প্ৰবেশ_klass_map = nullptr,
	       ধ্রুৱ hb_map_t *lookahead_klass_map = nullptr) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    auto snap = c->serializer->snapshot ();
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    বাবে (ধ্রুৱ auto& _ : rule)
    {
      যদি (!_) অনুৱর্ত;
      auto o_snap = c->serializer->snapshot ();
      auto *o = out->rule.serialize_append (c->serializer);
      যদি (unlikely (!o)) অনুৱর্ত;

      যদি (!o->serialize_subset (c, _, এতত্,
				lookup_map,
				backtrack_klass_map,
				প্ৰবেশ_klass_map,
				lookahead_klass_map))
      {
	out->rule.বেৰ_কৰো ();
	c->serializer->revert (o_snap);
      }
    }

    তার্কিক ret = তার্কিক (out->rule);
    যদি (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (rule.sanitize (c, এতত্));
  }

  রক্ষিত:
  Array16Of<typename Types::template OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2, rule);
};

template <typename Types>
সংরচনা ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (পরিমাণ (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

template <typename Types>
সংরচনা ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

template <typename Types>
সংরচনা ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    সংরচনা ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    ঘূৰাই
    + hb_zip (এতত্+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_map ([&] (ধ্রুৱ ChainRuleSet &_) { ঘূৰাই _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই সঁচা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (এতত্+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ঘূৰাই c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ধ্রুৱ hb_pair_t<hb_codepoint_t, unsigned> _) { ঘূৰাই hb_pair_t<unsigned, ধ্রুৱ ChainRuleSet&> (_.first, এতত্+ruleSet[_.second]); })
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    সংরচনা ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (এতত্+coverage, hb_range ((unsigned) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      ঘূৰাই c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ধ্রুৱ hb_pair_t<hb_codepoint_t, unsigned> _) { ঘূৰাই hb_pair_t<unsigned, ধ্রুৱ ChainRuleSet&> (_.first, এতত্+ruleSet[_.second]); })
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ
  {
    সংরচনা ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (এতত্+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    (এতত্+coverage).collect_coverage (c->প্ৰবেশ);

    সংরচনা ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুৱ
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([digest] (ধ্রুৱ ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ ChainRuleSet &rule_set = এতত্+ruleSet[(এতত্+coverage).get_coverage (c->glyphs[0])];
    সংরচনা ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    ঘূৰাই rule_set.would_apply (c, lookup_context);
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->buffer->cur().codepoint);
    যদি (index == NOT_COVERED) return_trace (মিছা);

    ধ্রুৱ ChainRuleSet &rule_set = এতত্+ruleSet[index];
    সংরচনা ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->format = format;

    ধ্রুৱ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (এতত্+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, এতত্, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (তার্কিক (new_coverage));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && ruleSet.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template ArrayOf<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

template <typename Types>
সংরচনা ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (glyphs))
      ঘূৰাই মিছা;

    ধ্রুৱ ClassDef &backtrack_class_def = এতত্+backtrackClassDef;
    ধ্রুৱ ClassDef &প্ৰবেশ_class_def = এতত্+প্ৰবেশ_Class_Def;
    ধ্রুৱ ClassDef &lookahead_class_def = এতত্+lookaheadClassDef;

    hb_map_t caches[3] = {};
    সংরচনা ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &প্ৰবেশ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (এতত্+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    প্ৰবেশ_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ঘূৰাই
    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_enumerate
    | hb_map ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ ChainRuleSet &> p)
	      { ঘূৰাই প্ৰবেশ_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই সঁচা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (c->glyphs))
      ঘূৰাই;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ধ্রুৱ ClassDef &backtrack_class_def = এতত্+backtrackClassDef;
    ধ্রুৱ ClassDef &প্ৰবেশ_class_def = এতত্+প্ৰবেশ_Class_Def;
    ধ্রুৱ ClassDef &lookahead_class_def = এতত্+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &প্ৰবেশ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ঘূৰাই প্ৰবেশ_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ধ্রুৱ ChainRuleSet& chainrule_set = এতত্+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (c->glyphs))
      ঘূৰাই;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs)) ঘূৰাই;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ধ্রুৱ ClassDef &backtrack_class_def = এতত্+backtrackClassDef;
    ধ্রুৱ ClassDef &প্ৰবেশ_class_def = এতত্+প্ৰবেশ_Class_Def;
    ধ্রুৱ ClassDef &lookahead_class_def = এতত্+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    সংরচনা ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &প্ৰবেশ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (unsigned _)
    { ঘূৰাই প্ৰবেশ_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<unsigned, ধ্রুৱ typename Types::template OffsetTo<ChainRuleSet>&> _)
                {
                  ধ্রুৱ ChainRuleSet& chainrule_set = এতত্+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ
  {
    যদি (!(এতত্+coverage).intersects (c->glyphs))
      ঘূৰাই;

    ধ্রুৱ ClassDef &backtrack_class_def = এতত্+backtrackClassDef;
    ধ্রুৱ ClassDef &প্ৰবেশ_class_def = এতত্+প্ৰবেশ_Class_Def;
    ধ্রুৱ ClassDef &lookahead_class_def = এতত্+lookaheadClassDef;

    hb_map_t caches[3] = {};
    সংরচনা ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &প্ৰবেশ_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_enumerate
    | hb_filter([&] (unsigned klass)
    { ঘূৰাই প্ৰবেশ_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ধ্রুৱ ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    (এতত্+coverage).collect_coverage (c->প্ৰবেশ);

    ধ্রুৱ ClassDef &backtrack_class_def = এতত্+backtrackClassDef;
    ধ্রুৱ ClassDef &প্ৰবেশ_class_def = এতত্+প্ৰবেশ_Class_Def;
    ধ্রুৱ ClassDef &lookahead_class_def = এতত্+lookaheadClassDef;

    সংরচনা ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &প্ৰবেশ_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([&] (ধ্রুৱ ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুৱ
  {
    ধ্রুৱ ClassDef &প্ৰবেশ_class_def = এতত্+প্ৰবেশ_Class_Def;
    ধ্রুৱ ClassDef &lookahead_class_def = এতত্+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (এতত্))
    | hb_apply ([digest, &প্ৰবেশ_class_def, &lookahead_class_def] (ধ্রুৱ ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &প্ৰবেশ_class_def, &lookahead_class_def); })
    ;
  }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ ClassDef &backtrack_class_def = এতত্+backtrackClassDef;
    ধ্রুৱ ClassDef &প্ৰবেশ_class_def = এতত্+প্ৰবেশ_Class_Def;
    ধ্রুৱ ClassDef &lookahead_class_def = এতত্+lookaheadClassDef;

    unsigned পূর্ণাঙ্ক index = প্ৰবেশ_class_def.get_class (c->glyphs[0]);
    ধ্রুৱ ChainRuleSet &rule_set = এতত্+ruleSet[index];
    সংরচনা ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &প্ৰবেশ_class_def,
       &lookahead_class_def}
    };
    ঘূৰাই rule_set.would_apply (c, lookup_context);
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  unsigned cache_cost () ধ্রুৱ
  {
    ঘূৰাই (এতত্+প্ৰবেশ_Class_Def).cost () + (এতত্+lookaheadClassDef).cost ();
  }
  স্থির তার্কিক cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    ঘূৰাই context_cache_func (c, op);
  }

  সংরচনা external_cache_t
  {
    hb_ot_layout_দ্বিমিক_cache_t coverage;
    hb_ot_layout_mapping_cache_t প্ৰবেশ_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  শূন্য *external_cache_create () ধ্রুৱ
  {
    unsigned গুনতি = ruleSet.len;
    unsigned size = পরিমাণ (external_cache_t) -
		    HB_VAR_ARRAY * পরিমাণ (hb_ot_layout_ruleset_digest_t) +
		    গুনতি * পরিমাণ (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    যদি (likely (cache))
    {
      cache->coverage.clear ();
      cache->প্ৰবেশ_class.clear ();
      cache->lookahead_class.clear ();
      বাবে (unsigned i = 0; i < গুনতি; i++)
	(এতত্+ruleSet[i]).collect_first_প্ৰবেশ_classes (&cache->rule_sets[i]);
    }
    ঘূৰাই cache;
  }
  তার্কিক apply_cached (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুৱ { ঘূৰাই _apply (c, সঁচা, external_cache); }
  তার্কিক apply (hb_ot_apply_context_t *c, শূন্য *external_cache) ধ্রুৱ { ঘূৰাই _apply (c, মিছা, external_cache); }
  তার্কিক _apply (hb_ot_apply_context_t *c, তার্কিক cached, শূন্য *external_cache) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage_দ্বিমিক (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#অন্যথা
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    যদি (index == NOT_COVERED) return_trace (মিছা);

    ধ্রুৱ ClassDef &backtrack_class_def = এতত্+backtrackClassDef;
    ধ্রুৱ ClassDef &প্ৰবেশ_class_def = এতত্+প্ৰবেশ_Class_Def;
    ধ্রুৱ ClassDef &lookahead_class_def = এতত্+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t প্ৰবেশ_cache_data = {
      &প্ৰবেশ_class_def,
      cache ? &cache->প্ৰবেশ_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    ধ্রুৱ শূন্য *প্ৰবেশ_match_data = cached
      ? static_cast<ধ্রুৱ শূন্য *> (&প্ৰবেশ_class_def)
      : static_cast<ধ্রুৱ শূন্য *> (&প্ৰবেশ_cache_data);
    ধ্রুৱ শূন্য *lookahead_match_data = cached
      ? static_cast<ধ্রুৱ শূন্য *> (&lookahead_class_def)
      : static_cast<ধ্রুৱ শূন্য *> (&lookahead_cache_data);
    তার্কিক backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ধ্রুৱ শূন্য *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<ধ্রুৱ শূন্য *> (&lookahead_cache_data)
      : static_cast<ধ্রুৱ শূন্য *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    সংরচনা ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       প্ৰবেশ_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_মান : get_class_cache_মান
#অন্যথা
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &প্ৰবেশ_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_মান : get_class_মান
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (প্ৰবেশ_class_def, c->buffer->cur())
         : প্ৰবেশ_class_def.get_class (c->buffer->cur().codepoint, প্ৰবেশ_cache_data.cache);
#অন্যথা
    index = cached
         ? get_class_cached2 (প্ৰবেশ_class_def, c->buffer->cur())
          : প্ৰবেশ_class_def.get_class (c->buffer->cur().codepoint);
#endif
    ধ্রুৱ ChainRuleSet &rule_set = এতত্+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ধ্রুৱ hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#অন্যথা
    ধ্রুৱ hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    auto *out = c->serializer->start_embed (*এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, এতত্);

    hb_map_t backtrack_klass_map;
    hb_map_t প্ৰবেশ_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, এতত্, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->প্ৰবেশ_Class_Def.serialize_subset (c, প্ৰবেশ_Class_Def, এতত্, &প্ৰবেশ_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, এতত্, &lookahead_klass_map);

    যদি (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   প্ৰবেশ_klass_map,
						   lookahead_klass_map)))
      return_trace (মিছা);

    ধ্রুৱ hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (এতত্+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (এতত্+প্ৰবেশ_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    পূর্ণাঙ্ক non_zero_index = -1, index = 0;
    তার্কিক ret = সঁচা;
    ধ্রুৱ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    auto last_non_zero = c->serializer->snapshot ();
    বাবে (ধ্রুৱ auto& _ : + hb_enumerate (ruleSet)
			 | hb_filter (প্ৰবেশ_klass_map, hb_first))
    {
      auto *o = out->ruleSet.serialize_append (c->serializer);
      যদি (unlikely (!o))
      {
	ret = মিছা;
	ভঙ্গ;
      }
      যদি (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, এতত্,
			       lookup_map,
			       &backtrack_klass_map,
			       &প্ৰবেশ_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    যদি (!ret || non_zero_index == -1) return_trace (মিছা);

    // prune empty trailing ruleSets
    যদি (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (তার্কিক (out->ruleSet));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) &&
		  backtrackClassDef.sanitize (c, এতত্) &&
		  প্ৰবেশ_Class_Def.sanitize (c, এতত্) &&
		  lookaheadClassDef.sanitize (c, এতত্) &&
		  ruleSet.sanitize (c, এতত্));
  }

  রক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		প্ৰবেশ_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::template LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::template OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

template <typename Types>
সংরচনা ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);

    যদি (!(এতত্+প্ৰবেশ[0]).intersects (glyphs))
      ঘূৰাই মিছা;

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    সংরচনা ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {এতত্, এতত্, এতত্}
    };
    ঘূৰাই chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     প্ৰবেশ.len, প্ৰবেশ.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই সঁচা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);

    যদি (!(এতত্+প্ৰবেশ[0]).intersects (c->glyphs))
      ঘূৰাই;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs))
      ঘূৰাই;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {এতত্, এতত্, এতত্},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ধ্রুৱ HBUINT16 *) backtrack.arrayZ,
				 প্ৰবেশ.len, (ধ্রুৱ HBUINT16 *) প্ৰবেশ.arrayZ + 1,
				 lookahead.len, (ধ্রুৱ HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 এতত্,
				 প্ৰবেশ[0], lookup_context);

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);

    যদি (!(এতত্+প্ৰবেশ[0]).intersects (c->glyphs))
      ঘূৰাই;

    hb_set_t* cur_active_glyphs = c->ঠেলো_cur_active_glyphs ();
    যদি (unlikely (!cur_active_glyphs))
      ঘূৰাই;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    সংরচনা ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {এতত্, এতত্, এতত্}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  প্ৰবেশ.len, প্ৰবেশ.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->বেৰ_কৰো_cur_done_glyphs ();
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ
  {
    যদি (!intersects (c->glyphs))
      ঘূৰাই;

    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);

    (এতত্+প্ৰবেশ[0]).collect_coverage (c->প্ৰবেশ);

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);

    সংরচনা ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {এতত্, এতত্, এতত্}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 প্ৰবেশ.len, প্ৰবেশ.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    যদি (প্ৰবেশ.len > 1)
      (এতত্+প্ৰবেশ[1]).collect_coverage (digest);
    অন্যথা যদি (lookahead.len)
      (এতত্+lookahead[0]).collect_coverage (digest);
    অন্যথা
      *digest = hb_set_digest_t::full ();
  }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    সংরচনা ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {এতত্, এতত্, এতত্}
    };
    ঘূৰাই chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     প্ৰবেশ.len, প্ৰবেশ.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ
  {
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    ঘূৰাই এতত্+প্ৰবেশ[0];
  }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);

    unsigned পূর্ণাঙ্ক index = (এতত্+প্ৰবেশ[0]).get_coverage (c->buffer->cur().codepoint);
    যদি (index == NOT_COVERED) return_trace (মিছা);

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    সংরচনা ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {এতত্, এতত্, এতত্}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      প্ৰবেশ.len, প্ৰবেশ.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  template<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  তার্কিক serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ধ্রুৱ শূন্য* base) ধ্রুৱ
  {
    TRACE_SERIALIZE (এতত্);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    যদি (unlikely (!out->serialize (c->serializer, 0u))) return_trace (মিছা);

    বাবে (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      যদি (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (মিছা);
    }

    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    যদি (unlikely (!c->serializer->embed (এতত্->format))) return_trace (মিছা);

    যদি (!serialize_coverage_offsets (c, backtrack.iter (), এতত্))
      return_trace (মিছা);

    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    যদি (!serialize_coverage_offsets (c, প্ৰবেশ.iter (), এতত্))
      return_trace (মিছা);

    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    যদি (!serialize_coverage_offsets (c, lookahead.iter (), এতত্))
      return_trace (মিছা);

    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ধ্রুৱ hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_গুনতি_2 = c->serializer->copy<HBUINT16> (lookup.len);
    যদি (!lookup_গুনতি_2) return_trace (মিছা);

    unsigned গুনতি = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_গুনতি_2, গুনতি, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!backtrack.sanitize (c, এতত্))) return_trace (মিছা);
    hb_barrier ();
    ধ্রুৱ auto &প্ৰবেশ = StructAfter<decltype (প্ৰবেশ_X)> (backtrack);
    যদি (unlikely (!প্ৰবেশ.sanitize (c, এতত্))) return_trace (মিছা);
    hb_barrier ();
    যদি (unlikely (!প্ৰবেশ.len)) return_trace (মিছা); /* To be consistent with Context. */
    ধ্রুৱ auto &lookahead = StructAfter<decltype (lookaheadX)> (প্ৰবেশ);
    যদি (unlikely (!lookahead.sanitize (c, এতত্))) return_trace (মিছা);
    hb_barrier ();
    ধ্রুৱ auto &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  রক্ষিত:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		প্ৰবেশ_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::template OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (10);
};

সংরচনা ChainContext
{
  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    পক্ষ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    পক্ষ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    পক্ষ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    পক্ষ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }		format;	/* Format identifier */
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
সংরচনা ExtensionFormat1
{
  unsigned পূর্ণাঙ্ক get_type () ধ্রুৱ { ঘূৰাই extensionLookupType; }

  template <typename X>
  ধ্রুৱ X& get_subtable () ধ্রুৱ
  { ঘূৰাই এতত্ + reinterpret_cast<ধ্রুৱ Offset32To<typename T::SubTable> &> (extensionOffset); }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, এতত্))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);

    auto *out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিছা);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    ধ্রুৱ auto& src_offset =
        reinterpret_cast<ধ্রুৱ Offset32To<typename T::SubTable> &> (extensionOffset);
    auto& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, এতত্, get_type ()));
  }

  রক্ষিত:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  সার্ৱজনিক:
  DEFINE_SIZE_STATIC (8);
};

template <typename T>
সংরচনা Extension
{
  unsigned পূর্ণাঙ্ক get_type () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.get_type ();
    স্ৱভাৱ:ঘূৰাই 0;
    }
  }
  template <typename X>
  ধ্রুৱ X& get_subtable () ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.template get_subtable<typename T::SubTable> ();
    স্ৱভাৱ:ঘূৰাই Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  template <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ধ্রুৱ
  {
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); ঘূৰাই u.format1.subset (c);
    স্ৱভাৱ: ঘূৰাই c->default_return_মান ();
    }
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ধ্রুৱ
  {
    যদি (unlikely (!c->may_dispatch (এতত্, &u.format.v))) ঘূৰাই c->no_dispatch_return_মান ();
    TRACE_DISPATCH (এতত্, u.format.v);
    ৱিকল্প (u.format.v) {
    পক্ষ 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    স্ৱভাৱ:return_trace (c->default_return_মান ());
    }
  }

  রক্ষিত:
  সঙ্ঘ {
  সংরচনা { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

সংরচনা hb_ot_layout_lookup_accelerator_t
{
  template <typename TLookup>
  স্থির hb_ot_layout_lookup_accelerator_t *create (ধ্রুৱ TLookup &lookup)
  {
    unsigned গুনতি = lookup.get_subtable_গুনতি ();

    unsigned size = পরিমাণ (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * পরিমাণ (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    গুনতি * পরিমাণ (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    auto *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    যদি (unlikely (!thiz))
      ঘূৰাই nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    যেতিয়ালৈ (গুনতি && !thiz->subtables[গুনতি - 1].apply_func)
      গুনতি--;

    thiz->digest.init ();
    বাবে (auto& subtable : hb_iter (thiz->subtables, গুনতি))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->গুনতি = গুনতি;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    বাবে (unsigned i = 0; i < গুনতি; i++)
      যদি (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    ঘূৰাই thiz;
  }

  শূন্য fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    বাবে (unsigned i = 0; i < গুনতি; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  তার্কিক may_have (hb_codepoint_t g) ধ্রুৱ
  { ঘূৰাই digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  তার্কিক apply (hb_ot_apply_context_t *c, তার্কিক use_cache) ধ্রুৱ
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
	ঘূৰাই subtables[0].apply_cached_no_digest (c);
#endif
      ঘূৰাই subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    যদি (use_cache)
    {
      ঘূৰাই
      + hb_iter (hb_iter (subtables, গুনতি))
      | hb_map ([&c] (ধ্রুৱ hb_accelerate_subtables_context_t::hb_applicable_t &_) { ঘূৰাই _.apply_cached (c); })
      | hb_any
      ;
    }
    অন্যথা
#endif
    {
      ঘূৰাই
      + hb_iter (hb_iter (subtables, গুনতি))
      | hb_map ([&c] (ধ্রুৱ hb_accelerate_subtables_context_t::hb_applicable_t &_) { ঘূৰাই _.apply (c); })
      | hb_any
      ;
    }
    ঘূৰাই মিছা;
  }

  তার্কিক cache_enter (hb_ot_apply_context_t *c) ধ্রুৱ
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ঘূৰাই subtable_cache_user_idx != (unsigned) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#অন্যথা
    ঘূৰাই মিছা;
#endif
  }
  শূন্য cache_leave (hb_ot_apply_context_t *c) ধ্রুৱ
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  গুপ্ত:
  unsigned গুনতি = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  unsigned subtable_cache_user_idx = (unsigned) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

সংরচনা GSUBGPOS
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

  size_t get_size () ধ্রুৱ
  {
    যদি (version.major != 1) ঘূৰাই version.static_size;

    ঘূৰাই min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#অন্যথা
	   0
#endif
	   ;
  }

  ধ্রুৱ ScriptList &get_script_list () ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && scriptList2) ঘূৰাই এতত্ + scriptList2;
#endif
    ঘূৰাই এতত্ + scriptList;
  }
  ধ্রুৱ FeatureList &get_feature_list () ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && featureList2) ঘূৰাই এতত্ + featureList2;
#endif
    ঘূৰাই এতত্ + featureList;
  }
  template <typename TLookup>
  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    যদি (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (এতত্, get_size ())))
      return_trace (মিছা);
    hb_barrier ();

    তার্কিক script_list_ok = scriptList.sanitize (c, এতত্);
    তার্কিক feature_list_ok = featureList.sanitize (c, এতত্);
    তার্কিক lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, এতত্);

#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u)
    {
      যদি (scriptList2) script_list_ok = scriptList2.sanitize (c, এতত্);
      যদি (featureList2) feature_list_ok = featureList2.sanitize (c, এতত্);
      যদি (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, এতত্);
    }
#endif

    যদি (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (মিছা);

#ifndef HB_NO_VAR
    যদি (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, এতত্, &get_feature_list ()))))
      return_trace (মিছা);
#endif

    return_trace (সঁচা);
  }

  template <typename TLookup>
  তার্কিক subset (hb_subset_layout_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    যদি (version.major != 1) return_trace (মিছা);

    auto *out = c->subset_context->serializer->start_embed (এতত্);
    যদি (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (মিছা);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u)
    {
      যদি (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (মিছা);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      যদি (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 এতত্, c);
      অন্যথা
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 এতত্, c);

      যদি (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 এতত্, c);
      অন্যথা
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 এতত্, c);

      যদি (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, এতত্, c);
      অন্যথা
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, এতত্, c);
    }
    অন্যথা
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     এতত্,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     এতত্,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					এতত্,
					c);
    }

#ifndef HB_NO_VAR
    যদি (version.to_int () >= 0x00010001u)
    {
      auto snapshot = c->subset_context->serializer->snapshot ();
      যদি (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (মিছা);

      // if all axes are pinned all feature vars are dropped.
      তার্কিক ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, এতত্, c);
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

    return_trace (সঁচা);
  }

  unsigned পূর্ণাঙ্ক get_lookup_গুনতি () ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && lookupList2)
      ঘূৰাই (এতত্ + lookupList2).len;
#endif
    ঘূৰাই (এতত্ + lookupList).len;
  }
  ধ্রুৱ Lookup& get_lookup (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && lookupList2)
      ঘূৰাই (এতত্ + lookupList2)[i];
#endif
    ঘূৰাই (এতত্ + lookupList)[i];
  }
  ধ্রুৱ FeatureVariations &get_feature_variations () ধ্রুৱ
  {
    ঘূৰাই (version.to_int () >= 0x00010001u && hb_barrier ()) ? এতত্+featureVars : Null (FeatureVariations);
  }

  তার্কিক has_data () ধ্রুৱ { ঘূৰাই version.to_int (); }
  unsigned পূর্ণাঙ্ক get_script_গুনতি () ধ্রুৱ
  { ঘূৰাই get_script_list ().len; }
  ধ্রুৱ Tag& get_script_tag (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই get_script_list ().get_tag (i); }
  unsigned পূর্ণাঙ্ক get_script_tags (unsigned পূর্ণাঙ্ক start_offset,
				unsigned পূর্ণাঙ্ক *script_গুনতি /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ধ্রুৱ
  { ঘূৰাই get_script_list ().get_tags (start_offset, script_গুনতি, script_tags); }
  ধ্রুৱ Script& get_script (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই get_script_list ()[i]; }
  তার্কিক find_script_index (hb_tag_t tag, unsigned পূর্ণাঙ্ক *index) ধ্রুৱ
  { ঘূৰাই get_script_list ().find_index (tag, index); }

  unsigned পূর্ণাঙ্ক get_feature_গুনতি () ধ্রুৱ
  { ঘূৰাই get_feature_list ().len; }
  hb_tag_t get_feature_tag (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  unsigned পূর্ণাঙ্ক get_feature_tags (unsigned পূর্ণাঙ্ক start_offset,
				 unsigned পূর্ণাঙ্ক *feature_গুনতি_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) ধ্রুৱ
  { ঘূৰাই get_feature_list ().get_tags (start_offset, feature_গুনতি_2, feature_tags); }
  ধ্রুৱ Feature& get_feature (unsigned পূর্ণাঙ্ক i) ধ্রুৱ
  { ঘূৰাই get_feature_list ()[i]; }
  তার্কিক find_feature_index (hb_tag_t tag, unsigned পূর্ণাঙ্ক *index) ধ্রুৱ
  { ঘূৰাই get_feature_list ().find_index (tag, index); }

  তার্কিক find_variations_index (ধ্রুৱ পূর্ণাঙ্ক *coords, unsigned পূর্ণাঙ্ক অঙ্ক_coords,
			      unsigned পূর্ণাঙ্ক *index,
			      ItemVarStoreInstancer *instancer) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    ঘূৰাই মিছা;
#endif
    ঘূৰাই get_feature_variations ().find_index (coords, অঙ্ক_coords, index, instancer);
  }
  তার্কিক get_feature_variations_state (ধ্রুৱ পূর্ণাঙ্ক *coords,
				     unsigned পূর্ণাঙ্ক অঙ্ক_coords,
				     unsigned পূর্ণাঙ্ক *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<unsigned> *lookup_conditions) ধ্রুৱ
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    ঘূৰাই সঁচা;
#অন্যথা
    find_variations_index (coords, অঙ্ক_coords, index, instancer);
    ঘূৰাই get_feature_variations ().collect_lookup_conditions (
	coords,
	অঙ্ক_coords,
	instancer,
	lookup_conditions);
#endif
  }
  তার্কিক get_lookup_variations (unsigned feature_index,
                              unsigned variations_index,
                              hb_sorted_array_t<ধ্রুৱ unsigned> conditions,
                              hb_set_t *lookup_indexes) ধ্রুৱ
  {
#ifdef HB_NO_VAR
    ঘূৰাই মিছা;
#অন্যথা
    ঘূৰাই get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  ধ্রুৱ Feature& get_feature_variation (unsigned পূর্ণাঙ্ক feature_index,
					unsigned পূর্ণাঙ্ক variations_index) ধ্রুৱ
  {
#ifndef HB_NO_VAR
    যদি (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      ধ্রুৱ Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      যদি (feature)
	ঘূৰাই *feature;
    }
#endif
    ঘূৰাই get_feature (feature_index);
  }

  শূন্য feature_variation_collect_lookups (ধ্রুৱ hb_set_t *feature_indexes,
					  ধ্রুৱ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) ধ্রুৱ
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  শূন্য collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুৱ
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  শূন্য collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ধ্রুৱ
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  template <typename TLookup>
  শূন্য closure_lookups (hb_face_t      *face,
			ধ্রুৱ hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) ধ্রুৱ
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::template dispatch_recurse_func<hb_closure_lookups_context_t>);

    বাবে (unsigned lookup_index : *lookup_indexes)
      reinterpret_cast<ধ্রুৱ TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  শূন্য prune_langsys (ধ্রুৱ hb_map_t *duplicate_feature_map,
                      ধ্রুৱ hb_set_t *layout_scripts,
                      hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) ধ্রুৱ
  {
    hb_prune_langsys_context_t c (এতত্, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    unsigned গুনতি = get_script_গুনতি ();
    বাবে (unsigned script_index = 0; script_index < গুনতি; script_index++)
    {
      ধ্রুৱ Tag& tag = get_script_tag (script_index);
      যদি (!layout_scripts->has (tag)) অনুৱর্ত;
      ধ্রুৱ Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  শূন্য prune_features (ধ্রুৱ hb_map_t *lookup_indices, /* IN */
		       ধ্রুৱ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       ধ্রুৱ hb_hashmap_t<unsigned, ধ্রুৱ Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) ধ্রুৱ
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
      ঘূৰাই;
    }
#endif

    বাবে (unsigned i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      যদি (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        অনুৱর্ত;


      ধ্রুৱ Feature *f = &(get_feature (i));
      ধ্রুৱ Feature** p = nullptr;
      যদি (feature_substitutes_map->has (i, &p))
        f = *p;

      যদি (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        অনুৱর্ত;

      যদি (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  শূন্য collect_name_ids (ধ্রুৱ hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         ধ্রুৱ hb_hashmap_t<unsigned, ধ্রুৱ Feature*> *feature_substitutes_map,
                         ধ্রুৱ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         ধ্রুৱ hb_set_t *catch_all_features,
                         তার্কিক all_axes_pinned HB_UNUSED) ধ্রুৱ
  {
    unsigned গুনতি = get_feature_গুনতি ();
    বাবে (unsigned i = 0 ; i < গুনতি; i++)
    {
      যদি (!feature_index_map->has (i)) অনুৱর্ত;
      hb_tag_t tag = get_feature_tag (i);
      ধ্রুৱ Feature *feature = &get_feature (i);
      ধ্রুৱ Feature **substitute;
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
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [এতত্] (unsigned i) { ঘূৰাই get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  template <typename T>
  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = সঁচা;
      এতত্->table = sc.reference_table<T> (face);

      যদি (unlikely (এতত্->table->is_blocklisted (এতত্->table.get_blob (), face)))
      {
	hb_blob_destroy (এতত্->table.get_blob ());
	এতত্->table = hb_blob_get_empty ();
      }

      এতত্->lookup_গুনতি = table->get_lookup_গুনতি ();

      এতত্->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (এতত্->lookup_গুনতি, পরিমাণ (*accels));
      যদি (unlikely (!এতত্->accels))
      {
	এতত্->lookup_গুনতি = 0;
	এতত্->table.destroy ();
	এতত্->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      বাবে (unsigned পূর্ণাঙ্ক i = 0; i < এতত্->lookup_গুনতি; i++)
      {
	auto *accel = এতত্->accels[i].get_relaxed ();
	যদি (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (এতত্->accels);
      এতত্->table.destroy ();
    }

    hb_blob_t *get_blob () ধ্রুৱ { ঘূৰাই table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (unsigned lookup_index) ধ্রুৱ
    {
      যদি (unlikely (lookup_index >= lookup_গুনতি)) ঘূৰাই nullptr;

    retry:
      auto *accel = accels[lookup_index].get_acquire ();
      যদি (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	যদি (unlikely (!accel))
	  ঘূৰাই nullptr;

	যদি (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  গচ্ছ retry;
	}
      }

      ঘূৰাই accel;
    }

    hb_blob_ptr_t<T> table;
    unsigned পূর্ণাঙ্ক lookup_গুনতি;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  রক্ষিত:
  template <typename TLookup>
  ধ্রুৱ Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ধ্রুৱ Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  template <typename TLookup>
  ধ্রুৱ Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  template <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { ঘূৰাই reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  template <typename TLookup>
  ধ্রুৱ Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  ধ্রুৱ Offset16To<RecordListOfFeature>& subset_feature_list_offset () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { ঘূৰাই reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  ধ্রুৱ Offset32To<RecordListOfFeature>& subset_feature_list2_offset () ধ্রুৱ
  { ঘূৰাই reinterpret_cast<ধ্রুৱ Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  ধ্রুৱ শূন্য* get_lookup_list_field_offset () ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (version.to_int () >= 0x00010002u && lookupList2) ঘূৰাই &lookupList2;
#endif
    ঘূৰাই &lookupList;
  }

  template <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  স্থির তার্কিক serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ধ্রুৱ OffsetIn& in,
				       ধ্রুৱ Base *base,
				       Ts&&... ds)
  {
    out = 0;
    যদি (in.is_null ()) ঘূৰাই মিছা;

    auto *s = c->serializer;
    s->push ();
    তার্কিক ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    যদি (ret) s->add_link (out, s->বেৰ_কৰো_pack ());
    অন্যথা s->বেৰ_কৰো_discard ();
    ঘূৰাই ret;
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
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
