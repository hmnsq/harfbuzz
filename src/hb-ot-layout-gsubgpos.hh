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
  return_t dispatch (ध्रुव T &obj) { प्रत्यागम obj.intersects (एतत्->glyphs); }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) ध्रुव { प्रत्यागम r; }

  ध्रुव hb_set_t *glyphs;

  hb_intersects_context_t (ध्रुव hb_set_t *glyphs_) :
                            glyphs (glyphs_) {}
};

संरचना hb_have_non_1to1_context_t :
       hb_dispatch_context_t<hb_have_non_1to1_context_t, तार्किक>
{
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj) { प्रत्यागम obj.may_have_non_1to1 (); }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) ध्रुव { प्रत्यागम r; }
};

संरचना hb_closure_context_t :
       hb_dispatch_context_t<hb_closure_context_t>
{
  प्रकारनिर्देश return_t (*recurse_func_t) (hb_closure_context_t *c, अचिन्हित lookup_index, hb_set_t *covered_seq_indicies, अचिन्हित seq_index, अचिन्हित end_index);
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj) { obj.closure (एतत्); प्रत्यागम hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम hb_empty_t (); }
  शून्य recurse (अचिन्हित lookup_index, hb_set_t *covered_seq_indicies, अचिन्हित seq_index, अचिन्हित end_index)
  {
    यदि (unlikely (nesting_level_बायाँ == 0 || !recurse_func))
      प्रत्यागम;

    nesting_level_बायाँ--;
    recurse_func (एतत्, lookup_index, covered_seq_indicies, seq_index, end_index);
    nesting_level_बायाँ++;
  }

  शून्य reset_lookup_visit_गिनती ()
  { lookup_गिनती = 0; }

  तार्किक lookup_limit_exceeded ()
  { प्रत्यागम lookup_गिनती > HB_MAX_LOOKUP_VISIT_गिनती; }

  तार्किक should_visit_lookup (अचिन्हित पूर्णाङ्क lookup_index)
  {
    यदि (lookup_गिनती++ > HB_MAX_LOOKUP_VISIT_गिनती)
      प्रत्यागम असत्य;

    यदि (is_lookup_done (lookup_index))
      प्रत्यागम असत्य;

    प्रत्यागम सत्य;
  }

  तार्किक is_lookup_done (अचिन्हित पूर्णाङ्क lookup_index)
  {
    यदि (unlikely (done_lookups_glyph_गिनती->in_error () ||
		  done_lookups_glyph_set->in_error ()))
      प्रत्यागम सत्य;

    /* Have we visited this lookup with the current set of glyphs? */
    अचिन्हित निकालो = glyphs->get_population ();
    यदि (unlikely (निकालो == HB_MAP_मूल्य_INVALID)) // Avoid sentinel interference.
      निकालो--;
    यदि (done_lookups_glyph_गिनती->get (lookup_index) != निकालो)
    {
      done_lookups_glyph_गिनती->set (lookup_index, निकालो);

      यदि (!done_lookups_glyph_set->has (lookup_index))
      {
	यदि (unlikely (!done_lookups_glyph_set->set (lookup_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	  प्रत्यागम सत्य;
      }

      done_lookups_glyph_set->get (lookup_index)->clear ();
    }

    hb_set_t *covered_glyph_set = done_lookups_glyph_set->get (lookup_index);
    यदि (unlikely (covered_glyph_set->in_error ()))
      प्रत्यागम सत्य;
    यदि (parent_active_glyphs ().is_subset (*covered_glyph_set))
      प्रत्यागम सत्य;

    covered_glyph_set->union_ (parent_active_glyphs ());
    प्रत्यागम असत्य;
  }

  ध्रुव hb_set_t& previous_parent_active_glyphs () {
    यदि (active_glyphs_ढेर.length <= 1)
      प्रत्यागम *glyphs;

    प्रत्यागम active_glyphs_ढेर[active_glyphs_ढेर.length - 2];
  }

  ध्रुव hb_set_t& parent_active_glyphs ()
  {
    यदि (!active_glyphs_ढेर)
      प्रत्यागम *glyphs;

    प्रत्यागम active_glyphs_ढेर.tail ();
  }

  hb_set_t* धकेलो_cur_active_glyphs ()
  {
    hb_set_t *s = active_glyphs_ढेर.push ();
    यदि (unlikely (active_glyphs_ढेर.in_error ()))
      प्रत्यागम nullptr;
    प्रत्यागम s;
  }

  तार्किक निकालो_cur_done_glyphs ()
  {
    यदि (!active_glyphs_ढेर)
      प्रत्यागम असत्य;

    active_glyphs_ढेर.निकालो ();
    प्रत्यागम सत्य;
  }

  hb_face_t *face;
  hb_set_t *glyphs;
  hb_set_t output[1];
  hb_vector_t<hb_set_t> active_glyphs_ढेर;
  recurse_func_t recurse_func = nullptr;
  अचिन्हित पूर्णाङ्क nesting_level_बायाँ;

  hb_closure_context_t (hb_face_t *face_,
			hb_set_t *glyphs_,
			hb_map_t *done_lookups_glyph_गिनती_2,
			hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set_,
			अचिन्हित पूर्णाङ्क nesting_level_बायाँ_2 = HB_MAX_NESTING_LEVEL) :
			  face (face_),
			  glyphs (glyphs_),
			  nesting_level_बायाँ (nesting_level_बायाँ_2),
			  done_lookups_glyph_गिनती (done_lookups_glyph_गिनती_2),
			  done_lookups_glyph_set (done_lookups_glyph_set_)
  {}

  ~hb_closure_context_t () { flush (); }

  शून्य set_recurse_func (recurse_func_t func) { recurse_func = func; }

  शून्य flush ()
  {
    output->del_range (face->get_अंक_glyphs (), HB_SET_मूल्य_INVALID);	/* Remove invalid glyphs. */
    glyphs->union_ (*output);
    output->clear ();
    active_glyphs_ढेर.निकालो ();
    active_glyphs_ढेर.reset ();
  }

  गुप्त:
  hb_map_t *done_lookups_glyph_गिनती;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *done_lookups_glyph_set;
  अचिन्हित पूर्णाङ्क lookup_गिनती = 0;
};



संरचना hb_closure_lookups_context_t :
       hb_dispatch_context_t<hb_closure_lookups_context_t>
{
  प्रकारनिर्देश return_t (*recurse_func_t) (hb_closure_lookups_context_t *c, अचिन्हित lookup_index);
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj) { obj.closure_lookups (एतत्); प्रत्यागम hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम hb_empty_t (); }
  शून्य recurse (अचिन्हित lookup_index)
  {
    यदि (unlikely (nesting_level_बायाँ == 0 || !recurse_func))
      प्रत्यागम;

    /* Return if new lookup was recursed to before. */
    यदि (lookup_limit_exceeded ()
        || visited_lookups->in_error ()
        || visited_lookups->has (lookup_index))
      // Don't increment lookup count here, that will be done in the call to closure_lookups()
      // made by recurse_func.
      प्रत्यागम;

    nesting_level_बायाँ--;
    recurse_func (एतत्, lookup_index);
    nesting_level_बायाँ++;
  }

  शून्य set_lookup_visited (अचिन्हित lookup_index)
  { visited_lookups->add (lookup_index); }

  शून्य set_lookup_inactive (अचिन्हित lookup_index)
  { inactive_lookups->add (lookup_index); }

  तार्किक lookup_limit_exceeded ()
  {
    तार्किक ret = lookup_गिनती > HB_MAX_LOOKUP_VISIT_गिनती;
    यदि (ret)
      DEBUG_संदेश (SUBSET, nullptr, "lookup visit count limit exceeded in lookup closure!");
    प्रत्यागम ret; }

  तार्किक is_lookup_visited (अचिन्हित lookup_index)
  {
    यदि (unlikely (lookup_गिनती++ > HB_MAX_LOOKUP_VISIT_गिनती))
    {
      DEBUG_संदेश (SUBSET, nullptr, "total visited lookup count %u exceeds max limit, lookup %u is dropped.",
                 lookup_गिनती, lookup_index);
      प्रत्यागम सत्य;
    }

    यदि (unlikely (visited_lookups->in_error ()))
      प्रत्यागम सत्य;

    प्रत्यागम visited_lookups->has (lookup_index);
  }

  hb_face_t *face;
  ध्रुव hb_set_t *glyphs;
  recurse_func_t recurse_func;
  अचिन्हित पूर्णाङ्क nesting_level_बायाँ;

  hb_closure_lookups_context_t (hb_face_t *face_,
				ध्रुव hb_set_t *glyphs_,
				hb_set_t *visited_lookups_,
				hb_set_t *inactive_lookups_,
				अचिन्हित nesting_level_बायाँ_2 = HB_MAX_NESTING_LEVEL) :
				face (face_),
				glyphs (glyphs_),
				recurse_func (nullptr),
				nesting_level_बायाँ (nesting_level_बायाँ_2),
				visited_lookups (visited_lookups_),
				inactive_lookups (inactive_lookups_),
				lookup_गिनती (0) {}

  शून्य set_recurse_func (recurse_func_t func) { recurse_func = func; }

  गुप्त:
  hb_set_t *visited_lookups;
  hb_set_t *inactive_lookups;
  अचिन्हित पूर्णाङ्क lookup_गिनती;
};

संरचना hb_would_apply_context_t :
       hb_dispatch_context_t<hb_would_apply_context_t, तार्किक>
{
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj) { प्रत्यागम obj.would_apply (एतत्); }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) ध्रुव { प्रत्यागम r; }

  hb_face_t *face;
  ध्रुव hb_codepoint_t *glyphs;
  अचिन्हित पूर्णाङ्क len;
  तार्किक zero_context;

  hb_would_apply_context_t (hb_face_t *face_,
			    ध्रुव hb_codepoint_t *glyphs_,
			    अचिन्हित पूर्णाङ्क len_,
			    तार्किक zero_context_) :
			      face (face_),
			      glyphs (glyphs_),
			      len (len_),
			      zero_context (zero_context_) {}
};

संरचना hb_collect_glyphs_context_t :
       hb_dispatch_context_t<hb_collect_glyphs_context_t>
{
  प्रकारनिर्देश return_t (*recurse_func_t) (hb_collect_glyphs_context_t *c, अचिन्हित पूर्णाङ्क lookup_index);
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj) { obj.collect_glyphs (एतत्); प्रत्यागम hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम hb_empty_t (); }
  शून्य recurse (अचिन्हित पूर्णाङ्क lookup_index)
  {
    यदि (unlikely (nesting_level_बायाँ == 0 || !recurse_func))
      प्रत्यागम;

    /* Note that GPOS sets recurse_func to nullptr already, so it doesn't get
     * past the previous check.  For GSUB, we only want to collect the output
     * glyphs in the recursion.  If output is not requested, we can go home now.
     *
     * Note further, that the above is not exactly correct.  A recursed lookup
     * is allowed to match input that is not matched in the context, but that's
     * not how most fonts are built.  It's possible to relax that and recurse
     * with all sets here if it proves to be an issue.
     */

    यदि (output == hb_set_get_empty ())
      प्रत्यागम;

    /* Return if new lookup was recursed to before. */
    यदि (recursed_lookups->has (lookup_index))
      प्रत्यागम;

    hb_set_t *old_before = before;
    hb_set_t *old_निवेश  = निवेश;
    hb_set_t *old_after  = after;
    before = निवेश = after = hb_set_get_empty ();

    nesting_level_बायाँ--;
    recurse_func (एतत्, lookup_index);
    nesting_level_बायाँ++;

    before = old_before;
    निवेश  = old_निवेश;
    after  = old_after;

    recursed_lookups->add (lookup_index);
  }

  hb_face_t *face;
  hb_set_t *before;
  hb_set_t *निवेश;
  hb_set_t *after;
  hb_set_t *output;
  recurse_func_t recurse_func;
  hb_set_t *recursed_lookups;
  अचिन्हित पूर्णाङ्क nesting_level_बायाँ;

  hb_collect_glyphs_context_t (hb_face_t *face_,
			       hb_set_t  *glyphs_before, /* OUT.  May be NULL */
			       hb_set_t  *glyphs_निवेश,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_after,  /* OUT.  May be NULL */
			       hb_set_t  *glyphs_output, /* OUT.  May be NULL */
			       अचिन्हित पूर्णाङ्क nesting_level_बायाँ_2 = HB_MAX_NESTING_LEVEL) :
			      face (face_),
			      before (glyphs_before ? glyphs_before : hb_set_get_empty ()),
			      निवेश  (glyphs_निवेश  ? glyphs_निवेश  : hb_set_get_empty ()),
			      after  (glyphs_after  ? glyphs_after  : hb_set_get_empty ()),
			      output (glyphs_output ? glyphs_output : hb_set_get_empty ()),
			      recurse_func (nullptr),
			      recursed_lookups (hb_set_create ()),
			      nesting_level_बायाँ (nesting_level_बायाँ_2) {}
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

  संरचना glyph_set_cache_कुंजी_t
  {
    glyph_set_cache_कुंजी_t () = स्वभाव;
    glyph_set_cache_कुंजी_t (ध्रुव शून्य *data_, अचिन्हित मूल्य_3, glyph_set_type_t type_)
      : data (data_), value (मूल्य_3), type (type_) {}

    तार्किक चालक == (ध्रुव glyph_set_cache_कुंजी_t &o) ध्रुव
    { प्रत्यागम data == o.data && value == o.value && type == o.type; }

    uint32_t hash () ध्रुव
    {
      uint32_t current = hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (value);
      प्रत्यागम current * 31 + hb_hash ((अचिन्हित) type);
    }

    ध्रुव शून्य *data = nullptr;
    अचिन्हित value = 0;
    glyph_set_type_t type = GLYPH_SET_CLASS;
  };

  संरचना context_set_cache_कुंजी_t
  {
    context_set_cache_कुंजी_t () = स्वभाव;
    context_set_cache_कुंजी_t (ध्रुव शून्य *rule_,
			     ध्रुव शून्य *record_,
			     ध्रुव शून्य *data_,
			     hb_codepoint_t active_glyphs_,
			     अचिन्हित मूल्य_3,
			     अचिन्हित format_)
      : rule (rule_), record (record_), data (data_), active_glyphs (active_glyphs_),
	value (मूल्य_3), format (format_) {}

    तार्किक चालक == (ध्रुव context_set_cache_कुंजी_t &o) ध्रुव
    {
      प्रत्यागम rule == o.rule &&
	     record == o.record &&
	     data == o.data &&
	     active_glyphs == o.active_glyphs &&
	     value == o.value &&
	     format == o.format;
    }

    uint32_t hash () ध्रुव
    {
      uint32_t current = hb_hash ((uintptr_t) rule);
      current = current * 31 + hb_hash ((uintptr_t) record);
      current = current * 31 + hb_hash ((uintptr_t) data);
      current = current * 31 + hb_hash (active_glyphs);
      current = current * 31 + hb_hash (value);
      प्रत्यागम current * 31 + hb_hash (format);
    }

    ध्रुव शून्य *rule = nullptr;
    ध्रुव शून्य *record = nullptr;
    ध्रुव शून्य *data = nullptr;
    hb_codepoint_t active_glyphs = HB_CODEPOINT_INVALID;
    अचिन्हित value = 0;
    अचिन्हित format = 0;
  };

  संरचना recurse_कुंजी_t
  {
    recurse_कुंजी_t () = स्वभाव;
    recurse_कुंजी_t (अचिन्हित lookup_index_,
                   hb_codepoint_t active_glyphs_,
                   hb_codepoint_t recurse_path_,
                   hb_codepoint_t context_set_)
      : lookup_index (lookup_index_),
        active_glyphs (active_glyphs_),
        recurse_path (recurse_path_),
        context_set (context_set_) {}

    तार्किक चालक == (ध्रुव recurse_कुंजी_t &o) ध्रुव
    {
      प्रत्यागम lookup_index == o.lookup_index &&
             active_glyphs == o.active_glyphs &&
             recurse_path == o.recurse_path &&
             context_set == o.context_set;
    }

    uint32_t hash () ध्रुव
    {
      uint32_t current = 0x84222325;
      current = (current ^ hb_hash (lookup_index)) * 16777619;
      current = (current ^ hb_hash (active_glyphs)) * 16777619;
      current = (current ^ hb_hash (recurse_path)) * 16777619;
      current = (current ^ hb_hash (context_set)) * 16777619;
      प्रत्यागम current;
    }

    अचिन्हित lookup_index = 0;
    hb_codepoint_t active_glyphs = 0;
    hb_codepoint_t recurse_path = 0;
    hb_codepoint_t context_set = HB_CODEPOINT_INVALID;
  };

  प्रकारनिर्देश return_t (*recurse_func_t) (hb_depend_context_t *c, अचिन्हित lookup_index, hb_bit_page_t *covered_seq_indices, अचिन्हित seq_index, अचिन्हित end_index);
  /* return_t is hb_empty_t — dispatch is purely for side-effects, like hb_closure_context_t. */
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj) { obj.depend (एतत्); प्रत्यागम hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम hb_empty_t (); }

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
    यदि (lookups_seen.has (lookup_idx)) प्रत्यागम;
    lookups_seen.add (lookup_idx);

    /* Intern the ordered recursion path instead of copying and interning the
     * lookups_seen set.  Path identity is more specific than set identity but
     * never merges states with different cycle-detection behavior. */
    hb_codepoint_t parent_path = recurse_path;
    uint64_t path_कुंजी = (uint64_t) parent_path << 32 | lookup_idx;
    hb_codepoint_t *path = nullptr;
    hb_codepoint_t path_index;
    यदि (!recurse_path_to_index.has (path_कुंजी, &path))
    {
      path_index = recurse_path_to_index.get_population () + 1;
      यदि (unlikely (!recurse_path_to_index.set (path_कुंजी, path_index)))
      {
	depend_data->fail ();
	lookups_seen.del (lookup_idx);
	प्रत्यागम;
      }
    }
    अन्यथा
      path_index = *path;
    recurse_path = path_index;
    recurse_func (एतत्, lookup_idx, covered_seq_indices, seq_index, end_index);
    recurse_path = parent_path;
    lookups_seen.del (lookup_idx);
  }

  संरचना active_glyphs_frame_t
  {
    hb_set_t glyphs;
    hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
    तार्किक use_मूल = असत्य;
  };

  ध्रुव hb_set_t& parent_active_glyphs ()
  {
    यदि (!active_glyphs_ढेर_depth)
      प्रत्यागम *glyphs;

    ध्रुव active_glyphs_frame_t &frame = active_glyphs_ढेर[active_glyphs_ढेर_depth - 1];
    प्रत्यागम frame.use_मूल ? *glyphs : frame.glyphs;
  }

  hb_set_t* धकेलो_cur_active_glyphs (hb_codepoint_t active_idx = HB_CODEPOINT_INVALID)
  {
    यदि (active_glyphs_ढेर_depth == active_glyphs_ढेर.length &&
	unlikely (!active_glyphs_ढेर.धकेलो_or_fail ()))
      प्रत्यागम nullptr;

    active_glyphs_frame_t &frame = active_glyphs_ढेर[active_glyphs_ढेर_depth++];
    frame.glyphs.clear ();
    frame.active_idx = active_idx;
    frame.use_मूल = असत्य;
    प्रत्यागम &frame.glyphs;
  }

  तार्किक धकेलो_मूल_active_glyphs ()
  {
    यदि (active_glyphs_ढेर_depth == active_glyphs_ढेर.length &&
	unlikely (!active_glyphs_ढेर.धकेलो_or_fail ()))
      प्रत्यागम असत्य;

    active_glyphs_ढेर[active_glyphs_ढेर_depth++].use_मूल = सत्य;
    प्रत्यागम सत्य;
  }

  तार्किक निकालो_cur_done_glyphs ()
  {
    यदि (!active_glyphs_ढेर_depth)
      प्रत्यागम असत्य;

    active_glyphs_ढेर_depth--;
    प्रत्यागम सत्य;
  }

  तार्किक get_parent_active_glyphs_index (hb_codepoint_t *active_idx)
  {
    यदि (!active_glyphs_ढेर_depth ||
	active_glyphs_ढेर[active_glyphs_ढेर_depth - 1].use_मूल)
    {
      *active_idx = HB_CODEPOINT_INVALID;
      प्रत्यागम सत्य;
    }

    active_glyphs_frame_t &frame = active_glyphs_ढेर[active_glyphs_ढेर_depth - 1];
    *active_idx = frame.active_idx;
    यदि (*active_idx != HB_CODEPOINT_INVALID)
      प्रत्यागम सत्य;

    hb_codepoint_t glyph;
    यदि (frame.glyphs.get_singleton (&glyph) &&
	likely (glyph < SINGLE_GLYPH_ACTIVE_FLAG))
      *active_idx = glyph | SINGLE_GLYPH_ACTIVE_FLAG;
    अन्यथा यदि (!find_or_create_recurse_set (frame.glyphs, active_idx))
      प्रत्यागम असत्य;

    frame.active_idx = *active_idx;
    प्रत्यागम सत्य;
  }

  तार्किक get_context_set_cache_कुंजी (ध्रुव शून्य *rule,
				  ध्रुव शून्य *record,
				  ध्रुव शून्य *data,
				  अचिन्हित value,
				  अचिन्हित format,
				  context_set_cache_कुंजी_t *key)
  {
    hb_codepoint_t active_idx;
    यदि (!get_parent_active_glyphs_index (&active_idx))
      प्रत्यागम असत्य;
    *key = context_set_cache_कुंजी_t (rule, record, data, active_idx, value, format);
    प्रत्यागम सत्य;
  }

  तार्किक get_recurse_कुंजी (अचिन्हित lookup_idx, recurse_कुंजी_t *key)
  {
    /* The active glyphs and in-progress lookup set are part of the state.
     * Omitting the latter would make memoization incorrect for cyclic lookup
     * graphs whose available recursive paths depend on their ancestry.
     * Edge flags are not part of the state: they affect neither traversal nor
     * edge identity, so revisiting with different flags can only find edges
     * that the deduplication table already contains. */
    hb_codepoint_t active_idx;
    यदि (!get_parent_active_glyphs_index (&active_idx))
      प्रत्यागम असत्य;

    *key = recurse_कुंजी_t (lookup_idx,
                          active_idx,
                          recurse_path,
                          depend_data->current_context_set_index);
    प्रत्यागम !completed_recursions.has (*key);
  }

  शून्य finish_recurse (ध्रुव recurse_कुंजी_t &key)
  { depend_data->check_success (completed_recursions.set (key, सत्य)); }

  शून्य reset_recurse_cache ()
  {
    completed_recursions.clear ();
    recurse_path_to_index.clear ();
    recurse_path = 0;
  }

  तार्किक find_or_create_recurse_set (ध्रुव hb_set_t &set, hb_codepoint_t *index)
  {
    hb_codepoint_t *existing = nullptr;
    यदि (recurse_set_to_index.has (&set, &existing))
    {
      *index = *existing;
      प्रत्यागम सत्य;
    }

    hb_set_t *copy = hb_object_create<hb_set_t> ();
    यदि (unlikely (!copy))
      प्रत्यागम depend_data->fail ();
    copy->set (set);
    यदि (unlikely (copy->in_error ()))
    {
      hb_set_destroy (copy);
      प्रत्यागम depend_data->fail ();
    }

    *index = recurse_sets.length;
    यदि (unlikely (*index & SINGLE_GLYPH_ACTIVE_FLAG))
    {
      hb_set_destroy (copy);
      प्रत्यागम depend_data->fail ();
    }
    यदि (unlikely (!recurse_sets.धकेलो_or_fail (hb::unique_ptr<hb_set_t> {copy})))
      प्रत्यागम depend_data->fail ();
    यदि (unlikely (!recurse_set_to_index.set (recurse_sets[*index].get (), *index)))
      प्रत्यागम depend_data->fail ();
    प्रत्यागम सत्य;
  }

  ध्रुव hb_set_t *find_or_create_glyph_set (ध्रुव शून्य *data,
                                             अचिन्हित value,
                                             glyph_set_type_t type);
  hb_codepoint_t find_or_create_cached_context_set (ध्रुव hb_set_t *set);

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
      निवेश_position_glyphs.clear ();
      cached_context_sets.clear ();
      compact_workspace.clear ();
    }

    hb_set_t preliminary_context;
    hb_set_t position_scratch;
    hb_set_t pos_glyphs;
    hb_set_t position_context;
    hb_set_t disjunctive_indices;
    hb_set_t filtered_disjunctive_indices;
    hb_vector_t<ध्रुव hb_set_t *> backtrack_sets;
    hb_vector_t<ध्रुव hb_set_t *> lookahead_sets;
    hb_vector_t<ध्रुव hb_set_t *> निवेश_position_glyphs;
    hb_vector_t<hb_codepoint_t> cached_context_sets;
    hb_vector_t<अचिन्हित> compact_workspace;
    depend_scratch_t *next = nullptr;
  };

  depend_scratch_t *acquire_depend_scratch ()
  {
    depend_scratch_t *scratch = depend_scratch_pool;
    यदि (scratch)
      depend_scratch_pool = scratch->next;
    अन्यथा
    {
      scratch = (depend_scratch_t *) hb_malloc (परिमाण (depend_scratch_t));
      यदि (unlikely (!scratch))
      {
        depend_data->fail ();
        प्रत्यागम nullptr;
      }
      नूतन (scratch) depend_scratch_t ();
    }

    scratch->next = nullptr;
    scratch->clear ();
    प्रत्यागम scratch;
  }

  शून्य release_depend_scratch (depend_scratch_t *scratch)
  {
    scratch->next = depend_scratch_pool;
    depend_scratch_pool = scratch;
  }

  hb_depend_data_builder_t *depend_data;
  hb_face_t *face;
  hb_set_t *glyphs;
  hb_vector_t<active_glyphs_frame_t> active_glyphs_ढेर;
  अचिन्हित active_glyphs_ढेर_depth = 0;
  recurse_func_t recurse_func;
  hb_codepoint_t lookup_index = HB_CODEPOINT_INVALID;
  hb_bit_set_t lookups_seen;
  hb_vector_t<hb::unique_ptr<hb_set_t>> recurse_sets;
  hb_hashmap_t<ध्रुव hb_set_t *, hb_codepoint_t> recurse_set_to_index;
  hb_hashmap_t<uint64_t, hb_codepoint_t> recurse_path_to_index;
  hb_codepoint_t recurse_path = 0;
  hb_hashmap_t<recurse_कुंजी_t, तार्किक> completed_recursions;
  hb_vector_t<hb::unique_ptr<hb_set_t>> glyph_sets;
  hb_hashmap_t<glyph_set_cache_कुंजी_t, hb_codepoint_t> glyph_set_to_index;
  hb_hashmap_t<uintptr_t, hb_codepoint_t> cached_context_set_indices;
  hb_hashmap_t<context_set_cache_कुंजी_t, hb_codepoint_t> context_set_cache;
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
    यावत् (depend_scratch_pool)
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
       hb_dispatch_context_t<hb_collect_coverage_context_t<set_t>, ध्रुव Coverage &>
{
  प्रकारनिर्देश ध्रुव Coverage &return_t; // Stoopid that we have to dupe this here.
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj) { प्रत्यागम obj.get_coverage (); }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम Null (Coverage); }
  तार्किक stop_sublookup_iteration (return_t r) ध्रुव
  {
    r.collect_coverage (set);
    प्रत्यागम असत्य;
  }

  hb_collect_coverage_context_t (set_t *set_) :
				   set (set_) {}

  set_t *set;
};

