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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-buffer.hh"
#अन्तर्भाव "hb-map.hh"
#अन्तर्भाव "hb-set.hh"
#अन्तर्भाव "hb-ot-map.hh"
#अन्तर्भाव "hb-ot-layout-common.hh"
#अन्तर्भाव "hb-ot-layout-gdef-table.hh"
#अन्तर्भाव "hb-depend-data.hh"


नामाकाश OT {


संरचना hb_intersects_context_t :
       hb_dispatch_context_t<hb_intersects_context_t, तार्किक>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { परत obj.intersects (हे->glyphs); }
  स्थिर return_t default_return_मूल्य () { परत असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) स्थिरांक { परत r; }

  स्थिरांक hb_set_t *glyphs;

  hb_intersects_context_t (स्थिरांक hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

संरचना hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, तार्किक>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { परत obj.may_have_non_1to1 (); }
  स्थिर return_t default_return_मूल्य () { परत असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) स्थिरांक { परत r; }
};

संरचना hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  प्रकारव्याख्या return_t (*recurse_func_t) (hb_closure_context_t *c, अचिन्हित lookup_index, hb_set_t *covered_seq_indicies, अचिन्हित seq_index, अचिन्हित end_index);
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { obj.closure (हे); परत hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }
  शून्य recurse (अचिन्हित lookup_index, hb_set_t *covered_seq_indicies, अचिन्हित seq_index, अचिन्हित end_index)
  {
    जर (unlikely (nesting_level_डावा == 0 || !recurse_func))
      परत;

    nesting_level_डावा--;
    recurse_func (हे, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_डावा++;
  }

  शून्य reset_lookup_visit_मोजणी ()
  { lookup_मोजणी = 0; }

  तार्किक lookup_limit_exceeded ()
  { परत lookup_मोजणी > HB_MAX_LOOKUP_VISIT_मोजणी; }

  तार्किक should_visit_lookup (अचिन्हित पूर्णांक lookup_index)
  {
    जर (lookup_मोजणी++ > HB_MAX_LOOKUP_VISIT_मोजणी)
      परत असत्य;

    जर (is_lookup_done (lookup_index))
      परत असत्य;

    परत सत्य;
  }

  तार्किक is_lookup_done (अचिन्हित पूर्णांक lookup_index)
  {
    जर (unlikely (done_lookups_glyph_मोजणी->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      परत सत्य;

    /* Have we visited this lookup with the current set of glyphs? */
    अचिन्हित काढा = glyphs->get_population ();
    जर (unlikely (काढा == HB_MAP_मूल्य_INVALID)) // Avoid sentinel interference.
      काढा--;
    जर (done_lookups_glyph_मोजणी->get (lookup_index) != काढा)
    {
      done_lookups_glyph_मोजणी->set (lookup_index, काढा);

      जर (!done_lookups_glyph_set->has (lookup_index))
      {
	जर (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  परत सत्य;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    जर (unlikely (covered_glyph_set->in_error ()))
      परत सत्य;
    जर (parent_active_glyphs ().is_subset (*covered_glyph_set))
      परत सत्य;

    covered_glyph_set->union_ (parent_active_glyphs ());
    परत असत्य;
  }

  स्थिरांक hb_set_t& previous_parent_active_glyphs () {
    जर (active_glyphs_चळत.length <= 1)
      परत *glyphs;

    परत active_glyphs_चळत[active_glyphs_चळत.length - 2];
  }

  स्थिरांक hb_set_t& parent_active_glyphs ()
  {
    जर (!active_glyphs_चळत)
      परत *glyphs;

    परत active_glyphs_चळत.tail ();
  }

  hb_set_t* ढकला_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_चळत.push ();
    जर (unlikely (active_glyphs_चळत.in_error ()))
      परत nullptr;
    परत s;
  }

  तार्किक काढा_cur_done_glyphs ()
  {
    जर (!active_glyphs_चळत)
      परत असत्य;

    active_glyphs_चळत.काढा ();
    परत सत्य;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_चळत;
  recurse_func_t recurse_func = nullptr;
  अचिन्हित पूर्णांक nesting_level_डावा;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_मोजणी_2,
			hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			अचिन्हित पूर्णांक nesting_level_डावा_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_डावा (nesting_level_डावा_2),
			  done_lookups_glyph_मोजणी (done_lookups_glyph_मोजणी_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  शून्य set_recurse_func (recurse_func_t func) { recurse_func = func; }

  शून्य flush ()
  {
    output->del_range (face->get_अंक_glyphs (), HB_SET_मूल्य_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_चळत.काढा ();
    active_glyphs_चळत.reset ();
  }

  खाजगी:
  hb_map_t *done_lookups_glyph_मोजणी;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  अचिन्हित पूर्णांक lookup_मोजणी = 0;
};



संरचना hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  प्रकारव्याख्या return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, अचिन्हित lookup_index);
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { obj.closure_lookups (हे); परत hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }
  शून्य recurse (अचिन्हित lookup_index)
  {
    जर (unlikely (nesting_level_डावा == 0 || !recurse_func))
      परत;

    /* Return if new lookup was recursed to before. */
    जर (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      परत;

    nesting_level_डावा--;
    recurse_func (हे, lookup_index);
    nesting_level_डावा++;
  }

  शून्य set_lookup_visited (अचिन्हित lookup_index)
  { visited_lookups->add (lookup_index); }

  शून्य set_lookup_inactive (अचिन्हित lookup_index)
  { inactive_lookups->add (lookup_index); }

  तार्किक lookup_limit_exceeded ()
  {
    तार्किक ret = lookup_मोजणी > HB_MAX_LOOKUP_VISIT_मोजणी;
    जर (ret)
      DEBUG_संदेश (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    परत ret; }

  तार्किक is_lookup_visited (अचिन्हित lookup_index)
  {
    जर (unlikely (lookup_मोजणी++ > HB_MAX_LOOKUP_VISIT_मोजणी))
    {
      DEBUG_संदेश (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_मोजणी, lookup_index);
      परत सत्य;
    }

    जर (unlikely (visited_lookups->in_error ()))
      परत सत्य;

    परत visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  स्थिरांक hb_set_t *glyphs;
  recurse_func_t recurse_func;
  अचिन्हित पूर्णांक nesting_level_डावा;

  hb_closure_lookups_context_t (hb_face_t *face_,
				स्थिरांक hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				अचिन्हित nesting_level_डावा_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_डावा (nesting_level_डावा_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_मोजणी (0) {}

  शून्य set_recurse_func (recurse_func_t func) { recurse_func = func; }

  खाजगी:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  अचिन्हित पूर्णांक lookup_मोजणी;
};

संरचना hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, तार्किक>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { परत obj.would_apply (हे); }
  स्थिर return_t default_return_मूल्य () { परत असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) स्थिरांक { परत r; }

  hb_face_t *face;
  स्थिरांक hb_codepoint_t *glyphs;
  अचिन्हित पूर्णांक len;
  तार्किक zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    स्थिरांक hb_codepoint_t *glyphs_,
			    अचिन्हित पूर्णांक len_,
			    तार्किक zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

संरचना hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  प्रकारव्याख्या return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, अचिन्हित पूर्णांक lookup_index);
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { obj.collect_glyphs (हे); परत hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }
  शून्य recurse (अचिन्हित पूर्णांक lookup_index)
  {
    जर (unlikely (nesting_level_डावा == 0 || !recurse_func))
      परत;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    जर (output == hb_set_get_empty ())
      परत;

    /* Return if new lookup was recursed to before. */
    जर (recursed_lookups->has (lookup_index))
      परत;

    hb_set_t *old_before = before;
    hb_set_t *old_आदान  = आदान;
    hb_set_t *old_after  = after;
    before = आदान = after = hb_set_get_empty ();

    nesting_level_डावा--;
    recurse_func (हे, lookup_index);
    nesting_level_डावा++;

    before = old_before;
    आदान  = old_आदान;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *आदान;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  अचिन्हित पूर्णांक nesting_level_डावा;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_आदान,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       अचिन्हित पूर्णांक nesting_level_डावा_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      आदान  (glyphs_आदान  ? glyphs_आदान  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_डावा (nesting_level_डावा_2) {}
  ~hb_collect_glyphs_context_t () { hb_set_destroy (recursed_lookups); }

  शून्य set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

संरचना hb_depend_context_t :
       hb_dispatch_context_t<hb_depend_context_t>
{
  गणना glyph_set_type_t
  {
    GLYPH_SET_CLASS,
    GLYPH_SET_COVERAGE,
    GLYPH_SET_SINGLE
  };

  स्थिर constexpr hb_codepoint_t SINGLE_GLYPH_ACTIVE_FLAG = 0x80000000u;

  संरचना glyph_set_cache_किल्ली_t
  {
    glyph_set_cache_किल्ली_t () = पूर्वनिर्धारित;
    glyph_set_cache_किल्ली_t (स्थिरांक शून्य *data_, अचिन्हित मूल्य_3, glyph_set_type_t type_)
      : data (data_), value (मूल्य_3), type (type_) {}

    तार्किक चालक == (स्थिरांक glyph_set_cache_किल्ली_t &o) स्थिरांक
    { परत data == o.data && value == o.value && type == o.type; }

    uint32_t hash () स्थिरांक
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      परत current * 31 + hb_hash ((अचिन्हित) type);
    }

    स्थिरांक शून्य *data = nullptr;
    अचिन्हित value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  संरचना context_set_cache_किल्ली_t
  {
    context_set_cache_किल्ली_t () = पूर्वनिर्धारित;
    context_set_cache_किल्ली_t (स्थिरांक शून्य *rule_,
			     स्थिरांक शून्य *record_,
			     स्थिरांक शून्य *data_,
			     hb_codepoint_t active_glyphs_,
			     अचिन्हित मूल्य_3,
			     अचिन्हित format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (मूल्य_3), format (format_) {}

    तार्किक चालक == (स्थिरांक context_set_cache_किल्ली_t &o) स्थिरांक
    {
      परत rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () स्थिरांक
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      परत current * 31 + hb_hash (format);
    }

    स्थिरांक शून्य *rule = nullptr;
    स्थिरांक शून्य *record = nullptr;
    स्थिरांक शून्य *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    अचिन्हित value = 0;
    अचिन्हित format = 0;
  };

  संरचना recurse_किल्ली_t
  {
    recurse_किल्ली_t () = पूर्वनिर्धारित;
    recurse_किल्ली_t (अचिन्हित lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    तार्किक चालक == (स्थिरांक recurse_किल्ली_t &o) स्थिरांक
    {
      परत lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () स्थिरांक
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      परत current;
    }

    अचिन्हित lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  प्रकारव्याख्या return_t (*recurse_func_t) (hb_depend_context_t *c, अचिन्हित lookup_index, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { obj.depend (हे); परत hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }

  शून्य recurse (अचिन्हित lookup_idx, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index)
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
    जर (lookups_seen.has (lookup_idx)) परत;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_किल्ली = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    जर (!recurse_path_to_index.has (path_किल्ली, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      जर (unlikely (!recurse_path_to_index.set (path_किल्ली, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	परत;
      }
    }
    अन्यथा
      path_index = *path;
    recurse_path = path_index;
    recurse_func (हे, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  संरचना active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    तार्किक use_मूळ = असत्य;
  };

  स्थिरांक hb_set_t& parent_active_glyphs ()
  {
    जर (!active_glyphs_चळत_depth)
      परत *glyphs;

    स्थिरांक active_glyphs_frame_t &frame = active_glyphs_चळत[active_glyphs_चळत_depth - 1];
    परत frame.use_मूळ ? *glyphs : frame.glyphs;
  }

  hb_set_t* ढकला_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    जर (active_glyphs_चळत_depth == active_glyphs_चळत.length &&
	unlikely (!active_glyphs_चळत.ढकला_or_fail ()))
      परत nullptr;

    active_glyphs_frame_t &frame = active_glyphs_चळत[active_glyphs_चळत_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_मूळ = असत्य;
    परत &frame.glyphs;
  }

  तार्किक ढकला_मूळ_active_glyphs ()
  {
    जर (active_glyphs_चळत_depth == active_glyphs_चळत.length &&
	unlikely (!active_glyphs_चळत.ढकला_or_fail ()))
      परत असत्य;

    active_glyphs_चळत[active_glyphs_चळत_depth++].use_मूळ = सत्य;
    परत सत्य;
  }

  तार्किक काढा_cur_done_glyphs ()
  {
    जर (!active_glyphs_चळत_depth)
      परत असत्य;

    active_glyphs_चळत_depth--;
    परत सत्य;
  }

  तार्किक get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    जर (!active_glyphs_चळत_depth ||
	active_glyphs_चळत[active_glyphs_चळत_depth - 1].use_मूळ)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      परत सत्य;
    }

    active_glyphs_frame_t &frame = active_glyphs_चळत[active_glyphs_चळत_depth - 1];
    *active_idx = frame.active_idx;
    जर (*active_idx != HB_CODEPOINT_INVALID)
      परत सत्य;

    hb_codepoint_t glyph;
    जर (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    अन्यथा जर (!find_or_create_recurse_set (frame.glyphs, active_idx))
      परत असत्य;

    frame.active_idx = *active_idx;
    परत सत्य;
  }

  तार्किक get_context_set_cache_किल्ली (स्थिरांक शून्य *rule,
				  स्थिरांक शून्य *record,
				  स्थिरांक शून्य *data,
				  अचिन्हित value,
				  अचिन्हित format,
				  context_set_cache_किल्ली_t *key)
  {
    hb_codepoint_t active_idx;
    जर (!get_parent_active_glyphs_index (&active_idx))
      परत असत्य;
    *key = context_set_cache_किल्ली_t (rule, record, data, active_idx, value, format);
    परत सत्य;
  }

  तार्किक get_recurse_किल्ली (अचिन्हित lookup_idx, recurse_किल्ली_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    जर (!get_parent_active_glyphs_index (&active_idx))
      परत असत्य;

    *key = recurse_किल्ली_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    परत !completed_recursions.has (*key);
  }

  शून्य finish_recurse (स्थिरांक recurse_किल्ली_t &key)
  { depend_data->check_success (completed_recursions.set (key, सत्य)); }

  शून्य reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  तार्किक find_or_create_recurse_set (स्थिरांक hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    जर (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      परत सत्य;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    जर (unlikely (!copy))
      परत depend_data->fail ();
    copy->set (set);
    जर (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      परत depend_data->fail ();
    }

    *index = recurse_sets.length;
    जर (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      परत depend_data->fail ();
    }
    जर (unlikely (!recurse_sets.ढकला_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      परत depend_data->fail ();
    जर (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      परत depend_data->fail ();
    परत सत्य;
  }

  स्थिरांक hb_set_t *find_or_create_glyph_set (स्थिरांक शून्य *data,
                                             अचिन्हित value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (स्थिरांक hb_set_t *set);

  संरचना depend_scratch_t
  {
    शून्य clear ()
    {
      preliminary_context.clear ();
      position_scratch.clear ();
      pos_glyphs.clear ();
      position_context.clear ();
      disjunctive_indices.clear ();
      filtered_disjunctive_indices.clear ();
      backtrack_sets.clear ();
      lookahead_sets.clear ();
      आदान_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<स्थिरांक hb_set_t *> backtrack_sets;
    hb_vector_t<स्थिरांक hb_set_t *> lookahead_sets;
    hb_vector_t<स्थिरांक hb_set_t *> आदान_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<अचिन्हित> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    जर (scratch)
      depend_scratch_pool = scratch->next;
    अन्यथा
    {
      scratch = (depend_scratch_t *) hb_malloc (आकार (depend_scratch_t));
      जर (unlikely (!scratch))
      {
        depend_data->fail ();
        परत nullptr;
      }
      नवीन (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    परत scratch;
  }

  शून्य release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_चळत;
  अचिन्हित active_glyphs_चळत_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<स्थिरांक hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_किल्ली_t, तार्किक> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_किल्ली_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_किल्ली_t, hb_codepoint_t> context_set_cache;
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
    पर्यंत (depend_scratch_pool)
    {
      depend_scratch_t *scratch = depend_scratch_pool;
      depend_scratch_pool = scratch->next;
      scratch->~depend_scratch_t ();
      hb_free (scratch);
    }
  }
  शून्य set_recurse_func (recurse_func_t func) { recurse_func = func; }
};

ढाँचा <typename set_t>
संरचना hb_collect_coverage_context_t :
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, स्थिरांक Coverage &>
{
  प्रकारव्याख्या स्थिरांक Coverage &return_t; // Stoopid that we have to dupe this here.
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { परत obj.get_coverage (); }
  स्थिर return_t default_return_मूल्य () { परत Null (Coverage); }
  तार्किक stop_sublookup_iteration (return_t r) स्थिरांक
  {
    r.collect_coverage (set);
    परत असत्य;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

संरचना matcher_t
{
  प्रकारव्याख्या तार्किक (*match_func_t) (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data);

  ढाँचा <typename context_t>
  शून्य init (स्थिरांक context_t *c, तार्किक context_match = असत्य)
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

  शून्य set_match_func (match_func_t match_func_,
		       स्थिरांक शून्य *match_data_)
  { match_func = match_func_; match_data = match_data_; }

  गणना may_match_t {
    MATCH_NO,
    MATCH_YES,
    MATCH_MAYBE
  };

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_match_t may_match (hb_glyph_info_t &info,
			 hb_codepoint_t glyph_data) स्थिरांक
  {
    जर (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      परत MATCH_NO;

    जर (match_func)
      परत match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    परत MATCH_MAYBE;
  }

  गणना may_skip_t {
    SKIP_NO,
    SKIP_YES,
    SKIP_MAYBE
  };

  ढाँचा <typename context_t>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  may_skip_t may_skip (स्थिरांक context_t *c,
		       स्थिरांक hb_glyph_info_t &info) स्थिरांक
  {
    जर (!c->check_glyph_property (&info, lookup_props))
      परत SKIP_YES;

    जर (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      परत SKIP_MAYBE;

    परत SKIP_NO;
  }

  सार्वजनिक:
  अचिन्हित पूर्णांक lookup_props = 0;
  hb_mask_t mask = -1;
  तार्किक ignore_zwnj = असत्य;
  तार्किक ignore_zwj = असत्य;
  तार्किक ignore_hidden = असत्य;
  तार्किक per_syllable = असत्य;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  स्थिरांक शून्य *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
संरचना glyph_data_t
{
  ढाँचा <typename T>
  glyph_data_t (स्थिरांक T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (आकार (T) == 3)
#endif
  {
    static_assert (आकार (T) == 2 || आकार (T) == 3, "invalid glyph data size");
  }

  अचिन्हित चालक [] (अचिन्हित i) स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (wide)
      परत ((स्थिरांक HBUINT24 *) data)[i];
#endif
    परत ((स्थिरांक HBUINT16 *) data)[i];
  }

  स्थिरांक शून्य *data;
#ifndef HB_NO_BEYOND_64K
  तार्किक wide;
#endif
};

ढाँचा <typename context_t>
संरचना skipping_iterator_t
{
  शून्य init (context_t *c_, तार्किक context_match = असत्य)
  {
    c = c_;
    end = c->buffer->len;
    match_glyph_data16 = nullptr;
#ifndef HB_NO_BEYOND_64K
    match_glyph_data24 = nullptr;
#endif
    matcher.init (c, context_match);
  }
  शून्य set_lookup_props (अचिन्हित पूर्णांक lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  शून्य set_match_func (matcher_t::match_func_t match_func_,
		       स्थिरांक शून्य *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  शून्य set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (स्थिरांक HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (स्थिरांक HBUINT24 *) glyph_data.data : nullptr;
#अन्यथा
    match_glyph_data16 = (स्थिरांक HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य reset (अचिन्हित पूर्णांक start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  शून्य reset_back (अचिन्हित पूर्णांक start_index_, तार्किक from_out_buffer = असत्य)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य reset_fast (अचिन्हित पूर्णांक start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (स्थिरांक hb_glyph_info_t &info) स्थिरांक
  { परत matcher.may_skip (c, info); }

  गणना match_t {
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
    जर (unlikely (skip == matcher_t::SKIP_YES))
      परत SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    जर (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      परत MATCH;

    जर (skip == matcher_t::SKIP_NO)
      परत NOT_MATCH;

    परत SKIP;
  }

  तार्किक next (अचिन्हित *unsafe_to = nullptr)
  {
    स्वतः *info = c->buffer->info;
    स्थिरांक चिन्हित stop = (चिन्हित) end - 1;
    पर्यंत ((चिन्हित) idx < stop)
    {
      idx++;
      निवड (match (info[idx]))
      {
	स्थिती MATCH:
	{
	  advance_glyph_data ();
	  परत सत्य;
	}
	स्थिती NOT_MATCH:
	{
	  जर (unsafe_to)
	    *unsafe_to = idx + 1;
	  परत असत्य;
	}
	स्थिती SKIP:
	  सुरूठेवा;
      }
    }
    जर (unsafe_to)
      *unsafe_to = end;
    परत असत्य;
  }
  तार्किक prev (अचिन्हित *unsafe_from = nullptr)
  {
    स्वतः *out_info = c->buffer->out_info;
    स्थिरांक अचिन्हित stop = 0;
    पर्यंत (idx > stop)
    {
      idx--;
      निवड (match (out_info[idx]))
      {
	स्थिती MATCH:
	{
	  advance_glyph_data ();
	  परत सत्य;
	}
	स्थिती NOT_MATCH:
	{
	  जर (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  परत असत्य;
	}
	स्थिती SKIP:
	  सुरूठेवा;
      }
    }
    जर (unsafe_from)
      *unsafe_from = 0;
    परत असत्य;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    जर (match_glyph_data16) परत *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    अन्यथा
    जर (match_glyph_data24) परत *match_glyph_data24;
#endif
    परत 0;
  }
  HB_ALWAYS_INLINE
  शून्य
  advance_glyph_data ()
  {
    जर (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    अन्यथा
    जर (match_glyph_data24) match_glyph_data24++;
#endif
  }

  अचिन्हित पूर्णांक idx;
  संरक्षित:
  context_t *c;
  matcher_t matcher;
  स्थिरांक HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  स्थिरांक HBUINT24 *match_glyph_data24;
#endif

  अचिन्हित पूर्णांक end;
};

संरचना hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, तार्किक, HB_DEBUG_APPLY>
{
  स्थिरांक अक्षर *get_name () { परत "APPLY"; }
  प्रकारव्याख्या return_t (*recurse_func_t) (hb_ot_apply_context_t *c, अचिन्हित पूर्णांक lookup_index);

  ढाँचा <typename T>
  स्थिर ओळीत स्वतः apply_ (स्थिरांक T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  ढाँचा <typename T>
  स्थिर ओळीत स्वतः apply_ (स्थिरांक T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { परत apply_(obj, हे, hb_prioritize); }

  स्थिर return_t default_return_मूल्य () { परत असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) स्थिरांक { परत r; }
  return_t recurse (अचिन्हित पूर्णांक sub_lookup_index)
  {
    प्रतिज्ञा (recurse_func);
    जर (unlikely (nesting_level_डावा == 0))
    {
      buffer->successful = असत्य;
      परत default_return_मूल्य ();
    }

    buffer->max_ops--;
    जर (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = असत्य;
      परत default_return_मूल्य ();
    }

    nesting_level_डावा--;
    तार्किक ret = recurse_func (हे, sub_lookup_index);
    nesting_level_डावा++;
    परत ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_आदान, iter_context;

  अचिन्हित पूर्णांक table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  स्थिरांक GDEF &gdef;
  स्थिरांक GDEF::accelerator_t &gdef_accel;
  स्थिरांक ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  अचिन्हित पूर्णांक lookup_index = (अचिन्हित) -1;
  अचिन्हित पूर्णांक lookup_props = 0;
  अचिन्हित पूर्णांक nesting_level_डावा = HB_MAX_NESTING_LEVEL;

  तार्किक has_glyph_classes;
  तार्किक auto_zwnj = सत्य;
  तार्किक auto_zwj = सत्य;
  तार्किक per_syllable = असत्य;
  तार्किक random = असत्य;
  अचिन्हित new_syllables = (अचिन्हित) -1;

  चिन्हित last_base = -1; // GPOS uses
  अचिन्हित last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t चळत_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  शून्य *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (अचिन्हित पूर्णांक table_index_,
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
#अन्यथा
			      Null (GDEF)
#endif
			     ),
			gdef_accel (
#ifndef HB_NO_OT_LAYOUT
			      *face->table.GDEF
#अन्यथा
			      Null (GDEF::accelerator_t)
#endif
			     ),
			var_store (gdef.get_var_store ()),
			var_store_cache (var_store_cache_),
			direction (buffer_->props.direction),
			has_glyph_classes (gdef.has_glyph_classes ())
  {
    init_iters ();
    match_positions.set_storage (चळत_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    जर (buffer->messaging ())
    {
      प्रतिज्ञा (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = हे;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    जर (buffer->messaging ())
    {
      प्रतिज्ञा (buffer->changed_func == buffer_changed_trampoline);
      प्रतिज्ञा (buffer->changed_data == हे);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  शून्य init_iters ()
  {
    iter_आदान.init (हे, असत्य);
    iter_context.init (हे, सत्य);
  }

  शून्य set_lookup_mask (hb_mask_t mask, तार्किक init = सत्य) { lookup_mask = mask; last_base = -1; last_base_until = 0; जर (init) init_iters (); }
  शून्य set_auto_zwj (तार्किक auto_zwj_, तार्किक init = सत्य) { auto_zwj = auto_zwj_; जर (init) init_iters (); }
  शून्य set_auto_zwnj (तार्किक auto_zwnj_, तार्किक init = सत्य) { auto_zwnj = auto_zwnj_; जर (init) init_iters (); }
  शून्य set_per_syllable (तार्किक per_syllable_, तार्किक init = सत्य) { per_syllable = per_syllable_; जर (init) init_iters (); }
  शून्य set_random (तार्किक random_) { random = random_; }
  शून्य set_recurse_func (recurse_func_t func) { recurse_func = func; }
  शून्य set_lookup_index (अचिन्हित पूर्णांक lookup_index_) { lookup_index = lookup_index_; }
  शून्य set_lookup_props (अचिन्हित पूर्णांक lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_संख्या ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    परत buffer->random_state;
  }

  स्थिर शून्य buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, शून्य *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  शून्य buffer_changed ()
  {
    buffer->update_digest ();
    जर (likely (has_glyph_classes))
      साठी (अचिन्हित i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  तार्किक match_properties_mark (स्थिरांक hb_glyph_info_t *info,
			      अचिन्हित पूर्णांक    glyph_props,
			      अचिन्हित पूर्णांक    match_props) स्थिरांक
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    जर (match_props & LookupFlag::UseMarkFilteringSet)
    {
      अचिन्हित set_index = match_props >> 16;
      परत gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    जर (match_props & LookupFlag::MarkAttachmentType)
      परत (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    परत सत्य;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_glyph_property (स्थिरांक hb_glyph_info_t *info,
			     अचिन्हित match_props) स्थिरांक
  {
    अचिन्हित पूर्णांक glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    जर (glyph_props & match_props & LookupFlag::IgnoreFlags)
      परत असत्य;

    जर (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      परत match_properties_mark (info, glyph_props, match_props);

    परत सत्य;
  }

  शून्य _set_glyph_class (hb_codepoint_t glyph_index,
			  अचिन्हित पूर्णांक class_guess = 0,
			  तार्किक ligature = असत्य,
			  तार्किक component = असत्य)
  {
    buffer->digest.add (glyph_index);

    जर (new_syllables != (अचिन्हित) -1)
      buffer->cur().syllable() = new_syllables;

    अचिन्हित पूर्णांक props = _hb_glyph_info_get_glyph_props (&buffer->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    जर (ligature)
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
    जर (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    जर (likely (has_glyph_classes))
    {
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    अन्यथा जर (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    अन्यथा
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  शून्य _set_glyph_class_props (hb_glyph_info_t &info,
			       अचिन्हित पूर्णांक props,
			       hb_codepoint_t glyph_index)
  {
    props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
    _hb_glyph_info_set_glyph_props (&info, props | gdef_accel.get_glyph_props (glyph_index));
  }
  शून्य _set_glyph_class_props (hb_glyph_info_t &info)
  {
    _set_glyph_class_props (info,
			    _hb_glyph_info_get_glyph_props (&info),
			    info.codepoint);
  }

  शून्य replace_glyph (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    (शून्य) buffer->replace_glyph (glyph_index);
  }
  शून्य replace_glyph_inplace (hb_codepoint_t glyph_index)
  {
    _set_glyph_class (glyph_index);
    buffer->cur().codepoint = glyph_index;
  }
  शून्य replace_glyph_with_ligature (hb_codepoint_t glyph_index,
				    अचिन्हित पूर्णांक class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, सत्य);
    (शून्य) buffer->replace_glyph (glyph_index);
  }
  शून्य output_glyph_for_component (hb_codepoint_t glyph_index,
				   अचिन्हित पूर्णांक class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, असत्य, सत्य);
    (शून्य) buffer->output_glyph (glyph_index);
  }
};

गणना वर्ग hb_ot_subtable_cache_op_t
{
  ENTER,
  LEAVE,
};

संरचना hb_accelerate_subtables_context_t :
       hb_dispatch_context_t<hb_accelerate_subtables_context_t>
{
  ढाँचा <typename T>
  स्थिर ओळीत स्वतः collect_second_glyphs_ (स्थिरांक T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (शून्य, obj.collect_second_glyphs (digest) )
  ढाँचा <typename T>
  स्थिर ओळीत शून्य collect_second_glyphs_ (स्थिरांक T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  ढाँचा <typename T>
  स्थिर ओळीत स्वतः apply_ (स्थिरांक T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<1>) HB_RETURN (तार्किक, obj->apply (c, external_cache) )
  ढाँचा <typename T>
  स्थिर ओळीत स्वतः apply_ (स्थिरांक T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<0>) HB_RETURN (तार्किक, obj->apply (c) )
  ढाँचा <typename T>
  स्थिर ओळीत तार्किक apply_to (स्थिरांक शून्य *obj, hb_ot_apply_context_t *c, शून्य *external_cache)
  {
    स्थिरांक T *typed_obj = (स्थिरांक T *) obj;
    परत apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ढाँचा <typename T>
  स्थिर ओळीत स्वतः apply_cached_ (स्थिरांक T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<2>) HB_RETURN (तार्किक, obj->apply_cached (c, external_cache) )
  ढाँचा <typename T>
  स्थिर ओळीत स्वतः apply_cached_ (स्थिरांक T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<1>) HB_RETURN (तार्किक, obj->apply (c, external_cache) )
  ढाँचा <typename T>
  स्थिर ओळीत स्वतः apply_cached_ (स्थिरांक T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<0>) HB_RETURN (तार्किक, obj->apply (c) )
  ढाँचा <typename T>
  स्थिर ओळीत तार्किक apply_cached_to (स्थिरांक शून्य *obj, hb_ot_apply_context_t *c, शून्य *external_cache)
  {
    स्थिरांक T *typed_obj = (स्थिरांक T *) obj;
    परत apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  ढाँचा <typename T>
  स्थिर ओळीत स्वतः cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (तार्किक, T::cache_func (c, op) )
  ढाँचा <typename T=शून्य>
  स्थिर ओळीत तार्किक cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { परत असत्य; }
  ढाँचा <typename Type>
  स्थिर ओळीत तार्किक cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    परत cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  प्रकारव्याख्या तार्किक (*hb_apply_func_t) (स्थिरांक शून्य *obj, hb_ot_apply_context_t *c, शून्य *external_cache);
  प्रकारव्याख्या तार्किक (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  संरचना hb_applicable_t
  {
    मित्र संरचना hb_accelerate_subtables_context_t;
    मित्र संरचना hb_ot_layout_lookup_accelerator_t;

    ढाँचा <typename T>
    शून्य init (स्थिरांक T &obj_,
	       hb_apply_func_t apply_func_
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
	       , hb_apply_func_t apply_cached_func_
	       , hb_cache_func_t cache_func_
	       , शून्य *external_cache_
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
    तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
    {
      परत digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    तार्किक apply_no_digest (hb_ot_apply_context_t *c) स्थिरांक
    {
      परत apply_func (obj, c, nullptr);
    }
#अन्यथा
    तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
    {
      परत digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    तार्किक apply_cached (hb_ot_apply_context_t *c) स्थिरांक
    {
      परत digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    तार्किक apply_no_digest (hb_ot_apply_context_t *c) स्थिरांक
    {
      परत apply_func (obj, c, external_cache);
    }
    तार्किक apply_cached_no_digest (hb_ot_apply_context_t *c) स्थिरांक
    {
      परत apply_cached_func (obj, c, external_cache);
    }

    तार्किक cache_enter (hb_ot_apply_context_t *c) स्थिरांक
    {
      परत cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    शून्य cache_leave (hb_ot_apply_context_t *c) स्थिरांक
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    खाजगी:
    स्थिरांक शून्य *obj;
    hb_apply_func_t apply_func;
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    hb_apply_func_t apply_cached_func;
    hb_cache_func_t cache_func;
    शून्य *external_cache;
#endif
    hb_set_digest_t digest;
  };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ढाँचा <typename T>
  स्वतः cache_cost (स्थिरांक T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  ढाँचा <typename T>
  स्वतः cache_cost (स्थिरांक T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  ढाँचा <typename T>
  स्वतः external_cache_create (स्थिरांक T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  ढाँचा <typename T>
  स्वतः external_cache_create (स्थिरांक T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    शून्य *external_cache = nullptr;
    जर (i < 8)
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
    अचिन्हित cost = cache_cost (obj, hb_prioritize);
    जर (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    परत hb_empty_t ();
  }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }

  hb_accelerate_subtables_context_t (hb_applicable_t *array_) :
				     array (array_) {}

  hb_applicable_t *array;
  अचिन्हित i = 0;
  hb_set_digest_t digest_second;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  अचिन्हित subtable_cache_user_idx = (अचिन्हित) -1;
  अचिन्हित subtable_cache_user_cost = 0;
#endif
};


प्रकारव्याख्या तार्किक (*intersects_func_t) (स्थिरांक hb_set_t *glyphs, अचिन्हित value, स्थिरांक शून्य *data, शून्य *cache);
प्रकारव्याख्या शून्य (*intersected_glyphs_func_t) (स्थिरांक hb_set_t *glyphs, स्थिरांक शून्य *data, अचिन्हित value, hb_set_t *intersected_glyphs, शून्य *cache);
प्रकारव्याख्या शून्य (*collect_glyphs_func_t) (hb_set_t *glyphs, अचिन्हित value, स्थिरांक शून्य *data);
प्रकारव्याख्या तार्किक (*match_func_t) (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data);
प्रकारव्याख्या अचिन्हित (*get_मूल्य_func_t) (hb_glyph_info_t &info, स्थिरांक शून्य *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
संरचना hb_ot_layout_ruleset_digest_t
{
  शून्य init () { mask = 0; }
  शून्य set_full () { mask = (uint64_t) -1; }
  शून्य add (अचिन्हित value) { mask |= (uint64_t) 1 << (value & 63); }
  तार्किक may_have (अचिन्हित value) स्थिरांक { परत mask & ((uint64_t) 1 << (value & 63)); }

  uint64_t mask;
};

संरचना ContextClosureFuncs
{
  intersects_func_t intersects;
  intersected_glyphs_func_t intersected_glyphs;
};
संरचना ContextCollectGlyphsFuncs
{
  collect_glyphs_func_t collect;
};
संरचना ContextApplyFuncs
{
  match_func_t match;
};
संरचना ChainContextApplyFuncs
{
  match_func_t match[3];
};


स्थिर ओळीत तार्किक intersects_glyph (स्थिरांक hb_set_t *glyphs, अचिन्हित value, स्थिरांक शून्य *data HB_UNUSED, शून्य *cache HB_UNUSED)
{
  परत glyphs->has (value);
}
स्थिर ओळीत तार्किक intersects_class (स्थिरांक hb_set_t *glyphs, अचिन्हित value, स्थिरांक शून्य *data, शून्य *cache)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  जर (map->has (value, &cached_v))
    परत *cached_v;

  तार्किक v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  परत v;
}
स्थिर ओळीत तार्किक intersects_coverage (स्थिरांक hb_set_t *glyphs, अचिन्हित value, स्थिरांक शून्य *data, शून्य *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  परत (data+coverage).intersects (glyphs);
}


स्थिर ओळीत शून्य intersected_glyph (स्थिरांक hb_set_t *glyphs HB_UNUSED, स्थिरांक शून्य *data, अचिन्हित value, hb_set_t *intersected_glyphs, HB_UNUSED शून्य *cache)
{
  अचिन्हित g = reinterpret_cast<स्थिरांक HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<अचिन्हित, hb_set_t>;

स्थिर ओळीत शून्य intersected_class_glyphs (स्थिरांक hb_set_t *glyphs, स्थिरांक शून्य *data, अचिन्हित value, hb_set_t *intersected_glyphs, शून्य *cache)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  जर (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    परत;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

स्थिर ओळीत शून्य intersected_coverage_glyphs (स्थिरांक hb_set_t *glyphs, स्थिरांक शून्य *data, अचिन्हित value, hb_set_t *intersected_glyphs, HB_UNUSED शून्य *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


स्थिर ओळीत तार्किक array_is_subset_of (स्थिरांक hb_set_t *glyphs,
				       अचिन्हित पूर्णांक मोजणी,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       स्थिरांक शून्य *intersects_data,
				       शून्य *cache)
{
  साठी (अचिन्हित i = 0; i < मोजणी; i++)
    जर (!intersects_func (glyphs, values[i], intersects_data, cache)) परत असत्य;
  परत सत्य;
}


स्थिर ओळीत शून्य collect_glyph (hb_set_t *glyphs, अचिन्हित value, स्थिरांक शून्य *data HB_UNUSED)
{
  glyphs->add (value);
}
स्थिर ओळीत शून्य collect_class (hb_set_t *glyphs, अचिन्हित value, स्थिरांक शून्य *data)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
स्थिर ओळीत शून्य collect_coverage (hb_set_t *glyphs, अचिन्हित value, स्थिरांक शून्य *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

ओळीत स्थिरांक hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (स्थिरांक शून्य *data,
                                                अचिन्हित value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_किल्ली_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  जर (glyph_set_to_index.has (key, &existing))
    परत glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  जर (unlikely (!set))
  {
    depend_data->fail ();
    परत nullptr;
  }

  जर (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  अन्यथा जर (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  अन्यथा
    set->add (value);
  जर (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    परत nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  जर (unlikely (!glyph_sets.ढकला_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    परत nullptr;
  }
  जर (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    परत nullptr;
  }
  परत glyph_sets[index].get ();
}

ओळीत hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (स्थिरांक hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  जर (cached_context_set_indices.has (key, &existing))
    परत *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  जर (unlikely (index == HB_CODEPOINT_INVALID))
    परत HB_CODEPOINT_INVALID;
  जर (unlikely (!cached_context_set_indices.set (key, index)))
    परत depend_data->fail_invalid ();
  परत index;
}

स्थिर ओळीत शून्य collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  अचिन्हित पूर्णांक मोजणी,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  स्थिरांक शून्य *collect_data)
{
  साठी (अचिन्हित i = 0; i < मोजणी; i++)
    collect_func (glyphs, values[i], collect_data);
}


स्थिर ओळीत तार्किक match_always (hb_glyph_info_t &info HB_UNUSED, अचिन्हित value HB_UNUSED, स्थिरांक शून्य *data HB_UNUSED)
{
  परत सत्य;
}
स्थिर ओळीत तार्किक match_glyph (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data HB_UNUSED)
{
  परत info.codepoint == value;
}
स्थिर ओळीत तार्किक match_class (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *>(data);
  परत class_def.get_class (info.codepoint) == value;
}
स्थिर ओळीत अचिन्हित get_class_मूल्य (hb_glyph_info_t &info, स्थिरांक शून्य *data)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *> (data);
  परत class_def.get_class (info.codepoint);
}
संरचना match_class_cache_data_t
{
  स्थिरांक ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
स्थिर ओळीत तार्किक match_class_cache (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data)
{
  स्थिरांक स्वतः &cache_data = *reinterpret_cast<स्थिरांक match_class_cache_data_t *> (data);
  परत cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
स्थिर ओळीत अचिन्हित get_class_cache_मूल्य (hb_glyph_info_t &info, स्थिरांक शून्य *data)
{
  स्थिरांक स्वतः &cache_data = *reinterpret_cast<स्थिरांक match_class_cache_data_t *> (data);
  परत cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
स्थिर ओळीत अचिन्हित get_class_cached (स्थिरांक ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = info.syllable();
  जर (klass < 255)
    परत klass;
  klass = class_def.get_class (info.codepoint);
  जर (likely (klass < 255))
    info.syllable() = klass;
  परत klass;
}
स्थिर ओळीत तार्किक match_class_cached (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *>(data);
  परत get_class_cached (class_def, info) == value;
}
स्थिर ओळीत अचिन्हित get_class_cached_मूल्य (hb_glyph_info_t &info, स्थिरांक शून्य *data)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *> (data);
  परत get_class_cached (class_def, info);
}
स्थिर ओळीत अचिन्हित get_class_cached1 (स्थिरांक ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = info.syllable() & 0x0F;
  जर (klass < 15)
    परत klass;
  klass = class_def.get_class (info.codepoint);
  जर (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  परत klass;
}
स्थिर ओळीत तार्किक match_class_cached1 (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *>(data);
  परत get_class_cached1 (class_def, info) == value;
}
स्थिर ओळीत अचिन्हित get_class_cached2 (स्थिरांक ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = (info.syllable() & 0xF0) >> 4;
  जर (klass < 15)
    परत klass;
  klass = class_def.get_class (info.codepoint);
  जर (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  परत klass;
}
स्थिर ओळीत तार्किक match_class_cached2 (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *>(data);
  परत get_class_cached2 (class_def, info) == value;
}
स्थिर ओळीत अचिन्हित get_class_cached2_मूल्य (hb_glyph_info_t &info, स्थिरांक शून्य *data)
{
  स्थिरांक ClassDef &class_def = *reinterpret_cast<स्थिरांक ClassDef *> (data);
  परत get_class_cached2 (class_def, info);
}
स्थिर ओळीत तार्किक match_coverage (hb_glyph_info_t &info, अचिन्हित value, स्थिरांक शून्य *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  परत (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

स्थिर ओळीत तार्किक would_match_आदान (hb_would_apply_context_t *c,
				      अचिन्हित पूर्णांक मोजणी, /* Including the first glyph (not matched) */
				      glyph_data_t आदान, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      स्थिरांक शून्य *match_data)
{
  जर (मोजणी != c->len)
    परत असत्य;

  साठी (अचिन्हित पूर्णांक i = 1; i < मोजणी; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    जर (likely (!match_func (info, आदान[i - 1], match_data)))
      परत असत्य;
  }

  परत सत्य;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#अन्यथा
ओळीत
#endif
स्थिर तार्किक match_आदान (hb_ot_apply_context_t *c,
			 अचिन्हित पूर्णांक मोजणी, /* Including the first glyph (not matched) */
			 glyph_data_t आदान, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 स्थिरांक शून्य *match_data,
			 अचिन्हित पूर्णांक *end_position,
			 अचिन्हित पूर्णांक *p_total_component_मोजणी = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  जर (मोजणी == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    जर (p_total_component_मोजणी)
      *p_total_component_मोजणी = hb_glyph_info_get_lig_अंक_comps (&buffer->cur());
    return_trace (सत्य);
  }

  जर (unlikely (मोजणी > HB_MAX_CONTEXT_LENGTH)) return_trace (असत्य);

  स्वतः &skippy_iter = c->iter_आदान;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (आदान);

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

  अचिन्हित पूर्णांक total_component_मोजणी = 0;

  अचिन्हित पूर्णांक first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  अचिन्हित पूर्णांक first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  गणना {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  साठी (अचिन्हित पूर्णांक i = 1; i < मोजणी; i++)
  {
    अचिन्हित unsafe_to;
    जर (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (असत्य);
    }

    जर (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (असत्य);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    अचिन्हित पूर्णांक this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    अचिन्हित पूर्णांक this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    जर (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      जर (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	जर (ligbase == LIGBASE_NOT_CHECKED)
	{
	  तार्किक found = असत्य;
	  स्थिरांक स्वतः *out = buffer->out_info;
	  अचिन्हित पूर्णांक j = buffer->out_len;
	  पर्यंत (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    जर (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = सत्य;
	      थांबा;
	    }
	    j--;
	  }

	  जर (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  अन्यथा
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	जर (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (असत्य);
      }
    }
    अन्यथा
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      जर (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (असत्य);
    }

    total_component_मोजणी += hb_glyph_info_get_lig_अंक_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  जर (p_total_component_मोजणी)
  {
    total_component_मोजणी += hb_glyph_info_get_lig_अंक_comps (&buffer->cur());
    *p_total_component_मोजणी = total_component_मोजणी;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (सत्य);
}
स्थिर ओळीत तार्किक ligate_आदान (hb_ot_apply_context_t *c,
				 अचिन्हित पूर्णांक मोजणी, /* Including the first glyph */
				 अचिन्हित पूर्णांक match_end,
				 hb_codepoint_t lig_glyph,
				 अचिन्हित पूर्णांक total_component_मोजणी)
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

  तार्किक is_base_ligature = _hb_glyph_info_is_base_glyph (&buffer->info[c->match_positions.arrayZ[0]]);
  तार्किक is_mark_ligature = _hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[0]]);
  साठी (अचिन्हित पूर्णांक i = 1; i < मोजणी; i++)
    जर (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = असत्य;
      is_mark_ligature = असत्य;
      थांबा;
    }
  तार्किक is_ligature = !is_base_ligature && !is_mark_ligature;

  अचिन्हित पूर्णांक klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  अचिन्हित पूर्णांक lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  अचिन्हित पूर्णांक last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  अचिन्हित पूर्णांक last_अंक_components = hb_glyph_info_get_lig_अंक_comps (&buffer->cur());
  अचिन्हित पूर्णांक components_so_far = last_अंक_components;

  जर (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_मोजणी);
    जर (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  साठी (अचिन्हित पूर्णांक i = 1; i < मोजणी; i++)
  {
    पर्यंत (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      जर (is_ligature)
      {
	अचिन्हित पूर्णांक this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	जर (this_comp == 0)
	  this_comp = last_अंक_components;
	प्रतिज्ञा (components_so_far >= last_अंक_components);
	अचिन्हित पूर्णांक new_lig_comp = components_so_far - last_अंक_components +
				    hb_min (this_comp, last_अंक_components);
	  _hb_glyph_info_set_lig_props_for_mark (&buffer->cur(), lig_id, new_lig_comp);
      }
      (शून्य) buffer->next_glyph ();
    }

    last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
    last_अंक_components = hb_glyph_info_get_lig_अंक_comps_in_ligation (&buffer->cur());
    components_so_far += last_अंक_components;

    /* Skip the base glyph */
    buffer->idx++;
  }

  जर (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    साठी (अचिन्हित i = buffer->idx; i < buffer->len; ++i)
    {
      जर (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) थांबा;

      अचिन्हित this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      जर (!this_comp) थांबा;

      प्रतिज्ञा (components_so_far >= last_अंक_components);
      अचिन्हित new_lig_comp = components_so_far - last_अंक_components +
			      hb_min (this_comp, last_अंक_components);
      _hb_glyph_info_set_lig_props_for_mark (&buffer->info[i], lig_id, new_lig_comp);
    }
  }
  return_trace (सत्य);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#अन्यथा
ओळीत
#endif
स्थिर तार्किक match_backtrack (hb_ot_apply_context_t *c,
			     अचिन्हित पूर्णांक मोजणी,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     स्थिरांक शून्य *match_data,
			     अचिन्हित पूर्णांक *match_start)
{
  TRACE_APPLY (nullptr);

  जर (!मोजणी)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (सत्य);
  }

  स्वतः &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
  {
    अचिन्हित unsafe_from;
    जर (!skippy_iter.prev (&unsafe_from))
    {
      *match_start = unsafe_from;
      return_trace (असत्य);
    }
  }

  *match_start = skippy_iter.idx;
  return_trace (सत्य);
}

#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#अन्यथा
ओळीत
#endif
स्थिर तार्किक match_lookahead (hb_ot_apply_context_t *c,
			     अचिन्हित पूर्णांक मोजणी,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     स्थिरांक शून्य *match_data,
			     अचिन्हित पूर्णांक start_index,
			     अचिन्हित पूर्णांक *end_index)
{
  TRACE_APPLY (nullptr);

  जर (!मोजणी)
  {
    *end_index = start_index;
    return_trace (सत्य);
  }

  स्वतः &skippy_iter = c->iter_context;
  प्रतिज्ञा (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
  {
    अचिन्हित unsafe_to;
    जर (!skippy_iter.next (&unsafe_to))
    {
      *end_index = unsafe_to;
      return_trace (असत्य);
    }
  }

  *end_index = skippy_iter.idx + 1;
  return_trace (सत्य);
}



संरचना LookupRecord
{
  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक hb_map_t         *lookup_map) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->embed (*हे);
    जर (unlikely (!out)) return_trace (असत्य);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

स्थिर ओळीत अचिन्हित serialize_lookuprecord_array (hb_serialize_context_t *c,
						     स्थिरांक hb_array_t<स्थिरांक LookupRecord> lookupRecords,
						     स्थिरांक hb_map_t *lookup_map)
{
  अचिन्हित मोजणी = 0;
  साठी (स्थिरांक LookupRecord& r : lookupRecords)
  {
    जर (!lookup_map->has (r.lookupListIndex))
      सुरूठेवा;

    जर (!r.serialize (c, lookup_map))
      परत 0;

    मोजणी++;
  }
  परत मोजणी;
}

गणना ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

स्थिर ओळीत स्थिरांक hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			स्थिरांक शून्य *data,
			अचिन्हित value,
			तार्किक filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<अचिन्हित> *compact_workspace = nullptr)
{
  निवड (context_format)
  {
    स्थिती ContextFormat::SimpleContext:
      परत c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    स्थिती ContextFormat::ClassBasedContext:
    {
      स्थिरांक hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      जर (unlikely (!class_set))
	परत nullptr;
      जर (!filter_class_by_parent)
	परत class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      थांबा;
    }
    स्थिती ContextFormat::CoverageBasedContext:
      परत c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  जर (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    परत nullptr;
  }
  परत scratch;
}

स्थिर ओळीत शून्य context_closure_recurse_lookups (hb_closure_context_t *c,
					     अचिन्हित आदान_मोजणी, glyph_data_t आदान,
					     अचिन्हित lookup_मोजणी_2,
					     स्थिरांक LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     अचिन्हित value,
					     ContextFormat context_format,
					     स्थिरांक शून्य *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     शून्य *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  साठी (अचिन्हित पूर्णांक i = 0; i < lookup_मोजणी_2; i++)
  {
    अचिन्हित seqIndex = lookupRecord[i].sequenceIndex;
    जर (seqIndex >= आदान_मोजणी) सुरूठेवा;

    तार्किक has_pos_glyphs = असत्य;

    जर (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = सत्य;
      pos_glyphs.clear ();
      जर (seqIndex == 0)
      {
        निवड (context_format) {
        स्थिती ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          थांबा;
        स्थिती ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          थांबा;
        स्थिती ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          थांबा;
        }
      }
      अन्यथा
      {
        स्थिरांक शून्य *आदान_data = आदान.data;
        अचिन्हित आदान_मूल्य = seqIndex - 1;
        जर (context_format != ContextFormat::SimpleContext)
        {
          आदान_data = data;
          आदान_मूल्य = आदान[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, आदान_data, आदान_मूल्य, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs))
      परत;
    जर (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } अन्यथा {
      *cur_active_glyphs = *c->glyphs;
    }

    अचिन्हित endIndex = आदान_मोजणी;
    जर (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->काढा_cur_done_glyphs ();
  }
}

स्थिर तार्किक
context_depend_sets_are_cached (hb_depend_context_t *c,
				अचिन्हित आदान_मोजणी_2,
				अचिन्हित lookup_मोजणी,
				स्थिरांक LookupRecord lookup_records[],
				स्थिरांक शून्य *rule,
				अचिन्हित value,
				ContextFormat context_format,
				स्थिरांक शून्य *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  तार्किक have_active_idx = असत्य;
  साठी (अचिन्हित i = 0; i < lookup_मोजणी; i++)
  {
    जर (lookup_records[i].sequenceIndex >= आदान_मोजणी_2)
      सुरूठेवा;

    जर (!have_active_idx)
    {
      जर (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	परत असत्य;
      have_active_idx = सत्य;
    }

    hb_depend_context_t::context_set_cache_किल्ली_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    जर (!c->context_set_cache.has (key, &cached_context_set))
      परत असत्य;
    जर (unlikely (!cached_context_sets->ढकला_or_fail (*cached_context_set)))
      परत c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  परत have_active_idx;
}

स्थिर ओळीत शून्य context_depend_recurse_lookups (hb_depend_context_t *c,
					     अचिन्हित आदान_मोजणी, glyph_data_t आदान,
					     अचिन्हित lookup_मोजणी_2,
					     स्थिरांक LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     स्थिरांक शून्य *rule,
					     अचिन्हित value,
					     ContextFormat context_format,
					     स्थिरांक शून्य *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     शून्य *cache,
					     स्थिरांक hb_set_t &preliminary_context,
					     स्थिरांक hb_vector_t<स्थिरांक hb_set_t *> *आदान_position_glyphs,
					     स्थिरांक hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  जर (unlikely (आदान_मोजणी > HB_MAX_CONTEXT_LENGTH))
    परत;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  अचिन्हित cached_context_index = 0;
  साठी (अचिन्हित पूर्णांक i = 0; i < lookup_मोजणी_2; i++)
  {
    अचिन्हित seqIndex = lookupRecord[i].sequenceIndex;
    जर (seqIndex >= आदान_मोजणी)
      सुरूठेवा;

    hb_depend_context_t::context_set_cache_किल्ली_t context_cache_किल्ली;
    जर (unlikely (!c->get_context_set_cache_किल्ली (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_किल्ली)))
      परत;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    जर (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    अन्यथा जर (c->context_set_cache.has (context_cache_किल्ली, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    अन्यथा
    {
      जर (unlikely (!आदान_position_glyphs))
      {
	c->depend_data->fail ();
	परत;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_मूल्य_INVALID;
      पर्यंत (preliminary_context.next (&elem)) {
	जर (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} अन्यथा {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      साठी (अचिन्हित j = 0; j < आदान_position_glyphs->length; j++) {
	जर (j != seqIndex && (*आदान_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*आदान_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_मूल्य_INVALID;
      पर्यंत (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	स्थिरांक hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	जर (unlikely (!original_set)) सुरूठेवा;

	/* Dependency context sets are never inverted. */
	जर (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      साठी (अचिन्हित j = 0; j < आदान_position_glyphs->length; j++) {
	जर (j != seqIndex && (*आदान_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  जर (!(*आदान_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    तार्किक cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*आदान_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*आदान_position_glyphs)[j]);
	    जर (unlikely (idx == HB_CODEPOINT_INVALID))
	      परत;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      जर (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	परत;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      जर (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	परत;

      जर (unlikely (!c->context_set_cache.set (context_cache_किल्ली, context_set_idx)))
      {
	c->depend_data->fail ();
	परत;
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
    तार्किक nested_contextual = (saved_context_set_index != HB_CODEPOINT_INVALID);

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
    जर (आदान_मोजणी > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    जर (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    तार्किक has_pos_glyphs = असत्य;

    जर (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = सत्य;
      pos_glyphs.clear ();
      जर (seqIndex == 0)
      {
        निवड (context_format) {
        स्थिती ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          थांबा;
        स्थिती ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          थांबा;
        स्थिती ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          थांबा;
        }
      }
      अन्यथा
      {
        स्थिरांक शून्य *आदान_data = आदान.data;
        अचिन्हित आदान_मूल्य = seqIndex - 1;
        जर (context_format != ContextFormat::SimpleContext)
        {
          आदान_data = data;
          आदान_मूल्य = आदान[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, आदान_data, आदान_मूल्य, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    जर (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      जर (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	जर (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->ढकला_cur_active_glyphs (active_idx);
      जर (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	परत;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    अन्यथा जर (unlikely (!c->ढकला_मूळ_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      परत;
    }

    अचिन्हित endIndex = आदान_मोजणी;
    जर (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->काढा_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

ढाँचा <typename context_t>
स्थिर ओळीत शून्य recurse_lookups (context_t *c,
                                    अचिन्हित पूर्णांक lookup_मोजणी_2,
                                    स्थिरांक LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  साठी (अचिन्हित पूर्णांक i = 0; i < lookup_मोजणी_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

स्थिर ओळीत शून्य apply_lookup (hb_ot_apply_context_t *c,
				 अचिन्हित पूर्णांक मोजणी, /* Including the first glyph */
				 अचिन्हित पूर्णांक lookup_मोजणी_2,
				 स्थिरांक LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 अचिन्हित पूर्णांक match_end)
{
  hb_buffer_t *buffer = c->buffer;
  पूर्णांक end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    अचिन्हित पूर्णांक bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    पूर्णांक delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    साठी (अचिन्हित पूर्णांक j = 0; j < मोजणी; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  साठी (अचिन्हित पूर्णांक i = 0; i < lookup_मोजणी_2 && buffer->successful; i++)
  {
    अचिन्हित पूर्णांक idx = lookupRecord[i].sequenceIndex;
    जर (idx >= मोजणी)
      सुरूठेवा;

    अचिन्हित पूर्णांक orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    जर (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      सुरूठेवा;

    जर (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      थांबा;

    जर (unlikely (buffer->max_ops <= 0))
      थांबा;

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      जर (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (अचिन्हित) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    तार्किक ret = c->recurse (lookupRecord[i].lookupListIndex);

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      जर (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (अचिन्हित) lookupRecord[i].lookupListIndex);
    }

    जर (!ret)
      सुरूठेवा;

    अचिन्हित पूर्णांक new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    पूर्णांक delta = new_len - orig_len;

    जर (!delta)
      सुरूठेवा;

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
    जर (end < पूर्णांक (c->match_positions.arrayZ[idx]))
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

    अचिन्हित पूर्णांक next = idx + 1; /* next now is the position after the recursed lookup. */

    जर (delta > 0)
    {
      जर (unlikely (delta + मोजणी > HB_MAX_CONTEXT_LENGTH))
	थांबा;
      जर (unlikely (मोजणी + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (मोजणी + delta)))
        परत;
    }
    अन्यथा
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (पूर्णांक) next - (पूर्णांक) मोजणी);
      next -= delta;
    }

    /* Shift! */
    स_हटाओ (c->match_positions + next + delta, c->match_positions + next,
	     (मोजणी - next) * आकार (c->match_positions.arrayZ[0]));
    next += delta;
    मोजणी += delta;

    /* Fill in new entries. */
    साठी (अचिन्हित पूर्णांक j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    साठी (; next < मोजणी; next++)
      c->match_positions.arrayZ[next] += delta;
  }

  प्रतिज्ञा (end >= 0);
  (शून्य) buffer->move_to (end);
}



/* Contextual lookups */

संरचना ContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  स्थिरांक शून्य *intersects_data;
  शून्य *intersects_cache;
  शून्य *intersected_glyphs_cache;
};

प्रकारव्याख्या ContextClosureLookupContext ContextDependLookupContext;

संरचना ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  स्थिरांक शून्य *collect_data;
};

संरचना ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  स्थिरांक शून्य *match_data;
  get_मूल्य_func_t get_मूल्य;
};

स्थिर ओळीत तार्किक context_intersects (स्थिरांक hb_set_t *glyphs,
				       अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
				       glyph_data_t आदान, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  परत array_is_subset_of (glyphs,
			     आदान_मोजणी ? आदान_मोजणी - 1 : 0, आदान,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

स्थिर ओळीत शून्य context_closure_lookup (hb_closure_context_t *c,
					   अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
					   glyph_data_t आदान, /* Array of input values--start with second glyph */
					   अचिन्हित पूर्णांक lookup_मोजणी_2,
					   स्थिरांक LookupRecord lookupRecord[],
					   अचिन्हित value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  जर (context_intersects (c->glyphs,
			  आदान_मोजणी, आदान,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     आदान_मोजणी, आदान,
				     lookup_मोजणी_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

स्थिर ओळीत शून्य context_depend_lookup (hb_depend_context_t *c,
					  अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
					  glyph_data_t आदान, /* Array of input values--start with second glyph */
					  अचिन्हित पूर्णांक lookup_मोजणी_2,
					  स्थिरांक LookupRecord lookupRecord[],
					  स्थिरांक शून्य *rule,
					  अचिन्हित value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  जर (!context_intersects (c->glyphs,
			   आदान_मोजणी, आदान,
			   lookup_context))
    परत;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  जर (unlikely (!scratch))
    परत;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  तार्किक context_sets_cached = context_depend_sets_are_cached (
    c, आदान_मोजणी, lookup_मोजणी_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  जर (unlikely (!c->depend_data->successful))
    परत;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<स्थिरांक hb_set_t *> &आदान_position_glyphs = scratch->आदान_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  जर (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    स्थिरांक hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      सत्य, &position_scratch, &scratch->compact_workspace);
    जर (unlikely (!pos0_glyphs ||
		  !आदान_position_glyphs.ढकला_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      परत;
    }

    /* Positions 1+ (Input array) */
    साठी (अचिन्हित i = 0; i + 1 < आदान_मोजणी; i++)
    {
      स्थिरांक hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	आदान[i], असत्य, &position_scratch);
      जर (unlikely (!pos_glyphs ||
		    !आदान_position_glyphs.ढकला_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	परत;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  आदान_मोजणी, आदान,
				  lookup_मोजणी_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &आदान_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

स्थिर ओळीत शून्य context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
						  glyph_data_t आदान, /* Array of input values--start with second glyph */
						  अचिन्हित पूर्णांक lookup_मोजणी_2,
						  स्थिरांक LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->आदान,
		 आदान_मोजणी ? आदान_मोजणी - 1 : 0, आदान,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_मोजणी_2, lookupRecord);
}

स्थिर ओळीत तार्किक context_would_apply_lookup (hb_would_apply_context_t *c,
					       अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
					       glyph_data_t आदान, /* Array of input values--start with second glyph */
					       अचिन्हित पूर्णांक lookup_मोजणी_2 HB_UNUSED,
					       स्थिरांक LookupRecord lookupRecord[] HB_UNUSED,
					       स्थिरांक ContextApplyLookupContext &lookup_context)
{
  परत would_match_आदान (c,
			    आदान_मोजणी, आदान,
			    lookup_context.funcs.match, lookup_context.match_data);
}

स्थिर ओळीत तार्किक context_apply_lookup (hb_ot_apply_context_t *c,
					 अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
					 glyph_data_t आदान, /* Array of input values--start with second glyph */
					 अचिन्हित पूर्णांक lookup_मोजणी_2,
					 स्थिरांक LookupRecord lookupRecord[],
					 स्थिरांक ContextApplyLookupContext &lookup_context)
{
  जर (unlikely (आदान_मोजणी > HB_MAX_CONTEXT_LENGTH)) परत असत्य;

  अचिन्हित match_end = 0;
  तार्किक ret = असत्य;
  जर (match_आदान (c,
		   आदान_मोजणी, आदान,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  आदान_मोजणी,
		  lookup_मोजणी_2, lookupRecord,
		  match_end);
    ret = सत्य;
  }
  अन्यथा
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = असत्य;
  }

  परत ret;
}

स्थिर ओळीत तार्किक context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  निवड (op)
  {
    स्थिती hb_ot_subtable_cache_op_t::ENTER:
    {
      जर (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	परत असत्य;
      स्वतः &info = c->buffer->info;
      अचिन्हित मोजणी = c->buffer->len;
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      परत सत्य;
    }
    स्थिती hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (अचिन्हित) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      थांबा;
    }
  }
  परत असत्य;
}

ढाँचा <typename Types>
संरचना Rule
{
  ढाँचा <typename SetTypes, typename RuleType>
  मित्र संरचना RuleSetOf;

  तार्किक intersects (स्थिरांक hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) स्थिरांक
  {
    परत context_intersects (glyphs,
			       आदान_मोजणी, आदान_Z.arrayZ,
			       lookup_context);
  }

  शून्य depend (hb_depend_context_t *c, अचिन्हित value, ContextDependLookupContext &lookup_context) स्थिरांक
  {
    स्थिरांक स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (आदान_Z.as_array ((आदान_मोजणी ? आदान_मोजणी - 1 : 0)));
    context_depend_lookup (c,
			   आदान_मोजणी, आदान_Z.arrayZ,
			   lookup_मोजणी_2, lookupRecord.arrayZ,
			   हे,
			   value, lookup_context);
  }

  शून्य closure (hb_closure_context_t *c, अचिन्हित value, ContextClosureLookupContext &lookup_context) स्थिरांक
  {
    जर (unlikely (c->lookup_limit_exceeded ())) परत;

    स्थिरांक स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (आदान_Z.as_array ((आदान_मोजणी ? आदान_मोजणी - 1 : 0)));
    context_closure_lookup (c,
			    आदान_मोजणी, आदान_Z.arrayZ,
			    lookup_मोजणी_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) स्थिरांक
  {
    जर (unlikely (c->lookup_limit_exceeded ())) परत;
    जर (!intersects (c->glyphs, lookup_context)) परत;

    स्थिरांक स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (आदान_Z.as_array (आदान_मोजणी ? आदान_मोजणी - 1 : 0));
    recurse_lookups (c, lookup_मोजणी_2, lookupRecord.arrayZ);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) स्थिरांक
  {
    स्थिरांक स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (आदान_Z.as_array (आदान_मोजणी ? आदान_मोजणी - 1 : 0));
    context_collect_glyphs_lookup (c,
				   आदान_मोजणी, आदान_Z.arrayZ,
				   lookup_मोजणी_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest,
			      स्थिरांक ClassDef *class_def = nullptr) स्थिरांक
  {
    जर (आदान_मोजणी <= 1)
    {
      *digest = hb_set_digest_t::full ();
      परत;
    }

    अचिन्हित second = आदान_Z.arrayZ[0];
    जर (!class_def)
      digest->add (second);
    अन्यथा जर (!second)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      class_def->collect_class (digest, second);
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
		    स्थिरांक ContextApplyLookupContext &lookup_context) स्थिरांक
  {
    स्थिरांक स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (आदान_Z.as_array (आदान_मोजणी ? आदान_मोजणी - 1 : 0));
    परत context_would_apply_lookup (c,
				       आदान_मोजणी, आदान_Z.arrayZ,
				       lookup_मोजणी_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  तार्किक apply (hb_ot_apply_context_t *c,
	      स्थिरांक ContextApplyLookupContext &lookup_context) स्थिरांक
  {
    TRACE_APPLY (हे);
    स्थिरांक स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (आदान_Z.as_array (आदान_मोजणी ? आदान_मोजणी - 1 : 0));
    return_trace (context_apply_lookup (c, आदान_मोजणी, आदान_Z.arrayZ, lookup_मोजणी_2, lookupRecord.arrayZ, lookup_context));
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक hb_map_t *आदान_mapping, /* old->new glyphid or class mapping */
		  स्थिरांक hb_map_t *lookup_map) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->start_embed (हे);
    जर (unlikely (!c->extend_min (out))) return_trace (असत्य);

    out->आदान_मोजणी = आदान_मोजणी;
    स्थिरांक स्वतः आदान = आदान_Z.as_array (आदान_मोजणी - 1);
    साठी (स्थिरांक स्वतः org : आदान)
    {
      typename Types::HBUINT d;
      d = आदान_mapping->get (org);
      c->copy (d);
    }

    स्थिरांक स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (आदान_Z.as_array ((आदान_मोजणी ? आदान_मोजणी - 1 : 0)));

    अचिन्हित मोजणी = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_मोजणी_2), lookup_map);
    return_trace (c->check_assign (out->lookup_मोजणी_2, मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक subset (hb_subset_context_t *c,
	       स्थिरांक hb_map_t *lookup_map,
	       स्थिरांक hb_map_t *klass_map = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (unlikely (!आदान_मोजणी)) return_trace (असत्य);
    स्थिरांक स्वतः आदान = आदान_Z.as_array (आदान_मोजणी - 1);

    स्थिरांक hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    जर (!hb_all (आदान, mapping)) return_trace (असत्य);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  c->check_range (आदान_Z.arrayZ,
				  आदान_Z.item_size * (आदान_मोजणी ? आदान_मोजणी - 1 : 0) +
				  LookupRecord::static_size * lookup_मोजणी_2));
  }

  संरक्षित:
  HBUINT16	आदान_मोजणी;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_मोजणी_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		आदान_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, आदान_Z);
};

ढाँचा <typename Types, typename RuleType>
संरचना RuleSetOf
{
  तार्किक intersects (स्थिरांक hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) स्थिरांक
  {
    परत
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_map ([&] (स्थिरांक RuleType &_) { परत _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  शून्य collect_first_आदान_classes (hb_ot_layout_ruleset_digest_t *digest) स्थिरांक
  {
    digest->init ();
    साठी (अचिन्हित i = 0; i < rule.len; i++)
    {
      स्थिरांक RuleType &r = हे+rule.arrayZ[i];
      जर (r.आदान_मोजणी <= 1)
      {
	digest->set_full ();
	परत;
      }
      digest->add (r.आदान_Z.arrayZ[0]);
    }
  }

  शून्य depend (hb_depend_context_t *c, अचिन्हित value, ContextDependLookupContext &lookup_context) स्थिरांक
  {
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  शून्य closure (hb_closure_context_t *c, अचिन्हित value,
		ContextClosureLookupContext &lookup_context) स्थिरांक
  {
    जर (unlikely (c->lookup_limit_exceeded ())) परत;

    परत
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) स्थिरांक
  {
    जर (unlikely (c->lookup_limit_exceeded ())) परत;
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) स्थिरांक
  {
    परत
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest,
			      स्थिरांक ClassDef *class_def = nullptr) स्थिरांक
  {
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([=] (स्थिरांक RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
		    स्थिरांक ContextApplyLookupContext &lookup_context) स्थिरांक
  {
    परत
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_map ([&] (स्थिरांक RuleType &_) { परत _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c,
	      स्थिरांक ContextApplyLookupContext &lookup_context,
	      स्थिरांक hb_ot_layout_ruleset_digest_t *digest = nullptr) स्थिरांक
  {
    TRACE_APPLY (हे);

    अचिन्हित अंक_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    जर (HB_OPTIMIZE_SIZE_VAL || अंक_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (हे))
      | hb_map ([&] (स्थिरांक RuleType &_) { परत _.apply (c, lookup_context); })
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
    स्वतः &skippy_iter = c->iter_context;
    skippy_iter.reset (c->buffer->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_data ((HBUINT16 *) nullptr);
    अचिन्हित unsafe_to = (अचिन्हित) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *first = nullptr, *second = nullptr;
    तार्किक matched = skippy_iter.next (&unsafe_to1);
    जर (likely (matched))
    {
      जर (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जा slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      जर (digest && lookup_context.get_मूल्य &&
	  !digest->may_have (lookup_context.get_मूल्य (*first, lookup_context.match_data)))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (असत्य);
	}
    }
    अन्यथा
    {
      तार्किक unsafe_to_concat = असत्य;
      तार्किक ret =
      + hb_iter (rule)
      | hb_map (hb_add (हे))
      | hb_filter ([&] (स्थिरांक RuleType &_)
		   {
		     जर (_.आदान_मोजणी <= 1) परत सत्य;
		     unsafe_to_concat = सत्य;
		     परत असत्य;
		   })
      | hb_map ([&] (स्थिरांक RuleType &_) { परत _.apply (c, lookup_context); })
      | hb_any
      ;
      जर (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    जर (likely (matched))
    {
      जर (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जा slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    स्वतः match_आदान = lookup_context.funcs.match;
    स्वतः *आदान_data = lookup_context.match_data;
    साठी (अचिन्हित पूर्णांक i = 0; i < अंक_rules; i++)
    {
      स्थिरांक स्वतः &r = हे+rule.arrayZ[i];

      स्थिरांक स्वतः &आदान = r.आदान_Z;

      जर (r.आदान_मोजणी <= 1 ||
	  (!match_आदान ||
	   match_आदान (*first, आदान.arrayZ[0], आदान_data)))
      {
        जर (!second ||
	    (r.आदान_मोजणी <= 2 ||
	     (!match_आदान ||
	      match_आदान (*second, आदान.arrayZ[1], आदान_data)))
	   )
	{
	  जर (r.apply (c, lookup_context))
	  {
	    जर (unsafe_to != (अचिन्हित) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (सत्य);
	  }
	}
	अन्यथा
	  unsafe_to = unsafe_to2;
      }
      अन्यथा
      {
	जर (unsafe_to == (अचिन्हित) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	साठी (; i + 1 < अंक_rules; i++)
	{
	  स्थिरांक स्वतः &r2 = हे+rule.arrayZ[i + 1];
	  स्थिरांक स्वतः &input2 = r2.आदान_Z;
	  जर (r2.आदान_मोजणी <= 1 || input2.arrayZ[0] != आदान.arrayZ[0])
	    थांबा;
	}
      }
    }
    जर (likely (unsafe_to != (अचिन्हित) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (असत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
	       स्थिरांक hb_map_t *lookup_map,
	       स्थिरांक hb_map_t *klass_map = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);

    स्वतः snap = c->serializer->snapshot ();
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    साठी (स्थिरांक स्वतः& _ : rule)
    {
      जर (!_) सुरूठेवा;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = out->rule.serialize_append (c->serializer);
      जर (unlikely (!o)) सुरूठेवा;

      जर (!o->serialize_subset (c, _, हे, lookup_map, klass_map))
      {
	out->rule.काढा ();
	c->serializer->revert (o_snap);
      }
    }

    तार्किक ret = तार्किक (out->rule);
    जर (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (rule.sanitize (c, हे));
  }

  संरक्षित:
  Array16Of<typename Types::ढाँचा OffsetTo<RuleType>>
		rule;			/* Array of Rule tables
					 * ordered by preference */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, rule);
};

ढाँचा <typename Types>
संरचना RuleSet : RuleSetOf<SmallTypes, Rule<Types>> {};

ढाँचा <typename Types>
संरचना ClassRuleSet : RuleSetOf<Types, Rule<SmallTypes>> {};


ढाँचा <typename Types>
संरचना ContextFormat1_4
{
  using RuleSet = OT::RuleSet<Types>;

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    परत
    + hb_zip (हे+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (हे))
    | hb_map ([&] (स्थिरांक RuleSet &_) { परत _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत सत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (हे+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      परत c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](स्थिरांक hb_pair_t<hb_codepoint_t, अचिन्हित> _) { परत hb_pair_t<अचिन्हित, स्थिरांक RuleSet&> (_.first, हे+ruleSet[_.second]); })
    | hb_apply ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (हे+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      परत c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](स्थिरांक hb_pair_t<hb_codepoint_t, अचिन्हित> _) { परत hb_pair_t<अचिन्हित, स्थिरांक RuleSet&> (_.first, हे+ruleSet[_.second]); })
    | hb_apply ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक
  {
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (हे+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    (हे+coverage).collect_coverage (c->आदान);

    संरचना ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_apply ([digest] (स्थिरांक RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  {
    स्थिरांक RuleSet &rule_set = हे+ruleSet[(हे+coverage).get_coverage (c->glyphs[0])];
    संरचना ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    परत rule_set.would_apply (c, lookup_context);
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (c->buffer->cur().codepoint);
    जर (likely (index == NOT_COVERED))
      return_trace (असत्य);

    स्थिरांक RuleSet &rule_set = हे+ruleSet[index];
    संरचना ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    स्थिरांक hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (हे+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, हे, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (coverage.sanitize (c, हे) && ruleSet.sanitize (c, हे));
  }

  संरक्षित:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


ढाँचा <typename Types>
संरचना ContextFormat2_5
{
  using RuleSet = OT::ClassRuleSet<Types>;

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    जर (!(हे+coverage).intersects (glyphs))
      परत असत्य;

    स्थिरांक ClassDef &class_def = हे+classDef;

    hb_map_t cache;
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (हे+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    परत
    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_enumerate
    | hb_map ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक RuleSet &> p)
	      { परत class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत सत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    जर (!(हे+coverage).intersects (c->glyphs))
      परत;

    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    स्थिरांक ClassDef &class_def = हे+classDef;

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
    | hb_filter ([&] (अचिन्हित _)
    { परत class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक typename Types::ढाँचा OffsetTo<RuleSet>&> _)
                {
                  स्थिरांक RuleSet& rule_set = हे+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    जर (!(हे+coverage).intersects (c->glyphs))
      परत;

    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    स्थिरांक ClassDef &class_def = हे+classDef;

    hb_map_t cache;
    intersected_class_cache_t intersected_cache;
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache,
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (अचिन्हित _)
    { परत class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक typename Types::ढाँचा OffsetTo<RuleSet>&> _)
                {
                  स्थिरांक RuleSet& rule_set = हे+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक
  {
    जर (!(हे+coverage).intersects (c->glyphs))
      परत;

    स्थिरांक ClassDef &class_def = हे+classDef;

    hb_map_t cache;
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_enumerate
    | hb_filter ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक RuleSet &> p)
    { परत class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (स्थिरांक RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    (हे+coverage).collect_coverage (c->आदान);

    स्थिरांक ClassDef &class_def = हे+classDef;
    संरचना ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  {
    स्थिरांक ClassDef &class_def = हे+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_apply ([=, &class_def] (स्थिरांक RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  {
    स्थिरांक ClassDef &class_def = हे+classDef;
    अचिन्हित पूर्णांक index = class_def.get_class (c->glyphs[0]);
    स्थिरांक RuleSet &rule_set = हे+ruleSet[index];
    संरचना ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    परत rule_set.would_apply (c, lookup_context);
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  अचिन्हित cache_cost () स्थिरांक
  {
    परत (हे+classDef).cost ();
  }
  स्थिर तार्किक cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    परत context_cache_func (c, op);
  }

  संरचना external_cache_t
  {
    hb_ot_layout_द्विमान_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  शून्य *external_cache_create () स्थिरांक
  {
    अचिन्हित मोजणी = ruleSet.len;
    अचिन्हित size = आकार (external_cache_t) -
		    HB_VAR_ARRAY * आकार (hb_ot_layout_ruleset_digest_t) +
		    मोजणी * आकार (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    जर (likely (cache))
    {
      cache->coverage.clear ();
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
	(हे+ruleSet[i]).collect_first_आदान_classes (&cache->rule_sets[i]);
    }
    परत cache;
  }
  तार्किक apply_cached (hb_ot_apply_context_t *c, शून्य *external_cache) स्थिरांक { परत _apply (c, सत्य, external_cache); }
  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) स्थिरांक { परत _apply (c, असत्य, external_cache); }
  तार्किक _apply (hb_ot_apply_context_t *c, तार्किक cached, शून्य *external_cache) स्थिरांक
  {
    TRACE_APPLY (हे);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage_द्विमान (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    जर (index == NOT_COVERED) return_trace (असत्य);

    स्थिरांक ClassDef &class_def = हे+classDef;

    संरचना ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_मूल्य : get_class_मूल्य
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    स्थिरांक RuleSet &rule_set = हे+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    स्थिरांक hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#अन्यथा
    स्थिरांक hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;
    जर (unlikely (!out->coverage.serialize_subset (c, coverage, हे)))
      return_trace (असत्य);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, हे, &klass_map);

    स्थिरांक hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (हे+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (हे+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    स्थिरांक hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    तार्किक ret = सत्य;
    पूर्णांक non_zero_index = -1, index = 0;
    स्वतः snapshot = c->serializer->snapshot();
    साठी (स्थिरांक स्वतः& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      स्वतः *o = out->ruleSet.serialize_append (c->serializer);
      जर (unlikely (!o))
      {
	ret = असत्य;
	थांबा;
      }

      जर (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, हे, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    जर (!ret || non_zero_index == -1) return_trace (असत्य);

    //prune empty trailing ruleSets
    --index;
    पर्यंत (index > non_zero_index)
    {
      out->ruleSet.काढा ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (तार्किक (out->ruleSet));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (coverage.sanitize (c, हे) && classDef.sanitize (c, हे) && ruleSet.sanitize (c, हे));
  }

  संरक्षित:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::ढाँचा LOffsetTo<ClassDef>
		classDef;		/* Offset to glyph ClassDef table--from
					 * beginning of table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<RuleSet>>
		ruleSet;		/* Array of RuleSet tables
					 * ordered by class */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};


ढाँचा <typename Types>
संरचना ContextFormat3_6
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    जर (!(हे+coverageZ[0]).intersects (glyphs))
      परत असत्य;

    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      हे
    };
    परत context_intersects (glyphs,
			       glyph_मोजणी_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत सत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    जर (!(हे+coverageZ[0]).intersects (c->glyphs))
      परत;

    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    स्थिरांक LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_मोजणी_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      हे,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_मोजणी_2, (स्थिरांक HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_मोजणी_2, lookupRecord,
			   हे,
			   coverageZ[0], lookup_context);

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    जर (!(हे+coverageZ[0]).intersects (c->glyphs))
      परत;

    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    स्थिरांक LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_मोजणी_2));
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      हे
    };
    context_closure_lookup (c,
			    glyph_मोजणी_2, coverageZ.arrayZ + 1,
			    lookup_मोजणी_2, lookupRecord,
			    0, lookup_context);

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक
  {
    जर (!intersects (c->glyphs))
      परत;
    स्थिरांक LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_मोजणी_2));
    recurse_lookups (c, lookup_मोजणी_2, lookupRecord);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    (हे+coverageZ[0]).collect_coverage (c->आदान);

    स्थिरांक LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_मोजणी_2));
    संरचना ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      हे
    };

    context_collect_glyphs_lookup (c,
				   glyph_मोजणी_2, coverageZ.arrayZ + 1,
				   lookup_मोजणी_2, lookupRecord,
				   lookup_context);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  {
    जर (glyph_मोजणी_2 <= 1)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      (हे+coverageZ[1]).collect_coverage (digest);
  }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  {
    स्थिरांक LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_मोजणी_2));
    संरचना ContextApplyLookupContext lookup_context = {
      {match_coverage},
      हे
    };
    परत context_would_apply_lookup (c,
				       glyph_मोजणी_2, coverageZ.arrayZ + 1,
				       lookup_मोजणी_2, lookupRecord,
				       lookup_context);
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverageZ[0]; }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    अचिन्हित पूर्णांक index = (हे+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    जर (index == NOT_COVERED) return_trace (असत्य);

    स्थिरांक LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_मोजणी_2));
    संरचना ContextApplyLookupContext lookup_context = {
      {match_coverage},
      हे
    };
    return_trace (context_apply_lookup (c,
					glyph_मोजणी_2,
					coverageZ.arrayZ + 1,
					lookup_मोजणी_2, lookupRecord,
					lookup_context));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    out->format = format;
    out->glyph_मोजणी_2 = glyph_मोजणी_2;

    स्वतः coverages = coverageZ.as_array (glyph_मोजणी_2);

    साठी (स्थिरांक स्वतः& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      स्वतः *o = c->serializer->allocate_size<typename Types::ढाँचा OffsetTo<Coverage>> (Types::Offset::static_size);
      जर (unlikely (!o)) return_trace (असत्य);
      जर (!o->serialize_subset (c, offset, हे)) return_trace (असत्य);
    }

    स्थिरांक स्वतः& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_मोजणी_2));
    स्थिरांक hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    अचिन्हित मोजणी = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_मोजणी_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_मोजणी_2, मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे))) return_trace (असत्य);
    hb_barrier ();
    अचिन्हित पूर्णांक मोजणी = glyph_मोजणी_2;
    जर (unlikely (!मोजणी)) return_trace (असत्य); /* We want to access coverageZ[0] freely. */
    जर (unlikely (!c->check_array (coverageZ.arrayZ, मोजणी))) return_trace (असत्य);
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (unlikely (!coverageZ[i].sanitize (c, हे))) return_trace (असत्य);
    स्थिरांक LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_मोजणी_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_मोजणी_2)));
  }

  संरक्षित:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_मोजणी_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_मोजणी_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::ढाँचा OffsetTo<Coverage>>
		coverageZ;		/* Array of offsets to Coverage
					 * table in glyph sequence order */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, coverageZ);
};

संरचना Context
{
  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    स्थिती 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    स्थिती 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    स्थिती 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    स्थिती 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
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

संरचना ChainContextClosureLookupContext
{
  ContextClosureFuncs funcs;
  ContextFormat context_format;
  स्थिरांक शून्य *intersects_data[3];
  शून्य *intersects_cache[3];
  शून्य *intersected_glyphs_cache;
};

प्रकारव्याख्या ChainContextClosureLookupContext ChainContextDependLookupContext;

संरचना ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  स्थिरांक शून्य *collect_data[3];
};

संरचना ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  स्थिरांक शून्य *match_data[3];
  get_मूल्य_func_t get_आदान_मूल्य;
};

स्थिर ओळीत तार्किक chain_context_intersects (स्थिरांक hb_set_t *glyphs,
					     अचिन्हित पूर्णांक backtrack_मोजणी,
					     glyph_data_t backtrack,
					     अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
					     glyph_data_t आदान, /* Array of input values--start with second glyph */
					     अचिन्हित पूर्णांक lookahead_मोजणी,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  तार्किक bt_ok = array_is_subset_of (glyphs,
			     backtrack_मोजणी, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  तार्किक in_ok = array_is_subset_of (glyphs,
			     आदान_मोजणी ? आदान_मोजणी - 1 : 0, आदान,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  तार्किक la_ok = array_is_subset_of (glyphs,
			     lookahead_मोजणी, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  परत bt_ok && in_ok && la_ok;
}

स्थिर ओळीत शून्य chain_context_closure_lookup (hb_closure_context_t *c,
						 अचिन्हित पूर्णांक backtrack_मोजणी,
						 glyph_data_t backtrack,
						 अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
						 glyph_data_t आदान, /* Array of input values--start with second glyph */
						 अचिन्हित पूर्णांक lookahead_मोजणी,
						 glyph_data_t lookahead,
						 अचिन्हित पूर्णांक lookup_मोजणी_2,
						 स्थिरांक LookupRecord lookupRecord[],
						 अचिन्हित value,
						 ChainContextClosureLookupContext &lookup_context)
{
  जर (chain_context_intersects (c->glyphs,
				backtrack_मोजणी, backtrack,
				आदान_मोजणी, आदान,
				lookahead_मोजणी, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     आदान_मोजणी, आदान,
		     lookup_मोजणी_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

स्थिर ओळीत शून्य chain_context_depend_lookup (hb_depend_context_t *c,
						अचिन्हित पूर्णांक backtrack_मोजणी,
						glyph_data_t backtrack,
						अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
						glyph_data_t आदान, /* Array of input values--start with second glyph */
						अचिन्हित पूर्णांक lookahead_मोजणी,
						glyph_data_t lookahead,
						अचिन्हित पूर्णांक lookup_मोजणी_2,
						स्थिरांक LookupRecord lookupRecord[],
						स्थिरांक शून्य *rule,
						अचिन्हित value,
						ChainContextDependLookupContext &lookup_context)
{
  जर (!chain_context_intersects (c->glyphs,
				 backtrack_मोजणी, backtrack,
				 आदान_मोजणी, आदान,
				 lookahead_मोजणी, lookahead,
				 lookup_context))
    परत;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  जर (unlikely (!scratch))
    परत;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  तार्किक context_sets_cached = context_depend_sets_are_cached (
    c, आदान_मोजणी, lookup_मोजणी_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  जर (unlikely (!c->depend_data->successful))
    परत;
  जर (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    आदान_मोजणी, आदान,
				    lookup_मोजणी_2, lookupRecord,
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
    परत;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<स्थिरांक hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<स्थिरांक hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  साठी (अचिन्हित i = 0; i < backtrack_मोजणी; i++)
  {
    स्थिरांक hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], असत्य, &position_scratch);
    जर (unlikely (!pos_glyphs))
      परत;
    जर (!pos_glyphs->is_empty ())
    {
      जर (unlikely (!backtrack_sets.ढकला_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        परत;
      }
    }
  }

  /* Extract lookahead glyphs */
  साठी (अचिन्हित i = 0; i < lookahead_मोजणी; i++)
  {
    स्थिरांक hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], असत्य, &position_scratch);
    जर (unlikely (!pos_glyphs))
      परत;
    जर (!pos_glyphs->is_empty ())
    {
      जर (unlikely (!lookahead_sets.ढकला_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        परत;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  साठी (स्थिरांक hb_set_t *back_set : backtrack_sets)
  {
    जर (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } अन्यथा जर (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      जर (unlikely (set_idx == HB_CODEPOINT_INVALID))
        परत;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  साठी (स्थिरांक hb_set_t *look_set : lookahead_sets)
  {
    जर (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } अन्यथा जर (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      जर (unlikely (set_idx == HB_CODEPOINT_INVALID))
        परत;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  जर (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    परत;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<स्थिरांक hb_set_t *> &आदान_position_glyphs = scratch->आदान_position_glyphs;

  जर (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    स्थिरांक hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      सत्य, &position_scratch, &scratch->compact_workspace);
    जर (unlikely (!pos0_glyphs ||
		  !आदान_position_glyphs.ढकला_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      परत;
    }

    /* Positions 1+ (Input array) */
    साठी (अचिन्हित i = 0; i + 1 < आदान_मोजणी; i++)
    {
      स्थिरांक hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	आदान[i], असत्य, &position_scratch);
      जर (unlikely (!pos_glyphs ||
		    !आदान_position_glyphs.ढकला_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	परत;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  आदान_मोजणी, आदान,
				  lookup_मोजणी_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &आदान_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

स्थिर ओळीत शून्य chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							अचिन्हित पूर्णांक backtrack_मोजणी,
							glyph_data_t backtrack,
							अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
							glyph_data_t आदान, /* Array of input values--start with second glyph */
							अचिन्हित पूर्णांक lookahead_मोजणी,
							glyph_data_t lookahead,
							अचिन्हित पूर्णांक lookup_मोजणी_2,
							स्थिरांक LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_मोजणी, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->आदान,
		 आदान_मोजणी ? आदान_मोजणी - 1 : 0, आदान,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_मोजणी, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_मोजणी_2, lookupRecord);
}

स्थिर ओळीत तार्किक chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     अचिन्हित पूर्णांक backtrack_मोजणी,
						     glyph_data_t backtrack HB_UNUSED,
						     अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
						     glyph_data_t आदान, /* Array of input values--start with second glyph */
						     अचिन्हित पूर्णांक lookahead_मोजणी,
						     glyph_data_t lookahead HB_UNUSED,
						     अचिन्हित पूर्णांक lookup_मोजणी_2 HB_UNUSED,
						     स्थिरांक LookupRecord lookupRecord[] HB_UNUSED,
						     स्थिरांक ChainContextApplyLookupContext &lookup_context)
{
  परत (c->zero_context ? !backtrack_मोजणी && !lookahead_मोजणी : सत्य)
      && would_match_आदान (c,
			    आदान_मोजणी, आदान,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

स्थिर ओळीत तार्किक chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       अचिन्हित पूर्णांक backtrack_मोजणी,
					       glyph_data_t backtrack,
					       अचिन्हित पूर्णांक आदान_मोजणी, /* Including the first glyph (not matched) */
					       glyph_data_t आदान, /* Array of input values--start with second glyph */
					       अचिन्हित पूर्णांक lookahead_मोजणी,
					       glyph_data_t lookahead,
					       अचिन्हित पूर्णांक lookup_मोजणी_2,
					       स्थिरांक LookupRecord lookupRecord[],
					       स्थिरांक ChainContextApplyLookupContext &lookup_context)
{
  जर (unlikely (आदान_मोजणी > HB_MAX_CONTEXT_LENGTH)) परत असत्य;

  अचिन्हित start_index = c->buffer->out_len;
  अचिन्हित end_index = c->buffer->idx;
  अचिन्हित match_end = 0;
  तार्किक ret = सत्य;
  जर (!(match_आदान (c,
		     आदान_मोजणी, आदान,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_मोजणी, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    परत असत्य;
  }

  जर (!match_backtrack (c,
			backtrack_मोजणी, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    परत असत्य;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		आदान_मोजणी,
		lookup_मोजणी_2, lookupRecord,
		match_end);

  परत ret;
}

ढाँचा <typename Types>
संरचना ChainRule
{
  ढाँचा <typename SetTypes, typename ChainRuleType>
  मित्र संरचना ChainRuleSetOf;

  तार्किक intersects (स्थिरांक hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    परत chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     आदान.lenP1, आदान.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  शून्य depend (hb_depend_context_t *c, अचिन्हित value, ChainContextDependLookupContext &lookup_context) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 आदान.lenP1, आदान.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 हे,
				 value,
				 lookup_context);
  }

  शून्य closure (hb_closure_context_t *c, अचिन्हित value,
		ChainContextClosureLookupContext &lookup_context) स्थिरांक
  {
    जर (unlikely (c->lookup_limit_exceeded ())) परत;

    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  आदान.lenP1, आदान.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) स्थिरांक
  {
    जर (unlikely (c->lookup_limit_exceeded ())) परत;
    जर (!intersects (c->glyphs, lookup_context)) परत;

    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 आदान.lenP1, आदान.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest,
			      स्थिरांक ClassDef *आदान_class_def = nullptr,
			      स्थिरांक ClassDef *lookahead_class_def = nullptr) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);

    अचिन्हित second;
    स्थिरांक ClassDef *class_def;
    जर (आदान.lenP1 > 1)
    {
      second = आदान.arrayZ[0];
      class_def = आदान_class_def;
    }
    अन्यथा जर (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    अन्यथा
    {
      *digest = hb_set_digest_t::full ();
      परत;
    }

    जर (!class_def)
      digest->add (second);
    अन्यथा जर (!second)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      class_def->collect_class (digest, second);
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
		    स्थिरांक ChainContextApplyLookupContext &lookup_context) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    परत chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     आदान.lenP1, आदान.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  तार्किक apply (hb_ot_apply_context_t *c,
	      स्थिरांक ChainContextApplyLookupContext &lookup_context) स्थिरांक
  {
    TRACE_APPLY (हे);
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      आदान.lenP1, आदान.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) स्थिरांक
  {
    c->copy (len);
    साठी (स्थिरांक स्वतः g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक hb_map_t *lookup_map,
		  स्थिरांक hb_map_t *backtrack_map,
		  स्थिरांक hb_map_t *आदान_map = nullptr,
		  स्थिरांक hb_map_t *lookahead_map = nullptr) स्थिरांक
  {
    TRACE_SERIALIZE (हे);

    स्थिरांक hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    जर (आदान_map) mapping = आदान_map;
    serialize_array (c, आदान.lenP1, + आदान.iter ()
				     | hb_map (mapping));

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    जर (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_मोजणी_2 = c->embed (&(lookup.len));
    जर (!lookup_मोजणी_2) return_trace (असत्य);

    अचिन्हित मोजणी = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_मोजणी_2, मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक subset (hb_subset_context_t *c,
	       स्थिरांक hb_map_t *lookup_map,
	       स्थिरांक hb_map_t *backtrack_map = nullptr,
	       स्थिरांक hb_map_t *आदान_map = nullptr,
	       स्थिरांक hb_map_t *lookahead_map = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);

    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);

    जर (!backtrack_map)
    {
      स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      जर (!hb_all (backtrack, glyphset) ||
	  !hb_all (आदान, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (असत्य);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    अन्यथा
    {
      जर (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (आदान, आदान_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (असत्य);

      serialize (c->serializer, lookup_map, backtrack_map, आदान_map, lookahead_map);
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    /* Hyper-optimized sanitized because this is really hot. */
    जर (unlikely (!backtrack.len.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    जर (unlikely (!आदान.lenP1.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    जर (unlikely (!lookahead.len.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  संरक्षित:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		आदान_X;			/* Array of input values (start with
					 * second glyph) */
  Array16Of<typename Types::HBUINT>
		lookaheadX;		/* Array of lookahead values's (to be
					 * matched after the input sequence) */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  सार्वजनिक:
  DEFINE_SIZE_MIN (8);
};

ढाँचा <typename Types, typename ChainRuleType>
संरचना ChainRuleSetOf
{
  तार्किक intersects (स्थिरांक hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) स्थिरांक
  {
    परत
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_map ([&] (स्थिरांक ChainRuleType &_) { परत _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  शून्य collect_first_आदान_classes (hb_ot_layout_ruleset_digest_t *digest) स्थिरांक
  {
    digest->init ();
    साठी (अचिन्हित i = 0; i < rule.len; i++)
    {
      स्थिरांक ChainRuleType &r = हे+rule.arrayZ[i];
      स्थिरांक स्वतः &आदान = StructAfter<decltype (r.आदान_X)> (r.backtrack);
      जर (आदान.lenP1 <= 1)
      {
	digest->set_full ();
	परत;
      }
      digest->add (आदान.arrayZ[0]);
    }
  }
  शून्य depend (hb_depend_context_t *c, अचिन्हित value, ChainContextDependLookupContext &lookup_context) स्थिरांक
  {
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  शून्य closure (hb_closure_context_t *c, अचिन्हित value, ChainContextClosureLookupContext &lookup_context) स्थिरांक
  {
    जर (unlikely (c->lookup_limit_exceeded ())) परत;

    परत
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) स्थिरांक
  {
    जर (unlikely (c->lookup_limit_exceeded ())) परत;

    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) स्थिरांक
  {
    परत
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest,
			      स्थिरांक ClassDef *आदान_class_def = nullptr,
			      स्थिरांक ClassDef *lookahead_class_def = nullptr) स्थिरांक
  {
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_apply ([=] (स्थिरांक ChainRuleType &_)
		{ _.collect_second_glyphs (digest, आदान_class_def, lookahead_class_def); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
		    स्थिरांक ChainContextApplyLookupContext &lookup_context) स्थिरांक
  {
    परत
    + hb_iter (rule)
    | hb_map (hb_add (हे))
    | hb_map ([&] (स्थिरांक ChainRuleType &_) { परत _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c,
	      स्थिरांक ChainContextApplyLookupContext &lookup_context,
	      स्थिरांक hb_ot_layout_ruleset_digest_t *digest = nullptr) स्थिरांक
  {
    TRACE_APPLY (हे);

    अचिन्हित अंक_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    जर (HB_OPTIMIZE_SIZE_VAL || अंक_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (हे))
      | hb_map ([&] (स्थिरांक ChainRuleType &_) { परत _.apply (c, lookup_context); })
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
    स्वतः &skippy_iter = c->iter_context;
    skippy_iter.reset (c->buffer->idx);
    skippy_iter.set_match_func (match_always, nullptr);
    skippy_iter.set_glyph_data ((HBUINT16 *) nullptr);
    अचिन्हित unsafe_to = (अचिन्हित) -1, unsafe_to1, unsafe_to2 = 0;
    hb_glyph_info_t *first = nullptr, *second = nullptr;
    तार्किक matched = skippy_iter.next (&unsafe_to1);
    जर (likely (matched))
    {
      जर (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जा slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      जर (digest && lookup_context.get_आदान_मूल्य &&
	  !digest->may_have (lookup_context.get_आदान_मूल्य (*first, lookup_context.match_data[1])))
	{
	  c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
	  return_trace (असत्य);
	}
    }
    अन्यथा
    {
      तार्किक unsafe_to_concat = असत्य;
      तार्किक ret =
      + hb_iter (rule)
      | hb_map (hb_add (हे))
      | hb_filter ([&] (स्थिरांक ChainRuleType &_)
		   {
		     स्थिरांक स्वतः &आदान = StructAfter<decltype (_.आदान_X)> (_.backtrack);
		     स्थिरांक स्वतः &lookahead = StructAfter<decltype (_.lookaheadX)> (आदान);
		     जर (आदान.lenP1 <= 1 && lookahead.len == 0) परत सत्य;
		     unsafe_to_concat = सत्य;
		     परत असत्य;
		   })
      | hb_map ([&] (स्थिरांक ChainRuleType &_) { परत _.apply (c, lookup_context); })
      | hb_any
      ;
      जर (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    जर (likely (matched))
    {
      जर (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        जा slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    स्वतः match_आदान = lookup_context.funcs.match[1];
    स्वतः match_lookahead = lookup_context.funcs.match[2];
    स्वतः *आदान_data = lookup_context.match_data[1];
    स्वतः *lookahead_data = lookup_context.match_data[2];
    साठी (अचिन्हित पूर्णांक i = 0; i < अंक_rules; i++)
    {
      स्थिरांक स्वतः &r = हे+rule.arrayZ[i];

      स्थिरांक स्वतः &आदान = StructAfter<decltype (r.आदान_X)> (r.backtrack);
      स्थिरांक स्वतः &lookahead = StructAfter<decltype (r.lookaheadX)> (आदान);

      अचिन्हित lenP1 = आदान.lenP1;
      जर (lenP1 > 1 ?
	   (!match_आदान ||
	    match_आदान (*first, आदान.arrayZ[0], आदान_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        जर (!second ||
	    (lenP1 > 2 ?
	     (!match_आदान ||
	      match_आदान (*second, आदान.arrayZ[1], आदान_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  जर (r.apply (c, lookup_context))
	  {
	    जर (unsafe_to != (अचिन्हित) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (सत्य);
	  }
	}
	अन्यथा
	  unsafe_to = unsafe_to2;
      }
      अन्यथा
      {
	जर (unsafe_to == (अचिन्हित) -1)
	  unsafe_to = unsafe_to1;

	जर (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  साठी (; i + 1 < अंक_rules; i++)
	  {
	    स्थिरांक स्वतः &r2 = हे+rule.arrayZ[i + 1];
	    स्थिरांक स्वतः &input2 = StructAfter<decltype (r2.आदान_X)> (r2.backtrack);
	    जर (input2.lenP1 <= 1 || input2.arrayZ[0] != आदान.arrayZ[0])
	      थांबा;
	  }
	}
      }
    }
    जर (likely (unsafe_to != (अचिन्हित) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (असत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
	       स्थिरांक hb_map_t *lookup_map,
	       स्थिरांक hb_map_t *backtrack_klass_map = nullptr,
	       स्थिरांक hb_map_t *आदान_klass_map = nullptr,
	       स्थिरांक hb_map_t *lookahead_klass_map = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);

    स्वतः snap = c->serializer->snapshot ();
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    साठी (स्थिरांक स्वतः& _ : rule)
    {
      जर (!_) सुरूठेवा;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = out->rule.serialize_append (c->serializer);
      जर (unlikely (!o)) सुरूठेवा;

      जर (!o->serialize_subset (c, _, हे,
				lookup_map,
				backtrack_klass_map,
				आदान_klass_map,
				lookahead_klass_map))
      {
	out->rule.काढा ();
	c->serializer->revert (o_snap);
      }
    }

    तार्किक ret = तार्किक (out->rule);
    जर (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (rule.sanitize (c, हे));
  }

  संरक्षित:
  Array16Of<typename Types::ढाँचा OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, rule);
};

ढाँचा <typename Types>
संरचना ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (आकार (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

ढाँचा <typename Types>
संरचना ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

ढाँचा <typename Types>
संरचना ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    परत
    + hb_zip (हे+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (हे))
    | hb_map ([&] (स्थिरांक ChainRuleSet &_) { परत _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत सत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (हे+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      परत c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](स्थिरांक hb_pair_t<hb_codepoint_t, अचिन्हित> _) { परत hb_pair_t<अचिन्हित, स्थिरांक ChainRuleSet&> (_.first, हे+ruleSet[_.second]); })
    | hb_apply ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (हे+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      परत c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](स्थिरांक hb_pair_t<hb_codepoint_t, अचिन्हित> _) { परत hb_pair_t<अचिन्हित, स्थिरांक ChainRuleSet&> (_.first, हे+ruleSet[_.second]); })
    | hb_apply ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक
  {
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (हे+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    (हे+coverage).collect_coverage (c->आदान);

    संरचना ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_apply ([digest] (स्थिरांक ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  {
    स्थिरांक ChainRuleSet &rule_set = हे+ruleSet[(हे+coverage).get_coverage (c->glyphs[0])];
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    परत rule_set.would_apply (c, lookup_context);
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (c->buffer->cur().codepoint);
    जर (index == NOT_COVERED) return_trace (असत्य);

    स्थिरांक ChainRuleSet &rule_set = हे+ruleSet[index];
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    स्थिरांक hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (हे+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, हे, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (coverage.sanitize (c, हे) && ruleSet.sanitize (c, हे));
  }

  संरक्षित:
  HBUINT16	format;			/* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by Coverage Index */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, ruleSet);
};

ढाँचा <typename Types>
संरचना ChainContextFormat2_5
{
  using ChainRuleSet = OT::ChainClassRuleSet<Types>;

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    जर (!(हे+coverage).intersects (glyphs))
      परत असत्य;

    स्थिरांक ClassDef &backtrack_class_def = हे+backtrackClassDef;
    स्थिरांक ClassDef &आदान_class_def = हे+आदान_Class_Def;
    स्थिरांक ClassDef &lookahead_class_def = हे+lookaheadClassDef;

    hb_map_t caches[3] = {};
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &आदान_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (हे+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    आदान_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    परत
    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_enumerate
    | hb_map ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक ChainRuleSet &> p)
	      { परत आदान_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत सत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    जर (!(हे+coverage).intersects (c->glyphs))
      परत;

    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    स्थिरांक ClassDef &backtrack_class_def = हे+backtrackClassDef;
    स्थिरांक ClassDef &आदान_class_def = हे+आदान_Class_Def;
    स्थिरांक ClassDef &lookahead_class_def = हे+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &आदान_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (अचिन्हित _)
    { परत आदान_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक typename Types::ढाँचा OffsetTo<ChainRuleSet>&> _)
                {
                  स्थिरांक ChainRuleSet& chainrule_set = हे+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    जर (!(हे+coverage).intersects (c->glyphs))
      परत;

    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs)) परत;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    स्थिरांक ClassDef &backtrack_class_def = हे+backtrackClassDef;
    स्थिरांक ClassDef &आदान_class_def = हे+आदान_Class_Def;
    स्थिरांक ClassDef &lookahead_class_def = हे+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &आदान_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (अचिन्हित _)
    { परत आदान_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक typename Types::ढाँचा OffsetTo<ChainRuleSet>&> _)
                {
                  स्थिरांक ChainRuleSet& chainrule_set = हे+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक
  {
    जर (!(हे+coverage).intersects (c->glyphs))
      परत;

    स्थिरांक ClassDef &backtrack_class_def = हे+backtrackClassDef;
    स्थिरांक ClassDef &आदान_class_def = हे+आदान_Class_Def;
    स्थिरांक ClassDef &lookahead_class_def = हे+lookaheadClassDef;

    hb_map_t caches[3] = {};
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &आदान_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_enumerate
    | hb_filter([&] (अचिन्हित klass)
    { परत आदान_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (स्थिरांक ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    (हे+coverage).collect_coverage (c->आदान);

    स्थिरांक ClassDef &backtrack_class_def = हे+backtrackClassDef;
    स्थिरांक ClassDef &आदान_class_def = हे+आदान_Class_Def;
    स्थिरांक ClassDef &lookahead_class_def = हे+lookaheadClassDef;

    संरचना ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &आदान_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_apply ([&] (स्थिरांक ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  {
    स्थिरांक ClassDef &आदान_class_def = हे+आदान_Class_Def;
    स्थिरांक ClassDef &lookahead_class_def = हे+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (हे))
    | hb_apply ([digest, &आदान_class_def, &lookahead_class_def] (स्थिरांक ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &आदान_class_def, &lookahead_class_def); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  {
    स्थिरांक ClassDef &backtrack_class_def = हे+backtrackClassDef;
    स्थिरांक ClassDef &आदान_class_def = हे+आदान_Class_Def;
    स्थिरांक ClassDef &lookahead_class_def = हे+lookaheadClassDef;

    अचिन्हित पूर्णांक index = आदान_class_def.get_class (c->glyphs[0]);
    स्थिरांक ChainRuleSet &rule_set = हे+ruleSet[index];
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &आदान_class_def,
       &lookahead_class_def}
    };
    परत rule_set.would_apply (c, lookup_context);
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  अचिन्हित cache_cost () स्थिरांक
  {
    परत (हे+आदान_Class_Def).cost () + (हे+lookaheadClassDef).cost ();
  }
  स्थिर तार्किक cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    परत context_cache_func (c, op);
  }

  संरचना external_cache_t
  {
    hb_ot_layout_द्विमान_cache_t coverage;
    hb_ot_layout_mapping_cache_t आदान_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  शून्य *external_cache_create () स्थिरांक
  {
    अचिन्हित मोजणी = ruleSet.len;
    अचिन्हित size = आकार (external_cache_t) -
		    HB_VAR_ARRAY * आकार (hb_ot_layout_ruleset_digest_t) +
		    मोजणी * आकार (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    जर (likely (cache))
    {
      cache->coverage.clear ();
      cache->आदान_class.clear ();
      cache->lookahead_class.clear ();
      साठी (अचिन्हित i = 0; i < मोजणी; i++)
	(हे+ruleSet[i]).collect_first_आदान_classes (&cache->rule_sets[i]);
    }
    परत cache;
  }
  तार्किक apply_cached (hb_ot_apply_context_t *c, शून्य *external_cache) स्थिरांक { परत _apply (c, सत्य, external_cache); }
  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) स्थिरांक { परत _apply (c, असत्य, external_cache); }
  तार्किक _apply (hb_ot_apply_context_t *c, तार्किक cached, शून्य *external_cache) स्थिरांक
  {
    TRACE_APPLY (हे);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage_द्विमान (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    जर (index == NOT_COVERED) return_trace (असत्य);

    स्थिरांक ClassDef &backtrack_class_def = हे+backtrackClassDef;
    स्थिरांक ClassDef &आदान_class_def = हे+आदान_Class_Def;
    स्थिरांक ClassDef &lookahead_class_def = हे+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t आदान_cache_data = {
      &आदान_class_def,
      cache ? &cache->आदान_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    स्थिरांक शून्य *आदान_match_data = cached
      ? static_cast<स्थिरांक शून्य *> (&आदान_class_def)
      : static_cast<स्थिरांक शून्य *> (&आदान_cache_data);
    स्थिरांक शून्य *lookahead_match_data = cached
      ? static_cast<स्थिरांक शून्य *> (&lookahead_class_def)
      : static_cast<स्थिरांक शून्य *> (&lookahead_cache_data);
    तार्किक backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    स्थिरांक शून्य *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<स्थिरांक शून्य *> (&lookahead_cache_data)
      : static_cast<स्थिरांक शून्य *> (&backtrack_class_def);
#endif

    /* match_class_caches1 is slightly faster. Use it for lookahead,
     * which is typically longer. */
    संरचना ChainContextApplyLookupContext lookup_context = {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 :
        backtrack_uses_lookahead_cache ? match_class_cache : match_class,
        cached ? match_class_cached2 : match_class_cache,
        cached ? match_class_cached1 : match_class_cache}},
      {backtrack_match_data,
       आदान_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_मूल्य : get_class_cache_मूल्य
#अन्यथा
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &आदान_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_मूल्य : get_class_मूल्य
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (आदान_class_def, c->buffer->cur())
         : आदान_class_def.get_class (c->buffer->cur().codepoint, आदान_cache_data.cache);
#अन्यथा
    index = cached
         ? get_class_cached2 (आदान_class_def, c->buffer->cur())
          : आदान_class_def.get_class (c->buffer->cur().codepoint);
#endif
    स्थिरांक ChainRuleSet &rule_set = हे+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    स्थिरांक hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#अन्यथा
    स्थिरांक hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, हे);

    hb_map_t backtrack_klass_map;
    hb_map_t आदान_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, हे, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->आदान_Class_Def.serialize_subset (c, आदान_Class_Def, हे, &आदान_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, हे, &lookahead_klass_map);

    जर (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   आदान_klass_map,
						   lookahead_klass_map)))
      return_trace (असत्य);

    स्थिरांक hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (हे+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (हे+आदान_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    पूर्णांक non_zero_index = -1, index = 0;
    तार्किक ret = सत्य;
    स्थिरांक hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    स्वतः last_non_zero = c->serializer->snapshot ();
    साठी (स्थिरांक स्वतः& _ : + hb_enumerate (ruleSet)
			 | hb_filter (आदान_klass_map, hb_first))
    {
      स्वतः *o = out->ruleSet.serialize_append (c->serializer);
      जर (unlikely (!o))
      {
	ret = असत्य;
	थांबा;
      }
      जर (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, हे,
			       lookup_map,
			       &backtrack_klass_map,
			       &आदान_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    जर (!ret || non_zero_index == -1) return_trace (असत्य);

    // prune empty trailing ruleSets
    जर (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (तार्किक (out->ruleSet));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (coverage.sanitize (c, हे) &&
		  backtrackClassDef.sanitize (c, हे) &&
		  आदान_Class_Def.sanitize (c, हे) &&
		  lookaheadClassDef.sanitize (c, हे) &&
		  ruleSet.sanitize (c, हे));
  }

  संरक्षित:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::ढाँचा LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::ढाँचा LOffsetTo<ClassDef>
		आदान_Class_Def;		/* Offset to glyph ClassDef
					 * table containing input sequence
					 * data--from beginning of table */
  typename Types::ढाँचा LOffsetTo<ClassDef>
		lookaheadClassDef;	/* Offset to glyph ClassDef table
					 * containing lookahead sequence
					 * data--from beginning of table */
  Array16Of<typename Types::ढाँचा OffsetTo<ChainRuleSet>>
		ruleSet;		/* Array of ChainRuleSet tables
					 * ordered by class */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4 + 4 * Types::LOffset::static_size, ruleSet);
};

ढाँचा <typename Types>
संरचना ChainContextFormat3
{
  using RuleSet = OT::RuleSet<SmallTypes>;

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);

    जर (!(हे+आदान[0]).intersects (glyphs))
      परत असत्य;

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {हे, हे, हे}
    };
    परत chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     आदान.len, आदान.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत सत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);

    जर (!(हे+आदान[0]).intersects (c->glyphs))
      परत;

    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs))
      परत;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {हे, हे, हे},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (स्थिरांक HBUINT16 *) backtrack.arrayZ,
				 आदान.len, (स्थिरांक HBUINT16 *) आदान.arrayZ + 1,
				 lookahead.len, (स्थिरांक HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 हे,
				 आदान[0], lookup_context);

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);

    जर (!(हे+आदान[0]).intersects (c->glyphs))
      परत;

    hb_set_t* cur_active_glyphs = c->ढकला_cur_active_glyphs ();
    जर (unlikely (!cur_active_glyphs))
      परत;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {हे, हे, हे}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  आदान.len, आदान.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->काढा_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक
  {
    जर (!intersects (c->glyphs))
      परत;

    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);

    (हे+आदान[0]).collect_coverage (c->आदान);

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);

    संरचना ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {हे, हे, हे}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 आदान.len, आदान.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    जर (आदान.len > 1)
      (हे+आदान[1]).collect_coverage (digest);
    अन्यथा जर (lookahead.len)
      (हे+lookahead[0]).collect_coverage (digest);
    अन्यथा
      *digest = hb_set_digest_t::full ();
  }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {हे, हे, हे}
    };
    परत chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     आदान.len, आदान.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक
  {
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    परत हे+आदान[0];
  }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);

    अचिन्हित पूर्णांक index = (हे+आदान[0]).get_coverage (c->buffer->cur().codepoint);
    जर (index == NOT_COVERED) return_trace (असत्य);

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {हे, हे, हे}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      आदान.len, आदान.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, स्थिरांक शून्य* base) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->serializer->start_embed<Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>> ();
    जर (unlikely (!out->serialize (c->serializer, 0u))) return_trace (असत्य);

    साठी (स्वतः& offset : it) {
      स्वतः *o = out->serialize_append (c->serializer);
      जर (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);

    जर (unlikely (!c->serializer->embed (हे->format))) return_trace (असत्य);

    जर (!serialize_coverage_offsets (c, backtrack.iter (), हे))
      return_trace (असत्य);

    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    जर (!serialize_coverage_offsets (c, आदान.iter (), हे))
      return_trace (असत्य);

    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    जर (!serialize_coverage_offsets (c, lookahead.iter (), हे))
      return_trace (असत्य);

    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    स्थिरांक hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_मोजणी_2 = c->serializer->copy<HBUINT16> (lookup.len);
    जर (!lookup_मोजणी_2) return_trace (असत्य);

    अचिन्हित मोजणी = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_मोजणी_2, मोजणी, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!backtrack.sanitize (c, हे))) return_trace (असत्य);
    hb_barrier ();
    स्थिरांक स्वतः &आदान = StructAfter<decltype (आदान_X)> (backtrack);
    जर (unlikely (!आदान.sanitize (c, हे))) return_trace (असत्य);
    hb_barrier ();
    जर (unlikely (!आदान.len)) return_trace (असत्य); /* To be consistent with Context. */
    स्थिरांक स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (आदान);
    जर (unlikely (!lookahead.sanitize (c, हे))) return_trace (असत्य);
    hb_barrier ();
    स्थिरांक स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  संरक्षित:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>
		आदान_X		;	/* Array of coverage
					 * tables in input sequence, in glyph
					 * sequence order */
  Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>
		lookaheadX;		/* Array of coverage tables
					 * in lookahead sequence, in glyph
					 * sequence order */
  Array16Of<LookupRecord>
		lookupX;		/* Array of LookupRecords--in
					 * design order) */
  सार्वजनिक:
  DEFINE_SIZE_MIN (10);
};

संरचना ChainContext
{
  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    स्थिती 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    स्थिती 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    स्थिती 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; }		format;	/* Format identifier */
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


ढाँचा <typename T>
संरचना ExtensionFormat1
{
  अचिन्हित पूर्णांक get_type () स्थिरांक { परत extensionLookupType; }

  ढाँचा <typename X>
  स्थिरांक X& get_subtable () स्थिरांक
  { परत हे + reinterpret_cast<स्थिरांक Offset32To<typename T::SubTable> &> (extensionOffset); }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, हे))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);

    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    स्थिरांक स्वतः& src_offset =
        reinterpret_cast<स्थिरांक Offset32To<typename T::SubTable> &> (extensionOffset);
    स्वतः& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, हे, get_type ()));
  }

  संरक्षित:
  HBUINT16	format;			/* Format identifier. Set to 1. */
  HBUINT16	extensionLookupType;	/* Lookup type of subtable referenced
					 * by ExtensionOffset (i.e. the
					 * extension subtable). */
  Offset32	extensionOffset;	/* Offset to the extension subtable,
					 * of lookup type subtable. */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

ढाँचा <typename T>
संरचना Extension
{
  अचिन्हित पूर्णांक get_type () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.get_type ();
    पूर्वनिर्धारित:परत 0;
    }
  }
  ढाँचा <typename X>
  स्थिरांक X& get_subtable () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.ढाँचा get_subtable<typename T::SubTable> ();
    पूर्वनिर्धारित:परत Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  ढाँचा <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.subset (c);
    पूर्वनिर्धारित: परत c->default_return_मूल्य ();
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  ExtensionFormat1<T>	format1;
  } u;
};


/*
 * GSUB/GPOS Common
 */

संरचना hb_ot_layout_lookup_accelerator_t
{
  ढाँचा <typename TLookup>
  स्थिर hb_ot_layout_lookup_accelerator_t *create (स्थिरांक TLookup &lookup)
  {
    अचिन्हित मोजणी = lookup.get_subtable_मोजणी ();

    अचिन्हित size = आकार (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * आकार (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    मोजणी * आकार (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    स्वतः *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    जर (unlikely (!thiz))
      परत nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    पर्यंत (मोजणी && !thiz->subtables[मोजणी - 1].apply_func)
      मोजणी--;

    thiz->digest.init ();
    साठी (स्वतः& subtable : hb_iter (thiz->subtables, मोजणी))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->मोजणी = मोजणी;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      जर (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    परत thiz;
  }

  शून्य fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  तार्किक may_have (hb_codepoint_t g) स्थिरांक
  { परत digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक apply (hb_ot_apply_context_t *c, तार्किक use_cache) स्थिरांक
  {
    जर (मोजणी == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      जर (use_cache)
	परत subtables[0].apply_cached_no_digest (c);
#endif
      परत subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    जर (use_cache)
    {
      परत
      + hb_iter (hb_iter (subtables, मोजणी))
      | hb_map ([&c] (स्थिरांक hb_accelerate_subtables_context_t::hb_applicable_t &_) { परत _.apply_cached (c); })
      | hb_any
      ;
    }
    अन्यथा
#endif
    {
      परत
      + hb_iter (hb_iter (subtables, मोजणी))
      | hb_map ([&c] (स्थिरांक hb_accelerate_subtables_context_t::hb_applicable_t &_) { परत _.apply (c); })
      | hb_any
      ;
    }
    परत असत्य;
  }

  तार्किक cache_enter (hb_ot_apply_context_t *c) स्थिरांक
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    परत subtable_cache_user_idx != (अचिन्हित) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#अन्यथा
    परत असत्य;
#endif
  }
  शून्य cache_leave (hb_ot_apply_context_t *c) स्थिरांक
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  खाजगी:
  अचिन्हित मोजणी = 0; /* Number of subtables in the array. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  अचिन्हित subtable_cache_user_idx = (अचिन्हित) -1;
#endif
  hb_accelerate_subtables_context_t::hb_applicable_t subtables[HB_VAR_ARRAY];
};

संरचना GSUBGPOS
{
  using LookupListT = LookupList<SmallTypes>;
  using LookupList2T = LookupList<MediumTypes>;
  ढाँचा <typename TLookup>
  using TLookupList = List16OfOffsetTo<TLookup, typename SmallTypes::HBLUINT>;
  ढाँचा <typename TLookup>
  using TLookupList2 = List16OfOffsetTo<TLookup, typename MediumTypes::HBLUINT>;
  ढाँचा <typename TLookup>
  using TLookupOffsetList = LookupOffsetList<TLookup, typename SmallTypes::HBLUINT>;
  ढाँचा <typename TLookup>
  using TLookupOffsetList2 = LookupOffsetList<TLookup, typename MediumTypes::HBLUINT>;

  माप_प्रकार get_size () स्थिरांक
  {
    जर (version.major != 1) परत version.static_size;

    परत min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#अन्यथा
	   0
#endif
	   ;
  }

  स्थिरांक ScriptList &get_script_list () स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (version.to_int () >= 0x00010002u && scriptList2) परत हे + scriptList2;
#endif
    परत हे + scriptList;
  }
  स्थिरांक FeatureList &get_feature_list () स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (version.to_int () >= 0x00010002u && featureList2) परत हे + featureList2;
#endif
    परत हे + featureList;
  }
  ढाँचा <typename TLookup>
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (हे, get_size ())))
      return_trace (असत्य);
    hb_barrier ();

    तार्किक script_list_ok = scriptList.sanitize (c, हे);
    तार्किक feature_list_ok = featureList.sanitize (c, हे);
    तार्किक lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, हे);

#ifndef HB_NO_BEYOND_64K
    जर (version.to_int () >= 0x00010002u)
    {
      जर (scriptList2) script_list_ok = scriptList2.sanitize (c, हे);
      जर (featureList2) feature_list_ok = featureList2.sanitize (c, हे);
      जर (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, हे);
    }
#endif

    जर (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (असत्य);

#ifndef HB_NO_VAR
    जर (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, हे, &get_feature_list ()))))
      return_trace (असत्य);
#endif

    return_trace (सत्य);
  }

  ढाँचा <typename TLookup>
  तार्किक subset (hb_subset_layout_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (version.major != 1) return_trace (असत्य);

    स्वतः *out = c->subset_context->serializer->start_embed (हे);
    जर (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (असत्य);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    जर (version.to_int () >= 0x00010002u)
    {
      जर (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (असत्य);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      जर (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 हे, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 हे, c);

      जर (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 हे, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 हे, c);

      जर (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, हे, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, हे, c);
    }
    अन्यथा
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     हे,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     हे,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					हे,
					c);
    }

#ifndef HB_NO_VAR
    जर (version.to_int () >= 0x00010001u)
    {
      स्वतः snapshot = c->subset_context->serializer->snapshot ();
      जर (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (असत्य);

      // if all axes are pinned all feature vars are dropped.
      तार्किक ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, हे, c);
      जर (!ret)
      {
        out->featureVars = 0;
	जर (version.to_int () < 0x00010002u)
	{
	  c->subset_context->serializer->revert (snapshot);
	  out->version.major = 1;
	  out->version.minor = 0;
	}
      }
    }
#endif

    return_trace (सत्य);
  }

  अचिन्हित पूर्णांक get_lookup_मोजणी () स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (version.to_int () >= 0x00010002u && lookupList2)
      परत (हे + lookupList2).len;
#endif
    परत (हे + lookupList).len;
  }
  स्थिरांक Lookup& get_lookup (अचिन्हित पूर्णांक i) स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (version.to_int () >= 0x00010002u && lookupList2)
      परत (हे + lookupList2)[i];
#endif
    परत (हे + lookupList)[i];
  }
  स्थिरांक FeatureVariations &get_feature_variations () स्थिरांक
  {
    परत (version.to_int () >= 0x00010001u && hb_barrier ()) ? हे+featureVars : Null (FeatureVariations);
  }

  तार्किक has_data () स्थिरांक { परत version.to_int (); }
  अचिन्हित पूर्णांक get_script_मोजणी () स्थिरांक
  { परत get_script_list ().len; }
  स्थिरांक Tag& get_script_tag (अचिन्हित पूर्णांक i) स्थिरांक
  { परत get_script_list ().get_tag (i); }
  अचिन्हित पूर्णांक get_script_tags (अचिन्हित पूर्णांक start_offset,
				अचिन्हित पूर्णांक *script_मोजणी /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) स्थिरांक
  { परत get_script_list ().get_tags (start_offset, script_मोजणी, script_tags); }
  स्थिरांक Script& get_script (अचिन्हित पूर्णांक i) स्थिरांक
  { परत get_script_list ()[i]; }
  तार्किक find_script_index (hb_tag_t tag, अचिन्हित पूर्णांक *index) स्थिरांक
  { परत get_script_list ().find_index (tag, index); }

  अचिन्हित पूर्णांक get_feature_मोजणी () स्थिरांक
  { परत get_feature_list ().len; }
  hb_tag_t get_feature_tag (अचिन्हित पूर्णांक i) स्थिरांक
  { परत i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  अचिन्हित पूर्णांक get_feature_tags (अचिन्हित पूर्णांक start_offset,
				 अचिन्हित पूर्णांक *feature_मोजणी_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) स्थिरांक
  { परत get_feature_list ().get_tags (start_offset, feature_मोजणी_2, feature_tags); }
  स्थिरांक Feature& get_feature (अचिन्हित पूर्णांक i) स्थिरांक
  { परत get_feature_list ()[i]; }
  तार्किक find_feature_index (hb_tag_t tag, अचिन्हित पूर्णांक *index) स्थिरांक
  { परत get_feature_list ().find_index (tag, index); }

  तार्किक find_variations_index (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक अंक_coords,
			      अचिन्हित पूर्णांक *index,
			      ItemVarStoreInstancer *instancer) स्थिरांक
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    परत असत्य;
#endif
    परत get_feature_variations ().find_index (coords, अंक_coords, index, instancer);
  }
  तार्किक get_feature_variations_state (स्थिरांक पूर्णांक *coords,
				     अचिन्हित पूर्णांक अंक_coords,
				     अचिन्हित पूर्णांक *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<अचिन्हित> *lookup_conditions) स्थिरांक
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    परत सत्य;
#अन्यथा
    find_variations_index (coords, अंक_coords, index, instancer);
    परत get_feature_variations ().collect_lookup_conditions (
	coords,
	अंक_coords,
	instancer,
	lookup_conditions);
#endif
  }
  तार्किक get_lookup_variations (अचिन्हित feature_index,
                              अचिन्हित variations_index,
                              hb_sorted_array_t<स्थिरांक अचिन्हित> conditions,
                              hb_set_t *lookup_indexes) स्थिरांक
  {
#ifdef HB_NO_VAR
    परत असत्य;
#अन्यथा
    परत get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  स्थिरांक Feature& get_feature_variation (अचिन्हित पूर्णांक feature_index,
					अचिन्हित पूर्णांक variations_index) स्थिरांक
  {
#ifndef HB_NO_VAR
    जर (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      स्थिरांक Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      जर (feature)
	परत *feature;
    }
#endif
    परत get_feature (feature_index);
  }

  शून्य feature_variation_collect_lookups (स्थिरांक hb_set_t *feature_indexes,
					  स्थिरांक hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) स्थिरांक
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  शून्य collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) स्थिरांक
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  ढाँचा <typename TLookup>
  शून्य closure_lookups (hb_face_t      *face,
			स्थिरांक hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) स्थिरांक
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::ढाँचा dispatch_recurse_func<hb_closure_lookups_context_t>);

    साठी (अचिन्हित lookup_index : *lookup_indexes)
      reinterpret_cast<स्थिरांक TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  शून्य prune_langsys (स्थिरांक hb_map_t *duplicate_feature_map,
                      स्थिरांक hb_set_t *layout_scripts,
                      hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) स्थिरांक
  {
    hb_prune_langsys_context_t c (हे, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    अचिन्हित मोजणी = get_script_मोजणी ();
    साठी (अचिन्हित script_index = 0; script_index < मोजणी; script_index++)
    {
      स्थिरांक Tag& tag = get_script_tag (script_index);
      जर (!layout_scripts->has (tag)) सुरूठेवा;
      स्थिरांक Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  शून्य prune_features (स्थिरांक hb_map_t *lookup_indices, /* IN */
		       स्थिरांक hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       स्थिरांक hb_hashmap_t<अचिन्हित, स्थिरांक Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) स्थिरांक
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    जर (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      परत;
    }
#endif

    साठी (अचिन्हित i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      जर (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        सुरूठेवा;


      स्थिरांक Feature *f = &(get_feature (i));
      स्थिरांक Feature** p = nullptr;
      जर (feature_substitutes_map->has (i, &p))
        f = *p;

      जर (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        सुरूठेवा;

      जर (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  शून्य collect_name_ids (स्थिरांक hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         स्थिरांक hb_hashmap_t<अचिन्हित, स्थिरांक Feature*> *feature_substitutes_map,
                         स्थिरांक hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         स्थिरांक hb_set_t *catch_all_features,
                         तार्किक all_axes_pinned HB_UNUSED) स्थिरांक
  {
    अचिन्हित मोजणी = get_feature_मोजणी ();
    साठी (अचिन्हित i = 0 ; i < मोजणी; i++)
    {
      जर (!feature_index_map->has (i)) सुरूठेवा;
      hb_tag_t tag = get_feature_tag (i);
      स्थिरांक Feature *feature = &get_feature (i);
      स्थिरांक Feature **substitute;
      जर (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        जर (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    जर (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [हे] (अचिन्हित i) { परत get_feature_tag (i); },
                                                  nameids_to_retain);
#endif
  }

  ढाँचा <typename T>
  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      hb_sanitize_context_t sc;
      sc.lazy_some_gpos = सत्य;
      हे->table = sc.reference_table<T> (face);

      जर (unlikely (हे->table->is_blocklisted (हे->table.get_blob (), face)))
      {
	hb_blob_destroy (हे->table.get_blob ());
	हे->table = hb_blob_get_empty ();
      }

      हे->lookup_मोजणी = table->get_lookup_मोजणी ();

      हे->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (हे->lookup_मोजणी, आकार (*accels));
      जर (unlikely (!हे->accels))
      {
	हे->lookup_मोजणी = 0;
	हे->table.destroy ();
	हे->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      साठी (अचिन्हित पूर्णांक i = 0; i < हे->lookup_मोजणी; i++)
      {
	स्वतः *accel = हे->accels[i].get_relaxed ();
	जर (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (हे->accels);
      हे->table.destroy ();
    }

    hb_blob_t *get_blob () स्थिरांक { परत table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (अचिन्हित lookup_index) स्थिरांक
    {
      जर (unlikely (lookup_index >= lookup_मोजणी)) परत nullptr;

    retry:
      स्वतः *accel = accels[lookup_index].get_acquire ();
      जर (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	जर (unlikely (!accel))
	  परत nullptr;

	जर (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  जा retry;
	}
      }

      परत accel;
    }

    hb_blob_ptr_t<T> table;
    अचिन्हित पूर्णांक lookup_मोजणी;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  संरक्षित:
  ढाँचा <typename TLookup>
  स्थिरांक Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () स्थिरांक
  { परत reinterpret_cast<स्थिरांक Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  ढाँचा <typename TLookup>
  स्थिरांक Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () स्थिरांक
  { परत reinterpret_cast<स्थिरांक Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  ढाँचा <typename TLookup>
  स्थिरांक Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () स्थिरांक
  { परत reinterpret_cast<स्थिरांक Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  ढाँचा <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { परत reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  ढाँचा <typename TLookup>
  स्थिरांक Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () स्थिरांक
  { परत reinterpret_cast<स्थिरांक Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  स्थिरांक Offset16To<RecordListOfFeature>& subset_feature_list_offset () स्थिरांक
  { परत reinterpret_cast<स्थिरांक Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { परत reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  स्थिरांक Offset32To<RecordListOfFeature>& subset_feature_list2_offset () स्थिरांक
  { परत reinterpret_cast<स्थिरांक Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  स्थिरांक शून्य* get_lookup_list_field_offset () स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (version.to_int () >= 0x00010002u && lookupList2) परत &lookupList2;
#endif
    परत &lookupList;
  }

  ढाँचा <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  स्थिर तार्किक serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       स्थिरांक OffsetIn& in,
				       स्थिरांक Base *base,
				       Ts&&... ds)
  {
    out = 0;
    जर (in.is_null ()) परत असत्य;

    स्वतः *s = c->serializer;
    s->push ();
    तार्किक ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    जर (ret) s->add_link (out, s->काढा_pack ());
    अन्यथा s->काढा_discard ();
    परत ret;
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
  सार्वजनिक:
  DEFINE_SIZE_MIN (10);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_GSUBGPOS_HH */