संरचना matcher_t
{
  प्रकारनिर्देश तार्किक (*match_func_t) (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data);

  ढाँचा <typename context_t>
  शून्य init (ध्रुव context_t *c, तार्किक context_match = असत्य)
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
		       ध्रुव शून्य *match_data_)
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
			 hb_codepoint_t glyph_data) ध्रुव
  {
    यदि (!(info.mask & mask) ||
	(per_syllable && syllable && syllable != info.syllable ()))
      प्रत्यागम MATCH_NO;

    यदि (match_func)
      प्रत्यागम match_func (info, glyph_data, match_data) ? MATCH_YES : MATCH_NO;

    प्रत्यागम MATCH_MAYBE;
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
  may_skip_t may_skip (ध्रुव context_t *c,
		       ध्रुव hb_glyph_info_t &info) ध्रुव
  {
    यदि (!c->check_glyph_property (&info, lookup_props))
      प्रत्यागम SKIP_YES;

    यदि (unlikely (_hb_glyph_info_is_default_ignorable (&info) &&
		  (ignore_zwnj || !_hb_glyph_info_is_zwnj (&info)) &&
		  (ignore_zwj || !_hb_glyph_info_is_zwj (&info)) &&
		  (ignore_hidden || !_hb_glyph_info_is_hidden (&info))))
      प्रत्यागम SKIP_MAYBE;

    प्रत्यागम SKIP_NO;
  }

  सार्वजनिक:
  अचिन्हित पूर्णाङ्क lookup_props = 0;
  hb_mask_t mask = -1;
  तार्किक ignore_zwnj = असत्य;
  तार्किक ignore_zwj = असत्य;
  तार्किक ignore_hidden = असत्य;
  तार्किक per_syllable = असत्य;
  uint8_t syllable = 0;
  match_func_t match_func = nullptr;
  ध्रुव शून्य *match_data = nullptr;
};

/* A view of 16- or 24-bit glyph IDs or coverage offsets. */
संरचना glyph_data_t
{
  ढाँचा <typename T>
  glyph_data_t (ध्रुव T *data_) : data (data_)
#ifndef HB_NO_BEYOND_64K
                              , wide (परिमाण (T) == 3)
#endif
  {
    static_assert (परिमाण (T) == 2 || परिमाण (T) == 3, "invalid glyph data size");
  }

  अचिन्हित चालक [] (अचिन्हित i) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    यदि (wide)
      प्रत्यागम ((ध्रुव HBUINT24 *) data)[i];
#endif
    प्रत्यागम ((ध्रुव HBUINT16 *) data)[i];
  }

  ध्रुव शून्य *data;
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
  शून्य set_lookup_props (अचिन्हित पूर्णाङ्क lookup_props)
  {
    matcher.lookup_props = lookup_props;
  }
  शून्य set_match_func (matcher_t::match_func_t match_func_,
		       ध्रुव शून्य *match_data_)
  {
    matcher.set_match_func (match_func_, match_data_);
  }
  शून्य set_glyph_data (glyph_data_t glyph_data)
  {
#ifndef HB_NO_BEYOND_64K
    match_glyph_data16 = glyph_data.wide ? nullptr : (ध्रुव HBUINT16 *) glyph_data.data;
    match_glyph_data24 = glyph_data.wide ? (ध्रुव HBUINT24 *) glyph_data.data : nullptr;
#अन्यथा
    match_glyph_data16 = (ध्रुव HBUINT16 *) glyph_data.data;
#endif
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य reset (अचिन्हित पूर्णाङ्क start_index_)
  {
    // For GSUB forward iterator
    idx = start_index_;
    end = c->buffer->len;
    matcher.syllable = c->buffer->cur().syllable();
  }
  शून्य reset_back (अचिन्हित पूर्णाङ्क start_index_, तार्किक from_out_buffer = असत्य)
  {
    // For GSUB backward iterator
    idx = start_index_;
    matcher.syllable = c->buffer->cur().syllable();
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य reset_fast (अचिन्हित पूर्णाङ्क start_index_)
  {
    // Doesn't set end or syllable. Used by GPOS which doesn't care / change.
    idx = start_index_;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  matcher_t::may_skip_t may_skip (ध्रुव hb_glyph_info_t &info) ध्रुव
  { प्रत्यागम matcher.may_skip (c, info); }

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
    यदि (unlikely (skip == matcher_t::SKIP_YES))
      प्रत्यागम SKIP;

    matcher_t::may_match_t match = matcher.may_match (info, get_glyph_data ());
    यदि (match == matcher_t::MATCH_YES ||
	(match == matcher_t::MATCH_MAYBE &&
	 skip == matcher_t::SKIP_NO))
      प्रत्यागम MATCH;

    यदि (skip == matcher_t::SKIP_NO)
      प्रत्यागम NOT_MATCH;

    प्रत्यागम SKIP;
  }

  तार्किक next (अचिन्हित *unsafe_to = nullptr)
  {
    स्वतः *info = c->buffer->info;
    ध्रुव चिन्हित stop = (चिन्हित) end - 1;
    यावत् ((चिन्हित) idx < stop)
    {
      idx++;
      विकल्प (match (info[idx]))
      {
	पक्ष MATCH:
	{
	  advance_glyph_data ();
	  प्रत्यागम सत्य;
	}
	पक्ष NOT_MATCH:
	{
	  यदि (unsafe_to)
	    *unsafe_to = idx + 1;
	  प्रत्यागम असत्य;
	}
	पक्ष SKIP:
	  अनुवर्त;
      }
    }
    यदि (unsafe_to)
      *unsafe_to = end;
    प्रत्यागम असत्य;
  }
  तार्किक prev (अचिन्हित *unsafe_from = nullptr)
  {
    स्वतः *out_info = c->buffer->out_info;
    ध्रुव अचिन्हित stop = 0;
    यावत् (idx > stop)
    {
      idx--;
      विकल्प (match (out_info[idx]))
      {
	पक्ष MATCH:
	{
	  advance_glyph_data ();
	  प्रत्यागम सत्य;
	}
	पक्ष NOT_MATCH:
	{
	  यदि (unsafe_from)
	    *unsafe_from = hb_max (1u, idx) - 1u;
	  प्रत्यागम असत्य;
	}
	पक्ष SKIP:
	  अनुवर्त;
      }
    }
    यदि (unsafe_from)
      *unsafe_from = 0;
    प्रत्यागम असत्य;
  }

  HB_ALWAYS_INLINE
  hb_codepoint_t
  get_glyph_data ()
  {
    यदि (match_glyph_data16) प्रत्यागम *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
    अन्यथा
    यदि (match_glyph_data24) प्रत्यागम *match_glyph_data24;
#endif
    प्रत्यागम 0;
  }
  HB_ALWAYS_INLINE
  शून्य
  advance_glyph_data ()
  {
    यदि (match_glyph_data16) match_glyph_data16++;
#ifndef HB_NO_BEYOND_64K
    अन्यथा
    यदि (match_glyph_data24) match_glyph_data24++;
#endif
  }

  अचिन्हित पूर्णाङ्क idx;
  रक्षित:
  context_t *c;
  matcher_t matcher;
  ध्रुव HBUINT16 *match_glyph_data16;
#ifndef HB_NO_BEYOND_64K
  ध्रुव HBUINT24 *match_glyph_data24;
#endif

  अचिन्हित पूर्णाङ्क end;
};

संरचना hb_ot_apply_context_t :
       hb_dispatch_context_t<hb_ot_apply_context_t, तार्किक, HB_DEBUG_APPLY>
{
  ध्रुव वर्ण *get_name () { प्रत्यागम "APPLY"; }
  प्रकारनिर्देश return_t (*recurse_func_t) (hb_ot_apply_context_t *c, अचिन्हित पूर्णाङ्क lookup_index);

  ढाँचा <typename T>
  स्थिर अन्तःस्थ स्वतः apply_ (ध्रुव T &obj, hb_ot_apply_context_t *c, hb_priority<1>) HB_RETURN (return_t, obj.apply (c, nullptr) )
  ढाँचा <typename T>
  स्थिर अन्तःस्थ स्वतः apply_ (ध्रुव T &obj, hb_ot_apply_context_t *c, hb_priority<0>) HB_RETURN (return_t, obj.apply (c) )
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj) { प्रत्यागम apply_(obj, एतत्, hb_prioritize); }

  स्थिर return_t default_return_मूल्य () { प्रत्यागम असत्य; }
  तार्किक stop_sublookup_iteration (return_t r) ध्रुव { प्रत्यागम r; }
  return_t recurse (अचिन्हित पूर्णाङ्क sub_lookup_index)
  {
    प्रतिज्ञा (recurse_func);
    यदि (unlikely (nesting_level_बायाँ == 0))
    {
      buffer->successful = असत्य;
      प्रत्यागम default_return_मूल्य ();
    }

    buffer->max_ops--;
    यदि (unlikely (buffer->max_ops < 0))
    {
      buffer->successful = असत्य;
      प्रत्यागम default_return_मूल्य ();
    }

    nesting_level_बायाँ--;
    तार्किक ret = recurse_func (एतत्, sub_lookup_index);
    nesting_level_बायाँ++;
    प्रत्यागम ret;
  }

  skipping_iterator_t<hb_ot_apply_context_t> iter_निवेश, iter_context;

  अचिन्हित पूर्णाङ्क table_index; /* GSUB/GPOS */
  hb_font_t *font;
  hb_face_t *face;
  hb_buffer_t *buffer;
  hb_sanitize_context_t sanitizer;
  recurse_func_t recurse_func = nullptr;
  ध्रुव GDEF &gdef;
  ध्रुव GDEF::accelerator_t &gdef_accel;
  ध्रुव ItemVariationStore &var_store;
  hb_scalar_cache_t *var_store_cache;

  hb_direction_t direction;
  hb_mask_t lookup_mask = 1;
  अचिन्हित पूर्णाङ्क lookup_index = (अचिन्हित) -1;
  अचिन्हित पूर्णाङ्क lookup_props = 0;
  अचिन्हित पूर्णाङ्क nesting_level_बायाँ = HB_MAX_NESTING_LEVEL;

  तार्किक has_glyph_classes;
  तार्किक auto_zwnj = सत्य;
  तार्किक auto_zwj = सत्य;
  तार्किक per_syllable = असत्य;
  तार्किक random = असत्य;
  अचिन्हित new_syllables = (अचिन्हित) -1;

  चिन्हित last_base = -1; // GPOS uses
  अचिन्हित last_base_until = 0; // GPOS uses

  hb_vector_t<uint32_t> match_positions;
  uint32_t ढेर_match_positions[8];
#ifndef HB_NO_BUFFER_MESSAGE
  hb_buffer_t::changed_func_t orig_changed_func = nullptr;
  शून्य *orig_changed_data = nullptr;
#endif

  hb_ot_apply_context_t (अचिन्हित पूर्णाङ्क table_index_,
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
    match_positions.set_storage (ढेर_match_positions);

#ifndef HB_NO_BUFFER_MESSAGE
    यदि (buffer->messaging ())
    {
      प्रतिज्ञा (buffer->changed_func == nullptr ||
	      buffer->changed_func == buffer_changed_trampoline);
      orig_changed_func = buffer->changed_func;
      orig_changed_data = buffer->changed_data;
      buffer->changed_func = buffer_changed_trampoline;
      buffer->changed_data = एतत्;
    }
#endif
  }
  ~hb_ot_apply_context_t ()
  {
#ifndef HB_NO_BUFFER_MESSAGE
    यदि (buffer->messaging ())
    {
      प्रतिज्ञा (buffer->changed_func == buffer_changed_trampoline);
      प्रतिज्ञा (buffer->changed_data == एतत्);
      buffer->changed_func = orig_changed_func;
      buffer->changed_data = orig_changed_data;
    }
#endif
  }

  शून्य init_iters ()
  {
    iter_निवेश.init (एतत्, असत्य);
    iter_context.init (एतत्, सत्य);
  }

  शून्य set_lookup_mask (hb_mask_t mask, तार्किक init = सत्य) { lookup_mask = mask; last_base = -1; last_base_until = 0; यदि (init) init_iters (); }
  शून्य set_auto_zwj (तार्किक auto_zwj_, तार्किक init = सत्य) { auto_zwj = auto_zwj_; यदि (init) init_iters (); }
  शून्य set_auto_zwnj (तार्किक auto_zwnj_, तार्किक init = सत्य) { auto_zwnj = auto_zwnj_; यदि (init) init_iters (); }
  शून्य set_per_syllable (तार्किक per_syllable_, तार्किक init = सत्य) { per_syllable = per_syllable_; यदि (init) init_iters (); }
  शून्य set_random (तार्किक random_) { random = random_; }
  शून्य set_recurse_func (recurse_func_t func) { recurse_func = func; }
  शून्य set_lookup_index (अचिन्हित पूर्णाङ्क lookup_index_) { lookup_index = lookup_index_; }
  शून्य set_lookup_props (अचिन्हित पूर्णाङ्क lookup_props_)
  {
    lookup_props = gdef_accel.sanitize_lookup_props (lookup_props_);
    init_iters ();
  }

  uint32_t random_संख्या ()
  {
    /* http://www.cplusplus.com/reference/random/minstd_rand/ */
    buffer->random_state = buffer->random_state * 48271 % 2147483647;
    प्रत्यागम buffer->random_state;
  }

  स्थिर शून्य buffer_changed_trampoline (hb_buffer_t *buffer HB_UNUSED, शून्य *user_data)
  {
    ((hb_ot_apply_context_t *) user_data)->buffer_changed ();
  }
  शून्य buffer_changed ()
  {
    buffer->update_digest ();
    यदि (likely (has_glyph_classes))
      प्रति (अचिन्हित i = 0; i < buffer->len; i++)
	_set_glyph_class_props (buffer->info[i]);
  }

  HB_ALWAYS_INLINE
  HB_HOT
  तार्किक match_properties_mark (ध्रुव hb_glyph_info_t *info,
			      अचिन्हित पूर्णाङ्क    glyph_props,
			      अचिन्हित पूर्णाङ्क    match_props) ध्रुव
  {
    /* If using mark filtering sets, the high short of
     * match_props has the set index.
     */
    यदि (match_props & LookupFlag::UseMarkFilteringSet)
    {
      अचिन्हित set_index = match_props >> 16;
      प्रत्यागम gdef_accel.mark_set_covers (set_index, info->codepoint);
    }

    /* The second byte of match_props has the meaning
     * "ignore marks of attachment type different than
     * the attachment type specified."
     */
    यदि (match_props & LookupFlag::MarkAttachmentType)
      प्रत्यागम (match_props & LookupFlag::MarkAttachmentType) == (glyph_props & LookupFlag::MarkAttachmentType);

    प्रत्यागम सत्य;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक check_glyph_property (ध्रुव hb_glyph_info_t *info,
			     अचिन्हित match_props) ध्रुव
  {
    अचिन्हित पूर्णाङ्क glyph_props = _hb_glyph_info_get_glyph_props (info);

    /* Not covered, if, for example, glyph class is ligature and
     * match_props includes LookupFlags::IgnoreLigatures
     */
    यदि (glyph_props & match_props & LookupFlag::IgnoreFlags)
      प्रत्यागम असत्य;

    यदि (glyph_props & HB_OT_LAYOUT_GLYPH_PROPS_MARK)
      प्रत्यागम match_properties_mark (info, glyph_props, match_props);

    प्रत्यागम सत्य;
  }

  शून्य _set_glyph_class (hb_codepoint_t glyph_index,
			  अचिन्हित पूर्णाङ्क class_guess = 0,
			  तार्किक ligature = असत्य,
			  तार्किक component = असत्य)
  {
    buffer->digest.add (glyph_index);

    यदि (new_syllables != (अचिन्हित) -1)
      buffer->cur().syllable() = new_syllables;

    अचिन्हित पूर्णाङ्क props = _hb_glyph_info_get_glyph_props (&buffer->cur());
    props |= HB_OT_LAYOUT_GLYPH_PROPS_SUBSTITUTED;
    यदि (ligature)
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
    यदि (component)
      props |= HB_OT_LAYOUT_GLYPH_PROPS_MULTIPLIED;
    यदि (likely (has_glyph_classes))
    {
      _set_glyph_class_props (buffer->cur(), props, glyph_index);
    }
    अन्यथा यदि (class_guess)
    {
      props &= HB_OT_LAYOUT_GLYPH_PROPS_PRESERVE;
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props | class_guess);
    }
    अन्यथा
      _hb_glyph_info_set_glyph_props (&buffer->cur(), props);
  }
  शून्य _set_glyph_class_props (hb_glyph_info_t &info,
			       अचिन्हित पूर्णाङ्क props,
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
				    अचिन्हित पूर्णाङ्क class_guess)
  {
    _set_glyph_class (glyph_index, class_guess, सत्य);
    (शून्य) buffer->replace_glyph (glyph_index);
  }
  शून्य output_glyph_for_component (hb_codepoint_t glyph_index,
				   अचिन्हित पूर्णाङ्क class_guess)
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
  स्थिर अन्तःस्थ स्वतः collect_second_glyphs_ (ध्रुव T &obj,
					     hb_set_digest_t *digest,
					     hb_priority<1>) HB_RETURN (शून्य, obj.collect_second_glyphs (digest) )
  ढाँचा <typename T>
  स्थिर अन्तःस्थ शून्य collect_second_glyphs_ (ध्रुव T &obj HB_UNUSED,
					     hb_set_digest_t *digest,
					     hb_priority<0>)
  { *digest = hb_set_digest_t::full (); }

  ढाँचा <typename T>
  स्थिर अन्तःस्थ स्वतः apply_ (ध्रुव T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<1>) HB_RETURN (तार्किक, obj->apply (c, external_cache) )
  ढाँचा <typename T>
  स्थिर अन्तःस्थ स्वतः apply_ (ध्रुव T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<0>) HB_RETURN (तार्किक, obj->apply (c) )
  ढाँचा <typename T>
  स्थिर अन्तःस्थ तार्किक apply_to (ध्रुव शून्य *obj, hb_ot_apply_context_t *c, शून्य *external_cache)
  {
    ध्रुव T *typed_obj = (ध्रुव T *) obj;
    प्रत्यागम apply_ (typed_obj, c, external_cache, hb_prioritize);
  }

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
  ढाँचा <typename T>
  स्थिर अन्तःस्थ स्वतः apply_cached_ (ध्रुव T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<2>) HB_RETURN (तार्किक, obj->apply_cached (c, external_cache) )
  ढाँचा <typename T>
  स्थिर अन्तःस्थ स्वतः apply_cached_ (ध्रुव T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<1>) HB_RETURN (तार्किक, obj->apply (c, external_cache) )
  ढाँचा <typename T>
  स्थिर अन्तःस्थ स्वतः apply_cached_ (ध्रुव T *obj, hb_ot_apply_context_t *c, शून्य *external_cache, hb_priority<0>) HB_RETURN (तार्किक, obj->apply (c) )
  ढाँचा <typename T>
  स्थिर अन्तःस्थ तार्किक apply_cached_to (ध्रुव शून्य *obj, hb_ot_apply_context_t *c, शून्य *external_cache)
  {
    ध्रुव T *typed_obj = (ध्रुव T *) obj;
    प्रत्यागम apply_cached_ (typed_obj, c, external_cache, hb_prioritize);
  }

  ढाँचा <typename T>
  स्थिर अन्तःस्थ स्वतः cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op,
				  hb_priority<1>) HB_RETURN (तार्किक, T::cache_func (c, op) )
  ढाँचा <typename T=शून्य>
  स्थिर अन्तःस्थ तार्किक cache_func_ (hb_ot_apply_context_t *c,
				  hb_ot_subtable_cache_op_t op HB_UNUSED,
				  hb_priority<0>) { प्रत्यागम असत्य; }
  ढाँचा <typename Type>
  स्थिर अन्तःस्थ तार्किक cache_func_to (hb_ot_apply_context_t *c,
				    hb_ot_subtable_cache_op_t op)
  {
    प्रत्यागम cache_func_<Type> (c, op, hb_prioritize);
  }
#endif

  प्रकारनिर्देश तार्किक (*hb_apply_func_t) (ध्रुव शून्य *obj, hb_ot_apply_context_t *c, शून्य *external_cache);
  प्रकारनिर्देश तार्किक (*hb_cache_func_t) (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op);

  संरचना hb_applicable_t
  {
    मित्र संरचना hb_accelerate_subtables_context_t;
    मित्र संरचना hb_ot_layout_lookup_accelerator_t;

    ढाँचा <typename T>
    शून्य init (ध्रुव T &obj_,
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
    तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
    {
      प्रत्यागम digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, nullptr);
    }
    तार्किक apply_no_digest (hb_ot_apply_context_t *c) ध्रुव
    {
      प्रत्यागम apply_func (obj, c, nullptr);
    }
#अन्यथा
    तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
    {
      प्रत्यागम digest.may_have (c->buffer->cur().codepoint) && apply_func (obj, c, external_cache);
    }
    तार्किक apply_cached (hb_ot_apply_context_t *c) ध्रुव
    {
      प्रत्यागम digest.may_have (c->buffer->cur().codepoint) &&  apply_cached_func (obj, c, external_cache);
    }
    तार्किक apply_no_digest (hb_ot_apply_context_t *c) ध्रुव
    {
      प्रत्यागम apply_func (obj, c, external_cache);
    }
    तार्किक apply_cached_no_digest (hb_ot_apply_context_t *c) ध्रुव
    {
      प्रत्यागम apply_cached_func (obj, c, external_cache);
    }

    तार्किक cache_enter (hb_ot_apply_context_t *c) ध्रुव
    {
      प्रत्यागम cache_func (c, hb_ot_subtable_cache_op_t::ENTER);
    }
    शून्य cache_leave (hb_ot_apply_context_t *c) ध्रुव
    {
      cache_func (c, hb_ot_subtable_cache_op_t::LEAVE);
    }
#endif

    गुप्त:
    ध्रुव शून्य *obj;
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
  स्वतः cache_cost (ध्रुव T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.cache_cost () )
  ढाँचा <typename T>
  स्वतः cache_cost (ध्रुव T &obj, hb_priority<0>) HB_AUTO_RETURN ( 0u )

  ढाँचा <typename T>
  स्वतः external_cache_create (ध्रुव T &obj, hb_priority<1>) HB_AUTO_RETURN ( obj.external_cache_create () )
  ढाँचा <typename T>
  स्वतः external_cache_create (ध्रुव T &obj, hb_priority<0>) HB_AUTO_RETURN ( nullptr )
#endif

  /* Dispatch interface. */
  ढाँचा <typename T>
  return_t dispatch (ध्रुव T &obj)
  {
    collect_second_glyphs_ (obj, &digest_second, hb_prioritize);

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    शून्य *external_cache = nullptr;
    यदि (i < 8)
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
    यदि (cost > subtable_cache_user_cost)
    {
      subtable_cache_user_idx = i - 1;
      subtable_cache_user_cost = cost;
    }
#endif

    प्रत्यागम hb_empty_t ();
  }
  स्थिर return_t default_return_मूल्य () { प्रत्यागम hb_empty_t (); }

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


प्रकारनिर्देश तार्किक (*intersects_func_t) (ध्रुव hb_set_t *glyphs, अचिन्हित value, ध्रुव शून्य *data, शून्य *cache);
प्रकारनिर्देश शून्य (*intersected_glyphs_func_t) (ध्रुव hb_set_t *glyphs, ध्रुव शून्य *data, अचिन्हित value, hb_set_t *intersected_glyphs, शून्य *cache);
प्रकारनिर्देश शून्य (*collect_glyphs_func_t) (hb_set_t *glyphs, अचिन्हित value, ध्रुव शून्य *data);
प्रकारनिर्देश तार्किक (*match_func_t) (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data);
प्रकारनिर्देश अचिन्हित (*get_मूल्य_func_t) (hb_glyph_info_t &info, ध्रुव शून्य *data);

/* One word per RuleSet, summarizing the classes accepted at the first
 * input position after the current glyph.  Collisions only cause extra work. */
संरचना hb_ot_layout_ruleset_digest_t
{
  शून्य init () { mask = 0; }
  शून्य set_full () { mask = (uint64_t) -1; }
  शून्य add (अचिन्हित value) { mask |= (uint64_t) 1 << (value & 63); }
  तार्किक may_have (अचिन्हित value) ध्रुव { प्रत्यागम mask & ((uint64_t) 1 << (value & 63)); }

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


स्थिर अन्तःस्थ तार्किक intersects_glyph (ध्रुव hb_set_t *glyphs, अचिन्हित value, ध्रुव शून्य *data HB_UNUSED, शून्य *cache HB_UNUSED)
{
  प्रत्यागम glyphs->has (value);
}
स्थिर अन्तःस्थ तार्किक intersects_class (ध्रुव hb_set_t *glyphs, अचिन्हित value, ध्रुव शून्य *data, शून्य *cache)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *>(data);
  hb_map_t *map = (hb_map_t *) cache;

  hb_codepoint_t *cached_v;
  यदि (map->has (value, &cached_v))
    प्रत्यागम *cached_v;

  तार्किक v = class_def.intersects_class (glyphs, value);
  map->set (value, v);

  प्रत्यागम v;
}
स्थिर अन्तःस्थ तार्किक intersects_coverage (ध्रुव hb_set_t *glyphs, अचिन्हित value, ध्रुव शून्य *data, शून्य *cache HB_UNUSED)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  प्रत्यागम (data+coverage).intersects (glyphs);
}


स्थिर अन्तःस्थ शून्य intersected_glyph (ध्रुव hb_set_t *glyphs HB_UNUSED, ध्रुव शून्य *data, अचिन्हित value, hb_set_t *intersected_glyphs, HB_UNUSED शून्य *cache)
{
  अचिन्हित g = reinterpret_cast<ध्रुव HBUINT16 *>(data)[value];
  intersected_glyphs->add (g);
}

using intersected_class_cache_t = hb_hashmap_t<अचिन्हित, hb_set_t>;

स्थिर अन्तःस्थ शून्य intersected_class_glyphs (ध्रुव hb_set_t *glyphs, ध्रुव शून्य *data, अचिन्हित value, hb_set_t *intersected_glyphs, शून्य *cache)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *>(data);

  intersected_class_cache_t *map = (intersected_class_cache_t *) cache;

  hb_set_t *cached_v;
  यदि (map->has (value, &cached_v))
  {
    intersected_glyphs->union_ (*cached_v);
    प्रत्यागम;
  }

  hb_set_t v;
  class_def.intersected_class_glyphs (glyphs, value, &v);

  intersected_glyphs->union_ (v);

  map->set (value, std::move (v));
}

स्थिर अन्तःस्थ शून्य intersected_coverage_glyphs (ध्रुव hb_set_t *glyphs, ध्रुव शून्य *data, अचिन्हित value, hb_set_t *intersected_glyphs, HB_UNUSED शून्य *cache)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).intersect_set (*glyphs, *intersected_glyphs);
}


स्थिर अन्तःस्थ तार्किक array_is_subset_of (ध्रुव hb_set_t *glyphs,
				       अचिन्हित पूर्णाङ्क गिनती,
				       glyph_data_t values,
				       intersects_func_t intersects_func,
				       ध्रुव शून्य *intersects_data,
				       शून्य *cache)
{
  प्रति (अचिन्हित i = 0; i < गिनती; i++)
    यदि (!intersects_func (glyphs, values[i], intersects_data, cache)) प्रत्यागम असत्य;
  प्रत्यागम सत्य;
}


स्थिर अन्तःस्थ शून्य collect_glyph (hb_set_t *glyphs, अचिन्हित value, ध्रुव शून्य *data HB_UNUSED)
{
  glyphs->add (value);
}
स्थिर अन्तःस्थ शून्य collect_class (hb_set_t *glyphs, अचिन्हित value, ध्रुव शून्य *data)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *>(data);
  class_def.collect_class (glyphs, value);
}
स्थिर अन्तःस्थ शून्य collect_coverage (hb_set_t *glyphs, अचिन्हित value, ध्रुव शून्य *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  (data+coverage).collect_coverage (glyphs);
}

अन्तःस्थ ध्रुव hb_set_t *
hb_depend_context_t::find_or_create_glyph_set (ध्रुव शून्य *data,
                                                अचिन्हित value,
                                                glyph_set_type_t type)
{
  /* Class definitions and coverages are immutable for the lifetime of the
   * context, so their expanded glyph sets can be shared across traversals. */
  glyph_set_cache_कुंजी_t key (data, value, type);
  hb_codepoint_t *existing = nullptr;
  यदि (glyph_set_to_index.has (key, &existing))
    प्रत्यागम glyph_sets[*existing].get ();

  hb_set_t *set = hb_object_create<hb_set_t> ();
  यदि (unlikely (!set))
  {
    depend_data->fail ();
    प्रत्यागम nullptr;
  }

  यदि (type == GLYPH_SET_COVERAGE)
    collect_coverage (set, value, data);
  अन्यथा यदि (type == GLYPH_SET_CLASS)
    collect_class (set, value, data);
  अन्यथा
    set->add (value);
  यदि (unlikely (set->in_error ()))
  {
    hb_set_destroy (set);
    depend_data->fail ();
    प्रत्यागम nullptr;
  }

  hb_codepoint_t index = glyph_sets.length;
  यदि (unlikely (!glyph_sets.धकेलो_or_fail (hb::unique_ptr<hb_set_t> {set})))
  {
    depend_data->fail ();
    प्रत्यागम nullptr;
  }
  यदि (unlikely (!glyph_set_to_index.set (key, index)))
  {
    depend_data->fail ();
    प्रत्यागम nullptr;
  }
  प्रत्यागम glyph_sets[index].get ();
}

अन्तःस्थ hb_codepoint_t
hb_depend_context_t::find_or_create_cached_context_set (ध्रुव hb_set_t *set)
{
  uintptr_t key = (uintptr_t) set;
  hb_codepoint_t *existing = nullptr;
  यदि (cached_context_set_indices.has (key, &existing))
    प्रत्यागम *existing;

  hb_codepoint_t index = depend_data->find_or_create_context_set (*set);
  यदि (unlikely (index == HB_CODEPOINT_INVALID))
    प्रत्यागम HB_CODEPOINT_INVALID;
  यदि (unlikely (!cached_context_set_indices.set (key, index)))
    प्रत्यागम depend_data->fail_invalid ();
  प्रत्यागम index;
}

स्थिर अन्तःस्थ शून्य collect_array (hb_collect_glyphs_context_t *c HB_UNUSED,
				  hb_set_t *glyphs,
				  अचिन्हित पूर्णाङ्क गिनती,
				  glyph_data_t values,
				  collect_glyphs_func_t collect_func,
				  ध्रुव शून्य *collect_data)
{
  प्रति (अचिन्हित i = 0; i < गिनती; i++)
    collect_func (glyphs, values[i], collect_data);
}


स्थिर अन्तःस्थ तार्किक match_always (hb_glyph_info_t &info HB_UNUSED, अचिन्हित value HB_UNUSED, ध्रुव शून्य *data HB_UNUSED)
{
  प्रत्यागम सत्य;
}
स्थिर अन्तःस्थ तार्किक match_glyph (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data HB_UNUSED)
{
  प्रत्यागम info.codepoint == value;
}
स्थिर अन्तःस्थ तार्किक match_class (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *>(data);
  प्रत्यागम class_def.get_class (info.codepoint) == value;
}
स्थिर अन्तःस्थ अचिन्हित get_class_मूल्य (hb_glyph_info_t &info, ध्रुव शून्य *data)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *> (data);
  प्रत्यागम class_def.get_class (info.codepoint);
}
संरचना match_class_cache_data_t
{
  ध्रुव ClassDef *class_def;
  hb_ot_layout_mapping_cache_t *cache;
};
स्थिर अन्तःस्थ तार्किक match_class_cache (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data)
{
  ध्रुव स्वतः &cache_data = *reinterpret_cast<ध्रुव match_class_cache_data_t *> (data);
  प्रत्यागम cache_data.class_def->get_class (info.codepoint, cache_data.cache) == value;
}
स्थिर अन्तःस्थ अचिन्हित get_class_cache_मूल्य (hb_glyph_info_t &info, ध्रुव शून्य *data)
{
  ध्रुव स्वतः &cache_data = *reinterpret_cast<ध्रुव match_class_cache_data_t *> (data);
  प्रत्यागम cache_data.class_def->get_class (info.codepoint, cache_data.cache);
}
स्थिर अन्तःस्थ अचिन्हित get_class_cached (ध्रुव ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = info.syllable();
  यदि (klass < 255)
    प्रत्यागम klass;
  klass = class_def.get_class (info.codepoint);
  यदि (likely (klass < 255))
    info.syllable() = klass;
  प्रत्यागम klass;
}
स्थिर अन्तःस्थ तार्किक match_class_cached (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *>(data);
  प्रत्यागम get_class_cached (class_def, info) == value;
}
स्थिर अन्तःस्थ अचिन्हित get_class_cached_मूल्य (hb_glyph_info_t &info, ध्रुव शून्य *data)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *> (data);
  प्रत्यागम get_class_cached (class_def, info);
}
स्थिर अन्तःस्थ अचिन्हित get_class_cached1 (ध्रुव ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = info.syllable() & 0x0F;
  यदि (klass < 15)
    प्रत्यागम klass;
  klass = class_def.get_class (info.codepoint);
  यदि (likely (klass < 15))
    info.syllable() = (info.syllable() & 0xF0) | klass;
  प्रत्यागम klass;
}
स्थिर अन्तःस्थ तार्किक match_class_cached1 (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *>(data);
  प्रत्यागम get_class_cached1 (class_def, info) == value;
}
स्थिर अन्तःस्थ अचिन्हित get_class_cached2 (ध्रुव ClassDef &class_def, hb_glyph_info_t &info)
{
  अचिन्हित klass = (info.syllable() & 0xF0) >> 4;
  यदि (klass < 15)
    प्रत्यागम klass;
  klass = class_def.get_class (info.codepoint);
  यदि (likely (klass < 15))
    info.syllable() = (info.syllable() & 0x0F) | (klass << 4);
  प्रत्यागम klass;
}
स्थिर अन्तःस्थ तार्किक match_class_cached2 (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *>(data);
  प्रत्यागम get_class_cached2 (class_def, info) == value;
}
स्थिर अन्तःस्थ अचिन्हित get_class_cached2_मूल्य (hb_glyph_info_t &info, ध्रुव शून्य *data)
{
  ध्रुव ClassDef &class_def = *reinterpret_cast<ध्रुव ClassDef *> (data);
  प्रत्यागम get_class_cached2 (class_def, info);
}
स्थिर अन्तःस्थ तार्किक match_coverage (hb_glyph_info_t &info, अचिन्हित value, ध्रुव शून्य *data)
{
  Offset32To<Coverage> coverage;
  coverage = value;
  प्रत्यागम (data+coverage).get_coverage (info.codepoint) != NOT_COVERED;
}

स्थिर अन्तःस्थ तार्किक would_match_निवेश (hb_would_apply_context_t *c,
				      अचिन्हित पूर्णाङ्क गिनती, /* Including the first glyph (not matched) */
				      glyph_data_t निवेश, /* Array of input values--start with second glyph */
				      match_func_t match_func,
				      ध्रुव शून्य *match_data)
{
  यदि (गिनती != c->len)
    प्रत्यागम असत्य;

  प्रति (अचिन्हित पूर्णाङ्क i = 1; i < गिनती; i++)
  {
    hb_glyph_info_t info;
    info.codepoint = c->glyphs[i];
    यदि (likely (!match_func (info, निवेश[i - 1], match_data)))
      प्रत्यागम असत्य;
  }

  प्रत्यागम सत्य;
}
#ifndef HB_OPTIMIZE_SIZE
HB_ALWAYS_INLINE
#अन्यथा
अन्तःस्थ
#endif
स्थिर तार्किक match_निवेश (hb_ot_apply_context_t *c,
			 अचिन्हित पूर्णाङ्क गिनती, /* Including the first glyph (not matched) */
			 glyph_data_t निवेश, /* Array of input values--start with second glyph */
			 match_func_t match_func,
			 ध्रुव शून्य *match_data,
			 अचिन्हित पूर्णाङ्क *end_position,
			 अचिन्हित पूर्णाङ्क *p_total_component_गिनती = nullptr)
{
  TRACE_APPLY (nullptr);

  hb_buffer_t *buffer = c->buffer;

  यदि (गिनती == 1)
  {
    *end_position = buffer->idx + 1;
    c->match_positions[0] = buffer->idx;
    यदि (p_total_component_गिनती)
      *p_total_component_गिनती = hb_glyph_info_get_lig_अंक_comps (&buffer->cur());
    return_trace (सत्य);
  }

  यदि (unlikely (गिनती > HB_MAX_CONTEXT_LENGTH)) return_trace (असत्य);

  स्वतः &skippy_iter = c->iter_निवेश;
  skippy_iter.reset (buffer->idx);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (निवेश);

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

  अचिन्हित पूर्णाङ्क total_component_गिनती = 0;

  अचिन्हित पूर्णाङ्क first_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  अचिन्हित पूर्णाङ्क first_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());

  गणना {
    LIGBASE_NOT_CHECKED,
    LIGBASE_MAY_NOT_SKIP,
    LIGBASE_MAY_SKIP
  } ligbase = LIGBASE_NOT_CHECKED;

  प्रति (अचिन्हित पूर्णाङ्क i = 1; i < गिनती; i++)
  {
    अचिन्हित unsafe_to;
    यदि (!skippy_iter.next (&unsafe_to))
    {
      *end_position = unsafe_to;
      return_trace (असत्य);
    }

    यदि (unlikely (i + 1 > c->match_positions.length &&
		  !c->match_positions.resize_dirty  (i + 1)))
      return_trace (असत्य);
    c->match_positions.arrayZ[i] = skippy_iter.idx;

    अचिन्हित पूर्णाङ्क this_lig_id = _hb_glyph_info_get_lig_id (&buffer->info[skippy_iter.idx]);
    अचिन्हित पूर्णाङ्क this_lig_comp = _hb_glyph_info_get_lig_comp (&buffer->info[skippy_iter.idx]);

    यदि (first_lig_id && first_lig_comp)
    {
      /* If first component was attached to a previous ligature component,
       * all subsequent components should be attached to the same ligature
       * component, otherwise we shouldn't ligate them... */
      यदि (first_lig_id != this_lig_id || first_lig_comp != this_lig_comp)
      {
	/* ...unless, we are attached to a base ligature and that base
	 * ligature is ignorable. */
	यदि (ligbase == LIGBASE_NOT_CHECKED)
	{
	  तार्किक found = असत्य;
	  ध्रुव स्वतः *out = buffer->out_info;
	  अचिन्हित पूर्णाङ्क j = buffer->out_len;
	  यावत् (j && _hb_glyph_info_get_lig_id (&out[j - 1]) == first_lig_id)
	  {
	    यदि (_hb_glyph_info_get_lig_comp (&out[j - 1]) == 0)
	    {
	      j--;
	      found = सत्य;
	      भङ्ग;
	    }
	    j--;
	  }

	  यदि (found && skippy_iter.may_skip (out[j]) == matcher_t::SKIP_YES)
	    ligbase = LIGBASE_MAY_SKIP;
	  अन्यथा
	    ligbase = LIGBASE_MAY_NOT_SKIP;
	}

	यदि (ligbase == LIGBASE_MAY_NOT_SKIP)
	  return_trace (असत्य);
      }
    }
    अन्यथा
    {
      /* If first component was NOT attached to a previous ligature component,
       * all subsequent components should also NOT be attached to any ligature
       * component, unless they are attached to the first component itself! */
      यदि (this_lig_id && this_lig_comp && (this_lig_id != first_lig_id))
	return_trace (असत्य);
    }

    total_component_गिनती += hb_glyph_info_get_lig_अंक_comps_in_ligation (&buffer->info[skippy_iter.idx]);
  }

  *end_position = skippy_iter.idx + 1;

  यदि (p_total_component_गिनती)
  {
    total_component_गिनती += hb_glyph_info_get_lig_अंक_comps (&buffer->cur());
    *p_total_component_गिनती = total_component_गिनती;
  }

  c->match_positions.arrayZ[0] = buffer->idx;

  return_trace (सत्य);
}
स्थिर अन्तःस्थ तार्किक ligate_निवेश (hb_ot_apply_context_t *c,
				 अचिन्हित पूर्णाङ्क गिनती, /* Including the first glyph */
				 अचिन्हित पूर्णाङ्क match_end,
				 hb_codepoint_t lig_glyph,
				 अचिन्हित पूर्णाङ्क total_component_गिनती)
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
  प्रति (अचिन्हित पूर्णाङ्क i = 1; i < गिनती; i++)
    यदि (!_hb_glyph_info_is_mark (&buffer->info[c->match_positions.arrayZ[i]]))
    {
      is_base_ligature = असत्य;
      is_mark_ligature = असत्य;
      भङ्ग;
    }
  तार्किक is_ligature = !is_base_ligature && !is_mark_ligature;

  अचिन्हित पूर्णाङ्क klass = is_ligature ? HB_OT_LAYOUT_GLYPH_PROPS_LIGATURE : 0;
  अचिन्हित पूर्णाङ्क lig_id = is_ligature ? _hb_allocate_lig_id (buffer) : 0;
  अचिन्हित पूर्णाङ्क last_lig_id = _hb_glyph_info_get_lig_id (&buffer->cur());
  अचिन्हित पूर्णाङ्क last_अंक_components = hb_glyph_info_get_lig_अंक_comps (&buffer->cur());
  अचिन्हित पूर्णाङ्क components_so_far = last_अंक_components;

  यदि (is_ligature)
  {
    _hb_glyph_info_set_lig_props_for_ligature (&buffer->cur(), lig_id, total_component_गिनती);
    यदि (_hb_glyph_info_get_general_category (&buffer->cur()) == HB_UNICODE_GENERAL_CATEGORY_NON_SPACING_MARK)
    {
      _hb_glyph_info_set_general_category (&buffer->cur(), HB_UNICODE_GENERAL_CATEGORY_OTHER_LETTER);
    }
  }
  c->replace_glyph_with_ligature (lig_glyph, klass);

  प्रति (अचिन्हित पूर्णाङ्क i = 1; i < गिनती; i++)
  {
    यावत् (buffer->idx < c->match_positions.arrayZ[i] && buffer->successful)
    {
      यदि (is_ligature)
      {
	अचिन्हित पूर्णाङ्क this_comp = _hb_glyph_info_get_lig_comp (&buffer->cur());
	यदि (this_comp == 0)
	  this_comp = last_अंक_components;
	प्रतिज्ञा (components_so_far >= last_अंक_components);
	अचिन्हित पूर्णाङ्क new_lig_comp = components_so_far - last_अंक_components +
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

  यदि (!is_mark_ligature && last_lig_id)
  {
    /* Re-adjust components for any marks following. */
    प्रति (अचिन्हित i = buffer->idx; i < buffer->len; ++i)
    {
      यदि (last_lig_id != _hb_glyph_info_get_lig_id (&buffer->info[i])) भङ्ग;

      अचिन्हित this_comp = _hb_glyph_info_get_lig_comp (&buffer->info[i]);
      यदि (!this_comp) भङ्ग;

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
अन्तःस्थ
#endif
स्थिर तार्किक match_backtrack (hb_ot_apply_context_t *c,
			     अचिन्हित पूर्णाङ्क गिनती,
			     glyph_data_t backtrack,
			     match_func_t match_func,
			     ध्रुव शून्य *match_data,
			     अचिन्हित पूर्णाङ्क *match_start)
{
  TRACE_APPLY (nullptr);

  यदि (!गिनती)
  {
    *match_start = c->buffer->backtrack_len ();
    return_trace (सत्य);
  }

  स्वतः &skippy_iter = c->iter_context;
  skippy_iter.reset_back (c->buffer->backtrack_len ());
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (backtrack);

  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
  {
    अचिन्हित unsafe_from;
    यदि (!skippy_iter.prev (&unsafe_from))
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
अन्तःस्थ
#endif
स्थिर तार्किक match_lookahead (hb_ot_apply_context_t *c,
			     अचिन्हित पूर्णाङ्क गिनती,
			     glyph_data_t lookahead,
			     match_func_t match_func,
			     ध्रुव शून्य *match_data,
			     अचिन्हित पूर्णाङ्क start_index,
			     अचिन्हित पूर्णाङ्क *end_index)
{
  TRACE_APPLY (nullptr);

  यदि (!गिनती)
  {
    *end_index = start_index;
    return_trace (सत्य);
  }

  स्वतः &skippy_iter = c->iter_context;
  प्रतिज्ञा (start_index >= 1);
  skippy_iter.reset (start_index - 1);
  skippy_iter.set_match_func (match_func, match_data);
  skippy_iter.set_glyph_data (lookahead);

  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
  {
    अचिन्हित unsafe_to;
    यदि (!skippy_iter.next (&unsafe_to))
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
		  ध्रुव hb_map_t         *lookup_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->embed (*एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    return_trace (c->check_assign (out->lookupListIndex, lookup_map->get (lookupListIndex), HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  HBUINT16	sequenceIndex;		/* Index into current glyph
					 * sequence--first glyph = 0 */
  HBUINT16	lookupListIndex;	/* Lookup to apply to that
					 * position--zero--based */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (4);
};

स्थिर अन्तःस्थ अचिन्हित serialize_lookuprecord_array (hb_serialize_context_t *c,
						     ध्रुव hb_array_t<ध्रुव LookupRecord> lookupRecords,
						     ध्रुव hb_map_t *lookup_map)
{
  अचिन्हित गिनती = 0;
  प्रति (ध्रुव LookupRecord& r : lookupRecords)
  {
    यदि (!lookup_map->has (r.lookupListIndex))
      अनुवर्त;

    यदि (!r.serialize (c, lookup_map))
      प्रत्यागम 0;

    गिनती++;
  }
  प्रत्यागम गिनती;
}

गणना ContextFormat { SimpleContext = 1, ClassBasedContext = 2, CoverageBasedContext = 3 };

स्थिर अन्तःस्थ ध्रुव hb_set_t *
depend_position_glyphs (hb_depend_context_t *c,
			ContextFormat context_format,
			ध्रुव शून्य *data,
			अचिन्हित value,
			तार्किक filter_class_by_parent,
			hb_set_t *scratch,
			hb_vector_t<अचिन्हित> *compact_workspace = nullptr)
{
  विकल्प (context_format)
  {
    पक्ष ContextFormat::SimpleContext:
      प्रत्यागम c->find_or_create_glyph_set (nullptr, value,
                                           hb_depend_context_t::GLYPH_SET_SINGLE);
    पक्ष ContextFormat::ClassBasedContext:
    {
      ध्रुव hb_set_t *class_set = c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_CLASS);
      यदि (unlikely (!class_set))
	प्रत्यागम nullptr;
      यदि (!filter_class_by_parent)
	प्रत्यागम class_set;
      scratch->set (*class_set);
      scratch->intersect (c->parent_active_glyphs (), compact_workspace);
      भङ्ग;
    }
    पक्ष ContextFormat::CoverageBasedContext:
      प्रत्यागम c->find_or_create_glyph_set (
        data, value, hb_depend_context_t::GLYPH_SET_COVERAGE);
  }

  यदि (unlikely (scratch->in_error ()))
  {
    c->depend_data->fail ();
    प्रत्यागम nullptr;
  }
  प्रत्यागम scratch;
}

स्थिर अन्तःस्थ शून्य context_closure_recurse_lookups (hb_closure_context_t *c,
					     अचिन्हित निवेश_गिनती, glyph_data_t निवेश,
					     अचिन्हित lookup_गिनती_2,
					     ध्रुव LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     अचिन्हित value,
					     ContextFormat context_format,
					     ध्रुव शून्य *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     शून्य *cache)
{
  hb_set_t covered_seq_indicies;
  hb_set_t pos_glyphs;
  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < lookup_गिनती_2; i++)
  {
    अचिन्हित seqIndex = lookupRecord[i].sequenceIndex;
    यदि (seqIndex >= निवेश_गिनती) अनुवर्त;

    तार्किक has_pos_glyphs = असत्य;

    यदि (!covered_seq_indicies.has (seqIndex))
    {
      has_pos_glyphs = सत्य;
      pos_glyphs.clear ();
      यदि (seqIndex == 0)
      {
        विकल्प (context_format) {
        पक्ष ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          भङ्ग;
        पक्ष ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          भङ्ग;
        पक्ष ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          भङ्ग;
        }
      }
      अन्यथा
      {
        ध्रुव शून्य *निवेश_data = निवेश.data;
        अचिन्हित निवेश_मूल्य = seqIndex - 1;
        यदि (context_format != ContextFormat::SimpleContext)
        {
          निवेश_data = data;
          निवेश_मूल्य = निवेश[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, निवेश_data, निवेश_मूल्य, &pos_glyphs, cache);
      }
    }

    covered_seq_indicies.add (seqIndex);
    hb_set_t *cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs))
      प्रत्यागम;
    यदि (has_pos_glyphs) {
      *cur_active_glyphs = std::move (pos_glyphs);
    } अन्यथा {
      *cur_active_glyphs = *c->glyphs;
    }

    अचिन्हित endIndex = निवेश_गिनती;
    यदि (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indicies, seqIndex, endIndex);

    c->निकालो_cur_done_glyphs ();
  }
}

स्थिर तार्किक
context_depend_sets_are_cached (hb_depend_context_t *c,
				अचिन्हित निवेश_गिनती_2,
				अचिन्हित lookup_गिनती,
				ध्रुव LookupRecord lookup_records[],
				ध्रुव शून्य *rule,
				अचिन्हित value,
				ContextFormat context_format,
				ध्रुव शून्य *data,
				hb_vector_t<hb_codepoint_t> *cached_context_sets)
{
  hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
  तार्किक have_active_idx = असत्य;
  प्रति (अचिन्हित i = 0; i < lookup_गिनती; i++)
  {
    यदि (lookup_records[i].sequenceIndex >= निवेश_गिनती_2)
      अनुवर्त;

    यदि (!have_active_idx)
    {
      यदि (unlikely (!c->get_parent_active_glyphs_index (&active_idx)))
	प्रत्यागम असत्य;
      have_active_idx = सत्य;
    }

    hb_depend_context_t::context_set_cache_कुंजी_t key (
      rule, &lookup_records[i], data, active_idx, value, context_format);
    hb_codepoint_t *cached_context_set = nullptr;
    यदि (!c->context_set_cache.has (key, &cached_context_set))
      प्रत्यागम असत्य;
    यदि (unlikely (!cached_context_sets->धकेलो_or_fail (*cached_context_set)))
      प्रत्यागम c->depend_data->fail ();
  }
  /* A rule with no valid records has no cache entries, and its preparation
   * can still allocate dependency sets in an observable order. */
  प्रत्यागम have_active_idx;
}

स्थिर अन्तःस्थ शून्य context_depend_recurse_lookups (hb_depend_context_t *c,
					     अचिन्हित निवेश_गिनती, glyph_data_t निवेश,
					     अचिन्हित lookup_गिनती_2,
					     ध्रुव LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */,
					     ध्रुव शून्य *rule,
					     अचिन्हित value,
					     ContextFormat context_format,
					     ध्रुव शून्य *data,
					     intersected_glyphs_func_t intersected_glyphs_func,
					     शून्य *cache,
					     ध्रुव hb_set_t &preliminary_context,
					     ध्रुव hb_vector_t<ध्रुव hb_set_t *> *निवेश_position_glyphs,
					     ध्रुव hb_vector_t<hb_codepoint_t> *cached_context_sets,
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
  यदि (unlikely (निवेश_गिनती > HB_MAX_CONTEXT_LENGTH))
    प्रत्यागम;

  hb_bit_page_t covered_seq_indices;
  hb_set_t &pos_glyphs = scratch->pos_glyphs;
  hb_set_t &position_context = scratch->position_context;
  hb_set_t &disjunctive_indices = scratch->disjunctive_indices;
  hb_set_t &filtered_disjunctive_indices = scratch->filtered_disjunctive_indices;
  अचिन्हित cached_context_index = 0;
  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < lookup_गिनती_2; i++)
  {
    अचिन्हित seqIndex = lookupRecord[i].sequenceIndex;
    यदि (seqIndex >= निवेश_गिनती)
      अनुवर्त;

    hb_depend_context_t::context_set_cache_कुंजी_t context_cache_कुंजी;
    यदि (unlikely (!c->get_context_set_cache_कुंजी (rule, &lookupRecord[i], data, value,
						 context_format, &context_cache_कुंजी)))
      प्रत्यागम;

    hb_codepoint_t context_set_idx;
    hb_codepoint_t *cached_context_set_idx;
    यदि (cached_context_sets)
      context_set_idx = (*cached_context_sets)[cached_context_index++];
    अन्यथा यदि (c->context_set_cache.has (context_cache_कुंजी, &cached_context_set_idx))
      context_set_idx = *cached_context_set_idx;
    अन्यथा
    {
      यदि (unlikely (!निवेश_position_glyphs))
      {
	c->depend_data->fail ();
	प्रत्यागम;
      }

      /* Build position-specific context_set for this lookup. */
      position_context.clear ();  // Will hold direct glyphs
      disjunctive_indices.clear ();  // Will hold encoded set references
      filtered_disjunctive_indices.clear ();

      /* Process preliminary_context in one pass. */
      hb_codepoint_t elem = HB_SET_मूल्य_INVALID;
      यावत् (preliminary_context.next (&elem)) {
	यदि (!(elem & HB_DEPEND_CONTEXT_SET_FLAG)) {
	  /* Direct glyph */
	  position_context.add (elem);
	} अन्यथा {
	  /* Encoded set reference - defer processing until we have all direct requirements */
	  disjunctive_indices.add (elem);
	}
      }

      /* Add singleton glyphs from OTHER input positions. */
      प्रति (अचिन्हित j = 0; j < निवेश_position_glyphs->length; j++) {
	यदि (j != seqIndex && (*निवेश_position_glyphs)[j]->get_population () == 1) {
	  position_context.add ((*निवेश_position_glyphs)[j]->get_min ());
	}
      }

      /* Now position_context has all direct requirements.
       * Process deferred encoded sets and filter them. */
      elem = HB_SET_मूल्य_INVALID;
      यावत् (disjunctive_indices.next (&elem)) {
	hb_codepoint_t set_idx = elem & 0x7FFFFFFF;
	ध्रुव hb_set_t *original_set = c->depend_data->get_set_from_index (set_idx);
	यदि (unlikely (!original_set)) अनुवर्त;

	/* Dependency context sets are never inverted. */
	यदि (!original_set->intersects (position_context)) {
	  /* No overlap with position_context - add full requirement */
	  filtered_disjunctive_indices.add (elem);
	}
	/* Otherwise: position_context intersects with original_set.
	 * Since disjunctive requirements need only ONE glyph from the set,
	 * and position_context already has a glyph from this set, the
	 * requirement is satisfied - no need to add anything. */
      }

      /* Process non-singleton input positions. */
      प्रति (अचिन्हित j = 0; j < निवेश_position_glyphs->length; j++) {
	यदि (j != seqIndex && (*निवेश_position_glyphs)[j]->get_population () > 1) {
	  /* Input-position sets are also never inverted. */
	  यदि (!(*निवेश_position_glyphs)[j]->intersects (position_context)) {
	    /* No overlap with position_context - add full requirement */
	    तार्किक cached_set = context_format == ContextFormat::CoverageBasedContext ||
			      (context_format == ContextFormat::ClassBasedContext && j != 0);
	    hb_codepoint_t idx = cached_set
	      ? c->find_or_create_cached_context_set ((*निवेश_position_glyphs)[j])
	      : c->depend_data->find_or_create_context_set (*(*निवेश_position_glyphs)[j]);
	    यदि (unlikely (idx == HB_CODEPOINT_INVALID))
	      प्रत्यागम;
	    filtered_disjunctive_indices.add (HB_DEPEND_CONTEXT_SET_FLAG | idx);
	  }
	  /* Otherwise: position_context intersects, requirement satisfied */
	}
      }

      /* Combine direct + disjunctive. */
      position_context.union_ (filtered_disjunctive_indices);
      यदि (unlikely (position_context.in_error () ||
		    filtered_disjunctive_indices.in_error ()))
      {
	c->depend_data->fail ();
	प्रत्यागम;
      }

      context_set_idx = position_context.is_empty ()
	? HB_CODEPOINT_INVALID
	: c->depend_data->find_or_create_context_set (position_context);
      यदि (unlikely (!position_context.is_empty () &&
		    context_set_idx == HB_CODEPOINT_INVALID))
	प्रत्यागम;

      यदि (unlikely (!c->context_set_cache.set (context_cache_कुंजी, context_set_idx)))
      {
	c->depend_data->fail ();
	प्रत्यागम;
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
    यदि (निवेश_गिनती > 1) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_CONTEXT_POSITION;
    }
    यदि (nested_contextual) {
      c->depend_data->current_edge_flags |= HB_SUBSET_DEPEND_EDGE_FLAG_FROM_NESTED_CONTEXT;
    }

    तार्किक has_pos_glyphs = असत्य;

    यदि (!covered_seq_indices.has (seqIndex))
    {
      has_pos_glyphs = सत्य;
      pos_glyphs.clear ();
      यदि (seqIndex == 0)
      {
        विकल्प (context_format) {
        पक्ष ContextFormat::SimpleContext:
          pos_glyphs.add (value);
          भङ्ग;
        पक्ष ContextFormat::ClassBasedContext:
          intersected_glyphs_func (&c->parent_active_glyphs (), data, value, &pos_glyphs, cache);
          भङ्ग;
        पक्ष ContextFormat::CoverageBasedContext:
          pos_glyphs.set (c->parent_active_glyphs ());
          भङ्ग;
        }
      }
      अन्यथा
      {
        ध्रुव शून्य *निवेश_data = निवेश.data;
        अचिन्हित निवेश_मूल्य = seqIndex - 1;
        यदि (context_format != ContextFormat::SimpleContext)
        {
          निवेश_data = data;
          निवेश_मूल्य = निवेश[seqIndex - 1];
        }

        intersected_glyphs_func (c->glyphs, निवेश_data, निवेश_मूल्य, &pos_glyphs, cache);
      }
    }

    covered_seq_indices.add (seqIndex);
    यदि (has_pos_glyphs)
    {
      hb_codepoint_t active_idx = HB_CODEPOINT_INVALID;
      यदि (context_format == ContextFormat::SimpleContext)
      {
	hb_codepoint_t glyph = seqIndex ? pos_glyphs.get_min () : value;
	यदि (likely (glyph < hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG))
	  active_idx = glyph | hb_depend_context_t::SINGLE_GLYPH_ACTIVE_FLAG;
      }
      hb_set_t *cur_active_glyphs = c->धकेलो_cur_active_glyphs (active_idx);
      यदि (unlikely (!cur_active_glyphs))
      {
	c->depend_data->current_context_set_index = saved_context_set_index;
	c->depend_data->current_edge_flags = saved_edge_flags;
	प्रत्यागम;
      }
      *cur_active_glyphs = std::move (pos_glyphs);
    }
    अन्यथा यदि (unlikely (!c->धकेलो_मूल_active_glyphs ()))
    {
      c->depend_data->current_context_set_index = saved_context_set_index;
      c->depend_data->current_edge_flags = saved_edge_flags;
      प्रत्यागम;
    }

    अचिन्हित endIndex = निवेश_गिनती;
    यदि (context_format == ContextFormat::CoverageBasedContext)
      endIndex += 1;

    c->recurse (lookupRecord[i].lookupListIndex, &covered_seq_indices, seqIndex, endIndex);
    c->निकालो_cur_done_glyphs ();

    /* Restore outer context after this lookup, so subsequent lookups in this
     * rule see the original context rather than this lookup's context. */
    c->depend_data->current_context_set_index = saved_context_set_index;
    c->depend_data->current_edge_flags = saved_edge_flags;
  }
}

ढाँचा <typename context_t>
स्थिर अन्तःस्थ शून्य recurse_lookups (context_t *c,
                                    अचिन्हित पूर्णाङ्क lookup_गिनती_2,
                                    ध्रुव LookupRecord lookupRecord[] /* Array of LookupRecords--in design order */)
{
  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < lookup_गिनती_2; i++)
    c->recurse (lookupRecord[i].lookupListIndex);
}

स्थिर अन्तःस्थ शून्य apply_lookup (hb_ot_apply_context_t *c,
				 अचिन्हित पूर्णाङ्क गिनती, /* Including the first glyph */
				 अचिन्हित पूर्णाङ्क lookup_गिनती_2,
				 ध्रुव LookupRecord lookupRecord[], /* Array of LookupRecords--in design order */
				 अचिन्हित पूर्णाङ्क match_end)
{
  hb_buffer_t *buffer = c->buffer;
  पूर्णाङ्क end;

  /* All positions are distance from beginning of *output* buffer.
   * Adjust. */
  {
    अचिन्हित पूर्णाङ्क bl = buffer->backtrack_len ();
    end = bl + match_end - buffer->idx;

    पूर्णाङ्क delta = bl - buffer->idx;
    /* Convert positions to new indexing. */
    प्रति (अचिन्हित पूर्णाङ्क j = 0; j < गिनती; j++)
      c->match_positions.arrayZ[j] += delta;
  }

  प्रति (अचिन्हित पूर्णाङ्क i = 0; i < lookup_गिनती_2 && buffer->successful; i++)
  {
    अचिन्हित पूर्णाङ्क idx = lookupRecord[i].sequenceIndex;
    यदि (idx >= गिनती)
      अनुवर्त;

    अचिन्हित पूर्णाङ्क orig_len = buffer->backtrack_len () + buffer->lookahead_len ();

    /* This can happen if earlier recursed lookups deleted many entries. */
    यदि (unlikely (c->match_positions.arrayZ[idx] >= orig_len))
      अनुवर्त;

    यदि (unlikely (!buffer->move_to (c->match_positions.arrayZ[idx])))
      भङ्ग;

    यदि (unlikely (buffer->max_ops <= 0))
      भङ्ग;

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      यदि (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "start recursing to lookup %u at %u",
			  (अचिन्हित) lookupRecord[i].lookupListIndex,
			  buffer->idx);
    }

    तार्किक ret = c->recurse (lookupRecord[i].lookupListIndex);

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      यदि (buffer->have_output)
        c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "end recursing to lookup %u",
			  (अचिन्हित) lookupRecord[i].lookupListIndex);
    }

    यदि (!ret)
      अनुवर्त;

    अचिन्हित पूर्णाङ्क new_len = buffer->backtrack_len () + buffer->lookahead_len ();
    पूर्णाङ्क delta = new_len - orig_len;

    यदि (!delta)
      अनुवर्त;

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
    यदि (end < पूर्णाङ्क (c->match_positions.arrayZ[idx]))
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

    अचिन्हित पूर्णाङ्क next = idx + 1; /* next now is the position after the recursed lookup. */

    यदि (delta > 0)
    {
      यदि (unlikely (delta + गिनती > HB_MAX_CONTEXT_LENGTH))
	भङ्ग;
      यदि (unlikely (गिनती + delta > c->match_positions.length &&
		    !c->match_positions.resize_dirty  (गिनती + delta)))
        प्रत्यागम;
    }
    अन्यथा
    {
      /* NOTE: delta is non-positive. */
      delta = hb_max (delta, (पूर्णाङ्क) next - (पूर्णाङ्क) गिनती);
      next -= delta;
    }

    /* Shift! */
    स_हटाओ (c->match_positions + next + delta, c->match_positions + next,
	     (गिनती - next) * परिमाण (c->match_positions.arrayZ[0]));
    next += delta;
    गिनती += delta;

    /* Fill in new entries. */
    प्रति (अचिन्हित पूर्णाङ्क j = idx + 1; j < next; j++)
      c->match_positions.arrayZ[j] = c->match_positions.arrayZ[j - 1] + 1;

    /* And fixup the rest. */
    प्रति (; next < गिनती; next++)
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
  ध्रुव शून्य *intersects_data;
  शून्य *intersects_cache;
  शून्य *intersected_glyphs_cache;
};

प्रकारनिर्देश ContextClosureLookupContext ContextDependLookupContext;

संरचना ContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ध्रुव शून्य *collect_data;
};

संरचना ContextApplyLookupContext
{
  ContextApplyFuncs funcs;
  ध्रुव शून्य *match_data;
  get_मूल्य_func_t get_मूल्य;
};

स्थिर अन्तःस्थ तार्किक context_intersects (ध्रुव hb_set_t *glyphs,
				       अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
				       glyph_data_t निवेश, /* Array of input values--start with second glyph */
				       ContextClosureLookupContext &lookup_context)
{
  प्रत्यागम array_is_subset_of (glyphs,
			     निवेश_गिनती ? निवेश_गिनती - 1 : 0, निवेश,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data,
			     lookup_context.intersects_cache);
}

स्थिर अन्तःस्थ शून्य context_closure_lookup (hb_closure_context_t *c,
					   अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
					   glyph_data_t निवेश, /* Array of input values--start with second glyph */
					   अचिन्हित पूर्णाङ्क lookup_गिनती_2,
					   ध्रुव LookupRecord lookupRecord[],
					   अचिन्हित value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					   ContextClosureLookupContext &lookup_context)
{
  यदि (context_intersects (c->glyphs,
			  निवेश_गिनती, निवेश,
			  lookup_context))
    context_closure_recurse_lookups (c,
				     निवेश_गिनती, निवेश,
				     lookup_गिनती_2, lookupRecord,
				     value,
				     lookup_context.context_format,
				     lookup_context.intersects_data,
				     lookup_context.funcs.intersected_glyphs,
				     lookup_context.intersected_glyphs_cache);
}

स्थिर अन्तःस्थ शून्य context_depend_lookup (hb_depend_context_t *c,
					  अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
					  glyph_data_t निवेश, /* Array of input values--start with second glyph */
					  अचिन्हित पूर्णाङ्क lookup_गिनती_2,
					  ध्रुव LookupRecord lookupRecord[],
					  ध्रुव शून्य *rule,
					  अचिन्हित value, /* Index of first glyph in Coverage or Class value in ClassDef table */
					  ContextDependLookupContext &lookup_context)
{
  यदि (!context_intersects (c->glyphs,
			   निवेश_गिनती, निवेश,
			   lookup_context))
    प्रत्यागम;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  यदि (unlikely (!scratch))
    प्रत्यागम;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  तार्किक context_sets_cached = context_depend_sets_are_cached (
    c, निवेश_गिनती, lookup_गिनती_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data,
    &scratch->cached_context_sets);
  यदि (unlikely (!c->depend_data->successful))
    प्रत्यागम;

  /* Build preliminary_context (empty for ContextSubst - no backtrack/lookahead) */
  hb_set_t &preliminary_context = scratch->preliminary_context;

  /* Build glyph sets for ALL input positions */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ध्रुव hb_set_t *> &निवेश_position_glyphs = scratch->निवेश_position_glyphs;

  /* Cached labels no longer need the expanded input-position sets. */
  यदि (!context_sets_cached)
  {
    /* Position 0 (Coverage glyph) — must be in both the class AND the
     * coverage, so intersect with parent_active_glyphs (= coverage ∩ glyphs
     * set by the ContextFormat2/3 caller). */
    ध्रुव hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data, value,
      सत्य, &position_scratch, &scratch->compact_workspace);
    यदि (unlikely (!pos0_glyphs ||
		  !निवेश_position_glyphs.धकेलो_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      प्रत्यागम;
    }

    /* Positions 1+ (Input array) */
    प्रति (अचिन्हित i = 0; i + 1 < निवेश_गिनती; i++)
    {
      ध्रुव hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data,
	निवेश[i], असत्य, &position_scratch);
      यदि (unlikely (!pos_glyphs ||
		    !निवेश_position_glyphs.धकेलो_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	प्रत्यागम;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  निवेश_गिनती, निवेश,
				  lookup_गिनती_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data,
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &निवेश_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

स्थिर अन्तःस्थ शून्य context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
						  अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
						  glyph_data_t निवेश, /* Array of input values--start with second glyph */
						  अचिन्हित पूर्णाङ्क lookup_गिनती_2,
						  ध्रुव LookupRecord lookupRecord[],
						  ContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->निवेश,
		 निवेश_गिनती ? निवेश_गिनती - 1 : 0, निवेश,
		 lookup_context.funcs.collect, lookup_context.collect_data);
  recurse_lookups (c,
		   lookup_गिनती_2, lookupRecord);
}

स्थिर अन्तःस्थ तार्किक context_would_apply_lookup (hb_would_apply_context_t *c,
					       अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
					       glyph_data_t निवेश, /* Array of input values--start with second glyph */
					       अचिन्हित पूर्णाङ्क lookup_गिनती_2 HB_UNUSED,
					       ध्रुव LookupRecord lookupRecord[] HB_UNUSED,
					       ध्रुव ContextApplyLookupContext &lookup_context)
{
  प्रत्यागम would_match_निवेश (c,
			    निवेश_गिनती, निवेश,
			    lookup_context.funcs.match, lookup_context.match_data);
}

स्थिर अन्तःस्थ तार्किक context_apply_lookup (hb_ot_apply_context_t *c,
					 अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
					 glyph_data_t निवेश, /* Array of input values--start with second glyph */
					 अचिन्हित पूर्णाङ्क lookup_गिनती_2,
					 ध्रुव LookupRecord lookupRecord[],
					 ध्रुव ContextApplyLookupContext &lookup_context)
{
  यदि (unlikely (निवेश_गिनती > HB_MAX_CONTEXT_LENGTH)) प्रत्यागम असत्य;

  अचिन्हित match_end = 0;
  तार्किक ret = असत्य;
  यदि (match_निवेश (c,
		   निवेश_गिनती, निवेश,
		   lookup_context.funcs.match, lookup_context.match_data,
		   &match_end))
  {
    c->buffer->unsafe_to_break (c->buffer->idx, match_end);
    apply_lookup (c,
		  निवेश_गिनती,
		  lookup_गिनती_2, lookupRecord,
		  match_end);
    ret = सत्य;
  }
  अन्यथा
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, match_end);
    ret = असत्य;
  }

  प्रत्यागम ret;
}

स्थिर अन्तःस्थ तार्किक context_cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
{
  विकल्प (op)
  {
    पक्ष hb_ot_subtable_cache_op_t::ENTER:
    {
      यदि (!HB_BUFFER_TRY_ALLOCATE_VAR (c->buffer, syllable))
	प्रत्यागम असत्य;
      स्वतः &info = c->buffer->info;
      अचिन्हित गिनती = c->buffer->len;
      प्रति (अचिन्हित i = 0; i < गिनती; i++)
	info[i].syllable() = 255;
      c->new_syllables = 255;
      प्रत्यागम सत्य;
    }
    पक्ष hb_ot_subtable_cache_op_t::LEAVE:
    {
      c->new_syllables = (अचिन्हित) -1;
      HB_BUFFER_DEALLOCATE_VAR (c->buffer, syllable);
      भङ्ग;
    }
  }
  प्रत्यागम असत्य;
}

ढाँचा <typename Types>
संरचना Rule
{
  ढाँचा <typename SetTypes, typename RuleType>
  मित्र संरचना RuleSetOf;

  तार्किक intersects (ध्रुव hb_set_t *glyphs, ContextClosureLookupContext &lookup_context) ध्रुव
  {
    प्रत्यागम context_intersects (glyphs,
			       निवेश_गिनती, निवेश_Z.arrayZ,
			       lookup_context);
  }

  शून्य depend (hb_depend_context_t *c, अचिन्हित value, ContextDependLookupContext &lookup_context) ध्रुव
  {
    ध्रुव स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array ((निवेश_गिनती ? निवेश_गिनती - 1 : 0)));
    context_depend_lookup (c,
			   निवेश_गिनती, निवेश_Z.arrayZ,
			   lookup_गिनती_2, lookupRecord.arrayZ,
			   एतत्,
			   value, lookup_context);
  }

  शून्य closure (hb_closure_context_t *c, अचिन्हित value, ContextClosureLookupContext &lookup_context) ध्रुव
  {
    यदि (unlikely (c->lookup_limit_exceeded ())) प्रत्यागम;

    ध्रुव स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array ((निवेश_गिनती ? निवेश_गिनती - 1 : 0)));
    context_closure_lookup (c,
			    निवेश_गिनती, निवेश_Z.arrayZ,
			    lookup_गिनती_2, lookupRecord.arrayZ,
			    value, lookup_context);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ध्रुव
  {
    यदि (unlikely (c->lookup_limit_exceeded ())) प्रत्यागम;
    यदि (!intersects (c->glyphs, lookup_context)) प्रत्यागम;

    ध्रुव स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array (निवेश_गिनती ? निवेश_गिनती - 1 : 0));
    recurse_lookups (c, lookup_गिनती_2, lookupRecord.arrayZ);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ध्रुव
  {
    ध्रुव स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array (निवेश_गिनती ? निवेश_गिनती - 1 : 0));
    context_collect_glyphs_lookup (c,
				   निवेश_गिनती, निवेश_Z.arrayZ,
				   lookup_गिनती_2, lookupRecord.arrayZ,
				   lookup_context);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest,
			      ध्रुव ClassDef *class_def = nullptr) ध्रुव
  {
    यदि (निवेश_गिनती <= 1)
    {
      *digest = hb_set_digest_t::full ();
      प्रत्यागम;
    }

    अचिन्हित second = निवेश_Z.arrayZ[0];
    यदि (!class_def)
      digest->add (second);
    अन्यथा यदि (!second)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      class_def->collect_class (digest, second);
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
		    ध्रुव ContextApplyLookupContext &lookup_context) ध्रुव
  {
    ध्रुव स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array (निवेश_गिनती ? निवेश_गिनती - 1 : 0));
    प्रत्यागम context_would_apply_lookup (c,
				       निवेश_गिनती, निवेश_Z.arrayZ,
				       lookup_गिनती_2, lookupRecord.arrayZ,
				       lookup_context);
  }

  तार्किक apply (hb_ot_apply_context_t *c,
	      ध्रुव ContextApplyLookupContext &lookup_context) ध्रुव
  {
    TRACE_APPLY (एतत्);
    ध्रुव स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array (निवेश_गिनती ? निवेश_गिनती - 1 : 0));
    return_trace (context_apply_lookup (c, निवेश_गिनती, निवेश_Z.arrayZ, lookup_गिनती_2, lookupRecord.arrayZ, lookup_context));
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव hb_map_t *निवेश_mapping, /* old->new glyphid or class mapping */
		  ध्रुव hb_map_t *lookup_map) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->start_embed (एतत्);
    यदि (unlikely (!c->extend_min (out))) return_trace (असत्य);

    out->निवेश_गिनती = निवेश_गिनती;
    ध्रुव स्वतः निवेश = निवेश_Z.as_array (निवेश_गिनती - 1);
    प्रति (ध्रुव स्वतः org : निवेश)
    {
      typename Types::HBUINT d;
      d = निवेश_mapping->get (org);
      c->copy (d);
    }

    ध्रुव स्वतः &lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>>
					   (निवेश_Z.as_array ((निवेश_गिनती ? निवेश_गिनती - 1 : 0)));

    अचिन्हित गिनती = serialize_lookuprecord_array (c, lookupRecord.as_array (lookup_गिनती_2), lookup_map);
    return_trace (c->check_assign (out->lookup_गिनती_2, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक subset (hb_subset_context_t *c,
	       ध्रुव hb_map_t *lookup_map,
	       ध्रुव hb_map_t *klass_map = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    यदि (unlikely (!निवेश_गिनती)) return_trace (असत्य);
    ध्रुव स्वतः निवेश = निवेश_Z.as_array (निवेश_गिनती - 1);

    ध्रुव hb_map_t *mapping = klass_map == nullptr ? c->plan->glyph_map : klass_map;
    यदि (!hb_all (निवेश, mapping)) return_trace (असत्य);
    return_trace (serialize (c->serializer, mapping, lookup_map));
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  c->check_range (निवेश_Z.arrayZ,
				  निवेश_Z.item_size * (निवेश_गिनती ? निवेश_गिनती - 1 : 0) +
				  LookupRecord::static_size * lookup_गिनती_2));
  }

  रक्षित:
  HBUINT16	निवेश_गिनती;		/* Total number of glyphs in input
					 * glyph sequence--includes the first
					 * glyph */
  HBUINT16	lookup_गिनती_2;		/* Number of LookupRecords */
  UnsizedArrayOf<typename Types::HBUINT>
		निवेश_Z;			/* Array of match inputs--start with
					 * second glyph */
/*UnsizedArrayOf<LookupRecord>
		lookupRecordX;*/	/* Array of LookupRecords--in
					 * design order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, निवेश_Z);
};

ढाँचा <typename Types, typename RuleType>
संरचना RuleSetOf
{
  तार्किक intersects (ध्रुव hb_set_t *glyphs,
		   ContextClosureLookupContext &lookup_context) ध्रुव
  {
    प्रत्यागम
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_map ([&] (ध्रुव RuleType &_) { प्रत्यागम _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  शून्य collect_first_निवेश_classes (hb_ot_layout_ruleset_digest_t *digest) ध्रुव
  {
    digest->init ();
    प्रति (अचिन्हित i = 0; i < rule.len; i++)
    {
      ध्रुव RuleType &r = एतत्+rule.arrayZ[i];
      यदि (r.निवेश_गिनती <= 1)
      {
	digest->set_full ();
	प्रत्यागम;
      }
      digest->add (r.निवेश_Z.arrayZ[0]);
    }
  }

  शून्य depend (hb_depend_context_t *c, अचिन्हित value, ContextDependLookupContext &lookup_context) ध्रुव
  {
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव RuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }

  शून्य closure (hb_closure_context_t *c, अचिन्हित value,
		ContextClosureLookupContext &lookup_context) ध्रुव
  {
    यदि (unlikely (c->lookup_limit_exceeded ())) प्रत्यागम;

    प्रत्यागम
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव RuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c,
                        ContextClosureLookupContext &lookup_context) ध्रुव
  {
    यदि (unlikely (c->lookup_limit_exceeded ())) प्रत्यागम;
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव RuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
		       ContextCollectGlyphsLookupContext &lookup_context) ध्रुव
  {
    प्रत्यागम
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव RuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest,
			      ध्रुव ClassDef *class_def = nullptr) ध्रुव
  {
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([=] (ध्रुव RuleType &_) { _.collect_second_glyphs (digest, class_def); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
		    ध्रुव ContextApplyLookupContext &lookup_context) ध्रुव
  {
    प्रत्यागम
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_map ([&] (ध्रुव RuleType &_) { प्रत्यागम _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c,
	      ध्रुव ContextApplyLookupContext &lookup_context,
	      ध्रुव hb_ot_layout_ruleset_digest_t *digest = nullptr) ध्रुव
  {
    TRACE_APPLY (एतत्);

    अचिन्हित अंक_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    यदि (HB_OPTIMIZE_SIZE_VAL || अंक_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (एतत्))
      | hb_map ([&] (ध्रुव RuleType &_) { प्रत्यागम _.apply (c, lookup_context); })
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
    यदि (likely (matched))
    {
      यदि (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        गच्छ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      यदि (digest && lookup_context.get_मूल्य &&
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
      | hb_map (hb_add (एतत्))
      | hb_filter ([&] (ध्रुव RuleType &_)
		   {
		     यदि (_.निवेश_गिनती <= 1) प्रत्यागम सत्य;
		     unsafe_to_concat = सत्य;
		     प्रत्यागम असत्य;
		   })
      | hb_map ([&] (ध्रुव RuleType &_) { प्रत्यागम _.apply (c, lookup_context); })
      | hb_any
      ;
      यदि (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    यदि (likely (matched))
    {
      यदि (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        गच्छ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    स्वतः match_निवेश = lookup_context.funcs.match;
    स्वतः *निवेश_data = lookup_context.match_data;
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_rules; i++)
    {
      ध्रुव स्वतः &r = एतत्+rule.arrayZ[i];

      ध्रुव स्वतः &निवेश = r.निवेश_Z;

      यदि (r.निवेश_गिनती <= 1 ||
	  (!match_निवेश ||
	   match_निवेश (*first, निवेश.arrayZ[0], निवेश_data)))
      {
        यदि (!second ||
	    (r.निवेश_गिनती <= 2 ||
	     (!match_निवेश ||
	      match_निवेश (*second, निवेश.arrayZ[1], निवेश_data)))
	   )
	{
	  यदि (r.apply (c, lookup_context))
	  {
	    यदि (unsafe_to != (अचिन्हित) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (सत्य);
	  }
	}
	अन्यथा
	  unsafe_to = unsafe_to2;
      }
      अन्यथा
      {
	यदि (unsafe_to == (अचिन्हित) -1)
	  unsafe_to = unsafe_to1;

	// Skip ahead to next possible first glyph match.
	प्रति (; i + 1 < अंक_rules; i++)
	{
	  ध्रुव स्वतः &r2 = एतत्+rule.arrayZ[i + 1];
	  ध्रुव स्वतः &input2 = r2.निवेश_Z;
	  यदि (r2.निवेश_गिनती <= 1 || input2.arrayZ[0] != निवेश.arrayZ[0])
	    भङ्ग;
	}
      }
    }
    यदि (likely (unsafe_to != (अचिन्हित) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (असत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
	       ध्रुव hb_map_t *lookup_map,
	       ध्रुव hb_map_t *klass_map = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    स्वतः snap = c->serializer->snapshot ();
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    प्रति (ध्रुव स्वतः& _ : rule)
    {
      यदि (!_) अनुवर्त;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = out->rule.serialize_append (c->serializer);
      यदि (unlikely (!o)) अनुवर्त;

      यदि (!o->serialize_subset (c, _, एतत्, lookup_map, klass_map))
      {
	out->rule.निकालो ();
	c->serializer->revert (o_snap);
      }
    }

    तार्किक ret = तार्किक (out->rule);
    यदि (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (rule.sanitize (c, एतत्));
  }

  रक्षित:
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

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    प्रत्यागम
    + hb_zip (एतत्+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_map ([&] (ध्रुव RuleSet &_) { प्रत्यागम _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम सत्य; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };

    + hb_zip (एतत्+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      प्रत्यागम c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ध्रुव hb_pair_t<hb_codepoint_t, अचिन्हित> _) { प्रत्यागम hb_pair_t<अचिन्हित, ध्रुव RuleSet&> (_.first, एतत्+ruleSet[_.second]); })
    | hb_apply ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव RuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (), *cur_active_glyphs);

    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (एतत्+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      प्रत्यागम c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ध्रुव hb_pair_t<hb_codepoint_t, अचिन्हित> _) { प्रत्यागम hb_pair_t<अचिन्हित, ध्रुव RuleSet&> (_.first, एतत्+ruleSet[_.second]); })
    | hb_apply ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव RuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव
  {
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      nullptr
    };

    + hb_zip (एतत्+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव RuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    (एतत्+coverage).collect_coverage (c->निवेश);

    संरचना ContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      nullptr
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([digest] (ध्रुव RuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    ध्रुव RuleSet &rule_set = एतत्+ruleSet[(एतत्+coverage).get_coverage (c->glyphs[0])];
    संरचना ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    प्रत्यागम rule_set.would_apply (c, lookup_context);
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
    यदि (likely (index == NOT_COVERED))
      return_trace (असत्य);

    ध्रुव RuleSet &rule_set = एतत्+ruleSet[index];
    संरचना ContextApplyLookupContext lookup_context = {
      {match_glyph},
      nullptr
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    ध्रुव hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, एतत्, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && ruleSet.sanitize (c, एतत्));
  }

  रक्षित:
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

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    यदि (!(एतत्+coverage).intersects (glyphs))
      प्रत्यागम असत्य;

    ध्रुव ClassDef &class_def = एतत्+classDef;

    hb_map_t cache;
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    hb_set_t retained_coverage_glyphs;
    (एतत्+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);


    प्रत्यागम
    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_enumerate
    | hb_map ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव RuleSet &> p)
	      { प्रत्यागम class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम सत्य; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    यदि (!(एतत्+coverage).intersects (c->glyphs))
      प्रत्यागम;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ध्रुव ClassDef &class_def = एतत्+classDef;

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
    { प्रत्यागम class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव typename Types::ढाँचा OffsetTo<RuleSet>&> _)
                {
                  ध्रुव RuleSet& rule_set = एतत्+_.second;
                  rule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    यदि (!(एतत्+coverage).intersects (c->glyphs))
      प्रत्यागम;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ध्रुव ClassDef &class_def = एतत्+classDef;

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
    { प्रत्यागम class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव typename Types::ढाँचा OffsetTo<RuleSet>&> _)
                {
                  ध्रुव RuleSet& rule_set = एतत्+_.second;
                  rule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव
  {
    यदि (!(एतत्+coverage).intersects (c->glyphs))
      प्रत्यागम;

    ध्रुव ClassDef &class_def = एतत्+classDef;

    hb_map_t cache;
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      &class_def,
      &cache
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_enumerate
    | hb_filter ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव RuleSet &> p)
    { प्रत्यागम class_def.intersects_class (c->glyphs, p.first); })
    | hb_map (hb_second)
    | hb_apply ([&] (ध्रुव RuleSet & _)
    { _.closure_lookups (c, lookup_context); });
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    (एतत्+coverage).collect_coverage (c->निवेश);

    ध्रुव ClassDef &class_def = एतत्+classDef;
    संरचना ContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      &class_def
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव RuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  {
    ध्रुव ClassDef &class_def = एतत्+classDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([=, &class_def] (ध्रुव RuleSet &_) { _.collect_second_glyphs (digest, &class_def); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    ध्रुव ClassDef &class_def = एतत्+classDef;
    अचिन्हित पूर्णाङ्क index = class_def.get_class (c->glyphs[0]);
    ध्रुव RuleSet &rule_set = एतत्+ruleSet[index];
    संरचना ContextApplyLookupContext lookup_context = {
      {match_class},
      &class_def
    };
    प्रत्यागम rule_set.would_apply (c, lookup_context);
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  अचिन्हित cache_cost () ध्रुव
  {
    प्रत्यागम (एतत्+classDef).cost ();
  }
  स्थिर तार्किक cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    प्रत्यागम context_cache_func (c, op);
  }

  संरचना external_cache_t
  {
    hb_ot_layout_द्विआधारी_cache_t coverage;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  शून्य *external_cache_create () ध्रुव
  {
    अचिन्हित गिनती = ruleSet.len;
    अचिन्हित size = परिमाण (external_cache_t) -
		    HB_VAR_ARRAY * परिमाण (hb_ot_layout_ruleset_digest_t) +
		    गिनती * परिमाण (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    यदि (likely (cache))
    {
      cache->coverage.clear ();
      प्रति (अचिन्हित i = 0; i < गिनती; i++)
	(एतत्+ruleSet[i]).collect_first_निवेश_classes (&cache->rule_sets[i]);
    }
    प्रत्यागम cache;
  }
  तार्किक apply_cached (hb_ot_apply_context_t *c, शून्य *external_cache) ध्रुव { प्रत्यागम _apply (c, सत्य, external_cache); }
  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) ध्रुव { प्रत्यागम _apply (c, असत्य, external_cache); }
  तार्किक _apply (hb_ot_apply_context_t *c, तार्किक cached, शून्य *external_cache) ध्रुव
  {
    TRACE_APPLY (एतत्);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage_द्विआधारी (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    यदि (index == NOT_COVERED) return_trace (असत्य);

    ध्रुव ClassDef &class_def = एतत्+classDef;

    संरचना ContextApplyLookupContext lookup_context = {
      {cached ? match_class_cached : match_class},
      &class_def,
      cached ? get_class_cached_मूल्य : get_class_मूल्य
    };

    index = cached ? get_class_cached (class_def, c->buffer->cur()) : class_def.get_class (c->buffer->cur().codepoint);
    ध्रुव RuleSet &rule_set = एतत्+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ध्रुव hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#अन्यथा
    ध्रुव hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;
    यदि (unlikely (!out->coverage.serialize_subset (c, coverage, एतत्)))
      return_trace (असत्य);

    hb_map_t klass_map;
    out->classDef.serialize_subset (c, classDef, एतत्, &klass_map);

    ध्रुव hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (एतत्+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (एतत्+classDef).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    ध्रुव hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    तार्किक ret = सत्य;
    पूर्णाङ्क non_zero_index = -1, index = 0;
    स्वतः snapshot = c->serializer->snapshot();
    प्रति (ध्रुव स्वतः& _ : + hb_enumerate (ruleSet)
			 | hb_filter (klass_map, hb_first))
    {
      स्वतः *o = out->ruleSet.serialize_append (c->serializer);
      यदि (unlikely (!o))
      {
	ret = असत्य;
	भङ्ग;
      }

      यदि (coverage_glyph_classes.has (_.first) &&
	  o->serialize_subset (c, _.second, एतत्, lookup_map, &klass_map)) {
	non_zero_index = index;
        snapshot = c->serializer->snapshot();
      }

      index++;
    }

    यदि (!ret || non_zero_index == -1) return_trace (असत्य);

    //prune empty trailing ruleSets
    --index;
    यावत् (index > non_zero_index)
    {
      out->ruleSet.निकालो ();
      index--;
    }
    c->serializer->revert (snapshot);

    return_trace (तार्किक (out->ruleSet));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && classDef.sanitize (c, एतत्) && ruleSet.sanitize (c, एतत्));
  }

  रक्षित:
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

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    यदि (!(एतत्+coverageZ[0]).intersects (glyphs))
      प्रत्यागम असत्य;

    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      एतत्
    };
    प्रत्यागम context_intersects (glyphs,
			       glyph_गिनती_2, coverageZ.arrayZ + 1,
			       lookup_context);
  }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम सत्य; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    यदि (!(एतत्+coverageZ[0]).intersects (c->glyphs))
      प्रत्यागम;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ध्रुव LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    ContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      एतत्,
      nullptr,  /* intersects_cache */
      nullptr   /* intersected_glyphs_cache */
    };
    context_depend_lookup (c,
			   glyph_गिनती_2, (ध्रुव HBUINT16 *) (coverageZ.arrayZ + 1),
			   lookup_गिनती_2, lookupRecord,
			   एतत्,
			   coverageZ[0], lookup_context);

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    यदि (!(एतत्+coverageZ[0]).intersects (c->glyphs))
      प्रत्यागम;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ध्रुव LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    संरचना ContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      एतत्
    };
    context_closure_lookup (c,
			    glyph_गिनती_2, coverageZ.arrayZ + 1,
			    lookup_गिनती_2, lookupRecord,
			    0, lookup_context);

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव
  {
    यदि (!intersects (c->glyphs))
      प्रत्यागम;
    ध्रुव LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    recurse_lookups (c, lookup_गिनती_2, lookupRecord);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    (एतत्+coverageZ[0]).collect_coverage (c->निवेश);

    ध्रुव LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    संरचना ContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      एतत्
    };

    context_collect_glyphs_lookup (c,
				   glyph_गिनती_2, coverageZ.arrayZ + 1,
				   lookup_गिनती_2, lookupRecord,
				   lookup_context);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  {
    यदि (glyph_गिनती_2 <= 1)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      (एतत्+coverageZ[1]).collect_coverage (digest);
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    ध्रुव LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    संरचना ContextApplyLookupContext lookup_context = {
      {match_coverage},
      एतत्
    };
    प्रत्यागम context_would_apply_lookup (c,
				       glyph_गिनती_2, coverageZ.arrayZ + 1,
				       lookup_गिनती_2, lookupRecord,
				       lookup_context);
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverageZ[0]; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    अचिन्हित पूर्णाङ्क index = (एतत्+coverageZ[0]).get_coverage (c->buffer->cur().codepoint);
    यदि (index == NOT_COVERED) return_trace (असत्य);

    ध्रुव LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    संरचना ContextApplyLookupContext lookup_context = {
      {match_coverage},
      एतत्
    };
    return_trace (context_apply_lookup (c,
					glyph_गिनती_2,
					coverageZ.arrayZ + 1,
					lookup_गिनती_2, lookupRecord,
					lookup_context));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    out->format = format;
    out->glyph_गिनती_2 = glyph_गिनती_2;

    स्वतः coverages = coverageZ.as_array (glyph_गिनती_2);

    प्रति (ध्रुव स्वतः& offset : coverages)
    {
      /* TODO(subset) This looks like should not be necessary to write this way. */
      स्वतः *o = c->serializer->allocate_size<typename Types::ढाँचा OffsetTo<Coverage>> (Types::Offset::static_size);
      यदि (unlikely (!o)) return_trace (असत्य);
      यदि (!o->serialize_subset (c, offset, एतत्)) return_trace (असत्य);
    }

    ध्रुव स्वतः& lookupRecord = StructAfter<UnsizedArrayOf<LookupRecord>> (coverageZ.as_array (glyph_गिनती_2));
    ध्रुव hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;


    अचिन्हित गिनती = serialize_lookuprecord_array (c->serializer, lookupRecord.as_array (lookup_गिनती_2), lookup_map);
    return_trace (c->serializer->check_assign (out->lookup_गिनती_2, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्))) return_trace (असत्य);
    hb_barrier ();
    अचिन्हित पूर्णाङ्क गिनती = glyph_गिनती_2;
    यदि (unlikely (!गिनती)) return_trace (असत्य); /* We want to access coverageZ[0] freely. */
    यदि (unlikely (!c->check_array (coverageZ.arrayZ, गिनती))) return_trace (असत्य);
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < गिनती; i++)
      यदि (unlikely (!coverageZ[i].sanitize (c, एतत्))) return_trace (असत्य);
    ध्रुव LookupRecord *lookupRecord = &StructAfter<LookupRecord> (coverageZ.as_array (glyph_गिनती_2));
    return_trace (likely (c->check_array (lookupRecord, lookup_गिनती_2)));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier--format = 3 */
  HBUINT16	glyph_गिनती_2;		/* Number of glyphs in the input glyph
					 * sequence */
  HBUINT16	lookup_गिनती_2;		/* Number of LookupRecords */
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
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    पक्ष 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    पक्ष 6: hb_barrier (); return_trace (c->dispatch (u.format6, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मूल्य ());
    }
  }

  रक्षित:
  सङ्घ {
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
  ध्रुव शून्य *intersects_data[3];
  शून्य *intersects_cache[3];
  शून्य *intersected_glyphs_cache;
};

प्रकारनिर्देश ChainContextClosureLookupContext ChainContextDependLookupContext;

संरचना ChainContextCollectGlyphsLookupContext
{
  ContextCollectGlyphsFuncs funcs;
  ध्रुव शून्य *collect_data[3];
};

संरचना ChainContextApplyLookupContext
{
  ChainContextApplyFuncs funcs;
  ध्रुव शून्य *match_data[3];
  get_मूल्य_func_t get_निवेश_मूल्य;
};

स्थिर अन्तःस्थ तार्किक chain_context_intersects (ध्रुव hb_set_t *glyphs,
					     अचिन्हित पूर्णाङ्क backtrack_गिनती,
					     glyph_data_t backtrack,
					     अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
					     glyph_data_t निवेश, /* Array of input values--start with second glyph */
					     अचिन्हित पूर्णाङ्क lookahead_गिनती,
					     glyph_data_t lookahead,
					     ChainContextClosureLookupContext &lookup_context)
{
  तार्किक bt_ok = array_is_subset_of (glyphs,
			     backtrack_गिनती, backtrack,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[0],
			     lookup_context.intersects_cache[0]);
  तार्किक in_ok = array_is_subset_of (glyphs,
			     निवेश_गिनती ? निवेश_गिनती - 1 : 0, निवेश,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[1],
			     lookup_context.intersects_cache[1]);
  तार्किक la_ok = array_is_subset_of (glyphs,
			     lookahead_गिनती, lookahead,
			     lookup_context.funcs.intersects,
			     lookup_context.intersects_data[2],
			     lookup_context.intersects_cache[2]);

  प्रत्यागम bt_ok && in_ok && la_ok;
}

स्थिर अन्तःस्थ शून्य chain_context_closure_lookup (hb_closure_context_t *c,
						 अचिन्हित पूर्णाङ्क backtrack_गिनती,
						 glyph_data_t backtrack,
						 अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
						 glyph_data_t निवेश, /* Array of input values--start with second glyph */
						 अचिन्हित पूर्णाङ्क lookahead_गिनती,
						 glyph_data_t lookahead,
						 अचिन्हित पूर्णाङ्क lookup_गिनती_2,
						 ध्रुव LookupRecord lookupRecord[],
						 अचिन्हित value,
						 ChainContextClosureLookupContext &lookup_context)
{
  यदि (chain_context_intersects (c->glyphs,
				backtrack_गिनती, backtrack,
				निवेश_गिनती, निवेश,
				lookahead_गिनती, lookahead,
				lookup_context))
    context_closure_recurse_lookups (c,
		     निवेश_गिनती, निवेश,
		     lookup_गिनती_2, lookupRecord,
		     value,
		     lookup_context.context_format,
		     lookup_context.intersects_data[1],
		     lookup_context.funcs.intersected_glyphs,
		     lookup_context.intersected_glyphs_cache);
}

स्थिर अन्तःस्थ शून्य chain_context_depend_lookup (hb_depend_context_t *c,
						अचिन्हित पूर्णाङ्क backtrack_गिनती,
						glyph_data_t backtrack,
						अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
						glyph_data_t निवेश, /* Array of input values--start with second glyph */
						अचिन्हित पूर्णाङ्क lookahead_गिनती,
						glyph_data_t lookahead,
						अचिन्हित पूर्णाङ्क lookup_गिनती_2,
						ध्रुव LookupRecord lookupRecord[],
						ध्रुव शून्य *rule,
						अचिन्हित value,
						ChainContextDependLookupContext &lookup_context)
{
  यदि (!chain_context_intersects (c->glyphs,
				 backtrack_गिनती, backtrack,
				 निवेश_गिनती, निवेश,
				 lookahead_गिनती, lookahead,
				 lookup_context))
    प्रत्यागम;

  hb_depend_context_t::depend_scratch_t *scratch = c->acquire_depend_scratch ();
  यदि (unlikely (!scratch))
    प्रत्यागम;
  HB_SCOPE_GUARD (c->release_depend_scratch (scratch));

  तार्किक context_sets_cached = context_depend_sets_are_cached (
    c, निवेश_गिनती, lookup_गिनती_2, lookupRecord, rule, value,
    lookup_context.context_format, lookup_context.intersects_data[1],
    &scratch->cached_context_sets);
  यदि (unlikely (!c->depend_data->successful))
    प्रत्यागम;
  यदि (context_sets_cached)
  {
    context_depend_recurse_lookups (c,
				    निवेश_गिनती, निवेश,
				    lookup_गिनती_2, lookupRecord,
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
    प्रत्यागम;
  }

  /* Cache misses and rules without valid records still perform the original
   * dependency-set interning in its observable order. */
  hb_set_t &position_scratch = scratch->position_scratch;
  hb_vector_t<ध्रुव hb_set_t *> &backtrack_sets = scratch->backtrack_sets;
  hb_vector_t<ध्रुव hb_set_t *> &lookahead_sets = scratch->lookahead_sets;

  /* Extract backtrack glyphs */
  प्रति (अचिन्हित i = 0; i < backtrack_गिनती; i++)
  {
    ध्रुव hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[0],
      backtrack[i], असत्य, &position_scratch);
    यदि (unlikely (!pos_glyphs))
      प्रत्यागम;
    यदि (!pos_glyphs->is_empty ())
    {
      यदि (unlikely (!backtrack_sets.धकेलो_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        प्रत्यागम;
      }
    }
  }

  /* Extract lookahead glyphs */
  प्रति (अचिन्हित i = 0; i < lookahead_गिनती; i++)
  {
    ध्रुव hb_set_t *pos_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[2],
      lookahead[i], असत्य, &position_scratch);
    यदि (unlikely (!pos_glyphs))
      प्रत्यागम;
    यदि (!pos_glyphs->is_empty ())
    {
      यदि (unlikely (!lookahead_sets.धकेलो_or_fail (pos_glyphs)))
      {
        c->depend_data->fail ();
        प्रत्यागम;
      }
    }
  }

  /* Build preliminary_context with backtrack + lookahead encoded */
  hb_set_t &preliminary_context = scratch->preliminary_context;
  प्रति (ध्रुव hb_set_t *back_set : backtrack_sets)
  {
    यदि (back_set->get_population () == 1) {
      preliminary_context.add (back_set->get_min ());  // Direct glyph
    } अन्यथा यदि (back_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*back_set)
        : c->find_or_create_cached_context_set (back_set);
      यदि (unlikely (set_idx == HB_CODEPOINT_INVALID))
        प्रत्यागम;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  प्रति (ध्रुव hb_set_t *look_set : lookahead_sets)
  {
    यदि (look_set->get_population () == 1) {
      preliminary_context.add (look_set->get_min ());  // Direct glyph
    } अन्यथा यदि (look_set->get_population () > 1) {
      hb_codepoint_t set_idx = lookup_context.context_format == ContextFormat::SimpleContext
        ? c->depend_data->find_or_create_context_set (*look_set)
        : c->find_or_create_cached_context_set (look_set);
      यदि (unlikely (set_idx == HB_CODEPOINT_INVALID))
        प्रत्यागम;
      preliminary_context.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);  // Encoded set reference
    }
  }
  यदि (unlikely (preliminary_context.in_error ()))
  {
    c->depend_data->fail ();
    प्रत्यागम;
  }

  /* Build glyph sets for ALL input positions */
  hb_vector_t<ध्रुव hb_set_t *> &निवेश_position_glyphs = scratch->निवेश_position_glyphs;

  यदि (!context_sets_cached)
  {
    /* Position 0 (first input coverage/class/glyph) — must be in both the
     * class AND the coverage, so intersect with parent_active_glyphs. */
    ध्रुव hb_set_t *pos0_glyphs = depend_position_glyphs (
      c, lookup_context.context_format, lookup_context.intersects_data[1], value,
      सत्य, &position_scratch, &scratch->compact_workspace);
    यदि (unlikely (!pos0_glyphs ||
		  !निवेश_position_glyphs.धकेलो_or_fail (pos0_glyphs)))
    {
      c->depend_data->fail ();
      प्रत्यागम;
    }

    /* Positions 1+ (Input array) */
    प्रति (अचिन्हित i = 0; i + 1 < निवेश_गिनती; i++)
    {
      ध्रुव hb_set_t *pos_glyphs = depend_position_glyphs (
	c, lookup_context.context_format, lookup_context.intersects_data[1],
	निवेश[i], असत्य, &position_scratch);
      यदि (unlikely (!pos_glyphs ||
		    !निवेश_position_glyphs.धकेलो_or_fail (pos_glyphs)))
      {
	c->depend_data->fail ();
	प्रत्यागम;
      }
    }
  }

  context_depend_recurse_lookups (c,
				  निवेश_गिनती, निवेश,
				  lookup_गिनती_2, lookupRecord,
				  rule,
				  value,
				  lookup_context.context_format,
				  lookup_context.intersects_data[1],
				  lookup_context.funcs.intersected_glyphs,
				  lookup_context.intersected_glyphs_cache,
				  preliminary_context,
				  context_sets_cached ? nullptr : &निवेश_position_glyphs,
				  context_sets_cached ? &scratch->cached_context_sets : nullptr,
				  scratch);
}

स्थिर अन्तःस्थ शून्य chain_context_collect_glyphs_lookup (hb_collect_glyphs_context_t *c,
							अचिन्हित पूर्णाङ्क backtrack_गिनती,
							glyph_data_t backtrack,
							अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
							glyph_data_t निवेश, /* Array of input values--start with second glyph */
							अचिन्हित पूर्णाङ्क lookahead_गिनती,
							glyph_data_t lookahead,
							अचिन्हित पूर्णाङ्क lookup_गिनती_2,
							ध्रुव LookupRecord lookupRecord[],
							ChainContextCollectGlyphsLookupContext &lookup_context)
{
  collect_array (c, c->before,
		 backtrack_गिनती, backtrack,
		 lookup_context.funcs.collect, lookup_context.collect_data[0]);
  collect_array (c, c->निवेश,
		 निवेश_गिनती ? निवेश_गिनती - 1 : 0, निवेश,
		 lookup_context.funcs.collect, lookup_context.collect_data[1]);
  collect_array (c, c->after,
		 lookahead_गिनती, lookahead,
		 lookup_context.funcs.collect, lookup_context.collect_data[2]);
  recurse_lookups (c,
		   lookup_गिनती_2, lookupRecord);
}

स्थिर अन्तःस्थ तार्किक chain_context_would_apply_lookup (hb_would_apply_context_t *c,
						     अचिन्हित पूर्णाङ्क backtrack_गिनती,
						     glyph_data_t backtrack HB_UNUSED,
						     अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
						     glyph_data_t निवेश, /* Array of input values--start with second glyph */
						     अचिन्हित पूर्णाङ्क lookahead_गिनती,
						     glyph_data_t lookahead HB_UNUSED,
						     अचिन्हित पूर्णाङ्क lookup_गिनती_2 HB_UNUSED,
						     ध्रुव LookupRecord lookupRecord[] HB_UNUSED,
						     ध्रुव ChainContextApplyLookupContext &lookup_context)
{
  प्रत्यागम (c->zero_context ? !backtrack_गिनती && !lookahead_गिनती : सत्य)
      && would_match_निवेश (c,
			    निवेश_गिनती, निवेश,
			    lookup_context.funcs.match[1], lookup_context.match_data[1]);
}

स्थिर अन्तःस्थ तार्किक chain_context_apply_lookup (hb_ot_apply_context_t *c,
					       अचिन्हित पूर्णाङ्क backtrack_गिनती,
					       glyph_data_t backtrack,
					       अचिन्हित पूर्णाङ्क निवेश_गिनती, /* Including the first glyph (not matched) */
					       glyph_data_t निवेश, /* Array of input values--start with second glyph */
					       अचिन्हित पूर्णाङ्क lookahead_गिनती,
					       glyph_data_t lookahead,
					       अचिन्हित पूर्णाङ्क lookup_गिनती_2,
					       ध्रुव LookupRecord lookupRecord[],
					       ध्रुव ChainContextApplyLookupContext &lookup_context)
{
  यदि (unlikely (निवेश_गिनती > HB_MAX_CONTEXT_LENGTH)) प्रत्यागम असत्य;

  अचिन्हित start_index = c->buffer->out_len;
  अचिन्हित end_index = c->buffer->idx;
  अचिन्हित match_end = 0;
  तार्किक ret = सत्य;
  यदि (!(match_निवेश (c,
		     निवेश_गिनती, निवेश,
		     lookup_context.funcs.match[1], lookup_context.match_data[1],
		     &match_end) && (end_index = match_end)
       && match_lookahead (c,
			   lookahead_गिनती, lookahead,
			   lookup_context.funcs.match[2], lookup_context.match_data[2],
			   match_end, &end_index)))
  {
    c->buffer->unsafe_to_concat (c->buffer->idx, end_index);
    प्रत्यागम असत्य;
  }

  यदि (!match_backtrack (c,
			backtrack_गिनती, backtrack,
			lookup_context.funcs.match[0], lookup_context.match_data[0],
			&start_index))
  {
    c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
    प्रत्यागम असत्य;
  }

  c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);
  apply_lookup (c,
		निवेश_गिनती,
		lookup_गिनती_2, lookupRecord,
		match_end);

  प्रत्यागम ret;
}

ढाँचा <typename Types>
संरचना ChainRule
{
  ढाँचा <typename SetTypes, typename ChainRuleType>
  मित्र संरचना ChainRuleSetOf;

  तार्किक intersects (ध्रुव hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    प्रत्यागम chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     निवेश.lenP1, निवेश.arrayZ,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  शून्य depend (hb_depend_context_t *c, अचिन्हित value, ChainContextDependLookupContext &lookup_context) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_depend_lookup (c,
				 backtrack.len, backtrack.arrayZ,
				 निवेश.lenP1, निवेश.arrayZ,
				 lookahead.len, lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 एतत्,
				 value,
				 lookup_context);
  }

  शून्य closure (hb_closure_context_t *c, अचिन्हित value,
		ChainContextClosureLookupContext &lookup_context) ध्रुव
  {
    यदि (unlikely (c->lookup_limit_exceeded ())) प्रत्यागम;

    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  निवेश.lenP1, निवेश.arrayZ,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  value,
				  lookup_context);
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ध्रुव
  {
    यदि (unlikely (c->lookup_limit_exceeded ())) प्रत्यागम;
    यदि (!intersects (c->glyphs, lookup_context)) प्रत्यागम;

    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c,
		       ChainContextCollectGlyphsLookupContext &lookup_context) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 निवेश.lenP1, निवेश.arrayZ,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest,
			      ध्रुव ClassDef *निवेश_class_def = nullptr,
			      ध्रुव ClassDef *lookahead_class_def = nullptr) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);

    अचिन्हित second;
    ध्रुव ClassDef *class_def;
    यदि (निवेश.lenP1 > 1)
    {
      second = निवेश.arrayZ[0];
      class_def = निवेश_class_def;
    }
    अन्यथा यदि (lookahead.len)
    {
      second = lookahead.arrayZ[0];
      class_def = lookahead_class_def;
    }
    अन्यथा
    {
      *digest = hb_set_digest_t::full ();
      प्रत्यागम;
    }

    यदि (!class_def)
      digest->add (second);
    अन्यथा यदि (!second)
      *digest = hb_set_digest_t::full ();
    अन्यथा
      class_def->collect_class (digest, second);
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
		    ध्रुव ChainContextApplyLookupContext &lookup_context) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    प्रत्यागम chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     निवेश.lenP1, निवेश.arrayZ,
					     lookahead.len, lookahead.arrayZ, lookup.len,
					     lookup.arrayZ, lookup_context);
  }

  तार्किक apply (hb_ot_apply_context_t *c,
	      ध्रुव ChainContextApplyLookupContext &lookup_context) ध्रुव
  {
    TRACE_APPLY (एतत्);
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      निवेश.lenP1, निवेश.arrayZ,
					      lookahead.len, lookahead.arrayZ, lookup.len,
					      lookup.arrayZ, lookup_context));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize_array (hb_serialize_context_t *c,
			HBUINT16 len,
			Iterator it) ध्रुव
  {
    c->copy (len);
    प्रति (ध्रुव स्वतः g : it)
    {
      typename Types::HBUINT d;
      d = g;
      c->copy (d);
    }
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  ध्रुव hb_map_t *lookup_map,
		  ध्रुव hb_map_t *backtrack_map,
		  ध्रुव hb_map_t *निवेश_map = nullptr,
		  ध्रुव hb_map_t *lookahead_map = nullptr) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    ध्रुव hb_map_t *mapping = backtrack_map;
    serialize_array (c, backtrack.len, + backtrack.iter ()
				       | hb_map (mapping));

    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    यदि (निवेश_map) mapping = निवेश_map;
    serialize_array (c, निवेश.lenP1, + निवेश.iter ()
				     | hb_map (mapping));

    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    यदि (lookahead_map) mapping = lookahead_map;
    serialize_array (c, lookahead.len, + lookahead.iter ()
				       | hb_map (mapping));

    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);

    HBUINT16* lookup_गिनती_2 = c->embed (&(lookup.len));
    यदि (!lookup_गिनती_2) return_trace (असत्य);

    अचिन्हित गिनती = serialize_lookuprecord_array (c, lookup.as_array (), lookup_map);
    return_trace (c->check_assign (*lookup_गिनती_2, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक subset (hb_subset_context_t *c,
	       ध्रुव hb_map_t *lookup_map,
	       ध्रुव hb_map_t *backtrack_map = nullptr,
	       ध्रुव hb_map_t *निवेश_map = nullptr,
	       ध्रुव hb_map_t *lookahead_map = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);

    यदि (!backtrack_map)
    {
      ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
      यदि (!hb_all (backtrack, glyphset) ||
	  !hb_all (निवेश, glyphset) ||
	  !hb_all (lookahead, glyphset))
	return_trace (असत्य);

      serialize (c->serializer, lookup_map, c->plan->glyph_map);
    }
    अन्यथा
    {
      यदि (!hb_all (backtrack, backtrack_map) ||
	  !hb_all (निवेश, निवेश_map) ||
	  !hb_all (lookahead, lookahead_map))
	return_trace (असत्य);

      serialize (c->serializer, lookup_map, backtrack_map, निवेश_map, lookahead_map);
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    /* Hyper-optimized sanitized because this is really hot. */
    यदि (unlikely (!backtrack.len.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    यदि (unlikely (!निवेश.lenP1.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    यदि (unlikely (!lookahead.len.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  रक्षित:
  Array16Of<typename Types::HBUINT>
		backtrack;		/* Array of backtracking values
					 * (to be matched before the input
					 * sequence) */
  HeadlessArray16Of<typename Types::HBUINT>
		निवेश_X;			/* Array of input values (start with
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
  तार्किक intersects (ध्रुव hb_set_t *glyphs, ChainContextClosureLookupContext &lookup_context) ध्रुव
  {
    प्रत्यागम
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_map ([&] (ध्रुव ChainRuleType &_) { प्रत्यागम _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  शून्य collect_first_निवेश_classes (hb_ot_layout_ruleset_digest_t *digest) ध्रुव
  {
    digest->init ();
    प्रति (अचिन्हित i = 0; i < rule.len; i++)
    {
      ध्रुव ChainRuleType &r = एतत्+rule.arrayZ[i];
      ध्रुव स्वतः &निवेश = StructAfter<decltype (r.निवेश_X)> (r.backtrack);
      यदि (निवेश.lenP1 <= 1)
      {
	digest->set_full ();
	प्रत्यागम;
      }
      digest->add (निवेश.arrayZ[0]);
    }
  }
  शून्य depend (hb_depend_context_t *c, अचिन्हित value, ChainContextDependLookupContext &lookup_context) ध्रुव
  {
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव ChainRuleType &_) { _.depend (c, value, lookup_context); })
    ;
  }
  शून्य closure (hb_closure_context_t *c, अचिन्हित value, ChainContextClosureLookupContext &lookup_context) ध्रुव
  {
    यदि (unlikely (c->lookup_limit_exceeded ())) प्रत्यागम;

    प्रत्यागम
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव ChainRuleType &_) { _.closure (c, value, lookup_context); })
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c,
                        ChainContextClosureLookupContext &lookup_context) ध्रुव
  {
    यदि (unlikely (c->lookup_limit_exceeded ())) प्रत्यागम;

    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव ChainRuleType &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c, ChainContextCollectGlyphsLookupContext &lookup_context) ध्रुव
  {
    प्रत्यागम
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव ChainRuleType &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest,
			      ध्रुव ClassDef *निवेश_class_def = nullptr,
			      ध्रुव ClassDef *lookahead_class_def = nullptr) ध्रुव
  {
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_apply ([=] (ध्रुव ChainRuleType &_)
		{ _.collect_second_glyphs (digest, निवेश_class_def, lookahead_class_def); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c,
		    ध्रुव ChainContextApplyLookupContext &lookup_context) ध्रुव
  {
    प्रत्यागम
    + hb_iter (rule)
    | hb_map (hb_add (एतत्))
    | hb_map ([&] (ध्रुव ChainRuleType &_) { प्रत्यागम _.would_apply (c, lookup_context); })
    | hb_any
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c,
	      ध्रुव ChainContextApplyLookupContext &lookup_context,
	      ध्रुव hb_ot_layout_ruleset_digest_t *digest = nullptr) ध्रुव
  {
    TRACE_APPLY (एतत्);

    अचिन्हित अंक_rules = rule.len;

#ifndef HB_NO_OT_RULESETS_FAST_PATH
    यदि (HB_OPTIMIZE_SIZE_VAL || अंक_rules <= 4)
#endif
    {
    slow:
      return_trace (
      + hb_iter (rule)
      | hb_map (hb_add (एतत्))
      | hb_map ([&] (ध्रुव ChainRuleType &_) { प्रत्यागम _.apply (c, lookup_context); })
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
    यदि (likely (matched))
    {
      यदि (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        गच्छ slow;
      }

      first = &c->buffer->info[skippy_iter.idx];
      unsafe_to1 = skippy_iter.idx + 1;

      यदि (digest && lookup_context.get_निवेश_मूल्य &&
	  !digest->may_have (lookup_context.get_निवेश_मूल्य (*first, lookup_context.match_data[1])))
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
      | hb_map (hb_add (एतत्))
      | hb_filter ([&] (ध्रुव ChainRuleType &_)
		   {
		     ध्रुव स्वतः &निवेश = StructAfter<decltype (_.निवेश_X)> (_.backtrack);
		     ध्रुव स्वतः &lookahead = StructAfter<decltype (_.lookaheadX)> (निवेश);
		     यदि (निवेश.lenP1 <= 1 && lookahead.len == 0) प्रत्यागम सत्य;
		     unsafe_to_concat = सत्य;
		     प्रत्यागम असत्य;
		   })
      | hb_map ([&] (ध्रुव ChainRuleType &_) { प्रत्यागम _.apply (c, lookup_context); })
      | hb_any
      ;
      यदि (unsafe_to_concat)
	c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to1);
      return_trace (ret);
    }
    matched = skippy_iter.next ();
    यदि (likely (matched))
    {
      यदि (skippy_iter.may_skip (c->buffer->info[skippy_iter.idx]))
      {
	/* Can't use the fast path if eg. the next char is a default-ignorable
	 * or other skippable. */
        गच्छ slow;
      }

      second = &c->buffer->info[skippy_iter.idx];
      unsafe_to2 = skippy_iter.idx + 1;
    }

    स्वतः match_निवेश = lookup_context.funcs.match[1];
    स्वतः match_lookahead = lookup_context.funcs.match[2];
    स्वतः *निवेश_data = lookup_context.match_data[1];
    स्वतः *lookahead_data = lookup_context.match_data[2];
    प्रति (अचिन्हित पूर्णाङ्क i = 0; i < अंक_rules; i++)
    {
      ध्रुव स्वतः &r = एतत्+rule.arrayZ[i];

      ध्रुव स्वतः &निवेश = StructAfter<decltype (r.निवेश_X)> (r.backtrack);
      ध्रुव स्वतः &lookahead = StructAfter<decltype (r.lookaheadX)> (निवेश);

      अचिन्हित lenP1 = निवेश.lenP1;
      यदि (lenP1 > 1 ?
	   (!match_निवेश ||
	    match_निवेश (*first, निवेश.arrayZ[0], निवेश_data))
	  :
	   (!lookahead.len || !match_lookahead ||
	    match_lookahead (*first, lookahead.arrayZ[0], lookahead_data)))
      {
	lenP1 = hb_max (lenP1, 1u);
        यदि (!second ||
	    (lenP1 > 2 ?
	     (!match_निवेश ||
	      match_निवेश (*second, निवेश.arrayZ[1], निवेश_data))
	     :
	     (lookahead.len <= 2 - lenP1 || !match_lookahead ||
	      match_lookahead (*second, lookahead.arrayZ[2 - lenP1], lookahead_data))))
	{
	  यदि (r.apply (c, lookup_context))
	  {
	    यदि (unsafe_to != (अचिन्हित) -1)
	      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);
	    return_trace (सत्य);
	  }
	}
	अन्यथा
	  unsafe_to = unsafe_to2;
      }
      अन्यथा
      {
	यदि (unsafe_to == (अचिन्हित) -1)
	  unsafe_to = unsafe_to1;

	यदि (lenP1 > 1)
	{
	  // Skip ahead to next possible first glyph match.
	  प्रति (; i + 1 < अंक_rules; i++)
	  {
	    ध्रुव स्वतः &r2 = एतत्+rule.arrayZ[i + 1];
	    ध्रुव स्वतः &input2 = StructAfter<decltype (r2.निवेश_X)> (r2.backtrack);
	    यदि (input2.lenP1 <= 1 || input2.arrayZ[0] != निवेश.arrayZ[0])
	      भङ्ग;
	  }
	}
      }
    }
    यदि (likely (unsafe_to != (अचिन्हित) -1))
      c->buffer->unsafe_to_concat (c->buffer->idx, unsafe_to);

    return_trace (असत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
	       ध्रुव hb_map_t *lookup_map,
	       ध्रुव hb_map_t *backtrack_klass_map = nullptr,
	       ध्रुव hb_map_t *निवेश_klass_map = nullptr,
	       ध्रुव hb_map_t *lookahead_klass_map = nullptr) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    स्वतः snap = c->serializer->snapshot ();
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    प्रति (ध्रुव स्वतः& _ : rule)
    {
      यदि (!_) अनुवर्त;
      स्वतः o_snap = c->serializer->snapshot ();
      स्वतः *o = out->rule.serialize_append (c->serializer);
      यदि (unlikely (!o)) अनुवर्त;

      यदि (!o->serialize_subset (c, _, एतत्,
				lookup_map,
				backtrack_klass_map,
				निवेश_klass_map,
				lookahead_klass_map))
      {
	out->rule.निकालो ();
	c->serializer->revert (o_snap);
      }
    }

    तार्किक ret = तार्किक (out->rule);
    यदि (!ret) c->serializer->revert (snap);

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (rule.sanitize (c, एतत्));
  }

  रक्षित:
  Array16Of<typename Types::ढाँचा OffsetTo<ChainRuleType>>
		rule;			/* Array of ChainRule tables
					 * ordered by preference */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, rule);
};

ढाँचा <typename Types>
संरचना ChainRuleSet : ChainRuleSetOf<Types, ChainRule<Types>> {};

static_assert (परिमाण (ChainRuleSet<MediumTypes>) ==
	       HBUINT16::static_size + Offset24::static_size, "");

ढाँचा <typename Types>
संरचना ChainClassRuleSet : ChainRuleSetOf<Types, ChainRule<SmallTypes>> {};

ढाँचा <typename Types>
संरचना ChainContextFormat1_4
{
  using ChainRuleSet = OT::ChainRuleSet<Types>;

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    प्रत्यागम
    + hb_zip (एतत्+coverage, ruleSet)
    | hb_filter (*glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_map ([&] (ध्रुव ChainRuleSet &_) { प्रत्यागम _.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम सत्य; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ChainContextDependLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };

    + hb_zip (एतत्+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      प्रत्यागम c->parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ध्रुव hb_pair_t<hb_codepoint_t, अचिन्हित> _) { प्रत्यागम hb_pair_t<अचिन्हित, ध्रुव ChainRuleSet&> (_.first, एतत्+ruleSet[_.second]); })
    | hb_apply ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव ChainRuleSet&>& _) { _.second.depend (c, _.first, lookup_context); })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, intersected_glyph},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (एतत्+coverage, hb_range ((अचिन्हित) ruleSet.len))
    | hb_filter ([&] (hb_codepoint_t _) {
      प्रत्यागम c->previous_parent_active_glyphs ().has (_);
    }, hb_first)
    | hb_map ([&](ध्रुव hb_pair_t<hb_codepoint_t, अचिन्हित> _) { प्रत्यागम hb_pair_t<अचिन्हित, ध्रुव ChainRuleSet&> (_.first, एतत्+ruleSet[_.second]); })
    | hb_apply ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव ChainRuleSet&>& _) { _.second.closure (c, _.first, lookup_context); })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव
  {
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_glyph, nullptr},
      ContextFormat::SimpleContext,
      {nullptr, nullptr, nullptr}
    };

    + hb_zip (एतत्+coverage, ruleSet)
    | hb_filter (*c->glyphs, hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव ChainRuleSet &_) { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    (एतत्+coverage).collect_coverage (c->निवेश);

    संरचना ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_glyph},
      {nullptr, nullptr, nullptr}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  {
    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([digest] (ध्रुव ChainRuleSet &_) { _.collect_second_glyphs (digest); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    ध्रुव ChainRuleSet &rule_set = एतत्+ruleSet[(एतत्+coverage).get_coverage (c->glyphs[0])];
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    प्रत्यागम rule_set.would_apply (c, lookup_context);
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
    यदि (index == NOT_COVERED) return_trace (असत्य);

    ध्रुव ChainRuleSet &rule_set = एतत्+ruleSet[index];
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_glyph, match_glyph, match_glyph}},
      {nullptr, nullptr, nullptr}
    };
    return_trace (rule_set.apply (c, lookup_context));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;

    ध्रुव hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (एतत्+coverage, ruleSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->ruleSet, एतत्, lookup_map), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (तार्किक (new_coverage));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && ruleSet.sanitize (c, एतत्));
  }

  रक्षित:
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

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    यदि (!(एतत्+coverage).intersects (glyphs))
      प्रत्यागम असत्य;

    ध्रुव ClassDef &backtrack_class_def = एतत्+backtrackClassDef;
    ध्रुव ClassDef &निवेश_class_def = एतत्+निवेश_Class_Def;
    ध्रुव ClassDef &lookahead_class_def = एतत्+lookaheadClassDef;

    hb_map_t caches[3] = {};
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    hb_set_t retained_coverage_glyphs;
    (एतत्+coverage).intersect_set (*glyphs, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    निवेश_class_def.intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    प्रत्यागम
    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_enumerate
    | hb_map ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव ChainRuleSet &> p)
	      { प्रत्यागम निवेश_class_def.intersects_class (glyphs, p.first) &&
		       coverage_glyph_classes.has (p.first) &&
		       p.second.intersects (glyphs, lookup_context); })
    | hb_any
    ;
  }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम सत्य; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    यदि (!(एतत्+coverage).intersects (c->glyphs))
      प्रत्यागम;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ध्रुव ClassDef &backtrack_class_def = एतत्+backtrackClassDef;
    ध्रुव ClassDef &निवेश_class_def = एतत्+निवेश_Class_Def;
    ध्रुव ClassDef &lookahead_class_def = एतत्+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    ChainContextDependLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (अचिन्हित _)
    { प्रत्यागम निवेश_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव typename Types::ढाँचा OffsetTo<ChainRuleSet>&> _)
                {
                  ध्रुव ChainRuleSet& chainrule_set = एतत्+_.second;
                  chainrule_set.depend (c, _.first, lookup_context);
                })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    यदि (!(एतत्+coverage).intersects (c->glyphs))
      प्रत्यागम;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs)) प्रत्यागम;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ध्रुव ClassDef &backtrack_class_def = एतत्+backtrackClassDef;
    ध्रुव ClassDef &निवेश_class_def = एतत्+निवेश_Class_Def;
    ध्रुव ClassDef &lookahead_class_def = एतत्+lookaheadClassDef;

    hb_map_t caches[3] = {};
    intersected_class_cache_t intersected_cache;
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_class, intersected_class_glyphs},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]},
      &intersected_cache
    };

    + hb_enumerate (ruleSet)
    | hb_filter ([&] (अचिन्हित _)
    { प्रत्यागम निवेश_class_def.intersects_class (&c->parent_active_glyphs (), _); },
		 hb_first)
    | hb_apply ([&] (ध्रुव hb_pair_t<अचिन्हित, ध्रुव typename Types::ढाँचा OffsetTo<ChainRuleSet>&> _)
                {
                  ध्रुव ChainRuleSet& chainrule_set = एतत्+_.second;
                  chainrule_set.closure (c, _.first, lookup_context);
                })
    ;

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव
  {
    यदि (!(एतत्+coverage).intersects (c->glyphs))
      प्रत्यागम;

    ध्रुव ClassDef &backtrack_class_def = एतत्+backtrackClassDef;
    ध्रुव ClassDef &निवेश_class_def = एतत्+निवेश_Class_Def;
    ध्रुव ClassDef &lookahead_class_def = एतत्+lookaheadClassDef;

    hb_map_t caches[3] = {};
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_class, nullptr},
      ContextFormat::ClassBasedContext,
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def},
      {&caches[0], &caches[1], &caches[2]}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_enumerate
    | hb_filter([&] (अचिन्हित klass)
    { प्रत्यागम निवेश_class_def.intersects_class (c->glyphs, klass); }, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ध्रुव ChainRuleSet &_)
    { _.closure_lookups (c, lookup_context); })
    ;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    (एतत्+coverage).collect_coverage (c->निवेश);

    ध्रुव ClassDef &backtrack_class_def = एतत्+backtrackClassDef;
    ध्रुव ClassDef &निवेश_class_def = एतत्+निवेश_Class_Def;
    ध्रुव ClassDef &lookahead_class_def = एतत्+lookaheadClassDef;

    संरचना ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_class},
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def}
    };

    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([&] (ध्रुव ChainRuleSet &_) { _.collect_glyphs (c, lookup_context); })
    ;
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  {
    ध्रुव ClassDef &निवेश_class_def = एतत्+निवेश_Class_Def;
    ध्रुव ClassDef &lookahead_class_def = एतत्+lookaheadClassDef;
    + hb_iter (ruleSet)
    | hb_map (hb_add (एतत्))
    | hb_apply ([digest, &निवेश_class_def, &lookahead_class_def] (ध्रुव ChainRuleSet &_)
		{ _.collect_second_glyphs (digest, &निवेश_class_def, &lookahead_class_def); })
    ;
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    ध्रुव ClassDef &backtrack_class_def = एतत्+backtrackClassDef;
    ध्रुव ClassDef &निवेश_class_def = एतत्+निवेश_Class_Def;
    ध्रुव ClassDef &lookahead_class_def = एतत्+lookaheadClassDef;

    अचिन्हित पूर्णाङ्क index = निवेश_class_def.get_class (c->glyphs[0]);
    ध्रुव ChainRuleSet &rule_set = एतत्+ruleSet[index];
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_class, match_class, match_class}},
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def}
    };
    प्रत्यागम rule_set.would_apply (c, lookup_context);
  }

  ध्रुव Coverage &get_coverage () ध्रुव { प्रत्यागम एतत्+coverage; }

  अचिन्हित cache_cost () ध्रुव
  {
    प्रत्यागम (एतत्+निवेश_Class_Def).cost () + (एतत्+lookaheadClassDef).cost ();
  }
  स्थिर तार्किक cache_func (hb_ot_apply_context_t *c, hb_ot_subtable_cache_op_t op)
  {
    प्रत्यागम context_cache_func (c, op);
  }

  संरचना external_cache_t
  {
    hb_ot_layout_द्विआधारी_cache_t coverage;
    hb_ot_layout_mapping_cache_t निवेश_class;
    hb_ot_layout_mapping_cache_t lookahead_class;
    hb_ot_layout_ruleset_digest_t rule_sets[HB_VAR_ARRAY];
  };
  शून्य *external_cache_create () ध्रुव
  {
    अचिन्हित गिनती = ruleSet.len;
    अचिन्हित size = परिमाण (external_cache_t) -
		    HB_VAR_ARRAY * परिमाण (hb_ot_layout_ruleset_digest_t) +
		    गिनती * परिमाण (hb_ot_layout_ruleset_digest_t);
    external_cache_t *cache = (external_cache_t *) hb_malloc (size);
    यदि (likely (cache))
    {
      cache->coverage.clear ();
      cache->निवेश_class.clear ();
      cache->lookahead_class.clear ();
      प्रति (अचिन्हित i = 0; i < गिनती; i++)
	(एतत्+ruleSet[i]).collect_first_निवेश_classes (&cache->rule_sets[i]);
    }
    प्रत्यागम cache;
  }
  तार्किक apply_cached (hb_ot_apply_context_t *c, शून्य *external_cache) ध्रुव { प्रत्यागम _apply (c, सत्य, external_cache); }
  तार्किक apply (hb_ot_apply_context_t *c, शून्य *external_cache) ध्रुव { प्रत्यागम _apply (c, असत्य, external_cache); }
  तार्किक _apply (hb_ot_apply_context_t *c, तार्किक cached, शून्य *external_cache) ध्रुव
  {
    TRACE_APPLY (एतत्);
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    external_cache_t *cache = (external_cache_t *) external_cache;
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage_द्विआधारी (c->buffer->cur().codepoint, cache ? &cache->coverage : nullptr);
#अन्यथा
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
#endif
    यदि (index == NOT_COVERED) return_trace (असत्य);

    ध्रुव ClassDef &backtrack_class_def = एतत्+backtrackClassDef;
    ध्रुव ClassDef &निवेश_class_def = एतत्+निवेश_Class_Def;
    ध्रुव ClassDef &lookahead_class_def = एतत्+lookaheadClassDef;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    match_class_cache_data_t निवेश_cache_data = {
      &निवेश_class_def,
      cache ? &cache->निवेश_class : nullptr
    };
    match_class_cache_data_t lookahead_cache_data = {
      &lookahead_class_def,
      cache ? &cache->lookahead_class : nullptr
    };
    ध्रुव शून्य *निवेश_match_data = cached
      ? static_cast<ध्रुव शून्य *> (&निवेश_class_def)
      : static_cast<ध्रुव शून्य *> (&निवेश_cache_data);
    ध्रुव शून्य *lookahead_match_data = cached
      ? static_cast<ध्रुव शून्य *> (&lookahead_class_def)
      : static_cast<ध्रुव शून्य *> (&lookahead_cache_data);
    तार्किक backtrack_uses_lookahead_cache =
      !cached && &backtrack_class_def == &lookahead_class_def;
    ध्रुव शून्य *backtrack_match_data = backtrack_uses_lookahead_cache
      ? static_cast<ध्रुव शून्य *> (&lookahead_cache_data)
      : static_cast<ध्रुव शून्य *> (&backtrack_class_def);
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
       निवेश_match_data,
       lookahead_match_data},
      cached ? get_class_cached2_मूल्य : get_class_cache_मूल्य
#अन्यथा
      {{cached && &backtrack_class_def == &lookahead_class_def ? match_class_cached1 : match_class,
        cached ? match_class_cached2 : match_class,
        cached ? match_class_cached1 : match_class}},
      {&backtrack_class_def,
       &निवेश_class_def,
       &lookahead_class_def},
      cached ? get_class_cached2_मूल्य : get_class_मूल्य
#endif
    };

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    index = cached
         ? get_class_cached2 (निवेश_class_def, c->buffer->cur())
         : निवेश_class_def.get_class (c->buffer->cur().codepoint, निवेश_cache_data.cache);
#अन्यथा
    index = cached
         ? get_class_cached2 (निवेश_class_def, c->buffer->cur())
          : निवेश_class_def.get_class (c->buffer->cur().codepoint);
#endif
    ध्रुव ChainRuleSet &rule_set = एतत्+ruleSet[index];
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    ध्रुव hb_ot_layout_ruleset_digest_t *digest = cache && index < ruleSet.len ? &cache->rule_sets[index] : nullptr;
#अन्यथा
    ध्रुव hb_ot_layout_ruleset_digest_t *digest = nullptr;
#endif
    return_trace (rule_set.apply (c, lookup_context, digest));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->format = format;
    out->coverage.serialize_subset (c, coverage, एतत्);

    hb_map_t backtrack_klass_map;
    hb_map_t निवेश_klass_map;
    hb_map_t lookahead_klass_map;

    out->backtrackClassDef.serialize_subset (c, backtrackClassDef, एतत्, &backtrack_klass_map);
    // TODO: subset inputClassDef based on glyphs survived in Coverage subsetting
    out->निवेश_Class_Def.serialize_subset (c, निवेश_Class_Def, एतत्, &निवेश_klass_map);
    out->lookaheadClassDef.serialize_subset (c, lookaheadClassDef, एतत्, &lookahead_klass_map);

    यदि (unlikely (!c->serializer->propagate_error (backtrack_klass_map,
						   निवेश_klass_map,
						   lookahead_klass_map)))
      return_trace (असत्य);

    ध्रुव hb_set_t* glyphset = c->plan->glyphset_gsub ();
    hb_set_t retained_coverage_glyphs;
    (एतत्+coverage).intersect_set (*glyphset, retained_coverage_glyphs);

    hb_set_t coverage_glyph_classes;
    (एतत्+निवेश_Class_Def).intersected_classes (&retained_coverage_glyphs, &coverage_glyph_classes);

    पूर्णाङ्क non_zero_index = -1, index = 0;
    तार्किक ret = सत्य;
    ध्रुव hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;
    स्वतः last_non_zero = c->serializer->snapshot ();
    प्रति (ध्रुव स्वतः& _ : + hb_enumerate (ruleSet)
			 | hb_filter (निवेश_klass_map, hb_first))
    {
      स्वतः *o = out->ruleSet.serialize_append (c->serializer);
      यदि (unlikely (!o))
      {
	ret = असत्य;
	भङ्ग;
      }
      यदि (coverage_glyph_classes.has (_.first) &&
          o->serialize_subset (c, _.second, एतत्,
			       lookup_map,
			       &backtrack_klass_map,
			       &निवेश_klass_map,
			       &lookahead_klass_map))
      {
        last_non_zero = c->serializer->snapshot ();
	non_zero_index = index;
      }

      index++;
    }

    यदि (!ret || non_zero_index == -1) return_trace (असत्य);

    // prune empty trailing ruleSets
    यदि (index > non_zero_index) {
      c->serializer->revert (last_non_zero);
      out->ruleSet.len = non_zero_index + 1;
    }

    return_trace (तार्किक (out->ruleSet));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) &&
		  backtrackClassDef.sanitize (c, एतत्) &&
		  निवेश_Class_Def.sanitize (c, एतत्) &&
		  lookaheadClassDef.sanitize (c, एतत्) &&
		  ruleSet.sanitize (c, एतत्));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
		coverage;		/* Offset to Coverage table--from
					 * beginning of table */
  typename Types::ढाँचा LOffsetTo<ClassDef>
		backtrackClassDef;	/* Offset to glyph ClassDef table
					 * containing backtrack sequence
					 * data--from beginning of table */
  typename Types::ढाँचा LOffsetTo<ClassDef>
		निवेश_Class_Def;		/* Offset to glyph ClassDef
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

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    यदि (!(एतत्+निवेश[0]).intersects (glyphs))
      प्रत्यागम असत्य;

    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, nullptr},
      ContextFormat::CoverageBasedContext,
      {एतत्, एतत्, एतत्}
    };
    प्रत्यागम chain_context_intersects (glyphs,
				     backtrack.len, backtrack.arrayZ,
				     निवेश.len, निवेश.arrayZ + 1,
				     lookahead.len, lookahead.arrayZ,
				     lookup_context);
  }

  तार्किक may_have_non_1to1 () ध्रुव
  { प्रत्यागम सत्य; }

  शून्य depend (hb_depend_context_t *c) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    यदि (!(एतत्+निवेश[0]).intersects (c->glyphs))
      प्रत्यागम;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs))
      प्रत्यागम;
    get_coverage ().intersect_set (*c->glyphs, *cur_active_glyphs);

    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ChainContextDependLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {एतत्, एतत्, एतत्},
      {nullptr, nullptr, nullptr},  /* intersects_cache */
      nullptr                        /* intersected_glyphs_cache */
    };
    chain_context_depend_lookup (c,
				 backtrack.len, (ध्रुव HBUINT16 *) backtrack.arrayZ,
				 निवेश.len, (ध्रुव HBUINT16 *) निवेश.arrayZ + 1,
				 lookahead.len, (ध्रुव HBUINT16 *) lookahead.arrayZ,
				 lookup.len, lookup.arrayZ,
				 एतत्,
				 निवेश[0], lookup_context);

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    यदि (!(एतत्+निवेश[0]).intersects (c->glyphs))
      प्रत्यागम;

    hb_set_t* cur_active_glyphs = c->धकेलो_cur_active_glyphs ();
    यदि (unlikely (!cur_active_glyphs))
      प्रत्यागम;
    get_coverage ().intersect_set (c->previous_parent_active_glyphs (),
				   *cur_active_glyphs);

    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    संरचना ChainContextClosureLookupContext lookup_context = {
      {intersects_coverage, intersected_coverage_glyphs},
      ContextFormat::CoverageBasedContext,
      {एतत्, एतत्, एतत्}
    };
    chain_context_closure_lookup (c,
				  backtrack.len, backtrack.arrayZ,
				  निवेश.len, निवेश.arrayZ + 1,
				  lookahead.len, lookahead.arrayZ,
				  lookup.len, lookup.arrayZ,
				  0, lookup_context);

    c->निकालो_cur_done_glyphs ();
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) ध्रुव
  {
    यदि (!intersects (c->glyphs))
      प्रत्यागम;

    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    recurse_lookups (c, lookup.len, lookup.arrayZ);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    (एतत्+निवेश[0]).collect_coverage (c->निवेश);

    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);

    संरचना ChainContextCollectGlyphsLookupContext lookup_context = {
      {collect_coverage},
      {एतत्, एतत्, एतत्}
    };
    chain_context_collect_glyphs_lookup (c,
					 backtrack.len, backtrack.arrayZ,
					 निवेश.len, निवेश.arrayZ + 1,
					 lookahead.len, lookahead.arrayZ,
					 lookup.len, lookup.arrayZ,
					 lookup_context);
  }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    यदि (निवेश.len > 1)
      (एतत्+निवेश[1]).collect_coverage (digest);
    अन्यथा यदि (lookahead.len)
      (एतत्+lookahead[0]).collect_coverage (digest);
    अन्यथा
      *digest = hb_set_digest_t::full ();
  }

  तार्किक would_apply (hb_would_apply_context_t *c) ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {एतत्, एतत्, एतत्}
    };
    प्रत्यागम chain_context_would_apply_lookup (c,
					     backtrack.len, backtrack.arrayZ,
					     निवेश.len, निवेश.arrayZ + 1,
					     lookahead.len, lookahead.arrayZ,
					     lookup.len, lookup.arrayZ, lookup_context);
  }

  ध्रुव Coverage &get_coverage () ध्रुव
  {
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    प्रत्यागम एतत्+निवेश[0];
  }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);

    अचिन्हित पूर्णाङ्क index = (एतत्+निवेश[0]).get_coverage (c->buffer->cur().codepoint);
    यदि (index == NOT_COVERED) return_trace (असत्य);

    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    संरचना ChainContextApplyLookupContext lookup_context = {
      {{match_coverage, match_coverage, match_coverage}},
      {एतत्, एतत्, एतत्}
    };
    return_trace (chain_context_apply_lookup (c,
					      backtrack.len, backtrack.arrayZ,
					      निवेश.len, निवेश.arrayZ + 1,
					      lookahead.len, lookahead.arrayZ,
					      lookup.len, lookup.arrayZ, lookup_context));
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_iterator (Iterator))>
  तार्किक serialize_coverage_offsets (hb_subset_context_t *c, Iterator it, ध्रुव शून्य* base) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->serializer->start_embed<Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>> ();
    यदि (unlikely (!out->serialize (c->serializer, 0u))) return_trace (असत्य);

    प्रति (स्वतः& offset : it) {
      स्वतः *o = out->serialize_append (c->serializer);
      यदि (unlikely (!o) || !o->serialize_subset (c, offset, base))
        return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    यदि (unlikely (!c->serializer->embed (एतत्->format))) return_trace (असत्य);

    यदि (!serialize_coverage_offsets (c, backtrack.iter (), एतत्))
      return_trace (असत्य);

    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    यदि (!serialize_coverage_offsets (c, निवेश.iter (), एतत्))
      return_trace (असत्य);

    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    यदि (!serialize_coverage_offsets (c, lookahead.iter (), एतत्))
      return_trace (असत्य);

    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    ध्रुव hb_map_t *lookup_map = c->table_tag == HB_OT_TAG_GSUB ? &c->plan->gsub_lookups : &c->plan->gpos_lookups;

    HBUINT16 *lookup_गिनती_2 = c->serializer->copy<HBUINT16> (lookup.len);
    यदि (!lookup_गिनती_2) return_trace (असत्य);

    अचिन्हित गिनती = serialize_lookuprecord_array (c->serializer, lookup.as_array (), lookup_map);
    return_trace (c->serializer->check_assign (*lookup_गिनती_2, गिनती, HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!backtrack.sanitize (c, एतत्))) return_trace (असत्य);
    hb_barrier ();
    ध्रुव स्वतः &निवेश = StructAfter<decltype (निवेश_X)> (backtrack);
    यदि (unlikely (!निवेश.sanitize (c, एतत्))) return_trace (असत्य);
    hb_barrier ();
    यदि (unlikely (!निवेश.len)) return_trace (असत्य); /* To be consistent with Context. */
    ध्रुव स्वतः &lookahead = StructAfter<decltype (lookaheadX)> (निवेश);
    यदि (unlikely (!lookahead.sanitize (c, एतत्))) return_trace (असत्य);
    hb_barrier ();
    ध्रुव स्वतः &lookup = StructAfter<decltype (lookupX)> (lookahead);
    return_trace (likely (lookup.sanitize (c)));
  }

  रक्षित:
  HBUINT16	format;			/* Format identifier--format = 3 */
  Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>
		backtrack;		/* Array of coverage tables
					 * in backtracking sequence, in  glyph
					 * sequence order */
  Array16Of<typename Types::ढाँचा OffsetTo<Coverage>>
		निवेश_X		;	/* Array of coverage
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
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
#ifndef HB_NO_BEYOND_64K
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    पक्ष 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
#endif
    स्वभाव:return_trace (c->default_return_मूल्य ());
    }
  }

  रक्षित:
  सङ्घ {
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
  अचिन्हित पूर्णाङ्क get_type () ध्रुव { प्रत्यागम extensionLookupType; }

  ढाँचा <typename X>
  ध्रुव X& get_subtable () ध्रुव
  { प्रत्यागम एतत् + reinterpret_cast<ध्रुव Offset32To<typename T::SubTable> &> (extensionOffset); }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    यदि (unlikely (!c->may_dispatch (एतत्, एतत्))) प्रत्यागम c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (एतत्, format);
    return_trace (get_subtable<typename T::SubTable> ().dispatch (c, get_type (), std::forward<Ts> (ds)...));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  { dispatch (c); }

  /* This is called from may_dispatch() above with hb_sanitize_context_t. */
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  extensionLookupType != T::SubTable::Extension);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    out->format = format;
    out->extensionLookupType = extensionLookupType;

    ध्रुव स्वतः& src_offset =
        reinterpret_cast<ध्रुव Offset32To<typename T::SubTable> &> (extensionOffset);
    स्वतः& dest_offset =
        reinterpret_cast<Offset32To<typename T::SubTable> &> (out->extensionOffset);

    return_trace (dest_offset.serialize_subset (c, src_offset, एतत्, get_type ()));
  }

  रक्षित:
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
  अचिन्हित पूर्णाङ्क get_type () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.get_type ();
    स्वभाव:प्रत्यागम 0;
    }
  }
  ढाँचा <typename X>
  ध्रुव X& get_subtable () ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.ढाँचा get_subtable<typename T::SubTable> ();
    स्वभाव:प्रत्यागम Null (typename T::SubTable);
    }
  }

  // Specialization of dispatch for subset. dispatch() normally just
  // dispatches to the sub table this points too, but for subset
  // we need to run subset on this subtable too.
  ढाँचा <typename ...Ts>
  typename hb_subset_context_t::return_t dispatch (hb_subset_context_t *c, Ts&&... ds) ध्रुव
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); प्रत्यागम u.format1.subset (c);
    स्वभाव: प्रत्यागम c->default_return_मूल्य ();
    }
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ध्रुव
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) प्रत्यागम c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.dispatch (c, std::forward<Ts> (ds)...));
    स्वभाव:return_trace (c->default_return_मूल्य ());
    }
  }

  रक्षित:
  सङ्घ {
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
  स्थिर hb_ot_layout_lookup_accelerator_t *create (ध्रुव TLookup &lookup)
  {
    अचिन्हित गिनती = lookup.get_subtable_गिनती ();

    अचिन्हित size = परिमाण (hb_ot_layout_lookup_accelerator_t) -
		    HB_VAR_ARRAY * परिमाण (hb_accelerate_subtables_context_t::hb_applicable_t) +
		    गिनती * परिमाण (hb_accelerate_subtables_context_t::hb_applicable_t);

    /* The following is a calloc because when we are collecting subtables,
     * some of them might be invalid and hence not collect; as a result,
     * we might not fill in all the count entries of the subtables array.
     * Zeroing it allows the set digest to gatekeep it without having to
     * initialize it further. */
    स्वतः *thiz = (hb_ot_layout_lookup_accelerator_t *) hb_calloc (1, size);
    यदि (unlikely (!thiz))
      प्रत्यागम nullptr;

    hb_accelerate_subtables_context_t c_accelerate_subtables (thiz->subtables);
    lookup.dispatch (&c_accelerate_subtables);

    /* Invalid subtables do not collect and leave zeroed tail entries
     * (see the calloc note above); trim them so the apply paths never
     * see a null apply func. */
    यावत् (गिनती && !thiz->subtables[गिनती - 1].apply_func)
      गिनती--;

    thiz->digest.init ();
    प्रति (स्वतः& subtable : hb_iter (thiz->subtables, गिनती))
      thiz->digest.union_ (subtable.digest);
    thiz->digest_second = c_accelerate_subtables.digest_second;

    thiz->गिनती = गिनती;

#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    thiz->subtable_cache_user_idx = c_accelerate_subtables.subtable_cache_user_idx;

    प्रति (अचिन्हित i = 0; i < गिनती; i++)
      यदि (i != thiz->subtable_cache_user_idx)
       thiz->subtables[i].apply_cached_func = thiz->subtables[i].apply_func;
#endif

    प्रत्यागम thiz;
  }

  शून्य fini ()
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    प्रति (अचिन्हित i = 0; i < गिनती; i++)
      hb_free (subtables[i].external_cache);
#endif
  }

  तार्किक may_have (hb_codepoint_t g) ध्रुव
  { प्रत्यागम digest.may_have (g); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  तार्किक apply (hb_ot_apply_context_t *c, तार्किक use_cache) ध्रुव
  {
    यदि (गिनती == 1)
    {
      /* The accelerator digest is the union of the subtable digests, so
       * for a single subtable it equals that subtable's digest, which the
       * caller has already tested; skip the redundant retest. Zeroed
       * invalid entries were trimmed at create time, so the subtable is
       * always valid here. */
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
      यदि (use_cache)
	प्रत्यागम subtables[0].apply_cached_no_digest (c);
#endif
      प्रत्यागम subtables[0].apply_no_digest (c);
    }
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    यदि (use_cache)
    {
      प्रत्यागम
      + hb_iter (hb_iter (subtables, गिनती))
      | hb_map ([&c] (ध्रुव hb_accelerate_subtables_context_t::hb_applicable_t &_) { प्रत्यागम _.apply_cached (c); })
      | hb_any
      ;
    }
    अन्यथा
#endif
    {
      प्रत्यागम
      + hb_iter (hb_iter (subtables, गिनती))
      | hb_map ([&c] (ध्रुव hb_accelerate_subtables_context_t::hb_applicable_t &_) { प्रत्यागम _.apply (c); })
      | hb_any
      ;
    }
    प्रत्यागम असत्य;
  }

  तार्किक cache_enter (hb_ot_apply_context_t *c) ध्रुव
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    प्रत्यागम subtable_cache_user_idx != (अचिन्हित) -1 &&
	   subtables[subtable_cache_user_idx].cache_enter (c);
#अन्यथा
    प्रत्यागम असत्य;
#endif
  }
  शून्य cache_leave (hb_ot_apply_context_t *c) ध्रुव
  {
#ifndef HB_NO_OT_LAYOUT_LOOKUP_CACHE
    subtables[subtable_cache_user_idx].cache_leave (c);
#endif
  }


  hb_set_digest_t digest;
  hb_set_digest_t digest_second;
  गुप्त:
  अचिन्हित गिनती = 0; /* Number of subtables in the array. */
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

  माप_प्रकार get_size () ध्रुव
  {
    यदि (version.major != 1) प्रत्यागम version.static_size;

    प्रत्यागम min_size +
	   (version.to_int () >= 0x00010001u ? featureVars.static_size : 0) +
#ifndef HB_NO_BEYOND_64K
	   (version.to_int () >= 0x00010002u ? 3 * Offset32::static_size : 0)
#अन्यथा
	   0
#endif
	   ;
  }

  ध्रुव ScriptList &get_script_list () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010002u && scriptList2) प्रत्यागम एतत् + scriptList2;
#endif
    प्रत्यागम एतत् + scriptList;
  }
  ध्रुव FeatureList &get_feature_list () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010002u && featureList2) प्रत्यागम एतत् + featureList2;
#endif
    प्रत्यागम एतत् + featureList;
  }
  ढाँचा <typename TLookup>
  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!version.sanitize (c) ||
		  version.major != 1 ||
		  !c->check_range (एतत्, get_size ())))
      return_trace (असत्य);
    hb_barrier ();

    तार्किक script_list_ok = scriptList.sanitize (c, एतत्);
    तार्किक feature_list_ok = featureList.sanitize (c, एतत्);
    तार्किक lookup_list_ok = typed_lookup_list_offset<TLookup> ().sanitize (c, एतत्);

#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010002u)
    {
      यदि (scriptList2) script_list_ok = scriptList2.sanitize (c, एतत्);
      यदि (featureList2) feature_list_ok = featureList2.sanitize (c, एतत्);
      यदि (lookupList2) lookup_list_ok = typed_lookup_list2_offset<TLookup> ().sanitize (c, एतत्);
    }
#endif

    यदि (unlikely (!(script_list_ok && feature_list_ok && lookup_list_ok)))
      return_trace (असत्य);

#ifndef HB_NO_VAR
    यदि (unlikely (!(version.to_int () < 0x00010001u ||
		    featureVars.sanitize (c, एतत्, &get_feature_list ()))))
      return_trace (असत्य);
#endif

    return_trace (सत्य);
  }

  ढाँचा <typename TLookup>
  तार्किक subset (hb_subset_layout_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    यदि (version.major != 1) return_trace (असत्य);

    स्वतः *out = c->subset_context->serializer->start_embed (एतत्);
    यदि (unlikely (!c->subset_context->serializer->extend_min (out))) return_trace (असत्य);

    out->version = version;

#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010002u)
    {
      यदि (unlikely (!c->subset_context->serializer->extend_size (out, get_size ())))
        return_trace (असत्य);

      out->scriptList = 0;
      out->featureList = 0;
      out->lookupList = 0;

      यदि (lookupList2)
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list2_offset<TLookup> (),
				 एतत्, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->lookupList2,
				 subset_lookup_list_offset<TLookup> (),
				 एतत्, c);

      यदि (featureList2)
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list2_offset (),
				 एतत्, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->featureList2,
				 subset_feature_list_offset (),
				 एतत्, c);

      यदि (scriptList2)
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList2, एतत्, c);
      अन्यथा
	serialize_subset_offset (c->subset_context, out->scriptList2, scriptList, एतत्, c);
    }
    अन्यथा
#endif
    {
      out->subset_lookup_list_offset<TLookup> ()
	  .serialize_subset (c->subset_context,
			     subset_lookup_list_offset<TLookup> (),
			     एतत्,
			     c);

      out->subset_feature_list_offset ()
	  .serialize_subset (c->subset_context,
			     subset_feature_list_offset (),
			     एतत्,
			     c);

      out->scriptList.serialize_subset (c->subset_context,
					scriptList,
					एतत्,
					c);
    }

#ifndef HB_NO_VAR
    यदि (version.to_int () >= 0x00010001u)
    {
      स्वतः snapshot = c->subset_context->serializer->snapshot ();
      यदि (version.to_int () < 0x00010002u &&
	  !c->subset_context->serializer->extend_min (&out->featureVars))
        return_trace (असत्य);

      // if all axes are pinned all feature vars are dropped.
      तार्किक ret = !c->subset_context->plan->all_axes_pinned
                 && out->featureVars.serialize_subset (c->subset_context, featureVars, एतत्, c);
      यदि (!ret)
      {
        out->featureVars = 0;
	यदि (version.to_int () < 0x00010002u)
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

  अचिन्हित पूर्णाङ्क get_lookup_गिनती () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010002u && lookupList2)
      प्रत्यागम (एतत् + lookupList2).len;
#endif
    प्रत्यागम (एतत् + lookupList).len;
  }
  ध्रुव Lookup& get_lookup (अचिन्हित पूर्णाङ्क i) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010002u && lookupList2)
      प्रत्यागम (एतत् + lookupList2)[i];
#endif
    प्रत्यागम (एतत् + lookupList)[i];
  }
  ध्रुव FeatureVariations &get_feature_variations () ध्रुव
  {
    प्रत्यागम (version.to_int () >= 0x00010001u && hb_barrier ()) ? एतत्+featureVars : Null (FeatureVariations);
  }

  तार्किक has_data () ध्रुव { प्रत्यागम version.to_int (); }
  अचिन्हित पूर्णाङ्क get_script_गिनती () ध्रुव
  { प्रत्यागम get_script_list ().len; }
  ध्रुव Tag& get_script_tag (अचिन्हित पूर्णाङ्क i) ध्रुव
  { प्रत्यागम get_script_list ().get_tag (i); }
  अचिन्हित पूर्णाङ्क get_script_tags (अचिन्हित पूर्णाङ्क start_offset,
				अचिन्हित पूर्णाङ्क *script_गिनती /* IN/OUT */,
				hb_tag_t     *script_tags /* OUT */) ध्रुव
  { प्रत्यागम get_script_list ().get_tags (start_offset, script_गिनती, script_tags); }
  ध्रुव Script& get_script (अचिन्हित पूर्णाङ्क i) ध्रुव
  { प्रत्यागम get_script_list ()[i]; }
  तार्किक find_script_index (hb_tag_t tag, अचिन्हित पूर्णाङ्क *index) ध्रुव
  { प्रत्यागम get_script_list ().find_index (tag, index); }

  अचिन्हित पूर्णाङ्क get_feature_गिनती () ध्रुव
  { प्रत्यागम get_feature_list ().len; }
  hb_tag_t get_feature_tag (अचिन्हित पूर्णाङ्क i) ध्रुव
  { प्रत्यागम i == Index::NOT_FOUND_INDEX ? HB_TAG_NONE : get_feature_list ().get_tag (i); }
  अचिन्हित पूर्णाङ्क get_feature_tags (अचिन्हित पूर्णाङ्क start_offset,
				 अचिन्हित पूर्णाङ्क *feature_गिनती_2 /* IN/OUT */,
				 hb_tag_t     *feature_tags /* OUT */) ध्रुव
  { प्रत्यागम get_feature_list ().get_tags (start_offset, feature_गिनती_2, feature_tags); }
  ध्रुव Feature& get_feature (अचिन्हित पूर्णाङ्क i) ध्रुव
  { प्रत्यागम get_feature_list ()[i]; }
  तार्किक find_feature_index (hb_tag_t tag, अचिन्हित पूर्णाङ्क *index) ध्रुव
  { प्रत्यागम get_feature_list ().find_index (tag, index); }

  तार्किक find_variations_index (ध्रुव पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क अंक_coords,
			      अचिन्हित पूर्णाङ्क *index,
			      ItemVarStoreInstancer *instancer) ध्रुव
  {
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    प्रत्यागम असत्य;
#endif
    प्रत्यागम get_feature_variations ().find_index (coords, अंक_coords, index, instancer);
  }
  तार्किक get_feature_variations_state (ध्रुव पूर्णाङ्क *coords,
				     अचिन्हित पूर्णाङ्क अंक_coords,
				     अचिन्हित पूर्णाङ्क *index,
				     ItemVarStoreInstancer *instancer,
				     hb_vector_t<अचिन्हित> *lookup_conditions) ध्रुव
  {
    lookup_conditions->reset ();
#ifdef HB_NO_VAR
    *index = FeatureVariations::NOT_FOUND_INDEX;
    प्रत्यागम सत्य;
#अन्यथा
    find_variations_index (coords, अंक_coords, index, instancer);
    प्रत्यागम get_feature_variations ().collect_lookup_conditions (
	coords,
	अंक_coords,
	instancer,
	lookup_conditions);
#endif
  }
  तार्किक get_lookup_variations (अचिन्हित feature_index,
                              अचिन्हित variations_index,
                              hb_sorted_array_t<ध्रुव अचिन्हित> conditions,
                              hb_set_t *lookup_indexes) ध्रुव
  {
#ifdef HB_NO_VAR
    प्रत्यागम असत्य;
#अन्यथा
    प्रत्यागम get_feature_variations ().get_lookup_variations (
	feature_index, get_feature_variation (feature_index, variations_index),
	conditions, lookup_indexes);
#endif
  }
  ध्रुव Feature& get_feature_variation (अचिन्हित पूर्णाङ्क feature_index,
					अचिन्हित पूर्णाङ्क variations_index) ध्रुव
  {
#ifndef HB_NO_VAR
    यदि (FeatureVariations::NOT_FOUND_INDEX != variations_index &&
	version.to_int () >= 0x00010001u)
    {
      ध्रुव Feature *feature = get_feature_variations ().find_substitute (variations_index,
									  feature_index);
      यदि (feature)
	प्रत्यागम *feature;
    }
#endif
    प्रत्यागम get_feature (feature_index);
  }

  शून्य feature_variation_collect_lookups (ध्रुव hb_set_t *feature_indexes,
					  ध्रुव hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
					  hb_set_t       *lookup_indexes /* OUT */) ध्रुव
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_lookups (feature_indexes, feature_record_cond_idx_map, lookup_indexes);
#endif
  }

  शून्य collect_feature_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
#ifndef HB_NO_VAR
    get_feature_variations ().collect_variation_indices (c);
#endif
  }

#ifndef HB_NO_VAR
  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ध्रुव
  { get_feature_variations ().collect_feature_substitutes_with_variations (c); }
#endif

  ढाँचा <typename TLookup>
  शून्य closure_lookups (hb_face_t      *face,
			ध्रुव hb_set_t *glyphs,
			hb_set_t       *lookup_indexes /* IN/OUT */) ध्रुव
  {
    hb_set_t visited_lookups, inactive_lookups;
    hb_closure_lookups_context_t c (face, glyphs, &visited_lookups, &inactive_lookups);

    c.set_recurse_func (TLookup::ढाँचा dispatch_recurse_func<hb_closure_lookups_context_t>);

    प्रति (अचिन्हित lookup_index : *lookup_indexes)
      reinterpret_cast<ध्रुव TLookup &> (get_lookup (lookup_index)).closure_lookups (&c, lookup_index);

    hb_set_union (lookup_indexes, &visited_lookups);
    hb_set_subtract (lookup_indexes, &inactive_lookups);
  }

  शून्य prune_langsys (ध्रुव hb_map_t *duplicate_feature_map,
                      ध्रुव hb_set_t *layout_scripts,
                      hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map,
                      hb_set_t       *new_feature_indexes /* OUT */) ध्रुव
  {
    hb_prune_langsys_context_t c (एतत्, script_langsys_map, duplicate_feature_map, new_feature_indexes);

    अचिन्हित गिनती = get_script_गिनती ();
    प्रति (अचिन्हित script_index = 0; script_index < गिनती; script_index++)
    {
      ध्रुव Tag& tag = get_script_tag (script_index);
      यदि (!layout_scripts->has (tag)) अनुवर्त;
      ध्रुव Script& s = get_script (script_index);
      s.prune_langsys (&c, script_index);
    }
  }

  शून्य prune_features (ध्रुव hb_map_t *lookup_indices, /* IN */
		       ध्रुव hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map, /* IN */
		       ध्रुव hb_hashmap_t<अचिन्हित, ध्रुव Feature*> *feature_substitutes_map, /* IN */
		       hb_set_t       *feature_indices /* IN/OUT */) ध्रुव
  {
#ifndef HB_NO_VAR
    // This is the set of feature indices which have alternate versions defined
    // if the FeatureVariation's table and the alternate version(s) intersect the
    // set of lookup indices.
    hb_set_t alternate_feature_indices;
    get_feature_variations ().closure_features (lookup_indices, feature_record_cond_idx_map, &alternate_feature_indices);
    यदि (unlikely (alternate_feature_indices.in_error()))
    {
      feature_indices->err ();
      प्रत्यागम;
    }
#endif

    प्रति (अचिन्हित i : hb_iter (feature_indices))
    {
      hb_tag_t tag =  get_feature_tag (i);
      यदि (tag == HB_TAG ('p', 'r', 'e', 'f'))
        // Note: Never ever drop feature 'pref', even if it's empty.
        // HarfBuzz chooses shaper for Khmer based on presence of this
        // feature.	See thread at:
	// http://lists.freedesktop.org/archives/harfbuzz/2012-November/002660.html
        अनुवर्त;


      ध्रुव Feature *f = &(get_feature (i));
      ध्रुव Feature** p = nullptr;
      यदि (feature_substitutes_map->has (i, &p))
        f = *p;

      यदि (!f->featureParams.is_null () &&
          tag == HB_TAG ('s', 'i', 'z', 'e'))
        अनुवर्त;

      यदि (!f->intersects_lookup_indexes (lookup_indices)
#ifndef HB_NO_VAR
          && !alternate_feature_indices.has (i)
#endif
	  )
	feature_indices->del (i);
    }
  }

  शून्य collect_name_ids (ध्रुव hb_map_t *feature_index_map,
                         hb_set_t *nameids_to_retain, /* OUT */
                         ध्रुव hb_hashmap_t<अचिन्हित, ध्रुव Feature*> *feature_substitutes_map,
                         ध्रुव hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map HB_UNUSED,
                         ध्रुव hb_set_t *catch_all_features,
                         तार्किक all_axes_pinned HB_UNUSED) ध्रुव
  {
    अचिन्हित गिनती = get_feature_गिनती ();
    प्रति (अचिन्हित i = 0 ; i < गिनती; i++)
    {
      यदि (!feature_index_map->has (i)) अनुवर्त;
      hb_tag_t tag = get_feature_tag (i);
      ध्रुव Feature *feature = &get_feature (i);
      ध्रुव Feature **substitute;
      यदि (feature_substitutes_map->has (i, &substitute))
      {
        feature = *substitute;
        यदि (catch_all_features->has (i))
          get_feature (i).collect_name_ids (tag, nameids_to_retain);
      }
      feature->collect_name_ids (tag, nameids_to_retain);
    }
#ifndef HB_NO_VAR
    यदि (!all_axes_pinned)
      get_feature_variations ().collect_name_ids (feature_index_map, feature_record_cond_idx_map,
                                                  [एतत्] (अचिन्हित i) { प्रत्यागम get_feature_tag (i); },
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
      एतत्->table = sc.reference_table<T> (face);

      यदि (unlikely (एतत्->table->is_blocklisted (एतत्->table.get_blob (), face)))
      {
	hb_blob_destroy (एतत्->table.get_blob ());
	एतत्->table = hb_blob_get_empty ();
      }

      एतत्->lookup_गिनती = table->get_lookup_गिनती ();

      एतत्->accels = (hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *) hb_calloc (एतत्->lookup_गिनती, परिमाण (*accels));
      यदि (unlikely (!एतत्->accels))
      {
	एतत्->lookup_गिनती = 0;
	एतत्->table.destroy ();
	एतत्->table = hb_blob_get_empty ();
      }
    }
    ~accelerator_t ()
    {
      प्रति (अचिन्हित पूर्णाङ्क i = 0; i < एतत्->lookup_गिनती; i++)
      {
	स्वतः *accel = एतत्->accels[i].get_relaxed ();
	यदि (accel)
	  accel->fini ();
	hb_free (accel);
      }
      hb_free (एतत्->accels);
      एतत्->table.destroy ();
    }

    hb_blob_t *get_blob () ध्रुव { प्रत्यागम table.get_blob (); }

    hb_ot_layout_lookup_accelerator_t *get_accel (अचिन्हित lookup_index) ध्रुव
    {
      यदि (unlikely (lookup_index >= lookup_गिनती)) प्रत्यागम nullptr;

    retry:
      स्वतः *accel = accels[lookup_index].get_acquire ();
      यदि (unlikely (!accel))
      {
	accel = hb_ot_layout_lookup_accelerator_t::create (table->get_lookup (lookup_index));
	यदि (unlikely (!accel))
	  प्रत्यागम nullptr;

	यदि (unlikely (!accels[lookup_index].cmpexch (nullptr, accel)))
	{
	  accel->fini ();
	  hb_free (accel);
	  गच्छ retry;
	}
      }

      प्रत्यागम accel;
    }

    hb_blob_ptr_t<T> table;
    अचिन्हित पूर्णाङ्क lookup_गिनती;
    hb_atomic_t<hb_ot_layout_lookup_accelerator_t *> *accels;
  };

  रक्षित:
  ढाँचा <typename TLookup>
  ध्रुव Offset16To<TLookupList<TLookup>>& typed_lookup_list_offset () ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव Offset16To<TLookupList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  ढाँचा <typename TLookup>
  ध्रुव Offset32To<TLookupList2<TLookup>>& typed_lookup_list2_offset () ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव Offset32To<TLookupList2<TLookup>>&> (lookupList2); }
#endif

  ढाँचा <typename TLookup>
  ध्रुव Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset () ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }
  ढाँचा <typename TLookup>
  Offset16To<TLookupOffsetList<TLookup>>& subset_lookup_list_offset ()
  { प्रत्यागम reinterpret_cast<Offset16To<TLookupOffsetList<TLookup>>&> (lookupList); }

#ifndef HB_NO_BEYOND_64K
  ढाँचा <typename TLookup>
  ध्रुव Offset32To<TLookupOffsetList2<TLookup>>& subset_lookup_list2_offset () ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव Offset32To<TLookupOffsetList2<TLookup>>&> (lookupList2); }
#endif

  ध्रुव Offset16To<RecordListOfFeature>& subset_feature_list_offset () ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव Offset16To<RecordListOfFeature>&> (featureList); }
  Offset16To<RecordListOfFeature>& subset_feature_list_offset ()
  { प्रत्यागम reinterpret_cast<Offset16To<RecordListOfFeature>&> (featureList); }

#ifndef HB_NO_BEYOND_64K
  ध्रुव Offset32To<RecordListOfFeature>& subset_feature_list2_offset () ध्रुव
  { प्रत्यागम reinterpret_cast<ध्रुव Offset32To<RecordListOfFeature>&> (featureList2); }
#endif

  ध्रुव शून्य* get_lookup_list_field_offset () ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    यदि (version.to_int () >= 0x00010002u && lookupList2) प्रत्यागम &lookupList2;
#endif
    प्रत्यागम &lookupList;
  }

  ढाँचा <typename OffsetOut, typename OffsetIn, typename Base, typename ...Ts>
  स्थिर तार्किक serialize_subset_offset (hb_subset_context_t *c,
				       OffsetOut& out,
				       ध्रुव OffsetIn& in,
				       ध्रुव Base *base,
				       Ts&&... ds)
  {
    out = 0;
    यदि (in.is_null ()) प्रत्यागम असत्य;

    स्वतः *s = c->serializer;
    s->push ();
    तार्किक ret = c->dispatch (base+in, std::forward<Ts> (ds)...);
    यदि (ret) s->add_link (out, s->निकालो_pack ());
    अन्यथा s->निकालो_discard ();
    प्रत्यागम ret;
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
